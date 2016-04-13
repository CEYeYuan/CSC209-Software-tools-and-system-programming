#!/bin/sh
iterator=`seq 4`
for i in $iterator
do
	echo $i>out.$i
done

