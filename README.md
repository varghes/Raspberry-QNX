# Raspberry-QNX
Raspberry Pi -QNX  ( BCM2835 )
QNX Neutrino 6.5.0 sp1 BSP for Raspberry Pi b+ & Raspberry Pi Zero W(bcm2835) 

## QNX Porting
* 1.Kernel Up & fully functioning 
* 2.Serial Debug 
* 3.Graphics
* 4.Some Extent SPI Touch screen 

Features to be added 1.MMC 2.Ethernet/Wifi

## Compiling 
* 1.Unzip it 
* 2.Create a Empty Project  
* 3.File ->Import -> General -> Existing projects into workspace --> File System ( select the folder) 
* 4.Build it

## QNX OS- Booting:

* 1.Copy the files of "working_image" to the SD Card.. 
Connect USB to Serial /TTL adapter . power ON 
* 2.Compiled image will be here BSP_Rasp/images

## Raspberry Pi Zero W 
* 1.Use the latest Firmware  https://github.com/raspberrypi/firmware/tree/master/boot
* 2.Add the below lines at the end of  "Config.txt" File 

    enable_uart=1
    dtoverlay=pi3-miniuart-bt
    init_uart_clock=3000000
	
* 3.Copy these files from Working_image to SD Card

   kernel.img
   boot.scr
   ifs-bcm2835.bin
   
![Image boot](https://github.com/varghes/Raspberry-QNX/blob/master/Screenshot/boot.jpg)

## Commands: 
   refer .build file .. ls,uname,sloginfo,kill,date

## Apps: 
* 1.Led-red 
* 2.Led-green 
* 3.egl-gears-lite 
* 4.gf_splash /usr/share/backdrops/1024x768/neutrino_02_1024x768.jpg

## photon apps 
Enable Photon ,io-graphics, phfont
pls note : QNX stopped supporting photon support on ARM platform..
* 1.devc-pty 
* 2.bkgdmgr 
* 3.pwm 
* 4.shelf 
* 5.pterm 
* 6.mypro 
* 7.gui-app 
* 8.pfm 
* 9.pv 
* 10.phsutdown 
* 11.phcalc
![Image Main](https://github.com/varghes/Raspberry-QNX/blob/master/Screenshot/main.jpg){:height="50%" width="50%"}
![Image calc](https://github.com/varghes/Raspberry-QNX/blob/master/Screenshot/calculator.jpg){:height="50%" width="50%"}

Licensing ( Refer license.txt) 
Source code is distributed under Apache License. 
Please Note : Final QNX Image distribution requires run time license for each systems.. contact QNX 
