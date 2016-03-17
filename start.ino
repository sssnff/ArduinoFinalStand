#include <TaskScheduler.h>
#include "sound.h"

//prototypes
void Everything();
void playMelody();
void matrix();

//leds
#define blueLed 11           
#define greenLed 10
#define redLed 9

#define PinOut1 4
#define PinOut2 13
#define PinOut3 2
#define PinOut4 12

#define PinIn1 8
#define PinIn2 7
#define PinIn3 6
#define PinIn4 5

//input
String incomingString = "";

//music
int melody[] = { 
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

//keyboard
int PinOut[4] {PinOut1, PinOut2, PinOut3, PinOut4};
int PinIn[4] {PinIn1, PinIn2, PinIn3, PinIn4};
const char value[4][4]
{ {'1', '4', '7', '*'},
  {'2', '5', '8', '0' },
  {'3', '6', '9', '#'},
  {'A', 'B', 'C', 'D'}
};

//Tasks
Task taskMelody(200, 5, &playMelody);
Task taskText(50, TASK_FOREVER, &matrix);
Task taskEverything(50, TASK_FOREVER, &Everything);

Scheduler runner;


void Everything() {  
  if (Serial.available() > 0){
      incomingString = Serial.readString();
     }  
   if(incomingString == "blue on"){ 
      digitalWrite(blueLed, HIGH);
   }  

   if(incomingString == "blue off"){ 
      digitalWrite(blueLed, LOW);
   }

    if(incomingString == "green on"){ 
      digitalWrite(greenLed, HIGH);
   }  

   if(incomingString == "green off"){ 
      digitalWrite(greenLed, LOW);
   }

    if(incomingString == "red on"){ 
      digitalWrite(redLed, HIGH);
   }  

   if(incomingString == "red off"){ 
      digitalWrite(redLed, LOW);
   }

    if(incomingString == "bip on"){
        taskMelody.enable();
    }
    
    if(incomingString == "bip off"){
        taskMelody.disable();
    }
}

//functions
void playMelody(){
   for (int thisNote = 0; thisNote < 8; thisNote++) {
      int noteDuration = 1000 / noteDurations[thisNote];
      tone(3, melody[thisNote], noteDuration);
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);
}
}

void matrix (){ 
  for (int i = 1; i <= 4; i++)
  {
    digitalWrite(PinOut[i - 1], LOW); 
    for (int j = 1; j <= 4; j++) 
    {
      if (digitalRead(PinIn[j - 1]) == LOW) 
      {
        Serial.println( value[i - 1][j - 1]); 
        delay(128);
      }
    }
    digitalWrite(PinOut[i - 1], HIGH); 
  }
}

void setup () {
  pinMode(blueLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(redLed, OUTPUT);

  pinMode (PinOut4, OUTPUT); 
  pinMode (PinOut3, OUTPUT);
  pinMode (PinOut2, OUTPUT);
  pinMode (PinOut1, OUTPUT);
 
  pinMode (PinIn4, INPUT); 
  digitalWrite(PinIn4, HIGH);
  pinMode (PinIn3, INPUT);
  digitalWrite(PinIn3, HIGH);
  pinMode (PinIn2, INPUT);
  digitalWrite(PinIn2, HIGH);
  pinMode (PinIn1, INPUT);
  digitalWrite(PinIn1, HIGH);  
  
  Serial.begin(115200);  
  runner.init();
   
  runner.addTask(taskMelody);
  runner.addTask(taskText);
  runner.addTask(taskEverything);
  
  taskMelody.disable();
  taskText.enable();
  taskEverything.enable();
}

void loop () {
  runner.execute();
}
