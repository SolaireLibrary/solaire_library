#ifndef SOLAIRE_CORE_TYPELIST_HPP
#define SOLAIRE_CORE_TYPELIST_HPP

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
#include "core.hpp"

namespace solaire {
	struct typelist_end{};

	namespace implementation {
		template<class LIST>
		constexpr uint32_t typelist_length(const uint32_t aSum = 0) {
			return typelist_length<typename LIST::tail_t>(aSum + 1);
		}

		template<>
		constexpr uint32_t typelist_length<typelist_end>(const uint32_t aSum) {
			return aSum;
		}

		template<class LIST, const uint32_t INDEX>
		struct typelist_access {
			typedef typename typelist_access<typename LIST::tail_t, INDEX - 1>::type type;
		};

		template<class LIST>
		struct typelist_access<LIST, 0> {
			typedef typename LIST::head_t type;
		};

		template<class LIST, const uint32_t INDEX>
		struct value_at_helper{
			static const typename typelist_access<LIST, INDEX>::type& value_at(const LIST& aList) {
				return value_at_helper<typename LIST::tail_t, INDEX - 1>::value_at(aList.tail);
			}
		};

		template<class LIST>
		struct value_at_helper<LIST, 0> {
			static const typename LIST::head_t& value_at(const LIST& aList) {
				return aList.head;
			}
		};
	}

	template<class A, class B = typelist_end>
	struct typelist {
		typedef A head_t;
		typedef B tail_t;

		head_t head;
		tail_t tail;

		enum {
			LENGTH = implementation::typelist_length<typelist<A,B>>()
		};

		template<const uint32_t INDEX>
		using type_at = typename implementation::typelist_access<typelist<A, B>, INDEX>::type;

		template<const uint32_t INDEX>
		type_at<INDEX>& value_at() {
			return const_cast<type_at<INDEX>&>(const_cast<const typelist<A, B>*>(this)->value_at<INDEX>());
		}

		template<const uint32_t INDEX>
		const type_at<INDEX>& value_at() const {
			return implementation::value_at_helper<typelist<A, B>, INDEX>::value_at(*this);
		}

		typelist() :
			head(),
			tail()
		{}

		template<class A, class ...PARAMS>
		typelist(const A aFirst, const PARAMS... aRest) :
			head(aFirst),
			tail(aRest...)
		{
			static_assert(sizeof...(PARAMS) == LENGTH - 1, "solaire::typelist : Parameter count must match length of list");
		}
	};

	namespace implementation {
		template<class A, class ...PARAMS>
		struct auto_typelist {
			typedef typename typelist<A, typename auto_typelist<PARAMS...>::type> type;
		};

		template<class A>
		struct auto_typelist<A> {
			typedef typelist<A> type;
		};
	}

	template<class ...PARAMS>
	using auto_typelist = typename implementation::auto_typelist<PARAMS...>::type;
}

#endif