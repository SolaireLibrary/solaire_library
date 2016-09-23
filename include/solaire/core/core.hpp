#ifndef SOLAIRE_CORE_CORE_HPP
#define SOLAIRE_CORE_CORE_HPP

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
#include <stdexcept>
#include "core.hpp"

// Define constants
	#define SOLAIRE_CORE 1

	#define SOLAIRE_ERROR 				0

	#define SOLAIRE_WINDOWS				1
	#define SOLAIRE_LINUX				2
	#define SOLAIRE_MAC					3
	#define SOLAIRE_ANDROID				4
	#define SOLAIRE_BSD					5

	#define SOLAIRE_MSVC				1
	#define SOLAIRE_GCC					2
	#define SOLAIRE_INTEL_COMPILER		3
	#define SOLAIRE_CLANG				4

	#define SOLAIRE_X86					1
	#define SOLAIRE_x64					2
	#define SOLAIRE_ARM					3
	#define SOLAIRE_ITANIUM				4
	#define SOLAIRE_POWERPC				5

	#define SOLAIRE_CPP_98              1998
	#define SOLAIRE_CPP_03              2003
	#define SOLAIRE_CPP_11              2011
	#define SOLAIRE_CPP_14              2014
	#define SOLAIRE_CPP_17              2017

	#define SOLAIRE_STATIC_COMPILE			1
	#define SOLAIRE_DYNAMIC_COMPILE			2

// Detect OS
	#if (defined(_WIN16) || defined(_WIN32) || defined(_WIN64)) && ! defined(SOLAIRE_OS)
		#include "solaire/core/os/windows.hpp"
	#endif
	
	#if defined(__ANDROID__) && ! defined(SOLAIRE_OS)
		#include "solaire/core/os/amdrpod.hpp"
	#endif
	
	#if defined(__linux__) && ! defined(SOLAIRE_OS)
		#include "solaire/core/os/linux.hpp"
	#endif
	
	#if defined(__APPLE__) && ! defined(SOLAIRE_OS)
		#include "solaire/core/os/mac.hpp"
	#endif
	
	#if (defined(_FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__bsdi__) || defined(__DragonFly__)) && ! defined(SOLAIRE_OS)
		#include "solaire/core/os/bsd.hpp"
	#endif
	
	#ifndef SOLAIRE_OS
		#include "solaire/core/os/default.hpp"
	#endif

// Detect Instruction set

	#if defined(_WIN16)
		#define SOLAIRE_DEFAULT_WORD_SIZE 16
	#else
		#define SOLAIRE_DEFAULT_WORD_SIZE 32
	#endif

	#if defined(_M_AMD64) || defined(__amd64__) || defined(__x86_64__)
		#define SOLAIRE_INSTRUCTION_SET SOLAIRE_X64
		#define SOLAIRE_WORD_SIZE 64
		#pragma message("SOLAIRE_INSTRUCTION_SET = SOLAIRE_X64")
	#endif

	#if defined(_M_IX86) || defined(__i386__) || defined(_X86_) || defined(__I86__)
		#define SOLAIRE_INSTRUCTION_SET SOLAIRE_X86
		#define SOLAIRE_WORD_SIZE 32
		#pragma message("SOLAIRE_INSTRUCTION_SET = SOLAIRE_X86")
	#endif

	#if defined(_M_ARM) || defined(_M_ARMT) || defined(__arm__) || defined(__thumb__)
		#define SOLAIRE_INSTRUCTION_SET SOLAIRE_ARM
		#if defined(__aarch64__)
			#define SOLAIRE_WORD_SIZE 64
		#else
			#define SOLAIRE_WORD_SIZE SOLAIRE_DEFAULT_WORD_SIZE
		#endif
		#pragma message("SOLAIRE_INSTRUCTION_SET = SOLAIRE_ARM")
	#endif

	#if defined(_M_IA64) || defined(__ia64__) || defined(__itanium__)
		#define SOLAIRE_INSTRUCTION_SET SOLAIRE_ITANIUM
		#define SOLAIRE_WORD_SIZE 64
		#pragma message("SOLAIRE_INSTRUCTION_SET = SOLAIRE_ITANIUM")
	#endif

	#if defined(_M_PPC) || defined(__powerpc__) || defined(__powerpc64__)
		#define SOLAIRE_INSTRUCTION_SET SOLAIRE_POWERPC
		#if defined(__powerpc64__)
			#define SOLAIRE_WORD_SIZE 64
		#else
			#define SOLAIRE_WORD_SIZE SOLAIRE_DEFAULT_WORD_SIZE
		#endif
		#pragma message("SOLAIRE_INSTRUCTION_SET = SOLAIRE_POWERPC")
	#endif

	#ifndef SOLAIRE_INSTRUCTION_SET
        #define SOLAIRE_INSTRUCTION_SET SOLAIRE_ERROR
        #define SOLAIRE_WORD_SIZE SOLAIRE_DEFAULT_WORD_SIZE
		#pragma message("SOLAIRE_INSTRUCTION_SET = SOLAIRE_ERROR")
	#endif

	#undef SOLAIRE_DEFAULT_WORD_SIZE

// Detect compiler

	#if defined(__INTEL_COMPILER) && ! defined(SOLAIRE_COMPILER)
		#include "solaire/core/compilers/intel.hpp"
	#endif

	#if defined(_MSC_VER) && ! defined(SOLAIRE_COMPILER)
		#include "solaire/core/compilers/msvc.hpp"
	#endif

	#if defined(__clang__) && ! defined(SOLAIRE_COMPILER)
		#include "solaire/core/compilers/clang.hpp"
	#endif

	#if (defined(__GNUC__) || defined(__MINGW32__)) && ! defined(SOLAIRE_COMPILER)
		#include "solaire/core/compilers/gcc.hpp"
	#endif

	#ifndef SOLAIRE_COMPILER
		#include "solaire/core/compilers/default.hpp"
	#endif

// Define missing macros

	#ifndef SOLAIRE_COMPILE_MODE
		#define SOLAIRE_COMPILE_MODE SOLAIRE_STATIC_COMPILE
	#endif

// define language extensions

namespace solaire {
	template<class R, class ...PARAMS>
	using function_ptr = R(*)(PARAMS...);

	template<class CLASS, class R, class ...PARAMS>
	using member_function_ptr = R(CLASS::*)(PARAMS...);

	template<class CLASS, class R, class ...PARAMS>
	using const_member_function_ptr = R(CLASS::*)(PARAMS...) const;

	template<class CLASS, class T>
	using member_data_ptr = T CLASS::*;
}

    #if SOLAIRE_CPP_VER >= SOLAIRE_CPP_11
        #define SOLAIRE_CONSTEXPR_11 constexpr
        #define SOLAIRE_CONSTEXPR_I11 constexpr
        #define SOLAIRE_CONSTEXPR_C11 constexpr
    #else
        #define SOLAIRE_CONSTEXPR_11
        #define SOLAIRE_CONSTEXPR_I11 inline
        #define SOLAIRE_CONSTEXPR_C11 static const
    #endif

    #if SOLAIRE_CPP_VER >= SOLAIRE_CPP_14
        #define SOLAIRE_CONSTEXPR_14 constexpr
        #define SOLAIRE_CONSTEXPR_I14 constexpr
        #define SOLAIRE_CONSTEXPR_C14 constexpr
    #else
        #define SOLAIRE_CONSTEXPR_14
        #define SOLAIRE_CONSTEXPR_I14 inline
        #define SOLAIRE_CONSTEXPR_C14 static const
    #endif

    #if SOLAIRE_CPP_VER >= SOLAIRE_CPP_17
        #define SOLAIRE_CONSTEXPR_17 constexpr
        #define SOLAIRE_CONSTEXPR_I17 constexpr
        #define SOLAIRE_CONSTEXPR_C17 constexpr
    #else
        #define SOLAIRE_CONSTEXPR_17
        #define SOLAIRE_CONSTEXPR_I17 inline
        #define SOLAIRE_CONSTEXPR_C17 static const
    #endif

	#define SOLAIRE_EXPORT_CLASS class
	#define SOLAIRE_EXPORT_INTERFACE class

	#ifndef SOLAIRE_COMPILE_MODE
		#define SOLAIRE_COMPILE_MODE SOLAIRE_STATIC_COMPILE
	#endif

	#ifndef SOLAIRE_EXPORT_CALL
		#if SOLAIRE_INSTRUCTION_SET == SOLAIRE_X86 || SOLAIRE_INSTRUCTION_SET == SOLAIRE_X64
			#define SOLAIRE_EXPORT_CALL __cdecl
		#else
			#define SOLAIRE_EXPORT_CALL
		#endif
	#endif

	#ifndef SOLAIRE_INTERFACE_CALL
		#if SOLAIRE_INSTRUCTION_SET == SOLAIRE_X86 || SOLAIRE_INSTRUCTION_SET == SOLAIRE_X64
			//#define SOLAIRE_INTERFACE_CALL __stdcall
			#define SOLAIRE_INTERFACE_CALL
		#else
			#define SOLAIRE_INTERFACE_CALL
		#endif
	#endif

	#ifndef runtime_assert
		#if	SOLAIRE_COMPILER == SOLAIRE_MSVC
			#define runtime_assert(aCondition, aMessage) if(!(aCondition)) throw std::exception(aMessage);
		#else
			#define runtime_assert(aCondition, aMessage) if(!(aCondition)) throw std::runtime_error(aMessage);
		#endif
	#endif

#endif
