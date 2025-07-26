/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joudafke <joudafke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 16:40:57 by joudafke          #+#    #+#             */
/*   Updated: 2025/07/26 18:33:37 by joudafke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

void	handle_heredoc_eof(char *delimiter, int line_number)
{
	printf("minishell: warning: here-document");
	printf(" line %d delimited by end-of-file", line_number);
	printf(" (wanted `%s')\n", delimiter);
}

int	write_heredoc_line(int fd_input, char *line)
{
	if (write(fd_input, line, ft_strlen(line)) == -1 || write(fd_input, "\n",
			1) == -1)
	{
		perror("write");
		free(line);
		close(fd_input);
		exit(EXIT_FAILURE);
	}
	return (0);
}

void	cleanup_heredocs(t_ast_node *node)
{
	if (!node)
		return ;
	if (node->redir_type == HEREDOC && node->heredoc_tmpfile)
	{
		unlink(node->heredoc_tmpfile);
		free(node->heredoc_tmpfile);
		node->heredoc_tmpfile = NULL;
	}
	cleanup_heredocs(node->left);
	cleanup_heredocs(node->right);
	cleanup_heredocs(node->redirections);
}

void	get_rand(void)
{
	char	rand[32 + 1];

	create_rand(rand);
}

void	create_rand(char *str)
{
	int		fd;
	int		len;
	char	buff[64 + 1];
	int		i;

	fd = open("/dev/urandom", O_RDONLY);
	if (fd < 0)
		return ;
	len = 0;
	while (len < 32)
	{
		buff[read(fd, buff, 64)] = 0;
		i = 0;
		while (buff[i] && len < 32)
		{
			if (ft_isalnum(buff[i]))
			{
				str[len] = buff[i];
				++len;
			}
			++i;
		}
	}
	str[len] = 0;
	close(fd);
}
