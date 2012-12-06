#Target
TGT = Debug
#Project Name
PROJNAME = NAV-CODE

# Compiler
CC = g++
OPTS = -c -Wall

#Target
ifeq ($(TGT), Release)
   OPTS	:= -O2
endif

# Project Destination
PROJECT = bin/$(TGT)/$(PROJNAME)

# Directories
OBJDIR = obj/$(TGT)/src
SRCDIR = src
INSTDIR = ~/bin

# Libraries
LIBS = 

# Files and folders
SRCS    = $(shell find $(SRCDIR) -name '*.cpp')
#SRCDIRS = $(shell find . -name '*.cpp' | dirname {} | sort | uniq | sed 's/\/$(SRCDIR)//g' )
SRCDIRS = $(shell find $(SRCDIR) -type d | sed 's/$(SRCDIR)/./g' )
OBJS    = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))

# Targets
$(PROJECT): buildrepo $(OBJS)
	$(CC) $(OBJS) $(LIBS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(OPTS) -c $< -o $@
	
clean:
	rm $(PROJECT) $(OBJDIR) -Rf
	
buildrepo:
	@$(call make-repo)

install:
	cp $(PROJECT) $(INSTDIR)
# Create obj directory structure
define make-repo
	mkdir -p $(OBJDIR)
	for dir in $(SRCDIRS); \
	do \
		mkdir -p $(OBJDIR)/$$dir; \
	done
endef
