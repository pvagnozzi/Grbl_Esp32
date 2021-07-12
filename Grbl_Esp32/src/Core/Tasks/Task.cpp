#include <utility>
#include "Task.h"

namespace Core {

    void taskExecutor(void* params) {
        TaskWrapper* wrapper = (TaskWrapper*) params;
        wrapper->execute();
    }

    Task::Task(
            TaskFunction proc, 
            TaskName taskName,
            TaskParameters parameters, 
            Size stackSize, 
            TaskPriority priority,            
            CoreId coreId) : 
            m_wrapper(proc, parameters),
            m_handle(),
            m_status(),
            m_priority(priority) {
        
        void* p = &m_wrapper;

        if (coreId >= 0) {
            xTaskCreatePinnedToCore(
                taskExecutor,
			    taskName,
			    stackSize,
			    p,
			    priority,
			    &m_handle,
                coreId);     

        }
        else {
            xTaskCreate(
                taskExecutor,
                taskName,
                stackSize,
                p,
                priority,
                &m_handle);     
        }

        this->priority(m_priority);
    }

    Task::Task(Task&& other) :
        m_wrapper(std::move(other.m_wrapper)),
        m_handle(std::move(other.m_handle)),
        m_status(std::move(other.m_status)),
        m_priority(std::move(other.m_priority)) {    
    }

    Task::~Task() {
        abort();
    }

    void Task::priority(TaskPriority priority) {
        vTaskPrioritySet(m_handle, priority);
        m_priority = priority;
    }

    void Task::abort() {                
        destroyTask(TaskStatus::Aborted);        
    }

    void Task::suspend() {
        changeStatus(TaskStatus::Suspended, vTaskSuspend);
    }

    void Task::resume() {
        changeStatus(TaskStatus::Resumed, vTaskResume);
    }

    void Task::doCompleted() {
        destroyTask(TaskStatus::Completed);        
    }

    void Task::destroyTask(TaskStatus status) {
        if (!isHandle()) {
            return;
        }

        vTaskDelete(m_handle);
        m_handle = NULL;
        m_status = status;
    }
}
