# STM3210TemperatureMeasurment
STM3210 ADC Temperature Measurment

## Idea:

## Concept:

## Result: 

STM32L0x3_ADCTEMP_LCD_USART
The internal ADC temperature can be shown by pressing the UP-Button. <br>
Based on the temperature measurements from the STM will print the recalculated temperature in Celsius on the HD44780 display and glows the displays <br> RGB LEDs according to the following list: <br>
<ul>
<li>-15.0 C° to +5.0  C° -> blue </li>
<li> +5.1 C° to +15.0 C° -> yellow </li>
<li> +5.1 C° to +25.0 C° -> orange </li>
<li>      Above +25.1 C° -> red </li>
</ul>
Furthermore the STM sends the measured temperature via serial communication (USART) back to PC (USB2.0-Port). 
By pressing the DOWN-BTemperature_Measurment_STM3210utton the measurment stops and ASCII Input can be recieved via USART which will displayed on the LCD.

## Project Status: 
Done.

## Built with:
C

## Author:
Kian David Warias
