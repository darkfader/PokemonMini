VERSION := 14
MINDX_ZIP := "http://pokeme.shizzle.it/infos/PM Dev/Assemblers/mindx_v13.zip"
VERSION2 :=
PATH := .:$(PATH)
#CC := $(CXX)
LD := $(CXX)
FLAGS = -O3 -fomit-frame-pointer -march=i586 -mcpu=i686 -mno-cygwin -DVERSION="\"0.$(VERSION)$(VERSION2)\""
CFLAGS = $(FLAGS)
CPPFLAGS = $(FLAGS)
LDFLAGS = $(FLAGS)
#COMPARE = fc /b
COMPARE = diff -q --binary
PMDIS = pmdis
OUTPUTS = pmas cpu/pm.s
#pmdis

.PHONY: help
help:
	@echo "Targets: clean release debug releasetest debugtest"

../pmas-0.$(VERSION).tar.gz: zap
	tar -czf $@ *

../pmas-0.$(VERSION).zip: release releasetest clean
	zip - -q -9 -r . -x ./src/ ./obj/ ./src/* ./obj/* > $@

.PHONY: upload
upload: ../pmas-0.$(VERSION).tar.gz ../pmas-0.$(VERSION).zip
	$(foreach file,$+,$(MAKE) $(file);)
	$(foreach file,$+,curl --url ftp://upload.sourceforge.net/incoming/ --upload-file $(file);)

.PHONY: debug
debug: FLAGS += -g -DDEBUG
debug: $(OUTPUTS)

.PHONY: release
release: $(OUTPUTS)
	-( strip pmas.exe && upx -9 -q pmas.exe )
#	-( strip pmdis.exe && upx -9 -q pmdis.exe )

.PHONY: debugtest
debugtest: PMAS = gdb -q --args pmas
debugtest: debug #test/opcodes1.min test/opcodes2.min
	#$(COMPARE) test/opcodes1.min test/opcodes2.min
	$(PMAS) test/test.s test/test.min

.PHONY: releasetest
releasetest: PMAS = pmas
releasetest: release test/readme.min test/test1.min test/test2.min test/opcodes1.min test/opcodes2.min
	$(COMPARE) test/opcodes1.min test/opcodes2.min
	$(COMPARE) test/test1.min test/test2.min

#	$(PMDIS) test/opcodes2.min test/opcodes2.dis.s
#	$(PMAS) test/opcodes2.dis.s test/opcodes2.as.min test/opcodes2.as.sym
#	$(COMPARE) test/opcodes2.min test/opcodes2.as.min

pmas: obj/pmas.o obj/eval.o obj/misc.o obj/symbol.o obj/stack.o obj/valuetype.o obj/macrolist.o obj/instruction.o obj/mem.o
	$(LD) $(LDFLAGS) -o $@ $+

pmdis: obj/pmdis.o
	$(LD) $(LDFLAGS) -o $@ $+

test/%.min: test/%.s pmas
	$(PMAS) $< $@ $(@:min=sym)

test/%.min: test/%.S pmas
	$(CPP) $< | $(PMAS) - $@ $(@:min=sym)

src/eval.cpp:: src/*.h
	touch -m $@

src/pmas.cpp:: src/*.h
	touch -m $@

src/pmdis.cpp:: src/*.h
	touch -m $@

cpu/pm.s: cpu/mindx.txt parsemindx
	parsemindx cpu/mindx.txt cpu/pm.s highlight.tmp
	(cat cpu/pm_wordfile_head.txt; /bin/sort -u highlight.tmp) > cpu/pm_wordfile.txt
	rm highlight.tmp

parsemindx: src/parsemindx.cpp
	$(CXX) $(CPPFLAGS) -o $@ $<

cpu/mindx.txt:
	wget -O mindx.zip $(MINDX_ZIP)
	unzip -o mindx.zip cpu/mindx.txt
	rm mindx.zip

obj/%.o: src/%.cpp
	$(CXX) $(CPPFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	-rm obj/*.o test/*.min test/*.sym test/*.dis.s parsemindx parsemindx.exe

.PHONY: zap
zap: clean
	-rm pmas pmas.exe pmdis pmdis.exe cpu/pm_wordfile.txt
