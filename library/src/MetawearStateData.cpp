//
// Created by Michael on 10/1/2018.
//

#include <MetawearStateData.h>
#include <MetawearWrapper.h>
#include <metawear/core/logging.h>
#include <metawear/platform/memory.h>
#include <MexUtility.h>

MetawearStateData::MetawearStateData(MetawearWrapper *wrapper) {
    MblMwMetaWearBoard* board = wrapper->getBoard();
    m_data = mbl_mw_metawearboard_serialize(board,&m_size);
    for(std::map<std::string, StreamHandler*>::iterator it = wrapper->begin(); it != wrapper->end(); ++it){
        std::string identifier = it->first;
        StreamHandler* handler = it->second;
        if(handler->getStreamType() == StreamType::LOG) {
            m_handlers.push_back(mbl_mw_logger_get_id(handler->getLogger()));
        }
    }
}


void MetawearStateData::restore(MetawearWrapper* wrapper){
    MblMwMetaWearBoard* board = wrapper->getBoard();
    mbl_mw_metawearboard_deserialize(board,m_data,m_size);
    for(uint8_t value: m_handlers) {
        MblMwDataLogger* logger = mbl_mw_logger_lookup_id(board,value);
        auto id = mbl_mw_logger_generate_identifier(logger);
        wrapper->registerHandler(new StreamHandler(logger,std::string(id)));
    }
}
