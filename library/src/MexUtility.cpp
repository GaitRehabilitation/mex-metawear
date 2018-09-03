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

std::string MexUtility::typeToString(matlab::data::ArrayType type){
    switch (type){
        case matlab::data::ArrayType::LOGICAL:
            return "LOGICAL";
        case matlab::data::ArrayType::CHAR:
            return "CHAR";
        case matlab::data::ArrayType::MATLAB_STRING:
            return "MATLAB_STRING";
        case matlab::data::ArrayType::DOUBLE:
            return "DOUBLE";
        case matlab::data::ArrayType::SINGLE:
            return "SINGLE";
        case matlab::data::ArrayType::INT8:
            return "INT8";
        case matlab::data::ArrayType::UINT8:
            return "UINT8";
        case matlab::data::ArrayType::INT16:
            return "INT16";
        case matlab::data::ArrayType::UINT16:
            return "UINT16";
        case matlab::data::ArrayType::INT32:
            return "INT32";
        case matlab::data::ArrayType::UINT32:
            return "UINT32";
        case matlab::data::ArrayType::INT64:
            return "INT64";
        case matlab::data::ArrayType::UINT64:
            return "UINT64";
        case matlab::data::ArrayType::COMPLEX_DOUBLE:
            return "COMPLEX_DOUBLE";
        case matlab::data::ArrayType::COMPLEX_SINGLE:
            return "COMPLEX_SINGLE";
        case matlab::data::ArrayType::COMPLEX_INT8:
            return "COMPLEX_INT8";
        case matlab::data::ArrayType::COMPLEX_UINT8:
            return "COMPLEX_UINT8";
        case matlab::data::ArrayType::COMPLEX_INT16:
            return "COMPLEX_INT16";
        case matlab::data::ArrayType::COMPLEX_UINT16:
            return "COMPLEX_UINT16";
        case matlab::data::ArrayType::COMPLEX_INT32:
            return "COMPLEX_INT32";
        case matlab::data::ArrayType::COMPLEX_UINT32:
            return "COMPLEX_UINT32";
        case matlab::data::ArrayType::COMPLEX_INT64:
            return "COMPLEX_INT64";
        case matlab::data::ArrayType::COMPLEX_UINT64:
            return "COMPLEX_UINT64";
        case matlab::data::ArrayType::CELL:
            return "CELL";
        case matlab::data::ArrayType::STRUCT:
            return "STRUCT";
        case matlab::data::ArrayType::OBJECT:
            return "OBJECT";
        case matlab::data::ArrayType::VALUE_OBJECT:
            return "VALUE_OBJECT";
        case matlab::data::ArrayType::HANDLE_OBJECT_REF:
            return "HANDLE_OBJECT_REF";
        case matlab::data::ArrayType::ENUM:
            return "ENUM";
        case matlab::data::ArrayType::SPARSE_LOGICAL:
            return "SPARSE_LOGICAL";
        case matlab::data::ArrayType::SPARSE_DOUBLE:
            return "SPARSE_DOUBLE";
        case matlab::data::ArrayType::SPARSE_COMPLEX_DOUBLE:
            return "SPARSE_COMPLEX_DOUBLE";
        case matlab::data::ArrayType::UNKNOWN:
            return "UNKNOWN";
    }
}

void MexUtility::checkType(std::shared_ptr<matlab::engine::MATLABEngine> matlabPtr,ParameterType parameterType,int index,matlab::data::ArrayType type,matlab::data::ArrayType target){
    if(type == target)
        return;
    switch (parameterType){
        case INPUT:
            error(matlabPtr,"Input parameter " + std::to_string(index - 1) + " expects type " + typeToString(target) + " but found " + typeToString(type));
        case OUTPUT:
            error(matlabPtr,"Output parameter " + std::to_string(index - 1) + " expects type " + typeToString(target) + " but found " + typeToString(type));
    }
}

void MexUtility::checkNumberOfParameters(std::shared_ptr<matlab::engine::MATLABEngine> matlabPtr, ParameterType parameterType,size_t current, size_t target){
    if(current == target)
        return;
    switch (parameterType){
        case INPUT:
            error(matlabPtr,"Expects " + std::to_string(target - 1) + "inputs but found " + std::to_string(current - 1));
        case OUTPUT:
            error(matlabPtr,"Expects " + std::to_string(target - 1) + "outputs but found " + std::to_string(current - 1));
    }
}



