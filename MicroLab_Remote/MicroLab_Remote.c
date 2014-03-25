#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "Timer.h"
#include "Ports_init.h"

#define DEV			 128							//����� ������� �� ��������� NEC
#define VOL_Plus	 136							//�������
#define VOL_Minus    64
#define INPUT		 32

//��'������ �������
void TransmitCmnd (char, char);
void TransmitByte (char);
void FreqOn  (void);
void FreqOff (void);

int main(void)
{
		button_init();								//��� ������
		IR_diod_init();								//��� ��-����

		
    while(1)
    {
		
		
		if (PINB&(1<<2))							//Button VOL+
		{
			while(PINB&(1<<2)) _delay_ms(20);
			_delay_ms(20);
			TransmitCmnd(DEV, VOL_Plus);
			
			set_sleep_mode(SLEEP_MODE_PWR_DOWN);	//���� � ����� ���
			sleep_enable();
			sleep_cpu();
			sleep_disable();			
			
		}
		if (PINB&(1<<3))							//Button VOL-
		{
			while(PINB&(1<<3)) _delay_ms(20);
			_delay_ms(20);
			TransmitCmnd(DEV, VOL_Minus);
			
			set_sleep_mode(SLEEP_MODE_PWR_DOWN);	//���� � ����� ���
			sleep_enable();
			sleep_cpu();
			sleep_disable();			
			
		}
		if (PINB&(1<<4))							//Button INPUT
		{
			while(PINB&(1<<4)) _delay_ms(20);
			_delay_ms(20);
			TransmitCmnd(DEV, INPUT);
						
			set_sleep_mode(SLEEP_MODE_PWR_DOWN);	//���� � ����� ���
			sleep_enable();
			sleep_cpu();
			sleep_disable();
		}
        
    }
}

void TransmitCmnd (char device, char command)
{

	//��������� �������
	IR_PORT &= ~IR_Line;							//�������� ������ �����
	FreqOn ();										//������ ������� 36000
	_delay_us (9000);								//���� 9 ��
	
	IR_PORT |= IR_Line;								//���������� ������ �����
	FreqOff ();										//��������� ������� 36000
	_delay_us (4400);								//���� 4,4 ��

	//�������� �������
	TransmitByte (device);							//������ ����� ����������
	TransmitByte (~device);							//��������� ����� ����������
	TransmitByte (command);							//������ �������
	TransmitByte (~command);						//��������� �������

	//����������� �������
	IR_PORT &= ~IR_Line;							//�������� ������ �����
	FreqOn ();										//������ ������� 36000
	_delay_us (660);								//���� 0,66 ��

	IR_PORT |= IR_Line;								//���������� ������ �����
	FreqOff ();										//��������� ������� 36000
	
}


//����� ����� �� ������ IR
//byte - ���� ������
void TransmitByte (char byte)
{
	for (char i=0; i<8; i++){

		IR_PORT &= ~IR_Line;						//�������� ������ �����
		FreqOn ();									//������ ������� 36000
		_delay_us (660);							//����� 0,66 ��

		IR_PORT |= IR_Line;							//���������� ������ �����
		FreqOff ();									//��������� ������� 36000
		if (byte & 0x80)							//����
		_delay_us (1580);							//���� ��� = 1 �� 1,58 ��
		else
		_delay_us (460);							//���� ��� = 0 �� 0,46 ��
		
		byte <<= 1;									//�������� ����
	}
}