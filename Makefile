CC      ?= gcc
TARGET  ?= piratas
SRC_DIR ?= src
INC_DIR ?= include
BUILD   ?= build

SOURCES := $(wildcard $(SRC_DIR)/*.c)
OBJECTS := $(patsubst $(SRC_DIR)/%.c,$(BUILD)/%.o,$(SOURCES))

CPPFLAGS += -I$(INC_DIR)
CFLAGS   ?= -std=c11 -Wall -Wextra -Wpedantic -O2

# Prefer pkg-config (Linux/macOS and some Windows setups).
RAYLIB_CFLAGS ?= $(shell pkg-config --cflags raylib 2>/dev/null)
RAYLIB_LIBS   ?= $(shell pkg-config --libs raylib 2>/dev/null)

# Override these on Windows if pkg-config is unavailable, for example:
# mingw32-make RAYLIB_CFLAGS="-IC:/raylib/include" RAYLIB_LIBS="-LC:/raylib/lib -lraylib -lopengl32 -lgdi32 -lwinmm"
CPPFLAGS += $(RAYLIB_CFLAGS)
LDLIBS   += $(RAYLIB_LIBS)

ifeq ($(OS),Windows_NT)
    EXE := .exe
    ifeq ($(strip $(RAYLIB_LIBS)),)
        LDLIBS += -lraylib -lopengl32 -lgdi32 -lwinmm
    endif
else
    EXE :=
    ifeq ($(strip $(RAYLIB_LIBS)),)
        LDLIBS += -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
    endif
endif

.PHONY: all run clean info

all: $(BUILD)/$(TARGET)$(EXE)

$(BUILD)/$(TARGET)$(EXE): $(OBJECTS)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(BUILD)/%.o: $(SRC_DIR)/%.c | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BUILD):
	@mkdir -p $(BUILD)

run: all
	./$(BUILD)/$(TARGET)$(EXE)

clean:
	$(RM) -r $(BUILD)

info:
	@echo "Compiler: $(CC)"
	@echo "Sources:  $(SOURCES)"
	@echo "Raylib CFLAGS: $(RAYLIB_CFLAGS)"
	@echo "Raylib LIBS:   $(RAYLIB_LIBS)"
