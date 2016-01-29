ps    			---show the processes within current terminal
ps aux  		---show all the processes
|				---pipe: the output of one program become the input of another
>				---output result to a file
< 				---read from a standard input (i.e scanf, not argv)
wc 				---word count : newline, word, byte count
grep			---find pattern match
				e.g:
				 ps | grep pager
				13479 pts/0    00:00:00 pager
				13904 pts/0    00:00:00 pager
				14059 pts/0    00:00:00 pager
cut 			---split the words line by line: -d " "	use white space as delimitter
					-f select the fields
				e.g:

				(trusty)yeyuan@localhost:~/gitRepo/csc209/lecture$ ps
				  PID TTY          TIME CMD
				12891 pts/0    00:00:00 bash
				13469 pts/0    00:00:00 man
				13479 pts/0    00:00:00 pager
				13894 pts/0    00:00:00 man

				 ps | cut -d " " -f 2
				 		
				pts/0
				pts/0
				pts/0
				pts/0
				pts/0
				pts/0
				pts/0
