/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strspn.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhadhadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 22:07:48 by yhadhadi          #+#    #+#             */
/*   Updated: 2024/08/21 22:08:26 by yhadhadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strcspn(const char *str, const char *reject)
{
	const char	*p_str = str;

	while (*p_str && !ft_strchr(reject, *p_str))
		++p_str;
	return (p_str - str);
}
