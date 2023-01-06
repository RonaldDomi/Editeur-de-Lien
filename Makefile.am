ACLOCAL_AMFLAGS = -I m4

AM_CFLAGS=-Wall -Werror -g
AM_CFLAGS+=-D DEBUG
AM_CFLAGS+=-D WARNING
# Uncomment if performance when running with -DDEBUG is an issue
# Warning, if uncommented, issuing calls to debug functions during options
# parsing might result in debug flag incorrectly set to 0 for some files
#AM_CFLAGS+=-D CACHE_DEBUG_FLAG



SUBDIRS=
# SUBDIRS+=. Examples_fusion

bin_PROGRAMS=
bin_PROGRAMS+=Options_and_debug_example
bin_PROGRAMS+=readelf_header
bin_PROGRAMS+=readelf_section_table
bin_PROGRAMS+=readelf_section_content
bin_PROGRAMS+=readelf_symbol_table
bin_PROGRAMS+=readelf_relocation
bin_PROGRAMS+=readelf_all
LDADD= -lm

COMMON=debug.h debug.c util.h util.c
COMMON+=modules/freadoctet.h modules/freadoctet.c modules/CustomElf.h modules/CheckElf.h modules/CheckElf.c
COMMON+=modules/readHeader.h modules/readHeader.c modules/readSectionTable.h modules/readSectionTable.c
COMMON+=modules/readContent.h modules/readContent.c

EXTRA_DIST=filtre.pl
Options_and_debug_example_SOURCES=$(COMMON) Options_and_debug_example.c
readelf_all_SOURCES=$(COMMON) readelf_all.c 
readelf_header_SOURCES=$(COMMON) readelf_header.c 
readelf_section_table_SOURCES=$(COMMON) readelf_section_table.c
readelf_section_content_SOURCES=$(COMMON) readelf_section_content.c
readelf_symbol_table_SOURCES=$(COMMON) readelf_symbol_table.c
readelf_relocation_SOURCES=$(COMMON) readelf_relocation.c

EXTRA_DIST+=
