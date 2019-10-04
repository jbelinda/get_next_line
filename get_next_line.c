/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbelinda <jbelinda@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/28 05:22:46 by jbelinda          #+#    #+#             */
/*   Updated: 2019/10/03 23:48:49 by jbelinda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include "get_next_line.h"

int			gnl_get_next_char(t_fdnode *fd)
{
	return (fd->fd);
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
