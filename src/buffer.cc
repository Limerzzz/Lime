/*
 * @Author: Limer
 * @Date: 2022-06-01 13:12:34
 * @LastEditors: Limer
 * @LastEditTime: 2022-06-16 13:11:20
 * @Description:
 */

#include "buffer.h"
#include <iostream>

Buffer::Buffer() { str_.clear(); }
Buffer::~Buffer() { str_.clear(); }

void Buffer::append(const char* str) { str_.append(str); }

ssize_t Buffer::size() { return str_.length(); }

void Buffer::clear() { str_.clear(); }

const char* Buffer::c_str() { return str_.c_str(); }

void Buffer::getline() { std::getline(std::cin, str_); }

void Buffer::SetBuffer(const char* str) { str_ = str; }