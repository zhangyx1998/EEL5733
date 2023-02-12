test: $(patsubst %.in,%,$(wildcard test/*.in))

test/%.tmp: test/%.in
	$(eval								\
		EXEC=$(BUILD)/$(shell			\
			echo $(notdir $(@:.tmp=)) |	\
			sed -E 's/\.[0-9]+$$//g'	\
		)								\
	)
	@if [ ! -f $(EXEC) ]; then	\
		BUILD="$(BUILD)"		\
		CFLAGS="$(CFLAGS)"		\
		make $(EXEC);			\
	fi
	@$(EXEC) < $< 1> $@

LABEL_PASS="\033[1;32m[PASSING]\033[1;0m\033[1;2m"
LABEL_FAIL="\033[1;31m[FAILING]\033[1;0m\033[1;2m"

test/%: test/%.tmp
	@diff -u $@.out $@.tmp > $@.diff					\
		&& echo $(LABEL_PASS) "$(notdir $@)""\033[1;0m"	\
		&& rm $@.diff									\
		|| echo $(LABEL_FAIL) "$(notdir $@) -> $@.diff""\033[1;0m"
	@rm $@.tmp

.PHONY: test/%
