#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

//Marina Boat Management, ASSIGNMENT ONE


//Define Function Prototypes
void parseFromCSV(const char* line);
void printInventory();
void addBoat(const char* filename);
void removeBoat(const char* filename);
void boatPayment();
void newMonthPayment();
int compareBoatNames(const void* a, const void* b);


//Step One, Define Constants
#define MAX_BOATS 120 //boats allowed in array
#define MAX_BOAT_LENGTH 100 
#define MAX_BOAT_NAME 127 //boat name length
# define FILENAME "BoatData.csv"

//Step two, Enum, Struct, Union
   //type of boat, enum
   typedef enum {
      slip,
      land,
      trailor,
      storage,
      noPlace
    } TypeOfBoat;

   //details of boat type, union
   typedef union {
      int slipNumber; //Slip, 1-85
      char bayLetter; //Land, A-Z
      char licenseTag[16]; //Trailor
      int storageNumber; //Storage, 1-50
    } BoatTypeDetails; 

   //boat information, struct
   typedef struct {
      char boatName[MAX_BOAT_NAME];
      float boatLength;
      TypeOfBoat typeOfBoat;
      BoatTypeDetails boatTypeDetails;
      float amountOwed;
    } BoatInfo;

//Step three, Gloabal Variables
   BoatInfo* boats[MAX_BOATS] = {NULL};
   int boatCount = 0;
   char nameInput[128];


//Step four, Helper Functions

   //load boat data when program starts
   void loadData(const char* filename) {
      FILE* file = fopen(filename, "r"); //read file only

      if (!file) {
         printf("No File Available");
         exit(1);
       } //end of if statement, no file loaded
      
      char line[256];
      while (fgets(line, sizeof(line), file)) {
         if (boatCount >= MAX_BOATS) {
            printf("Maximum Boat Capacity Reached\n");
            break;
          } //end of if, maximum boats 
     
         line[strcspn(line, "\n")] = 0; //removes blank line, if exists
         parseFromCSV(line);
       } //end of while statement
       
      fclose(file); //close file when data is done 
   } //end of loadData Method


   //Parse the CSV file Method
   void parseFromCSV(const char* line) {
      BoatInfo* b = malloc(sizeof(BoatInfo));
      char name[MAX_BOAT_NAME];
      char type[16];
      char detail[16];
      float length;
      float amount;

      if (sscanf(line, " %[^,],%f,%[^,],%[^,],%f", name, &length, type, detail, &amount) != 5) {
         free(b);
         return;
      } //end of if

      strncpy(b->boatName, name, MAX_BOAT_NAME);
      b->boatLength = length;
      b->amountOwed = amount;
      
      if (strcmp(type, "slip") == 0) {
           b->typeOfBoat = slip;
           b->boatTypeDetails.slipNumber = atoi(detail);
      } else if (strcmp(type, "land") == 0) {
           b->typeOfBoat = land;
           b->boatTypeDetails.bayLetter = detail[0];
      } else if (strcmp(type, "trailor") == 0) {
           b->typeOfBoat = trailor;
           strncpy(b->boatTypeDetails.licenseTag, detail, sizeof(b->boatTypeDetails.licenseTag) - 1);
      } else if (strcmp(type, "storage") == 0) {
           b->typeOfBoat = storage;
           b->boatTypeDetails.storageNumber = atoi(detail);
      } else {
           b->typeOfBoat = noPlace;
      } 

      boats[boatCount++] = b;
   } //End of Parse Data Method



   //save Boat Data when Program exits
   void saveData(const char* filename) {
      FILE* file = fopen(filename, "w"); //write new data in file
      
      if (!file) {
         printf("No File Avialable\n");
         exit (1);
       } //end of if, no file loaded

      for (int i = 0; i < boatCount; i++) {
         BoatInfo* b = boats[i];

         fprintf(file, "%s,%.1f,", b->boatName, b->boatLength); //boat info in CSV format

         if (b->typeOfBoat == slip) {
            fprintf(file, "slip,%d,", b->boatTypeDetails.slipNumber);
          } else if (b->typeOfBoat == land) {
            fprintf(file, "land,%c,", b->boatTypeDetails.bayLetter);
          } else if (b->typeOfBoat == trailor) {
            fprintf(file, "trailor,%s,", b->boatTypeDetails.licenseTag);
          } else if (b->typeOfBoat == storage) {
            fprintf(file, "storage,%d,", b->boatTypeDetails.storageNumber);
          } //end of if statements for boat type details

         fprintf(file, "%.2f\n", b->amountOwed); //amount owed
       } //end of for statement
    
     fclose(file); //close file after rewriting boat data
  } //end of saveData Method




//Step five, Menu Options Display

   //get the users letter menu option
   char getMenuChoice() {
      char choice;
      printf("(I)nventory, (A)dd, (R)emove, (P)ayment, (M)onth, e(X)it : ");
      scanf(" %c", &choice);
      return choice;
    } //end of users letter choice method

   //Print Menu Options
   bool printMenuOptions() {
      static bool firstRun = true; //start with first run

      if (firstRun) {
         printf("aml1712@miami.edu: BoatManagement BoatData.csv\n");
         printf("Welcome to the Boat Management System\n");
         printf("-------------------------------------\n");
         firstRun = false; //don't print welcome anymore
       } //end of if, first run

      //user Inputs Choice Letter
      while (1) {
         char choice = getMenuChoice();
       
         if (choice == 'I' || choice == 'i') {
            printInventory();
            return true;
          } else if (choice == 'A' || choice == 'a') {
            addBoat("BoatData.csv");
            return true;
          } else if (choice == 'R' || choice == 'r') {
            removeBoat("BoatData.csv");
            return true;
          } else if (choice == 'P' || choice == 'p') {
            boatPayment();
            return true;
          } else if (choice == 'M' || choice == 'm') {
            newMonthPayment();
            return true;
          } else if (choice == 'X' || choice == 'x') {
            printf("Exiting the Boat Management System\n");
            return false; //EXIT
          } else {
            printf("Invalid option\n");
          }
      } //end of WHILE statment
  } //End of print Menu Options Method




//Step six, Menu Option Choice Functions


   //Comapre Boat Names for A-Z
   int compareBoatNames(const void* a, const void* b) {
         BoatInfo* boatA = *(BoatInfo**)a;
         BoatInfo* boatB = *(BoatInfo**)b;
         return strcmp(boatA->boatName, boatB->boatName);
     } //end of int compare for printing alphabetical


   //Print Inventory Method
   void printInventory() {
   
   //sort the boats by letters
   qsort(boats, boatCount, sizeof(BoatInfo*), compareBoatNames);

   //Loop & Print boats in inventory
   for (int i = 0; i < boatCount; i++) {
      BoatInfo* b = boats[i];
      char typeStr[10] = "";
      char typeInfo[16] = "";

      if (b->typeOfBoat == slip) {
         strcpy(typeStr, "slip");
         sprintf(typeInfo, "# %d", b->boatTypeDetails.slipNumber);
       } else if (b->typeOfBoat == land) {
            strcpy(typeStr, "land");
            sprintf(typeInfo, "%c", b->boatTypeDetails.bayLetter);
       } else if (b->typeOfBoat == trailor) {
            strcpy(typeStr, "trailor");
            sprintf(typeInfo, "%s", b->boatTypeDetails.licenseTag);
       } else if (b->typeOfBoat == storage) {
            strcpy(typeStr, "storage");
            sprintf(typeInfo, "# %d", b->boatTypeDetails.storageNumber);
      } //end of if statement

   //print the boat info in format
   printf("%-20s %3.0f' %-8s %6s Owes $%7.2f\n",
      b->boatName,
      b->boatLength,
      typeStr,
      typeInfo,
      b->amountOwed);
   } //end of for
} //end of PRINT inventory Method



   //Add Boat Method
   void addBoat(const char* filename) {
      if (boatCount >= MAX_BOATS) {
         printf("Maximum Amount of Boats Allotted, Cannot Add More");
         return;
       } //end of if statement

      char input[256];

      printf("Please enter the boat data in CSV format: ");
      
      while (getchar() != '\n');
      
      if (fgets(input, sizeof(input), stdin) != NULL) {
         size_t len = strlen(input);
      
      if (len > 0 && input[len - 1] == '\n') {
         input[len - 1] = '\0';
       }
     }

     BoatInfo* newBoat = (BoatInfo*)malloc(sizeof(BoatInfo));
     int typeOfBoat;
     sscanf(input, "%127[^,],%f,%d,%f", newBoat->boatName, &newBoat->boatLength, &typeOfBoat, &newBoat->amountOwed);
     newBoat->typeOfBoat = (TypeOfBoat)typeOfBoat;
     boats[boatCount] = newBoat;
     boatCount++;



      FILE* file = fopen(filename, "a"); //open file
      fprintf(file, "%s\n", input); //add new input to file
      fclose(file); //close file
   } //End of Add Boat Method




   //Remove Boat Method
   void removeBoat(const char* filename) {
      char removalName[128];
      printf("Please enter the boat name: ");

      while (getchar() != '\n');
      fgets(removalName, sizeof(removalName), stdin);

      size_t len = strlen(removalName);
      if (len > 0 && removalName[len - 1] == '\n') {
         removalName[len - 1] = '\0';
       } 

      for (int i = 0; removalName[i]; i++) {
         removalName[i] = tolower(removalName[i]);
       } //end of for

      int found = 0;

     //look for boat
     for (int i = 0; i < boatCount; i++) {
        char tempName[128];
        strcpy(tempName, boats[i]->boatName);
      
     for (int j = 0; tempName[j]; j++) {
        tempName[j] = tolower(tempName[j]);
      } //end of for J

     if (strcmp(removalName, tempName) == 0) {
        free(boats[i]);

     for (int j = i; j < boatCount - 1; j++) {
                boats[j] = boats[j + 1];
            } //end of for

            boats[boatCount - 1] = NULL; 
            boatCount--;
            found = 1;
            break;
     } //end of if
   } //end of for
 
   if (!found) {
      printf("No boat with that name\n");
   } else {
      saveData(filename);
    }
} //End of Remove Boat Method




   //Calculate Boat Payment
   void boatPayment() {
      char nameInput[128];
      printf("Please enter The Boat Name: ");

      while (getchar() != '\n');
      fgets(nameInput, sizeof(nameInput), stdin);

      size_t len = strlen(nameInput);
      if (len > 0 && nameInput[len - 1] == '\n') {
          nameInput[len - 1] = '\0';
       }

      for (int i = 0; nameInput[i]; i++) {
        nameInput[i] = tolower(nameInput[i]);
       }

       int found = 0;
       for (int i = 0; i < boatCount; i++) {
          char boatNameLower[128];
          strcpy(boatNameLower, boats[i]->boatName);
        for (int j = 0; boatNameLower[j]; j++) {
            boatNameLower[j] = tolower(boatNameLower[j]);
        }

        if (strcmp(nameInput, boatNameLower) == 0) {
            found = 1;
            printf("Please enter the amount to be paid: ");
           
            float payment;
            scanf("%f", &payment);

            if (payment > boats[i]->amountOwed) {
               printf("That is more than the amount owed, $%.2f\n", boats[i]->amountOwed);
            } else {
                  boats[i]->amountOwed -= payment;
              }
              break;
          }
        }
        if (!found) {
           printf("No boat with that name\n");
         }
   } //End of boatPayment Method





   //Update Month Method
   void newMonthPayment() {
      for (int i =0; i < boatCount; i++) {
         BoatInfo* b = boats[i];
         float monthlyRate = 0.0;

         if (b->typeOfBoat == slip) {
            monthlyRate = 12.50;
         } else if (b->typeOfBoat == land) {
            monthlyRate = 14.00;
         } else if (b->typeOfBoat == trailor) {
            monthlyRate = 25.00;
         } else if (b->typeOfBoat == storage) {
            monthlyRate = 11.20;
         } //end of if 

         float monthlyDue = b->boatLength * monthlyRate;
         b->amountOwed += monthlyDue;
      } //end of for statement
   } //end of NewMonthly Payment Method




   //Step seven, Main Method
   int main() {
      loadData(FILENAME); //load data when program starts
      
      while (1) {
         if (!printMenuOptions()) {
               break; //exit if user types x
            }
          } //end of while

      saveData(FILENAME);
      return 0;
   } //end of MAIN method





















































   


















