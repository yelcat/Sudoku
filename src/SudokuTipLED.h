#define RED_PIN 11
#define GREEN_PIN 10
#define GLUE_PIN 9

void setupLEDPins() {
    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(GLUE_PIN, OUTPUT);  
}

void _setColor(int red, int green, int blue)
{
  #ifdef COMMON_ANODE
    red = 255 - red;
    green = 255 - green;
    blue = 255 - blue;
  #endif
  analogWrite(RED_PIN, red);
  analogWrite(GREEN_PIN, green);
  analogWrite(GLUE_PIN, blue);  
}

#define LED_BLUE _setColor(0, 0, 255)
#define LED_RED _setColor(255, 0, 0)
#define LED_GREEN _setColor(0, 255, 0)
#define LED_YELLOW _setColor(255, 255, 0)
#define LED_PURPLE _setColor(80, 0, 80)
#define LED_AQUA _setColor(0, 255, 255) 
#define LED_WHITE _setColor(255, 255, 255) 
#define DISABLE_LED _setColor(0, 0, 0) 

inline void showVictory() 
{
    LED_GREEN;
    delay(300);
    LED_YELLOW;
    delay(300);
    LED_PURPLE;
    delay(300);
    LED_AQUA;
    delay(300);
    LED_GREEN;
    delay(300);
    LED_YELLOW;
    delay(300);
    LED_PURPLE;
    delay(300);
    LED_AQUA;
    delay(300);
}
