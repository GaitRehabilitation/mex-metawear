//
// Created by michaelpollind on 7/14/18.
//

#ifndef MEX_METAWEAR_MEXUTILITY_H
#define MEX_METAWEAR_MEXUTILITY_H

#include <mex.hpp>

class MexUtility {
public:
    static void error(std::shared_ptr<matlab::engine::MATLABEngine> matlabPtr, std::string error);
    static void printf(std::shared_ptr<matlab::engine::MATLABEngine> matlabPtr, std::string output);
private:
    MexUtility() = default;
};

#endif //MEX_METAWEAR_MEXUTILITY_H
