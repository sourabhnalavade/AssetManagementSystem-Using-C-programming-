#include "../header/myheader.h"

extern int employeeCount;

extern int assetCount;
// extern int LaptopID;
// extern int DesktopID;

extern char adminDataFile[MAX_NAME_LENGTH];
extern char employeeDataFile[MAX_NAME_LENGTH];
extern char requestDataFile[MAX_NAME_LENGTH];
extern char requestStatusDataFile[MAX_NAME_LENGTH];
extern char AssetFreepoolDataFile[MAX_NAME_LENGTH];
extern char assetDataFile[MAX_NAME_LENGTH];

/*
    Author Name  : Sourabh Dipak Nalavade
    Description of Function  : 
        The funtion takes asset details from console and stores it in file named assets.csv using file i/o operations.
        It generates unique AssetId for each asset.
*/


void addAsset()
{
    Asset newAsset={0};
    int c = 0;

    FILE *a = fopen(assetDataFile, "a+");

    FILE *freepool = fopen(AssetFreepoolDataFile, "a+");

    printf("\t\t\t\033[0;33mChoose from Below options \n\033[0m");
    printf("\n\t\t\t\033[0;32m 1. Laptop\n");
    printf("\n\t\t\t 2. Desktop\n");
    printf("\n\t\t\t Choose your option : \033[0m");
   
    scanf("%d", &c);
    if(c == 1)
    {
        int LaptopID;
        int DesktopID;
        FILE *file = fopen("assetIdFile.txt","r+");

        fscanf(file,"%d %d",&LaptopID,&DesktopID);
        fclose(file);
        file = fopen("assetIdFile.txt","w");  
        LaptopID++;
        fprintf(file,"%d %d",LaptopID,DesktopID);
        fclose(file);
        newAsset.id = LaptopID;
        
        strcpy(newAsset.type, "Laptop");
        strcpy(newAsset.status, "free");
        fwrite(&newAsset, sizeof(newAsset), 1, a);
        fwrite(&newAsset, sizeof(newAsset), 1, freepool);
        fclose(a);
        fclose(freepool);
    }
    else if(c == 2)
    {
        int LaptopID;
        int DesktopID;
        FILE *file = fopen("assetIdFile.txt","r+");

        fscanf(file,"%d %d",&LaptopID,&DesktopID);
        fclose(file);
        file = fopen("assetIdFile.txt","w");  
        DesktopID++;
        fprintf(file,"%d %d",LaptopID,DesktopID);
        fclose(file);

        newAsset.id = DesktopID;
        strcpy(newAsset.type, "Desktop");
        strcpy(newAsset.status, "free");
        fwrite(&newAsset, sizeof(newAsset), 1, a);
        fwrite(&newAsset, sizeof(newAsset), 1, freepool);
        fclose(a);
        fclose(freepool);
    }
    fclose(a);
    fclose(freepool);
   
    printf("\n\t\t\t\033[0;32m Asset with Type = %s and AssetId = %d added successfully.\n\033[0m", newAsset.type, newAsset.id);
}

/*
    Author Name  : Sourabh Dipak Nalavade
    Description of Function  : 
        This fuction is called from the employee.c file.
        The function reads the asset data from the file uing fread and displays the tagged assets of the employee
        who is currently logged in.
*/
void viewMyAssets(int employeeId)
{
    Request approvedRequest;

    FILE *myassets = fopen(requestStatusDataFile, "rb");

    printf("\n\033[0;33m <----------------------------------------------  Asset Details ---------------------------------------------->\033[0m\n");

    int assetpresent=0;
    while (fread(&approvedRequest, sizeof(Request), 1, myassets) == 1)
    {
        if (approvedRequest.empid == employeeId && strcmp(approvedRequest.status, "Approved") == 0)
        {
            printf("\n\t\t\t\033[0;32m Employee Id = %d Tagged Asset : %s \n\033[0m", approvedRequest.empid, approvedRequest.assetname);
            assetpresent=1;
        }
    }
    if(assetpresent==0)
    {
        printf("\n\t\t\t\033[0;31m Currently you are not tagged with any asset !!!\n\033[0m");
        printf("\n");
    }


    fclose(myassets);
}

/*
    Author Name  : Sourabh Dipak Nalavade
    Description of Function  : 
        This fuction is called from the admin.c file.
        The function reads the asset data from the file uing fread and updates the asset data with the new data.
*/
void updateAsset()
{
    Asset updatedAsset;
    int assetId;
    char type[MAX_NAME_LENGTH];

    FILE *assetfp = fopen(assetDataFile, "rb+");

    if (assetfp == NULL)
    {
        printf("Error: Unable to open users file.\n");
        exit(1);
    }

    printf("\n\t\t\t\033[0;32m Enter Asset ID to update: \033[0m");
    scanf("%d", &assetId);

    while (fread(&updatedAsset, sizeof(Asset), 1, assetfp) == 1)
    {
        if (updatedAsset.id == assetId)
        {
            printf("\n\t\t\t\033[0;32m Enter updated Asset Type : \033[0m");
            scanf("%s", type);

            strcpy(updatedAsset.type, type);

            fseek(assetfp, -sizeof(Asset), SEEK_CUR);

            fwrite(&updatedAsset, sizeof(Asset), 1, assetfp);
            printf("\n\t\t\t\033[0;33m Asset updated successfully.\n\033[0m");
            fclose(assetfp);
            return;
        }
    }
    fclose(assetfp);

    printf("\n\t\t\t\033[0;31m Asset with assetId %d not found.\n\033[0m",assetId);

}

/*
    Author Name  : Sourabh Dipak Nalavade
    Description of Function  : 
        The function reads the data from asstes.csv file.
        If the asset with the entered id is found,
        the particular asset is removed from the file using Asset id as a key.
*/
void deleteAsset()
{
    Asset deleteAsset;
    int assetId;

    FILE *delfp = fopen(assetDataFile, "rb+");

    if (delfp == NULL)
    {
        printf("Error: Unable to open assetdata.csv file.\n");
        exit(1);
    }

    printf("\t\t\t\033[0;32m Enter asset ID to remove: \033[0m");
    scanf("%d", &assetId);

    int present = 0;
    while (fread(&deleteAsset, sizeof(Asset), 1, delfp) == 1)
    {
        if (deleteAsset.id == assetId)
        {
            present = 1;
            strcpy(deleteAsset.type, " ");
            strcpy(deleteAsset.status, " ");
            deleteAsset.id = 0;

            fseek(delfp, -sizeof(Asset), SEEK_CUR);

            fwrite(&deleteAsset, sizeof(Asset), 1, delfp);

            printf("\n\t\t\t\033[0;32m Asset removed successfully.\n");
            fclose(delfp);
            break;
        }
    }
    fclose(delfp);
    
    if(present)
    {
        FILE *freepool = fopen(AssetFreepoolDataFile, "rb+");

        while (fread(&deleteAsset, sizeof(Asset), 1, freepool) == 1)
        {
            if (deleteAsset.id == assetId)
            {

                strcpy(deleteAsset.type, " ");
                strcpy(deleteAsset.status, " ");
                deleteAsset.id = 0;

                fseek(freepool, -sizeof(Asset), SEEK_CUR);

                fwrite(&deleteAsset, sizeof(Asset), 1, freepool);

                fclose(freepool);
                return;
            }
        }
        fclose(freepool);
    }
    

    printf("\n\t\t\t\033[0;31m Asset with assetId %d not found.\n\033[0m",assetId);

}

/*
    Author Name  : Sourabh Dipak Nalavade
    Description of Function  : 
        The function is responsible for searching  and retrieving the information about an assets using Asset id as a key.
*/
void searchAsset()
{
    Asset asset;
    int assetId;
    
    FILE *fp = fopen(assetDataFile, "r");

    if (fp == NULL)
    {
        printf("Error: Unable to open assetdata.csv file.\n");
        exit(1);
    }

    printf("\n\t\t\t\033[0;32m Enter Asset ID to search: \033[0m");
    scanf("%d", &assetId);
    printf("\t\t\t\033[0;33m Asset ID\t\t\t Type \n\033[0m");
   
    while (fread(&asset, sizeof(Asset), 1, fp) == 1)
    {
        if (asset.id == assetId)
        {
            printf("\t\t\t\033[0;33m  %d \t\t\t\t %s \n\033[0m", asset.id, asset.type);

            fclose(fp);
            return;
        }
    }
    printf("\n\t\t\t\033[0;31m Asset with assetId %d not found.\n\033[0m",assetId);
    fclose(fp);
}

/*
    Author Name  : Sourabh Dipak Nalavade
    Description of Function  : 
        The function reads the file named freepool.csv and retrievs the information 
        about an Asset in free pool using Asset status as a key.

*/
void FreePoolAsset()
{
    printf("\033[0;33m<------------------------------------  Assets in Free Pool  ------------------------------------>\033[0m\n");
    printf("\n");
    Asset asset;

    FILE *fp = fopen(AssetFreepoolDataFile, "rb+");

    if (fp == NULL)
    {
        printf("Error: Unable to open freepooldata.csv file.\n");
        exit(1);
    }

    int assetpresent = 0;
    printf("\t\t\t\033[0;33m Asset ID\t\t\t Type \n\033[0m");
    printf("\033[0;33m--------------------------------------------------------------------------------------------------\033[0m\n");
    while (fread(&asset, sizeof(Asset), 1, fp) == 1)
    {
        if (strcmp(asset.status, "free") == 0)
        {
           
            
            printf("\t\t\t\033[0;33m  %d\t\t\t\t %s \n\033[0m", asset.id, asset.type);
            assetpresent = 1;
        }
    }
    if (assetpresent == 0)
    {
        printf("\n\t\t\t\033[0;31m Currently no asset in freepool.  \033[0m\n");
    }
    fclose(fp);
}