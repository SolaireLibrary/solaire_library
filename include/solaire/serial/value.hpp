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

#ifndef SOLAIRE_SERIALISE_GENERIC_VALUE_HPP
#define SOLAIRE_SERIALISE_GENERIC_VALUE_HPP

#include <cstdint>
#include <vector>
#include <map>
#include <string>

namespace solaire { namespace serial {
	class value {
	public:
		enum type : uint8_t {
			TYPE_VOID,
			TYPE_BOOL,
			TYPE_CHAR,
			TYPE_UNSIGNED,
			TYPE_SIGNED,
			TYPE_FLOAT,
			TYPE_POINTER,
			TYPE_STRING,
			TYPE_ARRAY,
			TYPE_OBJECT
		};

		typedef std::vector<value> array_t;
		typedef std::map<std::string, value> object_t;
	private:
		union {
			bool mBool;
			char mChar;
			uint64_t mUnsigned;
			int64_t mSigned;
			double mFloat;
			void* mPointer;
			std::string* mString;
			array_t* mArray;
			object_t* mObject;
		};
		type mType;
	public:
		value();
		value(const type);
		value(const bool);
		value(const char);
		value(const uint8_t);
		value(const uint16_t);
		value(const uint32_t);
		value(const uint64_t);
		value(const int8_t);
		value(const int16_t);
		value(const int32_t);
		value(const int64_t);
		value(const float);
		value(const double);
		value(void* const);
		value(const char* const);
		value(const std::string&);
		value(value&&);
		value(const value&);
		~value();

		value& operator=(value&&);
		value& operator=(const value&);

		type get_type() const;

		bool is_void() const;
		bool is_bool() const;
		bool is_char() const;
		bool is_unsigned() const;
		bool is_signed() const;
		bool is_float() const;
		bool is_pointer() const;
		bool is_string() const;
		bool is_array() const;
		bool is_object() const;

		bool get_bool() const;
		char get_char() const;
		uint64_t get_unsigned() const;
		int64_t get_signed() const;
		double get_float() const;
		void* get_pointer() const;
		const std::string& get_string() const;
		const array_t& get_array() const;
		const object_t& get_object() const;

		bool& get_bool();
		char& get_char();
		uint64_t& get_unsigned();
		int64_t& get_signed();
		double& get_float();
		void*& get_pointer();
		std::string& get_string();
		array_t& get_array();
		object_t& get_object();

		void set_void();
		bool& set_bool();
		char& set_char();
		uint64_t& set_unsigned();
		int64_t& set_signed();
		double& set_float();
		void*& set_pointer();
		std::string& set_string();
		array_t& set_array();
		object_t& set_object();

		value& operator[](const int32_t);
		const value& operator[](const int32_t) const;
		value& operator[](const std::string&);
		const value& operator[](const std::string&) const;
		uint32_t size() const;
		void reserve(const int32_t);

		template<class T>
		value& push_back(T aParam) { mArray->emplace_back(aParam); return mArray->back(); }

		template<class T>
		value& add(const std::string& aName, T aParam) { return mObject->emplace(aName, aParam).first->second; }

		inline value& operator[](const char* const aName) { return operator[](std::string(aName)); }
		inline const value& operator[](const char* const aName) const { return operator[](std::string(aName)); }

		inline operator bool() const { return get_bool(); }
		inline operator char() const { return get_char(); }
		inline operator uint8_t() const { return (uint8_t) get_unsigned(); }
		inline operator uint16_t() const { return (uint16_t) get_unsigned(); }
		inline operator uint32_t() const { return (uint32_t) get_unsigned(); }
		inline operator uint64_t() const { return get_unsigned(); }
		inline operator int8_t() const { return (int8_t) get_signed(); }
		inline operator int16_t() const { return (int16_t) get_signed(); }
		inline operator int32_t() const { return (int32_t) get_signed(); }
		inline operator int64_t() const { return get_signed(); }
		inline operator float() const { return (float) get_float(); }
		inline operator double() const { return get_float(); }
		inline operator const std::string&() const { return get_string(); }
		inline operator const char*() const { return get_string().c_str(); }
		inline operator const array_t&() const { return get_array(); }
		inline operator const object_t&() const { return get_object(); }

		inline operator bool&() { return get_bool(); }
		inline operator char&() { return get_char(); }
		inline operator uint64_t&() { return get_unsigned(); }
		inline operator int64_t&() { return get_signed(); }
		inline operator double&() { return get_float(); }
		inline operator std::string&() { return get_string(); }
		inline operator array_t&() { return get_array(); }
		inline operator object_t&() { return get_object(); }
	};

	namespace implementation {
		template<const value::type TYPE> struct value_type { typedef void type; };
		template<> struct value_type<value::TYPE_BOOL> { typedef bool type; };
		template<> struct value_type<value::TYPE_CHAR> { typedef char type; };
		template<> struct value_type<value::TYPE_UNSIGNED> { typedef uint64_t type; };
		template<> struct value_type<value::TYPE_SIGNED> { typedef int64_t type; };
		template<> struct value_type<value::TYPE_FLOAT> { typedef double type; };
		template<> struct value_type<value::TYPE_POINTER> { typedef void* type; };
		template<> struct value_type<value::TYPE_STRING> { typedef std::string type; };
		template<> struct value_type<value::TYPE_ARRAY> { typedef value::array_t type; };
		template<> struct value_type<value::TYPE_OBJECT> { typedef value::object_t type; };
	}

	template<const value::type TYPE>
	using type = typename implementation::value_type<TYPE>::type;
}}

#endif