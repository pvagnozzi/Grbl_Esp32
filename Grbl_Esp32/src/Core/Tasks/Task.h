#pragma once
#ifndef __CORE_TASK_H__
#define __CORE_TASK_H__

#include "TaskWrapper.h"

#include <inttypes.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

#include "../Object.h"

#ifndef TASK_STACK_SIZE_DEFAULT
#define TASK_STACK_SIZE_DEFAULT 2028
#endif

namespace Core {

    enum TaskPriority {
        Idle,
        Lowest,
        BelowNormal,
        Normal,
        AboveNormal,
        Highest,
        Critical
    };

    enum TaskStatus : uint8_t {    
        Active = 1,
        Suspended = 2,
        Resumed = 4,
        Aborted = 8,
        Completed = 16        
    };

    class Task : public Object {
    public:
        typedef typename TaskWrapper::TaskFunction TaskFunction;
        typedef typename TaskWrapper::TaskParameters TaskParameters;
        typedef TaskHandle_t TaskHandle;
        
        typedef uint16_t        Size;
        typedef int             CoreId;
        typedef const char *    TaskName;

    private:        
        TaskWrapper     m_wrapper;
        TaskHandle      m_handle;
        TaskStatus      m_status;
        TaskPriority    m_priority;

    protected:
        template <typename F>
        void changeStatus(TaskStatus status, F func) {
            if (status == m_status) {
                return;
            }

            func(m_handle);

            m_status = status;
        }

        void doCompleted();
        void destroyTask(TaskStatus status = TaskStatus::Aborted);    
             
    public:
        Task(
            TaskFunction proc, 
            TaskName taskName,
            TaskParameters parameters = NULL,            
            Size stackSize = TASK_STACK_SIZE_DEFAULT, 
            TaskPriority priority = TaskPriority::Normal,
            CoreId core = -1);
        Task(const Task&) = delete;
        Task(Task&& other);
        virtual ~Task();

        inline TaskStatus status() const noexcept {
            return m_status;
        }

        inline bool running() const noexcept {
            return isHandle() && (m_status == TaskStatus::Active || m_status == TaskStatus::Resumed);
        }

        inline TaskHandle handle() const noexcept {
            return m_handle;
        }
        
        inline bool isHandle() const noexcept {
            return m_handle != NULL;
        }

        inline TaskPriority priority() const noexcept {
            return m_priority;
        }

        void priority(TaskPriority priority);

        void abort();
        void suspend();
        void resume();

        inline operator TaskHandle() const noexcept {
            return handle();
        }
    };
   
}

#endif