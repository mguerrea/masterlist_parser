#ifndef MASTERLIST_H
#define MASTERLIST_H

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef enum		options
{
	EXPORT_ALL = 1 << 0,
	EXPORT_SIGNER = 1 << 1,
	EXPORT_CSCA = 1 << 2,
	PRINT_INFOS = 1 << 3
}				options;


typedef struct args {
	options options;
	char *file;
}				args;


unsigned char	*ft_strstr(const unsigned char *str, const unsigned char *to_find, int len);
int getLength(unsigned char* ptr);
unsigned char	*strsub(unsigned char const *s, unsigned int start, size_t len);
char *getTime(unsigned char *ptr);
void exportMasterlist(unsigned char *masterList, int len);
void exportSigner(unsigned char *masterList, int len, args *params);
void printMemory(unsigned char *ptr, int len); // for debug purposes
void printInfos(unsigned char *masterList, int len);
void printPrettyTime(char *time);

#endif