//
// Created by hapYP on 8/23/22.
//

#ifndef STANDARD_PROJECT_UTILITY_H
#define STANDARD_PROJECT_UTILITY_H

#include <stdio.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>


/**
* @brief: Set the buffer to zero for stdout.
* @param: none
* @retType: int
* @retVal: 0: success, non-zero: fail
* @bug:
**/
extern int SetStdoutNoBuffer();

/**
* @brief: Print the information of stack.
* @param:
* @retType: int
* @retVal: 0: success, non-zero: fail
* @bug:
**/
extern int PrintfBacktrace();

/**
* @brief: Get the name of the process.
* @param:
* @retType: std::string
* @retVal: The name of the process.
* @bug:
**/
extern std::string GetNameOfProcess();

#endif //STANDARD_PROJECT_UTILITY_H
