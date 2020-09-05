# MCU name
MCU = atmega32u4

# Bootloader selection
BOOTLOADER = atmel-dfu

MOUSEKEY_ENABLE 	= yes		# Mouse keys(+4700)
EXTRAKEY_ENABLE 	= yes   # Audio control and System control(+450)
CONSOLE_ENABLE 		= yes   # Console for debug(+400)
COMMAND_ENABLE 		= no		# Commands for debug and configuration
NKRO_ENABLE 			= yes   # USB Nkey Rollover


OPT_DEFS 						+= -DDEBUG_MATRIX
EXTRAFLAGS 					+= -flto
