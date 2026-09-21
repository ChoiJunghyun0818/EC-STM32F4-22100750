#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//structure 밑에 있는 fields들 
struct Student {
    char name[20]; // char: character
    int grade; // integral: 수
    int class;
    float average; // float: 소수
};

//main 코드의 시작
int main()
{
    struct Student *s1 = malloc(sizeof(struct Student));
    //malloc: memory allocation, gives you an empty space for you too fill with info
    //*s1: a pointer that allows things to be found within struct student.
    // for example, s1->name allows name to be found within the allocated Struct

    strcpy(s1->name, "고길동"); // strcpy: string copy.
    s1->grade = 1;  // grade is found within structure, and is 1
    s1->class = 3; // class is found within structure
    s1->average = 65.389999f;

    printf("이름: %s\n", s1->name);
    printf("학년: %d\n", s1->grade);
    printf("반: %d\n", s1->class);
    printf("평균점수: %f\n", s1->average); // print commands

    free(s1); // after using S1 you have to free it

    return 0;
}