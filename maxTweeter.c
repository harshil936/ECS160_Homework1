#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

const int ARRAY_SIZE = 21000;

/*
    Function to get the index of the "name"
*/
int getIndex(char* line, char* naam ){ 
    const char* tok;

    int num = 1;
    int actualNum = -2;
    int counter = 0;

    for (tok = strtok(line, ","); tok && *tok; tok = strtok(NULL, ",\n")){
        if (strcmp(tok, naam) == 0){
            counter++;
            if(actualNum == -2){
                actualNum = num;
            }
        }
        num++;
    }

    if(counter == 1){
        return actualNum;
    }
    else if(counter == 0){
        printf("The column %s is not in the csv file.\n", naam);
        return -1;
    }
    else{
        printf("There are %d columns that have %s as their header.\n", counter, naam);
        return -1;
    }
}

/*
    Function to get field in that column
*/
char* getfield(char* line, int num){
    char* tok;
    for (tok = strtok(line, ","); tok && *tok; tok = strtok(NULL, ",\n"))
    {
        if (!--num)
            return tok;
    }
    return NULL;
}

typedef struct query{
        char* tweeter;
        int posts;
    }query;

//Implemented quicksort based on code from this website: https://www.geeksforgeeks.org/quick-sort/
void swap(query* a, query* b)
{
    struct query t = *a;
    *a = *b;
    *b = t;

}
 
/* This function takes last element as pivot, places
   the pivot element at its correct position in sorted
    array, and places all smaller (smaller than pivot)
   to left of pivot and all greater elements to right
   of pivot */
int partition (query arr[], int low, int high)
{
    int pivot = arr[high].posts;    // pivot
    int i = (low - 1);  // Index of smaller element
 
    for (int j = low; j <= high- 1; j++)
    {
        // If current element is smaller than or
        // equal to pivot
        if (arr[j].posts <= pivot)
        {
            i++;    // increment index of smaller element
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}
 
/* The main function that implements QuickSort
 arr[] --> Array to be sorted,
  low  --> Starting index,
  high  --> Ending index */
void quickSort(query arr[], int low, int high)
{
    if (low < high)
    {
        /* pi is partitioning index, arr[p] is now
           at right place */
        int pi = partition(arr, low, high);
 
        // Separately sort elements before
        // partition and after partition
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void printArray(query arr[], int highInd){
    printf("{");

    if(highInd < 9){

        for (int i = highInd; i >= 0; --i)
        {
            printf("{%s = %d}", arr[i].tweeter, arr[i].posts);
        }

    }

    else{

        for (int i = highInd; i > highInd-10; --i)
        {
            printf("{%s = %d}", arr[i].tweeter, arr[i].posts);
        }

    }

    printf("}\n");
}

bool containsSpace (char* line){
    for(int i = 0; i < strlen(line)-1; i++){
        if(!isspace(line[i])) {
            return 1;
        }
    }
    return 0;
}

int main(int argc, char*argv[]){

    if(argv[1] == NULL){
        printf("Invalid Input File");
        return -1;
    }

    if(argv[2] != NULL){
        printf("Invalid Input File\n");
	return -1;
    }

    char* filename = argv[1];
    FILE* stream = fopen(filename, "r+");

    if(stream == NULL){
        printf("Invalid Input File");
        return -1;
    }
    
    char line[1024];
    int columnNo = 0;
    int row = 0;

    bool FirstLoop = 0;
    int addingInd = 0; 

    struct query q[ARRAY_SIZE]; 

    while (fgets(line, 1024, stream)){

        /*
        It just copies the content from the line to tmp because line is memory location, therefore we use strdup: string duplication.
        */
        char* tmp = strdup(line);
        
        if (strcmp(tmp,"\n") == 0 ){
            printf(" There is an empty line in your input file at line number: %d\n", row+1 );
        }

        else if (containsSpace(tmp) == 0 ){
            printf(" There are white spaces in your input file at line number: %d\n", row+1 );
        }
         
        else {
            /*
            Only to get the index of the column 
            */
            if( FirstLoop == 0){ 
                char* naam = "\"name\"";
                columnNo = getIndex(tmp, naam);
                if ( columnNo == -1){
                    return 0;
                }
                else{
                    // printf(" Names is at column number %d\n", columnNo );
                    FirstLoop = 1;
                    row = 0;
                    // printf("Field 3 would be %d\n", columnNo);
                }
            }


            /*
            This for the second row onwards
            */
            else {
                row++;

                bool found = 0;

                int k = 0;

                char* account = getfield(tmp, columnNo);
		if(account == NULL){
			printf("Invalid Input File\n");
			return -1;		
		}
                if(strcmp(account, "\"\"") == 0){
                    account = "\"No Name\"";
                }
                
                //printf("In first loop rn\n");
                while(k < addingInd){
                    if(strcmp(account, q[k].tweeter) == 0) {
                        q[k].posts++;
                        found = 1;
                        break;
                    }
                    k++;
                }
                
                if(!found){
                    q[addingInd].tweeter = strdup(account);
                    q[addingInd].posts = 1;
                    addingInd++;
                }
            }
        } 
        free(tmp);
    }

    quickSort(q, 0, addingInd - 1);

    printArray(q, addingInd - 1);
}
