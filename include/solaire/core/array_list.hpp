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
		T* mData;
		I mSize;
		I mCapacity;
	public:
		constexpr size_t size() const { return static_cast<size_t>(mSize); }
		constexpr size_t capacity() const { return static_cast<size_t>(mCapacity); }
		inline void clear() { mSize = 0; }
		inline T& operator[](const size_t aIndex) { return mData[aIndex]; }
		constexpr const T& operator[](const size_t aIndex) const { return mData[aIndex]; }
		inline T& back() { return mData[mSize - 1]; }
		constexpr const T& back() const { return mData[mSize - 1]; }
		inline T& front() { return mData[mSize - 1]; }
		constexpr const T& front() const { return mData[mSize - 1]; }
		inline void pop_back() { mSize = mSize == 0 : 0 : mSize - 1; }
		inline iterator begin() { return mData; }
		constexpr const_iterator begin() const { return mData; }
		inline iterator end() { return mData + mSize; }
		constexpr const_iterator end() const { return mData + mSize; }
		inline void reserve(size_t aSize) const { if(aSize > mCapacity) resize(aSize); }
		inline void pop_front() { erase(begin()); }
		inline T& push_front(const T& aValue) { return *insert(begin(), aValue); }

		constexpr array_list() :
			mData(nullptr), 
			mSize(0), 
			mCapacity(0) 
		{}

		constexpr array_list(size_t aCapacity) :
			mData(nullptr),
			mSize(0),
			mCapacity(0)
		{
			resize(aCapacity);
		}


		template<class I2 = I, class ALLOCATOR2 = ALLOCATOR, I2 DEFAULT_ALLOCATION2 = DEFAULT_ALLOCATION>
		array_list(const array_list<T, I2, ALLOCATOR2, DEFAULT_ALLOCATION2>& aOther) :
			mData(nullptr),
			mSize(0),
			mCapacity(0)
		{
			const size_t s = aOther.size();
			mData = static_cast<T*>(mAllocator.allocate(aCapacity * sizeof(T)));
			mSize = s;
			mCapacity = s;
			for(size_t i = 0; i < s; ++i) new(mData + i) T(aOther[i]);
		}

		template<class I2 = I, I2 DEFAULT_ALLOCATION2 = DEFAULT_ALLOCATION, class ENABLE = typename std::enable_if<std::is_move_constructible<ALLOCATOR>::value>::type>
		array_list(array_list<T, I2, ALLOCATOR, DEFAULT_ALLOCATION2>&& aOther) :
			mAllocator(std::move(aOther.mAllocator)),
			mData(aOther.mData),
			mSize(aOther.mSize),
			mCapacity(aOther.mCapacity)
		{
			aOther.mData = nullptr;
			aOther.mSize = 0;
			aOther.mCapacity = 0;
		}

		~array_list() {
			if(mData) {
				for(I i = 0; i < mCapacity; ++i) mData[i].~T();
				mAllocator.deallocate(mData);
				mSize = 0;
				mCapacity = 0;
				mData = nullptr;
			}
		}

		template<class I2 = I, class ALLOCATOR2 = ALLOCATOR, I2 DEFAULT_ALLOCATION2 = DEFAULT_ALLOCATION>
		array_list<T,I,ALLOCATOR,DEFAULT_ALLOCATION>& operator=(const array_list<T, I2, ALLOCATOR2, DEFAULT_ALLOCATION2>& aOther) {
			clear();
			const size_t s - aOther.size();
			reserve(s);
			for(size_t i = 0; i < s; ++i) mData[i] = aOther[i];
			mSize = s;
			return *this;
		}


		template<class I2 = I, I2 DEFAULT_ALLOCATION2 = DEFAULT_ALLOCATION, class ENABLE = typename std::enable_if<std::is_move_assignable<ALLOCATOR>::value>::type>
		array_list<T, I, ALLOCATOR, DEFAULT_ALLOCATION>& operator=(const array_list<T, I2, ALLOCATOR, DEFAULT_ALLOCATION2>& aOther) {
			std::swap(mAllocator, aOther.mAllocator);
			std::swap(mData, aOther.mData);
			size_t tmp = mSize;
			mSize = aOther.mSize;
			aOther.mSize = tmp;
			tmp = mCapacity;
			mCapacity = aOther.mCapacity;
			aOther.mCapacity = tmp;
			return *this;
		}

		inline T& push_back(const T& aValue) {
			if(mSize == mCapacity) resize(mCapacity == 0 ? DEFAULT_ALLOCATION : mCapacity * 2);
			return mData[mSize++] = aValue;
		}

		iterator erase(const const_iterator aPosition) {
			const size_t offset = aPosition - mData;
			--mSize;
			for(size_t i = offset; i < mSize; ++i) mData[i] = std::move(mData[i + 1]);
			return mData + offset;
		}

		iterator insert(const const_iterator aPosition, const T& aValue) {
			const size_t offset = aPosition - mData;
			if(mSize == mCapacity) resize(mCapacity == 0 ? DEFAULT_ALLOCATION : mCapacity * 2);
			for(size_t i = mSize; i > offset; --i) mData[i] = std::move(mData[i-1]);
			mData[offset] = aValue;
			++mSize;
			return mData + offset;
		}
	private:
		void resize(const I aCapacity) {
			T* const tmp = static_cast<T*>(mAllocator.allocate(aCapacity * sizeof(T)));
			for(I i = 0; i < mSize; ++i) new(tmp + i) T(std::move(mData[i]));
			for(I i = mSize; i < aCapacity; ++i) new(tmp + i) T();
			for(I i = 0; i < mCapacity; ++i) mData[i].~T();
			if(mData) mAllocator.deallocate(mData);
			mData = tmp;
			mCapacity = aCapacity;
		}
	};

	template<class T, size_t SIZE, class I = size_t>
	using static_array_list = array_list<T, I, static_allocator<SIZE * sizeof(T)>, SIZE>;

	template<class T, class I = size_t, I DEFAULT_ALLOCATION = 16>
	using heap_array_list = array_list<T, I, heap_allocator, DEFAULT_ALLOCATION>;
}

#endif