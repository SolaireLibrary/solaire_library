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

#ifndef SOLAIRE_SERIALISE_GENERIC_PARSER_HPP
#define SOLAIRE_SERIALISE_GENERIC_PARSER_HPP

#include <cstdint>
#include <type_traits>

namespace solaire { namespace serial {
	class value_parser {
	public:
		virtual ~value_parser() {};

		virtual void begin_array() = 0;
		virtual void end_array() = 0;
		virtual void begin_object() = 0;
		virtual void end_object() = 0;

		virtual void name(const char*) = 0;

		virtual void value_void() = 0;
		virtual void value_bool(const bool) = 0;
		virtual void value_char(const char) = 0;
		virtual void value_unsigned(const uint64_t) = 0;
		virtual void value_signed(const int64_t) = 0;
		virtual void value_float(const double) = 0;
		virtual void value_pointer(void* const) = 0;
		virtual void value_string(const char* const) = 0;
	};
}}
#endif