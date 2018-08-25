//
// Created by Michael on 8/24/2018.
//

#ifndef MEX_METAWEAR_QUERYHANDLER_H
#define MEX_METAWEAR_QUERYHANDLER_H


#include <map>
#include <mex.hpp>
#include "FunctionWrapper.h"

class FunctionWrapper;
class ConnectionHandler;
class QueryHandler {
private:
    ConnectionHandler* m_connectionHandler;
public:
    QueryHandler(ConnectionHandler* connectionHandler,FunctionWrapper* wrapper);
    ~QueryHandler();

    static void mexQueryGyro(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs);
    static void mexQueryAccelerometer(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs);

};

#endif //MEX_METAWEAR_QUERYHANDLER_H
