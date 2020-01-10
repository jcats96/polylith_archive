#include <SoftwareSerial.h>
#include <Wire.h>


int mega_sensor[] = { 2,3,4,5,6,7 };
// defines pins to communicate midi over
SoftwareSerial midiSerial(8,9);
uint8_t sensor0[5*6] = { 0 };


void setup()
{
	Wire.begin(8);
	midiSerial.begin(31250);
}

void sendMidi(byte b1, byte b2, byte b3)
{

	midiSerial.write(b1);
	midiSerial.write(b2);
	midiSerial.write(b3);
}


void loop()
{
	for (int i = 0; i < 5; i++)
	{
		Serial.print("request ");
		Serial.println(i+1);
		int wirebyte = 0;
		//Serial.println(wirebyte);
		Wire.requestFrom(i+2, 6);
		delay(15);
		//int time = millis();
		if (Wire.available())
		//Serial.println("requested");

		while (Wire.available())
		{
			char c = Wire.read(); // receive a byte as character
			//Serial.println(int(c));
			if (sensor0[6*i+wirebyte] != c||c<=-1)
			{
				sendMidi(0xB0, 0x50 + wirebyte + 6*i, c);
				Serial.println("change detected");
				Serial.print("channel ");
				Serial.println(6 * i + wirebyte);
				Serial.println(int(c));
				sensor0[6 * i + wirebyte] = (c);
			}
			wirebyte++;
			
		}

	}
	
		delay(20);

}

