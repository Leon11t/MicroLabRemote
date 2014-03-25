/*
 * Buttons.h
 *
 * Created: 21.03.2014 23:33:37
 *  Author: Taras
 */ 
//IR Diode port definitions
#define IR_PORT 	PORTB
#define IR_PIN 		PINB
#define IR_DDR 		DDRB
#define IR_HF_Line 	(1<<0)							 //PB0-for freq
#define IR_Line 	(1<<1)							 //PB1-for delays


#ifndef BUTTONS_H_
#define BUTTONS_H_

void button_init()
{
	DDRB&=~(1<<2)&~(1<<3)&~(1<<4);					//Вхід на кнопку
	PORTB|=(1<<2)|(1<<3)|(1<<4);					//Підтяжка кнопки відсутня, зовнішня підтяжка
}

void IR_diod_init(void)
{
	IR_DDR |= (IR_Line | IR_HF_Line);				//IR_Diod init
	IR_PORT |= IR_Line;
}


#endif /* BUTTONS_H_ */