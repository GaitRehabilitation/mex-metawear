//
// Created by michaelpollind on 7/14/18.
//

#ifndef MEX_METAWEAR_METAMOTIONLINUXWRAPPER_H
#define MEX_METAWEAR_METAMOTIONLINUXWRAPPER_H

#include <metawear/platform/btle_connection.h>
#include <metawear/sensor/sensor_common.h>
#include <functional>
#include <blepp/uuid.h>
#include <bluetooth/bluetooth.h>
#include <blepp/att_pdu.h>

#include "MetaMotionBluetooth.h"

class MblMwMetaWearBoard;
namespace BLEPP{
    class BLEGATTStateMachine;
}
using namespace BLEPP;


class MetaMotionBluetoothLinux : public MetaMotionBluetooth{

private:
    BLEPP::BLEGATTStateMachine* m_gatt;
    MblMwMetaWearBoard *m_metaWearBoard;
    bool m_isConnected;


    static void read_gatt_char(void *context, const void *caller,
                                  const MblMwGattChar *characteristic,
                                  MblMwFnIntVoidPtrArray handler);
    static void write_gatt_char(void *context, const void *caller,
                                   MblMwGattCharWriteType writeType,
                                   const MblMwGattChar *characteristic,
                                   const uint8_t *value, uint8_t length);
    static void enable_char_notify(void *context, const void *caller,
                                      const MblMwGattChar *characteristic,
                                      MblMwFnIntVoidPtrArray handler,
                                      MblMwFnVoidVoidPtrInt ready);
    static void on_disconnect(void *context, const void *caller,
                                 MblMwFnVoidVoidPtrInt handler);

    std::function<void()> m_foundServiceCharacteristics;
    std::function<void(const  PDUNotificationOrIndication&)> m_characteristicNotify;
public:

    MetaMotionBluetoothLinux();
    ~MetaMotionBluetoothLinux();

    void connect();
    void disconnect();

};

#endif //MEX_METAWEAR_METAMOTIONLINUXWRAPPER_H
