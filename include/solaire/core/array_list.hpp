#ifndef SOLAIRE_CORE_ARRAY_LIST_HPP
#define SOLAIRE_CORE_ARRAY_LIST_HPP

//Copyright 2016 Adam G. Smith
//
//Licensed under the Apache License, Version 2.0 (the "License");
//you may not use this file except in compliance with the License.
//You may obtain a copy of the License at
//
//http ://www.apache.org/licenses/LICENSE-2.0
//
//Unless required by applicable law or agreed to in writing, software
//distributed under the License is distributed on an "AS IS" BASIS,
//WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//See the License for the specific language governing permissions and
//limitations under the License.

#include "allocator.hpp"

namespace solaire {
	template<class T, class I, class ALLOCATOR, I DEFAULT_ALLOCATION>
	class array_list {
	public:
		typedef T type;
		typedef I index;
		typedef ALLOCATOR allocator;
		typedef T* iterator;
		typedef const T* const_iterator;

		template<class T2 = T, class I2 = I, class ALLOCATOR2 = ALLOCATOR, I2 DEFAULT_ALLOCATION2 = DEFAULT_ALLOCATION>
		friend class array_list;
	private:
		ALLOCATOR mAllocator;
		I mSize;
	public:
		constexpr size_t size() const { return static_cast<size_t>(mSize); }
		constexpr size_t capacity() const { return static_cast<size_t>(mAllocator.size() / sizeof(T)); }
		inline void clear() { mSize = 0; }
		inline T& operator[](const size_t aIndex) { return static_cast<T*>(mAllocator.get())[aIndex]; }
		constexpr const T& operator[](const size_t aIndex) const { return static_cast<T*>(mAllocator.get())[aIndex]; }
		inline T& back() { return static_cast<T*>(mAllocator.get())[mSize - 1]; }
		constexpr const T& back() const { return static_cast<T*>(mAllocator.get())[mSize - 1]; }
		inline T& front() { return static_cast<T*>(mAllocator.get())[mSize - 1]; }
		constexpr const T& front() const { return static_cast<T*>(mAllocator.get())[mSize - 1]; }
		inline void pop_back() { mSize = mSize == 0 : 0 : mSize - 1; }
		inline iterator begin() { return static_cast<T*>(mAllocator.get()); }
		constexpr const_iterator begin() const { return static_cast<T*>(mAllocator.get()); }
		inline iterator end() { return static_cast<T*>(mAllocator.get()) + mSize; }
		constexpr const_iterator end() const { return static_cast<T*>(mAllocator.get()) + mSize; }
		inline void reserve(size_t aSize) const { if(aSize > capacity()) resize(aSize); }
		inline void pop_front() { erase(begin()); }
		inline T& push_front(const T& aValue) { return *insert(begin(), aValue); }

		constexpr array_list() :
			mAllocator(),
			mSize(0)
		{}

		constexpr array_list(size_t aCapacity) :
			mAllocator(),
			mSize(0)
		{
			resize(aCapacity);
		}


		template<class I2 = I, class ALLOCATOR2 = ALLOCATOR, I2 DEFAULT_ALLOCATION2 = DEFAULT_ALLOCATION>
		array_list(const array_list<T, I2, ALLOCATOR2, DEFAULT_ALLOCATION2>& aOther) :
			mAllocator(),
			mSize(0)
		{
			const size_t s = aOther.size();
			mAllocator.allocate(s * sizeof(T));
			mSize = s;
			T* const ptr = static_cast<T*>(mAllocator.get());
			for(size_t i = 0; i < s; ++i) new(ptr + i) T(aOther[i]);
		}

		template<class I2 = I, I2 DEFAULT_ALLOCATION2 = DEFAULT_ALLOCATION, class ENABLE = typename std::enable_if<std::is_move_constructible<ALLOCATOR>::value>::type>
		array_list(array_list<T, I2, ALLOCATOR, DEFAULT_ALLOCATION2>&& aOther) :
			mAllocator(),
			mSize(aOther.mSize)
		{
			std::swap(mAllocator, aOther.mAllocator);
			aOther.mSize = 0;
		}

		~array_list() {
			if(mAllocator.size() != 0) {
				T* const ptr = static_cast<T*>(mAllocator.get());
				for(I i = 0; i < capacity(); ++i) ptr[i].~T();
				mAllocator.deallocate();
				mSize = 0;
			}
		}

		template<class I2 = I, class ALLOCATOR2 = ALLOCATOR, I2 DEFAULT_ALLOCATION2 = DEFAULT_ALLOCATION>
		array_list<T,I,ALLOCATOR,DEFAULT_ALLOCATION>& operator=(const array_list<T, I2, ALLOCATOR2, DEFAULT_ALLOCATION2>& aOther) {
			const size_t s = aOther.size();
			reserve(s);
			mSize = s;
			T* const ptr = static_cast<T*>(mAllocator.get());
			for(size_t i = 0; i < s; ++i) ptr[i] = aOther[i];
			return *this;
		}

		template<class I2 = I, I2 DEFAULT_ALLOCATION2 = DEFAULT_ALLOCATION, class ENABLE = typename std::enable_if<std::is_move_assignable<ALLOCATOR>::value>::type>
		array_list<T, I, ALLOCATOR, DEFAULT_ALLOCATION>& operator=(const array_list<T, I2, ALLOCATOR, DEFAULT_ALLOCATION2>& aOther) {
			std::swap(mAllocator, aOther.mAllocator);
			size_t tmp = mSize;
			mSize = aOther.mSize;
			aOther.mSize = tmp;
			return *this;
		}

		inline T& push_back(const T& aValue) {
			if(mSize == capacity()) resize(capacity() == 0 ? DEFAULT_ALLOCATION : capacity() * 2);
			return static_cast<T*>(mAllocator.get())[mSize++] = aValue;
		}

		iterator erase(const const_iterator aPosition) {
			const size_t offset = aPosition - static_cast<T*>(mAllocator.get());
			--mSize;
			for(size_t i = offset; i < mSize; ++i) static_cast<T*>(mAllocator.get())[i] = std::move(static_cast<T*>(mAllocator.get())[i + 1]);
			return static_cast<T*>(mAllocator.get()) + offset;
		}

		iterator insert(const const_iterator aPosition, const T& aValue) {
			const size_t offset = aPosition - static_cast<T*>(mAllocator.get());
			if(mSize == capacity()) resize(capacity() == 0 ? DEFAULT_ALLOCATION : capacity() * 2);
			for(size_t i = mSize; i > offset; --i) static_cast<T*>(mAllocator.get())[i] = std::move(static_cast<T*>(mAllocator.get())[i-1]);
			static_cast<T*>(mAllocator.get())[offset] = aValue;
			++mSize;
			return static_cast<T*>(mAllocator.get()) + offset;
		}
	private:
		template<class A = ALLOCATOR>
		typename std::enable_if<std::is_move_assignable<A>::value, void>::type resize(const I aCapacity) {
			if(aCapacity <= capacity()) return;
			ALLOCATOR tmp;
			tmp.allocate(aCapacity * sizeof(T));
			T* const oldPtr = static_cast<T*>(mAllocator.get());
			T* const newPtr = static_cast<T*>(tmp.get());
			for(I i = 0; i < mSize; ++i) new(newPtr + i) T(std::move(oldPtr[i]));
			for(I i = mSize; i < aCapacity; ++i) new(newPtr + i) T();
			for(I i = 0; i < capacity(); ++i) oldPtr[i].~T();
			std::swap(mAllocator, tmp);
			tmp.deallocate();
		}

		template<class A = ALLOCATOR>
		typename std::enable_if<! std::is_move_assignable<A>::value, void>::type resize(const I aCapacity) {
			if(aCapacity > capacity()) throw std::runtime_error("solaire::array_list::resize : New size exeeds capacity");
		}
	};

	template<class T, size_t SIZE, class I = size_t>
	using static_array_list = array_list<T, I, static_allocator<SIZE * sizeof(T)>, SIZE>;

	template<class T, class I = size_t, I DEFAULT_ALLOCATION = 16>
	using heap_array_list = array_list<T, I, heap_allocator, DEFAULT_ALLOCATION>;
}

#endif