#ifndef ENVIRONMENT_VARIABLE_H
#define ENVIRONMENT_VARIABLE_H

void set_variable(char *key, char *value);
char *get_variable(char *key);
char *get_environment_variable(char *key);

char *at_fun();
char *dollar_fun();
char *star_fun();
char *quest_fun();
char *number_fun(char *key);
char *sharp_fun();
char *uid_fun();
char *random_fun();
char *oldpwd_fun();
char *pwd_fun();
void set_args(char * arg);

#endif /* ! ENVIRONMENT_VARIABLE_H */