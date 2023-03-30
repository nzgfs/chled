#include <stdio.h>
#include "test.h"
#include "Ex3_API.h"
#include <stdint.h>//uint8_t的头文件 
#include <pthread.h>
#include <sys/wait.h>
//Chen Hang

uint8_t state[2]={0,0};

int n=1;
int inputflag; 

 
void *print(void *arg){
	                        
	while(n){                     
		if(!inputflag){               //主线程非输入状态
			
			HIDE_CURSOR();     //隐藏光标
				MOVETO(2,9);  //移动光标到led2
				
				if(state[0]==1)  color(blue);
				else color(white);
				printf("  ");
				color(none);//重新更改回默认背景色，防止其余位置颜色打印错误
				MOVETO(4,8);//移动光标到led2标签
				printf("LED 2");
			HIDE_CURSOR();     //隐藏光标
				MOVETO(2,29);  //移动光标到led4

				if(state[1]==1)  color(blue);
				else color(white);
				printf("  ");
				color(none);
				MOVETO(4,28);//移动光标到led4标签
				printf("LED 4");
			
			printf("\n");                   //完成图形界面打印
			color(none);      
			usleep(50000);                  //更新缓冲时间
		}
	}
	pthread_exit(NULL);                    //线程处于结束状态则停止线程
}


void rotate(){
	uint8_t temp = state[0];           
		 
	state[0]=state[1];
	state[1]=temp;    
			
		
		
}

int main(void)
{
	CLEAR();//清屏
	int led, cas;

	int fd_LED = open(LED, O_RDWR);
	size_t rt_args[] = { 0, 500 };
	
	pthread_t new;
	inputflag=1;                                  
	pthread_create(&new,NULL,print,NULL); //将新线程new与执行的函数print绑定 
	
	//打印初始界面 
	
	HIDE_CURSOR(); 
		MOVETO(2,9);
		if(state[0]==1) color(blue);
		else color(white);
		printf("  ");
		color(none);
		MOVETO(4,8);
		printf("LED 2");
	HIDE_CURSOR(); 
		MOVETO(2,29);
		if(state[1]==1) color(blue);
		else color(white);
		printf("  ");
		color(none);
		MOVETO(4,28);
		printf("LED 4");
	printf("\n");
	color(none);

    
	while (1)
	{
		cas = 0;                                //接收用户输入的命令 
		inputflag=1;                              //此时主线程处于待输入状态 
		color(49);
		MOVETO(5,0);                            //光标移动至打印菜单处 
		//打印菜单 
		printf("\n\n\nFS4412_LED Control:\n"); 
		printf("[1]: switch on or off\n");
		printf("[2]: move a step\n");
		printf("[3]: rotate\n\n\n\n");
		printf("[q]: Quit\n\n");
		//移光标动至提示用户输入处 
		MOVETO(17,0);
		SHOW_CURSOR();                         //显示光标提示用户输入位置 
		color(49);
		printf("Please select: ");              
		SHOW_CURSOR();
		MOVETO(17,15);
		scanf("%d", &cas);                     
		printf("\n\n");
		
		 
		if (cas == 1)                           
		{
			inputflag=1;                         //待输入
			
			MOVETO(19,0);
			color(49);
			printf("Please input LED number(2/4): ");
			SHOW_CURSOR();                     
			scanf("%d", &led); 
			while(led!=2&&led!=4)
			{
				MOVETO(19,0);
				printf("                                ");
				inputflag=1;                         
				
				MOVETO(19,0);
				color(49);
				printf("Please input 2 or 4: ");
				SHOW_CURSOR();                     
				scanf("%d", &led);
			}
            if(led==2) led=1;
            if(led==4) led=2;               
			FS4412_LED_switch(fd_LED, led);    
			state[led-1]=!state[led-1];    
			 
			inputflag=0;                         //结束输入
			usleep(750000);                    //新线程缓冲并更新所有小灯状态 
			//手动清除功能1提示 
			MOVETO(19,0);
			printf("                                ");
		}
		else if (cas == 2)                           
		{
			while (getchar() != '\n');              //清除换行符保证后续输入正确 
			
			if(state[0]!=state[1])
			{
				rt_args[0] = 1;
				FS4412_LED_rotate(fd_LED, rt_args);   
			}
			
			rotate();                             
			inputflag=0;
			usleep(500000);                        
			
			MOVETO(19,0);
			
		}
		else if (cas == 3)                           
		{
			while (getchar() != '\n');               
			
			if(state[0]!=state[1])
			{
				rt_args[0] = 3;
				FS4412_LED_rotate(fd_LED, rt_args);    
			
			}
			MOVETO(20,0);
			printf("rotating...\n");
			inputflag=0;
			//根据FS4412_LED_rotate函数，控制LED灯循环的时间间隔为0.5秒
			
			rotate();
			int k;                               
			for(k=0;k<10;k++){                          //5秒对应10个间隔 
				usleep(500000);                    //新线程缓冲并更新所有小灯状态 
				rotate();                        //移动小灯状态数组，保证硬件和界面一致性  
			} 
			
			MOVETO(20,0);
			printf("                ");
			//停止LED灯循环移动 
			if(state[0]!=state[1])
			{
				rt_args[0] = 0;
				FS4412_LED_rotate(fd_LED, rt_args);
			}
			
			usleep(500000);                        //新线程缓冲并更新所有小灯状态 
		}
		else                                       //选择q退出 
			break;
	}
	n=0;//线程结束 
	inputflag=1; 
	//将光标移动至便于用户输入的位置，保证退出时界面的美观 
	MOVETO(20,0);
	color(49);
	return 0;
}

