//
// Created by michaelpollind on 18/07/18.
//

#ifndef MEX_METAWEAR_METAMOTIONBLUETOOTH_H
#define MEX_METAWEAR_METAMOTIONBLUETOOTH_H


class MetaMotionBluetooth {
    MetaMotionBluetooth();
    ~MetaMotionBluetooth();

    virtual void connect() = 0;
    virtual void disConnect() = 0;
    virtual void commitChanges() = 0;


    

};


#endif //MEX_METAWEAR_METAMOTIONBLUETOOTH_H
