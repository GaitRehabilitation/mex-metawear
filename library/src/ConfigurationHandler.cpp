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
#include <metawear/sensor/sensor_fusion.h>


ConfigurationHandler::ConfigurationHandler(ConnectionHandler* connectionHandler,FunctionWrapper* wrapper) :
        m_connectionHandler(connectionHandler){
    std::map<std::string, WrapperMethod *> functions =  {
            {"configure_gyro_sample", mexConfigureGyroSample},
            {"configure_gyro_range", mexConfigureGyroRange},
            {"configure_gyro_config", mexConfigureGyroWrite},

            {"configure_accelerometer_sample", mexConfigureAccelerometerSample},
            {"configure_accelerometer_range", mexConfigureAccelerometerRange},
            {"configure_accelerometer_config", mexConfigureAccelerometerWrite},

            {"configure_magnetometer_power", mexConfigureMagnetometerPowerPreset},

            {"configure_barometer_oversampling", mexConfigureBarometerOversampling},
            {"configure_barometer_standby", mexConfigureBarometerStandby},
            {"configure_barometer_impulseFilter", mexConfigureBarometerImpulseFilter},
            {"configure_barometer_write", mexConfigureBarometerWrite},

            {"configure_fusion_mode", mexConfigureSensorFusionMode},
            {"configure_fusion_range", mexConfigureSensorFusionRange},
            {"configure_fusion_write", mexConfigureSensorFusionWrite},

            {"fusion_calibrate",mexFusionCalibrate},
            {"set_connection_parameters",mexConfigureConnectionSettings},

            {"teardown",mexTeardown}
    };
    wrapper->registerMethod(this, functions);
}



void ConfigurationHandler::mexConfigureConnectionSettings(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs) {
    ConfigurationHandler* handler = static_cast<ConfigurationHandler*>(context);

    MexUtility::checkNumberOfParameters(engine, MexUtility::ParameterType::INPUT, inputs.size(), 6);
    MexUtility::checkType(engine, MexUtility::ParameterType::INPUT, 1, inputs[1].getType(),matlab::data::ArrayType::CHAR);
    MexUtility::checkType(engine, MexUtility::ParameterType::INPUT, 2, inputs[2].getType(),matlab::data::ArrayType::DOUBLE);
    MexUtility::checkType(engine, MexUtility::ParameterType::INPUT, 3, inputs[2].getType(),matlab::data::ArrayType::DOUBLE);
    MexUtility::checkType(engine, MexUtility::ParameterType::INPUT, 4, inputs[2].getType(),matlab::data::ArrayType::DOUBLE);
    MexUtility::checkType(engine, MexUtility::ParameterType::INPUT, 5, inputs[2].getType(),matlab::data::ArrayType::DOUBLE);

    matlab::data::CharArray address = inputs[1];
    matlab::data::TypedArray<double> min_connection_interval = inputs[2];
    matlab::data::TypedArray<double> max_connection_interval = inputs[3];
    matlab::data::TypedArray<double> latency= inputs[4];
    matlab::data::TypedArray<double> timeout= inputs[5];

    MetawearWrapper *wrapper = handler->m_connectionHandler->mexGetDeviceAndVerify(engine,address.toAscii());
    MblMwMetaWearBoard *board = wrapper->getBoard();

    mbl_mw_settings_set_connection_parameters(board, static_cast<float>(min_connection_interval[0]), static_cast<float>(max_connection_interval[0]), static_cast<uint16_t>(latency[0]), (uint16_t)timeout[0]);
}


void ConfigurationHandler::mexConfigureAccelerometerSample(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,
        ParameterWrapper& outputs, ParameterWrapper& inputs){
    ConfigurationHandler* handler = static_cast<ConfigurationHandler*>(context);

    MexUtility::checkNumberOfParameters(engine,MexUtility::ParameterType::INPUT,inputs.size(),3);
    MexUtility::checkType(engine,MexUtility::ParameterType::INPUT,1,inputs[1].getType(),matlab::data::ArrayType::CHAR);
    MexUtility::checkType(engine,MexUtility::ParameterType::INPUT,2,inputs[2].getType(),matlab::data::ArrayType::DOUBLE);

    matlab::data::CharArray address =  inputs[1];
    matlab::data::TypedArray<double> sample = inputs[2];
    MetawearWrapper* wrapper =  handler->m_connectionHandler->mexGetDeviceAndVerify(engine,address.toAscii());
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
    MetawearWrapper* wrapper =  handler->m_connectionHandler->mexGetDeviceAndVerify(engine,address.toAscii());
    MblMwMetaWearBoard*  board = wrapper->getBoard();
    mbl_mw_acc_set_range(board ,(float)range[0]);

}

void ConfigurationHandler::mexConfigureAccelerometerWrite(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs){
    ConfigurationHandler* handler = static_cast<ConfigurationHandler*>(context);

    MexUtility::checkNumberOfParameters(engine,MexUtility::ParameterType::INPUT,inputs.size(),2);
    MexUtility::checkType(engine,MexUtility::ParameterType::INPUT,1,inputs[1].getType(),matlab::data::ArrayType::CHAR);

    matlab::data::CharArray address =  inputs[1];
    MetawearWrapper* wrapper =  handler->m_connectionHandler->mexGetDeviceAndVerify(engine,address.toAscii());
    MblMwMetaWearBoard*  board = wrapper->getBoard();
    mbl_mw_acc_write_acceleration_config(board );
}



void ConfigurationHandler::mexConfigureGyroWrite(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs){
    ConfigurationHandler* handler = static_cast<ConfigurationHandler*>(context);

    MexUtility::checkNumberOfParameters(engine,MexUtility::ParameterType::INPUT,inputs.size(),2);
    MexUtility::checkType(engine,MexUtility::ParameterType::INPUT,1,inputs[1].getType(),matlab::data::ArrayType::CHAR);

    matlab::data::CharArray address =  inputs[1];
    MetawearWrapper* wrapper =  handler->m_connectionHandler->mexGetDeviceAndVerify(engine,address.toAscii());
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
    MetawearWrapper* wrapper =  handler->m_connectionHandler->mexGetDeviceAndVerify(engine,address.toAscii());
    if(wrapper == nullptr)  MexUtility::error(engine, "Unknown Sensor");
    MblMwMetaWearBoard*  board = wrapper->getBoard();
    mbl_mw_gyro_bmi160_set_range(board, range);
}


    void ConfigurationHandler::mexConfigureGyroSample(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs){
    ConfigurationHandler* handler = static_cast<ConfigurationHandler*>(context);

    MexUtility::checkNumberOfParameters(engine,MexUtility::ParameterType::INPUT,inputs.size(),3);
    MexUtility::checkType(engine,MexUtility::ParameterType::INPUT,1,inputs[1].getType(),matlab::data::ArrayType::CHAR);
    MexUtility::checkType(engine,MexUtility::ParameterType::INPUT,2,inputs[2].getType(),matlab::data::ArrayType::DOUBLE);

    matlab::data::CharArray address =  inputs[1];
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

    MetawearWrapper* wrapper =  handler->m_connectionHandler->mexGetDeviceAndVerify(engine,address.toAscii());
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
    MetawearWrapper* wrapper =  handler->m_connectionHandler->mexGetDeviceAndVerify(engine,address.toAscii());
    MblMwMetaWearBoard*  board = wrapper->getBoard();
    mbl_mw_mag_bmm150_set_preset(board,preset);
}



void ConfigurationHandler::mexConfigureBarometerStandby(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs){
    ConfigurationHandler* handler = static_cast<ConfigurationHandler*>(context);
    MexUtility::checkNumberOfParameters(engine,MexUtility::ParameterType::INPUT,inputs.size(),3);
    MexUtility::checkType(engine,MexUtility::ParameterType::INPUT,1,inputs[1].getType(),matlab::data::ArrayType::CHAR);
    MexUtility::checkType(engine,MexUtility::ParameterType::INPUT,2,inputs[2].getType(),matlab::data::ArrayType::DOUBLE);

    matlab::data::CharArray address =  inputs[1];
    matlab::data::TypedArray<double> input = inputs[2];

    MetawearWrapper* wrapper =  handler->m_connectionHandler->mexGetDeviceAndVerify(engine,address.toAscii());
    if(wrapper == nullptr)  MexUtility::error(engine, "Unknown Sensor");
    MblMwMetaWearBoard*  board = wrapper->getBoard();
    mbl_mw_baro_bosch_set_standby_time(board, (float)input[0]);

}

void ConfigurationHandler::mexConfigureBarometerOversampling(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs){
    ConfigurationHandler* handler = static_cast<ConfigurationHandler*>(context);
    MexUtility::checkNumberOfParameters(engine,MexUtility::ParameterType::INPUT,inputs.size(),3);
    MexUtility::checkType(engine,MexUtility::ParameterType::INPUT,1,inputs[1].getType(),matlab::data::ArrayType::CHAR);
    MexUtility::checkType(engine,MexUtility::ParameterType::INPUT,2,inputs[2].getType(),matlab::data::ArrayType::CHAR);

    matlab::data::CharArray address = inputs[1];
    matlab::data::CharArray input = inputs[2];

    MetawearWrapper* wrapper =  handler->m_connectionHandler->mexGetDeviceAndVerify(engine,address.toAscii());
    MblMwMetaWearBoard*  board = wrapper->getBoard();

    MblMwBaroBoschOversampling oversampling = MblMwBaroBoschOversampling::MBL_MW_BARO_BOSCH_OVERSAMPLING_SKIP;
    if(input.toAscii() == "SKIP"){
        oversampling = MblMwBaroBoschOversampling::MBL_MW_BARO_BOSCH_OVERSAMPLING_SKIP;
    } else if(input.toAscii()  == "ULTRA_LOW"){
        oversampling = MblMwBaroBoschOversampling::MBL_MW_BARO_BOSCH_OVERSAMPLING_ULTRA_LOW_POWER;
    } else if(input.toAscii()  == "LOW"){
        oversampling = MblMwBaroBoschOversampling::MBL_MW_BARO_BOSCH_OVERSAMPLING_LOW_POWER;
    } else if(input.toAscii()  == "STANDARD"){
        oversampling = MblMwBaroBoschOversampling::MBL_MW_BARO_BOSCH_OVERSAMPLING_STANDARD;
    } else if(input.toAscii()  == "HIGH"){
        oversampling = MblMwBaroBoschOversampling::MBL_MW_BARO_BOSCH_OVERSAMPLING_HIGH;
    } else if(input.toAscii()  == "ULTRA_HIGH"){
        oversampling = MblMwBaroBoschOversampling::MBL_MW_BARO_BOSCH_OVERSAMPLING_ULTRA_HIGH;
    } else {
        MexUtility::error(engine, "Supported power preset: SKIP, ULTRA_LOW, LOW, STANDARD, HIGH, ULTRA_HIGH");
    }
    mbl_mw_baro_bosch_set_oversampling(board, oversampling);

}

void ConfigurationHandler::mexConfigureBarometerImpulseFilter(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs){
    ConfigurationHandler* handler = static_cast<ConfigurationHandler*>(context);
    MexUtility::checkNumberOfParameters(engine,MexUtility::ParameterType::INPUT,inputs.size(),3);
    MexUtility::checkType(engine,MexUtility::ParameterType::INPUT,1,inputs[1].getType(),matlab::data::ArrayType::CHAR);
    MexUtility::checkType(engine,MexUtility::ParameterType::INPUT,2,inputs[2].getType(),matlab::data::ArrayType::DOUBLE);

    matlab::data::CharArray address =  inputs[1];
    matlab::data::TypedArray<double> input = inputs[2];

    MetawearWrapper* wrapper =  handler->m_connectionHandler->mexGetDeviceAndVerify(engine,address.toAscii());
    MblMwMetaWearBoard*  board = wrapper->getBoard();

    MblMwBaroBoschIirFilter iir = MBL_MW_BARO_BOSCH_IIR_FILTER_OFF;
    switch((int)input[0]){
        case 0:
            iir = MBL_MW_BARO_BOSCH_IIR_FILTER_OFF;
            break;
        case 2:
            iir = MBL_MW_BARO_BOSCH_IIR_FILTER_AVG_2;
            break;
        case 4:
            iir = MBL_MW_BARO_BOSCH_IIR_FILTER_AVG_4;
            break;
        case 8:
            iir = MBL_MW_BARO_BOSCH_IIR_FILTER_AVG_8;
            break;
        case 16:
            iir = MBL_MW_BARO_BOSCH_IIR_FILTER_AVG_16;
            break;
        default:
            MexUtility::error(engine, "Supported power preset: 0, 2, 4, 8, 16");
            break;
    }
    mbl_mw_baro_bosch_set_iir_filter(board, iir);
}

void ConfigurationHandler::mexConfigureBarometerWrite(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs){
    ConfigurationHandler* handler = static_cast<ConfigurationHandler*>(context);
    MexUtility::checkNumberOfParameters(engine,MexUtility::ParameterType::INPUT,inputs.size(),2);
    MexUtility::checkType(engine,MexUtility::ParameterType::INPUT,1,inputs[1].getType(),matlab::data::ArrayType::CHAR);

    matlab::data::CharArray address =  inputs[1];
    MetawearWrapper* wrapper =  handler->m_connectionHandler->mexGetDeviceAndVerify(engine,address.toAscii());
    MblMwMetaWearBoard*  board = wrapper->getBoard();
    mbl_mw_baro_bosch_write_config(board);
}


void ConfigurationHandler::mexConfigureSensorFusionMode(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs) {
    ConfigurationHandler* handler = static_cast<ConfigurationHandler*>(context);
    MexUtility::checkNumberOfParameters(engine,MexUtility::ParameterType::INPUT,inputs.size(),3);
    MexUtility::checkType(engine,MexUtility::ParameterType::INPUT,1,inputs[1].getType(),matlab::data::ArrayType::CHAR);
    MexUtility::checkType(engine,MexUtility::ParameterType::INPUT,2,inputs[2].getType(),matlab::data::ArrayType::CHAR);

    matlab::data::CharArray address = inputs[1];
    matlab::data::CharArray input = inputs[2];

    MetawearWrapper* wrapper =  handler->m_connectionHandler->mexGetDeviceAndVerify(engine,address.toAscii());
    MblMwMetaWearBoard*  board = wrapper->getBoard();

    MblMwSensorFusionMode mode = MBL_MW_SENSOR_FUSION_MODE_NDOF;
    if(input.toAscii() == "NDoF"){
        mode = MBL_MW_SENSOR_FUSION_MODE_NDOF;
    } else if(input.toAscii()  == "Sleep"){
        mode = MBL_MW_SENSOR_FUSION_MODE_SLEEP;
    }  else if(input.toAscii()  == "IMUPlus"){
        mode = MBL_MW_SENSOR_FUSION_MODE_IMU_PLUS;
    } else if(input.toAscii()  == "Compass"){
        mode = MBL_MW_SENSOR_FUSION_MODE_COMPASS;
    } else if(input.toAscii()  == "M4G"){
        mode = MBL_MW_SENSOR_FUSION_MODE_M4G;
    } else {
        MexUtility::error(engine, "Supported power preset: Sleep, NDoF, IMUPlus, Compass, M4G");
    }
    mbl_mw_sensor_fusion_set_mode(board, mode);
}
void ConfigurationHandler::mexConfigureSensorFusionRange(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs){
    ConfigurationHandler* handler = static_cast<ConfigurationHandler*>(context);
    MexUtility::checkNumberOfParameters(engine,MexUtility::ParameterType::INPUT,inputs.size(),3);
    MexUtility::checkType(engine,MexUtility::ParameterType::INPUT,1,inputs[1].getType(),matlab::data::ArrayType::CHAR);
    MexUtility::checkType(engine,MexUtility::ParameterType::INPUT,2,inputs[2].getType(),matlab::data::ArrayType::DOUBLE);

    matlab::data::CharArray address =  inputs[1];
    matlab::data::TypedArray<double> input = inputs[2];

    MetawearWrapper* wrapper =  handler->m_connectionHandler->mexGetDeviceAndVerify(engine,address.toAscii());
    MblMwMetaWearBoard*  board = wrapper->getBoard();

    MblMwSensorFusionAccRange fusionRange;
    switch((int)input[0]){
        case 2:
            fusionRange = MBL_MW_SENSOR_FUSION_ACC_RANGE_2G;
            break;
        case 4:
            fusionRange = MBL_MW_SENSOR_FUSION_ACC_RANGE_4G;
            break;
        case 8:
            fusionRange = MBL_MW_SENSOR_FUSION_ACC_RANGE_8G;
            break;
        case 16:
            fusionRange = MBL_MW_SENSOR_FUSION_ACC_RANGE_16G;
            break;
        default:
            MexUtility::error(engine, "Supported power preset: 2, 4, 8, 16");
            break;
    }
    mbl_mw_sensor_fusion_set_acc_range(board, fusionRange);
}
void ConfigurationHandler::mexConfigureSensorFusionWrite(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs){
    ConfigurationHandler* handler = static_cast<ConfigurationHandler*>(context);
    MexUtility::checkNumberOfParameters(engine,MexUtility::ParameterType::INPUT,inputs.size(),2);
    MexUtility::checkType(engine,MexUtility::ParameterType::INPUT,1,inputs[1].getType(),matlab::data::ArrayType::CHAR);

    matlab::data::CharArray address =  inputs[1];
    MetawearWrapper* wrapper =  handler->m_connectionHandler->mexGetDeviceAndVerify(engine,address.toAscii());
    MblMwMetaWearBoard*  board = wrapper->getBoard();
    mbl_mw_sensor_fusion_write_config(board);
}

void ConfigurationHandler::mexConfigureProximity(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs){

}

void ConfigurationHandler::mexConfigureAmbientLight(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs){

}

void ConfigurationHandler::mexConfigureTemperature(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs){

}

void ConfigurationHandler::mexConfigureHumidity(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs){

}

void ConfigurationHandler::mexFusionCalibrate(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs){
    ConfigurationHandler* handler = static_cast<ConfigurationHandler*>(context);
    MexUtility::checkNumberOfParameters(engine,MexUtility::ParameterType::INPUT,inputs.size(),2);
    MexUtility::checkType(engine,MexUtility::ParameterType::INPUT,1,inputs[1].getType(),matlab::data::ArrayType::CHAR);

    matlab::data::CharArray address =  inputs[1];
    MetawearWrapper* wrapper =  handler->m_connectionHandler->mexGetDeviceAndVerify(engine,address.toAscii());
    MblMwMetaWearBoard*  board = wrapper->getBoard();

    auto signal = mbl_mw_sensor_fusion_calibration_state_data_signal(board);
    struct Payload{
        std::queue<MblMwCalibrationState*> queue;
        std::mutex lock;
    };
    auto c = new Payload();
    mbl_mw_datasignal_subscribe(signal, c, [](void* context, const MblMwData* data) {
        Payload* results = ( Payload*)context;
        MblMwCalibrationState* casted = (MblMwCalibrationState*) data->value;
        MblMwCalibrationState* temp = new MblMwCalibrationState();
        memcpy(temp,casted, sizeof(MblMwCalibrationState));
        results->lock.lock();
        results->queue.push(temp);
        results->lock.unlock();
    });

    mbl_mw_sensor_fusion_set_mode(board,MBL_MW_SENSOR_FUSION_MODE_NDOF);
    mbl_mw_sensor_fusion_write_config(board);

    mbl_mw_sensor_fusion_enable_data(board, MBL_MW_SENSOR_FUSION_DATA_CORRECTED_ACC);
    mbl_mw_sensor_fusion_enable_data(board, MBL_MW_SENSOR_FUSION_DATA_CORRECTED_GYRO);
    mbl_mw_sensor_fusion_enable_data(board, MBL_MW_SENSOR_FUSION_DATA_CORRECTED_MAG);
    mbl_mw_sensor_fusion_enable_data(board, MBL_MW_SENSOR_FUSION_DATA_EULER_ANGLE);
    mbl_mw_sensor_fusion_start(board);

    bool isCalibrated = false;
    do {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        c->lock.lock();
        while (!c->queue.empty()) {
            MblMwCalibrationState *entry = c->queue.front();
            MexUtility::printf(engine, "calibration state: {accelerometer: " + std::to_string(entry->accelrometer) +
                                       +", gyroscope: " + std::to_string(entry->gyroscope) +
                                       +", magnetometer: " + std::to_string(entry->magnetometer) +
                                       +"} \n");
            if (!isCalibrated) {
                isCalibrated = (entry->accelrometer == 3 && entry->gyroscope == 3 && entry->magnetometer == 3);
            }
            free(entry);
            c->queue.pop();
        }
        c->lock.unlock();
        mbl_mw_datasignal_read(signal);
    }
    while(!isCalibrated);
    mbl_mw_datasignal_unsubscribe(signal);
    delete(c);
}

void ConfigurationHandler::mexTeardown(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs){
    ConfigurationHandler* handler = static_cast<ConfigurationHandler*>(context);
    MexUtility::checkNumberOfParameters(engine,MexUtility::ParameterType::INPUT,inputs.size(),2);
    MexUtility::checkType(engine,MexUtility::ParameterType::INPUT,1,inputs[1].getType(),matlab::data::ArrayType::CHAR);

    matlab::data::CharArray address =  inputs[1];
    MetawearWrapper* wrapper =  handler->m_connectionHandler->mexGetDeviceAndVerify(engine,address.toAscii());
    MblMwMetaWearBoard*  board = wrapper->getBoard();

    wrapper->tearDown();
}