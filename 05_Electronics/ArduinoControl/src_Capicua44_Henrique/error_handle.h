//class to handle all the error messages for the openventilator system 
//autor: Rodrigo Borges
//date: 03.22.2020

class error_handle {   
  public:                       
    int errorcode      = -1;    // Attribute error message code
	bool stoped        = false; // Attribute running system status ... if not stoped... 
	bool alarmed       = false; // Attribute to turn alarm on or off 
    string messagetext = '';    // Attribute message text
	
	void seterror(int errorcode) {				
		stop(errorcode);	
		alarm(errorcode);	
        cout geterrormsg(errorcode)
    }	
  private:
    // errormsg structure ["error message", "stop comand", "alar comand"]
    string errormsg[10][3] = {	{"00 - unknow error message", 0, 0}, 
								{"01 - message 1", 0, 0}, 
								{"02 - message 2", 0, 0}, 
								{"03 - message 3", 0, 0}, 
								{"04 - message 4", 0, 0}, 
								{"05 - message 5", 0, 0}, 
								{"06 - message 6", 0, 0}, 
								{"07 - message 7", 0, 0}, 
								{"08 - message 8", 0, 0}, 
								{"09 - message 9", 0, 0}}; 
	// method stop to let programa stop or continue							  
    void stop(int errorcode) {		
            stoped = (errormsg[errorcode][2] == 1);
    };
	// method alarm to show if the alarm must be ringing or not							  
	void alarm(int errorcode) {
			alarmed = (errormsg[errorcode][3] == 1);
    };
	// method geterrormsg to get the error message for a given errorcode							  
	void geterrormsg(int errorcode){
		cout errormsg[errorcode]
    };
	
void sendalarm(bool alarmed){
	if alarmed{
		//set alarm pin here
	}
}

int main() {
  error_handle errorobj;  // object to handle de error messages
  int errorcode;
  
  if not errorobj.stoped{
	  try{
		  sendalarm(errorobj.alarmed);
		  }		  
		  //...code continue here
		  //...
		  //...
	  }	  
	  catch(){
		  errorobj.seterror(errodcode)
	  }	  
  }  
  return 0;
}