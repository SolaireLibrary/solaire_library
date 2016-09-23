#ifndef SOLAIRE_CORE_BIN_UTILS_HPP
#define SOLAIRE_CORE_BIN_UTILS_HPP

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
	void read_bits(void* const, const uint32_t, const void*, const uint32_t, const uint32_t) throw();

	constexpr uint32_t popcount(const uint8_t) throw();
	constexpr uint32_t popcount(const uint16_t) throw();
	constexpr uint32_t popcount(const uint32_t) throw();
	constexpr uint32_t popcount(const uint64_t) throw();
	constexpr uint32_t popcount(const int8_t) throw();
	constexpr uint32_t popcount(const int16_t) throw();
	constexpr uint32_t popcount(const int32_t) throw();
	constexpr uint32_t popcount(const int64_t) throw();
	uint32_t popcount(const void* const, const uint32_t) throw();

	constexpr uint8_t reflect(const uint8_t) throw();
	constexpr uint16_t reflect(const uint16_t) throw();
	constexpr uint32_t reflect(const uint32_t) throw();
	constexpr uint64_t reflect(const uint64_t) throw();
	constexpr int8_t reflect(const int8_t) throw();
	constexpr int16_t reflect(const int16_t) throw();
	constexpr int32_t reflect(const int32_t) throw();
	constexpr int64_t reflect(const int64_t) throw();
	void reflect(void* const, const void* const, const uint32_t) throw();
}

#endif