#!../../bin/linux-aarch64/motorZynqIOC

< envPaths

epicsEnvSet("PORT", "ZYNQ1")
epicsEnvSet("P", "Det")
epicsEnvSet("R", "{Mtr}")
epicsEnvSet("PREFIX", "$(P)$(R)")

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/motorZynqIOC.dbd"
motorZynqIOC_registerRecordDeviceDriver pdbbase

## Create the motor controller
## zynqMotorCreateController(portName, numAxes, baseAddr, movingPollMs, idlePollMs)
zynqMotorCreateController("ZYNQ1", 4, 0x80000000, 100, 1000)

## Load motor records from substitutions file
dbLoadTemplate("db/motor.substitutions")

## Load custom DRV8434A parameter records from substitutions file
dbLoadTemplate("db/zynqMotor.substitutions")

dbLoadRecords("$(ASYN)/db/asynRecord.db", "P=$(PREFIX),R=asyn1,PORT=$(PORT),ADDR=0,OMAX=0,IMAX=0")

cd "${TOP}/iocBoot/${IOC}"
iocInit
