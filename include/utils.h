#ifndef _UTILS_H
#define _UTILS_H
#include <cstdint>
#include <cstdbool>
#include <string>
#include <vector>

class UtilsSystem {
public:
	static bool FileExists(const char*);
	static int64_t Timestamp(void);
	static std::string CurrentDatetime(const char*);
	static void SplitString(std::string, const char, std::vector<std::string>&);
	static std::string FormatString(const char* format, ...);
};
#endif