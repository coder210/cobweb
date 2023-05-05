/****************************************************
Copyright (C): 2020-2021, lanchong.xyz/Ltd.
File name: cobweb_utils.c
Description: 通用方法实现
Author: ydlc
Version: 1.0
Date: 2021.4.22
History:
*****************************************************/

#include "cobweb.h"
#include <time.h>


char* 
cobweb_strsep(char** stringp, const char* delim){
	char* s;
	const char* spanp;
	int c, sc;
	char* tok;
	if ((s = *stringp) == NULL)
		return (NULL);
	for (tok = s;;) {
		c = *s++;
		spanp = delim;
		do {
			if ((sc = *spanp++) == c) {
				if (c == 0)
					s = NULL;
				else
					s[-1] = 0;
				*stringp = s;
				return (tok);
			}
		} while (sc != 0);
	}
}


char*
cobweb_read_file(const char* filename) {
	FILE* fp = fopen(filename, "rb");
	if (fp == NULL) {
		ccp_red_print("read config file failed");
		return NULL;
	}
	fseek(fp, 0, SEEK_END);
	int total_size = (int)ftell(fp);
	char* buffer = (char*)cobweb_malloc((total_size + 1) * sizeof(char));
	rewind(fp);
	size_t read_size = fread(buffer, 1, total_size, fp);
	buffer[read_size] = 0;
	fclose(fp);
	return buffer;
}

void
cobweb_append_file(const char* filename, const char* data) {
	FILE* fp = fopen(filename, "a+");
	if (fp != NULL) {
		fprintf(fp, "%s\n", data);
		fclose(fp);
	}
}

char*
cobweb_strdup(const char* str) {
	size_t sz = strlen(str);
	char* ret = (char*)cobweb_malloc(sz + 1);
	if (ret != NULL) {
		memcpy(ret, str, sz + 1);
	}
	return ret;
}

void
cobweb_write_short(uint8_t* buffer, short value) {
	buffer[0] = (value >> 8) & 0xff;
	buffer[1] = value & 0xff;
}

short
cobweb_read_short(uint8_t* buffer) {
	short r = (short)buffer[0] << 8 | (short)buffer[1];
	return r;
}

void
cobweb_write_int(uint8_t* buffer, int value) {
	buffer[0] = ((0xff000000 & value) >> 24);
	buffer[1] = ((0xff0000 & value) >> 16);
	buffer[2] = ((0xff00 & value) >> 8);
	buffer[3] = (0xff & value);
}

int
cobweb_read_int(uint8_t* buffer) {
	int r = buffer[3];
	r |= (int)buffer[2] << 8;
	r |= (int)buffer[1] << 16;
	r |= (int)buffer[0] << 24;
	return r;
}

// ms
int64_t
cobweb_timestamp(void) {
	long sec = 0;
	long usec = 0;
	ccp_timeofday(&sec, &usec);
	int64_t timestamp = sec * 1000 + usec / 1000;
	return timestamp;
}

struct datetime_t 
cobweb_currdate(void) {
	return ccp_datetime("%Y%m%d");
}

struct datetime_t
cobweb_currtime(void) {
	return ccp_datetime("%H:%M:%S");
}
