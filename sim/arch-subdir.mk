## Fragment to pass common settings into arch subdir builds.
## NB: This should eventually be merged into sim/Makefile.am once there are no
## subdir configure scripts.
#
#   Copyright (C) 1993-2023 Free Software Foundation, Inc.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

AM_V = $(V)

EXEEXT = 

LN_S = ln -s

CC = gcc
C_DIALECT = 
CC_FOR_BUILD = gcc
CFLAGS_FOR_BUILD = -g -O2
CPPFLAGS_FOR_BUILD = $(CPPFLAGS)
CFLAGS = -g -O2    
CPPFLAGS = 
LDFLAGS =  

AR = ar --plugin /usr/lib/gcc/x86_64-pc-linux-gnu/12.2.1/liblto_plugin.so --plugin /usr/lib/gcc/x86_64-pc-linux-gnu/12.2.1/liblto_plugin.so
AR_FLAGS = rc
RANLIB = ranlib --plugin /usr/lib/gcc/x86_64-pc-linux-gnu/12.2.1/liblto_plugin.so --plugin /usr/lib/gcc/x86_64-pc-linux-gnu/12.2.1/liblto_plugin.so

LIBTOOL = $(SHELL) $(top_builddir)/libtool
AM_V_lt = $(am__v_lt_$(V))
am__v_lt_ = $(am__v_lt_$(AM_DEFAULT_VERBOSITY))
am__v_lt_0 = --silent
am__v_lt_1 = 

DEPMODE = depmode=gcc3
DEPDIR = .deps

SDL_CFLAGS = 
TERMCAP_LIB = -ltinfo
READLINE_LIB = ../readline/readline/libreadline.a
READLINE_CFLAGS = -I$(READLINE_SRC)/..

COMMON_LIBS = -ltinfo -lm 
WARN_CFLAGS =  -Wall -Wdeclaration-after-statement -Wpointer-arith -Wno-unused -Wunused-value -Wunused-function -Wno-switch -Wno-char-subscripts -Wempty-body -Wunused-but-set-parameter -Wno-error=maybe-uninitialized -Wmissing-declarations -Wmissing-prototypes -Wdeclaration-after-statement -Wmissing-parameter-type -Wpointer-sign -Wold-style-declaration
WERROR_CFLAGS = -Werror

# Maintainer support, enable with --enable-maintainer-mode.
# Once all subdirs are moved to automake, we can drop this custom logic and
# switch back to using # directly.
MAINT = ; @true
# The following line is commented depending upon --enable-maintainer-mode.
#MAINT =

SIM_INLINE = -DDEFAULT_INLINE=0

SIM_HW_CFLAGS = -DWITH_HW=1 -DHAVE_DV_SOCKSER
