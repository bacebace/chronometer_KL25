//lcd
#include "derivative.h" /* include peripheral declarations */
#include "delay.h"
#define SET_BIT(reg, idx)	reg = (reg | (1 << idx)) //seta o bit idx do registrador reg

void init_GPIO_LCD(){
	SET_BIT(SIM_SCGC5, 11);		   			// Habilita clock do PORTC
	//zera bits do MUX de PTC0 a PTC9
	PORTC_PCR0 = PORTC_PCR0 & 0xFFFFF8FF;
	PORTC_PCR1 = PORTC_PCR1 & 0xFFFFF8FF;
	PORTC_PCR2 = PORTC_PCR2 & 0xFFFFF8FF;
	PORTC_PCR3 = PORTC_PCR3 & 0xFFFFF8FF;
	PORTC_PCR4 = PORTC_PCR4 & 0xFFFFF8FF;
	PORTC_PCR5 = PORTC_PCR5 & 0xFFFFF8FF;
	PORTC_PCR6 = PORTC_PCR6 & 0xFFFFF8FF;
	PORTC_PCR7 = PORTC_PCR7 & 0xFFFFF8FF;
	PORTC_PCR8 = PORTC_PCR8 & 0xFFFFF8FF;
	PORTC_PCR9 = PORTC_PCR9 & 0xFFFFF8FF;
	//habilita como gpio (001) as portas ptc0 a ptc9
	SET_BIT(PORTC_PCR0, 8);
	SET_BIT(PORTC_PCR1, 8);
	SET_BIT(PORTC_PCR2, 8);
	SET_BIT(PORTC_PCR3, 8);
	SET_BIT(PORTC_PCR4, 8);
	SET_BIT(PORTC_PCR5, 8);
	SET_BIT(PORTC_PCR6, 8);
	SET_BIT(PORTC_PCR7, 8);
	SET_BIT(PORTC_PCR8, 8);
	SET_BIT(PORTC_PCR9, 8);
	//seta portas ptc0 a ptc9 como saida
	SET_BIT(GPIOC_PDDR, 0); //d0
	SET_BIT(GPIOC_PDDR, 1); //d1
	SET_BIT(GPIOC_PDDR, 2); //d2
	SET_BIT(GPIOC_PDDR, 3); //d3
	SET_BIT(GPIOC_PDDR, 4); //d4
	SET_BIT(GPIOC_PDDR, 5); //d5
	SET_BIT(GPIOC_PDDR, 6); //d6
	SET_BIT(GPIOC_PDDR, 7); //d7
	SET_BIT(GPIOC_PDDR, 8); //rs
	SET_BIT(GPIOC_PDDR, 9); //enable
}

void init_LCD() { //inicializa o lcd -- esta no manual
	//function set
	GPIOC_PDOR = GPIOC_PDOR & 0xFFFFFEFF; //zera RS (bit 8)
	SET_BIT(GPIOC_PDOR, 9); //enable = 1
	GPIOC_PDOR = GPIOC_PDOR & 0xFFFFFF7F; //zera d7
	GPIOC_PDOR = GPIOC_PDOR & 0xFFFFFFBF; //zera d6
	SET_BIT(GPIOC_PDOR, 5); //seta d5
	SET_BIT(GPIOC_PDOR,4); //seta d4
	SET_BIT(GPIOC_PDOR, 3); //seta d3 a fim de que seja ativado o modo com 2 linhas
	//d2 ate d0 sao dont care 
	SET_BIT(GPIOC_PCOR, 9); //enable = 0
	delay(50);
	//display on off control
	SET_BIT(GPIOC_PDOR, 9); //enable = 1
	SET_BIT(GPIOC_PCOR, 7); //zera d7
	SET_BIT(GPIOC_PCOR, 6); //zera d6
	SET_BIT(GPIOC_PCOR, 5); //zera d5
	SET_BIT(GPIOC_PCOR, 4); //zera d4
	SET_BIT(GPIOC_PDOR, 3); //seta d3
	SET_BIT(GPIOC_PDOR, 2); //seta d2, display on
	SET_BIT(GPIOC_PDOR, 1); //seta d1, cursor on
	SET_BIT(GPIOC_PDOR, 0); //seta d0, blink on
	SET_BIT(GPIOC_PCOR, 9); //enable = 0
	delay(50);
	//display clear
	SET_BIT(GPIOC_PDOR, 9); //enable = 1
	SET_BIT(GPIOC_PCOR, 7); //zera d7
	SET_BIT(GPIOC_PCOR, 6); //zera d6
	SET_BIT(GPIOC_PCOR, 5); //zera d5
	SET_BIT(GPIOC_PCOR, 4); //zera d4
	SET_BIT(GPIOC_PCOR, 3); //zera d3
	SET_BIT(GPIOC_PCOR, 2); //zera d2
	SET_BIT(GPIOC_PCOR, 1); //zera d1
	SET_BIT(GPIOC_PDOR, 0); //seta d0
	SET_BIT(GPIOC_PCOR, 9); //enable = 0
	delay(1600);
	delay(400);
	//entry mode set
	SET_BIT(GPIOC_PDOR, 9); //enable = 1
	SET_BIT(GPIOC_PCOR, 7); //zera d7
	SET_BIT(GPIOC_PCOR, 6); //zera d6
	SET_BIT(GPIOC_PCOR, 5); //zera d5
	SET_BIT(GPIOC_PCOR, 4); //zera d4
	SET_BIT(GPIOC_PCOR, 3); //zera d3
	SET_BIT(GPIOC_PDOR, 2); //seta d2
	SET_BIT(GPIOC_PDOR, 1); //seta d1 - increment mode
	SET_BIT(GPIOC_PCOR, 0); //zera d0 - entire shift off
	SET_BIT(GPIOC_PCOR, 9); //enable = 0
	delay(1000);
	delay(1000);
}

void clear_LCD() {
	GPIOC_PDOR = GPIOC_PDOR & 0xFFFFFEFF; //zera RS (bit 8)
	SET_BIT(GPIOC_PDOR, 9); //enable = 1
	SET_BIT(GPIOC_PCOR, 7); //zera d7
	SET_BIT(GPIOC_PCOR, 6); //zera d6
	SET_BIT(GPIOC_PCOR, 5); //zera d5
	SET_BIT(GPIOC_PCOR, 4); //zera d4
	SET_BIT(GPIOC_PCOR, 3); //zera d3
	SET_BIT(GPIOC_PCOR, 2); //zera d2
	SET_BIT(GPIOC_PCOR, 1); //zera d1
	SET_BIT(GPIOC_PDOR, 0); //seta d0
	SET_BIT(GPIOC_PCOR, 9); //enable = 0
	delay(1600);
	delay(400);
}

void putchar_LCD(char c) { //escreve um caractere no LCD na posicao atual do cursor
	SET_BIT(GPIOC_PDOR, 8); //seta rs
	SET_BIT(GPIOC_PDOR, 9); //enable = 1
	GPIOC_PDOR = GPIOC_PDOR & 0xFFFFFF00;
	GPIOC_PDOR = GPIOC_PDOR | c;
	SET_BIT(GPIOC_PCOR, 9); //enable = 0
	delay(50);
}

void putcmd_LCD(unsigned char c, unsigned int i) {
//envia caractere de comando para o módulo LCD
//c eh o caractere (byte) de comando
//i especifica o tempo em microsegundo que deve ser esperado para tudo dar certo
	SET_BIT(GPIOC_PCOR, 8); //zera rs
	SET_BIT(GPIOC_PDOR, 9); //enable = 1
	GPIOC_PDOR = GPIOC_PDOR & 0xFFFFFF00;
	GPIOC_PDOR = GPIOC_PDOR | c;
	SET_BIT(GPIOC_PCOR, 9); //enable = 0
	delay(i);
}

void setpos_LCD(unsigned int m, unsigned int n) {
//primeiro parametro especifica a linha (1 ou 2) do posicionamento do cursor
//segundo parametro especifica o local onde o cursor sera posicionado
	if (m==0 || m==1) {
		SET_BIT(GPIOC_PCOR, 8); //zera rs
		SET_BIT(GPIOC_PDOR, 9); //enable = 1
		SET_BIT(GPIOC_PDOR, 7); //seta d7 (porque precisa)
		GPIOC_PDOR = GPIOC_PDOR & 0xFFFFFF80; //zera de d6 a d0
		GPIOC_PDOR = GPIOC_PDOR | n;
		SET_BIT(GPIOC_PCOR, 9); //enable = 0
		delay(50);
	}
	else {
		SET_BIT(GPIOC_PCOR, 8); //zera rs
		SET_BIT(GPIOC_PDOR, 9); //enable = 1
		SET_BIT(GPIOC_PDOR, 7); //seta d7 (porque precisa)
		GPIOC_PDOR = GPIOC_PDOR & 0xFFFFFF80; //zera de d6 a d0
		GPIOC_PDOR = GPIOC_PDOR | 0x00000040;
		GPIOC_PDOR = GPIOC_PDOR | n;
		SET_BIT(GPIOC_PCOR, 9); //enable = 0
		delay(50);
	}
	
}

void puts_LCD(char *crono) {
//crono[0] crono [1]	- hh
//crono[3] crono [4]	- mm
//crono[6] crono [7]	- ss
//crono[9] crono [10]	- cc
//tabela ascii		dec		hex
//0					48		0x30
//1					49		0x31
//2					50		0x32
//3					51		0x33
//4					52		0x34
//5					53		0x35
//6					54		0x36
//7					55		0x37
//8					56		0x38
//9					57		0x39
	
	int i;	
	clear_LCD();
	for (i=0; crono[i]!='\0'; i++) {
		putchar_LCD(crono[i]);
	}
}
