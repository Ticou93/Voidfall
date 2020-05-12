#include "TaskSystem.h"

class TestTaskSystem {
public:
    std::mutex debugmutex;

    void grid(int64_t a) {
        int64_t z = 0;
        for (int64_t x = 0; x < a; x++) {
            ++z;
        }
    }

    int testprog() {
        TaskSystem sys;
        sys.Init();

        sys.Execute([] (){std::cout << "Number of cores " << SYSTEM_NUMCORES << std::endl; });
        std::this_thread::sleep_for(4s);

        for (int64_t i = 0; i < 200000; ++i) {
            sys.Execute([this,i]() {
                grid(i);
                });
            if (i % 10000 == 0) {
                sys.Execute([=]() {
                    debugmutex.lock();
#ifdef _WIN32
                    std::cout << "Taskline TID:" << "N/A" << " NID:" << std::this_thread::get_id() << " running on core " << GetCurrentProcessorNumber() << std::endl;
#endif
#ifdef __linux__
                    unsigned int cpu;
                    unsigned int node;
                    getcpu(&cpu, &node);
		            std::cout << "Taskline TID:" << "N/A" << " NID:" << std::this_thread::get_id() << " running on core " << cpu << " and node " << node << std::endl;
#endif
                    debugmutex.unlock();
                    std::this_thread::sleep_for(1s);
                });
            }
        }

        std::this_thread::sleep_for(7s);

        for (int64_t i = 0; i < 200000; ++i) {
            sys.Execute([this,i]() {
                grid(i);
                });
            if (i % 10000 == 0) {
                sys.Execute([=]() {
                    debugmutex.lock();
#ifdef _WIN32
                    std::cout << "Taskline TID:" << "N/A" << " NID:" << std::this_thread::get_id() << " running on core " << GetCurrentProcessorNumber() << std::endl;
#endif
#ifdef __linux__
                    unsigned int cpu;
                    unsigned int node;
                    getcpu(&cpu, &node);
		            std::cout << "Taskline TID:" << "N/A" << " NID:" << std::this_thread::get_id() << " running on core " << cpu << " and node " << node << std::endl;
#endif
                    debugmutex.unlock();
                    std::this_thread::sleep_for(1s);
                    });
            }
        }

        sys.Execute([] {std::cout << "Finished!" << std::endl; });
        sys.Deinit();
        return 0;
    }
};
