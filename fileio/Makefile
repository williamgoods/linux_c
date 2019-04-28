path=/root/tlpi-dist/lib/

seek_io:
	- @gcc -I $(path) seek_io.c get_num.c -o seek_io

update_seek_io:
	- @make clean
	- @make seek_io

tee:
	- @gcc tee.c -o tee

update_tee:
	- @rm tee 
	- @make tee


clean:
	- @rm seek_io
