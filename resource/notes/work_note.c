// work note

[grep]

egrep 'Error|Exception' logfile.txt // search 'Error' or 'Exception'
egrep "\<you\>.*\<me\>" world.txt	// search 'you' and 'me' happne in the same line

grep -i Error logfile	// case insensitive search
grep -C 2 'hello' *	// prints two lines of context around each matching line
grep -w ERROR logfile	// whole word instead of just pattern
grep 'ERROR\>' * // Searches only for words ending in 'ERROR', use `\<' and `\>' to match the start and end of words
grep -nRw VirtualKeys $(ls -tr device_e0004_20120423_163943.txt*) // show grep result according log modified time
grep -nR -A 100 sii9234_early_suspend kernel_e0004_20120423_163943.txt | egrep 'sii9234_early_suspend|Update'

[find]
find ./kernel -name sii9234.cpp	// find filename sii9234.cpp below kernel/
find / -name mysql -type d		// find a directory called mysql under / directory:

[sed]
sed -n 2,4p somefile.txt	// extract 2~4 row from somefile.txt
sed -n 1000, 2000p log.txt | egrep 'error|ANR'

[NV Doc pass]
Mobile-HTC@nvidia.com
rjkOzzSu

NV Display Expert:  Peter Zu	(0987132290) PZu@nvidia.com
NV Display Expert:  Vick  Yu	(0912581543) vyu@nvidia.com

[Repo]

1. Reset 所有 git 回到某個時間點（也可針對單一 git）：
  $ . setbuildenv u
  $ cd $TOP
  $ chmod 755 reset
  $ repo forall -pvc '$TOP/reset "10/10 23:59 2011"'

2. 過濾出 git 中某個時間區間的 commit：
  $ chmod 755 filter-date
  $ cd framework/base
  $ ../../filter-date "10/14 00:00" "10/14 23:59"

3.
  $ repo forall -pvc '$TOP/repocheck "base"'

[Build Code]
// 查看自己的 define 有沒有進去
vim $OUT/obj/KERNEL/.config

// 查看 device 是 XA, XB, or ...
在 kernel log 裡面 search "phase"

// flash code in Ship build
Use RUU mode to flash your zip in ship build.
You can check ship build on the string of hboot.

How to use RUU.
1. fastboot oem rebootRUU
2. fastboot flash zip xxx.zip

// 乾淨的 build 出 boot.img
mv out/target/product/endeavortd/root/ out/target/product/endeavortd/root.bak
build_all_noprebuilt bootimage


[GPIO]

// Kernle gpio 設定大全 (這些只是 config, 真正要 work 要呼叫 tegra_gpio_enable)

Config          Sample Code
-------         ------------
O(L)            #include <linux/gpio.h>
                gpio_direction_output(unsigned gpio, 0);

O(H)            #include <linux/gpio.h>
                gpio_direction_output(unsigned gpio, 1);

I(NP)           #include <linux/gpio.h>
I(PU)           gpio_direction_input(unsigned gpio);
I(PD)           #include <mach/pinmux.h>
                int tegra_pinmux_set_pullupdown(enum tegra_pingroup pg, enum tegra_pullupdown pupd);

                enum tegra_pullupdown {
                    TEGRA_PUPD_NORMAL = 0,
                    TEGRA_PUPD_PULL_DOWN,
                    TEGRA_PUPD_PULL_UP,
                };

A               #include <mach/gpio.h>
                tegra_gpio_disable(int gpio);

NORMAL,     OUTPUT == output only
TRISTATE,   INPUT  == input only
NORMAL,     INPUT  == bi-direction
TRISTATE,   OUTPUT == disable input and output

// GPIO 和 SFIO
SFIO 的 Mux 參數要去 arch/arm/mach-tegra/pinmux-t3-tables.c 查詢
GPIO 則使用 RSVD (Reserved 的意思，使用 RSVD 就要去 bootloader 確定使否有 config 這個 pin)

// 如果gpio不如自己預期怎麼辦
  1. grep一下, 是否被別人修改
  2. 在 pinmux 設定之後, 緊接用 gpio_dump 看值對不對
  3. Active 狀態不需要在 resume 特別設定
  4. 在 pinmux 的 config 只是單純設定, 真正要作用，要呼叫 tegra_gpio_enable
  5. gpio_request_array 也只是設定，下面要加 tegra_gpio_enable 迴圈來 enable
  6. request 過的 gpio 不需要再 request一次，直接 tegra_gpio_setvalue 就可以


[properties]
// change resolution policy
adb shell setprop persist.tegra.hdmi.resolution Max
// gr debug
adb shell setprop debug.gr.mode 1

[adb]
// wifi adb
adb tcpip 5555
adb connect 192.168.1.3:5555
adb logcat
adb disconnect

// Dump HtcSsdTestTool log
adb pull /data/htclog htc_log

// 清除 htclog
adb shell rm /data/htclog/＊

// Dump gpio in kernel
// 如果要在 suspend 狀態下 dump gpio, 要把 mach-tegra/gpio.c 下 tegra_gpio_suspend(void) 裡面的 gpio_dump 註解拿掉
adb shell cat /d/htc_tegra_gpio

// See gpio dump in uart right before suspend
In Hboot: writeconfigex 1, then reboot

// 沒開 SsdTestTool, 看 device log 的方式
1. 把 system/core/rootdir/init.rc 裡面 dlog 的註解拿掉
2. adb pull /data/dlog/ dlog/

// 調整 backlight 最亮
echo 255 > sys/class/backlight/tegra-pwm-bl/brightness

// 設定調整 hdmi 參數
adb shell "echo 4 > /sys/devices/nvhost/tegradc.1/graphics/fb1/blank" (關)
adb shell "echo 0 > /sys/devices/nvhost/tegradc.1/graphics/fb1/blank" (開)
*echo 4，本來顯示的畫面會暗掉，再 echo 0 會在亮起來
*echo 4 之後，拔除 cable 再插上，還是可以顯示

adb shell "cat /sys/class/switch/tegradc.1/state (ok, 1280x720)
adb shell "cat /sys/class/switch/tegradc.1/subsystem/hdmi/state" (ok, 0:unplugged, 1:plugged)

// 將 hal 層的更動更新到 device 裡面
adb remount;	//要更動 read-only 的區域要先做的動作
adb push $OUT/system/lib/hw/gralloc.tegra.so system/lib/hw;
adb reboot;

// 看SurfaceFlinger的設定
adb shell dumpsys SurfaceFlinger

// Disable HWC
adb shell service call SurfaceFlinger 1008 i32 1

// Disable overscan
adb shell setprop persist.tegra.hdmi.overscan.val 0

// 進kernel D-cable看不到log使用
adb shell cat proc/kmsg
adb shell cat /proc/kmsg 2>&1 | tee log.txt

// dump clock state
cat /d/clock/clock_tree

// dump dc regs
adb shell cat /d/tegradc.1/regs

// 列出所有 file node, 然後去找 file node 下面有 regs 的欄位可以 cat 出來看該 device 的 regs 值
adb shell ls -R

// 查看某 deamon 是否正在運行
adb shell top -n 1 | grep hdmi

// 看 RAMDUMP lastkmsg
adb shell cat /proc/last_kmsg > lastk.txt

// dump layer
adb shell dumplayer

// 連續執行指令
watch -n 0.3 "adb shell cat /d/clock/clock_tree | grep disp1"

// 看掛在哪個 func, 前提 adb 活著
echo w > /proc/sysrq-trigger

// pcbid r/w
fastboot oem readpcbid
fastboot oem writepcbid 2

// skew r/w
fastboot oem readsku
fastboot oem writesku 1 0x802FFFF
fastboot oem writesku b 1 //write engineer id

//hboot r/w i2c command
The SLAVE_ADDR should be the address you mentioned with a 1 bit right shift as
0x96->0x4B
0x76->0x3B

i2cw BUS SLAVE_ADDR REGISTER_OFFSET LENGTH DATA
hboot> i2cw 4 x4B x12 1 x15

i2cr BUS SLAVE_ADDR REGISTER_OFFSET LENGTH
hboot> i2cr 4 x4B x12 1

// fastboot r/w i2c register
bus   addr  offset   byte   value
fastboot oem i2cw 4     x3b   x93      1      x80
fastboot oem i2cr 4     x3b   x93      1

// remove watermark
adb pull /data/dalvik-cache/system@framework@services.jar@classes.dex .;perl -pi -e 's/com\/android\/server\/ShowWatermarkService/cmm\/android\/server\/ShowWatermarkService/g' system@framework@services.jar@classes.dex;adb push system@framework@services.jar@classes.dex /data/dalvik-cache/

[Issue]
// ITS debug skill
1. 不要相信 QA 的 time stamp, 拿到 issue 馬上檢查 log 時間的正確性

// framework keyword
ANR(Application Not Responding) fatal error fingerprint Watchdog "About to"(surfaceflinger) die 'I DEBUG'

// kernel keyword
DISP dc brightness late_resume early_suspend panic

log 裡面看到 tegra-grhost 代表某個 module hung 住

當問題發生在某個 func 的時候，可以把該 func 變成一個單純印 log 的 func，釐清是否是該 func 搞得鬼！

夾版本的時候，先用daily build找出哪兩個build之間發生問題，然後可以燒好的daily build 然後在單燒壞的daily build的kernel，確認問題是否發生在kernel。

[Code]
1. Please alway add comment in code if it is workaround or issue
2. porting 的時候要確認每個部份都有正確的被編譯，那就是在你預期要 run 到的地方都加一些 print 確保他真的 run 到了！
3. 發生undefined reference to 的時候，檢查是否函數被 #ifdef 給包住了！
4. 有 device_create 才有 node. tegra/fb.c 是 fbmem.c fb_ioctl 裡面的實做

[MHL]
// Sync QCT code
1. git checkout -b ics403-qct8960 remotes/origin/ics403-qct8960
2. ~/repo sync kernel
3. ics403-qct8960 就看 msm/sii9234

[MFG Communication]
1. 那一版的ROM
2. 在什麼機台
3. 測試的方法 (之前有作什麼測試) step by step
4. 判定 fail 的標準, fail rate 怎麼算
5. 異常現象解釋, log file