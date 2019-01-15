#include <TransilienEspApi.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>

//------- Install From Library Manager -------



//------- Replace the following! 
char ssid[] = "SSID";       // your network SSID (name)
char password[] = "PASSWORD";  // your network key


WiFiClientSecure client;
TransilienEspApi api(client);

unsigned long api_mtbs = 60000; //mean time between api requests
unsigned long api_due_time = 0;

void setup() {

  Serial.begin(115200);

  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);
}

void printTransilienEspData(String StationB) {
  Serial.println("---------------------------------");
  Serial.println("Getting Schedule data for " + StationB);

  TransilienEspResponse response = api.GetTransilienEspInfo(StationB);
  if (response.error == "") {
    
	
    Serial.print("Prochain passage: ");
    Serial.println(response.schedules0_message );
    Serial.print("Destination: ");
    Serial.println(response.schedules0_destination);
	Serial.print("Etat: ");
	Serial.println(response.schedules0_etat);	

    
	Serial.print("Passage suivant: ");
    Serial.println(response.schedules1_message );
    Serial.print("Destination: ");
    Serial.println(response.schedules1_destination);
    Serial.print("Etat: ");
	Serial.println(response.schedules1_etat);
	
    Serial.print("Passage suivant 2: ");
    Serial.println(response.schedules2_message );
    Serial.print("Destination: ");
    Serial.println(response.schedules2_destination);
    Serial.print("Etat: ");
	Serial.println(response.schedules2_etat);
	
	Serial.print("Passage suivant 3: ");
    Serial.println(response.schedules3_message );
    Serial.print("Destination: ");
    Serial.println(response.schedules3_destination);
    Serial.print("Etat: ");
	Serial.println(response.schedules3_etat);
	
    Serial.print("Call: ");
    Serial.println(response.metadata_call);
    Serial.print("Date: ");
    Serial.println(response.metadata_date);
    Serial.print("Version: ");
    Serial.println(response.metadata_version);



  } else {
    Serial.print("Error getting data: ");
    Serial.println(response.error);
  }
  Serial.println("---------------------------------");
}

void loop() {
  unsigned long timeNow = millis();
  if ((timeNow > api_due_time))  {
    printTransilienEspData("gare/87543181/depart/87111278");
	printTransilienEspData("gare/87543181/depart/");
    api_due_time = timeNow + api_mtbs;
  }
}
