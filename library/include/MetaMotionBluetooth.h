//
// Created by michaelpollind on 18/07/18.
//

#ifndef MEX_METAWEAR_METAMOTIONBLUETOOTH_H
#define MEX_METAWEAR_METAMOTIONBLUETOOTH_H

class MblMwMetaWearBoard;

class MetaMotionBluetooth {
public:
    MetaMotionBluetooth() {};

    virtual ~MetaMotionBluetooth() = 0;

    virtual void connect() = 0;

    virtual void disconnect() = 0;

    MblMwMetaWearBoard* getMetawareBoard() = 0;
};


#endif //MEX_METAWEAR_METAMOTIONBLUETOOTH_H
