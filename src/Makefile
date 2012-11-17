include ../make.mk

OBJFILES = Filter.o Port.o

LIBFILE = ../bin/filter-graph.a

all: $(LIBFILE)

$(LIBFILE): $(BIN) $(OBJFILES)
	$(AR) -cru $(LIBFILE) $(OBJFILES)
	$(RANLIB) $(LIBFILE)

lint: *.cc *.h
	$(CPPLINT) --filter=-build/header_guard *.h *.cc

clean:
	-rm -f *.o *.a

depend:
	-$(CXX) -M $(CXXFLAGS) *.cc > .depend.mk  

-include .depend.mk

