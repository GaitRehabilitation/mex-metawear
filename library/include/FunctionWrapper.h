//
// Created by Michael on 8/23/2018.
//

#ifndef MEX_METAWEAR_FUNCTIONWRAPPER_H
#define MEX_METAWEAR_FUNCTIONWRAPPER_H

#include "MatlabDataArray.hpp"
#include "mex.hpp"
#include <string>
#include <vector>
#include <map>

class FunctionWrapper;

class ParameterWrapper{
private:
    std::vector<matlab::data::Array>::iterator m_begin;
    std::vector<matlab::data::Array>::iterator m_end;

    size_t  m_size;
public:

    ParameterWrapper( std::vector<matlab::data::Array>::iterator b,  std::vector<matlab::data::Array>::iterator e, size_t size) : m_begin(b), m_end(e), m_size(size) {}


    typename std::iterator_traits<std::vector<matlab::data::Array>::iterator>::difference_type internal_size() {
        return std::distance(m_begin, m_end);
    }

    std::vector<matlab::data::Array>::iterator begin() {
        return m_begin;
    }

    std::vector<matlab::data::Array>::iterator end() {
        return m_end;
    }

    bool empty() {
        return m_size == 0;
    }

    typename size_t size() {
        return m_size;
    }


    typename std::iterator_traits<std::vector<matlab::data::Array>::iterator>::reference operator[](size_t i) {
        if (static_cast<int>(i) + 1 > internal_size())
            throw matlab::engine::Exception("ArgumentList index out of range.");

        return *(m_begin + i);
    }
};

typedef void (WrapperMethod)(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context, ParameterWrapper& outputs, ParameterWrapper& inputs);
typedef struct {
    char* target;
    WrapperMethod* wrapper;
} MethodTable;


class FunctionWrapper{
private:
    typedef struct {
        void* context;
        WrapperMethod* wrapper;
    } MethodContainer;

    std::map<std::string,MethodContainer*> m_methods;
public:
    FunctionWrapper();
    ~FunctionWrapper();
    void registerMethod(void* context, const std::map<std::string,WrapperMethod*>& methods);
    bool invoke(const std::string& key,std::shared_ptr<matlab::engine::MATLABEngine> engine, ParameterWrapper& outputs, ParameterWrapper& inputs);
};





#endif //MEX_METAWEAR_FUNCTIONWRAPPER_H
