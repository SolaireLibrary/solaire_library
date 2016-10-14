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

#ifndef SOLAIRE_SERIALISE_GENERIC_VALUE_TO_PARSER_HPP
#define SOLAIRE_SERIALISE_GENERIC_VALUE_TO_PARSER_HPP

#include "value.hpp"
#include "value_parser.hpp"

namespace solaire { namespace serial {
	class parser_to_value : public value_parser {
	private:
		value mRoot;
		std::vector<value*> mStack;
		std::string mName;

		void append_value(value&&);
	public:
		parser_to_value();
		value& get();
		const value& get() const;

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

	void value_to_parser(const value&, value_parser&);
}}

#endif