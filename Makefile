# Makefile for Top application
TOP = .
include $(TOP)/configure/CONFIG
DIRS += configure
DIRS += motorZynqApp
DIRS += iocs
motorZynqApp_DEPEND_DIRS = configure
include $(TOP)/configure/RULES_TOP
