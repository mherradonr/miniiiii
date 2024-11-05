#include "../include/minishell.h"

char *get_env_value(t_env_list **env_list, char *key)
{
    t_env_list *tmp = *env_list;

    while (tmp)
    {
        if (strcmp(tmp->content->key, key) == 0)
            return tmp->content->value;
        tmp = tmp->next;
    }
    return(NULL);
}

void free_env_list(t_env_list *env_list)
{
    t_env_list *tmp;

    while (env_list)
    {
        tmp = env_list;
        env_list = env_list->next;

        if (tmp->content->key)
            free(tmp->content->key);

        if (tmp->content->value)
            free(tmp->content->value);

        free(tmp->content);
        free(tmp);
    }
}

int already_exists(t_env_list **env_list, char *key)
{
    t_env_list *tmp = *env_list;

    while (tmp)
    {
        if (strcmp(tmp->content->key, key) == 0)
            return 1;
        tmp = tmp->next;
    }
    return(0);
}

char *get_env_value(t_env_list **env_list, char *key)
{
    t_env_list *tmp = *env_list;

    while (tmp)
    {
        if (strcmp(tmp->content->key, key) == 0)
            return tmp->content->value;
        tmp = tmp->next;
    }
    return(NULL);
}

int initialize_environment(t_env_list **env_list, char **envp, char *shell_path)
{
    *env_list = init_env_list(envp);
    if (!*env_list)
        return perror("Failed to initialize environment list"), 1;

    if (initialize_pwd(env_list))
        return perror("Failed to initialize PWD"), 1;

    if (initialize_oldpwd(env_list))
        return perror("Failed to initialize OLDPWD"), 1;

    if (update_shlvl(env_list))
        return perror("Failed to update SHLVL"), 1;

    if (initialize_underscore(env_list, shell_path))
        return perror("Failed to initialize _"), 1;

    return(0);
}