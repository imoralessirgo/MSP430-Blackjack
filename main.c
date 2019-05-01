/************** ECE2049 DEMO CODE ******************/
/************** 14 May 2018   **********************/
/***************************************************/

#include <msp430.h>

/* Peripherals.c and .h are where the functions that implement
 * the LEDs and keypad, etc are. It is often useful to organize
 * your code by putting like functions together in files.
 * You include the header associated with that file(s)
 * into the main file of your project. */
#include "peripherals.h"
#include "utils/test_runner.h"
#include <stdlib.h>
#include "my_deck.h"  // holds Deck of cards
#include "shuffle.h"   // function for index shuffle
#include "display.h"  // function for card display
#include "score.h"  // function to add up scores

// Function Prototypes
void swDelay(char numLoops);

// Declare globals here

#define max_cards (10)

// Main
void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;    // Stop watchdog timer. Always need to stop this!!
                                 // You can then configure it properly, if desired

    // *** System initialization ***
    initLeds();
    configDisplay();
    configKeypad();

    Graphics_clearDisplay(&g_sContext); // Clear the display

    // Write some text to the display
    Graphics_drawStringCentered(&g_sContext, "MSP430",  AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext, "Blackjack",       AUTO_STRING_LENGTH, 48, 25, TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext, "Press #", AUTO_STRING_LENGTH, 48, 55, TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext, "to start", AUTO_STRING_LENGTH, 48, 65, TRANSPARENT_TEXT);

    // Draw a box around everything because it looks nice
    Graphics_Rectangle box = {.xMin = 5, .xMax = 91, .yMin = 5, .yMax = 91 };
    Graphics_drawRectangle(&g_sContext, &box);

    // We are now done writing to the display.  However, if we stopped here, we would not
    // see any changes on the actual LCD.  This is because we need to send our changes
    // to the LCD, which then refreshes the display.
    // Since this is a slow operation, it is best to refresh (or "flush") only after
    // we are done drawing everything we need.
    Graphics_flushBuffer(&g_sContext);

    int state, key_pressed, last_state, new_state;
           state=0;

    while (1)    // Forever loop
    {
        key_pressed = getKey();
        if (state != last_state)
            new_state = 1;
        else
            new_state=0;
        last_state = state;
        switch(state)
        {
        case 0: // Welcome screen
            if (new_state){
                Graphics_clearDisplay(&g_sContext); // Clear the display
                Graphics_drawStringCentered(&g_sContext, "MSP430",  AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);
                Graphics_drawStringCentered(&g_sContext, "Blackjack",       AUTO_STRING_LENGTH, 48, 25, TRANSPARENT_TEXT);
                Graphics_drawStringCentered(&g_sContext, "Press #", AUTO_STRING_LENGTH, 48, 55, TRANSPARENT_TEXT);
                Graphics_drawStringCentered(&g_sContext, "to start", AUTO_STRING_LENGTH, 48, 65, TRANSPARENT_TEXT);
                Graphics_flushBuffer(&g_sContext);
            }
            int sp=0,sd=0,x,y;
            int d=0,p=0,i=0; //array position
            int player[max_cards]= {0,0,0,0,0,0,0,0,0,0};
            int dealer[max_cards]= {0,0,0,0,0,0,0,0,0,0};
            if (key_pressed == '#'){
                swDelay(1);
                state= 1;
                key_pressed=0;
            }
            break;
        case 1: // Shuffle
            if(new_state){
            Graphics_clearDisplay(&g_sContext); // Clear the display
            Graphics_drawStringCentered(&g_sContext, "Cut the deck",       AUTO_STRING_LENGTH, 48, 25, TRANSPARENT_TEXT);
            Graphics_drawStringCentered(&g_sContext, "Press any number", AUTO_STRING_LENGTH, 48, 55, TRANSPARENT_TEXT);
            Graphics_flushBuffer(&g_sContext);
            }
            if(key_pressed){
               shuffle(index,52,key_pressed); // definition in shuffle.c --- prototype in shuffle.h
               state = 2;
            }
            break;
        case 2: // Dealing
            if (new_state)
            {
                Graphics_clearDisplay(&g_sContext); // Clear the display
                Graphics_drawStringCentered(&g_sContext, "You",
                                            AUTO_STRING_LENGTH, 24, 5,
                                            TRANSPARENT_TEXT);
                Graphics_drawStringCentered(&g_sContext, "Dealer",
                                            AUTO_STRING_LENGTH, 74, 5,
                                            TRANSPARENT_TEXT);
                Graphics_drawStringCentered(&g_sContext, "|",
                                            AUTO_STRING_LENGTH, 48, 5,
                                            TRANSPARENT_TEXT);
                Graphics_drawStringCentered(&g_sContext, "_________________",
                                            AUTO_STRING_LENGTH, 48, 8,
                                            TRANSPARENT_TEXT);
                make_string(myDeck[index[i]].suit, myDeck[index[i]].rank, 24,
                            20);
                player[p] = myDeck[index[i]].val;
                p++;
                i++;
                make_string(myDeck[index[i]].suit, myDeck[index[i]].rank, 24,
                            30);
                player[p] = myDeck[index[i]].val;
                p++;
                i++;
                make_string(myDeck[index[i]].suit, myDeck[index[i]].rank, 74,
                            20);
                dealer[d] = myDeck[index[i]].val;
                d++;
                i++;
                Graphics_drawStringCentered(&g_sContext, "* Hit",
                AUTO_STRING_LENGTH,
                                            24, 90,
                                            TRANSPARENT_TEXT);
                Graphics_drawStringCentered(&g_sContext, "# Stay",
                AUTO_STRING_LENGTH,
                                            74, 90,
                                            TRANSPARENT_TEXT);
                Graphics_flushBuffer(&g_sContext);
                sp = sum_all(player);
                sd = sum_all(dealer);
                state = 3;
            }
            break;
        case 3: // Player's turn
            if (new_state){
                x=24,y=40;
            }
            while (state == 3)
            {
                key_pressed= getKey();
                if(sp<=10){
                    setLeds(0x1);
                }else if(sp<=15){
                    setLeds(0x4);
                }else if(sp<=18){
                    setLeds(BIT3);
                }else if(sp==21){
                    state=6;
                }
                if((key_pressed == '*') && (p < 10) && (sp < 22))
                {
                    player[p] = myDeck[index[i]].val;
                    make_string(myDeck[index[i]].suit, myDeck[index[i]].rank, x,y);
                    Graphics_flushBuffer(&g_sContext);
                    sp =sum_all(player), p++, i++, y += 10;

                }else if (sp>21){
                    state=5;
                }else if ((key_pressed == '#') || (p == 10))
                {
                    state = 4;
                }
                swDelay(1);
                setLeds(0x0);
            }
            break;
        case 4: // Dealer's turn
            for(d=1, x=74,y= 30; (d < 10)  && (sd < 17); sd = sum_all(dealer),d++,i++,y+=10){
                dealer[d] = myDeck[index[i]].val;
                make_string(myDeck[index[i]].suit, myDeck[index[i]].rank, x,y);
                Graphics_flushBuffer(&g_sContext);
            }
            fix_A(player,sp);
            swDelay(2);
            if(sd>21){
                state=6;
            }else if (sd>sp){
                state=5;
            }else if (sd<sp){
                state=6;
            }else if(sd==sp){
                state=7;
            }
            break;
        case 5: // user lost
            if(new_state){
                Graphics_clearDisplay(&g_sContext); // Clear the display
                Graphics_drawStringCentered(&g_sContext, "You",  AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);
                Graphics_drawStringCentered(&g_sContext, "LOST!",       AUTO_STRING_LENGTH, 48, 25, TRANSPARENT_TEXT);
                Graphics_drawStringCentered(&g_sContext, "Press 0", AUTO_STRING_LENGTH, 48, 55, TRANSPARENT_TEXT);
                Graphics_drawStringCentered(&g_sContext, "to play again!", AUTO_STRING_LENGTH, 48, 65, TRANSPARENT_TEXT);
                Graphics_flushBuffer(&g_sContext);
            }
            BuzzerOn();
            swDelay(3);
            BuzzerOff();
            swDelay(3);
            if(key_pressed == '0'){
            state=0;
            }
            break;
        case 6: // user won
            if(new_state){
                Graphics_clearDisplay(&g_sContext); // Clear the display
                Graphics_drawStringCentered(&g_sContext, "You",  AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);
                Graphics_drawStringCentered(&g_sContext, "WON!",       AUTO_STRING_LENGTH, 48, 25, TRANSPARENT_TEXT);
                Graphics_drawStringCentered(&g_sContext, "Press 0", AUTO_STRING_LENGTH, 48, 55, TRANSPARENT_TEXT);
                Graphics_drawStringCentered(&g_sContext, "to play again!", AUTO_STRING_LENGTH, 48, 65, TRANSPARENT_TEXT);
                Graphics_flushBuffer(&g_sContext);
            }
            BuzzerOn();
            BuzzerOff();
            if(key_pressed== '0'){
            state=0;
            }
            break;
        case 7: // tie
                    if(new_state){
                        Graphics_clearDisplay(&g_sContext); // Clear the display
                        Graphics_drawStringCentered(&g_sContext, "It's a",  AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);
                        Graphics_drawStringCentered(&g_sContext, "TIE!",       AUTO_STRING_LENGTH, 48, 25, TRANSPARENT_TEXT);
                        Graphics_drawStringCentered(&g_sContext, "Press 0", AUTO_STRING_LENGTH, 48, 55, TRANSPARENT_TEXT);
                        Graphics_drawStringCentered(&g_sContext, "to play again!", AUTO_STRING_LENGTH, 48, 65, TRANSPARENT_TEXT);
                        Graphics_flushBuffer(&g_sContext);
                    }
                    if(key_pressed == '0'){
                    state=0;
                    }
                    break;
        }
    }  // end while (1)
}


void swDelay(char numLoops)
{
	// This function is a software delay. It performs
	// useless loops to waste a bit of time
	//
	// Input: numLoops = number of delay loops to execute
	// Output: none
	//
	// smj, ECE2049, 25 Aug 2013

	volatile unsigned int i,j;	// volatile to prevent removal in optimization
			                    // by compiler. Functionally this is useless code

	for (j=0; j<numLoops; j++)
    {
    	i = 50000 ;					// SW Delay
   	    while (i > 0)				// could also have used while (i)
	       i--;
    }
}
