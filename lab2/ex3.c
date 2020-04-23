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
char **tokenCommands (int maxComNum, int maxComSize, int *readComNum, char *buffer);
void freeStrCom(char **tokenStrCom, int size);
int run_command (char *buffer, int **fd, int k, int readComNum);

int main () {
  char *buffer = NULL;
  int maxTokenNum = 10, maxTokenSize = 20, maxComNum = 10, readComNum = 0;
  char **strCom = NULL;
  printf("GENIE > ");
  int readBytes = -1;
  while ((readBytes = getline(&buffer, &maxTokenSize, stdin)) != -1) {
    strCom = tokenCommands (maxComNum, maxTokenSize*maxTokenNum+100, &readComNum, buffer);
    int fd[readComNum-1][2];
    int found = 0;
    if (readComNum == 1) {
      found = run_command(strCom[0], fd, -1, readComNum);
      if (found == 1) return 0;
    }
    else {
      for (int i = 0; i < readComNum-1; i++) {
        if (pipe(fd[i]) == -1) {
          printf ("cannot open pipe!\n");
          return -1;
        }
      }
      for (int k = 0; k < readComNum; k++) {
        found = run_command(strCom[k], fd, k, readComNum);
        if (found == -1) break;
        if (found == 1) return 0;
      }
    }
    freeStrCom(strCom, readComNum);
    printf ("\nGENIE < ");
  }
}


int run_command (char *buffer, int **fd, int k, int readComNum) {
  char ** strArr = NULL;
  int readTokenNum = 0;
  int maxTokenSize = 20, maxTokenNum= 10;
  if(strcmp(buffer, "quit\n") == 0) {
    printf("Goodbye!\n");
    return 1;
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
    return -1;
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
      stdin = 0; stdout = 0;
      printf ("\nstdin stdout: %d %d \n", stdin, stdout);
      if (k == 0) dup2(fd[k][1], stdout);
      else if (k == readComNum-1)
      {
        dup2(fd[k-1][0], stdin);
        close(fd[k-1][1]);
      }
      else {
        dup2(fd[k-1][0], stdin);
        dup2(fd[k][1], stdout);
      }
      execv(instrc, arg);
    }
  }
  freeTokenArray(strArr, readTokenNum);
  strArr = NULL;
  return 0;
}

char** tokenCommands (int maxComNum, int maxComSize, int *readComNum, char *buffer) {
  char **tokenStrCom;
  char *token;
  int i;

  tokenStrCom = (char**)malloc(sizeof(char*)*maxComNum);
  for (int i = 0; i < maxComNum; i++) {
    tokenStrCom[i] = NULL;
  }

  token = strtok(buffer, "|");

  i = 0;
  while (i < maxComNum && (token != NULL)) {
    tokenStrCom[i] = (char*)malloc(sizeof(char)*maxComSize);

    if (i == 0) {
      strncpy(tokenStrCom[i], token, maxComSize-1);
    } else {
      strncpy(tokenStrCom[i], token+1, maxComSize-1);
    }
    tokenStrCom[i][strlen(tokenStrCom[i])-1]='\n';
    tokenStrCom[i][maxComSize-1] = '\0';
    i++;
    token = strtok(NULL, "|");
  }
  *readComNum = i;
  return tokenStrCom;
}

char ** readTokens(int maxTokenNum, int maxTokenSize, int *readTokenNum, char *buffer) {
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

void TokenArg (int maxTokenNum, int maxTokenSize, int *readTokenNum, char *buffer, char **tokenStrArr) {
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

void freeStrCom (char **tokenStrCom, int size) {
  int i = 0;

   for (i = 0; i < size; i ++) {
     if (tokenStrCom[i] != NULL) {
       free(tokenStrCom[i]);
       tokenStrCom[i] = NULL;
     }
   }
   free(tokenStrCom);
}

