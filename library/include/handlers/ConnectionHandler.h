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

#ifndef MEX_METAWEAR_CONNECTIONHANDLER_H
#define MEX_METAWEAR_CONNECTIONHANDLER_H

#include <map>
#include <mex.hpp>
#include "FunctionWrapper.h"

class MetawearWrapper;
class FunctionWrapper;
class ConnectionHandler {
private:
    std::map<std::string, MetawearWrapper *> m_devices;
public:
    ConnectionHandler(FunctionWrapper *wrapper);

    ~ConnectionHandler();

    MetawearWrapper *getDevice(const std::string &mac);
    MetawearWrapper *mexGetDeviceAndVerify(std::shared_ptr<matlab::engine::MATLABEngine> engine,const std::string &mac);

    void addDevice(MetawearWrapper* wrapper);


    MetawearWrapper *removeDevice(const std::string &mac);

    static void mexConnect(std::shared_ptr<matlab::engine::MATLABEngine> engine, void *context, ParameterWrapper &outputs,ParameterWrapper &inputs);
    static void mexDisconnect(std::shared_ptr<matlab::engine::MATLABEngine> engine, void *context, ParameterWrapper &outputs, ParameterWrapper &inputs);
    static void mexDisconnectAlldevices(std::shared_ptr<matlab::engine::MATLABEngine> engine, void *context, ParameterWrapper &outputs, ParameterWrapper &inputs);

};

#endif //MEX_METAWEAR_CONNECTIONHANDLER_H
