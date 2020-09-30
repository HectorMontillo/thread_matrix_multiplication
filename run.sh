#!/bin/bash
echo "Bash version ${BASH_VERSION}..."
echo "n seq seq(memory) fork fork(memory) thread thread(memory)"
for n in {1..5}
  do
    echo "Bench #$n"
    for i in {4..800..40}
      do 
        start=`date +%s%3N`
        ./seq $i
        end=`date +%s%3N`

        start2=`date +%s%3N`
        ./seqMM $i
        end2=`date +%s%3N`

        start3=`date +%s%3N`
        ./proccess $i
        end3=`date +%s%3N`

        start4=`date +%s%3N`
        ./proccessMM $i
        end4=`date +%s%3N`

        start5=`date +%s%3N`
        ./thread $i
        end5=`date +%s%3N`

        start6=`date +%s%3N`
        ./threadMM $i
        end6=`date +%s%3N`

        echo $i `expr $end - $start` `expr $end2 - $start2`  `expr $end3 - $start3` `expr $end4 - $start4`  `expr $end5 - $start5`  `expr $end6 - $start6`
done
done