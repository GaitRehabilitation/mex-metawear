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
#include "handlers/ConnectionHandler.h"
#include "handlers/CaptureHandler.h"
#include "handlers/QueryHandler.h"
#include "handlers/ConfigurationHandler.h"




MexFunction::MexFunction() : matlab::mex::Function(),
    m_devices(),
    m_functionWrapper(new FunctionWrapper()){

    m_connectionHandler = new ConnectionHandler(m_functionWrapper);
    m_configurationHandler = new ConfigurationHandler(m_connectionHandler,m_functionWrapper);
    m_captureHandler = new CaptureHandler(m_connectionHandler,m_functionWrapper);
    m_queryHandler = new QueryHandler(m_connectionHandler,m_functionWrapper);
}

void MexFunction::operator()(matlab::mex::ArgumentList outputs, matlab::mex::ArgumentList inputs) {

    if(inputs.size() < 1) {
        MexUtility::error(getEngine(), "Identifier is required");
        return;
    }
    matlab::data::CharArray target = inputs[0];

    ParameterWrapper ouput = ParameterWrapper(outputs.begin(),outputs.end(),outputs.size());
    ParameterWrapper input = ParameterWrapper(inputs.begin(),inputs.end(),inputs.size());

    m_functionWrapper->invoke(target.toAscii(),getEngine(),ouput,input);
}

MexFunction::~MexFunction() {

}
