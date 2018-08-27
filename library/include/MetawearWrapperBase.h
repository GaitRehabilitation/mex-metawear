//
// Created by Michael on 8/20/2018.
//

#ifndef MEX_METAWEAR_METAWEARWRAPPERBASE_H
#define MEX_METAWEAR_METAWEARWRAPPERBASE_H

#include <string>
#include <metawear/core/types.h>
#include <metawear/core/cpp/metawearboard_def.h>
#include <metawear/sensor/gyro_bmi160.h>
#include "StreamTypes.h"
#include "MetawearDatastream.h"


class MetawearWrapperBase{
protected:
    std::string m_firmwareVersion;
    std::string m_model;

    volatile bool m_ready;

    std::string m_mac;
    bool m_isMetawerReady;
    MblMwMetaWearBoard *m_metaWearBoard;

    MetawearDataStream<CartesianFloatContainer> m_accelerationStream;
    MetawearDataStream<CartesianFloatContainer> m_gyroStream;
public:


    MetawearWrapperBase(const std::string& mac);
    ~MetawearWrapperBase();

    const std::string& getMacAddress() const;

    void configureMetawear();

    void startGyro();
    void startAccelerometer();

    void stopGyro();
    void stopAccelerometer();

    void configureGyroscope(MblMwGyroBmi160Range range, MblMwGyroBmi160Odr sample);
    void configureAccelerometer(float range, float sample);

    bool isReady();

    virtual void connect() = 0;
    virtual void disconnect() = 0;

     MetawearDataStream<CartesianFloatContainer>* getAccelerationStream();
    MetawearDataStream<CartesianFloatContainer>* getGyroStream();


};

#endif //MEX_METAWEAR_METAWEARWRAPPERBASE_H
