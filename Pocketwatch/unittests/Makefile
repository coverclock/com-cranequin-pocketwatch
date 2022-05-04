#
# Pocketwatch Makefile
# <https://github.com/coverclock/com-cranequin-pocketwatch>
#
# Copyright 2022 Cranequin LLC, Colorado, USA.
# <http://cranequin.com>
#
# Licensed under the terms of the GNU LGPL version 2.1.
# <https://github.com/coverclock/com-cranequin-pocketwatch/blob/main/Pocketwatch/libraries/Pocketwatch/License-LGPL-21.pdf>
#
# This is a work of John Sloan.
# <mailto:jsloan@diag.com>
#

CXX	:= g++
PROJECT	:= Pocketwatch
PROGRAM	:= $(shell echo $(PROJECT) | tr '[A-Z]' '[a-z]')

.PHONY:	all

all:	$(PROGRAM)

$(PROGRAM):	$(PROJECT).cpp ../libraries/$(PROJECT)/$(PROJECT).h ../libraries/$(PROJECT)/$(PROJECT).cpp
	$(CXX) -o $@ $<
