#!/bin/bash

while read line
do
    echo "$line"
done < <({($@) & THISPID=$! ; } && echo $THISPID && wait $THISPID)
