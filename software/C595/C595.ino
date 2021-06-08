// C-595 Calculator Chip Emulator
//
// requires MiniCore https://github.com/MCUdude/MiniCore
// set clock internal 1MHz
// set BOD Disabled

#include <Arduino.h>
#include "Decimal.h"
#include "FastGPIO.h"
void runTests();
//#define TEST

char keyCode = ' ';
char prevKeyCode = ' ';

const int MAX_NUMBER_STRING_SIZE = 32;
char valString[MAX_NUMBER_STRING_SIZE];

char op;

Decimal mem;
Decimal res;
Decimal num1;
Decimal num2;

enum State {result, operand1, operand2, operation, percent};
const char* StateName[5] = {"result","operand1","operand2","operation","percent"};
enum State state = result;

#define Sp 0x01
#define Sa 0x02
#define Sb 0x04
#define Sc 0x08
#define Sd 0x10
#define Se 0x20
#define Sf 0x40
#define Sg 0x80

const int KN = 12;
const int KO = 11;
const int KP = 10;
const int N_DIGITS = 9;
const int N_SEGMENTS = 8;

constexpr int digitPins[N_DIGITS] = { 3,2,A5,A4,A3,A2,A1,A0,13}; //D1-D9

// if PIN_PB6 not defined - check using MiniCore
constexpr int segmentPins[N_SEGMENTS] = {4,PIN_PB6,PIN_PB7,5,6,7,8,9}; // DP, A - G
int digits[9] = {0,1,2,3,4,5,6,7,8}; // initial display

// segment patterns
uint8_t digitSegments[35] = {Sa|Sb|Sc|Sd|Se|Sf,Sb|Sc,Sa|Sb|Sg|Se|Sd,Sa|Sb|Sc|Sd|Sg,Sb|Sc|Sf|Sg,Sa|Sf|Sg|Sc|Sd,Sa|Sg|Sc|Sd|Se|Sf,Sa|Sb|Sc,Sa|Sb|Sc|Sd|Se|Sf|Sg,Sa|Sb|Sc|Sd|Sf|Sg, // 0-9
  Sa|Sb|Sc|Sd|Se|Sf|Sp,Sb|Sc|Sp,Sa|Sb|Sg|Se|Sd|Sp,Sa|Sb|Sc|Sd|Sg|Sp,Sb|Sc|Sf|Sg|Sp,Sa|Sf|Sg|Sc|Sd|Sp,Sa|Sg|Sc|Sd|Se|Sf|Sp,Sa|Sb|Sc|Sp,Sa|Sb|Sc|Sd|Se|Sf|Sg|Sp,Sa|Sb|Sc|Sd|Sf|Sg|Sp, // 0-9 with dp
  Sg,Sd|Se|Sf,Sa|Sf|Sg|Se|Sd,Sa|Sf|Se|Sd,Sp,0x00,  // '-' L E C DP and blank
  Sf|Sb|Sg|Se|Sc, Se|Sf|Sa|Sb|Sc|Sg, Sa|Sf|Se|Sd, Sa|Sf|Se|Sg|Sc, Se|Sf|Sa|Sb|Sc|Sg, Sb|Sc|Sd|Se|Sg,  Se|Sf|Sa|Sb|Sc|Sg , Sf|Sg|Sb|Sc|Sd, 0x00 // "HACKAdAY" (26-34)
  };

bool awaitingRelease[10] = {false};
int currentDigitIndex = 0;

// the setup function runs once when you press reset or power the board
void setup() 
{
  // initialize
  FastGPIO::Pin<digitPins[0]>::setOutput(LOW);
  FastGPIO::Pin<digitPins[1]>::setOutput(LOW);
  FastGPIO::Pin<digitPins[2]>::setOutput(LOW);
  FastGPIO::Pin<digitPins[3]>::setOutput(LOW);
  FastGPIO::Pin<digitPins[4]>::setOutput(LOW);
  FastGPIO::Pin<digitPins[5]>::setOutput(LOW);
  FastGPIO::Pin<digitPins[6]>::setOutput(LOW);
  FastGPIO::Pin<digitPins[7]>::setOutput(LOW);
  FastGPIO::Pin<digitPins[8]>::setOutput(LOW);

  FastGPIO::Pin<segmentPins[0]>::setOutput(LOW);
  FastGPIO::Pin<segmentPins[1]>::setOutput(LOW);
  FastGPIO::Pin<segmentPins[2]>::setOutput(LOW);
  FastGPIO::Pin<segmentPins[3]>::setOutput(LOW);
  FastGPIO::Pin<segmentPins[4]>::setOutput(LOW);
  FastGPIO::Pin<segmentPins[5]>::setOutput(LOW);
  FastGPIO::Pin<segmentPins[6]>::setOutput(LOW);
  FastGPIO::Pin<segmentPins[7]>::setOutput(LOW);

  FastGPIO::Pin<KN>::setInputPulledUp(); //pinMode(KN, INPUT);
  FastGPIO::Pin<KO>::setInputPulledUp(); //pinMode(KO, INPUT);
  FastGPIO::Pin<KP>::setInputPulledUp(); //pinMode(KP, INPUT);
  
  Serial.begin(9600);
  Serial.println("Starting");

#if defined(TEST)
  runTests();
#endif
}



void loop()  
{
   for(int cdi=0; cdi < 9; cdi++)
   {
      //setAllDigitPinsLowApartFrom(cdi);
      setAllDigitPins(LOW);
      setAllSegmentPins(LOW);

      switch(cdi)
      {
        case 0: FastGPIO::Pin<digitPins[0]>::setOutputValue(HIGH); break;
        case 1: FastGPIO::Pin<digitPins[1]>::setOutputValue(HIGH); break;
        case 2: FastGPIO::Pin<digitPins[2]>::setOutputValue(HIGH); break;
        case 3: FastGPIO::Pin<digitPins[3]>::setOutputValue(HIGH); break;
        case 4: FastGPIO::Pin<digitPins[4]>::setOutputValue(HIGH); break;
        case 5: FastGPIO::Pin<digitPins[5]>::setOutputValue(HIGH); break;
        case 6: FastGPIO::Pin<digitPins[6]>::setOutputValue(HIGH); break;
        case 7: FastGPIO::Pin<digitPins[7]>::setOutputValue(HIGH); break;
        case 8: FastGPIO::Pin<digitPins[8]>::setOutputValue(HIGH); break;
      } 
     
       // display the pattern for the current digit
       uint8_t segment = digitSegments[digits[cdi]];
       FastGPIO::Pin<segmentPins[0]>::setOutputValue(((segment & (1<<0)) != 0) ? HIGH : LOW);
       FastGPIO::Pin<segmentPins[1]>::setOutputValue(((segment & (1<<1)) != 0) ? HIGH : LOW);
       FastGPIO::Pin<segmentPins[2]>::setOutputValue(((segment & (1<<2)) != 0) ? HIGH : LOW);
       FastGPIO::Pin<segmentPins[3]>::setOutputValue(((segment & (1<<3)) != 0) ? HIGH : LOW);
       FastGPIO::Pin<segmentPins[4]>::setOutputValue(((segment & (1<<4)) != 0) ? HIGH : LOW);
       FastGPIO::Pin<segmentPins[5]>::setOutputValue(((segment & (1<<5)) != 0) ? HIGH : LOW);
       FastGPIO::Pin<segmentPins[6]>::setOutputValue(((segment & (1<<6)) != 0) ? HIGH : LOW);
       FastGPIO::Pin<segmentPins[7]>::setOutputValue(((segment & (1<<7)) != 0) ? HIGH : LOW);

       delayMicroseconds(50);
   } 
   
   // clear all segments
   setAllSegmentPins(LOW);
   
   //setAllDigitPinsHighApartFrom(currentDigitIndex);
   setAllDigitPins(HIGH);
   switch(currentDigitIndex)
   {
        case 0: FastGPIO::Pin<digitPins[0]>::setOutputValue(LOW); break;
        case 1: FastGPIO::Pin<digitPins[1]>::setOutputValue(LOW); break;
        case 2: FastGPIO::Pin<digitPins[2]>::setOutputValue(LOW); break;
        case 3: FastGPIO::Pin<digitPins[3]>::setOutputValue(LOW); break;
        case 4: FastGPIO::Pin<digitPins[4]>::setOutputValue(LOW); break;
        case 5: FastGPIO::Pin<digitPins[5]>::setOutputValue(LOW); break;
        case 6: FastGPIO::Pin<digitPins[6]>::setOutputValue(LOW); break;
        case 7: FastGPIO::Pin<digitPins[7]>::setOutputValue(LOW); break;
        case 8: FastGPIO::Pin<digitPins[8]>::setOutputValue(LOW); break;
   }
   
   // see if a key was pressed
#if defined(TEST)   
   if(serialReadKey())
#else   
   if(readKey(currentDigitIndex))
#endif
   {
      processKey(keyCode);     
      prevKeyCode = keyCode;
              
      if(res.error)
      {
         for(int i = 0; i < 9; i++)
            digits[i] = 26+i;    
      }
      else
         setDisplay(res);    

#if defined(TEST)
      Serial.print(keyCode); 
      Serial.print(" ["); 
      for(int i = 0; i < 9; i++)
      {
         Serial.print(digits[i]); Serial.print(",");
      }
      Serial.print("] ");
      Serial.print("\""); Serial.print(res.format(valString));  Serial.print("\" ");
    
      Serial.print(" res ");Serial.print(res); 
      Serial.print(" num1 ");Serial.print(num1);
      Serial.print(" num2 ");Serial.print(num2);
      Serial.print(" next state ");Serial.println(StateName[state]);
#endif
    }   
   
   currentDigitIndex++;
   currentDigitIndex %= N_DIGITS;
}

void setAllDigitPins(int val)
{
   FastGPIO::Pin<digitPins[0]>::setOutputValue(val);
   FastGPIO::Pin<digitPins[1]>::setOutputValue(val);
   FastGPIO::Pin<digitPins[2]>::setOutputValue(val);
   FastGPIO::Pin<digitPins[3]>::setOutputValue(val);
   FastGPIO::Pin<digitPins[4]>::setOutputValue(val);
   FastGPIO::Pin<digitPins[5]>::setOutputValue(val);
   FastGPIO::Pin<digitPins[6]>::setOutputValue(val);
   FastGPIO::Pin<digitPins[7]>::setOutputValue(val);
   FastGPIO::Pin<digitPins[8]>::setOutputValue(val);
}
void setAllSegmentPins(int val)
{
   FastGPIO::Pin<segmentPins[0]>::setOutputValue(val);
   FastGPIO::Pin<segmentPins[1]>::setOutputValue(val);
   FastGPIO::Pin<segmentPins[2]>::setOutputValue(val);
   FastGPIO::Pin<segmentPins[3]>::setOutputValue(val);
   FastGPIO::Pin<segmentPins[4]>::setOutputValue(val);
   FastGPIO::Pin<segmentPins[5]>::setOutputValue(val);
   FastGPIO::Pin<segmentPins[6]>::setOutputValue(val);
   FastGPIO::Pin<segmentPins[7]>::setOutputValue(val);
}

bool isop(char keyCode)
{
  return keyCode == '+' || keyCode == '-' || keyCode == 'x' || keyCode == '*' || keyCode == '/';
}
bool isDecimal(char keyCode)
{
  return keyCode == '.' || (keyCode >= '0' && keyCode <= '9');
}

void setDisplay(Decimal num)
{
    bool dpPending = false;
    char * str = num.format(valString);

    int d;
    for(d = 0; d < 9; d++)
      digits[d] = 25; // blank

    for(int i = strlen(str) - 1, d=8 ; i >= 0; i--)
    {
        char c = str[i];
        if(c == '.')
        {
           dpPending = true;
           continue;
        }
        else
        {
           digits[d--] = (c >= '0' && c <= '9') ? (c - '0' + (dpPending ? 10 : 0)) : c == '-' ? 20 : c == 'E' ? 22 : 25;
        }
       
        dpPending = false;
    }
}


bool readKey(int currentDigitIndex)
{  
     // see if a key is pressed
     bool an = !FastGPIO::Pin<KN>::isInputHigh(); // int an = digitalRead(KN);
     bool ao = !FastGPIO::Pin<KO>::isInputHigh(); // int ao = digitalRead(KO);
     bool ap = !FastGPIO::Pin<KP>::isInputHigh(); // int ap = digitalRead(KP);

     char nop = ap ? 'P' : ao ? 'O' : an ? 'N' : ' ';
//     char nop = ap == LOW ? 'P' : ao ==LOW ? 'O' : an == LOW ? 'N' : ' ';

     if(nop == ' ')
     { 
        awaitingRelease[currentDigitIndex] = false;

        return false;
     }

     if(!awaitingRelease[currentDigitIndex])
     {            
        keyCode = keyFromMatrix(currentDigitIndex, nop);

//        Serial.print("key "); Serial.print(keyFromMatrix(currentDigitIndex, nop));  Serial.print(" dig "); Serial.print(currentDigitIndex + 1); Serial.print(" nop "); Serial.print(nop); Serial.print(" "); Serial.print(an); Serial.print(" "); Serial.print(ao); Serial.print(" "); Serial.print(ap); Serial.println(); 

        awaitingRelease[currentDigitIndex] = true;

        return true;
      }

      return false;
}

char keyFromMatrix(int currentDigitIndex, char nop)
{
   switch(currentDigitIndex)
   {
     case 0:
       return '1';
       break;
     case 1: //D2
       return nop == 'P' ? 'c' : nop == 'N' ? '2' : '=';
       break;
     case 2: //D3
       return nop == 'N' ? '3' : '+';
       break;
     case 3: //D4
       return nop == 'N' ? '4' : '-';
       break;
     case 4: //D5
       return nop == 'N' ? '5' : 'x';
       break;
     case 5: //D6
       return nop == 'N' ? '6' : '/';
       break;
     case 6: //D7
       return nop == 'P' ? 'm' : nop == 'N' ? '7' : '%';
       break;
     case 7: //D8
       return nop == 'N' ? '8' : '.';
       break;
     case 8: //D9
       return nop == 'N' ? '9' : '0';
       break;
     default:
       return ' ';   
   }
}

bool serialReadKey()
{
  if(Serial.available()) 
  {
    keyCode = (char)Serial.read(); 
    return true;
  }

  return false;
}


void processKey(char key)
{
  switch(state)
  {
     case result:
       if(isDecimal(key))
       {
         res = num1.setVal(key);
         state = operand1;
       }
       else if (isop(key))
       {
          num1 = res;
          op = key;
          state = operation;
       }
       else if (key == 'm')
       {
          num1 = memory();
          state = operand1;
       }
       else if (key == 'c')
       {
          clear();
       }
       break;  
     case operand1:
       if(isDecimal(key))
       {
         res = num1.updateVal(key);
       }
       else if (key == '=')
       {
          res = num1;
          mem += res;
          state = result;
       }
       else if (isop(key))
       {
          op = key;
          state = operation;
       }
       else if (key == 'm')
       {
          num1 = memory();
       }
       break;
     case operation:
       if(isDecimal(key))
       {
         res = num2.setVal(key);
         state = operand2;
       }
       else if (isop(key))
       {
         op = key;
       }
       else if (key == 'm')
       {
          num2 = memory();
          state = operand2;
       }
       else if (key == 'c')
       {
          clear();
          state = result;
       }
       break;
     case operand2:
       if(isDecimal(key))
       {
          res = num2.updateVal(key);
       }
       else if (key == '=')
       {
         res = calc(num1, op, num2);
         mem += res;
         state = result;
       }
       else if (isop(key))
       {
         res = calc(num1, op, num2);
         num1 = res;
         state = operation;
       }
       else if (key == '%')
       {
          res = calc( num1, '%', num2);
          num2 = res;
          state = percent;
       }
        else if (key == 'm')
       {
          num2 = memory();
       }
       break;
     case percent:
       if(key == '=')
       {
          res = calc(num1, op, num2);
          mem += res;
          state = result;
       }
       else if (isDecimal(key))
       {
          res = num1.setVal(key);
          state = operand1;
       }
       else if (isop(key))
       {
          op = key;
          num1 = res;
          state = operation;
       }
       break;
  }
}

void clear()
{
  num1.clear();
  num2.clear();
  res.clear();  
}

Decimal memory()
{
     Decimal ret = mem;
     if(prevKeyCode == 'm')
     {
        mem.clear();       
     }
     ret.resetOnUpdate = true;

     res = ret;
     
     return ret; 
}

Decimal calc(Decimal& num1, char op, Decimal& num2)
{
  Decimal ret;

  switch(op)
  {
    case '+':
       ret = num1 + num2;
       break;
    case '-':
       ret = num1 - num2;
       break;
    case 'x': case '*':
       ret = num1 * num2;
       break;
    case '/':
       ret = num1 / num2;
       break;
    case '%':
    {
     //  Decimal num1Copy = num1;
       ret = (num1 * num2).percent();
    }
       break;
   
  }

  return ret;
}

void runTests()
{
    t(0        ,  0, "0");
    t(100      , 0,  "100");
    t(-100     , 0,  "-100");
    t(100      , 2,  "10000");
    t(-100      , 2, "-10000");
    t(100000000, 0,  "100000000");
    t(-100000000, 0, "-1E8");
    t(123456789, 0,  "123456789");
    t(123456789, 1,  "1234567E3");
    t(123456789, 7,  "1234567E9");
    t(123456789, 8,  "123456E11");
    t(-123456789, 8, "-12345E12");
    t(-123456789, 0, "-123456E3");
    t(-123456789, 1, "-123456E4");
    t(1234567,    2, "123456700");
    
    t(1        , -1, "0.1");
    t(1        , -2, "0.01");
    t(1        , -8, "0.00000001");
    t(1        , -9, "1.0E-9");
    t(12       , -3, "0.012");
    t(123      , -3, "0.123");
    t(123      , -2, "1.23");
    t(1230     , -3, "1.230");
    t(12300    , -4, "1.2300");
    t(123      , -1, "12.3");
    t(123      , -8, "0.00000123");
    t(123      , -9, "1.23E-7");
    t(123456789, -1, "12345678.9");
    t(123456789, -5, "1234.56789");
    t(123456789, -8, "1.23456789");
    t(123456789, -9, "1.23456E-1");
    t(123456789, -10,"1.23456E-2");
    t(123456789, -20,"1.2345E-12");

    o(12,0,'*',12,0,"144");
    o(144,0,'/',12,0,"12");
    o(13,0,'/',2,0,"6.5");
    o(144,0,'%',10,0,"14.4");
    o(10,0,'*',10,0,"100");
    o(-10,0,'*',10,0,"-100");
    o(10,0,'*',-10,0,"-100");
    o(-10,0,'*',-10,0,"100");
    o(10,0,'+',10,0,"20");
    o(-10,0,'+',10,0,"0");
    o(10,0,'+',-10,0,"0");
    o(-10,0,'+',-10,0,"-20");
    o(10,0,'-',10,0,"0");
    o(-10,0,'-',10,0,"-20");
    o(10,0,'-',-10,0,"20");
    o(-10,0,'-',-10,0,"0");

    o(1,0,'/',1,0,"1");
    o(0,0,'/',1,0,"0");
    o(1,0,'/',0,0,"0");
    o(-1,0,'/',1,0,"-1");
    o(0,0,'/',-1,0,"0");
    o(-1,0,'/',0,0,"0");
    o(22,0,'/',7,0,"3.14285714");

    k("1+1=", "2");
    k("1+1+", "2");
    k("c1+1+1+", "3");
    k("c1+2+3+4+", "10");
    k("c100000*100000=", "1E10");
    k("100-1=", "99");
    k("+1=", "100");
    k("-10*-10=", "100");
}

void t(long v, int e, const char* expected)
{
   char* actual = Decimal(v,e).format(valString);
   Serial.print("[");Serial.print(v);Serial.print(","); Serial.print(e);Serial.print("] e:\""); 
   Serial.print(expected); Serial.print("\" a:\"");Serial.print(actual);Serial.print("\" ");
   Serial.print(strcmp(actual,expected) == 0 ? "" : " ***diff*** ");
   Serial.print(strlen(actual));
   unsigned int max = (strchr(actual, '.') != NULL) ? 10 : 9;
   Serial.println(strlen(actual) <= max ? "" : " len! ");
}

void o(long v, int e, char oper, long v2, int e2, const char* expected)
{
  Decimal result;

  switch (oper)
  {
    case '+':
       result = Decimal(v,e) + Decimal(v2,e2);
       break;
    case '-':
       result = Decimal(v,e) - Decimal(v2,e2);
       break;
    case '*':
    case 'x':
       result = Decimal(v,e) * Decimal(v2,e2);
       break;
    case '/':
       result = Decimal(v,e) / Decimal(v2,e2);
       break;
    case '%':
       result = Decimal(v,e).percent();
       break;
       
  }
   char* actual = result.format(valString);
   Serial.print("[");Serial.print(v);Serial.print(","); Serial.print(e);Serial.print("] ");
   Serial.print(oper == '+' ? "+" : oper == '-' ? "-" : oper == '*' ? "*" : oper == '/' ? "/" : oper == '%' ? "%" : "?");
   Serial.print(" [");Serial.print(v2);Serial.print(","); Serial.print(e2);Serial.print("] ");
   Serial.print(" => ");Serial.print(result);
   Serial.print(" e:\""); 
   Serial.print(expected); Serial.print("\" a:\"");Serial.print(actual);Serial.print("\" ");
   Serial.print(strcmp(actual,expected) == 0 ? "" : " ***diff*** ");
   unsigned int max = (strchr(actual, '.') != NULL) ? 10 : 9;
   Serial.println(strlen(actual) <= max ? "" : " len! ");
}

void k( const char* keys, const char* expected)
{
  for(unsigned int i = 0; i < strlen(keys); i++)          
     processKey( keys[i]);       

  Serial.print("\"");  Serial.print(keys);  Serial.print("\" "); 
  char* actual = res.format(valString);
  Serial.print("e:\"");  Serial.print(expected); 
  Serial.print("\" a:\"");Serial.print(actual);Serial.print("\" ");
  Serial.println(strcmp(actual,expected) == 0 ? "" : " ***diff*** ");
}

