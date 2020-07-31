//uart.c
#include "derivative.h"
#include "delay.h"
#include "lcd.h"
#define SET_BIT(reg, idx)	reg = (reg | (1 << idx)) //seta o bit idx do registrador reg

void init_UART0(){	// inicializa a operacao da UART0 (MUX, Clock)
//taxa de 19200 bps (clock); pacotes de 8 bits; 1 stop bit; sem bit de paridade
    SET_BIT(SIM_SCGC5,9); //habilita clock de PORTA
    PORTA_PCR1 = PORTA_PCR1 & 0xFFFFF8FF;    
    SET_BIT(PORTA_PCR1,9); //UART0_RX em PTA1 (opcao 2 do mux)
    PORTA_PCR2 = PORTA_PCR2 & 0xFFFFF8FF;  
    SET_BIT(PORTA_PCR2,9); //UART0_TX em PTA2 (opcao 2 do mux)
    
    SET_BIT(SIM_SCGC4,10); //habilita clock de UART0
    SIM_SOPT2 = SIM_SOPT2 & 0xF3FFFFFF; //zera bits 26 e 27 (UART0SRC)
    SET_BIT(SIM_SOPT2,26); //seto o bit26 -- UART0SCR=01 -> seleciono MCGFLLCLK clock or MCGPLLCLK/2 clock
    
/* baud rate = (baud clock)/((SRC+1)*BR)
baud clock = 20.97 MHz
SRC por padrao eh 15
para baud rate ser 1200, BR deve ser 1092.1875 (aproximamos para 1092 = 0b 00100 01000100)
BDH = 0b00100
BDL = 0b01000100 */
    UART0_BDH = UART0_BDH & 0xE0; //zera os bits de 0 a 4 de UART0_BDH
    UART0_BDH = UART0_BDH | 0b00100; 
    UART0_BDL = 0b01000100; //UART0_BDL
    
    //bit 4 de UART0_C1 ja vem desabilitado -> receiver e transmitter usam dados de 8 bits
    //bit 5 de UART0_BDH ja vem desabilitado -> one stop bit
    
    UART0_C2 = UART0_C2 | 0b00001100; // bit 2 = receiver; bit 3 = transmitter (habilitei ambos)
    SET_BIT(UART0_C2,5); //Receiver Interrupt Enable for RDRF
    
    SET_BIT(NVIC_ISER, 12); //nvic vai administrar a interrupcao da uart0
    NVIC_IPR3 = NVIC_IPR3 & 0xFFFFFFFC; //prioridade 0
    SET_BIT(PORTA_PCR1,24); //zera flag isf
    SET_BIT(PORTA_PCR2,24); //zera flag isf
}

char getchar_UART0() { //recebe um caractere da UART0 - - - recebe do teclado??
	//codigo do moodle
	char c;
	if (UART0_S1 & UART0_S1_RDRF_MASK){ //RDRF eh 1 quando tem coisa no receiver
		c = UART0_D; // le o caractere
		return c;
	}	
	return '\0'; 
}

void putchar_UART0(char c) { //envia um caractere pela UART0 
	//codigo do moodle
	while( !(UART0_S1 & UART0_S1_TDRE_MASK)); //TDRE eh 0 quando tem coisa no transmitter, entao espera ate nao ter mais
	UART0_D = c; // envia caracter a ser transmitido
}

void puts_UART0(char *vetor) { //envia uma string pela UART0
	int i;
		for (i=0; vetor[i]!='\0';i++){
			putchar_UART0(vetor[i]);
		}
}
