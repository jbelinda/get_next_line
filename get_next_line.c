/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbelinda <jbelinda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/28 05:22:46 by jbelinda          #+#    #+#             */
/*   Updated: 2019/10/14 04:58:37 by jbelinda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <unistd.h>
#include "get_next_line.h"

/*
** Get next character from `fd', implementing buffered input
** Assigns readed character to `c' and return GNL_OK or
** GNL_ERR on file error or GNL_EOF on end of file.
** TODO: In case of ERR/EOF removes `fd' from `fdlist'
*/

static int		gnl_getchar(u_char *c, t_fdnode *fd, t_list **fdlist)
{
	t_list *cur;
	t_list *prev;

	if (fd->i == fd->bytes_in_buf)
	{
		if ((fd->bytes_in_buf = read(fd->fd, fd->buf, BUFF_SIZE)) <= 0)
		{
			cur = *fdlist;
			prev = cur;
			while (cur->content != (void *)fd)
			{
				prev = cur;
				cur = cur->next;
			}
			if (prev == cur)
				*fdlist = cur->next;
			else
				prev->next = cur->next;
			ft_memdel(&cur->content);
			ft_memdel((void **)&cur);
			return (fd->bytes_in_buf == 0 ? GNL_EOF : GNL_ERR);
		}
		else
			fd->i = 0;
	}
	*c = fd->buf[fd->i++];
	return (GNL_OK);
}

/*
** Lookups for fd' in `fdlist', creates new node if `fd' not exist
*/

static t_fdnode	*gnl_fd_lookup(t_list **fdl, int fd)
{
	t_list			*iter;
	t_fdnode		*sfd;

	iter = *fdl;
	while (iter)
	{
		if (((t_fdnode *)(iter->content))->fd == fd)
			return ((t_fdnode *)(iter->content));
		iter = iter->next;
	}
	if ((sfd = (t_fdnode *)ft_memalloc(sizeof(t_fdnode))))
	{
		sfd->fd = fd;
		if ((iter = ft_lstnew((const void *)sfd, sizeof(t_fdnode))))
		{
			ft_lstadd(fdl, iter);
			ft_memdel((void **)&sfd);
			return ((t_fdnode *)(iter->content));
		}
		else
			ft_memdel((void**)&sfd);
	}
	return (NULL);
}

static char		*gnl_build_ln(char *s1, size_t n1, u_char *s2, size_t n2)
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

static int		gnl_get_line(t_list **fdl, t_fdnode *fd, char **ln)
{
	int		st;
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
	return (st);
}

/*
** Read string from `fd'. assigns its address to `*ln'
** returns GNL_OK on success, GNL_ERR on error, GNL_EOF on EOF
*/

int				get_next_line(int fd, char **ln)
{
	static t_list	*fdl = NULL;
	t_fdnode		*fdn;

	if (fd < 0 || !ln || read(fd, NULL, 0) || !(fdn = gnl_fd_lookup(&fdl, fd)))
		return (GNL_ERR);
	return (gnl_get_line(&fdl, fdn, ln));
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
