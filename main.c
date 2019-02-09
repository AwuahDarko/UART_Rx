/*
 * File:   main.c
 * Author: Yaw Awuah Darko
 *
 * Created on December 15, 2018, 8:49 PM
 */


#include "m_UART_rx.h"

void LCD_Init();
void LCD_Clear();
void LCD_Command(char );
void LCD_Char(char x);
void LCD_String(const char *);
void LCD_String_xy(char ,char ,const char*);
void MSdelay(unsigned int); 
void initUART_rx(void);
void FOSC(void);
void readBuffer(void);
void screenFormat(void);
void reset(void);

uint8_t UART_Buffer[100];

short pos = -1;
short row = 1;
short index = -1;
//*****************************************//
void __interrupt ISR(void)
{
    if(RCIF == 1)
    {
        ++index;
        UART_Buffer[index] = RCREG; // Read The Received Data Buffer
        readBuffer();
        reset();
        RCIF = 0;   
    }
}
//*****************************************//

void main(void) {
    FOSC();
    initUART_rx();
    LCD_Init();
    
     LCD_String_xy(1,0,"Initializing...");
    __delay_ms(1000);
    LCD_Clear();
    LCD_String_xy(1,0,"Welcome!");
    __delay_ms(1000);
    LCD_Clear();
    
    while(1)
    {
      //Do nothing 
    }
    return;
}
//---------------------------------------------------
//---------------------------------------------------

void initUART_rx(void)
{
    //setting baud rate
    BRGH = 1; //high speed baud rate
    SPBRG = 52; //baud rate 9600 bps
    
    SYNC = 0; //enable async / disable sync
    SPEN = 1; //serial port enable
    
    TRISCbits.RC6 = 1;
    TRISCbits.RC7 = 1;
    
    //REceivr interrupt enable
    RCIE = 1;
    PEIE = 1;
    GIE = 1;
    
    CREN = 1; //enable data reception 
}

void FOSC(void)
{
    IRCF0 = 1;
    IRCF1 = 1;
    IRCF2 = 1;
}

void LCD_Init()
{
    MSdelay(15);           /*15ms,16x2 LCD Power on delay*/
   	LCD_Port = 0x00;       /*Set PORTD as output PORT for LCD data(D0-D7) pins*/
    LCD_Control = 0x00;    /*Set PORTC as output PORT LCD Control(RS,EN) Pins*/
	LCD_Command(0x38);     /*uses 2 line and initialize 5*7 matrix of LCD*/
    LCD_Command(0x01);     /*clear display screen*/
    LCD_Command(0x0c);     /*display on cursor off*/
    LCD_Command(0x06);     /*increment cursor (shift cursor to right)*/
}

void LCD_Clear()
{
    LCD_Command(0x01);     /*clear display screen*/
}

void LCD_Command(char cmd )
{
	ldata= cmd;            /*Send data to PORT as a command for LCD*/   
	RS = 0;                /*Command Register is selected*/
	EN = 1;                /*High-to-Low pulse on Enable pin to latch data*/ 
	NOP();
	EN = 0;
	MSdelay(3);	
}

void LCD_Char(char dat)
{
	ldata= dat;            /*Send data to LCD*/  
	RS = 1;                /*Data Register is selected*/
	EN=1;                  /*High-to-Low pulse on Enable pin to latch data*/   
	NOP();
	EN=0;
    MSdelay(1);
}

void LCD_String(const char *msg)
{
	while((*msg)!=0)
	{		
	  LCD_Char(*msg);
	  msg++;	
    }		
}

void LCD_String_xy(char row,char pos,const char *msg)
{
    char location=0;
    if(row<=1)
    {
        location=(0x80) | ((pos) & 0x0f); /*Print message on 1st row and desired location*/
        LCD_Command(location);
    }
    else
    {
        location=(0xC0) | ((pos) & 0x0f); /*Print message on 2nd row and desired location*/
        LCD_Command(location);    
    }  
    LCD_String(msg);
}

void MSdelay(unsigned int val)
{
     unsigned int i,j;
        for(i=0;i<=val;i++)
            for(j=0;j<81;j++);      /*This count Provide delay of 1 ms for 8MHz Frequency */
}

void readBuffer(void)
{
    screenFormat();
    
    for(int i = 0; i <= index; ++i)
    {
        switch(UART_Buffer[i])
        {
            case 0b00000001:{ 
                ++pos;
                LCD_String_xy(row,pos,"1");
                break;
            }
            case 0b00000010:{
                ++pos;
                LCD_String_xy(row,pos,"2");
                break;
            }
            case 0b00000011:{
                ++pos;
                LCD_String_xy(row,pos,"3");
                break;
            }
            case 0b00000100: {
                ++pos;
                LCD_String_xy(row,pos,"A");
                break;
            }
            case 0b00000101: {
                ++pos;
                LCD_String_xy(row,pos,"4");
                break;
            }
            case 0b00000110: {
                ++pos;
                LCD_String_xy(row,pos,"5");
                break;
            }
            case 0b00000111: {
                ++pos;
                LCD_String_xy(row,pos,"6");
                break;
            }
            case 0b00001000: {
                ++pos;
                LCD_String_xy(row,pos,"B");
                break;
            }
            case 0b00001001: {
                ++pos;
                LCD_String_xy(row,pos,"7");
                break;
            }
            case 0b00001010: {
                ++pos;
                LCD_String_xy(row,pos,"8");
                break;
            }
            case 0b00001011: {
                ++pos;
                LCD_String_xy(row,pos,"9");
                break;
            }
            case 0b00001100: {
                ++pos;
                LCD_String_xy(row,pos,"C");
                break;
            }
            case 0b00001101: {
                ++pos;
                LCD_String_xy(row,pos,"*");
                break;
            }
            case 0b00001110: {
                ++pos;
                LCD_String_xy(row,pos,"0");
                break;
            }
            case 0b00001111: {
                ++pos;
                LCD_String_xy(row,pos,"#");
                break;
            }
            case 0b00010000: {
                ++pos;
                LCD_String_xy(row,pos,"D");
                break;
            }
        }
}
}

void screenFormat(void)
{
    if(row == 1 && pos >= 15)
    {
        row = 3; pos = -1;
    }
    else if(row == 2 && pos > 15)
    {
        row = 1; pos = -1;
        LCD_Clear();
    }
}

void reset(void)
{
    index = -1;
}