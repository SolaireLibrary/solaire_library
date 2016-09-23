#ifndef SOLAIRE_CORE_INTEGER_HPP
#define SOLAIRE_CORE_INTEGER_HPP

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

#include <type_traits>
#include "core.hpp"
#include "constexpr_maths.hpp"
#include "value_compare.hpp"

namespace solaire {

	namespace implementation {

		// Default definition of integer_container

		template<const int32_t BITS, const bool SIGNED, class ENABLE = void>
		struct integer_container {
			typedef void type;
		};

		// Specialised definitions of integer_container

		template<const int32_t BITS, const bool SIGNED>
		struct integer_container<BITS, SIGNED, typename std::enable_if<eq<>(BITS, 1)>::type> {
			typedef bool type;
		};

		template<const int32_t BITS>
		struct integer_container<BITS, false, typename std::enable_if<gt<>(BITS, 1) && lte<>(BITS, 8)>::type> {
			typedef uint8_t type;
		};

		template<const int32_t BITS>
		struct integer_container<BITS, false, typename std::enable_if<gt<>(BITS, 8) && lte<>(BITS, 16)>::type> {
			typedef uint16_t type;
		};

		template<const int32_t BITS>
		struct integer_container<BITS, false, typename std::enable_if<gt<>(BITS, 16) && lte<>(BITS, 32)>::type> {
			typedef uint32_t type;
		};

		template<const int32_t BITS>
		struct integer_container<BITS, false, typename std::enable_if<gt<>(BITS, 32) && lte<>(BITS, 64)>::type> {
			typedef uint64_t type;
		};

		template<const int32_t BITS>
		struct integer_container<BITS, true, typename std::enable_if<gt<>(BITS, 1) && lte<>(BITS, 8)>::type> {
			typedef int8_t type;
		};

		template<const int32_t BITS>
		struct integer_container<BITS, true, typename std::enable_if<gt<>(BITS, 8) && lte<>(BITS, 16)>::type> {
			typedef int16_t type;
		};

		template<const int32_t BITS>
		struct integer_container<BITS, true, typename std::enable_if<gt<>(BITS, 16) && lte<>(BITS, 32)>::type> {
			typedef int32_t type;
		};

		template<const int32_t BITS>
		struct integer_container<BITS, true, typename std::enable_if<gt<>(BITS, 32) && lte<>(BITS, 64)>::type> {
			typedef int64_t type;
		};
	}

	/*!
		\brief Defines integer properties for a signed or unsigned integer of a specific size.
		\detail
		\tparam BITS The length of the integer in bits.
		\tparam SIGNED True if the integer is signed.
		\version 2.0
	*/
	template<const uint32_t BITS_, const bool SIGNED>
	struct integer {
		static_assert(BITS_ > 0, "solaire::integer : 0 bit integers are not supported");
		static_assert(BITS_ <= 64, "solaire::integer : Integers greater than 64 bit are not supported");
		static_assert(!(BITS_ == 1 && SIGNED), "solaire::integer : 1 bit signed integers are not supported");

		typedef typename implementation::integer_container<BITS_, SIGNED>::type type;						//!< A primative type that can store any value representable by this integer.

		enum : type {
			BITS = BITS_,																					//!< The number of bits in this integer.
			BYTES = BITS / 8,																				//!< The number complete bytes in this integer.
			TRAIL_BITS = BITS % 8,																			//!< The number of bits trailing the last complete byte.
			IS_SIGNED = SIGNED,																				//!< Is the integer signed (1) or not (0).
			IS_BYTE_ALIGNED = TRAIL_BITS == 0,																//!< Does the integer have trailing bits (0) or not (1).
			BYTES_TOTAL = BYTES + IS_BYTE_ALIGNED ? 0 : 1,													//!< The total number of bytes required to store this integer.
			MIN_VALUE = IS_SIGNED ? implementation::i_min<type>(BITS) : implementation::u_min<type>(BITS),	//!< The minimum value that can be stored in this interger.
			MAX_VALUE = IS_SIGNED ? implementation::i_max<type>(BITS) : implementation::u_max<type>(BITS),	//!< The maximum value that can be stored in this interger.
			MASK = bitmask_constexpr<type>(BITS),															//!< Full bitmask of this interger.
			TRAIL_MASK = bitmask_constexpr<type>(TRAIL_BITS)												//!< Bitmask of the trailing bits.
		};
	};

	/*!
		\brief An unsigned integer definition.
		\tparam BITS The number of bits in the integer.
		\version 1.0
		\see integer
	*/
	template<const uint32_t BITS>
	using unsigned_integer = integer<BITS, false>;

	/*!
		\brief A signed integer definition.
		\tparam BITS The number of bits in the integer.
		\version 1.0
		\see integer
	*/
	template<const uint32_t BITS>
	using signed_integer = integer<BITS, true>;

	/*!
		\brief An integer definition that matches a primative type.
		\tparam T The primative type.
		\version 1.0
		\see integer
	*/
	template<class T>
	using integer_from_type = integer<sizeof(T) * 8, std::is_signed<T>::value>;

	namespace implementation {
		
		#define SOLAIRE_0_to_7(aMacro, aBase)\
			aMacro(aBase + 0) aMacro(aBase + 1) aMacro(aBase + 2) aMacro(aBase + 3)\
			aMacro(aBase + 4) aMacro(aBase + 5) aMacro(aBase + 6) aMacro(aBase + 7)

		#define SOLAIRE_0_to_15(aMacro, aBase) SOLAIRE_0_to_7(aMacro, aBase) SOLAIRE_0_to_7(aMacro, aBase + 8)
		#define SOLAIRE_0_to_31(aMacro, aBase) SOLAIRE_0_to_15(aMacro, aBase) SOLAIRE_0_to_15(aMacro, aBase + 16)
		#define SOLAIRE_0_to_63(aMacro, aBase) SOLAIRE_0_to_31(aMacro, aBase) SOLAIRE_0_to_31(aMacro, aBase + 32)

		template<class T>
		constexpr uint8_t range_bits_u(const T aLow, const T aHigh) throw() {
			#define SOLAIRE_MACRO(aBits) aLow >= 0 && aHigh >= 0 && aHigh <= u_max(aBits) ? aBits :
			return SOLAIRE_0_to_63(SOLAIRE_MACRO, 1) 0;
			#undef SOLAIRE_MACRO
		}

		template<class T>
		constexpr uint8_t range_bits_i(const T aLow, const T aHigh, const uint8_t aBits = 64) throw() {
			#define SOLAIRE_MACRO(aBits) (aLow < 0 || aHigh < 0) && aLow >= i_min(aBits) && aHigh <= i_max(aBits) ? aBits :
			return SOLAIRE_0_to_63(SOLAIRE_MACRO, 1) 0;
			#undef SOLAIRE_MACRO
		}

		#undef SOLAIRE_0_to_7
		#undef SOLAIRE_0_to_15
		#undef SOLAIRE_0_to_31
		#undef SOLAIRE_0_to_63
	}

	/*!
		\brief Create an integer definiton that can represent values between an inclusive range.
		\tparam T The type of LOW and HIGH.
		\tparam LOW The minimum value that must be represented.
		\tparam HIGH The maximum value that must be represented.
		\version 1.0
		\see integer
	*/
	template<class T, const T LOW, const T HIGH>
	using integer_from_range = integer<
		value_compare<T, LOW, 0>::LESS_THAN ? 
			implementation::range_bits_i<T>(LOW, HIGH) : 
			implementation::range_bits_u<T>(LOW, HIGH), 
		value_compare<T, LOW, 0>::LESS_THAN
	>;
}

#endif