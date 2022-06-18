/*
 * @Author: Limer
 * @Date: 2022-05-19 12:38:24
 * @LastEditors: Limer
 * @LastEditTime: 2022-05-24 19:52:03
 * @Description:
 */
#include "util.h"
#include <errno.h>
#include <cstdio>

void errif(bool condition, const char* str) {
    if (condition == true) {
        perror(str);
    }
}