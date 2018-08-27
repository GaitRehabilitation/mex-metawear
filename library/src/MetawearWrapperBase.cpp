//
// Created by Michael on 8/22/2018.
//

#include <MetawearWrapperBase.h>
#include <metawear/core/settings.h>
#include <metawear/sensor/accelerometer.h>
#include <metawear/core/datasignal.h>
#include <metawear/platform/memory.h>
#include <metawear/core/status.h>
#include <metawear/sensor/gyro_bmi160.h>

#include "MetawearWrapperBase.h"

MetawearWrapperBase::MetawearWrapperBase(const std::string& mac) :
    m_mac(mac), m_accelerationStream(1000),m_gyroStream(1000),m_ready(false){
}

MetawearWrapperBase::~MetawearWrapperBase() {

}

bool MetawearWrapperBase::isReady() {
    return m_ready;
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
                                            auto gyro_signal = mbl_mw_gyro_bmi160_get_packed_rotation_data_signal(
                                                    wrapper->m_metaWearBoard);
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

void MetawearWrapperBase::startGyro(){
    mbl_mw_gyro_bmi160_enable_rotation_sampling(m_metaWearBoard);
    mbl_mw_gyro_bmi160_start(m_metaWearBoard);
}

void MetawearWrapperBase::stopGyro(){
    mbl_mw_gyro_bmi160_disable_rotation_sampling(m_metaWearBoard);
    mbl_mw_gyro_bmi160_stop(m_metaWearBoard);
}

void MetawearWrapperBase::startAccelerometer(){
    mbl_mw_acc_enable_acceleration_sampling(m_metaWearBoard);
    mbl_mw_acc_start(m_metaWearBoard);
}

void MetawearWrapperBase::stopAccelerometer(){
    mbl_mw_acc_disable_acceleration_sampling(m_metaWearBoard);
    mbl_mw_acc_stop(m_metaWearBoard);
}


void MetawearWrapperBase::configureGyroscope(MblMwGyroBmi160Range range, MblMwGyroBmi160Odr sample){
    mbl_mw_gyro_bmi160_set_odr(m_metaWearBoard,sample);
    mbl_mw_gyro_bmi160_set_range(m_metaWearBoard,range);
    mbl_mw_gyro_bmi160_write_config(m_metaWearBoard);
}

void MetawearWrapperBase::configureAccelerometer(float range, float sample){
    mbl_mw_acc_set_odr(m_metaWearBoard,sample);
    mbl_mw_acc_set_range(m_metaWearBoard,range);
    mbl_mw_acc_write_acceleration_config(m_metaWearBoard);
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
