/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joudafke <joudafke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 00:00:00 by ibarbouc          #+#    #+#             */
/*   Updated: 2025/07/23 01:22:14 by joudafke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

extern pid_t	g_heredoc_interrupted;

void	handle_heredoc_eof(char *delimiter, int line_number)
{
	printf("minishell: warning: here-document");
	printf(" line %d delimited by end-of-file", line_number);
	printf(" (wanted `%s')\n", delimiter);
}

int	write_heredoc_line(int fd_input, char *line)
{
	if (write(fd_input, line, strlen(line)) == -1 || write(fd_input, "\n",
			1) == -1)
	{
		perror("write");
		free(line);
		close(fd_input);
		exit(EXIT_FAILURE);
	}
	return (0);
}

int	no_cmd_process_redirect_in(t_ast_node *redirect_in_node)
{
	int	fd_input;

	fd_input = open(redirect_in_node->filename, O_RDONLY);
	if (fd_input == -1)
	{
		perror("open");
		return (-1);
	}
	close(fd_input);
	return (0);
}
