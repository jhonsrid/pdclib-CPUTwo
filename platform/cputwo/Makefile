# platform/cputwo/Makefile — build libpdclib.a for CPUTwo bare-metal
#
# Usage:
#   make                       # build with host gcc (for initial testing)
#   make CC=cputwo-tcc         # cross-compile with cputwo-tcc
#   make CC=cputwo-tcc AR=ar   # same, explicit ar
#
# The platform directory overlays the generic sources: any .c file that
# exists under platform/cputwo/functions/ shadows the corresponding file
# in functions/.  This is the "copy platform over source tree" model from
# Quickstart.txt, implemented without actually copying files.

THISDIR  := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
TOPDIR   := $(abspath $(THISDIR)../../)/

CC      ?= gcc
AR      ?= ar
RANLIB  ?= ranlib

# When building with TCC, use TCC's built-in ar mode so the ELF symbol
# table is generated correctly (macOS ar/ranlib don't understand ELF).
ifneq ($(findstring tcc,$(CC)),)
  AR     := $(CC) -ar
  RANLIB := :
endif

CFLAGS  += -std=c99 -Wall -Wextra \
            -I$(TOPDIR)include \
            -I$(THISDIR)include \
            -D_PDCLIB_BUILD \
            -D_PDCLIB_STATIC_DEFINE

# dlmalloc uses #include "pdclib/_PDCLIB_config.h", so it needs include/ in path.
# Use -w to suppress any redefinition warnings from mixing pdclib + system headers.
DLMALLOC_CFLAGS := $(filter-out -O% -Wall -Wextra,$(CFLAGS)) -w \
                    -include stddef.h \
                    -DMORECORE=sbrk \
                    -DHAVE_MORECORE=1 \
                    -DHAVE_MMAP=0 \
                    -DLACKS_SYS_TYPES_H \
                    -DLACKS_FCNTL_H \
                    -DLACKS_UNISTD_H \
                    -DLACKS_SYS_PARAM_H \
                    -DLACKS_SYS_MMAN_H \
                    -DLACKS_STRINGS_H \
                    -DLACKS_SCHED_H \
                    -DUSE_LOCKS=0 \
                    -DMALLINFO_FIELD_TYPE=unsigned

TARGET  := libpdclib.a
CRT0    := crt0.o
OBJDIR  := obj

# --------------------------------------------------------------------------
# Source collection
# --------------------------------------------------------------------------

# All platform-specific sources (relative path from functions root)
PLAT_SRCS_ABS  := $(shell find "$(THISDIR)functions" -name '*.c')
PLAT_RELPATHS  := $(PLAT_SRCS_ABS:$(THISDIR)functions/%=%)

# All generic sources (absolute paths)
GEN_SRCS_ABS   := $(shell find "$(TOPDIR)functions" -name '*.c')
GEN_RELPATHS   := $(GEN_SRCS_ABS:$(TOPDIR)functions/%=%)

# Exclude dlmalloc from default build: it requires sbrk (provided by platform)
# and conflicts with host system headers during test builds.
# Enable with: make WITH_DLMALLOC=1
DLMALLOC_REL   := _dlmalloc/malloc.c
GEN_RELPATHS_NODL  := $(filter-out $(DLMALLOC_REL), $(GEN_RELPATHS))
PLAT_RELPATHS_NODL := $(filter-out $(DLMALLOC_REL), $(PLAT_RELPATHS))

# Generic sources that are NOT overridden by the platform
FILTERED_GEN   := $(filter-out $(PLAT_RELPATHS_NODL), $(GEN_RELPATHS_NODL))

# --------------------------------------------------------------------------
# Object lists
# --------------------------------------------------------------------------

GEN_OBJS    := $(addprefix $(OBJDIR)/gen/,   $(FILTERED_GEN:.c=.o))
PLAT_OBJS   := $(addprefix $(OBJDIR)/plat/,  $(PLAT_RELPATHS_NODL:.c=.o))

ifdef WITH_DLMALLOC
DLMALLOC_OBJ := $(OBJDIR)/gen/$(DLMALLOC_REL:.c=.o)
else
DLMALLOC_OBJ :=
endif

ALL_OBJS    := $(GEN_OBJS) $(PLAT_OBJS) $(DLMALLOC_OBJ)

# --------------------------------------------------------------------------
# Rules
# --------------------------------------------------------------------------

.PHONY: all clean

all: $(CRT0) $(TARGET)

$(TARGET): $(ALL_OBJS)
	$(AR) rcs $@ $^
	$(RANLIB) $@

# C runtime startup (assembled separately, not archived into libpdclib.a)
$(CRT0): $(THISDIR)crt0.S
	$(CC) $(CFLAGS) -c $< -o $@

# Generic sources (normal CFLAGS)
$(OBJDIR)/gen/%.o: $(TOPDIR)functions/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# compiler-rt builtins (must use -O0 to prevent the compiler from lowering
# 64-bit operations back into recursive calls to the very builtins we define)
COMPILERRT_DIR := $(THISDIR)functions/_compiler_rt
COMPILERRT_CFLAGS := $(filter-out -O% -Wall -Wextra,$(CFLAGS)) -w -O0 -I$(COMPILERRT_DIR)

$(OBJDIR)/plat/_compiler_rt/%.o: $(COMPILERRT_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(COMPILERRT_CFLAGS) -c $< -o $@

# Platform sources (normal CFLAGS, overrides generic)
$(OBJDIR)/plat/%.o: $(THISDIR)functions/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# dlmalloc (special CFLAGS)
$(OBJDIR)/gen/_dlmalloc/malloc.o: $(TOPDIR)functions/_dlmalloc/malloc.c
	@mkdir -p $(dir $@)
	$(CC) $(DLMALLOC_CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(TARGET) $(CRT0)
