
/*
    Author Name  : Sourabh Dipak Nalavade
*/

#include "../header/myheader.h"

extern int Empid;

extern char adminDataFile[MAX_NAME_LENGTH];
extern char employeeDataFile[MAX_NAME_LENGTH];
extern char requestDataFile[MAX_NAME_LENGTH];
extern char requestStatusDataFile[MAX_NAME_LENGTH];
extern char AssetFreepoolDataFile[MAX_NAME_LENGTH];
extern char assetDataFile[MAX_NAME_LENGTH];


int employeeId;

/*
    Author Name  : Sourabh Dipak Nalavade
    Description of Function  : 
        The  function is responsible for  an employee login.
        It takes the “USERNAME” and “PASSWORD” parameter as an input.
        This function will compare the provided username and password with the stored employee credentials to determine if they are valid or not.
        If valid the login is considered as successful, else it prompts user to enter valid details.

*/
int employeeLogin(char *username, char *password)
{

    Employee newEmployee;

    FILE *fp = fopen(employeeDataFile, "rb+");

    if (fp == NULL)
    {
        printf("Error: Unable to open users file.\n");
        exit(1);
    }

    while (fread(&newEmployee, sizeof(Employee), 1, fp) == 1)
    {
        if (strcmp(newEmployee.fname, username) == 0 && strcmp(newEmployee.password, password) == 0)
        {
            employeeId = newEmployee.id;
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

/*
    Author Name  : Sourabh Dipak Nalavade
    Description of Function  : 
        The function is called from employee.c. The employee is propmt to enter the asset type which he/she wants to request.
        The request which contains the asset data is stored in file named request.csv using fwrite.
*/
void requestForAsset(int employeeId, int assetchoice)
{
    Request request = {0};
    request.aid = assetchoice;
    request.empid = employeeId;

    if (assetchoice == 1)
    {
        strncpy(request.assetname, "Laptop", MAX_NAME_LENGTH - 1);
    }
    if (assetchoice == 2)
    {
        strncpy(request.assetname, "Desktop", MAX_NAME_LENGTH - 1);
    }

    strncpy(request.status, "Pending", MAX_NAME_LENGTH - 1);

    FILE *rp = fopen(requestDataFile, "a");

    FILE *requeststatus = fopen(requestStatusDataFile, "a");

    if (rp == NULL)
    {
        printf("Error: Unable to open users file.\n");
        exit(1);
    }
    else
    {
        fwrite(&request, sizeof(Request), 1, rp);
        fclose(rp);
        printf("\t\t\033[0;33m Your request for %s is pending !!! Wait till administrator approve the request !!!\033[0m", request.assetname);
        printf("\n");
        fwrite(&request, sizeof(Request), 1, requeststatus);
        fclose(requeststatus);
    }
    fclose(rp);
    fclose(requeststatus);
}

/*
    Author Name  : Sourabh Dipak Nalavade
    Description of Function  : 
        The function reads the data from file requeststatus.csv using fread and prints the data on console.
*/
void viewRequestStatus()
{
    Request myrequestStatus;
    FILE *requeststatus = fopen(requestStatusDataFile, "rb");
    while (fread(&myrequestStatus, sizeof(Request), 1, requeststatus) == 1)
    {
        if (myrequestStatus.empid == employeeId)
            printf("\n");
            printf("\t\t\t\033[0;32m Employee Id = %d \tRequested Asset : %s  \tStatus : %s\n\033[0m", myrequestStatus.empid, myrequestStatus.assetname, myrequestStatus.status);
            printf("\n");
    }
    fclose(requeststatus);
}

/*
    Author Name  : Sourabh Dipak Nalavade
    Description of Function  : 
            The function is responsible for handling the logout process for an employee.

*/
void employeeLogout()
{
    printf("***************************************************************************************************\n");
    printf("\t\t\t\033[0;32m Logged out successfully!!!\n\033[0m");
    printf("***************************************************************************************************\n");

}

/*
    Author Name  : Sourabh Dipak Nalavade
    Description of Function  : 
         The function  displays an interactive menu for employee.
*/
void EmployeeMenu(int employeeId)
{

    int choice;
    int assetchoice;
    do
    {
        
        printf("\n\033[0;33m                                    Employee Menu \n\033[0m");
        printf("........................................................................................................................\n");
        printf("\n");
        printf("\033[0;32m 1. View My Assets\t");
        printf("\t2. Request Asset\t");
        printf("\t3. View Asset Request Status\t");
        printf("\t4.\033[0m \033[0;31m Logout\n\033[0m");
        printf("........................................................................................................................\n");
        printf("\n");       
        printf("\t\033[0;32m Enter your choice: \033[0m");
        scanf("%d", &choice);
        printf("\n");
        switch (choice)
        {
        case 1:
            viewMyAssets(employeeId);
            break;
        case 2:
            printf("................................................................\n");
            printf("\n\033[0;33m--- .....   Select the Asset from Below Menu   ..... ---\033[0m\n");
            printf("\n");
            printf("................................................................\n");
            printf("\n");
            printf("\t\t\033[0;32m 1. Laptop\n");
            printf("\n");
            printf("\t\t2. Desktop\n\033[0m");
             printf("\n");
            printf("\t\tEnter your choice: ");
            scanf("%d", &assetchoice);
            printf("\n");
            requestForAsset(employeeId, assetchoice);
            break;
        case 3:
            viewRequestStatus();
            break;
        case 4:
            employeeLogout();
            return;

        default:
            printf("\n\033[0;31m<--*****************  Invalid choice. Please try again  *****************-->\n\033[0m");
            break;
        }
    } while (1);
}

/*
    Author Name  : Sourabh Dipak Nalavade
    Description of Function  : 
        The function prompts employee to enter their UserName and Password for successful login in Asset Management System.
        The function retreives the login details of an employee from the employee.csv file using fread.
        Then compares it with the entered login details. If data matches, employee is logged into the system,
        else it prompts user to enter valid details.

*/
void getEmployeeLoginDetails(void)
{
    char username[MAX_NAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    printf("\033[0;33m--------------------------Enter Your Login Details------------------------- \n\033[0m");

    printf("\n\t\t\033[0;32m Enter your Username: ");
    scanf(" %s", username);

    printf("\n\t\tEnter your Password: ");
    printf("\033[8m");
    scanf(" %s", password);
    printf("\033[0m");

    if (employeeLogin(username, password))
    {
        printf("\n\033[0;32m              <--***************** Login successfull  *****************-->\033[0m\n");
        printf("\n");
        printf("***************************************************************************************************\n");
        printf("\n");       
        printf("\033[0;32m                  Hello  %s , Welcome To Asset Management System\n\033[0m",username);
        printf("\n");
        printf("***************************************************************************************************\n");
        
        EmployeeMenu(employeeId);
    }
    else
    {
        printf("\n");
        printf("\n\033[0;31m <--***************** Invalid Credentials!!! please enter valid details  *****************-->\033[0m\n");
        printf("\n");
        getEmployeeLoginDetails();
    }
}