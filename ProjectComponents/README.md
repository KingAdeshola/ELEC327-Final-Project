#MoistureSensor-I2C-Display-MotorPump-and-MSP430G2553

##Motivations
Sprinklers, while useful, present a unique problem of water wastage. In many cases, soil can be oversaturated, leading to a phenomenon known as waterlogging where plant soil is saturated to the point that little or no air is present in soil pores. This neglects to account for those who forget to water their plants. To overcome this issue, considering the plethora of other obstacles that disrupt optimal plant hydration, we decided to build a self-watering system that automatically provides plants with water based on the current soil moisture.
##Process
  To accomplish this, we utilized a 16x2 LCD Display, a soil moisture sensor, a TONGLING low-level trigger as a motor activation switch, an MSP430G2553, an MH sensor series, and a water pump motor. We will read the analog data out of a moisture sensor probed into the soil, process the moisture data through the MH sensor series, convert it from an analog signal to a digital signal, and vary what is displayed based on the converted digital signal. The moisture sensor determines soil moisture by pushing a current through the soul, identifying the soil resistance, and converting this into a moisture value that is received as an analog signal.
##Modes
  There are three modes for the system that are determined by the sensor moisture data. This moisture reading is in a range between 0 and 1023 with 0 being the most moist and 1023 being the driest. The first mode is the “Soil too wet” mode which occurs when the sensor reading is lower than 500, in which case the water pump will disengage. The second mode is the “Moisture OK” mode, which occurs when the sensor reading is between 500 and 750. This mode also disengages the water pump if it is currently engaged. The third mode is the “Water needed” mode, which occurs when the sensor reading is above 750. In this mode, the water pump automatically engages and begins to water the plant. All three modes are displayed on the 16x2 LCD display along with the soil moisture readings. 
##Water Pump
  The water pump is controlled by a low-level trigger that is connected to pin 2.3 of an MSP430G2553. If the state is either “Soil too wet” or “Moisture OK”, pin 2.3 will be set to the ON state, which outputs a voltage of 3.3 V, setting the low-level trigger to an OFF state. If the state is “Water needed”, pin 2.3 will be set to the OFF state, which outputs 0 V, setting the low-level trigger to an ON state that engages the water pump. This water pump will be engaged until the system enters the “Soil too wet” mode. 
##Code
  The code in the main.c file configures the analog-to-digital converter (ADC) and sets up an LCD display for output. The main loop continuously reads the moisture level using the ADC, displays it on the LCD, and checks against predefined moisture thresholds to determine soil moisture status. Depending on the moisture level, corresponding messages are displayed ("Soil too wet," "Moisture OK," or "Water needed"), and an optional LED can be controlled to reflect the soil's moisture status. The code also includes a delay for testing purposes and provides a basic framework for monitoring and displaying soil moisture conditions.
##Important Connections
MH-Sensor-Series AO to P1.3
LCD SDA is connected in series to a 300 ohm resistor as well as to pin 1.7
LCD SCL is connected in series to a 300 ohm resistor as well as to pin 1.6
The relay’s IN is connected to pin 2.3

