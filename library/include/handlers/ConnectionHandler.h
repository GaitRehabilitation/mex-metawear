//
// Created by Michael on 8/23/2018.
//

#ifndef MEX_METAWEAR_CONNECTIONHANDLER_H
#define MEX_METAWEAR_CONNECTIONHANDLER_H

#include <map>
#include <mex.hpp>
#include "FunctionWrapper.h"

class MetawearWrapper;
class FunctionWrapper;
class ConnectionHandler {
private:
    std::map<std::string,MetawearWrapper*> m_devices;
public:
    ConnectionHandler(FunctionWrapper* wrapper);
    ~ConnectionHandler();

    MetawearWrapper* getDevice(const std::string& mac);
    MetawearWrapper* addDevice(const std::string& mac);
    MetawearWrapper* removeDevice(const std::string& mac);

    MetawearWrapper* mexToMetawearWrapper(std::shared_ptr<matlab::engine::MATLABEngine> engine,matlab::data::ObjectArray& o);

     static void mexConnect(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs);
     static void mexDisconnect(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs);
};

#endif //MEX_METAWEAR_CONNECTIONHANDLER_H
