# =========================================================================
#     This makefile was generated by
#     Bakefile 0.2.8 (http://www.bakefile.org)
#     Do not modify, all changes will be overwritten!
# =========================================================================



# -------------------------------------------------------------------------
# These are configurable options:
# -------------------------------------------------------------------------



# -------------------------------------------------------------------------
# Do not modify the rest of this file!
# -------------------------------------------------------------------------

### Variables: ###


### Conditionally set variables: ###



### Targets: ###

all: test

install: 

uninstall: 

clean: 
	rm -f ./*.o
	rm -f ./*.d
	-(cd test && $(MAKE) clean)

test: 
	(cd test && $(MAKE) all)

.PHONY: all install uninstall clean test


# Dependencies tracking:
-include ./*.d