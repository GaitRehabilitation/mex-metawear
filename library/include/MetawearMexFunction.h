//
// Created by michaelpollind on 7/14/18.
//

#ifndef MEX_METAWEAR_MEXMOTION_H
#define MEX_METAWEAR_MEXMOTION_H

#include <map>
#include "mex.hpp"
#include "mexAdapter.hpp"
#include "FunctionWrapper.h"
#include <iostream>     // std::streambuf, std::cout
#include <fstream>      // std::ofstream
#include <strstream>

class MetawearWrapper;
class FunctionWrapper;
class ConnectionHandler;
class CaptureHandler;
class QueryHandler;
class ConfigurationHandler;
class MexFunction : public matlab::mex::Function {
private:
    std::map<std::string,MetawearWrapper*> m_devices;
    FunctionWrapper* m_functionWrapper;

    std::ostrstream  m_ostream;

    ConnectionHandler* m_connectionHandler;
    CaptureHandler* m_captureHandler;
    QueryHandler* m_queryHandler;
    ConfigurationHandler* m_configurationHandler;

public:
    MexFunction ();
    void operator()(matlab::mex::ArgumentList outputs, matlab::mex::ArgumentList inputs) override;
    ~MexFunction () override;

    static void mexDebugCapture(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs);


};
#endif //MEX_METAWEAR_MEXMOTION_H
