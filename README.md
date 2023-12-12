# 42-Sh

## Description

42sh is a powerful and user-friendly interactive shell designed to provide a seamless command-line interface for users. Built with efficiency and extensibility in mind, it aims to enhance the user experience in navigating and executing commands on a Unix-like system.

## Features

//TODO

## Table of Contents

- [Installation](#installation)
- [Usage](#usage)
- [Contributing](#contributing)

## Installation

```sh
git clone git@github.com:ErwannLesech/42-Sh.git
cd 42-Sh
make
```

## Usage

```sh
./42sh
Usage #TODO
```

## Contributing

All contributions are done by doing a merge request when you are done working on your own branch

Please, follow these conventions for the repository property :

- Branch name

```sh
^(<type>/<name>)$
```

- Commit name

```sh
^(<type>(<scope>): <description>
<BLANK LINE>
<body>)$
```

- Naming convention

### Types **mandatory**
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

### Scopes **optional**
The `scope` refers to the module, component, or area of the project that your commit affects.

### Description **mandatory**
The `description` contains a concise description of the change.
* Use the imperative, present tense: "change" not "changed" nor "changes"
* Don't capitalize the first letter
* No dot (.) at the end

### Body
The `body` provides a more in-depth explanation of the changes made in the commit.
* Use the imperative, present tense: "change" not "changed" nor "changes"

### Exemples
```
fix(http_parser): use the correct HTTP response code for wrong version
 
Use the HTTP error 405 instead of 400 (Bad request) if the request contains an unsupported HTTP version.
```