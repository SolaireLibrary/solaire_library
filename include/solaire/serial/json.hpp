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

#ifndef SOLAIRE_SERIALISE_JSON_HPP
#define SOLAIRE_SERIALISE_JSON_HPP

#include <vector>
#include <iostream>
#include "value_parser.hpp"

namespace solaire { namespace serial {
	class to_json : public value_parser {
	private:
		struct state {
			uint16_t is_array : 1;
			uint16_t is_object : 1;
			uint16_t size : 14;
		};
		std::vector<state> mStateStack;
		std::ostream& mStream;
	public:
		to_json(std::ostream&);
		// Inherited from value_parser
		void begin_array() override;
		void end_array() override;
		void begin_object() override;
		void end_object() override;
		void name(const char*) override;
		void value_void() override;
		void value_bool(const bool) override;
		void value_char(const char) override;
		void value_unsigned(const uint64_t) override;
		void value_signed(const int64_t) override;
		void value_float(const double) override;
		void value_pointer(void* const) override;
		void value_string(const char* const) override;
	};
	
	void from_json(std::istream&, value_parser&);
}}

#endif