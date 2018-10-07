/**
 * @author ${hisham_maged10}
 *https://github.com/hisham-maged10
 * ${DesktopApps}
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <conio.h>
#include <windows.h>
#include <string.h>
#define ImagesNumber 1000
#define ArraySize 784
#define PxSize 785
#define PxLineSize 28
#define TYPE int
/*                                                       Main.c                                                             */
typedef struct
{
    int img_array[ArraySize];
}img;
typedef struct
{
    img ImgData;
    TYPE key;
}data;
typedef struct
{
    TYPE data;
    struct node*next;
}node;
typedef struct
{
    node *head;
    node *tail;
}Llist;
typedef struct
{
    Llist Hash_Chain[ImagesNumber];

}chain;
int HashMapChainCollisions=0;
int firstrun=1;
int LoadedImagesNo=0;
data Img_Data[ImagesNumber];
data ForSeachImg[ImagesNumber];
chain HashMap;
int firstChain=1;
void OperationMenu();
void ChainingMenu();
void MainMenu();
node *CreateNode(TYPE value);
void Initialize(Llist *linkedlist);
void Add(Llist *linkedlist,int value);
void Load();
void Put(TYPE MapKey,img CurrImage);
int removeFromChaining(img CurrImage);
int HashCode(img Img);
int returnkey;
int Get(img CurrImage);


node* CreateNode(TYPE value)
{
    node *n = (node*)malloc(sizeof(node));
    n->data=value;
    n->next=NULL;
    return n;
}
void Initialize(Llist *linkedlist)
{
    linkedlist->head=NULL;
    linkedlist->tail=NULL;
}
void Add(Llist *linkedlist,int value)
{
    node *n = CreateNode(value);
    node *temp=linkedlist->tail;
    if(temp!=NULL)
    {
        linkedlist->tail->next=n;
        linkedlist->tail=n;
    }
    else
    {
        linkedlist->head=n;
        linkedlist->tail=n;
    }
}
void Load()
{
    FILE *file;
    int PxCounter=1,ImgCounter=0;
    char File_Name[25];
    if(!firstrun)
    {
    puts("Enter the name of the file you want to search in:-\n");
    scanf("%s",&File_Name);
    }
    else
    {
        strcpy(File_Name,"Data.txt");
    }
    if((strstr(File_Name,".txt")!=NULL))
    {
    file=fopen(File_Name,"r");
    }
    else
    {
        strcat(File_Name,".txt");
        file=fopen(File_Name,"r");
    }
    if(file)
    {
        while(!feof(file))
        {
            if(PxCounter==PxSize)
            {
                if(firstrun)
                {
                fscanf(file,"%d",&Img_Data[ImgCounter++].key);
                PxCounter=1;
                }
                else if(firstrun==0)
                {
                    ImgCounter++;
                    LoadedImagesNo++;
                    PxCounter=1;

                }
                if(ImgCounter==ImagesNumber)
                {
                    break;
                }
            }
            if(firstrun)
            {
            fscanf(file,"%d",&Img_Data[ImgCounter].ImgData.img_array[(PxCounter-1)]);
            }
            else
            {
            fscanf(file,"%d",&ForSeachImg[ImgCounter].ImgData.img_array[(PxCounter-1)]);
            }
            PxCounter++;


        }
        firstrun=0;
    fclose(file);
    }
    else if(file==NULL)
    {
        puts("Error, File Doesn't Exist.");
       if(getch())
            Load();
    }


}
/*get function checks whether the Image is in the hashtable and the slot's LinkedList or not and if so it returns the Key which is it's place in the HashTable
if not return -1 and in the search function if it checks and finds the returned value -1 it will print error, not found and if found it will print Image Found and prints the Key of the Image*/
int Get(img CurrImage)
{
    int Hash,Key,i,PxCounter;
    Key=PxCounter=0;
    int ErrorFlag=0;
    Hash=HashCode(CurrImage);
    node* thisnode;
    /* if the node is not equal to null then take the Key,hashcode of the Image and make a temporary node that holds the head node*/
    if(HashMap.Hash_Chain[Hash].head)
    {
        Key=HashMap.Hash_Chain[Hash].head->data;
        i=HashCode(Img_Data[Key].ImgData);
        thisnode=HashMap.Hash_Chain[Hash].head;
    }
    /* if the node is equal to null then return Error value -1 */
    else if(HashMap.Hash_Chain[Hash].head==NULL)
    {
        return -1;
    }
    /* while the temporary node is not equal to null then firstly do a check that each image we point to is still equal to the hashcode we got
    then
    make an infinity loop with PxCounter increasing by one each time with stopping condition that u reached the given number of pixels which is 784
    inside that infinity loop check if the two images are the same, if not then make the error flag =-1 and break the infinity loop
    then make a check if Errorflag is not equal to -1 then return the key else if errorflag equals -1 then return the error value which is equal to -1*/
    while(thisnode)
    {
        Key=thisnode->data;
        i=HashCode(Img_Data[Key].ImgData);
        if(Hash==i)
        {
            PxCounter=0;
            ErrorFlag=0;
            /* infinity loop */
            while(1)
            {
                /* stopping condition */
                if(PxCounter==ArraySize) break;
                /* check */
                if(Img_Data[Key].ImgData.img_array[PxCounter]!=CurrImage.img_array[PxCounter])
                {
                    ErrorFlag=-1;
                    break;
                }
                /* loop's counter */
                PxCounter++;
            }
        /* same image*/
        if(ErrorFlag!=-1)
        {
            return Key;
        }
        /* to get to the next node */
         thisnode=thisnode->next;
        }
        else
        {
            return -1;
        }


        }
    return -1;
}
/*the put function gets a HashCode using "Hash mod 1000" technique and checks whether in that slot in the hashtable there is a collision, if so then add an element
to the linked list's Tail's next and make the tail point to it where each slot in the hashtable is a pointer to a linked list if not then initialize a linkedlist in that slot and put the key of the image in the Head and tail of the linked list*/
void Put(TYPE MapKey,img CurrImage)
{
    /*  Get the HashCode of the Currently passed Image */
    int Hash=HashCode(CurrImage);
    /* if there was already a collision in the HashMap then add the MapKey to the next node of the linkedList and increase the number of collisions which
    is a global variable */
    if(HashMap.Hash_Chain[Hash].head)
    {
        Add(&HashMap.Hash_Chain[Hash],MapKey);
        HashMapChainCollisions++;
    }
    /* else it is the first time then head will be null then initialize the linked list and put the key in it's tail which is also it's head at the moment */
    else
    {
        Initialize(&HashMap.Hash_Chain[Hash]);
        Add(&HashMap.Hash_Chain[Hash],MapKey);
    }
}
/* This methods is a copy of the get method but the last conditions are modified for Removal if found*/
int removeFromChaining(img CurrImage)
{
    int Hash,Key,i,PxCounter;
    Key=PxCounter=0;
    int ErrorFlag=0;
    Hash=HashCode(CurrImage);
    node* thisnode,*Check;
    /* if the node is not equal to null then take the Key,hashcode of the Image and make a temporary node that holds the head node*/
    if(HashMap.Hash_Chain[Hash].head)
    {
        Key=HashMap.Hash_Chain[Hash].head->data;
        i=HashCode(Img_Data[Key].ImgData);
        thisnode=HashMap.Hash_Chain[Hash].head;
        Check=thisnode;  //make a node to check where it is in the list
    }
    /* else the node is equal to null then return the ErrorValue which is equal to -1 */
    else if(HashMap.Hash_Chain[Hash].head==NULL)
    {
        return -1;
    }
    /* while the temporary node is not equal to null then firstly do a check that each image we point to is still equal to the hashcode we got
    then
    make an infinity loop with PxCounter increasing by one each time with stopping condition that u reached the given number of pixels which is 784
    inside that infinity loop check if the two images are the same, if not then make the error flag =1 and break the infinity loop
    then make a check if Errorflag is equal 0 then return the key else if errorflag equals 1 then it means that the two images are not the same then the error value which is equal
    to -1
    */
    while(thisnode)
    {
        Key=thisnode->data;
        i=HashCode(Img_Data[Key].ImgData);
        if(Hash==i)
        {
            PxCounter=0;
            /* infinity loop */
            while(1)
            {
                /* stopping condition */
                if(PxCounter==ArraySize) break;
                /* check */
                if(Img_Data[Key].ImgData.img_array[PxCounter] != CurrImage.img_array[PxCounter])
                {
                    ErrorFlag=-1;
                    break;
                }
                else
                {
                    ErrorFlag=0;
                }
                /* loop's counter */
                PxCounter++;
            }
        /* same image*/
        if(ErrorFlag!=-1)//search for it as get but when we find it we have 3 option either its in between the head and the tail or on the head or on the tail
            {
                if(HashMap.Hash_Chain[Hash].head == thisnode)//means in the head
                {
                    HashMap.Hash_Chain[Hash].head=NULL;
                    HashMap.Hash_Chain[Hash].tail=NULL;
                    free(thisnode);
                    returnkey=Key;
                    return Key;
                }
                else//means the node we want remove is the tail or in the middle and will make temporary node equal to the tail or the target of the linkedlist and free the node that we want to remove
                {
                      while(Check->next != thisnode){Check=Check->next;}
                    if(thisnode->next){free(thisnode);}
                    else if(!thisnode->next)
                    {
                        HashMap.Hash_Chain[Hash].tail=Check;
                        Check->next=NULL;
                        free(thisnode);
                    }
                    returnkey=Key;
                    return Key;

                }
            }
            /* to get to the next node */
            thisnode=thisnode->next;
        }
        /* Error Occured */
        else
        {
            return -1;
        }
        }

return -1;
    }


int HashCode(img Img)
{
    int TotalLine,RowNo,PxCount,Hash;
    TotalLine=PxCount=Hash=0;
    RowNo=1;
    while(1)
    {
        /* The loop ends when all the data is Covered */
        if(PxCount==ArraySize)
        {
            break;
        }
    /* Multiply the Sum of values covered in the loop till now with the number of rows, and get the hashcode where the hashcode is the sum of all
    the columns multiplied by the Rows' numbers and inc. the Rows' Number by one and make the Sum of values covered equals zero to sum the next row separately*/
    if(PxCount%PxLineSize==0)
    {
        TotalLine*=(RowNo++);
        Hash+=TotalLine;
        TotalLine=0;
    }
    //To sum all the Values of the Columns
    else
    {
     TotalLine+=Img.img_array[PxCount];
    }
    PxCount++; //for breaking condition
    }
    Hash%=1000; //to get the correct hash
    return Hash;
}

void ChainingMenu()
{
    system("cls");
    printf("      ---------------------------------------ChainingMenu---------------------------------------\n\n");
    int j;
    if(firstChain)
    {
    Load();
    fflush(stdin);
    clock_t Start = clock();
    for(j=0; j<ImagesNumber; j++)
    {
        Put(Img_Data[j].key,Img_Data[j].ImgData);
       // printf("%d\n",)
    }
    clock_t End = clock();
    double time_spent = (double)(End - Start) / CLOCKS_PER_SEC;
    printf("Collisions: %d\n\n",HashMapChainCollisions);
    printf("Time: %lf\n\n",time_spent);
    Load();
    //printAllKeysImages();
    firstChain=0;
    }
    printf("                                    Search (1)\n\n");
    printf("                                    Remove (2)\n\n");
    printf("                                    please choose an option: \n");
    switch(getch())
    {
    case '1':
        {
            for(j=0;j<=LoadedImagesNo;j++)
            {
             if(Get(ForSeachImg[j].ImgData)==-1)
             {
                 puts("Error, Not Found!!\n");
             }
             else
             {
                 printf("Image Found and of ID[%d]\n\n",Get(ForSeachImg[j].ImgData));
             }
            }
            printf("press any key to continue...");
            getch();
            ChainingMenu();
            break;
        }
    case '2':
        {

            for(j=0;j<=LoadedImagesNo;j++)
            {
                if(removeFromChaining(ForSeachImg[j].ImgData)==-1)
             {
                 puts("Error, Not Found!!\n");
             }
             else
             {
                 printf("Image Found and of ID[%d] and deleted\n\n",returnkey);
             }
            }
            printf("press any key to continue...");
            getch();
            ChainingMenu();
            break;
        }
    default:
    {
        printf("Please choose a correct option ! (press any key to continue)");
        getch();
        system("cls");
        MainMenu();
        break;
    }
    }
}

/*
OperationMenu:
the menu that gives the users the option to navigate the program using getch where a switch case is applied to getch
*/
void OperationMenu()
{
    system("cls");
     printf("      ---------------------------------------OperationsMenu(Separate Chaining)---------------------------------------\n\n");
    printf("                                    HashMap by Chaining (1)\n\n");
    printf("                                    Return to Main Menu(2)\n\n");
    printf("                                    please choose an option: \n");
    switch(getch())
    {
    case '1':
        {
            ChainingMenu();
            break;
        }
    case '2':
        {
            MainMenu();
            break;
        }
    default:
    {
        printf("Please choose a correct option ! (press any key to continue)");
        getch();
        system("cls");
        MainMenu();
        break;
    }
    }
}
void printAllKeysImages()
{
    int i;
    for(i=0;i<=ImagesNumber;i++)
    {
        printf("%d\n",ForSeachImg[i].key);
    }
}
/*
MainMenu:
the menu that gives the users the option to start the program or exit it using getch where a switch case is applied to getch
*/
void MainMenu()
{
    system("cls");
     printf("      ---------------------------------------MainMenu---------------------------------------\n\n");
    printf("                                    Operation (1)\n");
    printf("                                    Exit (2)\n");
    printf("                                    please choose an option: \n");
    switch(getch())
    {
    case '1':
        {
            OperationMenu();
            break;
        }
    case '2':
        {
            exit(1);
            break;
        }
    default:
    {
        printf("Please choose a correct option ! (press any key to continue)");
        getch();
        system("cls");
        MainMenu();
        break;
    }
    }
}
/* Main function: it changes the color for the whole program to background:blue and foreground:white then loads the Images then opens the MainMenu*/
int main()
{
    system("color 17");
    MainMenu();
    return 0;
}

