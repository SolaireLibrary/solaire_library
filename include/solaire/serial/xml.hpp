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

#ifndef SOLAIRE_SERIALISE_XML_HPP
#define SOLAIRE_SERIALISE_XML_HPP

#include <vector>
#include <iostream>
#include "value_parser.hpp"

namespace solaire { namespace serial {

	namespace implementation {
		struct xml_element {
			typedef std::pair<std::string, std::string> attribute;

			std::vector<attribute> attributes;
			std::vector<element> children;
			std::string body;
			std::string name;
			bool is_comment;
		
			static void write(std::ostream&, const xml_element&);
			static xml_element read(std::istream&);
			xml_element();
		};
	}

	class to_xml : public value_parser {
	public:
		enum primitive_value_mode : uint8_t {
			PRIMITIVE_AS_ATTRIBUTE,
			PRIMITIVE_AS_ELEMENT
		};
	private:
		struct state {
			struct {
				uint8_t is_array : 1;
				uint8_t is_object : 1;
			};
			implementation::xml_element* element;
		};
		std::string mName;
		std::vector<state> mStateStack;
		implementation::xml_element mRoot;
		std::ostream& mStream;
		const primitive_value_mode mPrimitiveMode;

		void write_value(const char* const);
	public:
		to_xml(std::ostream&, const primitive_value_mode = PRIMITIVE_AS_ATTRIBUTE);
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

	void from_xml(std::istream&, value_parser&);
}}

#endif