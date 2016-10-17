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

	struct element {
		typedef std::pair<std::string, std::string> attribute;

		std::vector<attribute> attributes;
		std::vector<element> children;
		std::string body;
		std::string name;
	};

	element from_xml_tree(std::istream& aStream) {
		//! \todo Implement
		return element();
	}

	void primative_value(const std::string& aName, const std::string& aValue, value_parser& aParser) {
		//! \todo Implement
	}

	void from_xml(const element& aElement, value_parser& aParser) {
		if(aElement.body.empty()) {
			if(aElement.children.empty()) {
				// No value
				aParser.value_void();
			}else {
				if(aElement.attributes.size() == 1 && aElement.attributes[0].first == "value") {
					// Attribute value
					primative_value(aElement.name, aElement.attributes[0].second, aParser);
				}else {
					if(aElement.attributes.empty()) {
						const std::string& name = aElement.children[0].name;
						if(name == "value" || name == "array" || name == "object") {
							// Children array
							aParser.begin_array();
							for(const element& i : aElement.children) from_xml(aElement, aParser);
							aParser.end_array();
						}else {
							// Children object
							aParser.begin_object();
							for(const element& i : aElement.children) {
								aParser.name(i.name.c_str());
								from_xml(aElement, aParser);
							}
							aParser.end_object();
						}
					}else {
						// Attribute / children object
						aParser.begin_object();
						for(const element::attribute& i : aElement.attributes) {
							primative_value(i.first, i.second, aParser);
						}
						for(const element& i : aElement.children) {
							aParser.name(i.name.c_str());
							from_xml(aElement, aParser);
						}
						aParser.end_object();
					}
				}
			}
		}else {
			if(aElement.children.empty()) {
				// Body value
				primative_value(aElement.name, aElement.body, aParser);
			}else {
				throw std::runtime_error("solaire::serial::from_xml : Cannot decode element with attributes and body");
			}
		}
	}
	
	void from_xml(std::istream& aStream, value_parser& aParser) {
		from_xml(from_xml_tree(aStream), aParser);
	}
	
	// to_xml

	//! \todo Add header element
	
	to_xml::to_xml(std::ostream& aStream) :
		mStream(aStream)
	{}

	void to_xml::write_value(const char* const aValue) {
		state& tmp = mStateStack.back();
		++tmp.size;
		if(tmp.is_array) mName = "value";
		mStream << '<' << mName.c_str() << " value=\"" << aValue << "\"/>";
	}
	
	void to_xml::begin_array() {
		if(mStateStack.empty() || mStateStack.back().is_array) mName = "array";
		state tmp;
		tmp.size = 0;
		tmp.is_object = 1;
		mStateStack.push_back(tmp);
		mStream << '<' << mName.c_str() << '>';
	}
	
	void to_xml::end_array() {
		const state tmp = mStateStack.back();
		mStateStack.pop_back();
		if (tmp.is_object) mName = "array";
		mStream << '</' << mName.c_str() << '>';
	}
	
	void to_xml::begin_object() {
		if(mStateStack.empty() || mStateStack.back().is_array) mName = "object";
		state tmp;
		tmp.size = 0;
		tmp.is_object = 1;
		mStateStack.push_back(tmp);
		mStream << '<' << mName.c_str() << '>';
	}
	
	void to_xml::end_object() {
		const state tmp = mStateStack.back();
		mStateStack.pop_back();
		if(tmp.is_object) mName = "object";
		mStream << '<' << '/' << mName.c_str() << '>';
		//! \todo Write primative members as attributes
	}
	
	void to_xml::name(const char* aName) {
		mName = aName;
	}
	
	void to_xml::value_void() {
		write_value("void");
	}
	
	void to_xml::value_bool(const bool aValue) {
		write_value(aValue ? "true" : "false");
	}
	
	void to_xml::value_char(const char aValue) {
		const char tmp[2] = { aValue, '\0' };
		write_value(tmp);
	}
	
	void to_xml::value_unsigned(const uint64_t aValue) {
		char buffer[32];
		snprintf(buffer, 32, "%llu", aValue);
		write_value(buffer);
	}
	
	void to_xml::value_signed(const int64_t aValue) {
		char buffer[32];
		snprintf(buffer, 32, "%lli", aValue);
		write_value(buffer);
	}
	
	void to_xml::value_float(const double aValue) {
		char buffer[32];
		snprintf(buffer, 32, "%lf", aValue);
		write_value(buffer);
	}
	
	void to_xml::value_pointer(void* const aValue) {
		char buffer[32];
		buffer[0] = '#';
		snprintf(buffer + 1, 31, "%llu", *reinterpret_cast<uint64_t* const*>(&aValue));
		write_value(buffer);
	}
	
	void to_xml::value_string(const char* const aValue) {
		write_value(aValue);
	}
	
}}