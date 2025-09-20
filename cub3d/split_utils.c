#include "./cub3d.h"

void	ft_free_split1(char **split)
{
	int	i;

	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}
int ft_split_len(char **split)
{
	int	i;

	i = 0;
	while (split[i])
		i++;
	return (i);
}

void	*ft_calloc1(size_t nitem, size_t size)
{
	size_t			totalsize;
	size_t			i;
	unsigned char	*ptr;

	totalsize = size * nitem;
	i = 0;
	ptr = (unsigned char *)malloc(totalsize);
	if (!ptr)
		return (NULL);
	while (i < totalsize)
	{
		ptr[i] = 0;
		i++;
	}
	return ((void *)ptr);
}

char *ft_strjoin1(char *left, char *buff)
{
    int i, j;
    char *temp;

    if (!left && !buff)
        return (NULL);
    if (!left)
        left = ft_calloc1(1, sizeof(char));

    temp = malloc(sizeof(char) * (ft_strlen(left) + ft_strlen(buff) + 1));
    if (!temp)
        return (NULL);

    i = -1;
    while (left[++i])
        temp[i] = left[i];
    j = -1;
    while (buff[++j])
        temp[i++] = buff[j];
    temp[i] = '\0';

    free(left); // free only old left
    return temp;  // caller must free buff (line) after join
}
