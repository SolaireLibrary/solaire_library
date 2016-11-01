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

#ifndef SOLAIRE_UNITS_CONVERTABLE_VALUE_HPP
#define SOLAIRE_UNITS_CONVERTABLE_VALUE_HPP

namespace solaire {
	template<class CONVERTER>
	class convertable_value {
	public:
		typedef typename CONVERTER::type type;
		typedef typename CONVERTER::unit unit;
	private:
		type mValue;
		constexpr convertable_value(const type aValue) : mValue(aValue) {}
	public:
		constexpr convertable_value() : mValue() {}
		constexpr convertable_value(const unit aUnit, const type aValue) : mValue(CONVERTER::convert_to_default(aUnit, aValue)) {}
		constexpr convertable_value(const convertable_value<CONVERTER>& aOther) : mValue(aOther.mValue) {}
		constexpr type get(const unit aUnit) const { return CONVERTER::convert_from_default(mValue, aUnit); }
		constexpr convertable_value operator+(const convertable_value<CONVERTER> aOther) const { return convertable_value(mValue + aOther.mValue); }
		constexpr convertable_value operator-(const convertable_value<CONVERTER> aOther) const { return convertable_value(mValue - aOther.mValue); }
		constexpr convertable_value operator*(const convertable_value<CONVERTER> aOther) const { return convertable_value(mValue * aOther.mValue); }
		constexpr convertable_value operator/(const convertable_value<CONVERTER> aOther) const { return convertable_value(mValue / aOther.mValue); }
		inline convertable_value& operator+=(const convertable_value<CONVERTER> aOther) { mValue += aOther.mValue; return *this; }
		inline convertable_value& operator-=(const convertable_value<CONVERTER> aOther) { mValue -= aOther.mValue; return *this; }
		inline convertable_value& operator*=(const convertable_value<CONVERTER> aOther) { mValue *= aOther.mValue; return *this; }
		inline convertable_value& operator/=(const convertable_value<CONVERTER> aOther) { mValue /= aOther.mValue; return *this; }
		constexpr bool operator==(const convertable_value<CONVERTER> aOther) const { return mValue == aOther.mValue; }
		constexpr bool operator!=(const convertable_value<CONVERTER> aOther) const { return mValue != aOther.mValue; }
		constexpr bool operator<(const convertable_value<CONVERTER> aOther) const { return mValue < aOther.mValue; }
		constexpr bool operator>(const convertable_value<CONVERTER> aOther) const { return mValue > aOther.mValue; }
		constexpr bool operator<=(const convertable_value<CONVERTER> aOther) const { return mValue <= aOther.mValue; }
		constexpr bool operator>=(const convertable_value<CONVERTER> aOther) const { return mValue >= aOther.mValue; }
	};
}