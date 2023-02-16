test: test_clean $(patsubst %.in,%,$(wildcard test/*.in))

test_clean:
	$(foreach F,$(wildcard test/*.diff),$(shell rm $F))

test/%.tmp: test/%.in
	$(eval									\
		EXEC=$(shell						\
			echo "$(notdir $(@:.tmp=))" |	\
			sed -E 's/\.[0-9]+$$//g'		\
		)									\
	)
	$(eval EXEC=$(BUILD)/$(strip $(EXEC)))
	@BUILD="$(BUILD)" CFLAGS="$(CFLAGS)" ENSURE="$(EXEC)" \
	make ensure
	@$(EXEC) < $< 1> $@

LABEL_PASS="\033[1;32m[PASSING]\033[1;0m\033[1;2m"
LABEL_FAIL="\033[1;31m[FAILING]\033[1;0m\033[1;2m"

define test_report
	@tput sgr 0; tput bold;
	@if [ $1 == 0 ]; then					\
		tput setaf 10; printf "[PASSING]";	\
	else									\
		tput setaf  9; printf "[FAILING]";	\
	fi;
	@tput sgr 0; tput setaf 0;
	@printf " $2";
	@if [ $1 != 0 ]; then				\
		printf " => test/$2.diff";		\
	fi;
	@tput sgr 0; printf "\n";
endef

test/%: test/%.tmp
	$(eval CODE=$(shell diff -u $@.out $@.tmp > $@.diff && rm $@.diff; echo $$?))
	$(call test_report,$(CODE),$(notdir $@))
	@rm $@.tmp
