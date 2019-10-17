/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbelinda <jbelinda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/28 05:22:46 by jbelinda          #+#    #+#             */
/*   Updated: 2019/10/17 13:27:22 by jbelinda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <unistd.h>
#include "libft.h"
#include "get_next_line.h"

/*
** Releases dynamic data associated with fd.
** Also releases all data if there are no active fds left
*/

static void		gnl_cleanup(t_fds *fdl, int fd)
{
	ft_memdel((void **)&(fdl->fda)[fd]);
	if (--(fdl->fd_count) == 0)
		ft_memdel((void **)&(fdl->fda));
}

/*
**
*/

static int		gnl_validate_fd(int fd, t_fds *fds)
{
	void	*p;

	if (fd < 0 || read(fd, NULL, 0))
		return (GNL_ERR);
	if (fd > fds->fd_max - 1)
	{
		p = ft_memrealloc(fds->fda, PTR_SZ * fds->fd_max, PTR_SZ * (fd + 1));
		if (p)
		{
			fds->fda = p;
			fds->fd_max = fd;
		}
		else
			return (GNL_ERR);
	}
	if (fds->fda[fd] == NULL)
		fds->fda[fd] = (t_fdn *)ft_memalloc(FDN_SZ);
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
		if ((fdn->bytes_in_buf = read(fd, fdn->buf, BUFF_SIZE)) <= 0)
			return (fdn->bytes_in_buf == 0 ? GNL_EOF : GNL_ERR);
		else
			fdn->i = 0;
	}
	*c = fdn->buf[fdn->i++];
	return (GNL_OK);
}

static char		*gnl_build_ln(char *s1, size_t n1, char *s2, size_t n2)
{
	char	*s;

	s = (char *)ft_memjoin(s1, n1, s2, n2);
	ft_memdel((void **)&s1);
	return (s);
}

/*
** Reads `fdnode' associated fd char by char until '\n', EOF, input error
** and build line, terminated with '\0' instead of '\n'
** Returns GNL_OK on success, GNL_ERR on i/o error, GNL_EOF on EOF
*/
static int		gnl_get_line(t_fds *fds, int fd, char **ln)
{
	int		st;
/*
	u_char	c;

	fd->line = NULL;
	fd->l = 0;
	fd->ci = 0;
	ft_bzero(fd->chunk, CHUNK_SIZE);
	while (((st = gnl_getchar(&c, fd, fdl)) == GNL_OK) && (c != '\n'))
		if (fd->ci < CHUNK_SIZE - 1)
			fd->chunk[fd->ci++] = c;
		else
		{
			fd->line = gnl_build_ln(fd->line, fd->l, fd->chunk, fd->ci + 1);
			if (!fd->line)
				return (GNL_ERR);
			fd->l += fd->ci;
			ft_bzero(fd->chunk, CHUNK_SIZE);
			fd->ci = 0;
			fd->chunk[fd->ci++] = c;
		}
	if (st != GNL_ERR && fd->ci)
	{
		st = GNL_OK;
		*ln = gnl_build_ln(fd->line, fd->l, fd->chunk, fd->ci + 1);
	}
*/
	return (st);
}
/*
** Read string from `fd'. assigns its address to `*ln'
** returns GNL_OK on success, GNL_ERR on error, GNL_EOF on EOF
*/

int		get_next_line(const int fd, char **ln)
{
	static t_fds	fdl = {NULL, 0, 0};

	if (!ln || gnl_validate_fd(fd, &fdl) != GNL_OK)
		return (GNL_ERR);
	return (gnl_get_line(&fdl, fd, ln));
}

int	main(void)
{
	int fd1, fd2;
	int s1, s2;
	char *l1, *l2;

	fd1 = open("testgnl1", O_RDONLY);
	fd2 = open("testgnl2", O_RDONLY);
	while (1)
	{
		s1 = get_next_line(fd1, &l1);
		s2 = get_next_line(fd2, &l2);
		if (s1 == GNL_OK)
		{
			ft_putnbr(fd1);
			ft_putstr(": ");
			ft_putendl(l1);
			free(l1);
		}
		else
			close(fd1);
		if (s2 == GNL_OK)
		{
			ft_putnbr(fd2);
			ft_putstr(": ");
			ft_putendl(l2);
			free(l2);
		}
		else
			close(fd2);
		if (s1 != GNL_OK && s2 != GNL_OK)
			break ;
	}
	return (0);
}
