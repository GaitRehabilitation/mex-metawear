//
// Created by Michael on 8/22/2018.
//

#include <MetawearWrapperBase.h>
#include <metawear/core/settings.h>
#include <metawear/sensor/accelerometer.h>
#include <metawear/core/datasignal.h>
#include <metawear/platform/memory.h>
#include <metawear/core/status.h>

#include "MetawearWrapperBase.h"

MetawearWrapperBase::MetawearWrapperBase(const std::string& mac, unsigned int size) :
    m_mac(mac), m_accelerationStream(size){
}

MetawearWrapperBase::~MetawearWrapperBase() {
    auto stream_acc = m_accelerationStream.dump();
    for(auto i: stream_acc){
        delete i;
    }
}

void MetawearWrapperBase::configureMetawear() {
    mbl_mw_metawearboard_initialize(m_metaWearBoard, this,
                                    [](void *context, MblMwMetaWearBoard *board, int32_t status) -> void {
                                        auto *wrapper = static_cast<MetawearWrapperBase *>(context);

                                        if (!status) {
                                            auto dev_info = mbl_mw_metawearboard_get_device_information(
                                                    wrapper->m_metaWearBoard);
                                            std::cout << "firmware = " << std::string(dev_info->firmware_revision);
                                            wrapper->m_firmwareVersion = dev_info->firmware_revision;
                                            wrapper->m_model = dev_info->model_number;
                                            mbl_mw_memory_free((void *) dev_info);
                                            std::cout << "model = " << wrapper->m_model;
                                            std::cout << "Board Initialized";
                                        } else {
                                            switch (status) {
                                                case MBL_MW_STATUS_OK :
                                                    std::cout << "Error Initializing board: " << "MBL_MW_STATUS_OK";
                                                    break;
                                                case MBL_MW_STATUS_WARNING_UNEXPECTED_SENSOR_DATA :
                                                    std::cout << "Error Initializing board: "
                                                              << "MBL_MW_STATUS_WARNING_UNEXPECTED_SENSOR_DATA";
                                                    break;
                                                case MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE :
                                                    std::cout << "Error Initializing board: "
                                                              << "MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE";
                                                    break;
                                                case MBL_MW_STATUS_ERROR_UNSUPPORTED_PROCESSOR :
                                                    std::cout << "Error Initializing board: "
                                                              << "MBL_MW_STATUS_ERROR_UNSUPPORTED_PROCESSOR";
                                                    break;
                                                case MBL_MW_STATUS_WARNING_INVALID_RESPONSE :
                                                    std::cout << "Error Initializing board: "
                                                              << "MBL_MW_STATUS_WARNING_INVALID_RESPONSE";
                                                    break;
                                                case MBL_MW_STATUS_ERROR_TIMEOUT :
                                                    std::cout << "Error Initializing board: "
                                                              << "MBL_MW_STATUS_ERROR_TIMEOUT";
                                                    break;
                                                case MBL_MW_STATUS_ERROR_SERIALIZATION_FORMAT :
                                                    std::cout << "Error Initializing board: "
                                                              << "MBL_MW_STATUS_ERROR_SERIALIZATION_FORMAT";
                                                    break;
                                                case MBL_MW_STATUS_ERROR_ENABLE_NOTIFY :
                                                    std::cout << "Error Initializing board: "
                                                              << "MBL_MW_STATUS_ERROR_ENABLE_NOTIFY";
                                                    break;
                                                default:
                                                    std::cout << "Error Initializing board: " << "Unknown Error";
                                                    break;
                                            }
                                        }
                                    });


//    auto battery_signal = mbl_mw_settings_get_battery_state_data_signal(m_metaWearBoard);
//    mbl_mw_datasignal_subscribe(battery_signal, this, [](void *context, const MblMwData *data) -> void {
//
//    });

    // subscribe to acceleration handler
    auto acc_signal = mbl_mw_acc_get_packed_acceleration_data_signal(m_metaWearBoard);
    mbl_mw_datasignal_subscribe(acc_signal, this, [](void *context, const MblMwData *data) -> void {
        auto wrapper = static_cast<MetawearWrapperBase *>(context);
        auto acceleration = static_cast<MblMwCartesianFloat *>(data->value);
        auto c = new CartesianFloatContainer();
        c->x = acceleration->x;
        c->y = acceleration->y;
        c->z = acceleration->z;
        c->epoch = data->epoch;
        wrapper->m_accelerationStream.push(c);
    });
}

const std::string& MetawearWrapperBase::getMacAddress() const{
    return m_mac;
}
