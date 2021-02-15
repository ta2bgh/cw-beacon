#include "si5351.h"

#define PIN_SP 10
#define PIN_TX 13

Si5351 si5351;

int32_t cal_factor = 68000;    // Your calibration value. Please see si5351_calibration in examples
uint16_t duration = 85;    // Morse code typing speed in milliseconds - higher number means slower typing
uint16_t hz = 750;         // Volume up if a high ohm speaker connected to circuit of the morse generator
uint64_t target_freq = 14448000000ULL; // 10 MHz, in hundredths of hertz
String cw_message = "VVV de TA2BGH  LOCATOR IS KM59gs  PWR IS 10mW  ANT IS HANDMADE VERTICAL";   // Your message
//----------
void setup() {
  pinMode(PIN_TX, OUTPUT);

  si5351.init(SI5351_CRYSTAL_LOAD_8PF, 0, 0);
  si5351.set_correction(cal_factor, SI5351_PLL_INPUT_XO);
  si5351.set_pll(SI5351_PLL_FIXED, SI5351_PLLA);
  si5351.set_freq(target_freq, SI5351_CLK0);
  }
//----------
void loop() {
  cw_string_proc(cw_message);
  delay(500);                           // Duration of the break at the end before the long signal - in milliseconds

  cw(true);
  delay(5000);                         // Duration of the long signal at the end - in milliseconds

  cw(false);
  delay(1000);                          // Duration of the pause at the end after the long signal - in milliseconds
}
//----------
void cw_string_proc(String str) {                      // Processing string to characters
  for (uint8_t j = 0; j < str.length(); j++)
    cw_char_proc(str[j]);
}
//----------
void cw_char_proc(char m) {                            // Processing characters to Morse symbols
  String s;

  if (m == ' ') {                                      // Pause between words
    word_space();
    return;
  }

  if (m > 96)                                          // ACSII, case change a-z to A-Z
    if (m < 123)
      m -= 32;

  switch (m) {                                         // Morse
    case 'A': s = ".-#";     break;
    case 'B': s = "-...#";   break;
    case 'C': s = "-.-.#";   break;
    case 'D': s = "-..#";    break;
    case 'E': s = ".#";      break;
    case 'F': s = "..-.#";   break;
    case 'G': s = "--.#";    break;
    case 'H': s = "....#";   break;
    case 'I': s = "..#";     break;
    case 'J': s = ".---#";   break;
    case 'K': s = "-.-#";    break;
    case 'L': s = ".-..#";   break;
    case 'M': s = "--#";     break;
    case 'N': s = "-.#";     break;
    case 'O': s = "---#";    break;
    case 'P': s = ".--.#";   break;
    case 'Q': s = "--.-#";   break;
    case 'R': s = ".-.#";    break;
    case 'S': s = "...#";    break;
    case 'T': s = "-#";      break;
    case 'U': s = "..-#";    break;
    case 'V': s = "...-#";   break;
    case 'W': s = ".--#";    break;
    case 'X': s = "-..-#";   break;
    case 'Y': s = "-.--#";   break;
    case 'Z': s = "--..#";   break;

    case '1': s = ".----#";  break;
    case '2': s = "..---#";  break;
    case '3': s = "...--#";  break;
    case '4': s = "....-#";  break;
    case '5': s = ".....#";  break;
    case '6': s = "-....#";  break;
    case '7': s = "--...#";  break;
    case '8': s = "---..#";  break;
    case '9': s = "----.#";  break;
    case '0': s = "-----#";  break;

    case '?': s = "..--..#"; break;
    case '=': s = "-...-#"; break;
    case ',': s = "--..--#"; break;
    case '/': s = "-..-.#";  break;
  }

  for (uint8_t i = 0; i < 7; i++) {
    switch (s[i]) {
      case '.': di();  break;                          // di
      case '-': dah();  break;                          // dah
      case '#': char_space(); return;                  // end of morse symbol
    }
  }
}
//----------
void di() {
  cw(true);                                            // TX di
  delay(duration);

  cw(false);                                           // stop TX di
  delay(duration);
}
//----------
void dah() {
  cw(true);                                            // TX dah
  delay(3 * duration);

  cw(false);                                           // stop TX dah
  delay(duration);
}
//----------
void char_space() {                                    // 3x, pause between letters
  delay(2 * duration);                                 // 1x from end of character + 2x from the beginning of new character
}
//----------
void word_space() {                                    // 7x, pause between words
  delay(6 * duration);                                 // 1x from end of the word + 6x from the beginning of new word
}
//----------
void cw(bool state) {                                  // TX-CW, TX-LED, 750 Hz sound
  if (state) {
    si5351.output_enable(SI5351_CLK0, 1);
    digitalWrite(PIN_TX, HIGH);
    tone(PIN_SP, hz);
  }
  else {
    si5351.output_enable(SI5351_CLK0, 0);
    digitalWrite(PIN_TX, LOW);
    noTone(PIN_SP);
  }
}
