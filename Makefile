BIN = bin

SUBDIRS = src test

all: lint $(BIN) $(SUBDIRS) run_tests

$(BIN):
	-mkdir bin

$(OBJFILES):
	$(CXX) $^ -o $@

%.o: src/%.cc
	$(CXX) $< -o $@

depend:
	-@for x in $(SUBDIRS); do $(MAKE) -C $$x depend; done

lint:
	@echo "\n========== Checking coding style ==========\n"
	-@for x in $(SUBDIRS); do $(MAKE) -C $$x lint; done

run_tests:
	@echo "\n========== Running all tests ==========\n"
	@bin/testsuite

clean:
	-@for x in $(SUBDIRS); do $(MAKE) -C $$x clean; done
	-rm -f bin/*

.PHONY: $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@

-include .depend.mk
