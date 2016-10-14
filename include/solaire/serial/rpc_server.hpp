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

#ifndef SOLAIRE_SERIALISE_GENERIC_RPC_SERVER_HPP
#define SOLAIRE_SERIALISE_GENERIC_RPC_SERVER_HPP

#include "dispatcher.hpp"

namespace solaire { namespace serial {
	class rpc_server : public function_map_dispatcher {
	protected:
		static bool is_notification(const value&);
		value on_recieve_request(const value&) const;
		void on_recieve_notification(const value&) const;
	};
}}

#endif