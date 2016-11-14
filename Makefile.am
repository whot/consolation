SUBDIRS = src
dist_man_MANS = consolation.8
EXTRA_DIST = AUTHORS ChangeLog LICENSE
consolation.8: README src/shared.c $(top_srcdir)/configure.ac
	help2man --no-info --source=Debian --section=8 --include=README \
	src/consolation \
	| sed -e 's,\\fB,.TP\n\\fB,g' > consolation.8.new   \
	&& mv consolation.8.new consolation.8
