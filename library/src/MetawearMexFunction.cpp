//
// Created by michaelpollind on 7/14/18.
//

#include <MatlabDataArray/MDArray.hpp>
#include <MatlabDataArray/CharArray.hpp>
#include "mex.hpp"
#include "mexAdapter.hpp"
#include "MexUtility.h"
#include "regex"
#include "MetawearMexFunction.h"
#include "MetawearWrapper.h"

#include "FunctionWrapper.h"
#include "ConnectionHandler.h"

MexFunction::MexFunction() : matlab::mex::Function(),
    m_devices(),
    m_functionWrapper(new FunctionWrapper()){

    m_connectionHandler = new ConnectionHandler(m_functionWrapper);
}

/*
void MexFunction::registerMethod(MethodTable *methods){
    MethodTable* entry = methods;
    while(entry->target){
        m_methods.emplace(std::string(entry->target),entry->wrapper);
        entry++;
    }
}*/

void MexFunction::operator()(matlab::mex::ArgumentList outputs, matlab::mex::ArgumentList inputs) {

    if(inputs.size() >= 1) {
        MexUtility::error(getEngine(), "Identifier is required");
        return;
    }
    matlab::data::CharArray target = inputs[0];

    ParameterWrapper ouput = ParameterWrapper(outputs.begin(),outputs.end(),outputs.size());
    ParameterWrapper input = ParameterWrapper(inputs.begin(),inputs.end(),inputs.size());

    m_functionWrapper->invoke(target.toAscii(),getEngine(),ouput,input);

    /*
    matlab::data::ArrayFactory factory;
    if(inputs.size() == 0){
        MexUtility::error(m_matlabPtr,"Empty fields are not allowed in this program.");
        return;
    }

   /* if(inputs[0].getType() == matlab::data::ArrayType::CHAR){
        matlab::data::CharArray target = inputs[0];
        if(target.toAscii() == "register") mexMetawareRegister(outputs,inputs);
        else if(target.toAscii() == "connect") mexMetawareConnect(outputs, inputs);
        else if(target.toAscii() == "configure") mexMetwareConfigure(outputs,inputs);
        else if(target.toAscii() == "start") mexMetwareStart(outputs,inputs);
        else if(target.toAscii() == "query") mexMetwareQuery(outputs,inputs);
        else if(target.toAscii() == "stop") mexMetwareStop(outputs,inputs);
        else MexUtility::error(m_matlabPtr,"Unknown handler for first argument");
    }*/

}

MexFunction::~MexFunction() {

}

/*
void MexFunction::mexMetawareConnect(matlab::mex::ArgumentList &outputs, matlab::mex::ArgumentList &inputs) {
    if(inputs.size() != 2){
        MexUtility::error(m_matlabPtr,"Two Inputs Required");
        return;
    }

    if(outputs.size() != 1){
        MexUtility::error(m_matlabPtr,"Single argument for output");
        return;
    }

    if(inputs[1].getType() != matlab::data::ArrayType::OBJECT){
        MexUtility::error(m_matlabPtr,"Second Parameter has to be object type");
        return;
    }
    matlab::data::ObjectArray obj = inputs[1];
    MetawearWrapper* wrapper = getDevice(obj);
    if(wrapper == nullptr)
        return;
    wrapper->connect();
}


MetawearWrapper* MexFunction::getDevice(const  matlab::data::ObjectArray& payload){

    try {
        matlab::data::CharArray address = m_matlabPtr->getProperty(payload, "address");
        return m_devices.at(address.toAscii());
    }
    catch(matlab::engine::MATLABExecutionException e){
        MexUtility::error(m_matlabPtr,"Unknown handler");
    }
    return nullptr;
}

void MexFunction::mexMetawareRegister(matlab::mex::ArgumentList& outputs, matlab::mex::ArgumentList& inputs) {
    if(inputs.size() != 2){
        MexUtility::error(m_matlabPtr,"Two Inputs Required");
        return;
    }

    if(outputs.size() != 1){
        MexUtility::error(m_matlabPtr,"Single argument for output");
        return;
    }

    if(inputs[1].getType() != matlab::data::ArrayType::CHAR){
        MexUtility::error(m_matlabPtr,"Second Parameter has to be string type");
        return;
    }
    matlab::data::CharArray address = inputs[1];

    // standard IEEE 802 format for MAC-48 address. verify if the format is correct.
    std::regex addressMatch ("^([0-9A-Fa-f]{2}[:-]){5}([0-9A-Fa-f]{2})$");
    if(!std::regex_match (address.toAscii(),addressMatch)){
        MexUtility::error(m_matlabPtr,"Invalid bluetooth address for second parameter");
        return;
    }
    matlab::data::ArrayFactory factory;
    matlab::data::ObjectArray result(factory.createEmptyArray());
    // set the address property for result
    m_matlabPtr->setProperty(result,"address",address);
    outputs[0] = result;

    MetawearWrapper* wrapper = new MetawearWrapper(address.toAscii(),1000);
    wrapper->connect();

}

void MexFunction::mexMetwareConfigure(matlab::mex::ArgumentList& outputs, matlab::mex::ArgumentList& inputs) {


    if (inputs[1].getType() == matlab::data::ArrayType::CHAR) {
        matlab::data::CharArray target = inputs[1];
        if (target.toAscii() == "accelerometer") {
            if (inputs[2].getType() != matlab::data::ArrayType::OBJECT) {
                MexUtility::error(m_matlabPtr, "Third Parameter has to be object type");
                return;
            }
            matlab::data::ObjectArray obj = inputs[2];
            MetawearWrapper *wrapper = getDevice(obj);
            if (wrapper == nullptr) {
                MexUtility::error(m_matlabPtr,"Unknown Device");
                return;
            }

            if (inputs[3].getType() != matlab::data::ArrayType::DOUBLE) {
                MexUtility::error(m_matlabPtr, "Third Parmeters has to be type double");
                return;
            }

            matlab::data::TypedArray<double> d = inputs[3];
            wrapper->configureAccelerometer((float)d[0],(float)d[1]);

        } else if (target.toAscii() == "gyro") {

        } else if (target.toAscii() == "i2c") {

        } else if (target.toAscii() == "spi") {

        } else if (target.toAscii() == "fusion") {

        } else if (target.toAscii() == "magnetometer") {

        } else if (target.toAscii() == "humidity") {

        } else if (target.toAscii() == "barometer") {

        } else if (target.toAscii() == "commit") {

        }
    }
}


void MexFunction::mexMetwareStart(matlab::mex::ArgumentList& outputs, matlab::mex::ArgumentList& inputs) {
    if(inputs.size() != 2){
        MexUtility::error(m_matlabPtr,"two Inputs Required");
        return;
    }

    if(inputs[1].type == matlab::data::ArrayType::OBJECT) {
        matlab::data::ObjectArray o = inputs[1];
        std::string address = getAddress(o);
    }


}

void MexFunction::mexMetwareStop(matlab::mex::ArgumentList& outputs, matlab::mex::ArgumentList& inputs) {

}

void MexFunction::mexMetwareQuery(matlab::mex::ArgumentList &outputs, matlab::mex::ArgumentList &inputs) {
    if(inputs.size() >= 2){
        if(inputs[1].getType() == matlab::data::ArrayType::CHAR) {
            matlab::data::CharArray target = inputs[1];
            if(target.toAscii() == "accelerometer"){

            }
            else if(target.toAscii() == "gyro"){

            }
            else if(target.toAscii() == "i2c"){

            }
            else if(target.toAscii() == "spi"){

            }
            else if(target.toAscii() == "fusion"){

            }
            else if(target.toAscii() == "magnetometer"){

            }
            else if(target.toAscii() == "humidity"){

            }
            else if(target.toAscii() == "barometer"){

            }
            else if(target.toAscii() == "commit"){

            }
        }
    }
}

std::string MexFunction::getAddress(matlab::data::ObjectArray &objectArray) {
    matlab::data::CharArray address = m_matlabPtr->getProperty(objectArray,"address");
    return address.toAscii();
}
*/