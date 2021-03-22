/* GHERASIE STEFANIA-ELENA 313 CB */

#define SIZE 40
#define CHAR_TO_INDEX(c) ((int)c - (int)'a') + 1

/*returneaza indicele corespunzator caracterului*/
int char_to_index(char ch) 
{
  int i;
  /*pozitia 0 - caracterul '/' */
  if (CHAR_TO_INDEX(ch) == CHAR_TO_INDEX('/')) return 0;
  /*pozitia 28 - caracterul '.' */
  if (CHAR_TO_INDEX(ch) == CHAR_TO_INDEX('.')) return 28;
  for (i = 0; i <= 9; i++) {
    if (CHAR_TO_INDEX(i + '0') == CHAR_TO_INDEX(ch)) return 29 + i;
  }
  return CHAR_TO_INDEX(ch);
}

/*numarul numarul de cuvinte din sir*/
int count_words(char str[100]) 
{
  int i, nr = 1;
  for (i = 0; str[i] != '\0'; i++)
    if (str[i] == ' ') nr++;
  return nr;
}

/*returneaza directorul anterior*/
char* previous_dir(char current_dir[]) 
{
  int i;
  for (i = strlen(current_dir) - 2; i >= 0; i--)
    if (current_dir[i] == '/') {
      current_dir[i] = '\0';
      break;
    }
  if (i == 0) strcpy(current_dir, "/");
  return current_dir;
}

/*calculeaza numarul de directoare*/
int number_of_directories(char str[100]) 
{
  int i, nr = 0;
  for (i = 0; i <= strlen(str); i++)
    if (str[i] == '/') nr++;
  return nr;
}

/*returneaza calea calea absoluta catre director */
char* directory_path(char path[], char str[], char current_dir[],
                     FILE* error_file) 
{
  if (strcmp(str, "/") == 0) /*directorul root*/
    strcpy(path, "/");
  else if (str[0] == '/') /*calea incepe cu '/', este absoluta*/
    sprintf(path, "%s/", str);
  else if (strcmp(str, ".") == 0) /*director curent*/
    strcpy(path, current_dir);
  else if (strcmp(str, "..") == 0) /*director precedent*/
  {
    if (strcmp(current_dir, "/") != 0) {
      memcpy(path, previous_dir(path), 100);
      if (path[strlen(path) - 1] != '/') strcat(path, "/");
    } else
      fprintf(error_file, "%s: No such file or directory\n", str);
  } else
    sprintf(path, "%s%s/", current_dir,
            str); /*concatenare la directorul curent*/
  return path;
}

/*returneaza calea absoluta catre fisier*/
char* file_path(char path[], char str[], char current_dir[]) 
{
  if (str[0] == '/') /*calea incepe cu '/', este absoluta*/
    sprintf(path, "%s", str);
  else
    sprintf(path, "%s%s", current_dir,
            str); /*concatenare la directorul curent*/
  return path;
}

/*testare trie este gol*/
int isEmpty(TNode* trie) 
{
  int i;
  for (i = 0; i < SIZE; i++)
    if (trie->children[i]) return 0;
  return 1;
}

int isFileNode(TNode* node) 
{ return (node->isFile != 0); }

int isDirectoryNode(TNode* node) 
{ return (node->isDirectory != 0); }

int isFreeNode(TNode* node) 
{
  int i;
  for (i = 0; i < SIZE; i++)
    if (node->children[i]) return 0;
  return 1;
}

/*testare directorul este gol*/
int isEmptyDir(TNode* trie, char dir[100]) 
{
  int i, index;
  int length = strlen(dir);

  TNode* node = trie;
  for (i = 0; i < length; i++) {
    index = char_to_index(dir[i]);
    if (!node->children[index]) return 0;
    node = node->children[index];
  }
  return isEmpty(node);
}

/*cauta in trie fisierul*/
int searchFile(TNode* trie, char file[100]) 
{
  int i, index;
  int length = strlen(file);

  TNode* node = trie;
  for (i = 0; i < length; i++) {
    index = char_to_index(file[i]);
    if (!node->children[index]) return 0;
    node = node->children[index];
  }
  return (node != NULL && node->isFile);
}

/*cauta in trie directorul*/
int searchDirectory(TNode* trie, char dir[100]) 
{
  int i, index;
  int length = strlen(dir);

  TNode* node = trie;
  for (i = 0; i < length; i++) {
    index = char_to_index(dir[i]);
    if (!node->children[index]) return 0;
    node = node->children[index];
  }
  return (node != NULL && node->isDirectory);
}

/*directorul anterior a fost creat*/
int director_exists(TNode* node, char dir[]) 
{
  char path[100];
  memcpy(path, previous_dir(dir), sizeof(path));
  if (strcmp(path, "/") != 0) strcat(path, "/");
  return searchDirectory(node, path);
}

/*creare trie*/
TNode* CreateNode() 
{
  TNode* node = NULL;
  node = (TNode*)malloc(sizeof(TNode));
  if (!node) return node;
  int i;
  node->isFile = 0;
  node->isDirectory = 0;
  for (i = 0; i < SIZE; i++) node->children[i] = NULL;
  return node;
}

/*comanda pwd*/
void pwd(char current_dir[], FILE* out_file) 
{
  char path[100];
  strcpy(path, current_dir);

  if (strcmp(path, "/") != 0) path[strlen(path) - 1] = '\0';
  /*afisare director curent*/
  fprintf(out_file, "%s\n", path);
}

/*afiseaza output-ul comenzii ls*/
void display_ls(TNode* trie, char str[], int num, FILE* out_file, int ls_f) 
{
    /*cazul in care e director*/
  if (isDirectoryNode(trie)) {
    str[num] = '\0';
    if (number_of_directories(str) == 1 && strcmp(str, "") != 0) {
      str[num - 1] = '\0';
      if (ls_f == 0)
        fprintf(out_file, "%s ", str);
      else
        fprintf(out_file, "%s/ ", str);
      str[num - 1] = '/';
    }
  }
  /*cazul in care e fisier*/
  if (isFileNode(trie)) {
    str[num] = '\0';
    if (number_of_directories(str) == 0 && strcmp(str, "") != 0) {
      if (ls_f == 0)
        fprintf(out_file, "%s ", str);
      else
        fprintf(out_file, "%s* ", str);
    }
  }
  int i;

  for (i = 0; i < SIZE; i++) {
    if (trie->children[i]) {
        /*trecerea de la indice la caracter*/
      if (i == 0)
        str[num] = '/';
      else if (i == 28)
        str[num] = '.';
      else if (i > 28 && i < 39)
        str[num] = i - 29 + '0';
      else {
        str[num] = i - 1 + 'a';
      }
      /*apelare recursiva a functiei pentru a afisa fiecare caracter*/
      display_ls(trie->children[i], str, num + 1, out_file, ls_f);
    }
  }
}


/*comanda ls*/
int ls(TNode* trie, char path[20], FILE* out_file, int ls_f) 
{
  int i, index, num = 0;
  char str[100];
  int length = strlen(path);

  TNode* node = trie;
  if (isFreeNode(node) == 1) {
    fprintf(out_file, "\n");
    return 1;
  }
  /*gasire director*/
  for (i = 0; i < length; i++) {
    index = char_to_index(path[i]);
    if (!node->children[index]) return 0;
    node = node->children[index];
  }
  if (node != NULL && node->isDirectory)

  {
    /*afisarea fiilor directorului*/
    display_ls(node, str, num, out_file, ls_f);
    fprintf(out_file, "\n");
    return 1;
  }
  return 0;
}

/*adauga un fisier in trie*/
void add_file(TNode* trie, char file[]) 
{
  int i, index;
  int length = strlen(file);

  TNode* node = trie;
  for (i = 0; i < length; i++) {
    index = char_to_index(file[i]);
    if (!node->children[index]) node->children[index] = CreateNode();
    node = node->children[index];
  }
  node->isFile = 1;
}

/*comanda touch*/
void touch(TNode* trie, char str[], char current_dir[], FILE* error_file) 
{
  char path[100];
  memcpy(path, file_path(path, str, current_dir), sizeof(path));
  char path2[100];
  memcpy(path2, path, sizeof(path));
  if (director_exists(trie, path2) == 1)
    add_file(trie, path);
  else
    fprintf(error_file, "%s: No such file or directory\n", str);
}

/*adauga un director in trie*/
void add_dir(TNode* trie, char dir[]) 
{
  int i, index;
  int length = strlen(dir);

  TNode* node = trie;
  for (i = 0; i < length; i++) {
    index = char_to_index(dir[i]);
    if (!node->children[index]) node->children[index] = CreateNode();
    node = node->children[index];
  }
  node->isDirectory = 1;
}

/*comanda mkdir*/
void mkdir(TNode* trie, char str[], char current_dir[], FILE* error_file) 
{
  char path[100];
  memcpy(path, current_dir, sizeof(path));
  memcpy(path, directory_path(path, str, current_dir, error_file),
         sizeof(path));
  char path2[100];
  strcpy(path2, path);
  if (searchDirectory(trie, path) == 1)
    fprintf(error_file, "%s: already exists\n", str);
  else if (director_exists(trie, path2) == 1)
    add_dir(trie, path);
  else
    fprintf(error_file, "%s: No such file or directory\n", str);
}

/*sterge fisierul*/
int deleteFile(TNode* node, char file[], int num, int len) 
{
  if (node) {
    if (num == len) {
      if (node->isFile) {
        /*se sterge eticheta de fisier*/
        node->isFile = 0;
        if (isFreeNode(node)) return 1;
        return 0;
      }
    } else {
      int index = char_to_index(file[num]);
      /*stergere recursiva a fiecarui caracter*/
      if (deleteFile(node->children[index], file, num + 1, len)) {
        free(node->children[index]);
        return (!isFileNode(node) && isFreeNode(node));
      }
    }
  }

  return 0;
}

/*sterge director*/
int deleteDir(TNode* node, char dir[], int num, int len) 
{
  if (node) {
    if (num == len) {
      if (node->isDirectory) {
        /*se sterge eticheta de director*/
        node->isDirectory = 0;
        if (isFreeNode(node)) return 1;
        return 0;
      }
    } else {
      int index = char_to_index(dir[num]);
      /*stergere recursiva a fiecarui caracter*/
      if (deleteDir(node->children[index], dir, num + 1, len)) {
        free(node->children[index]);
        return (!isDirectoryNode(node) && isFreeNode(node));
      }
    }
  }
  return 0;
}

/*sterge director sau fisier*/
void deletePath(TNode* trie, char path[]) 
{
  int len = strlen(path);

  if (len > 0) {
    if (searchDirectory(trie, path) == 1)
      deleteDir(trie, path, 0, len);
    else
      deleteFile(trie, path, 0, len);
  }
}

/*comanda mv*/
void mv(TNode* trie, char files[100], char dest_dir[10], char current_dir[100],
        FILE* out_file, FILE* error_file) 
{
  int i;
  files[strlen(files) - 1] = '\0';
  /*verificare comanda are suficienti operanti*/
  if (strcmp(files, dest_dir) == 0) {
    fprintf(error_file, "mv %s: missing operand\n", dest_dir);
    return;
  }
  char path[100];  
  /*obtinere cale absoluta director destinatie*/
  memcpy(path, directory_path(path, dest_dir, current_dir, error_file),
         sizeof(path));
  for (i = strlen(files) - 1; i >= 0; i--)
    if (files[i] == ' ') {
      files[i] = '\0';
      break;
    }
  char* buffer = strtok(files, " ");
  char file[100], dest_dir2[100];
  while (buffer) {
    /*obtinere cale absoluta pentru fiecare fisier*/
    memcpy(file, file_path(file, buffer, current_dir), sizeof(file));
    /*cazurile de eroare*/
    if (searchFile(trie, file) == 0)
      fprintf(error_file, "%s: No such file or directory\n", buffer);
    else {
      if (searchDirectory(trie, path) == 0) {
        path[strlen(path) - 1] = '\0';
        if (searchFile(trie, path) == 1)
          fprintf(error_file, "%s: Not a directory\n", dest_dir);
        else
          fprintf(error_file, "%s: No such file or directory\n", dest_dir);
        return;
      }
      memcpy(dest_dir2, path, sizeof(path));
      strcat(dest_dir2, file);
      /*creare fisier la noua cale*/
      touch(trie, dest_dir2, current_dir, error_file);
      /*sterge fisier de la vechea cale*/
      deletePath(trie, file);
    }
    buffer = strtok(NULL, " ");
  }
}

/*comanda cp*/
void cp(TNode* trie, char files[100], char dest_dir[10], char current_dir[100],
        FILE* out_file, FILE* error_file) 
{
  int i;
  files[strlen(files) - 1] = '\0';
  if (strcmp(files, dest_dir) == 0) {
    fprintf(error_file, "cp %s: missing operand\n", dest_dir);
    return;
  }
  char path[100];
    /*obtinere cale absoluta director destinatie*/
  memcpy(path, directory_path(path, dest_dir, current_dir, error_file),
         sizeof(path));

  for (i = strlen(files) - 1; i >= 0; i--)
    if (files[i] == ' ') {
      files[i] = '\0';
      break;
    }
  char* buffer = strtok(files, " ");
  char file[100], dest_dir2[100];
  while (buffer) {
    /*obtinere cale absoluta pentru fiecare fisier*/
    memcpy(file, file_path(file, buffer, current_dir), sizeof(file));

    /*cazurile de eroare*/
    if (searchFile(trie, file) == 0)
      fprintf(error_file, "%s: No such file or directory\n", buffer);
    else {
      if (searchDirectory(trie, path) == 0) {
        fprintf(error_file, "%s: No such file or directory\n", dest_dir);
        return;
      }
      memcpy(dest_dir2, path, sizeof(path));
      strcat(dest_dir2, buffer);
      /*creare fisier la noua cale*/
      touch(trie, dest_dir2, current_dir, error_file);
    }
    buffer = strtok(NULL, " ");
  }
}

/*comanda rmdir*/
void rmdir(TNode* trie, char str[], char current_dir[], FILE* error_file) 
{
  char path[100];
  /*obtinere cale absoluta director*/
  memcpy(path, directory_path(path, str, current_dir, error_file),
         sizeof(path));
  /*cazuri eroare*/
  if (searchDirectory(trie, path) == 0)
    fprintf(error_file, "%s: No such file or directory\n", str);
  else if (isEmptyDir(trie, path) == 0)
    fprintf(error_file, "%s: Directory not empty\n", str);
  else
    deletePath(trie, path); /*stergere director*/
}

/*comanda rm*/
void rm(TNode* trie, char str[], char current_dir[], FILE* error_file) 
{
  char path[100];
  /*obtinere cale absoluta fisier*/

  memcpy(path, file_path(path, str, current_dir), sizeof(path));
  if (searchFile(trie, path) == 1) {
    deletePath(trie, path);     /*stergere fisier*/
  } else {
    /*obtinere cale absoluta director(daca nu e fisier*/
    memcpy(path, directory_path(path, str, current_dir, error_file),
           sizeof(path));
    if (searchDirectory(trie, path) == 1)
      deletePath(trie, path);   /*stergere director*/
    else
      fprintf(error_file, "%s: No such file or directory\n", str);
  }
}
/*eliberarea memoriei*/
void free_trie(TNode* trie) 
{
  int i;
  if (!trie) return;

  for (i = 0; i < SIZE; i++) {
    if (trie->children[i] != NULL) {
      free_trie(trie->children[i]);
    }
  }
  free(trie);
}