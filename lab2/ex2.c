#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

char **readTokens (int maxTokenNum, int maxTokenSize, int *readTokenNum, char *buffer);
void freeTokenArray(char **tokenStrArr, int size);

int main () {
  char *buffer = NULL;
  int maxTokenNum = 10, maxTokenSize = 20, readTokenNum = 0;
  char **strArr = NULL;
  printf("GENIE > ");
  int readBytes = -1;
  while ((readBytes = getline(&buffer, &maxTokenSize, stdin)) != -1) {
    if(strcmp(buffer, "quit\n") == 0) {
      printf("Goodbye!\n");
      return 0;
    }
    strArr = readTokens(maxTokenNum, maxTokenSize, &readTokenNum, buffer);
    struct stat st;
    char instrc[30];
    sprintf(instrc, "%s", strArr[0]);
    if (strArr[0][0] != '/') {
      sprintf(instrc, "/bin/%s", strArr[0]);
    }
    int invalid = -1;
    if ((invalid = stat(instrc, &st)) == -1) {
      printf("%s not found\n", strArr[0]);
    } else {
      int childPid = fork();
      if (childPid != 0) {
        wait(NULL);
      } else {
        char *arg[readTokenNum+1];
        for (int i = 0; i < readTokenNum; i++) {
          arg[i] = strArr[i];
        }
        arg[readTokenNum] = NULL;
        execv(instrc, arg);
      }
    }
    freeTokenArray(strArr, readTokenNum);
    strArr = NULL;
    printf("\nGENIE > ");
  }
}

char ** readTokens(int maxTokenNum, int maxTokenSize, int *readTokenNum, char *buffer)
  char **tokenStrArr;
  char *token;
  int i ;

  tokenStrArr = (char**)malloc(sizeof(char*)*maxTokenNum);

  for(int i = 0; i < maxTokenNum; i++) {
    tokenStrArr[i] = (char*)malloc(sizeof(char)*maxTokenSize);
  }

  token = strtok(buffer, " \n");

  i = 0;
  while(i < maxTokenNum && (token != NULL)) {

    strncpy(tokenStrArr[i], token, maxTokenSize-1);

    tokenStrArr[i][maxTokenSize-1] = '\0';

    if(strcmp(tokenStrArr[i],"echo") == 0) {
      TokenArg(maxTokenNum, maxTokenSize, readTokenNum, buffer, tokenStrArr);
      return tokenStrArr;
    }

    i++;
    token = strtok(NULL, " \n");
  }

  *readTokenNum = i;
  return tokenStrArr;
}

void TokenArg (int maxTokenNum, int maxTokenSize, int *readTokenNum, char *buffer, char
  int i = 1;
  bool quoting =  false;
  char token[20] = "\0";
  int len = 0;

  for (int j = 5; buffer[j] != '\n' && i < maxTokenNum; j++) {
    if (quoting == false && buffer[j] == ' ') {
      token[len] = '\0';
      strncpy(tokenStrArr[i], token, maxTokenSize-1);
      i++;
      len = 0;
    } else if (quoting == false && buffer[j] == 34){
      quoting = true;
    } else if (quoting == true && buffer[j] == 34) {
      quoting = false;
    } else {
      token[len] = buffer[j];
      len++;
    }
  }

  token[len] = '\0';
  strncpy(tokenStrArr[i], token, maxTokenSize-1);
  i++;
  *readTokenNum = i;
}

void freeTokenArray(char **tokenStrArr, int size) {
  int i = 0;

  for (i = 0; i < size; i++) {
    if (tokenStrArr[i] != NULL) {
      free(tokenStrArr[i]);
      tokenStrArr[i] = NULL;
    }
  }

  free(tokenStrArr);
}

