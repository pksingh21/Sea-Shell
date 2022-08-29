#include "./headers.h"
#include "changeDir.h"
#include "clock.h"
#include "echo.h"
#include "env.h"
#include "ls.h"
#include "pinfo.h"
#include "piping.h"
#include "prompt.h"
#include "pwd.h"
#include "redirect.h"
#include "relativePath.h"
#include "split.h"
// #include <cstddef>
#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
int cin, zin, zpid, g_pid;

void contolC() {
  printf("we came at a control C function");
  cin = 1;
  kill(g_pid, 9);
  exit(0);
  return;
}

void contolZ() {
  zin = 1;
  kill(g_pid, SIGSTOP);
  exit(0);
  return;
}
void controlD() {
  kill(g_pid, SIGSTOP);
exit(0);
  return;
}
typedef struct {
  int PID;
  char Pname[100];
} Jobs;
Jobs jb[1000], temp[1000];
int pcnt = 0;
char *root;
int childpr[1000];
int childProcess = 0;
void perform(char **args) {
  int bg = -1;
  _Bool background = 0;
  int tct = 0;
  for (int i = 0; i < pcnt; i++) {
    if (kill(jb[i].PID, 0) != -1) {
      temp[tct] = jb[i];
      tct++;
    } else
      printf("%s with Pid %d exited.\n", jb[i].Pname, jb[i].PID);
  }
  pcnt = 0;
  for (int i = 0; i < tct; i++) {
    jb[pcnt++] = temp[i];
  }
  for (int i = 0; args[i] != NULL; i++)
    if (strcmp(args[i], "&") == 0) {
      bg = i;
      background = 1;
    }
  if (bg != -1)
    args[bg] = NULL;
  for (int i = 0; args[i] != NULL; i++) {
    if (strcmp(args[i], "|") == 0) {
      piping(args);
      return;
    }
  }
  for (int i = 0; args[i] != NULL; i++) {
    if (strcmp(args[i], "<") == 0 || strcmp(args[i], ">") == 0 ||
        strcmp(args[i], ">>") == 0) {
      HandleRedirection(args);
      return;
    }
  }
  if (strcmp(args[0], "cd") == 0 && !background && args[2] == NULL) {
    change_dir(args, root);
    return;
  }
  if (strcmp(args[0], "echo") == 0 && !background) {
    Echo(args);
    return;
  }
  if (strcmp(args[0], "pwd") == 0 && !background) {
    Print_present();
    return;
  }
  if (strcmp(args[0], "clock") == 0) {
    clk(args[1]);
    return;
  }
  if (strcmp(args[0], "quit") == 0 && !background) {
    for (int i = 0; i < pcnt; i++) {
      kill(jb[i].PID, 9);
    }
    exit(0);
    return;
  }
  if (strcmp(args[0], "pinfo") == 0 && !background) {
    process_info(args, root);
    return;
  }
  if (strcmp(args[0], "ls") == 0 && !background) {
    Listing(args);
    return;
  }
  if (strcmp(args[0], "setenv") == 0) {
    SetEnv(args);
    return;
  }
  if (strcmp(args[0], "unsetenv") == 0) {
    UnSetEnv(args);
    return;
  }
  if (strcmp(args[0], "remindme") == 0) {
    // Create a Child Process run sleep for t seconds then print reminder.
    pid_t pid = fork();
    if (pid == 0) {
      sleep(atoi(args[1]));
      printf("\nReminder: ");
      for (int i = 2; args[i] != NULL; i++)
        printf("%s ", args[i]);
      printf("\n");
    } else {
      // Do not wait for child to end
      signal(SIGCHLD, SIG_IGN);
    }
    return;
  }
  if (strcmp(args[0], "jobs") == 0) {
    char checker[400], str[100];
    for (int i = 0; i < pcnt; i++) {
      printf("[%d] ", i + 1);
      sprintf(str, "/proc/%d/status", jb[i].PID);
      FILE *fd;
      fd = fopen(str, "r");
      for (int it = 0; it < 400; it++)
        checker[i] = '\0';
      while (1) {
        fgets(checker, 100, fd);
        if (checker[0] == 'S')
          break;
      }
      for (int it = 8; checker[it] != '\0' && checker[it] != '\n'; it++)
        printf("%c", checker[it]);
      printf(" %s %d\n", jb[i].Pname, jb[i].PID);
    }
    return;
  }
  if (strcmp(args[0], "kjob") == 0) {
    if (atoi(args[1]) > pcnt) {
      printf("background process with job id %s doesnt exist.\n", args[1]);
      return;
    }
    if (args[2] != NULL && args[3] == NULL) {
      kill(jb[atoi(args[1]) - 1].PID, atoi(args[2]));
    } else
      printf("No of required Arguments is 3\n");
    return;
  }
  if (strcmp(args[0], "fg") == 0) {
    printf("%s\n", jb[atoi(args[1]) - 1].Pname);
    kill(jb[atoi(args[1]) - 1].PID, SIGCONT);
    if (atoi(args[1]) > pcnt) {
      printf("Process with job id %s doesnt exist\n", args[1]);
      return;
    } else {
      if (args[1] != NULL && args[2] == NULL) {
        while (1) {
          if (kill(jb[atoi(args[1]) - 1].PID, 0) == -1) {
            break;
          } else {
            cin = 0;
            zin = 0;
            g_pid = jb[atoi(args[1]) - 1].PID;
            int status = 0;
            while (waitpid(g_pid, &status, WUNTRACED) > 0) {
            }
            cin = 0;
            zin = 0;
          }
        }
      } else
        printf("There should be 3 arguments\n");
    }
    return;
  }
  if (strcmp(args[0], "bg") == 0) {
    if (atoi(args[1]) > pcnt) {
      printf("process with job id %s doesnt exist\n", args[1]);
      return;
    } else {
      if (args[1] != NULL && args[2] == NULL) {
        kill(jb[atoi(args[1]) - 1].PID, SIGCONT);
      } else
        printf("There should be 3 arguments\n");
    }
    return;
  }
  if (strcmp(args[0], "overkill") == 0) {
    for (int i = 0; i < pcnt; i++) {
      kill(jb[i].PID, 9);
    }
    return;
  }
  int i, x = 0;
  int status = 0;
  int pid = fork();
  if (pid < 0) {
    perror("ERROR ");
  }
  if (pid == 0) {
    if (background) {
      // Close stdin for child Process so it doesnt take input from terminal
      fclose(stdin);
      fopen("/dev/null", "r");
      int errno;
      setpgid(getpid(), pid);
      // Try to execute the Process
      if (execvp(args[0], args) == -1)
        printf("execvp failed: command not found\n");
    } else {
      int errno;
      if (execvp(args[0], args) == -1)
        printf("execvp failed: command not found\n");
    }
    exit(0);
  } else {
    signal(SIGCHLD, SIG_IGN);
    if (background) {
      printf("Starting Background Job %d\n", pid);
      jb[pcnt].PID = (int)pid;
      strcpy(jb[pcnt].Pname, args[0]);
      pcnt++;
    } else {
      zpid = pid;
      g_pid = pid;
      cin = 0;
      zin = 0;
      int status = 0;
      if (cin == 1) {
        kill(pid, 9);
      }
      while (waitpid(pid, &status, WUNTRACED) > 0) {
        if (cin == 1) {
          kill(pid, 9);
          break;
        }
        if (zin == 1) {
          jb[pcnt].PID = pid;
          strcpy(jb[pcnt].Pname, args[0]);
          pcnt++;
          break;
        }
        waitpid(pid, &status, 0);
      }
      cin = 0;
      zin = 0;
      // Wait if process has to be run in foreground
    }
  }
  return;
}

void ExecuteShell() {
  char *s = (char *)(malloc(10000));
  char *tk;
  int i;
  char **commands = malloc(10000 * (sizeof(char *)));
  signal(SIGINT, contolC);
  signal(SIGTSTP, contolZ);
  signal(EOF, controlD);
  printf("Welcome to Sea Shell! A minimalist shell written in C . To exit you "
         "can press Ctrl C or Ctrl Z\n");
  while (1) {
    printPromt(root);
    // fgets(s, 10000, stdin);
    s = readline("$ ");
    if (!strcmp(s, "^C"))
      return;
    ;
    if (!strcmp(s, "^D"))
      return;
    ;
    if (!strcmp(s, "^Z"))
      return;
    ;

    add_history(s);
    signal(SIGINT, contolC);
    signal(SIGTSTP, contolZ);
    if (strcmp(s, "ls")) {
      commands = getAll(s);
      for (i = 0; commands[i] != NULL; i++) {
        perform(Split(commands[i]));
      }
    } else {
      int status;
      char *args[2];

      args[0] = "/bin/ls"; // first arg is the full path to the executable
      args[1] = NULL;      // list of args must be NULL terminated

      if (fork() == 0)
        execv(args[0], args); // child: call execv with the path and the args
      else
        wait(&status);
    }
    rl_free(s);
    s = NULL;
  }
  if(s) rl_free(s);
}
int main(int arg1, char **arg2) {
  pid_t rootpid = getpid();
  setpgid(rootpid, rootpid);
  root = getenv("PWD");
  ExecuteShell();
  return 0;
}