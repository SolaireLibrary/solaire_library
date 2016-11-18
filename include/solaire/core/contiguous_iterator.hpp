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

#ifndef SOLAIRE_CORE_CONTIGUOUS_ITERATOR_HPP
#define SOLAIRE_CORE_CONTIGUOUS_ITERATOR_HPP

#include "iterator.hpp"

namespace solaire {
	template<class T>
	const iterator_helper& create_contiguous_iterator(T* aBase, void* aData) {
		struct data {
			T* ptr;
			int32_t offset;
		};

		static iterator_helper OBJ = {
			[](void* aObj)->void* {
			data& d = *static_cast<data*>(aObj);
			return d.ptr + d.offset;
		},
			[](void* aObj, int32_t aOffset)->void {
			data& d = *static_cast<data*>(aObj);
			d.offset += aOffset;
		},
			[](const void* aObj, const void* aOther)->int32_t {
			const data& a = *static_cast<const data*>(aObj);
			const data& b = *static_cast<const data*>(aOther);
			return a.offset - b.offset;
		}
		};
		data& d = *static_cast<data*>(aData);
		d.ptr = aBase;
		d.offset = 0;
		return OBJ;
	}

	//template<class T>
	//class contiguous_iterator_template {
	//private:
	//	T* mPtr;
	//public:
	//	typedef T type;
	//	contiguous_iterator_template() : mPtr(nullptr) {}
	//	contiguous_iterator_template(T* const aPtr) : mPtr(aPtr) {}
	//	inline void forward(const int aOffset) { mPtr += aOffset; }
	//	inline void backward(const int aOffset) { mPtr -= aOffset };
	//	inline int get_offset(const contiguous_iterator_template<T>& aOther) const { return mPtr - aOther.mPtr; }
	//	inline T* get() { return *mPtr; }
	//};

	//template<class T>
	//using contiguous_iterator = iterator<contiguous_iterator_template<T>, false>;

	//template<class T>
	//using const_contiguous_iterator = iterator<contiguous_iterator_template<T>, true>;
}
