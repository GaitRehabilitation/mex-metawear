//
// Created by michaelpollind on 7/14/18.
//

#ifndef MEX_METAWEAR_METAMOTIONWRAPPER_H
#define MEX_METAWEAR_METAMOTIONWRAPPER_H

class MetaMotionWrapper{

private:
public:

    MetaMotionWrapper();
    virtual ~MetaMotionWrapper();

    virtual void connect() = 0;
    virtual void disConnect()= 0;

    virtual void enableGyro(bool enable) = 0;
    virtual void enableAccelerometer(bool enable) = 0;
    virtual void enableBarometer(bool enable) = 0;
    virtual void enableConductance(bool enable) = 0;
    virtual void enableHumidity(bool enable) = 0;
    virtual void enableSensorFusion(bool enable) = 0;


    virtual void setGyroSamples(int rate) = 0;
    virtual void setAccelerometerSamples(int rate) = 0;

    virtual void commit() = 0;
};


#endif //MEX_METAWEAR_METAMOTIONWRAPPER_H
