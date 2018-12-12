#include <SoftwareSerial.h>
SoftwareSerial xbee(0, 1); // RX, TX
char c = 'U';// characters to be sent
char d = 'L';
char e = 'T';
int occupancy = 0;// occupancy information in one byte

const int trigPin1 = 22;
const int echoPin1 = 24;
const int trigPin2 = 28;
const int echoPin2 = 30;
int PeopleCount[4]; // The value in each index refers to the number of people to the according sensor number
// defines variables
long duration;
long duration2;
int distance;
int distance2;
int state;
int state1;
int state2;
int state3;
int trigger;
int initialArray[] ={0,0,0,0,0};
int initialArray2[] = {0,0,0,0,0};
int testArray[]= {2,20,2,20,2};
int Array[5];
int Array2[5];
bool stability;
bool stability2;
void setup() {
pinMode(trigPin1, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin1, INPUT); // Sets the echoPin as an Input
pinMode(trigPin2, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin2, INPUT); // Sets the echoPin as an Input
Serial.begin(9600); // Starts the serial communication
  xbee.begin(9600); //xbee
}

void loop() {

Sensor2();
if (state == 1){
  if (trigger ==0){
    if( isIncreasing(Array2) ==true ){
 occupancy++;
    Array2[0]= 1;
    Array2[1]=10;
     Array2[2]=1;
     Array2[3]=10;
    Serial.print("occupancy of room 1 is   ");
    Serial.print(occupancy);
      Serial.print(" person(s) ");
delay(6000);
    Array2[0]= 1;
    Array2[1]=100;
     Array2[2]=1;
     Array2[3]=300;
      
      }
       
    }
    
  if
  
  (trigger ==1){
    if( isDecreasing(Array2) ==true ){
 occupancy--;
   Array2[0]= 1;
    Array2[1]=10;
     Array2[2]=1;
  
    Serial.print("occupancy of room 1 is   ");
    Serial.print(occupancy);
      Serial.print(" person(s) ");
delay(4000);
      
      }
       
    }

    
  
  }
  




xbee.write("room 1 occupancy：");
xbee.write(occupancy);


  //--- display the character just sent on console ---

}

void Sensor1(){
  // Clears the trigPin
digitalWrite(trigPin1, LOW);
delayMicroseconds(2);
// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin1, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin1, LOW);
// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin1, HIGH);
// Calculating the distance
distance = duration*0.034/2;
if (distance<500){         
//sometimes there is overflow causing distance to be around 3000cm
// Prints the distance on the Serial Monitor
Serial.println(" ");
Serial.println("Distance:(Sensor1) ");
updateDistance(distance, initialArray, Array );
Serial.print(Array[0]);
Serial.print(" ");
Serial.print(Array[1]);
Serial.print(" ");
Serial.print(Array[2]);
Serial.print(" ");
Serial.print(Array[3]);
Serial.print(" ");
Serial.print(Array[4]);
Serial.print(" ");

  for (int i = 0; i < 5; i++) {
      initialArray[i] = Array[i];
   }
   //Copy new aray to the old array for next loop
delay(100);
}
}

void Sensor2(){
  // Clears the trigPin
  delay(500);
digitalWrite(trigPin2, LOW);
delayMicroseconds(2);
// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin2, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin2, LOW);
// Reads the echoPin, returns the sound wave travel time in microseconds
duration2 = pulseIn(echoPin2, HIGH);
// Calculating the distance
distance2= duration2*0.034/2;
if (distance2<500){         
//sometimes there is overflow causing distance to be around 3000cm
// Prints the distance on the Serial Monitor
Serial.println(" ");
updateDistance(distance2, initialArray2, Array2 );


stability2 = isUnstable(Array2);//true means stable



if (stability2 == true) {
  state = 0;
  
  }
if (stability2 == false){
  state =1;
  
    

if( isIncreasing(Array2) ==true ){ 
trigger = 0;
  
  }
 else{
    if( isDecreasing(Array2) ==true ){
trigger = 1;    
   
      }
  }
   
  }
  for (int i = 0; i < 5; i++) {
      initialArray2[i] = Array2[i];
   }
   //Copy new aray to the old array for next loop
delay(100);
}
}


void  updateDistance(int distance, int *oldArray,int *Array ){
  //DistanceArray is an array with 3 distances values
  //The oldest distance element from distance array is discarded
  //The most recent distance values from send_trigger() function is added
  Array[0] = oldArray[1];
  Array[1] = oldArray[2];
  Array[2] = oldArray[3];
  Array[3] = oldArray[4];
  Array[4] = distance;
  return;  
}
bool isUnstable(int *Array ){
  
  for ( int i=0; i<4; i++)
  {
    if ( Array[i] != Array [i+1]){
     Serial.print("occupancy of room 1 is   ");
    Serial.print(occupancy);
      Serial.print(" person(s) ");
    return false;}
  }      

  return true;
    }
  
  
  
bool isIncreasing(int *Array ){
for (int i = 0; i<4; i++){

  if(Array[i] < Array[i+1]){
    }
  else{
    return false;}
  }
  return true;
  }
  
bool isDecreasing(int *Array ){
for (int i = 0; i<4; i++){

  if(Array[i] > Array[i+1]){
    }
  else{
    return false;}
  }
  return true;
  }


