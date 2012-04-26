int NvGrFbDevOpen (NvGrModule* mod, hw_device_t** out, int index)

	#ifdef HAVE_NATIVE_UEVENT
	        err = uevent_add_native_handler(handle_uevent, dev);
	        if (err < 0)
	            LOGE("Failed to register native uevent handler device: %s\n", strerror(errno));
	#else
	        hdmi_thread_off = 0;
	        err = hdmi_uevent_init();
	        if (err < 0)
	            LOGE("Failed to init hdmi uevent handler device: %s\n", strerror(errno));
	        else
	            pthread_create(&hdmi_hotplug_thread, NULL, (void *) &hdmi_hotplug, (void *) dev);
	#endif

// a: handle_uevent
static void handle_uevent(void *data, const char *msg, int msg_len)
{
    NvGrFbDev *dev = data;
    const char *s = msg;

    if (strcmp(s, "change@/devices/virtual/switch/hdmi"))
        return;

    s += strlen(s) + 1;

    while(*s) {
        if (!strncmp(s, "SWITCH_STATE=", strlen("SWITCH_STATE="))) {
            int state = atoi(s + strlen("SWITCH_STATE="));
            HDCP_TRACE("handle_uevent: state is %d", state);
            handle_hotplug(dev, state);
        }
        s += strlen(s) + 1;
    }
}

// b: handle_hotplug
static void handle_hotplug(NvGrFbDev *dev, int state)
{
    NvGrDisplay *dpy = &dev->display[NVGR_DISPLAY_HDMI];
    pthread_mutex_lock(&dev->hdmi_mutex);

    if (state) {
        struct tegra_fb_modedb fb_modedb;
        char value[PROPERTY_VALUE_MAX];
        int ii;
        int ret;

        dpy->mode = NULL;
        for (ii = 0; ii < DispMode_Count; ii++) {
            dev->hdmi_mode[ii] = NULL;
        }
        dev->hdmi_status.demo = 0;

        fb_modedb.modedb = dev->hdmi_modedb;
        fb_modedb.modedb_len = sizeof(dev->hdmi_modedb);

        if (ioctl(dpy->fd, FBIO_TEGRA_GET_MODEDB, &fb_modedb) < 0) // get what driver got from EDID
            goto done;

        property_get("nvidia.gralloc.demo", value, "0");
        if (!strcmp(value, "1"))
            dev->hdmi_status.demo = 1;

        dev->hdmi_modedb_len = fb_modedb.modedb_len;

        for (ii = 0; ii < dev->hdmi_modedb_len; ii++) {
            struct fb_var_screeninfo *mode = &dev->hdmi_modedb[ii];
            int refresh = fbmode_calc_refresh(mode);

            if (mode->vmode & FB_VMODE_STEREO_FRAME_PACK) {

            } else {
                if (!dev->hdmi_mode[DispMode_720p] &&
                    mode->xres == 1280 && mode->yres == 720 && refresh == 60)
                    dev->hdmi_mode[DispMode_720p] = mode;

                if (!dev->hdmi_mode[DispMode_1080p] &&
                    mode->xres == 1920 && mode->yres == 1080 && refresh == 30)
                    dev->hdmi_mode[DispMode_1080p] = mode;

                if (!dev->hdmi_mode[DispMode_1080p] &&
                    mode->xres == 1920 && mode->yres == 1080 && refresh == 60)
                    dev->hdmi_mode[DispMode_1080p] = mode;
            }
        }

        NvGrFbGetOverscan(dev);

        /* Force a modeset now, so HDMI will be enabled before HDCP begins polling */
#if USE_ALL_OVERLAYS
        ret = set_default_hdmi_mode(dev); // <-- call set_hdmi_mode to ioctl fb:w
#else
        ret = set_preferred_hdmi_mode(dev);
#endif
        if (ret < 0) {
            /* don't want anyone thinking HDMI is actually enabled */
            hdmi_disable(dev);
            goto done;
        }

        property_get("nvidia.gralloc.480p.demo", value, "0");
        if (!strcmp(value, "1")){
            dev->hdmi_mode[DispMode_720p] = NULL;
            dev->hdmi_mode[DispMode_1080p] = NULL;
        }
        hdmi_enable(dev);
    } else {
        hdmi_disable(dev);
#if !USE_ALL_OVERLAYS
        disable_display_transformation(&dev->hdmi_transform);
#endif
    }

    /* Request a new frame */
    if (dev->redraw_callback) {
        dev->redraw_callback(dev->redraw_data);
    }

done:
    pthread_mutex_unlock(&dev->hdmi_mutex);

#if !USE_ALL_OVERLAYS
    if (dpy->hotplug_callback) {
        dpy->hotplug_callback(dpy->hotplug_opaque, dev->hdmi_status.enabled ? dpy->mode : NULL);
    }
#endif
}

// framework/base/policy/src/com/android/internal/policy/impl/PhoneWindowManager.java
public class PhoneWindowManager implements WindowManagerPolicy {
	...

    void initializeHdmiState() {
        boolean plugged = false;
        // watch for HDMI plug messages if the hdmi switch exists
        if (new File("/sys/devices/virtual/switch/hdmi/state").exists()) {
            mHDMIObserver.startObserving("DEVPATH=/devices/virtual/switch/hdmi");


