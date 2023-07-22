/****************************************************
Copyright (C): 2020-2021, lanchong.xyz/Ltd.
File name: cobweb_utils.cpp
Description: 通用方法实现
Author: ydlc
Version: 1.0
Date: 2021.4.22
History:
*****************************************************/
#include "utils.h"
#include <chrono>
#include <string>
#include <ctime>
#include <sstream>
#include <cstdarg>

bool 
UtilsSystem::FileExists(const char* filename) {
	if (FILE* file = fopen(filename, "r")) {
		fclose(file);
		return true;
	}
	else {
		return false;
	}
}

int64_t 
UtilsSystem::Timestamp(void) {
	std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> millis
		= std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
	time_t total_milliseconds = millis.time_since_epoch().count();
	return total_milliseconds;
}

std::string
UtilsSystem::CurrentDatetime(const char* format) {
	if (format != NULL) {
		std::string stime;
		std::stringstream strtime;
		std::time_t currenttime = std::time(0);
		char tAll[255];
		std::strftime(tAll, sizeof(tAll), format, std::localtime(&currenttime));
		strtime << tAll;
		stime = strtime.str();
		return stime;
	}
	else {
		return "";
	}
}

void
UtilsSystem::SplitString(std::string str, const char split, std::vector<std::string>& substr_vector) {
	int index = -1;
	while ((index = str.find(split)) != -1) {
		std::string substr = str.substr(0, index);
		substr_vector.push_back(substr);
		str = str.substr(index + 1);
	}
	if (substr_vector.size() > 0) {
		substr_vector.push_back(str);
	}
}

std::string 
UtilsSystem::FormatString(const char* format, ...) {
	std::string str;
	va_list ap;
	va_start(ap, format);
	int length = _vscprintf(format, ap) +1;
	std::vector<char> vectorChars(length);
	_vsnprintf(vectorChars.data(), length, format, ap);
	str.assign(vectorChars.data());
	va_end(ap);
	return str;
}

