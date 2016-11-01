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

#include "solaire/parallel/thread_pool.hpp"

// thread_pool

thread_pool::thread_pool() :
	mExit(false)
{}

thread_pool::thread_pool(const uint8_t aWorkers) :
	mExit(false)
{
	for(uint8_t i = 0; i < aWorkers; ++i) mWorkers.push_back(std::thread(&thread_pool::worker_function, this, i));
}

thread_pool::~thread_pool() {
	mExit = true;
	mCondition.notify_all();
	for(std::thread& i : mWorkers) i.join();
}

void thread_pool::on_scheduled(const priority aPriority) {
	mCondition.notify_one();
}

void thread_pool::worker_function(uint8_t aID) {
	const auto wait_for_notification = [this]()->bool {
		if(mExit) return false;
		std::unique_lock<std::mutex> lock(mTasksLock);
		mCondition.wait(lock);
		if(mExit) return false;
		return true;
	};
	
	const auto get_task = [this](task& aTask)->bool{
		if(mHighPriority == PRIORITY_INVALID) return false;
		std::lock_guard<std::mutex> lock(mTasksLock);
		do {
			if(mTasks[mHighPriority].empty()) {
				if(mHighPriority < PRIORITY_LOW) ++mHighPriority;
			}else {
				aTask = std::move(mTasks[mHighPriority].front());
				mTasks[mHighPriority].pop_front();
				return true;
			}
		}while(mHighPriority < PRIORITY_LOW);
		mHighPriority = PRIORITY_INVALID;
		return false;
	};

	task task;
	while(wait_for_notification()) {
		while(get_task(task)) {
			task.function();
			std::lock_guard<std::mutex> lock(mTasksLock);
			mCompletedTasks.push_back(task);
		}
	}
}