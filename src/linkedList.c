#include "masterlist.h"

LIST	*lstnew(char *content)
{
	LIST *new;

	if (!(new = (LIST *)malloc(sizeof(LIST))))
		return (NULL);
	if (content)
	{
		if (!(new->content = strdup(content)))
		{
			free(new);
			return (NULL);
		}
	}
	else
		new->content = NULL;
	new->next = NULL;
	return (new);
}

void	lstaddback(LIST **list, LIST *new)
{
	LIST *temp;

	if (!list)
		return ;
	temp = *list;
	if (!temp)
	{
		*list = new;
		return ;
	}
	while (temp->next)
		temp = temp->next;
	temp->next = new;
}

void	lstdel(LIST **alst)
{
	if (*alst)
	{
		if ((*alst)->next)
			lstdel(&(*alst)->next);
		free((*alst)->content);
		free(*alst);
		*alst = NULL;
	}
}
