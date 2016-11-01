# The Photon Radon Controller

In short, we've taken Particle's Photon to control a home's radon system based on external environmental data such as temperature.

Watch on YouTube: https://www.youtube.com/watch?v=rYFEdOL03ME

# What is Radon?

> Radon is a chemical element with symbol Rn and atomic number 86. It is a radioactive, colorless, odorless, tasteless[2] noble gas, occurring naturally as a decay product of radium. Its most stable isotope, 222Rn, has a half-life of 3.8 days. Radon is one of the densest substances that remains a gas under normal conditions. It is also the only gas under normal conditions that has no stable isotopes, and is considered a health hazard due to its radioactivity. Intense radioactivity has also hindered chemical studies of radon and only a few compounds are known.

Source - https://en.wikipedia.org/wiki/Radon

Useful Links
* EPA on Radon - https://www.epa.gov/radon
* EPA map of Radon - https://www.epa.gov/radon/find-information-about-local-radon-zones-and-state-contact-information#radonmap
* Radon Mitigation https://en.wikipedia.org/wiki/Radon_mitigation

# Problem Statement

During the normal summer months the radon mitigation system works as expected. In my house it lowers the radon levels from ~8.0 to ~0.6 depending on a few environmental factors such as rainfall.

Durning the winter months especially when he temperature dips below freezing, it's not uncommon for the warm air being expelled to condensate and freeze the outside pipe solid. Once frozen the radon mitigation system is useless and the radon levels return to their original state.

# Project Goals

Keep the pipe from freezing in the cold winters months therefor keeping the system active.

# Use Cases

* When the temperature outside drops below freezing, turn on a deicer which warms up the outside pipe.
* When the temperature outside drops below some state deactivate the fan.
* If the pressure in the exhaust pipe and if it looses vacuum, deactivate the fan and activate the deicer.

# Hardware Used in this Build

* Particle's Photon - https://docs.particle.io/datasheets/photon-datasheet/
* Particle's Relay Shield - https://docs.particle.io/datasheets/particle-shields/#relay-shield
* Adafruit Thermistor - https://www.adafruit.com/products/372
* Pressure Transducer - MPX5100GP http://www.digikey.com/product-detail/en/freescale-semiconductor-nxp/MPX5100GP/MPX5100GP-ND/464061
* Various LEDs, Resistors, and Capacitors,
* Adafruit's PermaProto Board - https://www.adafruit.com/products/1609
* Power Supply - RockBirds RB-1202 New DC 12V 2.0A Switching Power Supply Adapter For 110V- 240V
* PVC to mount everything.

# Sketch
  Coming.

# Future Goals

* Keep a running log of the temperature and pressure adjusting the threshold values to keep the fan running for the longest amount of time possible.
* Build a mobile app with live data retrieved from the radon controller.
