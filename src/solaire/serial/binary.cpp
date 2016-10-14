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

#include "binary.hpp"
#include "value.hpp"

namespace solaire { namespace serial {
	enum {
		END_OBJECT = UINT8_MAX,
		END_ARRAY = END_OBJECT - 1
	};

	const uint8_t* read_value(const uint8_t* aPtr, value_parser& aParser) {
		const value::type type = static_cast<value::type>(*aPtr);
		++aPtr;
		switch(type) {
		case value::TYPE_VOID:
			break;
		case value::TYPE_BOOL:
			aParser.value_bool(*reinterpret_cast<const bool*>(aPtr));
			aPtr += sizeof(bool);
			break;
		case value::TYPE_CHAR:
			aParser.value_char(*reinterpret_cast<const char*>(aPtr));
			aPtr += sizeof(char);
			break;
		case value::TYPE_UNSIGNED:
			aParser.value_unsigned(*reinterpret_cast<const uint64_t*>(aPtr));
			aPtr += sizeof(uint64_t);
			break;
		case value::TYPE_SIGNED:
			aParser.value_signed(*reinterpret_cast<const int64_t*>(aPtr));
			aPtr += sizeof(int64_t);
			break;
		case value::TYPE_FLOAT:
			aParser.value_float(*reinterpret_cast<const double*>(aPtr));
			aPtr += sizeof(double);
			break;
		case value::TYPE_POINTER:
			aParser.value_pointer(*reinterpret_cast<void* const*>(aPtr));
			aPtr += sizeof(void*);
			break;
		case value::TYPE_STRING:
			{
				const uint16_t size = *reinterpret_cast<const uint16_t*>(aPtr);
				aPtr += sizeof(uint16_t);
				char* const str = reinterpret_cast<char*>(const_cast<uint8_t*>(aPtr));
				const char tmp = str[size];
				str[size] = '\0';
				aPtr += size;
				try {
					aParser.value_string(str);
				}catch(std::exception& e) {
					str[size] = tmp;
					throw e;
				}
				str[size] = tmp;
			}
			break;
		case value::TYPE_ARRAY:
			{
				aParser.begin_array();
				while(*aPtr != END_ARRAY) {
					aPtr = read_value(aPtr, aParser);
				}
				aParser.end_array();
				++aPtr;
			}
			break;
		case value::TYPE_OBJECT:
			{
				aParser.begin_object();
				while(*aPtr != END_OBJECT) {
					const uint16_t size = *reinterpret_cast<const uint16_t*>(aPtr);
					aPtr += sizeof(uint16_t);
					char* const str = reinterpret_cast<char*>(const_cast<uint8_t*>(aPtr));
					const char tmp = str[size];
					str[size] = '\0';
					aPtr += size;
					try {
						aParser.name(str);
					}catch (std::exception& e) {
						str[size] = tmp;
						throw e;
					}
					str[size] = tmp;
					aPtr = read_value(aPtr, aParser);
				}
				aParser.end_object();
				++aPtr;
			}
			break;
		}
		return aPtr;
	}

	void binary_to_value(const void* aPtr, const uint32_t aSize, value_parser& aParser) {
		const uint8_t* ptr = static_cast<const uint8_t*>(aPtr);
		const uint8_t* const end = ptr + aSize;
		while(ptr < end) {
			ptr = read_value(ptr, aParser);
		}
	}

	// value_to_binary

	void value_to_binary::write(const void* aPtr, const uint32_t aBytes) {
		const uint8_t* const ptr = static_cast<const uint8_t*>(aPtr);
		mBin.reserve(mBin.size() + aBytes);
		for(uint32_t i = 0; i < aBytes; ++i) mBin.push_back(ptr[i]);
	}

	const void* value_to_binary::get() const {
		return &mBin[0];
	}

	uint32_t value_to_binary::size() const {
		return mBin.size();
	}

	void value_to_binary::begin_array() {
		const uint8_t t = value::TYPE_ARRAY;
		write(&t, sizeof(uint8_t));
	}

	void value_to_binary::end_array() {
		const uint8_t t = END_ARRAY;
		write(&t, sizeof(uint8_t));
	}

	void value_to_binary::begin_object() {
		const uint8_t t = value::TYPE_OBJECT;
		write(&t, sizeof(uint8_t));
	}

	void value_to_binary::end_object() {
		const uint8_t t = END_OBJECT;
		write(&t, sizeof(uint8_t));
	}

	void value_to_binary::name(const char* aValue) {
		const uint16_t size = (uint16_t) strlen(aValue);
		write(&size, sizeof(uint16_t));
		write(aValue, size);
	}

	void value_to_binary::value_void() {
		const uint8_t t = value::TYPE_VOID;
		write(&t, sizeof(uint8_t));
	}

	void value_to_binary::value_bool(const bool aValue) {
		const uint8_t t = value::TYPE_BOOL;
		write(&t, sizeof(uint8_t));
		write(&aValue, sizeof(bool));
	}

	void value_to_binary::value_char(const char aValue) {
		const uint8_t t = value::TYPE_CHAR;
		write(&t, sizeof(uint8_t));
		write(&aValue, sizeof(char));
	}

	void value_to_binary::value_unsigned(const uint64_t aValue) {
		const uint8_t t = value::TYPE_UNSIGNED;
		write(&t, sizeof(uint8_t));
		write(&aValue, sizeof(uint64_t));
	}

	void value_to_binary::value_signed(const int64_t aValue) {
		const uint8_t t = value::TYPE_SIGNED;
		write(&t, sizeof(uint8_t));
		write(&aValue, sizeof(int64_t));
	}

	void value_to_binary::value_float(const double aValue) {
		const uint8_t t = value::TYPE_FLOAT;
		write(&t, sizeof(uint8_t));
		write(&aValue, sizeof(double));
	}

	void value_to_binary::value_pointer(void* const aValue) {
		const uint8_t t = value::TYPE_POINTER;
		write(&t, sizeof(uint8_t));
		write(&aValue, sizeof(void*));
	}

	void value_to_binary::value_string(const char* const aValue) {
		const uint8_t t = value::TYPE_STRING;
		const uint16_t size = (uint16_t) strlen(aValue);
		write(&t, sizeof(uint8_t));
		write(&size, sizeof(uint16_t));
		write(aValue, size);
	}
}}