#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#define NUM_QUESTIONS 10
#define NUM_OPTIONS 4
#define LINE_LENGTH 200
#define ROLL_NO_LENGTH 11
#define NAME_LENGTH 30
#define QUESTION_LENGTH 100
#define OPTION_LENGTH 20

#define RESET   "\033[0m"      // Reset color
#define RED     "\033[0;31m"   // Red color
#define GREEN   "\033[0;32m"   // Green color
#define REVERSE   "\033[7m"     // Reverse text style
#define BOLD      "\033[1m"     // Bold text style
#define YELLOW  "\033[0;33m"   // Yellow color
#define BLINK     "\033[5m"     // Blink text style
#define BLUE    "\033[0;34m"   // Blue color
#define CYAN    "\033[0;36m"   // Cyan color

typedef struct {
    char *question;
    char **options;
    int answer;
} QUESTIONS;

void free_memory(QUESTIONS *Q);
void print_dots(int num);
int print_questions(FILE *fp, QUESTIONS *Q);
QUESTIONS memory_allocation_for_structure(QUESTIONS *Q);
void score_greetings(int score);
void file_check(FILE *fp);
int find_student(char* requested_roll_num);

void main() {
    int i=0;

    char* requested_roll_num = malloc(ROLL_NO_LENGTH*sizeof(char));
    QUESTIONS *Q = malloc(NUM_QUESTIONS * sizeof(QUESTIONS));
    memory_allocation_for_structure(Q);

    while(1){
       system("cls");
        printf(CYAN);
        printf("\n\t\t   +-------+                          ___ ___       ________   ");
		printf("\n\t\t   |       |      |         |            |                /    ");
		printf("\n\t\t   |       |      |         |            |               /     ");
		printf("\n\t\t   |       |      |         |            |              /      ");
		printf("\n\t\t   |    // |      |         |            |             /       ");
		printf("\n\t\t   |   //  |      |         |            |            /        ");
		printf("\n\t\t   +--//---+      |_________|         ___|___        /_______ \n"RESET);
        printf("\t\t\tTHIS GAME IS BUILT FOR CYBER SECURITY STUDENTS\n");
		printf("\n\t\t   RULES OF THE GAME   :     \n\n");
        printf("\t\t   1) YOUR ROLL NUMBER WILL BE VERIFIED BEFORE ENTERING THE QUIZ\n");
        printf("\t\t   2) 1 MARKS WILL BE AWARDED FOR CORRECT     \n");
        printf("\t\t   3) NO MARKS WILL AWARDED FOR INCORRECT     \n");
        printf("\t\t   4) SCORE CARD WILL BE GIVEN AT END         \n\n"); // quiz intro

        printf(BLUE"\tPLEASE ENTER YOUR ROLL NUMBER : ");
        printf(CYAN);
        scanf("%s", requested_roll_num);                   ///requesting roll_no.
        printf(RESET);

        int found = find_student(requested_roll_num);

        if (found) {
            int choice;
            printf("\t\tENTER THE TOPIC YOU ARE INTERESTED IN....\n\n\n");
            printf(CYAN"1.CLANGUAGE\n\n2.GENERAL_KNOWLEDGE\n\n3.CRICKET\n\n\t\t"RESET);
            scanf("%d", &choice);
            FILE *fp;
            switch(choice){
                case 1:
                        system("cls");
                        fp = fopen("c_questions.csv", "r");
                        file_check(fp);
                        printf(REVERSE"HERE ARE SOME C LANGUAGE BASIC QUESTIONS !!\n\n"RESET);
                        break;

                case 2:
                        system ("cls");
                        fp = fopen("GK_questions.csv", "r");
                        file_check(fp);
                        printf(REVERSE"HERE ARE SOME GENERAL KNOWLEDGE QUESTIONS!!\n\n"RESET);
                        break;

                case 3:
                        system("cls");
                        fp = fopen("cricket_questions.csv", "r");
                        file_check(fp);
                        printf(REVERSE"HERE ARE SOME CRICKET QUESTIONS !!\n\n"RESET);
                        break;

                default: printf("Invalid choice.\n");
                         printf("Enter valid option.");
                         print_dots(5);
            }
            int score = print_questions(fp,Q);
            score_greetings(score);
        }
        else{
             system("cls");
            printf(RED"\t\t\t\nTHERE IS NO STUDENT WITH THAT ROLL NUMBER\n");
            printf("\n\t\t\nEnter correct no."RESET);
            print_dots(2);
            main();
        }
        printf("Play Again.");
        print_dots(5);
    i++;
   };

}

QUESTIONS memory_allocation_for_structure(QUESTIONS *Q){

        for (int i = 0; i < NUM_QUESTIONS; i++) {
            Q[i].question = malloc(sizeof(char) * QUESTION_LENGTH);
            Q[i].options = malloc(sizeof(char*) * NUM_OPTIONS);
        for (int j = 0; j < NUM_OPTIONS; j++) {
                Q[i].options[j] = malloc(sizeof(char) * OPTION_LENGTH);
            }
        }
}

int find_student(char *requested_roll_num){
        int found=0;
        char* roll_num_str = malloc(ROLL_NO_LENGTH * sizeof(char));                  ///declarations & allocating dynamic memory;
        char* line = (char*)calloc(LINE_LENGTH, sizeof(char));
        char* name = (char*)calloc(NAME_LENGTH, sizeof(char));

        FILE *fp;
        fp = fopen("students.csv", "r");                   ///opening file
        file_check(fp);

        requested_roll_num[5]= toupper(requested_roll_num[5]);
        while (fgets(line, LINE_LENGTH , fp) != NULL) {

            if (sscanf(line, "%[^,],%[^\n]", roll_num_str, name) == 2) {

                    int name_length = strlen(name);
                    name = realloc(name,name_length*sizeof(char));      ///reallocating

                if (strcmp(roll_num_str, requested_roll_num) == 0) {
                    found++;
                    system("cls");
                    printf(YELLOW"\n\n\t\t\tWelcome !! %s!!\n\n\n"RESET, name);
                    break;
                }
            }
        }
        free(line);
        free(name);
        free(roll_num_str);
        free(requested_roll_num);
        fclose(fp);
    return found;
}


void file_check(FILE *fp){
    if (fp == NULL) {
        printf("Failed to open the file.\n");
    }
}


void free_memory(QUESTIONS *Q){

    for(int i=0;i<NUM_QUESTIONS;i++){
        free(Q[i].question);
        for(int j=0;j<NUM_OPTIONS;j++){
            free(Q[i].options[j]);
        }
    }
     free(Q);
}


int print_questions(FILE *fp, QUESTIONS *Q){
    int i = 0, score = 0, ans;
    char* buffer_line = malloc(sizeof(char) * LINE_LENGTH);
    while (fgets(buffer_line, 200, fp) != NULL) {
        sscanf(buffer_line, "%[^,],%[^,],%[^,],%[^,],%[^,],%d",
            Q[i].question, Q[i].options[0], Q[i].options[1],
            Q[i].options[2], Q[i].options[3], &Q[i].answer);
   // }
    //reallocate_memory(Q);
/*for(int i=0;i<NUM_QUESTIONS;i++){
            Q[i].question = realloc(Q[i].question,strlen(Q[i].question)*sizeof(char));
            //Q[i].option[i] = realloc(Q[i].question,strlen(Q[i].option)*sizeof(char));
        for(int j=0;j<NUM_OPTIONS;j++){
            Q[i].options[i][j]=realloc(Q[i].options[i][j],strlen(Q[i].options[i][j])*sizeof(char));
        }
    }*/

    //for(int i=0;i<NUM_QUESTIONS;i++){
        printf(BOLD"%d. %s\n\n"RESET, i + 1, Q[i].question);
        printf("\t1. %s\n", Q[i].options[0]);
        printf("\t2. %s\n", Q[i].options[1]);
        printf("\t3. %s\n", Q[i].options[2]);
        printf("\t4. %s\n", Q[i].options[3]);
        printf(BLINK"Choose an option: "RESET);
        scanf("%d", &ans);

       if (ans == Q[i].answer) {
            score++;
            printf(GREEN"Correct\n\n"RESET);
        } else {
            printf(RED"Wrong\n\n"RESET);
        }i++;
    }

    printf(BLUE"Your score : " YELLOW "%d/%d\n" RESET, score,NUM_QUESTIONS);
    sleep(1);
    fclose(fp);
    free(buffer_line);
    free(Q);
    return score;
}

void print_dots(int num){
    for(int i=0;i<num;i++){
        sleep(1);
        printf(".");
    }
}

void score_greetings(int score){
     system("cls");
     printf("\n\n\n");
     if(score>5){
            printf(CYAN"YOU PLAYED WELL!! YOUR SCORE IS PRETTY GOOD.\n\n\n"RESET);
            sleep(2);
    }else{
            printf(RED"YOU SCORED VERY LESS!\n\nNO WORRIES!\n\nBETTER LUCK NEXT TIME!!\n\n"RESET);
            sleep(2);
        }
}




