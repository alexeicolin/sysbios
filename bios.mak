#
#  ======== bios.mak ========
#

#
# Where to install/stage the packages
# Typically this would point to the devkit location
#
DESTDIR ?= <UNDEFINED>

prefix ?= /
docdir ?= /docs/bios
packagesdir ?= /packages

# Set up dependencies
XDC_INSTALL_DIR ?= $(TREES)/xdcprod/xdcprod-r87/product/$(BUILD_HOST_OS)/xdctools_3_25_04_87_eng

# Do you want to build SMP-enabled libraries (if supported for your target)?
# Set to either 0 (disabled) or 1 (enabled)
#
BIOS_SMPENABLED=1

#
# Set location of various cgtools
# These variables can be set here or on the command line.
#
ti.targets.C28_large ?=
ti.targets.C28_float ?=
ti.targets.C64P ?=
ti.targets.C64P_big_endian ?=
ti.targets.C674 ?=

ti.targets.elf.C64P ?=
ti.targets.elf.C64P_big_endian ?=
ti.targets.elf.C64T ?=
ti.targets.elf.C66 ?=
ti.targets.elf.C66_big_endian ?=
ti.targets.elf.C674 ?=

ti.targets.arm.elf.Arm9 ?=
ti.targets.arm.elf.A8Fnv ?=
ti.targets.arm.elf.M3 ?=
ti.targets.arm.elf.M4 ?=
ti.targets.arm.elf.M4F ?=

ti.targets.msp430.elf.MSP430X ?=
ti.targets.msp430.elf.MSP430X_small ?=

ti.targets.arp32.elf.ARP32 ?=
ti.targets.arp32.elf.ARP32_far ?=

gnu.targets.arm.M3 ?=
gnu.targets.arm.M4 ?=
gnu.targets.arm.M4F ?=

gnu.targets.arm.A8F ?=
gnu.targets.arm.A15F ?=

iar.targets.msp430.MSP430X_small ?=

iar.targets.arm.M3 ?=
iar.targets.arm.M4 ?=
iar.targets.arm.M4F ?=

#
# Set XDCARGS to some of the variables above.  XDCARGS are passed
# to the XDC build engine... which will load bios.bld... which will
# extract these variables and use them to determine what to build and which
# toolchains to use.
#
# Note that not all of these variables need to be set to something valid.
# Unfortunately, since these vars are unconditionally assigned, your build line
# will be longer and more noisy than necessary (e.g., it will include CC_V5T
# assignment even if you're just building for C64P).
#
# Some background is here:
#     http://rtsc.eclipse.org/docs-tip/Command_-_xdc#Environment_Variables
#
XDCARGS= \
    BIOS_SMPENABLED=\"$(BIOS_SMPENABLED)\" \
    ti.targets.C28_large=\"$(ti.targets.C28_large)\" \
    ti.targets.C28_float=\"$(ti.targets.C28_float)\" \
    ti.targets.C64P=\"$(ti.targets.C64P)\" \
    ti.targets.C64P_big_endian=\"$(ti.targets.C64P_big_endian)\" \
    ti.targets.C674=\"$(ti.targets.C674)\" \
    ti.targets.arm.elf.Arm9=\"$(ti.targets.arm.elf.Arm9)\" \
    ti.targets.arm.elf.A8Fnv=\"$(ti.targets.arm.elf.A8Fnv)\" \
    ti.targets.arm.elf.M3=\"$(ti.targets.arm.elf.M3)\" \
    ti.targets.arm.elf.M4=\"$(ti.targets.arm.elf.M4)\" \
    ti.targets.arm.elf.M4F=\"$(ti.targets.arm.elf.M4F)\" \
    ti.targets.elf.C64P=\"$(ti.targets.elf.C64P)\" \
    ti.targets.elf.C64P_big_endian=\"$(ti.targets.elf.C64P_big_endian)\" \
    ti.targets.elf.C64T=\"$(ti.targets.elf.C64T)\" \
    ti.targets.elf.C66=\"$(ti.targets.elf.C66)\" \
    ti.targets.elf.C66_big_endian=\"$(ti.targets.elf.C66_big_endian)\" \
    ti.targets.elf.C674=\"$(ti.targets.elf.C674)\" \
    ti.targets.msp430.elf.MSP430X=\"$(ti.targets.msp430.elf.MSP430X)\" \
    ti.targets.msp430.elf.MSP430X_small=\"$(ti.targets.msp430.elf.MSP430X_small)\" \
    ti.targets.arp32.elf.ARP32=\"$(ti.targets.arp32.elf.ARP32)\" \
    ti.targets.arp32.elf.ARP32_far=\"$(ti.targets.arp32.elf.ARP32_far)\" \
    gnu.targets.arm.M3=\"$(gnu.targets.arm.M3)\" \
    gnu.targets.arm.M4=\"$(gnu.targets.arm.M4)\" \
    gnu.targets.arm.M4F=\"$(gnu.targets.arm.M4F)\" \
    gnu.targets.arm.A8F=\"$(gnu.targets.arm.A8F)\"\
    gnu.targets.arm.A9F=\"$(gnu.targets.arm.A9F)\"\
    gnu.targets.arm.A15F=\"$(gnu.targets.arm.A15F)\"\
    iar.targets.msp430.MSP430X_small=\"$(iar.targets.msp430.MSP430X_small)\" \
    iar.targets.arm.M3=\"$(iar.targets.arm.M3)\" \
    iar.targets.arm.M4=\"$(iar.targets.arm.M4)\" \
    iar.targets.arm.M4F=\"$(iar.targets.arm.M4F)\"

#
# Set XDCPATH to contain necessary repositories.
#
XDCPATH =
export XDCPATH

#
# Set XDCOPTIONS.  Use -v for a verbose build.
#
#XDCOPTIONS=v
export XDCOPTIONS

#
# Set XDC executable command
# Note that XDCBUILDCFG points to the bios.bld file which uses
# the arguments specified by XDCARGS
#
XDC = $(XDC_INSTALL_DIR)/xdc XDCARGS="$(XDCARGS)" XDCBUILDCFG=./bios.bld

######################################################
## Shouldnt have to modify anything below this line ##
######################################################

all:
	@ echo building bios packages ...
	@ $(XDC) .interfaces -Pr ./packages
	@ $(XDC) all -P ./packages/ti/sysbios
	@ $(XDC) all -P ./packages/ti/sysbios/fatfs
	@ $(XDC) all -P ./packages/ti/sysbios/family/c674/pmi
	@ $(XDC) all -P ./packages/ti/sysbios/family/c674/pscl

clean:
	@ echo cleaning bios packages ...
	@ $(XDC) clean -Pr ./packages

install-packages:
	@ echo installing bios packages to $(DESTDIR) ...
	@ mkdir -p $(DESTDIR)/$(prefix)/$(docdir)
	@ cp -rf $(wildcard Bios_*_release_notes.html) docs/* $(DESTDIR)/$(prefix)/$(docdir)
	@ mkdir -p $(DESTDIR)/$(prefix)/$(packagesdir)
	@ cp -rf packages/* $(DESTDIR)/$(prefix)/$(packagesdir)
