//
// Created by michaelpollind on 7/14/18.
//

#ifndef MEX_METAWEAR_MEXMOTION_H
#define MEX_METAWEAR_MEXMOTION_H

#include "mex.hpp"
#include "mexAdapter.hpp"
#include "MetamotionWrapper.h"

class MexFunction : public matlab::mex::Function {
    std::vector<MetaMotionWrapper*> m_devices;
public:
    void operator()(matlab::mex::ArgumentList outputs, matlab::mex::ArgumentList inputs);
    ~MexFunction();

private:
    void metwareConnect(matlab::mex::ArgumentList outputs, matlab::mex::ArgumentList inputs);
    void metwareConfigure(matlab::mex::ArgumentList outputs, matlab::mex::ArgumentList inputs);
    void metwareStart(matlab::mex::ArgumentList outputs, matlab::mex::ArgumentList inputs);
    void metwareStop(matlab::mex::ArgumentList outputs, matlab::mex::ArgumentList inputs);


};
#endif //MEX_METAWEAR_MEXMOTION_H
