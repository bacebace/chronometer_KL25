//delay.c
#include "derivative.h" /* include peripheral declarations */
#define SET_BIT(reg, idx)	reg = (reg | (1 << idx))		// Macro que seta o bit idx do registrador reg

void init_GPIO(){

	SET_BIT(SIM_SCGC5, 10);		   			// Habilita clock do PORTB (System Clock Gating Control
	SET_BIT(SIM_SCGC5, 13);					// clock do PORTE
	PORTB_PCR18 = PORTB_PCR18 & 0xFFFFF8FF;	// Zera bits de MUX de PTB18
	PORTE_PCR23 = PORTE_PCR23 & 0xFFFFF8FF;	//zera dos bits 8 ao 10 de PTE23
	SET_BIT(PORTB_PCR18, 8);				// Seta bit 0 do MUX de PTB18, assim os bits de MUX serao 001
	SET_BIT(PORTE_PCR23, 8);				// habilito PTE23 como gpio
	SET_BIT(GPIOB_PDDR, 18);				// Seta pino 18 do PORTB como saida 
	SET_BIT(GPIOE_PDDR, 23);					// seta pino 23 do PORTE como saida
}

void delay(uint32_t t) {		// deve atrasar t microsegundo (entre 10 e 1600 microsegundos)
		
		// alterar o codigo Assembly abaixo para gerar atraso de t microsegundo
			__asm volatile(
						"mov r0, %0 \n"		// %0 primeiro parametro (r, que na vdd eh t) - usado em "again"
					
						"mov r4, #3 \n"		// essa parte eh para t<=5 -- pula para "return"
						"cmp r0, r4 \n"
						"ble return \n"
					
						"mov r3, r0 \n"		// usado em "completa"
						"ldr r4, numero \n"
					
						"mov r1, #2 \n"		// r1 recebe 2
						"ldr r2, multi \n"	// carrega multi em r2
						"mul r0,r1 \n"		// multiplica r0 e r1
						"mul r0,r2 \n"		// multiplica r0 e r2
					
					"again: \n"				// label para o branch
						"cmp r0,#0 \n"		// ve se r0 eh 0
						"beq completa \n"	// se r0 = 0, vai para "completa"
						"sub r0,#1 \n"		// subtrai 1 de r0
						"b   again\n"
						
					"completa: \n"
						"cmp r3, #0 \n"
						"blt return \n"
						"beq return \n"
						"sub r3, r4 \n"
						"b completa \n"
					
					"return: \n"
						".align 2 \n"
					"multi: \n"
						".word 2 \n"
					"numero: \n"
					"	.word 20 \n"
					:
					:"r" (t)				// entrada 
					);
			return;
}
