// HDMI resolution flow

// Board Part: kernel/arch/arm/mach-tegra/board-endeavor-panel.c

statir void __init tegra_endeavor_init(void) // mach-tegra/board-endeavoru.c

	int __init endeavor_panel_init(void) // mach-tegra/board-endeavor-panel.c

		err = nvhost_device_register(&endeavor_disp2_device);

			ret = device_add(&dev->dev);

struct nvhost_device {
	const char			 *name;
	struct device		 dev;
	int					 id;
	u32					 num_resources;
	struct resource		 *resource;
	struct nvhost_master *host;
};

static struct nvhost_device endeavor_disp2_device = {
	.name			= "tegradc",
	.id				= 1,
	.resource		= endeavor_disp2_resources,
	.num_resources	= ARRAY_SIZE(endeavor_disp2_resources),
	.dev = {
		.platform_data = &endeavor_disp2_pdata,
	},
};

//HDMI
static struct tegra_dc_platform_data endeavor_disp2_pdata = {
	.flags        = 0,
	.default_out  = &endeavor_disp2_out, // <-- a
	.fb           = &endeavor_hdmi_fb_data, // <-- b
	.emc_clk_rate = 300000000,
};

//DSI
static struct tegra_dc_platform_data endeavor_disp1_pdata = {
	.flags        = TEGRA_DC_FLAG_ENABLED,
	.default_out  = &endeavor_disp1_out,
	.fb           = &endeavor_dsi_fb_data,
	.emc_clk_rate = 204000000,
};

// a
static struct tegra_dc_out endeavor_disp2_out = {
	.type         = TEGRA_DC_OUT_HDMI,
	.flags        = TEGRA_DC_OUT_HOTPLUG_HIGH,
	.parent_clk   = "pll_d2_out0",

	.dcc_bus      = 3,
	.hotplug_gpio = MHL_HPD,

	.max_pixclock = KHZ2PICOS(148500),

	.align        = TEGRA_DC_ALIGN_MSB,
	.order        = TEGRA_DC_ORDER_RED_BLUE,

	.enable       = endeavor_hdmi_enable,
	.disable      = endeavor_hdmi_disable,
	.postsuspend  = endeavor_hdmi_vddio_disable,
	.hotplug_init = endeavor_hdmi_vddio_enable,
};

// b
static struct tegra_fb_data endeavor_hdmi_fb_data = {
	.win            = 0,
	.xres           = 1366,
	.yres           = 768,
	.bits_per_pixel = 32,
	.flags          = TEGRA_FB_FLIP_ON_PROBE,
};

// Driver Part // drivers/video/tegra/dc/dc.c
struct nvhost_driver tegra_dc_driver = {
	.driver = {
		.name  = "tegradc",
		.owner = THIS_MODULE,
	},
	.probe       = tegra_dc_probe,
	.remove      = tegra_dc_remove,
	.suspend     = tegra_dc_suspend,
	.resume      = tegra_dc_resume,
};

static int __init tegra_dc_module_init(void)
{
	int ret = tegra_dc_ext_module_init();
	if (ret)
		return ret;
	return nvhost_driver_register(&tegra_dc_driver);
}

static void __exit tegra_dc_module_exit(void)
{
	nvhost_driver_unregister(&tegra_dc_driver);
	tegra_dc_ext_module_exit();
}

// Probe function // kernel/drivers/video/tegra/dc/dc.c
static int tegra_dc_probe(struct nvhost_device *ndev)

	dc->pdata = ndev->dev.platform_data; // pass board setting to driver

	tegra_dc_set_out(dc, dc->pdata->default_out)
				|
			dc->out = out
			dc->out_ops = &tegra_dc_hdmi_ops // assign corresponding output operations
			dc->out_ops->init(dc)
						   |
			.init = tegra_dc_hdmi_init,
							|
				INIT_DELAYED_WORK(&hdmi->work, tegra_dc_hdmi_detect_worker) // <-- a
				...

				hdmi->hpd_switch.name = "hdmi";
				ret = switch_dev_register(&hdmi->hpd_switch);

	dc->fb = tegra_fb_register(ndev, dc, dc->pdata->fb, fb_mem); // <-- b

// a
static void tegra_dc_hdmi_detect_worker(struct work_struct *work) // hdmi irq also queue this work
{
	struct tegra_dc_hdmi_data *hdmi =
		container_of(to_delayed_work(work), struct tegra_dc_hdmi_data, work);
	struct tegra_dc *dc = hdmi->dc;

	tegra_dc_enable(dc); // <-- a1
	msleep(5);
	if (!tegra_dc_hdmi_detect(dc)) {	// <-- a2
		tegra_dc_disable(dc);
		tegra_fb_update_monspecs(dc->fb, NULL, NULL);

		dc->connected = false;
		tegra_dc_ext_process_hotplug(dc->ndev->id); // <-- a3
	}
}

// a1: tegra_dc_enable
tegra_dc_enable,
	|
dc->enabled = _tegra_dc_enable(dc)
					|
return _tegra_dc_controller_enable(dc)
					|
	if (dc->out->enable)
		dc->out->enable(); // --> enterprise_hdmi_enable in board file

	if (dc->out_ops && dc->out_ops->enable)
		dc->out_ops->enable(dc); // --> tegra_dc_hdmi_enable


// a2: tegra_dc_hdmi_detect
tegra_dc_hdmi_detect
		|
	err = tegra_edid_get_monspecs(hdmi->edid, &specs);	// read EDID from TV
	err = tegra_edid_get_eld(hdmi->edid, &hdmi->eld);
	tegra_dc_hdmi_detect_config(dc, &specs);
				|
		tegra_fb_update_monspecs(dc->fb, specs, tegra_dc_hdmi_mode_filter);
					|
			fb_notifier_call_chain(FB_EVENT_NEW_MODELIST, &event); // <--

		#ifdef CONFIG_SWITCH
			hdmi->hpd_switch.state = 0;
			switch_set_state(&hdmi->hpd_switch, 1); // trigger gralloc: hdmi_uevent_polling
		#endif
		dev_info(&dc->ndev->dev, "display detected\n");

		dc->connected = true;
		tegra_dc_ext_process_hotplug(dc->ndev->id);


// a3: tegra_dc_ext_process_hotplug


// b: tegra_fb_register

dc->fb = tegra_fb_register(ndev, dc, dc->pdata->fb, fb_mem)
			|
	tegra_fb_set_par(info);
			|
		 fb_find_nearest_mode(&m, &info->modelist);

