/*
 Name:		mega_cap_I2Cslave.ino
 Created:	4/15/2018 8:43:27 PM
 Author:	jcats
*/
#include <CapacitiveSensor.h>
#include <Wire.h>
 
// unique address of the slave device
// has to be unique within all devices connected to master over i2c
const int i2c_address = 5;


// each pad is an object of type CapacitiveSensor
// each slider is an array of 5 pads

CapacitiveSensor SlideOne[5] = {
	CapacitiveSensor(2,22),
	CapacitiveSensor(2,24),
	CapacitiveSensor(2,26),
	CapacitiveSensor(2,28),
	CapacitiveSensor(2,30),
};

CapacitiveSensor SlideTwo[5] = {
	CapacitiveSensor(3,23),
	CapacitiveSensor(3,25),
	CapacitiveSensor(3,27),
	CapacitiveSensor(3,29),
	CapacitiveSensor(3,31),
};

CapacitiveSensor SlideThree[5] = {
	CapacitiveSensor(4,32),
	CapacitiveSensor(4,34),
	CapacitiveSensor(4,36),
	CapacitiveSensor(4,38),
	CapacitiveSensor(4,40),
	
};

CapacitiveSensor SlideFour[5] = {
	CapacitiveSensor(5,33),
	CapacitiveSensor(5,35),
	CapacitiveSensor(5,37),
	CapacitiveSensor(5,39),
	CapacitiveSensor(5,41),
};

CapacitiveSensor SlideFive[5] = {
	CapacitiveSensor(6,42),
	CapacitiveSensor(6,44),
	CapacitiveSensor(6,46),
	CapacitiveSensor(6,48),
	CapacitiveSensor(6,50),

};

CapacitiveSensor SlideSix[5] = {
	CapacitiveSensor(7,43),
	CapacitiveSensor(7,45),
	CapacitiveSensor(7,47),
	CapacitiveSensor(7,49),
	CapacitiveSensor(7,51),
};

// set initial positions for all sliders to zero
uint8_t position[6] = { 0 };


uint8_t readSlider(CapacitiveSensor Slide[5],int samples,uint8_t pos,int threshold)
{
	int read[5] = { 0 };
	int weighted_sum = 0;
	int total_cap = 0;
	int numtouched = 0;
	float position;


		for (int i = 0; i < 5; i++) // for each cap slider
	{
  // read each pad
		if (Slide[i].capacitiveSensor(1) <= 0)
		{ // if it's not connected, output =0
			Serial.print("Sensor ");
				Serial.print(i);
				Serial.println(" not attached!,timeout");
			read[i] = 0;
		}
		else
		{ // if it's connected take the output
			read[i] = Slide[i].capacitiveSensor(samples);
			constrain(read[i], 0, 5000);
		}
		if (read[i] > threshold)
		{ 
			weighted_sum += (i + 1)*read[i];
			total_cap += read[i];
			numtouched++;
		}
	}
	if (numtouched > 0)
	{
  //((float(weighted_sum) / float(total_cap))-1)\
  //will end up being between 0 and 4, 0 being the lowest slider position
  //the range is 0-120, midi byte can have a max of 128? 
		position = 30* (((float(weighted_sum) / float(total_cap))-1));
		//Serial.print("the position is ");
		return uint8_t(position);
		//Serial.println(position);	
	}
	else return pos;
}



void send_positions()
{
	for (int i = 0; i < 6; i++)
	{
		position[i] = constrain(position[i], 0, 120);
		Wire.write((position[i]));


	}
	Serial.println("message sent");
}




/*---------------------MAIN-------------------------*/

// the setup function runs once when you press reset or power the board
void setup() 
{
	Wire.begin(i2c_address);
	Wire.onRequest(send_positions);
}

//
int threshold = 200;
// the loop function runs over and over again until power down or reset
void loop() {
  // send it the previous 
	position[0]=  readSliderdebug(SlideOne, 10, position[0],threshold);

	position[1] = readSliderdebug(SlideTwo, 10, position[1], threshold);

	position[2] = readSliderdebug(SlideThree, 10, position[2], threshold);

	position[3] = readSliderdebug(SlideFour, 10, position[3], threshold);

	position[4] = readSliderdebug(SlideFive, 10, position[4], threshold);

	position[5] = readSliderdebug2(SlideSix, 10, position[5], threshold);
	
	// wait one second, cap sensors need some time to discharge
	delay(1);

}



/*---------DEBUG--------------------------*/



uint8_t readSliderdebug(CapacitiveSensor Slide[5], int samples, uint8_t pos, int threshold)
{
  int read[5] = { 0 };
  float weighted_sum = 0;
  float total_cap = 0;
  int numtouched = 0;
  float position;

  for (int i = 0; i < 5; i++)
  {
    if (Slide[i].capacitiveSensor(1) <= -2)
    {
      Serial.print("Sensor ");
      Serial.print(i);
      Serial.println(" not attached!,timeout");
      read[i] = 0;

    }
    else
    {
      read[i] = Slide[i].capacitiveSensor(samples);
      constrain(read[i], 0, 5000);
      //Serial.println(read[i]);
    }
    if (read[i] > threshold)
    {
      weighted_sum += (i + 1)*read[i];
      total_cap += read[i];
      numtouched++;
    }
  }
  //Serial.println(numtouched);
  
  if (numtouched > 0)
  {
    //Serial.println((float(weighted_sum) / float(total_cap)));
    position = 30 * (((weighted_sum / total_cap) - 1));

    Serial.println(position);
    //Serial.print("the position is ");
    return uint8_t(position);
    Serial.println(position); 
  }
  else return pos;
}




uint8_t readSliderdebug2(CapacitiveSensor Slide[5], int samples, uint8_t pos, int threshold)
{
  // allocate an array to hold the readings, initialize all to zero
  int read[5] = { 0 };
  float weighted_sum = 0;
  float total_cap = 0;
  int numtouched = 0;
  float position;

  for (int i = 0; i < 5; i++)
  {
    if (Slide[i].capacitiveSensor(1) <= -2)
    {
      Serial.print("Sensor ");
      Serial.print(i);
      Serial.println(" not attached!,timeout");
      read[i] = 0;

    }
    else
    {
      read[i] = Slide[i].capacitiveSensor(samples);
      constrain(read[i], 0, 5000);
      Serial.println(read[i]);
    }
    if (read[i] > threshold)
    {
      weighted_sum += (i + 1)*read[i];
      total_cap += read[i];
      numtouched++;
    }
  }
  //Serial.println(numtouched);

  if (numtouched > 0)
  {
    //Serial.println((float(weighted_sum) / float(total_cap)));
    position = 30 * (((weighted_sum / total_cap) - 1));

    Serial.println(position);
    //Serial.print("the position is ");
    return uint8_t(position);
    Serial.println(position);
  }
  else return pos;
}




