/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbelinda <jbelinda@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/28 05:22:46 by jbelinda          #+#    #+#             */
/*   Updated: 2019/10/06 02:56:28 by jbelinda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include "get_next_line.h"

/*
** Get next character from `fd', implementing buffered input
** Returns readed character or GNL_ERR on file error or
** GNL_EOF on end of file;
*/

int			gnl_get_next_char(t_fdnode *fd)
{
	if (fd->i < fd->bytes_in_buf)
		return (fd->buf[fd->i++]);
	if ((fd->bytes_in_buf = read(fd->fd, fd->buf, BUFF_SIZE)) <= 0)
		return (fd->bytes_in_buf == 0 ? GNL_EOF : GNL_ERR);
	fd->i = 1;
	return (fd->buf[0]);
}

char		*gnl_get_line(t_fdnode *fdnode)
{
	return ((char *)(fdnode - fdnode));
}

/*
** Lookups for `fd' in `fdlist', creates new node if `fd' not exist
*/

static t_fdnode	*gnl_fd_lookup(fd)
{
	static t_list	*fdlist = NULL;
	t_list			*iter;
	t_fdnode		*sfd;

	iter = fdlist;
	while (iter)
	{
		if (((t_fdnode *)(iter->content))->fd == fd)
			return ((t_fdnode *)(iter->content));
		iter = iter->next;
	}
	if (!iter)
	{
		if (!(sfd = (t_fdnode *)ft_memalloc(sizeof(t_fdnode))))
		{
			sfd->fd = fd;
			sfd->status = GNL_OK;
			if ((iter = ft_lstnew((const void *)sfd, sizeof(t_fdnode))))
			{
				ft_lstadd(&fdlist, iter);
				ft_memdel((void **)&sfd);
			}
		}
	}
	return ((t_fdnode *)(iter->content));
}

/*
** Read string from `fd'. assigns its address to `*line'
** returns GNL_OK on success, GNL_ERR on error, GNL_EOF on EOF
*/

int				get_next_line(int fd, char **line)
{
	t_fdnode		*fdnode;

	if (fd < 0 || !line || !(fdnode = gnl_fd_lookup(fd)))
		return (GNL_ERR);
	*line = gnl_get_line(fdnode);
	return (fdnode->status);
}
