SRCS=$(wildcard *.c)
BINS=$(patsubst %.c,%,$(SRCS))
DEPS=$(patsubst %.c,%.d,$(SRCS))
OBJS=$(patsubst %.c,%.o,$(SRCS))
CC=gcc
CFLAGS=-O2 -Wall -g -I/usr/local/include/large_number
LDFLAGS=-lmysqlclient -llarge_number -L/usr/local/lib/large_number
.PHONY:all test clean install
all:
	-make $(BINS)
	-$(RM) $(DEPS) $(OBJS)
clean:
	-$(RM) $(BIN) $(OBJS) $(DEPS) src/*~ 
	clear
%.d: %.c 
	@set -e; rm -f $@; \
	$(CC) -M $(CFLAGS) $< | sed 's,\($(notdir $*)\)\.o[ :]*,$(dir $*)\1.o $@ : ,g' > $@; 
	-include $(DEPS)
