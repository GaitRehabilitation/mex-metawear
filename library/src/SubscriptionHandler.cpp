#include <utility>

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

#include "handlers/SubscriptionHandler.h"
#include <MexUtility.h>
#include <StreamHandler.h>
#include <metawear/sensor/sensor_fusion.h>
#include <handlers/CaptureHandler.h>
#include <MetawearWrapper.h>
#include <metawear/sensor/accelerometer.h>
#include "handlers/ConnectionHandler.h"

SubscriptionHandler::SubscriptionHandler(ConnectionHandler *connectionHandler, FunctionWrapper *wrapper) : m_connectionHandler(connectionHandler) {
    std::map<std::string, WrapperMethod *> functions = {
            // metawear('subscribe_acc',boolean: log or not to log, {filters})
            {"subscribe_acc",    mexSubscribeAcc},
            {"subscribe_gyro",   mexSubscribeGyro},
            {"subscribe_fusion", mexSubscribeFusion},

            {"unsubscribe",      mexUnSubscribe}
    };
    wrapper->registerMethod(this, functions);
}

SubscriptionHandler::~SubscriptionHandler() {

}


void SubscriptionHandler::mexSubscribeGyro(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs) {
    SubscriptionHandler *handler = static_cast<SubscriptionHandler*>(context);

    MexUtility::checkNumberOfParameters(engine, MexUtility::ParameterType::INPUT, inputs.size(), 3);
    MexUtility::checkNumberOfParameters(engine, MexUtility::ParameterType::OUTPUT, outputs.size(), 1);
    MexUtility::checkType(engine, MexUtility::ParameterType::INPUT, 1, inputs[1].getType(),matlab::data::ArrayType::CHAR);
    MexUtility::checkType(engine, MexUtility::ParameterType::INPUT, 2, inputs[2].getType(), matlab::data::ArrayType::CHAR);

    matlab::data::CharArray address = inputs[1];
    matlab::data::CharArray type = inputs[2];

    MetawearWrapper *wrapper = handler->m_connectionHandler->mexGetDeviceAndVerify(engine,address.toAscii());
    MblMwMetaWearBoard *board = wrapper->getBoard();

    matlab::data::ArrayFactory factory;
    switch (getSubscriptionMode(engine,type.toAscii())) {
        case StreamType::STREAMING: {
            auto gyro_signal = mbl_mw_gyro_bmi160_get_packed_rotation_data_signal(board);
            std::string result = wrapper->registerHandler(new StreamHandler(gyro_signal, StreamType::STREAMING, "gyro"));
            if(result.empty())
                MexUtility::error(engine,"Failed to Subscribe to Handler: gyro_streaming");
            outputs[0] = factory.createCharArray(result);
        }
        break;
        case StreamType::LOG: {
            auto gyro_signal = mbl_mw_gyro_bmi160_get_rotation_data_signal(board);
            std::string result = wrapper->registerHandler(new StreamHandler(gyro_signal, StreamType::LOG, "gyro"));
            if(result.empty())
                MexUtility::error(engine,"Failed to Subscribe to Handler: gyro_logging");
            outputs[0] = factory.createCharArray(result);
        }
        break;
    }
}

void SubscriptionHandler::mexSubscribeAcc(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs){
    SubscriptionHandler *handler = static_cast<SubscriptionHandler*>(context);

    MexUtility::checkNumberOfParameters(engine, MexUtility::ParameterType::INPUT, inputs.size(), 3);
    MexUtility::checkNumberOfParameters(engine, MexUtility::ParameterType::OUTPUT, outputs.size(), 1);
    MexUtility::checkType(engine, MexUtility::ParameterType::INPUT, 1, inputs[1].getType(),matlab::data::ArrayType::CHAR);
    MexUtility::checkType(engine, MexUtility::ParameterType::INPUT, 2, inputs[2].getType(), matlab::data::ArrayType::CHAR);

    matlab::data::CharArray address = inputs[1];
    matlab::data::CharArray type = inputs[2];

    MetawearWrapper *wrapper = handler->m_connectionHandler->mexGetDeviceAndVerify(engine,address.toAscii());
    MblMwMetaWearBoard *board = wrapper->getBoard();

    matlab::data::ArrayFactory factory;
    switch (getSubscriptionMode(engine,type.toAscii())) {
        case StreamType::STREAMING: {
            auto acc_signal = mbl_mw_acc_get_packed_acceleration_data_signal(board);
            std::string result = wrapper->registerHandler(new StreamHandler(acc_signal, StreamType::STREAMING, "acc"));
            if(result.empty())
                MexUtility::error(engine,"Failed to Subscribe to Handler: acc_streaming");
            outputs[0] = factory.createCharArray(result);
        }
        break;
        case StreamType::LOG: {
            auto acc_signal = mbl_mw_acc_get_acceleration_data_signal(board);
            std::string result = wrapper->registerHandler(new StreamHandler(acc_signal, StreamType::LOG, "acc"));
            if(result.empty())
                MexUtility::error(engine,"Failed to Subscribe to Handler: acc_logging");
            outputs[0] = factory.createCharArray(result);
        }
        break;
    }
}

void SubscriptionHandler::mexSubscribeFusion(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs) {
    SubscriptionHandler *handler = static_cast<SubscriptionHandler *>(context);

    MexUtility::checkNumberOfParameters(engine, MexUtility::ParameterType::INPUT, inputs.size(), 4);
    MexUtility::checkNumberOfParameters(engine, MexUtility::ParameterType::OUTPUT, outputs.size(), 1);
    MexUtility::checkType(engine, MexUtility::ParameterType::INPUT, 1, inputs[1].getType(), matlab::data::ArrayType::CHAR);
    MexUtility::checkType(engine, MexUtility::ParameterType::INPUT, 2, inputs[2].getType(), matlab::data::ArrayType::CHAR);
    MexUtility::checkType(engine, MexUtility::ParameterType::INPUT, 3, inputs[3].getType(), matlab::data::ArrayType::CHAR);

    matlab::data::CharArray address = inputs[1];
    matlab::data::CharArray mode = inputs[2];
    matlab::data::CharArray type = inputs[3];

    MetawearWrapper *wrapper = handler->m_connectionHandler->mexGetDeviceAndVerify(engine,address.toAscii());
    MblMwMetaWearBoard *board = wrapper->getBoard();

    matlab::data::ArrayFactory factory;
    if (type.toAscii() == "CORRECTED_ACC") {
        auto corrected_acc = mbl_mw_sensor_fusion_get_data_signal(board, MBL_MW_SENSOR_FUSION_DATA_CORRECTED_ACC);
        std::string result = wrapper->registerHandler(new StreamHandler(corrected_acc,getSubscriptionMode(engine,mode.toAscii()),"sensor_fuson_data_corrected_acc"));
        if(result.empty())
            MexUtility::error(engine,"Failed to Subscribe to Handler: CORRECTED_ACC");
        outputs[0] = factory.createCharArray(result);
    } else if (type.toAscii() == "CORRECTED_GYRO") {
        auto corrected_gyro = mbl_mw_sensor_fusion_get_data_signal(board, MBL_MW_SENSOR_FUSION_DATA_CORRECTED_GYRO);
        std::string result = wrapper->registerHandler(new StreamHandler(corrected_gyro,getSubscriptionMode(engine,mode.toAscii()),"fusion_corrected_gyro"));
        if(result.empty())
            MexUtility::error(engine,"Failed to Subscribe to Handler: CORRECTED_GYRO");
        outputs[0] = factory.createCharArray(result);
    } else if (type.toAscii() == "CORRECTED_MAG") {
        auto corrected_mag = mbl_mw_sensor_fusion_get_data_signal(board, MBL_MW_SENSOR_FUSION_DATA_CORRECTED_MAG);
        std::string result = wrapper->registerHandler(new StreamHandler(corrected_mag,getSubscriptionMode(engine,mode.toAscii()),"fusion_corrected_mag"));
        if(result.empty())
            MexUtility::error(engine,"Failed to Subscribe to Handler: CORRECTED_MAG");
        outputs[0] = factory.createCharArray(result);
    } else if (type.toAscii() == "QUATERNION") {
        auto quaternion_signal = mbl_mw_sensor_fusion_get_data_signal(board, MBL_MW_SENSOR_FUSION_DATA_QUATERNION);
        std::string result = wrapper->registerHandler(new StreamHandler(quaternion_signal,getSubscriptionMode(engine,mode.toAscii()),"fusion_quaternion"));
        if(result.empty())
            MexUtility::error(engine,"Failed to Subscribe to Handler: EULER_ANGLE");
        outputs[0] = factory.createCharArray(result);
    } else if (type.toAscii() == "EULER_ANGLE") {
        auto euler_angle = mbl_mw_sensor_fusion_get_data_signal(board, MBL_MW_SENSOR_FUSION_DATA_EULER_ANGLE);
        std::string result = wrapper->registerHandler(new StreamHandler(euler_angle,getSubscriptionMode(engine,mode.toAscii()),"fusion_euler_angle"));
        if(result.empty())
            MexUtility::error(engine,"Failed to Subscribe to Handler: EULER_ANGLE");
        outputs[0] = factory.createCharArray(result);
    } else if (type.toAscii() == "GRAVITY_VECTOR") {
        auto gravity_signal = mbl_mw_sensor_fusion_get_data_signal(board, MBL_MW_SENSOR_FUSION_DATA_GRAVITY_VECTOR);
        std::string result = wrapper->registerHandler(new StreamHandler(gravity_signal,getSubscriptionMode(engine,mode.toAscii()),"fusion_gravity"));
        if(result.empty())
            MexUtility::error(engine,"Failed to Subscribe to Handler: GRAVITY_VECTOR");
        outputs[0] = factory.createCharArray(result);
    } else if (type.toAscii() == "LINEAR_ACC") {
        auto linear_acceleration_signal = mbl_mw_sensor_fusion_get_data_signal(board, MBL_MW_SENSOR_FUSION_DATA_LINEAR_ACC);
        std::string result = wrapper->registerHandler(new StreamHandler(linear_acceleration_signal,getSubscriptionMode(engine,mode.toAscii()),"fusion_linear_acc"));
        if(result.empty())
            MexUtility::error(engine,"Failed to Configure: LINEAR_ACC");
        outputs[0] = factory.createCharArray(result);
    } else {
        MexUtility::error(engine, "Fusion Supports Parameter: CORRECTED_ACC, CORRECTED_GYRO, QUATERNION, EULER_ANGLE, GRAVITY_VECTOR, LINEAR_ACC");
    }
}

void SubscriptionHandler::mexUnSubscribe(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs) {
    SubscriptionHandler *handler = static_cast<SubscriptionHandler *>(context);

    MexUtility::checkNumberOfParameters(engine, MexUtility::ParameterType::INPUT, inputs.size(), 3);
    MexUtility::checkNumberOfParameters(engine, MexUtility::ParameterType::OUTPUT, outputs.size(), 1);
    MexUtility::checkType(engine, MexUtility::ParameterType::INPUT, 1, inputs[1].getType(),matlab::data::ArrayType::CHAR);
    MexUtility::checkType(engine, MexUtility::ParameterType::INPUT, 2, inputs[2].getType(),matlab::data::ArrayType::CHAR);

    matlab::data::CharArray address = inputs[1];
    matlab::data::CharArray key = inputs[2];
    MetawearWrapper *wrapper = handler->m_connectionHandler->mexGetDeviceAndVerify(engine,address.toAscii());
    wrapper->removeHandler(key.toAscii());
}

StreamType SubscriptionHandler::getSubscriptionMode(std::shared_ptr<matlab::engine::MATLABEngine> engine,const std::string &target) {
    if (target == "STREAMING") {
        return StreamType::STREAMING;
    } else if (target == "LOG") {
        return StreamType::LOG;
    }
    MexUtility::error(std::move(engine), "Supported Types: STREAMING, LOG");
}

