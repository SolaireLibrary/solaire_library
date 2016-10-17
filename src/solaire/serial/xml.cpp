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

	void child_value(const element&, value_parser&, const xml_conflict_mode);

	element from_xml_tree(std::istream& aStream) {
		//! \todo Implement
		return element();
	}

	void primative_value(const std::string& aValue, value_parser& aParser) {
		//! \todo Implement
	}

	inline void primative_value(const std::string& aName, const std::string& aValue, value_parser& aParser) {
		aParser.name(aName.c_str());
		primative_value(aValue, aParser);
	}

	inline void body_value(const element& aElement, value_parser& aParser, const xml_conflict_mode aConflictMode) {
		primative_value(aElement.name, aElement.body, aParser);
	}
	
	inline void attribute_value(const element& aElement, value_parser& aParser, const xml_conflict_mode aConflictMode) {
		primative_value(aElement.attributes[0].second, aParser);
	}

	void attribute_object(const element& aElement, value_parser& aParser, const xml_conflict_mode aConflictMode) {
		aParser.begin_object();
		for(const element::attribute& i : aElement.attributes) {
			primative_value(i.first, i.second, aParser);
		}
		aParser.end_object();
	}

	void attribute_array(const element& aElement, value_parser& aParser, const xml_conflict_mode aConflictMode) {
		aParser.begin_array();
		for(const element::attribute& i : aElement.attributes) {
			primative_value(i.second, aParser);
		}
		aParser.begin_array();
	}

	void child_object(const element& aElement, value_parser& aParser, const xml_conflict_mode aConflictMode) {
		aParser.begin_object();
		for(const element& i : aElement.children) {
			aParser.name(i.name.c_str());
			child_value(aElement, aParser, aConflictMode);
		}
		aParser.end_object();
	}

	void child_array(const element& aElement, value_parser& aParser, const xml_conflict_mode aConflictMode) {
		aParser.begin_array();
		for(const element& i : aElement.children) {
			child_value(aElement, aParser, aConflictMode);
		}
		aParser.begin_array();
	}

	void attribute_child_object(const element& aElement, value_parser& aParser, const xml_conflict_mode aConflictMode) {
		aParser.begin_object();
		for(const element::attribute& i : aElement.attributes) {
			primative_value(i.first, i.second, aParser);
		}
		for(const element& i : aElement.children) {
			aParser.name(i.name.c_str());
			child_value(aElement, aParser, aConflictMode);
		}
		aParser.end_object();
	}

	void attribute_child_array(const element& aElement, value_parser& aParser, const xml_conflict_mode aConflictMode) {
		aParser.begin_array();
		for(const element::attribute& i : aElement.attributes) {
			primative_value(i.second, aParser);
		}
		for(const element& i : aElement.children) {
			child_value(aElement, aParser, aConflictMode);
		}
		aParser.begin_array();
	}

	bool is_attribute_array(const element& aElement) {
		//! \todo implement
		return false;
	}

	bool is_object_array(const element& aElement) {
		for(const element& i : aElement.children) {
			if(i.name == "value") continue;
			else if(i.name == "array") continue;
			else if(i.name == "object") continue;
			else return false;
		}
		return true;
	}

	void pure_attribute(const element& aElement, value_parser& aParser, const xml_conflict_mode aConflictMode) {
		if(aElement.attributes.size() == 1 && aElement.attributes[0].first == "value") primative_value(aElement.attributes[0].second, aParser);
		else if(is_attribute_array(aElement)) attribute_array(aElement, aParser, aConflictMode);
		else attribute_object(aElement, aParser, aConflictMode);
	}

	void pure_children(const element& aElement, value_parser& aParser, const xml_conflict_mode aConflictMode) {
		if(aElement.children.size() == 1) child_value(aElement.children[0], aParser, aConflictMode);
		else if(is_object_array(aElement)) child_array(aElement, aParser, aConflictMode);
		else child_object(aElement, aParser, aConflictMode);
	}

	void mixed_attribute_children(const element& aElement, value_parser& aParser, const xml_conflict_mode aConflictMode) {
		const bool aa = is_attribute_array(aElement);
		const bool oa = is_object_array(aElement);
		if(aa && oa) attribute_child_array(aElement, aParser, aConflictMode);
		else if(aa && ! oa) throw std::runtime_error("solaire::serial::from_xml : Attribute array / Child object");
		else if (oa && !aa) throw std::runtime_error("solaire::serial::from_xml : Attribute object / Child array");
		else attribute_child_object(aElement, aParser, aConflictMode);
	}

	void child_value(const element& aElement, value_parser& aParser, const xml_conflict_mode aConflictMode) {
		const bool body = ! aElement.body.empty();
		const bool attributes = ! aElement.attributes.empty();
		const bool children = ! aElement.children.empty();

		if(attributes || children) {
			if(body) {
				switch(aConflictMode)
				{
				case XML_CONFLICT_IGNORE_BODY :
					pure_attribute(aElement, aParser, aConflictMode);
					break;
				case XML_CONFLICT_IGNORE_ATTRIBUTES :
					body_value(aElement, aParser, aConflictMode);
					break;
				default: // XML_CONFLICT_ERROR
					throw std::runtime_error("solaire::serial::from_xml : Element with both body value and attributes (XML_CONFLICT_ERROR)");
				}
			}else {
				if(attributes && ! children) {
					pure_attribute(aElement, aParser, aConflictMode);
				}else if (children && ! attributes) {
					pure_children(aElement, aParser, aConflictMode);
				}else {
					mixed_attribute_children(aElement, aParser, aConflictMode);
				}
			}
		}else if(body) {
			body_value(aElement, aParser, aConflictMode);
		}else {
			aParser.value_void();
		}
	}
	
	void from_xml(std::istream& aStream, value_parser& aParser, const xml_conflict_mode aConflictMode) {
		child_value(from_xml_tree(aStream), aParser, aConflictMode);
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