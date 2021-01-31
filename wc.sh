#!/bin/bash

days=($(seq 9))
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
            echo -n $(wc -l ${dir}/day${day}${part}.${ext} | awk '{print $1}')
            echo -n ", "
        done
        echo
    done
done
