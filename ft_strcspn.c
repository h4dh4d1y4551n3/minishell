/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcspn.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhadhadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 22:07:49 by yhadhadi          #+#    #+#             */
/*   Updated: 2024/08/21 22:10:22 by yhadhadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strspn(const char *str, const char *accept)
{
	const char	*p_str = str;

	while (*p_str && ft_strchr(accept, *p_str))
		++p_str;
	return (p_str - str);
}
