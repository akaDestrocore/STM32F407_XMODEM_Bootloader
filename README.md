
# STM32F4 XMODEM Bootloader

Bootloader application for STM32F4 based systems that uses XMODEM 8-bit communication via UART.

Install latest **[Tera Term version](https://github.com/TeraTermProject/osdn-download/releases)** then launch Tera Term. Select **'Serial'** and select **COM 'Port'** that you are using for your UART communication. Then from the top panel go to **'Setup->Serial port...'** and change speed to the desired baud rate value. I am using **'115200'** in my project and if you are using the register only version of it you have to change **USART_BRR** register value to the desired baud rate value by your own. In abstract version you can use the handle structure for selecting the desired baud rate in **'USART_Config()'** function. After that press and hold **user button** of your board then do the same for the **reset button**. Tera Term terminal should ask for a binary file afterwards. Just go to **'File->Transfer->XMODEM->Send...'** and select the binary of the new version of your firmware. For the firmware linker and example **.c** file please check the **[/blinky binary](https://github.com/akaDestrocore/STM32F407_Bootloader_Remake/tree/main/blinky%20binary)** folder. After the update is over you will get a message telling that in the terminal. 
## Author

**[Mykola ABLAPOKHIN @akaDestrocore](https://github.com/akaDestrocore)**


## Screenshots

![App Screenshot](https://i.imgur.com/gORzPKc.png)


## Features

- There are two versions of the project. One is Abstract and the other one is register only. The difference between them is only the initialization functions that are register based and doesn't use any additional driver in register based version. There are no other differences than that.
- It is designed that way that user has to hold user button while resetting the device. After that bootloader will ask for a binary file of a firmware. Firmware version control is supported. There are blinky firmware example binaries of different versions included in the repo.
- Firmware is copied to device completely before booting. This is provided in order to avoid situations when there are errors while writing to flash memory is in progress. 


## Documentation

[STM32F40x FLASH Module Organization](https://www.st.com/resource/en/reference_manual/dm00031020-stm32f405-415-stm32f407-417-stm32f427-437-and-stm32f429-439-advanced-arm-based-32-bit-mcus-stmicroelectronics.pdf#page=75&zoom=100,165,121)

[Tera Term Binary Transfer Protocols](https://ttssh2.osdn.jp/manual/4/en/reference/sourcecode.html#xyzmodem)


## Test firmware uploads

For these tests I used exactly the same binaries that are in **[/blinky binary](https://github.com/akaDestrocore/STM32F407_Bootloader_Remake/tree/main/blinky%20binary)** folder.

First uploading **['ver2_orange_blinky.bin'](https://github.com/akaDestrocore/STM32F407_Bootloader_Remake/raw/main/blinky%20binary/ver2_orange_blinky.bin)** 

![Screenshot1](https://i.imgur.com/woTcT05.png)

![Screenshot2](https://i.imgur.com/RPyIy3A.png)



After the upload is over there is a message in the terminal:

![Screenshot3](https://i.imgur.com/IcTk2Q6.png)

After that I uploaded **['ver1_red_blinky.bin'](https://github.com/akaDestrocore/STM32F407_Bootloader_Remake/raw/main/blinky%20binary/ver1_red_blinky.bin)**. And as you can see it is not uploaded onto device because its version is lower then the **['ver2_orange_blinky.bin'](https://github.com/akaDestrocore/STM32F407_Bootloader_Remake/raw/main/blinky%20binary/ver2_orange_blinky.bin)**'s:

![Screenshot4](https://i.imgur.com/5WAR5UD.png)

After that I uploaded a newer version of firmware -  **['ver3_blue_blinky.bin'](https://github.com/akaDestrocore/STM32F407_Bootloader_Remake/raw/main/blinky%20binary/ver3_blue_blinky.bin)**.

![Screenshit5](https://i.imgur.com/rkOoAmW.png)
