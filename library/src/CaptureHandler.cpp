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
#include <handlers/CaptureHandler.h>

#include "handlers/CaptureHandler.h"
#include "handlers/ConnectionHandler.h"
#include <iostream>       // std::cout, std::endl
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

CaptureHandler::CaptureHandler(ConnectionHandler *connectionHandler, FunctionWrapper *wrapper):
    m_connectionHandler(connectionHandler),m_printStream(nullptr){

    std::map<std::string, WrapperMethod *> functions = {
            //enable
            {"enable_gyro",            mexEnableGyro},
            {"enable_accelerometer",   mexEnableAccelerometer},
            {"enable_magnetometer",    mexEnableMagnetometer},
            {"set_sensor_fusion_flag", mexSetSensorFusionFlag},
            {"enable_sensorfusion",    mexEnableSensorFusion},

            //disable
            {"disable_gyro",           mexDisableGyro},
            {"disable_accelerometer",  mexDisableAccelerometer},
            {"disable_magnetometer",   mexDisableMagnetometer},
            {"disable_sensorfusion",   mexDisableSensorFusion},

            {"query",                  mexQuery},

            {"start_logging",          mexStartLogger},
            {"stop_logging",           mexStopLogger},
            {"clear_logging",          mexClearLogger},
            {"download_logging",       mexDownloadLogger},
            {"download_logging_all",   mexDownloadLoggerAll}
    };
    wrapper->registerMethod(this, functions);
}

void CaptureHandler::mexStartLogger(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs){
    CaptureHandler* handler = static_cast<CaptureHandler*>(context);

    MexUtility::checkNumberOfParameters(engine,MexUtility::ParameterType::INPUT,inputs.size(),2);
    MexUtility::checkType(engine,MexUtility::ParameterType::INPUT,1,inputs[1].getType(),matlab::data::ArrayType::CHAR);

    matlab::data::CharArray address =  inputs[1];
    MetawearWrapper* wrapper =  handler->m_connectionHandler->mexGetDeviceAndVerify(engine,address.toAscii());
    MblMwMetaWearBoard*  board = wrapper->getBoard();

    mbl_mw_logging_start(board,0);
}
void CaptureHandler::mexStopLogger(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs){
    CaptureHandler* handler = static_cast<CaptureHandler*>(context);

    MexUtility::checkNumberOfParameters(engine,MexUtility::ParameterType::INPUT,inputs.size(),2);
    MexUtility::checkType(engine,MexUtility::ParameterType::INPUT,1,inputs[1].getType(),matlab::data::ArrayType::CHAR);

    matlab::data::CharArray address =  inputs[1];
    MetawearWrapper* wrapper =  handler->m_connectionHandler->mexGetDeviceAndVerify(engine,address.toAscii());
    MblMwMetaWearBoard*  board = wrapper->getBoard();

    mbl_mw_logging_stop(board);
}

void CaptureHandler::mexClearLogger(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs){
    CaptureHandler* handler = static_cast<CaptureHandler*>(context);

    MexUtility::checkNumberOfParameters(engine,MexUtility::ParameterType::INPUT,inputs.size(),2);
    MexUtility::checkType(engine,MexUtility::ParameterType::INPUT,1,inputs[1].getType(),matlab::data::ArrayType::CHAR);

    matlab::data::CharArray address =  inputs[1];
    MetawearWrapper* wrapper =  handler->m_connectionHandler->mexGetDeviceAndVerify(engine,address.toAscii());
    MblMwMetaWearBoard*  board = wrapper->getBoard();

    mbl_mw_logging_clear_entries(board);
}

void CaptureHandler::mexDownloadLogger(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs){
    CaptureHandler* handler = static_cast<CaptureHandler*>(context);

    MexUtility::checkNumberOfParameters(engine,MexUtility::ParameterType::INPUT,inputs.size(),2);
    MexUtility::checkType(engine,MexUtility::ParameterType::INPUT,1,inputs[1].getType(),matlab::data::ArrayType::CHAR);

    matlab::data::CharArray address =  inputs[1];
    MetawearWrapper* wrapper =  handler->m_connectionHandler->mexGetDeviceAndVerify(engine,address.toAscii());
    MblMwMetaWearBoard*  board = wrapper->getBoard();

    MexUtility::printf(engine,"Started Downloading logs for:" + address.toAscii() + "\n");

    if(handler->m_printStream == nullptr)
        handler->m_printStream = new MexPrintStream(engine);

    handler->m_printStream->setBlock();
    static auto progress_update = [](void* context, uint32_t entries_left, uint32_t total_entries)-> void {
        MexPrintStream* stream = static_cast<MexPrintStream*>(context);
        stream->printf("download progress= " + std::to_string(entries_left) + "/" + std::to_string(total_entries));
        if (!entries_left) {
            stream->printf("download complete");
            stream->release();
        }
    };
    static auto unknown_entry = [](void* context, uint8_t id, int64_t epoch, const uint8_t* data, uint8_t length) -> void {
        MexPrintStream* stream = static_cast<MexPrintStream*>(context);
//        stream->printf("received unknown log entry: id=" + std::to_string(id));
    };

    static MblMwLogDownloadHandler download_handler = {  handler->m_printStream, progress_update, unknown_entry };
    mbl_mw_logging_download(board,100,&download_handler);
    handler->m_printStream->block();

}
void CaptureHandler::mexDownloadLoggerAll(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs){
    CaptureHandler* handler = static_cast<CaptureHandler*>(context);
    std::map<std::string ,MetawearWrapper*> sensors = handler->m_connectionHandler->getDevices();

    if(handler->m_printStream == nullptr)
        handler->m_printStream = new MexPrintStream(engine);

    struct SenCount{
        MexPrintStream* stream;
        std::atomic<size_t > count;
    };
    static SenCount c = {handler->m_printStream};
    c.count = sensors.size();
    c.stream = handler->m_printStream;

    int index = 0;
    for (auto it = sensors.begin(); it != sensors.end(); it++) {
        MetawearWrapper *wrapper = it->second;
        std::string address = it->first;

        handler->m_printStream->setBlock();
        static auto progress_update = [](void *context, uint32_t entries_left, uint32_t total_entries) -> void {
            SenCount *s = static_cast<SenCount *>(context);
            s->stream->printf(
                    "download progress= " + std::to_string(entries_left) + "/" + std::to_string(total_entries));
            if (!entries_left) {
                s->stream->printf("download complete");
                s->count--;
                if (s->count <= 0) {
                    s->stream->release();
                }
            }
        };
        static auto unknown_entry = [](void *context, uint8_t id, int64_t epoch, const uint8_t *data,
                                       uint8_t length) -> void {
            MexPrintStream *stream = static_cast<MexPrintStream *>(context);
//        stream->printf("received unknown log entry: id=" + std::to_string(id));
        };
        MblMwMetaWearBoard *board = wrapper->getBoard();
        MexUtility::printf(engine, "Started Downloading logs for:" + address + "\n");

        static MblMwLogDownloadHandler download_handler = {&c, progress_update, unknown_entry};
        mbl_mw_logging_download(board, 100, &download_handler);
    }
    handler->m_printStream->block();
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
    MetawearWrapper *wrapper = handler->m_connectionHandler->mexGetDeviceAndVerify(engine,address.toAscii());

    StreamHandler* streamHandler = wrapper->getHandler(key.toAscii());
    if(streamHandler == nullptr)
        MexUtility::error(engine,"Unknown handler");
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
            case MBL_MW_DT_ID_CALIBRATION_STATE:{
                MexUtility::checkNumberOfParameters(engine, MexUtility::ParameterType::OUTPUT, outputs.size(), 4);
                streamHandler->lockStream();
                matlab::data::ArrayFactory factory;
                matlab::data::TypedArray<int64_t> epochs = factory.createArray<int64_t>({1, streamHandler->size()});
                matlab::data::TypedArray<double> magnetometer = factory.createArray<double>({1, streamHandler->size()});
                matlab::data::TypedArray<double> accelerometer = factory.createArray<double>({1, streamHandler->size()});
                matlab::data::TypedArray<double> gyroscope = factory.createArray<double>({1, streamHandler->size()});

                unsigned int i = 0;
                while (!streamHandler->isEmpty()) {
                    auto entry = streamHandler->peek();
                    streamHandler->pop();
                    auto c = static_cast<MblMwCalibrationState *>(entry->getData());
                    epochs[i] = entry->getEpoch();

                    magnetometer[i] = c->magnetometer;
                    accelerometer[i] = c->accelrometer;
                    gyroscope[i] = c->gyroscope;
                    free(entry);
                    ++i;
                }
                streamHandler->unLockStream();
                outputs[0] = epochs;
                outputs[1] = accelerometer;
                outputs[2] = gyroscope;
                outputs[3] = magnetometer;
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
        }
    }

}

void CaptureHandler::mexSetSensorFusionFlag(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs) {
    CaptureHandler *handler = static_cast<CaptureHandler *>(context);

    MexUtility::checkNumberOfParameters(engine, MexUtility::ParameterType::INPUT, inputs.size(), 3);
    MexUtility::checkType(engine, MexUtility::ParameterType::INPUT, 1, inputs[1].getType(),
                          matlab::data::ArrayType::CHAR);
    MexUtility::checkType(engine, MexUtility::ParameterType::INPUT, 2, inputs[2].getType(),
                          matlab::data::ArrayType::CHAR);

    matlab::data::CharArray address = inputs[1];
    MetawearWrapper *wrapper = handler->m_connectionHandler->mexGetDeviceAndVerify(engine,address.toAscii());
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
    MetawearWrapper* wrapper =  handler->m_connectionHandler->mexGetDeviceAndVerify(engine,address.toAscii());
    MblMwMetaWearBoard*  board = wrapper->getBoard();

    mbl_mw_sensor_fusion_clear_enabled_mask(board);
    mbl_mw_sensor_fusion_stop(board);

}

void CaptureHandler::mexEnableMagnetometer(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs){
    CaptureHandler* handler = static_cast<CaptureHandler*>(context);
    MexUtility::checkNumberOfParameters(engine,MexUtility::ParameterType::INPUT,inputs.size(),2);
    MexUtility::checkType(engine,MexUtility::ParameterType::INPUT,1,inputs[1].getType(),matlab::data::ArrayType::CHAR);

    matlab::data::CharArray address =  inputs[1];
    MetawearWrapper* wrapper =  handler->m_connectionHandler->mexGetDeviceAndVerify(engine,address.toAscii());
    MblMwMetaWearBoard*  board = wrapper->getBoard();

    mbl_mw_mag_bmm150_enable_b_field_sampling(board);
    mbl_mw_mag_bmm150_start(board);
}

void CaptureHandler::mexDisableMagnetometer(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs){
    CaptureHandler* handler = static_cast<CaptureHandler*>(context);
    MexUtility::checkNumberOfParameters(engine,MexUtility::ParameterType::INPUT,inputs.size(),2);
    MexUtility::checkType(engine,MexUtility::ParameterType::INPUT,1,inputs[1].getType(),matlab::data::ArrayType::CHAR);

    matlab::data::CharArray address =  inputs[1];
    MetawearWrapper* wrapper =  handler->m_connectionHandler->mexGetDeviceAndVerify(engine,address.toAscii());
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
    MetawearWrapper* wrapper =  handler->m_connectionHandler->mexGetDeviceAndVerify(engine,address.toAscii());
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
    MetawearWrapper* wrapper =  handler->m_connectionHandler->mexGetDeviceAndVerify(engine,address.toAscii());
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
    MetawearWrapper* wrapper =  handler->m_connectionHandler->mexGetDeviceAndVerify(engine,address.toAscii());
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
    MetawearWrapper *wrapper = handler->m_connectionHandler->mexGetDeviceAndVerify(engine,address.toAscii());
    MblMwMetaWearBoard *board = wrapper->getBoard();
    mbl_mw_acc_disable_acceleration_sampling(board);
    mbl_mw_acc_stop(board);
}


CaptureHandler::~CaptureHandler() {

}

