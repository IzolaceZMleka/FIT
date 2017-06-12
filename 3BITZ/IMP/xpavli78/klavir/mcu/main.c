/*
Autor: Jan Pavlica
Login: xpavli78
Datum: 12. 12. 2016
Prevzate kody:
	Demo – klávesnice využívající LCD = 20%
	Generátor signálu s nastavitelným kmitočtem a velikostí amplitudy napětí = 10%
*/

#include <fitkitlib.h>
#include <keyboard/keyboard.h>
#include <lcd/display.h>
#include <stdlib.h>
#include <string.h>

#define TICKS_PER_SECOND 32768  // pocet tiku za 1s
#define SIN_SAMPLES 32    // pocet vzorku harmonickeho signalu (sinus)
#define SAW_SAMPLES 32    // pocet vzorku piloviteho signalu
#define SQUARE_SAMPLES 2  // pocet vzorku obdelnikoveho signalu
#define CONST_SAMPLES 1  // pocet vzorku konstantniho signalu

// definice nekterych tonu

#define c 261
#define d 294
#define e 329
#define f 349
#define g 391
#define gS 415
#define a 440
#define aS 455
#define b 466
#define cH 523
#define cSH 554
#define dH 587
#define dSH 622
#define eH 659
#define fH 698
#define fSH 740
#define gH 784
#define gSH 830
#define aH 880

// konstanty pro nasobeni oktav

#define OCT_1 1
#define OCT_2 2
#define OCT_3 4
#define OCT_4 8
#define OCT_5 16
#define OCT_6 32
#define OCT_7 64
/*
 * ---------------------------------------------------------------------------- 
 */
enum SIG_ID {SIG_CONST, SIG_HARM, SIG_SAW, SIG_SQUARE} sig_type;

// pole se vzorky signalu
                 
unsigned char arr_square[SQUARE_SAMPLES] = {
  0, 255
};

/*
 * ---------------------------------------------------------------------------- 
 */
unsigned char *signal_arr = arr_square;  // ukazatel na pole vzorku zvoleneho signalu
int signal_arr_index = 0;   // index aktualniho vzorku, ktery se bude prevadet
int frequency = 440;        // frekvence signalu
unsigned char samples = SQUARE_SAMPLES;  // pocet vzorku zvoleneho signalu
int ticks;                  // pocet tiku, po kterych ma dojit k preruseni od casovace
int freq_max;               // maximalni frekvence zvoleneho signalu, kterou lze generovat
unsigned char y_scale = 0;          // zmena meritka osy Y na dany pocet % (0...100) 
char last_ch; //naposledy precteny znak
char char_cnt;
int octava = OCT_4; // promenna pro nasobeni frekvnce k dosazeni oktavy
char octava_msg = '4'; // promenna pro ulozeni aktualni octavy
int delay = 0; // promenna pro detekci zpozdeni


/*******************************************************************************
 * Vypis uzivatelske napovedy (funkce se vola pri vykonavani prikazu "help")
*******************************************************************************/
void print_user_help(void)
{
  term_send_str_crlf("Napoveda pro piano:");
  term_send_str_crlf("  Klavesy 1-7 tony");
  term_send_str_crlf("  Klavesa 8 Imperial March");
  term_send_str_crlf("  Klavesa 9 Mario");
  term_send_str_crlf("  Ostatni klavesy zmena oktavy");
  term_send_str_crlf("Prikazy pro terminal:");
  term_send_str_crlf("  SW - Imperial March");
  term_send_str_crlf("  MARIO - Mario");
  term_send_str_crlf("  DELAY - Pro prodlevu trvani tonu");
}

// funkce pro prehrani tonu po urcitou delku nasledovany pauzou

void zahrej(int ton, int delka_tonu, int delka_pauzy)
{
	y_scale = 100;
	frequency = ton; // nastaveni tonu
	set_ticks();
	delay_ms(delka_tonu); // trvani tonu
	y_scale = 0;
	delay_ms(delka_pauzy); // trvani prodlevy po zazneni tobu
}
// melodie pro Mario
void play3()
{
	// frekvence prevzaty z http://cncpp.divilabs.com/2013/12/c-code-for-mario-theme-intro-song.html
	    zahrej(330,100,100);
        zahrej(330,100,300);
        zahrej(330,100,300);
        zahrej(262,100,100);
        zahrej(330,100,300);
        zahrej(392,100,700);
        zahrej(196,100,700);
        zahrej(262,300,300);
        zahrej(196,300,300);
        zahrej(164,300,300);
        zahrej(220,300,100);
        zahrej(246,100,300);
        zahrej(233,200,0);
        zahrej(220,100,300);
        zahrej(196,100,150);
        zahrej(330,100,150);
        zahrej(392,100,150);
        zahrej(440,100,300);
        zahrej(349,100,100);
        zahrej(392,100,300);
        zahrej(330,100,300);
        zahrej(262,100,100);
        zahrej(294,100,100);
        zahrej(247,100,500);
        zahrej(262,300,300);
        zahrej(196,300,300);
        zahrej(164,300,300);
        zahrej(220,300,100);
        zahrej(246,100,300);
        zahrej(233,200,0);
        zahrej(220,100,300);
        zahrej(196,100,150);
        zahrej(330,100,150);
        zahrej(392,100,150);
        zahrej(440,100,300);
        zahrej(349,100,100);
        zahrej(392,100,300);
        zahrej(330,100,300);
        zahrej(262,100,100);
        zahrej(294,100,100);
        zahrej(247,100,900);
        zahrej(392,100,100);
        zahrej(370,100,100);
        zahrej(349,100,100);
        zahrej(311,100,300);
        zahrej(330,100,300);
        zahrej(207,100,100);
        zahrej(220,100,100);
        zahrej(262,100,300);
        zahrej(220,100,100);
        zahrej(262,100,100);
        zahrej(294,100,500);
        zahrej(392,100,100);
        zahrej(370,100,100);
        zahrej(349,100,100);
        zahrej(311,100,300);
        zahrej(330,100,300);
        zahrej(523,100,300);
        zahrej(523,100,100);
        zahrej(523,100,1100);
        zahrej(392,100,100);
        zahrej(370,100,100);
        zahrej(349,100,100);
        zahrej(311,100,300);
        zahrej(330,100,300);
        zahrej(207,100,100);
        zahrej(220,100,100);
        zahrej(262,100,300);
        zahrej(220,100,100);
        zahrej(262,100,100);
        zahrej(294,100,500);
        zahrej(311,300,300);
        zahrej(296,300,300);
        zahrej(262,300,1300);
        zahrej(262,100,100);
        zahrej(262,100,300);
        zahrej(262,100,300);
        zahrej(262,100,100);
        zahrej(294,100,300);
        zahrej(330,200,50);
        zahrej(262,200,50);
        zahrej(220,200,50);
        zahrej(196,100,700);
        zahrej(262,100,100);
        zahrej(262,100,300);
        zahrej(262,100,300);
        zahrej(262,100,100);
        zahrej(294,100,100);
        zahrej(330,100,700);
        zahrej(440,100,300);
        zahrej(392,100,500);
        zahrej(262,100,100);
        zahrej(262,100,300);
        zahrej(262,100,300);
        zahrej(262,100,100);
        zahrej(294,100,300);
        zahrej(330,200,50);
        zahrej(262,200,50);
        zahrej(220,200,50);
        zahrej(196,100,700);
        /*Intro*/
        zahrej(330,100,100);
        zahrej(330,100,300);
        zahrej(330,100,300);
        zahrej(262,100,100);
        zahrej(330,100,300);
        zahrej(392,100,700);
        zahrej(196,100,700);
        zahrej(196,100,125);
        zahrej(262,100,125);
        zahrej(330,100,125);
        zahrej(392,100,125);
        zahrej(523,100,125);
        zahrej(660,100,125);
        zahrej(784,100,575);
        zahrej(660,100,575);
        zahrej(207,100,125);
        zahrej(262,100,125);
        zahrej(311,100,125);
        zahrej(415,100,125);
        zahrej(523,100,125);
        zahrej(622,100,125);
        zahrej(830,100,575);
        zahrej(622,100,575);
        zahrej(233,100,125);
        zahrej(294,100,125);
        zahrej(349,100,125);
        zahrej(466,100,125);
        zahrej(587,100,125);
        zahrej(698,100,125);
        zahrej(932,100,575);
        zahrej(932,100,125);
        zahrej(932,100,125);
        zahrej(932,100,125);
        zahrej(1046,675,0);
}

//melodie Imperial March
void play()
{
	// frekvence prevzaty z http://processors.wiki.ti.com/index.php/Playing_The_Imperial_March
    zahrej(a, 500, 20);
    zahrej(a, 500, 20);
    zahrej(a, 500, 20);
    zahrej(f, 350, 20);
    zahrej(cH, 150, 20);
    zahrej(a, 500, 20);
    zahrej(f, 350, 20);
    zahrej(cH, 150, 20);
    zahrej(a, 650, 20);
	
	y_scale = 0;
    delay_ms(150);
	y_scale = 100;
    //end of first bit
 
    zahrej(eH, 500, 20);
    zahrej(eH, 500, 20);
    zahrej(eH, 500, 20);
    zahrej(fH, 350, 20);
    zahrej(cH, 150, 20);
    zahrej(gS, 500, 20);
    zahrej(f, 350, 20);
    zahrej(cH, 150, 20);
    zahrej(a, 650, 20);
 
    y_scale = 0;
    delay_ms(150);
	y_scale = 100;
    //end of second bit...
 
    zahrej(aH, 500, 20);
    zahrej(a, 300, 20);
    zahrej(a, 150, 20);
    zahrej(aH, 400, 20);
    zahrej(gSH, 200, 20);
    zahrej(gH, 200, 20);
    zahrej(fSH, 125, 20);
    zahrej(fH, 125, 20);
    zahrej(fSH, 250, 20);
 
    y_scale = 0;
    delay_ms(250);
	y_scale = 100;
 
    zahrej(aS, 250, 20);
    zahrej(dSH, 400, 20);
    zahrej(dH, 200, 20);
    zahrej(cSH, 200, 20);
    zahrej(cH, 125, 20);
    zahrej(b, 125, 20);
    zahrej(cH, 250, 20);
 
    y_scale = 0;
    delay_ms(250);
	y_scale = 100;
 
    zahrej(f, 125, 20);
    zahrej(gS, 500, 20);
    zahrej(f, 375, 20);
    zahrej(a, 125, 20);
    zahrej(cH, 500, 20);
    zahrej(a, 375, 20);
    zahrej(cH, 125, 20);
    zahrej(eH, 650, 20);
 
    //end of third bit... (Though it doesn't play well)
    //let's repeat it
 
    zahrej(aH, 500, 20);
    zahrej(a, 300, 20);
    zahrej(a, 150, 20);
    zahrej(aH, 400, 20);
    zahrej(gSH, 200, 20);
    zahrej(gH, 200, 20);
    zahrej(fSH, 125, 20);
    zahrej(fH, 125, 20);
    zahrej(fSH, 250, 20);
 
    y_scale = 0;
    delay_ms(250);
	y_scale = 100;
 
    zahrej(aS, 250, 20);
    zahrej(dSH, 400, 20);
    zahrej(dH, 200, 20);
    zahrej(cSH, 200, 20);
    zahrej(cH, 125, 20);
    zahrej(b, 125, 20);
    zahrej(cH, 250, 20);
 
    y_scale = 0;
    delay_ms(250);
	y_scale = 100;
 
    zahrej(f, 250, 20);
    zahrej(gS, 500, 20);
    zahrej(f, 375, 20);
    zahrej(cH, 125, 20);
    zahrej(a, 500, 20);
    zahrej(f, 375, 20);
    zahrej(cH, 125, 20);
    zahrej(a, 650, 20);
    //end of the song
	y_scale = 0;
 
}

// dekodovani stisknutych klaves, nastaveni frekvenci pro ton
char tone_decode(unsigned int in_ch)
{

  if (in_ch & KEY_1){
  //    RS232_SendChar('1');
      frequency = 32.7032 * octava;
      return 'C';
	  
  }


 if (in_ch & KEY_2){
  //    RS232_SendChar('2');
  frequency =  36.7081 * octava;
      return 'D';
  }

 if (in_ch & KEY_3){
  //    RS232_SendChar('3');
  frequency = 41.2034 * octava;
      return 'E';
  }

 if (in_ch & KEY_4){
  //    RS232_SendChar('5');
  frequency = 43.6535 * octava;
      return 'F';
  }

 if (in_ch & KEY_5){
	   frequency = 48.9994 * octava;
      return 'G';
  }

 if (in_ch & KEY_6){
	  frequency = 55.0000 * octava;
      return 'A';
  }

 if (in_ch & KEY_7){
	  frequency = 61.7354 * octava;
      return 'H';
  }

 if (in_ch & KEY_8){
      return 'I';
  }

 if (in_ch & KEY_9){
      return 'M';
  }

 if (in_ch & KEY_A){
	 octava = OCT_1;
	 octava_msg = '1';
      return '1';
  }

 if (in_ch & KEY_B){
	 octava = OCT_2;
	  octava_msg = '2';
      return '2';
  }

 if (in_ch & KEY_C){
	 octava = OCT_3;
	  octava_msg = '3';
      return '3';
  }

 if (in_ch & KEY_D){
	  octava = OCT_4;
	   octava_msg = '4';
      return '4';
  }
  
   if (in_ch & KEY_h){
	    octava = OCT_7;
		 octava_msg = '7';
      return '7';
  }

 if (in_ch & KEY_m){
	  octava = OCT_5;
	   octava_msg = '5';
      return '5';
  }
   if (in_ch & KEY_0){
	    octava = OCT_6;
		 octava_msg = '6';
      return '6';
  }

  return 0;
}


/*******************************************************************************
 * Obsluha klavesnice
*******************************************************************************/
int keyboard_idle()
{
	
  char ch = tone_decode(read_word_keyboard_4x4());;
  
  if (ch != last_ch) 
  {
	
    last_ch = ch;
	
    if (ch != 0 && ch > 64 && ch != 'M' && ch != 'I') 
    {
      LCD_clear();
	  LCD_append_string("Ton: ");
	  LCD_append_char(ch);
	  LCD_append_char(octava_msg);
	  
	  set_ticks();
	  
	  y_scale = 100;
	  if(delay)
	  {
		delay_ms(500);
		y_scale = 0;
	  }
    }
	else
	{
		y_scale = 0;
		if(ch != 'M' && ch >= '1' && ch <= '7' && ch != 'I')
		{
			LCD_clear();
			LCD_append_string("Octava ");
			LCD_append_char(ch);
		}
		else if(ch == 'I')
		{
			LCD_clear();
			LCD_append_string("Hraje: Imperial March");
			play();
		}
		else if(ch == 'M')
		{
			LCD_clear();
			LCD_append_string("Hraje: Mario");
			play3();
		}
		else
		{
			LCD_clear();
			LCD_append_string("Ton: ");
		}
			
	}
  }
  return 0;
}



void set_ticks(void);
void set_freq_max(void);

/*******************************************************************************
 * Dekodovani a vykonani uzivatelskych prikazu
*******************************************************************************/
unsigned char decode_user_cmd(char *UString, char *String)
{	 
	if (strcmp2(UString, "SW"))  // Imperial March
	{     
            LCD_clear();
			LCD_append_string("Hraje: Imperial March");
			play();
    }
    else if (strcmp5(UString, "MARIO"))  // Mario
	{     
		LCD_clear();
			LCD_append_string("Hraje: Mario");
			play3();
  }
  else if (strcmp5(UString, "DELAY"))  // Mario
	{     
		LCD_clear();
	    LCD_append_string("Zapnuta prodleva tonu");
		delay = 1;
  }
	else { return (CMD_UNKNOWN); }

  return USER_COMMAND;
}

/*******************************************************************************
 * Inicializace periferii/komponent po naprogramovani FPGA
*******************************************************************************/
void fpga_initialized()
{
   LCD_init();
   LCD_append_string("Klavir");
   LCD_send_cmd(LCD_DISPLAY_ON_OFF | LCD_DISPLAY_ON | LCD_CURSOR_OFF, 0);  // vypni kurzor

  term_send_str_crlf("Ovladani piana (viz help).");
  term_send_str_crlf("Reproduktor pripojte na vyvod 31 (konektoru JP9) a zem FITkitu");
}


/*******************************************************************************
 * Obsluha preruseni casovace timer A0
*******************************************************************************/
interrupt (TIMERA0_VECTOR) Timer_A (void)
{  
  if (++signal_arr_index >= samples){ signal_arr_index = 0; }
  unsigned int smpl = (signal_arr[signal_arr_index]*y_scale)/100;
  DAC12_0DAT = smpl;  // nahrani dalsiho vzorku pro prevod
  
  CCR0 += ticks;  // nastav po kolika ticich ma dojit k dalsimu preruseni
}

/*******************************************************************************
 * Funkce nastavi pocet tiku (do promenne ticks), za kolik dojde k preruseni
 * od casovace na zaklade zadane frekvence a funkce (jejich vzorku) 
*******************************************************************************/
void set_ticks(void)
{
  ticks = TICKS_PER_SECOND / frequency / samples;
}

/*******************************************************************************
 * Funkce nastavi do promenne freq_max maximalni hodnotu frekvence pro zvoleny signal
*******************************************************************************/
void set_freq_max(void)
{
  // maximalni frekvence je odvozena od poctu vzorku zvoleneho signalu
  freq_max = TICKS_PER_SECOND / samples / 2;
}


/*******************************************************************************
 * Hlavni funkce
*******************************************************************************/
int main(void)
{
  initialize_hardware();
  WDTCTL = WDTPW + WDTHOLD;	 // zastav watchdog

  // nastaveni maximalni frekvence signalu a poctu tiku do preruseni
  set_freq_max();
  set_ticks();
  
  // nastaveni casovace
	CCTL0 = CCIE;  // povol preruseni pro casovac (rezim vystupni komparace) 
  CCR0 = ticks; // nastav po kolika ticich ma dojit k preruseni
  TACTL = TASSEL_1 + MC_2;  // ACLK (f_tiku = 32768 Hz = 0x8000 Hz), nepretrzity rezim

  // nastaveni DAC
  ADC12CTL0 |= 0x0020;    // nastaveni referencniho napeti z ADC na 1,5 V
  DAC12_0CTL |= 0x1060;   // nastaveni kontrolniho registru DAC (8-bitovy rezim, medium speed)
  DAC12_0CTL |= 0x100;    // vystupni napeti nasobit 1x
  DAC12_0DAT = 0;         // vynulovani vystupni hodnoty DAC

  // hlavni smycka
  while (1)
	{
	keyboard_idle();      // obsluha klavesnice
    terminal_idle();      // obsluha terminalu
  }
}
