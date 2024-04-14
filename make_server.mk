# Many thanks to Job Vranish - see https://spin.atomicobject.com/2016/08/26/makefile-c-projects/

TARGET_EXEC := chat_server

BUILD_DIR := ./build_server
SRC_DIRS  := ./src

SRCS := $(wildcard $(SRC_DIRS)/*.c) $(wildcard $(SRC_DIRS)/**/*.c) $(wildcard $(SRC_DIRS)/**/*.s)

# Exclude client.c since it has the main function for client
EXCLUDED_SRCS := ./src/client/client_main.c
SRCS := $(filter-out $(EXCLUDED_SRCS), $(SRCS))

OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS  := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS := $(INC_FLAGS) -MMD -MP
LDFLAGS  := -pthread -lpthread

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# assembly
$(BUILD_DIR)/%.s.o: %.s
	$(MKDIR_P) $(dir $@)
	$(AS) $(ASFLAGS) -c $< -o $@

# c source
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# c++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@


.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p
