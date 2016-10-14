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

#include "solaire/serial/value.hpp"

namespace solaire { namespace serial {
	// value

	value::value() :
		mType(TYPE_VOID)
	{}

	value::value(const type aType) :
		mType(aType)
	{
		switch (aType) {
		case TYPE_VOID:
			break;
		case TYPE_BOOL:
			mBool = false;
			break;
		case TYPE_CHAR:
			mChar = '\0';
			break;
		case TYPE_UNSIGNED:
			mUnsigned = 0;
			break;
		case TYPE_SIGNED:
			mSigned = 0;
			break;
		case TYPE_FLOAT:
			mFloat = 0.0;
			break;
		case TYPE_POINTER:
			mPointer = nullptr;
			break;
		case TYPE_STRING:
			mString = new std::string();
			break;
		case TYPE_ARRAY:
			mArray = new array_t();
			break;
		case TYPE_OBJECT:
			mObject = new object_t();
			break;
		}
	}

	value::value(const bool aValue) :
		mType(TYPE_BOOL),
		mBool(aValue)
	{}

	value::value(const char aValue) :
		mType(TYPE_CHAR),
		mChar(aValue)
	{}

	value::value(const uint8_t aValue) :
		mType(TYPE_UNSIGNED),
		mUnsigned(aValue)
	{}

	value::value(const uint16_t aValue) :
		mType(TYPE_UNSIGNED),
		mUnsigned(aValue)
	{}

	value::value(const uint32_t aValue) :
		mType(TYPE_UNSIGNED),
		mUnsigned(aValue)
	{}

	value::value(const uint64_t aValue) :
		mType(TYPE_UNSIGNED),
		mUnsigned(aValue)
	{}

	value::value(const int8_t aValue) :
		mType(TYPE_SIGNED),
		mSigned(aValue)
	{}

	value::value(const int16_t aValue) :
		mType(TYPE_SIGNED),
		mSigned(aValue)
	{}

	value::value(const int32_t aValue) :
		mType(TYPE_SIGNED),
		mSigned(aValue)
	{}

	value::value(const int64_t aValue) :
		mType(TYPE_SIGNED),
		mSigned(aValue)
	{}

	value::value(const float aValue) :
		mType(TYPE_FLOAT),
		mFloat(aValue)
	{}

	value::value(const double aValue) :
		mType(TYPE_FLOAT),
		mFloat(aValue)
	{}

	value::value(void* const  aValue) :
		mType(TYPE_POINTER),
		mPointer(aValue)
	{}

	value::value(const char* const aValue) :
		mType(TYPE_STRING),
		mString(new std::string(aValue))
	{}

	value::value(const std::string& aValue) :
		mType(TYPE_STRING),
		mString(new std::string(aValue))
	{}

	value::value(value&& aOther) :
		mType(TYPE_VOID)
	{
		operator=(std::move(aOther));
	}

	value::value(const value& aOther) :
		mType(TYPE_VOID)
	{
		operator=(aOther);
	}

	value::~value() {
		set_void();
	}

	value& value::operator=(value&& aOther) {
		//! \todo Implement
		enum {
			SIZE = sizeof(value)
		};
		uint8_t tmp[SIZE];
		memcpy(tmp, this, SIZE);
		memcpy(this, &aOther, SIZE);
		memcpy(&aOther, tmp, SIZE);
		return *this;
	}

	value& value::operator=(const value& aOther) {
		switch (aOther.mType) {
		case TYPE_VOID:
			set_void();
			break;
		case TYPE_BOOL:
			set_bool() = aOther.get_bool();
			break;
		case TYPE_CHAR:
			set_char() = aOther.get_char();
			break;
		case TYPE_UNSIGNED:
			set_unsigned() = aOther.get_unsigned();
			break;
		case TYPE_SIGNED:
			set_signed() = aOther.get_signed();
			break;
		case TYPE_FLOAT:
			set_float() = aOther.get_float();
			break;
		case TYPE_POINTER:
			set_pointer() = aOther.get_pointer();
			break;
		case TYPE_STRING:
			set_string() = aOther.get_string();
			break;
		case TYPE_ARRAY:
			set_array() = aOther.get_array();
			break;
		case TYPE_OBJECT:
			set_object() = aOther.get_object();
			break;
		}
		return *this;
	}

	value::type value::get_type() const {
		return mType;
	}

	bool value::is_void() const {
		return mType == TYPE_VOID;
	}

	void value::set_void() {
		switch(mType) {
		case TYPE_STRING:
			delete mString;
			break;
		case TYPE_ARRAY:
			delete mArray;
			break;
		case TYPE_OBJECT:
			delete mObject;
			break;
		}
		mType = TYPE_VOID;
	}


	bool value::is_bool() const {
		try {
			get_bool();
			return true;
		}catch(...){
			return false;
		}
	}

	bool& value::set_bool() {
		set_void();
		mType = TYPE_BOOL;
		return mBool;
	}

	bool& value::get_bool() {
		if(mType != TYPE_BOOL) {
			const bool tmp = const_cast<const value*>(this)->get_bool();
			set_bool() = tmp;
		}
		return mBool;
	}

	bool value::get_bool() const {
		switch(mType) {
		case TYPE_BOOL:
			return mBool;
		case TYPE_CHAR:
			if(mChar == '1' || mChar == 'y' || mChar == 'Y') return true;
			if(mChar == '0' || mChar == 'n' || mChar == 'N') return true;
			break;
		case TYPE_UNSIGNED:
			return mUnsigned >= 1;
		case TYPE_SIGNED:
			return mSigned >= 1;
		case TYPE_FLOAT:
			return mFloat >= 1.0;
		case TYPE_POINTER:
			return mPointer != nullptr;
		case TYPE_STRING:
			//! \todo Conversion from string
			break;
		}
		throw std::runtime_error("solaire::value::get_bool : Type is not convertable to bool");
	}

	bool value::is_char() const {
		try {
			get_char();
			return true;
		}catch (...) {
			return false;
		}
	}

	char& value::set_char() {
		set_void();
		mType = TYPE_CHAR;
		return mChar;
	}

	char& value::get_char() {
		if(mType != TYPE_CHAR) {
			const char tmp = const_cast<const value*>(this)->get_char();
			set_char() = tmp;
		}
		return mChar;
	}

	char value::get_char() const {
		switch (mType) {
		case TYPE_BOOL:
			return mBool ? '1' : '0';
		case TYPE_CHAR:
			if(mChar == '1' || mChar == 'y' || mChar == 'Y') return true;
			if(mChar == '0' || mChar == 'n' || mChar == 'N') return false;
			break;
		case TYPE_UNSIGNED:
			if(mUnsigned <= 9) return '0' + (char) mUnsigned;
		case TYPE_SIGNED:
			if(mSigned >= 0 && mSigned <= 9) return '0' + (char) mSigned;
		case TYPE_FLOAT:
			if (mFloat >= 0 && mFloat <= 9) return '0' + (char) mFloat;
		case TYPE_STRING:
			return mString->front();
		}
		throw std::runtime_error("solaire::value::get_char : Type is not convertable to char");
	}

	bool value::is_unsigned() const {
		try {
			get_unsigned();
			return true;
		}catch (...) {
			return false;
		}
	}

	uint64_t& value::set_unsigned() {
		set_void();
		mType = TYPE_UNSIGNED;
		return mUnsigned;
	}

	uint64_t& value::get_unsigned() {
		if(mType != TYPE_UNSIGNED) {
			const uint64_t tmp = const_cast<const value*>(this)->get_unsigned();
			set_unsigned() = tmp;
		}
		return mUnsigned;
	}

	uint64_t value::get_unsigned() const {
		switch(mType) {
		case TYPE_BOOL:
			return mBool ? 0 : 1;
		case TYPE_CHAR:
			if(mChar >= '0' && mChar <= '9') return mChar - '0';
			if(mChar == 'y' || mChar == 'Y') return 1;
			if(mChar == 'n' || mChar == 'N') return 0;
		case TYPE_UNSIGNED:
			return mUnsigned;;
		case TYPE_SIGNED:
			if(mSigned >= 0) return static_cast<uint64_t>(mSigned);
		case TYPE_FLOAT:
			if(mFloat >= 0) return static_cast<uint64_t>(mFloat);
		case TYPE_STRING:
			//! \todo Conversion from string
			break;
		}
		throw std::runtime_error("solaire::value::get_unsigned : Type is not convertable to unsigned");
	}

	bool value::is_signed() const {
		try {
			get_signed();
			return true;
		}catch (...) {
			return false;
		}
	}

	int64_t& value::set_signed() {
		set_void();
		mType = TYPE_SIGNED;
		return mSigned;
	}

	int64_t& value::get_signed() {
		if(mType != TYPE_SIGNED) {
			const int64_t tmp = const_cast<const value*>(this)->get_signed();
			set_signed() = tmp;
		}
		return mSigned;
	}

	int64_t value::get_signed() const {
		switch(mType) {
		case TYPE_BOOL:
			return mBool ? 0 : 1;
		case TYPE_CHAR:
			if(mChar >= '0' && mChar <= '9') return mChar - '0';
			if(mChar == 'y' || mChar == 'Y') return 1;
			if(mChar == 'n' || mChar == 'N') return 0;
		case TYPE_UNSIGNED:
			if(mUnsigned <= INT64_MAX) return static_cast<int64_t>(mUnsigned);
		case TYPE_SIGNED:
			return mSigned;
		case TYPE_FLOAT:
			return static_cast<int64_t>(mFloat);
		case TYPE_STRING:
			//! \todo Conversion from string
			break;
		}
		throw std::runtime_error("solaire::value::get_signed : Type is not convertable to signed");
	}

	bool value::is_float() const {
		try {
			get_float();
			return true;
		}catch (...) {
			return false;
		}
	}

	double& value::set_float() {
		set_void();
		mType = TYPE_FLOAT;
		return mFloat;
	}

	double& value::get_float() {
		if(mType != TYPE_FLOAT) {
			const double tmp = const_cast<const value*>(this)->get_float();
			set_float() = tmp;
		}
		return mFloat;
	}

	double value::get_float() const {
		switch(mType) {
		case TYPE_BOOL:
			return mBool ? 0.0 : 1.0;
		case TYPE_CHAR:
			if(mChar >= '0' && mChar <= '9') return mChar - '0';
			if(mChar == 'y' || mChar == 'Y') return 1.0;
			if(mChar == 'n' || mChar == 'N') return 0.0;
		case TYPE_UNSIGNED:
			return static_cast<double>(mFloat);
		case TYPE_SIGNED:
			return static_cast<double>(mFloat);
		case TYPE_FLOAT:
			return mFloat;
		case TYPE_STRING:
			//! \todo Conversion from string
			break;
		}
		throw std::runtime_error("solaire::value::get_float : Type is not convertable to float");
	}

	bool value::is_pointer() const {
		try {
			get_pointer();
			return true;
		}catch (...) {
			return false;
		}
	}

	void*& value::set_pointer() {
		set_void();
		mType = TYPE_POINTER;
		return mPointer;
	}

	void*& value::get_pointer() {
		if(mType != TYPE_POINTER) {
			void* const tmp = const_cast<const value*>(this)->get_pointer();
			set_pointer() = tmp;
		}
		return mPointer;
	}

	void* value::get_pointer() const {
		switch (mType) {
		case TYPE_POINTER:
			return mPointer;
		case TYPE_STRING:
			//! \todo Conversion from string
			break;
		}
		throw std::runtime_error("solaire::value::get_pointer : Type is not convertable to pointer");
	}

	bool value::is_string() const {
		try {
			get_string();
			return true;
		}catch (...) {
			return false;
		}
	}

	std::string& value::set_string() {
		if (mType != TYPE_STRING) {
			set_void();
			mString = new std::string();
			mType = TYPE_STRING;
		}
		else {
			mString->clear();
		}
		return *mString;
	}

	std::string& value::get_string() {
		//! \Convert
		return *mString;
	}

	const std::string& value::get_string() const {
		return *mString;
	}

	bool value::is_array() const {
		return mType == TYPE_ARRAY;
	}

	value::array_t& value::set_array() {
		if (mType != TYPE_ARRAY) {
			set_void();
			mArray = new array_t();
			mType = TYPE_ARRAY;
		}
		else {
			mArray->clear();
		}
		return *mArray;
	}

	value::array_t& value::get_array() {
		return *mArray;
	}

	const value::array_t& value::get_array() const {
		return *mArray;
	}

	bool value::is_object() const {
		return mType == TYPE_OBJECT;
	}

	value::object_t& value::set_object() {
		if (mType != TYPE_OBJECT) {
			set_void();
			mObject = new object_t();
			mType = TYPE_OBJECT;
		}
		else {
			mObject->clear();
		}
		return *mObject;
	}

	value::object_t& value::get_object() {
		return *mObject;
	}

	const value::object_t& value::get_object() const {
		return *mObject;
	}

	value& value::operator[](const int32_t aIndex) {
		return const_cast<value&>(const_cast<const value*>(this)->operator[](aIndex));
	}

	const value& value::operator[](const int32_t aIndex) const {
		switch(mType) {
		case TYPE_ARRAY:
			return mArray->operator[](aIndex);
			break;
		case TYPE_OBJECT:
			{
				auto i = mObject->begin();
				for(int32_t i = 0; i < aIndex; ++i) ++i;
				return i->second;
			}
			break;
		default:
			throw std::runtime_error("solaire::value::operator[] : Value is not an array or object");
		}
	}

	value& value::operator[](const std::string& aName) {
		return const_cast<value&>(const_cast<const value*>(this)->operator[](aName));
	}

	const value& value::operator[](const std::string& aName) const {
		if(mType != TYPE_OBJECT) throw std::runtime_error("solaire::value::operator[] : Value is not an object");
		return mObject->find(aName)->second;
	}

	uint32_t value::size() const {
		return mType == TYPE_ARRAY ? mArray->size() : mType == TYPE_OBJECT ? mObject->size() : 0;
	}

	void value::reserve(int32_t aSize) {
		if(mType == TYPE_ARRAY) mArray->reserve(aSize);
	}
}}