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

	// memory_pool

	memory_pool::memory_pool() {

	}

	memory_pool::memory_pool(memory_pool&& aOther) {
		operator=(std::move(aOther));
	}

	memory_pool::~memory_pool() {
		mOpenBlocks.clear();
		mClosedBlocks.clear();
		for(const block& i : mRealBlocks) operator delete(i.first);
		mRealBlocks.clear();
	}

	memory_pool& memory_pool::operator=(memory_pool&& aOther) {
		std::swap(mRealBlocks, aOther.mRealBlocks);
		std::swap(mOpenBlocks, aOther.mOpenBlocks);
		std::swap(mClosedBlocks, aOther.mClosedBlocks);
		return *this;
	}

	void* memory_pool::allocate(size_t aSize) {
		// Check for an open block
		const auto end = mOpenBlocks.end();
		for(auto i = mOpenBlocks.begin(); i != end; ++i) if(i->second <= aSize) {
			block tmp = *i;
			mOpenBlocks.erase(i);
			if(tmp.second > aSize) {
				const size_t dif = tmp.second - aSize;
				tmp.second -= dif;
				mClosedBlocks.push_back({static_cast<uint8_t*>(tmp.first) + tmp.second, dif });
			}
			mClosedBlocks.push_back(tmp);
			return tmp.first;
		}

		// Allocate a new block
		const block tmp(operator new(aSize), aSize);
		if(! tmp.first) throw std::runtime_error("solaire::memory_pool::allocate : Failed to allocate memory");
		mRealBlocks.push_back(tmp);
		mClosedBlocks.push_back(tmp);
		return tmp.first;
	}

	void memory_pool::deallocate(void* aData) {
		// Check for closed block
		auto end = mClosedBlocks.end();
		for(auto i = mClosedBlocks.begin(); i != end; ++i) {
			if(i->first == aData) {
				const block tmp = *i;
				mClosedBlocks.erase(i);
				end = mOpenBlocks.end();
				// Check for block that follows this one
				const void* const next = static_cast<uint8_t*>(tmp.first) + tmp.second;
				for(i = mOpenBlocks.begin(); i != end; ++i)if (i->first == next) {
					i->first = tmp.first;
					i->second += tmp.second;
					return;
				}
				// Check for block that precedes this one
				for(i = mOpenBlocks.begin(); i != end; ++i) if(static_cast<uint8_t*>(i->first) + i->second == tmp.first) {
					i->second += tmp.second;
					return;
				}
				mOpenBlocks.push_back(tmp);
				return;
			}
		}
		
		throw std::runtime_error("solaire::memory_pool::deallocate : Memory not allocated from this allocator");
	}

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