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

void getTime(unsigned char *ptr, TIME *time)
{
	int len;

	for (int i = 0; i < 2; i++) // skip offset for the two first sequences
		ptr += (ptr[1] < 128) ? 2 : 2 + ptr[1] - 128;

	for (int i = 0; i < 4; i++) // skip the four first elements
	{
		len = getLength(ptr);
		len += (ptr[1] < 128) ? 2 : 2 + ptr[1] - 128;
		ptr += len;
	}
	ptr += 4; // skip offsets
	time->begin[0] = 0;
	time->end[0] = 0;
	strncat(time->begin, (char *)ptr, 12);
	ptr += 15; // go to next
	strncat(time->end, (char *)ptr, 12);
}

char *getSerialNumber(unsigned char *ptr)
{
	ptr += (ptr[1] < 128) ? 2 : 2 + ptr[1] - 128; // skip offset for first sequence
	ptr += (ptr[1] < 128) ? 2 : 2 + ptr[1] - 128; // skip offset for second sequence
	ptr += 5; // skip version

	int len = getLength(ptr);
	ptr += (ptr[1] < 128) ? 2 : 2 + ptr[1] - 128; // skip offset

	char str[60];
	for (int i = 0; i < len; i++)
		sprintf(str + i * 2, "%02x", ptr[i]);
	long num = strtol(str, NULL, 16);
	sprintf(str, "%ld", num);
	return(strdup(str));
}