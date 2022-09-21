//
// Created by root on 8/22/22.
//

#ifndef LOG_LIB_LOG_H
#define LOG_LIB_LOG_H

#include <iostream>
#include <string>
#include <queue>
#include <pthread.h>




enum LOG_DESTINATION{
    LOG_DESTINATION_TERMINAL = 0,
    LOG_DESTINATION_FILE = 1,
};

class Logger{
public:
    Logger(const Logger &) = delete;
    static Logger* GetInstance();
    /**
    * @brief: Initialize class Logger, open log file, initialize pthread_spinlock_t
    * @param:
    * @retType: int
    * @retVal: 0: success
    * @retVal: -1: fail
    * @bug:
    **/
    int InitLog(LOG_DESTINATION _logDestination = LOG_DESTINATION_FILE);

    /**
    * @brief: Push the log content to the buffer.
    * @param: _logLevel: FATAL, ERROR, WARNING, INFO, DEBUG, TRACE
    * @_format: C format
    * @...: va_list
    * @retType: int
    * @retVal: 0: success, -1: fail.
    * @bug:
    **/
    int LogToBuffer(const char *_logLevel, const char *_format, ...);

private:
    Logger();
    virtual ~Logger();

    /**
    * @brief: Create a thread acquiring data from buffer and write data to destination(file or terminal)
    * @param:
    * @retType: int
    * @retVal: 0: success; -1: fail.
    * @bug:
    **/
    int WriteLogToDestination();

    /**
    * @brief: Get data from buffer and write it to file.
    * @param: this pointer.
    * @retType: void *
    * @retVal:
    * @bug:
    **/
    static void* WriteLogToFile(void *_this);

    /**
    * @brief:
    * @param:
    * @retType:
    * @retVal:
    * @bug:
    **/
    static void* WriteLogToTerminal(void *_this);

    /**
    * @brief: Create a link binded with newest log file.
    * @param:
    * @retType:
    * @retVal:
    * @bug:
    **/
    static void CreateLinkToLog(const std::string &_logFilePath);


    // member variables
    static Logger *mInstance;  // The unique instance of class Logger;
    FILE *mFile;  // The file recording the log content.
    pthread_spinlock_t mLock;  // Protect the log in case of multi-thread.
    std::queue<std::string> mBuffer;
    LOG_DESTINATION mLogDestination;  // The output destination of log.
};

/**
* @brief: Get the name of the process.
* @param:
* @retType: std::string
* @retVal: The name of the process.
* @bug:
**/
extern std::string GetNameOfProcess();

/**
* @brief: Return a string of time by the specify format.
* @param: _format: 0: YYYY_MM_DD_HH_MM_SS.log   1: YYYY-MM-DD HH:MM:SS:
* @retType: std::string
* @retVal: The string of time in the specified format.
* @bug:
**/
enum TIME_STRING_FORMAT{
    LINE_FORMAT = 0,
    SPACE_FORMAT = 1,
};
extern std::string GetTimeString(TIME_STRING_FORMAT _format);


#define INFO(fmt, ...) do{ \
    Logger::GetInstance()->LogToBuffer("INFO", fmt, ##__VA_ARGS__); \
} while(0)


#endif //LOG_LIB_LOG_H
