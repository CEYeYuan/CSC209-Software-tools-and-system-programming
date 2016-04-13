echo files in my dir are *
echo file in my parent dir are ../*
echo file in $HOME are *    #dereference $HOME and *
echo file in $HOME are $HOME/*		#working	
echo "file in $HOME are $HOME/*"		#dereference $HOME ONLY	
echo 'file in $HOME are $HOME/*'		#print as the string literal
echo "Files in $HOME are `ls`"			#replace `ls` by its stdout
wc -w 01_linux.c
`wc -w 01_linux.c`						#error, interpret the whole as the cmd

set one two three
(trusty)yeyuan@localhost:~/gitRepo/csc209/lecture$ echo $1 $2 $3
one two three

(trusty)yeyuan@localhost:~/gitRepo/csc209/lecture$ set `date`
(trusty)yeyuan@localhost:~/gitRepo/csc209/lecture$ date  
Wed Apr  6 19:11:01 EDT 2016
(trusty)yeyuan@localhost:~/gitRepo/csc209/lecture$ echo $1
Wed
(trusty)yeyuan@localhost:~/gitRepo/csc209/lecture$ shift
(trusty)yeyuan@localhost:~/gitRepo/csc209/lecture$ echo $1
Apr
(trusty)yeyuan@localhost:~/gitRepo/csc209/lecture$ shift
(trusty)yeyuan@localhost:~/gitRepo/csc209/lecture$ echo $1
6
(trusty)yeyuan@localhost:~/gitRepo/csc209/lecture$ 



