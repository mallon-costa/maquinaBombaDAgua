
/* 
 * File: projeto3.c
 */

#define _XTAL_FREQ 8000000

#pragma config FOSC = HS   // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF  // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON  // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF   // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF   // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF   // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF    // Flash Program Memory Code Protection bit (Code protection off)

#include <stdio.h>      //biblioteca para entrada e saida dos dispositivos de E/S
#include <stdlib.h>     //funções envolvendo alocação de memória, controle de processos, conversões e outras.
#include <stdbool.h>    // usada para manipular variáveis lógicas, como verdadeiro e falso
#include <xc.h>     //Biblioteca para microcontroladores PIC

#include "lcd.h"    //biblioteca para utilização do LCD

int main ( void ) {
    
    int i = 0;
    int c = 16;
    int b = 0;
    CMCON = 0x07;           //Desativa os dois comparadores do PIC
    ADCON1 = 0x06;          //TOdos os pinos de AN0 a AN7 são configurados como digitais
    TRISA = 0x0F;          // configuração das entradas e saídas do PORTA (00001111) com RA0 a RA3 como entradas
    PORTA = 0x00;           //inicialização do PORTA
    PORTD = 0x00;           //inicialização do PORTD
    PORTC = 0x00;           //inicialização do PORTC
    TRISB = 0x00;          //configuração das entradas e saídas do PORTB com todos como saidas
    TRISC = 0x00;          //configuração das entradas e saídas do PORTC com todos como saidas
    TRISD = 0x80;          //configuração das entradas e saídas do PORTD (10000000) com todos como saídas, exceto RD7

    RD2 = 1;    //setando lcd light (pino RD2) como 1
    RD7 = 1;    //setando o pino RD7 como 1
    
    LCD lcd = { &PORTB, 2, 3, 4, 5, 6, 7 };    //PORT, RS, EN, B4, B5, B6, B7 (conexão dos pinos do led no PIC)

    LCD_Init(lcd);  //inicialização do LCD
    
    // Nome do Projeto
    LCD_Clear();    //limpa o LCD
    __delay_ms(500);  //delay de 500 milissegundos (1/2 segundo)
    LCD_Set_Cursor(0,0);    //Iniciando o cursor nas coordenadas 0, 0
    LCD_putrs("Projeto");   //enviando a string "Projeto" para o LCD
    __delay_ms(200);        //delay de 200 milissegundos
    LCD_Set_Cursor(1,0);    //Iniciando o cursor nas coordenadas 1, 0 
    LCD_putrs("Bomba D'Agua");      //enviando a string "Sensor de Nível" para o LCD
    __delay_ms(500);    //delay de 500 milissegundos
    
    for(i=0; i<15; i++) { // Move o texto para a direita 16 vezes
      LCD_Shift_Right();
      __delay_ms(150);
    }
    i=0;
    
    do {        //laço infinito
        
        LCD_Clear();        //Limpa o LCD
        LCD_Set_Cursor(0,0);    //Cursor inicia em 0, 0
        LCD_putrs("Nivel: ");   //enviando a string "Nível: 0" para o LCD
        LCD_Set_Cursor(1,0);
        LCD_putrs("Motor ");
   
        if(c>0)
        {
           RD2 = 1;     //liga o LED lcd light
           c--;
        }
        else
           RD2 = 0;     //desliga o LED lcd light
      
        if(b>0)
        {
            RD0 = 1;    //liga o buzzer
            __delay_ms(150);    //delay de 150 milissegundos
            RD0 = 0;    //desliga o buzzer
            b--;
        }
   
        if(RD7 == 0)    //verifica se o pino RD7 está desligado
            c = 16;
     
        if(PORTA == 0x0F)      //Verifica se PORTA está em seu estado original (nenhuma chave fechada)
        {                       //ou seja 00001111
            RD1 = 1;            //liga o motor
            LCD_Set_Cursor(0,7);    //Inicia o curso em 0, 7
            LCD_putrs("Bem Baixo");     //Envia a String "Bem baixo" no LCD
            LCD_Set_Cursor(1,6);
            LCD_putrs("ON");    //mostra no display que o motor está ligado
            PORTC = 1;          //liga o LED D3 - (00000001 significa 7 em binário)
            if(i == 0)
            {
               c = 16;
               b=3;
            }
            i=1;
        }
        else if(PORTA == 0x0E)      //Verifica se PORTA está com a primeira chave fechada (0x0E é 00001110) 
        {                           //o ultimo bit (foi de 1 para 0) significa que a chave foi fechada
            LCD_Set_Cursor(0,7);
            LCD_putrs("Baixo");
            if(i == 1) {
                LCD_Set_Cursor(1,6);
                LCD_putrs("ON");
            } else {
                LCD_Set_Cursor(1,6);
                LCD_putrs("OFF");
            }
            PORTC = 3;      //liga o LED D3 e D5 (3 em decimal significa 00000011 em binario)
        }
        else if(PORTA == 0x0C)      //Verifica se PORTA está com a segunda chave fechada (0x0C é 00001100) 
        {
            LCD_Set_Cursor(0,7);
            LCD_putrs("Medio");
            if(i == 1){
                LCD_Set_Cursor(1,6);
                LCD_putrs("ON");
            } else {
                LCD_Set_Cursor(1,6);
                LCD_putrs("OFF");
            }
            PORTC = 7;              //liga o LED D3, D5 e D4 (7 em decimal significa 00000111 em binario)
        }
        else if(PORTA == 0x08)      //Verifica se PORTA está com a terceira chave fechada (0x08 é 00001000)
        {
            LCD_Set_Cursor(0,7);
            LCD_putrs("Alto");
            if(i == 1){
                LCD_Set_Cursor(1,6);
                LCD_putrs("ON");
            } else {
                LCD_Set_Cursor(1,6);
                LCD_putrs("OFF");
            }
               PORTC = 15;          //liga o LED D3, D5, D4 e D1 (15 em decimal significa 00001111 em binario)
        }
        else if(PORTA == 0x00)    //Verifica se PORTA está com a quarta chave fechada (0x00 de hexadecimal é 00000000 binario)
        {
            LCD_Set_Cursor(0,7);
            LCD_putrs("Cheio");
            LCD_Set_Cursor(1,6);
            LCD_putrs("OFF");
            RD1 = 0;            //Desliga o motor
            if(i == 1)
            {
              c = 16;
              b = 3;
            }
            i=0;
            PORTC = 31;        //liga o LED D3, D5, D4, D1 e D2 (31 em decimal significa 00011111 em binario) 
        }
        else                    //Caso nenhuma das condicionais anteriores sejam verdadeiras
            PORTA = 0x0F;       //O registrador PORTA é setado com seu valor original
        __delay_ms(125);        //Delay de 125 milissegundos
}while(1);            // Loop infinito

    return (EXIT_SUCCESS);      //Finalização do programa, retornando saída bem sucedida
}
