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

int			gnl_get_next_char(t_fdnode *fd)
{
}

t_schunk	*gnl_collect_line(t_fdnode *fdnode)
{
	t_schunk	*head;
	t_schunk	*current;
	int			ch;

	if (!(head = (t_schunk*)malloc(sizeof(t_schunk))))
		return (NULL);
	if (!(head->chunk = (char *)malloc(CHUNK_SIZE)))
	{
		free(head);
		return (NULL);
	}
	head->next = NULL;
	head->i = 0;
	current = head;
	while ((ch = gnl_get_next_char(fdnode)) > 0)
	{
		if (ch == '\n')
			return (head);

	}
}

char		*gnl_build_line(t_schunk *chunks)
{
	size_t		len;
	t_schunk	*current;
	char		*str;
	char		*dst;

	len = 0;
	current = chunks;
	while (current)
	{
		len += current->i;
		current = current->next;
	}
	current = chunks;
	if ((str = (char *)malloc(len + 1)))
	{
		dst = str;
		while (current)
		{
			dst = ft_memcpy(dst, current->chunk, current->i) + current->i;
			*dst = '\0';
			current = current->next;
			free(chunks->chunk);
			free(chunks);
			chunks = current;
		}
	}
	return (str);
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
	if ((chunks = gnl_collect_line(fdnode)))
		*line = gnl_build_line(chunks);
	return (fdnode->status);
}	
