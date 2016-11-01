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

#ifndef SOLAIRE_UNITS_CONVERTER_HPP
#define SOLAIRE_UNITS_CONVERTER_HPP

namespace solaire {
	template<class T, class UNIT>
	class converter {
	public:
		typedef UNIT unit;
		typedef T type;
	protected:
		typedef implementation::unit_definition<UNIT> helper;

		static const char* get_name(const unit aUnit, const char* const* aPrefixs, const char* const* aNames) {
			static char BUFFER[64];
			const si_unit p = si_converter<T>::extract_prefix<unit>(aUnit);
			if(p == SI_NONE) return helper::NAMES[aUnit >> SI_OFFSET];
			const unit u = si_converter<T>::extract_unit<unit>(aUnit);
			strcpy(BUFFER, aPrefixs[p]);
			strcat(BUFFER, aNames[u >> SI_OFFSET]);
			return BUFFER;
		}
	public:
	};

	template<class T, class UNIT>
	class static_converter : public converter<T, UNIT>{
	private:
		static constexpr T convert_to_default(const si_unit aPrefix, const unit aUnit, const T aValue) {
			return helper::convert_to_default<T, UNIT>(aUnit, si_converter<T>::convert_to_default(aPrefix, aValue));
		}
		
		static constexpr T convert_from_default(const si_unit aPrefix, const unit aUnit, const T aValue) {
			return si_converter<T>::convert(helper::convert_from_default<T, UNIT>(aUnit, aValue), aPrefix);
		}
	public:
		static constexpr unit get_default_unit() { return helper::DEFAULT_UNIT; }
		static constexpr T convert(const UNIT aIn, const T aValue, const UNIT aOut) { return convert_from_default(aOut, convert_to_default(aIn, aValue)); }
		static constexpr T convert(const T aValue, const UNIT aOut) { return convert_from_default(aOut, aValue); }
		static constexpr T convert(const UNIT aIn, const T aValue) { return convert_to_default(aIn, aValue); }
		
		static constexpr T convert_to_default(const unit aUnit, const T aValue) {
			return convert_to_default(
				si_converter<T>::extract_prefix<unit>(aUnit),
				si_converter<T>::extract_unit<unit>(aUnit),
				aValue
			);
		}
		
		static constexpr T convert_from_default(const unit aUnit, const T aValue) {
			return convert_from_default(
				si_converter<T>::extract_prefix<unit>(aUnit),
				si_converter<T>::extract_unit<unit>(aUnit),
				aValue
			);
		}


		static const char* get_name(const unit aUnit) {
			return converter<T,UNIT>::get_name(aUnit, implementation::unit_definition<si_unit>::NAMES, helper::NAMES);
		}

		static const char* get_symbol(const unit aUnit) {
			return converter<T, UNIT>::get_name(aUnit, implementation::unit_definition<si_unit>::SYMBOLS, helper::SYMBOLS);
		}
	};
}