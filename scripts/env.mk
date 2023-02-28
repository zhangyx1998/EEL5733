# Function to print all re-configurable env args
define env
BUILD="$(BUILD)"	\
CFLAGS="$(CFLAGS)"	\
LDFLAGS="$(LDFLAGS)"
endef

# Compare and update env if detected changes
$(BUILD)/env: $(BUILD) tmp
	@	echo '$(call env)'				\
	|	sed -E "s/\" /\"\n/g" > tmp/env	\
	&&	cmp -s $@ tmp/env > /dev/null	\
	||	mv tmp/env $@
