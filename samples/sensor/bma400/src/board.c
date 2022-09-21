#include <init.h>
#include <device.h>
#include <kernel.h>

#define UICR_REGOUT0_VOUT_TARGET UICR_REGOUT0_VOUT_3V3

static int set_io_voltage( void )
{
	if (UICR_REGOUT0_VOUT_TARGET !=
	     (NRF_UICR->REGOUT0 & ((uint32_t)UICR_REGOUT0_VOUT_Msk))) {
		uint32_t target_value =
			(NRF_UICR->REGOUT0 & ~((uint32_t)UICR_REGOUT0_VOUT_Msk))
			| (UICR_REGOUT0_VOUT_TARGET << UICR_REGOUT0_VOUT_Pos);

		NRF_NVMC->CONFIG = NVMC_CONFIG_WEN_Wen << NVMC_CONFIG_WEN_Pos;
		while (NRF_NVMC->READY == NVMC_READY_READY_Busy){}

		NRF_UICR->REGOUT0 = target_value;

		NRF_NVMC->CONFIG = NVMC_CONFIG_WEN_Ren << NVMC_CONFIG_WEN_Pos;
		while (NRF_NVMC->READY == NVMC_READY_READY_Busy){}

		/* a reset is required for changes to take effect */
		NVIC_SystemReset();
	}

	return 0;
}

static int mtr4630a_init(const struct device *unused)
{
	return set_io_voltage();
}

SYS_INIT(mtr4630a_init, PRE_KERNEL_1, 30);
