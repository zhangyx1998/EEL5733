test: $(patsubst test/%.in,%.test,$(wildcard test/*.in))

%.test:
	$(eval FILE:=$(shell echo "$@" | sed -E 's/\.test$$//g'))
	$(eval EXEC:=$(BUILD)/$(shell sed -E 's/\.[0-9]+$$//g' <<< $(FILE)))
	@if [ ! -f $(EXEC) ]; then \
		BUILD="$(BUILD)" \
		CFLAGS="$(CFLAGS)" \
		make $(EXEC); \
	fi
	@$(EXEC) < test/$(FILE).in 1> test/$(FILE).out
	@diff test/$(FILE).ref test/$(FILE).out > test/$(FILE).diff \
	&& echo "\033[1;32m [PASSING] \033[1;0m\033[1;2m$(FILE).test\033[0m" \
	|| echo "\033[1;31m [FAILING] \033[1;0m\033[1;2m$(FILE).test\033[0m"

.PHONY: %.test %.in %.out
