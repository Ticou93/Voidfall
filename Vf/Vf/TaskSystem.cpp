#include "TaskSystem.h"
#include <string>
#include <thread>

void grid(int64_t a){
	int64_t z = 0;
	for (int64_t x = 0; x < a; x++) {
		++z;
	}
	
}

int testprog(){
	TaskSystem sys;
	sys.Init();

	sys.Execute([] (){cout << "Number of cores " << SYSTEM_NUMCORES << endl; });
	std::this_thread::sleep_for(4s);
	
	for (int64_t i = 0; i < 200000; ++i) {
		sys.Execute([i]() {
			grid(i);
			});
		if (i % 10000 == 0) {
			sys.Execute([=]() {
				g_debugmutex.lock();
				cout << "Taskline TID:" << "N/A" << " NID:" << std::this_thread::get_id() << " running on core " << GetCurrentProcessorNumber() << endl;
				g_debugmutex.unlock();
				std::this_thread::sleep_for(1s);
			});
		}
	}

	std::this_thread::sleep_for(7s);
	
	for (int64_t i = 0; i < 200000; ++i) {
		sys.Execute([i]() {
			grid(i);
			});
		if (i % 10000 == 0) {
			sys.Execute([=]() {
				g_debugmutex.lock();
				cout << "Taskline TID:" << "N/A" << " NID:" << std::this_thread::get_id() << " running on core " << GetCurrentProcessorNumber() << endl;
				g_debugmutex.unlock();
				std::this_thread::sleep_for(1s);
				});
		}
	}
	
	sys.Execute([] {cout << "Finished!" << endl; });
	sys.Deinit();
	return 0;
}
