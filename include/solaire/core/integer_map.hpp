#ifndef SOLAIRE_CORE_INTEGER_MAP_HPP
#define SOLAIRE_CORE_INTEGER_MAP_HPP

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

#include <algorithm>
#include <vector>

namespace solaire {

	template<class K, class T>
	class integer_map {
	public:
		typedef K key;
		typedef T type;
		typedef std::pair<const key,type> element;
		typedef element* iterator;
		typedef const element* const_iterator;
	private:
		struct internal_element {
			K first;
			T second;
			constexpr internal_element() : first(), second()			{}
			constexpr internal_element(K k, T v) : first(k), second(v)	{}
			constexpr bool operator==(const internal_element& o) const	{ return first == o.first; }
			constexpr bool operator!=(const internal_element& o) const	{ return first != o.first; }
			constexpr bool operator<(const internal_element& o) const	{ return first < o.first; }
			constexpr bool operator>(const internal_element& o) const	{ return first > o.first; }
			constexpr bool operator<=(const internal_element& o) const	{ return first <= o.first; }
			constexpr bool operator>=(const internal_element& o) const	{ return first >= o.first; }
		};
		std::vector<internal_element> mElements;
	public:
		inline T& operator[](const T aKey) { return find(aKey)->second; }
		inline const T& operator[](const T aKey) const { return find(aKey)->second; }
		inline size_t size() const { return mElements.size(); }
		inline iterator begin() { return reinterpret_cast<iterator>(&mElements[0]); }
		inline iterator end() { return begin() + mElements.size(); }
		inline const_iterator begin() const { return reinterpret_cast<const_iterator>(&mElements[0]); }
		inline const_iterator end() const { return begin() + mElements.size(); }

		iterator emplace(const T aKey, const T& aValue) {
			iterator p = end();
			if(mElements.empty()) {
				PUSH_ELEMENT:
				mElements.push_back(internal_element(aKey, aValue));
				p = reinterpret_cast<iterator>(&mElements.back());
			}else {
				p = find(aKey);
				if(p != end()) {
					p->second = aValue;
				}else {
					const auto e = mElements.end();
					for(auto i = mElements.begin(); i != e; ++i) {
						if(i->first > aKey) {
							const size_t o = e - i;
							mElements.insert(i, internal_element(aKey, aValue));
							p = begin() + o;
							break;
						}
					}
					if(p == end()) goto PUSH_ELEMENT;
				}
			}
			return p;
		}

		iterator find(const T aKey) {
			return const_cast<iterator>(const_cast<const integer_map<K,T>*>(this)->find(aKey));
		}

		const_iterator find(const T aKey) const {
			//! \todo Optimise
			auto i = std::lower_bound(mElements.begin(), mElements.end(), internal_element(aKey, T()));
			if(i != mElements.end() && !(aKey < i->first)) return reinterpret_cast<const_iterator>(&*i);
			else return end();
		}
	};
}

#endif