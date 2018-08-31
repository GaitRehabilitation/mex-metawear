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

#include <MexUtility.h>

#include "MatlabDataArray.hpp"

void MexUtility::error(std::shared_ptr<matlab::engine::MATLABEngine> matlabPtr,const std::string error) {
    matlab::data::ArrayFactory factory;
    matlabPtr->feval(matlab::engine::convertUTF8StringToUTF16String("error"), 0,
                     std::vector<matlab::data::Array>({factory.createScalar(error)}));
}

void MexUtility::printf(std::shared_ptr<matlab::engine::MATLABEngine> matlabPtr, const std::string output){
    matlab::data::ArrayFactory factory;

    // Pass stream content to MATLAB fprintf function
    matlabPtr->feval(matlab::engine::convertUTF8StringToUTF16String("fprintf"), 0,
                     std::vector<matlab::data::Array>({factory.createScalar(output)}));
}

