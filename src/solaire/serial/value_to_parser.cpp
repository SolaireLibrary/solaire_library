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

#include "value_to_parser.hpp"

namespace solaire { namespace serial {
	void value_to_parser(const value& aValue, value_parser& aParser) {
		switch (aValue.get_type()) {
		case value::TYPE_VOID:
			aParser.value_void();
			break;
		case value::TYPE_BOOL:
			aParser.value_bool(aValue.get_bool());
			break;
		case value::TYPE_CHAR:
			aParser.value_char(aValue.get_char());
			break;
		case value::TYPE_UNSIGNED:
			aParser.value_unsigned(aValue.get_unsigned());
			break;
		case value::TYPE_SIGNED:
			aParser.value_signed(aValue.get_signed());
			break;
		case value::TYPE_FLOAT:
			aParser.value_float(aValue.get_float());
			break;
		case value::TYPE_POINTER:
			aParser.value_pointer(aValue.get_pointer());
			break;
		case value::TYPE_STRING:
			aParser.value_string(aValue.get_string().c_str());
			break;
		case value::TYPE_ARRAY:
		{
			const value::array_t array_ = aValue.get_array();
			aParser.begin_array();
			for (const value& i : array_) value_to_parser(i, aParser);
			aParser.end_array();
		}
		break;
		case value::TYPE_OBJECT:
		{
			const value::object_t object = aValue.get_object();
			aParser.begin_object();
			for (const std::pair<std::string, value>& i : object) {
				aParser.name(i.first.c_str());
				value_to_parser(i.second, aParser);
			}
			aParser.end_object();
		}
		break;
		}
	}

	// parser_to_value

	parser_to_value::parser_to_value() {
		mStack.push_back(&mRoot);
	}

	value& parser_to_value::get() {
		return mRoot;
	}

	const value& parser_to_value::get() const {
		return mRoot;
	}

	void parser_to_value::append_value(value&& aValue) {
		value& head = *mStack.back();
		value* tmp = &head;
		const value::type htype = head.get_type();
		const value::type vtype = aValue.get_type();
		switch (htype) {
		case value::TYPE_ARRAY:
		{
			value::array_t& array_ = head.get_array();
			array_.push_back(value());
			tmp = &array_.back();
		}
		break;
		case value::TYPE_OBJECT:
		{
			if (mName.empty())  throw std::runtime_error("solaire::parser_to_value::append_value : No name set for object member");
			value::object_t& object = head.get_object();
			object.emplace(mName, value());
			tmp = &object.find(mName)->second;
			mName.clear();
		}
		break;
		}
		*tmp = std::move(aValue);
		if (vtype == value::TYPE_ARRAY || vtype == value::TYPE_OBJECT) mStack.push_back(tmp);
	}

	void parser_to_value::begin_array() {
		append_value(value(value::TYPE_ARRAY));
	}

	void parser_to_value::end_array() {
		value& head = *mStack.back();
		if (!head.is_array()) throw std::runtime_error("solaire::parser_to_value::end_array : Current value is not an array");
		if (mStack.size() > 1) mStack.pop_back();
	}

	void parser_to_value::begin_object() {
		append_value(value(value::TYPE_OBJECT));
	}

	void parser_to_value::end_object() {
		value& head = *mStack.back();
		if (!head.is_object()) throw std::runtime_error("solaire::parser_to_value::end_object : Current value is not an object");
		if (mStack.size() > 1) mStack.pop_back();
	}

	void parser_to_value::name(const char* aValue) {
		value& head = *mStack.back();
		if (!head.is_object()) throw std::runtime_error("solaire::parser_to_value::name : Current value is not an object");
		mName = aValue;
	}

	void parser_to_value::value_void() {
		append_value(value(value::TYPE_VOID));
	}

	void parser_to_value::value_bool(const bool aValue) {
		append_value(value(aValue));
	}

	void parser_to_value::value_char(const char aValue) {
		append_value(value(aValue));
	}

	void parser_to_value::value_unsigned(const uint64_t aValue) {
		append_value(value(aValue));
	}

	void parser_to_value::value_signed(const int64_t aValue) {
		append_value(value(aValue));
	}

	void parser_to_value::value_float(const double aValue) {
		append_value(value(aValue));
	}

	void parser_to_value::value_pointer(void* const aValue) {
		append_value(value(aValue));
	}

	void parser_to_value::value_string(const char* const aValue) {
		append_value(value(aValue));
	}
}}