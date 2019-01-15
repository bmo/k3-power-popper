# k3-power-popper
A very simple Arduino-based project to turn on an Elecraft K3 from the accessory connector pin 8. 

Works with nearly any Arduino board, I used an Arduino UNO for this, since it was lying around. Requires an additional resistor, NPN transistor, and bypass capacitors are recommended. See the .INO file for a description of the schematic.

## Operation
### Stand Alone Mode
When power is applied to the Arduino via USB or Vcc, after five seconds, the designated output pin (e.g. pin 4) will be asserted for 500ms.
Then the line will be de-asserted. If a transistor switching circuit (as detailed in the .INO) is connected to the output pin and
an Elecraft K3's ACC connector pin 8, it will pull-down the ~13v present to ground, which turns on the radio. 

### Controlled Mode
Additionally, if the Arduino is connected via USB to a PC (it will show up as a COM port), *the initial power-on operation will occur*,
AND the line status can be controlled by sending characters via the serial port (57,600 baud). No line endings are necessary, single 
character commands work:
    - p (lower case 'P') - pop the K3 line with a transition to ground, then open collector, turning on the radio if it's off
    - 0 (zero character) - switch the K3 line to ground
    - 1 (one character ) - switch the K3 line to open collector
    
