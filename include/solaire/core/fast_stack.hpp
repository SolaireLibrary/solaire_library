#ifndef SOLAIRE_CORE_FAST_STACK_HPP
#define SOLAIRE_CORE_FAST_STACK_HPP

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

	template<const class T, const uint32_t CAPACITY, class INDEX_TYPE = uint8_t>
	class fast_stack {
	private:
		T mValues[CAPACITY];
		INDEX_TYPE mSize;
	public:
		fast_stack() throw() :
			mSize(0)
		{}

		inline T* begin() throw() {
			return mValues;
		}

		inline T* end() throw() {
			return mValues + size();
		}

		inline const T* begin() const throw() {
			return mValues;
		}

		inline const T* end() const throw() {
			return mValues + size();
		}

		inline T* find(const T& aValue) throw() {
			return const_cast<T*>(const_cast<const fast_stack<T, CAPACITY, INDEX_TYPE>*>(this)->find(aValue));
		}

		const T* find(const T& aValue) const throw() {
			for(INDEX_TYPE i = 0; i < mSize; ++i) {;
				if(mValues[i] == aValue) return mValues + i;
			}
			return end();
		}

		bool push(const T& aValue) throw() {
			if(mSize >= CAPACITY) return false;
			mValues[mSize++] = aValue;
			return true;
		}

		inline T pop() throw() {
			return mValues[--mSize];
		}

		bool fast_erase(const T* const aValue) throw() {
			const INDEX_TYPE offset = aValue - mValues;
			if(offset >= mSize) return false;
			mValues[offset] = pop();
			return true;
		}

		bool erase(const T* const aValue) throw() {
			const INDEX_TYPE offset = aValue - mValues;
			if(offset >= SIZE) return false;
			--mSize;
			for(INDEX_TYPE i = offset; i < mSize; ++i) {
				mValues[i] = mValues[i + 1];
			}
			return true;
		}

		inline void clear() throw() {
			mSize = 0;
		}

		inline INDEX_TYPE size() const throw() {
			return mSize;
		}

		inline bool empty() const throw() {
			return mSize == 0;
		}

		inline T& operator[](const INDEX_TYPE aIndex) throw() {
			return mValues[aIndex];
		}

		inline const T& operator[](const INDEX_TYPE aIndex) const throw() {
			return mValues[aIndex];
		}
	};
}

#endif