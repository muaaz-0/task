#include <iostream>
#include <string.h>
#include <stdio.h>
using namespace std;

char calculated_checksum_hex[3];


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
    string Sentence1="$GPGGA,002153.000,3342.6618,N,11751.3858,W,1,10,1.2,27.0,M,-34.2,M,,0000*5E";

    //string Sentence2="$GPGGA,115739.00,4158.8441367,N,09147.4416929,W,4,13,0.9,255.747,M,-32.00,M,01,0000*6E";
    //string Sentence3="$GPGGA,002153.000,3342.6618,N,11751.3858,W,1,10,1.2,27.0,M,-34.2,M,,0000*5E"   //invalid sentence

Start_Parsing(Sentence1);
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
        return 0;
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
   
    char final_sentence[size + 1]; //  Increase size by 1 to accommodate the new character

    strcpy(final_sentence, packet2);   // Copy the packet to final sentence that will contained managed data

   

    int mispos[size];                    
    int empty_count = 0;

    for(int i = 0; i < size; i++) {
        if(final_sentence[i] == ',' && final_sentence[i + 1] == ',') {
            mispos[empty_count] = i + 1;
            empty_count++;
        }
    }
    //cout << "Number of empty positions: " << empty_count << endl;

    for(int j = 0; j < empty_count; j++) {          //loop till run according to empty positions

        int pos=mispos[j] + j;
                                                            
        for(int i = size; i >= pos; i--) {
        final_sentence[i] = final_sentence[i - 1];
    }
    final_sentence[pos] = MIS;                                                //replace # on the missing postion
    size++;                       // Increase size of array

    }
cout<<"final sentence is : "<<final_sentence<<endl;
    Tokenize_data(final_sentence); // CALL THE FUNCTION TO PARSE FINAL SENTENCE

                                                            
}

void Tokenize_data(char final_sentence[])
{   
char NumofPrmtrs[15];
char *token=strtok(final_sentence,",");
int param_no=1;
while(token!=NULL)
{ 
    switch(param_no){
        case 1:
        GPSdata.sentenceID=token;
        break;

        case 2:
        GPSdata.time=token;
        break;

        case 3:
        GPSdata.latitude=token;
        break;

        case 4:
        GPSdata.latDir=token[0];
        break;

        case 5:
        GPSdata.longitude=token;
        break;

        case 6:
        GPSdata.longDir=token[0];
        break;

        case 7:
        GPSdata.QualityIndicator=stoi(token);
        break;

        case 8:
        GPSdata.NumofSat=stoi(token);
        break;

        case 9:
        GPSdata.horizontaldilution=token;
        break;

        case 10:
        GPSdata.altitude=token;
        break;

        case 11:
        GPSdata.altitudeUnits=token[0];
        break;

        case 12:
        GPSdata.meansealevelheight=token;
        break;

        case 13:
        GPSdata.geoidseparationUnits=token[0];
        break;

        case 14:
        GPSdata.timesincelastDC=token;
        break;

        case 15:
        GPSdata.differentialstationID=token;
        break;

    }
   // cout<<"the token "<<param_no<<" :"<<token<<endl;
    token=strtok(NULL,",");
    param_no++;
}
    GPSdata.checksum=calculated_checksum_hex;
}

bool unit_test1() {
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

void print()
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