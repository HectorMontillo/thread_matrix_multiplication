#!/bin/bash
echo "Bash version ${BASH_VERSION}..."
for i in {4..4000..40}
  do 
    start=`date +%s%3N`
    ./proccess $i
    end=`date +%s%3N`

    start2=`date +%s%3N`
    ./main $i
    end2=`date +%s%3N`

    echo $i `expr $end - $start`   `expr $end2 - $start2`  
done