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
	
	void from_json(std::isteam&, value_parser&) {
		//! \todo Implement
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