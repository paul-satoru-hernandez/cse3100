#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <assert.h>

#define MAX 10240
#define MAX_WORD_COUNT 60000                //we have less than 60000 words
#define MAX_WORD_LENGTH 80                //each word is less than 80 letters

//Using these two global variables can be justified :)
char words[MAX_WORD_COUNT][MAX_WORD_LENGTH];        //2-d array to hold all the words
int word_count = 0;                    //number of words, initilized to 0

//Note the words in the dictionary file are sorted
//This fact could be useful
void read_file_to_array(char *filename)
{
    FILE *fp;

    //open the file for reading
    fp = fopen(filename, "r");
    if(fp==NULL)
    {
        printf("Cannot open file %s.\n", filename);
        exit(-1);
    }
    //make sure when each word is saved in the array words,
    //it does not ends with a '\n'
    //see how this is done using fscanf 
    while(!feof(fp))
        fscanf(fp, "%s\n", words[word_count++]);
    fclose(fp);
}

//TODO
//Test wether a string word is in the dictionary
//Return 1 if word is in the dictionary
//Return 0 otherwise
//Be efficient in implementing this function
//Efficiency is needed to pass test cases in limited time
int binary_search(char *word, int low, int high)
{
    if (low > high){
        return 0;
    }
    else{
        int mid = (low + high) / 2;
        if (!strcmp(word, words[mid])){
            return 1;
        }
        else if (strcmp(word, words[mid]) > 0){
            return binary_search(word, mid + 1, high);
        }
        else if (strcmp(word, words[mid]) < 0){
            return binary_search(word, low, mid-1);
        }
    }
    return -1;
}

int in_dict(char *word)
{
    return binary_search(word, 0, word_count);
}

//TODO
//Use key and shift to decrypt the encrypted message
//len is the length of the encrypted message
//Note the encrypted message is stored as an array of integers (not chars)
//The result is in decrypted

void decryption(unsigned char key, unsigned char shift, const int *encrypted, int len, char *decrypted)
{
    for (int i = 0; i < len; i++){
        decrypted[i] = (encrypted[i] ^ key) >> shift;
    }
}

//TODO
//calculate a score for a message msg
//the score is used to determine whether msg is the original message
int message_score(const char *msg)
{
    int score = 0;
    const char s[2] = " ";
    char *token;
    char copy[MAX_WORD_COUNT];
    strcpy(copy, msg);
    token = strtok(copy, s);

    while (token != NULL){
        if (in_dict(token)) score++;
        token = strtok(NULL, s);
    }
    return score;
}

//search using all the (key, shift) combinations
//to find the original message
//result is saved in message
void search(const int *encrypted, int len, char *message)
{
	char decrypted[MAX];

    int max_score = 0;
    strcpy(message, "");
    for(unsigned char k = 0; k < 255; k++)
    {
        for(unsigned char shift = 0; shift <= 24; shift++)
        {
            decryption(k, shift, encrypted, len, decrypted);
			int score = message_score(decrypted);
			if(score > max_score)
			{	
				max_score = score;
				strcpy(message, decrypted);
			}
        }
    }
}

//TODO
//read the encrypted message from the file to encrypted
//return number of bytes read
int read_encrypted(char *filename, int *encrypted)
{
    int bytes = 0;
    char a[4];

    FILE *fp;
    fp = fopen(filename, "r");
    if (fp == NULL){
        return -1;
    }
    while (!feof(fp)){
        for (int i = 0; i < 4; i++){
            a[i] = fgetc(fp);
        }
        encrypted[bytes] = *((int*)a);
        bytes++;
    }
    fclose(fp);
    return bytes - 1;
}

//Do not change the main() function
int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		printf("%s encrypted-message\n", argv[0]);
		return 0;
	}

	read_file_to_array("dict.txt");

	int encrypted[MAX];
	int len = read_encrypted(argv[1], encrypted);
	
	char message[MAX];
	strcpy(message, "");
	search(encrypted, len, message);
	printf("%s\n", message);
	return 0;
}
// int main(int argc, char*argv[])
// {
//     if (argc != 2)
//     {
//         printf("doing it wrong");
//         return 0;
//     }
//     read_file_to_array("dict.txt");

//     int id = in_dict("pineapple");
//     printf("in dictionary: %d\n", id);
//     return 0;
// }