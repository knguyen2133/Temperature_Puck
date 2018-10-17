#
_XDCBUILDCOUNT = 
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = C:/ti/simplelink_cc13x0_sdk_1_60_00_21/source;C:/ti/simplelink_cc13x0_sdk_1_60_00_21/kernel/tirtos/packages;C:/ti/ccsv7/ccs_base;C:/Users/Kevin/Documents/GitHub/Temperature_Puck/tirtos_builds_CC1310_LAUNCHXL_release_ccs/.config
override XDCROOT = C:/ti/xdctools_3_50_04_43_core
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = C:/ti/simplelink_cc13x0_sdk_1_60_00_21/source;C:/ti/simplelink_cc13x0_sdk_1_60_00_21/kernel/tirtos/packages;C:/ti/ccsv7/ccs_base;C:/Users/Kevin/Documents/GitHub/Temperature_Puck/tirtos_builds_CC1310_LAUNCHXL_release_ccs/.config;C:/ti/xdctools_3_50_04_43_core/packages;..
HOSTOS = Windows
endif
