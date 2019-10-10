/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbelinda <jbelinda@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/28 05:22:46 by jbelinda          #+#    #+#             */
/*   Updated: 2019/10/10 04:19:07 by jbelinda         ###   ########.fr       */
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
	(void)fdlist;
	if (fd->i < fd->bytes_in_buf)
		return ((*c = fd->buf[fd->i++]) ? GNL_OK : GNL_OK);
	if ((fd->bytes_in_buf = read(fd->fd, fd->buf, BUFF_SIZE)) <= 0)
		return (fd->bytes_in_buf == 0 ? GNL_EOF : GNL_ERR);
	fd->i = 1;
	return ((*c = fd->buf[0]) ? GNL_OK : GNL_OK);
}

/*
** Lookups for fd' in `fdlist', creates new node if `fd' not exist
*/

static t_fdnode	*gnl_fd_lookup(t_list **fdlist, int fd)
{
	t_list			*iter;
	t_fdnode		*sfd;

	iter = *fdlist;
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
			ft_lstadd(fdlist, iter);
			ft_memdel((void **)&sfd);
			return ((t_fdnode *)(iter->content));
		}
		else
			ft_memdel((void**)&sfd);
	}
	return (NULL);
}

/*
** Reads `fdnode' associated fd char by char until '\n', EOF, input error
** and build line, terminated with '\0' instead of '\n'
** Returns GNL_OK on success, GNL_ERR on i/o error, GNL_EOF on EOF
*/

static int		gnl_get_line(t_list **fdlist, t_fdnode *fdnode, char **line)
{
	int		st;
	char	*tmp;
	u_char	c;

	fdnode->line = NULL;
	fdnode->l = 0;
	fdnode->ci = 0;
	while (((st = gnl_getchar(&c, fdnode, fdlist)) == GNL_OK) && (c != '\n'))
	{
		if (fdnode->ci < CHUNK_SIZE - 1)
			fdnode->chunk[fdnode->ci++] = c;
		else
		{
			tmp = fdnode->line;
			fdnode->line = ft_memjoin(fdnode->line, fdnode->l,
										fdnode->chunk, fdnode->ci + 1);
			fdnode->l += fdnode->ci + 1;
			ft_memdel((void **)&tmp);
			fdnode->ci = 0;
			fdnode->chunk[fdnode->ci++] = c;
		}
	}
	if (st != GNL_ERR)
	{
		tmp = fdnode->line;
		fdnode->line = ft_memjoin(fdnode->line, fdnode->l,
									fdnode->chunk, fdnode->ci + 1);
		ft_memdel((void **)&tmp);
		*line = fdnode->line;
	}
	return (st);
}

/*
** Read string from `fd'. assigns its address to `*line'
** returns GNL_OK on success, GNL_ERR on error, GNL_EOF on EOF
*/

int				get_next_line(int fd, char **line)
{
	static t_list	*fdlist = NULL;
	t_fdnode		*fdnode;

	if (fd < 0 || !line || read(fd, NULL, 0) ||
		!(fdnode = gnl_fd_lookup(&fdlist, fd)))
		return (GNL_ERR);
	return (gnl_get_line(&fdlist, fdnode, line));
}

int main(void)
{
	int fd;
	int i;
	char *l;

	fd = open("testgnl", O_RDONLY);
	while ((i = get_next_line(fd, &l)) == GNL_OK)
		free(l);
	close(fd);
	return (i);
}
