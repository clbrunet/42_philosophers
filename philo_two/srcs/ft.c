/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clbrunet <clbrunet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/04 14:21:37 by clbrunet          #+#    #+#             */
/*   Updated: 2021/05/04 15:34:38 by clbrunet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static unsigned	ft_nlen(long n, int len)
{
	if (n > 9)
		return (ft_nlen(n / 10, len + 1));
	return (len);
}

static void		ft_itoa_conv(char *a, long n)
{
	if (n > 9)
		ft_itoa_conv(a - 1, n / 10);
	*a = n % 10 + '0';
}

char			*ft_itoa(int n)
{
	char		*a;
	long		l_n;
	unsigned	len;
	int			is_negative;

	l_n = (long)n;
	is_negative = 0;
	if (l_n < 0)
	{
		l_n *= -1;
		is_negative = 1;
	}
	len = ft_nlen(l_n, is_negative + 1);
	if (!(a = malloc((len + 1) * sizeof(char))))
		return ((char *)NULL);
	a[len] = 0;
	if (is_negative)
		*a = '-';
	ft_itoa_conv(a + len - 1, l_n);
	return (a);
}

static size_t	ft_strlen(const char *s)
{
	const char	*s_bp;

	s_bp = s;
	while (*s)
		s++;
	return ((size_t)(s - s_bp));
}

char			*ft_strjoin(char const *s1, char const *s2)
{
	char	*joint;
	char	*joint_bp;

	if (!s1 || !s2)
		return (NULL);
	if (!(joint = malloc((ft_strlen(s1) + ft_strlen(s2) + 1) * sizeof(char))))
		return (NULL);
	joint_bp = joint;
	while (*s1)
	{
		*joint = *s1;
		joint++;
		s1++;
	}
	while (*s2)
	{
		*joint = *s2;
		joint++;
		s2++;
	}
	*joint = 0;
	return (joint_bp);
}
