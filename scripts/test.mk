# Run all tests according to input files in test/
test: $(patsubst %.in,%.test,$(wildcard test/*.in))


# Dependencies for test runs
MSG_PASS:=$E0;1;92m[PASSING]$E0;90m
MSG_FAIL:=$E0;1;91m[FAILING]$E0;90m
# Run test on specific input
test/%.test: tmp/%.out
	$(eval F:=$(@:.test=))
	@	diff -u $F.out $< > $F.diff && rm $F.diff	\
	&&	echo "$(MSG_PASS) $(notdir $F)"				\
	||	echo "$(MSG_FAIL) $(notdir $F) -> $F.diff"
	@	tput sgr0; rm $<


# Dependencies for generating test outputs
define exec_name
$(shell echo $(BUILD)/$(notdir $1) | sed -E 's/\.[0-9]+$$//g')
endef
MSG_ERUN:=$E0;1;91mRuntime Error: $E0;90m
# Generate outputs using corresponding inputs
tmp/%.out: test/%.in tmp
	$(eval EXEC:=$(call exec_name,$(@:.out=)))
	$(eval TEST:=$(EXEC) < $< 1> $@)
	@	$(call env) $(MAKE) $(EXEC)
	@	$(TEST) \
	||	(echo "$(MSG_ERUN)$(TEST)"; tput sgr0)

# Report performance on all test inputs
perf: debug $(patsubst %.in,%.perf,$(wildcard test/*.in))


# Run performance test for specific input
test/%.perf: test/%.in
	$(eval EXEC:=$(call exec_name,$(@:.perf=)))
	$(eval FOUT:=$(patsubst $(BUILD)/%,$(BUILD)/src/%.o,$(EXEC)))
	@$(call env) CCFLAGS="$(CCFLAGS) -p" $(MAKE) $(EXEC)
	# gprof $(EXEC) $(FOUT) < $< 2> /dev/null

.PHONY: test test/%.test perf test/%.perf
