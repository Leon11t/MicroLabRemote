#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "Timer.h"
#include "Ports_init.h"

#define DEV			 128							//Назва девайса по протоколу NEC
#define VOL_Plus	 136							//Команди
#define VOL_Minus    64
#define INPUT		 32

//Об'являєм функції
void TransmitCmnd (char, char);
void TransmitByte (char);
void FreqOn  (void);
void FreqOff (void);

int main(void)
{
		button_init();								//Ініт кнопок
		IR_diod_init();								//Ініт ІЧ-діода

		
    while(1)
    {
		
		
		if (PINB&(1<<2))							//Button VOL+
		{
			while(PINB&(1<<2)) _delay_ms(20);
			_delay_ms(20);
			TransmitCmnd(DEV, VOL_Plus);
			
			set_sleep_mode(SLEEP_MODE_PWR_DOWN);	//Вхів в режим сну
			sleep_enable();
			sleep_cpu();
			sleep_disable();			
			
		}
		if (PINB&(1<<3))							//Button VOL-
		{
			while(PINB&(1<<3)) _delay_ms(20);
			_delay_ms(20);
			TransmitCmnd(DEV, VOL_Minus);
			
			set_sleep_mode(SLEEP_MODE_PWR_DOWN);	//Вхів в режим сну
			sleep_enable();
			sleep_cpu();
			sleep_disable();			
			
		}
		if (PINB&(1<<4))							//Button INPUT
		{
			while(PINB&(1<<4)) _delay_ms(20);
			_delay_ms(20);
			TransmitCmnd(DEV, INPUT);
						
			set_sleep_mode(SLEEP_MODE_PWR_DOWN);	//Вхів в режим сну
			sleep_enable();
			sleep_cpu();
			sleep_disable();
		}
        
    }
}

void TransmitCmnd (char device, char command)
{

	//стартовый импульс
	IR_PORT &= ~IR_Line;							//сбросить прямой вывод
	FreqOn ();										//подать частоту 36000
	_delay_us (9000);								//ждем 9 мс
	
	IR_PORT |= IR_Line;								//установить прямой вывод
	FreqOff ();										//выключить частоту 36000
	_delay_us (4400);								//ждем 4,4 мс

	//передача команды
	TransmitByte (device);							//прямой адрес устройства
	TransmitByte (~device);							//инверсный адрес устройства
	TransmitByte (command);							//прямая команда
	TransmitByte (~command);						//инверсная команда

	//завершающий импульс
	IR_PORT &= ~IR_Line;							//сбросить прямой вывод
	FreqOn ();										//подать частоту 36000
	_delay_us (660);								//ждем 0,66 мс

	IR_PORT |= IR_Line;								//установить прямой вывод
	FreqOff ();										//выключить частоту 36000
	
}


//вывод байта по каналу IR
//byte - байт данных
void TransmitByte (char byte)
{
	for (char i=0; i<8; i++){

		IR_PORT &= ~IR_Line;						//сбросить прямой вывод
		FreqOn ();									//подать частоту 36000
		_delay_us (660);							//пауза 0,66 мс

		IR_PORT |= IR_Line;							//установить прямой вывод
		FreqOff ();									//выключить частоту 36000
		if (byte & 0x80)							//ждем
		_delay_us (1580);							//если бит = 1 то 1,58 мс
		else
		_delay_us (460);							//если бит = 0 то 0,46 мс
		
		byte <<= 1;									//сдвинуть биты
	}
}