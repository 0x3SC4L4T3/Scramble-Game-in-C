#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


typedef struct node_s {
	char word[15];
	int point; //points assigned for each word
	struct node_s* next;
} wordnode;


bool validateExist(wordnode* list, char* word) {
    wordnode* itr = list;
    while (itr) {
        if (strcmp(itr->word, word) == 0) {
            return true;
        }
        itr = itr->next;
    }
    return false;
}

void insertWord(wordnode** listofWords, wordnode* newWord) {
    if (*listofWords == NULL)
    {
        *listofWords = newWord;
        return;
    }

    wordnode* last = *listofWords;
    while (last->next != NULL) {
        last = last->next;
    }
    last->next = newWord;
    return;
}

void del(wordnode* before_del)
{
    wordnode* temp;
    temp = before_del->next;
    before_del->next = temp->next;
    free(temp);
}

void deleteWord(wordnode** listofWords, char* word) {
    if (strcmp((*listofWords)->word, word) == 0) {
        *listofWords = (*listofWords)->next;
    }
    else {
        wordnode* itr = *listofWords;
        while (itr->next) {
            if (strcmp(itr->next->word, word) == 0) {
                break;
            }
            itr = itr->next;
        }
        del(itr);
    }
}

void printListofWords(wordnode* listofWords) {
    wordnode* itr = listofWords;
    while (itr) {
        printf("%s ", itr->word);
        itr = itr->next;
    }
}

int getSize(wordnode *listofWords) {
    int size = 0;
    wordnode* itr = listofWords;
    while (itr) {
        size++;
        itr = itr->next;
    }
    return size;
}

void readFileRandomly(wordnode** listofWords) {
    char fileName[256];
    int num = (rand() % (9 - 0 + 1)) + 0;
    char buf[10];
    sprintf(buf, "%ld", num);
    strcpy(fileName, "WordSet");
    strcat(fileName, buf);
    strcat(fileName, ".txt");
    FILE* fp = fopen(fileName, "r");
    if (fp == NULL) {
        printf("Unable to open file, %s\n" , fileName);
        return;
    }
    printf("Use %s\nLets play Word Scramble\n", fileName);
    char line[20]; 
    int point;
    while (!feof(fp)) {
        fgets(line, 20, fp);
            if (strcmp(line, "") == 0 || strcmp(line, "\n") == 0 || strcmp(line, "\r") == 0) {
                break;
            }
        char* word = strtok(line, " ");
        point = atoi(strtok(NULL, line));

        wordnode* newWord = (wordnode*)malloc(sizeof(wordnode));
        newWord->next = NULL;
        newWord->point = point;
        strcpy(newWord->word, word);
        insertWord(&(*listofWords), newWord);
    }
}


char* swapLetters(char* word) {

    int length = strlen(word);
    char* swappedWord = (char*)malloc(sizeof(char) * length);
    strcpy(swappedWord, word);
    for (int i = 0; i < length; i++) {
        int idx1 = (rand() % ((length - 1) - 0 + 1)) + 0;
        int idx2 = (rand() % ((length - 1) - 0 + 1)) + 0;
        char temp = swappedWord[idx1];
        swappedWord[idx1] = swappedWord[idx2];
        swappedWord[idx2] = temp;
    }

    return swappedWord;
}

void printSrambled(wordnode* listofWords) {
    char* word = swapLetters(listofWords->word);
    for (int i = 0; i < strlen(word); i++) {
        printf("%c ", word[i]);
    }
}

void printNotSrambled(wordnode* listofWords) {
    char* word = listofWords->word;
    for (int i = 0; i < strlen(word); i++) {
        printf("%c ", word[i]);
    }
}

void printGuesses(wordnode* listofWords) {
    int length = getSize(listofWords);
    int wordlength = 0;
    char* word = NULL;
    if (length < 5) {
        printf("%d clues to help you guess\n", length);
        wordnode* itr = listofWords;
        while (itr && length != 0) {
            word = (char*)malloc(sizeof(char) * strlen(itr->word));
            strcpy(word,itr->word);
            wordlength = strlen(word);
            int idx = (rand() % ((wordlength - 1) - 0 + 1)) + 0;
            word[idx] = ' ';
            printf(word);
            itr = itr->next;
            length--;
        }
    }
    else {
        int count = 0;
        printf("5 clues to help you guess\n");
        wordnode* itr = listofWords;
        while (itr && count <= 5) {
            word = (char*)malloc(sizeof(char) * strlen(itr->word));
            strcpy(word,itr->word);
            wordlength = strlen(word);
            int idx = (rand() % ((wordlength - 1) - 0 + 1)) + 0;
            for (int i = 0; i < wordlength; i++) {
                if (idx == i) {
                    printf("%c ", word[i]);
                }
                else {
                    printf("_ ");
                }
            }
            printf("\n");
            itr = itr->next;
            count++;
        }
    }

}

void printwords(wordnode* list1, wordnode* list2) {
    printf("Correct Guess\tWrong Guess\n");
    wordnode* itr1 = list1;
    wordnode* itr2 = list2;
    while (itr1 || itr2) {
        if (itr1) {
            printf("%s", itr1->word);
            itr1 = itr1->next;
        }
        printf("         ");
        if (itr2) {
            printf("%s", itr2->word);
            itr2 = itr2->next;
        }
        printf("\n");

    }
}

bool correctInput(char* buff) {
    if (strlen(buff) < 3 && strlen(buff) > 6){
        return false;
    }
    return true;
}

bool matchFound(wordnode* listofwords, char* word, int *points) {
    wordnode* itr = listofwords;
    while (itr) {
        if (strcmp(itr->word, word) == 0) {
            (*points) += itr->point;
            return true;
        }
        itr = itr->next;
    }
    return false;
}
void startGame() {
    int totalPoints = 0;
    wordnode* guessedWords = NULL;
    wordnode* wrongGuessedWords = NULL;
    wordnode* listofWords = NULL;

    readFileRandomly(&listofWords);

    char continuePlaying = 'y';
    char scramble = 'y';
    int c;
    while ((continuePlaying != 'n') || (continuePlaying != 'N')) {

        if (listofWords == NULL) {
            printf("Congratulations! You have won the game,\nYour total points are: %d\n", totalPoints);
            break;
        }


        printf("-- -- -- -- -- --\n");
        if (scramble == 'y' || scramble == 'Y') {
            printSrambled(listofWords);
        }
        else {
            printNotSrambled(listofWords);
        }
        
        printf("\n-- -- -- -- -- --\n");

        printf("There are %d words that you can guess\n", getSize(listofWords));
        printGuesses(listofWords);
        printwords(guessedWords, wrongGuessedWords);

        printf("You have now %d points\nwith %d correct guesses and %d wrong guessed\n", totalPoints, getSize(guessedWords), getSize(wrongGuessedWords));
        printf("Type your Guess [between 4 to 6 letters]: ");
        char input[100];
        fgets(input, 100, stdin);
        input[strlen(input) - 1] = '\0';
        while (!correctInput(input)) {
            printf("Type your Guess [between 4 to 6 letters]: ");
            fgets(input, 100, stdin);
            input[strlen(input) - 1] = '\0';
        }

        wordnode* newWord = (wordnode*)malloc(sizeof(wordnode));
        newWord->next = NULL;
        strcpy(newWord->word, input);
        if (matchFound(listofWords, input, &totalPoints)) {
            deleteWord(&listofWords, input);
            if(!validateExist(guessedWords, input)) {
                insertWord(&guessedWords, newWord);
                printf("Congrats, Match Found\n");
            }
            else {
                printf("Word already exists in guessed words list\n");
            }
        }
        else {
            if (!validateExist(wrongGuessedWords, input)) {
                insertWord(&wrongGuessedWords, newWord);
                printf("Sorry, Match Not Found\n");
            }
            else {
                printf("Word already exists in wrong guessed words list\n");
            }

        }

        printf("\n\n");

        printf("Continue playing (Q/q to Quit)?: ");
        continuePlaying = getc(stdin);

        if (continuePlaying == 'n' || continuePlaying == 'N') {
            break;
        }

        while ((c = getchar()) != EOF && c != '\n');

        printf("Want to scramble letters (Y/y for Yes)?: ");
        scramble = getc(stdin);

        while ((c = getchar()) != EOF && c != '\n');
    }

    printf("%d words that you have not guessed\n", getSize(listofWords));
    printListofWords(listofWords);
    printf("\n\nGood Bye\n");
}

int main() {
    srand(time(NULL));
    startGame();
	return 0;
}