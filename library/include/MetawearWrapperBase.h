//
// Created by Michael on 8/20/2018.
//

#ifndef MEX_METAWEAR_METAWEARWRAPPERBASE_H
#define MEX_METAWEAR_METAWEARWRAPPERBASE_H

#include <string>
#include <metawear/core/types.h>
#include <metawear/core/cpp/metawearboard_def.h>
#include "MetawearDatastream.h"

class MetawearWrapperBase{

private:
    typedef struct {
        float x;
        float y;
        float z;
        int64_t epoch;
    } CartesianFloatContainer;

    std::string m_firmwareVersion;
    std::string m_model;

    std::string m_mac;
    bool m_isMetawerReady;

    MetawearDataStream<CartesianFloatContainer*> m_accelerationStream;

public:
    MblMwMetaWearBoard *m_metaWearBoard;
protected:
    MetawearWrapperBase(const std::string& mac, unsigned int size);
    ~MetawearWrapperBase();

    const std::string& getMacAddress() const;

    void configureMetawear();

    virtual void connect();

    virtual void disconnect();

};

#endif //MEX_METAWEAR_METAWEARWRAPPERBASE_H
