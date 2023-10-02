CFLAGS = -Wall -fPIC
GENERATES = prog README prog-a prog-so liboutput.so liboutput_static.a
TRASH = *.o *~ o.* tests

TEST1_ARGS=
TEST2_ARGS=arg
TEST3_ARGS=arg foo bar

all:	README prog prog-a prog-so

test:
		

prog:	const.o fun.o prog.o

prog-a: liboutput_static.a prog.o
		$(LINK.c) prog.o -L. -l:liboutput_static.a $(LOADLIBES) $(LDLIBS) -o prog-a

prog-so: liboutput.so prog.o
		$(LINK.c) prog.o -L. -l:liboutput.so $(LOADLIBES) $(LDLIBS) -o prog-so

liboutput_static.a: fun.o const.o
		$(AR) -rcs liboutput_static.a fun.o const.o

liboutput.so: fun.o const.o
		$(LINK.c) -shared $^ $(LOADLIBES) $(LDLIBS) -o $@

README: prog
		./$< 2> $@

test: test_1 test_2 test_3

test_1: prog-a prog-so prog
		mkdir -p tests
		./prog $(TEST1_ARGS) > tests/prog_1.txt 2>/dev/null
		LD_LIBRARY_PATH=`pwd` ./prog-so $(TEST1_ARGS) > tests/prog_1-so.txt 2>/dev/null
		./prog-a $(TEST1_ARGS) > tests/prog_1-a.txt 2>/dev/null
		cmp tests/prog_1.txt tests/prog_1-a.txt 
		cmp tests/prog_1.txt tests/prog_1-so.txt

test_2: prog-a prog-so prog
		mkdir -p tests
		./prog $(TEST2_ARGS) > tests/prog_2.txt 2>/dev/null
		LD_LIBRARY_PATH=`pwd` ./prog-so $(TEST2_ARGS) > tests/prog_2-so.txt 2>/dev/null
		./prog-a $(TEST2_ARGS) > tests/prog_2-a.txt 2>/dev/null
		cmp tests/prog_2.txt tests/prog_2-a.txt 
		cmp tests/prog_2.txt tests/prog_2-so.txt

test_3: prog-a prog-so prog
		mkdir -p tests
		./prog $(TEST3_ARGS) > tests/prog_3.txt 2>/dev/null
		LD_LIBRARY_PATH=`pwd` ./prog-so $(TEST3_ARGS) > tests/prog_3-so.txt 2>/dev/null
		./prog-a $(TEST3_ARGS) > tests/prog_3-a.txt 2>/dev/null
		cmp tests/prog_3.txt tests/prog_3-a.txt 
		cmp tests/prog_3.txt tests/prog_3-so.txt


fun.o:  outlib.h

.PHONY: clean
clean:
		rm -rf $(TRASH)

.PHONY: distclean
distclean:      clean
		rm -rf $(GENERATES)