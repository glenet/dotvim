1. 跟kernel說我們有一個device叫什麼名字
2. 跟kernel說我們要使用第幾條i2c，名字叫什麼，i2c的位置在那裡
3. 在自己寫的driver上，註冊自己的platform driver，記住driver name跟device name一定要一至
4. 在自己的driver上，註冊i2c的driver，記得名稱也一定要跟i2c device一樣
5. 快樂的寫程式吧

// board file

static void __init tegra_endeavor_init(void)
{

#ifdef CONFIG_TEGRA_HDMI_MHL
	i2c_register_board_info(4, i2c_mhl_sii_info,
			ARRAY_SIZE(i2c_mhl_sii_info));
#endif

}


static struct i2c_board_info i2c_mhl_sii_info[] =
{
	{
		I2C_BOARD_INFO(MHL_SII9234_I2C_NAME, 0x72 >> 1),
		.platform_data = &mhl_sii_device_data,
		.irq = TEGRA_GPIO_TO_IRQ(EDGE_GPIO_MHL_INT)
	}
};


#define I2C_BOARD_INFO(dev_type, dev_addr) \
	.type = dev_type, .addr = (dev_addr)

struct i2c_board_info {
	char				type[I2C_NAME_SIZE];
	unsigned short		flags;
	unsigned short		addr;
	void				*platform_data;
	struct dev_archdata	*archdata;
	struct device_node 	*of_node;
	int					irq;
};

static T_MHL_PLATFORM_DATA mhl_sii_device_data = {
	.gpio_intr        = EDGE_GPIO_MHL_INT,
	.gpio_usb_sel     = EDGE_GPIO_MHL_USB_SEL,
	.gpio_reset       = EDGE_GPIO_MHL_RESET,
	.gpio_ddc_clk     = EDGE_GPIO_MHL_DDC_CLK,
	.gpio_ddc_data    = EDGE_GPIO_MHL_DDC_DATA,
	.ci2ca            = 0,
	.power            = mhl_sii_power,
	.enMhlD3Guard     = false,
}

// driver file

static const struct i2c_device_id sii9234_i2c_id[] = {
	{MHL_SII9234_I2C_NAME, 0},
	{}
};

static struct i2c_driver sii9234_driver = {
	.id_table = sii9234_i2c_id,
	.probe = sii9234_probe,
	.remove = sii9234_remove,
#ifndef CONFIG_HAS_EARLYSUSPEND
	.suspend = sii9234_suspend,
#endif
	.resume = sii9234_resume,
	.driver = {
		.name = MHL_SII9234_I2C_NAME, // #define MHL_SII9234_I2C_NAME   "sii9234"
		.owner = THIS_MODULE,
	},
};

static int __init sii9234_init(void)
{
	return i2c_add_driver(&sii9234_driver);
}

static void __exit sii9234_exit(void)
{
	i2c_del_driver(&sii9234_driver);
}
