/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbelinda <jbelinda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/28 05:22:46 by jbelinda          #+#    #+#             */
/*   Updated: 2019/10/27 02:27:41 by jbelinda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <unistd.h>
#include "libft.h"
#include "get_next_line.h"

/*
** Validates `fd', dynamically reallocate array for `fd'-related data
*/

static int		gnl_validate_fd(int fd, t_fds *fds)
{
	if (fd < 0 || read(fd, NULL, 0))
		return (GNL_ERR);
	if (fd > fds->fd_max)
	{
		if ((fds->fda = ft_memrealloc(fds->fda,
			(fds->fd_max + 1) * PTR_SZ,
			(fd + 1) * PTR_SZ)))
			fds->fd_max = fd;
		else
			return (GNL_ERR);
	}
	if (fds->fda[fd] == NULL)
	{
		if ((fds->fda[fd] = (t_fdn *)ft_memalloc(FDN_SZ)))
			fds->fd_count++;
	}
	return (fds->fda[fd] ? GNL_OK : GNL_ERR);
}

/*
** Get next character from `fd', implementing buffered input
** Assigns readed character to `c' and return GNL_OK or
** GNL_ERR on file error or GNL_EOF on end of file.
*/

static int		gnl_getchar(char *c, int fd, t_fdn *fdn)
{
	if (fdn->i == fdn->bytes_in_buf)
	{
		fdn->i = 0;
		if ((fdn->bytes_in_buf = read(fd, fdn->buf, BUFF_SIZE)) <= 0)
			return (fdn->bytes_in_buf == 0 ? GNL_EOF : GNL_ERR);
	}
	*c = fdn->buf[fdn->i++];
	return (GNL_OK);
}

/*
** Reads `fd' char by char until '\n', EOF, input error
** and build line, terminated with '\0' instead of '\n'
** Returns GNL_OK on success, GNL_ERR on i/o error, GNL_EOF on EOF
*/

static int		gnl_get_line(t_fds *fds, int fd, char **ln)
{
	int		status;
	char	c;
	t_fdn	*f;
	char	*tmp;

	f = fds->fda[fd];
	f->len = 0;
	f->ci = 0;
	while ((status = gnl_getchar(&c, fd, f)) != GNL_ERR)
	{
		if (status == GNL_EOF || c == '\n' || f->ci == (CHUNK_SIZE - 1))
		{
			f->chunk[f->ci] = '\0';
			tmp = *ln;
			*ln = (char *)ft_memjoin(*ln, f->len, f->chunk, f->ci + 1);
			if (f->len)
				ft_memdel((void **)&tmp);
			if (status == GNL_EOF || c == '\n')
				break ;
			f->len += f->ci;
			f->ci = 0;
		}
		f->chunk[f->ci++] = c;
	}
	return ((status != GNL_ERR && f->ci) ? GNL_OK : status);
}

/*
** Read '\n'-terminated string from `fd'. assigns its address to `*ln'
** returns GNL_OK on success, GNL_ERR on error, GNL_EOF on EOF
** Relases memory for fd-related data on all fds EOF/ERR
*/

int				get_next_line(const int fd, char **ln)
{
	static t_fds	fdl = {NULL, 0, -1};
	int				status;

	if (!ln || gnl_validate_fd(fd, &fdl) != GNL_OK)
		return (GNL_ERR);
	status = gnl_get_line(&fdl, fd, ln);
	if (status != GNL_OK)
	{
		ft_memdel((void **)&(fdl.fda[fd]));
		if (--fdl.fd_count == 0)
			ft_memdel((void **)&(fdl.fda));
	}
	return (status);
}
