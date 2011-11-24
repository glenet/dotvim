// HDMI resolution flow

// Board part
a. declare resource and platform device

// Driver part
b. create init func to call platform_add_devices
c. call sub_initcall to init devices
d. declare platform driver
e. call platform_driver_register with related device name
f. call platform_driver probe func after register success

// Board Part
static struct tegra_dc_out enterprise_disp2_out = {

	.enable     = enterprise_hdmi_enable

static struct tegra_dc_platform_data enterprise_disp2_pdata = {

	.default_out    = &enterprise_disp2_out

static struct nvhost_device enterprise_disp2_device = {

	.dev = {
         .platform_data = &enterprise_disp2_pdata,
     },

nvhost_device_register(&enterprise_disp2_device);
			|
	ret = device_add(&dev->dev); 
			|
	kobject_uevent(&dev->kobj, KOBJ_ADD);


// Driver Part
module_init(tegra_dc_module_init) (dc.c)
					|
	nvhost_driver_register(&tegra_dc_driver), 							tegra_dc_ext_module_init();
								|
		//.probe = tegra_dc_probe(struct nvhost_device *ndev)
					|
		// the place board setting pass to driver
		dc->pdata = ndev->dev.platform_data; (dc.c:2629) 	
														
		//.probe = tegra_dc_probe
					|
		tegra_dc_set_out(dc, dc->pdata->default_out) (dc.c:2679)
				|
			dc->out = out (dc.c:1759)
			dc->out_ops = &tegra_dc_hdmi_ops (dc.c:1770)	
			dc->out_ops->init(dc) (dc.c:1783)
						   |										   
			.init = tegra_dc_hdmi_init,
						|
			//INIT_DELAYED_WORK(&hdmi->work, tegra_dc_hdmi_detect_worker)
												|
					tegra_dc_enable,			tegra_dc_hdmi_detect
							|						|
		dc->enabled = _tegra_dc_enable(dc)		tegra_edid_get_monspecs(hdmi->edid, &specs)						
							|					tegra_dc_hdmi_detect_config(dc, &specs)
	return _tegra_dc_controller_enable(dc)				|
						|						tegra_fb_update_monspecs(dc->fb, specs, tegra_dc_hdmi_mode_filter);
				dc->out->enable();								|
	(就是 board part 的 enterprise_hdmi_enable)		  這個地方會神奇的 call 到 tegra_fb_set_par() ???
						...
		if (dc->out_ops && dc->out_ops->enable)
			dc->out_ops->enable(dc);
			(就是tegra_dc_hdmi_enable)

			//INIT_DELAYED_WORK(&hdmi->work, tegra_dc_hdmi_detect_worker)
													|
		tegra_dc_ext_process_hotplug(dc->ndev->id); (board-edge-panel.c:564 nvhost_device.id = 1)
					|
			tegra_dc_ext_queue_hotplug
						|
				pack.event.type = TEGRA_DC_EXT_EVENT_HOTPLUG (vendor/nvidia/tegra/core/drivers/libnvdc/events.c:65)
				...
				tegra_dc_ext_queue_event(control, &pack.event);
							|
					
					
					


		//.probe = tegra_dc_probe
					|
		dc->fb = tegra_fb_register(ndev, dc, dc->pdata->fb, fb_mem) (dc.c:2713, dc from parameter of tegra_fb_register: ndev)
					|
				tegra_fb->xres = fb_data->xres; (fb.c:386),		tegra_fb_set_par(info); (fb.c:453)
									||										|
								dc->pdata->fb				fb_find_nearest_mode(&m, &info->modelist); 
															// info 是從tegra_fb_register裡的dc->pdata->fb 來的

./drivers/video/tegra/fb.c:306
static struct fb_ops tegra_fb_ops = {
	...
	.fb_set_par = tegra_fb_set_par,
     
struct tegra_fb_info *tegra_fb_register(struct nvhost_device *ndev,
	...
	info->fbops = &tegra_fb_ops;
	...
	tegra_fb->info = info;
	...
	return tegra_fb;	// tegra_fb_register return 'tegra_fb_info'

// HDMI irq
tegra_dc_hdmi_irq
	...
	if (tegra_dc_hdmi_hpd(dc))
		queue_delayed_work(system_nrt_wq, &hdmi->work, msecs_to_jiffies(100));
												|
											在 tegra_dc_probe 裡面被 init


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
