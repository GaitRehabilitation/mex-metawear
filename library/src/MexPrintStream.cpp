#include <utility>

//
// Created by Michael on 9/13/2018.
//

#include <MexPrintStream.h>
#include <MexUtility.h>

MexPrintStream::MexPrintStream(std::shared_ptr<matlab::engine::MATLABEngine> engine):
    m_isBlocking(false),
    m_message(),
    m_lock() ,
    m_engine(std::move(engine)){

}

void MexPrintStream::printf(const std::string &message) {
    m_lock.lock();
    m_message.push(std::string(message) + "\n");
    m_lock.unlock();
}

void MexPrintStream::clear() {
    //pop till queue is empty
    while (!m_message.empty()) m_message.pop();
}

void MexPrintStream::block() {
    do {
        //sleep for 200 milliseconds and check for new messages
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        //lock and try to read messages from queue
        m_lock.lock();
        while (!m_message.empty()) {
            std::string message = m_message.front();
            MexUtility::printf(m_engine, message);
            m_message.pop();
        }
        m_lock.unlock();
    }
    while (m_isBlocking);
}

void MexPrintStream::release() {
    m_isBlocking = false;
}

void MexPrintStream::setBlock() {
    m_isBlocking = true;
}
