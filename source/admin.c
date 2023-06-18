#include "../header/myheader.h"

extern char adminDataFile[MAX_NAME_LENGTH];
extern char employeeDataFile[MAX_NAME_LENGTH];
extern char requestDataFile[MAX_NAME_LENGTH];
extern char requestStatusDataFile[MAX_NAME_LENGTH];
extern char AssetFreepoolDataFile[MAX_NAME_LENGTH];
extern char assetDataFile[MAX_NAME_LENGTH];

extern int Empid;

/*
    Author Name  : Sourabh Dipak Nalavade
    Description of Function  :
        The function adds a new Employee to system with the employee details such as 
        Employee firstname, lastname, designation, dtae of joining,etc.
        It also generates unique password and employee id for each employee.

*/
void viewAllEmployees()
{
    Employee newEmployee;
    FILE *fp = fopen(employeeDataFile, "rb+");

    if (fp == NULL)
    {
        printf("Error: Unable to open users file.\n");
        exit(1);
    }
    printf("\t\tEmployeeId\t\tName\t\t\tDesignation\t\tLocation\t\tJoining Date\n");
    printf("----------------------------------------------------------------------------------------------------------------------------------------------------");
    while (fread(&newEmployee, sizeof(Employee), 1, fp) == 1)
    {
        if(newEmployee.id!=0)
            printf("\n\t\t   %d                  %s %s             %s                  %s               %s\n",newEmployee.id,newEmployee.fname,newEmployee.lname,newEmployee.designation,newEmployee.location,newEmployee.joiningDate);
  
    }
    fclose(fp);
}
   
void addEmployee()
{
    
        
    Employee newEmployee;

    int Empid;
    FILE *empidfile = fopen("employeeid.txt","r+");
    
    fscanf(empidfile,"%d",&Empid);
    newEmployee.id = ++Empid;
    fclose(empidfile);

    FILE *empid = fopen("employeeid.txt","w");
    fprintf(empid,"%d",Empid);
    fclose(empid);


    FILE *nfp = fopen(employeeDataFile, "a");
    if (nfp == NULL)
    {
        printf("Error: Unable to open users file.\n");
        exit(1);
    }
    else
    {
        char date[] = __DATE__;
        
        strcpy(newEmployee.joiningDate, date);
        checkfirstname:
        printf("\t\t\t\033[0;33m Enter Employee FirstName   : \033[0m");
        scanf(" %[^\n]s", newEmployee.fname);
        for(int i=0; i<strlen(newEmployee.fname); i++)
        {
            if(!(isalpha(newEmployee.fname[i]) || newEmployee.fname[i]==' '))
            {
                printf("\n\t\t\t\033[0;31m Please enter valid FirstName \033[0m\n ");
                goto checkfirstname;
            }
        }
        //scanf("%s", newEmployee.fname);
        checklastname:
        printf("\t\t\t\033[0;33m Enter Employee LastName    : \033[0m");
        scanf(" %[^\n]s", newEmployee.lname);
        for(int i=0; i<strlen(newEmployee.lname); i++)
        {
            if(!(isalpha(newEmployee.lname[i]) || newEmployee.lname[i]==' '))
            {
                printf("\n\t\t\t\033[0;31m Please enter valid LastName \033[0m\n");
                goto checklastname;
            }
        }

        printf("\n\t\t\t\033[0;33m Enter Employee Designation : \033[0m");
        scanf(" %[^\n]s", newEmployee.designation);
        printf("\n\t\t\t\033[0;33m Enter Employee Location    : \033[0m");
        scanf(" %[^\n]s", newEmployee.location);

        int lnamelen = strlen(newEmployee.lname);
        char pass[MAX_PASSWORD_LENGTH];
        pass[0] = date[0];
        pass[1] = date[1];
        pass[2] = date[2];
        pass[3] = newEmployee.fname[0];
        pass[4] = newEmployee.fname[1];
        pass[5] = newEmployee.lname[lnamelen - 2];
        pass[6] = newEmployee.lname[lnamelen - 1];
        pass[7] = '\0';

        strcpy(newEmployee.password, pass);

        
        fwrite(&newEmployee, sizeof(Employee), 1, nfp);
        printf("\n");
        printf("\n\t\t\033[0;32m Employee \033[0;33m %s \033[0;32mwith Employee Id:\033[0;33m %d added successfully.\n\033[0m", newEmployee.fname, newEmployee.id);
        fclose(nfp);
        return;
    }
    fclose(nfp);
}

/*
    Author Name  : Sourabh Dipak Nalavade
    Description of Function  :
        The function is responsible for handling the logout process for an administrator.
*/
void adminlogout()
{
    printf("*****************************************************************\n");
    printf("\033[0;31m                Logged out Successfully !!!\n\033[0m");
     printf("*****************************************************************\n");
}

/*
    Author Name  : Sourabh Dipak Nalavade
    Description of Function  :
        The function is responsible for checking and handling any pending requests from employee.
        where administrator can check the pending request to approve or to decline requests for an assets.

*/
void checkPendingRequests()
{
    Request checkrequest;
    Request modifyrequest;
    Asset getasset;

    FILE *status = fopen(requestStatusDataFile, "r+");
    FILE *fp = fopen(requestDataFile, "r+");
    FILE *fpool = fopen(AssetFreepoolDataFile, "r+");

    if (status == NULL)
    {
        printf("Error: Unable to open users file.\n");
        exit(1);
    }

    if (fp == NULL)
    {
        printf("Error: Unable to open users file.\n");
        exit(1);
    }

    char choice = 'y';

    while (fread(&checkrequest, sizeof(Request), 1, fp) == 1)
    {

        printf("\t\t\033[0;33m Employee with empId %d has requested for asset %s . \n", checkrequest.empid, checkrequest.assetname);
        printf("\t\tDo you want to approve the request y/n ?  \033[0m");

        scanf(" %c", &choice);

        if (choice == 'y' || choice == 'Y')
        {
            while (fread(&modifyrequest, sizeof(modifyrequest), 1, status) == 1)
            {
                if (checkrequest.empid == modifyrequest.empid)
                {
                    strcpy(modifyrequest.status, "Approved");
                    while (fread(&getasset, sizeof(Asset), 1, fpool))
                    {
                        if (strcmp(checkrequest.assetname, getasset.type) == 0 && strcmp(getasset.status, "free") == 0)
                        {
                            strcpy(getasset.status, "Approved");
                            fseek(fpool, -sizeof(getasset), SEEK_CUR);
                            fwrite(&getasset, sizeof(getasset), 1, fpool);
                            fclose(fpool);
                            break;
                        }
                    }

                    fseek(status, -sizeof(modifyrequest), SEEK_CUR);

                    fwrite(&modifyrequest, sizeof(Request), 1, status);

                    break;
                }
            }

            printf("\n\t\t\033[0;32m Request approved successfully\n");
            printf("\n");
        }
        else
        {
            while (fread(&modifyrequest, sizeof(modifyrequest), 1, status) == 1)
            {
                if (checkrequest.empid == modifyrequest.empid)
                {
                    strcpy(modifyrequest.status, "Denied");

                    fseek(status, -sizeof(modifyrequest), SEEK_CUR);

                    fwrite(&modifyrequest, sizeof(Request), 1, status);
                    break;
                }
            }

            printf("\n\t\t\033[0;31m Request Denied \n");
            printf("\n");
        }
    }
    fclose(status);

    fclose(fp);
    
    fp = fopen(requestDataFile, "w");
    fclose(fp);
}

/*
    Author Name  : Sourabh Dipak Nalavade
    Description of Function  :
        The function updates data of a particular Employee with help of Employee id.   
*/

void updateEmployee()
{
    Employee newEmployee;
    int employeeId;
    char updatedFirstname[MAX_NAME_LENGTH];
    char updatedLastname[MAX_NAME_LENGTH];
    char updatedDesignation[MAX_NAME_LENGTH];
    char updatedLocation[MAX_NAME_LENGTH];

    FILE *fp = fopen(employeeDataFile, "rb+");

    if (fp == NULL)
    {
        printf("Error: Unable to open users file.\n");
        exit(1);
    }

    printf("\t\t\t\033[0;32m Enter employee ID to update: \033[0m");
    scanf("%d", &employeeId);
    while (fread(&newEmployee, sizeof(Employee), 1, fp) == 1)
    {
        if (newEmployee.id == employeeId)
        {
            printf("\n\t\t\t\033[0;32m Enter new  Firstname : ");
            scanf(" %[^\n]s", updatedFirstname);
            printf("\n\t\t\tEnter new Lastname : ");
            scanf(" %[^\n]s", updatedLastname);
            printf("\n\t\t\tEnter new Designatione : ");
            scanf(" %[^\n]s", updatedDesignation);
            printf("\n\t\t\tEnter new Location : \033[0m");
            scanf(" %[^\n]s", updatedLocation);

            strcpy(newEmployee.fname, updatedFirstname);
            strcpy(newEmployee.lname, updatedLastname);
            strcpy(newEmployee.designation, updatedDesignation);
            strcpy(newEmployee.location, updatedLocation);

            int lnamelen = strlen(newEmployee.lname);
            char pass[MAX_PASSWORD_LENGTH];

            pass[0] = newEmployee.joiningDate[0];
            pass[1] = newEmployee.joiningDate[1];
            pass[2] = newEmployee.joiningDate[2];
            pass[3] = newEmployee.fname[0];
            pass[4] = newEmployee.fname[1];
            pass[5] = newEmployee.lname[lnamelen - 2];
            pass[6] = newEmployee.lname[lnamelen - 1];
            pass[7] = '\0';

            strcpy(newEmployee.password, pass);

            fseek(fp, -sizeof(Employee), SEEK_CUR);
            fwrite(&newEmployee, sizeof(Employee), 1, fp);
            printf("\n\t\t\t\033[0;32m Employee updated successfully.\n\033[0m");
            fclose(fp);
            return;
        }
    }
    fclose(fp);

    printf("\n\t\t\t\033[0;31m Employee not found.\n");
}

/*
    Author Name  : Sourabh Dipak Nalavade
    Description of Function  :
        The function is responsible for removing an employee from System.
        The function  collect the necessary information about the Employee such as the Employee Id to remove particular employee.

*/
void removeEmployee()
{
    Employee newEmployee;
    int employeeId;

    FILE *fp = fopen(employeeDataFile, "rb+");

    if (fp == NULL)
    {
        printf("Error: Unable to open users file.\n");
        exit(1);
    }

    printf("\n\t\t\t\033[0;32m Enter employee ID to remove: \033[0m");
    scanf("%d", &employeeId);

    while (fread(&newEmployee, sizeof(Employee), 1, fp) == 1)
    {
        if (newEmployee.id == employeeId)
        {
            newEmployee.id = 0;
            strcpy(newEmployee.fname, " ");
            strcpy(newEmployee.lname, " ");
            strcpy(newEmployee.designation, " ");
            strcpy(newEmployee.location, " ");
            strcpy(newEmployee.joiningDate, " ");

            strcpy(newEmployee.password, " ");

            fseek(fp, -sizeof(Employee), SEEK_CUR);
            fwrite(&newEmployee, sizeof(Employee), 1, fp);
            printf("\n\t\t\t\033[0;33m Employee removed successfully.\n\033[0m");
            fclose(fp);
            return;
        }
    }
    fclose(fp);

    printf("\n\t\t\t\033[0;31m Employee not found.\n\033[0m");
}

/*
    Author Name  : Sourabh Dipak Nalavade
    Description of Function  :
        The function is responsible for searching  and retrieving the information about an Employee using Employee id as a key.
*/
void searchEmployee()
{
    Employee newEmployee;
    int employeeId;
    int found=0;
    FILE *fp = fopen(employeeDataFile, "rb+");

    if (fp == NULL)
    {
        printf("Error: Unable to open users file.\n");
        exit(1);
    }
    int option=0;
    char opt[10];
    search:
    printf("\n\t\t\t Enter 1 to search by Id ");
    printf("\n\t\t\t Enter 2 to search by Name \n");
    printf("\n\t\t\t\033[0;32m Enter your choice: ");
    scanf(" %s",opt);
    option = atoi(opt);

    if(option==0)
    {
        printf("\n\t\t\t\033[0;31m Please enter valid input");
        goto search;
    }

    if(option==1)
    {
        
        printf("\n\t\t\t\033[0;32m Enter employee ID to search: \033[0m");
        scanf("%d", &employeeId);

        while (fread(&newEmployee, sizeof(Employee), 1, fp))
        {
            if (newEmployee.id == employeeId)
            {
                found = 1;
                printf("\n<------------------------  \033[0;33m Employee Details  \033[0m ------------------------>\n ");
                printf("\n\t\t\tEmployeeID       | \033[0;32m  %d  \033[0m",newEmployee.id);
                printf("\n\t\t\tFirst Name       | \033[0;32m  %s  \033[0m", newEmployee.fname);
                printf("\n\t\t\tLast Name        | \033[0;32m  %s  \033[0m", newEmployee.lname);
                printf("\n\t\t\tDesignation      | \033[0;32m  %s  \033[0m", newEmployee.designation);
                printf("\n\t\t\tLocation         | \033[0;32m  %s  \033[0m", newEmployee.location);
                printf("\n\t\t\tDate of Joining  | \033[0;32m  %s  \033[0m\n", newEmployee.joiningDate);
                rewind(fp);
                fclose(fp);
            }
        }
        if(found==0)
        {
            printf("\n\t\t\t\033[0;31m Employee not found.\n\033[0m");
        }
        fclose(fp);
    }
    if(option==2)
    {
        found=0;
        char name[20];
        printf("\n\t\t\t\033[0;32m Enter Employee Name to search: \033[0m");
        scanf(" %s", name);

        while (fread(&newEmployee, sizeof(Employee), 1, fp) == 1)
        {
            if (strcmp(newEmployee.fname,name)==0)
            {
                found = 1;
                printf("\n<------------------------  \033[0;33m Employee Details  \033[0m ------------------------>\n ");
                printf("\n\t\t\tEmployeeID       | \033[0;32m  %d  \033[0m",newEmployee.id);
                printf("\n\t\t\tFirst Name       | \033[0;32m  %s  \033[0m", newEmployee.fname);
                printf("\n\t\t\tLast Name        | \033[0;32m  %s  \033[0m", newEmployee.lname);
                printf("\n\t\t\tDesignation      | \033[0;32m  %s  \033[0m", newEmployee.designation);
                printf("\n\t\t\tLocation         | \033[0;32m  %s  \033[0m", newEmployee.location);
                printf("\n\t\t\tDate of Joining  | \033[0;32m  %s  \033[0m\n", newEmployee.joiningDate);
            }
        }
        if(found==0)
        {
            printf("\n\t\t\t\033[0;31m Employee not found.\n\033[0m");
        }
        rewind(fp);
        fclose(fp);
    }
}

/*
    Author Name  : Sourabh Dipak Nalavade
    Description of Function  :
        The function  displays an interactive menu for administrator.
*/
void adminMenu()
{
    int choice;
    char c[MAX_NAME_LENGTH];
    while (1)
    {
        printf("\n--------------------------------------- Administrator Menu ----------------------------------------\n");
        printf("\n");
        printf("\t\t\t\033[0;36m 1.  Add Employee\n");
        printf("\t\t\t 2.  Update Employee\n");
        printf("\t\t\t 3.  Search Employee\n");
        printf("\t\t\t 4.  Remove Employee\n");
        printf("\t\t\t 5.  View All Employee\n");
        printf("\t\t\t 6.  View Pending Request for Assests\n");
        printf("\t\t\t 7.  Add Assests\n");
        printf("\t\t\t 8.  Update Assests\n");
        printf("\t\t\t 9.  Remove Assests\n");
        printf("\t\t\t 10.  search Assests\n");
        printf("\t\t\t 11. View Assests in FreePool\n");
        printf("\t\t\t 12.\033[0m \033[0;31mLogout\n\033[0m");
        printf("\n\t\t\t\033[0;33m Please Enter your choice ( **Note: please enter an integer value ): \033[0m");

        scanf(" %s", c);
        choice=atoi(c);
        printf("\n");

        switch (choice)
        {
        case 1:
            addEmployee();
            break;
        case 2:
            updateEmployee();
            break;
        case 3:
            searchEmployee();
            break;
        case 4:
            removeEmployee();
            break;
        case 5:
            viewAllEmployees();
            break;
        case 6:
            checkPendingRequests();
            break;
        case 7:
            addAsset();
            break;
        case 8:
            updateAsset();
            break;
        case 9:
            deleteAsset();
            break;
        case 10:
            searchAsset();
            break;
        case 11:
            FreePoolAsset();
            break;
        case 12:
            adminlogout();
            return;
        default:
            printf("\n\t\t\t\033[0;31m Invalid choice. Please try again.\n\033[0m");
            break;
        }
    }
}

/*
    Author Name  : Sourabh Dipak Nalavade
    Description of Function  :
        The “adminLogin” function is responsible for  an administrator’s login.
        It takes the “USERNAME” and “PASSWORD” parameter an input.
        This function will compare the provided username and password with the stored administrator credentials to determine if they are valid or not.
        If valid the login is considered as successful.

*/
int adminLogin(char *username, char *password)
{

    FILE *fp;
    char user[MAX_NAME_LENGTH];
    char pass[MAX_PASSWORD_LENGTH];

    fp = fopen(adminDataFile, "r");
    if (fp == NULL)
    {
        perror("Error: Unable to open admindata.csv file.\n");
        exit(1);
    }

    while (fscanf(fp, "%s %s", user, pass) == 2)
    {
        if (strcmp(username, user) == 0 && strcmp(password, pass) == 0)
        {
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
        The function is responsible for getting the login details of an Administrator. 
        The function prompts the Administrator to enter their UserName and Password for successful login in Asset Management System.
*/

void getAdminLoginDetails(char *username, char *password)
{
    printf("\n\033[0;33m---------------------------------    Enter Your Login Details    ----------------------------------\033[0m\n");

    printf("\n\t\t\t\033[0;32m Enter your username: \033[0m");
    scanf("%s", username);
    printf("\t\t\t\033[0;32m Enter your password: \033[0m");
    printf("\033[8m");
    scanf("%s", password);
    printf("\n\033[0m");
    if (adminLogin(username, password))
    {
        printf("---------------------------------------------------------------------------------------------------\n");
        printf("\n                                    \033[0;33m Login successfull  \033[0m\n");
        printf("\n");    
        printf("  \033[0;33m                  Hello  %s , Welcome To Administrator Module\n\033[0m ",username);
        printf("\n");
        printf("***************************************************************************************************\n");
        adminMenu();
    }
    else
    {
        printf("\n<--*****************\033[0;31m Invalid Credentials!!! please enter valid details \033[0m *****************-->\n");
        getAdminLoginDetails(username, password);
    }
}