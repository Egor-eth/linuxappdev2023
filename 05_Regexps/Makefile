CFLAGS = -Wall -O0 -g

.PHONY: all
all:	esub

esub:	esub.o

.PHONY: test
test: esub 
		./esub '([0-9]+)' '<\1>' 123 > test1_out.txt
		echo '123' | sed -E 's/([0-9]+)/<\1>/' > test1_gt.txt
		./esub '([0-9]+)([0-9]+)' '<\1>-\2' 'adasdadsadasd1234asdasd123123asdsa' > test2_out.txt
		echo 'adasdadsadasd1234asdasd123123asdsa' | sed -E 's/([0-9]+)([0-9]+)/<\1>-\2/' > test2_gt.txt
		cmp test1_out.txt test1_gt.txt
		cmp test2_out.txt test2_gt.txt

.PHONY: clean
clean:
		rm -f *.o esub test*_out.txt test*_gt.txt
