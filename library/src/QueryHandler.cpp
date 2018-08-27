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

    if(inputs.size() != 2)
        MexUtility::error(engine, "Two Inputs Required");
    if(outputs.size() != 4)
        MexUtility::error(engine, "Four Outputs Required");

    matlab::data::CharArray address =  inputs[1];
    MetawearWrapper* wrapper =  handler->m_connectionHandler->getDevice(address.toAscii());
    if(wrapper == nullptr){
        MexUtility::error(engine, "Invalid wrapper");
    }

    MetawearDataStream<CartesianFloatContainer>* stream = wrapper->getGyroStream();
    std::vector<CartesianFloatContainer> values = stream->dump();


    std::vector<int64_t> epochs;
    std::vector<float> x;
    std::vector<float> y;
    std::vector<float> z;

    //factory.createArray<uint64_t>({1, values.size()},);
    for(auto i = 0; i != values.size(); i++) {
        CartesianFloatContainer value = values[i];

        epochs.push_back(value.epoch);
        x.push_back(value.x);
        y.push_back(value.y);
        y.push_back(value.y);
        z.push_back(value.z);
    }

    matlab::data::ArrayFactory factory;
    outputs[0] = factory.createArray({1,epochs.size()},epochs.begin(),epochs.end());
    outputs[1] = factory.createArray({1,x.size()},x.begin(),x.end());
    outputs[2] = factory.createArray({1,y.size()},y.begin(),y.end());
    outputs[3] = factory.createArray({1,z.size()},z.begin(),z.end());
}

void QueryHandler::mexQueryAccelerometer(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs){
    auto * handler = static_cast<QueryHandler*>(context);

    if(inputs.size() != 2)
        MexUtility::error(engine, "Three Inputs Required");
    if(outputs.size() != 4)
        MexUtility::error(engine, "Four Outputs Required");

    matlab::data::CharArray address =  inputs[1];
    MetawearWrapper* wrapper =  handler->m_connectionHandler->getDevice(address.toAscii());
    if(wrapper == nullptr){
        MexUtility::error(engine, "Invalid wrapper");
    }

    MetawearDataStream<CartesianFloatContainer>* stream = wrapper->getAccelerationStream();
    std::vector<CartesianFloatContainer> values = stream->dump();

    MexUtility::printf(engine,"number of entries queried: " + std::to_string(values.size()) + "\n");

    matlab::data::ArrayFactory factory;

    matlab::data::TypedArray<int64_t> epochs =  factory.createArray<int64_t>({1,values.size()});
    matlab::data::TypedArray<double> x =  factory.createArray<double>({1,values.size()});
    matlab::data::TypedArray<double> y =  factory.createArray<double>({1,values.size()});
    matlab::data::TypedArray<double> z =  factory.createArray<double>({1,values.size()});


    //factory.createArray<uint64_t>({1, values.size()},);
    for(auto i = 0; i != values.size(); i++) {
        CartesianFloatContainer value = values[i];
        epochs[i] = value.epoch;
        x[i] = value.x;
        y[i] = value.y;
        z[i] = value.z;
    }

    outputs[0] = epochs;
    outputs[1] = x;
    outputs[2] = y;
    outputs[3] = z;

}


QueryHandler::~QueryHandler() {
}