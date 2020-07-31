#include "derivative.h"
#define SET_BIT(reg, idx)	reg = (reg | (1 << idx)) 

void init_interrupcao() {
	//configuraçao do systick
	SYST_RVR = SYST_RVR & 0xFF000000;
	SYST_RVR = SYST_RVR | 0x00033333; //valor do reload 
	SYST_CSR = SYST_CSR & 0xFFFFFFF8;
	SYST_CSR = SYST_CSR | 0x00000007; //usa clk do processador, interrupcao ativa, contador operando
	
	//configuracao do botao s1 (reset): NMI, PTA4
	SET_BIT(SIM_SCGC5,9); //clock do PORTA
	PORTA_PCR4 = PORTA_PCR4 & 0xFFFFF8FF; //zero o MUX (bits 8 a 10)
	PORTA_PCR4 = PORTA_PCR4 | 0x00000700; //alternativa 7 no MUX: NMI
	//PORTA_PCR4 = PORTA_PCR4 & 0xFFF0FFFF; //zero o IRQC
	//PORTA_PCR4 = PORTA_PCR4 | 0x00090000; //interrupt on rising edge
	
	//configuracao do botao s3 (stop): PTA12
	PORTA_PCR12 = PORTA_PCR12 & 0xFFFFF8FF; //zera o MUX
	PORTA_PCR12 = PORTA_PCR12 | 0x00000100; //alternativa 1 no MUX: gpio
	PORTA_PCR12 = PORTA_PCR12 & 0xFFF0FFFF; //zera o IRQC
	PORTA_PCR12 = PORTA_PCR12 | 0x00090000; //interrupt on rising edge
	SET_BIT(PORTA_PCR12, 24); //limpa ISF (interrupt status flag)
	NVIC_IPR7 = NVIC_IPR7 & 0xFF3FFFFF; //zera os bits 22 e 23 (prioridade 0)
	
	SET_BIT(NVIC_ISER, 30); //nvic vai administrar a interrupcao do s3
}
