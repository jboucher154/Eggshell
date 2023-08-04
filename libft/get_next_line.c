/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 14:59:45 by jebouche          #+#    #+#             */
/*   Updated: 2023/02/06 09:32:05 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/libft.h"

/*reads up to a buffer length into an allocated string that is returned. On 
allocation failure NULL is returned. On read failure or end of file reached 
the allocated string is freed and NULL is returned*/
static ssize_t	fill_cache(int fd, char **cache)
{
	ssize_t		returned;

	if (*cache)
		free(*cache);
	*cache = (char *) malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!(*cache))
		return (-1);
	returned = read(fd, *cache, BUFFER_SIZE);
	if (returned <= 0)
	{
		free(*cache);
		return (returned);
	}
	(*cache)[returned] = '\0';
	return (returned);
}

static char	*copy_to_temp(char *cache, size_t *len)
{
	char	*temp;
	size_t	i;

	i = 0;
	while (cache[(*len)] != '\0')
	{
		if (cache[(*len)] == '\n')
		{
			(*len) += 1;
			break ;
		}
		(*len) += 1;
	}
	temp = (char *) malloc(sizeof(char) * (*len + 1));
	if (!temp)
		return (NULL);
	temp[(*len)] = '\0';
	while (i < (*len))
	{
		temp[i] = cache[i];
		i++;
	}
	return (temp);
}

static char	*get_line(int fd, char *cache, char **line, ssize_t *returned)
{
	char	*temp;
	size_t	len;

	len = 0;
	if (!cache)
		*returned = fill_cache(fd, &cache);
	while (*returned > 0 && cache && cache[0] != '\0')
	{
		temp = copy_to_temp(cache, &len);
		if (ft_strchr(temp, '\n') != NULL)
		{
			ft_strlcpy(cache, cache + len, BUFFER_SIZE);
			*line = ft_gnl_join(*line, temp);
			temp = NULL;
			return (cache);
		}
		else if (len == ft_strlen(cache))
		{
			*line = ft_gnl_join(*line, temp);
			temp = NULL;
			*returned = fill_cache(fd, &cache);
			len = 0;
		}
	}
	return (NULL);
}

void	clean_cache(char **cache)
{
	size_t	i;

	i = 0;
	if (*cache && *cache[i] == '\0')
	{
		free(*cache);
		*cache = NULL;
	}
}

char	*get_next_line(int fd)
{
	char		*line;
	static char	*caches[256];
	ssize_t		returned;

	if (fd < 0 || fd >= 256 || BUFFER_SIZE <= 0)
		return (NULL);
	line = (char *) malloc(sizeof(char) * 1);
	if (!line)
		return (NULL);
	line[0] = '\0';
	returned = 1;
	caches[fd] = get_line(fd, caches[fd], &line, &returned);
	clean_cache(&caches[fd]);
	if (returned == 0 || returned == -1)
	{
		if (line && line[0] == '\0')
		{
			free(line);
			return (NULL);
		}
		else
			return (line);
	}
	return (line);
}
