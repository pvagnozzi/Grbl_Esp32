#pragma once
#ifndef __LOG_WRITER_H__
#define __LOG_WRITER_H__

#include "../Object.h"
#include "../String.h"

namespace Core {
    
    class LogWriter : public Object {
    public:
        virtual void write(LogWriter& item) = 0;
    };
} // namespace name


#endif