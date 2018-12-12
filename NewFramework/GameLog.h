#pragma once
#include <Windows.h>
#include <iostream>
#include <sstream>
#include <time.h>

//Show and save game logs
#define GAMELOG(s,...) printLog(s , ##__VA_ARGS__)

void printLog(const LPCSTR s, ...);