/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbelinda <jbelinda@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/28 05:24:25 by jbelinda          #+#    #+#             */
/*   Updated: 2019/09/29 20:06:37 by jbelinda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <limits.h>

# ifndef BUFF_SIZE
#  define BUFF_SIZE (4096)
# elif BUFF_SIZE > SSIZE_MAX
#  undef BUFF_SIZE
#  define BUFF_SIZE (4096)
# endif

# define GNL_OK (1)
# define GNL_FIN (0)
# define GNL_ERR (-1)

int	get_next_line(const int fd, char **line);

#endif
