//
// Created by Michael on 9/5/2018.
//

#include <StreamHandler.h>
#include <metawear/core/logging.h>

#include "StreamHandler.h"

StreamHandler::StreamHandler(MblMwDataSignal* signal,StreamType type){
    m_data = new std::queue<StreamEntry*>();
    m_logger_signal = nullptr;
    m_signal = signal;
    switch (type) {
        case StreamType::LOG:
            mbl_mw_datasignal_log(signal, this, [](void* context, MblMwDataLogger* logger) -> void {
                auto handler = static_cast<StreamHandler *>(context);
                if (logger != nullptr) {
                    handler->m_logger_signal = logger;
                    mbl_mw_logger_subscribe(logger, handler, [](void *context, const MblMwData *data) -> void {
                        StreamHandler*  handler = static_cast<StreamHandler *>(context);
                         handler->m_data_lock.lock();
                         handler->m_data->push(new StreamEntry(data));
                         handler->m_data_lock.unlock();
                    });

                    printf("logger ready\n");
                } else {
                    printf("Failed to create the logger\n");
                }
            });


            break;
        case StreamType::STREAMING :
            mbl_mw_datasignal_subscribe(signal, this, [](void *context, const MblMwData *data) -> void {
                auto handler = static_cast<StreamHandler *>(context);
                handler->m_data_lock.lock();
                handler->m_data->push(new StreamEntry(data));
                handler->m_data_lock.unlock();
            });
            break;
    }
}



StreamHandler::~StreamHandler() {
    mbl_mw_datasignal_unsubscribe(m_signal);
    if(m_logger_signal)
        mbl_mw_logger_remove(m_logger_signal);
    m_data_lock.lock();
    while(!m_data->empty()){
        StreamEntry* entry = m_data->front();
        free(entry);
        m_data->pop();
    }
    m_data_lock.unlock();
    free(m_data);
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
unsigned int StreamHandler::size(){
    return m_data->size();
}


MblMwDataSignal* StreamHandler::getSignal(){
    return m_signal;
}



StreamEntry::~StreamEntry() {
    free(m_data);
}

MblMwDataTypeId StreamEntry::getType() {
    return m_type;
}
