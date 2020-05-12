#pragma once

#include <atomic>
#include <cstddef>

//Thread safe circular feed container, constant size
//Does not care about anything else but providing the most basic functionality

template <class T, size_t _SIZE>
class CircularFIFO {
public:
	//Ignore buffer initialization
	CircularFIFO() : m_readpos(0), m_writepos(0) {}

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
	bool inline isEmpty() const { return m_writepos.load() == m_readpos.load(); }

	//Determine if the feed is not empty
	bool inline NotEmpty() const { return m_writepos.load() != m_readpos.load(); }

	//Determine if the feed is full
	bool inline isFull() const {
		if ((m_writepos.load() + 1) % _SIZE == m_readpos.load()) return true;
		return false;
	}
	//Determine if the feed is not full
	bool inline NotFull() { return !isFull(); }

protected:

	alignas(16) std::atomic<size_t> m_readpos;
	alignas(16) std::atomic<size_t> m_writepos;
	T m_buffer[_SIZE];
};