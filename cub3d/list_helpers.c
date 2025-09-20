#include "./cub3d.h"

void assign_configs(char *configs[6])
{
	configs[0] = "NO";
	configs[1] = "SO";
	configs[2] = "EA";
	configs[3] = "WE";
	configs[4] = "F";
	configs[5] = "C";
}
t_list	*create_configs_list(t_list **head)
{

	int	counter;
	char *configs[6];
	t_list *newnode;
	char *value;

	counter = 0;
	assign_configs(configs);
	while (counter < 6)
	{
		value = ft_strdup(configs[counter]);
		if(!value)
			return (NULL);
		newnode = ft_lstnew(value);
		if(!newnode)
		{
			ft_lstclear(head,free);
			free(value);
			return (NULL);
		}
		ft_lstadd_back(head,newnode);
		counter++;
	}
	return(*head);
}
int remove_id_from_list(t_list **head, char *value)
{
	t_list *temp;
	t_list *my_prev_node;
	temp = *head;

	my_prev_node = NULL;
	while (temp && ft_strcmp(temp->content ,value) != 0)
	{
		my_prev_node = temp;
		temp = temp->next;
	}
	if (temp)
	{
		if(my_prev_node)
			my_prev_node->next = temp->next;
		else
			*head = temp->next;
		ft_lstdelone(temp,free);
		return (0);
	}
	return(1);//node not found
}

char *copy_identifire(char *str)
{
	int	i;
	char *value;

	value = malloc(ft_len_untill_space(str) +1);
	if (!value)
		return (NULL);
	i = 0;
	while (str[i] && !ft_is_space(str[i]))
	{
		value[i] = str[i];
		i++;
	}
	value[i] = '\0';
	return (value);
}
