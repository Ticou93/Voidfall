#pragma once


//Simple unordered self-repairing array
template <class T, UINT __SIZE = 1024>
class USRArray {
public:
	USRArray(): m_count(0){}

	inline bool IsEmpty()const { return (m_count == 0); }
	inline UINT Count()const {return m_count;}
	inline UINT Size() const { return __SIZE;}

	inline T& operator[](UINT _index) { return m_buffer[_index]; }

	bool Add(T _t){
		if (m_count == __SIZE) return false; //Full
		m_buffer[m_count] = _t; //Set the element	
		++m_count;
		return true;
	}

	
	T Remove(UINT a_index) {
		T tmp = m_buffer[a_index];
		--m_count;
		m_buffer[a_index] = m_buffer[m_count]; //Take the last and set it to this spot
		return tmp;
	}

protected:
	T m_buffer[__SIZE];
	UINT m_count;
};

//Simple unordered self-repairing array dynamic, self resizing
template <class T, UINT __MINIMUMSIZE = 8>
class USRDynamicArray {
public:
	USRDynamicArray() : m_size(__MINIMUMSIZE),
		m_count(0),
		m_buffer(new T[__MINIMUMSIZE]) {
		//for (UINT i = 0; i < m_size; ++i) m_buffer[i] = 0;
		memset(m_buffer, 0, m_size * sizeof(*m_buffer));
	}

	inline bool IsEmpty()const { return (m_count == 0); }
	inline UINT Count()const { return m_count; }

	inline T& operator[](UINT _index) { return m_buffer[_index]; }

	//Insert a new element
	void Push(T _t) {
		if (m_count == m_size) {//Is last the last element not allocated?	
			UINT size_temp = m_size * 2;//Double size
			T* buffer_temp = new T[size_temp];//Allocate more memory
			//ASSERT(buffer_temp != NULL);//Should trigger an error
			//ASSERT(m_buffer != NULL);
			for (UINT i = 0; i < m_count; ++i) buffer_temp[i] = m_buffer[i];  //Copy old array to new array
			SDA(m_buffer);//delete old array
			m_buffer = buffer_temp;//set p to new array
			m_size = size_temp; //set new size
		}
		m_buffer[m_count] = _t; //Set the pointer element	
		++m_count;
	}
	inline T& Peek(UINT i) { return m_buffer[i]; }
	inline UINT Size() const { return m_size; }
	T Pop(UINT _index) {
		//Check if size of buffer is too large for constraints
		if (m_size > __MINIMUMSIZE && m_count < (m_size / 8)) {
			//time to halve the size
			UINT size_temp = m_size / 2;
			T* buffer_temp = new T[size_temp];//Allocate more memory
			for (UINT i = 0; i < m_count; ++i) buffer_temp[i] = m_buffer[i]; //Copy old array to new array
			SDA(m_buffer);//delete old array
			m_size = size_temp;
			m_buffer = buffer_temp;//set p to new array
		}
		T tmp = m_buffer[_index];
		--m_count;
		m_buffer[_index] = m_buffer[m_count]; //Take the last and set it to this spot
		return tmp;
	}
	~USRDynamicArray() { SDA(m_buffer); }
protected:
	T* m_buffer;
	UINT m_size;//Size of the allocated memory	
	UINT m_count; //last is the index to the empty spot in the back, so last being the number of elements stored	
};

