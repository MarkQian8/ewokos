#include <stdint.h>
#include <stdlib.h>

#define readl(a) 	(*(volatile uint32_t *)(a))
#define writel(v,a) 	(*(volatile uint32_t *)(a) = (v))
#define rk_clrsetreg(addr, clr, set) writel(((clr) | (set)) << 16 | (set), addr)
#define rk_clrreg(addr, clr)        writel((clr) << 16, addr)
#define rk_setreg(addr, set)        writel((set) << 16 | (set), addr)
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#define DIV_ROUND_UP(n,d) (((n) + (d) - 1) / (d))

#define PX30_CPUCLK_RATE(_rate, _aclk_div, _pclk_div)       \
{                               \
    .rate   = _rate##U,                 \
    .aclk_div = _aclk_div,                  \
    .pclk_div = _pclk_div,                  \
}


#define PX30_PLL_RATE(_rate, _refdiv, _fbdiv, _postdiv1,    \
            _postdiv2, _dsmpd, _frac)       \
{                               \
    .rate   = _rate##U,                 \
    .fbdiv = _fbdiv,                    \
    .postdiv1 = _postdiv1,                  \
    .refdiv = _refdiv,                  \
    .postdiv2 = _postdiv2,                  \
    .dsmpd = _dsmpd,                    \
    .frac = _frac,                      \
}


#define MHz     1000000
#define KHz     1000
#define OSC_HZ      (24 * MHz)

#define APLL_HZ     (600 * MHz)
#define GPLL_HZ     (1200 * MHz)
#define NPLL_HZ     (1188 * MHz)
#define ACLK_BUS_HZ (200 * MHz)
#define HCLK_BUS_HZ (150 * MHz)
#define PCLK_BUS_HZ (100 * MHz)
#define ACLK_PERI_HZ    (200 * MHz)
#define HCLK_PERI_HZ    (150 * MHz)
#define PCLK_PMU_HZ (100 * MHz)

enum {
    VCO_MAX_HZ  = 3200U * 1000000,
    VCO_MIN_HZ  = 800 * 1000000,
    OUTPUT_MAX_HZ   = 3200U * 1000000,
    OUTPUT_MIN_HZ   = 24 * 1000000,
};

enum {
	/* PLLCON0*/
	PLL_BP_SHIFT		= 15,
	PLL_POSTDIV1_SHIFT	= 12,
	PLL_POSTDIV1_MASK	= 7 << PLL_POSTDIV1_SHIFT,
	PLL_FBDIV_SHIFT		= 0,
	PLL_FBDIV_MASK		= 0xfff,

	/* PLLCON1 */
	PLL_PDSEL_SHIFT		= 15,
	PLL_PD1_SHIFT		= 14,
	PLL_PD_SHIFT		= 13,
	PLL_PD_MASK		= 1 << PLL_PD_SHIFT,
	PLL_DSMPD_SHIFT		= 12,
	PLL_DSMPD_MASK		= 1 << PLL_DSMPD_SHIFT,
	PLL_LOCK_STATUS_SHIFT	= 10,
	PLL_LOCK_STATUS_MASK	= 1 << PLL_LOCK_STATUS_SHIFT,
	PLL_POSTDIV2_SHIFT	= 6,
	PLL_POSTDIV2_MASK	= 7 << PLL_POSTDIV2_SHIFT,
	PLL_REFDIV_SHIFT	= 0,
	PLL_REFDIV_MASK		= 0x3f,

	/* PLLCON2 */
	PLL_FOUT4PHASEPD_SHIFT	= 27,
	PLL_FOUTVCOPD_SHIFT	= 26,
	PLL_FOUTPOSTDIVPD_SHIFT	= 25,
	PLL_DACPD_SHIFT		= 24,
	PLL_FRAC_DIV	= 0xffffff,

	/* CRU_MODE */
	PLLMUX_FROM_XIN24M	= 0,
	PLLMUX_FROM_PLL,
	PLLMUX_FROM_RTC32K,
	USBPHY480M_MODE_SHIFT	= 8,
	USBPHY480M_MODE_MASK	= 3 << USBPHY480M_MODE_SHIFT,
	NPLL_MODE_SHIFT		= 6,
	NPLL_MODE_MASK		= 3 << NPLL_MODE_SHIFT,
	DPLL_MODE_SHIFT		= 4,
	DPLL_MODE_MASK		= 3 << DPLL_MODE_SHIFT,
	CPLL_MODE_SHIFT		= 2,
	CPLL_MODE_MASK		= 3 << CPLL_MODE_SHIFT,
	APLL_MODE_SHIFT		= 0,
	APLL_MODE_MASK		= 3 << APLL_MODE_SHIFT,

	/* CRU_CLK_SEL0_CON */
	CORE_ACLK_DIV_SHIFT	= 12,
	CORE_ACLK_DIV_MASK	= 0x07 << CORE_ACLK_DIV_SHIFT,
	CORE_DBG_DIV_SHIFT	= 8,
	CORE_DBG_DIV_MASK	= 0x03 << CORE_DBG_DIV_SHIFT,
	CORE_CLK_PLL_SEL_SHIFT	= 7,
	CORE_CLK_PLL_SEL_MASK	= 1 << CORE_CLK_PLL_SEL_SHIFT,
	CORE_CLK_PLL_SEL_APLL	= 0,
	CORE_CLK_PLL_SEL_GPLL,
	CORE_DIV_CON_SHIFT	= 0,
	CORE_DIV_CON_MASK	= 0x0f << CORE_DIV_CON_SHIFT,

	/* CRU_CLK_SEL3_CON */
	ACLK_VO_PLL_SHIFT	= 6,
	ACLK_VO_PLL_MASK	= 0x3 << ACLK_VO_PLL_SHIFT,
	ACLK_VO_SEL_GPLL	= 0,
	ACLK_VO_SEL_CPLL,
	ACLK_VO_SEL_NPLL,
	ACLK_VO_DIV_SHIFT	= 0,
	ACLK_VO_DIV_MASK	= 0x1f << ACLK_VO_DIV_SHIFT,

	/* CRU_CLK_SEL5_CON */
	DCLK_VOPB_SEL_SHIFT	= 14,
	DCLK_VOPB_SEL_MASK	= 0x3 << DCLK_VOPB_SEL_SHIFT,
	DCLK_VOPB_SEL_DIVOUT	= 0,
	DCLK_VOPB_SEL_FRACOUT,
	DCLK_VOPB_SEL_24M,
	DCLK_VOPB_PLL_SEL_SHIFT	= 11,
	DCLK_VOPB_PLL_SEL_MASK	= 0x1 << DCLK_VOPB_PLL_SEL_SHIFT,
	DCLK_VOPB_PLL_SEL_CPLL	= 0,
	DCLK_VOPB_PLL_SEL_NPLL,
	DCLK_VOPB_DIV_SHIFT	= 0,
	DCLK_VOPB_DIV_MASK	= 0xff,

	/* CRU_CLK_SEL8_CON */
	DCLK_VOPL_SEL_SHIFT	= 14,
	DCLK_VOPL_SEL_MASK	= 0x3 << DCLK_VOPL_SEL_SHIFT,
	DCLK_VOPL_SEL_DIVOUT	= 0,
	DCLK_VOPL_SEL_FRACOUT,
	DCLK_VOPL_SEL_24M,
	DCLK_VOPL_PLL_SEL_SHIFT	= 11,
	DCLK_VOPL_PLL_SEL_MASK	= 0x1 << DCLK_VOPL_PLL_SEL_SHIFT,
	DCLK_VOPL_PLL_SEL_NPLL	= 0,
	DCLK_VOPL_PLL_SEL_CPLL,
	DCLK_VOPL_DIV_SHIFT	= 0,
	DCLK_VOPL_DIV_MASK	= 0xff,

	/* CRU_CLK_SEL14_CON */
	PERI_PLL_SEL_SHIFT	= 15,
	PERI_PLL_SEL_MASK	= 3 << PERI_PLL_SEL_SHIFT,
	PERI_PLL_GPLL		= 0,
	PERI_PLL_CPLL,
	PERI_HCLK_DIV_SHIFT	= 8,
	PERI_HCLK_DIV_MASK	= 0x1f << PERI_HCLK_DIV_SHIFT,
	PERI_ACLK_DIV_SHIFT	= 0,
	PERI_ACLK_DIV_MASK	= 0x1f << PERI_ACLK_DIV_SHIFT,

	/* CRU_CLKSEL15_CON */
	NANDC_CLK_SEL_SHIFT	= 15,
	NANDC_CLK_SEL_MASK	= 0x1 << NANDC_CLK_SEL_SHIFT,
	NANDC_CLK_SEL_NANDC	= 0,
	NANDC_CLK_SEL_NANDC_DIV50,
	NANDC_DIV50_SHIFT	= 8,
	NANDC_DIV50_MASK	= 0x1f << NANDC_DIV50_SHIFT,
	NANDC_PLL_SHIFT		= 6,
	NANDC_PLL_MASK		= 0x3 << NANDC_PLL_SHIFT,
	NANDC_SEL_GPLL		= 0,
	NANDC_SEL_CPLL,
	NANDC_SEL_NPLL,
	NANDC_DIV_SHIFT		= 0,
	NANDC_DIV_MASK		= 0x1f << NANDC_DIV_SHIFT,

	/* CRU_CLKSEL20_CON */
	EMMC_PLL_SHIFT		= 14,
	EMMC_PLL_MASK		= 3 << EMMC_PLL_SHIFT,
	EMMC_SEL_GPLL		= 0,
	EMMC_SEL_CPLL,
	EMMC_SEL_NPLL,
	EMMC_SEL_24M,
	EMMC_DIV_SHIFT		= 0,
	EMMC_DIV_MASK		= 0xff << EMMC_DIV_SHIFT,

	/* CRU_CLKSEL21_CON */
	EMMC_CLK_SEL_SHIFT	= 15,
	EMMC_CLK_SEL_MASK	= 1 << EMMC_CLK_SEL_SHIFT,
	EMMC_CLK_SEL_EMMC	= 0,
	EMMC_CLK_SEL_EMMC_DIV50,
	EMMC_DIV50_SHIFT	= 0,
	EMMC_DIV50_MASK		= 0xff << EMMC_DIV_SHIFT,

	/* CRU_CLKSEL22_CON */
	GMAC_PLL_SEL_SHIFT	= 14,
	GMAC_PLL_SEL_MASK	= 3 << GMAC_PLL_SEL_SHIFT,
	GMAC_PLL_SEL_GPLL	= 0,
	GMAC_PLL_SEL_CPLL,
	GMAC_PLL_SEL_NPLL,
	CLK_GMAC_DIV_SHIFT	= 8,
	CLK_GMAC_DIV_MASK	= 0x1f << CLK_GMAC_DIV_SHIFT,
	SFC_PLL_SEL_SHIFT	= 7,
	SFC_PLL_SEL_MASK	= 1 << SFC_PLL_SEL_SHIFT,
	SFC_DIV_CON_SHIFT	= 0,
	SFC_DIV_CON_MASK	= 0x7f,

	/* CRU_CLK_SEL23_CON */
	BUS_PLL_SEL_SHIFT	= 15,
	BUS_PLL_SEL_MASK	= 1 << BUS_PLL_SEL_SHIFT,
	BUS_PLL_SEL_GPLL	= 0,
	BUS_PLL_SEL_CPLL,
	BUS_ACLK_DIV_SHIFT	= 8,
	BUS_ACLK_DIV_MASK	= 0x1f << BUS_ACLK_DIV_SHIFT,
	RMII_CLK_SEL_SHIFT	= 7,
	RMII_CLK_SEL_MASK	= 1 << RMII_CLK_SEL_SHIFT,
	RMII_CLK_SEL_10M	= 0,
	RMII_CLK_SEL_100M,
	RMII_EXTCLK_SEL_SHIFT	= 6,
	RMII_EXTCLK_SEL_MASK	= 1 << RMII_EXTCLK_SEL_SHIFT,
	RMII_EXTCLK_SEL_INT	= 0,
	RMII_EXTCLK_SEL_EXT,
	PCLK_GMAC_DIV_SHIFT	= 0,
	PCLK_GMAC_DIV_MASK	= 0x0f << PCLK_GMAC_DIV_SHIFT,

	/* CRU_CLK_SEL24_CON */
	BUS_PCLK_DIV_SHIFT	= 8,
	BUS_PCLK_DIV_MASK	= 3 << BUS_PCLK_DIV_SHIFT,
	BUS_HCLK_DIV_SHIFT	= 0,
	BUS_HCLK_DIV_MASK	= 0x1f << BUS_HCLK_DIV_SHIFT,

	/* CRU_CLK_SEL25_CON */
	CRYPTO_APK_SEL_SHIFT	= 14,
	CRYPTO_APK_PLL_SEL_MASK	= 3 << CRYPTO_APK_SEL_SHIFT,
	CRYPTO_PLL_SEL_GPLL	= 0,
	CRYPTO_PLL_SEL_CPLL,
	CRYPTO_PLL_SEL_NPLL	= 0,
	CRYPTO_APK_DIV_SHIFT	= 8,
	CRYPTO_APK_DIV_MASK	= 0x1f << CRYPTO_APK_DIV_SHIFT,
	CRYPTO_PLL_SEL_SHIFT	= 6,
	CRYPTO_PLL_SEL_MASK	= 3 << CRYPTO_PLL_SEL_SHIFT,
	CRYPTO_DIV_SHIFT	= 0,
	CRYPTO_DIV_MASK		= 0x1f << CRYPTO_DIV_SHIFT,

	/* CRU_CLK_SEL30_CON */
	CLK_I2S1_DIV_CON_MASK	= 0x7f,
	CLK_I2S1_PLL_SEL_MASK	= 0X1 << 8,
	CLK_I2S1_PLL_SEL_GPLL	= 0X0 << 8,
	CLK_I2S1_PLL_SEL_NPLL	= 0X1 << 8,
	CLK_I2S1_SEL_MASK	= 0x3 << 10,
	CLK_I2S1_SEL_I2S1	= 0x0 << 10,
	CLK_I2S1_SEL_FRAC	= 0x1 << 10,
	CLK_I2S1_SEL_MCLK_IN	= 0x2 << 10,
	CLK_I2S1_SEL_OSC	= 0x3 << 10,
	CLK_I2S1_OUT_SEL_MASK	= 0x1 << 15,
	CLK_I2S1_OUT_SEL_I2S1	= 0x0 << 15,
	CLK_I2S1_OUT_SEL_OSC	= 0x1 << 15,

	/* CRU_CLK_SEL31_CON */
	CLK_I2S1_FRAC_NUMERATOR_SHIFT	= 16,
	CLK_I2S1_FRAC_NUMERATOR_MASK	= 0xffff << 16,
	CLK_I2S1_FRAC_DENOMINATOR_SHIFT	= 0,
	CLK_I2S1_FRAC_DENOMINATOR_MASK	= 0xffff,

	/* CRU_CLK_SEL34_CON */
	UART1_PLL_SEL_SHIFT	= 14,
	UART1_PLL_SEL_MASK	= 3 << UART1_PLL_SEL_SHIFT,
	UART1_PLL_SEL_GPLL	= 0,
	UART1_PLL_SEL_24M,
	UART1_PLL_SEL_480M,
	UART1_PLL_SEL_NPLL,
	UART1_DIV_CON_SHIFT	= 0,
	UART1_DIV_CON_MASK	= 0x1f << UART1_DIV_CON_SHIFT,

	/* CRU_CLK_SEL35_CON */
	UART1_CLK_SEL_SHIFT	= 14,
	UART1_CLK_SEL_MASK	= 3 << UART1_PLL_SEL_SHIFT,
	UART1_CLK_SEL_UART1	= 0,
	UART1_CLK_SEL_UART1_NP5,
	UART1_CLK_SEL_UART1_FRAC,
	UART1_DIVNP5_SHIFT	= 0,
	UART1_DIVNP5_MASK	= 0x1f << UART1_DIVNP5_SHIFT,

	/* CRU_CLK_SEL37_CON */
	UART2_PLL_SEL_SHIFT	= 14,
	UART2_PLL_SEL_MASK	= 3 << UART2_PLL_SEL_SHIFT,
	UART2_PLL_SEL_GPLL	= 0,
	UART2_PLL_SEL_24M,
	UART2_PLL_SEL_480M,
	UART2_PLL_SEL_NPLL,
	UART2_DIV_CON_SHIFT	= 0,
	UART2_DIV_CON_MASK	= 0x1f << UART2_DIV_CON_SHIFT,

	/* CRU_CLK_SEL38_CON */
	UART2_CLK_SEL_SHIFT	= 14,
	UART2_CLK_SEL_MASK	= 3 << UART2_PLL_SEL_SHIFT,
	UART2_CLK_SEL_UART2	= 0,
	UART2_CLK_SEL_UART2_NP5,
	UART2_CLK_SEL_UART2_FRAC,
	UART2_DIVNP5_SHIFT	= 0,
	UART2_DIVNP5_MASK	= 0x1f << UART2_DIVNP5_SHIFT,

	/* CRU_CLK_SEL40_CON */
	UART3_PLL_SEL_SHIFT	= 14,
	UART3_PLL_SEL_MASK	= 3 << UART3_PLL_SEL_SHIFT,
	UART3_PLL_SEL_GPLL	= 0,
	UART3_PLL_SEL_24M,
	UART3_PLL_SEL_480M,
	UART3_PLL_SEL_NPLL,
	UART3_DIV_CON_SHIFT	= 0,
	UART3_DIV_CON_MASK	= 0x1f << UART3_DIV_CON_SHIFT,

	/* CRU_CLK_SEL41_CON */
	UART3_CLK_SEL_SHIFT	= 14,
	UART3_CLK_SEL_MASK	= 3 << UART3_PLL_SEL_SHIFT,
	UART3_CLK_SEL_UART3	= 0,
	UART3_CLK_SEL_UART3_NP5,
	UART3_CLK_SEL_UART3_FRAC,
	UART3_DIVNP5_SHIFT	= 0,
	UART3_DIVNP5_MASK	= 0x1f << UART3_DIVNP5_SHIFT,

	/* CRU_CLK_SEL46_CON */
	UART5_PLL_SEL_SHIFT	= 14,
	UART5_PLL_SEL_MASK	= 3 << UART5_PLL_SEL_SHIFT,
	UART5_PLL_SEL_GPLL	= 0,
	UART5_PLL_SEL_24M,
	UART5_PLL_SEL_480M,
	UART5_PLL_SEL_NPLL,
	UART5_DIV_CON_SHIFT	= 0,
	UART5_DIV_CON_MASK	= 0x1f << UART5_DIV_CON_SHIFT,

	/* CRU_CLK_SEL47_CON */
	UART5_CLK_SEL_SHIFT	= 14,
	UART5_CLK_SEL_MASK	= 3 << UART5_PLL_SEL_SHIFT,
	UART5_CLK_SEL_UART5	= 0,
	UART5_CLK_SEL_UART5_NP5,
	UART5_CLK_SEL_UART5_FRAC,
	UART5_DIVNP5_SHIFT	= 0,
	UART5_DIVNP5_MASK	= 0x1f << UART5_DIVNP5_SHIFT,

	/* CRU_CLK_SEL49_CON */
	CLK_I2C_PLL_SEL_GPLL		= 0,
	CLK_I2C_PLL_SEL_24M,
	CLK_I2C_DIV_CON_MASK		= 0x7f,
	CLK_I2C_PLL_SEL_MASK		= 1,
	CLK_I2C1_PLL_SEL_SHIFT		= 15,
	CLK_I2C1_DIV_CON_SHIFT		= 8,
	CLK_I2C0_PLL_SEL_SHIFT		= 7,
	CLK_I2C0_DIV_CON_SHIFT		= 0,

	/* CRU_CLK_SEL50_CON */
	CLK_I2C3_PLL_SEL_SHIFT		= 15,
	CLK_I2C3_DIV_CON_SHIFT		= 8,
	CLK_I2C2_PLL_SEL_SHIFT		= 7,
	CLK_I2C2_DIV_CON_SHIFT		= 0,

	/* CRU_CLK_SEL52_CON */
	CLK_PWM_PLL_SEL_GPLL		= 0,
	CLK_PWM_PLL_SEL_24M,
	CLK_PWM_DIV_CON_MASK		= 0x7f,
	CLK_PWM_PLL_SEL_MASK		= 1,
	CLK_PWM1_PLL_SEL_SHIFT		= 15,
	CLK_PWM1_DIV_CON_SHIFT		= 8,
	CLK_PWM0_PLL_SEL_SHIFT		= 7,
	CLK_PWM0_DIV_CON_SHIFT		= 0,

	/* CRU_CLK_SEL53_CON */
	CLK_SPI_PLL_SEL_GPLL		= 0,
	CLK_SPI_PLL_SEL_24M,
	CLK_SPI_DIV_CON_MASK		= 0x7f,
	CLK_SPI_PLL_SEL_MASK		= 1,
	CLK_SPI1_PLL_SEL_SHIFT		= 15,
	CLK_SPI1_DIV_CON_SHIFT		= 8,
	CLK_SPI0_PLL_SEL_SHIFT		= 7,
	CLK_SPI0_DIV_CON_SHIFT		= 0,

	/* CRU_CLK_SEL55_CON */
	CLK_SARADC_DIV_CON_SHIFT	= 0,
	CLK_SARADC_DIV_CON_MASK		= 0x7ff,

	/* CRU_CLK_GATE10_CON */
	CLK_I2S1_OUT_MCLK_PAD_MASK	= 0x1 << 9,
	CLK_I2S1_OUT_MCLK_PAD_ENABLE	= 0x1 << 9,
	CLK_I2S1_OUT_MCLK_PAD_DISABLE	= 0x0 << 9,

	/* CRU_PMU_MODE */
	GPLL_MODE_SHIFT			= 0,
	GPLL_MODE_MASK			= 3 << GPLL_MODE_SHIFT,

	/* CRU_PMU_CLK_SEL0_CON */
	CLK_PMU_PCLK_DIV_SHIFT		= 0,
	CLK_PMU_PCLK_DIV_MASK		= 0x1f << CLK_PMU_PCLK_DIV_SHIFT,
};

struct px30_pll {
	unsigned int con0;
	unsigned int con1;
	unsigned int con2;
	unsigned int con3;
	unsigned int con4;
	unsigned int reserved0[3];
};


struct px30_cru {
	struct px30_pll pll[4];
	unsigned int reserved1[8];
	unsigned int mode;
	unsigned int misc;
	unsigned int reserved2[2];
	unsigned int glb_cnt_th;
	unsigned int glb_rst_st;
	unsigned int glb_srst_fst;
	unsigned int glb_srst_snd;
	unsigned int glb_rst_con;
	unsigned int reserved3[7];
	unsigned int hwffc_con0;
	unsigned int reserved4;
	unsigned int hwffc_th;
	unsigned int hwffc_intst;
	unsigned int apll_con0_s;
	unsigned int apll_con1_s;
	unsigned int clksel_con0_s;
	unsigned int reserved5;
	unsigned int clksel_con[60];
	unsigned int reserved6[4];
	unsigned int clkgate_con[18];
	unsigned int reserved7[(0x280 - 0x244) / 4 - 1];
	unsigned int ssgtbl[32];
	unsigned int softrst_con[12];
	unsigned int reserved8[(0x380 - 0x32c) / 4 - 1];
	unsigned int sdmmc_con[2];
	unsigned int sdio_con[2];
	unsigned int emmc_con[2];
	unsigned int reserved9[(0x400 - 0x394) / 4 - 1];
	unsigned int autocs_con[8];
};

struct cpu_rate_table {
	unsigned long rate;
	unsigned int aclk_div;
	unsigned int pclk_div;
};

struct pll_rate_table {
    unsigned long rate;
    unsigned int fbdiv;
    unsigned int postdiv1;
    unsigned int refdiv;
    unsigned int postdiv2;
    unsigned int dsmpd;
    unsigned int frac;
};

enum px30_pll_id {
    APLL,
    DPLL,
    CPLL,
    NPLL,
    GPLL,
    PLL_COUNT,
};

static struct px30_cru *cru = MMIO_BASE + 0x2b0000;

static struct pll_rate_table px30_pll_rates[] = {
    /* _mhz, _refdiv, _fbdiv, _postdiv1, _postdiv2, _dsmpd, _frac */
    PX30_PLL_RATE(1200000000, 1, 50, 1, 1, 1, 0),
    PX30_PLL_RATE(1188000000, 2, 99, 1, 1, 1, 0),
    PX30_PLL_RATE(1100000000, 12, 550, 1, 1, 1, 0),
    PX30_PLL_RATE(1008000000, 1, 84, 2, 1, 1, 0),
    PX30_PLL_RATE(1000000000, 6, 500, 2, 1, 1, 0),
    PX30_PLL_RATE(816000000, 1, 68, 2, 1, 1, 0),
    PX30_PLL_RATE(600000000, 1, 75, 3, 1, 1, 0),
};

static struct cpu_rate_table px30_cpu_rates[] = {
    PX30_CPUCLK_RATE(1200000000, 1, 5),
    PX30_CPUCLK_RATE(1008000000, 1, 5),
    PX30_CPUCLK_RATE(816000000, 1, 3),
    PX30_CPUCLK_RATE(600000000, 1, 3),
    PX30_CPUCLK_RATE(408000000, 1, 1),
};

static uint32_t pll_mode_mask[PLL_COUNT] = {
    APLL_MODE_MASK, DPLL_MODE_MASK, CPLL_MODE_MASK,
    NPLL_MODE_MASK, GPLL_MODE_MASK
};

static uint8_t pll_mode_shift[PLL_COUNT] = {
    APLL_MODE_SHIFT, DPLL_MODE_SHIFT, CPLL_MODE_SHIFT,
    NPLL_MODE_SHIFT, GPLL_MODE_SHIFT
};

static struct pll_rate_table auto_table;

static struct pll_rate_table *pll_clk_set_by_auto(uint32_t drate)
{
    struct pll_rate_table *rate = &auto_table;
    uint32_t ref_khz = OSC_HZ / KHz, refdiv, fbdiv = 0;
    uint32_t postdiv1, postdiv2 = 1;
    uint32_t fref_khz;
    uint32_t diff_khz, best_diff_khz;
    const uint32_t max_refdiv = 63, max_fbdiv = 3200, min_fbdiv = 16;
    const uint32_t max_postdiv1 = 7, max_postdiv2 = 7;
    uint32_t vco_khz;
    uint32_t rate_khz = drate / KHz;

    if (!drate) {
        printf("%s: the frequency can't be 0 Hz\n", __func__);
        return NULL;
    }

    postdiv1 = DIV_ROUND_UP(VCO_MIN_HZ / 1000, rate_khz);
    if (postdiv1 > max_postdiv1) {
        postdiv2 = DIV_ROUND_UP(postdiv1, max_postdiv1);
        postdiv1 = DIV_ROUND_UP(postdiv1, postdiv2);
    }

    vco_khz = rate_khz * postdiv1 * postdiv2;

    if (vco_khz < (VCO_MIN_HZ / KHz) || vco_khz > (VCO_MAX_HZ / KHz) ||
        postdiv2 > max_postdiv2) {
        printf("%s: Cannot find out a supported VCO for Freq (%uHz)\n",
               __func__, rate_khz);
        return NULL;
    }

    rate->postdiv1 = postdiv1;
    rate->postdiv2 = postdiv2;

    best_diff_khz = vco_khz;
    for (refdiv = 1; refdiv < max_refdiv && best_diff_khz; refdiv++) {
        fref_khz = ref_khz / refdiv;

        fbdiv = vco_khz / fref_khz;
        if (fbdiv >= max_fbdiv || fbdiv <= min_fbdiv)
            continue;

        diff_khz = vco_khz - fbdiv * fref_khz;
        if (fbdiv + 1 < max_fbdiv && diff_khz > fref_khz / 2) {
            fbdiv++;
            diff_khz = fref_khz - diff_khz;
        }

        if (diff_khz >= best_diff_khz)
            continue;

        best_diff_khz = diff_khz;
        rate->refdiv = refdiv;
        rate->fbdiv = fbdiv;
    }

    if (best_diff_khz > 4 * (MHz / KHz)) {
        printf("%s: Failed to match output frequency %u bestis %u Hz\n",
               __func__, rate_khz,
               best_diff_khz * KHz);
        return NULL;
    }

    return rate;
}

static const struct pll_rate_table *get_pll_settings(unsigned long rate)
{
    unsigned int rate_count = ARRAY_SIZE(px30_pll_rates);
    int i;

    for (i = 0; i < rate_count; i++) {
        if (rate == px30_pll_rates[i].rate)
            return &px30_pll_rates[i];
    }

    return pll_clk_set_by_auto(rate);
}

static const struct cpu_rate_table *get_cpu_settings(unsigned long rate)
{
    unsigned int rate_count = ARRAY_SIZE(px30_cpu_rates);
    int i;

    for (i = 0; i < rate_count; i++) {
        if (rate == px30_cpu_rates[i].rate)
            return &px30_cpu_rates[i];
    }

    return NULL;
}

static int rkclk_set_pll(struct px30_pll *pll, unsigned int *mode,
             enum px30_pll_id pll_id,
             unsigned long drate)
{
    const struct pll_rate_table *rate;
    uint32_t vco_hz, output_hz;

    rate = get_pll_settings(drate);
    if (!rate) {
        printf("%s unsupport rate\n", __func__);
        return -1;
    }

    /* All PLLs have same VCO and output frequency range restrictions. */
    vco_hz = OSC_HZ / 1000 * rate->fbdiv / rate->refdiv * 1000;
    output_hz = vco_hz / rate->postdiv1 / rate->postdiv2;

    // printf("PLL at %p: fb=%d, ref=%d, pst1=%d, pst2=%d, vco=%u Hz, output=%u Hz\n",
    //       pll, rate->fbdiv, rate->refdiv, rate->postdiv1,
    //       rate->postdiv2, vco_hz, output_hz);

    /*
     * When power on or changing PLL setting,
     * we must force PLL into slow mode to ensure output stable clock.
     */
    rk_clrsetreg(mode, pll_mode_mask[pll_id],
             PLLMUX_FROM_XIN24M << pll_mode_shift[pll_id]);

    /* use integer mode */
    rk_setreg(&pll->con1, 1 << PLL_DSMPD_SHIFT);
    /* Power down */
    rk_setreg(&pll->con1, 1 << PLL_PD_SHIFT);

    rk_clrsetreg(&pll->con0,
             PLL_POSTDIV1_MASK | PLL_FBDIV_MASK,
             (rate->postdiv1 << PLL_POSTDIV1_SHIFT) | rate->fbdiv);
    rk_clrsetreg(&pll->con1, PLL_POSTDIV2_MASK | PLL_REFDIV_MASK,
             (rate->postdiv2 << PLL_POSTDIV2_SHIFT |
             rate->refdiv << PLL_REFDIV_SHIFT));

    /* Power Up */
    rk_clrreg(&pll->con1, 1 << PLL_PD_SHIFT);

    /* waiting for pll lock */
    while (!(readl(&pll->con1) & (1 << PLL_LOCK_STATUS_SHIFT)))
        _delay(1);

    rk_clrsetreg(mode, pll_mode_mask[pll_id],
             PLLMUX_FROM_PLL << pll_mode_shift[pll_id]);

    return 0;
}

void cpu_set_clk(int hz){
     const struct cpu_rate_table *rate = get_cpu_settings(hz);
     if (!rate) {
        printf("%s unsupport rate %d\n", __func__, hz);
        return;
     }

    rk_clrsetreg(&cru->clksel_con[0],
             CORE_CLK_PLL_SEL_MASK | CORE_DIV_CON_MASK |
             CORE_ACLK_DIV_MASK | CORE_DBG_DIV_MASK,
             rate->aclk_div << CORE_ACLK_DIV_SHIFT |
             rate->pclk_div << CORE_DBG_DIV_SHIFT |
             CORE_CLK_PLL_SEL_APLL << CORE_CLK_PLL_SEL_SHIFT |
             0 << CORE_DIV_CON_SHIFT);
    rkclk_set_pll(&cru->pll[APLL], &cru->mode, 0, hz);
}