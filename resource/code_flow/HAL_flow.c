
// hardware/libhardware/include/hardware/hardware.h
 
// Every hardware module must have a data structure named HAL_MODULE_INFO_SYM
// and the fields of this data structure must begin with hw_module_t
// followed by module specific information.

// 實做 HAL stub 的首要工作是繼承 struct hw_module_t 抽象型別
typedef struct hw_module_t {

	...
} hw_module_t;

// Android HAL 規定不要直接使用 struct hw_module_t，要用類別繼承
// C 當中實做繼承的方式
// 1.宣告一個 data structure 將原始基本結構包裝起來
// 2. 將原始的基本結構放在第一個 field, 並取名 common

// hardware/libhardware/include/hardware/gralloc.h
typedef struct gralloc_module_t {
	struct hw_module_t common;

	...
} gralloc_module_t;

// vendor/nvidia/tegra/hal/libgralloc/nvgr.h
typedef struct NvGrModuleRec {
	gralloc_module_t    Base;

	...
} NvGrModule;

// vendor/nvidia/tegra/graphics-partner/android/libgralloc/nvgrmodule.c
NvGrModule HAL_MODULE_INFO_SYM =
{
	.Base =
	{
		.common = // 繼承的 hw_module_t
		{
			.tag = HARDWARE_MODULE_TAG,
			.version_major = 1,
			.version_minor = 0,
			.id = GRALLOC_HARDWARE_MODULE_ID,
			.name = "NVIDIA Tegra Graphics Memory Allocator",
			.author = "NVIDIA",
			.methods = &NvGrMethods	<- 見 line 40
		},
	}
	...
};

static hw_module_methods_t NvGrMethods =
{
	.open = NvGrDevOpen
};


static int NvGrDevOpen(const hw_module_t* module, const char* name, hw_device_t** device) {
	... 

    if (NvOsStrcmp(name, GRALLOC_HARDWARE_GPU0) == 0)
        idx = NvGrDevIdx_Alloc;
    else if (NvOsStrcmp(name, GRALLOC_HARDWARE_FB0) == 0)
        idx = NvGrDevIdx_Fb0;
    else
        return -EINVAL;

	... 
        switch (idx) {
        case NvGrDevIdx_Alloc:
            ret = NvGrAllocDevOpen(m, &m->Dev[idx]);
            break;
        case NvGrDevIdx_Fb0:
            ret = NvGrFbDevOpen(m, &m->Dev[idx], 0); // <--
            break;
        default:
            ret = -EINVAL;
            break;
        }
}

//frameworks/base/libs/ui/FramebufferNativeWindow.cpp
FramebufferNativeWindow::FramebufferNativeWindow() 
    : BASE(), fbDev(0), grDev(0), mUpdateOnDemand(false)
{
    hw_module_t const* module;
    if (hw_get_module(GRALLOC_HARDWARE_MODULE_ID, &module) == 0) { // <-- get GRALLOC_HARDWARE_MODULE_ID module
        int stride;
        int err;
        int i;
        err = framebuffer_open(module, &fbDev); // a
        LOGE_IF(err, "couldn't open framebuffer HAL (%s)", strerror(-err));
        
        err = gralloc_open(module, &grDev);
        LOGE_IF(err, "couldn't open gralloc HAL (%s)", strerror(-err)); // b

// a: framebuffer_open // hardware/libhardware/include/hardware/fb.h
static inline int framebuffer_open(const struct hw_module_t* module,
        struct framebuffer_device_t** device) {
    return module->methods->open(module,
            GRALLOC_HARDWARE_FB0, (struct hw_device_t**)device);
}

// b: gralloc_open //hardware/libhardware/include/hardware/gralloc.h
static inline int gralloc_open(const struct hw_module_t* module, 
        struct alloc_device_t** device) {
    return module->methods->open(module, 
            GRALLOC_HARDWARE_GPU0, (struct hw_device_t**)device);
}

// frameworks/base/services/surfaceflinger/DisplayHardware/DisplayHardware.cpp
void DisplayHardware::init(uint32_t dpy)
{
    mNativeWindow = new FramebufferNativeWindow(); // <--
    framebuffer_device_t const * fbDev = mNativeWindow->getDevice();


DisplayHardware::DisplayHardware(
        const sp<SurfaceFlinger>& flinger,
        uint32_t dpy)
    : DisplayHardwareBase(flinger, dpy),
      mFlinger(flinger), mFlags(0), mHwc(0)
{
    init(dpy); // <--
}

// frameworks/base/services/surfaceflinger/SurfaceFlinger.cpp
status_t SurfaceFlinger::readyToRun()
{
    LOGI(   "SurfaceFlinger's main thread ready to run. "
            "Initializing graphics H/W...");

    // we only support one display currently
    int dpy = 0;

    {
        // initialize the main display
        GraphicPlane& plane(graphicPlane(dpy));
        DisplayHardware* const hw = new DisplayHardware(this, dpy); // <--
        plane.setDisplayHardware(hw);
    }



// vendor/nvidia/tegra/graphics-partner/android/libgralloc/nvgrfb_linux.c
int NvGrFbDevOpen (NvGrModule* mod, hw_device_t** out, int index) 
{
	...
	dev->Base.setRotation = NvGrSetRotation;
	...

    /* Setup main display */
    if (NvGrFbOpen(dev, NVGR_DISPLAY_LCD, index) != 0) {
        goto fail;
    }

    /* Setup HDMI */
    if (NvGrFbOpen(dev, NVGR_DISPLAY_HDMI, index) == 0) {
        int err;

}

// frameworks/base/services/surfaceflinger/DisplayHardware/DisplayHardware.cpp
void DisplayHardware::setOrientation(int orientation) const
{
    framebuffer_device_t *fbDev = (framebuffer_device_t *)mNativeWindow->getDevice();
    int rotation = 0;

    mNativeWindow->setOrientation(orientation);
    if (fbDev && fbDev->setRotation) {
        switch (orientation) {
            case ISurfaceComposer::eOrientationDefault: rotation = 0; break;
            case ISurfaceComposer::eOrientation90: rotation = 90; break;
            case ISurfaceComposer::eOrientation180: rotation = 180; break;
            case ISurfaceComposer::eOrientation270: rotation = 270; break;
            default: return;
        }
        fbDev->setRotation(fbDev, rotation); // <-- dev->Base.setRotation
    }
}

// frameworks/base/services/surfaceflinger/SurfaceFlinger.cpp
bool SurfaceFlinger::threadLoop()
{
	...
    const DisplayHardware& hw(graphicPlane(0).displayHardware());
    if (LIKELY(hw.canDraw())) {
        // repaint the framebuffer (if needed)

        const int index = hw.getCurrentBufferIndex();
        GraphicLog& logger(GraphicLog::getInstance());
        if (mOrientationChanged) {
            mOrientationChanged = false;
            hw.setOrientation(mCurrentState.orientation); // <-- 




