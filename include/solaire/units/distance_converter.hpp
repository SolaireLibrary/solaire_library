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

#ifndef SOLAIRE_UNITS_DISTANCE_CONVERTER_HPP
#define SOLAIRE_UNITS_DISTANCE_CONVERTER_HPP

namespace solaire {
	enum distance_unit : uint32_t {
		//DIST_METER = 0 << SI_OFFSET,
		NON_SI_UNIT(DIST_METER, 0),
		NON_SI_UNIT(DIST_INCH, 1),
		NON_SI_UNIT(DIST_FOOT, 2),
		NON_SI_UNIT(DIST_YARD, 3),
		NON_SI_UNIT(DIST_MILE, 4),
		NON_SI_UNIT(DIST_NAUTICAL_MILE, 5),
		NON_SI_UNIT(DIST_ASTRONOMICAL_UNIT, 6),
		SI_UNIT(DIST_,METER)
	};

	namespace implementation {
		template<>
		struct unit_definition<distance_unit> {
			static constexpr distance_unit DEFAULT_UNIT = DIST_METER;
		
			static constexpr double SCALES[] = {
				1.0,					// Meter -> Meter
				39.3701,				// Meter -> Inch
				3.28084,				// Meter -> Foot
				1.0936133333333,		// Meter -> Yard
				0.000621371,			// Meter -> Mile
				0.00053995663640604751,	// Meter -> Nautical Mile
				6.68459e-12,			// Meter -> Astronomical Unit
			}; 
			
			static constexpr const char* NAMES[] = {
				"meter"
				"inch",
				"foot",
				"yard",
				"mile",
				"nautical mile",
				"astronomical unit"
			}; 
			
			static constexpr const char* SYMBOLS[] = {
				"m"
				"in",
				"ft",
				"yd",
				"mi",
				"n. mi",
				"au"
			}; 
			
			template<class T, class UNIT>
			static constexpr T convert_to_default(const UNIT aUnit, const T aValue) {
				return aValue / static_cast<T>(SCALES[aUnit >> SI_OFFSET]);
			}
			
			template<class T, class UNIT>
			static constexpr T convert_from_default(const UNIT aUnit, const T aValue) {
				return aValue * static_cast<T>(SCALES[aUnit >> SI_OFFSET]);
			}
		};
	}

	template<class T>
	using distance_converter = static_converter<T, distance_unit>;

	template<class T>
	using distance_value = convertable_value<distance_converter<T>>;
}