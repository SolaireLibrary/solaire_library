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
		virtual ~allocator() {}
		virtual void* allocate(size_t) = 0;
		virtual void deallocate(void*) = 0;
	};

	template<size_t SIZE>
	class static_allocator : public allocator {
	private:
		uint8_t mData[SIZE];
		bool mAllocated;
	public:
		constexpr static_allocator() : mData(), mAllocated(false) {}
		inline void deallocate(void* aData) override { if (aData == mData) mAllocated = false; }

		void* allocate(size_t aSize) override {
			if(mAllocated) throw std::runtime_error("solaire::static_allocator::allocate : Memory must be deallocated before it can be allocated again");
			if(aSize > SIZE) throw std::runtime_error("solaire::static_allocator::allocate : Allocation size is too large");
			mAllocated = true;
			return mData;
		}
	};

	class heap_allocator : public allocator {
	public:
		constexpr heap_allocator() {}
		inline void deallocate(void* aData) override { operator delete(aData); }

		inline void* allocate(size_t aSize) override {
			void* const tmp = operator new(aSize);
			if (tmp == nullptr) throw std::runtime_error("solaire::heap_allocator::allocate : Failed to allocate memory");
			return tmp;
		}
	};
}

#endif