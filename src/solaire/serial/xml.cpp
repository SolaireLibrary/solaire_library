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

#include <cstdio>
#include <ctype.h>
#include "solaire/serial/xml.hpp"

namespace solaire { namespace serial {
	
	void from_xml(std::istream& aStream, value_parser& aParser) {
		//! \todo Implement
	}
	
	// to_xml
	
	to_xml::to_xml(std::ostream& aStream) :
		mStream(aStream)
	{}

	void to_xml::write_value(const std::string& aValue) {
		const state tmp = mStateStack.back();
		if(tmp.is_array) {

		}else if(tmp.is_object) {
			
		}else {
			
		}
	}
	
	void to_xml::begin_array() {
		//! \todo Implement
	}
	
	void to_xml::end_array() {
		//! \todo Implement
	}
	
	void to_xml::begin_object() {
		//! \todo Implement
	}
	
	void to_xml::end_object() {
		//! \todo Implement
	}
	
	void to_xml::name(const char* aName) {
		//! \todo Implement
	}
	
	void to_xml::value_void() {
		//! \todo Implement
	}
	
	void to_xml::value_bool(const bool aValue) {
		//! \todo Implement
	}
	
	void to_xml::value_char(const char aValue) {
		//! \todo Implement
	}
	
	void to_xml::value_unsigned(const uint64_t aValue) {
		//! \todo Implement
	}
	
	void to_xml::value_signed(const int64_t aValue) {
		//! \todo Implement
	}
	
	void to_xml::value_float(const double aValue) {
		//! \todo Implement
	}
	
	void to_xml::value_pointer(void* const aValue) {
		//! \todo Implement
	}
	
	void to_xml::value_string(const char* const aValue) {
		//! \todo Implement
	}
	
}}