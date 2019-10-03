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
}

t_schunk	*gnl_collect_line(t_fdnode *fdnode)
{
	t_schunk	*head;
	t_schunk	*current;
	int			ch;

	if (!(head = (t_schunk*)malloc(sizeof(t_schunk))))
		return (NULL);
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
	if ((str = (char *)ft_memalloc(len + 1)))
	{
		dst = str;
		while (current)
		{
			dst = ft_memcpy(dst, current->chunk, current->i) + current->i;
			current = current->next;
			free(chunks);
			chunks = current;
		}
	}
	return (str);
}
/*
** Lookups for `fd' in `fdlist', creates new node if `fd' not exist
*/

static t_fdnode	*gnl_fd_lookup(fd)
{
	static t_fdnode	*fdlist = NULL;
	t_fdnode		*fds;

	if (!fdlist)
	{

	}
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
