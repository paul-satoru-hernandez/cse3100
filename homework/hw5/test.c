int message_score(const char *msg)
{
    int score = 0;
    const char s[2] = " ";
    char *token;
    char *copy = malloc(strlen(msg));
    strcpy(copy, msg);
    token = strtok(copy, s);

    while (token != NULL){
        if (in_dict(token)) score++;
        token = strtok(NULL, s);
    }

    //free(token);
    free(copy);
    return score;
}

int main(void){
    

    return 0;
}