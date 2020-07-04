# tegra30_debrick

- [Disclaimer](#Disclaimer)  
- [Thanks](#Thanks)  
- [License](#License)  
- [Background](#Background)  
- [Files](#Files)  
- [Nexus 7 Debrick](#Nexus-7-Debrick)  
- [Ouya Debrick](#Ouya-Debrick)  

## Disclaimer

If you're here, there is a good chance that your Tegra 3 device is already bricked. But I am not 
responsible for any additional issues that may arise from the (mis)use of the code/information 
contained within this repository, nor can I provide support for it.

## Thanks

[@ktemkin](https://github.com/ktemkin) / [@Qyriad](https://github.com/Qyriad) for their work on 
[fusee-launcher](https://github.com/Qyriad/fusee-launcher), and [@jevinskie](https://github.com/jevinskie) 
for their [Nexus 7 port](https://github.com/jevinskie/fusee-launcher) of the same. Also special 
thanks to [@ktemkin](https://github.com/ktemkin) and [@digetx](https://github.com/digetx) for their 
help/guidance/wisdom, as I almost certainly wouldn't have succeeded in debricking my Nexus 7 
without it. [@pgwipeout](https://github.com/pgwipeout)'s kernel work on Ouya allowed me to keep 
using mine (and retain interest in the device) over the years. Finally, Pyre on the OUYA Saviors 
Discord kindly shipped me a (working) Kickstarter Ouya so that I could figure out how to debrick it.

## License

[GNU General Public License v2.0](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html). 
fusee-launcher is also released under that, so it seemed fitting.

## Background

Last December I was gifted a bricked 2012 Nexus 7, but it did enumerate via APX mode, and I enjoy a 
challenge :)

## Files

The following files are contained within this repo:  

- [README.md](/README.md): this README  
- [LICENSE](/LICENSE): The GPLv2  
- [fusee-launcher](https://github.com/jevinskie/fusee-launcher): jevinskie's fork, added as a submodule  
- [nvflash\_v1.13.87205](/utils/nvflash_v1.13.87205): unpatched nvflash utility (from [here](https://github.com/AndroidRoot/androidroot.github.io/blob/master/download/nvflash-tools-linux.tar.bz2))  
- [nvflash\_v1.13.87205\_miniloader\_patched](/utils/nvflash_v1.13.87205_miniloader_patched): as above, with patched miniloader to override security fuse checks  
- [ipatch\_rcm\_sample.c](/payload/ipatch_rcm_sample.c): Tegra X1 Fusée Gelée payload to disable security fuse checks (by ktemkin)  
- [uart\_payload.c](/payload/uart_payload.c): as above, but tailored to Tegra 3 (Nexus 7 and Ouya)  
**Nexus 7:**  
- [bootloader-grouper-4.23.img](/bootloader/bootloader-grouper-4.23.img): unpatched 2012 Nexus 7 WiFi "grouper" bootloader  
- [bootloader-grouper-4.23\_uart.img](/bootloader/bootloader-grouper-4.23_uart.img): as above, with UART output patched in  
- [uart\_print.c](/bootloader/uart_print.c): source of UART print function patched into bootloader-grouper-4.23\_uart.img  
- [nexus\_7\_grouper\_bct.bin](/bct/nexus_7_grouper_bct.bin): BCT dumped from "grouper" using nvflash  
- [uart\_payload\_n7.bin](/payload/uart_payload_n7.bin): precompiled version of uart\_payload.c  
- [n7\_uart.jpg](/image/n7_uart.jpg): annotated image of PCB showing UARTA pinout  
- [n7\_uart\_schematic.jpg](/image/n7_uart_schematic.jpg): excerpt of ME370T schematic showing debug header pinout  
- [n7\_uart\_context.jpg](/image/n7_uart_context.jpg): picture of opened tablet showing context of header location  
- [flash.cfg](/utils/flash.cfg): partition config file for `nvflash`. Technically for the 16GB Nexus 7, but sufficient on Ouya for relevant steps in the Ouya Debrick section  
**Ouya:**  
- [ouya\_rev\_1.01\_2013-06-20.bin](/bootloader/ouya_rev_1.01_2013-06-20.bin): decrypted fastboot bootloader, dumped with nvflash  
- [ouya\_rev\_1.01\_2013-06-20\_sigcheck\_disabled.bin](/bootloader/ouya_rev_1.01_2013-06-20_sigcheck_disabled.bin): as above, but with signature check disabled allowing flashing of unsigned bootloader to eMMC  
- [ouya\_rev\_1.01\_bct.bin](/bct/ouya_rev_1.01_bct.bin): BCT dumped via nvflash and decrypted  
- [ouya\_apx\_glitch.jpg](/image/ouya_apx_glitch.jpg): annotated image of PCB showing how to force APX mode  
- [uart\_payload\_ouya.bin](/payload/uart_payload_ouya.bin): precompiled version of uart\_payload.c  
- [recovery-clockwork-6.0.4.8-ouya.img](/recovery/recovery-clockwork-6.0.4.8-ouya.img): ClockworkMod Recovery  

More detailed descriptions of the files can be found if you check their git history, for example [nvflash\_v1.13.87205\_miniloader\_patched's](https://github.com/tofurky/tegra30_debrick/commits/master/utils/nvflash_v1.13.87205_miniloader_patched).

## Nexus 7 Debrick

Coming soon 😬

## Ouya Debrick

Before attempting any of this, be sure that your system isn't recoverable via other means. This could be as simple as hooking up a USB cable and running `adb`, or possibly plugging in a USB keyboard and attempting to enter [recovery mode](https://web.archive.org/web/20150502163510/https://forums.ouya.tv/discussion/comment/11742/#Comment_11742).

### Prerequisites:

- Linux machine with:
	- free USB3 port (required for fusee-launcher)
	- (if 64-bit kernel/userland) 32-bit libraries installed (_for .deb-based distros_ `dpkg --add-architecture i386; apt install libc6:i386 libstdc++6:i386`)
	- `pyusb` installed (_for .deb-based distros_ `apt install python3-usb`)
	- `fastboot` installed (_for .deb-based distros_ `apt install fastboot`)
	- `adb` installed (_for .deb-based distros_ `apt install adb`)
	- recursive clone of this repository (`git clone --recursive https://github.com/tofurky/tegra30_debrick.git`)
- Ouya OTA update .zip (if you had a bad kernel flash, for example)
	- a large .7z with multiple versions can be found [here](https://archive.org/details/OuyaFirmware.7z)
- (if Ouya isn't automatically booting to APX) low ohm resistor (I used 47 ohm) connected to ground with an e.g. wire and/or test clip
	- if you're brave, dextrous, and very careful you _might_ even get away with a paperclip grounded to the springy clip on the edge of the PCB
- Some basic knowledge/familiarity with Linux command line
- Some basic knowledge/familiarity with flashing Android (e.g. fastboot and adb)

<!-- sections below are best viewed rendered as HTML via e.g. /usr/bin/markdown - if you're reading this in a text editor, sorry ;-) -->

### Steps

1. Connect Ouya to power, but leave powered off

2. Connect Ouya to *USB3* port on Linux machine via Micro-USB jack

3. Power on Ouya and check `dmesg` and/or `lsusb` output on Linux machine. Take note if the Ouya automatically enumerates in APX mode:

	<details><summary><i>Example <code>dmesg</code> output showing enumeration in APX mode:</i></summary>

		    [Thu Jul  2 16:28:11 2020] usb 2-3.3.2: new high-speed USB device number 8 using xhci_hcd
		    [Thu Jul  2 16:28:11 2020] usb 2-3.3.2: New USB device found, idVendor=0955, idProduct=7030, bcdDevice= 1.03
		    [Thu Jul  2 16:28:11 2020] usb 2-3.3.2: New USB device strings: Mfr=1, Product=2, SerialNumber=0
		    [Thu Jul  2 16:28:11 2020] usb 2-3.3.2: Product: APX
		    [Thu Jul  2 16:28:11 2020] usb 2-3.3.2: Manufacturer: NVIDIA Corp.
	</details>

	<details><summary><i>Example <code>lsusb</code> output showing device in APX mode:</i></summary>

		    matt@aquos:~/devel/ouya/tegra30_debrick$ lsusb
		    ...
			Bus 002 Device 055: ID 0955:7030 NVIDIA Corp. T30 [Tegra 3] recovery mode
		    ...
	</details>

4. If Ouya is *not* automatically booting to APX mode, do the following:
	* Disassemble Ouya by removing the 4 screws on the top and carefully sliding out PCB. [This iFixit teardown](https://www.ifixit.com/Teardown/Ouya+Teardown/14224) may be helpful.
	* Repeat steps 1 & 2 to reconnect power and Micro-USB cable
	* Taking ESD precautions, connect [pin on PFET](/image/ouya_apx_glitch.jpg) to ground via low ohm resistor (~47 ohm - other values including 0 ohm probably work)
	* Power on Ouya with button
	* Leave PFET pin grounded for approximately 2s after pressing power button. This is about the time it takes for the fan to spin up.
	* Check Linux `dmesg` output on your PC to see if the Ouya enumerated in APX mode.
		- If it did, but reset afterwards, you've held the PFET to ground for too long.
		- If it didn't, try holding it a small amount longer.

	<details><summary><i>Example <code>dmesg</code> output showing success:</i></summary>

		    [Thu Jul  2 16:28:11 2020] usb 2-3.3.2: new high-speed USB device number 8 using xhci_hcd
		    [Thu Jul  2 16:28:11 2020] usb 2-3.3.2: New USB device found, idVendor=0955, idProduct=7030, bcdDevice= 1.03
		    [Thu Jul  2 16:28:11 2020] usb 2-3.3.2: New USB device strings: Mfr=1, Product=2, SerialNumber=0
		    [Thu Jul  2 16:28:11 2020] usb 2-3.3.2: Product: APX
		    [Thu Jul  2 16:28:11 2020] usb 2-3.3.2: Manufacturer: NVIDIA Corp.
	</details>

5. From APX mode, execute fusee-launcher using [uart\_payload\_ouya.bin](/payload/uart_payload_ouya.bin). Within the `tegra30_debrick` directory, run:

		    sudo ./fusee-launcher/fusee-launcher.py ./payload/uart_payload_ouya.bin -P 7030

	<details><summary><i>Example terminal output:</i></summary>

		    matt@aquos:~/devel/ouya/tegra30_debrick$ sudo ./fusee-launcher/fusee-launcher.py ./payload/uart_payload_ouya.bin -P 7030
		    2020-07-02 22:04:36,408 INFO:usb.core:find(): using backend "usb.backend.libusb1"
		    ​
		    Important note: on desktop Linux systems, we currently require an XHCI host controller.
		    A good way to ensure you're likely using an XHCI backend is to plug your
		    device into a blue 'USB 3' port.
		    ​
		    Identified a Linux system; setting up the appropriate backend.
		    intermezzo_size: 0x00000078
		    target_payload_size: 0x000005ee
		    Found a Tegra with Device ID: b'0210380c06495d01'
		    Stack snapshot: b'0000000000000000100000003c9f0040'
		    EndpointStatus_stack_addr: 0x40009f3c
		    ProcessSetupPacket SP: 0x40009f30
		    InnerMemcpy LR stack addr: 0x40009f20
		    overwrite_len: 0x00004f20
		    overwrite_payload_off: 0x00004de0
		    payload_first_length: 0x000005ee
		    overwrite_payload_off: 0x00004de0
		    payload_second_length: 0x00000000
		    b'00a0004000300040ee05000000000000'
		    Setting rcm msg size to 0x00030064
		    RCM payload (len_insecure): b'64000300'
		    ​
		    Setting ourselves up to smash the stack...
		    Payload offset of intermezzo: 0x00000074
		    overwrite_payload_off: 0x00004de0
		    overwrite_len: 0x00004f20
		    payload_overwrite_len: 0x00004e5c
		    overwrite_payload_off: 0x00004de0
		    smash_padding: 0x000047f2
		    overwrite_payload_off: 0x00004de0
		    Uploading payload...
		    txing 20480 bytes total
		    txing 4096 bytes (0 already sent) to buf[0] 0x40003000
		    txing 4096 bytes (4096 already sent) to buf[1] 0x40005000
		    txing 4096 bytes (8192 already sent) to buf[0] 0x40003000
		    txing 4096 bytes (12288 already sent) to buf[1] 0x40005000
		    txing 4096 bytes (16384 already sent) to buf[0] 0x40003000
		    Smashing the stack...
		    sending status request with length 0x00004f20
		    The USB device stopped responding-- sure smells like we've smashed its stack. :)
		    Launch complete!
	</details>

	<details><summary><i>Example output from USB serial adapter connected to Ouya's UART after successfully running <code>uart_payload_ouya.bin</code>:</i></summary>

		    ----------------------------------------------------------------------------
		    APBDEV_PMC_RST_STATUS_0: 00000000
		    BIT_BootType: 00000002
		    overriding getSecurityMode function to always return 3 (production non-secure)...
		    writing PMC_SCRATCH0 to trigger RCM mode after soft reset...
		    jumping to 0xfff01004...
	</details>

	<details><summary><i>Example <code>dmesg</code> output after successfully running <code>uart_payload_ouya.bin</code>. Note that the USB device will reset and reenumerate in APX mode:</i></summary>

		    [Thu Jul  2 16:35:48 2020] usb 2-3.3.2: USB disconnect, device number 8
		    [Thu Jul  2 16:35:49 2020] usb 2-3.3.2: new high-speed USB device number 9 using xhci_hcd
		    [Thu Jul  2 16:35:49 2020] usb 2-3.3.2: New USB device found, idVendor=0955, idProduct=7030, bcdDevice= 1.03
		    [Thu Jul  2 16:35:49 2020] usb 2-3.3.2: New USB device strings: Mfr=1, Product=2, SerialNumber=0
		    [Thu Jul  2 16:35:49 2020] usb 2-3.3.2: Product: APX
		    [Thu Jul  2 16:35:49 2020] usb 2-3.3.2: Manufacturer: NVIDIA Corp.
	</details>

6. Boot from APX to fastboot's _nv3pserver_ mode like so:

		    sudo ./utils/nvflash_v1.13.87205_miniloader_patched --setbct --bct ./bct/ouya_rev_1.01_bct.bin --configfile ./utils/flash.cfg --bl ./bootloader/ouya_rev_1.01_2013-06-20_sigcheck_disabled.bin --go

	<details><summary><i>Example terminal output from <code>nvflash</code> command:</i></summary>

		    matt@aquos:~/devel/ouya/tegra30_debrick$ sudo ./utils/nvflash_v1.13.87205_miniloader_patched --setbct --bct ./bct/ouya_rev_1.01_bct.bin --configfile ./utils/flash.cfg --bl ./bootloader/ouya_rev_1.01_2013-06-20_sigcheck_disabled.bin --go
		    Nvflash v1.13.87205 started
		    chip uid from BR is: 0x0000000000000000015d49060c381002
		    rcm version 0X30001
		    System Information:
		       chip name: unknown
		       chip id: 0x30 major: 1 minor: 3
		       chip sku: 0x80
		       chip uid: 0x0000000000000000015d49060c381002
		       macrovision: disabled
		       hdcp: enabled
		       jtag: disabled
		       sbk burned: true
		       dk burned: true
		       boot device: emmc
		       operating mode: 3
		       device config strap: 0
		       device config fuse: 0
		       sdram config strap: 0
		    ​
		    sending file: ./bct/ouya_rev_1.01_bct.bin
		    - 6128/6128 bytes sent
		    ./bct/ouya_rev_1.01_bct.bin sent successfully
		    downloading bootloader -- load address: 0x80108000 entry point: 0x80108000
		    sending file: ./bootloader/ouya_rev_1.01_2013-06-20_sigcheck_disabled.bin
		    / 1011728/1011728 bytes sent
		    ./bootloader/ouya_rev_1.01_2013-06-20_sigcheck_disabled.bin sent successfully
		    waiting for bootloader to initialize
		    bootloader downloaded successfully
	</details>

	<details><summary><i>Example Ouya UART output after <code>nvflash</code> command:</i></summary>

		    Bootloader AVP Init
		    **********Aos DebugSemiHosting Initialized*******
		    ---------------------------------------------------
		    NVRM Initialized shmoo database
		    NVRM CLOCKS: PLLX0:      700000 Khz
		    NVRM CLOCKS: PLLM0:      800000 Khz
		    NVRM CLOCKS: PLLC0:      600000 Khz
		    NVRM CLOCKS: PLLP0:      408000 Khz
		    NVRM CLOCKS: PLLA0:      11289 Khz
		    NVRM CLOCKS: CPU:        700000 Khz
		    NVRM CLOCKS: AVP:        102000 Khz
		    NVRM CLOCKS: System Bus: 102000 Khz
		    NVRM CLOCKS: Memory Controller: 200000
		    NVRM CLOCKS: External Memory Controller: 400000
		    Fake BoardInfo: 0x0c5b:0x0b01:0x04:0x43:0x03
		    ADJUSTED CLOCKS:
		    MC clock is set to 200000 KHz
		    EMC clock is set to 400000 KHz (DDR clock is at 400000 KHz)
		    PLLX0 clock is set to 700000 KHz
		    PLLC0 clock is set to 600000 KHz
		    CPU clock is set to 700000 KHz
		    System and AVP clock is set to 102000 KHz
		    GraphicsHost clock is set to 163200 KHz
		    3D clock is set to 133333 KHz
		    2D clock is set to 133333 KHz
		    Epp clock is set to 133333 KHz
		    Mpe clock is set to 133333 KHz
		    Vde clock is set to 272000 KHz
		    Pinmux changes applied in kernel way
		    Bootloader Start at:44553 ms
		    ​
		    [bootloader] (built on Jun 20 2013, 22:10:09)
		    Initializing Display
		    Invalidate-only cache maint not supported in NvOs
		    Platform Pre Boot configuration...
		    Entering NvFlash recovery mode / Nv3p Server
	</details>

7. (Optional, but recommended) Take partition-by-partition dumps of eMMC by running:

		    for partition in BCT PT EBT EKS GP1 SOS LNX APP CAC UPP MSC USP MDA GPT UDA; do sudo ./utils/nvflash_v1.13.87205_miniloader_patched --resume --read $partition $partition.bin; done

	* Note that the last partition, UDA (userdata), has a decent chance of hanging. If it does, it may be necessary to power cycle the Ouya. To run it as a one-off:

		    sudo ./utils/nvflash_v1.13.87205_miniloader_patched --resume --read UDA UDA.bin

8. If Ouya was not automatically booting to APX mode (e.g. bad kernel)

	* Reflash LNX with e.g. CWM Recovery:

		    sudo ./utils/nvflash_v1.13.87205_miniloader_patched --resume --download LNX ./recovery/recovery-clockwork-6.0.4.8-ouya.img --go

	<details><summary><i>Example <code>nvflash</code> output:</i></summary>

		    matt@aquos:~/devel/ouya/tegra30_debrick$ sudo ./utils/nvflash_v1.13.87205_miniloader_patched --resume --download LNX ./recovery/recovery-clockwork-6.0.4.8-ouya.img --go
		    Nvflash v1.13.87205 started
		    [resume mode]
		    sending file: ./recovery/recovery-clockwork-6.0.4.8-ouya.img
		    - 8151040/8151040 bytes sent
		    ./recovery/recovery-clockwork-6.0.4.8-ouya.img sent successfully
	</details>

	* Confirm Ouya boots into recovery (being attached to a TV via HDMI helps here):

		    sudo adb devices

	<details><summary><i>Example <code>adb</code> output:</i></summary>

		    matt@aquos:~/devel/ouya/tegra30_debrick$ sudo adb devices
		    List of devices attached
		    015d49060c381002	recovery
	</details>

	<details><summary><i>Example <code>dmesg</code> output:</i></summary>

		    [Thu Jul  2 22:49:53 2020] usb 2-3.3.2: USB disconnect, device number 35
		    [Thu Jul  2 22:50:02 2020] usb 2-3.3.2: new high-speed USB device number 36 using xhci_hcd
		    [Thu Jul  2 22:50:02 2020] usb 2-3.3.2: New USB device found, idVendor=18d1, idProduct=d001, bcdDevice= 2.32
		    [Thu Jul  2 22:50:02 2020] usb 2-3.3.2: New USB device strings: Mfr=2, Product=3, SerialNumber=4
		    [Thu Jul  2 22:50:02 2020] usb 2-3.3.2: Product: Ouya
		    [Thu Jul  2 22:50:02 2020] usb 2-3.3.2: Manufacturer: Boxer8
		    [Thu Jul  2 22:50:02 2020] usb 2-3.3.2: SerialNumber: 015d49060c381002
	</details>

	* From recovery, reflash stock .zip, or `adb reboot-bootloader` to get to fastboot to reflash stock

9. If Ouya was automatically booting to APX mode (i.e. bad/erased bootloader/BCT):

	* Backup LNX partition if you haven't yet done so:

		    sudo ./utils/nvflash_v1.13.87205_miniloader_patched --resume --read LNX LNX.bin

	* Erase LNX partition via _nv3pserver_ mode by uploading all zeroes: 

		    truncate -s 8M LNX_all_zeroes.bin
		    sudo ./utils/nvflash_v1.13.87205_miniloader_patched --resume --download LNX LNX_all_zeroes.bin --go

	<details><summary><i>Example <code>nvflash</code> output:</i></summary>

		    matt@aquos:~/devel/ouya/tegra30_debrick$ truncate -s 8M LNX_all_zeroes.bin
		    matt@aquos:~/devel/ouya/tegra30_debrick$ sudo ./utils/nvflash_v1.13.87205_miniloader_patched --resume --download LNX LNX_all_zeroes.bin --go
		    Nvflash v1.13.87205 started
		    [resume mode]
		    sending file: LNX_all_zeroes.bin
		    / 8388608/8388608 bytes sent
		    LNX_all_zeroes.bin sent successfully
	</details>

	* _nvp3server_ should then try to boot Linux, and subsequently fail back to standard _fastboot_ mode

	<details><summary><i>Example UART output:</i></summary>

		    Start Downloading LNX
		    ​
		    End Downloading LNX
		    SocCpuMaxKHz = 1000000
		    SocCpuMinKHz = 32
		    PLLX0 FreqKHz = 700000
		    Checking for android ota recovery 
		    Key driver not found.. Booting OS
		    Cold-booting Linux
		     Booting failed
		    Starting Fastboot USB download protocol
		    Key driver not found.. Booting OS
	</details>

	<details><summary><i>Example <code>dmesg</code> output:</i></summary>

		    [Thu Jul  2 20:36:55 2020] usb 2-3.3.2: USB disconnect, device number 20
		    [Thu Jul  2 20:36:55 2020] usb 2-3.3.2: new high-speed USB device number 21 using xhci_hcd
		    [Thu Jul  2 20:36:55 2020] usb 2-3.3.2: New USB device found, idVendor=0955, idProduct=7000, bcdDevice= 0.00
		    [Thu Jul  2 20:36:55 2020] usb 2-3.3.2: New USB device strings: Mfr=1, Product=2, SerialNumber=3
		    [Thu Jul  2 20:36:55 2020] usb 2-3.3.2: Product: Fastboot
		    [Thu Jul  2 20:36:55 2020] usb 2-3.3.2: Manufacturer: NVIDIA Corp.
		    [Thu Jul  2 20:36:55 2020] usb 2-3.3.2: SerialNumber: 015d49060c381002
	</details>

	* Check to see if the device is available via fastboot (note: `sudo` isn't strictly necessary, but can workaround permissions issues):

		    sudo fastboot devices

	<details><summary><i>Example <code>fastboot</code> output:</i></summary>

		    matt@aquos:~/devel/ouya/tegra30_debrick$ sudo fastboot devices
		    015d49060c381002	fastboot
	</details>

	* Use the currently running _patched_ fastboot to reflash the _unpatched_ fastboot:

		    sudo fastboot flash bootloader ./bootloader/ouya_rev_1.01_2013-06-20.bin

	<details><summary><i>Example <code>fastboot</code> output:</i></summary>

		    matt@aquos:~/devel/ouya/tegra30_debrick$ fastboot flash bootloader ./bootloader/ouya_rev_1.01_2013-06-20.bin
		    target didn't report max-download-size
		    sending 'bootloader' (988 KB)...
		    OKAY [  0.212s]
		    writing 'bootloader'...
		    OKAY [  8.825s]
		    finished. total time: 9.037s
	</details>

	<details><summary><i>Example UART output:</i></summary>

		    Cmd Rcvd: getvar:slot-count
		    Response sent: OKAY
		    Cmd Rcvd: getvar:slot-suffixes
		    Response sent: OKAY
		    Cmd Rcvd: getvar:has-slot:bootloader
		    Response sent: OKAY
		    Cmd Rcvd: getvar:partition-type:bootloader
		    Response sent: OKAYbasic
		    Cmd Rcvd: getvar:max-download-size
		    Response sent: OKAY
		    Cmd Rcvd: download:000f7010
		    Response sent: DATA000f7010
		    ​
		    Response sent: OKAY
		    Cmd Rcvd: flash:bootloader
		    ​
		    Format partition USP 
		    Region=0 SD Erase start 512B-sector=2686976,512B-sector-num=65536 Response sent: OKAY
	</details>

	* Reboot into newly flashed bootloader:

		    sudo fastboot reboot-bootloader

	<details><summary><i>Example <code>fastboot</code> output:</i></summary>

		    matt@aquos:~/devel/ouya/tegra30_debrick$ sudo fastboot reboot-bootloader 
		    rebooting into bootloader...
		    OKAY [  0.004s]
		    finished. total time: 0.104s
	</details>

	* Reflash the Ouya kernel with the backup that was made earlier with `nvflash`:

		    sudo fastboot flash boot LNX.bin

	<details><summary><i>Example <code>fastboot</code> output:</i></summary>

		    matt@aquos:~/devel/ouya/tegra30_debrick$ sudo fastboot flash boot LNX.bin
		    target didn't report max-download-size
		    sending 'boot' (8192 KB)...
		    OKAY [  2.552s]
		    writing 'boot'...
		    OKAY [  2.683s]
		    finished. total time: 5.235s
	</details>

	* Boot into "new" kernel:

		    sudo fastboot continue

	<details><summary><i>Example <code>fastboot</code> output:</i></summary>

		    matt@aquos:~/devel/ouya/tegra30_debrick$ sudo fastboot continue
		    resuming boot...
		    OKAY [  0.004s]
		    finished. total time: 0.004s
	</details>
