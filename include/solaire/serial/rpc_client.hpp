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

#ifndef SOLAIRE_SERIALISE_GENERIC_RPC_CLIENT_HPP
#define SOLAIRE_SERIALISE_GENERIC_RPC_CLIENT_HPP

#include "dispatcher.hpp"

namespace solaire { namespace serial {
	class rpc_client : public dispatcher {
	private:
		mutable uint64_t mRequestID;
	protected:
		virtual value send_request(value&) const = 0;
		virtual void send_notification(value&) const = 0;

		// Inherited from dispatcher
		value call_function(const std::string&, const value* const, const uint32_t) const override;
	public:
		rpc_client();
		virtual ~rpc_client();
	};
}}

#endif