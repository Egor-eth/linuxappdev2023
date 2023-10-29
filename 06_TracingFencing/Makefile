CFLAGS = -Wall -O0 -g

.PHONY: all
all:	move inj_remove.so

move:	move.o

inj_remove.so: inj_remove.o
		$(LINK.c) -shared $^ $(LOADLIBES) $(LDLIBS) -o $@

.PHONY: test
test:	move inj_remove.so
		cp test1_gt.txt test1_PROTECT1_in.txt
		cp test1_gt.txt test1_PROTECT2_in.txt
		cp test1_gt.txt test1_clean_in.txt
		LD_PRELOAD=`pwd`/inj_remove.so ./move test1_PROTECT1_in.txt test1_inj_out.txt
		./move test1_PROTECT2_in.txt test1_no_inj_out.txt
		LD_PRELOAD=`pwd`/inj_remove.so ./move test1_clean_in.txt test1_inj_clean_out.txt


		cp test2_gt.txt test2_in.txt
		! strace -e trace=write -e inject=write:error=EBADF:when=1 ./move test2_in.txt test2_out.txt

		cp test3_gt.txt test3_in.txt
		! strace -e trace=read -e inject=read:error=EBADF:when=1 ./move test3_in.txt test3_out.txt

		cp test4_gt.txt test4_in.txt
		! strace -e trace=close -e inject=close:error=EBADF:when=3 ./move test4_in.txt test4_out.txt

		cp test5_gt.txt test5_in.txt
		! strace -e trace=close -e inject=close:error=EBADF:when=4 ./move test5_in.txt test5_out.txt

		cmp test1_inj_out.txt test1_gt.txt
		cmp test1_no_inj_out.txt test1_gt.txt
		ls test1_PROTECT1_in.txt
		! ls test1_PROTECT2_in.txt
		! ls test1_clean_in.txt

		! ls test2_out.txt
		ls test2_in.txt

		! ls test3_out.txt 
		ls test3_in.txt

		ls test4_in.txt

		ls test5_out.txt




.PHONY: clean
clean:
		rm -f *.o *.so move test*_in.txt test*_out.txt *.log
