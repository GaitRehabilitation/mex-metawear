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

#ifndef MEX_METAWEAR_MEXMOTION_H
#define MEX_METAWEAR_MEXMOTION_H

#include <map>
#include "mex.hpp"
#include "mexAdapter.hpp"
#include "FunctionWrapper.h"
#include <iostream>     // std::streambuf, std::cout
#include <fstream>      // std::ofstream
#include <strstream>

class MetawearWrapper;
class FunctionWrapper;
class ConnectionHandler;
class CaptureHandler;
class QueryHandler;
class ConfigurationHandler;
class SubscriptionHandler;
class MexFunction : public matlab::mex::Function {
private:
    std::map<std::string,MetawearWrapper*> m_devices;
    FunctionWrapper* m_functionWrapper;

    std::ostrstream  m_ostream;

    ConnectionHandler* m_connectionHandler;
    CaptureHandler* m_captureHandler;
    QueryHandler* m_queryHandler;
    ConfigurationHandler* m_configurationHandler;
    SubscriptionHandler* m_subscrptionHandler;

public:
    MexFunction ();
    void operator()(matlab::mex::ArgumentList outputs, matlab::mex::ArgumentList inputs) override;
    ~MexFunction () override;

    static void mexDebugCapture(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs);


};
#endif //MEX_METAWEAR_MEXMOTION_H
