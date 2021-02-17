#!/bin/bash

days=({1..25})
parts=("" "b")
languages=(python:py haskell:bin c++:bin)

tmpdir=$(mktemp -d)
trap 'rm -rf "${tmpdir}"' EXIT

ret=0
declare -A results
for day in "${days[@]}"; do
    for part in "${parts[@]}"; do
        if [ "${day}${part}" == "25b" ]; then
            continue
        fi
        echo "Checking: Day ${day}${part}"
        for language in "${languages[@]}"; do
            dir=${language%:*}
            ext=${language#*:}
            file="day${day}${part}.${ext}"
            echo "  Executing: ${dir}/${file}"
            output=$(cd "${dir}" && "./${file}")
            if [ -z "${output}" ]; then
                output="No output!"
            fi
            results[${dir}]="${output}"
        done
        unset result_vals
        declare -A result_vals
        for dir in "${!results[@]}"; do
            result="${results[${dir}]}"
            result_vals[${result}]=1
        done
        if [ ${#result_vals[@]} -ne 1 ]; then
            echo "  ERROR: mismatch:"
            ret=1
            for dir in "${!results[@]}"; do
                echo -e "    ${dir}:\t${results[${dir}]}"
            done
        else
            echo "  OK"
        fi
    done
done

if [ $ret -eq 0 ]; then
    echo "OVERALL: OK"
else
    echo "OVERALL: MISMATCH"
fi

exit $ret
