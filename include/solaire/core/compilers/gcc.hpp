#ifndef SOLAIRE_CORE_COMPILER_GCC_HPP
#define SOLAIRE_CORE_COMPILER_GCC_HPP

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

#define SOLAIRE_COMPILER SOLAIRE_GCC
#pragma message("SOLAIRE_COMPILER = SOLAIRE_GCC")
#define SOLAIRE_FORCE_INLINE inline __attribute__((always_inline))
#define SOLAIRE_COMPILER_VER_MAJOR __GNUC__
#define SOLAIRE_COMPILER_VER_MINOR __GNUC_MINOR__



#endif