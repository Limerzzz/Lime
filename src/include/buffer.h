/*
 * @Author: Limer
 * @Date: 2022-06-01 13:09:28
 * @LastEditors: Limer
 * @LastEditTime: 2022-06-16 13:10:48
 * @Description:
 */
#pragma once

#include <sys/types.h>
#include <string>
class Buffer {
   public:
    Buffer();
    ~Buffer();
    void append(const char*);
    ssize_t size();
    void clear();
    const char* c_str();
    void getline();
    void SetBuffer(const char*);

   private:
    std::string str_;
};