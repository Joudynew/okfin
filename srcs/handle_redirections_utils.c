/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirections_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joudafke <joudafke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 01:00:00 by ibarbouc          #+#    #+#             */
/*   Updated: 2025/07/23 01:22:14 by joudafke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

extern pid_t	g_heredoc_interrupted;

void	setup_heredoc_file(t_ast_node *heredoc_node, char *rand_name)
{
	create_rand(rand_name);
	heredoc_node->heredoc_tmpfile = ft_strdup(rand_name);
	if (!heredoc_node->heredoc_tmpfile)
	{
		perror("ft_strdup");
		exit(EXIT_FAILURE);
	}
}

void	handle_heredoc_child(t_heredoc_data *data, t_ast_node *heredoc_node,
		char *rand_name)
{
	int	fd_input;

	signal(SIGINT, sigint_heredoc_handler);
	signal(SIGQUIT, SIG_IGN);
	fd_input = open(rand_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_input == -1)
	{
		perror(rand_name);
		free_in_child(data->env_list, heredoc_node, NULL);
		free_tokens(data->token);
		free(data->input);
		exit(EXIT_FAILURE);
	}
	read_heredoc_lines(fd_input, heredoc_node->filename);
	close(fd_input);
	free_in_child(data->env_list, data->node, NULL);
	free_tokens(data->token);
	free(data->input);
	exit(0);
}

void	handle_heredoc_parent(char *rand_name, pid_t pid_heredoc,
		t_ast_node *heredoc_node)
{
	int	status;

	signal(SIGINT, SIG_IGN);
	waitpid(pid_heredoc, &status, 0);
	signal(SIGINT, check_signal);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		unlink(rand_name);
		free(heredoc_node->heredoc_tmpfile);
		heredoc_node->heredoc_tmpfile = NULL;
	}
}
