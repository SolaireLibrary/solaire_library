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
	struct iterator_template {
		typedef void type;
		void forward(const int aOffset);
		void backward(const int aOffset);
		size_t get_offset(const iterator_template& aOther) const;
		void* get();
	};

	namespace implementation {
		template<class TEMPLATE, bool CONST>
		class iterator {
		public:
			typedef typename TEMPLATE::type type;
			typedef iterator<TEMPLATE, CONST> this_t;
			template<class T = TEMPLATE, bool C = CONST> friend class iterator;
		private:
			TEMPLATE mTemplate;
		public:
			iterator() : mTemplate() {}
			iterator(const TEMPLATE& aTemplate) : mTemplate(aTemplate) {}
			template<bool C = CONST> iterator(const iterator<TEMPLATE,C>& aOther) : mTemplate(aOther.mTemplate) {}

			inline const type* operator->() const				{ return const_cast<TEMPLATE*>(&mTemplate)->get(); }
			inline const type& operator*() const				{ return *const_cast<TEMPLATE*>(&mTemplate)->get(); }
			inline this_t& operator+=(const int aOffset)		{ mTemplate.forward(aOffset); return *this; }
			inline this_t& operator-=(const int aOffset)		{ mTemplate.forward(aOffset); return *this; }
			inline this_t& operator++()							{ mTemplate.forward(1);  return *this; }
			inline this_t& operator--()							{ mTemplate.backward(1);  return *this; }
			inline this_t operator++(int)						{ this_t tmp = *this; mTemplate.forward(1);  return tmp; }
			inline this_t operator--(int)						{ this_t tmp = *this; mTemplate.backward(1);  return tmp; }
			inline size_t operator-(const this_t& aOther) const	{ return mTemplate.get_offset(aOther.mTemplate); }
			inline this_t operator+(const int aOffset) const	{ this_t tmp = *this; tmp += aOffset;  return tmp; }
			inline this_t operator-(const int aOffset) const	{ this_t tmp = *this; tmp -= aOffset;  return tmp; }
			inline bool operator==(const this_t& aOther) const	{ return mTemplate.get_offset(aOther.mTemplate) == 0; }
			inline bool operator!=(const this_t& aOther) const	{ return mTemplate.get_offset(aOther.mTemplate) != 0; }
			inline bool operator<(const this_t& aOther) const	{ return mTemplate.get_offset(aOther.mTemplate) > 0; }
			inline bool operator>(const this_t& aOther) const	{ return mTemplate.get_offset(aOther.mTemplate) < 0; }
			inline bool operator<=(const this_t& aOther) const	{ return mTemplate.get_offset(aOther.mTemplate) >= 0; }
			inline bool operator>=(const this_t& aOther) const	{ return mTemplate.get_offset(aOther.mTemplate) <= 0; }

			template<bool C = CONST> inline typename std::enable_if<!C, type*>::type operator->() { return mTemplate.get(); }
			template<bool C = CONST> inline typename std::enable_if<!C, type&>::type operator*() { return *mTemplate.get(); }
		};
	}

	template<class TEMPLATE>
	using iterator = implementation::iterator<TEMPLATE, false>;

	template<class TEMPLATE>
	using const_iterator = implementation::iterator<TEMPLATE, true>;
}
#endif