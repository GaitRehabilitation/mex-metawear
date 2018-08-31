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
#include <MetawearWrapper.h>
#include "handlers/ConfigurationHandler.h"
#include "handlers/ConnectionHandler.h"

ConfigurationHandler::ConfigurationHandler(ConnectionHandler* connectionHandler,FunctionWrapper* wrapper) :
        m_connectionHandler(connectionHandler){
    std::map<std::string, WrapperMethod *> functions =  {
            {"configure_gyro", mexConfigureGyro},
            {"configure_accelerometer", mexConfigureAccelerometer}
    };
    wrapper->registerMethod(this, functions);
}



void ConfigurationHandler::mexConfigureAccelerometer(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,
        ParameterWrapper& outputs, ParameterWrapper& inputs){
    ConfigurationHandler* handler = static_cast<ConfigurationHandler*>(context);

    if(inputs.size() != 4)
        MexUtility::error(engine, "Four Inputs Required");
    if (inputs[1].getType() != matlab::data::ArrayType::CHAR)
        MexUtility::error(engine, "Parameter 1 must be type string");
    if (inputs[2].getType() != matlab::data::ArrayType::DOUBLE)
        MexUtility::error(engine,"Parameter 2 must be type double");
    if (inputs[3].getType() != matlab::data::ArrayType::DOUBLE)
        MexUtility::error(engine, "Parameter 3 must be type double");

    matlab::data::CharArray address =  inputs[1];
    matlab::data::TypedArray<double> range = inputs[2];
    matlab::data::TypedArray<double> samples = inputs[3];
    MetawearWrapper* wrapper =  handler->m_connectionHandler->getDevice(address.toAscii());
    if(wrapper == nullptr){
        MexUtility::error(engine, "Invalid wrapper");
    }
    MexUtility::printf(engine,"configuring accelerometer with range: " + std::to_string((float)range[0]) + "g's samples:"+ std::to_string((float)samples[0]) + "Hz \n");
    wrapper->configureAccelerometer((float)range[0],(float)samples[1]);
}
void ConfigurationHandler::mexConfigureGyro(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs){
    ConfigurationHandler* handler = static_cast<ConfigurationHandler*>(context);

    if(inputs.size() != 4){
        MexUtility::error(engine, "Three Inputs Required");
        return;
    }
    if (inputs[1].getType() != matlab::data::ArrayType::CHAR)
        MexUtility::error(engine,"Parameter 1 must be type char");
    if (inputs[2].getType() != matlab::data::ArrayType::DOUBLE)
        MexUtility::error(engine,"Parameter 2 must be type double");
    if (inputs[3].getType() != matlab::data::ArrayType::DOUBLE)
        MexUtility::error(engine,"Parameter 3 must be type double");

    matlab::data::CharArray address =  inputs[1];
    matlab::data::TypedArray<double> s = inputs[2];
    matlab::data::TypedArray<double> r = inputs[3];

    MetawearWrapper* wrapper =  handler->m_connectionHandler->getDevice(address.toAscii());
    if(wrapper == nullptr){
        MexUtility::error(engine, "Invalid wrapper");
    }
    MblMwGyroBmi160Odr sample;
    MblMwGyroBmi160Range range;
    switch((int)s[0]){
        case 25:
            sample = MBL_MW_GYRO_BMI160_ODR_25Hz;
            break;
        case 50:
            sample = MBL_MW_GYRO_BMI160_ODR_50Hz;
            break;
        case 100:
            sample = MBL_MW_GYRO_BMI160_ODR_100Hz;
            break;
        case 200:
            sample = MBL_MW_GYRO_BMI160_ODR_200Hz;
            break;
        case 400:
            sample = MBL_MW_GYRO_BMI160_ODR_400Hz;
            break;
        case 800:
            sample = MBL_MW_GYRO_BMI160_ODR_800Hz;
            break;
        case 1600:
            sample = MBL_MW_GYRO_BMI160_ODR_1600Hz;
            break;
        case 3200:
            sample = MBL_MW_GYRO_BMI160_ODR_3200Hz;
            break;
        default:
            MexUtility::error(engine, "Gyro supported ranges: 25hz,50hz,100hz,200hz,400hz,800hz,1600hz,3200hz ");
            return;
    }

    switch((int)r[1]){
        case 2000:
            range = MBL_MW_GYRO_BMI160_RANGE_2000dps;
            break;
        case 1000:
            range = MBL_MW_GYRO_BMI160_RANGE_1000dps;
            break;
        case 500:
            range = MBL_MW_GYRO_BMI160_RANGE_500dps;
            break;
        case 250:
            range = MBL_MW_GYRO_BMI160_RANGE_250dps;
            break;
        case 125:
            range = MBL_MW_GYRO_BMI160_RANGE_125dps;
            break;
        default:
            MexUtility::error(engine, "Gyro supported ranges: 2000dps, 1000dps, 500dps, 250dps, 125dps ");
            return;
    }
    MexUtility::printf(engine,"configuring gyro with range: " + std::to_string((int)r[1]) + " samples:"+ std::to_string((int)s[0]) + "\n");

    wrapper->configureGyroscope(range,sample);

}




