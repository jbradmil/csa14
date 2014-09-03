#! /bin/bash

shopt -s nullglob

for file in inputs/*.list
do
    noext=${file%.list}
    ./scripts/make_reduced_tree.exe -i "$file" -l -n 100000 &> "logs/make_reduced_tree_${noext##*/}.log" &
done

wait
echo "Done"
exit 0;
