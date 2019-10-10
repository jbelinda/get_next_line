/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbelinda <jbelinda@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/28 05:24:25 by jbelinda          #+#    #+#             */
/*   Updated: 2019/10/10 03:40:20 by jbelinda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <limits.h>
# include <stddef.h>
# include <sys/types.h>
# include "libft.h"

# ifndef BUFF_SIZE
#  define BUFF_SIZE (4096)
# elif BUFF_SIZE > SSIZE_MAX
#  undef BUFF_SIZE
#  define BUFF_SIZE (4096)
# endif

# ifndef CHUNK_SIZE
#  define CHUNK_SIZE (4096)
# endif

# define GNL_OK (1)
# define GNL_EOF (0)
# define GNL_ERR (-1)

typedef	struct s_fdnode	t_fdnode;

struct		s_fdnode {
	int		fd;
	size_t	bytes_in_buf;
	size_t	i;
	u_char	buf[BUFF_SIZE];
	size_t	ci;
	u_char	chunk[CHUNK_SIZE];
	size_t	l;
	char	*line;
};

int			get_next_line(const int fd, char **line);

#endif
