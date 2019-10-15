/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbelinda <jbelinda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/28 05:24:25 by jbelinda          #+#    #+#             */
/*   Updated: 2019/10/15 02:11:19 by jbelinda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <limits.h>
# include <stddef.h>
# include <sys/types.h>

# ifndef BUFF_SIZE
#  define BUFF_SIZE (4096)
# elif BUFF_SIZE <=0
#  error Wrong BUFF_SIZE value
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

# define FDN_SZ (sizeof(t_fdn))
# define FDA_INIT_SZ (8)
# define FDA_INC (10)

typedef struct	s_fdn {
	size_t		bytes_in_buf;
	size_t		i;
	char		*buf[BUFF_SIZE];
	size_t		ci;
	char		*chunk[CHUNK_SIZE];
	size_t		l;
	char		*line;
}				t_fdn;

typedef	struct	s_fds
{
	t_fdn		*fda;
	size_t		fd_count;
	size_t		fd_max;
}				t_fds;

int				get_next_line(const int fd, char **line);

#endif
