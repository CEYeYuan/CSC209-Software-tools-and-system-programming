#!/bin/sh

P=`wc -w <01_linux.c`
Q=`wc -w <02_input_from_file.c`
if test $P -gt $Q
then
	echo $P P
else 
	echo $Q Q
fi