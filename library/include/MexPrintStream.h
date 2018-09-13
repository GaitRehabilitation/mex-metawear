//
// Created by Michael on 9/13/2018.
//

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
