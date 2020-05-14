#pragma once

#ifndef NDEBUG

#include <cassert>
#include <fstream>
#include <stdio.h>
#include <iostream>
#include <mutex>
#include <string>
#include <stdio.h>
#include <stdarg.h>

//Print any kind of message PRINT("Hello"), PRINT("Data %i %d", 42, 0.006); etc.
//Mutex guarded
//Guarded with zerro cost compiler elimination for release mode
#define PRINT(fmt, ...) {const std::lock_guard<std::mutex> lock(SyncMutex());fprintf(stderr, "%s:%i:%s(): " fmt "\n", __FILE__, __LINE__, __func__, __VA_ARGS__);}
#define DOUT(stream) {const std::lock_guard<std::mutex> lock(SyncMutex());std::cout << stream << "\n";}
#define DERR(stream) {const std::lock_guard<std::mutex> lock(SyncMutex());std::cerr << stream << "\n";}

//Single global scope mutex for debug syncing
inline std::mutex& SyncMutex() {
	static std::mutex g_debugmutex;
	return g_debugmutex;
}
#else
#define PRINT(fmt, ...) do{}while(0);//Eliminated by compiler for release
#define DOUT(stream) do{}while(0)
#define DERR(stream) do{}while(0)


#endif