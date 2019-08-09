#include "masterlist.h"

void usage(char **argv)
{
	printf("usage : %s [options] file\nwhere options are\n", argv[0]);
	printf(" %-20s export all certificates in separate files\n", "--export-all");
	printf(" %-20s export masterlist signer certificate\n", "--export-signer");
	printf(" %-20s export CSCA\n", "--export-CSCA");
}

int main(int argc, char **argv)
{
	if (argc < 3)
	{
		usage(argv);
		return (0);
	}

	options *params = malloc(sizeof(options));
	params->file = NULL;

	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "--export-all") == 0)
			params->export_all = 1;
		else if (strcmp(argv[i], "--export-signer") == 0)
			params->export_signer = 1;
		else if (strcmp(argv[i], "--export-CSCA") == 0)
			params->export_CSCA = 1;
		else if (argv[i][0] == '-')
		{
			printf("unknown option %s\n", argv[i]);
			usage(argv);
			return(0);
		}
		else if (params->file == NULL)
			params->file = argv[i];
	}

	FILE *f;
	if ((f = fopen(params->file, "rb")) == NULL)
	{
		printf("%s: No such file or directory\n", params->file);
		return (0);
	}
	fseek(f, 0, SEEK_END);
	long len = ftell(f);
	fseek(f, 0, SEEK_SET);

	unsigned char *masterList = malloc(len + 1);
	fread(masterList, 1, len, f);
	fclose(f);

	if (params->export_all == 1)
		exportCertificates(masterList, len);

	return (0);
}