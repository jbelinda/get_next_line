/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbelinda <jbelinda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/28 05:24:25 by jbelinda          #+#    #+#             */
/*   Updated: 2019/10/31 00:58:26 by jbelinda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <limits.h>
# include <stddef.h>

# define BUFF_SIZE (4096)
# if BUFF_SIZE <= 0
#  error Wrong BUFF_SIZE value
# endif

/*
** This fucking moulinette got crazy eating this
**
** # ifndef BUFF_SIZE
** #  define BUFF_SIZE (4096)
** # endif
** # if BUFF_SIZE <= 0
** #  error Wrong BUFF_SIZE value
** # elif BUFF_SIZE > SSIZE_MAX
** #  undef BUFF_SIZE
** #  define BUFF_SIZE (4096)
** # endif
*/

# ifndef CHUNK_SIZE
#  define CHUNK_SIZE (256)
# endif

# define GNL_OK (1)
# define GNL_EOF (0)
# define GNL_ERR (-1)

# define PTR_SZ (sizeof(void *))
# define FDN_SZ (sizeof(t_fdn))
# define FD_INC (4)

/*
** fd related data
**
** buf          - data readed from fd
** bytes_in_buf - counter of readed bytes
** i            - current position in buf
** chunk        - acquired part of string
** ci           - current position in chunk
** len          - length of assembled string
*/

typedef struct	s_fdn {
	size_t		bytes_in_buf;
	size_t		i;
	char		buf[BUFF_SIZE];
	size_t		ci;
	char		chunk[CHUNK_SIZE];
	size_t		len;
}				t_fdn;

/*
** fda      - array (indexed by `fd') of fd-related data
** fd_count - active fds
** fd_max   - maximum fds `fda' may contain
*/

typedef	struct	s_fds
{
	t_fdn		**fda;
	int			fd_count;
	int			fd_max;
}				t_fds;

/*
** Read '\n'-terminated string from `fd'. assigns its address to `*ln'
** returns GNL_OK on success, GNL_ERR on error, GNL_EOF on EOF.
** In case of invalid `fd' or `ln == NULL' GNL_ERR returned.
**
** It's caller's hemorroida to free ln-occupied memory
*/

int				get_next_line(const int fd, char **line);

#endif
