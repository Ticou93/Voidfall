#pragma once

#include <iostream>
#include <thread>
#include <atomic>
#include "CircularFIFO.h"
#include <functional>
#include <algorithm>
#include <condition_variable>
//#include "debug.h"
#include <cassert>
#include <mutex>

//Find out our count for system cores(including virtual ones) and suggested number of threads
const unsigned int SYSTEM_NUMCORES = std::thread::hardware_concurrency() - 1;
const unsigned int SYSTEM_NUM_HW_THREADS = std::max(1u, SYSTEM_NUMCORES);

constexpr unsigned int THREADBUFFERSIZE=1024;

#define BUFFERFUNCTIONTYPE std::function<void()>

#ifdef _WIN32
#include <Windows.h>
#include <sstream>
#endif

#ifdef __linux__
#include <pthread.h>
#endif

using namespace std;

class Taskline{
public:
    std::mutex g_debugmutex;

	Taskline() : m_threadid(0){}

	void Engage(unsigned int a_threadid, bool a_setaffinity = false){
		m_threadid = a_threadid;
		m_thread = std::thread(&Taskline::__threadexecute, this);
		if (m_threadid < SYSTEM_NUM_HW_THREADS){//Any subsequent threads above HW threads are not assigned to a core directly
#ifdef _WIN32 //Windows specific thread affinity
			HANDLE handle = (HANDLE)m_thread.native_handle();
			// Put each thread on to dedicated core
			DWORD_PTR affinityMask = 1ull << m_threadid;
			DWORD_PTR affinity_result = SetThreadAffinityMask(handle, affinityMask);
			assert(affinity_result > 0);
			//BOOL priority_result = SetThreadPriority(handle, THREAD_PRIORITY_HIGHEST);
			//assert(priority_result != 0);
			std::wstringstream wss;
			wss << "Taskline: " << m_threadid;
			HRESULT hr = SetThreadDescription(handle, wss.str().c_str());
			assert(SUCCEEDED(hr));
#endif
#ifdef __linux__
			cpu_set_t cpuset;
			CPU_ZERO(&cpuset);
			CPU_SET(m_threadid, &cpuset);
			int rc = pthread_setaffinity_np(m_thread.native_handle(), sizeof(cpu_set_t), &cpuset);
			assert(rc != 0);
#endif
		}
#ifdef _DEBUG
		PushThreadDataToConsole();
#endif
	}

	//Push a task into the task line, returns false if full
	inline bool Push(BUFFERFUNCTIONTYPE _task){
		m_wakecondition.notify_one();
		return m_taskbuffer.Push(_task);
	}

	//Tell the thread to die when done
	inline void TerminateAndJoin() {
		m_wakecondition.notify_one();
		while(m_taskbuffer.Push(NULL)==false);
		//while(Push(NULL)==false);
		m_thread.join();
	}

#ifdef _DEBUG
	//Show thread info runtime
	void PushThreadDataToConsole() {
		std::this_thread::sleep_for(200ms);
		Push([=](){
			g_debugmutex.lock();
#ifdef _WIN32
			std::cout << "Taskline TID:" << this->m_threadid << " NID:" << std::this_thread::get_id() << " running on core " << (int)GetCurrentProcessorNumber() << std::endl;
#endif
#ifdef __linux__
            unsigned int cpu;
            unsigned int node;
            getcpu(&cpu, &node);
		    std::cout << "Taskline TID:" << this->m_threadid << " NID:" << std::this_thread::get_id() << " running on core " << cpu << " and node " << node << std::endl;
#endif
			g_debugmutex.unlock();
			std::this_thread::sleep_for(200ms);
		});
	}
#endif

protected:
	//Execution scope call!
	void __threadexecute(){
		BUFFERFUNCTIONTYPE task;
		while (true) {
			if (m_taskbuffer.Pop(task)) {
				if (task == NULL) break;//Null pointer function is termination
				task();
			}else{
				std::unique_lock<std::mutex> lock(m_sleepmutex);
				//DPrintString("Sleeping " + std::to_string(m_threadid));
				m_wakecondition.wait_for(lock,50us);//Careful waiting for a lock, let it pass without notify to guarantee release
				//DPrintString("Waking " + std::to_string(m_threadid));
			}
		}
	}
	unsigned int m_threadid;//The high level id of the thread we use, intended as potential core ID as well if affinity is set
	std::condition_variable m_wakecondition;
	std::mutex m_sleepmutex;//Mutex to allow for sleeping and waking up
	std::thread m_thread;
	CircularFIFO<BUFFERFUNCTIONTYPE, THREADBUFFERSIZE> m_taskbuffer;//Default type task buffer
	friend class TaskSystem;//We can all just get along!
};

class TaskSystem {
public:
	TaskSystem() : m_tasklinecount(0),m_tasklines(NULL){}

	//assign the number of threads or desired number of tasklines
	void Init(const unsigned int a_numthreads = SYSTEM_NUM_HW_THREADS){
		m_tasklinecount = a_numthreads;
		m_tasklines = new Taskline[m_tasklinecount];

		//Start all threads
		for (unsigned int threadid = 0; threadid < m_tasklinecount; ++threadid) {
			m_tasklines[threadid].Engage(threadid);
		}
	}

	//Execute a job
	inline void Execute(BUFFERFUNCTIONTYPE a_task) {
		//Implement priority system such that a task is inserted where most applicable
		unsigned int i = 0;
		//Brute force, fills all cores one at a time
		while (m_tasklines[i].Push(a_task) == false) {
			++i;
			if (i >= m_tasklinecount) i = 0;//Go back to first
		}
	}

	void Deinit(){
		//DPrintString("System Deinitializing");
		for (unsigned int tlid = 0; tlid < m_tasklinecount; ++tlid) {
			m_tasklines[tlid].TerminateAndJoin();//Terminate includes joining!!
		}
		//DPrintString("System Deinitialized");
	}

	~TaskSystem() {
		assert(m_tasklines != NULL);
		delete[] m_tasklines;
		m_tasklines = NULL;
	}

protected:
	Taskline* m_tasklines;
	unsigned int m_tasklinecount;
};
