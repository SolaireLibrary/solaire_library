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

#ifndef SOLAIRE_PARALLEL_TASK_HPP
#define SOLAIRE_PARALLEL_TASK_HPP

#include <vector>
#include <setjmp.h>

namespace solaire {
	class task {
	private:
		enum state {
			TASK_INITIALISED,
			TASK_EXECUTING,
			TASK_PAUSED,
			TASK_COMPLETE
		};
		jmp_buf mBuffer;
		//std::deque<task*>* mTasks;
		task* mOther;
		int mJumpParam;
		state mState;
		void run_internal();
		void resume();
	protected:
		task();
		void pause();
	public:
		static void execute(std::vector<task*>&);
		virtual void run() = 0;
	};

	////

	void task::execute(std::vector<task*>& aTasks) {
		const size_t size = aTasks.size();
		for(size_t i = 0; i < size - 1; ++i) aTasks[i]->mOther = aTasks[i + 1];
		aTasks.back()->mOther = aTasks.front();
		aTasks.front()->run_internal();
	}

	void task::resume() {
		switch(mState) {
		case TASK_PAUSED:
			mState = mState = TASK_EXECUTING;
			//longjmp(mBuffer, mJumpParam++);
			break;
		case TASK_INITIALISED:
			run_internal();
			break;
		default:
			//throw std::runtime_error("solaire::task::next_task : Next task is in invalid state");
			break;
		}
	}

	task::task() :
		mOther(nullptr),
		mState(TASK_INITIALISED)
	{}

	void task::run_internal() {
		mState = TASK_EXECUTING;
		run();
		mState = TASK_COMPLETE;
		if(mOther->mState == TASK_INITIALISED) mOther->run_internal();
	}

	void task::pause() {
		if(mOther) {
			mState = TASK_PAUSED;
			int r = 0;
			//r = setjmp(mBuffer);
			if(r == 0) {
				mOther->resume();
			}
		}
	}

	////

	class print_task : public task {
	private:
		char mC;
	public:
		print_task(char aC) :
			mC(aC)
		{}

		// Inherited from task
		void run() override {
			for(uint32_t i = 0; i < 5; ++i) {
				std::cout << mC << std::endl;
				pause();
			}
		}
	};
}