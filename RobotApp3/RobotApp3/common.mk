BINARYDIR := bin
OBJDIR := obj


ifeq ($(OS),Windows_NT)
#Toolchain
    TOOLCHAIN_ROOT := C:/SysGCC/raspberry
    
	CC := $(TOOLCHAIN_ROOT)/bin/arm-linux-gnueabihf-gcc.exe
	CXX := $(TOOLCHAIN_ROOT)/bin/arm-linux-gnueabihf-g++.exe
	LD := $(CXX)
	AR := $(TOOLCHAIN_ROOT)/bin/arm-linux-gnueabihf-ar.exe
	OBJCOPY := $(TOOLCHAIN_ROOT)/bin/arm-linux-gnueabihf-objcopy.exe
else
	CC := gcc
	CXX := g++
	LD := $(CXX)
	AR := ar
	OBJCOPY := objcopy
endif

PREPROCESSOR_MACROS := DEBUG=1
INCLUDE_DIRS :=
LIBRARY_DIRS :=
LIBRARY_NAMES :=
ADDITIONAL_LINKER_INPUTS := 

CFLAGS := -ggdb -ffunction-sections -O0
CXXFLAGS := -ggdb -ffunction-sections -O0
LDFLAGS := -Wl,-gc-sections

START_GROUP := -Wl,--start-group
END_GROUP := -Wl,--end-group


# Define Messages
# English
MSG_ERRORS_NONE = Errors: none
MSG_BEGIN = -------- begin --------
MSG_END = --------  end  --------
MSG_SIZE_BEFORE = Size before: 
MSG_SIZE_AFTER = Size after:
MSG_FLASH = Creating load file for Flash:
MSG_EXTENDED_LISTING = Creating Extended Listing:
MSG_SYMBOL_TABLE = Creating Symbol Table:
MSG_LINKING = Linking:
MSG_COMPILING = Compiling C:
MSG_COMPILING_CPP = Compiling C++:
MSG_ASSEMBLING = Assembling:
MSG_CLEANING = Cleaning project:
MSG_CREATING_LIBRARY = Creating library:

