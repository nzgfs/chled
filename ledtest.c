
#include "Ex3_API.h"

int main(void)
{
	int led, cas;
	uint8_t led_sts[4];
	size_t rt_args[] = { 0, 500 };

	int fd_LED = open(LED, O_RDWR);

	while (1)
	{
		cas = 0;

		printf("\n\n\nFS4412_LED Control:\n");
		printf("[1]: switch on or off\n");
		printf("[2]: move a step\n");
		printf("[3]: rotate\n\n\n\n");
		printf("[q]: Quit\n\n");
		printf("Please select: ");
	
		scanf("%d", &cas);
		printf("\n\n");
		
		if (cas == 1)
		{
			
			printf("Please input LED number(1~4): ");
			scanf("%d", &led);
			if(led==2) led=1;
			if(led==4) led=2;
			
			FS4412_LED_switch(fd_LED, led);
		}
		else if (cas == 2)
		{
			while (getchar() != '\n');
			printf("Please input move direction [c(clockwise)/a(anti-clockwise)]: ");
			scanf("%c", &rt_args[0]);
			if (rt_args[0] == 'a' || rt_args[0] == 'A')
				rt_args[0] = 1;
			else if (rt_args[0] == 'c' || rt_args[0] == 'C')
				rt_args[0] = 2;
			FS4412_LED_rotate(fd_LED, rt_args);
		}
		else if (cas == 3)
		{
			while (getchar() != '\n');
			printf("Please input rotate direction [c(clockwise)/a(anti-clockwise)]: ");
			scanf("%c", &rt_args[0]);
			if (rt_args[0] == 'a' || rt_args[0] == 'A')
				rt_args[0] = 3;
			else if (rt_args[0] == 'c' || rt_args[0] == 'C')
				rt_args[0] = 4;
			
			
			
			FS4412_LED_rotate(fd_LED, rt_args);
			printf("rotating...\n");
			sleep(8);
			rt_args[0] = 0;
			FS4412_LED_rotate(fd_LED, rt_args);
			
		}
		else 
			break;
	}
	return 0;
}
