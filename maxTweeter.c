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
    int num = 1; //this is to keep track of what column we are in right now.
    int index = -2; // using this variable to store 'num' when the name is appeared.
    int counter = 0; // This is to keep track of number of appearance of 'name'
    for (tok = strtok(line, ","); tok && *tok; tok = strtok(NULL, ",\n")){
        if (strcmp(tok, naam) == 0){ // if the current token is equal to the 'name'.
            counter++; // increment the counter.
            if(index == -2){ // if this is the first appearance of the 'name'
                index = num; // change the value of the index so it never enters this if statement again.
            }
        }
        num++; // increment the num to keep going further.
    }

    if(counter == 1){
        return index;
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

//This is the "object" we will be using to store the names and number of posts associated with each name
typedef struct query{
        char* tweeter;
        int posts;
    }query;

//Implemented quicksort based on code from this website: https://www.geeksforgeeks.org/quick-sort/
/*This is a basic swap function that takes two query pointers as parameters 
and swaps the values at the locations they point to*/
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

/*This function prints the contents of the first argument, 
which is a array of "query" instances, starting from the index specified by the second argument.*/
void printArray(query arr[], int highInd){
    printf("{");

    /*This conditional checks whether or not index is high enough: 
    if the array has less than elements, then we need a separate for loop that takes care of this.*/
    if(highInd < 9){

        for (int i = highInd; i >= 0; --i)
        {
            printf("{%s = %d}", arr[i].tweeter, arr[i].posts);
        }

    }

    //This is the normal case, which prints the top 10 values.
    else{

        for (int i = highInd; i > highInd-10; --i)
        {
            printf("{%s = %d}", arr[i].tweeter, arr[i].posts);
        }

    }

    printf("}\n");
}

//This function takes in a String argument and checks whether or not the argument is just spaces or not
bool containsSpace (char* line){
    for(int i = 0; i < strlen(line)-1; i++){
        if(!isspace(line[i])) {
            return 1;
        }
    }
    return 0;
}

int main(int argc, char*argv[]){

    //This conditional checks whether there is an argument for the command: if there isn't, the program stops.
    if(argv[1] == NULL){
        printf("Invalid Input File");
        return -1;
    }

    //This conditional checks whether or not there is a second argument: if there is, the program terminates.
    if(argv[2] != NULL){
        printf("Invalid Input File\n");
	return -1;
    }

    //These lines of code deal with input streams and reading in the csv file.
    char* filename = argv[1];
    FILE* stream = fopen(filename, "r+");

    //checks whether or not the file is empty: if it is, the program terminates.
    if(stream == NULL){
        printf("Invalid Input File");
        return -1;
    }
    
    char line[1024];//Character array that will be used to read in each line of the file
    int columnNo = 0;//used to get the column number of "name"
    int row = 0;//keeps track on which row of the file we are on: this was meant for debugging purposes

    bool FirstLoop = 0;//boolean value that checks whether or not we are reading in the headers of the file
    int addingInd = 0;//This is the index at which we will be adding each element inside the array of names

    struct query q[ARRAY_SIZE];//This array will contain 'query' instances which will have the information of the tweeters and how many posts they posted

    while (fgets(line, 1024, stream)){

        /*
        It just copies the content from the line to tmp because line is memory location, therefore we use strdup: string duplication.
        */
        char* tmp = strdup(line);
        
        //This checks whether the current is empty or not
        if (strcmp(tmp,"\n") == 0 ){
            printf(" There is an empty line in your input file at line number: %d\n", row+1 );
        }

        //This calls the "containsSpace" function, which checks whether the line is just spaces or not
        else if (containsSpace(tmp) == 0 ){
            printf(" There are white spaces in your input file at line number: %d\n", row+1 );
        }
        
        //if the line has characters in it
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
                row++;//increments the row number for each iteration of the while loop

                bool found = 0;//this checks whether or not the current name was already found or not

                int k = 0;//incrementer for the index of the array "q"

                char* account = getfield(tmp, columnNo);//This calls the getfield method, which gets the name from the appropriate column
		if(account == NULL){
			printf("Invalid Input File\n");
			return -1;		
		}       
                //this conditional checks whether or not the account name is empty
                if(strcmp(account, "\"\"") == 0){
                    account = "\"No Name\"";
                }
                
                //This while loop searches through the array "q" and checks whether the name has been found yet or not
                while(k < addingInd){
                    if(strcmp(account, q[k].tweeter) == 0) {
                        q[k].posts++;
                        found = 1;
                        break;
                    }
                    k++;
                }
                
                //This conditional checks whether or not the name was found; if it wasn't, add it to the array
                if(!found){
                    q[addingInd].tweeter = strdup(account);
                    q[addingInd].posts = 1;
                    addingInd++;
                }
            }
        }
        //frees the memory space allocated to temp 
        free(tmp);
    }

    //sorts the array
    quickSort(q, 0, addingInd - 1);

    //prints the array contents, specifically the top 10
    printArray(q, addingInd - 1);
}
