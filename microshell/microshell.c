#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>

#include <memory.h>

#include <sys/stat.h>

#include <sys/types.h>

#include <errno.h>

#include <sys/wait.h>

#include <utime.h>

void touch(char ** arg) {
  FILE * plik;
  int pomoc = 1;
  if (arg[pomoc] == NULL) {
    printf("Podaj nazwe pliku");              /*touch*/
  } else {
    while (arg[pomoc] != NULL) {
      if (fopen(arg[pomoc], "a+") != NULL) {
        plik = fopen(arg[pomoc], "a+");
        fclose(plik);
        utime(arg[pomoc], NULL);

      } else {
        perror("Error");
      }
      pomoc++;
    }
  }
}

void wc(char ** arg) {
  FILE * plik;
  int pomoc = 1, total_l = 0, total_w = 0, total_s = 0;
  if (arg[pomoc] == NULL) {
    printf("Podaj nazwe pliku");
  } else {
    while (arg[pomoc] != NULL) {
      int lines = 0, words = 0, spacja = 0;
      char ch;
      struct stat st;
      stat(arg[pomoc], & st);
      size_t size = st.st_size;                         /*wc*/
      if (fopen(arg[pomoc], "r+") != NULL) {
        plik = fopen(arg[pomoc], "r");
        while (!feof(plik)) {
          ch = fgetc(plik);

          if (ch == '\n') {
            lines++;
          }
          if (ch == ' ') {
            spacja = 1;
          }
          if (ch != ' ' && spacja == 1) {
            words++;
            spacja = 0;
          }
        }

        fclose(plik);
        printf("%d %d %ld %s\n", lines, words, size, arg[pomoc]);
        total_l = total_l + lines;
        total_w = total_w + words;
        total_s = total_s + size;

      } else {
        perror(arg[pomoc]);
      }
      pomoc++;

    }
    if (pomoc > 2) {
      printf("%d %d %d TOTAL", total_l, total_w, total_s);
    }
  }
}

void cd(char ** arg) {
  char oldcwd[1024];
  if (arg[1] == NULL || !strcmp(arg[1], "~")) {
    getcwd(oldcwd, sizeof(oldcwd));
    chdir("/home");
    chdir(getenv("LOGNAME"));
  } else if (!strcmp(arg[1], "-")) {                  /*cd*/
    chdir(oldcwd);
  } else {
    getcwd(oldcwd, sizeof(oldcwd));
    if (chdir(arg[1]) != 0) {
      perror("podany katalog nie istnieje");
    }
  }
}
void moo(char **arg)
{
  int pomoc=1;
  printf("        (__)\n"); 
	printf("        (oo)\n"); 	   
	printf("  /------\\/\n");       /*moo*/
	printf(" / |    ||\n");     
	printf("* /\\----/\\ \n"); 
	printf("  ~~    ~~ \n");
  printf("----- ");
  if(arg[pomoc]==NULL)
  {
    printf("twoj tekst ");
  }else
  {
    while (arg[pomoc] != NULL)
    {
      printf("%s ",arg[pomoc]);
      pomoc++;
    }
  }
  printf("-----");
}
void other(char ** arg, int err) {
  pid_t id = fork();
  if (id == 0) {
    execvp(arg[0], arg);                        /*exec+fork()*/
    err = errno;
    if (err != 0) {
      perror("Podana komenda jest niepoprawna");
      printf("Error number: %d", err);
      exit(0);
    }
  } else {
    wait(NULL);
  }
}

int main(int argc, char * argv[]) {
  char cwd[1024];

  while (1) {
    getcwd(cwd, sizeof(cwd));
    printf("\033[0;32m");
    printf("%s", getenv("LOGNAME"));        /*kolorowanie skladni*/
    printf("\033[0;34m");
    printf("[%s]\n", cwd);
    printf("\033[0m");
    printf("$ ");

    char ** arg;
    arg = (char ** ) malloc(1024 * sizeof(char));
    char str[1024];
    int a = 0, err = 0;
    char * line;
    char * token;
    line = fgets(str, 1024, stdin);
    token = strtok(line, " \n\t");
    arg[a] = token;
    while ((token = strtok(NULL, " \n\t")) != NULL) {
      a++;
      arg[a] = token;
    }
    arg[a + 1] = NULL;
    if (!strcmp(arg[0], "exit")) {                      /*exit*/
      exit(0);
    } else if (!strcmp(arg[0], "help")) {             /*help*/
      printf("%s", "Autor: Michal Gulczynski\nFunkcje:\n-cd\n-exit\n-help\n-polecenia opisane w PATH\n-touch\n-wc\nDodatki:\n-obsluga kolorow\n-wyswietlanie loginu\n-rozbudowane cd\n-moo");
    } else if (!strcmp(arg[0], "touch")) {
      touch(arg);
    } else if (!strcmp(arg[0], "wc")) {
      wc(arg);
    } else if (!strcmp(arg[0], "cd")) {
      cd(arg);
    }else if (!strcmp(arg[0], "moo")) {
      moo(arg);
    }else {
      other(arg, err);
    }
    fflush(stdin);
    printf("\n");
  }
  return 0;
}