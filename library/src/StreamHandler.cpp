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

#include <StreamHandler.h>
#include <metawear/core/logging.h>
#include <metawear/core/anonymous_datasignal.h>

#include "StreamHandler.h"


void StreamHandler::configure() {
    switch (m_stream_type){
        case StreamType::LOG:
            if(m_logger_signal)
            {
                mbl_mw_logger_subscribe(m_logger_signal, this, [](void *context, const MblMwData *data) -> void {
                    auto *handler = static_cast<StreamHandler *>(context);
                    handler->m_data_lock.lock();
                    handler->m_data->push(new StreamEntry(data));
                    handler->m_data_lock.unlock();
                });
                this->m_isValid = true;
            } else {
                m_configuring = true;
                mbl_mw_datasignal_log(m_signal, this, [](void *context, MblMwDataLogger *logger) -> void {
                    auto h = static_cast<StreamHandler *>(context);
                    if (logger != nullptr) {
                        h->m_logger_signal = logger;
                        mbl_mw_logger_subscribe(logger, context, [](void *context, const MblMwData *data) -> void {
                            auto *handler = static_cast<StreamHandler *>(context);
                            handler->m_data_lock.lock();
                            handler->m_data->push(new StreamEntry(data));
                            handler->m_data_lock.unlock();
                        });
                        h->m_root_handler = mbl_mw_logger_generate_identifier(logger);
                        h->m_isValid = true;
                    }
                    h->m_configuring = false;
                });
                while (m_configuring)
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                }
            }
            break;
        case StreamType::STREAMING:
            mbl_mw_datasignal_subscribe(m_signal, this, [](void *context, const MblMwData *data) -> void {
                auto handler = static_cast<StreamHandler *>(context);
                handler->m_data_lock.lock();
                handler->m_data->push(new StreamEntry(data));
                handler->m_data_lock.unlock();
            });
            m_isValid = true;
            break;
        case StreamType::ANONYMOUS_DATASIGNAL:
            mbl_mw_anonymous_datasignal_subscribe(m_anonymous_signal,this,[](void* context, const MblMwData* data) {
                auto handler = static_cast<StreamHandler *>(context);
                handler->m_data_lock.lock();
                handler->m_data->push(new StreamEntry(data));
                handler->m_data_lock.unlock();
            });

            m_isValid = true;
            break;
    }
}

bool StreamHandler::isValid(){
    return m_isValid;
}

StreamHandler::StreamHandler(MblMwDataSignal* signal, StreamType type, const std::string& root_handler):
    m_root_handler(root_handler),
    m_stream_type(type),
    m_data(new std::queue<StreamEntry*>()),
    m_logger_signal(nullptr),
    m_signal(signal),
    m_isValid(false) {
}

StreamHandler::StreamHandler(MblMwAnonymousDataSignal* signal, const std::string& root_handler):
    m_root_handler(root_handler),
    m_stream_type(StreamType::ANONYMOUS_DATASIGNAL),
    m_data(new std::queue<StreamEntry*>()),
    m_logger_signal(nullptr),
    m_signal(nullptr),
    m_anonymous_signal(signal),
    m_isValid(false){
}

StreamHandler::StreamHandler(MblMwDataLogger *signal, const std::string &root_handler) :
        m_root_handler(root_handler),
        m_stream_type(StreamType::LOG),
        m_data(new std::queue<StreamEntry*>()),
        m_logger_signal(signal),
        m_signal(nullptr),
        m_anonymous_signal(nullptr),
        m_isValid(false){

}

void StreamHandler::clearSignals(){
    m_signal = nullptr;
    m_logger_signal = nullptr;

}

StreamHandler::~StreamHandler() {
    if(m_signal)
        mbl_mw_datasignal_unsubscribe(m_signal);
    if(m_logger_signal)
        mbl_mw_logger_remove(m_logger_signal);
    m_data_lock.lock();
    while(!m_data->empty()){
        StreamEntry* entry = m_data->front();
        delete(entry);
        m_data->pop();
    }
    m_data_lock.unlock();
    delete(m_data);
}

StreamEntry::StreamEntry(const MblMwData *data) :
    m_epoch(data->epoch),
    m_length(data->length),
    m_type(data->type_id){
    switch (data->type_id) {
        case MblMwDataTypeId::MBL_MW_DT_ID_UINT32: {
            auto c = static_cast<uint32_t *>(data->value);
            auto output = new float[m_length];
            for (int x = 0; x < m_length; ++x) {
                output[x] = (float) c[x];
            }
            m_data = output;
        }
        break;
        case MblMwDataTypeId::MBL_MW_DT_ID_FLOAT: {
            auto c = static_cast<float *>(data->value);
            auto output = new float(*c);
            m_data = output;
        }
            break;
        case MblMwDataTypeId::MBL_MW_DT_ID_CARTESIAN_FLOAT: {
            auto c = static_cast<MblMwCartesianFloat *>(data->value);
            auto output = new MblMwCartesianFloat();
            output->x = c->x;
            output->y = c->y;
            output->z = c->z;
            m_data = output;

        }
            break;
        case MblMwDataTypeId::MBL_MW_DT_ID_EULER_ANGLE: {
            auto c = static_cast<MblMwEulerAngles*>(data->value);
            auto output = new MblMwEulerAngles();
            output->heading = c->heading;
            output->pitch = c->pitch;
            output->roll = c->roll;
            output->yaw = c->yaw;
            m_data = output;

        }
            break;
        case MblMwDataTypeId::MBL_MW_DT_ID_QUATERNION:{
            auto c = static_cast<MblMwQuaternion*>(data->value);
            auto output = new MblMwQuaternion();
            output->x = c->x;
            output->y = c->y;
            output->z = c->z;
            output->w = c->w;
            m_data = output;

        }
            break;
        case MblMwDataTypeId::MBL_MW_DT_ID_CORRECTED_CARTESIAN_FLOAT:{
            auto c = static_cast<MblMwCorrectedCartesianFloat*>(data->value);
            auto output = new MblMwCorrectedCartesianFloat();
            output->x = c->x;
            output->y = c->y;
            output->z = c->z;
            output->accuracy = c->accuracy;
            m_data = output;

        }
            break;
        case MblMwDataTypeId::MBL_MW_DT_ID_CALIBRATION_STATE:{
            auto c = static_cast<MblMwCalibrationState*>(data->value);
            auto output = new MblMwCalibrationState();
            output->accelrometer = c->accelrometer;
            output->gyroscope = c->gyroscope;
            output->magnetometer = c->magnetometer;
            m_data = output;
        }
            break;
        case MblMwDataTypeId::MBL_MW_DT_ID_INT32:
            break;
        case MblMwDataTypeId::MBL_MW_DT_ID_BYTE_ARRAY:
            break;
        case MblMwDataTypeId::MBL_MW_DT_ID_BATTERY_STATE:
            break;
        case MblMwDataTypeId::MBL_MW_DT_ID_TCS34725_ADC:
            break;
        default:
            break;
    }
}


int64_t StreamEntry::getEpoch(){
    return m_epoch;
}

void* StreamEntry::getData(){
    return m_data;
}

uint8_t StreamEntry::getLength() {
    return m_length;
}

bool StreamHandler::isEmpty() {
    return m_data->empty();
}

void StreamHandler::unLockStream() {
    m_data_lock.unlock();
}

void StreamHandler::lockStream() {
    m_data_lock.lock();
}

StreamEntry *StreamHandler::peek(){
    return m_data->front();
}

void StreamHandler::pop() {
    m_data->pop();
}

std::string StreamHandler::getKey(){
    switch (m_stream_type){
        case StreamType::LOG:
            //using the generated identifier
            return m_root_handler;
        case StreamType::ANONYMOUS_DATASIGNAL:
            return m_root_handler;
        case StreamType::STREAMING:
            return m_root_handler + "_streaming";
    }
    return m_root_handler;
}

unsigned int StreamHandler::size(){
    return m_data->size();
}

MblMwDataSignal* StreamHandler::getSignal(){
    return m_signal;
}

StreamType StreamHandler::getStreamType(){
    return m_stream_type;

}
MblMwDataLogger* StreamHandler::getLogger(){
    return m_logger_signal;
}


StreamEntry::~StreamEntry() {
    free(m_data);
}

MblMwDataTypeId StreamEntry::getType() {
    return m_type;
}

