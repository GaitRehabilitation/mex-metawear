/**
* Copyright 2018 GaitRehabilitation
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#ifndef MEX_METAWEAR_MEXUTILITY_H
#define MEX_METAWEAR_MEXUTILITY_H

#include <mex.hpp>
#include <MatlabDataArray/ArrayType.hpp>

class MexUtility {
public:
    enum ParameterType{
        INPUT,
        OUTPUT
    };
    static void error(std::shared_ptr<matlab::engine::MATLABEngine> matlabPtr, std::string error);
    static void printf(std::shared_ptr<matlab::engine::MATLABEngine> matlabPtr, std::string output);
    static std::string typeToString(matlab::data::ArrayType type);
    static void checkType(std::shared_ptr<matlab::engine::MATLABEngine> matlabPtr,ParameterType parameterType,int index,matlab::data::ArrayType current,matlab::data::ArrayType target);
    static void checkNumberOfParameters(std::shared_ptr<matlab::engine::MATLABEngine> matlabPtr, ParameterType parameterType, size_t current, size_t target);

private:
    MexUtility() = default;
};

#endif //MEX_METAWEAR_MEXUTILITY_H
