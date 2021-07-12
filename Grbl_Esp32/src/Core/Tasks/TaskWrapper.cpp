#include <utility>
#include "TaskWrapper.h"

namespace Core {

    TaskWrapper::TaskWrapper(
        TaskFunction function, 
        TaskParameters parameters,
        CompleteFunction complete, 
        ExceptionFunction exception) :        
        m_function(function),
        m_parameters(parameters),
        m_complete(complete),
        m_exception(exception)  {
    }
        
    TaskWrapper::TaskWrapper(TaskWrapper&& other) :
        m_function(std::move(other.m_function)),
        m_parameters(std::move(other.m_parameters)),
        m_complete(std::move(other.m_complete)),
        m_exception(std::move(other.m_exception)) {

    }

    TaskWrapper::~TaskWrapper() {
    }

    void TaskWrapper::execute() {
        try {
            
        }
        catch(...) {

        }
    }

}

