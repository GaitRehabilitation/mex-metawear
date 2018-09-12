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


#include <MetawearWrapperBase.h>
#include <metawear/core/settings.h>
#include <metawear/sensor/accelerometer.h>
#include <metawear/core/datasignal.h>
#include <metawear/platform/memory.h>
#include <metawear/core/status.h>
#include <metawear/sensor/gyro_bmi160.h>
#include <metawear/sensor/magnetometer_bmm150.h>

#include "MetawearWrapperBase.h"

MetawearWrapperBase::MetawearWrapperBase(const std::string& mac) :
    m_mac(mac),
    m_ready(false),
    m_handlers(){
}

MetawearWrapperBase::~MetawearWrapperBase() {

}

bool MetawearWrapperBase::isReady() {
    return m_ready;
}

bool MetawearWrapperBase::hasHandler(const std::string& key){
    return m_handlers.count(key);
}


bool MetawearWrapperBase::registerHandler(const std::string& key,StreamHandler* handler){
    if(hasHandler(key))
        return false;
    m_handlers.emplace(key,handler);
    return true;
}

bool MetawearWrapperBase::removeHandler(const std::string& key){
    if(hasHandler(key)){
        m_handlers.erase(key);
        return true;
    }
    return false;
}

StreamHandler* MetawearWrapperBase::getHandler(const std::string& key){
    auto  it = m_handlers.find(key);
    if (it != m_handlers.end())
        return it->second;
    return nullptr;
}


void MetawearWrapperBase::configureMetawear() {

    mbl_mw_metawearboard_initialize(m_metaWearBoard, this,
                                    [](void *context, MblMwMetaWearBoard *board, int32_t status) -> void {
                                        auto *wrapper = static_cast<MetawearWrapperBase *>(context);

                                        if (!status) {
                                            std::cout << "Board Initialized";
                                            wrapper->m_ready = true;
                                        } else {
                                            switch (status) {
                                                case MBL_MW_STATUS_OK :
                                                    std::cout << "Error Initializing board: " << "MBL_MW_STATUS_OK \n";
                                                    break;
                                                case MBL_MW_STATUS_WARNING_UNEXPECTED_SENSOR_DATA :
                                                    std::cout << "Error Initializing board: "
                                                              << "MBL_MW_STATUS_WARNING_UNEXPECTED_SENSOR_DATA \n";
                                                    break;
                                                case MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE :
                                                    std::cout << "Error Initializing board: "
                                                              << "MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE \n";
                                                    break;
                                                case MBL_MW_STATUS_ERROR_UNSUPPORTED_PROCESSOR :
                                                    std::cout << "Error Initializing board: "
                                                              << "MBL_MW_STATUS_ERROR_UNSUPPORTED_PROCESSOR \n";
                                                    break;
                                                case MBL_MW_STATUS_WARNING_INVALID_RESPONSE :
                                                    std::cout << "Error Initializing board: "
                                                              << "MBL_MW_STATUS_WARNING_INVALID_RESPONSE \n";
                                                    break;
                                                case MBL_MW_STATUS_ERROR_TIMEOUT :
                                                    std::cout << "Error Initializing board: "
                                                              << "MBL_MW_STATUS_ERROR_TIMEOUT";
                                                    break;
                                                case MBL_MW_STATUS_ERROR_SERIALIZATION_FORMAT :
                                                    std::cout << "Error Initializing board: "
                                                              << "MBL_MW_STATUS_ERROR_SERIALIZATION_FORMAT \n";
                                                    break;
                                                case MBL_MW_STATUS_ERROR_ENABLE_NOTIFY :
                                                    std::cout << "Error Initializing board: "
                                                              << "MBL_MW_STATUS_ERROR_ENABLE_NOTIFY \n";
                                                    break;
                                                default:
                                                    std::cout << "Error Initializing board: " << "Unknown Error \n";
                                                    break;
                                            }
                                        }
                                    });


}


const std::string& MetawearWrapperBase::getMacAddress() const{
    return m_mac;
}


MblMwMetaWearBoard * MetawearWrapperBase::getBoard(){
    return m_metaWearBoard;
}
