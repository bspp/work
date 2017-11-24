cc = gcc -g 
target = collectd
obj = plugin.o types_list.o utils_avltree.o common.o collectd.o file.o merge.o
$(target):$(obj)
	    $(cc) $(obj) -Wall -o $(target)
plugin.o:plugin.c
	$(cc) -c plugin.c
types_list.o:types_list.c
	$(cc) -c types_list.c
utils_avltree.o:utils_avltree.c
	$(cc) -c utils_avltree.c
common.o:common.c
	$(cc) -c common.c
collectd.o:collectd.c
	$(cc) -c collectd.c
file.o:file.c
	$(cc) -c file.c
merge.o:merge.c
	$(cc) -c merge.c
clean:
	rm -rf $(obj)
	rm -rf *.csv
