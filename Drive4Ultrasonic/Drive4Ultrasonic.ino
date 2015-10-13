//Packages
#include <Arduino.h>

//Global Variable Declarations
	
	//PINS

		//Input Pins
			const int frontSonicLInputPin = 37;//PUT THE NUMBER OF THE PIN THAT IS CONNECTED TO THE SENSOR'S ECHO HERE
			const int frontSonicRInputPin = 31;
			const int leftSonicInputPin = 35;//PUT THE NUMBER OF THE PIN THAT IS CONNECTED TO THE SENSOR'S ECHO HERE
			const int rightSonicInputPin = 33;//PUT THE NUMBER OF THE PIN THAT IS CONNECTED TO THE SENSOR'S ECHO HERE

		//Output Pins 
			const int frontSonicLTriggerPin = 36;//PUT THE NUMBER OF THE PIN THAT IS CONNECTED TO THE SENSOR'S TRIGGER HERE
			const int frontSonicRTriggerPin = 30;
			const int leftSonicTriggerPin = 34;//PUT THE NUMBER OF THE PIN THAT IS CONNECTED TO THE SENSOR'S TRIGGER HERE
			const int rightSonicTriggerPin = 32;//PUT THE NUMBER OF THE PIN THAT IS CONNECTED TO THE SENSOR'S TRIGGER HERE

			const int leftMotorPin = 3;//LEFT MOTOR PIN HERE ---> NEEDS TO BE BETWEEN 2-13
			const int rightMotorPin = 2;//RIGHT MOTOR PIN HERE ---> NEEDS TO BE BETWEEN 2-13
	//GENERAL

		long frontLReading;
		long frontRReading;
		long leftReading;
		long rightReading;

void setup() {
	Serial.begin(9600);
	loadPins();
}

void loop() {
	frontLReading = getSonicDistance(frontSonicLInputPin, frontSonicLTriggerPin);
	frontRReading = getSonicDistance(frontSonicRInputPin, frontSonicRTriggerPin);
  leftReading = getSonicDistance(leftSonicInputPin, leftSonicTriggerPin);
  rightReading = getSonicDistance(rightSonicInputPin, rightSonicTriggerPin);
  
	if(frontLReading > 30 && frontRReading > 30){
		//go forward

		//see if we should tilt left or right
		if(frontLReading - frontRReading >= 10 && frontRReading < 100){
			//tilt towards left
      Serial.println("Tilt Left");
			analogWrite(leftMotorPin, 30);
			analogWrite(rightMotorPin, 60);
		} else if (frontRReading - frontLReading >= 10 && frontLReading < 100){
			//tilt towards right
      Serial.println("Tilt Right");
			analogWrite(leftMotorPin, 60);
			analogWrite(rightMotorPin, 30);
		} else {
			//go straight
      Serial.println("Go Straight");
			analogWrite(leftMotorPin, 63);
			analogWrite(rightMotorPin, 60);
		}
	} else {

    analogWrite(leftMotorPin, 0);
    analogWrite(rightMotorPin, 0);
    
    leftReading = getSonicDistance(leftSonicInputPin, leftSonicTriggerPin);
  
    rightReading = getSonicDistance(rightSonicInputPin, rightSonicTriggerPin);

    //poll and turn whichever way has more space
		//turn whichever way has more space
		if(rightReading > leftReading){
			//turn right
      Serial.println("Turn Right");
			analogWrite(leftMotorPin, 50);
			analogWrite(rightMotorPin, 0);
      delay(100);
		} else{
			//turn left
      Serial.println("Turn Left");
			analogWrite(rightMotorPin, 50);
			analogWrite(leftMotorPin, 0);
		  delay(100);
		}
	}//end go forward if/else



  delay(100);

}

//------------------------------------------------------------------------------------------------------

void loadPins(){
	//Input Pins
	pinMode(frontSonicLInputPin, INPUT);
	pinMode(frontSonicRInputPin, INPUT);
	pinMode(leftSonicInputPin, INPUT);
	pinMode(rightSonicInputPin, INPUT);

	//Output Pins
	pinMode(frontSonicLTriggerPin, OUTPUT);
	pinMode(frontSonicRTriggerPin, OUTPUT);
	pinMode(leftSonicTriggerPin, OUTPUT);
	pinMode(rightSonicTriggerPin, OUTPUT);
	pinMode(leftMotorPin, OUTPUT);
	pinMode(rightMotorPin, OUTPUT);
}

long getSonicDistance(int echoPin, int triggerPin){
	long distanceCM = 0;
	digitalWrite(triggerPin,HIGH);
	delayMicroseconds(300);
	digitalWrite(triggerPin,LOW);
	long duration = pulseIn(echoPin, HIGH,30000);
	long distance = (duration*34029L)/2000000L;
	distanceCM = distance;
	if(distanceCM == 0){
		distanceCM = 400;
	}   
	return (double)distanceCM; 
}

