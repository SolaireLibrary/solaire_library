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

#ifndef SOLAIRE_RW_MUTEX_HPP
#define SOLAIRE_RW_MUTEX_HPP

namespace solaire {
	template class<MUTEX>
	class rw_mutex {
	private:
		MUTEX mLock;
		std::atomic_uint32_t mReaders;
		std::atomic_uint8_t mWrite;
	public:
		rw_mutex() {
			mReaders = 0;
			mWrite = 0;
		}

		~rw_mutex() {
			
		}
		
		void read_lock() {
			if(mReaders > 0 || ! mWrite) { 
				++mReaders;
			}else {
				mLock.lock();
				mReaders = 1;
				mLock.unlock();
			}
		}
		
		void read_unlock() {
			if(mReaders == 0) throw std::runtime_error("solaire::rw_mutex::read_unlock : Mutex is not read locked");
			--mReaders;
		}
		
		bool try_read_lock() {
			if(mReaders >  0 || ! mWrite) { 
				++mReaders;
			}else {
				if(! mLock.try_lock()) return false;
				mReaders = 1;
				mLock.unlock();
			}
			return true;
		}
		
		void write_lock() {
			//! \todo Optimise
			while(mReaders > 0){}
			mLock.lock();
			mWrite = 1;
		}
		
		void write_unlock() {
			mWrite = 0;
			mLock.unlock();
		}
		
		bool try_write_lock() {
			if(mReaders == 0 && mLock.try_lock()) {
				mWrite = 1;
				return true;
			}else {
				return false;
			}
		}
		
		void write_promotion() {
			//! \todo Optimise
			if(mReaders == 0) throw std::runtime_error("solaire::rw_mutex : Mutex is not read locked");
			while(mReaders != 1){}
			mLock.lock();
			--mReaders;
			mWrite = 1;
		}
		
		void write_demotion() {
			++mReaders;
			mWrite = 0;
			mLock.unlock();
		}
	};

	template<class MUTEX> 
	class write_lock_guard {
	private:
		rw_mutex<MUTEX>& mLock;
	public:
		write_lock_guard(rw_mutex<MUTEX>& aLock) :
			mLock(aLock)
		{
			mLock.write_lock();
		}
		
		~write_lock_guard() {
			mLock.write_unlock();
		}
	};

	template<class MUTEX> 
	class read_lock_guard {
	private:
		rw_mutex<MUTEX>& mLock;
		bool mPromoted;
	public:
		write_lock_guard(rw_mutex<MUTEX>& aLock) :
			mLock(aLock),
			mPromoted(false)
		{
			mLock.read_lock();
		}
		
		~write_lock_guard() {
			if(mPromoted) mLock.write_demotion();
			mLock.read_unlock();
		}
		
		void write_promotion() {
			mLock.write_promotion();
			mPromoted = true;
		}
		
		void write_demotion() {
			mLock.write_demotion();
			mPromoted = false;
		}
	};
}