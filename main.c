
/*
    Author Name  = Sourabh Dipak Nalavade
*/

#include <stdio.h>
#include "header/myheader.h"



int Empid;

char adminDataFile[MAX_NAME_LENGTH]="admindata.csv";
char employeeDataFile[MAX_NAME_LENGTH]="employeedata.csv";
char requestDataFile[MAX_NAME_LENGTH]="requestdata.csv";
char requestStatusDataFile[MAX_NAME_LENGTH]="requeststatusdata.csv";
char AssetFreepoolDataFile[MAX_NAME_LENGTH]="freepooldata.csv";
char assetDataFile[MAX_NAME_LENGTH]="assetdata.csv";


int assetCount = 0;



void getId()
{
    int LaptopID=0;
    int DesktopID=0;
    int empid = 0;
    //set asset id's
    FILE *aidfile = fopen("assetIdFile.txt","r+");
    if(fscanf(aidfile,"%d %d",&LaptopID,&DesktopID)==EOF)
    {
        LaptopID = 0;
        DesktopID = 500;
        fprintf(aidfile,"%d %d",LaptopID,DesktopID);
    }
    fclose(aidfile);

    //set empid
    FILE *empidfile = fopen("employeeid.txt","r+");
    if(fscanf(empidfile,"%d",&empid)==EOF)
    {
        empid = 0;
        fprintf(empidfile,"%d",empid);
    }
    fclose(empidfile);
    
}



void getAdminLoginDetails(char *username, char *password);
void getEmployeeLoginDetails(void);

/*
    Author Name  : Sourabh Dipak Nalavade
    Description of  main function : 
            Its the entry point of program. The function prompts the user to choose the option for login 
            so as to login as Administrator or Employee.
            If 'A is chosen the Admin Module is called whereas if 'E' is chosen Employee Module is called.
*/
int main()
{
    getId();
    
    printf("***************************************************************************************************\n");
    printf("\n");
    printf("\033[0;33m\t\t\t\tWelcome To Asset Management System\n \033[0m ");
    printf("\n");
    printf("***************************************************************************************************\n");
    
    FILE *empid = fopen("employeeid.txt","r");
    fscanf(empid,"%d",&Empid);
    fclose(empid);

    char username[MAX_NAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    char c;
    while (1)
    {
       
        printf("\n\033[0;32m Choose From below options\033[0m \n");
        printf("\033[0;32m---------------------------------------------------------------------------------------------------\n");
        printf("\n\033[0;33m Enter A for Adminstration Login\033[0m \n");
        printf("\n");
        printf("\033[0;33m Enter E for Employee Login\033[0m \n");
        printf("\n");
        printf("\033[0;33m Enter Q to quit the program\033[0m\n");
        printf("\n");
        printf(" Enter the Option wnat to choose : ");
        scanf(" %c", &c);
        if (c == 'A' || c=='a')
        {
            getAdminLoginDetails(username, password);
        }
        else if (c == 'E' || c=='e')
        {
            getEmployeeLoginDetails();
        }
        else if (c == 'Q'|| c=='q')
        {
            exit(0);
        }
        else
        {
            printf("\t\033[0;31m Please choose the valid option from the menu given !!!!\n\033[0m");
        }
    }
    return 0;
}
