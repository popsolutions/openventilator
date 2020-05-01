![](../images/OpenVentilatorLogoSmall.png) **OPEN VENTILATOR - EXHAUST SECTION**

In this directory you can find the build descriptions and information for the parts of the exhaust section. They are one part of the system as a whole. To get an overview of the system as a whole, see the [toplevel description](../SystemDescription.md) or the [assembly document](../Assembly.md).

**What's here**

This section there are two system parts that are partly combined: pressure "valves" and air cleaning solutions. You will need both.

***Pressure "valves"***

In a normal ventilation machine, these are formed by relatively simple, but well calibrated spring-loaded valves. Since we assume those are not available, we need an other easy way to create a given pressure threshold. The water column allows to do this, by setting the depth of the tubes in the water. We create two pressure thresholds this way: PEEP and overpressure (OVP). See the [RegulatingWaterColumn](RegulatingWaterColumn) for this.
  
***Air cleaning solution***

The water column is the first part of the air cleaning, since it is filled with chloride water. But next, the air should be filtered better with an UV-C lamp, and perhaps also by heating the air to well above 60 degrees. See [UVSterilizer](UVSterilizer).
