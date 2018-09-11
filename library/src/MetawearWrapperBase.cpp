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


#include <MetawearWrapperBase.h>
#include <metawear/core/settings.h>
#include <metawear/sensor/accelerometer.h>
#include <metawear/core/datasignal.h>
#include <metawear/platform/memory.h>
#include <metawear/core/status.h>
#include <metawear/sensor/gyro_bmi160.h>
#include <metawear/sensor/magnetometer_bmm150.h>

#include "MetawearWrapperBase.h"

MetawearWrapperBase::MetawearWrapperBase(const std::string& mac) :
    m_handlers(),
    m_mac(mac),
    m_accelerationStream(1000),
    m_gyroStream(1000),
    m_magnetometer(1000),
    m_ready(false){
}

MetawearWrapperBase::~MetawearWrapperBase() {

}

bool MetawearWrapperBase::isReady() {
    return m_ready;
}


void MetawearWrapperBase::registerHandler(const std::string& key,StreamHandler* handler){

}
void MetawearWrapperBase::removeHandler(const std::string& key){

}
StreamHandler* MetawearWrapperBase::getHandler(const std::string& key){
    return nullptr;
}


void MetawearWrapperBase::configureMetawear() {

    mbl_mw_metawearboard_initialize(m_metaWearBoard, this,
                                    [](void *context, MblMwMetaWearBoard *board, int32_t status) -> void {
                                        auto *wrapper = static_cast<MetawearWrapperBase *>(context);

                                        if (!status) {
                                            std::cout << "Board Initialized";
                                            wrapper->m_ready = true;
                                            // subscribe to acceleration handler
                                            auto acc_signal = mbl_mw_acc_get_packed_acceleration_data_signal(wrapper->m_metaWearBoard);
                                            mbl_mw_datasignal_subscribe(acc_signal, wrapper, [](void *context,
                                                                                                const MblMwData *data) -> void {
                                                auto w = static_cast<MetawearWrapperBase *>(context);
                                                auto acceleration = static_cast<MblMwCartesianFloat *>(data->value);
                                                auto c = CartesianFloatContainer();
                                                c.x = acceleration->x;
                                                c.y = acceleration->y;
                                                c.z = acceleration->z;
                                                c.epoch = data->epoch;
                                                w->m_accelerationStream.push(c);
                                            });
                                            auto gyro_signal = mbl_mw_gyro_bmi160_get_packed_rotation_data_signal( wrapper->m_metaWearBoard);
                                            mbl_mw_datasignal_subscribe(gyro_signal, wrapper, [](void *context,
                                                                                                 const MblMwData *data) -> void {
                                                auto w = static_cast<MetawearWrapperBase *>(context);
                                                auto gyro = static_cast<MblMwCartesianFloat *>(data->value);
                                                auto c = CartesianFloatContainer();
                                                c.x = gyro->x;
                                                c.y = gyro->y;
                                                c.z = gyro->z;
                                                c.epoch = data->epoch;
                                                w->m_gyroStream.push(c);
                                            });

                                            auto bfield_signal = mbl_mw_mag_bmm150_get_b_field_data_signal(wrapper->m_metaWearBoard);
                                            mbl_mw_datasignal_subscribe(bfield_signal,wrapper,[](void *context,
                                                                                                 const MblMwData *data) -> void {
                                                auto w = static_cast<MetawearWrapperBase *>(context);
                                                auto gyro = static_cast<MblMwCartesianFloat *>(data->value);
                                                auto c = CartesianFloatContainer();
                                                c.x = gyro->x;
                                                c.y = gyro->y;
                                                c.z = gyro->z;
                                                c.epoch = data->epoch;
                                                w->m_magnetometer.push(c);
                                            });

                                        } else {
                                            switch (status) {
                                                case MBL_MW_STATUS_OK :
                                                    std::cout << "Error Initializing board: " << "MBL_MW_STATUS_OK \n";
                                                    break;
                                                case MBL_MW_STATUS_WARNING_UNEXPECTED_SENSOR_DATA :
                                                    std::cout << "Error Initializing board: "
                                                              << "MBL_MW_STATUS_WARNING_UNEXPECTED_SENSOR_DATA \n";
                                                    break;
                                                case MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE :
                                                    std::cout << "Error Initializing board: "
                                                              << "MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE \n";
                                                    break;
                                                case MBL_MW_STATUS_ERROR_UNSUPPORTED_PROCESSOR :
                                                    std::cout << "Error Initializing board: "
                                                              << "MBL_MW_STATUS_ERROR_UNSUPPORTED_PROCESSOR \n";
                                                    break;
                                                case MBL_MW_STATUS_WARNING_INVALID_RESPONSE :
                                                    std::cout << "Error Initializing board: "
                                                              << "MBL_MW_STATUS_WARNING_INVALID_RESPONSE \n";
                                                    break;
                                                case MBL_MW_STATUS_ERROR_TIMEOUT :
                                                    std::cout << "Error Initializing board: "
                                                              << "MBL_MW_STATUS_ERROR_TIMEOUT";
                                                    break;
                                                case MBL_MW_STATUS_ERROR_SERIALIZATION_FORMAT :
                                                    std::cout << "Error Initializing board: "
                                                              << "MBL_MW_STATUS_ERROR_SERIALIZATION_FORMAT \n";
                                                    break;
                                                case MBL_MW_STATUS_ERROR_ENABLE_NOTIFY :
                                                    std::cout << "Error Initializing board: "
                                                              << "MBL_MW_STATUS_ERROR_ENABLE_NOTIFY \n";
                                                    break;
                                                default:
                                                    std::cout << "Error Initializing board: " << "Unknown Error \n";
                                                    break;
                                            }
                                        }
                                    });


}

MetawearDataStream<CartesianFloatContainer >* MetawearWrapperBase::getAccelerationStream(){
    return &m_accelerationStream;
}

MetawearDataStream<CartesianFloatContainer>* MetawearWrapperBase::getGyroStream(){
    return &m_gyroStream;
}

const std::string& MetawearWrapperBase::getMacAddress() const{
    return m_mac;
}


MblMwMetaWearBoard * MetawearWrapperBase::getBoard(){
    return m_metaWearBoard;
}
