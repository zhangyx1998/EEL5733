# Run all tests according to input files in test/
test: all
	@	$(call env) $(MAKE) \
		$(patsubst %.in,%.test,$(wildcard test/*.in))

# Dependencies for test runs
MSG_PASS:=$E0;1;92m[PASSING]$E0;90m
MSG_FAIL:=$E0;1;91m[FAILING]$E0;90m
define TIME
$E0;2;3;95m($$(	\
	cat $(1:.out=.time) | xargs | 			\
	awk '{print $$1,$$2,"|",$$3,$$4,"|",$$5,$$6}'	\
))
endef
# Run test on specific input
test/%.test: tmp/%.out
	$(eval F:=$(@:.test=))
	@	diff -u $F.out $< > $F.diff && rm $F.diff			\
	&&	echo "$(MSG_PASS) $(notdir $F) $(call TIME,$<)$E0m"	\
	||	echo "$(MSG_FAIL) $(notdir $F) -> $F.diff$E0m"
	@	rm $<

# Dependencies for generating test outputs
define exec_name
$(shell echo $(BUILD)/$(notdir $1) | sed -E 's/\.[0-9]+$$//g')
endef
MSG_ERUN:=$E0;1;91mRuntime Error: $E0;90m
# Generate outputs using corresponding inputs
tmp/%.out: test/%.in $(call exec_name,$(@:.out=)) tmp
	$(eval EXEC:=$(call exec_name,$(@:.out=)))
	$(eval TEST:=$(EXEC) < $< 1> $@)
	@	$(call env) $(MAKE) $(EXEC)
	@	env time -p -o $(@:.out=.time) $(TEST)		\
	||	(echo "$(MSG_ERUN)$(TEST)"; tput sgr0)

# Report performance on all test inputs
perf: perf.env $(patsubst %.in,%.perf,$(wildcard test/*.in))

perf.env:
	$(eval CFLAGS+="-pg")
	$(eval LDFLAGS+="-pg")

# Run performance test for specific input
test/%.perf: perf.env tmp/%.out
	$(eval EXEC:=$(call exec_name,$(@:.perf=)))
	@mv gmon.out $@
	@-gprof $(EXEC) $@

.PHONY: test test/%.test perf perf.env
