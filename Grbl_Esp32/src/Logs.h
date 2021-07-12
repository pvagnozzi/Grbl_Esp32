#pragma once

#include "Config.h"
#include "NutsBolts.h"
#include "Exec.h"
#include "Error.h"
#include "Report.h"

#define LOG(level, format, ...)     grbl_msg_sendf(CLIENT_ALL, level, format, __VA_ARGS__)
#define LOG_DEBUG(format, ...)      LOG(MsgLevel::Debug, format, __VA_ARGS__)
#define LOG_INFO(format, ...)       LOG(MsgLevel::Info, format, __VA_ARGS__)
#define LOG_WARNING(format, ...)    LOG(MsgLevel::Warning, format, __VA_ARGS__)
#define LOG_ERROR(format, ...)      LOG(MsgLevel::Error, format, __VA_ARGS__)
