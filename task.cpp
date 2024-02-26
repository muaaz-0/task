#include <iostream>
#include <string.h>
#include <stdio.h>
using namespace std;

char calculated_checksum_hex[3];
enum Tokens {sentenceID, time, latitude, latDir, logitude, longitudeDir, QualityIndicator, Numofsattelites, horizontaldilution, altitude, altitudeUnits, meansealevelheight, geoidseparationUnits, timesincelastDC, differentialstationID};// enumeration of the tokens to use in switch statement

int Start_Parsing(string packet);
void Tokenize_data(char final_sentence[]);
void manage_missing(string packet);
bool unit_test1();
void print();

struct GPSData{
    string sentenceID;
    string time;
    string latitude;
    char latDir;
    string longitude;
    char longDir;
    unsigned int QualityIndicator;
    unsigned int NumofSat;
    string horizontaldilution;
    string altitude;
    char altitudeUnits;
    string meansealevelheight;
    char geoidseparationUnits;
    string timesincelastDC;
    string differentialstationID;
    string checksum;

}GPSdata;

//struct GPSdata ind_param;
int main()
{
    string Sentence="$GPGGA,002153.000,3342.6618,N,11751.3858,W,1,10,1.2,27.0,M,-34.2,M,,0000*5E";

    //string Sentence="$GPGGA,115739.00,4158.8441367,N,09147.4416929,W,4,13,0.9,255.747,M,-32.00,M,01,0000*6E";
    //string Sentence="$GPGGA,002153.000,3342.6618,N,11751.3858,W,1,10,1.2,27.0,M,-34.2,M,,0000*5E"   //invalid sentence

Start_Parsing(Sentence);
print();
if (unit_test1())
{
    cout<<"Test 1 Passed"<<endl;
}
else
{
cout<<"Test 1 Failed"<<endl;
}

 return 0;
}

int Start_Parsing(string packet)

{
    // Calculate checksum
    int i=0;
    unsigned int checksum = 0;
    for (i = 1; packet[i] != '*' && packet[i] != '\0'; i++) {
        checksum ^= packet[i];
    }
    
char provided_checksum[2];
      if (packet[i]=='*') //extract provided checksum
        {
          provided_checksum[0]=packet[i+1];  //store first element next to steic in provided_checksum[0]
          provided_checksum[1]=packet[i+2];  //store second ....               ..   provided_checksum[0]
          provided_checksum[2]='\0';         //store last element as null
        }
         // check if steric is present or not if not then no checksum found
    

    if (packet[i] != '*') {
        cout<<"Checksum Not Found! '-': "<<endl;
        return 0;
    }


    // Compute checksum in hexadecimal
    
    snprintf(calculated_checksum_hex, sizeof(calculated_checksum_hex), "%02X", checksum);



    int commas = 0;
    for (int i = 0; i < strlen(packet.c_str()); i++) // extract the number of commas in the packet
    {
        if (packet[i] == ',') {
            commas++;
        }       
    }
    // This section compares the first 7 character of the packet with GPGGA to confirm sentence as GGA and check if threre are 14 commas

    if (strncmp(packet.c_str(), "$GPGGA,", 7) == 0 && commas == 14 && packet[i] == '*')//c_str() function converts the string packet to a const character
     {
        cout<<"The sentence is in GGA format: \n"<<endl;

    } else {
       cout<<"The sentence is not in GGA format: \n"<<endl;
        return 0;
    }

    // Check packet integrity
    
    if (strcmp(provided_checksum, calculated_checksum_hex) == 0) {
        cout<<"Packet integrity valid: chksum \n"<< calculated_checksum_hex<<endl;

        manage_missing(packet);  //if packet integrity valid then manage manage missing function is called
    } else {
        cout<<"Packet integrity not valid: chksum \n"<< calculated_checksum_hex<<endl;   // if packet intefrity is not valid then return 0
        //return 0;
        //manage_missing(packet); 
    }
}

void manage_missing(string packet) {

 // Extract packet data excluding '$' and '*'
    char packet2[strlen(packet.c_str()) - 3];
    int k = 0;
    for (int i = 1; packet[i] != '*'; i++) {
        packet2[k++] = packet[i];
    }
    packet2[k] = ','; // Adding comma at the last point to make last data as token
    packet2[k+1]='\0';

    cout<<packet2<<endl;

//onwards this section the missing packets will be replaced by a '#'

 char MIS = '#';
    
    int size= strlen(packet2);
   
    char final_sentence[size + 1];      //  Increase size by 1 to accommodate the new character

    strcpy(final_sentence, packet2);    //  Copy the packet to final sentence that will contained managed data

   

    int mispos[size];                    //  Creating array to store position of missing character
    int empty_count = 0;                 //  Counter to count the number of missing positions

    for(int pos = 0; pos < size; pos++) {
        if(final_sentence[pos] == ',' && final_sentence[pos + 1] == ',') // checks for the consective commas
        {
            mispos[empty_count] = pos + 1;                               // if found consecutive comma then store next position for missing data
            empty_count++;
        }
    }
    //cout << "Number of empty positions: " << empty_count << endl;

    for(int j = 0; j < empty_count; j++)            //loop to store # on missing positions, run till no of empty elements
    {                                               //As when first element is added the missing positions will be updated so there need to update the position

        int pos=mispos[j] + j;                      // Update the missing position according to missing count
                                                            
        for(int i = size; i >= pos; i--)            // the loop will run in reverse order until position of missing element is found
        {
        final_sentence[i] = final_sentence[i - 1];  // elements are stored in one position next 
    }
    final_sentence[pos] = MIS;                      // replace # on the missing postion
    size++;                                         // Increase size of array

    }
//cout<<"final sentence is : "<<final_sentence<<endl;
   
    Tokenize_data(final_sentence);                  // CALL THE FUNCTION TO PARSE FINAL SENTENCE

                                                            
}

void Tokenize_data(char final_sentence[])
{   

char *token=strtok(final_sentence,",");      // Using strtok function to tokenize sentence with a ',' as delimeter
int param_no=0;
while(token!=NULL)                           // Loop runns until Find Null Value as at the end;
{ 
    //enum Tokens {sentenceID, time, latitude, latDir, logitude, longitudeDir, QualityIndicator, Numofsattelites, horizontaldilution, altitude, altitudeUnits, meansealevelheight, geoidseparationUnits, timesincelastDC, differentialstationID};

    switch(param_no)
    {
        case sentenceID:
        GPSdata.sentenceID=token;
        break;

        case time:
        GPSdata.time=token;
        break;

        case latitude:
        GPSdata.latitude=token;
        break;

        case latDir:
        GPSdata.latDir=token[0];
        break;

        case logitude:
        GPSdata.longitude=token;
        break;

        case longitudeDir:
        GPSdata.longDir=token[0];
        break;

        case QualityIndicator:
        GPSdata.QualityIndicator=stoi(token);
        break;

        case Numofsattelites:
        GPSdata.NumofSat=stoi(token);
        break;

        case horizontaldilution:
        GPSdata.horizontaldilution=token;
        break;

        case altitude:
        GPSdata.altitude=token;
        break;

        case altitudeUnits:
        GPSdata.altitudeUnits=token[0];
        break;

        case meansealevelheight:
        GPSdata.meansealevelheight=token;
        break;

        case geoidseparationUnits:
        GPSdata.geoidseparationUnits=token[0];
        break;

        case timesincelastDC:
        GPSdata.timesincelastDC=token;
        break;

        case differentialstationID:
        GPSdata.differentialstationID=token;
        break;

    }
 
    token=strtok(NULL,",");               // every time token is assigned the next previous delimeter is 
    param_no++;
}
    GPSdata.checksum=calculated_checksum_hex;          
}

bool unit_test1() // This function tests the values of the output by comparing with the input parameter and return true if all values are equal
{
    return (GPSdata.time == "002153.000") &&
           (GPSdata.latitude == "3342.6618") &&
           (GPSdata.latDir == 'N') &&
           (GPSdata.longitude == "11751.3858") &&
           (GPSdata.longDir == 'W') &&
           (GPSdata.QualityIndicator == 1) &&
           (GPSdata.NumofSat == 10) &&
           (GPSdata.horizontaldilution == "1.2") &&
           (GPSdata.altitude == "27.0") &&
           (GPSdata.altitudeUnits == 'M') &&
           (GPSdata.meansealevelheight == "-34.2") &&
           (GPSdata.geoidseparationUnits == 'M') &&
           (GPSdata.timesincelastDC == "#") &&
           (GPSdata.differentialstationID == "0000") &&
           (GPSdata.checksum == "5E");
}

void print()      // Print the parameters of the GGA sentence
{
    
cout<<"sentence ID: "<<                    GPSdata.sentenceID<<endl;
cout<<"time: "<<                                 GPSdata.time<<endl;
cout<<"latitude: "<<                         GPSdata.latitude<<endl;
cout<<"latitude Direction: "<<                 GPSdata.latDir<<endl;
cout<<"longitude: "<<                        GPSdata.longitude<<endl;
cout<<"longitude Direction: "<<                GPSdata.longDir<<endl;
cout<<"Quality Indicator: "<<        GPSdata.QualityIndicator<<endl;
cout<<"No of satellites: "<<                 GPSdata.NumofSat<<endl;
cout<<"horizontal dilution: "<<    GPSdata.horizontaldilution<<endl;
cout<<"altitude: "<<                         GPSdata.altitude<<endl;
cout<<"altitude units: "<<              GPSdata.altitudeUnits<<endl;
cout<<"mean sea level height: "<<  GPSdata.meansealevelheight<<endl;
cout<<"geoideal separation unit: "<<GPSdata.geoidseparationUnits<<endl;
cout<<"time since last DC: "<<        GPSdata.timesincelastDC<<endl;
cout<<"differential sation id: "<<GPSdata.differentialstationID<<endl;
cout<<"checksum: "<<                            GPSdata.checksum<<endl;
}