#ifndef SOLAIRE_MEMORY_POOL_HPP
#define SOLAIRE_MEMORY_POOL_HPP

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
	class memory_pool {
	private:
		typedef std::pair<void*, size_t> block;
		std::vector<block> mRealBlocks;
		std::vector<block> mOpenBlocks;
		std::vector<block> mClosedBlocks;

		memory_pool(const memory_pool&) = delete;
		memory_pool& operator=(const memory_pool&) = delete;
	public:
		memory_pool();
		memory_pool(memory_pool&&);
		~memory_pool();
		memory_pool& operator=(memory_pool&&);

		void* allocate(size_t);
		void deallocate();
	};

	class memory_pool_allocator : public allocator {
	private:
		memory_pool* mAllocator;
		void* mData;
		size_t mSize;

		memory_pool_allocator(const memory_pool_allocator&) = delete;
		memory_pool_allocator& operator=(const memory_pool_allocator&) = delete;
	public:
		memory_pool_allocator();
		memory_pool_allocator(memory_pool&);
		memory_pool_allocator(memory_pool_allocator&&);
		~memory_pool_allocator();
		memory_pool_allocator& operator=(memory_pool_allocator&&);

		// Inherited from allocator
		void deallocate() override;
		void* allocate(size_t aSize) override;
		void* get() const override;
		size_t size() const override;
	};
}

#endif