#include "masterlist.h"

unsigned char	*ft_strstr(const unsigned char *str, const unsigned char *to_find, int len)
{
	int i;
	int j;

	i = 0;
	j = 0;
	while (i < len)
	{
		j = 0;
		while (str[i + j] == to_find[j])
		{
			if (to_find[j + 1] == '\0')
				return ((unsigned char *)str + i);
			j++;
		}
		i++;
	}
	return (0);
}

unsigned char	*strsub(unsigned char const *s, unsigned int start, size_t len)
{
	unsigned char	*str;
	size_t	i;

	if (s)
	{
		if (!(str = malloc(len + 1)))
			return (NULL);
		str[len] = 0;
		i = -1;
		while (++i < len)
			str[i] = s[start + i];
		return (str);
	}
	else
		return (NULL);
}

void printMemory(unsigned char *ptr, int len) // for debug purposes
{
	for (int i = 0; i < len; i++)
		printf("%x\t", ptr[i]);
	printf("\n");
}

void printPrettyTime(char *time)
{
	printf("%c%c/%c%c/%c%c ", time[4], time[5], time[2], time[3], time[0], time[1]);
	printf("%c%c:%c%c:%c%c\n", time[6], time[7], time[8], time[9], time[10], time[11]);
}