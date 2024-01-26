#include "builtin.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * \brief Remove the i-th element of the array.
 * \param curpath The array of strings containing the current path.
 * \param index The index of the element to remove.
 * \return The array of strings containing the current path.
 * \example ["home", "user", "Documents"] -> ["home", "Documents"]
*/

char **remove_at(char **curpath, size_t index)
{
    // Get the length of the array
    size_t curpath_len = 0;
    while (curpath[curpath_len] != NULL)
    {
        curpath_len++;
    }

    // If the index is out of bounds
    if (index >= curpath_len)
    {
        return curpath;
    }

    // Free the element to remove
    free(curpath[index]);
    curpath[index] = NULL;

    // Move all the elements after the index to the left
    for (size_t i = index; i < curpath_len - 1; i++)
    {
        curpath[i] = curpath[i + 1];
    }
    curpath = realloc(curpath, sizeof(char*) * (curpath_len));
    curpath[curpath_len - 1] = NULL;

    return curpath;
}

/**
 * \brief Remove all the '..' from the path.
 * \param curpath The array of strings containing the current path.
 * \param curpath_len The length of the array.
 * \return The array of strings containing the current path.
 * \example ["home", "user", "Documents", ".."] -> ["home", "Documents"]
*/

char **remove_dotdot_from_array(char **curpath, size_t *curpath_len)
{
    // Remove all the '..' from the path
    while (*curpath_len > 0)
    {
        if (strcmp(curpath[*curpath_len - 1], "..") == 0)
        {
            size_t to_remove = 0;
            while (*curpath_len > 0 && strcmp(curpath[*curpath_len - 1], "..") == 0)
            {
                curpath = remove_at(curpath, *curpath_len - 1);
                *curpath_len -= 1;
                to_remove++;
            }
            while (*curpath_len > 0 && to_remove > 0)
            {
                curpath = remove_at(curpath, *curpath_len - 1);
                *curpath_len -= 1;
                to_remove--;
            }
        }
        else
            *curpath_len -= 1;
    }

    // Get the length of the array
    while (curpath[*curpath_len])
    {
        *curpath_len += 1;
    }

    // If the path is empty
    if (*curpath_len == 0)
    {
        curpath = realloc(curpath, sizeof(char*));
        curpath[0] = malloc(sizeof(char));
        curpath[0][0] = '\0';
        // Adding one to the length because we will add a '/' at the end
        *curpath_len += 1;
    }
    return curpath;
}


/**
 * \brief Rebuild the path from the array of strings.
 * \param curpath The array of strings containing the current path.
 * \param path The path to rebuild.
 * \param curpath_len The length of the array.
 * \return The path.
 * \example ["home", "user", "Documents"] -> /home/user/Documents
*/
char *rebuild_path_from_array(char **curpath, size_t curpath_len)
{

    // Remove all the '..' from the path
    curpath = remove_dotdot_from_array(curpath, &curpath_len);

    char *path = malloc(sizeof(char));
    path[0] = '\0';
    size_t j = 0;

    // Rebuild the path
    for (size_t k = 0; k < curpath_len; k++)
    {
        path = realloc(path, j + strlen(curpath[k]) + 2);
        path[j] = '/';
        j++;
        for (size_t l = 0; l < strlen(curpath[k]); l++)
        {
            path[j] = curpath[k][l];
            j++;
        }
    }

    // Free the array
    for (size_t k = 0; k < curpath_len; k++)
    {
        free(curpath[k]);
    }
    free(curpath);

    // Remove the last '/' if the path is not '/'
    if (j > 0 && path[j - 1] == '/' && j > 1)
    {
        path = realloc(path, j);
        path[j - 1] = '\0';
        j--;
    }
    else
    {
        path = realloc(path, j + 1);
        path[j] = '\0';
    }
    return path;
}

/**
 * \brief Create an array of strings containing the current path without the '/'.
 * \param path The path to split.
 * \param curpath_len The length of the array.
 * \return The array of strings containing the current path.
 * \example /home/user/Documents -> ["home", "user", "Documents"]
*/
char **create_curr_path_array(char *path, size_t *curpath_len)
{
    char **curpath = malloc(sizeof(char*));
    size_t path_len = strlen(path);
    size_t path_index = 0;

    while (path_index < path_len)
    {
        // Skip all the '/' at the beginning of the path
        while (path_index < path_len && path[path_index] == '/')
        {
            path_index++;
        }
        if (path_index >= path_len)
        {
            break;
        }

        // Get the next component of the path
        size_t components_index = 0;
        curpath[*curpath_len] = malloc(sizeof(char));
        curpath[*curpath_len][0] = '\0';

        while (path_index < path_len && path[path_index] != '/')
        {
            curpath[*curpath_len] = realloc(curpath[*curpath_len], sizeof(char) * (components_index + 2));
            curpath[*curpath_len][components_index] = path[path_index];
            curpath[*curpath_len][components_index + 1] = '\0';
            components_index++;
            path_index++;
        }
        curpath = realloc(curpath, sizeof(char*) * (*curpath_len + 2));
        *curpath_len += 1;
    }
    curpath = realloc(curpath, sizeof(char*) * (*curpath_len + 1));
    curpath[*curpath_len] = NULL;

    // Remove all the '.' from the path
    for (size_t k = 0; k < *curpath_len; k++)
    {
        if (strcmp(curpath[k], ".") == 0)
        {
            curpath = remove_at(curpath, k);
            *curpath_len -= 1;
            k--;
        }
    }
    return curpath;
}


/**
 * \brief Append the current path to the given path.
 * \param curr_path The path to append.
 * \param cd_builtin If the path is for the cd builtin.
 * \param exit_status The exit status.
 * \return The path. If the return value is NULL,
 * the path is invalid.
 * \example "test.sh" -> "/home/user/test.sh"
*/
char *append_pwd(char *curr_path, bool cd_builtin, int *exit_status)
{
    char *OLDPWD = getenv("OLDPWD") == NULL ? "/" : getenv("OLDPWD");
    char *PWD = getenv("PWD") == NULL ? "/" : getenv("PWD");
    size_t curr_path_len = strlen(curr_path);

    char *curr_pwd = PWD;

    // Current pwd is OLDPWD if cd is called with '-'
    // Enable '-' for cd builtin
    if (cd_builtin)
    {
        curr_pwd = curr_path_len > 0 && curr_path[0] == '-' ? OLDPWD : PWD;
    }
    

    char *path = strcpy(malloc(strlen(curr_pwd) + 1), curr_pwd);

    // Case where cd is called with '-'
    if (curr_path_len > 0 && curr_path[0] == '-' && cd_builtin)
    {
        if (curr_path_len > 1)
        {
            fprintf(stderr, "cd: %s: invalid option\n", curr_path);
            free(path);
            *exit_status = 2;
            return NULL;
        }
        printf("%s\n", path);
        fflush(stdout);
    }
    else if (curr_path_len > 0 && curr_path[0] != '/')
    {
        // Case where PWD is not ending with '/'
        if (PWD[strlen(PWD) - 1] != '/')
        {
            path = realloc(path, strlen(PWD) + 2);
            path[strlen(PWD)] = '/';
            path[strlen(PWD) + 1] = '\0';
        }
        path = realloc(path, strlen(path) + strlen(curr_path) + 1);
        path = strcat(path, curr_path);
    }
    else
    {
        // Case where curr_path is starting by '/'
        path = realloc(path, strlen(curr_path) + 1);
        path = strcpy(path, curr_path);
    }
    return path;
}

char *refactor_path(char *curr_path, bool cd_builtin, int *exit_status)
{
    char *HOME = getenv("HOME") == NULL ? "/" : getenv("HOME");
    

    size_t curr_path_len = strlen(curr_path);
    bool to_free = false;

    // Handle the '~' in the path
    if (curr_path_len > 0 && curr_path[0] == '~')
    {
        char *tmp = malloc(sizeof(char) * (strlen(HOME) + curr_path_len));
        strcpy(tmp, HOME);
        strcat(tmp, curr_path + 1);
        curr_path = tmp;
        to_free = true;
        curr_path_len = strlen(curr_path);
    }

    char *path = append_pwd(curr_path, cd_builtin, exit_status);
    if (path == NULL)
    {
        return NULL;
    }

    // Need to free curr_path if the curr_path starts with '~'
    if (to_free)
    {
        free(curr_path);
    }
    
    // Path contains the PWD / OLDPWD + curr_path
    if (access(path, F_OK) != 0)
    {
        fprintf(stderr, "cd: %s: No such file or directory\n", path);
        free(path);
        *exit_status = 1;
        return NULL;
    }
    size_t curpath_len = 0;
    char **curpath = create_curr_path_array(path, &curpath_len);
    free(path);

    return rebuild_path_from_array(curpath, curpath_len);
}
