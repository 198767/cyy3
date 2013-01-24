SRCS=$(wildcard *.c)
BIN=add
DEPS=$(patsubst %.c,%.d,$(SRCS))
OBJS=$(patsubst %.c,%.o,$(SRCS))
CC=gcc
CFLAGS=-O2 -Wall -g -I/usr/local/include/large_number -I/usr/local/include/vector
LDFLAGS=-lmysqlclient -llarge_number -L/usr/local/lib/large_number -lvector -L/usr/local/lib/vector
.PHONY:all test clean install
all:
	-make $(BIN)
	-$(RM) $(DEPS) $(OBJS)
$(BIN):$(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@
clean:
	-$(RM) $(BIN) $(OBJS) $(DEPS) src/*~ 
	clear
%.d: %.c 
	@set -e; rm -f $@; \
	$(CC) -M $(CFLAGS) $< | sed 's,\($(notdir $*)\)\.o[ :]*,$(dir $*)\1.o $@ : ,g' > $@; 
	-include $(DEPS)
