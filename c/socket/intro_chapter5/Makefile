cc = gcc -c -g  
target = tcpser1
obj = tcpserv01.o

tcpser1:
$(target):$(obj)
	     $(cc) $(obj) -Wall -o $(target)
tcpserv01.o:tcpserv01.c
	 		 $(cc) -c tcpserv01.c

target_cli1 = tcpcli01
obj_cli1 = tcpcli01.o

tcpcli1:
	$(target_cli1):$(obj_cli1)
			$(cc) $(obj_cli1) -Wall -o $(target_cli1)
tcpcli01.o:tcpcli01.c
	$(cc) -c tcpcli01.c

clean:
	  rm -rf $(obj) $(obj_cli1)
		rm -rf $(target) $(target_cli1)


