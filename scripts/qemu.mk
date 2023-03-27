QEMU     ?= $(shell which qemu-system-x86_64)
QEMU_IMG ?= $(shell which qemu-img)
IMG_SIZE ?= 10G
VM_DISK  ?= $(shell git branch --show-current).qcow2
VM_MEM   ?= 4G
# Errors if 
E_QEMU_NOT_INSTALLED=$E22;4;1mQEMU executables not found in path.$E0;8m
check-qemu:
	@	test -f $(QEMU)     && \
		test -f $(QEMU_IMG) || \
		(printf "$(E_QEMU_NOT_INSTALLED)"; exit 1)

UBUNTU_ISO_URL := https://mirror.mrjester.net/ubuntu/release/22.04.2/ubuntu-22.04.2-live-server-amd64.iso
var/ubuntu.iso: var
	@if test -f $@; then \
		echo "Using exitsing ubuntu image"; \
		file $@; \
	else \
		echo "Downloading ubuntu OS image..."; \
		curl $(UBUNTU_ISO_URL) -o $@;\
	fi

var/$(VM_DISK): var check-qemu var/ubuntu.iso
	$(info creating QEMU disk image)
	$(QEMU_IMG) create -f qcow2 $@ $(IMG_SIZE)
	$(info initiating installation process)
	$(QEMU) \
		-boot d \
		-cdrom var/ubuntu.iso \
		-hda $@ \
		-m $(VM_MEM)

qemu: var/$(VM_DISK)
	$(QEMU) -hda $(VM_DISK) -m $(VM_MEM)

.PHONY: check-qemu