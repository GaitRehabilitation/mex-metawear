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
#include <mex.hpp>


using namespace concurrency;
using namespace Windows::Devices::Bluetooth;
using namespace Windows::Devices::Bluetooth::Advertisement;
using namespace Windows::Devices::Bluetooth::GenericAttributeProfile;
using namespace Windows::Security::Cryptography;

static   Microsoft::WRL::Wrappers::RoInitializeWrapper initialize(RO_INIT_MULTITHREADED);

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
    BluetoothLEDevice^ m_device;

    static void read_gatt_char(void *context, const void *caller, const MblMwGattChar *characteristic,
                               MblMwFnIntVoidPtrArray handler);

    static void write_gatt_char(void *context, const void *caller, MblMwGattCharWriteType writeType,
                                const MblMwGattChar *characteristic, const uint8_t *value, uint8_t length);

    static void enable_char_notify(void *context, const void *caller, const MblMwGattChar *characteristic,
                                   MblMwFnIntVoidPtrArray handler, MblMwFnVoidVoidPtrInt ready);

    static void on_disconnect(void *context, const void *caller, MblMwFnVoidVoidPtrInt handler);
    GattCharacteristic^  findCharacterstic( uint64_t low, uint64_t high);
    void cleanup();
    void startDiscovery();

public:
    explicit MetawearWrapper(const std::string& mac);
    ~MetawearWrapper();
    void connect() override;
    void disconnect() override;


};

#endif //MEX_METAWEAR_METAWEARWRAPPER_H
