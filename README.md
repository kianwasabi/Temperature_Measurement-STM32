# Temperature Measurment with STM32L0x3:

STM32L0x3_ADCTEMP_LCD_USART
The internal ADC temperature can be shown by pressing the UP-Button. <br>
Based on the temperature measurements from the STM will print the recalculated temperature in Celsius on the HD44780 display. <br>
Futhermore, the display changes the background-color according to the following list: <br>
<ul>
<li>-15.0 C° to +5.0  C° -> blue </li>
<li> +5.1 C° to +15.0 C° -> yellow </li>
<li> +5.1 C° to +25.0 C° -> orange </li>
<li>      Above +25.1 C° -> red </li>
</ul>
Moreover, the STM sends the measured temperature via serial communication (USART) back to PC (USB2.0-Port). <br>
By pressing the DOWN-Button the measurment stops and an ASCII Input can be recieved via USART which will displayed on the LCD.
<br>

## Idea:
The temperature is read every 5 seconds via the ACD1 bus using the Basic Timer 6 and returned serially via the USART interface and displayed on the LCD (HD-4780). Depending on the temperature, the backlight of the LCD is adjusted. The user can choose between two modes by pressing the LCD buttons. 
- Mode 0, Button UP: Temperature measurement with output on the LCD and via the serial interface.
- Mode 1, Button DOWN: Text received serially and output on LCD
By default the temperature measurement is preselected. By pressing the SELECT button the user gets an overview of the available modes for 1.5s. If the user does not change to another mode, the overview disappears and the current mode is used again.
## Concept:
Figure 1 shows the structure of the program. The compiler starts at the blue highlighted starting point. First, the header files are included, the necessary constants are defined, and the global variables and function prototypes are declared. In the last two steps the functions and handlers of the interrupt service routines to be used are initialized and the main is started. 

<a href="url"><img src="https://user-images.githubusercontent.com/55065075/221699834-97b11384-a385-4771-977c-ddc862263803.png" height="auto" width="600" ></a>
<br>
This is followed by an output on the LCD relevant to the user on how to operate the program. This is as follows: 
1. temp. measurement & serial writing<br>
2. Press SELECT to change mode<br>
3. press UP for Temp. measurement<br>
4. press DOWN for Serial Writing<br>
5. temp. measurement set as default
<br></br>

Temperature measurement via ADC starts by setting AD_START bit in the ADC Control Register. After starting the measurement, the infinite while loop starts, where two modes can be selected by pressing the UP, DOWN and SELECT buttons of the HD44780. By default, the temperature measurement mode is selected. In this mode, the measured value is first output serially to the programming device when the ADC ISR is called, and is also displayed on the LCD. The color of the backlight of the LCD is adapted to the measured temperature only in this mode. By pressing the SELECT button, the user is shown the mode selection again. Pressing the DOWN button interrupts the current temperature measurement (ADSTP bit in the ADC Control Register is set) and serial reception of text input is activated. The received text is displayed on the LCD.

<a href="url"><img src="https://user-images.githubusercontent.com/55065075/221699990-f3acc159-2d07-4fc9-8c60-2b1e8ecb08f6.png" height="auto" width="400" ></a>

## Result: 

<img width="689" alt="Screenshot 2023-02-27 at 23 14 46" src="https://user-images.githubusercontent.com/55065075/221792761-ace1eadf-ebc5-4459-9b89-097c171d75db.png">

<img width="673" alt="Screenshot 2023-02-27 at 23 15 22" src="https://user-images.githubusercontent.com/55065075/221792786-114759cc-addb-4c61-b330-17f5bfa60236.png">

<img width="691" alt="Screenshot 2023-02-27 at 23 17 05" src="https://user-images.githubusercontent.com/55065075/221792930-10396815-2ca2-4214-a7ac-43e28b230bea.png">

<img width="695" alt="Screenshot 2023-02-27 at 23 16 00" src="https://user-images.githubusercontent.com/55065075/221793057-7451cd50-56cf-42e1-9643-832a9ae1fab0.png">

<img width="709" alt="Screenshot 2023-02-27 at 23 15 50" src="https://user-images.githubusercontent.com/55065075/221793091-3d1fbc49-c171-4874-a7fe-a051629c163a.png">


## Project Status: 
Done.

## Built with:
C & Assembly

## Author:
Kian David Warias
