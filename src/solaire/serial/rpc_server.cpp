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

#include "solaire/serial/rpc_server.hpp"

namespace solaire { namespace serial {
	// rpc_server

	bool rpc_server::is_notification(const value& aRequest) {
		const auto& object = aRequest.get_object();
		return object.find("id") == object.end();
	}

	value rpc_server::on_recieve_request(const value& aRequest) const {
		const std::string& method = aRequest["method"].get_string();
		const value& params = aRequest["params"];

		value response;
		response.add("id", aRequest["id"].get_unsigned());
		value& result = response.add("result", value::TYPE_VOID);

		try {
			if(params.is_void()) {
				result = std::move(call_function(method, nullptr, 0));
			}else {
				result = result = std::move(call_function(method, &params[0], params.size()));
			}
		}catch (std::exception& e) {
			response.add("error", e.what());
		}

		return response;
	}

	void rpc_server::on_recieve_notification(const value& aRequest) const {
		const std::string& method = aRequest["method"].get_string();
		const value& params = aRequest["params"];
		try {
			if(params.is_void()) {
				call_function(method, nullptr, 0);
			}else {
				call_function(method, &params[0], params.size());
			}
		}catch (...) {
	
		}
	}
}}