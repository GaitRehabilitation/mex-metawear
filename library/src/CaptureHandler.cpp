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

    matlab::data::ObjectArray o =  inputs[1];
    MetawearWrapper* wrapper =  handler->m_connectionHandler->mexToMetawearWrapper(engine,o);

    wrapper->startGyro();
}

void  CaptureHandler::mexEnableAccelerometer(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs){
    CaptureHandler* handler = static_cast<CaptureHandler*>(context);

    if(inputs.size() != 2){
        MexUtility::error(engine, "Three Inputs Required");
        return;
    }

    matlab::data::ObjectArray o =  inputs[1];
    MetawearWrapper* wrapper =  handler->m_connectionHandler->mexToMetawearWrapper(engine,o);

    wrapper->startAccelerometer();
}


void CaptureHandler::mexDisableGyro(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs){
    CaptureHandler* handler = static_cast<CaptureHandler*>(context);

    if(inputs.size() != 2){
        MexUtility::error(engine, "Three Inputs Required");
        return;
    }

    matlab::data::ObjectArray o =  inputs[1];
    MetawearWrapper* wrapper =  handler->m_connectionHandler->mexToMetawearWrapper(engine,o);

    wrapper->stopGyro();
}

void CaptureHandler::mexDisableAccelerometer(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs){
    CaptureHandler* handler = static_cast<CaptureHandler*>(context);

    if(inputs.size() != 2){
        MexUtility::error(engine, "Three Inputs Required");
        return;
    }

    matlab::data::ObjectArray o =  inputs[1];
    MetawearWrapper* wrapper =  handler->m_connectionHandler->mexToMetawearWrapper(engine,o);

    wrapper->stopAccelerometer();
}


CaptureHandler::~CaptureHandler() {

}

