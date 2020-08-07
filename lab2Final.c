
#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sched.h>
#include <sys/types.h>
#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <sys/stat.h>


#define SIZEOFSTACK 1024*1024
#define BUFFER 1024
int status;
int checkbuiltin(char **cmds);
int takecommand();



void initShell()
{
  fprintf(stderr, "\nHey there, welcome to Matt and Jared's utility program lab\n");
}
void builtin_exit()
{
    fprintf(stderr, "\nExiting. Thanks for using our program!\n");
    exit(1);
    return;
}


void builtin_help()
{
  fprintf(stderr, "\nHere are the list of commands that are supported:");
  fprintf(stderr, "\nls");
  fprintf(stderr, "\nfind");
  fprintf(stderr, "\ngrep");
  fprintf(stderr, "\nmore");
  fprintf(stderr, "\nexit");

  return;
}


char *read_line(void)
{
  char *str = NULL;
  ssize_t buffersize = 0;
  getline(&str, &buffersize, stdin);
  if(str[strlen(str) - 1] == '\n')/*slightly messy way to remove newline character */
  {
    str[strlen(str) - 1] = 0;
  }
  return str;
}

char **parse_line(char *line)
{
  /*fprintf(stderr, "\n%s", line);  used for testing */
  int x = 0, y = 0;
  char **tokens = malloc(4048);
  char *token;
  token = strtok(line, " \t\n");
  while(token != NULL)
  {
    tokens[x] = token;
  /*  fprintf(stderr, "\n%d%s", x, tokens[x]);  used for testing */
    x++;
    token = strtok(NULL, " ");
  }

  tokens[x] = NULL;
/*  while(y < x)/* used for testing
  {
    fprintf(stderr, "\n%s", tokens[y]);/* used for testing
    y++;
  }*/
  return tokens;

}
void builtin_ls(char **cmds)
{
  DIR *d;
  struct dirent *e;
  struct passwd *pwd;
  struct stat st;
  d = opendir("./");
  if (cmds[1] != NULL)
  {
        /*fprintf(stderr, "%d\n", strcmp(cmds[1], "-l")); Used for testing*/
    if (strcmp(cmds[1], "-l") == 0)
    {
        if ( d != NULL)
        {
          while ( e = readdir (d))
          {
            if (strcmp(e->d_name, ".") !=0 && strcmp(e->d_name, "..") !=0)
            {
              lstat(e->d_name,&st);
              /*fprintf(stderr, "File owner id: %d\n", st.st_uid);*/
              pwd = getpwuid(st.st_uid);
              fprintf(stderr, "File name: %s\n", e->d_name);
              fprintf(stderr, "\tSize in bytes: %ld\n", st.st_size);
              fprintf(stderr, "\tblocksize: %ld\n", st.st_blksize);
              fprintf(stderr, "\tFile owner (str): %s\n", pwd->pw_name);
              fprintf(stderr, "\tFile owner (int): %d\n", st.st_uid);
              fprintf(stderr, "\tGroup ID: %d\n", pwd->pw_gid);
              /*fprintf(stderr, "%s\n", e->d_type);*/
            }

          }
          (void) closedir(d);
        }
        else
        {
          fprintf(stderr, "Couldn't open present working directory");
        }
    }
  }
  else
  {
    /*fprintf(stderr, "hey");*/
    if ( d != NULL)
    {
      while ( e = readdir (d))
      {
        if (strcmp(e->d_name, ".") !=0 && strcmp(e->d_name, "..") !=0)
        {
            fprintf(stderr, "%s\n", e->d_name);
        }
      }
      (void) closedir(d);
    }
    else
    {
      fprintf(stderr, "Couldn't open present working directory");
    }
  }
}

void grep_func(char **cmds)
{
  FILE *fp;
  char line[500];
  char *newLine;
  int count = 0;

  fp = fopen(cmds[2],"r"); //returns if something goes wrong
  if (fp == NULL)
  {
    fprintf(stderr, "Couldn't find the file");
    return;
  }
  while(fgets(line, 500, fp) != NULL)
  {
    if(newLine = strchr(line, '\n'))
    {
      *newLine = '\0';
    }
    if(strstr(line, cmds[1]) != NULL)
    {
	  count++;
      //fprintf(stderr, "STRING %s found in the line %d: %s\n", cmds[1], count, line);
      fprintf(stderr, "%d %s\n", count, line);
    }
  }

}

void builtin_grep(char **cmds)
{
  if ((cmds[1] == NULL ) || cmds[2] == NULL)
	{
		fprintf(stderr, "More arguments needed. grep <string> <filename> \n");
		return;
	}

  FILE *fp;
  char line[500];
  char *newLine;
  int count = 0;

  fp = fopen(cmds[2],"r"); //returns if something goes wrong
  if (fp == NULL)
  {
    fprintf(stderr, "Couldn't find the file");
    return;
  }
  while(fgets(line, 500, fp) != NULL)
  {
    if(newLine = strchr(line, '\n'))
    {
      *newLine = '\0';
    }
    if(strstr(line, cmds[1]) != NULL)
    
    
    {
	  count++;
      fprintf(stderr, "STRING %s found in line number %d: %s\n", cmds[1], count, line);
    }
  }
	return;
}

void builtin_more(char **cmds)
{
	execl("/bin/more","more", NULL);
}

void find_dir(char *findthis, char *path)
{
	char newpath[PATH_MAX], cwd[1024];
	DIR * dir;
	struct dirent *ds;
	if (!(dir = opendir(path)))
	{
		return;
	}
	while((ds = readdir(dir)) != NULL)
	{

		if (strcmp(ds->d_name, ".") !=0 && strcmp(ds->d_name, "..") !=0)
		{
      getcwd(cwd, sizeof(cwd));
			strcpy(newpath, path);
			strcat(newpath, "/");
			strcat(newpath, ds->d_name);
			if(strcmp(findthis, ds->d_name) == 0)
			{
				fprintf(stderr, "%s/%s\n", cwd, ds->d_name);
			}
			find_dir(findthis, newpath);
		}
	}
	closedir(dir);
}
void builtin_find(char **cmds)
{
	if ((cmds[1] == NULL ) || cmds[2] == NULL)
	{
		fprintf(stderr, "More arguments needed. find <keyword> <directory> \n");
		return;
	}
	char * findthis = cmds[1];
	DIR *d;
	char pwd[PATH_MAX];

	if(!(d = opendir(cmds[2])))
	{
		getcwd(pwd, sizeof(pwd));
		fprintf(stderr, "Failed to open directory. This can be a bit finicky. The current working directory is %s\n", pwd);
		return;
	}
	find_dir(findthis, cmds[2]);


	return;
}

int checkbuiltin(char **cmds)
{

  int i = 7, x;
  char *listofcommands[i];
  listofcommands[0] = "exit";
  listofcommands[1] = "ls";
  listofcommands[2] = "find";
  listofcommands[3] = "grep";
  listofcommands[4] = "more";
  listofcommands[5] = "help";


  for (x = 0; x < i; x++)
  {
    if(strcmp(cmds[0], listofcommands[x]) == 0)
    {
        x++;
        break;
    }
  }
/*   fprintf(stderr, "\ncheckbuiltin2"); used for testing */
/*  fprintf(stderr, "\n%d", x);  used for testing */

  switch(x)
  {
    case 1:
        builtin_exit();
        return 1;
        break;
    case 2:
        builtin_ls(cmds);
        return 1;
        break;
    case 3:
        builtin_find(cmds);
        return 1;
        break;
    case 4:
        builtin_grep(cmds);
        return 1;
        break;
    case 5:
        builtin_more(cmds);
        return 1;
        break;
    case 6:
        builtin_help();
        return 1;
        break;
    default:
      break;
    }
    return 0;
}
int takecommand()
{
  char *line;
  char **cmds;
  initShell();
  line = read_line();
  cmds = parse_line(line);
  if(cmds[0]== NULL)
  {
    fprintf(stderr, "\nNo commands entered");
    return 1;
  }

  int ran = checkbuiltin(cmds);
  free(line);
  free(cmds);
  return 1;

}


int main()
{
  fprintf(stderr, "Type 'help' for a list of available commands");
  status = 0;
  do {

    int x = takecommand();

  } while(status == 0);

  return 0;

}
