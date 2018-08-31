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

#include <MexUtility.h>
#include <MetawearWrapper.h>
#include "handlers/CaptureHandler.h"
#include "handlers/ConnectionHandler.h"

CaptureHandler::CaptureHandler(ConnectionHandler *connectionHandler, FunctionWrapper *wrapper):
    m_connectionHandler(connectionHandler){
    std::map<std::string, WrapperMethod *> functions =  {
            //enable
            {"enable_gyro", mexEnableGyro},
            {"enable_accelerometer", mexEnableAccelerometer},

            //disable
            {"disable_gyro",mexDisableGyro},
            {"disable_accelerometer",mexDisableGyro}
    };
    wrapper->registerMethod(this, functions);
}

void CaptureHandler::mexEnableGyro(std::shared_ptr<matlab::engine::MATLABEngine> engine, void *context,
                                    ParameterWrapper &outputs, ParameterWrapper &inputs) {
    CaptureHandler* handler = static_cast<CaptureHandler*>(context);

    if(inputs.size() != 2){
        MexUtility::error(engine, "Three Inputs Required");
        return;
    }

    matlab::data::CharArray address =  inputs[1];
    MetawearWrapper* wrapper =  handler->m_connectionHandler->getDevice(address.toAscii());

    wrapper->startGyro();
}

void  CaptureHandler::mexEnableAccelerometer(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs){
    CaptureHandler* handler = static_cast<CaptureHandler*>(context);

    if(inputs.size() != 2){
        MexUtility::error(engine, "Three Inputs Required");
        return;
    }

    matlab::data::CharArray address =  inputs[1];
    MetawearWrapper* wrapper =  handler->m_connectionHandler->getDevice(address.toAscii());

    wrapper->startAccelerometer();
}


void CaptureHandler::mexDisableGyro(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs){
    CaptureHandler* handler = static_cast<CaptureHandler*>(context);

    if(inputs.size() != 2){
        MexUtility::error(engine, "Three Inputs Required");
        return;
    }

    matlab::data::CharArray address =  inputs[1];
    MetawearWrapper* wrapper =  handler->m_connectionHandler->getDevice(address.toAscii());

    wrapper->stopGyro();
}

void CaptureHandler::mexDisableAccelerometer(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs) {
    CaptureHandler *handler = static_cast<CaptureHandler *>(context);

    if (inputs.size() != 2) {
        MexUtility::error(engine, "Three Inputs Required");
        return;
    }

    matlab::data::CharArray address = inputs[1];
    MetawearWrapper *wrapper = handler->m_connectionHandler->getDevice(address.toAscii());

    wrapper->stopAccelerometer();
}


CaptureHandler::~CaptureHandler() {

}

