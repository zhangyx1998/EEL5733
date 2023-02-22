# Function to print all re-configurable env args
define env
BUILD="$(BUILD)"		\
CCFLAGS="$(CCFLAGS)"	\
LDFLAGS="$(LDFLAGS)"
endef

# Compare and update env if detected changes
$(BUILD)/env: $(BUILD) tmp
	@	echo '$(call env)'				\
	|	sed -E "s/\" /\"\n/g" > tmp/env	\
	&&	diff $@ tmp/env > /dev/null		\
	||	mv tmp/env $@
