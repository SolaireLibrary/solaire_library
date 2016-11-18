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

#ifndef SOLAIRE_CORE_ITERATOR_HPP
#define SOLAIRE_CORE_ITERATOR_HPP

namespace solaire {
	enum {
		ITERATOR_DATA_SIZE = 16
	};

	struct iterator_helper {
		void*(*dereference)(void*);
		void(*increment)(void*, int32_t);
		int32_t(*offset)(const void*, const void*);
	};

	template<class T>
	class iterator {
	private:
		uint8_t mData[ITERATOR_DATA_SIZE];
		const iterator_helper* mHelper;
	public:
		iterator() : mData(), mHelper(nullptr) {}
		iterator(const void* aData, const iterator_helper& aHelper) : mData(), mHelper(&aHelper) { memcpy(mData, aData, ITERATOR_DATA_SIZE); }

		inline T* operator->() { return static_cast<T*>(mHelper->dereference(mData)); }
		inline const T* operator->() const { return static_cast<T*>(mHelper->dereference(mData)); }
		inline T& operator*() { return *static_cast<T*>(mHelper->dereference(mData)); }
		inline const T& operator*() const { return *static_cast<T*>(mHelper->dereference(mData)); }
		inline int32_t operator-(const iterator<T>& aOther) const { return mHelper != aOther.mHelper ? 0 : mHelper->offset(mData, aOther.mData); }
		inline iterator<T>& operator+=(int32_t aOffset) { mHelper->increment(mData, aOffset); return *this; }
		inline iterator<T>& operator-=(int32_t aOffset) { mHelper->increment(mData, -aOffset); return *this; }
		inline iterator<T>& operator++() { mHelper->increment(mData, 1); return *this; }
		inline iterator<T>& operator--() { mHelper->increment(mData, -1); return *this; }
		inline iterator<T> operator++(int) { iterator<T> tmp = *this; mHelper->increment(mData, 1); return tmp; }
		inline iterator<T> operator--(int) { iterator<T> tmp = *this; mHelper->increment(mData, -1); return tmp; }
		inline iterator<T> operator+(int32_t aOffset) const { iterator<T> tmp = *this; mHelper->increment(tmp.mData, aOffset); return tmp; }
		inline iterator<T> operator-(int32_t aOffset) const { iterator<T> tmp = *this; mHelper->increment(tmp.mData, aOffset); return tmp; }
		inline bool operator==(const iterator<T>& aOther) const { return mHelper != aOther.mHelper ? false : mHelper->offset(mData, aOther.mData) == 0; }
		inline bool operator!=(const iterator<T>& aOther) const { return mHelper != aOther.mHelper ? true : mHelper->offset(mData, aOther.mData) != 0; }
		inline bool operator<(const iterator<T>& aOther) const { return mHelper != aOther.mHelper ? false : mHelper->offset(mData, aOther.mData) > 0; }
		inline bool operator>(const iterator<T>& aOther) const { return mHelper != aOther.mHelper ? false : mHelper->offset(mData, aOther.mData) < 0; }
		inline bool operator<=(const iterator<T>& aOther) const { return mHelper != aOther.mHelper ? false : mHelper->offset(mData, aOther.mData) >= 0; }
		inline bool operator>=(const iterator<T>& aOther) const { return mHelper != aOther.mHelper ? false : mHelper->offset(mData, aOther.mData) <= 0; }
	};

	//template<class TEMPLATE, bool CONST = ! std::is_same<typename TEMPLATE::type, std::remove_const<typename TEMPLATE::type>::type>::value>
	//class iterator {
	//public:
	//	typedef typename TEMPLATE::type type;
	//private:
	//	TEMPLATE mTemplate;
	//public:
	//	iterator() : mTemplate() {}
	//	template<bool CONST2 = CONST> iterator(const iterator<TEMPLATE, CONST2>& aOther) : mTemplate(aOther.mTemplate) {}
	//	iterator(const TEMPLATE& aTemplate) : mTemplate(aTemplate) {}
	//	inline const type* operator->() const { return const_cast<TEMPLATE*>(&mTemplate)->get(); }
	//	inline const type& operator*() const { return *const_cast<TEMPLATE*>(&mTemplate)->get(); }
	//	inline iterator<TEMPLATE,CONST>& operator+=(const int aOffset) { mTemplate.forward(aOffset); *this; }
	//	inline iterator<TEMPLATE,CONST>& operator-=(const int aOffset) { mTemplate.forward(aOffset); *this; }
	//	inline iterator<TEMPLATE,CONST>& operator++() { mTemplate.forward(1);  return *this; }
	//	inline iterator<TEMPLATE,CONST>& operator--() { mTemplate.backward(1);  return *this; }
	//	inline iterator<TEMPLATE,CONST> operator++(int) { iterator<TEMPLATE,CONST> tmp = *this; mTemplate.forward(1);  return tmp; }
	//	inline iterator<TEMPLATE,CONST> operator--(int) { iterator<TEMPLATE,CONST> tmp = *this; mTemplate.backward(1);  return tmp; }
	//	inline int operator-(const iterator<TEMPLATE,CONST>& aOther) const { return mTemplate.get_offset(aOther.mTemplate); }
	//	inline iterator<TEMPLATE,CONST> operator+(const int aOffset) const { iterator<TEMPLATE,CONST> tmp = *this; tmp += aOffset;  return tmp; }
	//	inline iterator<TEMPLATE,CONST> operator-(const int aOffset) const { iterator<TEMPLATE,CONST> tmp = *this; tmp -= aOffset;  return tmp; }
	//	
	//	template<bool C = CONST> inline typename std::enable_if<!C, type*>::type operator->() { return mTemplate.get(); }
	//	template<bool C = CONST> inline typename std::enable_if<!C, type&>::type operator*() { return *mTemplate.get(); }
	//};
}