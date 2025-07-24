/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joudafke <joudafke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 13:19:51 by ibarbouc          #+#    #+#             */
/*   Updated: 2025/07/24 23:26:35 by joudafke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include "parser.h"
# include "printf.h"
# include <errno.h>
# include <limits.h>
# include <linux/limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_env
{
	char			*name;
	char			*value;
	char			*env;
	struct s_env	*next;

}					t_env;

typedef enum e_quote_state
{
	NO_QUOTE,
	SINGLE_QUOTE,
	DOUBLE_QUOTE,
}					t_quote_state;

typedef struct s_heredoc_data
{
	t_env		*env_list;
	t_token		**token;
	char		*input;
	t_ast_node	*node;
}	t_heredoc_data;

typedef struct s_exit_data
{
	t_env		*env_list;
	t_ast_node	*node;
	char		*input;
	t_token		*token;
	char		*input_for_free;
}	t_exit_data;

typedef struct s_expand
{
	char			*result;
	char			*tmp;
	char			*expanded;
	char			temp[2];
	int				i;
	t_quote_state	quote;
}					t_expand;

typedef struct s_exec_data
{
	t_ast_node		*node;
	char			**envp;
	t_env			*env_list;
	t_token			*token;
	char			*input;
	t_ast_node		*first_node;
	bool			is_pipe;
}					t_exec_data;

typedef struct s_builtin_data
{
	t_ast_node	*node;
	t_env		*env_list;
	char		*input;
	t_token		*token;
	char		*input_for_free;
}	t_builtin_data;

typedef struct s_process_heredoc_data
{
	t_ast_node	*heredoc_node;
	t_env		*env_list;
	t_token		**token;
	char		*input;
	t_ast_node	*node;
}	t_process_heredoc_data;

typedef struct s_main_data
{
	char		*input;
	char		*expanded;
	t_env		*env_list;
	int			exit_status;
	t_token		*tokens;
	t_token		*tokens_head;
	t_ast_node	*ast;
	char		**envp;
}	t_main_data;

// expand
t_env				*create_env_list(char **envp);
void				add_env_node(t_env **env_list, char *name, char *value);
char				*expand_variables(char *input, t_env *env_list,
						int exit_status);
char				*get_env_value(char *name, t_env *env_list);
int					extract_var_name(char *input, int start_index,
						char *var_name_buffer);
int					is_valid_var_char(char c);

// builtin
void				exec_builtin(t_builtin_data *builtin_data);
void				handle_exit_builtin(t_builtin_data *data);
void				exec_builtin_commands(t_builtin_data *data);
int					builtin_echo(char **args);
int					builtin_env(t_env *env_list);
int					builtin_pwd(char **args);
void				ft_cd(char *path);
void				builtin_exit(t_exit_data *data);
void				exit_cleanup_and_exit(t_exit_data *data, long exit_code);
t_env				*builtin_unset(t_env *env, char *cmd);
t_env				*remove_env_node(t_env *env, t_env *previous,
						t_env *current);
int					builtin_export(t_env *env_list, char **cmd);
int					count_env_nodes(t_env *env);
void				fill_env_tab(t_env *env, char **tab);
void				export_sorted(char **env_cpy, int len_env);
t_env				*find_env_node(t_env *env, char *name);
void				add_or_replace(t_env **env, char *cmd);
void				add_export_only(t_env **env, char *cmd);
int					ft_is_num(char *str);

// caracteres & quotes
int					is_quote_closed(char *input);

// syntax
void				syntax_pipe(char *input);
void				*no_cmd_process_redirections(t_ast_node *redir_list);

// utils
int					ft_strcmp(char *s1, char *s2);
t_env				*free_list(t_env *a);
int					execute_ast(t_ast_node *node, t_exec_data *data);
char				*ft_strjoin2(char const *s1, char const *s2);
void				free_in_child(t_env *env_list, t_ast_node *node,
						char *path);
int					is_builtin(char *cmd);
int					needs_child_process(char *cmd);
void				cleanup_heredocs(t_ast_node *node);
void				handle_signal_status(int status);
int					execute_pipe(t_ast_node *node, t_exec_data *data);
int					execute_command(t_ast_node *node, t_exec_data *data);
void				execute_builtin_main(t_ast_node *node, t_exec_data *data);
char				**get_path(t_env *env_list);
char				*get_cmd(t_env *env_list, char *s1);

void				cleanup_shell(t_token *tokens, t_ast_node *ast,
						char *input);
int					process_tokens_and_ast(t_main_data *data);
void				execute_main_loop(t_main_data *data);
void				cleanup_final(t_main_data *data);
void				init_main_data(t_main_data *data, char **envp);
void				reset_loop_data(t_main_data *data);
int					process_input(t_main_data *data);
void				create_rand(char *str);
void				check_signal(int sig);

// redirections
void				no_cmd_process_append(t_ast_node *append_node);
int					process_redirect_out(t_ast_node *redirect_out_node);
void				process_append(t_ast_node *append_node);
void				*no_cmd_process_redirections(t_ast_node *redir_list);
void				*process_redirections(t_ast_node *redir_list);
void				preprocess_all_heredocs(t_ast_node *node, t_env *env_list,
						t_token **token, char *input);
void				process_heredoc_redirections(t_ast_node *node,
						t_env *env_list, t_token **token, char *input);
void				process_heredoc(t_process_heredoc_data *proc_data);
void				setup_heredoc_file(t_ast_node *heredoc_node,
						char *rand_name);
void				handle_heredoc_child(t_heredoc_data *data,
						t_ast_node *heredoc_node, char *rand_name);
void				handle_heredoc_parent(char *rand_name, pid_t pid_heredoc,
						t_ast_node *heredoc_node);
void				handle_heredoc_eof(char *delimiter, int line_number);
int					write_heredoc_line(int fd_input, char *line);
int					process_redirect_in(t_ast_node *redirect_in_node);
int					no_cmd_process_redirect_out(t_ast_node *redirect_out_node);
int					no_cmd_process_redirect_in(t_ast_node *redirect_in_node);
void				sigint_heredoc_handler(int sig);
void				read_heredoc_lines(int fd_input, char *delimiter);
void				cleanup(t_env *env_list, char **args, char *input);
void				handle_non_numeric_exit_arg(t_env *env_list, char **args,
						char *input);
int					ft_valid_nbr(char *str);

// lexer
t_token				*tokenize(char *input, int i);
int					process_input_char(char *input, t_token **token_list,
						int i);

// parser
t_ast_node			*parse_command(t_token **token_list, t_ast_node *cmd);
t_ast_node			*handle_word_token(t_token **token_list, t_ast_node *cmd);
t_ast_node			*handle_redir_token(t_token **token_list, t_ast_node *cmd);

#endif