#include "environment_variable.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

char *at_fun()
{
    return "";
}

char *star_fun()
{
    return "";
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
    return "";
}

char *number_fun()
{
    return "";
}

char *sharp_fun()
{
    return "";
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
