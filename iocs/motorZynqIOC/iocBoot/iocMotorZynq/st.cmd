#!../../bin/linux-aarch64/motorZynqIOC

< envPaths

epicsEnvSet("P",         "Det")
epicsEnvSet("R",         "Sample")
epicsEnvSet("PORT",      "ZYNQ")
epicsEnvSet("PREFIX",    "$(P){$(R)}")
epicsEnvSet("DRV_MODEL", "DRV8434A")

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/motorZynqIOC.dbd"
motorZynqIOC_registerRecordDeviceDriver pdbbase

## Create the motor controller
## zynqMotorCreateController(portName, numAxes, baseAddr, movingPollMs, idlePollMs)
zynqMotorCreateController("$(PORT)", 4, 0x80000000, 100, 1000, "$(DRV_MODEL)")

## Load motor records from substitutions file
dbLoadTemplate("db/motor.substitutions", "Sys=$(P), Dev=$(R), Port=$(PORT)")

## Load custom DRV8434A parameter records from substitutions file
dbLoadTemplate("db/zynqMotor.substitutions", "Sys=$(P), Dev=$(R), Port=$(PORT)")
dbLoadTemplate("db/$(DRV_MODEL).substitutions", "Sys=$(P), Dev=$(R), Port=$(PORT)")

dbLoadRecords("$(ASYN)/db/asynRecord.db", "P=$(PREFIX),R=asyn1,PORT=$(PORT),ADDR=0,OMAX=0,IMAX=0")

cd "${TOP}/iocBoot/${IOC}"
iocInit
