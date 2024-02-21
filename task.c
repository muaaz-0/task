#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
//#include "GPS_data_parser_header.h"
    
#define max_length 128
const char* const empty="0";
typedef struct {
	
    char sentenceID[10];//GPSData->sentenceID

	char  UTCTime[10];

	char  latitude[10];

	char  latitudeDirection;

	char  longitude[11];

	char  longitudeDirection;

	int   QualityIndicator;

	int   Numofsatellites;

	float horizontalDilution;

	float altitude;

	char  altitudeUnits;

	float meanSeaLevelHeight;

	char  geoidSeparationUnits;

	float timeSinceLastDifferentialCorrection;

	char  differentialStationID[4];
	
	char  checksum[2];
} GPSData;

int parse_gps_data(const char *packet, GPSData *gpsData);


int main() 
{
    const char *packet="$GPGGA,002153.000,3342.6618,,11751.3858,W,1,10,1.2,27.0,M,-34.2,M,,0000*100";

    printf("The Sentence is: %s\n",packet);
    
  
    GPSData gpsData;

    parse_gps_data(packet,&gpsData);

    printf("SentenceID is: %s\n",                                                   gpsData.sentenceID);

    printf("UTC time is: %s\n",                                                       gpsData.UTCTime);

    printf("latitude is: %s\n",                                                       gpsData.latitude);

    printf("Latitude Direction is: %c\n",                                    gpsData.latitudeDirection);

    printf("Longitude is: %s\n",                                                     gpsData.longitude);

    printf("LongitudeDirection is: %c\n",                                   gpsData.longitudeDirection);

    printf("QualityIndicator is: %d\n",                                       gpsData.QualityIndicator);

    printf("NumBer of Satellites is: %d\n",                                    gpsData.Numofsatellites);

    printf("HorizontalDilution is: %lf\n",                                  gpsData.horizontalDilution);

    printf("Altitude is: %lf\n",                                                      gpsData.altitude);

    printf("AltitudeUnits is: %c\n",                                             gpsData.altitudeUnits);

    printf("Mean SeaLevel Height is: %f\n",                                gpsData.meanSeaLevelHeight);

    printf("Geoid Separation Units is: %c\n",                             gpsData.geoidSeparationUnits);

    printf("Time Since Last Differential Correction is: %lf\n",gpsData.timeSinceLastDifferentialCorrection);

    printf("Differential Station ID is: %s\n",                           gpsData.differentialStationID);

    printf("Checksum is: %s\n",                                                       gpsData.checksum);
        return 0;
}
//------------------------------------------------ g++ main.c GPS_data_parser_header.c   ./a.exe
  

int parse_gps_data(const char *packet, GPSData *gpsData) {
    // Calculate checksum
    unsigned int checksum = 0;
    for (int i = 1; packet[i] != '*' && packet[i] != '\0'; i++) {
        checksum ^= packet[i];
    }

    // Verify checksum presence
    int i = 0;
    while (packet[i] != '*' && packet[i] != '\0') {
        i++;
    }
    if (packet[i] != '*') {
        printf("No checksum found! '-' :");
        return -1;
    }

    // Extract provided checksum
    char provided_checksum[3];
    strncpy(provided_checksum, packet + i + 1, 2);
    provided_checksum[2] = '\0';

    // Compute checksum in hexadecimal
    char calculated_checksum_hex[3];
    snprintf(calculated_checksum_hex, sizeof(calculated_checksum_hex), "%02X", checksum);

/* int Noof_empty_data=0;
int empty_count=0;
int empty_data_pos[15];
*/

    // Check sentence format
    int commas = 0;
    for (int i = 0; i < strlen(packet); i++) {
        if (packet[i] == ',') {
            commas++;
        }
       /* //checking for empty data positions
            if (packet[i] == ',' && packet[i+1] == ',') {
                empty_data_pos[commas]=commas;
                
                printf("empty data position is%d\n", empty_data_pos[commas]);
                empty_count++;
                printf("no of empty parameter is: %d\n", empty_count);

            }*/
        }
    
    if (strncmp(packet, "$GPGGA,", 7) == 0 && commas == 14 && packet[i] == '*') {
        printf("The sentence is in GGA format: "); // GGA sentence
    } else {
        printf("The sentence is not in GGA format: ");
    }

    // Check packet integrity
    if (strcmp(provided_checksum, calculated_checksum_hex) == 0) {

        printf("Packet integrity valid chksum \n", calculated_checksum_hex);
    } else {
        printf("Packet integrity not valid \n", calculated_checksum_hex );
    }

   // Extract packet data excluding '$' and '*'
    char packet2[strlen(packet) - 3];
    int k = 0;
    for (int i = 1; packet[i] != '*'; i++) {
        packet2[k++] = packet[i];
    }
    packet2[k] = '\0';

   

    // Tokenize packet data
    char packet_copy[max_length];
    strncpy(packet_copy, packet, max_length);
    char *token;
    token = strtok(packet_copy, ",");
    int j = 0;

    // Parse tokenized data
    while (token != NULL && j < 15) {
        switch (j) {
            case 0:

                strncpy(gpsData->sentenceID, token, sizeof(gpsData->sentenceID) - 1);
                gpsData->sentenceID[sizeof(gpsData->sentenceID) - 1] = '\0';
                break;

            case 1:
           // if (j!=empty_data_pos[j])
               { strncpy(gpsData->UTCTime, token, sizeof(gpsData->UTCTime) - 1);
                gpsData->UTCTime[sizeof(gpsData->UTCTime) - 1] = '\0';}
           // else
           // {strcpy(gpsData->UTCTime, empty );
           //printf("th time is: %s\n", gpsData->UTCTime);}
                break;
            case 2:
         //   if (j!=empty_data_pos[j])
                {strncpy(gpsData->latitude, token, sizeof(gpsData->latitude));
                gpsData->latitude[sizeof(gpsData->latitude) - 1] = '\0';}
         /* else
            {
                strcpy(gpsData->latitude,empty);
            }*/
                break;
            case 3:
                gpsData->latitudeDirection = token[0];
                break;
            case 4:
                strncpy(gpsData->longitude, token, sizeof(gpsData->longitude) - 1);
                gpsData->longitude[sizeof(gpsData->longitude) - 1] = '\0';
                break;
            case 5:
                gpsData->longitudeDirection = token[0];
                break;
            case 6:
                gpsData->QualityIndicator = atoi(token);
                break;
            case 7:
                gpsData->Numofsatellites = atoi(token);
                break;
            case 8:
                gpsData->horizontalDilution = atof(token);
                break;
            case 9:
                gpsData->altitude = atof(token);
                break;
            case 10:
                gpsData->altitudeUnits = token[0];
                break;
            case 11:
                gpsData->meanSeaLevelHeight = atof(token);
                break;
            case 12:
                gpsData->geoidSeparationUnits = token[0];
                break;
            case 13:
           // if (j!=empty_data_pos[j])
                {gpsData->timeSinceLastDifferentialCorrection = atof(token);
               // printf("I am empty data position: %lf\n",empty_data_pos[j]);
                }
           /*     else
                {gpsData->timeSinceLastDifferentialCorrection = atof(empty);
                printf("the value in empty is: %s", empty );
                }*/
                break;
            case 14:
            
                strncpy(gpsData->differentialStationID, token, sizeof(gpsData->differentialStationID));
                break;
        }
        token = strtok(NULL, ",");
        j++;
    }

    // Copy calculated checksum
    strncpy(gpsData->checksum, calculated_checksum_hex, sizeof(gpsData->checksum));
    return 0;
        }