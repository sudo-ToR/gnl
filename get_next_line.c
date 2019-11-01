/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnoirot <lnoirot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/18 17:08:54 by lnoirot           #+#    #+#             */
/*   Updated: 2019/11/01 19:33:29 by lnoirot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

static int		ft_search_line(char *buff, int i)
{
	int j;

	j = 0;
	while (j < BUFFER_SIZE - i)
	{
		if (buff[j] == '\n')
			return (1);
		j++;
	}
	return (0);
}

static char		*ft_strjoin_line(char *s1, char *s2, int i)
{
	char	*r;
	int		j;
	int		len2;
	int		len1;

	j = 0;
	len1 = 0;
	len2 = 0;
	while (s1[len1])
		len1++;
	while (len2 < BUFFER_SIZE - i)
		len2++;
	if (!(r = (char *)malloc(sizeof(char)
					* (len1 + len2 + 1))))
		return (NULL);
	while (*s1 && *s1 != '\n')
		r[j++] = *s1++;
	while (j < len1 + len2 && *s2 != '\n')
		r[j++] = *s2++;
	r[j] = '\0';
	return (r);
}

static char		*ft_strdup_line(char *buff, int i)
{
	char	*str;
	int		j;
	int		len;

	j = 0;
	len = 0;
	while (len < BUFFER_SIZE - i && buff[len] != '\n')
		len++;
	if (!(str = malloc(sizeof(char) * (len + 1))))
		return (NULL);
	while (j < len)
	{
		str[j] = buff[j];
		j++;
	}
	str[j] = '\0';
	return (str);
}

static int		get_next_line_pt1(char *buff, int *i, int fd, char **line)
{
	char *tmp;

	tmp = *line;
	if (!(*line = ft_strdup_line(buff, *i)))
		return (-1);
	free(tmp);
	while (ft_search_line(&buff[*i], *i) == 0)
	{
		if(!(read(fd, buff, BUFFER_SIZE)))
			return (0);
		tmp = *line;
		*line = ft_strjoin_line(*line, &buff[*i], *i);
		free(tmp);
		*i = 0;
	}
	if (ft_search_line(&buff[*i], *i) == 1)
	{
		while (*i < BUFFER_SIZE && buff[*i] != '\n')
			(*i)++;
		if (*i < BUFFER_SIZE && buff[*i] == '\n')
			(*i)++;
		return (1);
	}
	return (2);
}

int				get_next_line(int fd, char **line)
{
	static int		i = 0;
	static char		buff[BUFFER_SIZE];
	int				ret;

	ret = 0;
	while (i < BUFFER_SIZE && ret != 0)
	{
		ret = get_next_line_pt1(buff, &i, fd, line);
		printf("ret = %d\n", ret);
		if (ret == 1 || ret == -1)
			return (ret);
	}
	while (read(fd, buff, BUFFER_SIZE))
	{
		if (i == BUFFER_SIZE)
			i = 0;
		ret = get_next_line_pt1(buff, &i, fd, line);
		if (ret == 1 || ret == -1)
			return (ret);
	}
	if (i != 0)
		*line = "";
	return (0);
}

int		main(int argc, char **argv)
{
	if (argc != 2)
		return (0);
	char *line[20];
	int	fd;
	int ret = 1;
	if (!(fd = open(argv[1], O_RDONLY)))
		return (0);
	while (ret > 0)
	{
		ret = get_next_line(fd, line);	
		printf("line = %s\n", line[0]);
		printf("%d\n", ret);
	}
	close(fd);
	return (0);
}
