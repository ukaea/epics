TOP = .
include $(TOP)/configure/CONFIG

DIRS += configure

DIRS += src
src_DEPEND_DIRS  = configure

DIRS += test
test_DEPEND_DIRS = src

DIRS += examples
examples_DEPEND_DIRS = src

BRANCH = 2-2
DEFAULT_REPO = /opt/repositories/controls/darcs/epics/support/seq/branch-$(BRANCH)
GIT_MIRROR = /opt/repositories/controls/git/seq/branch-$(BRANCH)
SEQ_PATH = www/control/SoftDist/sequencer-$(BRANCH)
USER_AT_HOST = wwwcsr@www-csr.bessy.de
DATE = $(shell date -I)
SNAPSHOT = seq-$(BRANCH)-snapshot-$(DATE)
SEQ_TAG = seq-$(subst .,-,$(SEQ_RELEASE))
SEQ_TAG_TIME = $(shell darcs changes --all --xml-output \
	--matches 'exact "TAG $(SEQ_TAG)"' | perl -ne 'print "$$1.$$2" if /date=.(\d{12})(\d{2})/')

include $(TOP)/configure/RULES_TOP

html: src
	$(MAKE) -C documentation

docs: src
	$(MAKE) -C documentation pdf=1

docs.clean:
	$(MAKE) -C documentation clean

realclean clean: docs.clean

upload_docs: docs
	rsync -r -t $(TOP)/html/ $(USER_AT_HOST):$(SEQ_PATH)/

recreate_git_mirror:
	rm -rf $(GIT_MIRROR)
	git init $(GIT_MIRROR)
	touch $(GIT_MIRROR)/git.marks

upload_repo:
	darcs push $(DEFAULT_REPO)
	cd $(DEFAULT_REPO) && darcs push --all $(USER_AT_HOST):$(SEQ_PATH)/repo/branch-$(BRANCH)
	cd $(DEFAULT_REPO) && darcs convert export --read-marks $(GIT_MIRROR)/darcs.marks --write-marks $(GIT_MIRROR)/darcs.marks |\
	  (cd $(GIT_MIRROR) && git fast-import --import-marks=git.marks --export-marks=git.marks)
	cd $(GIT_MIRROR)/.git && git --bare update-server-info
	rsync -r --delete $(GIT_MIRROR)/.git/ $(USER_AT_HOST):$(SEQ_PATH)/repo/branch-$(BRANCH).git/

snapshot:
	darcs dist -d $(SNAPSHOT)
	rsync $(SNAPSHOT).tar.gz $(USER_AT_HOST):$(SEQ_PATH)/releases/
	ssh $(USER_AT_HOST) 'cd $(SEQ_PATH)/releases && ln -f -s $(SNAPSHOT).tar.gz seq-$(BRANCH)-snapshot-latest.tar.gz'
	$(RM) $(SNAPSHOT).tar.gz

release: upload_docs upload_repo
	darcs show files | xargs touch -t $(SEQ_TAG_TIME)
	darcs dist -d seq-$(SEQ_RELEASE) -t '^$(SEQ_TAG)$$'
	rsync seq-$(SEQ_RELEASE).tar.gz $(USER_AT_HOST):$(SEQ_PATH)/releases/
	$(RM) seq-$(SEQ_RELEASE).tar.gz

changelog: force
	DARCS_ALWAYS_COLOR=0 darcs changes -a --from-tag=. | egrep -v '^(Author|Date|patch)' > changelog

force:

.PHONY: html docs docs.clean upload_docs mirror upload_repo snapshot release recreate_git_mirror
