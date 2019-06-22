all:server client

server:
	- @gcc -I /root/tlpi-dist/ -o server is_seqnum_sv.c read_line.c /root/tlpi-dist/lib/get_num.c 

client:
	- @gcc is_seqnum_cl.c read_line.c -o client 

update:
	- @make clean 
	- @make all	

socketpair:
	- @gcc setpair.c -o socketpair

clean:
	- @rm server client
