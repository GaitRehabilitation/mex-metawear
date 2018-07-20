//
// Created by michaelpollind on 7/14/18.
//

#ifndef MEX_METAWEAR_MEXMOTION_H
#define MEX_METAWEAR_MEXMOTION_H

#include <map>
#include "mex.hpp"
#include "mexAdapter.hpp"
#include "MetamotionWrapper.h"

class MexFunction : public matlab::mex::Function {
private:
    std::shared_ptr<matlab::engine::MATLABEngine> m_matlabPtr;
    std::map<std::string,MetaMotionWrapper*> m_devices;
public:
    MexFunction();
    void operator()(matlab::mex::ArgumentList outputs, matlab::mex::ArgumentList inputs) override;
    ~MexFunction() override;
private:
    void mexMetwareConnect(matlab::mex::ArgumentList& outputs, matlab::mex::ArgumentList& inputs);
    void mexMetwareConfigure(matlab::mex::ArgumentList& outputs, matlab::mex::ArgumentList& inputs);
    void mexMetwareStart(matlab::mex::ArgumentList& outputs, matlab::mex::ArgumentList& inputs);
    void mexMetwareStop(matlab::mex::ArgumentList& outputs, matlab::mex::ArgumentList& inputs);
    void mexMetwareQuery(matlab::mex::ArgumentList& outputs, matlab::mex::ArgumentList& inputs);

    std::string getAddress(matlab::data::ObjectArray& objectArray);

};
#endif //MEX_METAWEAR_MEXMOTION_H
