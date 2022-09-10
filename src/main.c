/**
 ******************************************************************************
 *  FILE          : main.c
 *  PROJECT       : Labor Datentechnik - final project
 *  				ADC Temp.Measurment & USART Serial Communication
 *  FIRST VERSION : 2020-12-04
 *  DESCRIPTION   :	The User can choose the serial writing mode
 *  				(received text is shown on LCD) by pressing the DOWN-Button.
 *  				The internal temperature can be shown by pressing
 *  				the UP-Button.
 *  				Based on the temperature measurements from the internal
 *  				temperature sensor - STM will print temperature on HD44780
 *  				display and glow RGB LED according to a table below and:
 *  				-15	C to 5.0 C    	blue
 *  				5.1	C to 15 C   	yellow
 *  				15.1 C to 25 C 		orange
 *  				Above 25.1 C   		red
 *  				Furthermore the STM sends the temperature measurements
 *  				serial via USART.
 ******************************************************************************/
/*------------------------------------Includes------------------------------*/
#include <stm32l0xx.h>
#include <lcd_shield.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <delay.h>
#include <math.h>
/*------------------------------------Defines-------------------------------*/
// Temperature sensor calibration address
#define TEMP130_CAL_ADDR ((uint16_t*) ((uint32_t) 0x1FF8007E))
#define TEMP30_CAL_ADDR ((uint16_t*) ((uint32_t) 0x1FF8007A))
#define VDD_CALIB ((uint16_t) (300))
#define VDD_APPLI ((uint16_t) (330))
// LCD
#define LCD_SCREEN_SIZE 32
// Ring Buffer
#define BUFFER_SIZE 256
/*------------------------------------Variables------------------------------*/
// Temperature
static volatile int32_t temperature_in_C;
static volatile uint32_t temperature_ADC_value;
static char temperature_String[LCD_SCREEN_SIZE];
// Ring Buffer
static char ring_buffer[BUFFER_SIZE];
static volatile uint32_t write_pointer = 0;
static uint32_t read_pointer = 0;
//Button Control
static uint8_t button_state;
static uint8_t previous_button_state;
static int mode;/*0->TEMP,1->Serial Writing*/
/*-----------------------------------Prototypes------------------------------*/
static void usart_setup(void);
static void ADC_Init(void);
static void TIM6_Init(void);
void ADC1_COMP_IRQHandler(void);
void ADC1_COMP_IRQHandler(void);
int32_t ComputeTemperature(uint32_t);
void TemperatureToString(int32_t);
void LoadTemperatureToRingBuffer(void);
void PrintRingBufferOnLCD(void);
void SendTemperatureSerial(void);
void LEDColor(void);
void ButtonControl(void);
/*------------------------------------Functions------------------------------*/
static void usart_setup(void) {
	// Enable peripheral clock for port A
	RCC->IOPENR |= RCC_IOPENR_IOPAEN;

	// Set alternate functions for PA2 and PA3 to AF4 (USART2)
	GPIOA->AFR[0] = (GPIOA->AFR[0]
			& ~(GPIO_AFRL_AFSEL2_Msk | GPIO_AFRL_AFSEL3_Msk))
			| ((4 << GPIO_AFRL_AFSEL2_Pos) & GPIO_AFRL_AFSEL2_Msk)
			| ((4 << GPIO_AFRL_AFSEL3_Pos) & GPIO_AFRL_AFSEL3_Msk);

	// Set GPIO mode for PA2 and PA3 to alternate function (mode 2)
	GPIOA->MODER = (GPIOA->MODER
			& ~(GPIO_MODER_MODE2_Msk | GPIO_MODER_MODE3_Msk))
			| ((2 << GPIO_MODER_MODE2_Pos) & GPIO_MODER_MODE2_Msk)
			| ((2 << GPIO_MODER_MODE3_Pos) & GPIO_MODER_MODE3_Msk);

	RCC->APB1ENR |= RCC_APB1ENR_USART2EN; // enable peripheral clock for USART2

	// Set baudrate to 115200
	USART2->BRR = 16000000 / 115200;
	// enable USART2 receiver, transmitter, RX interrupt and USART
	USART2->CR1 |= (USART_CR1_RE | USART_CR1_TE | USART_CR1_RXNEIE
			| USART_CR1_UE);
	// Configure NVIC for USART
	NVIC_EnableIRQ(USART2_IRQn);
}

static void ADC_Init(void) {
	// APB2-Bus Clock Configuration
	RCC->APB2ENR |= RCC_APB2ENR_ADCEN;

	// Select CLK Mode
	ADC1->CFGR2 = ADC_CFGR2_CKMODE & ~(ADC_CFGR2_CKMODE_0 | ADC_CFGR2_CKMODE_1);
	// ADC Calibration
	if ((ADC1->CR & ADC_CR_ADEN) != 0) {
		ADC1->CR |= ADC_CR_ADDIS;
	}
	ADC1->CR |= ADC_CR_ADCAL;
	while ((ADC1->ISR & ADC_ISR_EOCAL) == 0) {
		lcd_print_string("Calibration...");
		delay_ms(500);
		lcd_clear_display();
	}
	ADC1->ISR |= ADC_ISR_EOCAL;

	// Enable the ADC
	ADC1->ISR |= ADC_ISR_ADRDY;
	ADC1->CR |= ADC_CR_ADEN;
	while ((ADC1->ISR & ADC_ISR_ADRDY) == 0) {
		lcd_print_string("Enable ADC...");
		delay_ms(500);
		lcd_clear_display();
	}

	//16x Oversampler rate, 4Bit Shift
	ADC1->CFGR2 = (ADC1->CFGR2 & (~ADC_CFGR2_CKMODE))
			| (ADC_CFGR2_OVSE | ADC_CFGR2_OVSR_1 | ADC_CFGR2_OVSR_0
					| ADC_CFGR2_OVSS_2);

	// Configuration of the temperature sensor
	const uint32_t TIM6_TRGO = 0x00000000; /*External Event TIM6 TRGO [000]*/
	ADC1->CFGR1 &= ~(ADC_CFGR1_EXTSEL); /*remove all selections*/
	ADC1->CFGR1 |= ADC_CFGR1_EXTEN_1 | TIM6_TRGO;/*Hardware trigger detection on the rising edge, TIM6_TRGO (TIM6 as trigger by keeping 000 in TSEL1)*/

	ADC1->CHSELR = ADC_CHSELR_CHSEL18;/*Temp. Sensor*/
	ADC1->SMPR |= ADC_SMPR_SMP_0 | ADC_SMPR_SMP_1 | ADC_SMPR_SMP_2; /*sample rate */
	ADC->CCR |= ADC_CCR_TSEN;/*enable Temp. sensor*/
	ADC->CCR |= ADC_CCR_VREFEN;/*enable Vref*/

	// Configure NVIC for ADC
	ADC1->IER = ADC_IER_EOCIE;
	NVIC_EnableIRQ(ADC1_COMP_IRQn);
}

static void TIM6_Init(void) {
	// Configuration of the Basic Timer 6
	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;/*CK_INT, fclk = 16MHz*/
	TIM6->CR1 = TIM_CR1_CEN; /*Enable the counter*/
	TIM6->CR2 = TIM_CR2_MMS_1; /*master mode [010]: update, The update event is selected as a trigger output (TRGO*/
	TIM6->PSC = 0xFFFF; /* set max prescaler, PSC = 65536-1*/
	TIM6->ARR = 0x4C4; /*set period in Auto Reload Register, ARR=1220*/
}

void USART2_IRQHandler(void) {
	uint8_t received = USART2->RDR & USART_RDR_RDR_Msk;

	if (mode == 1) {
		mode = 1;
		ring_buffer[write_pointer++] = received;
		if (write_pointer >= BUFFER_SIZE) {
			write_pointer = 0;
		}
	}
}

void ADC1_COMP_IRQHandler(void) {
	if ((ADC1->ISR & ADC_ISR_EOC) != 0) {
		ADC1->ISR |= ADC_ISR_EOC; /*Reset EOC flag*/
	}
	temperature_ADC_value = ADC1->DR;
	temperature_in_C = ComputeTemperature(temperature_ADC_value);
	TemperatureToString(temperature_in_C);
	LoadTemperatureToRingBuffer();
	SendTemperatureSerial();
}

int32_t ComputeTemperature(uint32_t measure) {
	// Computes Temperature with received ADC value
	int32_t temperature;
	temperature = ((measure * VDD_APPLI / VDD_CALIB)
			- (int32_t) *TEMP30_CAL_ADDR);
	temperature = temperature * (int32_t) (130 - 30);
	temperature = temperature
			/ (int32_t) (*TEMP130_CAL_ADDR - *TEMP30_CAL_ADDR);
	temperature = temperature + 30;
	return (temperature);
}

void TemperatureToString(int32_t temp_in_C) {
	//Convert numeric Temperature value to String
	int num = temp_in_C;
	int digits = (log10(num) + 1) + 2;
	char snum[digits];
	__itoa(num, snum, 10);
	//Concatenate Strings by adding "°C" to numeric Temperature value
	snum[sizeof(snum) - 2] = (char) 0xDF;/*°*/
	snum[sizeof(snum) - 1] = (char) 0x43;/*C*/
	char temp[] = " TEMP:";
	strcpy(temperature_String, temp);
	for (int i = 0; i < strlen(temp) - 2; i++) {
		temperature_String[strlen(temp) + i] = snum[i];
	}
}

void LoadTemperatureToRingBuffer(void) {
	//load String into Ring Buffer
	for (int i = 0; i < strlen(temperature_String); i++) {
		ring_buffer[write_pointer++] = temperature_String[i];
		if (write_pointer >= BUFFER_SIZE) {
			write_pointer = 0;
		}
	}
}

void PrintRingBufferOnLCD(void) {
	//Display Temperature on LCD
	if (read_pointer != write_pointer) {
		char c = ring_buffer[read_pointer++];
		lcd_print_char(c);
		if (read_pointer >= BUFFER_SIZE) {
			read_pointer = 0;
		}
		if (mode == 0) {
			LEDColor(); /*Change LCD Color if Buffer is loaded*/
		}
	} else {
		lcd_set_cursor(0, 0);
	}
}

void SendTemperatureSerial(void) {
	//Send Temperature Serial via USART2
	int wr_ptr = write_pointer; /*save Ring-Buffer pointer to keep content existing for DisplayTemperatureOnLCD() */
	int rd_ptr = read_pointer;
	while (rd_ptr != wr_ptr) {
		char c = ring_buffer[rd_ptr++];
		if (c == 0xDF) /*replace '°' with blank space*/
		{
			c = 0x20;
		}
		if (c != 0x20) { /*necessary for serial output*/
			USART2->TDR = c;
		}
		while ((USART2->ISR & USART_ISR_TC) == 0) { /*idle while ongoing transmission*/
		}
		if (rd_ptr >= BUFFER_SIZE) {
			rd_ptr = 0;
		}
	}
	USART2->TDR = 0x20;/*Separate output with blank space*/
}

void LEDColor(void) {
	//Glow LED for a desire RGB color according to temperature
	if (temperature_in_C >= -15 && temperature_in_C <= 5) {
		lcd_set_backlight_color(BG_BLUE);
	} else if (temperature_in_C > 5 && temperature_in_C <= 15) {
		lcd_set_backlight_color(BG_GREEN);
	} else if (temperature_in_C > 15 && temperature_in_C <= 25) {
		lcd_set_backlight_color(BG_YELLOW);
	} else if (temperature_in_C > 25) {
		lcd_set_backlight_color(BG_RED);
	} else {
		lcd_set_backlight_color(BG_OFF);
	}
}

void ButtonControl(void) {
	//Change between Serial Writing and ADC temperature measurement mode using LCD Buttons
	button_state = lcd_read_buttons();
	if (button_state != previous_button_state) {
		//User information Menu
		if (button_state & BUTTON_SELECT) {
			lcd_set_backlight_color(BG_OFF);
			lcd_print_string("UP: Temp.Meas.  " "DOWN:Ser.Writing");
			delay_ms(1500);/*hold usage information for 1.5s*/
		}
		//Mode 0: Button up; Temp. Measurement
		else if (button_state & BUTTON_UP) {
			mode = 0;
			lcd_clear_display();
			lcd_print_string("Measuring  the  " "  Temperature!  ");
			//Check if CR register is 0x0 to ensure that re-initialization is possible
			if ((ADC1->CR
					& (ADC_CR_ADEN | ADC_CR_ADCAL | ADC_CR_ADSTART
							| ADC_CR_ADDIS)) == 0) {
				ADC_Init(); /*re-initialization */
				ADC1->CR |= ADC_CR_ADSTART;
			}
		}
		//Mode 1: Button down; Stop ADC-Temp.Measurmet and start serial writing
		else if (button_state & BUTTON_DOWN) {
			mode = 1;
			lcd_clear_display();
			write_pointer = read_pointer;/*Reset Ring buffer pointers to avoid outputs*/
			lcd_set_backlight_color(BG_OFF);
			lcd_print_string("Serial Writing  " "enabled!");
			ADC1->CR |= ADC_CR_ADSTP;/*stop ongoing conversation*/
			ADC1->ISR |= ADC_ISR_EOC;/*Reset EOC flag*/
			if ((ADC1->CR & ADC_CR_ADSTART) == 0) {
				ADC1->CR |= ADC_CR_ADDIS;/*ADC disable*/
			}
		}
	}
	previous_button_state = button_state;
}

/*------------------------------------Main---------------------------------*/
int main(void) {
	//Initialization of LCD
	lcd_init();
	//Welcome screen
	lcd_print_string("Temp.Measurement" "& Serial Writing");
	delay_ms(1500);
	lcd_clear_display();
	lcd_print_string("Press SELECT    " "to change mode  ");
	delay_ms(1500);
	lcd_clear_display();
	lcd_print_string("Press UP for    " "Temp.Measurement");
	delay_ms(1500);
	lcd_clear_display();
	lcd_print_string("Press DOWN for  " "Serial Writing  ");
	delay_ms(1500);
	lcd_clear_display();
	lcd_print_string("Temp.Measurement" "set as default  ");
	delay_ms(1500);
	lcd_clear_display();
	//Initialization of USART, ADC and TIM6
	usart_setup();
	ADC_Init();
	TIM6_Init();
	//ADC start of conversion
	ADC1->CR |= ADC_CR_ADSTART;
	while (1) {
		ButtonControl();
		PrintRingBufferOnLCD();
	}
}
