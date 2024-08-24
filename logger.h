/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhadhadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/24 07:51:01 by yhadhadi          #+#    #+#             */
/*   Updated: 2024/08/24 07:55:11 by yhadhadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGGER_H
# define LOGGER_H

# include "libft.h"

typedef struct s_logger
{
	int		(*log)(const char *, void *);
	void	*cntx;
}	t_logger;

#endif
