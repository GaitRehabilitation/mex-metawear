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
#include <metawear/core/settings.h>
#include <metawear/sensor/accelerometer.h>
#include <metawear/core/datasignal.h>
#include <metawear/platform/memory.h>
#include <metawear/core/status.h>
#include <metawear/sensor/gyro_bmi160.h>
#include <metawear/sensor/magnetometer_bmm150.h>
#include <metawear/sensor/barometer_bosch.h>


ConfigurationHandler::ConfigurationHandler(ConnectionHandler* connectionHandler,FunctionWrapper* wrapper) :
        m_connectionHandler(connectionHandler){
    std::map<std::string, WrapperMethod *> functions =  {
            {"configure_gyro_sample", mexConfigureGyroSample},
            {"configure_gyro_range", mexConfigureGyroRange},
            {"configure_gyro_config", mexConfigureGyroWrite},

            {"configure_accelerometer_sample", mexConfigureAccelerometerSample},
            {"configure_accelerometer_range", mexConfigureAccelerometerRange},
            {"configure_accelerometer_config", mexConfigureAccelerometerWrite},

            {"configure_magnetometer_power", mexConfigureMagnetometerPowerPreset}

//            {"configure_ambient_light",mexConfigureAmbientLight},
//            {"configure_barometer",mexConfigureBarometer},
//            {"configure_color",mexConfigureAccelerometer},
//            {"configure_humidity",mexConfigureHumidity},
//            {"configure_magnetometer",mexConfigureMagnetometer},
//            {"configure_proximity",mexConfigureProximity},
//            {"configure_fusion",mexConfigureSensorFusion}

    };
    wrapper->registerMethod(this, functions);
}



void ConfigurationHandler::mexConfigureAccelerometerSample(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,
        ParameterWrapper& outputs, ParameterWrapper& inputs){
    ConfigurationHandler* handler = static_cast<ConfigurationHandler*>(context);

    MexUtility::checkNumberOfParameters(engine,MexUtility::ParameterType::INPUT,inputs.size(),4);
    MexUtility::checkType(engine,MexUtility::ParameterType::INPUT,1,inputs[1].getType(),matlab::data::ArrayType::CHAR);
    MexUtility::checkType(engine,MexUtility::ParameterType::INPUT,2,inputs[2].getType(),matlab::data::ArrayType::DOUBLE);

    matlab::data::CharArray address =  inputs[1];
    matlab::data::TypedArray<double> sample = inputs[2];
    MetawearWrapper* wrapper =  handler->m_connectionHandler->getDevice(address.toAscii());
    if(wrapper == nullptr)  MexUtility::error(engine, "Unknown Sensor");
    MblMwMetaWearBoard*  board = wrapper->getBoard();
    mbl_mw_acc_set_odr(board ,(float)sample[0]);
}

void ConfigurationHandler::mexConfigureAccelerometerRange(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,
                                                           ParameterWrapper& outputs, ParameterWrapper& inputs){
    ConfigurationHandler* handler = static_cast<ConfigurationHandler*>(context);

    MexUtility::checkNumberOfParameters(engine,MexUtility::ParameterType::INPUT,inputs.size(),3);
    MexUtility::checkType(engine,MexUtility::ParameterType::INPUT,1,inputs[1].getType(),matlab::data::ArrayType::CHAR);
    MexUtility::checkType(engine,MexUtility::ParameterType::INPUT,2,inputs[2].getType(),matlab::data::ArrayType::DOUBLE);

    matlab::data::CharArray address =  inputs[1];
    matlab::data::TypedArray<double> range = inputs[2];
    MetawearWrapper* wrapper =  handler->m_connectionHandler->getDevice(address.toAscii());
    if(wrapper == nullptr)  MexUtility::error(engine, "Unknown Sensor");
    MblMwMetaWearBoard*  board = wrapper->getBoard();
    mbl_mw_acc_set_range(board ,(float)range[0]);

}

void ConfigurationHandler::mexConfigureAccelerometerWrite(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs){
    ConfigurationHandler* handler = static_cast<ConfigurationHandler*>(context);

    MexUtility::checkNumberOfParameters(engine,MexUtility::ParameterType::INPUT,inputs.size(),3);
    MexUtility::checkType(engine,MexUtility::ParameterType::INPUT,1,inputs[1].getType(),matlab::data::ArrayType::CHAR);
    MexUtility::checkType(engine,MexUtility::ParameterType::INPUT,2,inputs[2].getType(),matlab::data::ArrayType::DOUBLE);

    matlab::data::CharArray address =  inputs[1];
    matlab::data::TypedArray<double> range = inputs[2];
    MetawearWrapper* wrapper =  handler->m_connectionHandler->getDevice(address.toAscii());
    if(wrapper == nullptr)  MexUtility::error(engine, "Unknown Sensor");
    MblMwMetaWearBoard*  board = wrapper->getBoard();
    mbl_mw_acc_write_acceleration_config(board );
}



void ConfigurationHandler::mexConfigureGyroWrite(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs){
    ConfigurationHandler* handler = static_cast<ConfigurationHandler*>(context);

    MexUtility::checkNumberOfParameters(engine,MexUtility::ParameterType::INPUT,inputs.size(),2);
    MexUtility::checkType(engine,MexUtility::ParameterType::INPUT,1,inputs[1].getType(),matlab::data::ArrayType::CHAR);

    matlab::data::CharArray address =  inputs[1];
    MetawearWrapper* wrapper =  handler->m_connectionHandler->getDevice(address.toAscii());
    if(wrapper == nullptr)  MexUtility::error(engine, "Unknown Sensor");
    MblMwMetaWearBoard*  board = wrapper->getBoard();
    mbl_mw_gyro_bmi160_write_config(board);
}
void ConfigurationHandler::mexConfigureGyroRange(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs) {
    ConfigurationHandler *handler = static_cast<ConfigurationHandler *>(context);

    MexUtility::checkNumberOfParameters(engine, MexUtility::ParameterType::INPUT, inputs.size(), 3);
    MexUtility::checkType(engine, MexUtility::ParameterType::INPUT, 1, inputs[1].getType(), matlab::data::ArrayType::CHAR);
    MexUtility::checkType(engine, MexUtility::ParameterType::INPUT, 2, inputs[2].getType(), matlab::data::ArrayType::DOUBLE);

    matlab::data::TypedArray<double> input = inputs[2];

    MblMwGyroBmi160Range range;
    switch ((int) input[0]) {
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
    }
    matlab::data::CharArray address =  inputs[1];
    MetawearWrapper* wrapper =  handler->m_connectionHandler->getDevice(address.toAscii());
    if(wrapper == nullptr)  MexUtility::error(engine, "Unknown Sensor");
    MblMwMetaWearBoard*  board = wrapper->getBoard();
    mbl_mw_gyro_bmi160_set_range(board, range);
}


    void ConfigurationHandler::mexConfigureGyroSample(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs){
    ConfigurationHandler* handler = static_cast<ConfigurationHandler*>(context);

    MexUtility::checkNumberOfParameters(engine,MexUtility::ParameterType::INPUT,inputs.size(),3);
    MexUtility::checkType(engine,MexUtility::ParameterType::INPUT,1,inputs[1].getType(),matlab::data::ArrayType::CHAR);
    MexUtility::checkType(engine,MexUtility::ParameterType::INPUT,2,inputs[2].getType(),matlab::data::ArrayType::DOUBLE);

    matlab::data::TypedArray<double> input = inputs[2];
    MblMwGyroBmi160Odr sample;
    switch((int)input[0]){
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

    matlab::data::CharArray address =  inputs[1];
    MetawearWrapper* wrapper =  handler->m_connectionHandler->getDevice(address.toAscii());
    if(wrapper == nullptr)  MexUtility::error(engine, "Unknown Sensor");
    MblMwMetaWearBoard*  board = wrapper->getBoard();
    mbl_mw_gyro_bmi160_set_odr(board,sample);
}


void ConfigurationHandler::mexConfigureMagnetometerPowerPreset(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs)
{
    ConfigurationHandler* handler = static_cast<ConfigurationHandler*>(context);
    MexUtility::checkNumberOfParameters(engine,MexUtility::ParameterType::INPUT,inputs.size(),3);
    MexUtility::checkType(engine, MexUtility::ParameterType::INPUT, 1, inputs[1].getType(), matlab::data::ArrayType::CHAR);
    MexUtility::checkType(engine, MexUtility::ParameterType::INPUT, 2, inputs[2].getType(), matlab::data::ArrayType::CHAR);

    matlab::data::CharArray address =  inputs[1];
    matlab::data::CharArray input = inputs[2];
    MblMwMagBmm150Preset preset;
    if(input.toAscii() == "LOW_POWER"){
        preset = MBL_MW_MAG_BMM150_PRESET_LOW_POWER;
    }
    else if(input.toAscii()  == "REGULAR"){
        preset = MBL_MW_MAG_BMM150_PRESET_REGULAR;
    }
    else if(input.toAscii()  == "ENHANCED_REGULAR"){
        preset = MBL_MW_MAG_BMM150_PRESET_ENHANCED_REGULAR;
    }
    else if(input.toAscii()  == "HIGH_ACCURACY"){
        preset = MBL_MW_MAG_BMM150_PRESET_HIGH_ACCURACY;
    }
    else{
        MexUtility::error(engine, "Supported power preset: LOW_POWER, REGULAR, ENHANCED_REGULAR, HIGH_ACCURACY");
    }
    MetawearWrapper* wrapper =  handler->m_connectionHandler->getDevice(address.toAscii());
    if(wrapper == nullptr)  MexUtility::error(engine, "Unknown Sensor");
    MblMwMetaWearBoard*  board = wrapper->getBoard();
    mbl_mw_mag_bmm150_set_preset(board,preset);

}

void ConfigurationHandler::mexConfigureBarometer(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs){
    ConfigurationHandler* handler = static_cast<ConfigurationHandler*>(context);

    MexUtility::checkNumberOfParameters(engine,MexUtility::ParameterType::INPUT,inputs.size(),4);
    MexUtility::checkType(engine,MexUtility::ParameterType::INPUT,1,inputs[1].getType(),matlab::data::ArrayType::CHAR);
    MexUtility::checkType(engine,MexUtility::ParameterType::INPUT,2,inputs[2].getType(),matlab::data::ArrayType::DOUBLE);
    MexUtility::checkType(engine,MexUtility::ParameterType::INPUT,3,inputs[3].getType(),matlab::data::ArrayType::DOUBLE);

    matlab::data::CharArray address =  inputs[1];
    matlab::data::TypedArray<double> s = inputs[2];
    matlab::data::TypedArray<double> r = inputs[3];

    MetawearWrapper* wrapper =  handler->m_connectionHandler->getDevice(address.toAscii());
    if(wrapper == nullptr)  MexUtility::error(engine, "Unknown Sensor");
    MblMwMetaWearBoard*  board = wrapper->getBoard();
    mbl_mw_baro_bosch_set_oversampling(board, MblMwBaroBoschOversampling::MBL_MW_BARO_BOSCH_OVERSAMPLING_ULTRA_HIGH);
    mbl_mw_baro_bosch_set_standby_time(board, 62.5f);
    mbl_mw_baro_bosch_set_iir_filter(board, MBL_MW_BARO_BOSCH_IIR_FILTER_AVG_4);
    mbl_mw_baro_bosch_write_config(board);
}

void ConfigurationHandler::mexConfigureSensorFusion(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs){

}


void ConfigurationHandler::mexConfigureProximity(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs){

}

void ConfigurationHandler::mexConfigureAmbientLight(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs){

}

void ConfigurationHandler::mexConfigureTemperature(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs){

}

void ConfigurationHandler::mexConfigureHumidity(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs){

}





