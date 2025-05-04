/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dfasius <dfasius@student.42.sg>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 16:26:41 by dfasius           #+#    #+#             */
/*   Updated: 2024/05/18 17:20:10 by dfasius          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	int	i;

	i = 0;
	while (lst)
	{
		lst = lst->next;
		i++;
	}
	return (i);
}
/*
#include <stdio.h>

int	main(void)
{
	t_list test5 = {.content = "hello5", .next = 0};
	t_list test4 = {.content = "hello4", .next = &test5};
	t_list test3 = {.content = "hello3", .next = &test4};
	t_list test2 = {.content = "hello2", .next = &test3};
	t_list test1 = {.content = "hello1", .next = &test2};

	printf("%d", ft_lstsize(&test1));
}
*/
