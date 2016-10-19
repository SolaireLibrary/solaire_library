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

	typedef implementation::xml_element element;

	bool is_number(const char* const aValue) {
		bool exponent = false;
		char last = '\0';
		const char* i = aValue;
		while(*i != '\0') {
			switch(*i) {
			case '-':
				if(! (i == aValue || last == 'e')) return false;
				break;
			case 'E':
			case 'e':
				if(i == aValue || exponent) return false;
				exponent = true;
				break;
			case '+':
				if(last != 'e') return false;
				break;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				break;
			default:
				return true;
			}
			last = *i == 'E' ? 'e' : *i;
			++i;
		}
	}

	void child_value(const element&, value_parser&);
	
	void from_xml_tree(element& aElement, std::istream& aStream) {
		char c;
		
		// Skip whitespace
		while(isspace(aStream.peak()) aStream >> c;
		
		// Open tag
		if(c != '<') throw std::runtime_error("solaire::serial::from_xml : Element must open with '<'");
		
		if(aStream.peak() == '!') {
			aStream >> c;
			aStream >> c;
			if(c != '-') throw std::runtime_error("solaire::serial::from_xml : Malformed opening tag");
			aStream >> c;
			if(c != '-') throw std::runtime_error("solaire::serial::from_xml : Malformed opening tag");
			aElement.is_comment = true;
		}else {
			// Skip whitespace
			while(isspace(aStream.peak()) aStream >> c;
			
			// Read name
			uint8_t name_length = 0;
			char name[64];
			while(true) {
				if(isspace(c) || c == '/' || c == '>') break;
				aStream >> c;
				name[name_length++] = c;
			}
			name[name_length] = '\0';
			aElement.name = name;
			
			// Skip whitespace
			while(isspace(aStream.peak()) aStream >> c;
			
			// Read attributes
			while(c != '/' && c != '>') {
				std::string val;
				name_length = 0;
				while(c != '=') {
					aStream >> c;
					name[name_length++] = c;
				}
				
				if(c != '"') throw std::runtime_error("solaire::serial::from_xml : Attribute must open with '\"'");
				
				while(c != '"') {
					aStream.read(&c, 1);
					val += c;
				}
				
				if(c != '"') throw std::runtime_error("solaire::serial::from_xml : Attribute must close with '\"'");
				
				aElement.attributes.push_back({ std::string(name), val });
			
				// Skip whitespace
				while(isspace(aStream.peak()) aStream >> c;
			}
			
			bool singleton = false;
			
			// Close tag
			if(c == '/') {
				aStream >> c;
				singleton = true;
			}
			
			if(c == '>'){
				aStream >> c;
				// Skip whitespace
				while(isspace(aStream.peak()) aStream >> c;
			}else {
				throw std::runtime_error("solaire::serial::from_xml : Element must open with '<'");
			}
		}
		
		// Read body
		if(singleton) return;
		if(c == '<') {
			// Read child elements
		}else {
			// Read text
			while(c != '<') {
				aElement.body += c;
				aStream.read(&c, 1);
			}
			
			// Trim whitespace
			while(isspace(aElement.body.back())) aElement.body.pop_back();
		}
		
		if(aElement.is_comment) {
			aStream >> c;
			if(c != '-') throw std::runtime_error("solaire::serial::from_xml : Malformed opening tag");
			aStream >> c;
			if(c != '-') throw std::runtime_error("solaire::serial::from_xml : Malformed opening tag");
			aStream >> c;
			if(c != '>') throw std::runtime_error("solaire::serial::from_xml : Malformed opening tag");
		}else {
			// Skip whitespace
			while(isspace(aStream.peak()) aStream >> c;
		
			// Open tag
			if(c != '<') throw std::runtime_error("solaire::serial::from_xml : Element closing tag must open with '</'");
			aStream >> c;
			if(c != '/') throw std::runtime_error("solaire::serial::from_xml : Element closing tag must open with '</'");
			
			// Skip whitespace
			while(isspace(aStream.peak()) aStream >> c;
			
			// Read name
			name_length = 0;
			while(true) {
				if(isspace(c) || c == '>') break;
				aStream >> c;
				name[name_length++] = c;
			}
			name[name_length] = '\0';
			
			// Skip whitespace
			while(isspace(aStream.peak()) aStream >> c;
			
			// Close tag
			if(c != '>') throw std::runtime_error("solaire::serial::from_xml : Element must close with '>'");
		}
	}

	void primative_value(const std::string& aValue, value_parser& aParser) {
		if(aValue.size() == 1) {
			aValue.value_char(aValue[0]);
		}else if(aValue == "void" || aValue == "null") {
			aParser.value_void();
		}else if(aValue == "true" || aValue == "TRUE" || aValue == "True") {
			aParser.value_bool(true);
		}else if(aValue == "false" || aValue == "FALSE" || aValue == "False") {
			aParser.value_bool(false);
		}else if(is_number(aValue.c_str())) {
			double tmp;
			sscanf(aValue.c_str(), "%lf", &tmp);
			aParser.value_float(tmp);
		}else {
			aParser.value_string(aValue.c_str());
		}
		//! \todo Handle pointers
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

	void child_object(element& aElement, value_parser& aParser, const xml_conflict_mode aConflictMode) {
		aParser.begin_object();
		for(element& i : aElement.children) {
			aParser.name(i.name.c_str());
			child_value(aElement, aParser, aConflictMode);
		}
		aParser.end_object();
	}

	void child_array(element& aElement, value_parser& aParser, const xml_conflict_mode aConflictMode) {
		aParser.begin_array();
		for(element& i : aElement.children) {
			child_value(aElement, aParser, aConflictMode);
		}
		aParser.begin_array();
	}

	void attribute_child_object(element& aElement, value_parser& aParser, const xml_conflict_mode aConflictMode) {
		aParser.begin_object();
		for(const element::attribute& i : aElement.attributes) {
			primative_value(i.first, i.second, aParser);
		}
		for(element& i : aElement.children) {
			aParser.name(i.name.c_str());
			child_value(aElement, aParser, aConflictMode);
		}
		aParser.end_object();
	}

	void attribute_child_array(element& aElement, value_parser& aParser, const xml_conflict_mode aConflictMode) {
		aParser.begin_array();
		for(const element::attribute& i : aElement.attributes) {
			primative_value(i.second, aParser);
		}
		for(element& i : aElement.children) {
			child_value(aElement, aParser, aConflictMode);
		}
		aParser.begin_array();
	}

	bool is_attribute_array(const element& aElement) {
		uint32_t i = 0;
		char buf[32];
		for(const element::attribute& j : aElement.attributes) {
			for(const char c : j.first) if (c < '0' || c >= '9') return false;
			uint64_t tmp;
			sscanf(buf, "%llu", &tmp);
			if(tmp != i) return false;
			++i;
		}
		return true;
	}

	bool is_child_array(const element& aElement) {
		for(const element& i : aElement.children) {
			if(i.name == "SOLAIRE_XML_VALUE") continue;
			else if(i.name == "SOLAIRE_XML_ARRAY") continue;
			else if(i.name == "SOLAIRE_XML_OBJECT") continue;
			else return false;
		}
		return true;
	}

	void pure_attribute(element& aElement, value_parser& aParser, const xml_conflict_mode aConflictMode) {
		if(aElement.attributes.size() == 1 && aElement.attributes[0].first == "SOLAIRE_XML_VALUE") primative_value(aElement.attributes[0].second, aParser);
		else if(is_attribute_array(aElement)) attribute_array(aElement, aParser, aConflictMode);
		else attribute_object(aElement, aParser, aConflictMode);
	}

	void pure_children(element& aElement, value_parser& aParser, const xml_conflict_mode aConflictMode) {
		if(aElement.children.size() == 1) child_value(aElement.children[0], aParser, aConflictMode);
		else if(is_child_array(aElement)) child_array(aElement, aParser, aConflictMode);
		else child_object(aElement, aParser, aConflictMode);
	}

	void mixed_attribute_children(element& aElement, value_parser& aParser, const xml_conflict_mode aConflictMode) {
		const bool aa = is_attribute_array(aElement);
		const bool oa = is_child_array(aElement);
		if(aa && oa) attribute_child_array(aElement, aParser, aConflictMode);
		else attribute_child_object(aElement, aParser, aConflictMode);
	}

	void child_value(element& aElement, value_parser& aParser) {
		bool body = ! aElement.body.empty();
		bool attributes = ! aElement.attributes.empty();
		const bool children = ! aElement.children.empty();

		if(attributes || children) {
			if(body) {
				aElement.attributes.push_back({ "SOLAIRE_XML_BODY", aElement.body });
				aElement.body.clear();
				attributes = true;
				body = false;
			}
			if(attributes && ! children) {
				pure_attribute(aElement, aParser, aConflictMode);
			}else if (children && ! attributes) {
				pure_children(aElement, aParser, aConflictMode);
			}else {
				mixed_attribute_children(aElement, aParser, aConflictMode);
			}
		}else if(body) {
			body_value(aElement, aParser, aConflictMode);
		}else {
			aParser.value_void();
		}
	}
	
	void from_xml(std::istream& aStream, value_parser& aParser) {
		child_value(implementation::xml_element::read(aStream), aParser, aConflictMode);
	}
	
	namespace implementation {
		// xml_element
		
		void xml_element::write(std::ostream& aStream, const xml_element& aElement) {
			//! \todo Write header element
			
			if(aElement.is_comment) {
				aStream << "<!-- " << aElement.name << " -->";
			}else {
				aStream << '<' << aElement.name;
				for(const xml_element::attribute& i : aElement.attributes) {
					aStream << ' ' << i.first << '=' << '"' << i.second << '"';
				}
				
				if(aStream.body.empty()) {
					if(aElement.children.empty()) {
						aStream << '/' << '>';
						return;
					}else {
						aStream << '>';
						for(const xml_element& i : aElement.children) {
							write(aStream, i);
						}
					}
				}else {
					aStream << '>' << aElement.body;
				}
				
				aStream << '<' << '/' << aElement.name << '>';
			}
		}
		
		xml_element xml_element::read(std::istream& aStream) {
			//! \todo Read header element
			xml_element root;
			from_xml_tree(root, aStream);
			return root;
		}
		
		xml_element::xml_element() :
			attributes(0),
			children(0),
			body(),
			name(),
			is_comment(false)
		{}
	}
	
	// to_xml

	//! \todo Add header element
	
	to_xml::to_xml(std::ostream& aStream, const primative_value_mode aPrimitiveMode) :
		mStream(aStream),
		mPrimitiveMode(aPrimitiveMode)
	{}

	void to_xml::write_value(const char* const aValue) {
		const state& s = mStateStack.back();
		
		const auto as_attribute = [&]() {
			s.element->attributes.push_back({ mName, std::string(aValue) });
			mName.clear();
		};
		
		const auto as_child = [&]() {
			as_child();
			element e;
			std::swap(e.name, mName);
			e.body = aValue;
			s.element->children.push_back(e);
		};
		
		if(s.is_object){
			if(mPrimitiveMode == PRIMITIVE_AS_ATTRIBUTE) {
				as_attribute();
			}else if(mPrimitiveMode == PRIMITIVE_AS_ELEMENT) {
				as_child();
			else {
				throw std::runtime_error("solaire::serial::to_xml::write_value : Invalid primitive encoding");
			}
		}else { 
			mName = "SOLAIRE_XML_VALUE";
			as_child();
			mName.clear()
		}
	}
	
	void to_xml::begin_array() {
		if(mStateStack.empty() || mStateStack.back().is_array) mName = "SOLAIRE_XML_ARRAY";
		state tmp;
		tmp.is_array = 1;
		if(mStateStack.empty()){
			tmp.element = &mRoot;
		}else {
			const state& s = mStateStack.back();
			element e;
			std::swap(mName, e.name);
			s.element->children.push_back(e);
			tmp.element = &s.element->children.back();
		}
		mStateStack.push_back(tmp);
	}
	
	void to_xml::end_array() {
		const state tmp = mStateStack.back();
		if(! tmp.is_array) throw std::runtime_error("solaire::serial::to_xml::end_array : Current value is not array");
		mStateStack.pop_back();
		if(mStateStack.empty()) element::write(mStream, mRoot);
	}
	
	void to_xml::begin_object() {
		if(mStateStack.empty() || mStateStack.back().is_array) mName = "SOLAIRE_XML_OBJECT";
		state tmp;
		tmp.is_object = 1;
		if(mStateStack.empty()){
			tmp.element = &mRoot;
		}else {
			const state& s = mStateStack.back();
			element e;
			std::swap(mName, e.name);
			s.element->children.push_back(e);
			tmp.element = &s.element->children.back();
		}
		mStateStack.push_back(tmp);
	}
	
	void to_xml::end_object() {
		const state tmp = mStateStack.back();
		if(! tmp.is_object) throw std::runtime_error("solaire::serial::to_xml::end_object : Current value is not object");
		mStateStack.pop_back();
		if(mStateStack.empty()) element::write(mStream, mRoot);
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