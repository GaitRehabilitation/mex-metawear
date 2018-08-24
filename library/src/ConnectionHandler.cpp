//
// Created by Michael on 8/23/2018.
//
#include <MexUtility.h>
#include <regex>
#include "ConnectionHandler.h"
#include "FunctionWrapper.h"
#include "MetawearWrapper.h"



ConnectionHandler::ConnectionHandler(FunctionWrapper* wrapper): m_devices() {
    std::map<std::string, WrapperMethod *> functions =  {
        {"connect", mexConnect},
        {"disconnect", mexDisconnect}
    };
    wrapper->registerMethod(this, functions);
}

MetawearWrapper* ConnectionHandler::getDevice(const std::string& mac){
    auto  it = m_devices.find(mac);
    if (it != m_devices.end())
        return it->second;
    return nullptr;
}

MetawearWrapper* ConnectionHandler::addDevice(const std::string& mac){
    auto wrapper = new MetawearWrapper(mac);
    m_devices.emplace(mac,wrapper);
    return wrapper;
}

MetawearWrapper* ConnectionHandler::removeDevice(const std::string& mac){
    auto item = getDevice(mac);
    if(item != nullptr)
        m_devices.erase(mac);
    return item;

}

ConnectionHandler::~ConnectionHandler() {

}

 void ConnectionHandler::mexConnect(std::shared_ptr<matlab::engine::MATLABEngine> engine,
         void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs){
    ConnectionHandler* handler = static_cast<ConnectionHandler*>(context);

    if(inputs.size() != 2){
         MexUtility::error(engine,"Two Inputs Required");
         return;
     }

     if(outputs.size() != 1){
         MexUtility::error(engine,"Single argument for output");
         return;
     }

     if(inputs[1].getType() != matlab::data::ArrayType::CHAR){
         MexUtility::error(engine,"Second Parameter has to be string type");
         return;
     }
     matlab::data::CharArray address = inputs[1];

     // standard IEEE 802 format for MAC-48 address. verify if the format is correct.
     std::regex addressMatch ("^([0-9A-Fa-f]{2}[:-]){5}([0-9A-Fa-f]{2})$");
     if(!std::regex_match (address.toAscii(),addressMatch)){
         MexUtility::error(engine,"Invalid bluetooth address for second parameter");
         return;
     }
     matlab::data::ArrayFactory factory;
     matlab::data::ObjectArray result(factory.createEmptyArray());
     // set the address property for result
     engine->setProperty(result,"address",address);
//     outputs[0] = result;
     //outputs[0]


     MetawearWrapper* wrapper = handler->addDevice(address.toAscii());
     wrapper->connect();

 }

void ConnectionHandler::mexDisconnect( std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs){
    ConnectionHandler* handler = static_cast<ConnectionHandler*>(context);
}
