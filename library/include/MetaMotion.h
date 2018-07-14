//
// Created by michaelpollind on 7/14/18.
//

#ifndef MEX_METAWEAR_METAMOTION_H
#define MEX_METAWEAR_METAMOTION_H

#include "mex.hpp"
#include "mexAdapter.hpp"
#include <unordered_map>
#include <fstream>
#include <MatlabEngine.hpp>

using matlab::mex::ArgumentList;
using matlab::engine::convertUTF8StringToUTF16String;
using namespace matlab::data;


class MetaMotion : public Function{

    // Pointer to MATLAB engine
    std::shared_ptr<matlab::engine::MATLABEngine> matlabPtr = getEngine();

public:
    void operator()(ArgumentList outputs, ArgumentList inputs);
    ~MetaMotion();

};


#endif //MEX_METAWEAR_METAMOTION_H
