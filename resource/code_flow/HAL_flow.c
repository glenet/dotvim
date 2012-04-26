
// hardware/libhardware/include/hardware/hardware.h
/* 
* Every hardware module must have a data structure named HAL_MODULE_INFO_SYM
* and the fields of this data structure must begin with hw_module_t
* followed by module specific information.
*/
// 實做 HAL stub 的首要工作是繼承 struct hw_module_t 抽象型別
typedef struct hw_module_t {

	...
} hw_module_t;

// Android HAL 規定不要直接使用 struct hw_module_t，要用類別繼承
// C 當中實做繼承的方式 1.宣告一個 data structure 將原始基本結構包裝起來 2. 將原始的基本結構放在第一個 field, 並取名 common
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

// vendor/nvidia/tegra/hal/libgralloc/nvgrmodule.c 
static hw_module_methods_t NvGrMethods =
{
	.open = NvGrDevOpen <-- 見 line 77
};

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

		...
	}
	
	...
};

int NvGrFbDevOpen (NvGrModule* mod, hw_device_t** out, int index) 
{
	...
	dev->Base.setRotation         = NvGrSetRotation; <-- 誰call了它, line 87
	...
	
	err = uevent_add_native_handler(handle_uevent, dev);
	
	...
}

static int NvGrDevOpen(const hw_module_t* module, const char* name, hw_device_t** device) {

	...

	case NvGrDevIdx_Fb0:
		ret = NvGrFbDevOpen(m, &m->Dev[idx], 0); <-- 見 line 66

}

./frameworks/base/libs/ui/FramebufferNativeWindow.cpp:83:    if (hw_get_module(GRALLOC_HARDWARE_MODULE_ID, &module) == 0) {
./frameworks/base/services/surfaceflinger/DisplayHardware/DisplayHardware.cpp:409:        fbDev->setRotation(fbDev, rotation);


SurfaceFlinger::readyToRun()

int nvdcEventData(struct nvdcState *state, int fd) //tegra/core/drivers/libnvdc/events.c
			|
	case TEGRA_DC_EXT_EVENT_HOTPLUG:
		return handleHotplug(state, event->data);
						|
			state->hotplug(&state->displays[hpData->handle]);
				|

