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

#ifndef SOLAIRE_SERIALISE_GENERIC_DISPATCH_HPP
#define SOLAIRE_SERIALISE_GENERIC_DISPATCH_HPP

#include <functional>
#include "encoder.hpp"

namespace solaire { namespace serial {
	namespace implementation {
		template<class R>
		struct function_wrapper {
			typedef std::function<value(const value* const, const uint32_t)> function_t;

			template<class F>
			static function_t wrap(F aFunction) {
				return [aFunction](const value* const aParams, const uint32_t aParamCount)->value {
					if(aParamCount != 0) throw std::runtime_error("solaire::implementation::function_wrapper : Wrong number of parameters");
					return encode<R>(aFunction());
				};
			}

			template<class A, class F>
			static function_t wrap(F aFunction) {
				return [aFunction](const value* const aParams, const uint32_t aParamCount)->value {
					if(aParamCount != 1) throw std::runtime_error("solaire::implementation::function_wrapper : Wrong number of parameters");
					return encode<R>(aFunction(
						encoder<A>::decode(aParams[0])
					));
				};
			}

			template<class A, class B, class F>
			static function_t wrap(F aFunction) {
				return [aFunction](const value* const aParams, const uint32_t aParamCount)->value {
					if(aParamCount != 2) throw std::runtime_error("solaire::implementation::function_wrapper : Wrong number of parameters");
					return encode<R>(aFunction(
						encoder<A>::decode(aParams[0]),
						encoder<B>::decode(aParams[1])
					));
				};
			}

			template<class A, class B, class C, class F>
			static function_t wrap(F aFunction) {
				return [aFunction](const value* const aParams, const uint32_t aParamCount)->value {
					if(aParamCount != 3) throw std::runtime_error("solaire::implementation::function_wrapper : Wrong number of parameters");
					return encode<R>(aFunction(
						encoder<A>::decode(aParams[0]),
						encoder<B>::decode(aParams[1]),
						encoder<B>::decode(aParams[2])
					));
				};
			}

			template<class A, class B, class C, class D, class F>
			static function_t wrap(F aFunction) {
				return [aFunction](const value* const aParams, const uint32_t aParamCount)->value {
					if(aParamCount != 4) throw std::runtime_error("solaire::implementation::function_wrapper : Wrong number of parameters");
					return encode<R>(aFunction(
						encoder<A>::decode(aParams[0]),
						encoder<B>::decode(aParams[1]),
						encoder<C>::decode(aParams[2]),
						encoder<D>::decode(aParams[3])
					));
				};
			}
		};

		template<>
		struct function_wrapper<void> {
			typedef std::function<value(const value* const, const uint32_t)> function_t;

			template<class F>
			static function_t wrap(F aFunction) {
				return [aFunction](const value* const aParams, const uint32_t aParamCount)->value {
					if(aParamCount != 0) throw std::runtime_error("solaire::implementation::function_wrapper : Wrong number of parameters");
					aFunction();
					return value();
				};
			}

			template<class A, class F>
			static function_t wrap(F aFunction) {
				return [aFunction](const value* const aParams, const uint32_t aParamCount)->value {
					if(aParamCount != 1) throw std::runtime_error("solaire::implementation::function_wrapper : Wrong number of parameters");
					aFunction(
						encoder<A>::decode(aParams[0])
					);
					return value();
				};
			}

			template<class A, class B, class F>
			static function_t wrap(F aFunction) {
				return [aFunction](const value* const aParams, const uint32_t aParamCount)->value {
					if(aParamCount != 2) throw std::runtime_error("solaire::implementation::function_wrapper : Wrong number of parameters");
					aFunction(
						encoder<A>::decode(aParams[0]),
						encoder<B>::decode(aParams[1])
					);
					return value();
				};
			}

			template<class A, class B, class C, class F>
			static function_t wrap(F aFunction) {
				return [aFunction](const value* const aParams, const uint32_t aParamCount)->value {
					if(aParamCount != 3) throw std::runtime_error("solaire::implementation::function_wrapper : Wrong number of parameters");
					aFunction(
						encoder<A>::decode(aParams[0]),
						encoder<B>::decode(aParams[1]),
						encoder<B>::decode(aParams[2])
					);
					return value();
				};
			}

			template<class A, class B, class C, class D, class F>
			static function_t wrap(F aFunction) {
				return [aFunction](const value* const aParams, const uint32_t aParamCount)->value {
					if(aParamCount != 4) throw std::runtime_error("solaire::implementation::function_wrapper : Wrong number of parameters");
					aFunction(
						encoder<A>::decode(aParams[0]),
						encoder<B>::decode(aParams[1]),
						encoder<C>::decode(aParams[2]),
						encoder<D>::decode(aParams[3])
					);
					return value();
				};
			}
		};
	}

	class dispatcher {
	private:
		template<class R>
		inline value call_(const std::string& aName) const {
			return call_function(aName, nullptr, 0);
		}

		template<class R, class A>
		inline value call_(const std::string& aName, A a) const {
			const value params[1] = {
				encode<A>(a)
			};
			return call_function(aName, params, 1);
		}

		template<class R, class A, class B>
		inline value call_(const std::string& aName, A a, B b) const {
			const value params[2] = {
				encode<A>(a),
				encode<B>(b)
			};
			return call_function(aName, params, 2);
		}

		template<class R, class A, class B, class C>
		inline value call_(const std::string& aName, A a, B b, C c) const {
			const value params[3] = {
				encode<A>(a),
				encode<B>(b),
				encode<C>(c)
			};
			return call_function(aName, params, 3);
		}

		template<class R, class A, class B, class C, class D>
		inline value call_(const std::string& aName, A a, B b, C c, D d) const {
			const value params[4] = {
				encode<A>(a),
				encode<B>(b),
				encode<C>(c),
				encode<D>(d)
			};
			return call_function(aName, params, 4);
		}
	protected:
		virtual value call_function(const std::string&, const value* const, const uint32_t) const = 0;
	public:
		virtual ~dispatcher(){}

		template<class R>
		inline typename std::enable_if<std::is_same<R, void>::value, R>::type call(const std::string& aName) const {
			call_<R>(aName);
		}

		template<class R, class A>
		inline typename std::enable_if<std::is_same<R, void>::value, R>::type call(const std::string& aName, A a) const {
			call_<R,A>(aName, a);
		}

		template<class R, class A, class B>
		inline typename std::enable_if<std::is_same<R, void>::value, R>::type call(const std::string& aName, A a, B b) const {
			call_<R,A,B>(aName, a, b);
		}

		template<class R, class A, class B, class C>
		inline typename std::enable_if<std::is_same<R, void>::value, R>::type call(const std::string& aName, A a, B b, C c) const {
			call_<R,A,B,C>(aName, a, b, c);
		}

		template<class R, class A, class B, class C, class D>
		inline typename std::enable_if<std::is_same<R, void>::value, R>::type call(const std::string& aName, A a, B b, C c, D d) const {
			call_<R,A,B,C,D>(aName, a, b, c, d);
		}

		template<class R>
		inline typename std::enable_if<! std::is_same<R, void>::value, R>::type call(const std::string& aName) const {
			return encoder<R>::decode(call_<R>(aName));
		}

		template<class R, class A>
		inline typename std::enable_if<! std::is_same<R, void>::value, R>::type call(const std::string& aName, A a) const {
			return encoder<R>::decode(call_<R, A>(aName, a));
		}

		template<class R, class A, class B>
		inline typename std::enable_if<! std::is_same<R, void>::value, R>::type call(const std::string& aName, A a, B b) const {
			return encoder<R>::decode(call_<R, A, B>(aName, a, b));
		}

		template<class R, class A, class B, class C>
		inline typename std::enable_if<! std::is_same<R, void>::value, R>::type call(const std::string& aName, A a, B b, C c) const {
			return encoder<R>::decode(call_<R, A, B, C>(aName, a, b, c));
		}

		template<class R, class A, class B, class C, class D>
		inline typename std::enable_if<! std::is_same<R, void>::value, R>::type call(const std::string& aName, A a, B b, C c, D d) const {
			return encoder<R>::decode(call_<R, A, B, C, D>(aName, a, b, c, d));
		}
	};

	class function_map_dispatcher : public dispatcher {
	private:
		typedef std::function<value(const value* const, const uint32_t)> function_t;
		std::map<std::string, function_t> mFunctions;

		void register_function_internal(const std::string&, function_t);
	protected:
		// Inherited from dispatcher
		value call_function(const std::string&, const value* const, const uint32_t) const override;
	public:
		virtual ~function_map_dispatcher();

		template<class R, class F>
		void register_function(const std::string& aName, F aFunction) {
			register_function_internal(aName, implementation::function_wrapper<R>::wrap<>(aFunction));
		}

		template<class R, class A, class F>
		void register_function(const std::string& aName, F aFunction) {
			register_function_internal(aName, implementation::function_wrapper<R>::wrap<A>(aFunction));
		}

		template<class R, class A, class B, class F>
		void register_function(const std::string& aName, F aFunction) {
			register_function_internal(aName, implementation::function_wrapper<R>::wrap<A, B>(aFunction));
		}

		template<class R, class A, class B, class  C, class F>
		void register_function(const std::string& aName, F aFunction) {
			register_function_internal(aName, implementation::function_wrapper<R>::wrap<A, B, C>(aFunction));
		}

		template<class R, class A, class B, class  C, class D, class F>
		void register_function(const std::string& aName, F aFunction) {
			register_function_internal(aName, implementation::function_wrapper<R>::wrap<A, B, C, D>(aFunction));
		}
	};
}}

#endif