#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <vector>

template<typename T>
class RingBuffer
{
public:
	typedef T*             iterator;
    typedef const T*       const_iterator;

	RingBuffer()
	{
		read = 0;
		write = 0;
		empty = true;
		overflow = false;
	}
	RingBuffer(int size)
	{
		RingBuffer();
		resize(size);		
	}


	void push_back(T item)
	{
		if(overflow)
		{
			if(onOverflow)
			{
				onOverflow();
			}
			else
			{
				throw std::exception("container overflow!");
			}
		}

		array[write++] = item;
		if( write >= size ) write = 0;
		empty = false;
		if ( write == read )
		{
			overflow = true; 
		}
	}
	T &pop()
	{
		if ( empty )
		{
			throw std::exception("container is empty");
		}
		T elem = array[ read++ ];
		if ( read >= size ) read = 0;
		overflow = false;
		if ( write == read )
			empty = true;
		return elem; 
	}

	void setReadPointerTo(int index)
	{
		read = index;
		if ( read >= size ) read = 0;
		if( write != read) empty = false;
	}

	int getReadPointer()
	{
		return read;
	}

	bool is_empty()
	{
		return empty;
	}

	void setOnOverflowFunc(void (*func)())
	{
		onOverflow = func;
	}
	void setOnEmptyFunc(void (*func)())
	{
		onEmpty = func;
	}

	void resize(int s)
	{
		size = s;
		array = new T[size];
	}
protected:
	T *array;
	int size;
	int read;
	int write;
	bool overflow;
	bool empty;
	void (*onOverflow)();
	void (*onEmpty)();
};

#endif