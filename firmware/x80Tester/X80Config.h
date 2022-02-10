#ifndef X80Config_h
#define X80Config_h

// Switch input 1 ("pass-through"), connector J4 sleeve
#define SWITCH_INPUT_ONE      A4

// Switch input 2, connector J3 sleeve 
#define SWITCH_INPUT_TWO      A3

// Switch input 3, connector J2 sleeve
#define SWITCH_INPUT_THREE    A2  // A2 or 12

// Switch input 4; connector J1 sleeve
#define SWITCH_INPUT_FOUR     A1

// Switch input 5; connector J1 ring 1 (requires splitter cable)
#define SWITCH_INPUT_FIVE     A0

// Switch output 1; connector J8
#define SWITCH_OUTPUT_ONE     7         // This is incorrectly shown as 8 on PCB

// Switch output 2; connector J9
#define SWITCH_OUTPUT_TWO     9

// Switch output 3; connector J10
#define SWITCH_OUTPUT_THREE     10

// Switch output 4; connector J11
#define SWITCH_OUTPUT_FOUR     11

// Buzzer for Morse code
#define BUZZER_PIN            13

# define POT_PIN              A5

// Potentiometer used for setting
#define SETTING_POTENTIOMETER A5

// Relay driver to activate and deactivate pass-through switch
#define RELAY_DRIVER          12

#define SWITCH_OUTPUT_ONE_OFF         digitalWrite(SWITCH_OUTPUT_ONE, LOW)
#define SWITCH_OUTPUT_ONE_ON          digitalWrite(SWITCH_OUTPUT_ONE, HIGH)
#define SWITCH_OUTPUT_TWO_OFF         digitalWrite(SWITCH_OUTPUT_TWO, LOW)
#define SWITCH_OUTPUT_TWO_ON          digitalWrite(SWITCH_OUTPUT_TWO, HIGH)
#define SWITCH_OUTPUT_THREE_OFF       digitalWrite(SWITCH_OUTPUT_THREE, LOW)
#define SWITCH_OUTPUT_THREE_ON        digitalWrite(SWITCH_OUTPUT_THREE, HIGH)
#define SWITCH_OUTPUT_FOUR_OFF        digitalWrite(SWITCH_OUTPUT_FOUR, LOW)
#define SWITCH_OUTPUT_FOUR_ON         digitalWrite(SWITCH_OUTPUT_FOUR, HIGH)

// Recommended to leave this off connected if switches output not being used 
// as it will use power which is unecessary
#define SWITCH_ONE_PASSTHROUGH_DISCONNECT        digitalWrite(RELAY_DRIVER, HIGH);
#define SWITCH_ONE_PASSTHROUGH_CONNECT           digitalWrite(RELAY_DRIVER, LOW);  // This will connect switch one in to switch 1 out

#define AUDIBLE_INFORMATION         1
#define AUDIBLE_WARNING             2


#endif
