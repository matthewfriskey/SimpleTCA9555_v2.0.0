#include <SimpleTCA9555.h>
SimpleTCA9555 expander(0x20);

void setup()
{
    expander.begin(21, 22, -1, 400000);
    expander.setPortAsOutput(0); // Port 0 outputs
    expander.setPortAsInput(1);  // Port 1 inputs
}

void loop()
{
    expander.setOutputPin(1, HIGH);
    delay(500);
    expander.setOutputPin(1, LOW);
    delay(500);
    expander.update();
}