/* GHERASIE STEFANIA-ELENA 313 CB */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tip.h"
#include "functions.h"

int main() {
  /*deschiderea fisierelor*/
  FILE* in_file = fopen("input.in", "rt");
  if (in_file == NULL) {
    fprintf(stderr, "ERROR: Can't open input file");
    return -1;
  }
  FILE* out_file = fopen("output.out", "wt");
  if (out_file == NULL) {
    fprintf(stderr, "ERROR: Can't open output file");
    return -1;
  }
  FILE* error_file = fopen("error.out", "wt");
  if (error_file == NULL) {
    fprintf(stderr, "ERROR: Can't open error file");
    return -1;
  }

  int num_operations, word_count, ls_f;
  char path[100], current_dir[100], command[100], command2[100];
  char files[100], dest_dir[100];

  /*crearea structurii de trie si adaugarea directorului root*/
  TNode* trie = CreateNode();
  add_dir(trie, "/");
  /*initial directorul curent este root*/
  strcpy(current_dir, "/");

  /*extragerea numarului de operatii din fisier*/
  fscanf(in_file, "%d", &num_operations);
  fgetc(in_file);

  while (num_operations) {
    num_operations--;

    /*obtinerea comenzilor*/
    fgets(command, 100 * sizeof(char), in_file);
    if (command[strlen(command) - 1] == '\n')
      command[strlen(command) - 1] = '\0';
    word_count = count_words(command);
    strcpy(command2, command);

    /*tratarea cazului fiecarei comenzi*/
    char* buffer = strtok(command, " ");

    if (strcmp(buffer, "pwd") == 0)
      pwd(current_dir, out_file);

    else if (strcmp(buffer, "cd") == 0) {
      buffer = strtok(NULL, " ");

      /*se obtine calea absoluta pt director*/
      memcpy(path, current_dir, sizeof(current_dir));
      memcpy(path, directory_path(path, buffer, current_dir, error_file),
             sizeof(path));

      /*testare daca directorul exista in structura*/
      if (searchDirectory(trie, path) == 1)
        memcpy(current_dir, path, sizeof(path));
      else {
        /*argumentul este de fapt fisier sau nu exista*/
        memcpy(path, file_path(path, buffer, current_dir), sizeof(path));
        if (searchFile(trie, path) == 1)
          fprintf(error_file, "%s: Not a directory\n", buffer);
        else
          fprintf(error_file, "%s: No such file or directory\n", buffer);
      }
    }

    else if (strcmp(buffer, "ls") == 0) {
      if (word_count > 3)
        fprintf(error_file, "%s: too many arguments\n", command2);
      else {
        ls_f = 0;
        buffer = strtok(NULL, " ");
        /*comanda este executata cu flag-ul '-F'*/
        if (buffer != NULL && strcmp(buffer, "-F") == 0) {
          ls_f = 1;
          buffer = strtok(NULL, " ");
        }

        /*nu mai exista alte argumente, se face ls pt directorul curent*/
        if (buffer == NULL)
          ls(trie, current_dir, out_file, ls_f);
        else {
          /*se obtine calea absoluta*/
          memcpy(path, current_dir, sizeof(current_dir));
          memcpy(path, directory_path(path, buffer, current_dir, error_file),
                 sizeof(path));

          /*verificare daca argumentul este director*/
          if (searchDirectory(trie, path) == 0)
            fprintf(error_file, "%s: No such file or directory\n", buffer);
          else
            ls(trie, path, out_file, ls_f);
        }
      }
    }

    else if (strcmp(buffer, "mkdir") == 0) {
      buffer = strtok(NULL, " ");
      while (buffer) {
        mkdir(trie, buffer, current_dir, error_file);
        buffer = strtok(NULL, " ");
      }
    }

    else if (strcmp(buffer, "touch") == 0) {
      buffer = strtok(NULL, " ");
      while (buffer) {
        touch(trie, buffer, current_dir, error_file);
        buffer = strtok(NULL, " ");
      }
    }

    else if (strcmp(buffer, "rmdir") == 0) {
      buffer = strtok(NULL, " ");
      while (buffer) {
        rmdir(trie, buffer, current_dir, error_file);
        buffer = strtok(NULL, " ");
      }
    }

    else if (strcmp(buffer, "rm") == 0) {
      buffer = strtok(NULL, " ");
      while (buffer) {
        rm(trie, buffer, current_dir, error_file);
        buffer = strtok(NULL, " ");
      }
    }

    else if (strcmp(buffer, "mv") == 0) {
      strcpy(files, "");
      buffer = strtok(NULL, " ");
      /*se salveaza toate argumentele comenzii in sirul "files" */
      /*calea unde trebuie mutate fisierele se salveaza in "dest_dir*/
      while (buffer) {
        strcat(files, buffer);
        strcat(files, " ");
        strcpy(dest_dir, buffer);
        buffer = strtok(NULL, " ");
      }
      mv(trie, files, dest_dir, current_dir, out_file, error_file);
    }

    else if (strcmp(buffer, "cp") == 0) {
      strcpy(files, "");
      buffer = strtok(NULL, " ");
      /*se salveaza toate argumentele comenzii in sirul "files" */
      /*calea unde trebuie mutate fisierele se salveaza in "dest_dir*/
      while (buffer) {
        strcat(files, buffer);
        strcat(files, " ");
        strcpy(dest_dir, buffer);
        buffer = strtok(NULL, " ");
      }
      cp(trie, files, dest_dir, current_dir, out_file, error_file);
    }

    /*comanda nu corespunde*/
    else
      fprintf(error_file, "%s: command not found\n", buffer);
  }

  /*eliberarea memoriei*/
  free_trie(trie);

  fclose(in_file);
  fclose(out_file);
  fclose(error_file);

  return 0;
}