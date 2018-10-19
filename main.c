#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
        int id;
        char first[15];
        char last[15];
        int grade1;
        int grade2;
        float finalGrade;
    }student;
//doing an array of struct is costly for memory especially when deleting and copying/shifting indices
//therefore we use an array of pointers

//function prototypes
student **create_class_list( char *filename, int *sizePtr); //allocates memory for classlist and stores it
int find (int idNo, student **list, int size);
void input_grades( char *filename, student **list, int size);
void compute_final_course_grades(student **list, int size); //assuming grades have already been input
void output_final_course_grades(char *filename, student **list, int size);
void print_list( student **list, int size);
void withdraw(int idNo, student **list, int *sizePtr);
void destroy_list(student **list, int *sizePtr);

int main()
{
    student **class_list;
    int size = 2; //temp assignment
    char filename[] = "my_class.txt";
    char outputFile[] = "outputFile.txt";
    int index;
    class_list = create_class_list(filename, &size);

    //test for find()
    index = find(400, class_list, size);
        printf("\nThe student ID searched for is located at index: %d\n", index);
    //test input_grades
    input_grades(filename, class_list, size);
        for(int i=0; i<size; i++){
            printf("%d\t%s %s %d %d\n", (*class_list[i]).id, class_list[i]->first, class_list[i]->last, class_list[i]->grade1, class_list[i] ->grade2);
        }
    //test function calls
    compute_final_course_grades(class_list, size);
    output_final_course_grades(outputFile, class_list, size);
    print_list(class_list, size);
    withdraw(200, class_list, &size);
    withdraw(500, class_list, &size); //test for student not in class
    print_list(class_list, size);
    destroy_list(class_list, &size);
    return 0;
}

//function works
student **create_class_list( char *filename, int *sizePtr){
    //open file
    FILE *input_file = fopen("my_class.txt", "r");
    int n =1;
    // open filename read-only
    //error-handling
    if(input_file == NULL){
        printf("File did not open");
        return NULL;
    }
     //read out class size from file
    fscanf(input_file, "%d", &n);
    //allocate array of pointers
    student **list = malloc(n * sizeof(student *)); //only needs to store pointer to students -> NOTE memory doesnt not have to be initialized to 0

    for(int i=0; i < n ;i++ ){
        //allocate memory for student
        list[i] = calloc(1, sizeof(student));//1st arg #ofobjects, 2nd arg #bytes for each object
        //reading data for student (line by line - > NOTE 3 inputs per fscanf)
        fscanf(input_file, "%d%s%s", &list[i]->id, list[i]->first, list[i]->last); //(*list[i]).id would ne equivalent
    }
    *sizePtr = n;//reset size in main to size read from file
    //close the file
    fclose(input_file);
    return list;
}
int find (int idNo, student **list, int size){
    int flag = 0;
    //count through class list and compare id's
    for(int i=0; i < size; i++){
        if(idNo == list[i]->id){
            flag=1;
            return i;
        }
    }

//return statements
if(flag == 0){
        return -1;
    }

    return 0; //function should not return 0
}
void input_grades( char *filename, student **list, int size){
    int proj1, proj2, index, idNo;
    //open file
    FILE *grades_file = fopen("class_grades.txt", "r");
    for(int i=0; i<size; i++){
        //scan id num and grade1&2 from line
        fscanf(grades_file,"%d%d%d", &idNo, &proj1, &proj2);
        //find appropriate index to update
        index = find(idNo, list, size);
        //update grades at index
        list[index]->grade1 = proj1;
        list[index]->grade2 = proj2;
    }

    return;
}
void compute_final_course_grades(student **list, int size){
    for(int i=0; i < size; i++){
        list[i]->finalGrade = (list[i]->grade1 + list[i]->grade2)/2; //average of respective grade 1 and 2
    }
    return;
}
void output_final_course_grades(char *file_out, student **list, int size){
    //open file
    FILE *output_file = fopen(file_out, "w"); //set output file with write capabilites
    //write in number of students
    fprintf(output_file, "%d\n", size);
    //loop through list and write out grades and ids
    for(int i=0; i < size; i++){
        fprintf(output_file, "ID:%d\tGrade:%f\n", list[i]->id, list[i]->finalGrade);
    }
    //note that id's are entered in sorted already but if they were not could create sort()
    fclose(output_file); //close file
    return; //return to main()
}
void print_list( student **list, int size){
    //print everything in a for loop - note could separate into various printf() statements to make code neater
    for(int i=0; i<size; i++){
        printf("ID:%d Name: %s %s Project 1 Grade:%d Project 2 Grade:%d Final Grade: %.2f\n", (*list[i]).id, list[i]->first, list[i]->last, list[i]->grade1, list[i]->grade2, list[i]->finalGrade);
    }
    return;
}
void withdraw(int idNo, student **list, int *sizePtr){ //note: sizePtr is passed instead of size so that size can be altered within function
    int size = *sizePtr, index = 0, flag = 0; //flag to see if student is in class
    //find student that had withdrawn by id
    for(int i=0; i<size; i++){
        if(list[i]->id == idNo){
            //check if matches withdrawn student id
            index = i;
            flag = 1;
            break; //exit for loop
        }//end if
    }
    if(flag ==0){
        printf("\nStudent is not in class.\n");
        return;
    }
    //move all remaining students forward to fill memory
    for(int i=index; i<size-1; i++){// being directly after index freed and move up one
        list[i] = list[i+1]; //point 1 to the 'right'
    }
    //free memory used by withdrawn student
    free(list[size]); //since everyone still in the class has moved up and been copied over the withdrawn student
    //reduce size
    *sizePtr = size - 1;
    return;
}
void destroy_list(student **list, int *sizePtr){
    //memory has to be freed in reverse order it is allocated
    for(int i=0; i < *sizePtr; i++){
        free(list[i]);
    }
    free(list);
    printf("\nList successfully destroyed.");
    return;
}
