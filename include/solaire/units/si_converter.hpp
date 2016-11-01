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

#ifndef SOLAIRE_UNITS_SI_CONVERTER_HPP
#define SOLAIRE_UNITS_SI_CONVERTER_HPP

namespace solaire {
	enum si_unit : uint32_t {
		SI_NONE,
		SI_YOTTA,
		SI_ZETTA,
		SI_EXA,
		SI_PETA,
		SI_TERA,
		SI_GIGA,
		SI_MEGA,
		SI_KILO,
		SI_MILLI,
		SI_MICRO,
		SI_NANO,
		SI_PICO,
		SI_FEMTO,
		SI_ATTO,
		SI_ZEPTO,
		SI_YOCTO
	};

	enum {
		SI_OFFSET = 8
	};

	#define NON_SI_UNIT(aUnit, aIndex) aUnit = aIndex << SI_OFFSET

	#define SI_UNIT(aUnitPrefix, aUnit)\
		aUnitPrefix ## YOTTA ## aUnit = SI_YOTTA | aUnitPrefix ## aUnit,\
		aUnitPrefix ## ZETTA ## aUnit = SI_ZETTA | aUnitPrefix ## aUnit,\
		aUnitPrefix ## EXA ## aUnit = SI_EXA | aUnitPrefix ## aUnit,\
		aUnitPrefix ## PETA ## aUnit = SI_PETA | aUnitPrefix ## aUnit,\
		aUnitPrefix ## TERA ## aUnit = SI_TERA | aUnitPrefix ## aUnit,\
		aUnitPrefix ## GIGA ## aUnit = SI_GIGA | aUnitPrefix ## aUnit,\
		aUnitPrefix ## MEGA ## aUnit = SI_MEGA | aUnitPrefix ## aUnit,\
		aUnitPrefix ## KILO ## aUnit = SI_KILO | aUnitPrefix ## aUnit,\
		aUnitPrefix ## MILLI ## aUnit = SI_MILLI | aUnitPrefix ## aUnit,\
		aUnitPrefix ## MICRO ## aUnit = SI_MICRO | aUnitPrefix ## aUnit,\
		aUnitPrefix ## NANO ## aUnit = SI_NANO | aUnitPrefix ## aUnit,\
		aUnitPrefix ## PICO ## aUnit = SI_PICO | aUnitPrefix ## aUnit,\
		aUnitPrefix ## FEMTO ## aUnit = SI_FEMTO | aUnitPrefix ## aUnit,\
		aUnitPrefix ## ATTO ## aUnit = SI_ATTO | aUnitPrefix ## aUnit,\
		aUnitPrefix ## ZEPTO ## aUnit = SI_ZEPTO | aUnitPrefix ## aUnit,\
		aUnitPrefix ## YOCTO ## aUnit = SI_YOCTO | aUnitPrefix ## aUnit
		
	namespace implementation {
		template<>
		struct unit_definition<si_unit> {
			static constexpr si_unit DEFAULT_unit = SI_NONE;
		
			static constexpr double SCALES[] = {
				1.0,	// None
				1e+24,	// Yotta
				1e+21,	// Zetta
				1e+18,	// Exa
				1e+15,	// Peta
				1e+12,	// Tera
				1e+9,	// Giga
				1e+6,	// Mega
				1e+3,	// Kilo
				1e-3,	// Milli
				1e-6,	// Micro
				1e-9,	// Nano
				1e-12,	// Pico
				1e-15,	// Femto
				1e-18,	// Atto
				1e-21,	// Zepto
				1e-24	// Yocto
			}; 
			
			static constexpr const char* NAMES[] = {
				"",			// None
				"yotta",	// Yotta
				"zetta",	// Zetta
				"exa"		// Exa
				"peta",		// Peta
				"tera",		// Tera
				"giga",		// Giga
				"mega",		// Mega
				"kilo",		// Kilo
				"milli",	// Milli
				"micro",	// Micro
				"nano",		// Nano
				"pico",		// Pico
				"femto",	// Femto
				"atto",		// Atto
				"zepto",	// Zepto
				"yocto"		// Yocto
			}; 
			
			static constexpr const char* SYMBOLS[] = {
				"",		// None
				"Y",	// Yotta
				"Z",	// Zetta
				"E"		// Exa
				"P",	// Peta
				"T",	// Tera
				"G",	// Giga
				"M",	// Mega
				"K",	// Kilo
				"m",	// Milli
				"u",	// Micro
				"n",	// Nano
				"p",	// Pico
				"f",	// Femto
				"a",	// Atto
				"z",	// Zepto
				"y"		// Yocto
			};
		};
	}

	template<class T>
	class si_converter {
	public:
		typedef si_unit unit;
		typedef T type;
		static constexpr unit get_default_unit() { return SI_NONE; }
		static constexpr T convert(const unit aIn, const T aValue, const unit aOut) { return convert_from_default(aOut, convert_to_default(aIn, aValue)); }
		static constexpr T convert(const T aValue, const unit aOut) { return convert(get_default_unit(), aValue, aOut); }
		static constexpr T convert(const unit aIn, const T aValue) { return convert(aIn, aValue, get_default_unit());}
		static constexpr T convert_to_default(const unit aUnit, const T aValue) { return aValue * static_cast<T>(implementation::unit_definition<si_unit>::SCALES[aUnit]); }
		static constexpr T convert_from_default(const unit aUnit, const T aValue) { return aValue / static_cast<T>(implementation::unit_definition<si_unit>::SCALES[aUnit]); }
		
		template<class UNIT2>
		static constexpr unit extract_prefix(const UNIT2 aUnit) { return static_cast<unit>(aUnit & UINT8_MAX); }
		
		template<class UNIT2>
		static constexpr UNIT2 extract_unit(const UNIT2 aUnit) { return static_cast<UNIT2>(aUnit & (~UINT8_MAX)); }
		
		static constexpr const char* get_name(const unit aUnit) { return implementation::unit_definition<si_unit>::NAMES[aUnit]; }
		static constexpr const char* get_symbol(const unit aUnit) { return implementation::unit_definition<si_unit>::SYMBOLS[aUnit]; }
	};
}