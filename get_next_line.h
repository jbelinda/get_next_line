/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbelinda <jbelinda@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/28 05:24:25 by jbelinda          #+#    #+#             */
/*   Updated: 2019/10/02 18:13:44 by jbelinda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <limits.h>
# include <stddef.h>
# include "libft.h"

# ifndef BUFF_SIZE
#  define BUFF_SIZE PAGE_SIZE
# elif BUFF_SIZE > SSIZE_MAX
#  undef BUFF_SIZE
#  define BUFF_SIZE PAGE_SIZE
# endif

#define CHUNK_SIZE	PAGE_SIZE

# define GNL_OK (1)
# define GNL_FIN (0)
# define GNL_ERR (-1)

typedef			struct s_fdnode	t_fdnode;
typedef			struct s_schunk	t_schunk;

struct			s_fdnode {
	int			status;
	int			fd;
	size_t		i;
	size_t		bytes_in_buf;
	char		*buf;
	t_fdnode	*next;
};

struct			s_schunk {
	char		*chunk;
	t_schunk	*next;
};


int	get_next_line(const int fd, char **line);

#endif
