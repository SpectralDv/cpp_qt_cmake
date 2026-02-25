#ifndef INCLUDE_H
#define INCLUDE_H


#include <QMainWindow>
#include <QPushButton>
#include <QMessageBox>
#include <QPainter>

#include "ui_mainwindow.h"

#include <iostream>
#include <vector>
#include <unordered_map>
#include <pthread.h>
#include <unistd.h> //for usleep
#include <fcntl.h>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>  // для inet_pton, inet_ntop (если нужны)
#pragma comment(lib, "ws2_32.lib")  // для линковки с Winsock 
#else
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#endif






#endif