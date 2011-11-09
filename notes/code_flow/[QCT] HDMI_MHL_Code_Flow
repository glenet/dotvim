// HDMI MHL Code Flow

// Insert

cable_detect_handler(drivers/misc/cable_detect.c)
sii9234_mhl_device_wakeup(sii9234.c) //cable insert 之後呼叫
	檢查 HDMI 是否 ready
	USB switch 
	MHL rest pin to High
	mhl_1v2_power(1)
		開對應 GPIO (MHL_SCL, MHL_SDA, MHL_HPD)
		hdmi_hpd_feature
			external_common_state->hpd_feature(1);
										hdmi_msm_hpd_feature
											hdmi_msm_hpd_on (由MHL控制HDMI, 這邊改為MHL->Panel->HDMI,
															NV的hdmi函數tegra_dc_hdmi_enable)
												clk, core power, 5v 
												確認 hpd 的狀態

			mutex_lock(&external_common_state_hpd_mutex);
			external_common_state->hpd_feature_on = 1;
			mutex_unlock(&external_common_state_hpd_mutex);
	
	TPI_Init(TPI.c)


// Suspend

<7>[  107.088554] [DISP]sii9234_early_suspend(isMHL=1, g_bGotUsbBus=1)
<7>[  107.091026] [DISP]Drv: TMDS Ouput Disabled
<6>[  107.091483] cy8c_ts_suspend: 84, 0
<6>[  107.138999] [VIB] pmic8058_vib_enable, er.ServerThread(parent:zygote): vibrates 0 msec
<6>[  107.346427] mipi_dsi_off+ 
<6>[  107.346518] Panel Type = PANEL_ID_HOY_SONY_OTM
Update: [2011/11/01 14:33:24 (107.366012291)]
<6>[  107.456748] holiday_panel_power(0): init=2
<4>[  107.468802] [DISP]mipi_dsi_disable_irq: IRQ cannot be disabled
<6>[  107.468894] [DISP] mipi_dsi_sw_reset: ----- need to do soft reset! -----
<6>[  107.479148] mipi_dsi_off-
<6>[  107.479270] [R] early_suspend end

sii9234_early_suspend(sii9234.c)
	g_bEnterEarlySuspend = true; // Enter the early suspend state
	cancel_delayed_work(&pInfo->mhl_on_delay_work); // Cancel the previous TMDS on delay work...
	sii9234_suspend(pInfo->i2c_client, PMSG_SUSPEND); // Turn-off the TMDS output... 
		SiiMhlTxDrvTmdsControl(false) 


// Resume

<7>[  110.689747] [DISP]sii9234_late_resume()

<7>[  111.682177] [DISP]mhl_on_delay_handler(0, 1)
<6>[  111.682512] mhl_sii9234_1v2_power(off): success
<4>[  111.683062] update_mhl_status: -+-+-+-+- MHL is NOT connected, status = -1 -+-+-+-+-
<6>[  111.683367] send_mhl_connect_notify: 0
<6>[  111.683916] [CABLE] mhl_status_notifier_func: isMHL 0, charging type 0, id_pin 1
<6>[  111.684221] [CABLE] MHL removed (cable_detect.c -> cable_detect_handler)
<6>[  111.684771] [CABLE] holiday_usb_dpdn_switch: Set USB path (board-holiday.c -> .usb_dpdn_switch = holiday_usb_dpdn_switch)
<7>[  111.685076] [DISP]sii9234_change_usb_owner(0)
<6>[  111.685442] MHL ISR

sii9234_late_resume
	queue_delayed_work(pInfo->wq, &pInfo->mhl_on_delay_work, HZ);
													|
		INIT_DELAYED_WORK(&pInfo->mhl_on_delay_work, mhl_on_delay_handler);
															|
			pInfo->mhl_1v2_power(0);
			update_mhl_status(false, CONNECT_TYPE_UNKNOWN); // Simulate the cable out to reinitiate the MHL...
				queue_work(pInfo->wq, &pInfo->mhl_notifier_work);
													|
					send_mhl_connect_notify
						list_for_each_entry(mhl_notifier, &g_lh_mhl_detect_notifier_list, mhl_notifier_link)
																								|
							mhl_detect_register_notifier(定義在 sii9234.c, 在 cable_detect.c 中被使用)
							// ./drivers/misc/cable_detect.c:1011: mhl_detect_register_notifier(&mhl_status_notifier);
								mhl_status_notifier_func(bool isMHL, int charging_type)




// Edge MHL's HDMI flow

tegra_dc_hdmi_irq

	if (tegra_dc_hdmi_hpd(dc))
		queue_delayed_work(system_nrt_wq, &hdmi->work, msecs_to_jiffies(100));
												|
											在 tegra_dc_probe 裡面被 init
													|
	tegra_dc_probe -> tegra_dc_set_out -> tegra_dc_set_out
													dc->out_ops->init(dc)
																	|
																.init = tegra_dc_hdmi_init
																				|
																	INIT_DELAYED_WORK(&hdmi->work, tegra_dc_hdmi_detect_worker)
																											|
																									tegra_dc_enable
																										|
																									tegra_dc_hdmi_enable
																										|
																								dc->out_ops->enabled

