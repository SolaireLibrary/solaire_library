#ifndef SOLAIRE_CORE_TREE_NODE_HPP
#define SOLAIRE_CORE_TREE_NODE_HPP

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

#include <cstdint>

namespace solaire {
	template<class T, uint8_t CHILDREN>
	class tree_node {
	public:
		typedef T type;
		typedef tree_node<T, CHILDREN> node_t;
	private:
		static_array_list<node_t*,CHILDREN,uint8_t> mChildren;
		node_t* mParent;

		tree_node(const node_t&) = delete;
		node_t& operator=(const node_t&) = delete;
		tree_node(node_t&&) = delete;
		node_t& operator=(node_t&&) = delete;
	public:
		T value;

		constexpr size_t children() const								{ return mChildren.size(); }
		inline node_t& prune()											{ return prune(*this); }
		constexpr bool is_leaf() const									{ return mChildren.size() == 0; }
		constexpr bool is_root() const									{ return mParent ? false : true; }
		inline node_t& get_root()										{ return *(mParent ? mParent->get_root() : this); }
		constexpr const node_t& get_root() const						{ return *(mParent ? mParent->get_root() : this); }
		inline node_t& operator[](const size_t aIndex)					{ return *mChildren[aIndex]; }
		constexpr const node_t& operator[](const size_t aIndex) const	{ return *mChildren[aIndex]; }

		constexpr tree_node() :
			mChildren(),
			mParent(nullptr),
			value()
		{}

		constexpr tree_node(const T& aValue) :
			mChildren(),
			mParent(nullptr),
			value(aValue)
		{}

		~tree_node() {
			for(node_t* i : mChildren) i->mParent = nullptr;
			mChildren.clear();
		}

		size_t size() const {
			size_t s = 1;
			for(const node_t* i : mChildren) s += i->size();
			return s;
		}

		node_t& prune(const size_t aIndex) {
			node_t* const tmp = mChildren[aIndex];
			mChildren.erase(mChildren.begin() + aIndex);
			tmp->mParent = nullptr;
			return *tmp;
		}

		node_t& prune(const node_t& aNode) {
			const size_t s = mChildren.size();
			for(size_t i = 0; i < s; ++i) if(mChildren[i] == &aNode) {
				return prune(i);
			}
			throw std::runtime_error("solaire::tree_node::prune : Node is not a child");
		}

		node_t& append(node_t& const aChild) {
			if(aChild.mParent != nullptr) aChild.prune();
			mChildren.push_back(&aChild);
			aChild.mParent = this;
			return aChild;
		}
	};

	template<class F, class N>
	bool depth_first(const F& aFn, N& aNode) {
		const size_t s = aNode.children();
		if(aFn(aNode.value)) return true;
		for(size_t i = 0; i < s; ++i) {
			if(depth_first(aFn, aNode[i])) return true;
		}
		return false;
	}
}

#endif