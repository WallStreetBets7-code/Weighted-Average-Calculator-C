/*
    Author: Sebastian Y. 
    Date of Completion: 26 November 2023

    Purpose: This program will take a user-defined number of weighted average categories for their class e.g. homework, tests, exams, etc. 
             The program will then prompt the user for each category weight and the user's current grade they recieved. The programs main bread and butter 
             dynamically allocates arrays for the weighted categories, the user score, as well as the weight category names. 

*/

#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <time.h>

void getChoice(void);
void buildValueArray(double weightPercent, double userGrade, int forLoopCounter, double *pointsArr, int weightAmt);
double sumArray(double *pointsArr, int sizeOfPointsArr);
void writeToFile(char *fileName, char (*categoryNames)[255], double *weightedPercent, double *userGradePercent, double finalGrade, int arrSize);
void clearKeyboardBuffer(void);

int main(void)
{
    getChoice();
    return 0; 
}

void getChoice(void) 
{
    int nocWeight, weightAmount, nocPercentGrade, nocWeightPercent;
    double finalGrade;

    char fileName[255];

    printf("$ Create a file\n$ Enter the file name: ");
    scanf("%254s", fileName); //%254s to prevent any overflow 

    //Prompting user for the amount of weighted categories there are
    do
    {
        printf("$ How many weighted categories? ");
        nocWeight = scanf(" %d", &weightAmount);
        clearKeyboardBuffer();
    }
    while(nocWeight != 1 || weightAmount < 1);

    //Dynamically allocating the array according to the provided weight category amounts
    //Because this array is dynamically allocated it is much better than the enum-switch version of this code, as we do not know the size of the array 
    char (*weightName)[255] = malloc(weightAmount * sizeof(char[255])); 
    double *weightPercentage = malloc(weightAmount * sizeof(double));
    double *userGrade = malloc(weightAmount * sizeof(double));

    //Array used to store the points calculated from the users grade and the weight of the category, will add these elements up later to determine final value
    double *pointsArr = malloc(weightAmount * sizeof(double));

    if(!weightName || !weightPercentage || !userGrade || !pointsArr)
    {
        printf("\033[0;31mERROR: Memory Allocation\n\033[0m\n"); //Red Text
        exit(1);
    }

    printf("\n");

    for(int i = 0; i < weightAmount; i++) 
    {
        printf("$ Enter the weight name [%d]: ", i + 1);
        fgets(weightName[i], sizeof(weightName[i]), stdin);

        //This bit will get rid of the newline character at the end and replace it with the null terminator
        size_t length = strlen(weightName[i]);
        if (length > 0 && weightName[i][length - 1] == '\n') 
            weightName[i][length - 1] = '\0'; // Replace newline with null terminator
        
        printf("$ Enter the weight for %s: ", &weightName[i]); 
        nocWeightPercent = scanf("%lf", &weightPercentage[i]); 

        if(weightPercentage[i] < 0 || weightPercentage[i] > 100 || nocWeightPercent != 1)
        {
            clearKeyboardBuffer();
            exit(1);
        }

        printf("$ Enter your grade: ");
        nocPercentGrade = scanf("%lf", &userGrade[i]);

        if(userGrade[i] < 0 || userGrade[i] > 100 || nocPercentGrade != 1)
        {
            clearKeyboardBuffer();
            exit(1);
        }

        printf("\n");
    
        //Building the array with values...
        buildValueArray(weightPercentage[i], userGrade[i], i, pointsArr, weightAmount);
        clearKeyboardBuffer();
    }  

    //Calculating the final grade...
    finalGrade = sumArray(pointsArr, weightAmount);

    //Writing to file...
    writeToFile(fileName, weightName, weightPercentage, userGrade, finalGrade, weightAmount);

    printf("File write complete\n");

    //Cleaning up any dynamically allocated arrays
    free(weightName);
    weightName = NULL;

    free(weightPercentage);
    weightPercentage = NULL;

    free(userGrade);
    userGrade = NULL;

    free(pointsArr);
    pointsArr = NULL; 
}

void buildValueArray(double weightPercent, double userGrade, int forLoopCounter, double *pointsArr, int weightAmt)
{
    double result;
    double result1 = weightPercent / 100; //converting percent value to decimal

    double totalWeightPoints = result1 * userGrade;

    //We pass the for loop counter (i) into this function as an argument to assign the array elements
    pointsArr[forLoopCounter] = totalWeightPoints;
}

double sumArray(double *pointsArr, int sizeOfPointsArr)
{
    //This function will add the elements of the array together given the arguments of the array and the initial size the user determines in the getChoice function
    int j; 
    double finalGrade = 0; 

    for(j = 0; j < sizeOfPointsArr; j++)
    {
        finalGrade += pointsArr[j];
    }
    return finalGrade;
}

void writeToFile(char *fileName, char (*categoryNames)[255], double *weightedPercent, double *userGradePercent, double finalGrade, int arrSize)
{
    //Write the calculations into a file with the name specified by the user
    FILE *output;
    output = fopen(fileName, "a");
    
    if(output == NULL) //Checking filestate
    {
        printf("\033[0;31mError opening file %s - Exiting\n\033[0m\n", fileName); //Red Text
        exit(1);
    }

    //Just a nice touch to this, adds the current date and time to the file for usertracking
    time_t currentTime; 
    char *currentDateandTime; 

    time(&currentTime);
    currentDateandTime = ctime(&currentTime);

    fprintf(output, "%s\n", currentDateandTime);

    //Iterating through the arrays passed by arguments. Printing...
    for(int k = 0; k < arrSize; k++)
        fprintf(output, "Category Name:\n%s\nCategory Weight:\n%.2f%%\nGrade Received:\n%.2f%%\n\n", categoryNames[k], weightedPercent[k], userGradePercent[k]);
    
    fprintf(output, "\nFinal grade: %f\n", finalGrade);
    fclose(output);
}

void clearKeyboardBuffer(void)
{
    char c = 's';
    while (c != '\n')
    {
        scanf("%c", &c);
    }
}