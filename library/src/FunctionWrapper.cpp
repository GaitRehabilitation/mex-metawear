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

#include "FunctionWrapper.h"
#include <mex.hpp>
#include <MatlabDataArray/ArrayFactory.hpp>

FunctionWrapper::FunctionWrapper(){

}

FunctionWrapper::~FunctionWrapper(){

}

void FunctionWrapper::registerMethod(void* context,const std::map<std::string,WrapperMethod*>& methods) {
    for (const auto &method : methods) {
        m_methods.emplace(std::string(method.first), new MethodContainer{context, method.second});
    }
}

bool FunctionWrapper::invoke(const std::string& key, std::shared_ptr<matlab::engine::MATLABEngine> engine, ParameterWrapper& outputs, ParameterWrapper& inputs){

    auto m =  m_methods.find(key);
    if (m != m_methods.end()) {
        auto method =  m->second;
        method->wrapper(engine,method->context,outputs,inputs);
        return true;
    }
    return false;
}


