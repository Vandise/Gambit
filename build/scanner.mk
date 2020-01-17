SCANNERMODULEFILES := $(shell find $(SRCDIR)/modules/scanner $(SRCDIR)/modules/shared $(SRCDIR)/util -type f -name *.$(SRCEXT))

scanner:
	$(CC) $(CXXSTD) $(INC) $(DYNLIBPARAM) $(SCANNERMODULEFILES) -o $(LIBDIR)/scanner.so