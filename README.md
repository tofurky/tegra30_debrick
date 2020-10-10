# tegra30_debrick

- [Disclaimer](#Disclaimer)  
- [Thanks](#Thanks)  
- [License](#License)  
- [Background](#Background)  
- [Files](#Files)  
- [Other Methods](#Other-Methods)  
- [Nexus 7 Debrick](#Nexus-7-2012-WiFi-Debrick)  
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
- [Makefile](/payload/Makefile): Makefile to build both payloads from uart\_payload.c  
- [uart_payload.lds](/payload/uart_payload.lds): linker script to build payloads from uart\_payload.c  
**Nexus 7:**  
- [bootloader-grouper-4.23.img](/bootloader/bootloader-grouper-4.23.img): unpatched 2012 Nexus 7 WiFi "grouper" bootloader  
- [bootloader-grouper-4.23\_uart.img](/bootloader/bootloader-grouper-4.23_uart.img): as above, with UART output patched in  
- [uart\_print.c](/bootloader/uart_print.c): source of UART print function patched into bootloader-grouper-4.23\_uart.img  
- [nexus\_7\_grouper\_bct.bin](/bct/nexus_7_grouper_bct.bin): BCT dumped from "grouper" using nvflash  
- [uart\_payload\_n7.bin](/payload/uart_payload_n7.bin): precompiled version of uart\_payload.c  
- [n7\_uart.jpg](/image/n7_uart.jpg): annotated image of PCB showing UARTA pinout  
- [n7\_uart\_schematic.jpg](/image/n7_uart_schematic.jpg): excerpt of ME370T schematic showing debug header pinout  
- [n7\_uart\_context.jpg](/image/n7_uart_context.jpg): picture of opened tablet showing context of header location  
- [flash.cfg](/utils/flash.cfg): partition config file for nvflash. Technically for the 16GB Nexus 7, but sufficient on Ouya for relevant steps in the Ouya Debrick section  
**Ouya:**  
- [ouya\_rev\_1.01\_2013-06-20.bin](/bootloader/ouya_rev_1.01_2013-06-20.bin): decrypted fastboot bootloader, dumped with nvflash  
- [ouya\_rev\_1.01\_2013-06-20\_sigcheck\_disabled.bin](/bootloader/ouya_rev_1.01_2013-06-20_sigcheck_disabled.bin): as above, but with signature check disabled allowing flashing of unsigned bootloader to eMMC  
- [ouya\_rev\_1.01\_bct.bin](/bct/ouya_rev_1.01_bct.bin): BCT dumped via nvflash and decrypted  
- [uart\_payload\_ouya.bin](/payload/uart_payload_ouya.bin): precompiled version of uart\_payload.c  
- [ouya\_apx\_glitch.jpg](/image/ouya_apx_glitch.jpg): annotated image of PCB showing how to force APX mode  
- [recovery-clockwork-6.0.4.8-ouya.img](/recovery/recovery-clockwork-6.0.4.8-ouya.img): ClockworkMod Recovery  

More detailed descriptions of the files can be found if you check their git history, for example [nvflash\_v1.13.87205\_miniloader\_patched's](https://github.com/tofurky/tegra30_debrick/commits/master/utils/nvflash_v1.13.87205_miniloader_patched).

## Other Methods

In the months it took me to finally put this repo together after working through this between February and March of 2020, an alternate method was posted to XDA Developers tailored to the Nexus 7.

The following are a couple guides based on this, the first link being the original:

- [Unbrick Nexus 7 with another Tegra 3 Device](https://forum.xda-developers.com/nexus-7/general/unbrick-nexus-7-tegra-3-device-t4078627) by [Jirmd](https://github.com/GeorgeMato4)
- [[TUTORIAL] How to unbrick Nexus 7 without blob.bin (REQUIRES ANOTHER NEXUS 7 2012)](https://forum.xda-developers.com/nexus-7/general/tutorial-how-to-unbrick-nexus-7-blob-bin-t4083879) by [enderzip](https://forum.xda-developers.com/member.php?u=10063311)

It also uses [@jevinskie](https://github.com/jevinskie)'s [fusee-launcher fork](https://github.com/jevinskie/fusee-launcher), but to grab the SBK using their [dump-sbk-via-usb.S](https://github.com/jevinskie/fusee-launcher/blob/n7/dump-sbk-via-usb.S) payload.

The cavaet is that at least at the moment it appears to require another working device to generate the blobs.

That method is not covered here, but is probably worth trying if the steps detailed here don't work for you.

<!-- sections below are best viewed rendered as HTML via e.g. /usr/bin/markdown - if you're reading this in a text editor, sorry ;-) -->

## Nexus 7 (2012 WiFi) Debrick

Before attempting any of this, be sure that your tablet isn't recoverable via other means. A dead/low battery can sometimes be worked around by plugging it in to a [charger and holding down the power button for 30s](https://web.archive.org/web/20200704193425/https://www.howtogeek.com/131109/the-ultimate-nexus-7-troubleshooting-guide-6-potential-problems-and-how-to-fix-them/). Leaving it hooked up to a charger for some time (30+ minutes) can also get them to respond again. Unlike the Ouya, a bad kernel flash is recoverable by forcing the tablet into fastboot recovery with a button combo. Holding the volume down + power button for several seconds can boot into fastboot recovery mode which will allow the kernel to be reflashed. The steps below are only meant as a last resort, like if your bootloader was wiped via a botched update or similar.

### Prerequisites:

- Linux machine with:
	- free USB3 port (required for fusee-launcher) (_Intel chipsets may work more reliably here_)
	- (if 64-bit kernel/userland) 32-bit libraries installed (_for .deb-based distros_ `dpkg --add-architecture i386; apt update && apt install libc6:i386 libstdc++6:i386`)
	- `pyusb` installed (_for .deb-based distros_ `apt install python3-usb`)
	- `fastboot` installed (_for .deb-based distros_ `apt install fastboot`)
	- `adb` installed (_for .deb-based distros_ `apt install adb`)
	- `cbootimage` installed (_for .deb-based distros_ `apt install cbootimage`)
	- recursive clone of this repository (`git clone --recursive https://github.com/tofurky/tegra30_debrick.git`)
- Factory Android .zip for "nakasi". The latest is [nakasi-lmy47v-factory-5a0bb059.zip](https://dl.google.com/dl/android/aosp/nakasi-lmy47v-factory-5a0bb059.zip). Others can be found [here](https://developers.google.com/android/images#nakasi).
- Some basic knowledge/familiarity with Linux command line
- Some basic knowledge/familiarity with flashing Android (e.g. fastboot and adb)

### Steps:

1. Connect Nexus 7 to *USB3* port on Linux machine via Micro-USB jack.
	* If you have reason to believe the battery may be at less than 30% capacity, let it sit there for an hour or two before proceeding. This isn't just to play it safe - the bootloader will refuse to operate in _nvp3server_ mode if it's at <= 29%.

2. Check output of `dmesg` and `lsusb` commands. Take note if the tablet automatically enumerates in APX mode:

	<details><summary><i>Example <code>dmesg</code> output showing enumeration in APX mode:</i></summary>

		    [Sat Jul  4 12:12:44 2020] usb 2-3.4: new high-speed USB device number 86 using xhci_hcd
		    [Sat Jul  4 12:12:44 2020] usb 2-3.4: New USB device found, idVendor=0955, idProduct=7330, bcdDevice= 1.03
		    [Sat Jul  4 12:12:44 2020] usb 2-3.4: New USB device strings: Mfr=1, Product=2, SerialNumber=0
		    [Sat Jul  4 12:12:44 2020] usb 2-3.4: Product: APX
		    [Sat Jul  4 12:12:44 2020] usb 2-3.4: Manufacturer: NVIDIA Corp.
	</details>

	<details><summary><i>Example <code>lsusb</code> output showing device in APX mode:</i></summary>

		    matt@aquos:~/devel/ouya/tegra30_debrick$ lsusb
		    ...
		    Bus 002 Device 086: ID 0955:7330 NVIDIA Corp. 
		    ...
	</details>

3. If the tablet is not automatically entering APX mode, try the following to coerce it:
	* Open up a terminal window and execute `dmesg -Tw`. This is so you can see the USB enumeration happen in real time.
	* With the tablet plugged into the Linux machine, try holding volume up + power simultaneously for around 10-15 seconds.
	* If that doesn't work, it might help to unplug the internal battery connector. First, unplug the USB cable.
	* Follow the first few steps [here](https://www.ifixit.com/Guide/Nexus+7+Battery+Replacement/9895) to gently remove the rear cover and unplug the battery cable. If you're careful, this can be done without tools.
	* While pressing the volume up button, plug in the USB cable. It's a bit tricky with the cover removed - it should slightly "click" inwards. Using the edge of your fingernail can help. You may need to use more pressure than expected to create electrical contact. (Note: it can take several attempts to get the tablet to actually enumerate. If you continuously see USB errors in `dmesg`, maybe try sharper pressure on the volume up button.)
	* The tablet should then hopefully enumerate in APX mode. If it does, release the volume up button and **reconnect the battery connector** (leave the USB cable plugged in).

4. From APX mode, execute fusee-launcher using [uart\_payload\_n7.bin](/payload/uart_payload_n7.bin). Within the `tegra30_debrick` directory, run:

		    sudo ./fusee-launcher/fusee-launcher.py ./payload/uart_payload_n7.bin -P 7330

	<details><summary><i>Example terminal output:</i></summary>

		    matt@aquos:~/devel/ouya/tegra30_debrick$ sudo ./fusee-launcher/fusee-launcher.py ./payload/uart_payload_n7.bin -P 7330
		    2020-07-04 12:16:54,982 INFO:usb.core:find(): using backend "usb.backend.libusb1"
		    ​
		    Important note: on desktop Linux systems, we currently require an XHCI host controller.
		    A good way to ensure you're likely using an XHCI backend is to plug your
		    device into a blue 'USB 3' port.
		    ​
		    Identified a Linux system; setting up the appropriate backend.
		    intermezzo_size: 0x00000078
		    target_payload_size: 0x000005ee
		    Found a Tegra with Device ID: b'05163c81bc245d01'
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

	<details><summary><i>Example output from USB serial adapter connected to the Nexus 7's UART after successfully running <code>uart_payload_n7.bin</code>:</i></summary>

		    ----------------------------------------------------------------------------
		    APBDEV_PMC_RST_STATUS_0: 00000000
		    BIT_BootType: 00000002
		    overriding getSecurityMode function to always return 3 (production non-secure)...
		    writing PMC_SCRATCH0 to trigger RCM mode after soft reset...
		    jumping to 0xfff01004...
	</details>

	<details><summary><i>Example <code>dmesg</code> output after successfully running <code>uart_payload_n7.bin</code>. Note that the USB device will reset and reenumerate in APX mode:</i></summary>

		    [Sat Jul  4 19:17:25 2020] usb 2-3.4: USB disconnect, device number 20
		    [Sat Jul  4 19:17:25 2020] usb 2-3.4: new high-speed USB device number 21 using xhci_hcd
		    [Sat Jul  4 19:17:26 2020] usb 2-3.4: New USB device found, idVendor=0955, idProduct=7330, bcdDevice= 1.03
		    [Sat Jul  4 19:17:26 2020] usb 2-3.4: New USB device strings: Mfr=1, Product=2, SerialNumber=0
		    [Sat Jul  4 19:17:26 2020] usb 2-3.4: Product: APX
		    [Sat Jul  4 19:17:26 2020] usb 2-3.4: Manufacturer: NVIDIA Corp.
	</details>

5. (Optional) Take a backup of the tablet's BCT **with the unpatched version of nvflash** for safekeeping and potential later use (i.e. use your backup instead of ./bct/nexus_7_grouper_bct.bin in subsequent steps) (Note: APX/nvflash will become unresponsive after this completes successfully - you'll need to cycle power and repeat steps 1 through 4):

		    sudo ./utils/nvflash_v1.13.87205 --getbct --bct BCT_READBACK_N7.BIN --configfile ./utils/flash.cfg

	<details><summary><i>Example <code>nvflash</code> output showing success:</i></summary>

		    matt@aquos:~/devel/ouya/tegra30_debrick$ sudo ./utils/nvflash_v1.13.87205 --getbct --bct BCT_READBACK_N7.BIN --configfile ./utils/flash.cfg
		    Nvflash v1.13.87205 started
		    chip uid from BR is: 0x0000000000000000015d24bc813c1605
		    rcm version 0X30001
		    System Information:
		       chip name: unknown
		       chip id: 0x30 major: 1 minor: 3
		       chip sku: 0x83
		       chip uid: 0x0000000000000000015d24bc813c1605
		       macrovision: disabled
		       hdcp: enabled
		       jtag: disabled
		       sbk burned: true
		       dk burned: true
		       boot device: emmc
		       operating mode: 4
		       device config strap: 1
		       device config fuse: 17
		       sdram config strap: 0
		    ​
		    retrieving bct into: BCT_READBACK_N7.BIN
		    BCT_READBACK_N7.BIN received successfully
	</details>

	* If you see an error like `bootloader status: Bct file not found (code: 21) message:  flags: 1073893660`, and you're _certain_ that you used the **unpatched** nvflash, it is possible that your BCT is damaged/missing. You will need to use the BCT from this repo (`./bct/nexus_7_grouper_bct.bin`) and add `--sync` to the _EBT_ nvflash command in step 7.

	<details><summary><i>Example <code>nvflash</code> output showing missing/corrupt BCT</i></summary>

		    matt@aquos:~/devel/ouya/tegra30_debrick$ sudo ./utils/nvflash_v1.13.87205 --getbct --bct BCT_READBACK_N7.BIN --configfile ./utils/flash.cfg 
		    Nvflash v1.13.87205 started
		    chip uid from BR is: 0x0000000000000000015d24bc813c1605
		    rcm version 0X30001
		    System Information:
		       chip name: unknown
		       chip id: 0x30 major: 1 minor: 3
		       chip sku: 0x83
		       chip uid: 0x0000000000000000015d24bc813c1605
		       macrovision: disabled
		       hdcp: enabled
		       jtag: disabled
		       sbk burned: true
		       dk burned: true
		       boot device: emmc
		       operating mode: 4
		       device config strap: 1
		       device config fuse: 17
		       sdram config strap: 0

		    retrieving bct into: BCT_READBACK_N7.BIN
		    Failed sending command 2 NvError 1179650command failure: getbct failed (bad data)
		    bootloader status: Bct file not found (code: 21) message:  flags: 1073893660
	</details>

	* Use `bct_dump` to confirm that the BCT looks OK:

		    bct_dump BCT_READBACK_N7.BIN

	<details><summary><i>Example <code>bct_dump</code> output:</i></summary>

		    matt@aquos:~/devel/ouya/tegra30_debrick$ bct_dump BCT_READBACK_N7.BIN 
		    Version       = 0x00030001;
		    BlockSize     = 0x00004000;
		    PageSize      = 0x00000200;
		    PartitionSize = 0x02000000;
		    OdmData       = 0x40000000;
		    # Bootloader used       = 1;
		    # Bootloaders max       = 4;
		    # BCT size              = 6128;
		    # Hash size             = 16;
		    # Crypto offset         = 16;
		    # Crypto length         = 6112;
		    # Max BCT search blocks = 64;
		    #
		    # These values are set by cbootimage using the
		    # bootloader provided by the Bootloader=...
		    # configuration option.
		    #
		    # Bootloader[0].Version      = 0x00000001;
		    # Bootloader[0].Start block  = 224;
		    # Bootloader[0].Start page   = 0;
		    # Bootloader[0].Length       = 2150992;
		    # Bootloader[0].Load address = 0x80108000;
		    # Bootloader[0].Entry point  = 0x80108000;
		    # Bootloader[0].Attributes   = 0x00000004;
		    # Bootloader[0].Bl AES Hash  = b28ebc06accf2bcd877e444bc28d00c0;
		    # Bootloader[0].RsaPssSigBl:
		    ​
		    SDRAM[0].MemoryType                         = NvBootMemoryType_Ddr3;
		    SDRAM[0].PllMChargePumpSetupControl         = 0x00000008;
		    SDRAM[0].PllMLoopFilterSetupControl         = 0x00000000;
		    SDRAM[0].PllMInputDivider                   = 0x0000000c;
		    ...
		    SDRAM[1].McEmemArbMisc1                     = 0x78000000;
		    SDRAM[1].McEmemArbRing1Throttle             = 0x001f0000;
		    SDRAM[1].McEmemArbOverride                  = 0x00000080;
		    SDRAM[1].McEmemArbRsv                       = 0xff00ff00;
		    SDRAM[1].McClkenOverride                    = 0x00000000;
	</details>

6. Boot from APX to fastboot's _nv3pserver_ mode like so:

		    sudo ./utils/nvflash_v1.13.87205_miniloader_patched --setbct --bct ./bct/nexus_7_grouper_bct.bin --configfile ./utils/flash.cfg --bl ./bootloader/bootloader-grouper-4.23.img --go

	<details><summary><i>Example <code>nvflash</code> output:</i></summary>

		    matt@aquos:~/devel/ouya/tegra30_debrick$ sudo ./utils/nvflash_v1.13.87205_miniloader_patched --setbct --bct ./bct/nexus_7_grouper_bct.bin --configfile ./utils/flash.cfg --bl ./bootloader/bootloader-grouper-4.23.img --go
		    Nvflash v1.13.87205 started
		    chip uid from BR is: 0x0000000000000000015d24bc813c1605
		    rcm version 0X30001
		    System Information:
		       chip name: unknown
		       chip id: 0x30 major: 1 minor: 3
		       chip sku: 0x83
		       chip uid: 0x0000000000000000015d24bc813c1605
		       macrovision: disabled
		       hdcp: enabled
		       jtag: disabled
		       sbk burned: true
		       dk burned: true
		       boot device: emmc
		       operating mode: 3
		       device config strap: 1
		       device config fuse: 17
		       sdram config strap: 0
		    ​
		    sending file: ./bct/nexus_7_grouper_bct.bin
		    - 6128/6128 bytes sent
		    ./bct/nexus_7_grouper_bct.bin sent successfully
		    downloading bootloader -- load address: 0x80108000 entry point: 0x80108000
		    sending file: ./bootloader/bootloader-grouper-4.23.img
		    - 2150992/2150992 bytes sent
		    ./bootloader/bootloader-grouper-4.23.img sent successfully
		    waiting for bootloader to initialize
		    bootloader downloaded successfully
	</details>

	<details><summary><i>Example UART output (Note: this is from the patched _uart.img bootloader variant, not what is shown in the above command):</i></summary>

		    Starting Miniloader
		    Transferring control to Bootloader
		    hip Id: 0x30 (Handheld SOC) Major: 0x1 Minor: 0x3 SKU: 0x83
		    NVRM Initialized shmoo database
		    NVRM CLOCKS: PLLX0:      700000 Khz
		    NVRM CLOCKS: PLLM0:      667000 Khz
		    NVRM CLOCKS: PLLC0:      600000 Khz
		    NVRM CLOCKS: PLLP0:      408000 Khz
		    NVRM CLOCKS: PLLA0:      11289 Khz
		    NVRM CLOCKS: CPU:        700000 Khz
		    NVRM CLOCKS: AVP:        102000 Khz
		    NVRM CLOCKS: System Bus: 102000 Khz
		    NVRM CLOCKS: Memory Controller: 333500
		    NVRM CLOCKS: External Memory Controller: 667000
		    PMIC_detection  PINMUX_AUX_GMI_CS2_N_0 register=30
		    PMIC_detection  id_value =0 RegData=0
		    BoardInfo: 0x0f41:0x0a00:0x01:0x44:0x02
		    KaiPmuGetCapabilities(): The power rail 33 is not mapped properly
		    KaiPmuGetCapabilities(): The power rail 33 is not mapped properly
		    Max77663IsRailEnabled() Rail2 is using FPS1
		    Max77663IsRailEnabled() Rail9 is using FPS0
		    Max77663IsRailEnabled() Rail13 is using FPS1
		    Max77663IsRailEnabled() Rail13 is using FPS1
		    ADJUSTED CLOCKS:
		    MC clock is set to 333500 KHz
		    EMC clock is set to 667000 KHz (DDR clock is at 667000 KHz)
		    PLLX0 clock is set to 700000 KHz
		    PLLC0 clock is set to 600000 KHz
		    CPU clock is set to 700000 KHz
		    System and AVP clock is set to 102000 KHz
		    GraphicsHost clock is set to 163200 KHz
		    3D clock is set to 111166 KHz
		    2D clock is set to 111166 KHz
		    Epp clock is set to 111166 KHz
		    Mpe clock is set to 111166 KHz
		    Vde clock is set to 272000 KHz
		    Bootloader Start at:22222 ms
		    read_battery_register i2c_addr=aa reg=2c
		    NvOdmI2cStatus_Success
		    getbatterycapacity capacity=30 
		    Initializing Display
		    OdmPmuApGpioGetCapabilities(): The VddRail 37 is more than registered rails
		    OdmPmuApGpioGetVoltage(): The VddRail 37 is more than registered rails
		    OdmPmuApGpioGetVoltage(): The VddRail 37 is more than registered rails
		    OdmPmuApGpioSetVoltage(): The VddRail 37 is more than registered rails
		    OdmPmuApGpioSetVoltage(): The VddRail 37 is more than registered rails
		    OdmPmuApGpioSetVoltage(): The VddRail 37 is more than registered rails
		    OdmPmuApGpioSetVoltage(): The VddRail 37 is more than registered rails
		    Project value(0x0)
		    Project value(0x0)
		    Invalidate-only cache maint not supported in NvOs
		    in nvrm_clocks.c, NvRmPowerModuleClockConfig pclk, state->SourceClock=6, state->actual_freq=12000, state->Divider=1
		    Project value(0x0)
		    in nvrm_clocks.c, NvRmPowerModuleClockConfig pclk, state->SourceClock=0, state->actual_freq=408000, state->Divider=1
		    OdmPmuApGpioGetCapabilities(): The VddRail 37 is more than registered rails
		    OdmPmuApGpioSetVoltage(): The VddRail 37 is more than registered rails
		    OdmPmuApGpioSetVoltage(): The VddRail 37 is more than registered rails
		    Project value(0x0)
		    ASUS_charger_mode+
		    Project value(0x0)
		    asus: [smb347_hot_temp_setting] Hard Limit Hot Temperature set success !
		    Exit charger mode due to Nv3pServer is active. 
		    Show google logo
		    show logo at 22873ms
		    ​
		    [bootloader] (built on Mar 21 2013, 17:12:55)
		    Platform Pre Boot configuration...
		    read_battery_register i2c_addr=aa reg=2c
		    NvOdmI2cStatus_Success
		    getbatterycapacity capacity=30 
		    Entering NvFlash recovery mode / Nv3p Server
		    ​
		    ​
		    Chip Uid: 015d24bc813c1605
	</details>

	* If `nvflash` errors out with something along the lines of `bootloader failed NvError 0x0`, it is possible that the battery is not charged enough to continue. The tablet screen will clearly say "battery is too low". If that's the case, cycle power, let it charge, and revisit in an hour or two. The cutoff seems to be 29%.

	<details><summary><i>Example <code>nvflash</code> output if battery is too low (this error could happen for other reasons, too - but if your tablet has been sitting dead for a while, it's a likely culprit)</i></summary>

		    matt@aquos:~/devel/ouya/tegra30_debrick$ sudo ./utils/nvflash_v1.13.87205_miniloader_patched --setbct --bct ./bct/nexus_7_grouper_bct.bin --configfile ./utils/flash.cfg --bl ./bootloader/bootloader-grouper-4.23.img --go
		    Nvflash v1.13.87205 started
		    chip uid from BR is: 0x0000000000000000015d24bc813c1605
		    rcm version 0X30001
		    System Information:
		       chip name: unknown
		       chip id: 0x30 major: 1 minor: 3
		       chip sku: 0x83
		       chip uid: 0x0000000000000000015d24bc813c1605
		       macrovision: disabled
		       hdcp: enabled
		       jtag: disabled
		       sbk burned: true
		       dk burned: true
		       boot device: emmc
		       operating mode: 3
		       device config strap: 1
		       device config fuse: 17
		       sdram config strap: 0
		    ​
		    sending file: ./bct/nexus_7_grouper_bct.bin
		    - 6128/6128 bytes sent
		    ./bct/nexus_7_grouper_bct.bin sent successfully
		    downloading bootloader -- load address: 0x80108000 entry point: 0x80108000
		    sending file: ./bootloader/bootloader-grouper-4.23.img
		    - 2150992/2150992 bytes sent
		    ./bootloader/bootloader-grouper-4.23.img sent successfully
		    waiting for bootloader to initialize
		    usb read error (71): Protocol error
		    bootloader failed NvError 0x0
		    command failure: bootloader download failed 
	</details>


7. Use the currently running _nv3pserver_ mode to reflash the bootloader to eMMC (Note: if step 5 failed due to corrupt/missing BCT, add `--sync` to this command. It will re-write the BCT.):

		    sudo ./utils/nvflash_v1.13.87205_miniloader_patched --resume --download EBT bootloader/bootloader-grouper-4.23.img --configfile ./utils/flash.cfg

	<details><summary><i>Example <code>nvflash</code> output:</i></summary>

		    matt@aquos:~/devel/ouya/tegra30_debrick$ sudo ./utils/nvflash_v1.13.87205_miniloader_patched --resume --download EBT bootloader/bootloader-grouper-4.23.img --configfile ./utils/flash.cfg 
		    Nvflash v1.13.87205 started
		    [resume mode]
		    sending file: bootloader/bootloader-grouper-4.23.img
		    - 2150992/2150992 bytes sent
		    bootloader/bootloader-grouper-4.23.img sent successfully
	</detail>

	<details><summary><i>Example UART output (from patched _uart.img)</i></summary>

		    BytesPerSector = 4096
		    ​
		    Start Downloading EBT
		    ​
		    End Downloading EBT
		    ​
		    !!!!!device update success!!!!!
		    ​
		    SocCpuMaxKHz = 1000000
		    SocCpuMinKHz = 32
		    PLLX0 FreqKHz = 700000
		    Project value(0x0)
		    Checking for android ota recovery 
		    Key driver not found.. Booting OS
		    ​
		    Cold-booting Linux
		    ​
		    Platform Pre OS Boot configuration...
		    Project value(0x0)
		    Warning: console set to hsport (				secure world tracing won't work)
		    The proc BoardInfo: 0x0f41:0x0a00:0x01:0x44:0x02
		    Project value(0x0)
		    mping to kernel at:47975 ms
	</details>

	* If you had a valid kernel and system image on the device, the tablet may boot into it immediately after flashing EBT. However, this does not mean that the bootloader/BCT were successfully flashed.

	<details><summary><i>Example <code>dmesg</code> output if stock kernel automatically boots:</i></summary>

		    [Sat Jul  4 23:23:00 2020] usb 2-3.4: USB disconnect, device number 66
		    [Sat Jul  4 23:23:14 2020] usb 2-3.4: new high-speed USB device number 67 using xhci_hcd
		    [Sat Jul  4 23:23:14 2020] usb 2-3.4: New USB device found, idVendor=18d1, idProduct=4e41, bcdDevice=99.99
		    [Sat Jul  4 23:23:14 2020] usb 2-3.4: New USB device strings: Mfr=2, Product=3, SerialNumber=4
		    [Sat Jul  4 23:23:14 2020] usb 2-3.4: Product: Android
		    [Sat Jul  4 23:23:14 2020] usb 2-3.4: Manufacturer: Android
		    [Sat Jul  4 23:23:14 2020] usb 2-3.4: SerialNumber: 015d24bc813c1605
	</details>

	* At this point, it is necessary to perform a cold boot to verify the bootloader and BCT are properly flashed. This can be achieved by holding the power button for approximately 10 seconds. Eventually, you should see a Google logo and the tablet will continue booting into Android if kernel and system partitions are intact.

8. Enter fastboot mode by holding the volume down and power keys for approximately 10s. While holding the buttons, the screen should go blank, briefly flash the Google logo, and then go to the screen with the Android mascot (it also says 'Start' at the top). Release the buttons:

	<details><summary><i>Example <code>dmesg</code> output showing fastboot enumerating:</i></summary>

		    [Sat Jul  4 23:27:51 2020] usb 2-3.4: USB disconnect, device number 69
		    [Sat Jul  4 23:27:51 2020] usb 2-3.4: new high-speed USB device number 70 using xhci_hcd
		    [Sat Jul  4 23:27:51 2020] usb 2-3.4: New USB device found, idVendor=18d1, idProduct=4e40, bcdDevice= 0.00
		    [Sat Jul  4 23:27:51 2020] usb 2-3.4: New USB device strings: Mfr=1, Product=2, SerialNumber=3
		    [Sat Jul  4 23:27:51 2020] usb 2-3.4: Product: Android
		    [Sat Jul  4 23:27:51 2020] usb 2-3.4: Manufacturer: Google, Inc
		    [Sat Jul  4 23:27:51 2020] usb 2-3.4: SerialNumber: 015d24bc813c1605
	</details>

	* From here it is possible to completely restore the stock OS using a [factory .zip image](https://developers.google.com/android/images#nakasi). Run the following to confirm communication with the device:

		    sudo fastboot devices

	<details><summary><i>Example <code>fastboot</code> output:</i></summary>
 
		    matt@aquos:~/devel/ouya/tegra30_debrick$ sudo fastboot devices
		    015d24bc813c1605	fastboot
	</details>

## Ouya Debrick

Before attempting any of this, be sure that your system isn't recoverable via other means. This could be as simple as hooking up a USB cable and running `adb`, or possibly plugging in a USB keyboard and attempting to enter [recovery mode](https://web.archive.org/web/20150502163510/https://forums.ouya.tv/discussion/comment/11742/#Comment_11742).

### Prerequisites:

- Linux machine with:
	- free USB3 port (required for fusee-launcher) (_Intel chipsets may work more reliably here_)
	- (if 64-bit kernel/userland) 32-bit libraries installed (_for .deb-based distros_ `dpkg --add-architecture i386; apt update && apt install libc6:i386 libstdc++6:i386`)
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
