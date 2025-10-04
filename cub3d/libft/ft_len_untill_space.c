/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_len_untill_space.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thdaib <thdaib@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 15:09:58 by thdaib            #+#    #+#             */
/*   Updated: 2025/10/04 15:11:02 by thdaib           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/libft.h"

int	ft_is_space(char c)
{
	return (c == 32 || (c >= 9 && c <= 13));
}

void	skip_spaces(char *str, int *i)
{
	while (ft_is_space(str[*i]))
		*i += 1;
}

int	ft_len_untill_space(char *str)
{
	int	i;

	i = 0;
	while (str[i] && !ft_is_space(str[i]))
		i++;
	return (i);
}
