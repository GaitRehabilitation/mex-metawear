//
// Created by Michael on 8/25/2018.
//
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

