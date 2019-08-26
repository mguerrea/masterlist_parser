#include "masterlist.h"

void usage(char **argv, char *error)
{
	if (error)
		printf("%s\n", error);
	printf("usage : %s -in file [options]\nwhere options are\n", argv[0]);
	printf(" %-20s input format (DER or PEM (default))\n", "-inform fmt");
	printf(" %-20s find if certs (DER) are in masterlist\n", "-find file(s)");
	printf(" %-20s export all certificates in separate files\n", "--export-all");
	printf(" %-20s export masterlist signer certificate\n", "--export-signer");
	printf(" %-20s export CSCA which signed the masterlist\n", "--export-CSCA");
	printf(" %-20s print infos of all certificates\n", "--print-infos");
	printf(" %-20s print validity of masterlist signer\n", "--print-validity");

	exit(0);
}

void getArgs(int i, char **argv, ARGS **params)
{
	LIST *tmp = NULL;
	while (argv[i] && argv[i][0] != '-')
	{
		LIST *new = lstnew(argv[i]);
		lstaddback(&tmp, new);
		i++;
	}
	(*params)->cert = tmp;
	if ((*params)->cert == NULL)
		usage(argv, "-find: missing argument");
}

void parseParams(int argc, char **argv, ARGS **params)
{
	(*params)->file = NULL;
	(*params)->options = 0;
	(*params)->cert = NULL;
	(*params)->format = NULL;

	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "-in") == 0)
		{
			if (argv[i + 1] && argv[i + 1][0] != '-')
				(*params)->file = argv[i + 1];
			else
				usage(argv, "-in: missing argument");
		}
		else if (strcmp(argv[i], "-inform") == 0)
		{
			if (argv[i + 1] && argv[i + 1][0] != '-')
				(*params)->format = argv[i + 1];
			else
				usage(argv, "-inform: missing argument");
		}
		else if (strcmp(argv[i], "--export-all") == 0)
			(*params)->options |= EXPORT_ALL;
		else if (strcmp(argv[i], "--export-signer") == 0)
			(*params)->options |= EXPORT_SIGNER;
		else if (strcmp(argv[i], "--export-CSCA") == 0)
			(*params)->options |= EXPORT_CSCA;
		else if (strcmp(argv[i], "--print-infos") == 0)
			(*params)->options |= PRINT_INFOS;
		else if (strcmp(argv[i], "--print-validity") == 0)
			(*params)->options |= PRINT_VALIDITY;
		else if (strcmp(argv[i], "-find") == 0)
			getArgs(i + 1, argv, params);
		else if (argv[i][0] == '-')
			usage(argv, "unknown option");
	}
	if ((*params)->file == NULL)
		usage(argv, "missing file");
}

long readDER(char *file, unsigned char **masterList, char **argv)
{
	FILE *f;
	if ((f = fopen(file, "rb")) == NULL)
	{
		perror(NULL);
		exit(0);
	}
	fseek(f, 0, SEEK_END);
	long len = ftell(f);
	fseek(f, 0, SEEK_SET);
	*masterList = malloc(len + 1);
	fread(*masterList, 1, len, f);
	fclose(f);
	if ((*masterList)[0] != 48)
		usage(argv, "wrong format");
	return len;
}

long readPEM(char *file, unsigned char **masterList, char **argv)
{
	long finalLen;
	FILE *f;
	char *str;
	if ((f = fopen(file, "r")) == NULL)
	{
		perror(NULL);
		exit(0);
	}
	fseek(f, 0, SEEK_END);
	long len = ftell(f);
	fseek(f, 0, SEEK_SET);
	str = malloc(len + 1);
	fread(str, 1, len, f);
	fclose(f);
	if (str[1] < 0)
		usage(argv, "wrong format");
	if (strncmp("pkdMasterListContent: ", str, 22) == 0)
	{
		str += 22;
		len -= 22;
	}
	*masterList = base64_decode(str, len, &finalLen);
	return(finalLen);
}

int main(int argc, char **argv)
{
	if (argc == 1)
		usage(argv, NULL);
	ARGS *params =  malloc(sizeof(ARGS));
	parseParams(argc, argv, &params);

	long len;
	unsigned char *masterList;

	if (!params->format || !strcmp(params->format, "pem") || !strcmp(params->format, "PEM"))
		len = readPEM(params->file, &masterList, argv);
	else if (!strcmp(params->format, "der") || !strcmp(params->format, "DER"))
		len = readDER(params->file, &masterList, argv);
	else
		usage(argv, "invalid format");
	
	if (params->options & EXPORT_ALL)
		exportMasterlist(masterList, len);
	if (params->options & (EXPORT_CSCA | EXPORT_SIGNER))
		exportSigner(masterList, len, params);
	if (params->options & PRINT_INFOS)
		printInfos(masterList, len);
	if (params->options & PRINT_VALIDITY)
		printValidity(masterList, len);
	while (params->cert)
	{
		findCert(masterList, len, params);
		params->cert = params->cert->next;
	}

	lstdel(&(params->cert));
	free(params);
	return (0);
}