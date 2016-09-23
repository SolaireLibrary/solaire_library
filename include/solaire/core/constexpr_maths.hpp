#ifndef SOLAIRE_CORE_CONSTEXPR_MATHS_HPP
#define SOLAIRE_CORE_CONSTEXPR_MATHS_HPP

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
#include <cmath>
#include "core.hpp"

namespace solaire {

	namespace implementation {
		// Helpers

		template<class T = int32_t>
		inline constexpr T sqrt_constexpr_internal(const T aValue, const T aCurrent, const T aPrevious) throw() {
			return aCurrent == aPrevious ? aCurrent : sqrt_constexpr_internal<T>(aValue, 0.5 * (aCurrent + aValue / aCurrent), aPrevious);
		}
	}

	// Base functions

	template<class T = int32_t>
	inline constexpr T sqrt_constexpr(const T aValue) throw() {
		return aValue >= 0 && aValue < std::numeric_limits<double>::infinity() ? implementation::sqrt_constexpr_internal<T>(aValue, aValue, static_cast<T>(0)) : std::numeric_limits<T>::quiet_NaN();
	}

	template<class T = int32_t>
	inline constexpr T pow_constexpr(const T aBase, const T aExp, const T  aResult = 1) throw() {
		return aExp < 1 ? aResult : pow_constexpr<T>(aBase * aBase, aExp / 2, (aExp % 2) ? aResult * aBase : aResult);
	}

	template<class T = int32_t>
	inline constexpr T bitmask_constexpr(const T aBits, const T aResult = 0) throw() {
		return aBits < 1 ? aResult : (bitmask_constexpr(aBits - 1) << static_cast<T>(1)) | static_cast<T>(1);
	}

	namespace implementation {
		// Min max range
		template<class T = int32_t> inline constexpr T u_min(const T aBits) throw() { return 0; }												//!< Minimum range of an unsigned integer. \param aBits The number of bits in the integer.
		template<class T = int32_t> inline constexpr T u_max(const T aBits) throw() { return pow_constexpr<int32_t>(2, aBits) - 1; }					//!< Maximum range of an unsigned integer. \param aBits The number of bits in the integer.
		template<class T = int32_t> inline constexpr T i_min(const T aBits) throw() { return -i_max(aBits); }									//!< Minimum range of a signed integer. \param aBits The number of bits in the integer.
		template<class T = int32_t> inline constexpr T i_max(const T aBits) throw() { return static_cast<T>(u_max<uint64_t>(aBits) / 2L) + 1; }	//!< Maximum range of a signed integer. \param aBits The number of bits in the integer.
	}

	// Comparison operators

	template<class T = int32_t> inline constexpr bool lt(const T a, const T b) throw() { return a < b; }		//!< Compare if one value is less than another.
	template<class T = int32_t> inline constexpr bool lte(const T a, const T b) throw() { return a <= b; }		//!< Compare if one value is less than or equal to another.
	template<class T = int32_t> inline constexpr bool gt(const T a, const T b) throw() { return a > b; }		//!< Compare if one value is greater than another.
	template<class T = int32_t> inline constexpr bool gte(const T a, const T b) throw() { return a >= b; }		//!< Compare if one value is greater than or equal to another.
	template<class T = int32_t> inline constexpr bool eq(const T a, const T b) throw() { return a == b; }		//!< Compare if one value is equal to another.
	template<class T = int32_t> inline constexpr bool neq(const T a, const T b) throw() { return a != b; }		//!< Compare if one value is not equal to another.
}

#endif