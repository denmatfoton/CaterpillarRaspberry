include common.mk


TARGET_NAME := RobotApp3
BIN_PATH := $(BINARYDIR)/$(TARGET_NAME)


SRCDIR := source
SOURCE_FILES := $(wildcard $(SRCDIR)/*.cpp)
SOURCE_FILES += $(wildcard $(SRCDIR)/motion/*.cpp)
SOURCE_FILES += $(wildcard $(SRCDIR)/remote_connection/*.cpp)

#add source directories to path
SOURCE_DIRS := $(sort $(dir $(SOURCE_FILES)))
VPATH = $(SOURCE_DIRS)

INCLUDE_DIRS += $(SOURCE_DIRS)
LIBRARY_NAMES += wiringPi pthread


CFLAGS += $(addprefix -I,$(INCLUDE_DIRS))
CXXFLAGS += $(addprefix -I,$(INCLUDE_DIRS))

CFLAGS += $(addprefix -D,$(PREPROCESSOR_MACROS))
CXXFLAGS += $(addprefix -D,$(PREPROCESSOR_MACROS))

LDFLAGS += $(addprefix -L,$(LIBRARY_DIRS))

LIBRARY_LDFLAGS = $(addprefix -l,$(LIBRARY_NAMES))


source_obj1 := $(SOURCE_FILES:.cpp=.o)
source_objs := $(source_obj1:.c=.o)

all_objs := $(addprefix $(OBJDIR)/, $(notdir $(source_objs)))


all: create_dirs $(BIN_PATH)


$(BIN_PATH): $(all_objs)
	$(LD) $(LDFLAGS) $(START_GROUP) $(all_objs) $(LIBRARY_LDFLAGS) $(END_GROUP) -o $@ 


clean:
	rm -rf $(BINARYDIR)
	rm -rf $(OBJDIR)

create_dirs:
	mkdir -p $(BINARYDIR)
	mkdir -p $(OBJDIR)


$(OBJDIR)/%.o : %.cpp
	@echo
	@echo $(MSG_COMPILING_CPP) $<
	$(CXX) $(CXXFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)

$(OBJDIR)/%.o : %.c
	@echo
	@echo $(MSG_COMPILING) $<
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)
