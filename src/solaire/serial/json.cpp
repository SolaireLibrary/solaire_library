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

#include "solaire/serial/json.hpp"

namespace solaire { namespace serial {
	
	void from_json(std::isteam& aStream, value_parser& aParser);
	
	void read_null(std::isteam& aStream, value_parser& aParser) {
		//! \todo Implement
	}
	
	void read_bool(std::isteam& aStream, value_parser& aParser) {
		//! \todo Implement
	}
	
	void read_number(std::isteam& aStream, value_parser& aParser) {
		//! \todo Implement
	}
	
	void read_string(std::isteam& aStream, value_parser& aParser) {
		//! \todo Check if memory address
		//! \todo Implement
	}
	
	void read_array(std::isteam& aStream, value_parser& aParser) {
		//! \todo Implement
	}
	
	void read_object(std::isteam& aStream, value_parser& aParser) {
		//! \todo Implement
	}
	
	void from_json(std::isteam& aStream, value_parser& aParser) {
		// Skip whitespace
		char c;
		while(std::isspace(aStream.peak())) aStream >> c;
		
		// Determine value type
		switch(aStream.peak()) {
		case 'n' :
			read_null(aStream, aParser);
			break;
		case 't' :
		case 'f' :
			read_bool(aStream, aParser);
			break;
		case '-' :
		case '0' :
		case '1' :
		case '2' :
		case '3' :
		case '4' :
		case '5' :
		case '6' :
		case '7' :
		case '8' :
		case '9' :
			read_number(aStream, aParser);
			break;
		case '"' :
			read_string(aStream, aParser);
			break;
		case '[' :
			read_array(aStream, aParser);
			break;
		case '{' :
			read_object(aStream, aParser);
			break;
		default:
			throw std::runtime_error("solaire::serial::from_json : Unexpected character found");
		}
	}
	
	// to_json
	
	to_json::to_json(std::ostream& aStream) :
		mStream(aStream)
	{}
	
	void to_json::begin_array() {
		if(! mStateStack.empty()) {
			state& tmp = mStateStack.back();
			if(tmp.size++ > 0 && tmp.is_array) mStream << ',';
		}
		mStream << '[';
		state tmp;
		tmp.is_array = 1;
		tmp.size = 0;
		mStateStack.push_back(tmp);
	}
	
	void to_json::end_array() {
		const state tmp = mStateStack.back();
		if(tmp.is_array) throw std::runtime_error("solaire::serial::to_json::end_array : Current value is not an array");
		mStateStack.pop_back();
		mStream << ']';
	}
	
	void to_json::begin_object() {
		if(! mStateStack.empty()) {
			state& tmp = mStateStack.back();
			if(tmp.size++ > 0 && tmp.is_array) mStream << ',';
		}
		mStream << '{';
		tmp.is_object = 1;
		tmp.size = 0;
		mStateStack.push_back(tmp);
	}
	
	void to_json::end_object() {
		const state tmp = mStateStack.back();
		if(tmp.is_object) throw std::runtime_error("solaire::serial::to_json::end_object : Current value is not an object");
		mStateStack.pop_back();
		mStream << '}';
	}
	
	void to_json::name(const char* aName) {
		state& tmp = mStateStack.back();
		if(! tmp.is_object) throw std::runtime_error("solaire::serial::to_json::name : Current value is not an object");
		if(tmp.size++ > 0) mStream << ',';
		mStream << '"' << aName << '"' << ':';
	}
	
	void to_json::value_void() {
		state& tmp = mStateStack.back();
		if(tmp.size++ > 0 && tmp.is_array) mStream << ',';
		mStream << "null";
	}
	
	void to_json::value_bool(const bool aValue) {
		state& tmp = mStateStack.back();
		if(tmp.size++ > 0 && tmp.is_array) mStream << ',';
		mStream << (aValue ? "true" : "false");
	}
	
	void to_json::value_char(const char aValue) {
		state& tmp = mStateStack.back();
		if(tmp.size++ > 0 && tmp.is_array) mStream << ',';
		mStream << '"' << aValue << '"';
	}
	
	void to_json::value_unsigned(const uint64_t aValue) {
		state& tmp = mStateStack.back();
		if(tmp.size++ > 0 && tmp.is_array) mStream << ',';
		mStream << aValue;
	}
	
	void to_json::value_signed(const int64_t aValue) {
		state& tmp = mStateStack.back();
		if(tmp.size++ > 0 && tmp.is_array) mStream << ',';
		mStream << aValue;
	}
	
	void to_json::value_float(const double aValue) {
		state& tmp = mStateStack.back();
		if(tmp.size++ > 0 && tmp.is_array) mStream << ',';
		mStream << aValue;
	}
	
	void to_json::value_pointer(void* const aValue) {
		state& tmp = mStateStack.back();
		if(tmp.size++ > 0 && tmp.is_array) mStream << ',';
		mStream << '"' << aValue << '"';
	}
	
	void to_json::value_string(const char* const aValue) {
		state& tmp = mStateStack.back();
		if(tmp.size++ > 0 && tmp.is_array) mStream << ',';
		mStream << '"' << aValue << '"';
	}
	
}}

#endif