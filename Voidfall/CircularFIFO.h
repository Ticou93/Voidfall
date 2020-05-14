#pragma once

#include <atomic>
#include <cstddef>

const uint64_t CACHELINESIZE = 64;//We assume 64 byte cachelines

//Thread safe circular feed container, constant size
//Does not care about anything else but providing the most basic functionality

template <class T, size_t _SIZE>
class CircularFIFO {
public:
	//Ignore buffer initialization
	CircularFIFO() : m_readpos(0),m_writepos(0){}

	//Insert an element
	inline bool Push(const T& t) {
		const size_t writepos = m_writepos.load(std::memory_order_relaxed);
		const size_t writeposnext = ((writepos + 1) % _SIZE);//increment and modulo the size to bypass sub zero barrier
		if (writeposnext == m_readpos.load(std::memory_order_acquire)) return false;//Full, return failed operation
		m_buffer[writepos] = t;
		m_writepos.store(writeposnext, std::memory_order_release);
		return true;
	}

	//Pop into the reference
	inline bool Pop(T& _t) {
		const size_t readpos = m_readpos.load(std::memory_order_relaxed);
		if (readpos == m_writepos.load(std::memory_order_acquire)) return false;//If empty, failed.	
		_t = m_buffer[readpos];
		m_readpos.store(((readpos + 1) % _SIZE), std::memory_order_release);
		return true;
	}

	//Determine if the feed is empty
	bool inline IsEmpty() const { return m_writepos.load() == m_readpos.load(); }

	//Determine if the feed is full
	bool inline IsFull() const {
		return ((m_writepos.load() + 1) % _SIZE) == m_readpos.load();
	}
	
protected:
	alignas(CACHELINESIZE) std::atomic<size_t> m_readpos;
	alignas(CACHELINESIZE) std::atomic<size_t> m_writepos;
	alignas(CACHELINESIZE) T m_buffer[_SIZE];
};

//NOTES
//x86 has acquire on reads, release on writes by default. Acquire on writes and release on reads is EXPENSIVE however.
//x86 does not differentiate between std::memory_order_cst and std::memory_order_acq_rel
//memory_order_cst is used by default on normal assignment and use such as = or load() without explicit argument, BE WARY as it is SLOW
//


//Uses byte overflow to avoid span check, can only contain 256 elements
//Single writer, single reader!
template <class T>
class CircularFIFO_256 {
public:
	//Ignore buffer initialization
	CircularFIFO_256() : m_readpos(0), m_writepos(0) {}

	//Insert an element, the writer
	inline bool Push(const T& t) {
		const uint8_t writepos = m_writepos.load(std::memory_order_relaxed);//We are the writer and only writer to m_writepos, relaxed because we know our writer side value already
		const uint8_t writeposnext = writepos + 1;
		if(writeposnext == m_readpos.load(std::memory_order_acquire)) return false;//Full, return failed operation, we aqcuire the latest data
		m_buffer[writepos] = t;
		m_writepos.store(writeposnext, std::memory_order_release);//We "release" the new m_writepos value to be visible by the reader
		return true;
	}

	//Pop into the reference, the reader
	inline bool Pop(T& _t) {
		const uint8_t readpos = m_readpos.load(std::memory_order_relaxed);//Relaxed because we are the reader and we write to m_readpos
		if (readpos == m_writepos.load(std::memory_order_acquire)) return false;//Acquire the m_writepos data. If empty(equal), failed.	
		_t = m_buffer[readpos];
		m_readpos.store(readpos + 1, std::memory_order_release);
		return true;
	}

	////Determine if the feed is empty, reader function!!!
	bool inline IsEmpty(){return m_writepos.load(std::memory_order_acquire) == m_readpos.load(std::memory_order_relaxed); }

	////Determine if the feed is full, writer function!!!
	bool inline IsFull(){
		return (m_writepos.load(std::memory_order_relaxed) + 1) == m_readpos.load(std::memory_order_acquire);
	}
protected:
	alignas(CACHELINESIZE) std::atomic<uint8_t> m_readpos;
	alignas(CACHELINESIZE) std::atomic<uint8_t> m_writepos;
	alignas(CACHELINESIZE) T m_buffer[256];
};
