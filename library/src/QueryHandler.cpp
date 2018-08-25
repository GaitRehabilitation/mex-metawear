//
// Created by Michael on 8/24/2018.
//

#include "handlers/QueryHandler.h"
#include "handlers/ConnectionHandler.h"
#include "FunctionWrapper.h"
#include <MetawearWrapper.h>
#include <MexUtility.h>
#include "StreamTypes.h"
#include "MatlabDataArray.hpp"

QueryHandler::QueryHandler(ConnectionHandler* connectionHandler,FunctionWrapper* wrapper):
        m_connectionHandler(connectionHandler) {
    std::map<std::string, WrapperMethod *> functions = {
            {"query_gyro",          mexQueryGyro},
            {"query_accelerometer", mexQueryAccelerometer},
    };
    wrapper->registerMethod(this, functions);
}

void QueryHandler::mexQueryGyro(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs) {
    auto * handler = static_cast<QueryHandler*>(context);

    if(inputs.size() != 2){
        MexUtility::error(engine, "Three Inputs Required");
        return;
    }

    matlab::data::ObjectArray o =  inputs[1];
    MetawearWrapper* wrapper =  handler->m_connectionHandler->mexToMetawearWrapper(engine,o);

    MetawearDataStream<CartesianFloatContainer *>* stream = wrapper->getGyroStream();
    std::vector<CartesianFloatContainer*> values = stream->dump();

    if(inputs.size() != 4){
        MexUtility::error(engine, "Four Outputs Required");
        return;
    }

    std::vector<int64_t> epochs;
    std::vector<float> x;
    std::vector<float> y;
    std::vector<float> z;

    //factory.createArray<uint64_t>({1, values.size()},);
    for(auto i = 0; i != values.size(); i++) {
        CartesianFloatContainer* value = values[i];

        epochs.push_back(value->epoch);
        x.push_back(value->x);
        y.push_back(value->y);
        z.push_back(value->z);
        delete value;
    }

    matlab::data::ArrayFactory factory;
    outputs[0] = factory.createArray({1,epochs.size()},epochs.begin(),epochs.end());
    outputs[1] = factory.createArray({1,x.size()},x.begin(),x.end());
    outputs[2] = factory.createArray({1,y.size()},y.begin(),y.end());
    outputs[3] = factory.createArray({1,z.size()},z.begin(),z.end());
}

void QueryHandler::mexQueryAccelerometer(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs){
    auto * handler = static_cast<QueryHandler*>(context);

    if(inputs.size() != 2){
        MexUtility::error(engine, "Three Inputs Required");
        return;
    }

    matlab::data::ObjectArray o =  inputs[1];
    MetawearWrapper* wrapper =  handler->m_connectionHandler->mexToMetawearWrapper(engine,o);

    MetawearDataStream<CartesianFloatContainer *>* stream = wrapper->getAccelerationStream();
    std::vector<CartesianFloatContainer*> values = stream->dump();

    if(inputs.size() != 4){
        MexUtility::error(engine, "Four Outputs Required");
        return;
    }

    std::vector<int64_t> epochs;
    std::vector<float> x;
    std::vector<float> y;
    std::vector<float> z;

    //factory.createArray<uint64_t>({1, values.size()},);
    for(auto i = 0; i != values.size(); i++) {
        CartesianFloatContainer* value = values[i];

        epochs.push_back(value->epoch);
        x.push_back(value->x);
        y.push_back(value->y);
        z.push_back(value->z);
        delete value;
    }

    matlab::data::ArrayFactory factory;
    outputs[0] = factory.createArray({1,epochs.size()},epochs.begin(),epochs.end());
    outputs[1] = factory.createArray({1,x.size()},x.begin(),x.end());
    outputs[2] = factory.createArray({1,y.size()},y.begin(),y.end());
    outputs[3] = factory.createArray({1,z.size()},z.begin(),z.end());

}


QueryHandler::~QueryHandler() {
}