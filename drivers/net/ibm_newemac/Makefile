#
# Makefile for the PowerPC 4xx on-chip ethernet driver
#

obj-$(CONFIG_IBM_NEW_EMAC) += ibm_newemac.o

ibm_newemac-objs := mal.o core.o phy.o
#ibm_newemac-$(CONFIG_IBM_NEW_EMAC_TOMAL) += tomal.o
ibm_newemac-$(CONFIG_IBM_NEW_EMAC_ZMII) += zmii.o
ibm_newemac-$(CONFIG_IBM_NEW_EMAC_RGMII) += rgmii.o
ibm_newemac-$(CONFIG_IBM_NEW_EMAC_XGMII) += xgmii.o
ibm_newemac-$(CONFIG_IBM_NEW_EMAC_TAH) += tah.o
ibm_newemac-$(CONFIG_IBM_NEW_EMAC_DEBUG) += debug.o
