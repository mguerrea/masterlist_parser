#ifndef MASTERLIST_H
#define MASTERLIST_H

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct options {
	int export_all;
	int export_signer;
	int export_CSCA;
	char *file;
}				options;

unsigned char	*ft_strstr(const unsigned char *str, const unsigned char *to_find, int len);
int getLength(unsigned char* ptr);
unsigned char	*ft_strsub(unsigned char const *s, unsigned int start, size_t len);
char *getTime(unsigned char *ptr);
void exportCertificates(unsigned char *masterList, int len);

#endif