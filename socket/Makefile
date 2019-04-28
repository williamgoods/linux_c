all:server client

server:
	- @gcc us_ucase_sv.c -o server

client:
	- @gcc us_ucase_cl.c -o client

update:
	- @make clean 
	- @make all	

socketpair:
	- @gcc setpair.c -o socketpair

clean:
	- @rm server client
