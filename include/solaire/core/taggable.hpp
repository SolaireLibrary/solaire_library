#ifndef SOLAIRE_CORE_TAGGABLE_HPP
#define SOLAIRE_CORE_TAGGABLE_HPP

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

#include "core.hpp"
#include "fast_stack.hpp"
//#include "hash.hpp"

namespace solaire {
	//class taggable {
	//public:
	//	virtual ~taggable(){}

	//	virtual bool has_tag(const char* const) const throw() = 0;
	//	virtual bool add_tag(const char* const) throw() = 0;
	//	virtual bool remove_tag(const char* const) throw() = 0;
	//	virtual bool remove_all_tags() throw() = 0;

	//	template<class ITERATOR>
	//	bool has_all_tags(const ITERATOR aBegin, const ITERATOR aEnd) const throw() {
	//		for(ITERATOR i = aBegin; i != aEnd; ++i) if(! has_tag(*i)) return false;
	//		return true;
	//	}

	//	template<class ITERATOR>
	//	bool has_any_tag(const ITERATOR aBegin, const ITERATOR aEnd) const throw() {
	//		for(ITERATOR i = aBegin; i != aEnd; ++i) if(has_tag(*i)) return true;
	//		return false;
	//	}

	//	//template<class T>
	//	inline bool has_all_tags(const std::initializer_list<const char*>& aTags) const throw() {
	//		return has_all_tags(aTags.begin(), aTags.end());
	//	}

	//	//template<class T>
	//	inline bool has_any_tag(const std::initializer_list<const char*>& aTags) const throw() {
	//		return has_any_tag(aTags.begin(), aTags.end());
	//	}
	//};



	//namespace implementation {

	//	template<class HASH>
	//	struct hash_tag {
	//		typename HASH::hash_t hash;

	//		hash_tag() :
	//			hash(0)
	//		{}

	//		hash_tag(const char* const aValue) :
	//			hash(HASH::hash(aValue, strlen(aValue)))
	//		{}

	//		inline hash_tag<HASH>& operator=(const hash_tag<HASH>& aOther) {
	//			hash = aOther.hash;
	//			return *this;
	//		}

	//		inline bool operator==(const hash_tag<HASH>& aOther) const {
	//			return hash == aOther.hash;
	//		}

	//		inline bool operator!=(const hash_tag<HASH>& aOther) const {
	//			return hash != aOther.hash;
	//		}
	//	};

	//	template<const uint32_t MAX_TAG_LENGTH>
	//	struct fixed_tag {
	//		char string[MAX_TAG_LENGTH];

	//		fixed_tag() {
	//			for (uint32_t i = 0; i < MAX_TAG_LENGTH; ++i) string[i] = '\0';
	//		}

	//		fixed_tag(const char* const aValue) {
	//			const uint32_t size = strlen(aValue);
	//			memcpy(string, aValue, size);
	//			for (uint32_t i = size; i < MAX_TAG_LENGTH; ++i) string[i] = '\0';
	//		}

	//		fixed_tag<MAX_TAG_LENGTH>& operator=(const fixed_tag<MAX_TAG_LENGTH>& aOther) {
	//			memcpy(string, aOther.string, MAX_TAG_LENGTH);
	//			return *this;
	//		}

	//		inline bool operator==(const fixed_tag<MAX_TAG_LENGTH>& aOther) const {
	//			return memcmp(string, aOther.string, MAX_TAG_LENGTH) == 0;
	//		}

	//		inline bool operator!=(const fixed_tag<MAX_TAG_LENGTH>& aOther) const {
	//			return memcmp(string, aOther.string, MAX_TAG_LENGTH) != 0;
	//		}
	//	};
	//}

	//template<const uint32_t MAX_TAGS, const uint32_t MAX_TAG_LENGTH, class TAG>
	//class fast_tag_container : public taggable{
	//private:
	//	fast_stack<TAG, MAX_TAGS, uint8_t> mTags;
	//	static_assert(MAX_TAGS < 255, "solaire::fast_tag_container must have a capacity of less than 255");
	//public:
	//	inline bool has_tag(const char* const aTag) const throw() override {
	//		return mTags.find(TAG(aTag)) != mTags.end();
	//	}

	//	inline bool add_tag(const char* const aTag) throw() override {
	//		return mTags.push(TAG(aTag));
	//	}

	//	inline bool remove_tag(const char* const aTag) throw() override {
	//		return mTags.fast_erase(mTags.find(TAG(aTag)));
	//	}

	//	inline bool remove_all_tags() throw() override {
	//		if(mTags.empty()) return false;
	//		mTags.clear();
	//		return true;
	//	}
	//};

	//template<const uint32_t MAX_TAGS, const uint32_t MAX_TAG_LENGTH>
	//using fast_fixed_tag_container = fast_tag_container<MAX_TAGS, MAX_TAG_LENGTH, implementation::fixed_tag<MAX_TAG_LENGTH>>;

	//template<const uint32_t MAX_TAGS, class HASH>
	//using fast_hash_tag_container = fast_tag_container<MAX_TAGS, 0, implementation::hash_tag<HASH>>;

	//template<const uint32_t MAX_TAGS>
	//using fast_add16_tag_container = fast_hash_tag_container<MAX_TAGS, add_hash<uint16_t>>;

}

#endif