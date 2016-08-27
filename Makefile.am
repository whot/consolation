SUBDIRS = src
dist_man_MANS = consolation.8
EXTRA_DIST = AUTHORS ChangeLog LICENSE
src/consolation:
	cd src && $(MAKE) consolation

consolation.8: src/consolation README
	help2man --no-info --source=Debian --section=8 --include=README \
	src/consolation \
	| sed -e 's,\\fB,.TP\n\\fB,g' > consolation.8.new   \
	&& mv consolation.8.new consolation.8
