#include <msp430g2553.h>

/*
 *  RS - Diz se vc esta enviando COMANDO ou CARACTER
 *  E -  Gatilho de borda de descida para escrita ou de subida para leitura.
 */

#define DR P1OUT = P1OUT | BIT4                 //define RS high PARA ENVIO DE CARACTER
#define CWR P1OUT = P1OUT & (~BIT4)             //define RS low PARA COMANDOS
#define ENABLE_HIGH P1OUT = P1OUT | BIT6        //define Enable high signal
#define ENABLE_LOW P1OUT = P1OUT & (~BIT6)      //define Enable Low signal

unsigned int i;
unsigned int j;

void inicializarLcd(void){
    P1DIR |= 0xFF;
    P1OUT &= 0x00;
    P2DIR |= 0xFF;
    P2OUT &= 0x00;
    enviaComando(0x33);
    enviaComando(0x32);
    enviaComando(0x28);                         //Modo 4 bits
    enviaComando(0x0E);                         //Limpa o display
    enviaComando(0x01);                         //Ativa o display e o cursor
    enviaComando(0x06);                         //Incrementa o cursor
    enviaComando(0x80);                         //Seleciona a linha 1 / coluna 1
}

void delay(unsigned int k){
    for(j=0;j<=k;j++)
    {
    for(i=0;i<100;i++);
    }
}

/*PULSO PARA ENVIO DO SINAL(dados)*/
void pulsoEscrita(void){
    ENABLE_HIGH;
    delay(2);
    ENABLE_LOW;
}

/*PULSO PARA RECEBIMENTO SINAL(comandos)*/
void pulsoLeitura(void){
    ENABLE_LOW;
    delay(2);
    ENABLE_HIGH;
}

/*Verifica se o processador interno do LCD esta ocupado, quando for 0, o LCD pode obter o proximo dado ou comando*/
void check_busy(void){
    P2DIR &= ~(BIT3);                           //torna P2.3 entrada
    while((P2IN&BIT3)==1)
    {
        pulsoLeitura();
    }
    P2DIR |= BIT3;                              //torna P2.3 saida
}

/*Funcao para enviar comandos*/
void enviaComando(unsigned char cmd){
    check_busy();
    CWR;
    P2OUT = (P2OUT & 0xF0)|((cmd>>4) & 0x0F);   //Envia os 4 primeiros bits
    pulsoEscrita();                             //Pulso para enviar os dados
    P2OUT = (P2OUT & 0xF0)|(cmd & 0x0F);        //Envia os ultimos 4 bits
    pulsoEscrita();                             //Pulso para enviar os dados
}

/*Envia os dados*/
void enviaDados(unsigned char data){
    check_busy();
    DR;
    P2OUT = (P2OUT & 0xF0)|((data>>4) & 0x0F);  //Envia os 4 primeiros bits pois possui somente 4 pinos(4bits) de sai­da
    pulsoEscrita();                             //Pulso para enviar os dados
    P2OUT = (P2OUT & 0xF0)|(data & 0x0F);       //Envia os ultimos 4 bits
    pulsoEscrita();                             //Pulso para enviar os dados
}

/*Envia caracter por caracter*/
void enviaString(unsigned char *s){
    while(*s){
        enviaDados(*s);
        s++;
    }
}

void enviaInt(int i){
    char caractere[2] = (char) i + 0x30;
    enviaString(&caractere);
}

void integerToLcd(int integer)
{
	int m=0,c=0,d=0,u=0;
	
	m = integer / 1000;
	c = ((integer - m*1000)-1) / 100;
	d = (integer%100)/10;
	u = integer%10;

	if (m != 0)
	    enviaInt(m);
	//se milesimo for 0 e centesimo for diferente de 0, ou, m for diferente de 0 e centesimo for igual a 0 MOSTRA(ex: 1034[mostra o zero], 0013[nao mostra], 0123[mostra o 1])
	if((c != 0) || (m != 0 && c == 0))
	    enviaInt(c);
	if((d != 0) || ((m != 0 || c != 0) && d == 0))
	    enviaInt(d);
	
    enviaInt(u);
}

void longToLcd(long val, int controle){
	int inteiro=0, decimal=0;

	// 1 -> Pressao
	// 2 -> Temperatura (ex: 273 = 27.3 C)

    if(controle == 1){
        inteiro = (int) val / 100;
	    decimal = (int) val % 100;
    }

    if(controle == 2){
	    inteiro = (int) val / 10;
	    decimal = (int) val % 10;
    }
    
	integerToLcd((int)inteiro);
	enviaString(".");
	integerToLcd((int)decimal);
}