# 42-Sh

## Description

42sh is a powerful and user-friendly interactive shell designed to provide a seamless command-line interface for users. Built with efficiency and extensibility in mind, it aims to enhance the user experience in navigating and executing commands on a Unix-like system.

## Table of Contents

- [Installation](#installation)
- [Usage](#usage)
- [Contributing](#contributing)

## Installation

```sh
git clone <path>
cd <path>
make
```

## Usage

There are 3 ways to run 42sh program :

```sh
Usage: ./42sh {OPTIONS} [SCRIPT] [ARGUMENTS ...]
```

- Read from an shell script 
```sh
$ ./42sh <shell script path>
```

- Read from a string, with option `-c`
```sh
$ ./42sh -c "<string>"
```

- Read from standard input
```sh
$ ./42sh < <shell script path>
$ cat <shell script path> | ./42sh
```

### OPTIONS

There are 2 more options that can be activated by inserting these flags into arguments.

```sh
$ ./42sh --logger # Print some informations during process
$ ./42sh --pretty-print # Print the parser AST
```

## Contributing

All contributions are done by doing a merge request when you are done working on your own branch

Please, follow these conventions for the repository property :

### Branch name

```sh
^(<type>/<name>)$
```

### Commit name

```sh
^(<type>(<scope>): <description>
<BLANK LINE>
<body>)$
```

## Naming Convention

### Types
The `type` refers to the purpose or nature of the commit. It is **mandatory**.
* `feat` Commits, that adds or remove a new feature
* `fix` Commits, that fixes a bug
* `refactor` Commits, that rewrite/restructure your code, however does not change any API behaviour
    * `perf` Commits are special `refactor` commits, that improve performance
* `style` Commits, that do not affect the meaning (white-space, formatting, missing semi-colons, etc)
* `test` Commits, that add missing tests or correcting existing tests
* `docs` Commits, that affect documentation only
* `build` Commits, that affect build components like build tool, ci pipeline, dependencies, project version, ...
* `ops` Commits, that affect operational components like infrastructure, deployment, backup, recovery, ...
* `chore` Miscellaneous commits e.g. modifying `.gitignore`

### Scopes 
The `scope` refers to the module, component, or area of the project that your commit affects. It is **optional**.

### Description
The `description` contains a concise description of the change. It is **mandatory**.
* Use the imperative, present tense: "change" not "changed" nor "changes"
* Don't capitalize the first letter
* No dot (.) at the end

### Body
The `body` provides a more in-depth explanation of the changes made in the commit. It is **optional** but highly recommended.
* Use the imperative, present tense: "change" not "changed" nor "changes"

### Exemples
```
fix(http_parser): use the correct HTTP response code for wrong version
 
Use the HTTP error 405 instead of 400 if the request contains an unsupported HTTP version.
```
