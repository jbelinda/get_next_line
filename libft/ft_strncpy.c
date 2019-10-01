/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbelinda <jbelinda@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/17 18:44:42 by jbelinda          #+#    #+#             */
/*   Updated: 2019/09/17 18:47:59 by jbelinda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

char	*ft_strncpy(char *dst, const char *src, size_t n)
{
	unsigned char		*to;
	const unsigned char *from;

	to = (unsigned char *)dst;
	from = (const unsigned char *)src;
	while (n--)
		if ((*to++ = *from++) == '\0')
		{
			while (n--)
				*to++ = '\0';
			break ;
		}
	return (dst);
}
