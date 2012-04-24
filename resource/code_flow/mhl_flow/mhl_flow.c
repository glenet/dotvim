// -- source tree relationship -- //
	sii9234
TPI.c	sii_mhltx.c

// -- callback between mhl and cable_detection -- //

// kernel/drivers/misc/cable_detect.c 
static int __init cable_detect_init(void)
{
	the_cable_info.connect_type = CONNECT_TYPE_NONE;
	/*usb_register_notifier(&usb_status_notifier);*/
#if (defined(CONFIG_CABLE_DETECT_ACCESSORY) && defined(CONFIG_TEGRA_HDMI_MHL))
	mhl_detect_register_notifier(&mhl_status_notifier); // cable_detect plant a call back in mhl driver
#endif
	return platform_driver_register(&cable_detect_driver);
}

static struct t_mhl_status_notifier mhl_status_notifier = {
	.name = "mhl_detect",
	.func = mhl_status_notifier_func, // callback func implementation
};

struct t_mhl_status_notifier {
	struct list_head mhl_notifier_link; // for add to list
	const char *name;
	void (*func)(bool isMHL, int charging_type);
};

// kernel/drivers/video/tegra/sii9234/sii9234.c
int mhl_detect_register_notifier(struct t_mhl_status_notifier *notifier)
{
	if (!notifier || !notifier->name || !notifier->func)
		return -EINVAL;

	mutex_lock(&mhl_notify_sem);
	list_add(&notifier->mhl_notifier_link,
		&g_lh_mhl_detect_notifier_list); 
	mutex_unlock(&mhl_notify_sem);

	// kernel/arch/arm/mach-tegra/include/mach/cable_detect.h 
	//     static LIST_HEAD(g_lh_mhl_detect_notifier_list);

	return 0;
}


static void send_mhl_connect_notify(struct work_struct *w)
{
	static struct t_mhl_status_notifier *mhl_notifier;
	T_MHL_SII9234_INFO *pInfo = sii9234_info_ptr;

	if (!pInfo)
		return;

	PR_DISP_DEBUG("%s: %d\n", __func__, pInfo->isMHL);
	mutex_lock(&mhl_notify_sem);
	list_for_each_entry(mhl_notifier,	// callback cable_detect func
		&g_lh_mhl_detect_notifier_list,
		mhl_notifier_link) {
			if (mhl_notifier->func != NULL)
#ifdef CONFIG_INTERNAL_CHARGING_SUPPORT
				mhl_notifier->func(pInfo->isMHL, pInfo->statMHL);
#else
				mhl_notifier->func(pInfo->isMHL, false);
#endif
		}
	mutex_unlock(&mhl_notify_sem);
}


// mhl flow
kernel/drivers/misc/cable_detect.c

#ifdef CONFIG_TEGRA_HDMI_MHL
	case DOCK_STATE_MHL:
		CABLE_INFO("MHL inserted\n");
		switch_set_state(&dock_switch, DOCK_STATE_MHL);
		pInfo->accessory_type = DOCK_STATE_MHL;
		pInfo->usb_dpdn_switch(PATH_MHL);
#ifdef MHL_INTERNAL_POWER
		if (!pInfo->mhl_internal_3v3 && !g_vbus)
			send_cable_connect_notify(CONNECT_TYPE_INTERNAL);
#endif
		sii9234_mhl_device_wakeup();
		break;
#endif


#ifdef CONFIG_TEGRA_HDMI_MHL
		case DOCK_STATE_MHL:
			CABLE_INFO("MHL removed\n");
			switch_set_state(&dock_switch, DOCK_STATE_UNDOCKED);
			sii9234_disableIRQ();
			break;
#endif
			

static void sii9234_irq_do_work(struct work_struct *work)
{
	T_MHL_SII9234_INFO *pInfo = sii9234_info_ptr;

	mutex_lock(&mhl_early_suspend_sem);
	if (!g_bEnterEarlySuspend) {
		uint8_t		event;
		uint8_t		eventParameter;

		PR_DISP_DEBUG("MHL ISR\n");
		SiiMhlTxGetEvents(&event, &eventParameter); // <--
#ifdef CONFIG_TEGRA_HDMI_MHL_SUPERDEMO
		Mhl_Proc_Remote_Event(pInfo);
#endif
		ProcessRcp(event, eventParameter);
	}
	mutex_unlock(&mhl_early_suspend_sem);

	enable_irq(pInfo->irq);
}


void SiiMhlTxGetEvents(uint8_t *event, uint8_t *eventParameter)
{
	TPI_Poll(); // <--

	if (mhlTxConfig.mhlConnectionEvent) {


	} else if (mhlTxConfig.mscMsgArrived) {

	}
}

void TPI_Poll(void)
{
	if (POWER_STATE_D0_MHL != fwPowerState) {

		Int4Isr(); // check register then process connection

	} else if (POWER_STATE_D0_MHL == fwPowerState) {

		if(deglitchingRsenNow) // plug out
			...

		MhlCbusIsr();
	}

}


