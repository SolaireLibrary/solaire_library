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

#ifndef SOLAIRE_PARALLEL_THREAD_POOL_HPP
#define SOLAIRE_PARALLEL_THREAD_POOL_HPP

#include "executor.hpp"
#include <condition_variable>
#include <atomic>

namespace solaire {
	class thread_pool : public executor {
	private:
		std::vector<std::thread> mWorkers;
		std::condition_variable mCondition;
		bool mExit;
		
		void worker_function(uint8_t);
	protected:
		// Inherited from executor
		
		void on_scheduled(const priority) override;
	public:
		thread_pool();
		thread_pool(const uint8_t);
		~thread_pool();
	};
}