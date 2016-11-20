#ifndef SOLAIRE_CORE_ALLOCATOR_HPP
#define SOLAIRE_CORE_ALLOCATOR_HPP

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

#include <cstdint>

namespace solaire {
	struct allocator {
		static allocator allocate(size_t);
		static void deallocate(allocator&);

		void* get() const;
		size_t size() const;
	};

	template<size_t SIZE>
	class static_allocator {
	private:
		uint8_t mData[SIZE];

		static_allocator(const static_allocator&) = delete;
		static_allocator<SIZE>& operator=(const static_allocator&) = delete;
		static_allocator(static_allocator&&) = delete;
		static_allocator<SIZE>& operator=(static_allocator&&) = delete;
	public:
		inline void* get() const { return mData; }
		inline size_t size() const { return SIZE; }

		constexpr static_allocator() : 
			mData() 
		{}

		inline static void deallocate(allocator& aData) {

		}

		inline static void* allocate(size_t aSize) {
			if(aSize > SIZE) throw std::runtime_error("solaire::static_allocator::allocate : Allocation size is too large");
			return static_allocator();
		}
	};

	class heap_allocator {
	private:
		void* mData;
		size_t mSize;

		heap_allocator(const heap_allocator&) = delete;
		heap_allocator& operator=(const heap_allocator&) = delete;
	public:
		inline void* get() const { return mData; }
		inline size_t size() const { return mSize; }

		constexpr heap_allocator() : 
			mData(nullptr), 
			mSize(0) 
		{}
		
		heap_allocator(heap_allocator&& aOther) : 
			mData(aOther.mData), 
			mSize(aOther.mSize) 
		{ 
			aOther.mData = nullptr; 
			aOther.mSize = 0; 
		}

		~heap_allocator() {
			if (mData) deallocate(*this);
		}

		heap_allocator& operator=(heap_allocator&& aOther) {
			std::swap(mData, aOther.mData);
			std::swap(mSize, aOther.mSize);
			return *this;
		}

		inline static void deallocate(heap_allocator& aData) { 
			operator delete(aData.mData); 
			aData.mData = nullptr;
			aData.mSize = 0;
		}

		inline static heap_allocator allocate(size_t aSize) {
			heap_allocator tmp;
			tmp.mData = operator new(aSize);
			tmp.mSize = aSize;
			if(tmp.mData == nullptr) throw std::runtime_error("solaire::heap_allocator::allocate : Failed to allocate memory");
			return tmp;
		}
	};
}

#endif