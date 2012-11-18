EXTRA_CXXFLAGS = -I/opt/local/include -lgtest -L/opt/local/lib ../bin/filter-graph.a

include ../make.mk

BINFILE = ../bin/testsuite
LIBFILE = ../bin/filter-graph.a

all: $(BINFILE)

$(BINFILE): *.cc $(LIBFILE)
	$(CXX) $(CXXFLAGS) *.cc -o $(BINFILE)

lint: 

depend:
	$(CXX) -M $(CXXFLAGS) *.cc > .depend.mk

clean:
	-rm -f $(BINFILE)

-include .depend.mk
