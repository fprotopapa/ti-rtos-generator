################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
CFG_SRCS += \
../app.cfg 

CMD_SRCS += \
../F2837xS_Headers_BIOS.cmd \
../TMS320F28377S.cmd 

ASM_SRCS += \
../F2837xS_CodeStartBranch.asm \
../F2837xS_usDelay.asm 

C_SRCS += \
../F2837xS_CpuTimers.c \
../F2837xS_GlobalVariableDefs.c \
../F2837xS_Gpio.c \
../F2837xS_SysCtrl.c \
../gui.c \
../main.c 

GEN_CMDS += \
./configPkg/linker.cmd 

GEN_FILES += \
./configPkg/linker.cmd \
./configPkg/compiler.opt 

GEN_MISC_DIRS += \
./configPkg/ 

C_DEPS += \
./F2837xS_CpuTimers.d \
./F2837xS_GlobalVariableDefs.d \
./F2837xS_Gpio.d \
./F2837xS_SysCtrl.d \
./gui.d \
./main.d 

GEN_OPTS += \
./configPkg/compiler.opt 

OBJS += \
./F2837xS_CodeStartBranch.obj \
./F2837xS_CpuTimers.obj \
./F2837xS_GlobalVariableDefs.obj \
./F2837xS_Gpio.obj \
./F2837xS_SysCtrl.obj \
./F2837xS_usDelay.obj \
./gui.obj \
./main.obj 

ASM_DEPS += \
./F2837xS_CodeStartBranch.d \
./F2837xS_usDelay.d 

GEN_MISC_DIRS__QUOTED += \
"configPkg\" 

OBJS__QUOTED += \
"F2837xS_CodeStartBranch.obj" \
"F2837xS_CpuTimers.obj" \
"F2837xS_GlobalVariableDefs.obj" \
"F2837xS_Gpio.obj" \
"F2837xS_SysCtrl.obj" \
"F2837xS_usDelay.obj" \
"gui.obj" \
"main.obj" 

C_DEPS__QUOTED += \
"F2837xS_CpuTimers.d" \
"F2837xS_GlobalVariableDefs.d" \
"F2837xS_Gpio.d" \
"F2837xS_SysCtrl.d" \
"gui.d" \
"main.d" 

GEN_FILES__QUOTED += \
"configPkg\linker.cmd" \
"configPkg\compiler.opt" 

ASM_DEPS__QUOTED += \
"F2837xS_CodeStartBranch.d" \
"F2837xS_usDelay.d" 

ASM_SRCS__QUOTED += \
"../F2837xS_CodeStartBranch.asm" \
"../F2837xS_usDelay.asm" 

C_SRCS__QUOTED += \
"../F2837xS_CpuTimers.c" \
"../F2837xS_GlobalVariableDefs.c" \
"../F2837xS_Gpio.c" \
"../F2837xS_SysCtrl.c" \
"../gui.c" \
"../main.c" 


