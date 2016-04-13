/*
 * TcpContinuousWave.ino
 *
 * A TCP server for Arduino with Ethernet shield that beeps
 * characters sent to the server as International Morse Code. See the
 * README.md file for more information.
 */

/*
* Copyright (c) 2013 Alicia M. F. Key
* 
* Permission is hereby granted, free of charge, to any person obtaining a 
* copy of this software and associated documentation files (the 
* "Software"), to deal in the Software without restriction, including 
* without limitation the rights to use, copy, modify, merge, publish, 
* distribute, sublicense, and/or sell copies of the Software, and to 
* permit persons to whom the Software is furnished to do so, subject to 
* the following conditions:
* 
* The above copyright notice and this permission notice shall be included 
* in all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY 
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*** BEGIN included libraries ***/

#include <SPI.h>
#include <Ethernet.h>

/*** END included libraries ***/


/*** BEGIN hardware configuration preprocessor directives ***/

// Pins where CW keyer components are hooked up
#define CW_VISUAL_KEYER_PIN 7
#define CW_AUDIO_KEYER_PIN 9
#define CW_AUDIO_FREQUENCY 880

// Pins where the status LED is hooked up
// See lightRGB() function below
#define RED_PIN 3
#define GREEN_PIN 5
#define BLUE_PIN 6

// Codes for driving the RGB status LED
// See lightRGB() function below
#define RED 1
#define GREEN 2
#define BLUE 3
#define YELLOW 4
#define MAGENTA 5
#define CYAN 6
#define BLACK 7
#define WHITE 8

/*** END hardware configuration preprocessor directives ***/


/*** BEGIN software configuration ***/

// CW_TICK_TIME_MILLISECONDS sets the fundamental "tick" time in for the
// morse code timing. Esseintially, it sets the words per minute WPM that
// it beeps out of the speaker.
//
// 56 ms is around 19 WPM, which is fairly fast
// 220 ms is closer to 6 WPM, better if you do not have prior experience
// with CW
#define CW_TICK_TIME_MILLISECONDS 56

// Enables or disable the visual and audio keyers
boolean visual_keyer_enabled = true;
boolean audio_keyer_enabled = true;

// Network settings. See http://arduino.cc/en/Reference/Ethernet
// For more information beyond comments here.
// Note that a statuc IP is used here for simplicity in this
// version 1.0

// Listen on port 73. You don't need to change this
EthernetServer server(73);

// Change this for the MAC address on your Ethernet board
byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0xC6, 0x1B };

// Chnage to the gateway on your network
IPAddress gateway(192, 168, 0, 1);

// The netmask on your network (likely doesn't need to be changed)
IPAddress subnet(255, 255, 255, 0);

// The actual IP address of the device.
IPAddress ip(192, 168, 0, 73);

/*** END software configuration ***/


/*** BEGIN setup and loop scheduling ***/

void setup() {                
  setupRgbLed();
  lightRGB(RED);
  setupCwKeyer();
  setupTcpServer();
  Serial.begin(9600);
  lightRGB(BLUE);
}

void setupTcpServer() {
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();
}


// Run the server loop
void loop() {
  EthernetClient client = server.available();
  if (client) {
    while (client.connected()) {
      lightRGB(GREEN);
      if(client.available()) {
        char symbol = client.read();
        if (symbol == '\r') {
          keyCwForCharacter(' ');
        } else if (symbol != '\n') {
          keyCwForCharacter(symbol);
        }
      }
    }
    lightRGB(BLUE);
    client.stop();
  }
}

/*** END setup and loop scheduling ***/


/*** BEGIN function to drive status LED ***/

void setupRgbLed() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);       
}

void lightRGB(int color) {
  switch(color) {
    case RED:
      digitalWrite(RED_PIN, HIGH);
      digitalWrite(GREEN_PIN, LOW);
      digitalWrite(BLUE_PIN, LOW);
      break;
      
    case GREEN:
      digitalWrite(RED_PIN, LOW);
      digitalWrite(GREEN_PIN, HIGH);
      digitalWrite(BLUE_PIN, LOW);
      break;
      
    case BLUE:
      digitalWrite(RED_PIN, LOW);
      digitalWrite(GREEN_PIN, LOW);
      digitalWrite(BLUE_PIN, HIGH);
      break;
      
    case YELLOW:
      digitalWrite(RED_PIN, HIGH);
      digitalWrite(GREEN_PIN, HIGH);
      digitalWrite(BLUE_PIN, LOW);
      break;
      
    case CYAN:
      digitalWrite(RED_PIN, LOW);
      digitalWrite(GREEN_PIN, HIGH);
      digitalWrite(BLUE_PIN, HIGH);
      break;
      
    case MAGENTA:
      digitalWrite(RED_PIN, HIGH);
      digitalWrite(GREEN_PIN, LOW);
      digitalWrite(BLUE_PIN, HIGH);
      break;
      
    case WHITE:
      digitalWrite(RED_PIN, HIGH);
      digitalWrite(GREEN_PIN, HIGH);
      digitalWrite(BLUE_PIN, HIGH);
      break;
    
    // Intentional drop-through
    case BLACK:
    default:
      digitalWrite(RED_PIN, LOW);
      digitalWrite(GREEN_PIN, LOW);
      digitalWrite(BLUE_PIN, LOW);
      break;
  }
}
/*** END function to handle status LED ***/


/*** BEGIN CW morse code lookup and keying functions ***/

// sets up the keyers system according to parameters at topd of file
void setupCwKeyer() {
  pinMode(CW_VISUAL_KEYER_PIN, OUTPUT);       
  pinMode(CW_AUDIO_KEYER_PIN, OUTPUT);       
}

// Translates a symbol into a morse code string
char *morseForSymbol(char symbol) {

  char *morse = NULL;

  if (symbol == '.') {
    morse = ".-.-.-";

  
  } else if (symbol == 'a' || symbol == 'A') {
    morse = ".-";
    
  } else if (symbol == 'b' || symbol == 'B') {
    morse = "-...";

  } else if (symbol == 'c' || symbol == 'C') {
    morse = "-.-.";

  } else if (symbol == 'd' || symbol == 'D') {
    morse = "-..";

  } else if (symbol == 'e' || symbol == 'E') {
    morse = ".";

  } else if (symbol == 'f' || symbol == 'F') {
    morse = "..-.";

  } else if (symbol == 'g' || symbol == 'G') {
    morse = "--.";

  } else if (symbol == 'h' || symbol == 'H') {
    morse = "....";

  } else if (symbol == 'i' || symbol == 'I') {
    morse = "..";

  } else if (symbol == 'j' || symbol == 'J') {
    morse = ".---";

  } else if (symbol == 'k' || symbol == 'K') {
    morse = "-.-";

  } else if (symbol == 'l' || symbol == 'L') {
    morse = ".-..";

  } else if (symbol == 'm' || symbol == 'M') {
    morse = "--";

  } else if (symbol == 'n' || symbol == 'N') {
    morse = "-.";

  } else if (symbol == 'o' || symbol == 'O') {
    morse = "---";

  } else if (symbol == 'p' || symbol == 'P') {
    morse = ".--.";

  } else if (symbol == 'q' || symbol == 'Q') {
    morse = "--.-";

  } else if (symbol == 'r' || symbol == 'R') {
    morse = ".-.";

  } else if (symbol == 's' || symbol == 'S') {
    morse = "...";

  } else if (symbol == 't' || symbol == 'T') {
    morse = "-";

  } else if (symbol == 'u' || symbol == 'U') {
    morse = "..-";

  } else if (symbol == 'v' || symbol == 'V') {
    morse = "...-";

  } else if (symbol == 'w' || symbol == 'W') {
    morse = ".--";

  } else if (symbol == 'x' || symbol == 'X') {
    morse = "-..-";

  } else if (symbol == 'y' || symbol == 'Y') {
    morse = "-.--";

  } else if (symbol == 'z' || symbol == 'Z') {
    morse = "--..";

  } else if (symbol == '0') {
    morse = "-----";

  } else if (symbol == '1') {
    morse = ".----";

  } else if (symbol == '2') {
    morse = "..---";

  } else if (symbol == '3') {
    morse = "...--";

  } else if (symbol == '4') {
    morse = "....-";

  } else if (symbol == '5') {
    morse = ".....";

  } else if (symbol == '6') {
    morse = "-....";

  } else if (symbol == '7') {
    morse = "--...";

  } else if (symbol == '8') {
    morse = "---..";

  } else if (symbol == '9') {
    morse = "----.";

  } else if (symbol == '?') {
    morse = "..--..";

  } else if (symbol == '=') {
    morse = "-...-";

  } else if (symbol == '/') {
    morse = "-..-.";

  } else if (symbol == '+') {
    morse = ".-.-.";

  } else if (symbol == ',') {
    morse = "--..--";

  } else {
    // For anything else, return a question mark
    morse = "..--.."; 
  }

  return morse;
}

void keyCwForCharacter(char symbol) {

  // CW Tick Time affects words per minute speed see
  // keyCwForCharacter() function below.
  int t_tick = CW_TICK_TIME_MILLISECONDS;

  // Length of dot, dash, between characters, between words
  int t_dot = t_tick * 1;
  int t_dash = t_tick * 3;
  int t_between_char = t_tick * 3;
  int t_between_words = t_tick * 7;

  Serial.print(symbol);
  if (symbol == ' ') {
    Serial.println("SPACE");
    delay(t_between_words);
  } else {
    char *symbol_in_morse = morseForSymbol(symbol);
    Serial.println(symbol_in_morse);
    int j_len = strlen(symbol_in_morse);
    int j = 0;
    Serial.println(symbol_in_morse);

    for (j=0; j<j_len; j++) {
      char dotdash = *(symbol_in_morse + j);
      int t_delay = 0;
      if (dotdash == '.') {
        t_delay = t_dot;
      } else if (dotdash == '-') {
        t_delay = t_dash;
      } else {
        t_delay = t_tick;
      }
      keyPress();
      delay(t_delay);
      keyRelease();
      delay(t_tick);
    }
    delay(t_between_char);
  }
}

// Turns ON the sound and visual indicator
void keyPress() {
  if (audio_keyer_enabled) {
    tone(CW_AUDIO_KEYER_PIN, CW_AUDIO_FREQUENCY);
  }
  if (visual_keyer_enabled) {
    digitalWrite(CW_VISUAL_KEYER_PIN, HIGH);
  }
}

// Turns OFF the sound and visual indicator
void keyRelease() {
  if (audio_keyer_enabled) {
    noTone(CW_AUDIO_KEYER_PIN);
  }
  if (visual_keyer_enabled) {
    digitalWrite(CW_VISUAL_KEYER_PIN, LOW);
  }
}

/*** END CW morse code lookup and keying functions ***/
