# STM3210TemperatureMeasurment
The temperature is read out every 5 seconds via the ACD1 bus using the Basic Timer 6 and returned serially via the USART interface and displayed on the LCD (HD-4780). Depending on the temperature, the backlight of the LCD is adjusted. The user can choose between two modes by pressing the LCD buttons. 
- Mode 0, Button UP: Temperature measurement with output on the LCD and via the serial interface.
- Mode 1, Button DOWN: Text received serially and output on LCD
By default the temperature measurement is preselected. By pressing the SELECT button the user gets an overview of the available modes for 1.5s. If the user does not change to another mode, the overview disappears and the current mode is used again.
## Idea:

## Concept:

Figure 1 shows the structure of the program. The compiler starts at the blue highlighted starting point. First, the header files are included, the necessary constants are defined, and the global variables and function prototypes are declared. In the last two steps the functions and handlers of the interrupt service routines to be used are initialized and the main is started. The 12 functions can be divided into five groups for the explanation in section 2.

## Result: 

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



## Project Status: 
Done.

## Built with:
C

## Author:
Kian David Warias
