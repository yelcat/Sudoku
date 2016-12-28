#define PRE_POS A0
#define NEXT_POS A1

#define PRE_QUES 3
#define NEXT_QUES 2

#define N1_BTN A2
#define N2_BTN A3
#define N3_BTN A4
#define N4_BTN A5

#define INTERVAL_COUNT 200

int interval = INTERVAL_COUNT;

void setupButtonPins() {
    pinMode(PRE_POS, INPUT_PULLUP);
    pinMode(NEXT_POS, INPUT_PULLUP);
    pinMode(PRE_QUES, INPUT_PULLUP);
    pinMode(NEXT_QUES, INPUT_PULLUP);

    pinMode(N1_BTN, INPUT_PULLUP);
    pinMode(N2_BTN, INPUT_PULLUP);
    pinMode(N3_BTN, INPUT_PULLUP);
    pinMode(N4_BTN, INPUT_PULLUP);

}

int readButtonPressed() 
{
    int val = -1;
    if (interval == 0) {
        if (digitalRead(N1_BTN) == LOW)
            val = 1;
        else if (digitalRead(N2_BTN) == LOW)
            val = 2;
        else if (digitalRead(N3_BTN) == LOW)
            val = 3;
        else if (digitalRead(N4_BTN) == LOW)
            val = 4;
        else if (digitalRead(PRE_POS) == LOW)
            val = 5;
        else if (digitalRead(NEXT_POS) == LOW)
            val = 6;
        else if (digitalRead(PRE_QUES) == LOW)
            val = 7;
        else if (digitalRead(NEXT_QUES) == LOW)
            val = 8;
        
        if (val != -1)
            interval = INTERVAL_COUNT;
    }
    else 
        interval--;

    return val;
}
