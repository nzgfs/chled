
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/io.h>
#include <linux/err.h>
#include <linux/uaccess.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/device.h>
#include <linux/atomic.h>

#include "FS4412_LED_drv.h"

#define DEV_NAME ("FS4412_LED")

#define LED_BASEMINOR 0
#define LED_COUNT 1

#define LOCK 0
#define ULOCK 1

#define MAX_NR 2
#define LED_MAGIC 'L'
#define LED_STATE	_IOR(LED_MAGIC, 0, uint8_t)
#define LED_SWITCH	_IOW(LED_MAGIC, 1, uint8_t)
#define LED_ROTATE	_IOW(LED_MAGIC, MAX_NR, size_t)

MODULE_AUTHOR("Chen Hang");
MODULE_DESCRIPTION("LED drv 2/4 for Ex3");
MODULE_LICENSE("GPL v2");
MODULE_VERSION("2.0");

struct FS4412_LED_struct
{
	dev_t cd;
	spinlock_t slock;
	atomic_t count;
	uint8_t dat[2];
	uint8_t state[2];
	size_t rt_cmd[2];
	struct cdev Ldev;
	struct task_struct *rt_thrd;
	struct class *cls;
	struct device *dev;
	GPIO_TypeDef *reg[2];
};

struct FS4412_LED_reg_ops_struct
{
	uint8_t GPIO_DAT_POS[2];
	uint8_t GPIO_DAT_MSK[2];
	uint8_t GPIO_DAT_CLR[2];
};

static int __init FS4412_LED_drv_init(void);
static void __exit FS4412_LED_drv_exit(void);

static int FS4412_LED_ioremap(void);
static void FS4412_LED_iounmap(void);

static int FS4412_LED_open(struct inode *inode, struct file *filp);
static int FS4412_LED_release(struct inode *inode, struct file *filp);
static long FS4412_LED_unlocked_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);

static void FS4412_LED_init(void);
static void FS4412_LED_deinit(void);

static void FS4412_LED_state(const uint8_t lock_flg);
static void FS4412_LED_switch(uint8_t pos);
static int FS4412_LED_rtctl(void);
static int FS4412_LED_rotate(void);

static struct FS4412_LED_struct FS4412_LED;
static struct FS4412_LED_reg_ops_struct FS4412_LED_reg_ops =
{
		{GPF3_DAT_4_POS, GPF3_DAT_5_POS},
		{GPF3_DAT_4_MSK, GPF3_DAT_5_MSK},
		{GPF3_DAT_4_CLR, GPF3_DAT_5_CLR}
};

static struct file_operations fops =
{
	.owner = THIS_MODULE,
	.open = FS4412_LED_open,
	.release = FS4412_LED_release,
	.unlocked_ioctl = FS4412_LED_unlocked_ioctl,
};

static int __init FS4412_LED_drv_init(void)
{
	int err;

	/* allocate device numbers to LED */
	err = alloc_chrdev_region(&FS4412_LED.cd, LED_BASEMINOR, LED_COUNT, DEV_NAME);//动态分配设备号
	if (err < 0)
	{
		printk(KERN_ERR "fail to allocate LED device number\n");
		return err;
	}

	/* initialize cdev structure of LED */
	cdev_init(&FS4412_LED.Ldev, &fops);//初始化cdev结构
	FS4412_LED.Ldev.owner = THIS_MODULE;

	/* add LED device to the system */
	err = cdev_add(&FS4412_LED.Ldev, FS4412_LED.cd, LED_COUNT);//注册
	if (err < 0)
	{
		printk(KERN_ERR "fail to add LED to system\n");
		goto cdev_add_err;
	}

	/* map GPIO */
	err = FS4412_LED_ioremap();//将通用输入输出引脚的IO地址空间映射到内核的虚拟地址空间，便于访问
	if (err < 0)
	{
		printk(KERN_ERR "fail to remap LED\n");
		goto ioremap_err;
	}

	/* create LED device */
	//加载模块时用户控件中的udev会自动响应device_create函数，在/sysfs下寻找对应的类从而创建设备节点
	FS4412_LED.cls = class_create(THIS_MODULE, DEV_NAME);//创建类存放到sysfs
	if (IS_ERR(FS4412_LED.cls))
	{
		printk(KERN_ERR "fail to create class");
		return PTR_ERR(FS4412_LED.cls);//错误码
	}
	FS4412_LED.dev = device_create(FS4412_LED.cls, NULL, FS4412_LED.cd, NULL, DEV_NAME);//创建设备节点
	if (IS_ERR(FS4412_LED.dev))
	{
		printk(KERN_ERR "fail to create device");
		return PTR_ERR(FS4412_LED.dev);
	}

	printk(KERN_NOTICE "insert LED drv\n");
	return 0;

ioremap_err:
	cdev_del(&FS4412_LED.Ldev);
cdev_add_err:
	unregister_chrdev_region(FS4412_LED.cd, LED_COUNT);
	return err;
}

static void __exit FS4412_LED_drv_exit(void)
{
	/* remove LED device */
	device_destroy(FS4412_LED.cls, FS4412_LED.cd);
	class_destroy(FS4412_LED.cls);
	/* unmap GPIO */
	FS4412_LED_iounmap();
	/* remove LED from the system */
	cdev_del(&FS4412_LED.Ldev);
	/* unregister device numbers of LED */
	unregister_chrdev_region(FS4412_LED.cd, LED_COUNT);
	printk(KERN_NOTICE "remove LED drv\n");
}

/**********************************************************************
 * @Function:	FS4412_LED_ioremap
 * @Description:	map physical address of GPIO register
 * 					related to LED to virtual address space
 * @Param[in]:	None
 * @Return:	0 if success, otherwise -EIO
 **********************************************************************/
static int FS4412_LED_ioremap(void)//将与 LED 相关的 GPIO 寄存器的物理地址映射到虚拟地址空间
{
	FS4412_LED.reg[0] = FS4412_LED.reg[1] = ioremap(GPF3_BASE, 8);
	if (!FS4412_LED.reg[0])
		return -EIO;
	
	return 0;
}

/**********************************************************************
 * @Function:	FS4412_LED_iounmap
 * @Description:	unmap GPIO register related to LED
 * @Param[in]:	None
 * @Return:	None
 **********************************************************************/
static void FS4412_LED_iounmap(void)
{
	iounmap(FS4412_LED.reg[0]);
	
}

static int FS4412_LED_open(struct inode *inode, struct file *filp)
{
	/* to be exclusive */
	if (atomic_read(&FS4412_LED.count))
	{
		printk(KERN_ERR "LED busy\n");
		return -EBUSY;
	}
	atomic_add(1, &FS4412_LED.count);//保证不被打断
	spin_lock_init(&FS4412_LED.slock);//保证只能同时被一个进程打开
	FS4412_LED_init();
	return 0;
}

static int FS4412_LED_release(struct inode *inode, struct file *filp)
{
	if (FS4412_LED.rt_thrd)
	{
		kthread_stop(FS4412_LED.rt_thrd);
		FS4412_LED.rt_thrd = NULL;
	}
	FS4412_LED_deinit();
	if (!atomic_read(&FS4412_LED.count))
	{
		printk(KERN_ERR "device closed before");
		return -ENODEV;
	}
	atomic_sub(1, &FS4412_LED.count);
	return 0;
}

static long FS4412_LED_unlocked_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	switch(cmd)
	{
	case LED_STATE:
		FS4412_LED_state(ULOCK);
		if (copy_to_user((void *) arg, FS4412_LED.state, //将内核空间的内容复制到用户空间
				sizeof(FS4412_LED.state)))
		{
			printk(KERN_ERR "fail to get LED state\n");
			return -EINVAL;
		}
		break;
	case LED_SWITCH:
		if (!arg || arg > 4)
			goto inval_err;
		FS4412_LED_switch(arg);
		break;
	case LED_ROTATE:
		if (copy_from_user(FS4412_LED.rt_cmd, (void *) arg,
				sizeof(FS4412_LED.rt_cmd)))
		{
			printk(KERN_ERR "fail to pass arguments\n");
			return -EINVAL;
		}
		if (FS4412_LED.rt_cmd[0] > 4
				|| FS4412_LED.rt_cmd[0] > 2 && FS4412_LED.rt_cmd[1] < 100)
			goto inval_err;
		return FS4412_LED_rtctl();
	}
	return 0;

inval_err:
	printk(KERN_ERR "invalid ioctl args\n");
	return -EINVAL;
}

/**********************************************************************
 * @Function:	FS4412_LED_init
 * @Description:	initialize GPIO controlling LEDs
 * @Param[in]:	None
 * @Return:	None
 **********************************************************************/
static void FS4412_LED_init(void)//初始化 GPIO 控制 LED
{
	writel( //往内存映射的I/O空间写32位数据
			readl(&FS4412_LED.reg[0]->CON) & GPF3_CON_4_CLR & GPF3_CON_5_CLR   //readl从内存映射的I/O空间读取32位数据
					| GPF3_CON_4_OUT | GPF3_CON_5_OUT, &FS4412_LED.reg[0]->CON); 
	FS4412_LED_deinit();//关灯
}

/**********************************************************************
 * @Function:	FS4412_LED_deinit
 * @Description:	switch off LEDs
 * @Param[in]:	None
 * @Return:	None
 **********************************************************************/
static void FS4412_LED_deinit(void)
{
	FS4412_LED_state(LOCK);
	writel(
			FS4412_LED.dat[0] & FS4412_LED_reg_ops.GPIO_DAT_CLR[0]
					& FS4412_LED_reg_ops.GPIO_DAT_CLR[1],
			&FS4412_LED.reg[0]->DAT);
	spin_unlock(&FS4412_LED.slock);
}

/**********************************************************************
 * @Function:	FS4412_LED_state
 * @Description:	read GPIO data registers to get LED state
 * @Param[in]:	(uint8_t)lock_flg: return with spin_lock if not 0
 * @Return:	None
 **********************************************************************/
static void FS4412_LED_state(const uint8_t lock_flg) //读取 GPIO 数据寄存器以获取 LED 状态
{
	uint8_t i;
	spin_lock(&FS4412_LED.slock);
	for (i = 0; i < 2; i++)
	{
		FS4412_LED.dat[i] = readl(&FS4412_LED.reg[i]->DAT);
		FS4412_LED.state[i] = FS4412_LED.dat[i]
								& FS4412_LED_reg_ops.GPIO_DAT_MSK[i];
		FS4412_LED.state[i] >>= FS4412_LED_reg_ops.GPIO_DAT_POS[i]; //右移后赋值
	}
	if (lock_flg)//lock锁，ulock解锁
		spin_unlock(&FS4412_LED.slock);
}

/**********************************************************************
 * @Function:	FS4412_LED_switch
 * @Description:	switch on/off LED
 * @Param[in]:	(uint8_t)pos: LED number
 * @Return:	None
 **********************************************************************/
static void FS4412_LED_switch(uint8_t pos)
{

	uint8_t dat;
	spin_lock(&FS4412_LED.slock);
	dat = readl(&FS4412_LED.reg[--pos]->DAT);
	writel(
			dat & FS4412_LED_reg_ops.GPIO_DAT_MSK[pos] ?
					dat & FS4412_LED_reg_ops.GPIO_DAT_CLR[pos] :
					dat | FS4412_LED_reg_ops.GPIO_DAT_MSK[pos],
			&FS4412_LED.reg[pos]->DAT);
	spin_unlock(&FS4412_LED.slock);
}

/**********************************************************************
 * @Function:	FS4412_LED_rtctl
 * @Description: control the movement of LED
 * @Param[in]:	None
 * @Return:	0 if success, otherwise -ENOMEM
 **********************************************************************/
static int FS4412_LED_rtctl(void)
{
	if (FS4412_LED.rt_cmd[0] > 2 && !FS4412_LED.rt_thrd)
	{
		FS4412_LED.rt_thrd = kthread_run((int (*)(void *)) FS4412_LED_rotate,
								NULL, "FS4412_LED_rotate");
		if (IS_ERR(FS4412_LED.rt_thrd))
			printk(KERN_ERR "fail to create kthread\n");
		return PTR_ERR(FS4412_LED.rt_thrd);
	}
	else if (FS4412_LED.rt_cmd[0] <= 2 && FS4412_LED.rt_thrd)
	{
		kthread_stop(FS4412_LED.rt_thrd);
		FS4412_LED.rt_thrd = NULL;
	}
	if (FS4412_LED.rt_cmd[0] && FS4412_LED.rt_cmd[0] <= 2)
		FS4412_LED_rotate();
	return 0;
}

/**********************************************************************
 * @Function:	FS4412_LED_rotate
 * @Description:	move LED
 * @Param[in]:	None
 * @Return:	0 if success
 **********************************************************************/
static int FS4412_LED_rotate(void)
{
	
	uint8_t dat;
	do{
		int i;
		for(i=0;i<2;i++)
		{
			spin_lock(&FS4412_LED.slock);
			dat = readl(&FS4412_LED.reg[i]->DAT);
			writel(
				dat & FS4412_LED_reg_ops.GPIO_DAT_MSK[i] ?
					dat & FS4412_LED_reg_ops.GPIO_DAT_CLR[i] :
					dat | FS4412_LED_reg_ops.GPIO_DAT_MSK[i],
			&FS4412_LED.reg[i]->DAT);
			spin_unlock(&FS4412_LED.slock);
			
		}
		if(FS4412_LED.rt_cmd[0] > 2) 
			mdelay(FS4412_LED.rt_cmd[1]);
	} while (FS4412_LED.rt_cmd[0] > 2 && !kthread_should_stop());
	
	return 0;
	
	
}

module_init(FS4412_LED_drv_init);//加载
module_exit(FS4412_LED_drv_exit);//卸载

