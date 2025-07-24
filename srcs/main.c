/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joudafke <joudafke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 23:44:21 by joudafke          #+#    #+#             */
/*   Updated: 2025/07/24 19:11:19 by joudafke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include <fcntl.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <unistd.h>

void	check_signal(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

char	*readline_stderr(const char *prompt)
{
	int		saved_stdout;
	int		devnull;
	char	*line;

	saved_stdout = dup(STDOUT_FILENO);
	devnull = open("/dev/null", O_WRONLY);
	dup2(STDERR_FILENO, STDOUT_FILENO);
	line = readline(prompt);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdout);
	close(devnull);
	return (line);
}

int	process_tokens_and_ast(t_main_data *data)
{
	data->expanded = expand_variables(data->input, data->env_list,
			data->exit_status);
	free(data->input);
	data->input = data->expanded;
	data->tokens = tokenize(data->input, 0);
	if (!data->tokens)
	{
		ft_putendl_fd("minishell: syntax error", STDERR_FILENO);
		free(data->input);
		return (1);
	}
	data->tokens_head = data->tokens;
	data->ast = parse_pipeline(&data->tokens);
	if (!data->ast)
	{
		cleanup_shell(data->tokens_head, data->ast, data->input);
		return (1);
	}
	return (0);
}

int	process_input(t_main_data *data)
{
	signal(SIGINT, check_signal);
	signal(SIGQUIT, SIG_IGN);
	data->input = readline_stderr("minishell : ");
	if (!data->input)
		return (0);
	add_history(data->input);
	if (is_quote_closed(data->input) != 0)
	{
		free(data->input);
		return (1);
	}
	return (2);
}

int	main(int ac, char **av, char **envp)
{
	t_main_data	data;
	int			result;

	(void)ac;
	(void)av;
	init_main_data(&data, envp);
	while (1)
	{
		reset_loop_data(&data);
		result = process_input(&data);
		if (result == 0)
			break ;
		if (result == 1)
			continue ;
		if (process_tokens_and_ast(&data))
			continue ;
		execute_main_loop(&data);
		cleanup_shell(data.tokens_head, data.ast, data.input);
	}
	cleanup_final(&data);
	return (0);
}
