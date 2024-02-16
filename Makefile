# ################################################
#                      工程
# ################################################
TARGET = GiM

# 构建目录
BUILD_DIR = build

GCC_PATH := C:/mingw64-posix/bin/

CC := $(GCC_PATH)/gcc
LD := $(GCC_PATH)/gcc

# ################################################
#                    源文件
# ################################################

INCLUDE_DIRS = \
	-Ilibs/SDL2/include \
	-Ines
	
LIBS = \
	-L./libs/SDL2/lib \
	-lSDL2 \
	-lmingw32

SOURCES = \
	main.c \
	# $(wildcard nes/*.c)

# ################################################
#                  构建可执行文件
# ################################################


CFLAGS  = $(INCLUDE_DIRS) -Wall -Wextra -g 

LDFLAGS = $(LIBS)

vpath %.c $(sort $(dir $(SOURCES)))

# list of objects
OBJECTS  =  $(addprefix $(BUILD_DIR)/,$(notdir $(SOURCES:.c=.o)))


$(BUILD_DIR)/%.o: %.c   Makefile | $(BUILD_DIR) 
	$(CC) -c $< -o $@ $(CFLAGS) 


$(TARGET): $(OBJECTS) Makefile
	$(LD) $(OBJECTS) $(LDFLAGS) -o $@

$(BUILD_DIR):
	mkdir $@	

#######################################
# 命令
#######################################

all: $(TARGET)

.PHONY : clean
clean:
	del /Q /F $(BUILD_DIR)
	del /Q /F $(TARGET).exe


# *** EOF ***