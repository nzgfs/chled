#include <stdio.h>
//清屏
#define CLEAR() printf("\033[2J")
// 定位光标
#define MOVETO(x,y) printf("\033[%d;%dH", (x), (y))

// 隐藏光标
#define HIDE_CURSOR() printf("\033[?25l")
 
// 显示光标
#define SHOW_CURSOR() printf("\033[?25h")
 
//清除从光标到行尾的内容
#define CLEAR_LINE() printf("\033[K")
 
//设置颜色
#define color(color) printf("\033[%dm",(color))


#define blue 44
#define white 47
#define none 49
