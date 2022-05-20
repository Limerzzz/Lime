/*
 * @Author: Limer
 * @Date: 2022-05-19 12:38:24
 * @LastEditors: Limer
 * @LastEditTime: 2022-05-19 12:39:42
 * @Description:
 */
#include "util.h"
#include "cstdio"

void errif(bool condition, const char* str) {
    if (condition == true) perror(str);
}