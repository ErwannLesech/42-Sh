#ifndef ENVIRONMENT_VARIABLE_H
#define ENVIRONMENT_VARIABLE_H

void set_variable(char *key, char *value);
char *get_variable(char *key);
char *get_environment_variable(char *key);

char *at_fun(void);
char *dollar_fun(void);
char *star_fun(void);
char *quest_fun(void);
char *number_fun(void);
char *sharp_fun(void);
char *uid_fun(void);
char *random_fun(void);
char *oldpwd_fun(void);
char *pwd_fun(void);

#endif /* ! ENVIRONMENT_VARIABLE_H */
