//
// Created by michaelpollind on 7/14/18.
//
#include "include/MetaMotionLinuxWrapper.h"

#include "metawear/core/data.h"
#include "metawear/core/datasignal.h"
#include "metawear/core/metawearboard.h"
#include "metawear/core/settings.h"
#include "metawear/core/status.h"
#include "metawear/core/types.h"
#include "metawear/platform/memory.h"
#include "metawear/sensor/ambientlight_ltr329.h"
#include "metawear/sensor/barometer_bosch.h"
#include "metawear/sensor/cpp/utils.h"
#include "metawear/sensor/gyro_bmi160.h"
#include "metawear/sensor/magnetometer_bmm150.h"


#include <blepp/blestatemachine.h>
#include <metawear/platform/btle_connection.h>
#include <blepp/uuid.h>
#include <bluetooth/bluetooth.h>


void convertToUUID(BLEPP::bt_uuid_t& result, uint8_t *low, uint8_t *high) {
    result.type = BLEPP::bt_uuid_type_t::BT_UUID128;
    for (int i = 0; i < 8; i++) {
        result.value.u128.data[i] = high[7 - i];
    }
    for (int i = 0; i < 8; i++) {
        result.value.u128.data[i + 8] = low[7 - i];
    }
}

void MetaMotionLinuxWrapper::read_gatt_char_qt(void *context, const void *caller, const MblMwGattChar *characteristic, MblMwFnIntVoidPtrArray handler) {
    MetaMotionLinuxWrapper* wrapper = static_cast<MetaMotionLinuxWrapper *>(context);

    BLEPP::bt_uuid_t temp;
    convertToUUID(temp,(uint8_t *)characteristic->service_uuid_low,(uint8_t *)characteristic->service_uuid_high);
    BLEPP::UUID serviceUuid =  BLEPP::UUID::from(temp);

    convertToUUID(temp,(uint8_t *)characteristic->service_uuid_low,(uint8_t *)characteristic->service_uuid_high);
    BLEPP::UUID characteristicUuid =  BLEPP::UUID::from(temp);


    for(auto& serviceHandler: wrapper->m_gatt->primary_services) {
        if(serviceHandler.uuid == serviceUuid) {
            for (auto &characteristicHandler: serviceHandler.characteristics) {
                if(characteristicHandler.uuid == characteristicUuid){
                    characteristicHandler.read_request();
                }
            }
        }
    }
}


void MetaMotionLinuxWrapper::write_gatt_char_qt(void *context, const void *caller, MblMwGattCharWriteType writeType, const MblMwGattChar *characteristic, const uint8_t *value, uint8_t length) {
    MetaMotionLinuxWrapper* wrapper = static_cast<MetaMotionLinuxWrapper *>(context);

    BLEPP::bt_uuid_t temp;
    convertToUUID(temp,(uint8_t *)characteristic->service_uuid_low,(uint8_t *)characteristic->service_uuid_high);
    BLEPP::UUID serviceUuid =  BLEPP::UUID::from(temp);

    convertToUUID(temp,(uint8_t *)characteristic->service_uuid_low,(uint8_t *)characteristic->service_uuid_high);
    BLEPP::UUID characteristicUuid =  BLEPP::UUID::from(temp);

    for(auto& serviceHandler: wrapper->m_gatt->primary_services) {
        if(serviceHandler.uuid == serviceUuid) {
            for (auto &characteristicHandler: serviceHandler.characteristics) {
                if(characteristicHandler.uuid == characteristicUuid){
                    characteristicHandler.write_request(value,length);
                }
            }
        }
    }

}

void MetaMotionLinuxWrapper::enable_char_notify_qt(void *context, const void *caller,const MblMwGattChar *characteristic,MblMwFnIntVoidPtrArray handler,MblMwFnVoidVoidPtrInt ready) {
    MetaMotionLinuxWrapper* wrapper = static_cast<MetaMotionLinuxWrapper *>(context);

    BLEPP::bt_uuid_t temp;
    convertToUUID(temp,(uint8_t *)characteristic->service_uuid_low,(uint8_t *)characteristic->service_uuid_high);
    BLEPP::UUID serviceUuid =  BLEPP::UUID::from(temp);

    convertToUUID(temp,(uint8_t *)characteristic->service_uuid_low,(uint8_t *)characteristic->service_uuid_high);
    BLEPP::UUID characteristicUuid =  BLEPP::UUID::from(temp);



    for(auto& serviceHandler: wrapper->m_gatt->primary_services) {
        if(serviceHandler.uuid == serviceUuid) {
            for (auto &characteristicHandler: serviceHandler.characteristics) {
                if(characteristicHandler.uuid == characteristicUuid){
                    characteristicHandler.set_notify_and_indicate(true, false,BLEPP::WriteType::Request);
                }
            }
        }
    }
}


void MetaMotionLinuxWrapper::on_disconnect_qt(void *context, const void *caller, MblMwFnVoidVoidPtrInt handler) {
    MetaMotionLinuxWrapper* wrapper = static_cast<MetaMotionLinuxWrapper *>(context);

}

MetaMotionLinuxWrapper::MetaMotionLinuxWrapper() {
    m_gatt = new BLEPP::BLEGATTStateMachine();
    m_gatt->cb_disconnected = [=](BLEPP::BLEGATTStateMachine::Disconnect d){
        mbl_mw_metawearboard_tear_down(this->m_metaWearBoard);
        delete m_gatt;
    };

    MblMwBtleConnection btleConnection;
    btleConnection.context = this;
    btleConnection.write_gatt_char = write_gatt_char_qt;
    btleConnection.read_gatt_char = read_gatt_char_qt;
    btleConnection.enable_notifications = enable_char_notify_qt;
    btleConnection.on_disconnect = on_disconnect_qt;
    this->m_metaWearBoard = mbl_mw_metawearboard_create(&btleConnection);


}

MetaMotionLinuxWrapper::~MetaMotionLinuxWrapper() {


}