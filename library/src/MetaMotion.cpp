//
// Created by michaelpollind on 7/14/18.
//

#include <MatlabDataArray/MDArray.hpp>
#include <MatlabDataArray/CharArray.hpp>
#include "include/MetaMotion.h"
#include "mex.hpp"
#include "mexAdapter.hpp"
#include "include/MexUtility.h"
#include "regex"

MexFunction::MexFunction() : m_devices(){
}

void MexFunction::operator()(matlab::mex::ArgumentList outputs, matlab::mex::ArgumentList inputs) {
    m_matlabPtr = getEngine();

    matlab::data::ArrayFactory factory;
    if(inputs.size() == 0){
        MexUtility::error(m_matlabPtr,"Empty fields are not allowed in this program.");
        return;
    }

    if(inputs[0].getType() == matlab::data::ArrayType::CHAR){
        matlab::data::CharArray target = inputs[0];
        if(target.toAscii() == "metware_register") mexMetwareConnect(outputs,inputs);
        else if(target.toAscii() == "metware_configure") mexMetwareConfigure(outputs,inputs);
        else if(target.toAscii() == "metware_start") mexMetwareStart(outputs,inputs);
        else if(target.toAscii() == "metware_query") mexMetwareQuery(outputs,inputs);
        else if(target.toAscii() == "metware_stop") mexMetwareStop(outputs,inputs);
        else MexUtility::error(m_matlabPtr,"Unknown handler for first argument");

    }

}

MexFunction::~MexFunction() {

}

void MexFunction::mexMetwareConnect(matlab::mex::ArgumentList& outputs, matlab::mex::ArgumentList& inputs) {
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


}

void MexFunction::mexMetwareConfigure(matlab::mex::ArgumentList& outputs, matlab::mex::ArgumentList& inputs) {
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
            else if(target.toAscii() == "humidigy"){

            }
            else if(target.toAscii() == "barometer"){

            }
            else if(target.toAscii() == "commit"){

            }
        }
    }
}

void MexFunction::mexMetwareStart(matlab::mex::ArgumentList& outputs, matlab::mex::ArgumentList& inputs) {
    if(inputs.size() != 2){
        MexUtility::error(m_matlabPtr,"two Inputs Required");
        return;
    }

    if(inputs[1].type == matlab::data::ArrayType::OBJECT) {
        std::string address = getAddress(inputs[1]);
    }


}

void MexFunction::mexMetwareStop(matlab::mex::ArgumentList& outputs, matlab::mex::ArgumentList& inputs) {

}

void MexFunction::mexMetwareQuery(matlab::mex::ArgumentList &outputs, matlab::mex::ArgumentList &inputs) {

}

std::string MexFunction::getAddress(matlab::data::ObjectArray &objectArray) {
    matlab::data::CharArray address = m_matlabPtr->getProperty(objectArray,"address");
    return address.toAscii();
}
