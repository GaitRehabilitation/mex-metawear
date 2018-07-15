//
// Created by michaelpollind on 7/14/18.
//

#include <MatlabDataArray/MDArray.hpp>
#include <MatlabDataArray/CharArray.hpp>
#include "include/MetaMotion.h"
#include "mex.hpp"
#include "mexAdapter.hpp"


void MexFunction::operator()(matlab::mex::ArgumentList outputs, matlab::mex::ArgumentList inputs) {
    if(inputs[0].getType() == matlab::data::ArrayType::MATLAB_STRING){
        matlab::data::CharArray target = inputs[0];
        if(target.toAscii() == "metware_connect") metwareConnect(outputs,inputs); else return;
        if(target.toAscii() == "metware_configure") metwareConnect(outputs,inputs); else return;
        if(target.toAscii() == "metware_start") metwareConnect(outputs,inputs); else return;
        if(target.toAscii() == "metware_stop") metwareConnect(outputs,inputs); else return;
    }
}

MexFunction::~MexFunction() {

}

void MexFunction::metwareConnect(matlab::mex::ArgumentList outputs, matlab::mex::ArgumentList inputs) {

}