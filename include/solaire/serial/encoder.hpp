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

#ifndef SOLAIRE_SERIALISE_ENCODER_HPP
#define SOLAIRE_SERIALISE_ENCODER_HPP

#include "value.hpp"
#include "value_to_parser.hpp"

namespace solaire { namespace serial {
	template<class T, class ENABLE = void>
	struct encoder {
		typedef T decode_t;
		typedef const T& encode_t;
		static T decode(const value&);
		static void encode(const T&, value_parser&);
	};

	template<class T>
	typename encoder<T>::type decode(const value& aValue) {
		return encoder<T>::decode(aValue);
	}

	template<class T>
	typename void encode(typename encoder<T>::decode_t aValue, value_parser& aParser) {
		encoder<T>::encode(aValue, aParser);
	}

	template<class T>
	typename value encode(typename encoder<T>::decode_t aValue) {
		parser_to_value parser;
		encode<T>(aValue, parser);
		return parser.get();
	}

	template<>
	struct encoder<void> {
		typedef void decode_t;
		typedef void encode_t;

		static void decode(const value& aValue) {
			aValue.is_void();
		}

		static void encode(value_parser& aParser) {
			aParser.value_void();
		}
	};

	template<>
	struct encoder<bool> {
		typedef bool decode_t;
		typedef const bool encode_t;

		static decode_t decode(const value& aValue) {
			return aValue.get_bool();
		}

		static void encode(encode_t aValue, value_parser& aParser) {
			aParser.value_bool(aValue);
		}
	};

	template<>
	struct encoder<char> {
		typedef char decode_t;
		typedef const char encode_t;

		static decode_t decode(const value& aValue) {
			return aValue.get_char();
		}

		static void encode(encode_t aValue, value_parser& aParser) {
			aParser.value_char(aValue);
		}
	};

	template<class T>
	struct encoder<T, typename std::enable_if<std::is_same<T, uint8_t>::value || std::is_same<T, uint16_t>::value || std::is_same<T, uint32_t>::value || std::is_same<T, uint64_t>::value>::type> {
		typedef T decode_t;
		typedef const T encode_t;

		static decode_t decode(const value& aValue) {
			return static_cast<T>(aValue.get_unsigned());
		}

		static void encode(encode_t aValue, value_parser& aParser) {
			aParser.value_unsigned(aValue);
		}
	};

	template<class T>
	struct encoder<T, typename std::enable_if<std::is_same<T, int8_t>::value || std::is_same<T, int16_t>::value || std::is_same<T, int32_t>::value || std::is_same<T, int64_t>::value>::type> {
		typedef T decode_t;
		typedef const T encode_t;

		static decode_t decode(const value& aValue) {
			return static_cast<T>(aValue.get_signed());
		}

		static void encode(encode_t aValue, value_parser& aParser) {
			aParser.value_signed(aValue);
		}
	}; 

	template<class T>
	struct encoder<T, typename std::enable_if<std::is_same<T, float>::value || std::is_same<T, double>::value>::type> {
		typedef T decode_t;
		typedef const T encode_t;

		static decode_t decode(const value& aValue) {
			return static_cast<T>(aValue.get_float());
		}

		static void encode(encode_t aValue, value_parser& aParser) {
			aParser.value_float(aValue);
		}
	};

	template<>
	struct encoder<std::string> {
		typedef std::string decode_t;
		typedef const std::string& encode_t;

		static decode_t decode(const value& aValue) {
			return aValue.get_string();
		}

		static void encode(encode_t aValue, value_parser& aParser) {
			aParser.value_string(aValue.c_str());
		}
	};

	template<class A, class B>
	struct encoder<std::pair<A,B>> {
		typedef std::pair<A,B> decode_t;
		typedef const std::pair<A,B>& encode_t;

		static decode_t decode(const value& aValue) {
			return decode_t(
				decode<A>(aValue[0]),
				decode<B>(aValue[1])
			);
		}

		static void encode(encode_t aValue, value_parser& aParser) {
			aParser.begin_array();
			encode<A>(aValue.first, aParser);
			encode<B>(aValue.second, aParser);
			aParser.end_array();
		}
	};

	template<class T>
	struct encoder<std::vector<T>> {
		typedef std::vector<T> decode_t;
		typedef const std::vector<T>& encode_t;

		static decode_t decode(const value& aValue) {
			const value::array_t& array_ = aValue.get_array();
			decode_t tmp(array_.size());
			for(const value& i : array_) tmp.push_back(decode<T>(i));
			return tmp;
		}

		static void encode(encode_t aValue, value_parser& aParser) {
			aParser.begin_array();
			for(const T& i : aValue) encode<T>(i, aParser);
			aParser.end_array();
		}
	};
}}

#endif