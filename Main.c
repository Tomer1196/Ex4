#define _CRT_SECURE_NO_WARNINGS
#define _OPEN_SYS_ITOA_EXT

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>*/ //uncomment this block to check for heap memory allocation leaks.
// Read https://docs.microsoft.com/en-us/visualstudio/debugger/finding-memory-leaks-using-the-crt-library?view=vs-2019
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct StudentCourseGrade
{
	char courseName[35];
	int grade;
}StudentCourseGrade;

typedef struct Student
{
	char name[35];
	StudentCourseGrade* grades; //dynamic array of courses
	int numberOfCourses;
}Student;


//Part A
char** split(char* string, const char* object, int coursesPerStudent);
void countStudentsAndCourses(const char* fileName, int** coursesPerStudent, int* numberOfStudents);
int countPipes(const char* lineBuffer, int maxCount);
char*** makeStudentArrayFromFile(const char* fileName, int** coursesPerStudent, int* numberOfStudents);
void printStudentArray(const char* const* const* students, const int* coursesPerStudent, int numberOfStudents);
void factorGivenCourse(char** const* students, const int* coursesPerStudent, int numberOfStudents, const char* courseName, int factor);
void studentsToFile(char*** students, int* coursesPerStudent, int numberOfStudents);

//Part B
Student* transformStudentArray(char*** students, const int* coursesPerStudent, int numberOfStudents);
void writeToBinFile(const char* fileName, Student* students, int numberOfStudents);
Student* readFromBinFile(const char* fileName);

int main()
{
	//Part A
	int* coursesPerStudent = NULL;
	int numberOfStudents = 0;
	char*** students = makeStudentArrayFromFile("studentList.txt", &coursesPerStudent, &numberOfStudents);

	//printStudentArray(students, coursesPerStudent, numberOfStudents);
	//factorGivenCourse(students, coursesPerStudent, numberOfStudents, "Complex Functions", +7);
	//printStudentArray(students, coursesPerStudent, numberOfStudents);
	studentsToFile(students, coursesPerStudent, numberOfStudents); //this frees all memory. Part B fails if this line runs. uncomment for testing (and comment out Part B)

	//test of func 1
	/*int amount = 0;
	const char* str = "| dont care | love |t";
	unsigned int maxLength = 0;
	maxLength = strlen(str);
	amount = countPipes(str, maxLength);
	printf("The symbol '|' appears %d times in this string\n", amount);*/

	//test of func 2
	/*countStudentsAndCourses("studentList.txt", &coursesPerStudent, &numberOfStudents);
	printf("There are %d students in the file\n", numberOfStudents);
	printf("The courses per student are:\n");
	for (int i = 0; i < 9; i++) {
		printf("%d   ", coursesPerStudent[i]);
	}*/

	//test of func 3
	/*int i = 0, j = 0, k = 0;
	int*** studentsARR = makeStudentArrayFromFile("studentList.txt", &coursesPerStudent, &numberOfStudents);
	printf("%s", studentsARR);
	printStudentArray(&studentsARR, &coursesPerStudent, &numberOfStudents);
	printf("There are %d students in the file\n", numberOfStudents);
	printf("The courses per student are:\n");
	for (int i = 0; i < 9; i++) {
		printf("%d   ", coursesPerStudent[i]);*/



		//Part B
		/*Student* transformedStudents = transformStudentArray(students, coursesPerStudent, numberOfStudents);
	writeToBinFile("students.bin", transformedStudents, numberOfStudents);
	Student* testReadStudents = readFromBinFile("students.bin");*/

	//add code to free all arrays of struct Student


	/*_CrtDumpMemoryLeaks();*/ //uncomment this block to check for heap memory allocation leaks.
	// Read https://docs.microsoft.com/en-us/visualstudio/debugger/finding-memory-leaks-using-the-crt-library?view=vs-2019

	return 0;
}

void countStudentsAndCourses(const char* fileName, int** coursesPerStudent, int* numberOfStudents)
{
	FILE* studentList;
	char text[1023];
	int counterOfStudents = 0; //Creating intiger for counting
	int* pointer = 0;
	studentList = fopen(fileName, "r"); //Set the file path
	if (!studentList) {
		printf("Error opening file\n");
		exit(1);
	}
	while (!feof(studentList)) {
		fgets(text, 1023, studentList);
		if (!counterOfStudents) {
			pointer = (int*)malloc((counterOfStudents + 1) * sizeof(int*));
		}
		else {
			pointer = realloc(pointer, (counterOfStudents + 1) * sizeof(int*));
		}
		if (!pointer) {
			printf("Memory allocation faild\n");
			exit(1);
		}
		pointer[counterOfStudents] = countPipes(text, 1023);
		*numberOfStudents = counterOfStudents + 1;
		counterOfStudents++;
	}
	*coursesPerStudent = pointer; //Set the number of the pipes in the pointer
	fclose(studentList);
}

int countPipes(const char* lineBuffer, int maxCount)
{
	int count = 0;
	if (maxCount <= 0)
		return 0;
	if (lineBuffer == NULL)
		return -1;
	for (int i = 0; i < strlen(lineBuffer); i++) {
		if (lineBuffer[i] == '|')
			count++;
		else
			continue;
	}
	return count;
}

char** split(char* string, const char* object, int coursesPerStudent) {
	int i = 0;
	char** stArray = (char**)malloc(coursesPerStudent * sizeof(char*));

	if (!stArray) {
		printf("Memory allocation failed\n");
		exit(1);
	}
	char* token = strtok(string, object);
	while (token) {
		stArray[i] = (char*)malloc(strlen(token) + 1);
		if (!stArray[i]) {
			printf("Memory allocation failed\n");
			exit(1);
		}
		strcpy(stArray[i], token);
		token = strtok(NULL, object);
		i++;
	}
	return stArray;
}


char*** makeStudentArrayFromFile(const char* fileName, int** coursesPerStudent, int* numberOfStudents)
{
	countStudentsAndCourses(fileName, coursesPerStudent, numberOfStudents);
	int i = 0, j = 0, k = 0, p = 1;
	const int tempNum = *numberOfStudents;
	const char CommaVertical[] = ",|";
	int studentCounter = 0;
	char*** Students = (char***)malloc(sizeof(char*) * tempNum);
	for (i = 0; i < tempNum; i++)
		Students[i] = (char*)malloc(sizeof(char*) * (((*(*coursesPerStudent + i)) * 2) + 1));

	FILE* fp;
	fp = fopen(fileName, "r");
	if (!fp) {
		printf("Error opening file\n");
		exit(1);
	}
	char tempBuffer[1023];

	while (fgets(tempBuffer, 1023, fp)) {
		char** arr = split(tempBuffer, "|", *(*coursesPerStudent + studentCounter) + 1);
		if (!arr) exit(1);
		Students[studentCounter][0] = arr[0];
		for (int i = 1; i <= *(*coursesPerStudent + studentCounter); i++) {
			char** courses = split(arr[i], ",", 2);
			int j = i * 2 - 1;
			Students[studentCounter][j] = courses[0];
			Students[studentCounter][j + 1] = courses[1];
		}
		studentCounter++;
	}
	fclose(fp);
	//printStudentArray(Students, *coursesPerStudent, *numberOfStudents);
	/*for (int i = 0; i < tempNum; i++) {
		for (int k = 0; k < *(*coursesPerStudent+k) * 2 + 1; k++)
			free(Students[i][k]);
		free(Students[i]);
	}
	free(Students);*/
	return Students;
}


void factorGivenCourse(char** const* students, const int* coursesPerStudent, int numberOfStudents, const char* courseName, int factor)
{
	char pointer[20] ="";
	int grade = 0,k=0;
	if ((-20)>factor||factor>(20))
	{
		printf("factor is not in the limit size");
		exit(1);
	}
	for (int i = 0; i < numberOfStudents; i++)
	{
		for (int j = 0; j < coursesPerStudent[i]*2; j++)
		{
			if (!strcmp(students[i][j], courseName))
			{
				j++;
				grade = atof(students[i][j]);
				if ((grade+factor>100)||(grade + factor<0))
				{
					if (grade + factor > 100) {
						grade == 100;
					}
					else {
						grade == 0;
					}

				}
				else
				{
					grade += factor;
				}
				_itoa(grade, pointer, 10 );
				students[i][j] = pointer;
			}
		}
	}
}

void printStudentArray(const char* const* const* students, const int* coursesPerStudent, int numberOfStudents)
{
	for (int i = 0; i < numberOfStudents; i++)
	{
		printf("name: %s\n*********\n", students[i][0]);
		for (int j = 1; j <= 2 * coursesPerStudent[i]; j += 2)
		{
			printf("course: %s\n", students[i][j]);
			printf("grade: %s\n", students[i][j + 1]);
		}
		printf("\n");
	}
}

void studentsToFile(char*** students, int* coursesPerStudent, int numberOfStudents)
{
	FILE* fp;
	fp = fopen("StudentFile.txt","w");
	if (fp == NULL)
	{
		printf("Error opening file");
		exit(1);
	}
	for (int i = 0; i < numberOfStudents; i++)
	{
		for (int j = 0; j < coursesPerStudent[i] * 2; j++)
		{
			if (j == 0) {
				fputs(students[i][j], fp);
				fputs("\n",fp);
				j++;
			}
			fputs(students[i][j], fp);
			fputs(" - ", fp);
			j++;
			fputs(students[i][j], fp);
			fputs(" \n ", fp);
		}
		fputs("\n", fp);
	}
	fclose(fp);
	for (int i = 0; i < numberOfStudents; i++) {
		for (int k = 0; k < coursesPerStudent[i] * 2; k++)
			free(students[i][k]);
		free(students[i]);
	}
	free(students); 
}

void writeToBinFile(const char* fileName, Student* students, int numberOfStudents)
{
	//add code here
}

Student* readFromBinFile(const char* fileName)
{
	//add code here
}

Student* transformStudentArray(char*** students, const int* coursesPerStudent, int numberOfStudents)
{
	//add code here
}
