/* K3 remote power controller */
/* Tested on Arduino UNO */

/* 
 *  From K3 Manual: A remote control system can pull the POWER ON
 *  line to ground (ACC connector, pg.  20 ) to turn the K3S ON. To turn it OFF, 
 *  the controller must send the K3S a “PS0;” remote-control command via the  
 *  RS232 interface, wait at least 100 ms, then deactivate the POWER ON signal. 
 * 
 * 
 */
 
/* 
 *  Two operation modes: At power on, it waits 5 seconds, then 'pops' the K3 by
 *  cycling pin 8 to ground for 500mS. You can also open the COM port that it 
 *  makes on your PC, and send a 'p' to 'pop' the k3 anytime.
 *  
 *  Or you can manually command the line to ground with '0', to open collector with '1'.
 *  No line ending is necessary, single character commands.
 */

/*   
 *   needs a small circuit on the Uno pin 4 to swith K3's pin 8 to ground to  
 *   turn on the radio, but otherwise leave it floating. 
 *   Circuit: connect 1.5-4K resistor to base of 
 *   2N3904 transistor. Emitter to Ground. Collector to Pin 8 on the K3 ACC connector. 
 *   Ground also to Pin 5 on K3 ACC Connector. For good hygiene, put bypass caps to
 *   ground on the base of the transistor, and on the collector, to bypass any RF. 
 *   0.01uF should work. Not critical. 
 *  
 */

/* This program will pull K3_PIN low for K3_LOT_TIME milliseconds K3_DELAY_TIME after power is applied */

/* arduino pin PD4 is the Digital Pin 4*/
#define K3_PIN PD4
#define K3_LOW_TIME 500  /* time in milliseconds to make the pin low */
#define K3_DELAY_TIME 5000
#define LED_OUTPUT 13

unsigned long global_start_time = millis();
bool power_on_reset_done = false;

void K3_low() {
  pinMode(K3_PIN, OUTPUT);
  digitalWrite(K3_PIN, LOW);
  digitalWrite(LED_OUTPUT, HIGH);   // turn the LED on (HIGH is the voltage level)
}

void K3_high() {
  digitalWrite(LED_OUTPUT, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(K3_PIN, HIGH);
  pinMode(K3_PIN, OUTPUT); 
}

void pop_k3() {
  Serial.print("Turning K3 ON\n");
  K3_high();
  delay(500);
  K3_low();
  Serial.print("Finished.\n");
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  pinMode(12, OUTPUT);
  digitalWrite( 12, HIGH);
  pinMode(LED_OUTPUT, OUTPUT);
  K3_low(); 
}

void loop() {
  pinMode(12, OUTPUT);
    digitalWrite( 12, HIGH);
    
  unsigned long now_time = millis();
  if (!power_on_reset_done && ((now_time-global_start_time) > K3_DELAY_TIME)) {
    power_on_reset_done = true;
    pop_k3();  
  }

   if (Serial.available() > 0) {
      // read the incoming byte:
      byte incomingByte = Serial.read();
      if (incomingByte == 'p') {
        pop_k3();
      }
      
      if (incomingByte == '1') {
        K3_low();
        Serial.print("Line open\n");
      }
       
      if (incomingByte == '0') { 
        // 
        K3_high();
        Serial.print("Line LOW (K3 ON)\n");
      }
   }
}
