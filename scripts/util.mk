# Directories maintained by make
DIRS := $(BUILD) var tmp

# Create directorys to work with
$(DIRS):
	@mkdir -p $@

# Errors if current repo is not clean
E_REPO_CLEAN=$E22;4;1mYou have uncommited changes!$E0;8m
repo_is_clean:
	@printf "$E0;91;2m"
	@	git update-index --refresh		\
	&&	git diff-index --quiet HEAD --	\
	||	(printf "$(E_REPO_CLEAN)"; exit 1)
	@printf "$E0m"

# Clean all make generated folders and files
clean:
	@rm -rf $(BUILD)
	@rm -rf $(ARCHIVE)
	@rm -rf test/*.diff
	@rm -rf tmp

.PHONY: repo_is_clean clean
