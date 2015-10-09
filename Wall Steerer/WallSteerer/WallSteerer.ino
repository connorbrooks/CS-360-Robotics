//Packages
#include <Arduino.h>
#include <Math.h>

//Global Variable Declarations
	
	//PINS

		//Input Pins
			const int frontSonicInputPin = 31;//PUT THE NUMBER OF THE PIN THAT IS CONNECTED TO THE SENSOR'S ECHO HERE
			const int leftSonicInputPin = 35;//PUT THE NUMBER OF THE PIN THAT IS CONNECTED TO THE SENSOR'S ECHO HERE
			const int rightSonicInputPin = 33;//PUT THE NUMBER OF THE PIN THAT IS CONNECTED TO THE SENSOR'S ECHO HERE

		//Output Pins 
			const int frontSonicTriggerPin = 30;//PUT THE NUMBER OF THE PIN THAT IS CONNECTED TO THE SENSOR'S TRIGGER HERE
			const int leftSonicTriggerPin = 34;//PUT THE NUMBER OF THE PIN THAT IS CONNECTED TO THE SENSOR'S TRIGGER HERE
			const int rightSonicTriggerPin = 32;//PUT THE NUMBER OF THE PIN THAT IS CONNECTED TO THE SENSOR'S TRIGGER HERE

			const int leftMotorPin = 3;//LEFT MOTOR PIN HERE ---> NEEDS TO BE BETWEEN 2-13
			const int rightMotorPin = 2;//RIGHT MOTOR PIN HERE ---> NEEDS TO BE BETWEEN 2-13
	//GENERAL
    
		long frontReading;
		long leftReading;
		long rightReading;
    
	    long lastFront;
	    long lastLeft;
	    long lastRight;

	    long angularDist;

	    //[left, right]
	    boolean adjustsNeeded[] = {false, false};

void setup() {
	Serial.begin(9600);
	loadPins();

	frontReading = getSonicDistance(frontSonicInputPin, frontSonicTriggerPin);
	leftReading = getSonicDistance(leftSonicInputPin, leftSonicTriggerPin);
	rightReading = getSonicDistance(rightSonicInputPin, rightSonicTriggerPin);
}

void loop() {
	lastFront = frontReading;
	frontReading = getSonicDistance(frontSonicInputPin, frontSonicTriggerPin);
  	lastLeft = leftReading;
  	leftReading = getSonicDistance(leftSonicInputPin, leftSonicTriggerPin);
  	lastRight = rightReading;
    rightReading = getSonicDistance(rightSonicInputPin, rightSonicTriggerPin);

	Serial.print("Front: ");
	Serial.print(frontReading);
	Serial.print(",\tLeft: ");
	Serial.print(leftReading);
	Serial.print(",\tRight: ");
	Serial.println(rightReading);

	if(frontReading > 70){
		//go forward

    	checkAngularDistance();

    	if(adjustsNeeded[0] = true){
    		//turn left some
    		analogWrite(leftMotorPin, 25);
				analogWrite(rightMotorPin, 60);
    	} else if (adjustsNeeded[1] = true) {
    		//turn right some
			  analogWrite(leftMotorPin, 60);
			  analogWrite(rightMotorPin, 25);
		  } else {
        //go straight
        analogWrite(leftMotorPin, 60);
        analogWrite(rightMotorPin, 60);   
		  }

		delay(50);

	} else {
	//stop movement and poll sides again

    analogWrite(leftMotorPin, 0);
    analogWrite(rightMotorPin, 0);
    
    delay(50);

    leftReading = getSonicDistance(leftSonicInputPin, leftSonicTriggerPin);
  
    rightReading = getSonicDistance(rightSonicInputPin, rightSonicTriggerPin);

    //poll and turn whichever way has more space
		//turn whichever way has more space
		if(rightReading > leftReading){
			//turn right
			analogWrite(leftMotorPin, 50);
			analogWrite(rightMotorPin, 0);
      		delay(200);
		} else{
			//turn left
			analogWrite(rightMotorPin, 50);
			analogWrite(leftMotorPin, 0);
      		delay(200);
		}
	}

}


void checkAngularDistance(){
	//use delta of side readings to guess which side robot is moving towards
	if(leftReading - lastLeft > rightReading - lastRight){
			adjustsNeeded[0] = false;
			//use geometric identities to estimate absolute distance to nearest wall on left side;
			angularDist = (long) ( (double)frontReading * sin(atan(((double)leftReading/(double)frontReading))) );
			//see if we need to turn towards the right some
			adjustsNeeded[1] = (angularDist < 70);
	} else {
			adjustsNeeded[1] = false;
			//use geometric identities to estimate absolute distance to nearest wall on right side;
			angularDist = (long) ( (double)frontReading * sin(atan(((double)rightReading/(double)frontReading))) );
			//see if we need to turn towards the left some
			adjustsNeeded[0] = (angularDist < 70);
	}
}


void loadPins(){
	//Input Pins
	pinMode(frontSonicInputPin, INPUT);
	pinMode(leftSonicInputPin, INPUT);
	pinMode(rightSonicInputPin, INPUT);

	//Output Pins
	pinMode(frontSonicTriggerPin, OUTPUT);
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


