//
// Created by michaelpollind on 7/14/18.
//
#include "include/MetaMotionBluetoothLinux.h"

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

void MetaMotionBluetoothLinux::read_gatt_char(void *context, const void *caller, const MblMwGattChar *characteristic, MblMwFnIntVoidPtrArray handler) {
    MetaMotionBluetoothLinux* wrapper = static_cast<MetaMotionBluetoothLinux *>(context);


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
                    characteristicHandler.cb_read = [=](const PDUReadResponse& response){
                        handler(wrapper, response.data, static_cast<uint8_t>(response.length));

                    };
                }
            }
        }
    }
}


void MetaMotionBluetoothLinux::write_gatt_char(void *context, const void *caller, MblMwGattCharWriteType writeType, const MblMwGattChar *characteristic, const uint8_t *value, uint8_t length) {
    MetaMotionBluetoothLinux* wrapper = static_cast<MetaMotionBluetoothLinux *>(context);

    BLEPP::bt_uuid_t temp;
    convertToUUID(temp,(uint8_t *)characteristic->service_uuid_low,(uint8_t *)characteristic->service_uuid_high);
    BLEPP::UUID serviceUuid =  BLEPP::UUID::from(temp);

    convertToUUID(temp,(uint8_t *)characteristic->service_uuid_low,(uint8_t *)characteristic->service_uuid_high);
    BLEPP::UUID characteristicUuid =  BLEPP::UUID::from(temp);

    for(auto& serviceHandler: wrapper->m_gatt->primary_services) {
        if(serviceHandler.uuid == serviceUuid) {
            for (auto &characteristicHandler: serviceHandler.characteristics) {
                if(characteristicHandler.uuid == characteristicUuid){
                    characteristicHandler.write_without_response = (writeType == MblMwGattCharWriteType::MBL_MW_GATT_CHAR_WRITE_WITHOUT_RESPONSE);
                    characteristicHandler.write_request(value,length);
                }
            }
        }
    }

}

void MetaMotionBluetoothLinux::enable_char_notify(void *context, const void *caller,const MblMwGattChar *characteristic,MblMwFnIntVoidPtrArray handler,MblMwFnVoidVoidPtrInt ready) {
    MetaMotionBluetoothLinux* wrapper = static_cast<MetaMotionBluetoothLinux *>(context);

    BLEPP::bt_uuid_t temp;
    convertToUUID(temp,(uint8_t *)characteristic->service_uuid_low,(uint8_t *)characteristic->service_uuid_high);
    BLEPP::UUID serviceUuid =  BLEPP::UUID::from(temp);

    convertToUUID(temp,(uint8_t *)characteristic->service_uuid_low,(uint8_t *)characteristic->service_uuid_high);
    BLEPP::UUID characteristicUuid =  BLEPP::UUID::from(temp);

    for(auto& serviceHandler: wrapper->m_gatt->primary_services) {
        if(serviceHandler.uuid == serviceUuid) {
            for (auto &characteristicHandler: serviceHandler.characteristics) {
                if(characteristicHandler.uuid == characteristicUuid){
                    characteristicHandler.set_notify_and_indicate(true, false);
                    characteristicHandler.cb_notify_or_indicate = [=](const  PDUNotificationOrIndication& notificationOrIndication){
                        if(notificationOrIndication.notification()) {
                            ready(caller, MBL_MW_STATUS_OK);
                        }
                        else{
                            ready(caller, MBL_MW_STATUS_ERROR_ENABLE_NOTIFY);
                        }

                    };
                }
            }
        }
    }
}


void MetaMotionBluetoothLinux::on_disconnect(void *context, const void *caller, MblMwFnVoidVoidPtrInt handler) {
    MetaMotionBluetoothLinux* wrapper = static_cast<MetaMotionBluetoothLinux *>(context);
    wrapper->m_gatt->cb_disconnected = [=](BLEPP::BLEGATTStateMachine::Disconnect disconnect){
        handler(wrapper,0);
    };
}


MetaMotionBluetoothLinux::MetaMotionBluetoothLinux(): m_isConnected(0) {
    m_gatt = new BLEPP::BLEGATTStateMachine();
    m_gatt->cb_disconnected = [=](BLEPP::BLEGATTStateMachine::Disconnect d){
        mbl_mw_metawearboard_tear_down(this->m_metaWearBoard);
        delete m_gatt;
    };

    MblMwBtleConnection btleConnection;
    btleConnection.context = this;
    btleConnection.write_gatt_char = write_gatt_char;
    btleConnection.read_gatt_char = read_gatt_char;
    btleConnection.enable_notifications = enable_char_notify;
    btleConnection.on_disconnect = on_disconnect;
    this->m_metaWearBoard = mbl_mw_metawearboard_create(&btleConnection);

}

void MetaMotionBluetoothLinux::connect() {
    m_foundServiceCharacteristics = [=](){
        BLEPP::pretty_print_tree(*this->m_gatt);

        for(auto& service: this->m_gatt->primary_services) {
            for (auto &characteristic: service.characteristics) {
                characteristic.cb_notify_or_indicate = m_characteristicNotify;
            }
        }
    };
    m_gatt->setup_standard_scan(m_foundServiceCharacteristics);

}

void MetaMotionBluetoothLinux::disConnect() {
    m_gatt->close();
}

void MetaMotionBluetoothLinux::commitChanges() {
    
}

MetaMotionBluetoothLinux::~MetaMotionBluetoothLinux() {


}