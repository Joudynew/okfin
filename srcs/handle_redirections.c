/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joudafke <joudafke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 20:59:41 by joudafke          #+#    #+#             */
/*   Updated: 2025/07/23 01:22:14 by joudafke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

pid_t	g_heredoc_interrupted = 0;

void	sigint_heredoc_handler(int sig)
{
	(void)sig;
	g_heredoc_interrupted = 1;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_done = 1;
}

void	read_heredoc_lines(int fd_input, char *delimiter)
{
	char	*line;
	int		line_number;

	line_number = 1;
	while (!g_heredoc_interrupted)
	{
		line = readline("heredoc> ");
		if (!line)
		{
			handle_heredoc_eof(delimiter, line_number);
			break ;
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			printf("HERE\n");
			free(line);
			break ;
		}
		write_heredoc_line(fd_input, line);
		free(line);
		line_number++;
	}
}

void	process_heredoc(t_process_heredoc_data *proc_data)
{
	char			rand_name[33];
	pid_t			pid_heredoc;
	t_heredoc_data	data;

	if (!proc_data->heredoc_node)
	{
		fprintf(stderr, "Erreur: heredoc_node est NULL\n");
		return ;
	}
	setup_heredoc_file(proc_data->heredoc_node, rand_name);
	data.env_list = proc_data->env_list;
	data.token = proc_data->token;
	data.input = proc_data->input;
	data.node = proc_data->node;
	g_heredoc_interrupted = 0;
	pid_heredoc = fork();
	if (pid_heredoc == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (pid_heredoc == 0)
		handle_heredoc_child(&data, proc_data->heredoc_node, rand_name);
	else
		handle_heredoc_parent(rand_name, pid_heredoc, proc_data->heredoc_node);
}

void	process_heredoc_redirections(t_ast_node *node, t_env *env_list,
		t_token **token, char *input)
{
	t_ast_node				*tmp;
	t_process_heredoc_data	proc_data;

	tmp = node->redirections;
	while (tmp)
	{
		if (tmp->redir_type == HEREDOC)
		{
			proc_data.heredoc_node = tmp;
			proc_data.env_list = env_list;
			proc_data.token = token;
			proc_data.input = input;
			proc_data.node = node;
			process_heredoc(&proc_data);
		}
		tmp = tmp->right;
	}
}

void	preprocess_all_heredocs(t_ast_node *node, t_env *env_list,
		t_token **token, char *input)
{
	if (!node)
		return ;
	if (node->type == NODE_COMMAND && node->redirections)
		process_heredoc_redirections(node, env_list, token, input);
	preprocess_all_heredocs(node->left, env_list, token, input);
	preprocess_all_heredocs(node->right, env_list, token, input);
}
