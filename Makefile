BIN = bin
SUBDIRS = src

all: $(SUBDIRS)

$(BIN):
	-mkdir bin

$(OBJFILES):
	$(CXX) $^ -o $@

%.o: src/%.cc src/%.h
	$(CXX) $< -o $@

depend:
	-for x in $(SUBDIRS); do $(MAKE) -C $$x depend; done

lint:
	-for x in $(SUBDIRS); do $(MAKE) -C $$x lint; done

clean:
	-for x in $(SUBDIRS); do $(MAKE) -C $$x clean; done
	-rm -f bin/*

.PHONY: $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@

-include .depend.mk
