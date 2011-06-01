PMAS_VERSION := 15
MINDX_ZIP := "http://pokeme.shizzle.it/infos/PM Dev/Assemblers/mindx_v14.zip"
VERSION2 :=
#PATH := .:/bin:/usr/bin:$(PATH)
CFLAGS := -DVERSION="\"0.$(PMAS_VERSION)$(VERSION2)\""
LD := $(CXX)
LDFLAGS := 
COMPARE = diff -q --binary
PMDIS = ./pmdis
OUTPUTS = pmas cpu/pm.s
#OUTPUTS += pmdis  #hmm.. need mindx.h

########
# help #
########

.PHONY: help
help:
	@echo "Command goals:"
	@echo "  help          This list."
	@echo "  release       Build release version of pmas."
	@echo "  debug         Build debug version of pmas."
	@echo "  releasetest   Run some tests."
	@echo "  debugtest     Run some tests under gdb to find bugs."
	@echo "  clean         Delete output and intermediate files."
	@echo "  zap           Delete intermediate files."

####################
# dependency stuff #
####################

SUFFIXES += .d
NODEPS:=clean zap
SOURCES:=$(shell find src/ -name "*.cpp")
DEPFILES:=$(patsubst src/%.cpp,obj/%.d,$(SOURCES))
ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))
	-include $(DEPFILES)
endif

obj/%.d: src/%.cpp
	$(CXX) $(CXXFLAGS) -MM -MT $(patsubst src/%.cpp,obj/%.o,$<) $< > $@

###########
# release #
###########

../pmas-0.$(PMAS_VERSION).tar.gz: zap
	tar -czf $@ *

../pmas-0.$(PMAS_VERSION).zip: release releasetest clean
	zip - -q -9 -r . -x ./src/ ./obj/ ./src/* ./obj/* > $@

.PHONY: upload
upload: ../pmas-0.$(PMAS_VERSION).tar.gz ../pmas-0.$(PMAS_VERSION).zip
	$(foreach file,$+,$(MAKE) $(file);)
	$(foreach file,$+,curl --url ftp://upload.sourceforge.net/incoming/ --upload-file $(file);)

########
# misc #
########

obj:
	mkdir obj

.PHONY: debug
debug: CFLAGS += -g -DDEBUG
debug: $(OUTPUTS)

.PHONY: release
release: CFLAGS += -O3
#-march=i586 -mcpu=i686 -O3 -fomit-frame-pointer
release: LDFLAGS += -s
release: $(OUTPUTS)

.PHONY: debugtest
debugtest: PMAS = echo r | gdb -q -x - --args pmas
debugtest: debug
	$(PMAS) test/test.s test/test.min

.PHONY: releasetest
releasetest: PMAS = ./pmas
releasetest: release test/readme.min test/test1.min test/test2.min test/test3.min test/opcodes1.min test/opcodes2.min test/opcodes3.min
	$(COMPARE) test/opcodes1.min test/opcodes2.min
	$(COMPARE) test/opcodes1.min test/opcodes3.min
	$(COMPARE) test/test1.min test/test2.min
	$(PMAS) test/test.s test/test.min

#	$(PMDIS) test/opcodes2.min test/opcodes2.dis.s
#	$(PMAS) test/opcodes2.dis.s test/opcodes2.as.min test/opcodes2.as.sym
#	$(COMPARE) test/opcodes2.min test/opcodes2.as.min

##############
# pmas/pmdis #
##############

PMAS_SOURCES := src/pmas.cpp src/eval.cpp src/misc.cpp src/symbol.cpp src/stack.cpp src/valuetype.cpp src/macrolist.cpp src/instruction.cpp src/mem.cpp
PMAS_OBJECTS := $(patsubst src/%.cpp,obj/%.o,$(PMAS_SOURCES))

pmas: obj $(PMAS_OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $(filter %.o,$+)

pmdis: obj obj/pmdis.o
	$(LD) $(LDFLAGS) -o $@ $(filter %.o,$+)

#########
# tests #
#########

test/%.min: test/%.s pmas
	$(PMAS) $< $@ $(@:min=sym)

test/%.min: test/%.S pmas
	$(CPP) $< | $(PMAS) - $@ $(@:min=sym)

#########
# mindx #
#########

cpu/pm.s: cpu/mindx.txt parsemindx
	./parsemindx cpu/mindx.txt cpu/pm.s highlight.tmp
	(cat cpu/pm_wordfile_head.txt; sort -u highlight.tmp) > cpu/pm_wordfile.txt
	rm highlight.tmp

parsemindx: src/parsemindx.cpp
	$(CXX) $(CFLAGS) -o $@ $+

cpu/mindx.txt:
	wget -O mindx.zip $(MINDX_ZIP)
	unzip -o mindx.zip cpu/mindx.txt
	rm mindx.zip

########
# misc #
########

obj/%.o: src/%.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	-rm -f obj/*.o test/*.min test/*.sym test/*.dis.s parsemindx parsemindx.exe

.PHONY: zap
zap: clean
	-rm -f pmas pmas.exe pmdis pmdis.exe cpu/pm_wordfile.txt

