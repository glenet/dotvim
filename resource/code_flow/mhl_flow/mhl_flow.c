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

static void mhl_status_notifier_func(bool isMHL, int charging_type)
{
	struct cable_detect_info *pInfo = &the_cable_info;
	int id_pin = gpio_get_value(pInfo->usb_id_pin_gpio);
	static uint8_t mhl_connected;

	CABLE_INFO("%s: isMHL %d, charging type %d, id_pin %d\n", __func__, isMHL, charging_type, id_pin);
	if (pInfo->accessory_type != DOCK_STATE_MHL) {
		CABLE_INFO("%s: accessory is not MHL, type %d\n", __func__, pInfo->accessory_type);
		return;
	}

	if (!isMHL) {
		CABLE_INFO("MHL removed\n");

		if (pInfo->usb_dpdn_switch)
			pInfo->usb_dpdn_switch(PATH_USB);

		if (pInfo->mhl_1v2_power)
			pInfo->mhl_1v2_power(0);
#ifdef MHL_INTERNAL_POWER
		send_cable_connect_notify(CONNECT_TYPE_CLEAR);
#endif
#ifdef MHL_REDETECT
		if (mhl_connected == 0) {
			CABLE_INFO("MHL re-detect\n");
			irq_set_irq_type(pInfo->idpin_irq, id_pin ? IRQF_TRIGGER_LOW : IRQF_TRIGGER_HIGH);
			pInfo->cable_redetect = 1;
		}
#endif
		mhl_connected = 0;

		pInfo->accessory_type = DOCK_STATE_UNDOCKED;
		sii9234_disableIRQ();
		enable_irq(pInfo->idpin_irq);
		return;
	}
	else {
		mhl_connected = 1;
		irq_set_irq_type(pInfo->idpin_irq, id_pin ? IRQF_TRIGGER_LOW : IRQF_TRIGGER_HIGH);
	}
}

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

		Int4Isr(); // a: check register then process connection

	} else if (POWER_STATE_D0_MHL == fwPowerState) {

		if(deglitchingRsenNow) // plug out
			...

		MhlCbusIsr(); // b
	}

}

// TPI_Poll: a
static	int	Int4Isr(void)
{
	if (reg74 & BIT_2) {

		MhlTxDrvProcessConnection(); // a

	} else if (reg74 & BIT_3) {

		MhlTxDrvProcessDisconnection(); // <--
	}

	if ((POWER_STATE_D3 == fwPowerState) && (reg74 & BIT_6)) {
		SwitchToD0();
		ProcessRgnd(); // <--
	}

	if (reg74 & BIT_4) {

		TPI_DEBUG_PRINT(("Drv: CBus Lockout\n"));
		ForceUsbIdSwitchOpen();
		ReleaseUsbIdSwitchOpen();

#ifdef CONFIG_CABLE_DETECT_ACCESSORY
		ProcessMhlStatus(false, true);
#endif

	}

	return I2C_ACCESSIBLE;
}

// Int4Isr: a
static void MhlTxDrvProcessConnection(void)
	void	SiiMhlTxNotifyConnection(bool mhlConnected)
		if (mhlConnected) {
			SiiMhlTxTmdsEnable();
				TPI_DEBUG_PRINT(("MhlTx:SiiMhlTxTmdsEnable\n"));
				SiiMhlTxDrvTmdsControl(true);
					if (enable) {
						SET_BIT(TPI_SLAVE_ADDR, 0x80, 4);
						TPI_DEBUG_PRINT(("Drv: TMDS Output Enabled\n"));
						SiiMhlTxDrvReleaseUpstreamHPDControl();  /* this triggers an EDID read */
					} else {
						CLR_BIT(TPI_SLAVE_ADDR, 0x80, 4);
						TPI_DEBUG_PRINT(("Drv: TMDS Ouput Disabled\n"));
					}

// TPI_Poll: b
static void MhlCbusIsr(void)

	if (BIT_6 & (dsHpdStatus ^ cbusInt)) {
		uint8_t status = cbusInt & BIT_6;
		/* SiiMhlTxNotifyDsHpdChange( cbusInt ); */
		TPI_DEBUG_PRINT(("Drv: Downstream HPD changed to: %02X\n", (int) cbusInt));
		SiiMhlTxNotifyDsHpdChange(status);
		if (status)
			SiiMhlTxDrvReleaseUpstreamHPDControl();


