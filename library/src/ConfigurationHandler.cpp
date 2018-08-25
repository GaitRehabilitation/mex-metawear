//
// Created by Michael on 8/24/2018.
//

#include <MexUtility.h>
#include <MetawearWrapper.h>
#include "handlers/ConfigurationHandler.h"
#include "handlers/ConnectionHandler.h"

ConfigurationHandler::ConfigurationHandler(ConnectionHandler* connectionHandler,FunctionWrapper* wrapper) :
        m_connectionHandler(connectionHandler){
    std::map<std::string, WrapperMethod *> functions =  {
            {"configure_gyro", mexConfigureGyro},
            {"configure_accelerometer", mexConfigureAccelerometer}
    };
    wrapper->registerMethod(this, functions);
}



void ConfigurationHandler::mexConfigureGyro(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs){
    ConfigurationHandler* handler = static_cast<ConfigurationHandler*>(context);

    if(inputs.size() != 3){
        MexUtility::error(engine, "Four Inputs Required");
        return;
    }

    matlab::data::ObjectArray o =  inputs[1];
    MetawearWrapper* wrapper =  handler->m_connectionHandler->mexToMetawearWrapper(engine,o);

    matlab::data::TypedArray<double> d = inputs[2];
    if(d.getNumberOfElements() != 2){
        MexUtility::error(engine, "Range and sample rate is required");
        return;
    }
    wrapper->configureAccelerometer((float)d[0],(float)d[1]);
}
void ConfigurationHandler::mexConfigureAccelerometer(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs){
    ConfigurationHandler* handler = static_cast<ConfigurationHandler*>(context);

    if(inputs.size() != 4){
        MexUtility::error(engine, "Four Inputs Required");
        return;
    }

    matlab::data::ObjectArray o =  inputs[1];
    MetawearWrapper* wrapper =  handler->m_connectionHandler->mexToMetawearWrapper(engine,o);

    matlab::data::TypedArray<int16_t> d = inputs[2];
    MblMwGyroBmi160Odr sample;
    MblMwGyroBmi160Range range;
    switch(d[0]){
        case 25:
            sample = MBL_MW_GYRO_BMI160_ODR_25Hz;
            break;
        case 50:
            sample = MBL_MW_GYRO_BMI160_ODR_50Hz;
            break;
        case 100:
            sample = MBL_MW_GYRO_BMI160_ODR_100Hz;
            break;
        case 200:
            sample = MBL_MW_GYRO_BMI160_ODR_200Hz;
            break;
        case 400:
            sample = MBL_MW_GYRO_BMI160_ODR_400Hz;
            break;
        case 800:
            sample = MBL_MW_GYRO_BMI160_ODR_800Hz;
            break;
        case 1600:
            sample = MBL_MW_GYRO_BMI160_ODR_1600Hz;
            break;
        case 3200:
            sample = MBL_MW_GYRO_BMI160_ODR_3200Hz;
            break;
        default:
            MexUtility::error(engine, "Gyro supported ranges: 25hz,50hz,100hz,200hz,400hz,800hz,1600hz,3200hz ");
            return;
    }

    switch(d[1]){
        case 2000:
            range = MBL_MW_GYRO_BMI160_RANGE_2000dps;
            break;
        case 1000:
            range = MBL_MW_GYRO_BMI160_RANGE_1000dps;
            break;
        case 500:
            range = MBL_MW_GYRO_BMI160_RANGE_500dps;
            break;
        case 250:
            range = MBL_MW_GYRO_BMI160_RANGE_250dps;
            break;
        case 125:
            range = MBL_MW_GYRO_BMI160_RANGE_125dps;
            break;
        default:
            MexUtility::error(engine, "Gyro supported ranges: 2000dps, 1000dps, 500dps, 250dps, 125dps ");
            return;
    }
    wrapper->configureGyroscope(range,sample);

}




