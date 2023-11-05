C_FILES_BROAD := broadcast_test.c
H_FILES := mycondvar.h
OBJS_BROAD := $(C_FILES_BROAD:.c=.o)

C_FLAGS := -Wall -Wextra -I.

broad: broadcast

broadcast: $(OBJS_BROAD)
	gcc -o $@ $^

%.o: %.c $(H_FILES)
	gcc -c $< $(C_FLAGS) -o $@

clean:
	rm -f myprogram $(OBJS_BROAD)