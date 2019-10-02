/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbelinda <jbelinda@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/28 05:22:46 by jbelinda          #+#    #+#             */
/*   Updated: 2019/10/02 18:13:41 by jbelinda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include "get_next_line.h"

int			get_next_char(t_fdnode *fd)
{
}

t_schunk	*gnl_build_line(t_fdnode *fdnode)
{
}

char		*gnl_build_line(t_schunk *chunks)
{
}

int	get_next_line(int fd, char **line)
{
	static t_fdnode	*fdlist = NULL;
	t_fdnode		*fdnode;
	t_schunk		*chunks;

	if (fd < 0)
		return (GNL_ERR);
	if (!fdlist)
	{
		if (!(fdlist = (t_fdnode *)malloc(sizeof(t_fdnode))))
			return (GNL_ERR);
		if (!(fdlist->buf = (char *)malloc((size_t)BUFF_SIZE)))
		{
			free(fdlist);
			return (GNL_ERR);
		}
		fdlist->fd = fd;
		fdlist->status = GNL_OK;
		fdlist->i = 0;
		fdlist->bytes_in_buf = 0;
		fdlist->next = NULL;
	}
	fdnode = fdlist;
	while (fdnode && fdnode->fd != fd)
		fdnode = fdnode->next;
	if (!fdnode)
	{
		if (!(fdnode = (t_fdnode *)malloc(sizeof(t_fdnode))))
			return (GNL_ERR);
		if (!(fdnode->buf = (char *)malloc((size_t)BUFF_SIZE)))
		{
			free(fdnode);
			return (GNL_ERR);
		}
		fdnode->fd = fd;
		fdnode->status = GNL_OK;
		fdnode->i = 0;
		fdnode->bytes_in_buf = 0;
		fdnode->next = fdlist;
		fdlist = fdnode;
	}
	if (chunks = gnl_collect_line(fdnode))
		*line = gnl_build_line(chunks);
	return (fdnode->status);
}	
