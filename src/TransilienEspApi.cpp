#include <TransilienEspApi.h>
#include <TinyXML.h>


//XML processor settings
String xmlOpen;
String xmlTag;
String xmlData;
String xmlAttrib;
TinyXML xml;
uint8_t buffer[150];


String trainmiss[20];
String trainnum[20];
String traindate[20];
String trainterm[20];
String trainetat[20];
 // reset etat because etat is not available for each train

 
void XML_callback(uint8_t statusflags, char* tagName, uint16_t tagNameLen, char* data, uint16_t dataLen) 
{
  if (statusflags & STATUS_START_TAG) 
  {   
	if ( tagNameLen )
   {
    xmlOpen = String(tagName);
   }

	}  
else if  (statusflags & STATUS_END_TAG)
  {
    //Serial.print("End tag ");
    //Serial.println(tagName);
  }
else if  (statusflags & STATUS_TAG_TEXT) 
  {
    xmlTag = String(tagName);
    xmlData = String(data);
  } 
else if  (statusflags & STATUS_ATTR_TEXT) 
{	
	xmlAttrib = String(tagName);
	xmlData = String(data);
  }           
}

TransilienEspApi::TransilienEspApi(Client &client)	{
  this->client = &client;
}

String TransilienEspApi::SendGetToTransilienEsp(String command) {
  
  long now;
  bool avail;
  String body="";
  body.reserve(700);
  
  xml.reset();
  xmlTag = "";
  xmlData = "";
  xmlOpen = "";
  
  xml.init((uint8_t *)buffer, sizeof(buffer), &XML_callback);
  
  bool finishedHeaders = false;
  bool currentLineIsBlank = true;
 
 String tmpstr ="OK";
  uint16_t timeout = 0 ;  
  int i = 0 ;
  int t = 0 ;


	if (client->connect(TransilienESP_HOST, Port)) {
		Serial.println(".... connected to server");
		
		String a="";
		char c;

		int ch_count=0;
		
		client->println("GET " + command + " HTTP/1.1");
		client->println("Host: " TransilienESP_HOST);
		client->println("Authorization: Basic dG5odG45MTc6NTNZNHZyaFE="); // Base 64 Encode from your "user:password" - > dG5odG45MTc6NTNZNHZyaFE=
		client->println(F("User-Agent: arduino/1.0.0"));
		client->println();
		now=millis();
		avail=false;
		while (millis()-now<1500) {
		
		while (client->available()) {
		
		char c = client->read();     
        xml.processChar(c);
		
		if (xmlTag != "" || xmlOpen !=""){
			if(xmlOpen.endsWith(F("/train")))
			    {if (t<20){t++;} else {t=20;}}
				
				if(xmlTag.endsWith(F("/date"))) {
				traindate[t] = xmlData.c_str();
				} else if   
				(xmlTag.endsWith(F("/num"))) 	{
				trainnum[t] = xmlData.c_str();
				} else if 
				(xmlTag.endsWith(F("/miss"))) 	{
				trainmiss[t] = xmlData.c_str();
				} else if 
				(xmlTag.endsWith(F("/term"))) 	{
				trainterm[t] = xmlData.c_str();
				} else if 
				(xmlTag.endsWith(F("/etat"))) 	{
				trainetat[t] = xmlData.c_str();
				} 
				
		xmlTag = "";
		xmlData = "";
		xmlOpen ="";
		}
		
		
		if(!finishedHeaders){
          if (currentLineIsBlank && c == '\n') {
            finishedHeaders = true;
          }
		} else {
		//      
		
		ch_count++;
		body=body+c;
        
        }
		
		if (c == '\n') {
          currentLineIsBlank = true;
        } else if (c != '\r') {
          currentLineIsBlank = false;
        }
		avail=true;
			}
			
		
			if (avail) {
				//Serial.println("Body:");
//				//Serial.println(ch_count);
				//Serial.println(t);
				//Serial.println("END");
				break;
			}
	
	} 

	} 
closeClient();

return String (t) ; 
}

TransilienEspResponse TransilienEspApi::GetTransilienEspInfo(String StationB) {

	
  
  String command = "/"; 
  command += StationB;

  //Serial.println(command);
   
   //Reset train etat
   for(int i=1;i<4;i++) {trainetat[i]="";}
  
  String response = SendGetToTransilienEsp(command);
  Serial.println(response);
 
  TransilienEspResponse responseObject = TransilienEspResponse();



  if (response != "") {

responseObject.schedules0_message = traindate[1]; 
responseObject.schedules0_destination = trainterm[1];
responseObject.schedules0_etat = trainetat[1];
responseObject.schedules1_message = traindate[2]; 
responseObject.schedules1_destination = trainterm[2];
responseObject.schedules1_etat = trainetat[2]; 
responseObject.schedules2_message = traindate[3]; 
responseObject.schedules2_destination = trainterm[3];
responseObject.schedules2_etat = trainetat[3]; 
responseObject.schedules3_message = traindate[4]; 
responseObject.schedules3_destination = trainterm[4];
responseObject.schedules3_etat = trainetat[4]; 


responseObject.metadata_call = command; // "GET /schedules/bus/52/la+muette+++boulainvilliers+rer/R"
responseObject.metadata_date = "date query"; // "2018-08-27T03:48:03+02:00"
responseObject.metadata_version = 1;
    
  } else {
      responseObject.error = "Failed to parse XML Transilien";
    }

  return responseObject;
}

void TransilienEspApi::closeClient() {
  if(client->connected()){
    client->stop();
  }
}