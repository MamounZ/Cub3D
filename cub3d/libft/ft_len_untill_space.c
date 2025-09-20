
#include "./includes/libft.h"

int ft_is_space(char c)
{
	return (c == 32 || (c >=9 && c <=13));
}
void skip_spaces(char *str,int *i)
{
	while (ft_is_space(str[*i]))
		*i += 1;
	
}
int ft_len_untill_space(char *str)
{
	int i;
	
	i = 0;
	
	while (str[i] && !ft_is_space(str[i]))
		i++;
	return(i);
	
}