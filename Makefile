##############################################################################
##                              mangledeggslib                              ##
##                                                                          ##
##                    Copyright © 2020 Alexander Nicholi                    ##
##                       Released under BSD-2-Clause.                       ##
##############################################################################

.PHONY: default cli lib

default: cli

cli: lib
	$(MAKE) -f etc/cli.mk -C etc

lib:
	$(MAKE) -f etc/lib.mk -C etc
