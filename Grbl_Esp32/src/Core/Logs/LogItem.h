#pragma once
#ifndef __CORE_LOG_ITEM_H__
#define __CORE_LOG_ITEM_H__

#include <utility>
#include "../Object.h"
#include "../String.h"

namespace Core {
    enum LogLevel {
        Debug,
        Trace,
        Info,
        Warning,
        Error,
        Fatal
    };
    
    class LogItem : public Object {
    private:
        LogLevel    m_level;
        String      m_message;
    public:
        LogItem(
            String message = "", 
            LogLevel level = LogLevel::Info) :
            m_level(level),
            m_message(message) {
        }

        LogItem(const LogItem& other) :
            m_level(other.m_level),
            m_message(other.m_message) {

        }
        
        LogItem(LogItem&& other) :
            m_level(std::move(other.m_level)),
            m_message(std::move(other.m_message)) {

        }
        virtual ~LogItem() = default;

        inline LogLevel level() const noexcept {
            return m_level;
        }

        inline const String& message() const noexcept {
            return m_message;
        }

        inline LogItem& operator =(const LogItem& item) noexcept {
            m_level = std::move(item.m_level);
            m_message = std::move(item.m_message);
            return *this;
        }
    };
} // namespace name


#endif