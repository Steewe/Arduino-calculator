#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define BACKLIGHT_PIN 3
LiquidCrystal_I2C lcd (0x27, 2, 1, 0, 4, 5, 6, 7);
char digits[] = "X0=+123-456*789/";
char symbol;
char oper=' ';
float comp1,comp2,result=0;
int state =0;
boolean in=false;
void setup() 
  {
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
  pinMode(8, INPUT);
  pinMode(9, INPUT);
  pinMode(13, OUTPUT);
  
  lcd.begin(16,2);
  lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.home();
  lcd.setCursor(0,0);
  }

void loop() 
{
  
  digitalWrite(13, LOW);
  for (int i=2;i<=5;i++)
  {
    digitalWrite(i, HIGH); 
      for (int j=6;j<=9;j++)
        {
          if (digitalRead(j)==HIGH)
            {
               symbol = digits[6-4*(i-2)+j];
               digitalWrite(13, HIGH);    
               in=true;
            }
        }
    digitalWrite(i, LOW); 
    }
    
  Serial.println(state);
  if (in)
  {
  in=false; 
  switch(state)
  {
   case 0: //pusty wyświetlacz
    {
       if (symbol-'0'>=0 && symbol-'0'<=9)
       {
         comp1=symbol-'0';
         lcd.print(symbol); 
         Serial.println(comp1);
         state=1;  
       }
    break;  
    }
  case 1: //jeden symbol
    {
      if (symbol-'0'>=0 && symbol-'0'<=9)
      {
       comp1=10*comp1+(symbol-'0'); 
       lcd.print (symbol); 
       Serial.println(comp2);
       }
       else if (symbol=='+' ||symbol=='-' ||symbol=='*' ||symbol=='/' )
       {
       oper= symbol;
       lcd.print (symbol);
       state=2;
       }
      break;
    }
   case 5:
   {
     if (symbol=='+' ||symbol=='-' ||symbol=='*' ||symbol=='/' )
       {
       oper= symbol;
       lcd.print (symbol);
       state=2;
       }
     break;
   }
   
   case 2: //jedno dzialanie
    {
      if (symbol-'0'>=0 && symbol-'0'<=9)
      {
         comp2=symbol-'0' ;
         lcd.print (symbol);
         Serial.println(comp2); 
         state=3; 
       } 
    break;  
    }
    case 3:
    {
     if (symbol-'0'>=0 && symbol-'0'<=9)
       {
       comp2=10*comp2+(symbol-'0'); 
       lcd.print (symbol); 
       Serial.println(comp2);
       }
    else if(symbol =='=')
       {
       lcd.print (symbol);
       lcd.setCursor(0,1);
       switch (oper)
         {
           case '+': { result= (comp1+comp2); break;}
           case '-': { result= (comp1-comp2); break;}             
           case '*': { result= (comp1*comp2); break;} 
           case '/': 
           { 
             if (comp2==0) lcd.print("tak sie nie robi");
             else {result= (comp1/comp2); break;}
           }
         }
         lcd.print (result);
         state=4;
         Serial.println(result);
         delay(600);
         lcd.clear();
         lcd.setCursor(0,0);        
         lcd.print (result);
         comp1=result;
         state=5;
       }
     }
    default: break;  
  }
  }
  if (symbol == 'X') 
  {
    lcd.clear();
    lcd.setCursor(0,0);
    state=0;
    comp1=0;
    comp2=0;
    oper=' ';
    
  }
  delay(100); 
}
