#ifndef SOLAIRE_CORE_TYPE_STRUCT_HPP
#define SOLAIRE_CORE_TYPE_STRUCT_HPP

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

namespace solaire {

	struct type_struct {
		void(*call_destructor)(void*);
		void(*call_default_constructor)(void*);
		void(*call_copy_constructor)(void*, const void*);
		void(*call_move_constructor)(void*, void*);
		void(*call_copy_assign)(void*, const void*);
		void(*call_move_assign)(void*, void*);
		uint32_t size;

		template<class T>
		static const type_struct& create() {
			static type_struct OBJ = {
				[](void* aObj) { static_cast<T*>(aObj)->~T(); },
				[](void* aObj) { new(aObj) T(); },
				[](void* aObj, const void* aOther) { new(aObj) T(*static_cast<const T*>(aOther)); },
				[](void* aObj, void* aOther) { new(aObj) T(std::move(*static_cast<T*>(aOther))); },
				[](void* aObj, const void* aOther) { *static_cast<T*>(aObj) = *static_cast<const T*>(aOther); },
				[](void* aObj, void* aOther) { *static_cast<T*>(aObj) = std::move(*static_cast<T*>(aOther)); },
				sizeof(T)
			};
			return OBJ;
		}
	};
}

#endif