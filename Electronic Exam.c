/*
 * Electronic Exam.c
 *
 * Created: 1/31/2024 7:33:11 PM
 *  Author: VTE
 */ 

#define  F_CPU 8000000UL
#include <util/delay.h>
#include "keypad.h"
#include "LCD.h"
#include <avr/interrupt.h>
#include <stdio.h>
#include "std_macros.h"
#include <avr/io.h>

#define  EEPROM_STATUS_LOCATION 0x20
#define  EEPROM_PASSWORD_LOCATION1 0x21
#define  EEPROM_PASSWORD_LOCATION2 0x22
#define  EEPROM_PASSWORD_LOCATION3 0x23
#define  EEPROM_PASSWORD_LOCATION4 0x24
#define  EEPROM_PASSWORD_LOCATION5 0x25
#define  EEPROM_PASSWORD_LOCATION6 0x26
#define  EEPROM_PASSWORD_LOCATION7 0x27
#define  EEPROM_PASSWORD_LOCATION8 0x28
#define  EEPROM_PASSWORD_LOCATION9 0x29
#define  EEPROM_PASSWORD_LOCATION10 0x30

char counter=0,counter2=0,counter3=0,k=0,m;
char arr[10];

int main(void)
{
    char value=NOTPRESSED,value2=NOTPRESSED,value3=NOTPRESSED;
    char res=0,i;

    keypad_vInit();
    LCD_vInit();

    // Checking if EEPROM has been initialized
    if (EEPROM_read(EEPROM_STATUS_LOCATION)==NOTPRESSED)
    {
        LCD_vSend_string("set correct ans:");
        _delay_ms(3000);

        // Setting correct answers in EEPROM
        for (i=0;i<10;i++)
        {
            LCD_clearscreen();
            LCD_vSend_string("for Q");
            LCD_vSend_char(i+48);
            LCD_vSend_string(" : ");
            do
            {
                value=keypad_u8check_press();
            }while (value==NOTPRESSED);
            LCD_vSend_char(value);
            _delay_ms(200);
            EEPROM_write(EEPROM_PASSWORD_LOCATION1+i,value);
            do
            {
                value2=keypad_u8check_press();
            }while (value2==NOTPRESSED&&value2!='=');
            _delay_ms(200);
            value=NOTPRESSED,value2=NOTPRESSED;
        }
        EEPROM_write(EEPROM_STATUS_LOCATION,0x00);
    }

    // Initializing timer for countdown
    timer_CTC_init_interrupt();
    LCD_movecursor(2,12);
    LCD_vSend_string("10:00");
    
    // Main loop for entering answers
    while(1)
    {
        // Clearing answer array
        for (i=0; i<10; i++)
        {
            arr[i]==NOTPRESSED;
        }

        // Prompting user to enter answers
        LCD_movecursor(1,1);
        LCD_vSend_string("Enter your ans:");
        _delay_ms(1000);
        LCD_movecursor(1,1);
        LCD_vSend_string("                ");

        // Loop for entering answers for each question
        for (i=0;i<10;i++)
        {
            LCD_movecursor(1,1);
            LCD_vSend_string("for Q");
            LCD_vSend_char(i+48);
            LCD_vSend_string(" : ");
            LCD_movecursor(1,9);
            do
            {
                arr[i]=keypad_u8check_press();
            }while (arr[i]==NOTPRESSED);
            LCD_movecursor(1,10);
            LCD_vSend_char(arr[i]);
            _delay_ms(200);
            LCD_movecursor(1,11);
            do
            {
                value3=keypad_u8check_press();
            }while (value3==NOTPRESSED&&value3!='=');
            _delay_ms(200);
            LCD_movecursor(1,10);
            LCD_vSend_char(' ');
            value3=NOTPRESSED;
        }

        // Checking entered answers against correct answers
        for (i=0; i<10; i++)
        {
            if(EEPROM_read(EEPROM_PASSWORD_LOCATION1+i)==arr[i])
            {
                res++;
            }
        }

        // Displaying the result
        LCD_movecursor(1,1);
        LCD_vSend_string("your degree is");
        LCD_movecursor(2,1);
        if (res==10)
            LCD_vSend_string("10/10");
        else
        {
            LCD_vSend_char(res+48);
            LCD_vSend_string("/10");
        }
        LCD_movecursor(2,12);
        LCD_vSend_string("     ");

        // Disabling timer interrupt and exiting main loop
        CLR_BIT(TIMSK,OCIE1A);
        break;
    }
}

// Interrupt Service Routine for Timer1 Compare A vector
ISR(TIMER1_COMPA_vect)
{
    // Incrementing counters and updating LCD display
    counter++;
    counter3+=5;
    if (counter3==60)
        counter3=0;
    LCD_movecursor(2,15);
    LCD_vSend_char(((60-counter3)/10)+48);
    LCD_vSend_char(((60-counter3)%10)+48);

    // Updating timer display every 12th cycle
    if((counter%12==0||counter2==0))
    {
        counter2++;
        LCD_movecursor(2,12);
        LCD_vSend_char(((10-counter2)/10)+48);
        LCD_vSend_char(((10-counter2)%10)+48);
        LCD_vSend_char(':');
    }

    // Checking if time is over and exiting main function
    if (counter2>=11)
    {
        LCD_clearscreen();
        LCD_movecursor(1,5);
        LCD_vSend_string("time Over");
        exit(0);
        CLR_BIT(TIMSK,OCIE1A);
    }
}
