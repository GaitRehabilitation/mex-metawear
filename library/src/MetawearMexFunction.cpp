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

#include <strstream>
MexFunction::MexFunction() : matlab::mex::Function(),
    m_devices(),
    m_functionWrapper(new FunctionWrapper()){

    std::cout.rdbuf(m_ostream.rdbuf());

    m_connectionHandler = new ConnectionHandler(m_functionWrapper);
    m_configurationHandler = new ConfigurationHandler(m_connectionHandler,m_functionWrapper);
    m_captureHandler = new CaptureHandler(m_connectionHandler,m_functionWrapper);
    m_queryHandler = new QueryHandler(m_connectionHandler,m_functionWrapper);

    std::map<std::string, WrapperMethod *> functions =  {
            {"info", mexDebugCapture}
    };
    m_functionWrapper->registerMethod(this, functions);

}

void MexFunction::operator()(matlab::mex::ArgumentList outputs, matlab::mex::ArgumentList inputs) {

    if(inputs.size() < 1) {
        MexUtility::error(getEngine(), "Identifier is required");
        return;
    }
    if(inputs[0].getType() != matlab::data::ArrayType::CHAR){
        MexUtility::error(getEngine(), "first Parameter has to be string type");
        return;
    }
    matlab::data::CharArray target = inputs[0];

    ParameterWrapper ouput = ParameterWrapper(outputs.begin(),outputs.end(),outputs.size());
    ParameterWrapper input = ParameterWrapper(inputs.begin(),inputs.end(),inputs.size());

    m_functionWrapper->invoke(target.toAscii(), getEngine(), ouput, input);

}

void MexFunction::mexDebugCapture(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs){
    MexFunction* handler = static_cast<MexFunction*>(context);

    matlab::data::ArrayFactory factory;
    engine->feval(matlab::engine::convertUTF8StringToUTF16String("fprintf"), 0,
                       std::vector<matlab::data::Array>({ factory.createScalar(matlab::engine::convertUTF8StringToUTF16String(handler->m_ostream.str())) }));

    handler->m_ostream.flush();
}
MexFunction::~MexFunction() {

}
