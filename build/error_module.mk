ERRORMODULEFILES := $(shell find $(SRCDIR)/modules/error -type f -name *.$(SRCEXT))

error_module:
	$(CC) $(CXXSTD) $(INC) $(DYNLIBPARAM) $(ERRORMODULEFILES) -o $(LIBDIR)/error.so