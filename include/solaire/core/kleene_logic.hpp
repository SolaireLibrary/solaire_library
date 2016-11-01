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

#ifndef SOLAIRE_CORE_KLEENE_HPP
#define SOLAIRE_CORE_KLEENE_HPP

namespace solaire {
	enum kleene_value : uint8_t {
		KLEENE_FALSE = 0,
		KLEENE_UNKNOWN = 1,
		KLEENE_TRUE = 2,

		unknown = KLEENE_UNKNOWN
	};

	class kleene {
	private:
		kleene_value mValue;
	public:
		constexpr kleene() : 									mValue(KLEENE_UNKNOWN) {}
		constexpr kleene(const bool aValue) : 					mValue(aValue ? KLEENE_TRUE : KLEENE_FALSE) {}
		constexpr kleene(const kleene_value aValue) : 			mValue(aValue) {}
		constexpr explicit operator bool() const 				{ return mValue == KLEENE_TRUE; }
		constexpr kleene operator!() const 						{ return mValue == KLEENE_TRUE ? KLEENE_FALSE : mValue == KLEENE_FALSE ? KLEENE_TRUE : KLEENE_UNKNOWN; }
		constexpr kleene operator&&(const kleene aOther) const	{ return mValue < aOther.mValue ? mValue : aOther.mValue; }
		constexpr kleene operator||(const kleene aOther) const	{ return mValue > aOther.mValue ? mValue : aOther.mValue; }
		constexpr bool operator<(const kleene aOther) const 	{ return mValue < aOther.mValue; }
		constexpr bool operator>(const kleene aOther) const 	{ return mValue > aOther.mValue; }
		constexpr bool operator<=(const kleene aOther) const 	{ return mValue <= aOther.mValue; }
		constexpr bool operator>=(const kleene aOther) const 	{ return mValue >= aOther.mValue; }
	};
}