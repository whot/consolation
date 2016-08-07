SUBDIRS = src
dist_man_MANS = consolation.8
EXTRA_DIST = AUTHORS LICENSE
consolation.8: README
	txt2man -s 8 -t Consolation README > consolation.8.new \
        && mv consolation.8.new consolation.8
