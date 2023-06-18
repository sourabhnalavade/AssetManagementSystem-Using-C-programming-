
/*
    Author Name  : Sourabh Dipak Nalavade
*/

#pragma once
#include <stdio.h>
#include<stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define MAX_NAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 20



// File variable names
extern char adminDataFile[MAX_NAME_LENGTH];
extern char employeeDataFile[MAX_NAME_LENGTH];
extern char requestDataFile[MAX_NAME_LENGTH];
extern char requestStatusDataFile[MAX_NAME_LENGTH];
extern char AssetFreepoolDataFile[MAX_NAME_LENGTH];
extern char assetDataFile[MAX_NAME_LENGTH];
/*
    Author Name  : Sourabh Dipak Nalavade
    Description of Stucture  : 
*/

typedef struct
{
    int id;
    char type[MAX_NAME_LENGTH];
    char status[MAX_NAME_LENGTH];
    char purchasedate[MAX_NAME_LENGTH];
} Asset;


/*
    Author Name  : Sourabh Dipak Nalavade
    Description of Stucture  : 
*/
typedef struct
{
    int id;
    char fname[MAX_NAME_LENGTH];
    char lname[MAX_NAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    char designation[MAX_NAME_LENGTH];
    char joiningDate[MAX_NAME_LENGTH];
    char location[MAX_NAME_LENGTH];

} Employee;

/*
    Author Name  : Sourabh Dipak Nalavade
    Description of Stucture  : 
*/

typedef struct
{
    char assetname[MAX_NAME_LENGTH];
    int aid;
    int empid;
    char status[MAX_NAME_LENGTH];
} Request;


extern int Empid;
extern int employeeCount;
extern int LaptopID;
extern int DesktopID;

// Admin Functionalities
int adminLogin(char *username, char *password);
void adminlogout(void);
void checkPendingRequests(void);
void adminMenu(void);
void getAdminLoginDetails(char *username, char *password);
void addEmployee(void);
void searchEmployee();
void removeEmployee();
void updateEmployee();
void viewAllEmployees();

// Employee Functionalities
void getEmployeeLoginDetails(void);
void EmployeeMenu(int employeeId);
void employeeLogout(void);
void viewRequestStatus(void);
void requestForAsset(int employeeId, int assetchoice);
int employeeLogin(char *username, char *password);
void viewMyAssets();

// Asset Functionalities
void addAsset(void);
void updateAsset(void);
void searchAsset(void);
void deleteAsset(void);
void FreePoolAsset(void);