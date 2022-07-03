
# Lime

## 项目描述
此项目是一个基于非阻塞 IO 和事件驱动设计的 C++ 网络编程库，基于 Reactor 模式设计，
原生支持多核多线程模式.

## 文件结构
src 包含所有的库相关文件
```
src/
├── CMakeLists.txt
├── acceptor.cc
├── buffer.cc
├── channel.cc
├── connection.cc
├── epoll.cc
├── event_loop.cc
├── include
│   ├── acceptor.h
│   ├── buffer.h
│   ├── channel.h
│   ├── connection.h
│   ├── epoll.h
│   ├── event_loop.h
│   ├── my_socket.h
│   ├── server.h
│   ├── thread_pool.h
│   └── util.h
├── my_socket.cc
├── server.cc
├── thread_pool.cc
└── util.cc
```
test 包含了一些测试代码，包括简单的服务器并发测试代码，服务器代码和客户端代码。
```
test/
├── CMakeLists.txt
├── client.cc
├── server.cc
└── thread_test.cc
```

## 编译测试

```
mkdir build && cd build
cmake ..
make
```
## 项目的不足
- 日志部分
- 信号部分