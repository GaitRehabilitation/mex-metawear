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

class MexUtility {
public:
    static void error(std::shared_ptr<matlab::engine::MATLABEngine> matlabPtr, std::string error);
    static void printf(std::shared_ptr<matlab::engine::MATLABEngine> matlabPtr, std::string output);
private:
    MexUtility() = default;
};

#endif //MEX_METAWEAR_MEXUTILITY_H
