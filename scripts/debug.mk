# Debug mode: outputs to $(BUILD)/debug, add DEBUG to CCFLAGS
debug: debug.env all

debug.env:
	$(eval CCFLAGS += -DDEBUG -g)

# Add specific flag for debug
%.debug:
	$(eval DBG_T :=$(shell	\
		echo $(@:.debug=) |	\
		sed -e 's/\//_/g' |	\
		tr 'a-z' 'A-Z'		\
	))
	$(eval CCFLAGS += -DDEBUG_$(DBG_T))

.PHONY: debug debug.env %.debug
