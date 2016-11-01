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

#ifndef SOLAIRE_PARALLEL_EXECUTOR_HPP
#define SOLAIRE_PARALLEL_EXECUTOR_HPP

#include <cstdint>
#include <memory>
#include <deque>
#include <vector>
#include <functional>
#include <mutex>
#include <future>

namespace solaire {
	class executor {
	public:
		enum priority : uint8_t {
			PRIORITY_HIGH,
			PRIORITY_LOW = 4,
			PRIORITY_MEDIUM = PRIORITY_LOW / 2,
			PRIORITY_INVALID = PRIORITY_LOW + 1
		};
		enum id : uint64_t {};
	protected:
		struct task {
			id id;
			std::shared_ptr<void> promise;
			std::function<void(void)> function;
		};

		std::vector<task> mCompletedTasks;
		std::deque<task> mTasks[PRIORITY_LOW + 1];
		std::atomic<uint8_t> mHighPriority;
		mutable std::mutex mTasksLock;
		
		virtual void on_scheduled(const priority) = 0;
	private:
		uint64_t mID;

		id schedule_internal(const priority, std::shared_ptr<void>, const std::function<void(void)>&);
		task& find(const id, priority&);
	public:
		executor();
		virtual ~executor();

		void clear_completed_tasks();
		size_t get_scheduled_count() const;
		void change_priority(const id, const priority);
		void cancel(const id);
		priority get_priority(const id) const;
		
		template<class R, class F, class... PARAMS>
		typename std::enable_if<! std::is_same<R,void>::value, std::future<R>>::type schedule(id& aID, const priority aPriority, F aFunction, PARAMS... aParams) {
			std::shared_ptr<std::promise<R>> fut(new std::promise<R>());
			aID = schedule_internal(aPriority, fut,[=]()->void {
				try{
					fut->set_value(aFunction(aParams...));
				}catch(std::exception&) {
					fut->set_exception(std::current_exception());
				}
			});
			return fut->get_future();
		}
		
		template<class R, class F,class... PARAMS>
		typename std::enable_if<std::is_same<R,void>::value, std::future<R>>::type schedule(id& aID, const priority aPriority, F aFunction, PARAMS... aParams) {
			std::shared_ptr<std::promise<R>> fut(new std::promise<R>());
			aID = schedule_internal(aPriority, fut, [=]()->void {
				try{
					aFunction(aParams...);
					fut->set_value();
				}catch(std::exception&) {
					fut->set_exception(std::current_exception());
				}
			});
			return fut->get_future();
		}

		template<class R, class F, class... PARAMS>
		inline std::future<R> schedule(const priority aPriority, F aFunction, PARAMS... aParams) {
			id i;
			return schedule<R, F, PARAMS...>(i, aPriority, aFunction, aParams...);
		}

		template<class R, class F, class... PARAMS>
		inline std::future<R> schedule(F aFunction, PARAMS... aParams) {
			id i;
			return schedule<R, F, PARAMS...>(i, PRIORITY_MEDIUM, aFunction, aParams...);
		}

		template<class R, class F, class... PARAMS>
		inline std::future<R> schedule(id& aID, F aFunction, PARAMS... aParams) {
			return schedule<R, F, PARAMS...>(aID, PRIORITY_MEDIUM, aFunction, aParams...);
		}
	};
}