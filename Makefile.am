SUBDIRS = src

ACLOCAL_AMFLAGS = -I m4 $$ACLOCAL_ARGS

PLUGIN_NAME = sylmruby
PLUGIN_SO = src/.libs/$(PLUGIN_NAME).so

all: build-mruby

build-mruby:
	cp build_mymruby.rb lib/mruby
	(cd lib/mruby; MRUBY_CONFIG=build_mymruby.rb rake)

include lib/sylplugin_factory/build/common.mk
