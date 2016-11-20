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
	class allocator {
	public:
		virtual ~allocator() {}
		virtual void* allocate(size_t) = 0;
		virtual void deallocate() = 0;
		virtual void* get() const = 0;
		virtual size_t size() const = 0;
	};

	template<size_t SIZE>
	class static_allocator : public allocator {
	private:
		uint8_t mData[SIZE];
		bool mAllocated;

		static_allocator(const static_allocator&) = delete;
		static_allocator<SIZE>& operator=(const static_allocator&) = delete;
		static_allocator(static_allocator&&) = delete;
		static_allocator<SIZE>& operator=(static_allocator&&) = delete;
	public:

		constexpr static_allocator() : 
			mData(),
			mAllocated(false)
		{}

		inline void deallocate() {
			if(! mAllocated) throw std::runtime_error("solaire::static_allocator::deallocate : No allocation");
			mAllocated = false;
		}

		inline void* allocate(size_t aSize) {
			if(mAllocated) throw std::runtime_error("solaire::static_allocator::allocate : Cannot allocate to an already allocated allocator");
			if(aSize > SIZE) throw std::runtime_error("solaire::static_allocator::allocate : Allocation size is too large");
			mAllocated = true;
			return mData;
		}

		inline void* get() const override { 
			return mData; 
		}

		inline size_t size() const override { 
			return SIZE;
		}
	};

	class heap_allocator : public allocator {
	private:
		void* mData;
		size_t mSize;

		heap_allocator(const heap_allocator&) = delete;
		heap_allocator& operator=(const heap_allocator&) = delete;
	public:
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
			if(mData) deallocate();
		}

		inline heap_allocator& operator=(heap_allocator&& aOther) {
			std::swap(mData, aOther.mData);
			std::swap(mSize, aOther.mSize);
			return *this;
		}

		inline void deallocate() override { 
			operator delete(mData); 
			mData = nullptr;
			mSize = 0;
		}

		inline void* allocate(size_t aSize) override {
			if(mData) throw std::runtime_error("solaire::static_allocator::allocate : Cannot allocate to an already allocated allocator");
			mData = operator new(aSize);
			mSize = aSize;
			if(mData == nullptr) throw std::runtime_error("solaire::heap_allocator::allocate : Failed to allocate memory");
			return mData;
		}
		
		inline void* get() const override { 
			return mData; 
		}

		inline size_t size() const override { 
			return mSize; 
		}
	};
}

#endif