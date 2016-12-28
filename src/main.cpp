/**
 * Sudoku Game
 */
#include <Arduino.h>
#include "SudokuTipLED.h"
#include "SudokuButton.h"
#include "SudokuMatrix.h"

SudokuMatrix matrix;

void setup() 
{
    Serial.begin(9600);

    setupMatrix(&matrix);
    setupButtonPins();
    setupLEDPins();

    LED_BLUE;

    showStart();
}

void loop() 
{
    int value = readButtonPressed();
    if (value > 0) {
        Serial.print("button value: ");
        Serial.println(value);
    }
    
    switch (value) {
        case 1:
        case 2:
        case 3:
        case 4:
            inputPositionValue(&matrix, value);
            break;
        case 5:
            prePosition(&matrix);
            break;
        case 6:
            nextPosition(&matrix);
            break;
        case 7:
            preQuestion(&matrix);
            break;
        case 8:
            nextQuestion(&matrix);
            break;
    }

    display(&matrix);
    if (matrix.isVictory)
        LED_GREEN;
    else
        LED_BLUE;
}
