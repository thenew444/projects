CC       := clang
# 1. Added $(shell ...) and appended GTK cflags so Clang can find gtk.h during compilation
GTK_CFLAGS := $(shell pkg-config --cflags gtk4)
GTK_LIBS   := $(shell pkg-config --libs gtk4)

CFLAGS   := -Wall -Wextra -Iinclude -g -fcolor-diagnostics $(GTK_CFLAGS)
LDFLAGS  := $(GTK_LIBS)

SRC_DIR  := src
BUILD_DIR:= build
BIN_DIR  := bin
INC_DIR  := include

SOURCES  := $(wildcard $(SRC_DIR)/*.c)
NAME     := $(patsubst $(SRC_DIR)/%.c,%,$(firstword $(SOURCES)))
TARGET   := $(BIN_DIR)/$(NAME)
C_OBJECTS := $(SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
OBJECTS   := $(C_OBJECTS) $(BUILD_DIR)/resources.o

# Blueprint target configuration
GENERATED_UI       := $(INC_DIR)/builder.ui
BLUEPRINT_COMPILER := /usr/bin/blueprint-compiler

# Colors
CYAN   := \033[1;36m
GREEN  := \033[1;32m
RESET  := \033[0m

all: $(TARGET)

# Link the executable
$(TARGET): $(GENERATED_UI) $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	@echo "$(CYAN)Linking $(TARGET)...$(RESET)"
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	@echo "$(GREEN)Done!$(RESET)"

# ADDED: Rule to compile modern Blueprint (.blp) layout into GTK XML (.ui)
$(GENERATED_UI): $(INC_DIR)/builder.blp
	@echo "Compiling Blueprint layout..."
	$(BLUEPRINT_COMPILER) compile $< --output $@

# FIXED: Replaced static include/builder.ui with $(GENERATED_UI) variable tracking
$(BUILD_DIR)/resources.c: $(INC_DIR)/resources.gresource.xml $(GENERATED_UI) $(INC_DIR)/style.css
	@mkdir -p $(BUILD_DIR)
	@echo "Generating resources bundle..."
	glib-compile-resources $(INC_DIR)/resources.gresource.xml --target=$@ --sourcedir=$(INC_DIR) --generate-source

$(BUILD_DIR)/resources.o: $(BUILD_DIR)/resources.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compile source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "Cleaning folders..."
	rm -rf $(BUILD_DIR) $(BIN_DIR) $(GENERATED_UI)

run: all
	@GSK_RENDERER=cairo ./$(TARGET)

.PHONY: all clean run