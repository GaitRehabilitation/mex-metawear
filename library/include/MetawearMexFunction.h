//
// Created by michaelpollind on 7/14/18.
//

#ifndef MEX_METAWEAR_MEXMOTION_H
#define MEX_METAWEAR_MEXMOTION_H

#include <map>
#include "mex.hpp"
#include "mexAdapter.hpp"

class MetawearWrapper;
class FunctionWrapper;
class ConnectionHandler;
class MexFunction : public matlab::mex::Function {
private:
    std::map<std::string,MetawearWrapper*> m_devices;
    FunctionWrapper* m_functionWrapper;
    ConnectionHandler* m_connectionHandler;


public:
    MexFunction ();
    void operator()(matlab::mex::ArgumentList outputs, matlab::mex::ArgumentList inputs) override;
    ~MexFunction () override;


    /*MetawearWrapper* getDevice(const  matlab::data::ObjectArray& payload);

private:
    void mexMetawareConnect(matlab::mex::ArgumentList &outputs, matlab::mex::ArgumentList &inputs);
    void mexMetawareRegister(matlab::mex::ArgumentList& outputs, matlab::mex::ArgumentList& inputs);
    void mexMetwareConfigure(matlab::mex::ArgumentList& outputs, matlab::mex::ArgumentList& inputs);
    void mexMetwareStart(matlab::mex::ArgumentList& outputs, matlab::mex::ArgumentList& inputs);
    void mexMetwareStop(matlab::mex::ArgumentList& outputs, matlab::mex::ArgumentList& inputs);
    void mexMetwareQuery(matlab::mex::ArgumentList& outputs, matlab::mex::ArgumentList& inputs);

    std::string getAddress(matlab::data::ObjectArray& objectArray);*/

};
#endif //MEX_METAWEAR_MEXMOTION_H
