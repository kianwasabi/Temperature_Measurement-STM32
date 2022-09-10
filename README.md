# STM3210TemperatureMeasurment
STM3210 ADC Temperature Measurment

STM32L0x3_ADCTEMP_LCD_USART

The internal ADC temperature can be shown by pressing the UP-Button. Based on the temperature measurements from the STM will print the recalculated temperature in Celsius on the HD44780 display and glows the displays RGB LEDs according to the following list: (-15 C° to 5.0 C° blue; 5.1 C° to 15 C° yellow; 15.1 C° to 25 C° orange; Above 25.1 C° red). Furthermore the STM sends the measured temperature via serial communication (USART) back to PC (USB2.0-Port). By pressing the DOWN-Button the measurment stops and ASCII Input can be recieved via USART which will displayed on the LCD.
