#include "masterlist.h"

void usage(char **argv)
{
	printf("usage : %s [options] file\nwhere options are\n", argv[0]);
	printf(" %-20s export all certificates in separate files\n", "--export-all");
	printf(" %-20s export masterlist signer certificate\n", "--export-signer");
	printf(" %-20s export CSCA\n", "--export-CSCA");
	printf(" %-20s print infos of all certificates\n", "--print-infos");
	printf(" %-20s print validity of masterlist signer\n", "--print-validity");
	exit(0);
}

void parseParams(int argc, char **argv, ARGS **params)
{
	(*params)->file = NULL;
	(*params)->options = 0;

	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "--export-all") == 0)
			(*params)->options |= EXPORT_ALL;
		else if (strcmp(argv[i], "--export-signer") == 0)
			(*params)->options |= EXPORT_SIGNER;
		else if (strcmp(argv[i], "--export-CSCA") == 0)
			(*params)->options |= EXPORT_CSCA;
		else if (strcmp(argv[i], "--print-infos") == 0)
			(*params)->options |= PRINT_INFOS;
		else if (strcmp(argv[i], "--print-validity") == 0)
			(*params)->options |= PRINT_VALIDITY;
		else if (argv[i][0] == '-')
		{
			printf("unknown option %s\n", argv[i]);
			usage(argv);
		}
		else if ((*params)->file == NULL)
			(*params)->file = argv[i];
	}

}

int main(int argc, char **argv)
{
	if (argc < 3)
		usage(argv);

	ARGS *params =  malloc(sizeof(ARGS));
	parseParams(argc, argv, &params);

	FILE *f;
	if ((f = fopen(params->file, "rb")) == NULL)
	{
		perror(NULL);
		exit(0);
	}
	fseek(f, 0, SEEK_END);
	long len = ftell(f);
	fseek(f, 0, SEEK_SET);
	unsigned char *masterList = malloc(len + 1);
	fread(masterList, 1, len, f);
	fclose(f);

	if (params->options & EXPORT_ALL)
		exportMasterlist(masterList, len);
	if (params->options & (EXPORT_CSCA | EXPORT_SIGNER))
		exportSigner(masterList, len, params);
	if (params->options & PRINT_INFOS)
		printInfos(masterList, len);
	if (params->options & PRINT_VALIDITY)
		printValidity(masterList, len);

	free(params);
	return (0);
}