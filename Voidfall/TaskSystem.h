#pragma once

#include <iostream>
#include <thread>
#include <atomic>
#include <functional>
#include <algorithm>
#include <condition_variable>
#include <cassert>
//#include "debug.h"


//Find out our count for system cores(including virtual ones) and suggested number of threads
const uint64_t MIN_THREADS = 1; // should not be changed, hack-y way to make uint64_t behave with
                                // GCC and MSVC.
const uint64_t SYSTEM_NUMCORES = std::thread::hardware_concurrency() - 1;
const uint64_t SYSTEM_NUM_HW_THREADS = std::max(MIN_THREADS, SYSTEM_NUMCORES);

const uint64_t TASKLINEBUFFERSIZE=1024;

using BufferFunctionType = std::function<void()>;

#ifdef _WIN32
#include <Windows.h>
#include <sstream>
#endif

#ifdef __linux__
#include <pthread.h>
#endif

#ifdef _WIN32
static uint64_t GetCurrentThreadProcessorNumber() {
	return (uint64_t)GetCurrentProcessorNumber();
}
#endif

#include "CicularFIFO.h"

using namespace std;

//A TaskID is granted when executing a task, it allows you to evaluate when a task is done
struct TaskID {
	uint64_t header;
	uint64_t tasknumber;
};
void LockThreadToCore(std::thread::native_handle_type a_handle, uint64_t a_core) {
#ifdef _WIN32 //Windows specific thread affinity
	HANDLE handle = (HANDLE)a_handle; // m_thread.native_handle();
	// Put each thread on to dedicated core
	DWORD_PTR affinityMask = 1ull << a_core;
	DWORD_PTR affinity_result = SetThreadAffinityMask(handle, affinityMask);
	//assert(affinity_result > 0);
	//BOOL priority_result = SetThreadPriority(handle, THREAD_PRIORITY_HIGHEST);
	//assert(priority_result != 0);
	std::wstringstream wss;
	wss << "Taskline: " << a_core;
	HRESULT hr = SetThreadDescription(handle, wss.str().c_str());
	//assert(SUCCEEDED(hr));
	//::SetThreadIdealProcessor(handle, m_tlid);
#endif //_WIN32
#ifdef __linux__
	////UNTESTED
	cpu_set_t cpuset;
	CPU_ZERO(&cpuset);
	CPU_SET(a_core, &cpuset);
	//int rc = pthread_setaffinity_np(m_thread.native_handle(), sizeof(cpu_set_t), &cpuset);
	int rc = pthread_setaffinity_np(a_handle, sizeof(cpu_set_t), &cpuset);
	assert(rc != 0);
#endif
}

class Taskline{
public:
	Taskline() : m_senttaskscount(0), m_tlid(0), m_finishedtaskscount(0){}

	void Engage(uint64_t a_tlid, bool a_setaffinity = false){
		m_tlid = a_tlid;
		m_thread = std::thread(&Taskline::__threadexecute, this);
		if (a_setaffinity && (m_tlid < SYSTEM_NUM_HW_THREADS)){//Any subsequent threads above HW threads are not assigned to a core directly
			LockThreadToCore(m_thread.native_handle(),m_tlid);
		}
	}

	//Send a task through the task line, returns false if full
	inline bool Dispatch(BufferFunctionType _task){
		m_wakecondition.notify_one();
		if(!m_taskbuffer.Push(_task)) return false;
		++m_senttaskscount;
		return true;
	}

	//Tell the thread to die when done
	inline void TerminateAndJoin() {
		m_wakecondition.notify_one();
		while(m_taskbuffer.Push(NULL)==false);
		m_thread.join();
	}
#ifdef _DEBUG
	Show thread info runtime
	void PushThreadDataToConsole() {
		std::this_thread::sleep_for(200ms);
		Push([=](){
			g_debugmutex.lock();
#   ifdef __WIN32
			cout << "Taskline ID:" << this->m_tlid << " NID:" << std::this_thread::get_id() << " running on core " << GetCurrentThreadProcessorNumber() << endl;
#   endif
#   ifdef __linux__
			cout << "Taskline ID:" << this->m_tlid << " NID:" << std::this_thread::get_id() << " running on core " << getcpu() << endl;
#   endif
	        return (uint64_t)getcpu(cpu, node, tcache);
			g_debugmutex.unlock();
			std::this_thread::sleep_for(200ms);
		});
	}
#endif
	//Is the taskline done with all tasks
	inline bool IsDone() const {return m_finishedtaskscount.load(std::memory_order_acquire) == m_senttaskscount;}
	//Is the taskline done with the certain taskid
	inline bool IsDoneWith(uint64_t a_taskid) const {return m_finishedtaskscount.load(std::memory_order_acquire) >= a_taskid;}
	inline uint64_t SentTasksCount() const {return m_senttaskscount;}
	inline uint64_t FinishedTasksCount() const { return m_finishedtaskscount.load(std::memory_order_acquire);}
protected:
	//Execution scope call!
	void __threadexecute(){
		BufferFunctionType task;
		while (true) {
			if (m_taskbuffer.Pop(task)) {
				if (task == NULL) break;//Null pointer function is termination
				task();
				m_finishedtaskscount.fetch_add(1,std::memory_order_release);//We are the only one writing so we release
			}else{
				std::unique_lock<std::mutex> lock(m_sleepmutex);
				m_wakecondition.wait_for(lock,50us);//Careful waiting for a lock, let it slip without notify to guarantee release
			}
		}
	}

	//CircularFIFO<BufferFunctionType, TASKLINEBUFFERSIZE> m_taskbuffer;//Default type task buffer
	CircularFIFO_256<BufferFunctionType> m_taskbuffer;
	alignas(64) std::atomic<uint64_t> m_finishedtaskscount;//Local count
	uint64_t m_tlid;//The high level id of the taskline we use, intended as potential core ID as well if affinity is set
	uint64_t m_senttaskscount;
	std::condition_variable m_wakecondition;
	std::mutex m_sleepmutex;//Mutex to allow for sleeping and waking up
	std::thread m_thread;
};

class TaskSystem {
public:
	explicit TaskSystem() : m_tasklines(nullptr), m_tasklinecount(0), m_lasttasklineindex(0), m_taskcount(0) {}

	//void AddDedicatedTaskline() {}
	//assign the number of desired number of tasklines, default is SYSTEM_NUM_HW THREADS
	void Init(){
		//Create hardware locked tasklines
		m_tasklinecount = SYSTEM_NUM_HW_THREADS;
		m_tasklines = new Taskline[m_tasklinecount];
		//Start all tasklines and test them
		for (uint64_t tlid = 0; tlid < m_tasklinecount; ++tlid) {
			m_tasklines[tlid].Engage(tlid,true);
			m_tasklines[tlid].Dispatch([]() {
				std::this_thread::sleep_for(1us);
				});
			++m_taskcount;
		}
		while (!IsAllDone());//Hammer it until all is proven ready
		//Explore benefits/drawbacks of locking main thread to a core
	}

	//Execute a job, cannot fail
	inline TaskID Execute(BufferFunctionType a_task) {
		//Naive load balancing, iterate through the tasklines equally
		++m_lasttasklineindex;//Add one
		if (m_lasttasklineindex >= m_tasklinecount) m_lasttasklineindex = 0;//Go back to first

		while (m_tasklines[m_lasttasklineindex].Dispatch(a_task) == false) {
			++m_lasttasklineindex;
			if (m_lasttasklineindex >= m_tasklinecount) m_lasttasklineindex = 0;//Go back to first
		}
		++m_taskcount;
		//create the return value, yes a struct, dont judge me
		return TaskID{ m_lasttasklineindex, m_tasklines[m_lasttasklineindex].SentTasksCount()};
	}

	////Allows a taskline argument "recommendation" to directly work with a thread. Not recommended at this time as it messes up any load balancing
	//inline TaskID Execute(uint64_t a_tlid, BufferFunctionType a_task) {
	//	m_lasttasklineindex = a_tlid;
	//	if (m_lasttasklineindex >= m_tasklinecount) m_lasttasklineindex = 0;

	//	//Naive load balancing, iterate through the tasklines equally
	//	while (m_tasklines[m_lasttasklineindex].Dispatch(a_task) == false){
	//		++m_lasttasklineindex;
	//		if (m_lasttasklineindex >= m_tasklinecount) m_lasttasklineindex = 0;
	//	}
	//	++m_taskcount;
	//	//create the return value, yes a struct, dont judge me
	//	return TaskID{m_lasttasklineindex, m_tasklines[m_lasttasklineindex].SentTasksCount()};
	//}

	////Insists to use the taskline provided, hammers the thread if necessary, use with caution
	//inline TaskID ExecuteInsist(uint64_t a_tlid, BufferFunctionType a_task) {
	//	while (m_tasklines[a_tlid].Dispatch(a_task) == false) {}////Hammer it
	//	++m_taskcount;
	//	//create the return value, yes a struct, dont judge me
	//	return TaskID{ m_lasttasklineindex, m_tasklines[m_lasttasklineindex].SentTasksCount() };
	//}

	//Ask if the task is done
	bool IsTaskDone(TaskID a_taskid) {
		return m_tasklines[a_taskid.header].IsDoneWith(a_taskid.tasknumber);
	}

	//Expensive call, use only when necessary
	bool IsAllDone() {
		uint64_t sum = 0;
		for (uint64_t tlid = 0; tlid < m_tasklinecount; ++tlid) {
			sum += m_tasklines[tlid].FinishedTasksCount();
		}
		return m_taskcount == sum;
	}

	void Deinit(){
		for (uint64_t tlid = 0; tlid < m_tasklinecount; ++tlid) {
			m_tasklines[tlid].TerminateAndJoin();//Terminate includes joining!!
		}
		m_tasklinecount = 0;
	}
	uint64_t TaskLineCount() const { return m_tasklinecount; }

	~TaskSystem() {if(m_tasklines!=NULL) delete[] m_tasklines;}
protected:
	uint64_t m_tasklinecount;
	uint64_t m_lasttasklineindex;//Which taskline got the last task, naive load balancing
	uint64_t m_taskcount;
	Taskline* m_tasklines;
};

//TaskSystem Tasker;

//A TaskID is granted when executing a task, it allows you to evaluate when a task is done
//struct TaskID {
//	uint64_t header;
//	uint64_t tasknumber;
//};

//
//class Taskline {
//public:
//	Taskline() {}
//
//	void Engage(uint64_t a_tlid, bool a_setaffinity = false) {
//		m_tlid = a_tlid;
//		m_thread = std::thread(&Taskline::__threadexecute, this);
//		if (a_setaffinity && (m_tlid < SYSTEM_NUM_HW_THREADS)) {//Any subsequent threads above HW threads are not assigned to a core directly
//#ifdef _WIN32 //Windows specific thread affinity
//			HANDLE handle = (HANDLE)m_thread.native_handle();
//			// Put each thread on to dedicated core
//			DWORD_PTR affinityMask = 1ull << m_tlid;
//			DWORD_PTR affinity_result = SetThreadAffinityMask(handle, affinityMask);
//			//assert(affinity_result > 0);
//			//BOOL priority_result = SetThreadPriority(handle, THREAD_PRIORITY_HIGHEST);
//			//assert(priority_result != 0);
//			std::wstringstream wss;
//			wss << "Taskline: " << m_tlid;
//			HRESULT hr = SetThreadDescription(handle, wss.str().c_str());
//			//assert(SUCCEEDED(hr));
//
//			//::SetThreadIdealProcessor(handle, m_tlid);
//
//#endif //_WIN32
//#ifdef __linux__
//			////UNTESTED
//			//cpu_set_t cpuset;
//			//CPU_ZERO(&cpuset);
//			//CPU_SET(m_threadid, &cpuset);
//			//int rc = pthread_setaffinity_np(m_thread.native_handle(), sizeof(cpu_set_t), &cpuset);
//			//assert(rc != 0);
//#endif
//		}
//#ifdef _DEBUG
//		//PushThreadDataToConsole();
//#endif
//	}
//
//	//Send a task through the task line, returns false if full
//	inline bool Dispatch(BufferFunctionType _task) {
//		m_wakecondition.notify_one();
//		bool r = m_taskbuffer.Push(_task);
//		if (r) ++m_senttaskscount;
//		return r;
//	}
//
//	//Tell the thread to die when done
//	inline void TerminateAndJoin() {
//		m_wakecondition.notify_one();
//		while (m_taskbuffer.Push(NULL) == false);
//		m_thread.join();
//	}
//#ifdef _DEBUG
//	//Show thread info runtime
//	//void PushThreadDataToConsole() {
//	//	std::this_thread::sleep_for(200ms);
//	//	Push([=](){
//	//		g_debugmutex.lock();
//	//		cout << "Taskline ID:" << this->m_tlid << " NID:" << std::this_thread::get_id() << " running on core " << GetCurrentThreadProcessorNumber() << endl;
//	//		g_debugmutex.unlock();
//	//		std::this_thread::sleep_for(200ms);
//	//	});
//	//}
//#endif
//	//Is the taskline done with all tasks
//	inline bool IsDone() const { return m_finishedtaskscount.load(std::memory_order_acquire) == m_senttaskscount; }
//	//Is the taskline done with the certain taskid
//	inline bool IsDoneWith(uint64_t a_taskid) const { return m_finishedtaskscount.load(std::memory_order_acquire) >= a_taskid; }
//	inline uint64_t SentTasksCount() const { return m_senttaskscount; }
//	inline uint64_t FinishedTasksCount() const { return m_finishedtaskscount.load(std::memory_order_acquire); }
//protected:
//	//Execution scope call!
//	void __threadexecute() {
//		BufferFunctionType task;
//		while (true) {
//			if (m_taskbuffer.Pop(task)) {
//				if (task == NULL) break;//Null pointer function is termination
//				task();
//				m_finishedtaskscount.fetch_add(1, std::memory_order_release);//We are the only one writing so we release
//			}
//			else {
//				std::unique_lock<std::mutex> lock(m_sleepmutex);
//				m_wakecondition.wait_for(lock, 50us);//Careful waiting for a lock, let it slip without notify to guarantee release
//			}
//		}
//	}
//
//	//CircularFIFO<BufferFunctionType, TASKLINEBUFFERSIZE> m_taskbuffer;//Default type task buffer
//	CircularFIFO_256<BufferFunctionType> m_taskbuffer;
//	alignas(64) std::atomic<uint64_t> m_finishedtaskscount{};//Local count
//	uint64_t m_tlid{};//The high level id of the taskline we use, intended as potential core ID as well if affinity is set
//	uint64_t m_senttaskscount{};
//	std::condition_variable m_wakecondition;
//	std::mutex m_sleepmutex;//Mutex to allow for sleeping and waking up
//	std::thread m_thread;
//};
//
//class TaskSystem {
//public:
//	TaskSystem() : m_tasklinecount(0), m_tasklines(NULL) {}
//
//	//assign the number of desired number of tasklines, default is SYSTEM_NUM_HW THREADS
//	void Init(uint64_t a_numtasklines = SYSTEM_NUM_HW_THREADS) {
//		m_tasklinecount = a_numtasklines;
//		m_tasklines = new Taskline[m_tasklinecount];
//
//		//Start all tasklines
//		for (uint64_t tlid = 0; tlid < m_tasklinecount; ++tlid) {
//			m_tasklines[tlid].Engage(tlid, true);
//		}
//		//Explore benefits/drawbacks of locking main thread to a core
//	}
//
//	//Execute a job, cannot fail
//	inline TaskID Execute(BufferFunctionType a_task) {
//		++m_taskcount;
//		//Naive load balancing, iterate through the tasklines equally
//		while (m_tasklines[m_lasttasklineindex].Dispatch(a_task) == false) {
//			++m_lasttasklineindex;
//			if (m_lasttasklineindex >= m_tasklinecount) m_lasttasklineindex = 0;//Go back to first
//		}
//		//create the return value, yes a struct, dont judge me
//		TaskID taskid{ m_lasttasklineindex, m_tasklines[m_lasttasklineindex].SentTasksCount() };
//
//		//Add one
//		++m_lasttasklineindex;
//		if (m_lasttasklineindex >= m_tasklinecount) m_lasttasklineindex = 0;//Go back to first
//		return taskid;
//	}
//
//	inline TaskID Execute(uint64_t a_tlid, BufferFunctionType a_task) {
//		++m_taskcount;
//		//Naive load balancing, iterate through the tasklines equally
//		while (m_tasklines[a_tlid].Dispatch(a_task) == false) {}
//		m_lasttasklineindex = a_tlid;
//		//create the return value, yes a struct, dont judge me
//		TaskID taskid{ m_lasttasklineindex, m_tasklines[m_lasttasklineindex].SentTasksCount() };
//
//		//Add one
//		++m_lasttasklineindex;
//		if (m_lasttasklineindex >= m_tasklinecount) m_lasttasklineindex = 0;//Go back to first
//		return taskid;
//	}
//	//Ask if the task is done
//	bool IsTaskDone(TaskID a_taskid) {
//		return m_tasklines[a_taskid.header].IsDoneWith(a_taskid.tasknumber);
//	}
//
//	//Expensive call, use only when necessary
//	bool IsAllDone() {
//		uint64_t sum = 0;
//		for (uint64_t tlid = 0; tlid < m_tasklinecount; ++tlid) {
//			sum += m_tasklines[tlid].FinishedTasksCount();
//		}
//		return m_taskcount == sum;
//	}
//
//	void Deinit() {
//		for (uint64_t tlid = 0; tlid < m_tasklinecount; ++tlid) {
//			m_tasklines[tlid].TerminateAndJoin();//Terminate includes joining!!
//		}
//	}
//	uint64_t TaskLineCount() const { return m_tasklinecount; }
//
//	~TaskSystem() {
//		//assert(m_tasklines != NULL);
//		if (m_tasklines != NULL) delete[] m_tasklines;
//	}
//protected:
//	uint64_t m_tasklinecount{};
//	uint64_t m_lasttasklineindex{};//Which taskline got the last task, naive load balancing
//	uint64_t m_taskcount{};
//	Taskline* m_tasklines;
//};
