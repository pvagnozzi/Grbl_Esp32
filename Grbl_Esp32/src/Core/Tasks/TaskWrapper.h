#pragma once
#ifndef __CORE_TASK_WRAPPER_H__
#define __CORE_TASK_WRAPPER_H__

#include "../Object.h"
#include <functional>

namespace Core {
     
    class TaskWrapper : public Object {
    public:
        typedef void *TaskParameters;
        typedef std::function<void(TaskParameters)> TaskFunction;        
        typedef std::function<void()> CompleteFunction;
        typedef std::function<void()> ExceptionFunction;

    private:
        TaskFunction        m_function;
        TaskParameters      m_parameters;
        CompleteFunction    m_complete;
        ExceptionFunction   m_exception;
    public:
        TaskWrapper(
            TaskFunction function, 
            TaskParameters parameters,
            CompleteFunction complete = NULL, 
            ExceptionFunction exception = NULL);
        TaskWrapper(const TaskWrapper&) = delete;
        TaskWrapper(TaskWrapper&& other);
        virtual ~TaskWrapper();

        void execute();
    };

}

#endif