PMAS_VERSION := 20
VERSION2 :=
MODIFIER :=
CFLAGS := -Wall -DVERSION="\"0.$(PMAS_VERSION)$(VERSION2)$(MODIFIER)\"" -DVERSIONN=$(PMAS_VERSION)
LD := $(CXX)
LDFLAGS :=
COMPARE = diff -q --binary
PMDIS = ./pmdis
OUTPUTS = pmas$(MODIFIER) cpu/pm.s
#OUTPUTS += pmdis  #hmm.. need mindx.h
AWK := awk

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
	@echo "  clean         Delete intermediate files."
	@echo "  cleanall      Delete output and intermediate files."
	@echo "  tag           Tag CVS."
	@echo "  upload_src    Upload new release with version to sf.net."

####################
# dependency stuff #
####################

NODEPS:=clean cleanall
SOURCES:=$(shell find src/ -name "*.cpp")
DEPFILES:=$(patsubst src/%.cpp,obj/%.d,$(SOURCES))
ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))
	-include $(DEPFILES)
endif

obj/%.d: obj src/%.cpp
	$(CXX) $(CXXFLAGS) -MM -MT $(patsubst src/%.cpp,obj/%.o,$(filter %.cpp,$+)) $(filter %.cpp,$+) > $@

###########
# release #
###########

# source
pmas-0.$(PMAS_VERSION)$(MODIFIER)_src.tar.gz: cleanall
	tar -c -z --exclude-vcs -f $@ Makefile README src cpu test

# binaries
pmas-0.$(PMAS_VERSION)$(MODIFIER).zip: release releasetest clean
	zip - -q -9 -r . -x *.zip ./src/ ./obj/ ./src/* ./obj/* > $@

.PHONY: upload_src
upload_src: pmas-0.$(PMAS_VERSION)$(MODIFIER)_src.tar.gz README.txt
	$(foreach file,$+,scp $(file) darkfader,pmas@frs.sourceforge.net:/home/frs/project/p/pm/pmas/;)

.PHONY: upload_bin
upload_bin: ../pmas-0.$(PMAS_VERSION)$(MODIFIER)_$(PLATFORM).zip
	$(foreach file,$+,scp $(file) darkfader,pmas@frs.sourceforge.net:/home/frs/project/p/pm/pmas/;)

.PHONY: tag
tag:
	cvs tag -c v0_$(PMAS_VERSION)$(MODIFIER)

README.txt: README ChangeLog.txt
	cat $+ > $@

ChangeLog.txt: cvs2cl.pl *
	perl cvs2cl.pl --tagdates --summary --no-wrap --prune --utc --no-times --group-within-date --hide-filenames --stdout . | $(AWK) '/^$$/ || /^\t$$/ { next } /\tAdded:/ || /\tChanged:/ || /\tDeleted:/ { next } /  tag / { print "\n============================\n" $$0 "\n============================\n"; next } // { print $$0 }' > $@

cvs2cl.pl:
	wget "http://www.red-bean.com/cvs2cl/cvs2cl.pl"

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
release: LDFLAGS +=							#-Wl,-s
release: $(OUTPUTS)

.PHONY: debugtest
debugtest: PMAS = echo r | gdb -q -x - --args pmas$(MODIFIER)
debugtest: debug
	$(PMAS) test/test.s test/test.min

.PHONY: releasetest
releasetest: PMAS = ./pmas$(MODIFIER)
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

PMAS_SOURCES := src/pmas$(MODIFIER).cpp src/eval.cpp src/misc.cpp src/symbol.cpp src/stack.cpp src/valuetype.cpp src/macrolist.cpp src/instruction.cpp src/mem.cpp src/tmplabel.cpp
PMAS_OBJECTS := $(patsubst src/%.cpp,obj/%.o,$(PMAS_SOURCES))

pmas$(MODIFIER): obj $(PMAS_OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $(filter %.o,$+)

pmdis: obj obj/pmdis.o
	$(LD) $(LDFLAGS) -o $@ $(filter %.o,$+)

#########
# tests #
#########

test/%.min: test/%.s pmas$(MODIFIER)
	$(PMAS) $< $@ $(@:min=sym)

test/%.min: test/%.S pmas$(MODIFIER)
	$(CPP) $< | $(PMAS) - $@ $(@:min=sym)

######################################################
# mindx (Pokemon Mini instruction set documentation) #
######################################################

cpu/pm.s: cpu/mindx.txt parsemindx
	./parsemindx cpu/mindx.txt cpu/pm.s highlight.tmp
	(cat cpu/pm_wordfile_head.txt; sort -u highlight.tmp) > cpu/pm_wordfile.txt
	rm highlight.tmp

parsemindx: src/parsemindx.cpp
	$(CXX) $(CFLAGS) -o $@ $+

#cpu/mindx.txt:
#	wget -O mindx.zip $(MINDX_ZIP)
#	unzip -o mindx.zip cpu/mindx.txt
#	rm mindx.zip

########
# misc #
########

obj/%.o: src/%.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	-rm -f obj/*.o obj/*.d test/*.min test/*.sym test/*.dis.s parsemindx parsemindx.exe
	-rmdir obj

.PHONY: cleanall
cleanall: clean
	-rm -f *.tar.gz pmas$(MODIFIER) pmas$(MODIFIER).exe pmdis pmdis.exe cpu/pm_wordfile.txt

