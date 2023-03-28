ARCH       ?= $(shell uname -m)
BRANCH     ?= $(shell git branch --show-current)
IMG_SIZE   ?= 10G
VM_MEM     ?= 2G
QEMU_ARGS  := -m $(VM_MEM) -nographic
ifeq ($(ARCH),arm64)
$(info Using architecture aarch64, a.k.a arm64)
QEMU       := qemu-system-aarch64
VM_DISK    := var/$(BRANCH).aarch64.qcow2
VM_BIOS    := var/QEMU_EFI.fd
VM_IMAGE   := var/ubuntu.aarch64.iso
QEMU_ARGS  += -machine virt,accel=hvf
QEMU_ARGS  += -cpu host -smp 8
QEMU_ARGS  += -bios $(VM_BIOS)
QEMU_ARGS  += -drive if=none,format=raw,file=$(VM_DISK),id=hd0
QEMU_ARGS  += -device virtio-blk-device,drive=hd0
QEMU_ARGS  += -netdev vmnet-shared,id=net0
QEMU_ARGS  += -device virtio-net,netdev=net0
else ifeq ($(ARCH),x86_64)
$(info Using architecture x86_64, a.k.a amd64)
QEMU       := qemu-system-x86_64
VM_DISK    := var/$(BRANCH).x86_64.qcow2
VM_IMAGE   := var/ubuntu.x86_64.iso
VM_BIOS    :=
QEMU_ARGS  += -hda $(VM_DISK)
else
$(info Unknown architecture "$(ARCH)")
QEMU       :=
endif
# Check for local installation of QEMU 
E_QEMU_NOT_INSTALLED=$E22;4;1mQEMU executables not found in path.$E0;8m
check-qemu:
	@	test -f $$(which $(QEMU))  && \
		test -f $$(which qemu-img) || \
		(printf "$(E_QEMU_NOT_INSTALLED)"; exit 1)

UBUNTU_ISO_AMD64 := https://mirror.mrjester.net/ubuntu/release/22.04.2/ubuntu-22.04.2-live-server-amd64.iso
var/ubuntu.x86_64.iso:
	@if test -f $@; then \
		echo "Using exitsing ubuntu image"; \
		file $@; \
	else \
		echo "Downloading ubuntu OS image..."; \
		curl $(UBUNTU_ISO_AMD64) -o $@; \
	fi

$(VM_DISK):
	$(info >> Creating QEMU disk image as $@...)
	qemu-img create -f qcow2 $@ $(IMG_SIZE)
	$(info >> Installing disk image to $@...)
	$(QEMU) -cdrom $(VM_IMAGE) $(QEMU_ARGS) || (rm -rf $@; exit 1)

UBUNTU_ISO_ARM64 := https://mirror.mrjester.net/ubuntu/release/22.04.2/ubuntu-22.04.2-live-server-arm64.iso
var/ubuntu.aarch64.iso:
	@if test -f $@; then \
		echo "Using exitsing ubuntu image"; \
		file $@; \
	else \
		echo "Downloading ubuntu OS image..."; \
		curl $(UBUNTU_ISO_ARM64) -o $@; \
	fi

QEMU_EFI_FD := https://releases.linaro.org/components/kernel/uefi-linaro/latest/release/qemu64/QEMU_EFI.fd
var/QEMU_EFI.fd:
	$(info >> Downloading QEMU EFI as $@...)
	@curl -L $(QEMU_EFI_FD) -o $@
	@touch $@

$(QEMU):
	$@ $(QEMU_ARGS)

qemu: var $(VM_IMAGE) $(VM_DISK) $(VM_BIOS) $(QEMU)
	$(info qemu: $^)

.PHONY: check-qemu $(QEMU)
