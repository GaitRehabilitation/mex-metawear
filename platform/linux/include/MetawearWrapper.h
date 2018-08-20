//
// Created by Michael on 8/20/2018.
//

#ifndef MEX_METAWEAR_METAWEARWRAPPER_H
#define MEX_METAWEAR_METAWEARWRAPPER_H

#include "MetawearWrapperBase.h"

class MetawearWrapper: public MetawearWrapperBase{

    void connect();
    void disconnect();

};

#endif //MEX_METAWEAR_METAWEARWRAPPER_H
