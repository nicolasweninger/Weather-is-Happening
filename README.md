# Weather is Happening
This is a Formlabs 2018 Hackathon Project, built by myself, Ben Parnas (https://www.linkedin.com/in/beparnas/) and Thomas Orr (https://www.linkedin.com/in/thomas-orr-a47b5176/). The project is inspired by the work of the bizzare Boston weather service weatherishappening.com.

The system consists of two parts: the remote weather station and the base station. At a high level, the system monitors windspeed and at a certain threshold, the base station will make a siren alarm sound and illuminate the red light.

The remote weather station is battery powered, waterproof and radio-enabled, allowing it to remotely transmit windspeed data to the weather base station. It has a relatively simple design, using only an Arduino clone, a radio module, an anemometer and a boost converter to step up the 3.7V battery voltage to 12V for the anemometer. 

The base station contains the same Arduino clone and radio module. It is powered by a 12V DC mains power supply. The light beacon and buzzer run off 12V, and the Arduino only has digital outputs of 3V3, so we needed to use some sort of switching mechanism. The only kind we had available to us on the day were mechanical relays with a 5V switching voltage. So, we connected the 3V3 Arduino outputs to transistors that were able to switch a >5V to the switch input of the relay, which in turn would switch power to the beacon and buzzer. All in all, it worked! 

A video demonstration can be found at https://www.youtube.com/watch?v=7nbSRbI2BcI
