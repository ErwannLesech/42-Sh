#include "environment_variable.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

char **args = NULL;

void set_args(char * arg)
{
    if (args == NULL)
    {
        args = malloc(sizeof(char *) * 2);
        args[0] = arg;
        args[1] = NULL;
    }
    else
    {
        int i = 0;
        while (args[i] != NULL)
        {
            i++;
        }
        args = realloc(args, sizeof(char *) * (i + 2));
        args[i] = arg;
        args[i + 1] = NULL;
    }
}

void clear_args()
{
    if (args != NULL)
    {
        free(args);
        args = NULL;
    }
}

char *at_fun()
{
    char *at_str = malloc(sizeof(char) *1);
    for (int i = 0; args[i] != NULL; i++)
    {
        at_str = realloc(at_str, sizeof(char) * (strlen(at_str) + strlen(args[i]) + 1));
        strcat(at_str, args[i]);
        strcat(at_str, " ");
    }
    return at_str;

}

char *star_fun()
{
    char *star_str = malloc(sizeof(char) *1);
    for (int i = 0; args[i] != NULL; i++)
    {
        star_str = realloc(star_str, sizeof(char) * (strlen(star_str) + strlen(args[i]) + 1));
        strcat(star_str, args[i]);
        strcat(star_str, " ");
    }
    return star_str;
}

char *dollar_fun()
{
    pid_t pid = getpid();
    char pid_str[10];
    sprintf(pid_str, "%d", pid);
    set_variable("$", pid_str);
    return get_environment_variable("$");
}

char *quest_fun()
{
    return get_environment_variable("?");
}

char *number_fun(char *key)
{
    if (args == NULL)
    {
        return "";
    }
    int num = atoi(key);
    if (num < 0)
    {
        num = 0;
    }
    for (int i = 0; args[i] != NULL; i++)
    {
        if (i == num)
        {
            return args[i];
        }
    }
    return "";
}

char *sharp_fun()
{
    int number_of_args = 0;
    for (int i = 0; args[i] != NULL; i++)
    {
        number_of_args++;
    }
    char *number_of_args_str = malloc(sizeof(char) * 20);
    sprintf(number_of_args_str, "%d", number_of_args);
    set_variable("#", number_of_args_str);
    free(number_of_args_str);
    return get_environment_variable("#");
}

char *random_fun()
{
    srand(time(NULL));
    int random_number = rand();
    random_number %= 32768;
    char *random_number_str = malloc(sizeof(char) * 10);
    sprintf(random_number_str, "%d", random_number);   
    set_variable("RANDOM", random_number_str);
    free(random_number_str);
    return get_environment_variable("RANDOM");
}

char *uid_fun()
{
    uid_t user_id = getuid();
    char user_id_str[10];
    sprintf(user_id_str, "%d", user_id);
    set_variable("UID", user_id_str);
    return get_environment_variable("UID");
}

char *oldpwd_fun()
{
    return "";
}

char *pwd_fun()
{
    return "";
}
