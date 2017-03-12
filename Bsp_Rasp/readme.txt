QNX Neutrino 6.5.0 sp1  BSP for Raspberry Pi b+ (bcm2835)

what is working 
1.Kernel Up & fully functioning 
2.Serial Debug 
3.Graphics 
4.Some Extent Touch screen ( Unfortunatly Photon is no longer supported in ARM architecture )

Features Yet to be added 
1.SPI
2.I2C
3.SD Card 
4.USB 
5.Network  

code was derived from on at91sam9xx source , so I left the original code for  SPI /I2C/USB/Input as it is ,
hoping that it will be helpfull for the developers ..

I am working on QT Graphics support ..


Compiling 
1.Unzip it 
2.File ->Import -> General -> Existing projects into workspace  --> File System ( select the folder)
3.Build it 

QNX OS- Running:

1.Copy the files of working_image to the SD Card.. Connect USB to Serial /TTL adapter . power ON
2.Compiled image will be here BSP_Rasp/images  

commands:
refer .build file .. ls,uname,sloginfo,kill,date

apps:
1.Led-red
2.Led-green
3.egl-gears-lite
4.gf_splash /usr/share/backdrops/1024x768/neutrino_02_1024x768.jpg

photon apps (enable Photon ,io-graphics, phfont) 
pls note : QNX stopped supporting photon support on ARM platform..
1.devc-pty  
2.bkgdmgr
3.pwm 
4.shelf
5.pterm
6.mypro
7.gui-app
8.pfm
9.pv
10.phsutdown
11.phcalc



Licensing  ( Refer license.txt)
1.Source code is distributed under Apache License.
2.Raspberry QNX Image file (ifs-bcm2835.bin) is provided strickly for Acadamic use.. 
  Please Note : Final QNX Image distribution requires run time license for each systems..
3.For any commercial use or distibution of any other purpose , please contact QNX licensing@qnx.com



  

