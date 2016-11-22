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

#include "solaire/core/memory_pool.hpp"

namespace solaire {


	// memory_pool_allocator

	memory_pool_allocator::memory_pool_allocator() :
		mAllocator(nullptr),
		mData(nullptr),
		mSize(0)
	{}

	memory_pool_allocator::memory_pool_allocator(memory_pool& aPool) :
		mAllocator(&aPool),
		mData(nullptr),
		mSize(0)
	{}

	memory_pool_allocator::memory_pool_allocator(memory_pool_allocator&& aOther) :
		mAllocator(aOther.mAllocator),
		mData(aOther.mData),
		mSize(aOther.mSize)
	{}

	memory_pool_allocator::~memory_pool_allocator() {
		if(mData) deallocate();
	}

	memory_pool_allocator& memory_pool_allocator::operator=(memory_pool_allocator&& aOther) {
		std::swap(mAllocator, aOther.mAllocator);
		std::swap(mData, aOther.mData);
		std::swap(mSize, aOther.mSize);
		return *this;
	}

	void memory_pool_allocator::deallocate() {
		if(! mData) throw std::runtime_error("solaire::memory_pool_allocator::deallocate : Not allocated");
		mAllocator->deallocate(mData);
		mData = nullptr;
		mData = 0;
	}

	void* memory_pool_allocator::allocate(size_t aSize) {
		if(! mAllocator) throw std::runtime_error("solaire::memory_pool_allocator::allocate : No memory pool assigned");
		if(mData) throw std::runtime_error("solaire::memory_pool_allocator::allocate : Already allocated");
		mData = mAllocator->allocate(aSize);
		mSize = aSize;
	}

	void* memory_pool_allocator::get() const {
		return mData;
	}

	size_t memory_pool_allocator::size() const {
		return mSize;
	}

}