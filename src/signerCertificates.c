#include "masterlist.h"

unsigned char *getSignerCertificates(unsigned char *masterList, int len)
{
	unsigned char *ptr = masterList;

	ptr += (ptr[1] < 128) ? 2 : 2 + ptr[1] - 128; // skip offset for sequence
	len = getLength(ptr);						  // get OID length
	ptr += (ptr[1] < 128) ? 2 : 2 + ptr[1] - 128; // skip offset for OID
	ptr += len;									  // skip OID content
	ptr += (ptr[1] < 128) ? 2 : 2 + ptr[1] - 128; // skip offset for [0]
	ptr += (ptr[1] < 128) ? 2 : 2 + ptr[1] - 128; // skip offset for sequence

	for (int i = 0; i < 3; i++) // skip first 3 elements
	{
		len = getLength(ptr);
		ptr += (ptr[1] < 128) ? 2 : 2 + ptr[1] - 128;
		ptr += len;
	}

	len = getLength(ptr);						  // get length of [0] (which contains the two certificates)
	ptr += (ptr[1] < 128) ? 2 : 2 + ptr[1] - 128; // skip offset

	return (strsub(ptr, 0, len)); // return the two certificates
}

static char *makeFileName(unsigned char *certificate, int len, int type)
{
	unsigned char countryNameOID[6] = {0x06, 0x03, 0x55, 0x04, 0x06, 0};

	unsigned char *ptr = ft_strstr(certificate, countryNameOID, len - 5);
	ptr += 7;
	char *fileName = malloc(sizeof(char) * 12);
	fileName[0] = 0;
	strncat(fileName, (char *)ptr, 2);
	if (type == EXPORT_SIGNER)
		strcat(fileName, "_MLS.cer");
	else
		strcat(fileName, "_CSCA.cer");
	return (fileName);
}

void exportCert(unsigned char *ptr, int type)
{
	int len = getLength(ptr);
	len += (ptr[1] < 128) ? 2 : 2 + ptr[1] - 128;
	unsigned char *certificate = strsub(ptr, 0, len);
	char *fileName = makeFileName(certificate, len, type);
	printf("file = %s\n", fileName);

	int fd = open(fileName, O_CREAT | O_RDWR, 0666);
	write(fd, certificate, len);
	free(fileName);
	free(certificate);
	close(fd);
}

void exportSigner(unsigned char *masterList, int len, ARGS *params)
{
	unsigned char *certList = getSignerCertificates(masterList, len);
	unsigned char *ptr = certList;

	if (params->options & EXPORT_SIGNER)
		exportCert(ptr, EXPORT_SIGNER);
	len = getLength(ptr);
	len += (ptr[1] < 128) ? 2 : 2 + ptr[1] - 128;
	ptr += len;
	if (params->options & EXPORT_CSCA && *ptr)
		exportCert(ptr, EXPORT_CSCA);
	else if (*ptr == 0)
		printf("CSCA is not included in the masterlist\n");
	free(certList);
}

void printValidity(unsigned char *masterList, int len)
{
	unsigned char *certList = getSignerCertificates(masterList, len);
	unsigned char *ptr = certList;

	TIME *time = malloc(sizeof(TIME));
	getTime(certList, time);
	printf("%-20s", "Not valid before: ");
	printPrettyTime(time->begin);
	printf("%-20s", "Not valid after: ");
	printPrettyTime(time->end);
	free(time);
}