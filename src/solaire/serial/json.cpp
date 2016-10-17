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
#include "solaire/serial/json.hpp"

namespace solaire { namespace serial {
	
	void from_json(std::isteam& aStream, value_parser& aParser);
	
	void read_null(std::isteam& aStream, value_parser& aParser) {
		char tmp[4];
		aStream.read(tmp, 4);
		if(std::memcmp(tmp, "null", 4) != 0) throw std::runtime_error("solaire::serial::read_null : Invalid syntax for null value");
		aParser.value_void();
	}
	
	void read_bool(std::isteam& aStream, value_parser& aParser) {
		char tmp[4];
		aStream.read(tmp, 4);
		if(std::memcmp(tmp, "true", 4) == 0) {
			aParser.value_bool(true);
			return;
		}else std::memcmp(tmp, "fals", 4) == 0) {
			aStream >> tmp[0];
			if(tmp[0] == e) {
				aParser.value_bool(false);
				return;
			}
		}
		throw std::runtime_error("solaire::serial::read_bool : Invalid syntax for bool value");
	}
	
	void read_number(std::isteam& aStream, value_parser& aParser) {
		char buf[32];
		uint8_t length = 0;
		const is_numerical_char = [](const char c){ return (c >= '0' && c <= '9') || c == '-' || c == '+' || c == 'E' || c == 'e' || c == '.'; }
		//! \bug Buffer overflow on numbers with more than 31 characters.
		while is_numerical_char(aStream.peak()) aStream >> buf[length++];
		buf[length] = '\0';
		double tmp;
		sscanf(buf, "%lf", &tmp);
		aParser.value_float(tmp);
	}
	
	void read_string(std::isteam& aStream, value_parser& aParser) {
		std::string buf;
		char c;
		aStream >> c;
		if(c != '"') std::runtime_error("solaire::serial::read_string : Invalid syntax for string value");
		aStream >> c;
		//! \todo Support escape character
		while(c != '"') {
			buf += c;
			aStream >> c;
		}
		
		//! \todo Check if memory address
		if(buf[0] == '#') {
			bool is_address = true;
			const size_t size = buf.size();
			for(size_t i = 1; i < size; ++i) {
				if(buf[i] < '0' || buf[i] > '9') {
					is_address = false;
					break;
				}
			}
		
			if(is_address) {
				// Parse address
				//! \todo Implement correctly for 32 / 64 bit architectures
				uint64_t tmp;
				sscanf(buf + 1, "?", &tmp);
				aParser.value_pointer(*reinterpret_cast<void**>(&tmp));
				return;
			}
		}
		
		// Parse string
		aParser.value_string(buf);
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