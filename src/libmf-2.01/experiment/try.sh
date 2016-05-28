#!/bin/bash
for k in $( seq 1 10 )
do
   mkdir /home/aaa${k}
   cd /home/aaa${k}
   for l in $( seq 1 10 )
   do
   mkdir bbb${l}
   cd /home/aaa${k}
   done
   cd ..
done
