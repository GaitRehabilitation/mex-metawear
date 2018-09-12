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
#include <metawear/sensor/accelerometer.h>
#include <metawear/sensor/magnetometer_bmm150.h>
#include <metawear/sensor/sensor_fusion.h>
#include <metawear/core/logging.h>
#include "handlers/CaptureHandler.h"
#include "handlers/ConnectionHandler.h"

CaptureHandler::CaptureHandler(ConnectionHandler *connectionHandler, FunctionWrapper *wrapper):
    m_connectionHandler(connectionHandler){
    std::map<std::string, WrapperMethod *> functions =  {
            //enable
            {"enable_gyro", mexEnableGyro},
            {"enable_accelerometer", mexEnableAccelerometer},
            {"enable_magnetometer", mexEnableMagnetometer},
            {"set_sensor_fusion_flag",mexSetSensorFusionFlag},
            {"enable_sensorfusion", mexEnableSensorFusion},

            //disable
            {"disable_gyro",mexDisableGyro},
            {"disable_accelerometer",mexDisableAccelerometer},
            {"disable_magnetometer", mexDisableMagnetometer},
            {"disable_sensorfusion",mexDisableSensorFusion},

            // metawear('subscribe_acc',boolean: log or not to log, {filters})
            {"subscribe_acc", mexSubscribeAcc},
            {"subscribe_gyro", mexSubscribeGyro},
            {"subscribe_fusion", mexSubscribeFusion},

            {"query", mexQuery},
            {"unsubscribe",mexUnSubscribe},

            {"start_logging", mexStartLogger},
            {"stop_logging", mexStopLogger}

    };
    wrapper->registerMethod(this, functions);
}

void CaptureHandler::mexStartLogger(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs){
    CaptureHandler* handler = static_cast<CaptureHandler*>(context);

    MexUtility::checkNumberOfParameters(engine,MexUtility::ParameterType::INPUT,inputs.size(),2);
    MexUtility::checkType(engine,MexUtility::ParameterType::INPUT,1,inputs[1].getType(),matlab::data::ArrayType::CHAR);

    matlab::data::CharArray address =  inputs[1];
    MetawearWrapper* wrapper =  handler->m_connectionHandler->getDevice(address.toAscii());
    if(wrapper == nullptr)  MexUtility::error(engine, "Unknown Sensor");
    MblMwMetaWearBoard*  board = wrapper->getBoard();

    mbl_mw_logging_start(board,0);
}
void CaptureHandler::mexStopLogger(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs){
    CaptureHandler* handler = static_cast<CaptureHandler*>(context);

    MexUtility::checkNumberOfParameters(engine,MexUtility::ParameterType::INPUT,inputs.size(),2);
    MexUtility::checkType(engine,MexUtility::ParameterType::INPUT,1,inputs[1].getType(),matlab::data::ArrayType::CHAR);

    matlab::data::CharArray address =  inputs[1];
    MetawearWrapper* wrapper =  handler->m_connectionHandler->getDevice(address.toAscii());
    if(wrapper == nullptr)  MexUtility::error(engine, "Unknown Sensor");
    MblMwMetaWearBoard*  board = wrapper->getBoard();

    static auto progress_update = [](void* context, uint32_t entries_left, uint32_t total_entries)-> void {
        printf("download progress= %d / %d\n", entries_left, total_entries);
        if (!entries_left) {
            printf("download complete\n");
        }
    };
    static auto unknown_entry = [](void* context, uint8_t id, int64_t epoch, const uint8_t* data, uint8_t length) -> void {
        printf("received unknown log entry: id= %d\n", id);
    };

    static MblMwLogDownloadHandler download_handler = { nullptr, progress_update, unknown_entry };
    mbl_mw_logging_download(board,100,&download_handler);
}

void CaptureHandler::mexUnSubscribe(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs) {
    CaptureHandler *handler = static_cast<CaptureHandler *>(context);

    MexUtility::checkNumberOfParameters(engine, MexUtility::ParameterType::INPUT, inputs.size(), 3);
    MexUtility::checkNumberOfParameters(engine, MexUtility::ParameterType::OUTPUT, outputs.size(), 1);
    MexUtility::checkType(engine, MexUtility::ParameterType::INPUT, 1, inputs[1].getType(),
                          matlab::data::ArrayType::CHAR);
    MexUtility::checkType(engine, MexUtility::ParameterType::INPUT, 2, inputs[2].getType(),
                          matlab::data::ArrayType::CHAR);

    matlab::data::CharArray address = inputs[1];
    matlab::data::CharArray key = inputs[2];
    MetawearWrapper *wrapper = handler->m_connectionHandler->getDevice(address.toAscii());
    if (wrapper == nullptr) MexUtility::error(engine, "Unknown Sensor");
    wrapper->removeHandler(key.toAscii());
}

void CaptureHandler::mexQuery(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs){
    CaptureHandler *handler = static_cast<CaptureHandler *>(context);

    MexUtility::checkNumberOfParameters(engine, MexUtility::ParameterType::INPUT, inputs.size(), 3);
    MexUtility::checkType(engine, MexUtility::ParameterType::INPUT, 1, inputs[1].getType(),
                          matlab::data::ArrayType::CHAR);
    MexUtility::checkType(engine, MexUtility::ParameterType::INPUT, 2, inputs[2].getType(),
                          matlab::data::ArrayType::CHAR);

    matlab::data::CharArray address = inputs[1];
    matlab::data::CharArray key = inputs[2];
    MetawearWrapper *wrapper = handler->m_connectionHandler->getDevice(address.toAscii());
    if (wrapper == nullptr) MexUtility::error(engine, "Unknown Sensor");

    StreamHandler* streamHandler = wrapper->getHandler(key.toAscii());
    //lock stream so data can retrieved without being modified
    if(streamHandler->isEmpty()){
        matlab::data::ArrayFactory factory;
        for(int x = 0; x < outputs.size(); ++x) {
            matlab::data::TypedArray<double> empty = factory.createArray<double>({1, 0});
            outputs[x] = empty;
        }
    }
    else {
        MblMwDataTypeId target = streamHandler->peek()->getType();
        switch (target) {
            case MblMwDataTypeId::MBL_MW_DT_ID_UINT32:
                break;
            case MblMwDataTypeId::MBL_MW_DT_ID_FLOAT:

                break;
            case MblMwDataTypeId::MBL_MW_DT_ID_CARTESIAN_FLOAT: {
                MexUtility::checkNumberOfParameters(engine, MexUtility::ParameterType::OUTPUT, outputs.size(), 4);
                streamHandler->lockStream();
                matlab::data::ArrayFactory factory;
                matlab::data::TypedArray<int64_t> epochs = factory.createArray<int64_t>({1, streamHandler->size()});
                matlab::data::TypedArray<double> x = factory.createArray<double>({1, streamHandler->size()});
                matlab::data::TypedArray<double> y = factory.createArray<double>({1, streamHandler->size()});
                matlab::data::TypedArray<double> z = factory.createArray<double>({1, streamHandler->size()});
                unsigned int i = 0;
                while (!streamHandler->isEmpty()) {
                    auto entry = streamHandler->peek();
                    streamHandler->pop();
                    auto c = static_cast<MblMwCartesianFloat *>(entry->getData());
                    epochs[i] = entry->getEpoch();

                    x[i] = c->x;
                    y[i] = c->y;
                    z[i] = c->z;
                    free(entry);
                    ++i;
                }
                streamHandler->unLockStream();
                outputs[0] = epochs;
                outputs[1] = x;
                outputs[2] = y;
                outputs[3] = z;
            }
                break;
            case MblMwDataTypeId::MBL_MW_DT_ID_EULER_ANGLE: {
                MexUtility::checkNumberOfParameters(engine, MexUtility::ParameterType::OUTPUT, outputs.size(), 5);
                streamHandler->lockStream();
                matlab::data::ArrayFactory factory;
                matlab::data::TypedArray<int64_t> epochs = factory.createArray<int64_t>({1, streamHandler->size()});
                matlab::data::TypedArray<double> yaw = factory.createArray<double>({1, streamHandler->size()});
                matlab::data::TypedArray<double> pitch = factory.createArray<double>({1, streamHandler->size()});
                matlab::data::TypedArray<double> roll = factory.createArray<double>({1, streamHandler->size()});
                matlab::data::TypedArray<double> heading = factory.createArray<double>({1, streamHandler->size()});

                unsigned int i = 0;
                while (!streamHandler->isEmpty()) {
                    auto entry = streamHandler->peek();
                    streamHandler->pop();
                    auto c = static_cast<MblMwEulerAngles *>(entry->getData());
                    epochs[i] = entry->getEpoch();

                    yaw[i] = c->yaw;
                    roll[i] = c->roll;
                    pitch[i] = c->pitch;
                    heading[i] = c->heading;

                    free(entry);
                    ++i;
                }
                streamHandler->unLockStream();
                outputs[0] = epochs;
                outputs[1] = yaw;
                outputs[2] = pitch;
                outputs[3] = roll;
                outputs[4] = heading;
            }
                break;
            case MblMwDataTypeId::MBL_MW_DT_ID_QUATERNION: {
                MexUtility::checkNumberOfParameters(engine, MexUtility::ParameterType::OUTPUT, outputs.size(), 5);
                streamHandler->lockStream();
                matlab::data::ArrayFactory factory;
                matlab::data::TypedArray<int64_t> epochs = factory.createArray<int64_t>({1, streamHandler->size()});
                matlab::data::TypedArray<double> x = factory.createArray<double>({1, streamHandler->size()});
                matlab::data::TypedArray<double> y = factory.createArray<double>({1, streamHandler->size()});
                matlab::data::TypedArray<double> z = factory.createArray<double>({1, streamHandler->size()});
                matlab::data::TypedArray<double> w = factory.createArray<double>({1, streamHandler->size()});

                unsigned int i = 0;
                while (!streamHandler->isEmpty()) {
                    auto entry = streamHandler->peek();
                    streamHandler->pop();
                    auto c = static_cast<MblMwQuaternion *>(entry->getData());
                    epochs[i] = entry->getEpoch();

                    x[i] = c->x;
                    y[i] = c->y;
                    z[i] = c->z;
                    w[i] = c->w;

                    free(entry);
                    ++i;
                }
                streamHandler->unLockStream();
                outputs[0] = epochs;
                outputs[1] = x;
                outputs[2] = y;
                outputs[3] = z;
                outputs[4] = w;
            }
                break;
            case MblMwDataTypeId::MBL_MW_DT_ID_CORRECTED_CARTESIAN_FLOAT:
                break;
            case MblMwDataTypeId::MBL_MW_DT_ID_INT32:
                break;
            case MblMwDataTypeId::MBL_MW_DT_ID_BYTE_ARRAY:
                break;
            case MblMwDataTypeId::MBL_MW_DT_ID_BATTERY_STATE:
                break;
            case MblMwDataTypeId::MBL_MW_DT_ID_TCS34725_ADC:
                break;
            default:
                break;
            case MBL_MW_DT_ID_OVERFLOW_STATE:
                break;
            case MBL_MW_DT_ID_SENSOR_ORIENTATION:
                break;
            case MBL_MW_DT_ID_STRING:
                break;
            case MBL_MW_DT_ID_LOGGING_TIME:
                break;
            case MBL_MW_DT_ID_BTLE_ADDRESS:
                break;
            case MBL_MW_DT_ID_BOSCH_ANY_MOTION:
                break;
            case MBL_MW_DT_ID_CALIBRATION_STATE:
                break;
        }
    }

}

void CaptureHandler::mexSubscribeGyro(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs){
    CaptureHandler* handler = static_cast<CaptureHandler*>(context);

    MexUtility::checkNumberOfParameters(engine,MexUtility::ParameterType::INPUT,inputs.size(),2);
    MexUtility::checkNumberOfParameters(engine,MexUtility::ParameterType::OUTPUT,outputs.size(),1);
    MexUtility::checkType(engine,MexUtility::ParameterType::INPUT,1,inputs[1].getType(),matlab::data::ArrayType::CHAR);

    matlab::data::CharArray address =  inputs[1];
    MetawearWrapper* wrapper =  handler->m_connectionHandler->getDevice(address.toAscii());
    if(wrapper == nullptr)  MexUtility::error(engine, "Unknown Sensor");
    MblMwMetaWearBoard*  board = wrapper->getBoard();

    auto gyro_signal = mbl_mw_gyro_bmi160_get_packed_rotation_data_signal(board);
    wrapper->registerHandler("gyro",new StreamHandler(gyro_signal,StreamType::STREAMING));

    matlab::data::ArrayFactory factory;
    matlab::data::CharArray key = factory.createCharArray("gyro");
    outputs[0] = key;

}

void CaptureHandler::mexSubscribeAcc(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs){
    CaptureHandler* handler = static_cast<CaptureHandler*>(context);

    MexUtility::checkNumberOfParameters(engine,MexUtility::ParameterType::INPUT,inputs.size(),2);
    MexUtility::checkNumberOfParameters(engine,MexUtility::ParameterType::OUTPUT,outputs.size(),1);
    MexUtility::checkType(engine,MexUtility::ParameterType::INPUT,1,inputs[1].getType(),matlab::data::ArrayType::CHAR);

    matlab::data::CharArray address =  inputs[1];
    MetawearWrapper* wrapper =  handler->m_connectionHandler->getDevice(address.toAscii());
    if(wrapper == nullptr)  MexUtility::error(engine, "Unknown Sensor");
    MblMwMetaWearBoard*  board = wrapper->getBoard();

    auto acc_signal = mbl_mw_acc_get_packed_acceleration_data_signal(board);
    wrapper->registerHandler("acc",new StreamHandler(acc_signal,StreamType::STREAMING));

    matlab::data::ArrayFactory factory;
    matlab::data::CharArray key = factory.createCharArray("acc");
    outputs[0] = key;

}

void CaptureHandler::mexSetSensorFusionFlag(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs) {
    CaptureHandler *handler = static_cast<CaptureHandler *>(context);

    MexUtility::checkNumberOfParameters(engine, MexUtility::ParameterType::INPUT, inputs.size(), 3);
    MexUtility::checkType(engine, MexUtility::ParameterType::INPUT, 1, inputs[1].getType(),
                          matlab::data::ArrayType::CHAR);
    MexUtility::checkType(engine, MexUtility::ParameterType::INPUT, 2, inputs[2].getType(),
                          matlab::data::ArrayType::CHAR);

    matlab::data::CharArray address = inputs[1];
    MetawearWrapper *wrapper = handler->m_connectionHandler->getDevice(address.toAscii());
    if (wrapper == nullptr) MexUtility::error(engine, "Unknown Sensor");
    MblMwMetaWearBoard *board = wrapper->getBoard();

    matlab::data::CharArray type = inputs[2];
    if (type.toAscii() == "CORRECTED_ACC") {
        mbl_mw_sensor_fusion_enable_data(board, MBL_MW_SENSOR_FUSION_DATA_CORRECTED_ACC);
    } else if (type.toAscii() == "CORRECTED_GYRO") {
        mbl_mw_sensor_fusion_enable_data(board, MBL_MW_SENSOR_FUSION_DATA_CORRECTED_GYRO);
    } else if (type.toAscii() == "CORRECTED_MAG") {
        mbl_mw_sensor_fusion_enable_data(board, MBL_MW_SENSOR_FUSION_DATA_CORRECTED_MAG);
    } else if (type.toAscii() == "QUATERNION") {
        mbl_mw_sensor_fusion_enable_data(board, MBL_MW_SENSOR_FUSION_DATA_QUATERNION);
    } else if (type.toAscii() == "EULER_ANGLE") {
        mbl_mw_sensor_fusion_enable_data(board, MBL_MW_SENSOR_FUSION_DATA_EULER_ANGLE);
    } else if (type.toAscii() == "GRAVITY_VECTOR") {
        mbl_mw_sensor_fusion_enable_data(board, MBL_MW_SENSOR_FUSION_DATA_GRAVITY_VECTOR);
    } else if (type.toAscii() == "LINEAR_ACC") {
        mbl_mw_sensor_fusion_enable_data(board, MBL_MW_SENSOR_FUSION_DATA_LINEAR_ACC);
    } else {
        MexUtility::error(engine,
                          "Fusion Supports Parameter: CORRECTED_ACC, CORRECTED_GYRO, QUATERNION, EULER_ANGLE, GRAVITY_VECTOR, LINEAR_ACC");
    }
}

void CaptureHandler::mexSubscribeFusion(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs) {
    CaptureHandler *handler = static_cast<CaptureHandler *>(context);

    MexUtility::checkNumberOfParameters(engine, MexUtility::ParameterType::INPUT, inputs.size(), 3);
    MexUtility::checkNumberOfParameters(engine, MexUtility::ParameterType::OUTPUT, outputs.size(), 1);
    MexUtility::checkType(engine, MexUtility::ParameterType::INPUT, 1, inputs[1].getType(),
                          matlab::data::ArrayType::CHAR);
    MexUtility::checkType(engine, MexUtility::ParameterType::INPUT, 2, inputs[2].getType(),
                          matlab::data::ArrayType::CHAR);

    matlab::data::CharArray address = inputs[1];
    MetawearWrapper *wrapper = handler->m_connectionHandler->getDevice(address.toAscii());
    if (wrapper == nullptr) MexUtility::error(engine, "Unknown Sensor");
    MblMwMetaWearBoard *board = wrapper->getBoard();

    matlab::data::CharArray type = inputs[2];
    if (type.toAscii() == "CORRECTED_ACC") {

        auto quaternion_signal = mbl_mw_sensor_fusion_get_data_signal(board, MBL_MW_SENSOR_FUSION_DATA_CORRECTED_ACC);
        wrapper->registerHandler("fusion_corrected_acc", new StreamHandler(quaternion_signal, StreamType::STREAMING));

        matlab::data::ArrayFactory factory;
        matlab::data::CharArray key = factory.createCharArray("fusion_corrected_acc");
        outputs[0] = key;
    } else if (type.toAscii() == "CORRECTED_GYRO") {

        auto quaternion_signal = mbl_mw_sensor_fusion_get_data_signal(board, MBL_MW_SENSOR_FUSION_DATA_CORRECTED_GYRO);
        wrapper->registerHandler("fusion_corrected_gyro", new StreamHandler(quaternion_signal, StreamType::STREAMING));

        matlab::data::ArrayFactory factory;
        matlab::data::CharArray key = factory.createCharArray("fusion_corrected_gyro");
        outputs[0] = key;
    } else if (type.toAscii() == "CORRECTED_MAG") {

        auto quaternion_signal = mbl_mw_sensor_fusion_get_data_signal(board, MBL_MW_SENSOR_FUSION_DATA_CORRECTED_MAG);
        wrapper->registerHandler("fusion_corrected_mag", new StreamHandler(quaternion_signal, StreamType::STREAMING));

        matlab::data::ArrayFactory factory;
        matlab::data::CharArray key = factory.createCharArray("fusion_corrected_mag");
        outputs[0] = key;
    } else if (type.toAscii() == "QUATERNION") {
        auto quaternion_signal = mbl_mw_sensor_fusion_get_data_signal(board, MBL_MW_SENSOR_FUSION_DATA_QUATERNION);
        wrapper->registerHandler("fusion_quaternion", new StreamHandler(quaternion_signal, StreamType::STREAMING));

        matlab::data::ArrayFactory factory;
        matlab::data::CharArray key = factory.createCharArray("fusion_quaternion");
        outputs[0] = key;
    } else if (type.toAscii() == "EULER_ANGLE") {
        auto quaternion_signal = mbl_mw_sensor_fusion_get_data_signal(board, MBL_MW_SENSOR_FUSION_DATA_EULER_ANGLE);
        wrapper->registerHandler("fusion_euler", new StreamHandler(quaternion_signal, StreamType::STREAMING));

        matlab::data::ArrayFactory factory;
        matlab::data::CharArray key = factory.createCharArray("fusion_euler");
        outputs[0] = key;
    } else if (type.toAscii() == "GRAVITY_VECTOR") {
        auto quaternion_signal = mbl_mw_sensor_fusion_get_data_signal(board, MBL_MW_SENSOR_FUSION_DATA_GRAVITY_VECTOR);
        wrapper->registerHandler("fusion_gravity_vector", new StreamHandler(quaternion_signal, StreamType::STREAMING));

        matlab::data::ArrayFactory factory;
        matlab::data::CharArray key = factory.createCharArray("fusion_gravity_vector");
        outputs[0] = key;
    } else if (type.toAscii() == "LINEAR_ACC") {
        auto quaternion_signal = mbl_mw_sensor_fusion_get_data_signal(board, MBL_MW_SENSOR_FUSION_DATA_LINEAR_ACC);
        wrapper->registerHandler("fusion_linear_acc", new StreamHandler(quaternion_signal, StreamType::STREAMING));

        matlab::data::ArrayFactory factory;
        matlab::data::CharArray key = factory.createCharArray("fusion_linear_acc");
        outputs[0] = key;
    } else {
        MexUtility::error(engine,
                          "Fusion Supports Parameter: CORRECTED_ACC, CORRECTED_GYRO, QUATERNION, EULER_ANGLE, GRAVITY_VECTOR, LINEAR_ACC");
    }
}

void CaptureHandler::mexEnableSensorFusion(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs){
    CaptureHandler* handler = static_cast<CaptureHandler*>(context);
    MexUtility::checkNumberOfParameters(engine,MexUtility::ParameterType::INPUT,inputs.size(),2);
    MexUtility::checkType(engine,MexUtility::ParameterType::INPUT,1,inputs[1].getType(),matlab::data::ArrayType::CHAR);


    matlab::data::CharArray address =  inputs[1];
    MetawearWrapper* wrapper =  handler->m_connectionHandler->getDevice(address.toAscii());
    if(wrapper == nullptr)  MexUtility::error(engine, "Unknown Sensor");
    MblMwMetaWearBoard*  board = wrapper->getBoard();

    mbl_mw_sensor_fusion_start(board);
}

void CaptureHandler::mexDisableSensorFusion(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs){
    CaptureHandler* handler = static_cast<CaptureHandler*>(context);
    MexUtility::checkNumberOfParameters(engine,MexUtility::ParameterType::INPUT,inputs.size(),2);
    MexUtility::checkType(engine,MexUtility::ParameterType::INPUT,1,inputs[1].getType(),matlab::data::ArrayType::CHAR);

    matlab::data::CharArray address =  inputs[1];
    MetawearWrapper* wrapper =  handler->m_connectionHandler->getDevice(address.toAscii());
    if(wrapper == nullptr)  MexUtility::error(engine, "Unknown Sensor");
    MblMwMetaWearBoard*  board = wrapper->getBoard();

    mbl_mw_sensor_fusion_clear_enabled_mask(board);
    mbl_mw_sensor_fusion_stop(board);

}

void CaptureHandler::mexEnableMagnetometer(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs){
    CaptureHandler* handler = static_cast<CaptureHandler*>(context);
    MexUtility::checkNumberOfParameters(engine,MexUtility::ParameterType::INPUT,inputs.size(),2);
    MexUtility::checkType(engine,MexUtility::ParameterType::INPUT,1,inputs[1].getType(),matlab::data::ArrayType::CHAR);

    matlab::data::CharArray address =  inputs[1];
    MetawearWrapper* wrapper =  handler->m_connectionHandler->getDevice(address.toAscii());
    if(wrapper == nullptr)  MexUtility::error(engine, "Unknown Sensor");
    MblMwMetaWearBoard*  board = wrapper->getBoard();

    mbl_mw_mag_bmm150_enable_b_field_sampling(board);
    mbl_mw_mag_bmm150_start(board);
}

void CaptureHandler::mexDisableMagnetometer(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs){
    CaptureHandler* handler = static_cast<CaptureHandler*>(context);
    MexUtility::checkNumberOfParameters(engine,MexUtility::ParameterType::INPUT,inputs.size(),2);
    MexUtility::checkType(engine,MexUtility::ParameterType::INPUT,1,inputs[1].getType(),matlab::data::ArrayType::CHAR);

    matlab::data::CharArray address =  inputs[1];
    MetawearWrapper* wrapper =  handler->m_connectionHandler->getDevice(address.toAscii());
    if(wrapper == nullptr)  MexUtility::error(engine, "Unknown Sensor");
    MblMwMetaWearBoard*  board = wrapper->getBoard();

    mbl_mw_mag_bmm150_disable_b_field_sampling(board);
    mbl_mw_mag_bmm150_stop(board);
}


void CaptureHandler::mexEnableGyro(std::shared_ptr<matlab::engine::MATLABEngine> engine, void *context,
                                    ParameterWrapper &outputs, ParameterWrapper &inputs) {
    CaptureHandler* handler = static_cast<CaptureHandler*>(context);
    MexUtility::checkNumberOfParameters(engine,MexUtility::ParameterType::INPUT,inputs.size(),2);
    MexUtility::checkType(engine,MexUtility::ParameterType::INPUT,1,inputs[1].getType(),matlab::data::ArrayType::CHAR);

    matlab::data::CharArray address =  inputs[1];
    MetawearWrapper* wrapper =  handler->m_connectionHandler->getDevice(address.toAscii());
    if(wrapper == nullptr)  MexUtility::error(engine, "Unknown Sensor");
    MblMwMetaWearBoard*  board = wrapper->getBoard();
    mbl_mw_gyro_bmi160_enable_rotation_sampling(board);
    mbl_mw_gyro_bmi160_start(board);
}

void  CaptureHandler::mexEnableAccelerometer(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs){
    CaptureHandler* handler = static_cast<CaptureHandler*>(context);

    if(inputs.size() != 2){
        MexUtility::error(engine, "Three Inputs Required");
        return;
    }

    matlab::data::CharArray address =  inputs[1];
    MetawearWrapper* wrapper =  handler->m_connectionHandler->getDevice(address.toAscii());
    MblMwMetaWearBoard* board = wrapper->getBoard();
    mbl_mw_acc_enable_acceleration_sampling(board);
    mbl_mw_acc_start(board);
}


void CaptureHandler::mexDisableGyro(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs){
    CaptureHandler* handler = static_cast<CaptureHandler*>(context);

    if(inputs.size() != 2){
        MexUtility::error(engine, "Three Inputs Required");
        return;
    }

    matlab::data::CharArray address =  inputs[1];
    MetawearWrapper* wrapper =  handler->m_connectionHandler->getDevice(address.toAscii());
    MblMwMetaWearBoard* board = wrapper->getBoard();
    mbl_mw_gyro_bmi160_disable_rotation_sampling(board);
    mbl_mw_gyro_bmi160_stop(board);
}

void CaptureHandler::mexDisableAccelerometer(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs) {
    CaptureHandler *handler = static_cast<CaptureHandler *>(context);

    if (inputs.size() != 2) {
        MexUtility::error(engine, "Three Inputs Required");
        return;
    }

    matlab::data::CharArray address = inputs[1];
    MetawearWrapper *wrapper = handler->m_connectionHandler->getDevice(address.toAscii());
    MblMwMetaWearBoard* board = wrapper->getBoard();
    mbl_mw_acc_disable_acceleration_sampling(board);
    mbl_mw_acc_stop(board);
}


CaptureHandler::~CaptureHandler() {

}

