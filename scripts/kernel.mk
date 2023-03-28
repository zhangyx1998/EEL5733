UNAME_R:=$(shell uname -r)
LINUX_HEADERS_HOME ?= /usr/src/linux-headers-$(UNAME_R)

KERNEL_MODULES:=$(patsubst kernel/%.c,%,$(wildcard kernel/*.c))

check-linux-headers:
	@# Check for linux headers \
	if ! test -f $(LINUX_HEADERS_HOME)/Makefile; then\
		echo "Linux headers not found at $(LINUX_HEADERS_HOME)"; \
		echo "Install it using: sudo apt install -y linux-headers-$(UNAME_R)"; \
		exit 1; \
	fi


$(BUILD)/kernel/%/Makefile:
	@mkdir -p $(shell dirname $@)
	@echo "obj-m += $(shell basename $(shell dirname $@)).o" > $@


$(BUILD)/%.ko: kernel/%.c $(BUILD)/kernel/%/Makefile
	$(eval KO:=$(shell basename $@))
	$(eval KM:=$(KO:.ko=))
	@cp kernel/$(KM).c $(BUILD)/kernel/$(KM)/$(KM).c
	$(MAKE) -C $(LINUX_HEADERS_HOME) modules M=$(PWD)/$(BUILD)/kernel/$(KM)
	@cp $(BUILD)/kernel/$(KM)/$(KO) $@

%.insmod: build/%.ko
	$(eval KO:=$(shell basename $<))
	$(eval KM:=$(KO:.ko=))
	@if ! test -d /sys/module/$(KM); then \
		sudo insmod $<; echo "installed: $(KM)"; \
	else \
		echo "already installed: $(KM)"; \
	fi

%.rmmod: build/%.ko
	$(eval KO:=$(shell basename $<))
	$(eval KM:=$(KO:.ko=))
	@if test -d /sys/module/$(KM); then \
		sudo rmmod $<; echo "removed: $(KM)"; \
	else \
		echo "not installed: $(KM)"; \
	fi

kernel-objects: check-linux-headers $(patsubst %,build/%.ko,$(KERNEL_MODULES))

install-kernel-modules: kernel-objects $(patsubst %,%.insmod,$(KERNEL_MODULES))

remove-kernel-modules: $(patsubst %,%.rmmod,$(KERNEL_MODULES))

KM ?= tuxdev
tux0: install-kernel-modules
	@if test -e /dev/tux0; then \
		sudo rm /dev/tux0; \
		echo "Existing node removed"; \
	fi
	@sudo mknod /dev/tux0 c 700 0; echo "Created device node /dev/tux0"

check-kernel-modules:
	@for km in $(KERNEL_MODULES); do \
		echo "["$$(test -d /sys/module/$$km && echo "INSTALLED" || echo " ------- ")"]" $$km; \
	done

.PHONY: \
	check-linux-headers \
	install-kernel-modules \
	remove-kernel-modules \
	check-kernel-modules \
	kernel-objects \
	/dev/%
