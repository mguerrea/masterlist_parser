#ifndef MASTERLIST_H
#define MASTERLIST_H

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct list
{
	char *content;
	struct list *next;
} LIST;

typedef enum options
{
	EXPORT_ALL = 1 << 0,
	EXPORT_SIGNER = 1 << 1,
	EXPORT_CSCA = 1 << 2,
	PRINT_INFOS = 1 << 3,
	PRINT_VALIDITY = 1 << 4
} OPTIONS;

typedef struct args
{
	OPTIONS options;
	char *file;
	LIST *cert;
} ARGS;

typedef struct time
{
	char begin[13];
	char end[13];
} TIME;

unsigned char *ft_strstr(const unsigned char *str, const unsigned char *to_find, int len);
int getLength(unsigned char *ptr);
unsigned char *strsub(unsigned char const *s, unsigned int start, size_t len);
void getTime(unsigned char *ptr, TIME *time);
void exportMasterlist(unsigned char *masterList, int len);
void exportSigner(unsigned char *masterList, int len, ARGS *params);
void printMemory(unsigned char *ptr, int len); // for debug purposes
void printInfos(unsigned char *masterList, int len);
void printPrettyTime(char *time);
void printValidity(unsigned char *masterList, int len);
void findCert(unsigned char *masterList, int len, ARGS *params);
LIST *lstnew(char *content);
void lstaddback(LIST **list, LIST *new);
void	lstdel(LIST **alst);
char *getSerialNumber(unsigned char *ptr);
int	ft_strncmp(const unsigned char *s1, const unsigned char *s2, size_t n);

#endif