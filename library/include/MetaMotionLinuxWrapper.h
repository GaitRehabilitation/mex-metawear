//
// Created by michaelpollind on 7/14/18.
//

#ifndef MEX_METAWEAR_METAMOTIONLINUXWRAPPER_H
#define MEX_METAWEAR_METAMOTIONLINUXWRAPPER_H

#include <metawear/platform/btle_connection.h>
#include <metawear/sensor/sensor_common.h>

class MblMwMetaWearBoard;
namespace BLEPP{
    class BLEGATTStateMachine;
}
class MetaMotionLinuxWrapper{

private:
    BLEPP::BLEGATTStateMachine* m_gatt;
    MblMwMetaWearBoard *m_metaWearBoard;

    static void read_gatt_char_qt(void *context, const void *caller,
                                  const MblMwGattChar *characteristic,
                                  MblMwFnIntVoidPtrArray handler);
    static void write_gatt_char_qt(void *context, const void *caller,
                                   MblMwGattCharWriteType writeType,
                                   const MblMwGattChar *characteristic,
                                   const uint8_t *value, uint8_t length);
    static void enable_char_notify_qt(void *context, const void *caller,
                                      const MblMwGattChar *characteristic,
                                      MblMwFnIntVoidPtrArray handler,
                                      MblMwFnVoidVoidPtrInt ready);
    static void on_disconnect_qt(void *context, const void *caller,
                                 MblMwFnVoidVoidPtrInt handler);

public:

    MetaMotionLinuxWrapper();
    ~MetaMotionLinuxWrapper();


};

#endif //MEX_METAWEAR_METAMOTIONLINUXWRAPPER_H
