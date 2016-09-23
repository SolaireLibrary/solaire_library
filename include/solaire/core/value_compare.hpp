#ifndef SOLAIRE_CORE_VALUE_COMPARE_HPP
#define SOLAIRE_CORE_VALUE_COMPARE_HPP

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

#include "core.hpp"

namespace solaire {

	/*!
		\param Compare two values.
		\tparam T The type of the values being compared.
		\tparam A The first value.
		\tparam B The second value.
		\version 2.0
	*/
	template<class T, const T A, const T B>
	struct value_compare {
		enum {
			LESS_THAN = A < B,							//!< 1 If A < B else 0.
			GREATER_THAN = A > B,						//!< 1 If A > B else 0.
			EQUAL = A == B,								//!< 1 If A == B else 0.
			NOT_EQUAL = A != B,							//!< 1 If A != B else 0.
			LESS_THAN_EQUAL = LESS_THAN || EQUAL,		//!< 1 If A <= B else 0.
			GREATER_THAN_EQUAL = GREATER_THAN || EQUAL	//!< 1 If A >= B else 0.
		};
		static constexpr T LOW_VALUE = A < B ? A : B;	//!< Thow lesser of A and B.
		static constexpr T HIGH_VALUE = A < B ? B : A;	//!< Thow greater of A and B.
		static constexpr T FIRST = A;					//!< A.
		static constexpr T SECOND = B;					//!< B.
	};
}

#endif