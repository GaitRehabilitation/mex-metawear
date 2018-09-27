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

#ifndef MEX_METAWEAR_MEXPRINTSTREAM_H
#define MEX_METAWEAR_MEXPRINTSTREAM_H

#include <string>
#include <mutex>
#include <queue>
#include <mex.hpp>

class MexPrintStream {
private:
    std::mutex m_lock;
    std::queue<std::string> m_message;
    volatile bool m_isBlocking;
    std::shared_ptr<matlab::engine::MATLABEngine> m_engine;
public:
    MexPrintStream(std::shared_ptr<matlab::engine::MATLABEngine> engine);

    void printf(const std::string& message);

    void clear();
    void release();
    void setBlock();

    void block();
};

#endif //MEX_METAWEAR_MEXPRINTSTREAM_H
