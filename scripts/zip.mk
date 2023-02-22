# Creates a zip file for current workspace
AUTHOR   := $(shell git config --get user.name || whoami)
AUTHOR   := $(shell echo $(AUTHOR) | sed -E 's/\W/_/g')
BRANCH   := $(shell git branch --show-current)
BRANCH   := $(patsubst assignment_%,Assignment_%,$(BRANCH))
FILE_LIST:= $(shell git ls-tree --full-tree --name-only -r HEAD)
ARCHIVE  :=	var/$(AUTHOR)_$(BRANCH).zip

$(ARCHIVE): var repo_is_clean
	@rm -f $@
	@zip $@ $(FILE_LIST) 1> /dev/null


zip: $(ARCHIVE)
	@	zipinfo -1 $(ARCHIVE) | tree --fromfile .	\
	||	zipinfo -1 $(ARCHIVE)
	@echo "$E92mZip file written: $E0;32;4m$<$E0m"

.PRECIOUS: var/$(AUTHOR)_$(BRANCH).zip
.PHONY: zip
