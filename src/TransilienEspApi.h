
#ifndef TransilienEspApi_h
#define TransilienEspApi_h

#include <Arduino.h>
#include <Client.h>

#define TransilienESP_HOST "api.transilien.com"

struct TransilienEspResponse{

String schedules0_message; 
String schedules0_destination;
String schedules0_etat;
String schedules1_message; 
String schedules1_destination;
String schedules1_etat; 
String schedules2_message; 
String schedules2_destination;
String schedules2_etat;
String schedules3_message; 
String schedules3_destination;
String schedules3_etat;  
String metadata_call ; // 
String metadata_date ; // 
int metadata_version; // 3

String error;
};


class TransilienEspApi
{
  public:
    TransilienEspApi (Client &client);
    String SendGetToTransilienEsp(String command);
    TransilienEspResponse GetTransilienEspInfo(String StationB);
    int Port = 443;

  private:
    Client *client;
    void closeClient();
	//void xmlparse ( String &line, const char *selstr, String &res );
	
};

#endif
