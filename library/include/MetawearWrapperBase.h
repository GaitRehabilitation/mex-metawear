/**
* Copyright 2018 GaitRehabilitation
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#ifndef MEX_METAWEAR_METAWEARWRAPPERBASE_H
#define MEX_METAWEAR_METAWEARWRAPPERBASE_H

#include <string>
#include <metawear/core/types.h>
#include <metawear/core/cpp/metawearboard_def.h>
#include <metawear/sensor/gyro_bmi160.h>
#include "StreamTypes.h"
#include "StreamHandler.h"
#include <map>
class MetawearWrapperBase{
protected:
    std::string m_firmwareVersion;
    std::string m_model;

    volatile bool m_ready;

    std::string m_mac;
    bool m_isMetawerReady;
    MblMwMetaWearBoard *m_metaWearBoard;

    std::map<std::string ,StreamHandler*> m_handlers;


public:
    bool hasHandler(const std::string&);
    bool registerHandler(const std::string&,StreamHandler*);
    bool removeHandler(const std::string&);
    StreamHandler* getHandler(const std::string&);

    MetawearWrapperBase(const std::string& mac);
    ~MetawearWrapperBase();

    const std::string& getMacAddress() const;

    void configureMetawear();
    bool isReady();

    virtual void connect() = 0;
    virtual void disconnect() = 0;

    MblMwMetaWearBoard * getBoard();

};

#endif //MEX_METAWEAR_METAWEARWRAPPERBASE_H
