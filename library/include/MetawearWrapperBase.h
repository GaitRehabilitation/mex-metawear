//
// Created by Michael on 8/20/2018.
//

#ifndef MEX_METAWEAR_METAWEARWRAPPERBASE_H
#define MEX_METAWEAR_METAWEARWRAPPERBASE_H

#include <string>

class MetawearWrapperBase{
private:
    bool m_isMetawerReady;
public:
    MetawearWrapperBase(std::string address);

    std::string getMacAddress();

    virtual void connect();

    virtual void disconnect();

};

#endif //MEX_METAWEAR_METAWEARWRAPPERBASE_H
