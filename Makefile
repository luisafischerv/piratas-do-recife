.DEFAULT_GOAL := all

-include config.mk

CC      = gcc
TARGET  ?= piratas
BUILD   ?= build
SRC_DIR ?= src
INC_DIR ?= include

CFLAGS   ?= -std=c11 -Wall -Wextra -Wpedantic -O2
CPPFLAGS += -I$(INC_DIR)

SOURCES := $(wildcard $(SRC_DIR)/*.c)
OBJECTS := $(patsubst $(SRC_DIR)/%.c,$(BUILD)/%.o,$(SOURCES))
DEPS    := $(OBJECTS:.o=.d)

# Use RAYLIB_DIR for a local installation, or pkg-config when available.
ifneq ($(strip $(RAYLIB_DIR)),)
CPPFLAGS += -I"$(RAYLIB_DIR)"
LDLIBS   += -L"$(RAYLIB_DIR)" -lraylib
else
CPPFLAGS += $(shell pkg-config --cflags raylib 2>/dev/null)
LDLIBS   += $(shell pkg-config --libs raylib 2>/dev/null)
endif

ifeq ($(OS),Windows_NT)
APP := $(BUILD)/$(TARGET).exe
ifeq ($(strip $(LDLIBS)),)
LDLIBS += -lraylib
endif
LDLIBS += -lopengl32 -lgdi32 -lwinmm
else
APP := $(BUILD)/$(TARGET)
ifeq ($(strip $(LDLIBS)),)
LDLIBS += -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
endif
endif

.PHONY: all run clean

all: $(APP)

run: $(APP)
	./$(APP)

$(APP): $(OBJECTS)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(BUILD)/%.o: $(SRC_DIR)/%.c | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -c $< -o $@

$(BUILD):
ifeq ($(OS),Windows_NT)
	@powershell -NoProfile -Command "New-Item -ItemType Directory -Force -Path '$(BUILD)' | Out-Null"
else
	@mkdir -p $(BUILD)
endif

clean:
ifeq ($(OS),Windows_NT)
	@powershell -NoProfile -Command "if (Test-Path -LiteralPath '$(BUILD)') { Remove-Item -LiteralPath '$(BUILD)' -Recurse -Force }"
else
	$(RM) -r $(BUILD)
endif

-include $(DEPS)
