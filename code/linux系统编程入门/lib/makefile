# 定义变量

# src=add.o sub.o
src=$(wildcard ./*.c)
objs=$(patsubst %.c, %.o, $(src))

app:add.o sub.o
	$(CC) $(objs) -o app

# add.o:add.c
# 	gcc -c add.c -o add.o

# sub.o:sub.c
# 	gcc -c sub.c -o sub.o

# 模式匹配
# 本质是个规则
%.o:%.c
	$(CC) %< -o $@

.PHONY:clean
clean:
	rm $(objs) -f

