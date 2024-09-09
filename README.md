# PKE Meter Custom Arduino Nano Every Modifications
## Notes
* I have included a basic wiring diagram for this project. The wiring diagram does not show any LED replacements/changes, but for those the wiring is simple, just wire positive side of the LED to the corresponding Pin, and negative side to the ground rail.
* I have also not included how to wire up the battery to the charge board, as that is self evident - red wire to positive terminal, black wire to negative terminal.
* I have also not included how to wire up the charge board to the Arduino; for this you should connect the 5V out negative terminal to the Arduino ground, and connect the 5V out positive terminal to the PKE Meter switch, and then the switch on to the Arduino VIN pin. 
* The DFPlayer mini can be connected directly to a speaker via the SPK_1 and SPK_2 pins; or to an amplifier board via the DAC_R and DAC_L pins. An amplifier board will also need to get power from the Arduino's 3.3V out and GND.

## Materials List
* [Arduino Nano Every](https://www.amazon.com/Arduino-Nano-Every-Single-Board/dp/B07VX7MX27)
* [DFPlayer Mini](https://www.amazon.com/DFPlayer-A-Mini-MP3-Player/dp/B089D5NLW1)
* [Battery](https://www.amazon.com/gp/product/B082152887/)
* [USB C 5V Step Up Converter Charge Board](https://www.amazon.com/gp/product/B09YD5C9QC/)
* [Amplifier](https://www.amazon.com/gp/product/B00LODGV64)
## Wiring Diagram
![Wiring Diagram for the Arduino Nano Every to PKE Meter and DFPlayer Mini](https://github.com/dylanyaga/PKEMeter_ArduinoNanoEvery/blob/main/WiringDiagram.png?raw=true)
