ifeq ($(wildcard Makefile.include),)
$(error Could not find Makefile.include, did you forget to run ./configure?)
endif

include Makefile.include

#CFLAGS = -D_GLIBCXX_USE_CXX11_ABI=0
CFLAGS += -O3 $(CFLAGS)

ifeq ($(ENABLE_DEBUG),True)
CFLAGS += -g
endif


export ENABLE_OPTIMIZATION
export VERSION_MAJOR
export REAL_TIME

AR = ar
RANLIB = ranlib
UNAME := $(shell uname)

ifeq ($(UNAME),Darwin)
SHARED_EXT = dylib
SHARED_EXT_PREFIX =
SHARED_EXT_SUFFIX = .dylib
SHARED_FLAG = -dynamiclib
else
STATIC_EXT = a
STATIC_EXT_PREFIX = .a
STATIC_EXT_SUFFIX =
SHARED_EXT = so
SHARED_EXT_PREFIX = .so
SHARED_EXT_SUFFIX =
SHARED_FLAG = -shared
endif

#XENOMAI = $(findstring xenomai,$(shell uname -r))
#ifeq ($(XENOMAI),)
OBJDIR = .o
#else
#OBJDIR = .o/xenomai
#endif

ifeq ($(V),)
vecho = @echo $1;
else
vecho =
endif

PKGINCLUDEDIRMAIN = $(PREFIX)/include/sml-$(VERSION)
PKGINCLUDEDIR = $(PREFIX)/include/sml-$(VERSION)/sml
PKGLIBDIR = $(PREFIX)/lib

all: targets

$(OBJDIR):
	@mkdir -p $(OBJDIR)

##############################################################################
# Compilation of sml library
##############################################################################
include sml/Makefile.include


CLEANFILES += $(TARGETS)

targets:: $(TARGETS)

clean:
	$(call vecho,[RM] $(CLEANFILES)) \
	rm -f $(CLEANFILES); \
	rm -rf $(OBJDIR)

distclean: clean
	$(call vecho,[RM-DIST]) \
	rm -f sml/sml-$(version_major).0.pc; \
	rm -f sml/sml-$(version_major).0.pc; \
	rm -f Makefile.include

install-headers:
uninstall-headers:

install-libs:
uninstall-libs:

install-scripts-common:
uninstall-scripts-common:
#install-scripts-common: scripts/sml-webots-project
#	$(call vecho,[INSTALL] scripts) \
#	mkdir -p $(DESTDIR)$(BINDIR); \
#	cp scripts/sml-webots-project $(DESTDIR)$(BINDIR)/
#uninstall-scripts-common:
#	$(call vecho,[UNINSTALL] scripts) \
#	rm -f $(DESTDIR)$(BINDIR)/sml-webots-project

install-scripts:
uninstall-scripts:
#install-scripts: install-scripts-common
#uninstall-scripts: uninstall-scripts-common

install-data:
uninstall-data:

install: $(TARGETS) install-headers install-libs install-scripts install-data

uninstall: uninstall-headers uninstall-libs uninstall-scripts uninstall-data

distcheck:
	$(call vecho,[DISTCHECK] sml-$(VERSION).tar.gz) \
	mkdir -p sml-$(VERSION)/sml; \
	cp -r configure Makefile matlab python tests sml-$(VERSION)/; \
	cp -r $(wildcard sml/*.cc) $(wildcard sml/*.hh) $(wildcard sml/*.h) sml/sml.pc.in sml/Makefile.include sml/kinematics sml-$(VERSION)/sml/; \
	mkdir -p sml-$(VERSION)/webots; \
	cp -r webots/robotwebots.* webots/Makefile.include webots/sml-webots.pc.in webots/project sml-$(VERSION)/webots/; \
	mkdir -p sml-$(VERSION)/scripts; \
	cp -r scripts/sml-webots-project.in sml-$(VERSION)/scripts/; \
	tar -czf sml-$(VERSION).tar.gz sml-$(VERSION); \
	rm -rf sml-$(VERSION)

.PHONY: all clean targets install uninstall distcheck install-headers uninstall-headers install-data uninstall-data install-scripts uninstall-scripts install-libs uninstall-libs

