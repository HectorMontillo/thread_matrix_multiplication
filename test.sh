#!/bin/bash
echo "Bash version ${BASH_VERSION}..."

for n in {1..5}
  do
    echo "Bench #$n"
    printf "n\tseq\tseq(mm)\tthr\tthr(mm)\tfork\tfork(mm)\n"
    for i in {4..800..40}
      do 
        printf "%s\t" $i
        ./seq $i
        ./seqMM $i
        ./thread $i
        ./threadMM $i
        ./proccess $i
        ./proccessMM $i

done
done