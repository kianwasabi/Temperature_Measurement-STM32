# STM3210TemperatureMeasurment
STM3210 ADC Temperature Measurment

## Idea:

## Concept:

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
