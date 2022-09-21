//
// Created by hapYP on 9/15/22.
//

#include "easylogging++.h"
#include <unistd.h>
#include <pthread.h>

INITIALIZE_EASYLOGGINGPP

#define TEST_LOG_THREAD_NUM 10

void* LogThread(void *_arg){
    int i = 0;
    for(i = 0; i < 100000; i++){
        LOG(INFO) << "test log" << i << ", tid = " << pthread_self();
        //INFO("test log [%d], tid = [%d]", i, pthread_self());
    }
    return nullptr;
}

int main(int argc, char* argv[]) {
    LOG(INFO) << "begin";

    pthread_t tidArray[TEST_LOG_THREAD_NUM] = {0};
    int i = 0;
    for(i = 0; i < TEST_LOG_THREAD_NUM; i++){
        pthread_create(&tidArray[i], NULL, LogThread, NULL);
    }
    for(i = 0; i < TEST_LOG_THREAD_NUM; i++){
        pthread_join(tidArray[i], NULL);
    }
    LOG(INFO) << ("Log test finished.");
//    printf("finish,%s", GetTimeString(SPACE_FORMAT).c_str());
    usleep(1000000);

    return 0;
}