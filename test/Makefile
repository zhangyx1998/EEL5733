test: $(patsubst %.in,%,$(wildcard test/*.in))

MSG_ERUN:=$E0;1;91mRuntime Error: $E0;90m
SED_CMD :=sed -E 's/\.[0-9]+$$//g'

tmp/%.out: test/%.in
	@mkdir -p tmp
	$(eval EXEC:=$(notdir $(@:.out=)))
	$(eval EXEC:=$(shell echo $(EXEC) | $(SED_CMD)))
	$(eval EXEC:=$(BUILD)/$(EXEC))
	$(eval TEST:=$(EXEC) < $< 1> $@)
	@	BUILD="$(BUILD)" CFLAGS="$(CFLAGS)"			\
		ENSURE="$(EXEC)"							\
		make ensure --no-print-directory
	@	$(TEST)										\
	||	(echo "$(MSG_ERUN)$(TEST)"; tput sgr0)

MSG_PASS:=$E0;1;92m[PASSING]$E0;90m
MSG_FAIL:=$E0;1;91m[FAILING]$E0;90m

test/%: tmp/%.out
	@	diff -u $@.out $< > $@.diff && rm $@.diff	\
	&&	echo "$(MSG_PASS) $(notdir $@)"				\
	||	echo "$(MSG_FAIL) $(notdir $@) -> $@.diff"
	@	tput sgr0; rm $<
