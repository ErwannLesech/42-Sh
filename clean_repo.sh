#!/bin/sh

# Remove all comments and empty lines from .gitignore
IGNORED_FILES=$(cat .gitignore | sed 's/^#.*$//g' | sed '/^\s*$/d')

# Remove all ignored files
for file in $IGNORED_FILES
do
    rm -rf $file
done

find . -name "Makefile" -type f -delete
find . -name "Makefile.in" -type f -delete
find . -name "*.a" -type f -delete
find . -name "*.o" -type f -delete

./tests/testsuite.sh -clean
