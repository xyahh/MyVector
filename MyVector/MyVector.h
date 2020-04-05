/*
Vector class that mimics the STL Vector functionalities.
Created by Juan Marcelo Portillo in Spring 2018.

This was made just for fun and to learn how Iterators, Containers work with
the Algorithm Library.

Things to note:
- This vector class is faster in Debug Mode because there's no Debugging going on (while in STL vector there is)
	but can be more dangerous.
- This vector class is just a bit faster in Release mode.

*/


#pragma once
#include <utility>

namespace std
{
	struct random_access_iterator_tag;
	template<class MyVec>
	class MyVectorIterator
	{
	public:

		using iterator_category = random_access_iterator_tag;
		using value_type = typename MyVec::value_type;
		using difference_type = typename MyVec::difference_type;
		using pointer = typename MyVec::pointer;
		using reference = typename MyVec::reference;



	public:


		MyVectorIterator() : pt() {}

		MyVectorIterator(pointer ptr) : pt(ptr) {}

		reference operator*() const
		{
			return *pt;
		}

		pointer operator->() const
		{
			return &(**this);
		}

		MyVectorIterator& operator++()
		{
			++pt;
			return *this;
		}

		MyVectorIterator operator++(int)
		{
			MyVectorIterator t(*this);
			++(*this);
			return t;
		}

		MyVectorIterator& operator--()
		{
			--pt;
			return *this;
		}

		MyVectorIterator operator--(int)
		{
			MyVectorIterator t(*this);
			--(*this);
			return t;
		}

		MyVectorIterator& operator+=(difference_type diff)
		{
			pt += diff;
			return *this;
		}

		MyVectorIterator operator+(difference_type diff) const
		{
			MyVectorIterator tmp(*this);
			return tmp += diff;
		}

		MyVectorIterator& operator-=(difference_type diff)
		{
			pt -= diff;
			return *this;
		}

		MyVectorIterator operator-(difference_type diff) const
		{
			MyVectorIterator tmp(*this);
			return tmp -= diff;
		}

		difference_type operator-(const MyVectorIterator& other) const
		{
			return pt - other.pt;
		}

		bool operator==(const MyVectorIterator& other) const
		{
			return pt == other.pt;
		}

		bool operator!=(const MyVectorIterator& other) const
		{
			return !(*this == other);
		}

		bool operator<(const MyVectorIterator& other) const
		{
			return pt < other.pt;
		}

		bool operator>(const MyVectorIterator& other) const
		{
			return other < *this;
		}

		bool operator<=(const MyVectorIterator& other) const
		{
			return !(other < *this);
		}

		bool operator>=(const MyVectorIterator& other) const
		{
			return !(*this < other);
		}

	private:

		pointer pt;

	};

	template<class T>
	class MyVector
	{
	public:

		using difference_type = int;
		using value_type = T;
		using pointer = value_type*;
		using reference = value_type&;
		using size_type = size_t;

		using base = MyVector<value_type>;
		using iter = MyVectorIterator<base>;

	public:

		MyVector()
		{
			data = nullptr;
			sz = 0;
			cpty = 0;
		}

		MyVector(size_type szSize)
		{
			data = nullptr;
			sz = 0;
			cpty = 0;
			resize(szSize);
		}

		void reserve(size_type new_capacity)
		{
			if (new_capacity > cpty)
			{
				pointer memory_block = static_cast<pointer>(::operator new(sizeof(value_type) * new_capacity));
				for (size_type i = 0; i < sz; ++i)
					::new(static_cast<void*>(&memory_block[i])) value_type(move(data[i]));
				safeDeleteData();
				data = memory_block;
				cpty = new_capacity;
			}
		}

		void resize(size_type new_size)
		{
			if (new_size > cpty)
				reserve(new_size);
			while (new_size > sz)
				emplace_back();
		}

		void shrink_to_fit()
		{
			if (sz < cpty)
			{
				pointer memory_block = static_cast<pointer>(::operator new(sizeof(value_type) * sz));
				for (size_type i = 0; i < sz; ++i)
					::new(static_cast<void*>(&memory_block[i])) value_type(move(data[i]));
				safeDeleteData();
				data = memory_block;
				cpty = sz;
			}
		}

		void push_back(const value_type& t)
		{
			emplace_back(t);
		}

		void push_back(value_type&& t) noexcept
		{
			emplace_back(move(t));
		}

		template<class... Args>
		void emplace_back(Args&& ... args)
		{
			if (sz == cpty)
				reserve(newCapacity(sz + 1));
			::new(static_cast<void*>(&data[sz++])) value_type(forward<Args>(args)...);
		}

		~MyVector()
		{
			safeDeleteData();
			sz = 0;
		}

		size_type size() const
		{
			return sz;
		}

		size_type capacity() const
		{
			return cpty;
		}

		void clear()
		{
			for (size_t i = 0; i < sz; ++i)
				data[i].~value_type(); //again, manually calling the destructors since we are not actually deleting any data from our block
			sz = 0;
		}

		value_type& operator[](size_type idx)
		{
			return data[idx];
		}

		value_type& operator[](size_type idx) const
		{
			return data[idx];
		}

		iter begin()
		{
			return iter(data);
		}

		iter end()
		{
			return iter(data + sz);
		}

	private:

		size_type newCapacity(size_type new_size)
		{
			/*if (cpty > max_size() - cpty / 2)
			return new_size;*/

			const size_type geometric = cpty + cpty / 2; 
			if (geometric < new_size)
				return (new_size);
			return geometric;	// geometric growth is sufficient
		}

		void safeDeleteData()
		{
			for (size_type i = 0; i < sz; ++i)
				data[i].~value_type(); 
			//manually calling the destructor because the deletion will only delete the data block without calling destructors
			::operator delete(data);
		}

	private:

		pointer		data;
		size_type	sz;
		size_type	cpty;
	};

}

