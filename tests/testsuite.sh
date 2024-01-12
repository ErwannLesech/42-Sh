#!/bin/bash

RED='\e[31m'
GREEN='\e[32m'
YELLOW='\e[33m'
BLUE='\e[34m'
TURQUOISE='\e[36m'
WHITE='\e[0m'

TOTAL_RUN=0
TOTAL_FAIL=0

MODULE_RUN=0
MODULE_FAIL=0

ref_file_out="/tmp/.ref_file_out"
ref_file_err="/tmp/.ref_file_err"
my_file_out="/tmp/.my_file_out"
my_file_err="/tmp/.my_file_err"

ex=../../../src/42sh

run_test()
{
    [ -e "$1" ] || echo "Missing test file $1" 1>&2
    success=true
    MODULE_RUN=$((MODULE_RUN+1))
    TOTAL_RUN=$((TOTAL_RUN+1))
    
    echo -ne "$BLUE-->> ${WHITE}$1...$WHITE"
    bash --posix "$1" > "$ref_file_out" 2> "$ref_file_err"
    REF_CODE=$?

    "$PWD/$ex" "$1" > "$my_file_out" 2> "$my_file_err"
    MY_CODE=$?

    diff -u "$ref_file_out" "$my_file_out" > $1.diff
    DIFF_CODE=$?

    if [ $REF_CODE -ne $MY_CODE ]; then
        echo -ne "$RED RETURNS $WHITE"
        success=false
    fi

    if [ $DIFF_CODE -ne 0 ]; then
        echo -ne "$RED STDOUT $WHITE"
        success=false
    fi

    if { [ -s $ref_file_err ] && [ ! -s $my_file_err ]; } || { [ ! -s $ref_file_err ] && [ -s $my_file_err ]; }; then
        echo -ne "$RED STDERR $WHITE"
        success=false
    fi

    if $success; then
        echo -e "$GREEN OK $WHITE"
        rm -f $1.diff
    else
        [ -s "$(realpath $1.diff)" ] && echo -ne "$RED (cat $(realpath $1.diff)) $WHITE"
        echo 
        TOTAL_FAIL=$((TOTAL_FAIL+1))
        MODULE_FAIL=$((MODULE_FAIL+1))
    fi
}

run_module()
{
    echo "$1"
    cd "$1"
    source ./testsuite.sh
    cd - > /dev/null

    if [ $MODULE_FAIL -eq 0 ]; then
        echo -e "$GREEN=============================="
        printf " $WHITE Test accuracy $GREEN%s$WHITE/$GREEN%s\n" "$((MODULE_RUN-MODULE_FAIL))" "$MODULE_RUN"
        echo -e "$GREEN==============================$WHITE"
    else
        echo -e "$RED=============================="
        printf " $WHITE Test accuracy $RED%s$WHITE/$RED%s\n" "$((MODULE_RUN-MODULE_FAIL))" "$MODULE_RUN"
        echo -e "$RED==============================$WHITE"
    fi

    MODULE_RUN=0
    MODULE_FAIL=0
}

run_testsuite() 
{
    for module in $@; do

        [ ${module} = "." ] && continue
        
        echo -e "$TURQUOISE=============================="
        printf " $WHITE%-36s $TURQUOISE%s\n" "$module"
        echo -e "$TURQUOISE==============================$WHITE"

        for submodule in $(find $module -type d); do
            if [ $submodule = "$module" ]; then
                continue
            fi
            run_module "$submodule"
        done
    done

    if [ $TOTAL_FAIL -eq 0 ]; then
        echo -e "$GREEN=============================="
        printf " $WHITE Overall test accuracy $GREEN%s$WHITE/$GREEN%s\n" "$((TOTAL_RUN-TOTAL_FAIL))" "$TOTAL_RUN"
        echo -e "$GREEN==============================$WHITE"
    else
        echo -e "$RED=============================="
        printf " $WHITE Overall test accuracy $RED%s$WHITE/$RED%s\n" "$((TOTAL_RUN-TOTAL_FAIL))" "$TOTAL_RUN"
        echo -e "$RED==============================$WHITE"
    fi
    PERCENT_SUCCESS=$(((TOTAL_RUN-TOTAL_FAIL)*100/TOTAL_RUN))

    echo -e "$BLUE=============================="
    echo -e "$WHITE    42Sh Moulinette: $([ $PERCENT_SUCCESS -eq 100 ] && echo -e "$GREEN" || echo "$RED") $PERCENT_SUCCESS%"
    echo -e "$BLUE=============================="
}

main()
{
    if [ $# -eq 0 ]; then
        run_testsuite $(find . -maxdepth 1 -type d)
    else
        if [ $1 = "--help" ]; then
            echo -e "${TURQUOISE}Usage: ./testsuite.sh [MODULE]$WHITE"
            echo "Run all tests from the specified MODULE"
            echo "If no MODULE is specified, run all testsuites"
            echo -e "$RED=============================="
            echo -e "${TURQUOISE}Usage: ./testsuite.sh -clean$WHITE"
            echo "Remove all .diff files"
            exit 0
        else
            if [ $1 = "-clean" ]; then
                echo -e "$TURQUOISE===================================$WHITE"
                printf " $WHITE%-36s $TURQUOISE%s\n" "Removing all .diff files"
                echo -e "$TURQUOISE==================================="
                
                for module in $(find . -type d); do
                    rm -f $module/*.diff
                done

                exit 0
            else       
                echo -e "$TURQUOISE==================================="
                printf " $WHITE%-36s $TURQUOISE%s\n" "$1"
                echo -e "$TURQUOISE===================================$WHITE"

                run_module "$1"
            fi
        fi
    fi
    exit 0
}

main $@

rm -f "$ref_file_out" "$ref_file_err" "$my_file_out" "$my_file_err"
