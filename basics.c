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

unsigned char	*ft_strsub(unsigned char const *s, unsigned int start, size_t len)
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
