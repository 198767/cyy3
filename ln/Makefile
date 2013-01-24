PKG_NAME=ln
TARGET_LIB=libln.so
SRCS=$(wildcard *.c)
DEPS=$(patsubst %.c,%.d,$(SRCS))
OBJS=$(patsubst %.c,%.o,$(SRCS))
HEADERS=$(wildcard *.h)
CC=gcc
CFLAGS= -O2 -Wall -g -fPIC 
LDFLAGS=-shared 
MAJOR_VERSION=1
MINOR_VERSION=1
.PHONY:all clean install
all:
	-make $(TARGET_LIB)
	-$(RM) $(DEPS) $(OBJS) *~
$(TARGET_LIB):$(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@
clean:
	-$(RM) $(TARGET_LIB) $(DEPS) $(OBJS) *~
	clear
install:all
	install -D $(TARGET_LIB) $(DESTDIR)/usr/local/lib/$(PKG_NAME)/$(TARGET_LIB).$(MAJOR_VERSION).$(MINOR_VERSION)
	ln -fs $(DESTDIR)/usr/local/lib/$(PKG_NAME)/$(TARGET_LIB).$(MAJOR_VERSION).$(MINOR_VERSION) $(DESTDIR)/usr/local/lib/$(PKG_NAME)/$(TARGET_LIB)
	install -d $(DESTDIR)/usr/local/include/$(PKG_NAME)
	install -t $(DESTDIR)/usr/local/include/$(PKG_NAME) $(HEADERS)
	echo "/usr/local/lib/$(PKG_NAME)" > /etc/ld.so.conf.d/$(PKG_NAME).conf
	ldconfig
%.d:%.c
	@set -e; $(RM) $@; \
	$(CC) $(CFLAGS) -M $< | sed 's,\($(notdir $*)\)\.o[ :]*,$(dir $*)\1.o $@ : ,g' > $@;
-include $(DEPS)
