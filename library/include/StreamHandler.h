//
// Created by Michael on 9/5/2018.
//

#ifndef MEX_METAWEAR_STREAMHANDLER_H
#define MEX_METAWEAR_STREAMHANDLER_H

#include <queue>
#include <metawear/core/data.h>
#include <metawear/core/datasignal.h>
#include <metawear/core/types.h>
#include <mutex>

struct MblMwDataSignal;
enum StreamType{
    LOG,
    STREAMING
};


class StreamEntry{
private:
    MblMwDataTypeId m_type;
    int64_t m_epoch;
    void* m_data;
    uint8_t m_length;
public:
    int64_t getEpoch();
    void* getData();
    MblMwDataTypeId getType();

    StreamEntry(const MblMwData *data);
    ~StreamEntry();
};
typedef void (HandleData)(void* context,const StreamEntry& entry);

class StreamHandler{
private:
    std::mutex m_data_lock;
    std::queue<StreamEntry*>* m_data;
    MblMwDataSignal* m_signal;
    MblMwDataLogger* m_logger_signal;
public:
    StreamHandler(MblMwDataSignal* signal,StreamType type);
    ~StreamHandler();

    void empty(void* context,HandleData*);
    bool pop(void* context, HandleData*);
    bool isEmpty();


};

#endif //MEX_METAWEAR_STREAMHANDLER_H
