/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joudafke <joudafke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 23:44:21 by joudafke          #+#    #+#             */
/*   Updated: 2025/07/23 01:22:14 by joudafke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include <fcntl.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <unistd.h>

pid_t	g_signal_pid = 0;

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

void	init_main_data(t_main_data *data, char **envp)
{
	data->input = NULL;
	data->expanded = NULL;
	data->tokens = NULL;
	data->tokens_head = NULL;
	data->ast = NULL;
	data->env_list = create_env_list(envp);
	data->exit_status = 0;
	data->envp = envp;
}

void	reset_loop_data(t_main_data *data)
{
	data->input = NULL;
	data->expanded = NULL;
	data->tokens = NULL;
	data->tokens_head = NULL;
	data->ast = NULL;
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
