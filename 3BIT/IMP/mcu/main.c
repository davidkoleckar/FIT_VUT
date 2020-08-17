/**
 * David Koleckar - xkolec07
 * 40%
 * Pridana obsluha preruseni, modifikace obsluhy klavesnice a pridany vypocty
 * Datum posledni modifikace 16.12.2016
 */

/*******************************************************************************
   main.c: 
   Copyright (C) 2009 Brno University of Technology,
                      Faculty of Information Technology
   Author(s): Zdenek Vasicek <vasicek AT stud.fit.vutbr.cz>

   LICENSE TERMS

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:
   1. Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the
      distribution.
   3. All advertising materials mentioning features or use of this software
      or firmware must display the following acknowledgement:

        This product includes software developed by the University of
        Technology, Faculty of Information Technology, Brno and its
        contributors.

   4. Neither the name of the Company nor the names of its contributors
      may be used to endorse or promote products derived from this
      software without specific prior written permission.

   This software or firmware is provided ``as is'', and any express or implied
   warranties, including, but not limited to, the implied warranties of
   merchantability and fitness for a particular purpose are disclaimed.
   In no event shall the company or contributors be liable for any
   direct, indirect, incidental, special, exemplary, or consequential
   damages (including, but not limited to, procurement of substitute
   goods or services; loss of use, data, or profits; or business
   interruption) however caused and on any theory of liability, whether
   in contract, strict liability, or tort (including negligence or
   otherwise) arising in any way out of the use of this software, even
   if advised of the possibility of such damage.

   $Id$


*******************************************************************************/

#include <fitkitlib.h>
#include <keyboard/keyboard.h>
#include <lcd/display.h>
#include <stdio.h>

char last_ch; //naposledy precteny znak
char char_cnt;
int jrychlost = 0; // jednotky rychlosti - po spusteni metry za sekundu
int jvzdalenost = 0; // jendotky vzdalenosti - po spusteni metry
unsigned long int o; // obvod kola
int A=0; // pomocna promena 
char vzdalenost_m[16]; 
char vzdalenost_km[16]; 
char rychlost_m[16]; 
char rychlost_km[16]; 
int counter = 0; //pocet preruseni
unsigned long int cas=0;
/*******************************************************************************
 * Vypis uzivatelske napovedy (funkce se vola pri vykonavani prikazu "help")
*******************************************************************************/
void print_user_help(void)
{
}


/*******************************************************************************
 * Obsluha klavesnice
*******************************************************************************/
int keyboard_idle()
{
  char ch;
  
  ch = key_decode(read_word_keyboard_4x4());
  if (ch != last_ch) 
  {
    last_ch = ch;
    if (ch != 0) 
    {
		//potvrzeni obvodu kolu - klavesa A
		if(ch == 'A' && A == 0){
			A = 1;
			LCD_clear();
			LCD_append_string("0 m/s  ");
			LCD_append_string(vzdalenost_m);
			LCD_append_string("m");
			return;
		}
		//zmena jednotek vzdalenosti
		if(ch == 'B'){
			if(jvzdalenost == 0){
				jvzdalenost = 1;
				LCD_clear();
				LCD_append_string("Jednotky km");
				delay_ms(2000);
				LCD_clear();
				if(jrychlost == 0){
					LCD_append_string(rychlost_m);
					LCD_append_string("m/s ");
				}
				else{
					LCD_append_string(rychlost_km);
					LCD_append_string("km/h ");
				}
				LCD_append_string(vzdalenost_km);
				LCD_append_string("km");
			}
			else{
				jvzdalenost = 0;
				LCD_clear();
				LCD_append_string("Jednotky m");
				delay_ms(2000);
				LCD_clear();
				if(jrychlost == 0){
					LCD_append_string(rychlost_m);
					LCD_append_string("m/s ");
				}
				else{
					LCD_append_string(rychlost_km);
					LCD_append_string("km/h ");
				}
				LCD_append_string(vzdalenost_m);
				LCD_append_string("m");
			}
		}
		//zmena jednotek rychlosti
		if(ch == 'C'){
			if(jrychlost == 0){
				jrychlost = 1;
				LCD_clear();
				LCD_append_string("Jednotky km/h");
				delay_ms(2000);
				LCD_clear();
				LCD_append_string(rychlost_km);
				LCD_append_string("km/h ");
				if(jvzdalenost == 0){
					LCD_append_string(vzdalenost_m);
					LCD_append_string("m");
				}
				else{
					LCD_append_string(vzdalenost_km);
					LCD_append_string("km");
				}
			}
			else{
				jrychlost = 0;
				LCD_clear();
				LCD_append_string("Jednotky m/s");
				delay_ms(2000);
				LCD_clear();
				LCD_append_string(rychlost_m);
				LCD_append_string("m/s ");
				if(jvzdalenost == 0){
					LCD_append_string(vzdalenost_m);
					LCD_append_string("m");
				}
				else{
					LCD_append_string(vzdalenost_km);
					LCD_append_string("km");
				}
			}
		}
		// prumerna rychlost
		if(ch == 'D'){
			LCD_clear();
			LCD_append_string("Prumer ");
			int prumer = ((o*counter)/100)/(cas/4);
			char p[16]; 			
			if(jrychlost == 0){
				sprintf (p, "%d", prumer);
				LCD_append_string(p);
				LCD_append_string("m/s ");
			}
			else{
				sprintf (p, "%d", prumer*3,6);
				LCD_append_string(p);
				LCD_append_string("km/h ");
			}
		}

		//cteni obvodu kola
		if(A==0){
			unsigned long int tmp;
			switch(ch){
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					tmp = o;
					o = tmp*10 + (ch - '0');
					LCD_append_char(ch);
			}
		  
		}
      char_cnt++;
      
    }
  }
  return 0;
}



/*******************************************************************************
 * Dekodovani a vykonani uzivatelskych prikazu
*******************************************************************************/
unsigned char decode_user_cmd(char *cmd_ucase, char *cmd)
{
  return CMD_UNKNOWN;
}

/*******************************************************************************
 * Inicializace periferii/komponent po naprogramovani FPGA
*******************************************************************************/
void fpga_initialized()
{
  LCD_init();
  LCD_clear();
  LCD_append_string("Zadej obvod kola");
  delay_ms(2100);
  LCD_clear();
  LCD_append_string("Obvod[cm] ");
}



/*******************************************************************************
 * Hlavni funkce
*******************************************************************************/
int main(void)
{
  char_cnt = 0;
  last_ch = 0;

  initialize_hardware();
  keyboard_init();

  WDG_stop(); // zastavit watchdog
  TACCTL0 = CCIE; // povolit preruseni pro casovac
  TACCR0 = 0x2000; // nastavit po kolika ticich (0x4000, tj. za 1/2 s) ma dojit k preruseni
  TACTL = TASSEL_1 + MC_2; // ACLK                
  P1IE |= BIT7;	// povolit preruseni na portu 1 bit 7
  P1IFG &= ~(BIT7); //vynulovani

  while (1)
  {
	delay_ms(50);
    keyboard_idle(); // obsluha klavesnice
    terminal_idle(); // obsluha terminalu
  }

}

unsigned int ujeta;
/*******************************************************************************
 * Obsluha preruseni
*******************************************************************************/
interrupt (TIMERA0_VECTOR) Timer_A (void) 
{   	
	
	cas++;
	if(cas%12 == 0)//kazde 3 sekundy
	{
		if(o*ujeta > 300)
			sprintf (rychlost_m, "%d", ((o*ujeta)/100)/3);
		else
			sprintf (rychlost_m, "0.%d", ((o*ujeta)/100)%3);

		sprintf (rychlost_km, "%d", (((o*ujeta)/100)/3)*3,6);
		ujeta = 0;
	}

	if (P1IFG & BIT7){
		P1IFG &= ~(BIT7);
		ujeta++;
		counter++;
		
		unsigned long int vysle_m = (o*counter)/100;
		if(vysle_m == 0)
			sprintf (vzdalenost_m, "%lu", 0);
		else
			sprintf (vzdalenost_m, "%lu", vysle_m);
		
		unsigned long int vysle_km = (o*counter)/100000;
		sprintf (vzdalenost_km, "%lu", vysle_km);

		LCD_clear();
		if(jrychlost == 0){
			LCD_append_string(rychlost_m);
			LCD_append_string("m/s ");
		}
		else{
			LCD_append_string(rychlost_km);
			LCD_append_string("km/h ");
		}
		if(jvzdalenost == 0){
			LCD_append_string(vzdalenost_m);
			LCD_append_string("m");
		}
		else{
			LCD_append_string(vzdalenost_km);
			LCD_append_string("km");
		}		
	} 

	TACCR0 += 0x2000;//1/4 sekundy	
}


