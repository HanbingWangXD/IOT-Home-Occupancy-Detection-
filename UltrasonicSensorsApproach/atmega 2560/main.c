/*
ECE 490/491 Group Number 2
IOT House - Occupancy Detection
Academic Year 2017-2018

Authors:
Alberto J. Leon Cevallos - 1359967
Hanbing Wang - 1443489

Sections:
1. Preprocessor Directives
2. Function Declarations
3. Variable Definitions
4. Main
5. Function Implementations
*/

/********************************************************************************* 1. Preprocessor Directives *********************************************************************************/
#include <avr/io.h>
#include <stdbool.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#define F_CPU 1000000
#include <util/delay.h>

#define FrontLeft 0
#define FrontRight 1
#define BackLeft 2
#define BackRight 3
#define enable            5
#define registerselection 6

#define BAUD 9600
#define BAUD_PRESCALE (((F_CPU / (BAUD * 16UL))) - 1)

/********************************************************************************* 2. Function Declarations *********************************************************************************/
// Pin initialization for dual mux/demux
void mux_init();

// Select sensor for echo/trigger signal
void sel_sensor(int SensorNumber);

// Pin initialization for trigger and echo signals
void trigger_echo_init();

// send trigger
int send_trigger();// send out a fast PWM, get distance value back

				   //DistanceArray is an array with 3 distances values
void updateDistance(int distance, int SensorNumber);	//The oldest distance element from distance array is discarded
														//The most recent distance values from send_trigger() function is added

														// Compare the distance values to determine if they are increasing or decreasing
int compareDistance(int SensorNumber);//check if the 3 distance elements are in ascending order

									  // Check if the distance values are stable
bool check_stable(int SensorNumber);

// Pin initialization for the UART
void uart_init();

// Send the data to the Pi
void send_data();

// Send command to LCD
void send_a_command(unsigned char command);

// Send a character to LCD
void send_a_character(unsigned char character);

// Send a string to LCD
void send_a_string(char *string_of_characters);

//  Send data to UART
void uart_sendint16(uint16_t data);

/********************************************************************************* 3. Variable Definitions *********************************************************************************/
int SensorNumber;
int SensorCount;
float echoInTime; // Data captured from the echo signal
int distance;
int distanceArray_FL[5]; // Distance array for Front Left sensor
int distanceArray_FR[5]; // Distance array for Front Right sensor
int distanceArray_BL[5]; // Distance array for Back Left sensor
int distanceArray_BR[5]; // Distance array for Back Right sensor
int newArray[3];
int Array[3];
int PeopleCount[4]; // The value in each index refers to the number of people to the according sensor number
					// For LCD
static volatile int pulse = 0;
static volatile int i = 0;

/********************************************************************************* 4. Main *********************************************************************************/
int main(void)
{
	// Delay to allow internal oscillator to stabilize
	_delay_ms(50);
	// Initialize Mux/Demux
	mux_init();
	// Initliaze trigger and echo signals
	trigger_echo_init();
	// Initialize UART for communication with Xbee
	uart_init();

	/* LCD Display *********************************************************
	DDRA = 0xFF;
	DDRB = 0xFF;
	DDRD = 0b11111110;
	_delay_ms(50);

	EIMSK |= (1 << INT0);
	MCUCR |= (1 << IVCE);
	TCCR1A = 0;
	EICRA |= (1 << ISC00);

	int16_t COUNTA = 0;
	char SHOWA[16];

	send_a_command(0x01); //Clear Screen 0x01 = 00000001
	_delay_ms(50);
	send_a_command(0x38);
	_delay_ms(50);
	send_a_command(0b00001111);
	_delay_ms(50);

	sei();
	// LCD Display ****************************************************************************/


	/* Replace with your application code */
	while (1)
	{
		// Front Left sensor
		sel_sensor(FrontLeft); // Select the sensor 
		distance = send_trigger(); // Gather distance value
		updateDistance(distance, FrontLeft); // Update distance value
											 // Check for stable values
		if (check_stable(FrontLeft)) {
			// If the values are stable, increase number of stable sensors in a row
			SensorCount++;
		}
		else {
			// If the values are not stable, check if they are increasing or decreasing and update the people count
			PeopleCount[FrontLeft] += compareDistance(FrontLeft);
		}
		// Check if 12 polls in a row are stable
		if (SensorCount == 12) {
			// Send data 
			int a = PeopleCount[0];
			int b = PeopleCount[1];
			int c = PeopleCount[2];
			int d = PeopleCount[3];
			int e = (1000 * a) + (100 * b) + (10 * c) + d;
			uart_sendint16(e);
			// Clear count
			SensorCount = 0;
		} // Go to next sensor


		  // Front Right sensor
		sel_sensor(FrontRight); // Select the sensor
		distance = send_trigger(); // Gather distance value
		updateDistance(distance, FrontRight); // Update distance value
											  // Check for stable values
		if (check_stable(FrontRight)) {
			// If the values are stable, increase number of stable sensors in a row
			SensorCount++;
		}
		else {
			// If the values are not stable, check if they are increasing or decreasing
			PeopleCount[FrontLeft] += compareDistance(FrontRight);
		}
		// Check if 12 polls in a row are stable
		if (SensorCount == 12) {
			// Send data 
			int a = PeopleCount[0];
			int b = PeopleCount[1];
			int c = PeopleCount[2];
			int d = PeopleCount[3];
			int e = (1000 * a) + (100 * b) + (10 * c) + d;
			uart_sendint16(e);
			// Clear count
			SensorCount = 0;
		} // Go to next sensor


		  // Back Left sensor
		sel_sensor(BackLeft); // Select the sensor
		distance = send_trigger(); // Gather distance value
		updateDistance(distance, BackLeft); // Update distance value
											// Check for stable values
		if (check_stable(BackLeft)) {
			// If the values are stable, increase number of stable sensors in a row
			SensorCount++;
		}
		else {
			// If the values are not stable, check if they are increasing or decreasing
			PeopleCount[FrontLeft] += compareDistance(BackLeft);
		}
		// Check if 12 polls in a row are stable
		if (SensorCount == 12) {
			// Send data 
			int a = PeopleCount[0];
			int b = PeopleCount[1];
			int c = PeopleCount[2];
			int d = PeopleCount[3];
			int e = (1000 * a) + (100 * b) + (10 * c) + d;
			uart_sendint16(e);
			// Clear count
			SensorCount = 0;
		} // Go to next sensor


		  // Back Right sensor
		sel_sensor(BackRight); // Select the sensor
		distance = send_trigger(); // Gather distance value
		updateDistance(distance, BackRight); // Update distance value
											 // Check for stable values
		if (check_stable(BackRight)) {
			// If the values are stable, increase number of stable sensors in a row
			SensorCount++;
		}
		else {
			// If the values are not stable, check if they are increasing or decreasing
			PeopleCount[FrontLeft] += compareDistance(BackRight);
		}
		// Check if 12 polls in a row are stable
		if (SensorCount == 12) {
			// Send data 
			int a = PeopleCount[0];
			int b = PeopleCount[1];
			int c = PeopleCount[2];
			int d = PeopleCount[3];
			int e = (1000 * a) + (100 * b) + (10 * c) + d;
			uart_sendint16(e);
			// Clear count
			SensorCount = 0;
		} // Go to next sensor




		  /* LCD Display ******************************************************
		  PORTD |= (1 << PIND2);
		  _delay_us(15);
		  PORTD &= ~(1 << PIND2);
		  COUNTA = pulse / 58;
		  send_a_command(0x80 + 0x40 + 0);
		  send_a_string("DISTANCE=");
		  itoa(COUNTA, SHOWA, 10);
		  send_a_string(SHOWA);
		  send_a_string("cm    ");
		  send_a_command(0x80 + 0);
		  // LCD Display ******************************************************/

		  /* Code for prototyping ******************************************************

		  // Code of prototyping  ******************************************************/
	}
}

/********************************************************************************* 5. Function Implementations *********************************************************************************/
void mux_init() {
	// Mux1 on PA3, Mux2 on PA2, Sel1 on PA1, Sel2 on PA0
	// Clear the value of the pins
	PORTA &= ~((1 << PA3) | (1 << PA2) | (1 << PA1) | (1 << PA0));
	// Set the pins to be outputs
	DDRA |= ((1 << PA3) | (1 << PA2) | (1 << PA1) | (1 << PA0));
}

void sel_sensor(int SensorNumber) {
	// Reset the pin selection by setting Mux1 and Mux2 to deactivate functionality while switching to desired sensor
	PORTA |= ((1 << PA3) | (1 << PA2));

	// Check to see sensor number
	switch (SensorNumber) {
	case FrontLeft:
		// The values of Sel to be 00
		PORTA &= ~((1 << PA1) | (1 << PA0));
		// Clear the values of Mux1 and Mux2 to activate
		PORTA &= ~((1 << PA3) | (1 << PA2));
		break;

	case FrontRight:
		// The values of Sel to be 01
		PORTA &= ~(1 << PA1);
		PORTA |= (1 << PA0);
		// Clear the values of Mux1 and Mux2 to activate
		PORTA &= ~((1 << PA3) | (1 << PA2));
		break;

	case BackLeft:
		// The values of Sel to be 10
		PORTA |= (1 << PA1);
		PORTA &= ~(1 << PA0);
		// Clear the values of Mux1 and Mux2 to activate
		PORTA &= ~((1 << PA3) | (1 << PA2));
		break;

	case BackRight:
		// The values of Sel to be 11
		PORTA |= ((1 << PA1) | (1 << PA0));
		// Clear the values of Mux1 and Mux2 to activate
		PORTA &= ~((1 << PA3) | (1 << PA2));
		break;
	}
}

void trigger_echo_init() {
	// Will be using 16bit Timer/Counter 1
	// Trigger signal on PB5, Echo signal on PD4
	// Clear the values of the needed pins
	PORTB &= ~(1 << PB5);
	PORTD &= ~(1 << PD4);
	// PB5 to be an output
	DDRB |= (1 << PB5);
	// PD4 to be an input
	DDRD &= ~(1 << PD4);
}

int send_trigger() {
	// Need to send a 10u sec pulse, period to be 100ms (need greater than 60ms)
	PORTB |= (1 << PINB5);
	_delay_us(15);// delay 15u sec pulse
	PORTB &= ~(1 << PINB5);
	// Use internal clock (16MHz default) for fastest operation CSn2:0=1
	// Clock Pre-scaler N=64, set OCR1A so that the period of PWM will be 100ms 
	// Use Mode of operation 15, so set COMnx1:0 = 2
	// Set up non-inverted PWM
	// Set the OCR1A to control frequency and use OCR1B to control duty cycle 
	TCCR1A |= (1 << WGM10) | (1 << WGM11);
	TCCR1B |= (1 << WGM12) | (1 << WGM13) | (1 << CS10) | (1 << CS11);
	TCCR1B &= ~(1 << CS12);
	TCCR1B &= ~(1 << CS10);
	TCCR1A |= (1<COM1A1);
	TCCR1A &= ~(1<COM1A0);
	OCR1A = 24999;
	OCR1B = 250;
	// Clear input capture flag
	TIFR1 = (1 << ICF1);
	// Capture on rising edge
	TCCR1B |= (1 << ICES1);
	/* monitor for capture*/
	while ((TIFR1&(1 << ICF1)) == 0);
	echoInTime = ICR1;
	/* calculate distance*/
	distance = (echoInTime*1.376) / 1000;
	/* clear capture flag */
	TIFR1 = (1 << ICF1);
	// return the value of the distance captured
	return distance;
}

void updateDistance(int distance, int SensorNumber) {

	switch (SensorNumber) {
	case FrontLeft:
		distanceArray_FL[0] = distanceArray_FL[1];
		distanceArray_FL[1] = distanceArray_FL[2];
		distanceArray_FL[2] = distanceArray_FL[3];
		distanceArray_FL[3] = distanceArray_FL[4];
		distanceArray_FL[4] = distance;

	case FrontRight:
		distanceArray_FR[0] = distanceArray_FR[1];
		distanceArray_FR[1] = distanceArray_FR[2];
		distanceArray_FR[2] = distanceArray_FR[3];
		distanceArray_FR[3] = distanceArray_FR[4];
		distanceArray_FR[4] = distance;
	case BackLeft:
		distanceArray_BL[0] = distanceArray_BL[1];
		distanceArray_BL[1] = distanceArray_BL[2];
		distanceArray_BL[2] = distanceArray_BL[3];
		distanceArray_BL[3] = distanceArray_BL[4];
		distanceArray_BL[4] = distance;

	case BackRight:
		distanceArray_BR[0] = distanceArray_BR[1];
		distanceArray_BR[1] = distanceArray_BR[2];
		distanceArray_BR[2] = distanceArray_BR[3];
		distanceArray_BR[3] = distanceArray_BR[4];
		distanceArray_BR[4] = distance;
	}
}

int compareDistance(int SensorNumber) {
	int increment_count = 0;
	switch (SensorNumber) {
	case FrontLeft:
		// Check if the numbers are decreasing
		if ((distanceArray_FL[2] > distanceArray_FL[3]) && (distanceArray_FL[3] > distanceArray_FL[4])) {
			// Reduce people count by 1
			increment_count = -1;
			return increment_count;
		}
		// Check if the numbers are increasing
		else if ((distanceArray_FL[2] < distanceArray_FL[3]) && (distanceArray_FL[3] < distanceArray_FL[4])) {
			// Increase the people count by 1
			increment_count = 1;
			return increment_count;
		}
		// In case of data variations, do not update value
		else {
			increment_count = 0;
			return increment_count;
		}

	case FrontRight:
		// Check if the numbers are decreasing
		if ((distanceArray_FR[2] > distanceArray_FR[3]) && (distanceArray_FR[3] > distanceArray_FR[4])) {
			// Reduce people count by 1
			increment_count = -1;
			return increment_count;
		}
		// Check if the numbers are increasing
		else if ((distanceArray_FR[2] < distanceArray_FR[3]) && (distanceArray_FR[3] < distanceArray_FR[4])) {
			// Increase the people count by 1
			increment_count = 1;
			return increment_count;
		}
		// In case of data variations, do not update value
		else {
			increment_count = 0;
			return increment_count;
		}

	case BackLeft:
		// Check if the numbers are decreasing
		if ((distanceArray_BL[2] > distanceArray_BL[3]) && (distanceArray_BL[3] > distanceArray_BL[4])) {
			// Reduce people count by 1
			increment_count = -1;
			return increment_count;
		}
		// Check if the numbers are increasing
		else if ((distanceArray_BL[2] < distanceArray_BL[3]) && (distanceArray_BL[3] < distanceArray_BL[4])) {
			// Increase the people count by 1
			increment_count = 1;
			return increment_count;
		}
		// In case of data variations, do not update value
		else {
			increment_count = 0;
			return increment_count;
		}

	case BackRight:
		// Check if the numbers are decreasing
		if ((distanceArray_BR[2] > distanceArray_BR[3]) && (distanceArray_BR[3] > distanceArray_BR[4])) {
			// Reduce people count by 1
			increment_count = -1;
			return increment_count;
		}
		// Check if the numbers are increasing
		else if ((distanceArray_BR[2] < distanceArray_BR[3]) && (distanceArray_BR[3] < distanceArray_BR[4])) {
			// Increase the people count by 1
			increment_count = 1;
			return increment_count;
		}
		// In case of data variations, do not update value
		else {
			increment_count = 0;
			return increment_count;
		}
	return increment_count;
}

bool check_stable(int SensorNumber) {
	switch (SensorNumber) {
	case FrontLeft:
		for (int i = 0; i<5; i++) {
			if (!(distanceArray_FL[i] && distanceArray_FL[i + 1])) {
				return false;
			}
		}
	case FrontRight:
		for (int i = 0; i<5; i++) {
			if (!(distanceArray_FR[i] && distanceArray_FR[i + 1])) {
				return false;
			}
		}
	case BackLeft:
		for (int i = 0; i<5; i++) {
			if (!(distanceArray_BL[i] && distanceArray_BL[i + 1])) {
				return false;
			}
		}
	case BackRight:
		for (int i = 0i; i<5; i++) {
			if (!(distanceArray_BR[i] && distanceArray_BR[i + 1])) {
				return false;
			}
		}
	}
	return true;
}

void uart_init() {
	// Will be using USART0 in asynchronous mode
	// Receive on PE0, Transmit on PE1
	// Cts on PE2, Rts on PE3
	// Clear the values of the needed pins
	PORTE &= ~((1 << PE3) | (1 << PE2) | (1 << PE1) | (1 << PE0));
	// PE0 to be an input
	DDRE &= ~(1 << PE0);
	// PE1 to be an output
	DDRE |= (1 << PE1);
	// PE2 to be an input
	DDRE &= ~(1 << PE2);
	// PE3 to be an output
	DDRE |= (1 << PE3);

	// Clock and Baud Rate
	UCSR0C &= ~((1 << UMSEL01) | (1 << UMSEL00)); // Asynchronous mode
	UCSR0A |= (1 << U2X0); // Double UART Transmission speed for more accurate Baud rate
	UBRR0H &= 0b0000;
	UBRR0L = 0x0C; // Set UBBR = 12 for Baud Rate 9600

				   // Set the Frame Format
	UCSR0C &= ~((1 << UPM01) | (1 << UPM00)); // Parity disabled
	UCSR0C &= ~(1 << USBS0); // Stop bit(s) = 1-bit
	UCSR0C &= ~(1 << UCSZ02);

	// Enable Transmitter
	UCSR0B |= (1 << TXEN0);
}

void send_data() {
	// Send the People totals
	// First Sensor
	while (!(UCSR0A & (1 << UDRE0))) {	// Wait for transmitter to be empty
		UDR0 = ((0b00 << 6) | PeopleCount[0]);
	}
	// Second Sensor
	while (!(UCSR0A & (1 << UDRE0))) {	// Wait for transmitter to be empty
		UDR0 = ((0b01 << 6) | PeopleCount[1]);
	}
	// Third Sensor
	while (!(UCSR0A & (1 << UDRE0))) {	// Wait for transmitter to be empty
		UDR0 = ((0b10 << 6) | PeopleCount[2]);
	}
	// Fourth Sensor
	while (!(UCSR0A & (1 << UDRE0))) {	// Wait for transmitter to be empty
		UDR0 = ((0b11 << 6) | PeopleCount[3]);
	}
}

// LCD Commands

ISR(INT0_vect) {
	if (i == 1)
	{
		TCCR1B = 0;
		pulse = TCNT1;
		TCNT1 = 0;
		i = 0;
	}
	if (i == 0)
	{
		TCCR1B |= (1 << CS10);
		i = 1;
	}
}

void send_a_command(unsigned char command) {
	PORTA = command;
	PORTD &= ~(1 << registerselection);
	PORTD |= 1 << enable;
	_delay_ms(8);
	PORTD &= ~1 << enable;
	PORTA = 0;
}

void send_a_character(unsigned char character) {
	PORTA = character;
	PORTD |= 1 << registerselection;
	PORTD |= 1 << enable;
	_delay_ms(8);
	PORTD &= ~1 << enable;
	PORTA = 0;
}

void send_a_string(char *string_of_characters) {
	while (*string_of_characters > 0)
	{
		send_a_character(*string_of_characters++);
	}
}

void uart_sendint16(uint16_t data) {
	/*
	Use this to send a 16bit long piece of data
	*/
	while ((UCSR0A & (1 << UDRE0)) == 0);//make sure the data register is cleared
	UDR0 = data;//send the lower bits
	while ((UCSR0A & (1 << UDRE0)) == 0);//make sure the data register is cleared
	UDR0 = (data >> 8); //send the higher bits
}