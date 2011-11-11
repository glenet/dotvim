./drivers/video/tegra/fb.c:306:	.fb_set_par = tegra_fb_set_par

static struct fb_ops tegra_fb_ops = {
     .owner = THIS_MODULE,
     .fb_check_var = tegra_fb_check_var,
     .fb_set_par = tegra_fb_set_par,
     .fb_setcolreg = tegra_fb_setcolreg,
     .fb_blank = tegra_fb_blank,
     .fb_pan_display = tegra_fb_pan_display,
     .fb_fillrect = tegra_fb_fillrect,
     .fb_copyarea = tegra_fb_copyarea,
     .fb_imageblit = tegra_fb_imageblit,
     .fb_ioctl = tegra_fb_ioctl,
};

struct tegra_fb_info *tegra_fb_register(struct nvhost_device *ndev,
	
	info->fbops = &tegra_fb_ops;

	tegra_fb->info = info;

	return tegra_fb;	// tegra_fb_register return 'tegra_fb_info'


void tegra_fb_update_monspecs(struct tegra_fb_info *fb_info,
                   struct fb_monspecs *specs,
                   bool (*mode_filter)(const struct tegra_dc *dc,
                           struct fb_videomode *mode))

bool tegra_dc_hdmi_detect_config(struct tegra_dc *dc,
                         struct fb_monspecs *specs)
{    
   
	tegra_fb_update_monspecs(dc->fb, specs, tegra_dc_hdmi_mode_filter);


tegra_dc_hdmi_detect
|
tegra_fb_update_monspecs

