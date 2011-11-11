// HDMI resolution flow

// Board part
a. declare resource and platform device
b. create init func to call platform_add_devices
c. call sub_initcall to init devices
// Driver part
d. declare platform driver
e. call platform_driver_register with related device name
f. call platform_driver probe func after register success

module_init(tegra_dc_module_init) (dc.c)
					|
	nvhost_driver_register(&tegra_dc_driver)
								|
		//.probe = tegra_dc_probe(struct nvhost_device *ndev)
					|
	dc->pdata = ndev->dev.platform_data; (dc.c:2629) 	// the place board setting
														// pass to driver


		//.probe = tegra_dc_probe
					|
	tegra_dc_set_out(dc, dc->pdata->default_out) (dc.c:2679)
				|
		dc->out_ops = &tegra_dc_hdmi_ops (dc.c:1770)	
		dc->out_ops->init(dc) (dc.c:1783)
					   |										   |
		.init = tegra_dc_hdmi_init,
						|
		INIT_DELAYED_WORK(&hdmi->work, tegra_dc_hdmi_detect_worker)
												|
							.detect = tegra_dc_hdmi_detect				
											|
						tegra_edid_get_monspecs(hdmi->edid, &specs), tegra_dc_hdmi_detect_config(dc, &specs)
																				|
															tegra_fb_update_monspecs(dc->fb, specs, tegra_dc_hdmi_mode_filter);
																			|
															  這個地方會神奇的 call 到 tegra_fb_set_par() ???


		//.probe = tegra_dc_probe
					|
		dc->fb = tegra_fb_register(ndev, dc, dc->pdata->fb, fb_mem) (dc.c:2713, dc from parameter of tegra_fb_register: ndev)
					|
				tegra_fb->xres = fb_data->xres; (fb.c:386)		tegra_fb_set_par(info); (fb.c:453)
									||										|
								dc->pdata->fb				fb_find_nearest_mode(&m, &info->modelist); 
															// info 是從tegra_fb_register裡的dc->pdata->fb 來的


// Prototype
static int __init tegra_dc_module_init(void);
void nvhost_driver_unregister(struct nvhost_driver *drv);

struct nvhost_driver tegra_dc_driver = {
	.driver = {
		.name = "tegradc",
		.owner = THIS_MODULE,
	},  
	.probe = tegra_dc_probe,
	.remove = tegra_dc_remove,
	.suspend = tegra_dc_suspend,
	.resume = tegra_dc_resume,
};

tegra_dc_probe(struct nvhost_device *ndev);

static void tegra_dc_set_out(struct tegra_dc *dc, struct tegra_dc_out *out);

struct tegra_dc_out_ops tegra_dc_hdmi_ops = {
	.init = tegra_dc_hdmi_init,
	.destroy = tegra_dc_hdmi_destroy,
	.enable = tegra_dc_hdmi_enable,
	.disable = tegra_dc_hdmi_disable,
	.detect = tegra_dc_hdmi_detect,
	.suspend = tegra_dc_hdmi_suspend,
	.resume = tegra_dc_hdmi_resume,
};  

static bool tegra_dc_hdmi_detect(struct tegra_dc *dc);

static int tegra_fb_set_par(struct fb_info *info);

struct tegra_fb_info *tegra_fb_register(struct nvhost_device *ndev,
					struct tegra_dc *dc,
					struct tegra_fb_data *fb_data,
					struct resource *fb_mem);
