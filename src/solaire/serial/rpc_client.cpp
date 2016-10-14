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

#include "solaire/serial/rpc_client.hpp"

namespace solaire { namespace serial {
	// rpc_client

	rpc_client::rpc_client() : 
		mRequestID(0)
	{}

	rpc_client::~rpc_client() {

	}

	value rpc_client::call_function(const std::string& aName, const value* const aParams, const uint32_t aParamCount) const {
		value request(value::TYPE_OBJECT);
		const bool notification = false;// aParams == nullptr || aParamCount == 0;
		{
			request.add("method", aName);
			if(notification) {
				request.add("params", value::TYPE_VOID);
			}else {
				value params(value::TYPE_ARRAY);
				params.reserve(aParamCount);
				if(aParamCount > 0) {
					for(uint32_t i = 0; i < aParamCount; ++i) params.push_back(aParams[i]);
				}
				request.add("params", params);
				request.add("id", mRequestID++);
			}
		}

		if(notification) {
			send_notification(request);
			return value();
		}else{
			const value response = send_request(request);
			const value& error = response["error"];
			if(! error.is_void()) throw std::runtime_error(error.get_string().c_str());
			return response["result"];
		}
	}
}}