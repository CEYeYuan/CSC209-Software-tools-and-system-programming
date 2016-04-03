#!/bin/sh
while read x;
	do echo "$x" | netcat -C 127.0.0.1 54248 ;
done < "test"