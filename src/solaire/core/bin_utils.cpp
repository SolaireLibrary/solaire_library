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

#include "solaire/core/bin_utils.hpp"

namespace solaire {

	static constexpr uint8_t POPCOUNT[16] = {
		//	0000	0001	0010	0011	0100	0101	0110	0111
			0,		1,		1,		2,		1,		2,		2,		3,
		//	1000	1001	1010	1011	1100	1101	1110	1111
			1,		2,		2,		3,		2,		3,		3,		4
	};

	static constexpr uint8_t REFLECT[16] = {
		//	0000	0001	0010	0011	0100	0101	0110	0111
			0,		8,		4,		12,		2,		10,		6,		14,
		//	1000	1001	1010	1011	1100	1101	1110	1111
			1,		9,		5,		13,		3,		11,		7,		15
	};

	/*!
		\brief Copy a number of bits from one location to another.
		\param aOut The first byte to write into.
		\param aOutOffset The offset into the first byte to write into.
		\param aIn The first byte to read from.
		\param aInOffset The offset into the first byte to read from.
		\param aBits The number of bits to read.
	*/
	void read_bits(void* const aOut, const uint32_t aOutOffset, const void* aIn, const uint32_t aInOffset, const uint32_t aBits) throw() {
		const uint32_t outByte = aOutOffset / 8;
		const uint32_t inByte = aOutOffset / 8;
		uint8_t* outPtr = static_cast<uint8_t*>(aOut) + outByte;
		const uint8_t* inPtr = static_cast<const uint8_t*>(aIn) + inByte;
		const uint32_t outTrail = aOutOffset & 7;
		const uint32_t inTrail = aOutOffset & 7;
		uint8_t outBit = 1 << outTrail;
		uint8_t inBit = 1 << inTrail;
		uint8_t bit;

		//! \todo Optimise
		for(uint32_t i = 0; i < aBits; ++i) {
			bit = *inPtr & inBit;

			if(bit) {
				*outPtr |= outBit;
			}else {
				*outPtr &= ~outBit;
			}

			if(outBit == 64) {
				outBit = 1;
				++outPtr;
			}else {
				outBit <<= 1;
			}

			if(inBit == 64) {
				inBit = 1;
				++inPtr;
			}else{
				inBit <<= 1;
			}
		}
	}
	
	/*!
		\brief Count the number of bits that are set.
		\param aValue The value to count.
		\return The number of set bits.
	*/
	constexpr uint32_t popcount(const uint8_t aValue) throw() {
		return POPCOUNT[aValue & 15] + POPCOUNT[aValue >> 4];
	}
	
	/*!
		\brief Count the number of bits that are set.
		\param aValue The value to count.
		\return The number of set bits.
	*/
	constexpr uint32_t popcount(const uint16_t aValue) throw() {
		return popcount(reinterpret_cast<const uint8_t*>(&aValue)[0]) + popcount(reinterpret_cast<const uint8_t*>(&aValue)[1]);
	}
	
	/*!
		\brief Count the number of bits that are set.
		\param aValue The value to count.
		\return The number of set bits.
	*/
	constexpr uint32_t popcount(const uint32_t aValue) throw() {
		return popcount(reinterpret_cast<const uint16_t*>(&aValue)[0]) + popcount(reinterpret_cast<const uint16_t*>(&aValue)[1]);
	}
	
	/*!
		\brief Count the number of bits that are set.
		\param aValue The value to count.
		\return The number of set bits.
	*/
	constexpr uint32_t popcount(const uint64_t aValue) throw() {
		return popcount(reinterpret_cast<const uint32_t*>(&aValue)[0]) + popcount(reinterpret_cast<const uint32_t*>(&aValue)[1]);
	}
	
	/*!
		\brief Count the number of bits that are set.
		\param aValue The value to count.
		\return The number of set bits.
	*/
	constexpr uint32_t popcount(const int8_t aValue) throw() {
		return popcount(*reinterpret_cast<const uint8_t*>(&aValue));
	}
	
	/*!
		\brief Count the number of bits that are set.
		\param aValue The value to count.
		\return The number of set bits.
	*/
	constexpr uint32_t popcount(const int16_t aValue) throw() {
		return popcount(*reinterpret_cast<const uint16_t*>(&aValue));
	}
	
	/*!
		\brief Count the number of bits that are set.
		\param aValue The value to count.
		\return The number of set bits.
	*/
	constexpr uint32_t popcount(const int32_t aValue) throw() {
		return popcount(*reinterpret_cast<const uint32_t*>(&aValue));
	}
	
	/*!
		\brief Count the number of bits that are set.
		\param aValue The value to count.
		\return The number of set bits.
	*/
	constexpr uint32_t popcount(const int64_t aValue) throw() {
		return popcount(*reinterpret_cast<const uint64_t*>(&aValue));
	}
	
	/*!
		\brief Count the number of bits that are set.
		\param aPtr The first byte to count.
		\param aBytes The number of bytes in aPtr.
		\return The number of set bits.
	*/
	uint32_t popcount(const void* const aPtr, const uint32_t aBytes) throw() {
		const uint8_t* i = static_cast<const uint8_t*>(aPtr);
		uint32_t bytes = aBytes;
		uint32_t ones = 0;

		while(bytes >= 8) {
			ones += popcount(*reinterpret_cast<const uint64_t*>(i));
			i += 8;
			bytes -= 8;
		}

		if(bytes >= 4) {
			ones += popcount(*reinterpret_cast<const uint32_t*>(i));
			i += 4;
			bytes -= 4;
		}

		if(bytes >= 2) {
			ones += popcount(*reinterpret_cast<const uint16_t*>(i));
			i += 2;
			bytes -= 2;
		}

		if(bytes >= 1) {
			ones += popcount(*i);
		}

		return ones;
	}
	
	/*!
		\brief Reverse the order of bits.
		\param aValue The value to reflect.
		\return The reflected value.
	*/
	constexpr uint8_t reflect(const uint8_t aValue) throw() {
		return (REFLECT[aValue & 15] << 4) | REFLECT[aValue >> 4];
	}
	
	/*!
		\brief Reverse the order of bits.
		\param aValue The value to reflect.
		\return The reflected value.
	*/
	constexpr uint16_t reflect(const uint16_t aValue) throw() {
		return
			(static_cast<uint16_t>(reflect(reinterpret_cast<const uint8_t*>(&aValue)[0])) << 8)  | 
			reflect(reinterpret_cast<const uint8_t*>(&aValue)[1]);
	}
	
	/*!
		\brief Reverse the order of bits.
		\param aValue The value to reflect.
		\return The reflected value.
	*/
	constexpr uint32_t reflect(const uint32_t aValue) throw() {
		return
			(static_cast<uint32_t>(reflect(reinterpret_cast<const uint16_t*>(&aValue)[0])) << 16) |
			reflect(reinterpret_cast<const uint16_t*>(&aValue)[1]);
	}
	
	/*!
		\brief Reverse the order of bits.
		\param aValue The value to reflect.
		\return The reflected value.
	*/
	constexpr uint64_t reflect(const uint64_t aValue) throw() {
		return
			(static_cast<uint64_t>(reflect(reinterpret_cast<const uint32_t*>(&aValue)[0])) << 32L) |
			reflect(reinterpret_cast<const uint32_t*>(&aValue)[1]);
	}
	
	/*!
		\brief Reverse the order of bits.
		\param aValue The value to reflect.
		\return The reflected value.
	*/
	constexpr int8_t reflect(const int8_t aValue) throw() {
		return reflect(*reinterpret_cast<const uint8_t*>(&aValue));
	}
	
	/*!
		\brief Reverse the order of bits.
		\param aValue The value to reflect.
		\return The reflected value.
	*/
	constexpr int16_t reflect(const int16_t aValue) throw() {
		return reflect(*reinterpret_cast<const uint16_t*>(&aValue));
	}
	
	/*!
		\brief Reverse the order of bits.
		\param aValue The value to reflect.
		\return The reflected value.
	*/
	constexpr int32_t reflect(const int32_t aValue) throw() {
		return reflect(*reinterpret_cast<const uint32_t*>(&aValue));
	}
	
	/*!
		\brief Reverse the order of bits.
		\param aValue The value to reflect.
		\return The reflected value.
	*/
	constexpr int64_t reflect(const int64_t aValue) throw() {
		return reflect(*reinterpret_cast<const uint64_t*>(&aValue));
	}

	/*!
		\brief Reverse the order of bits.
		\param aOut The destination of the reversed data.
		\param aIn The source of the data.
		\param aBytes The number of bytes in aIn.
	*/
	void reflect(void* const aOut, const void* const aIn, const uint32_t aBytes) throw() {
		uint8_t* i = static_cast<uint8_t*>(aOut) + aBytes - 1;
		const uint8_t* j = static_cast<const uint8_t*>(aIn);
		const uint8_t* const end = j + aBytes;
		while(j != end) {
			*i = reflect(*j);
			--i;
			++j;
		}
	}
}