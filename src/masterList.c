#include "masterlist.h"

unsigned char countryNameOID[6] = {0x06, 0x03, 0x55, 0x04, 0x06, 0};

unsigned char *getCertificateList(unsigned char *masterList, int len)
{
	unsigned char masterListOID[9] = {0x06, 0x06, 0x67, 0x81, 0x08, 0x01, 0x01, 0x02, 0};
	unsigned char *ptr = ft_strstr(masterList, masterListOID, len - 8) + 8;

	for (int i = 0; i < 3; i++) // skip offsets to go at the beginning of the cert list
		ptr += (ptr[1] < 128) ? 2 : 2 + ptr[1] - 128;
	ptr += 3;

	len = getLength(ptr);
	ptr += (ptr[1] < 128) ? 2 : 2 + ptr[1] - 128;

	return (strsub(ptr, 0, len));
}

void exportMasterlist(unsigned char *masterList, int len)
{
	unsigned char *certList = getCertificateList(masterList, len);
	unsigned char *beginning = certList;

	while (*certList)
	{
		len = getLength(certList);
		len += (certList[1] < 128) ? 2 : 2 + certList[1] - 128;

		unsigned char *certificate = strsub(certList, 0, len);

		unsigned char *ptr = ft_strstr(certificate, countryNameOID, len - 5);
		ptr += 7;
		char fileName[30] = {ptr[0], ptr[1], '_', 0};
		TIME *time = malloc(sizeof(TIME));
		getTime(certificate, time);
		strcat(fileName, time->begin); // add timestamp to fileName
		strcat(fileName, "_CSCA.cer");
		printf("file = %s\n", fileName);

		int fd = open(fileName, O_CREAT | O_RDWR, 0666);

		write(fd, certificate, len);
		free(certificate);
		close(fd);
		free(time);
		certList += len;
	}
	free(beginning);
}

void printInfos(unsigned char *masterList, int len)
{
	unsigned char commonNameOID[6]= {0x06, 0x03, 0x55, 0x04, 0x03, 0};
	unsigned char *certList = getCertificateList(masterList, len);
	unsigned char *beginning = certList;

	while (*certList)
	{
		len = getLength(certList);
		len += (certList[1] < 128) ? 2 : 2 + certList[1] - 128;
		unsigned char *certificate = strsub(certList, 0, len);
		
		unsigned char *ptr = ft_strstr(certificate, countryNameOID, len - 5);
		ptr += 7;
		printf("%.2s ", ptr);
		ptr = ft_strstr(certificate, commonNameOID, len - 5);
		if (ptr)
		{
		ptr += 5;
		int CNlen = getLength(ptr);
		ptr += (ptr[1] < 128) ? 2 : 2 + ptr[1] - 128;
		printf("%.*s ", CNlen, ptr);
		}
		else
			printf("No common name ");
		TIME *time = malloc(sizeof(TIME));
		getTime(certificate, time);
		printPrettyTime(time->begin);
		free(time);
		free(certificate);
		certList += len;
	}
}