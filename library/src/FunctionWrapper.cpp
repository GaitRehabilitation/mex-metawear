//
// Created by Michael on 8/23/2018.
//
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


