# Makefile tymczasowo ukradziony z internetów i adaptowany delikatnie na nasz
# potrzeby [http://ubuntuforums.org/showthread.php?t=1204739]

TARGET   = main

SRCEXT   = cpp
SRCDIR   = src
OBJDIR   = obj
BINDIR   = bin

SRCS    := $(shell find $(SRCDIR) -name '*.$(SRCEXT)')
SRCDIRS := $(shell find . -name '*.$(SRCEXT)' -exec dirname {} \; | uniq)
OBJS    := $(patsubst %.$(SRCEXT),$(OBJDIR)/%.o,$(SRCS))

DEBUG    = -g
INCLUDES = -I./inc
CFLAGS   = -Wall -pedantic -c $(DEBUG) $(INCLUDES)
LDFLAGS  = -lssl -lcrypto

ifeq ($(SRCEXT), cpp)
CC      = $(CXX)
CFLAGS += -std=c++11
else
CFLAGS  += -std=gnu99
endif

.PHONY: all clean distclean


all: $(BINDIR)/$(TARGET)

$(BINDIR)/$(TARGET): buildrepo $(OBJS)
	@mkdir -p `dirname $@`
	@echo "Linking $@..."
	@$(CC) $(OBJS) $(LDFLAGS) -o $@

$(OBJDIR)/%.o: %.$(SRCEXT)
	@echo "Generating dependencies for $<..."
	@$(call make-depend,$<,$@,$(subst .o,.d,$@))
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) $< -o $@

clean:
	$(RM) -r $(OBJDIR)

distclean: clean
	$(RM) -r $(BINDIR)

buildrepo:
	@$(call make-repo)

define make-repo
   for dir in $(SRCDIRS); \
   do \
	mkdir -p $(OBJDIR)/$$dir; \
   done
endef


# usage: $(call make-depend,source-file,object-file,depend-file)
define make-depend
  $(CC) -MM       \
        -MF $3    \
        -MP       \
        -MT $2    \
        $(CFLAGS) \
        $1
endef
