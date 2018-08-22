//
// Created by Michael on 8/20/2018.
//

#ifndef MEX_METAWEAR_METAWEARWRAPPER_H
#define MEX_METAWEAR_METAWEARWRAPPER_H

#include "MetawearWrapperBase.h"
#include <string>

#include <ppltasks.h>
#include <unordered_map>
#include <Windows.Foundation.h>
#include <Windows.Devices.Bluetooth.h>
#include <Windows.Devices.Bluetooth.Advertisement.h>
#include <wrl/wrappers/corewrappers.h>


using namespace concurrency;
using namespace Windows::Devices::Bluetooth;
using namespace Windows::Devices::Bluetooth::Advertisement;
using namespace Windows::Devices::Bluetooth::GenericAttributeProfile;
using namespace Windows::Security::Cryptography;

class MetawearWrapper: public MetawearWrapperBase{
private:
    struct Hasher {
        size_t operator() (Platform::Guid key) const {
            return key.GetHashCode();
        }
    };
    struct EqualFn {
        bool operator() (Platform::Guid t1, Platform::Guid t2) const {
            return t1.Equals(t2);
        }
    };

    std::unordered_map<Platform::Guid, GattDeviceService^, Hasher, EqualFn> m_services;
    std::unordered_map<Platform::Guid, GattCharacteristic^, Hasher, EqualFn> m_characterstics;

    std::string m_mac;
    BluetoothLEDevice^ m_device;

    task_completion_event<void> m_discover_device_event;
    task<void> m_event_set;

    GattCharacteristic^  findCharacterstic( uint64_t low, uint64_t high);

    static void read_gatt_char(void *context, const void *caller, const MblMwGattChar *characteristic,
                               MblMwFnIntVoidPtrArray handler);

    static void write_gatt_char(void *context, const void *caller, MblMwGattCharWriteType writeType,
                                const MblMwGattChar *characteristic, const uint8_t *value, uint8_t length);

    static void enable_char_notify(void *context, const void *caller, const MblMwGattChar *characteristic,
                                   MblMwFnIntVoidPtrArray handler, MblMwFnVoidVoidPtrInt ready);

    static void on_disconnect(void *context, const void *caller, MblMwFnVoidVoidPtrInt handler);

public:
    MetawearWrapper(std::string address);

    void connect();
    void disconnect();

    void cleanup();

};

#endif //MEX_METAWEAR_METAWEARWRAPPER_H
