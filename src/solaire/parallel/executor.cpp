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

#include "solaire/parallel/executor.hpp"

executor::executor() :
	mHighPriority(PRIORITY_INVALID),
	mID(0)
{}

executor::~executor() {
	for(auto& i : mTasks) i.clear();
	mCompletedTasks.clear();
}

executor::task& executor::find(const id aID, priority& aPriority) {
	for(uint8_t i = 0; i <= PRIORITY_LOW; ++i) {
		for(task& j : mTasks[i]) if (j.id == aID) {
			aPriority = static_cast<priority>(i);
			return j;
		}
	}
	for(task& i : mCompletedTasks)if (i.id == aID) {
		aPriority = PRIORITY_INVALID;
		return i;
	}
	throw std::runtime_error("solaire::executor::find : Invalid task ID");
}

executor::id executor::schedule_internal(const priority aPriority, std::shared_ptr<void> aFuture, const std::function<void(void)>& aTask) {
	if(aPriority > PRIORITY_LOW) throw std::runtime_error("solaire::thread_pool::schedule_internal : Invalid priority");
	mTasksLock.lock();
	mHighPriority = std::min<uint8_t>(mHighPriority, aPriority);
	task tmp;
	tmp.id = static_cast<id>(mID++);
	tmp.promise = aFuture;
	tmp.function = aTask;
	mTasks[aPriority].push_back(tmp);
	mTasksLock.unlock();
	on_scheduled(aPriority);
	return tmp.id;
}

size_t executor::get_scheduled_count() const {
	if(mHighPriority == PRIORITY_INVALID) return 0;
	size_t tmp = 0;
	std::lock_guard<std::mutex> lock(mTasksLock);
	for(const auto& i : mTasks) tmp += i.size();
	return tmp;
}
void executor::clear_completed_tasks() {
	std::lock_guard<std::mutex> lock(mTasksLock);
	mCompletedTasks.clear();
}

void executor::change_priority(const id aID, const priority aPriority) {
	std::lock_guard<std::mutex> lock(mTasksLock);
	for(auto& i : mTasks) {
		for(auto j = i.begin(); j != i.end(); ++j) if (j->id == aID) {
			if(aPriority != PRIORITY_INVALID) {
				mHighPriority = std::min<uint8_t>(aPriority, mHighPriority);
				mTasks[aPriority].push_back(*j);
			}
			i.erase(j);
			return;
		}
	}
}

void executor::cancel(const id aID) {
	change_priority(aID, PRIORITY_INVALID);
}

executor::priority executor::get_priority(const id aID) const {
	priority p;
	const_cast<executor*>(this)->find(aID, p);
	return p;
}