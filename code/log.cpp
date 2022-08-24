//
// Created by root on 8/22/22.
//

#include "log.h"
#include "utility.h"

#include <time.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/stat.h>


Logger* Logger::mInstance = nullptr;

Logger * Logger::GetInstance() {
    if(!mInstance) {
        mInstance = new Logger;
    }
    return mInstance;
}

Logger::Logger() {
    mFile = nullptr;
    InitLog(LOG_DESTINATION_FILE);
}

Logger::~Logger() {
    if(mFile){
        fclose(mFile);
        mFile = nullptr;
    }
}

int Logger::InitLog(LOG_DESTINATION _logDestination) {
    if(mFile){
        fclose(mFile);
        mFile = nullptr;
    }
    if(access("../log/", F_OK) != 0){
        mkdir("../log/", 0777);
    }
    std::string logFilePath = "../log/" + GetTimeString(LINE_FORMAT) + ".log";
    mFile = fopen(logFilePath.c_str(), "w");
    if(!mFile){
        return -1;
    }

    CreateLinkToLog(logFilePath);
    pthread_spin_init(&mLock, PTHREAD_PROCESS_PRIVATE);
    mLogDestination = _logDestination;

    /// Start the consume thread.
    WriteLogToDestination();
    return 0;
}

int Logger::LogToBuffer(const char *_logLevel, const char *_format, ...) {
    char msg[512] = {0};
    va_list vArgList;
    va_start(vArgList, _format);
    vsnprintf(msg, 512, _format, vArgList);
    va_end(vArgList);

    char content[1024] = {0};
    snprintf(content, sizeof(content), "[%s]:[%s]:[%s():%d]: %s\n",
             _logLevel, GetTimeString(SPACE_FORMAT).c_str(), __FUNCTION__ , __LINE__, msg);
    pthread_spin_lock(&mLock);
    mBuffer.push(content);
    pthread_spin_unlock(&mLock);
    return 0;
}

int Logger::WriteLogToDestination() {
    pthread_t tid = 0;
    switch (mLogDestination) {
        case LOG_DESTINATION_TERMINAL:{
            WriteLogToTerminal(this);
            break;
        }
        default:{
            pthread_create(&tid, nullptr, WriteLogToFile, this);
            break;
        }
    }
    return 0;
}

void* Logger::WriteLogToFile(void *_this) {
    pthread_detach(pthread_self());
    auto pLogger = (Logger*)_this;
    while(true) {
        while (pLogger->mBuffer.empty()) {
        }
        pthread_spin_lock(&(pLogger->mLock));
        auto logContent = pLogger->mBuffer.front();
        pLogger->mBuffer.pop();
        pthread_spin_unlock(&(pLogger->mLock));
//        fprintf(pLogger->mFile, "%s\n", logContent.c_str());
        fwrite(logContent.c_str(), logContent.length(), 1, pLogger->mFile);
        fflush(pLogger->mFile);
    }
    return nullptr;
}

void * Logger::WriteLogToTerminal(void *_this) {
    /// TODO:
    return nullptr;
}

void Logger::CreateLinkToLog(const std::string &_logFilePath) {
    auto processName = GetNameOfProcess();
    std::string linkName = processName + ".log";
    unlink(linkName.c_str());
    symlink(_logFilePath.c_str(), linkName.c_str());  // create a soft link.
//    link(_logFilePath.c_str(), linkName.c_str());  // This function create a hard link
}


std::string GetTimeString(TIME_STRING_FORMAT _format){
    struct timespec timestamp{0};
    clock_gettime(CLOCK_REALTIME, &timestamp);

    struct tm dateNow{0};
    localtime_r(&(timestamp.tv_sec), &dateNow);
    char buffer[64] = {0};

    switch (_format) {
        case LINE_FORMAT:{
            snprintf(buffer, sizeof(buffer), "%04d_%02d_%02d_%02d_%02d_%02d", dateNow.tm_year+1900, dateNow.tm_mon + 1,
                     dateNow.tm_mday, dateNow.tm_hour, dateNow.tm_min, dateNow.tm_sec);
            break;
        }
        default:{
            snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d %02d:%02d:%02d.%09lu", dateNow.tm_year+1900, dateNow.tm_mon + 1,
                     dateNow.tm_mday, dateNow.tm_hour, dateNow.tm_min, dateNow.tm_sec, timestamp.tv_nsec);
            break;
        }
    }
    return buffer;
}
