#!/bin/bash

root_dir=$(git rev-parse --show-toplevel)

# C files check
for file in $(find "$root_dir/src" -type f -name '*.c'); do
    function_count=$(grep -E '^(bool|int|char|double|void|float|struct [a-zA-Z][a-zA-Z_]*|unsigned|long)[[:space:]]+[*]*[a-zA-Z_][a-zA-Z0-9_]*[[:space:]]*\([^)]*\)[[:space:]]*' "$file" | wc -l)
    # echo "Processing file: $file"
    
    # Extract function prototypes and count parameters and lines
    grep -E '^(bool|int|char|double|void|float|struct [a-zA-Z][a-zA-Z_]*|unsigned|long)[[:space:]]+[*]*[a-zA-Z_][a-zA-Z0-9_]*[[:space:]]*\([^)]*\)[[:space:]]*' "$file" | while IFS= read -r line; do
        return_type=$(echo "$line" | awk '{print $1}')        
        function_name=$(echo "$line" | awk '{print $2}' | sed 's/([^)]*//g')

        # If the type of the function is a struct, then we need to remove the 'struct' keyword
        if [[ "$return_type" == "struct" ]]; then
            return_type=$(echo "$line" | awk '{print $1 " " $2}')
            function_name=$(echo "$line" | awk '{print $3}' | sed 's/([^)]*)//g')
        fi
        parameters=$(echo "$line" | sed 's/^[^(]*(//;s/)[^{]*$//;s/,/\n/g' | wc -l)

        if [[ "${function_name:0:1}" == "*" ]]; then
            function_name="\\$function_name"
        else
            function_name="$function_name"
        fi

        # Count the number of lines in the function (excluding blank and '{', '}' lines)
        lines_in_function=$(sed -n "/$return_type $function_name/,/^}/p" "$file" | sed '/^$/d' | sed '/^[[:space:]]*{$/d' | sed '/^[[:space:]]*}$/d' | wc -l)  
        lines_in_function=$((lines_in_function-1))

        if [[ "$parameters" -gt 4 ]]; then
            echo "Too many parameters in function: $function_name"
            echo "Parameters: $parameters"
        fi

        if [[ "$lines_in_function" -gt 40 ]]; then
            echo "Too many lines in function: $function_name"
            echo "Lines in function: $lines_in_function"
        fi

        # echo "Function: $return_type $function_name"
        # echo "Return type: $return_type"
        # echo "Parameters: $parameters"
        # echo "Lines in function: $lines_in_function"
        # echo "---------------------"

    done
    # echo "Total functions: $function_count"
    if [[ "$function_count" -gt 10 ]]; then
        echo "Too many functions in file: $file"
        echo "Total functions: $function_count"
    fi

    # Check for newline at EOF
    test "$(tail -c 1 "$file" | wc -l)" -eq 0 && echo "no newline at EOF in file: $file"
done

# Header files check
for header_file in $(find "$root_dir/src" -type f -name '*.h'); do
    # Get the header name without the path and extension
    header_name=$(basename "$header_file" .h)

    #put the header name in uppercase
    header_name=$(echo "$header_name" | tr '[:lower:]' '[:upper:]')
    header_name="${header_name}_H"

    # Check if the header file begins and ends correctly
    if ! grep -qE "#ifndef[[:space:]]*$header_name" "$header_file" || ! grep -qE "#define[[:space:]]*$header_name[[:space:]]*$" "$header_file" || ! grep -qE "#endif[[:space:]]*/\*.*$header_name.*\*/" "$header_file"; then
        echo "Header file $header_file is not correctly formatted."
        tail -n +9 "$header_file" | head -n 3   
        tail -n 1 "$header_file"
    fi

done
