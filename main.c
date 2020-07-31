//roteiro 9

#include "derivative.h" /* include peripheral declarations */
#include "delay.h"
#include "lcd.h"
#include "uart.h"
#include "interrupcao.h"
#define SET_BIT(reg, idx)	reg = (reg | (1 << idx)) //seta o bit idx do registrador reg

int cc = 0, ss=0, mm=0, hh=0;
char cmm;
char v[15] = "00:00:00:00"; //hh:mm:ss:cc		[v0][v1]:[v3][v4]:[v6][v7]:[v9][v10]

int flagS = 0; //quando eh 0, conta; quando eh 1, nao conta
int flagD = 1; //quando eh 0, nao mostra no terminal; quando eh 1, mostra no terminal
int flagL = 0; //quando eh 0, nao lista; quando eh 1, lista

void SysTick_Handler() { //valores do cronometro
	cc++;
	if(cc>99){
		cc=0;
		ss++;
	}

	if(ss>59) {
		ss=0;
		mm++;
	}
	if(mm>59){
		hh++;
		mm=0;
	}
	if(hh>24)
		hh=0;
}

void atualiza(){	//atualiza o vetor
	v[10] = 48 + cc%10;	//capta a unidade do centesimo
	v[9] = 48 + cc/10;	//capta a dezena do centesimo
	v[7] = 48 + ss%10;	//capta a unidade do segundo
	v[6] = 48 + ss/10;	//capta a dezena do segundo
	v[4] = 48 + mm%10;	//capta a unidade do minuto
	v[3] = 48 + mm/10;	//capta a dezena do minuto
	v[1] = 48 + hh%10;	//capta a unidade da hora
	v[0] = 48 + hh/10;	//capta a dezena da hora
}

void NMI_Handler(){ //botao s1 (reset)
	v[0]='0';
	v[1]='0';
	v[3]='0';
	v[4]='0';
	v[6]='0';
	v[7]='0';
	v[9]='0';
	v[10]='0';
	cc = 0;
	ss = 0;
	mm = 0;
	hh = 0;
}

void PORTA_IRQHandler(){ //botao s3 (stop)
	if (flagS==0)
		flagS = 1;
	else 
		flagS = 0;
	
	if (flagS == 0) {	//cronometro esta contando
		//SYST_CSR = SYST_CSR & 0xFFFFFFF8;
		SYST_CSR = SYST_CSR | 0x00000007;
	}

	if (flagS == 1) {	//cronometro esta parado
		SYST_CSR = SYST_CSR & 0xFFFFFFFE;
	}
	
	SET_BIT(PORTA_PCR12, 24); //zera a flag isf
}

void UART0_IRQHandler(){ //atualiza as flags D, L e ESC
	cmm = getchar_UART0();
	
	if (cmm == 'd' || cmm == 'D') {
		if (flagD==0)
			flagD = 1;
		else 
			flagD = 0;
	}
	
	if (cmm == 'l' || cmm == 'L') {
		if (flagL==0)
			flagL = 1;
		else
			flagL = 0;
	}
	
	SET_BIT(PORTA_PCR1, 24); //zerar a flag ISF do receptor
}

//				COLOCAR NAS INTERRUPCOES
void f_list() {
	if (flagL == 0) { //nao lista
		putchar_UART0(0x1B);
		puts_UART0("[2J");
		putchar_UART0(0x1B);
		puts_UART0("[H");
		puts_UART0(v); //escreve no terminal
		puts_LCD(v); //escreve no LCD
	}

	if (flagL != 0) { //lista	
		puts_UART0("\n\r");	//pula uma linha e poe o cursor no canto esquerdo
		puts_UART0(v); //escreve no terminal
		puts_LCD(v); //escreve no LCD
	}
}

void f_display () {
	if (flagD == 0) {	//nao mostra no terminal
		//limpa terminal
		putchar_UART0(0x1B);
		puts_UART0("[2J");
		putchar_UART0(0x1B);
		puts_UART0("[H");
		
		puts_LCD(v); //escreve no LCD
	}
	
	if (flagD != 0)	//mostra no terminal
		f_list();
}

int main(){
	//inicializacoes de GPIOs, LCD e UART0
	init_GPIO();
	init_GPIO_LCD();
	init_LCD();
	clear_LCD();
	init_UART0();
	init_interrupcao(); //systick, botao s1 (reset), botao s3 (stop)
	
	//ajusta a posicao do LCD
	setpos_LCD(1, 0);
	
	//limpa a tela e joga para o canto superior esquerdo
	putchar_UART0(0x1B);
	puts_UART0("[2J");
	putchar_UART0(0x1B);
	puts_UART0("[H");

	puts_UART0(v);
	
	while(1){
		
		atualiza();
		f_display();
		if (cmm == 0x1B) {	//se comando for ESC
			putchar_UART0(0x1B);
			puts_UART0("[2J");
			putchar_UART0(0x1B);
			puts_UART0("[H");
			clear_LCD();
			return 0;
		}
		
	}
//passo perfume para tirar foto
	return 0;
}
