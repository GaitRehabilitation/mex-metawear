//
// Created by michaelpollind on 7/14/18.
//


#include <MexUtility.h>

#include "MatlabDataArray.hpp"

void MexUtility::error(std::shared_ptr<matlab::engine::MATLABEngine> matlabPtr,const std::string error) {
    matlab::data::ArrayFactory factory;
  //  matlabPtr->feval(matlab::engine::convertUTF8StringToUTF16String("error"), 0,
   //                  std::vector<matlab::data::Array>({factory.createScalar(error)}));
}
