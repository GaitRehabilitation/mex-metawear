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
    int64_t m_epoch;
    void* m_data;
    uint8_t m_length;
    MblMwDataTypeId m_type;
public:
    MblMwDataTypeId  getType();
    int64_t getEpoch();
    void* getData() ;
    uint8_t getLength() ;

    StreamEntry(const MblMwData *data);
    ~StreamEntry();
};
typedef void (HandleData)(void* context,StreamEntry& entry);

class StreamHandler{
private:
    std::mutex m_data_lock;
    std::queue<StreamEntry*>* m_data;
    MblMwDataSignal* m_signal;
    MblMwDataLogger* m_logger_signal;
    std::string m_root_handler;
    StreamType m_stream_type;
public:
    StreamHandler(MblMwDataSignal* signal,StreamType type, const std::string& root_handler);
    ~StreamHandler();
    void lockStream();
    void unLockStream();
    StreamEntry* peek();
    unsigned int size();
    void pop();
    bool isEmpty();
    MblMwDataSignal* getSignal();
    std::string getKey();
};

#endif //MEX_METAWEAR_STREAMHANDLER_H
