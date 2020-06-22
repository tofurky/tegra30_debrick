# tegra30_debrick

- [Disclaimer](#disclaimer)  
- [Thanks](#thanks)  
- [License](#license)  
- [Background](#background)  
- [Files](#files)  

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
without it. Finally, Pyre on the OUYA Saviors Discord kindly shipped me a (working) Kickstarter 
Ouya so that I could figure out how to debrick it.

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
- [fusee-launcher](https://github.com/jevinskie/fusee-launcher): @jevinskie's fork, added as a submodule  
- [nvflash\_v1.13.87205](/utils/nvflash_v1.13.87205): unpatched nvflash utility (from [here](https://github.com/AndroidRoot/androidroot.github.io/blob/master/download/nvflash-tools-linux.tar.bz2))  
- [nvflash\_v1.13.87205\_miniloader\_patched](/utils/nvflash_v1.13.87205_miniloader_patched): as above, with patched miniloader to override security fuse checks  
- [ipatch\_rcm\_sample.c](/payload/ipatch_rcm_sample.c): Tegra X1 Fusée Gelée payload to disable security fuse checks (by @ktemkin)  
- [uart\_payload.c](/payload/uart_payload.c): as above, but tailored to Tegra 3 (Nexus 7 and Ouya)  
_Nexus 7:_  
- [bootloader-grouper-4.23.img](/bootloader/bootloader-grouper-4.23.img): unpatched 2012 Nexus 7 WiFi "grouper" bootloader  
- [bootloader-grouper-4.23\_uart.img](/bootloader/bootloader-grouper-4.23_uart.img): as above, with UART output patched in  
- [uart\_print.c](/bootloader/uart_print.c): source of UART print function patched into bootloader-grouper-4.23\_uart.img  
- [nexus\_7\_grouper\_bct.bin](/bct/nexus_7_grouper_bct.bin): BCT dumped from "grouper" using nvflash  
- [n7\_uart.jpg](/image/n7_uart.jpg): annotated image of PCB showing UARTA pinout  
- [n7\_uart\_schematic.jpg](/image/n7_uart_schematic.jpg): excerpt of ME370T schematic showing debug header pinout  
- [n7\_uart\_context.jpg](/image/n7_uart_context.jpg): picture of opened tablet showing context of header location  
_Ouya:_  
- [ouya\_rev\_1.01\_2013-06-20.bin](/bootloader/ouya_rev_1.01_2013-06-20.bin): decrypted fastboot bootloader, dumped with nvflash  
- [ouya\_rev\_1.01\_2013-06-20\_sigcheck\_disabled.bin](/bootloader/ouya_rev_1.01_2013-06-20_sigcheck_disabled.bin): as above, but with signature check disabled allowing flashing of unsigned bootloader to eMMC  
- [ouya\_rev\_1.01\_bct.bin](/bct/ouya_rev_1.01_bct.bin): BCT dumped via nvflash and decrypted  
- [ouya\_apx\_glitch.jpg](/image/ouya_apx_glitch.jpg): annotated image of PCB showing how to force APX mode  
More detailed descriptions of the files can be found if you check their git history, for example [nvflash\_v1.13.87205\_miniloader\_patched](/commits/master/utils/nvflash_v1.13.87205_miniloader_patched's).
