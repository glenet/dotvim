// HDCP logic

// NV Original
1. Identify key
2. check content
	if protect
		if have key
			play
		else
			not play

	if non-protect
		play

// HTC (hdmid: for AP knowing the state of HDMI)
Now, chiajung hardcode this behavior at gralloc.c -> dpy->Config.protect = 1;

if have key
	play
else
	non-play

// HDCP flow

tegra_dc_probe(struct nvhost_device *ndev)

dc->pdata = ndev->dev.platform_data;
tegra_dc_set_out(dc, dc->pdata->default_out);

tegra_dc_set_out(struct tegra_dc *dc, struct tegra_dc_out *out) // dc.c, run twice: dsi, hdmi

	dc->out = out;

	dc->out_ops = &tegra_dc_hdmi_ops;

	if (dc->out_ops && dc->out_ops->init)
		dc->out_ops->init(dc);

		tegra_dc_hdmi_init(struct tegra_dc *dc) // hdmi.c

			hdmi->nvhdcp = tegra_nvhdcp_create(hdmi, dc->ndev->id, dc->out->dcc_bus);


tegra_dc_hdmi_irq(int irq, void *ptr)

	if (tegra_dc_hdmi_hpd(dc))
		queue_delayed_work(system_nrt_wq, &hdmi->work, msecs_to_jiffies(100));

			tegra_dc_hdmi_detect_worker(struct work_struct *work)

				tegra_dc_enable(dc);

					if (!dc->enabled)
						dc->enabled = _tegra_dc_enable(dc);

							return _tegra_dc_controller_enable(dc);

								if (dc->out->enable)
									dc->out->enable();

								if (dc->out_ops && dc->out_ops->enable)
									dc->out_ops->enable(dc);


								tegra_dc_hdmi_enable(struct tegra_dc *dc)

									tegra_nvhdcp_set_plug(struct tegra_nvhdcp *nvhdcp, bool hpd)

										tegra_nvhdcp_on(struct tegra_nvhdcp *nvhdcp)

											nvhdcp_downstream_worker(struct work_struct *work)






//board-endeavor-panel.c
static struct nvhost_device endeavor_disp2_device = {
	.name		= "tegradc",
	.id		= 1,
	.resource	= endeavor_disp2_resources,
	.num_resources	= ARRAY_SIZE(endeavor_disp2_resources),
	.dev = {
		.platform_data = &endeavor_disp2_pdata,
	},
};


struct tegra_dc_out_ops tegra_dc_hdmi_ops = {
	.init = tegra_dc_hdmi_init,
	.destroy = tegra_dc_hdmi_destroy,
	.enable = tegra_dc_hdmi_enable,
	.disable = tegra_dc_hdmi_disable,
	.detect = tegra_dc_hdmi_detect,
	.suspend = tegra_dc_hdmi_suspend,
	.resume = tegra_dc_hdmi_resume,
};
























