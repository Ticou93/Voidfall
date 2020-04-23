#pragma once

#ifdef _DEBUG

#include <assert.h>
#include <fstream>
#include <stdio.h>
#include <iostream>
#include <mutex>
#include <string>


std::mutex g_debugmutex;

inline void DPrintString(const char* a_ch) {
	g_debugmutex.lock();
	std::cout << a_ch << std::endl;
	g_debugmutex.unlock();
}

inline void DPrintString(std::string str) {
	g_debugmutex.lock();
	std::cout << str << std::endl;
	g_debugmutex.unlock();
}


#endif