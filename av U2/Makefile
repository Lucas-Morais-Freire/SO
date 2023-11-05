C_FILES_BROAD := broadcast_test.c
C_FILES_SIGNAL := signal_test.c
H_FILES := mycondvar.h
OBJS_BROAD := $(C_FILES_BROAD:.c=.o)
OBJS_SIGNAL := $(C_FILES_SIGNAL:.c=.o)

C_FLAGS := -Wall -Wextra -I.

all: signal broadcast

signal: $(OBJS_SIGNAL)
	gcc -o $@ $^

broadcast: $(OBJS_BROAD)
	gcc -o $@ $^

%.o: %.c $(H_FILES)
	gcc -c $< $(C_FLAGS) -o $@

clean:
	rm -f .\broadcast.exe .\signal.exe $(OBJS_BROAD) $(OBJS_SIGNAL)