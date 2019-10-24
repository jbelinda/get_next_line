/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbelinda <jbelinda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/28 05:22:46 by jbelinda          #+#    #+#             */
/*   Updated: 2019/10/24 04:01:08 by jbelinda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <unistd.h>
#include "libft.h"
#include "get_next_line.h"

static int		gnl_validate_fd(int fd, t_fds *fds)
{
	void	*p;

	if (fd < 0 || read(fd, NULL, 0))
		return (GNL_ERR);
	if (fd > fds->fd_max)
	{
		p = ft_memrealloc(fds->fda, PTR_SZ * (fds->fd_max + 1), PTR_SZ * (fd + 1));
		if (p)
		{
			fds->fda = p;
			fds->fd_max = fd;
		}
		else
			return (GNL_ERR);
	}
	if (fds->fda[fd] == NULL)
		if ((fds->fda[fd] = (t_fdn *)ft_memalloc(FDN_SZ)))
			fds->fd_count++;
	return (fds->fda[fd] ? GNL_OK : GNL_ERR);
}

/*
** Get next character from `fd', implementing buffered input
** Assigns readed character to `c' and return GNL_OK or
** GNL_ERR on file error or GNL_EOF on end of file.
** TODO: In case of ERR/EOF removes `fd' from `fdlist'
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

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"

static char		*gnl_build_ln(char *s1, size_t n1, char *s2, size_t n2)
{
	char	*s;

	s = (char *)ft_memjoin(s1, n1, s2, n2);
	ft_memdel((void **)&s1);
	return (s);
}

#pragma clang diagnostic pop

/*
** Reads `fdnode' associated fd char by char until '\n', EOF, input error
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
	f->l = 0;
	f->ci = 0;
	while ((status = gnl_getchar(&c, fd, f)) != GNL_ERR)
	{
		if (status == GNL_EOF || c == '\n')
		{
			f->chunk[f->ci] = '\0';
			*ln = (char *)ft_memjoin(f->line, f->l, f->chunk, f->ci + 1);
			ft_memdel((void **)&f->line);
			status = f->ci ? GNL_OK : status;
			break ;
		}
		if (f->ci == (CHUNK_SIZE - 1))
		{
			f->chunk[f->ci] = '\0';
			tmp = f->line;
			f->line = (char *)ft_memjoin(f->line, f->l, f->chunk, f->ci + 1);
			ft_memdel((void **)&tmp);
			f->l += f->ci;
			f->ci = 0;
		}
		f->chunk[f->ci++] = c;
	}
	return (status);
}

/*
** Read string from `fd'. assigns its address to `*ln'
** returns GNL_OK on success, GNL_ERR on error, GNL_EOF on EOF
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