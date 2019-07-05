SCANNERMODULEFILES := $(shell find $(SRCDIR)/modules/scanner $(SRCDIR)/shared $(SRCDIR)/util -type f -name *.$(SRCEXT))

scanner:
	$(CC) $(CXXSTD) $(INC) $(DYNLIBPARAM) $(SCANNERMODULEFILES) -o $(LIBDIR)/scanner.so