//
// Created by Michael on 8/24/2018.
//

#ifndef MEX_METAWEAR_CONFIGURATIONHANDLER_H
#define MEX_METAWEAR_CONFIGURATIONHANDLER_H

#include <map>
#include <mex.hpp>
#include "FunctionWrapper.h"

class FunctionWrapper;
class ConnectionHandler;
class ConfigurationHandler {
private:
    ConnectionHandler* m_connectionHandler;
public:
    ConfigurationHandler (ConnectionHandler* connectionHandler,FunctionWrapper* wrapper);
    ~ConfigurationHandler();


    static void mexConfigureGyro(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs);
    static void mexConfigureAccelerometer(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs);

};

#endif //MEX_METAWEAR_CONFIGURATIONHANDLER_H
