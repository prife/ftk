ARCH := x86
TARGET := testftk

CFLAGS = -DBUILD_ON_PC

#CC = $(COMPILE)clang
CC = $(COMPILE)gcc

FTK_ROOT := ../

INCLUDE += $(FTK_ROOT)
INCLUDE += -I./

CFLAGS += -g -Wall

LIBS := ftk jpeg pthread z
DIRS := ./

LIB += $(foreach lib, $(LIBS), -l$(lib))
SRC += $(foreach dir, $(DIRS), $(wildcard $(dir)/*.c))

#SRC+=$(wildcard *.c)
#NEWSRC := $(filter-out ../mainUI//dvbui_pf_message_box.c, $(SRC))
#SRC=$(NEWSRC)

OBJS += $(addsuffix .o, $(basename $(SRC)))

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) -o $@ $(SRC) -I$(INCLUDE) $(LIB) $(CFLAGS)

clean :
	$(RM) $(OBJS) $(TARGET)
