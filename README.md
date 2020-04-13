# OpenVentilator

> Welcome to the OpenVentilator project. This is an Open Source Ventilator / Mechanical Respirator for the Covid-19 Crisis.
> Specially Designed for countries in Africa / South America / Middle East and other poor regions in the world

This initiative as other projects was born on the [Open Source COVID19 Medical Supplies](https://web.facebook.com/groups/opensourcecovid19medicalsupplies/) Facebook Group by the awareness of the need to create a Ventilator solution for the scarcity plaguing our society worldwide. I contacted Jeremias Almada from Argentina who by that time had presented an Ambu solution and a Cad Design. The idea was interesting but needed improvements.

Since then we tried to establish some development and community standards and evolved the project several times.

# :heavy_exclamation_mark::heavy_exclamation_mark:DISCLAIMER 
**PROJECT STATUS:** We still need validation with health regulatory institutions and compliance with clinical requirements. - The tests on Lung Simulators are Sucessfull, limited on application depending on hardware availability but viable on emergency cases

**PLEASE DO NOT USE THIS MACHINE IF NOT NEEDED**, WE DO NOT GUARANTEE THE OPERATION OF THIS MACHINE | THIS MACHINE IS FOR EMERGENCY and HEALTH SYSTEM COLLAPSE SCENARIOS :warning: 



## Main Goal

Design, Build, Validate a reliable Ventilation Medical Equipment Project for people, regions, countries in difficult economical situations with a component and mechanical-agnostic philosophy. (This is why we didn't continue putting efforts into the projects being developed by other teams, who have a different society and economic reality)

**The equipment must have as few industrial parts as possible. If necessary, industrial parts must be easily accessible, even in small towns and villages.**

The equipment should be built independent of the main motor or the ventilation tool (AmbuBag, Bellows etc) to increase modularity of the parts and resources and also the buildability by others.

The equipment can be built with as few tools as possible, and even with the use of scrap to facilitate access to materials (Sometimes, Scrap is the only source of materials in many poor regions and cities)

The technical features are still being decided, the ventilator has been testes on a Simulator [YOU CAN CHECK THE RESULTS HERE](https://github.com/popsolutions/openventilator/tree/master/00_Documentation/SimulatorTest/SpartanV1.0)

### Technical & Medical Requirments (MVP)

|       Specification         |      Spartan model         |      Mark II                 |
|-----------------------------|:--------------------------:|-----------------------------:|
|Volume control               |Mechanical crank-wheel      |Mechanical crank-wheel        |
|Ventilation frequency        | Manual Ajusted on I:E      |Electronic native control     |
|Inhale Speed                 | 12 position Switch         |Electronic controled          |
|Exhale Speed                 | 12 position Switch         |Electronic controled          |
|Inhale/Exhale proportion     |Adjustment with I:E speed   |Electronic native control     |
|Inhale Pressure Sensing      |    Not Available           |Electronic sensor             |
|Inhale MAX Pressure Valve    |Adjustment on Tube position |Adjustable with tube position |
|Exhale Pressure Sensor       |    Not Available           |Electronic sensor             |
|Flow Sensor                  |    Not Available           |Electronic if available       |
|Inlet air filtration         | HEPA / Coffee Paper Filter |HEPA                          |
|Inlet air humidifier         | HME Filter(recommended)    |Humidity and Moistue Exchanger|
|Exhaust filtration           | BUV Filter* + Heat Chamber |BUV Filter* + Heat Chamber    |
|Peep Pressure regulator      | Vynil hose solution        |Vynil hose solution           |
|Peep Pressure indicator      | Water column indicator     |Digital UI or WaterColumn     |
|Oximeter                     | Not available              |Electronic sensor             |   
|Heartbeat sensor             | Not available              |Electronic Sensor             |




## Risk Control
- Inalhation speed 1 ~ 6 s
- Exalhation speed 1 ~ 6 s
- BPM from 10 to 35 cycles a minute
- PEEP Valve control between 1mm/h20 ~ 30mm/h20
- Maximum Overpressure Valve (adjustable from 5cm/h2o ~ 50cm/h20)
- Mechanical Volumetric Ventilation Adjustment
- Contagion reduction by contaminated air on the Exhalation Circuit



## Success Criteria

Validated Prototype Thanks to [SAMTRONICS](http://www.samtronic.com.br/) This team was super professional and lent us one of the pulmonary simulators that they have available at the company.

## Current Status

<p float="left">
	<img src="https://www.popsolutions.co/web/image/64981/open%20respirator%20v1.11.jpg" alt="OpenVentilator" height="200">
	<img src="https://www.popsolutions.co/web/image/64982/open%20respirator%20v1.12.jpg" alt="OpenVentilator" height="200">
	<img src="https://www.popsolutions.co/web/image/64987/open%20respirator%20v1.17.jpg" alt="OpenVentilator" height="200">
</p>

## Modules 

- **Electronic Controller** Status `Use if Available` on the Spartan Model only Reles, Diodes and Switches
- **Mechanical Motor** Status `Mandatory` we recomend Bosch VW motor by the availability and stamina delivered
- **Humidifier**  Status:  `Deprecated` we are now using HME filter widly available.
- **Bellow Ventilator** Status `Recommended` We don't belive Ambu-bags are reliable as a Mission Critical Device. Use a Tire.
- **Filtering** Status `Adopted` Is the only way we managed to reduce contamination on the outake system
- **Peep Valve** Status:  `Adopted` - Pressure control with pipe height adjustment
- **Over Pressure Valve** Status `Adopted` - This is the Garantee that personal using this machine won't kill someone
- **Diverter Valve** Status `Adopted` - Please use it on the Vertical Position THIS IS MANDATORY to be reliable
- **O2 mixing system** Status `Researching & Testing` 

![Software Hardware overview](07_Software/OpenVentilator25_03.png)

### [Research we based on](https://github.com/popsolutions/openventilator/00_Documentation/Research)

### For more information: https://www.popsolutions.co/openventilator

## If you want to help

[First, ** CLICK HERE ** to complete the form please, so we can organize everybody](https://www.popsolutions.co/openventilator-jointheteam) 

Then join the Whatsapp group and talk with Amanda (+55 11 99735-5042 ): https://chat.whatsapp.com/HRMx9xzVdt8Gpmwgm7ZVZ3

### Slack Channel for work-in-progress discussions...

**Documentation, Hardware, Design and Code discussions** This was deprecated to avoid outsiders looking for profit</br>
[**Check the decisions ALREADY MADE**](https://openventilator-c-19.slack.com/archives/C010KFG8MUP)

### If we have seen further it is by standing on the shoulders of Giants.

Special thanks to:
 - [The most badass guy in the world that doesn't want to be mentioned]
 - [Jose Ignácio Méndez](https://www.linkedin.com/in/jos%C3%A9-ignacio-m%C3%A9ndez-0ba3ab53/)
 - [Jeremias Almada](https://www.linkedin.com/in/almada-jerem%C3%ADas-43888680)
 - Fabian Franz
 - [Washington Perez](https://www.linkedin.com/in/washingtonperez/) 
 - [Jaqueline Passos](https://www.linkedin.com/in/jaquelinepassos/)
 - [Amanda Pellini](https://www.linkedin.com/in/amanda-cristina-maciel-pellini-9177226a/)
 - [Marguel Gutierrez](https://www.linkedin.com/in/marguelgtz/)
 - [Henrique Aguilar](https://www.linkedin.com/in/henriaguilar/)
 - [Vandeir Soares](https://www.facebook.com/vandeir.soares.7)
 - [Fábio Soares](https://www.linkedin.com/in/fabio-julio-sores-soares-58852630/)
 - [GlobaltTech](http://www.globaltechc.com.br/)
 - [Samtronic](http://www.samtronic.com.br/)
 - [Ethan Moses](https://www.cameradactyl.com/)
 - [Joris Robijn](https://www.linkedin.com/in/jorisrobijn/)
 - [Ramon Bastos]
 - [Faizan Shaikh](https://www.linkedin.com/in/faizanzshaikh)
 - [Matheus Prado]
 - [Leonardo Automni]
 - [Diego Sangiorgi]
 - [Rodrigo Song]
 - [Wendell Mendes](https://www.linkedin.com/in/1endell)
 - [Rodrigo Borges](http://linkedin.com/in/rborges111)
 - [Henrique Nery](https://www.linkedin.com/in/henrique-nery-650216a2/) 
 - [Duit](https://www.duit.com.br/)
 - [Carlos Delfino](https://github.com/CarlosDelfino)
 - [Marcio Dultra](https://www.linkedin.com/in/marciodultra)
 - [Baú da Eletrônica](https://www.baudaeletronica.com.br/)
 - [Três meninas hardware store](https://www.google.com/maps/place/Casa+das+3+Meninas/@-23.5391312,-46.6524764,19.5z/data=!4m5!3m4!1s0x0:0x377232460c40d90d!8m2!3d-23.5391706!4d-46.6524278)
 - [Rogers Guedes]
 - [The MIT guys from this paper](https://web.mit.edu/2.75/projects/DMD_2010_Al_Husseini.pdf): Abdul Mohsen Al Husseini, Heon Ju Lee, Justin Negrete, Stephen Powelson, Amelia Servil, Alexander Slocum, Jussi Saukkonen. 

All our families, wives and husbands that for the last days have been supporting us on our craziness.

All the doctors, nurses and paramedics in the field fighting this common enemy.
