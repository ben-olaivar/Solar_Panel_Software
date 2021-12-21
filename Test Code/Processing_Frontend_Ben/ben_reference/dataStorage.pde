class data {
  /* OBJECT VARIABLES */
  float temp1;
  float temp2;
  float temp3;
  float voltage;
  float current;
  boolean overHeat;
  boolean overVolt;
  boolean overCurr;
  boolean shutdown;

  /* GENERAL VARIABLES */
  float tempThreshFarenheit = 10;       //! arbitrary number chosen for testing
  float voltageTrhesh = 10;             //! arbitrary number chosen for testing
  float currentThresh = 10;             //! arbitrary number chosen for testing

  /* Default constructor
  *     Sets all object variables to 0/false
  */
  public data() {
    temp1 = 0;
    temp2 = 0;
    temp3 = 0;
    voltage = 0;
    current = 0;
    overHeat = false;
    overVolt = false;
    overCurr = false;
    shutdown = false;
  }
  
  /* Constructor
  *     Sets all object variables to given values
  */
  public data(
      float t1, 
      float t2, 
      float t3, 
      float v, 
      float c, 
      boolean oh, 
      boolean ov, 
      boolean oc,
      boolean sd
    ) {
    temp1 = t1;
    temp2 = t2;
    temp3 = t3;
    voltage = v;
    current = c;
    overHeat = oh;
    overVolt = ov;
    overCurr = oc;
    shutdown = sd;
  }  

  /* Function: setTemps
  *     Sets temperature in degrees Farenheit (data from each node is in Farenheit)
  *     groups all 3 temperature values together to update temps (can be easily modified if we add more/remove temp sensors)
  */
  void setTemps(float t1, float t2, float t3) {
    temp1 = t1;
    temp2 = t2;
    temp3 = t3;
    
    if (t1 > tempThreshFarenheit) {
        overHeat = true;
        this.setShutdown(true);
    } else {
        overHeat = false;
        this.setShutdown(false);
    }
  }
  /* Function: getTempsFarenheit
  *     return the temperature values (FARENHEIT) in a float array. This can easily be modified if we add/remove temp sensors
  */
  float[] getTempsFarenheit() {
    float[] tempArray = {temp1, temp2, temp3};
    return tempArray;
  }
  /* Function: getTempsCelcius
  *     return the temperature values (CELCIUS) in a float array. This can easily be modified if we add/remove temp sensors
  */
  float[] getTempsCelcius() {
    float[] temps = getTempsFarenheit();
    for (int i = 0; i < temps.length; i++) {    // left as temps.length just in case we add/remove temp sensors
        temps[i] -= 32;
        temps[i] *= 5;
        temps[i] /= 9;
    }
    return temps;
  }

  /* Function: setVoltage
  *     sets the voltage in Volts
  */
  void setVoltage(float v) {
    voltage = v;
    if (v > voltageTrhesh) {
      overVolt = true;
        this.setShutdown(true);
    } else {
      overVolt = false;
        this.setShutdown(false);
    }
  }
  /* Function: getVoltage
  *     returns the voltage in Volts
  */
  float getVoltage() {
    return voltage;
  }

  /* Function: setCurrent
  *     sets teh current in Amps
  */
  void setCurrent(float c) {
    current = c;
    if (c > currentThresh) {
      overCurr = true;
        this.setShutdown(true);
    } else {
      overCurr = false;
        this.setShutdown(false);
    }
  }
  /* Function: getCurrent
  *     returns the current in Amps
  */
  float getCurrent() {
    return current;
  }
  
  /* Function: setShutdown
  *     changes the state of the 'shutdown' boolean. Other methods call this function
  */
  void setShutdown(boolean s) {
    shutdown = s;
  }
  /* Function: getCurrent
  *     returns true if node is in shutdown mode, else returns false
  */
  boolean isShutdown() {
    return shutdown;
  } 

}
