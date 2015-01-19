
PROGRAM := im_update                       # 最后的可执行文件

SOURCES := $(wildcard *.c)                 #通配当前目录下说有的.c文件
OBJS    := $(patsubst %.c,%.o,$(SOURCES))  #把$(SOURCES)中的变量符合后缀是.c的全部替换成.o，

.PHONY: clean 

$(PROGRAM): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)
%.o: %.c
	$(CC) -c $^ $(CFLAGS) $(INCLUDE)            # $@:当前规则的目的文件名， $<:依靠列表中的第 一个依靠文件，而 $^:整个依靠的列表（除掉了里面所有重复的文件名）。

clean:
	rm *.o $(PROGRAM) -f

