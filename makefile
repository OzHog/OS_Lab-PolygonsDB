


CFLAGS = -Wall
CC = gcc -std=c99
ECHO = echo "going to compile for target $@"
OS := $(shell uname)


ifneq (,$(findstring CYGWIN_NT,$(OS)))
EXT = .exe
else
EXT =
endif

PROG = ex2_q1$(EXT)
HELPER = ex2_q1_helper$(EXT)

all: $(HELPER) $(PROG) test

print:
	@echo MAIN_O: $(PROG)

clean:
	#rm -r $(PWD)/$(PROG)
	rm -vf *.o $(PROG) $(HELPER) *.log *.tmp

test:
	@echo going to run test...
	./$(PROG) < in.txt > out.log

$(PROG): ex2_q1.c
	$(ECHO)
	$(CC) $(CFLAGS) $^ -o $@

$(HELPER): ex2_q1_helper.c
	$(ECHO)
	$(CC) $(CFLAGS) $^ -o $@