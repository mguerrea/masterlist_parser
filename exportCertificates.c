#include "masterlist.h"

unsigned char *getCertificateList(unsigned char *masterList, int len)
{
	unsigned char OID[9] = {0x06, 0x06, 0x67, 0x81, 0x08, 0x01, 0x01, 0x02, 0};
	unsigned char *ptr = ft_strstr(masterList, OID, len - 8) + 8;

	for (int i = 0; i < 3; i++)
		ptr += (ptr[1] < 128) ? 2 : 2 + ptr[1] - 128;
	ptr += 3;

	len = getLength(ptr);

	if (ptr[1] < 128)
		ptr += 2;
	else
		ptr += 2 + ptr[1] - 128;
	/*	for (int i = 0; i < 10; i++)
		printf("%x\t", ptr[i]);
	printf("\n");*/

	return (ft_strsub(ptr, 0, len));
}

void exportCertificates(unsigned char *masterList, int len)
{
	unsigned char *certList = getCertificateList(masterList, len);

	while (*certList)
	{
		len = getLength(certList);
		len += (certList[1] < 128) ? 2 : 2 + certList[1] - 128;

		unsigned char *certificate = ft_strsub(certList, 0, len);

		unsigned char countryNameOID[6] = {0x06, 0x03, 0x55, 0x04, 0x06, 0};
		unsigned char *ptr = ft_strstr(certificate, countryNameOID, len - 5);
		ptr += 7;
		char fileName[30] = {ptr[0], ptr[1], '_', 0};
		char *time = getTime(certificate);
		strncat(fileName, time, 12);
		strcat(fileName, "_CSCA.cer");
		printf("file = %s\n", fileName);

		int fd = open(fileName, O_CREAT | O_RDWR, 0666);

		write(fd, certificate, len);
		free(certificate);
		close(fd);
		certList += len;
	}
}