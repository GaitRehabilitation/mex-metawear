/**
* Copyright 2018 GaitRehabilitation
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#ifndef MEX_METAWEAR_SUBSCRIPTIONHEANDLER_H
#define MEX_METAWEAR_SUBSCRIPTIONHEANDLER_H

#include <map>
#include <mex.hpp>
#include <StreamHandler.h>
#include "FunctionWrapper.h"

class FunctionWrapper;
class ConnectionHandler;
class SubscriptionHandler {
    private:
        ConnectionHandler* m_connectionHandler;
    public:
    SubscriptionHandler(ConnectionHandler* connectionHandler,FunctionWrapper* wrapper);
    ~SubscriptionHandler();

    static StreamType getSubscriptionMode(std::shared_ptr<matlab::engine::MATLABEngine> engine,const std::string& target);


    static void mexSubscribeGyro(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs);
    static void mexSubscribeAcc(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs);
    static void mexSubscribeFusion(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs);

    static void mexUnSubscribe(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs);
};
#endif //MEX_METAWEAR_SUBSCRIPTIONHEANDLER_H
