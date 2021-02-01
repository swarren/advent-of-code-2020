#!/bin/bash

days=($(seq 25))
parts=("" "b")
languages=(python:py haskell:hs c++:cpp)

echo -n "day, "
for language in "${languages[@]}"; do
    dir=${language%:*}
    ext=${language#*:}
    echo -n "${dir}, "
done
echo

for day in "${days[@]}"; do
    for part in "${parts[@]}"; do
        echo -n "${day}${part}, "
        for language in "${languages[@]}"; do
            dir=${language%:*}
            ext=${language#*:}
            file="${dir}/day${day}${part}.${ext}"
            if [ -f "${file}" ]; then
                echo -n $(wc -l ${dir}/day${day}${part}.${ext} | awk '{print $1}')
            else
                echo -n "0"
            fi
            echo -n ", "
        done
        echo
    done
done
