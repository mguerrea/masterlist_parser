#include "masterlist.h"

int getLength(unsigned char *ptr)
{
	int len;
	char str[20];

	if (ptr[1] < 128)
		len = ptr[1];
	else
	{
		int lenLen = ptr[1] - 128;
		ptr += 2;
		for (int i = 0; i < lenLen; i++)
			sprintf(str + (i * 2), "%02x", ptr[i]);
		str[lenLen * 2] = '\0';
		len = strtol(str, NULL, 16);
	}
	return (len);
}

char *getTime(unsigned char *ptr)
{
	int len;

	for (int i = 0; i < 2; i++)
		ptr += (ptr[1] < 128) ? 2 : 2 + ptr[1] - 128;

	for (int i = 0; i < 4; i++)
	{
		len = getLength(ptr);
		len += (ptr[1] < 128) ? 2 : 2 + ptr[1] - 128;
		ptr += len;
	}
	ptr += 4;
	return((char *)ptr);
}