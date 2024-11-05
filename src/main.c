/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaherradon <mariaherradon@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by ***********       #+#    #+#             */
/*   Updated: 2024/11/05 19:38:57 by mariaherrad      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

volatile int sig = 0;

int	slash_token(char *str, char *quote)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (str[i] == '|' && quote[i] == 'n')
		{
			while (str[++i])
			{
				if (str[i] == '|' && quote[i] == 'n')
					return (1);
				if (str[i] != '|' && str[i] != ' ')
					return (0);
			}
		}
	}
	return (0);
}


int	check_tocken(char *str)
{
	int		i;
	char	*trim;
	char	*quote;

	if (!str)
		return (0);
	i = -1;
	trim = ft_strtrim(str, " ");
	if (!trim)
		ft_error(MALLOC, NULL);
	if (ft_strlen(trim) == 0)
		return (0);
	quote = quote_line(trim);
	if (!quote)
		return (frees(trim, quote), 1);
	while (trim[++i])
		if ((trim[i] == ';' || trim[i] == '\\') && quote[i] == 'n')
			return (printf("%s '%c'\n", TOKEN, trim[i]), frees(trim, quote), 1);
	if (trim[ft_strlen(trim) - 1] == '<' || trim[ft_strlen(trim) - 1] == '>')
		return (frees(trim, quote), printf("%s 'newline'\n", TOKEN), 1);
	if (slash_token(trim, quote) == 1)
		return (frees(trim, quote), printf("%s '|'\n", TOKEN), 1);
	frees(trim, quote);
	return (0);
}

int main(int argn, char **argv, char **envp)
{
    char *here;
    char *current;
    char ***commands;
    int num_commands;
    t_env_list *env_list = NULL;
    char *shell_path = "/bin/minishell";

    if (argn > 1 || argv[1])
        return (printf("Error: bad number of arguments\n"), 0);

    handle_signals();
    if (initialize_environment(&env_list, envp, shell_path) != 0)
    {
        fprintf(stderr, "Error initializing environment\n");
        return (1);
    }
    while (1)
    {
        current = first_line();
        here = readline(current);
        if (!here)
            break;
        if (here && *here)
            add_history(here);
        if (ft_strncmp(here, "exit", ft_strlen("exit")) == 0)
            break;
        if (check_tocken(here) == 1)
        {
            free(here);
            free(current);
            continue;
        }
        commands = parse_input(here, &num_commands);
        if (!commands || num_commands == 0)
        {
            free(here);
            free(current);
            continue;
        }
        handle_redirection(commands[0]);
        if (!execute_builtin(commands[0]))
            execute_multiple_pipes(commands, num_commands);
        free_commands(commands, num_commands); 
        free(here);
        free(current);
    }
    free(here);
    free(current);
    clear_history();
    return(0);
}


