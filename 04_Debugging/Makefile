CFLAGS = -Wall -O0 -g

.PHONY: all
all:	range

range:	range.o

.PHONY: test
test: range test1.gdb test2.gdb test1_gt.txt test2_gt.txt
		gdb -x test1.gdb --args ./range 1 12 | grep @ > test1_out.txt
		gdb -x test2.gdb --args ./range -100 100 3 | grep @ > test2_out.txt
		cmp test1_out.txt test1_gt.txt
		cmp test2_out.txt test2_gt.txt

.PHONY: clean
clean:
		rm -f *.o range test*_out.txt
