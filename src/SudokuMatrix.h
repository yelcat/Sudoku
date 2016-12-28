#include "SudokuData.h"

#define SH 7
#define ST 6
#define DS 5

#define MATRIX_LENGTH 16
#define MATRIX_MIDDLE 8

const unsigned char SYMBOLS[] = 
{
    //blank	 1	  2	   3	4	 5	  6	   7	8	 9	  A	   b	C    d	  E    F    
    0xff,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e
};

struct SudokuMatrix {
    int mIdx;
    int qIdx;
    int pIdx;
    unsigned char data[MATRIX_LENGTH];
    bool isVictory;
};

inline void _shiftOut(int dsPin, int shPin, unsigned char n)
{
    for (int sIdx = 0; sIdx < 8; sIdx++) {
        if (n & 0x80)
            digitalWrite(dsPin, HIGH);
        else
            digitalWrite(dsPin, LOW);
        
        n <<= 1;
        digitalWrite(shPin, LOW);
        digitalWrite(shPin, HIGH);
    }
}

void _setQuestion(SudokuMatrix* matrix) 
{
    // copy question matrix
    const unsigned char* question = QUESTIONS[matrix->qIdx];
    for (matrix->mIdx = 0; matrix->mIdx < MATRIX_LENGTH; matrix->mIdx++) {
        matrix->data[matrix->mIdx] = question[matrix->mIdx];
    }

    // calc position index of point, first one can be input
    for (matrix->mIdx = 0; matrix->mIdx < MATRIX_LENGTH; matrix->mIdx++) {
        if (question[matrix->mIdx] == 0) {
            matrix->pIdx = matrix->mIdx;
            break;
        }
    }

    matrix->isVictory = false;

    // print matrix to serial port
    Serial.print("matrix qIdx: ");
    Serial.print(matrix->qIdx);
    Serial.print(", pIdx: ");
    Serial.print(matrix->pIdx);
    Serial.print(", mIdx: ");
    Serial.println(matrix->mIdx);
    for (matrix->mIdx = 0; matrix->mIdx < MATRIX_LENGTH; matrix->mIdx++) {
        Serial.print(matrix->data[matrix->mIdx]);
        if ((matrix->mIdx + 1) % 4 == 0)
            Serial.println(",");
        else 
            Serial.print(",");
    }
}

void showStart() 
{
    for (int i = 0; i < MATRIX_LENGTH; i++) {
        for (int j = 0; j < MATRIX_LENGTH; j++) {
            _shiftOut(DS, SH, i == j ? SYMBOLS[8] : SYMBOLS[0]);
        }
        digitalWrite(ST, LOW);
        digitalWrite(ST, HIGH);
        delay(100);
    }

    for (int i = 0; i < MATRIX_LENGTH; i++) {
        _shiftOut(DS, SH, SYMBOLS[0]);
    }
    digitalWrite(ST, LOW);
    digitalWrite(ST, HIGH);
    delay(200);

    for (int i = 0; i < MATRIX_LENGTH; i++) {
        _shiftOut(DS, SH, SYMBOLS[8]);
    }
    digitalWrite(ST, LOW);
    digitalWrite(ST, HIGH);
    delay(200);

    for (int i = 0; i < MATRIX_LENGTH; i++) {
        _shiftOut(DS, SH, SYMBOLS[0]);
    }
    digitalWrite(ST, LOW);
    digitalWrite(ST, HIGH);
    delay(200);

    for (int i = 0; i < MATRIX_LENGTH; i++) {
        _shiftOut(DS, SH, SYMBOLS[8]);
    }
    digitalWrite(ST, LOW);
    digitalWrite(ST, HIGH);
    delay(200);

    for (int i = 0; i < MATRIX_LENGTH; i++) {
        _shiftOut(DS, SH, SYMBOLS[0]);
    }
    digitalWrite(ST, LOW);
    digitalWrite(ST, HIGH);
    delay(200);
}

bool isCompleted(SudokuMatrix* matrix) 
{
    for (matrix->mIdx = 0; matrix->mIdx < MATRIX_LENGTH; matrix->mIdx++)
        if (matrix->data[matrix->mIdx] == 0)
            return false;
    
    return true;
}

inline bool _isUniqe(const SudokuMatrix* matrix, unsigned char n1, unsigned char n2, unsigned char n3, unsigned char n4) 
{
    if (matrix->data[n1] == matrix->data[n2])
        return false;
    if (matrix->data[n1] == matrix->data[n3])
        return false;
    if (matrix->data[n1] == matrix->data[n4])
        return false;
    if (matrix->data[n2] == matrix->data[n3]) 
        return false;
    if (matrix->data[n2] == matrix->data[n4])
        return false;
    if (matrix->data[n3] == matrix->data[n4])
        return false;

    return true;
}

void checkVictory(SudokuMatrix* matrix) 
{
    matrix->isVictory = 
        _isUniqe(matrix, 0, 1, 2, 3) 
        && _isUniqe(matrix, 4, 5, 6, 7) 
        && _isUniqe(matrix, 8, 9, 10, 11) 
        && _isUniqe(matrix, 12, 13, 14, 15) 

        && _isUniqe(matrix, 0, 4, 8, 12) 
        && _isUniqe(matrix, 1, 5, 9, 13) 
        && _isUniqe(matrix, 2, 6, 10, 14) 
        && _isUniqe(matrix, 3, 7, 11, 15) 

        && _isUniqe(matrix, 0, 1, 4, 5) 
        && _isUniqe(matrix, 2, 3, 6, 7) 
        && _isUniqe(matrix, 8, 9, 12, 13) 
        && _isUniqe(matrix, 10, 11, 14, 15);

    if (matrix->isVictory)
        showVictory();
}

void setupMatrix(SudokuMatrix* matrix) {
    pinMode(SH, OUTPUT);
    pinMode(ST, OUTPUT);
    pinMode(DS, OUTPUT);

    matrix->mIdx = 0;
    matrix->qIdx = 0;

    _setQuestion(matrix); 
}

void display(SudokuMatrix* matrix) 
{
    for (matrix->mIdx = 0; 
            matrix->mIdx < MATRIX_LENGTH; matrix->mIdx++) {
        unsigned char symbol = SYMBOLS[matrix->data[matrix->mIdx]];
        if (matrix->mIdx == matrix->pIdx) 
            symbol &= 0x7F;

        _shiftOut(DS, SH, symbol);
    }

    digitalWrite(ST, LOW);
    digitalWrite(ST, HIGH);
}

void prePosition(SudokuMatrix* matrix)
{
    const unsigned char* question = QUESTIONS[matrix->qIdx];
    do {
        matrix->pIdx--;
        if (matrix->pIdx < 0) 
            matrix->pIdx = MATRIX_LENGTH - 1;
    } while (question[matrix->pIdx] != 0);
}

void nextPosition(SudokuMatrix* matrix)
{
    const unsigned char* question = QUESTIONS[matrix->qIdx];
    do {
        matrix->pIdx++;
        if (matrix->pIdx == MATRIX_LENGTH)
            matrix->pIdx = 0;
    } while (question[matrix->pIdx] != 0);
}

void preQuestion(SudokuMatrix* matrix)
{
    matrix->qIdx--;
    if (matrix->qIdx < 0)
        matrix->qIdx = QUESTION_TOTAL - 1;

    _setQuestion(matrix);
}

void nextQuestion(SudokuMatrix* matrix)
{
    matrix->qIdx++;
    if (matrix->qIdx == QUESTION_TOTAL)
        matrix->qIdx = 0;
    
    _setQuestion(matrix);
}

void inputPositionValue(SudokuMatrix* matrix, unsigned char n) 
{
    matrix->data[matrix->pIdx] = n;
    if (isCompleted(matrix))
        checkVictory(matrix);
}
