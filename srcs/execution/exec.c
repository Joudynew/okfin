/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joudafke <joudafke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 16:40:11 by joudafke          #+#    #+#             */
/*   Updated: 2025/07/26 18:06:46 by joudafke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

char	**get_path(t_env *env_list)
{
	char	*path_name;
	char	**tab;

	path_name = "PATH";
	tab = NULL;
	while (env_list)
	{
		if (env_list->name && ft_strcmp(env_list->name, path_name) == 0)
		{
			if (env_list->value)
				tab = ft_split(env_list->value, ':');
			return (tab);
		}
		env_list = env_list->next;
	}
	return (NULL);
}

char	*get_cmd(t_env *env_list, char *s1)
{
	int		i;
	char	**str;
	char	*pathname;

	i = 0;
	if (!s1 || s1[0] == '\0')
		return (ft_putendl_fd("Command not found", STDERR_FILENO), NULL);
	if (ft_strchr(s1, '/'))
	{
		if (access(s1, F_OK | X_OK) == 0)
			return (ft_strdup(s1));
		return (perror(s1), NULL);
	}
	str = get_path(env_list);
	if (!str)
		return (perror("PATH not found"), NULL);
	while (str[i])
	{
		pathname = ft_strjoin2(str[i], s1);
		if (pathname && access(pathname, F_OK | X_OK) == 0)
			return (free_split(str), pathname);
		free(pathname);
		i++;
	}
	return (free_split(str), perror(s1), NULL);
}

int	execute_ast(t_ast_node *node, t_exec_data *data)
{
	if (node->type == NODE_PIPE)
		return (execute_pipe(node, data));
	else if (node->type == NODE_COMMAND)
		return (execute_command(node, data));
	return (0);
}

void	execute_main_loop(t_main_data *data)
{
	t_exec_data	exec_data;

	preprocess_all_heredocs(data->ast, data->env_list, &data->tokens_head,
		data->input);
	exec_data.node = data->ast;
	exec_data.envp = data->envp;
	exec_data.env_list = data->env_list;
	exec_data.token = data->tokens_head;
	exec_data.input = data->input;
	exec_data.first_node = data->ast;
	exec_data.is_pipe = false;
	execute_ast(data->ast, &exec_data);
	cleanup_heredocs(data->ast);
}
