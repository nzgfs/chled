
#ifndef _FS4412_LED_DRV_H
#define _FS4412_LED_DRV_H

	/*
	 * 	SFR memory map
	 */
#define GPIO_RIGHT_BASE 0x11000000U
#define GPIO_LEFT_BASE 0x11400000U
#define GPF3_BASE (GPIO_LEFT_BASE + 0x01E0U)


	/*
	 * 	SFR structures
	 */
typedef struct//特殊功能寄存器
{
	uint32_t CON;
	uint32_t DAT;
} GPIO_TypeDef;

	/*
	 * 	Bit operation
	 */

	/* Bit operation for GPF3_CON */
#define GPF3_CON_4_POS (16U)
#define GPF3_CON_4_MSK (0x0FU << GPF3_CON_4_POS)
#define GPF3_CON_4_CLR (~GPF3_CON_4_MSK)
#define GPF3_CON_4_OUT (0x01U << GPF3_CON_4_POS)

#define GPF3_CON_5_POS (20U)
#define GPF3_CON_5_MSK (0x0FU << GPF3_CON_5_POS)
#define GPF3_CON_5_CLR (~GPF3_CON_5_MSK)
#define GPF3_CON_5_OUT (0x01U << GPF3_CON_5_POS)



	/* Bit operation for GPF3_DAT */
#define GPF3_DAT_4_POS (4U)
#define GPF3_DAT_4_MSK (0x01U << GPF3_DAT_4_POS)
#define GPF3_DAT_4_CLR (~GPF3_DAT_4_MSK)

#define GPF3_DAT_5_POS (5U)
#define GPF3_DAT_5_MSK (0x01U << GPF3_DAT_5_POS)
#define GPF3_DAT_5_CLR (~GPF3_DAT_5_MSK)



#endif
