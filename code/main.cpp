#include "log.h"
#include <iostream>
#include <pthread.h>
#include <unistd.h>


#define TEST_LOG_THREAD_NUM 10


void* LogThread(void *_arg){
    int i = 0;
    for(i = 0; i < 100000; i++){
        INFO("test log [%d], tid = [%d]", i, pthread_self());
    }
    return nullptr;
}

int main() {
//    std::cout << GetTimeString(LINE_FORMAT) << std::endl;
//    std::cout << GetTimeString(SPACE_FORMAT) << std::endl;
    Logger::SetCpuIdWrite(1);
    INFO("Hello, world!");
//    printf("11111\n");
//    auto obj = Logger::GetInstance();
//    printf("11111\n");
//    obj->LogToBuffer("INFO", "Hello, world!%d", 100);

    pthread_t tidArray[TEST_LOG_THREAD_NUM] = {0};
    int i = 0;
    for(i = 0; i < TEST_LOG_THREAD_NUM; i++){
        pthread_create(&tidArray[i], NULL, LogThread, NULL);
    }
    for(i = 0; i < TEST_LOG_THREAD_NUM; i++){
        pthread_join(tidArray[i], NULL);
    }
    INFO("Log test finished.");
//    printf("finish,%s", GetTimeString(SPACE_FORMAT).c_str());
    usleep(1000000);
//    while(true){
//
//    }
    return 0;
}
