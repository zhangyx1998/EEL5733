TARGETS:=$(patsubst test/%.in,%.test,$(wildcard test/*.in))

test: all $(TARGETS)

%.test:
	$(eval FILE:=$(shell echo "$@" | sed -E 's/\.test$$//g'))
	$(eval EXEC:=$(shell echo "$(FILE)" | sed -E 's/\.[0-9]+$$//g'))
	@# @echo Running $(FILE).test on $(EXEC).bin
	@# Generate output
	@build/$(EXEC).bin < test/$(FILE).in 1> test/$(FILE).out
	@diff test/$(FILE).ref test/$(FILE).out > test/$(FILE).diff \
	&& echo "\033[1;32m [PASSING] \033[1;0m\033[1;2m$(FILE).test\033[0m" \
	|| echo "\033[1;31m [FAILING] \033[1;0m\033[1;2m$(FILE).test\033[0m"

.PHONY: %.test %.in %.out
