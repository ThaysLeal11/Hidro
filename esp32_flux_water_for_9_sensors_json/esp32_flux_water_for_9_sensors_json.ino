
#include <ArduinoJson.h>
#include <time.h>
byte sensorPin1 = 23;
byte sensorPin2 = 21;
byte sensorPin3 = 18;
byte sensorPin4 = 17;
byte sensorPin5 = 4;
byte sensorPin6 = 2;
byte sensorPin7 = 12;
byte sensorPin8 = 5;
byte sensorPin9 = 17;
DynamicJsonDocument doc(1024);

// The hall-effect flow sensor outputs approximately 4.5 pulses per second per
// litre/minute of flow.
float calibrationFactor = 6.5163;
float calibrationDifference = 2.7;

//pulso
volatile byte pulseCount1;  //sensor 1

float flowRate1;
unsigned int flowMilliLitres1;  
unsigned long totalMilliLitres1;


volatile byte pulseCount2;  //sensor 2

float flowRate2;
unsigned int flowMilliLitres2;
unsigned long totalMilliLitres2;


volatile byte pulseCount3;  //sensor 3

float flowRate3;
unsigned int flowMilliLitres3;
unsigned long totalMilliLitres3;

volatile byte pulseCount4; //sensor 4

float flowRate4;
unsigned int flowMilliLitres4;
unsigned long totalMilliLitres4;


volatile byte pulseCount5; //sensor 5

float flowRate5;
unsigned int flowMilliLitres5;
unsigned long totalMilliLitres5;

volatile byte pulseCount6;  //sensor 6

float flowRate6;
unsigned int flowMilliLitres6;
unsigned long totalMilliLitres6;


volatile byte pulseCount7; //sensor 7

float flowRate7;
unsigned int flowMilliLitres7;
unsigned long totalMilliLitres7;

volatile byte pulseCount8;  //sensor 8

float flowRate8;
unsigned int flowMilliLitres8;
unsigned long totalMilliLitres8;


volatile byte pulseCount9; //sensor 9

float flowRate9;
unsigned int flowMilliLitres9;
unsigned long totalMilliLitres9;


unsigned long oldTime; //fim do tempo

void setup()
{
  
  // Initialize a serial connection for reporting values to the host
  Serial.begin(115200);
   

  
  pinMode(sensorPin1, INPUT);     //sensor 1
  digitalWrite(sensorPin1, HIGH);

  pinMode(sensorPin2, INPUT);    //sensor2
  digitalWrite(sensorPin2, HIGH);

  pinMode(sensorPin3, INPUT);     //sensor 3
  digitalWrite(sensorPin3, HIGH);

  pinMode(sensorPin4, INPUT);    //sensor4
  digitalWrite(sensorPin4, HIGH);

  pinMode(sensorPin5, INPUT);     //sensor 5
  digitalWrite(sensorPin5, HIGH);

  pinMode(sensorPin6, INPUT);    //sensor6
  digitalWrite(sensorPin6, HIGH);

  pinMode(sensorPin7, INPUT);     //sensor 7
  digitalWrite(sensorPin7, HIGH);

  pinMode(sensorPin8, INPUT);    //sensor8
  digitalWrite(sensorPin8, HIGH);

  pinMode(sensorPin9, INPUT);     //sensor 9
  digitalWrite(sensorPin9, HIGH);



  pulseCount1        = 0;  //sensor 1
  flowRate1          = 0.0;
  flowMilliLitres1   = 0;
  totalMilliLitres1  = 0;
  oldTime           = 0;

  pulseCount2        = 0; //sensor 2
  flowRate2          = 0.0;
  flowMilliLitres2   = 0;
  totalMilliLitres2  = 0;
  oldTime           = 0;

    pulseCount3        = 0;  //sensor 3
  flowRate3          = 0.0;
  flowMilliLitres3   = 0;
  totalMilliLitres3  = 0;
  oldTime           = 0;

  pulseCount4        = 0; //sensor 4
  flowRate4          = 0.0;
  flowMilliLitres4   = 0;
  totalMilliLitres4  = 0;
  oldTime           = 0;

  pulseCount5        = 0;  //sensor 5
  flowRate5          = 0.0;
  flowMilliLitres5   = 0;
  totalMilliLitres5  = 0;
  oldTime           = 0;

  pulseCount6        = 0; //sensor 6
  flowRate6          = 0.0;
  flowMilliLitres6   = 0;
  totalMilliLitres6  = 0;
  oldTime           = 0;

  pulseCount7        = 0;  //sensor 7
  flowRate7          = 0.0;
  flowMilliLitres7   = 0;
  totalMilliLitres7  = 0;
  oldTime           = 0;

  pulseCount8        = 0; //sensor 8
  flowRate8          = 0.0;
  flowMilliLitres8   = 0;
  totalMilliLitres8  = 0;
  oldTime           = 0;

  pulseCount9        = 0; //sensor 9
  flowRate9          = 0.0;
  flowMilliLitres9   = 0;
  totalMilliLitres9  = 0;
  oldTime           = 0;


  // The Hall-effect sensor is connected to pin 2 which uses interrupt 0.
  // Configured to trigger on a FALLING state change (transition from HIGH
  // state to LOW state)
  attachInterrupt(sensorPin1, pulseCounter1, FALLING); //sensor 1
  attachInterrupt(sensorPin2, pulseCounter2, FALLING); //sensor 2
  attachInterrupt(sensorPin3, pulseCounter3, FALLING); //sensor 3
  attachInterrupt(sensorPin4, pulseCounter4, FALLING); //sensor 4
  attachInterrupt(sensorPin5, pulseCounter5, FALLING); //sensor 5
  attachInterrupt(sensorPin6, pulseCounter6, FALLING); //sensor 6
  attachInterrupt(sensorPin7, pulseCounter7, FALLING); //sensor 7
  attachInterrupt(sensorPin8, pulseCounter8, FALLING); //sensor 8 
  attachInterrupt(sensorPin9, pulseCounter9, FALLING); //sensor 9

}
 
/**
 * Main program loop
 */
void loop()
{
   //Serial.print("5");
   if((millis() - oldTime) > 1000)    // Only process counters once per second
  { 
    // Disable the interrupt while calculating flow rate and sending the value to
    // the host
    detachInterrupt(sensorPin1); //sensor1
    detachInterrupt(sensorPin2); //sensor2
    detachInterrupt(sensorPin3); //sensor3
    detachInterrupt(sensorPin4); //sensor4
    detachInterrupt(sensorPin5); //sensor5
    detachInterrupt(sensorPin6); //sensor6
    detachInterrupt(sensorPin7); //sensor7
    detachInterrupt(sensorPin8); //sensor8
    detachInterrupt(sensorPin9); //sensor9
   

    // Because this loop may not complete in exactly 1 second intervals we calculate
    // the number of milliseconds that have passed since the last execution and use
    // that to scale the output. We also apply the calibrationFactor to scale the output
    // based on the number of pulses per second per units of measure (litres/minute in
    // this case) coming from the sensor.
    float tmp1 = (1000.0 / (millis() - oldTime)) * pulseCount1; //sensor 1
    float tmp2 = (1000.0 / (millis() - oldTime)) * pulseCount2; //sensor 2
    float tmp3 = (1000.0 / (millis() - oldTime)) * pulseCount3; //sensor 3
    float tmp4 = (1000.0 / (millis() - oldTime)) * pulseCount4; //sensor 4
    float tmp5 = (1000.0 / (millis() - oldTime)) * pulseCount5; //sensor 5
    float tmp6 = (1000.0 / (millis() - oldTime)) * pulseCount6; //sensor 6
    float tmp7 = (1000.0 / (millis() - oldTime)) * pulseCount7; //sensor 7
    float tmp8 = (1000.0 / (millis() - oldTime)) * pulseCount8; //sensor 8
    float tmp9 = (1000.0 / (millis() - oldTime)) * pulseCount9; //sensor 9


    // As we have a add operation we need to avoid the positive value when no pulse is found
     if (tmp1 > 0) {
      flowRate1 = tmp1 / calibrationFactor + calibrationDifference; //sensor1
    }else {
      flowRate1 = 0;
    }

     if (tmp2 > 0) {
      flowRate2 = tmp2 / calibrationFactor + calibrationDifference; //sensor2
    }else {
      flowRate2 = 0;
    }

     if (tmp3 > 0) {
      flowRate3 = tmp3 / calibrationFactor + calibrationDifference; //sensor3
    }else {
      flowRate3 = 0;
    }

     if (tmp4 > 0) {
      flowRate4 = tmp4 / calibrationFactor + calibrationDifference; //sensor4
    }else {
      flowRate4 = 0;
    }

     if (tmp5 > 0) {
      flowRate5 = tmp5 / calibrationFactor + calibrationDifference; //sensor5
    }else {
      flowRate5 = 0;
    }

     if (tmp6 > 0) {
      flowRate6 = tmp6 / calibrationFactor + calibrationDifference; //sensor6
    }else {
      flowRate6 = 0;
    }

     if (tmp7 > 0) {
      flowRate7 = tmp7 / calibrationFactor + calibrationDifference; //sensor7
    }else {
      flowRate7 = 0;
    }

     if (tmp8 > 0) {
      flowRate8 = tmp8 / calibrationFactor + calibrationDifference; //sensor8
    }else {
      flowRate8 = 0;
    }

     if (tmp9 > 0) {
      flowRate9 = tmp9 / calibrationFactor + calibrationDifference; //sensor9
    }else {
      flowRate9 = 0;
    }


    
    // Note the time this processing pass was executed. Note that because we've
    // disabled interrupts the millis() function won't actually be incrementing right
    // at this point, but it will still return the value it was set to just before
    // interrupts went away.
    oldTime = millis();
    
    // Divide the flow rate in litres/minute by 60 to determine how many litres have
    // passed through the sensor in this 1 second interval, then multiply by 1000 to
    // convert to millilitres.
    flowMilliLitres1 = (flowRate1 / 60) * 1000; //sensor1
    flowMilliLitres2 = (flowRate2 / 60) * 1000; //sensor2
    flowMilliLitres3 = (flowRate3 / 60) * 1000; //sensor3
    flowMilliLitres4 = (flowRate4 / 60) * 1000; //sensor4
    flowMilliLitres5 = (flowRate5 / 60) * 1000; //sensor5
    flowMilliLitres6 = (flowRate6 / 60) * 1000; //sensor6
    flowMilliLitres7 = (flowRate7 / 60) * 1000; //sensor7
    flowMilliLitres8 = (flowRate8 / 60) * 1000; //sensor8
    flowMilliLitres9 = (flowRate9 / 60) * 1000; //sensor9

    
    // Add the millilitres passed in this second to the cumulative total
    totalMilliLitres1 += flowMilliLitres1;//sensor 1
    totalMilliLitres2 += flowMilliLitres2;//sensor 2
    totalMilliLitres3 += flowMilliLitres3;//sensor 3
    totalMilliLitres4 += flowMilliLitres4;//sensor 4
    totalMilliLitres5 += flowMilliLitres5;//sensor 5
    totalMilliLitres6 += flowMilliLitres6;//sensor 6
    totalMilliLitres7 += flowMilliLitres7;//sensor 7
    totalMilliLitres8 += flowMilliLitres8;//sensor 8
    totalMilliLitres9 += flowMilliLitres9;//sensor 9
 
      
    unsigned int frac1;//sensor 1
    unsigned int frac2;//sensor 2
    unsigned int frac3;//sensor 3
    unsigned int frac4;//sensor 4
    unsigned int frac5;//sensor 5
    unsigned int frac6;//sensor 6
    unsigned int frac7;//sensor 7
    unsigned int frac8;//sensor 8
    unsigned int frac9;//sensor 9

    // Print the flow rate for this second in litres / minute

   // this serial print was used to get the FREQUENCY value to calculate the other values.
   /* Serial.print("\n");       // Print tab space
    Serial.print("F1: "); //sensor1
    Serial.println(tmp1);
    Serial.print("F2: "); //sensor2
    Serial.println(tmp2);
    Serial.print("F3: "); //sensor3
    Serial.println(tmp3);
    Serial.print("F4: "); //sensor4
    Serial.println(tmp4);
    Serial.print("F5: "); //sensor5
    Serial.println(tmp5);
    Serial.print("F6: "); //sensor6
    Serial.println(tmp6);
    Serial.print("F7: "); //sensor7
    Serial.println(tmp7);
    Serial.print("F8: "); //sensor8
    Serial.println(tmp8);
    Serial.print("F9: "); //sensor9
    Serial.println(tmp9);*/

   /* Serial.print("\n");       // Print tab space
    Serial.print("Flow rate1: "); //sensor1
    Serial.print(flowRate1);  // Print the integer part of the variable
    Serial.print("L/min");
    Serial.print("\n");       // Print tab space

    Serial.print("Flow rate2: "); //sensor2
    Serial.print(flowRate2);  // Print the integer part of the variable
    Serial.print("L/min");
    Serial.print("\n");       // Print tab space

    Serial.print("Flow rate3: "); //sensor3
    Serial.print(flowRate3);  // Print the integer part of the variable
    Serial.print("L/min");
    Serial.print("\n");       // Print tab space

    Serial.print("Flow rate4: "); //sensor4
    Serial.print(flowRate4);  // Print the integer part of the variable
    Serial.print("L/min");
    Serial.print("\n");       // Print tab space

    Serial.print("Flow rate5: "); //sensor5
    Serial.print(flowRate5);  // Print the integer part of the variable
    Serial.print("L/min");
    Serial.print("\n");       // Print tab space

    Serial.print("Flow rate6: "); //sensor6
    Serial.print(flowRate6);  // Print the integer part of the variable
    Serial.print("L/min");
    Serial.print("\n");       // Print tab space

    Serial.print("Flow rate7: "); //sensor7
    Serial.print(flowRate7);  // Print the integer part of the variable
    Serial.print("L/min");
    Serial.print("\n");       // Print tab space

    Serial.print("Flow rate8: "); //sensor8
    Serial.print(flowRate8);  // Print the integer part of the variable
    Serial.print("L/min");
    Serial.print("\n");       // Print tab space

    Serial.print("Flow rate9: "); //sensor9
    Serial.print(flowRate9);  // Print the integer part of the variable
    Serial.print("L/min");
    Serial.print("\n");       // Print tab space*/

  

    /*// Print the cumulative total of litres flowed since starting
    Serial.print("\n");       // Print tab space
    Serial.print("Output Liquid Quantity1: ");      //sensor1  
    Serial.print(totalMilliLitres1);
    Serial.print("mL");
    Serial.print("\t");        // Print tab space
    Serial.print(totalMilliLitres1/1000);
    Serial.print("L");

    Serial.print("\n"); 
    Serial.print("Output Liquid Quantity2: ");     //sensor2   
    Serial.print(totalMilliLitres2);
    Serial.print("mL");
    Serial.print("\t");        // Print tab space
    Serial.print(totalMilliLitres2/1000);
    Serial.print("L");

    Serial.print("\n"); 
    Serial.print("Output Liquid Quantity3: ");      //sensor3 
    Serial.print(totalMilliLitres3);
    Serial.print("mL");
    Serial.print("\t");        // Print tab space
    Serial.print(totalMilliLitres3/1000);
    Serial.print("L");

    Serial.print("\n"); 
    Serial.print("Output Liquid Quantity4: ");     //sensor4   
    Serial.print(totalMilliLitres4);
    Serial.print("mL");
    Serial.print("\t");        // Print tab space
    Serial.print(totalMilliLitres4/1000);
    Serial.print("L");

    Serial.print("\n"); 
    Serial.print("Output Liquid Quantity5: ");      //sensor5 
    Serial.print(totalMilliLitres5);
    Serial.print("mL");
    Serial.print("\t");        // Print tab space
    Serial.print(totalMilliLitres5/1000);
    Serial.print("L");

    Serial.print("\n"); 
    Serial.print("Output Liquid Quantity6: ");     //sensor6   
    Serial.print(totalMilliLitres6);
    Serial.print("mL");
    Serial.print("\t");        // Print tab space
    Serial.print(totalMilliLitres6/1000);
    Serial.print("L");
    
    Serial.print("\n"); 
    Serial.print("Output Liquid Quantity7: ");      //sensor7  
    Serial.print(totalMilliLitres7);
    Serial.print("mL");     
    Serial.print("\t");        // Print tab space
    Serial.print(totalMilliLitres7/1000);
    Serial.print("L");

    Serial.print("\n"); 
    Serial.print("Output Liquid Quantity8: ");     //sensor8   
    Serial.print(totalMilliLitres8);
    Serial.print("mL");
    Serial.print("\t");        // Print tab space
    Serial.print(totalMilliLitres8/1000);
    Serial.print("L");

    Serial.print("\n"); 
    Serial.print("Output Liquid Quantity9: ");     //sensor9 
    Serial.print(totalMilliLitres9);
    Serial.print("mL");
    Serial.print("\t");        // Print tab space
    Serial.print(totalMilliLitres9/1000);
    Serial.print("L");
    Serial.print("\n"); */


doc["hash"] = "sens012301";
doc["volta"] = flowRate1;
doc["litro"] = pulseCount1;
serializeJson(doc, Serial);


// This prints:
// {"sensor":"gps","time":1351824120,"data":[48.756080,2.302038]}


// This prints:
// {"sensor":"gps","time":1351824120,"data":[48.756080,2.302038]}
    
    // Reset the pulse counter so we can start incrementing again
    pulseCount1 = 0;//sensor1
    pulseCount2 = 0;//sensor2
    pulseCount3 = 0;//sensor3
    pulseCount4 = 0;//sensor4
    pulseCount5 = 0;//sensor5
    pulseCount6 = 0;//sensor6
    pulseCount7 = 0;//sensor7
    pulseCount8 = 0;//sensor8
    pulseCount9 = 0;//sensor9


    // Enable the interrupt again now that we've finished sending output
    attachInterrupt(sensorPin1, pulseCounter1, FALLING);
    attachInterrupt(sensorPin2, pulseCounter2, FALLING);
    attachInterrupt(sensorPin3, pulseCounter3, FALLING);
    attachInterrupt(sensorPin4, pulseCounter4, FALLING);
    attachInterrupt(sensorPin5, pulseCounter5, FALLING);
    attachInterrupt(sensorPin6, pulseCounter6, FALLING);
    attachInterrupt(sensorPin7, pulseCounter7, FALLING);
    attachInterrupt(sensorPin8, pulseCounter8, FALLING);
    attachInterrupt(sensorPin9, pulseCounter9, FALLING);

  }
}

/*
Insterrupt Service Routine
 */
void pulseCounter1() //sensor1
{
  // Increment the pulse counter
  pulseCount1++;
}

void pulseCounter2()//sensor2
{
  // Increment the pulse counter
  pulseCount2++;
}
void pulseCounter3() //sensor3
{
  // Increment the pulse counter
  pulseCount3++;
}

void pulseCounter4()//sensor4
{
  // Increment the pulse counter
  pulseCount4++;
}
void pulseCounter5() //sensor5
{
  // Increment the pulse counter
  pulseCount5++;
}

void pulseCounter6()//sensor6
{
  // Increment the pulse counter
  pulseCount6++;
}
void pulseCounter7() //sensor7
{
  // Increment the pulse counter
  pulseCount7++;
}

void pulseCounter8()//sensor8
{
  // Increment the pulse counter
  pulseCount8++;
}
void pulseCounter9() //sensor9
{
  // Increment the pulse counter
  pulseCount9++;
}
