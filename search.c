#include <windows.h>
#include <string.h>
#include <stdio.h>

#define _ZERO 0

unsigned char * slash     = "\\";
unsigned char * t_one     = ".";
unsigned char * t_two     = "..";
unsigned char * expansion = "*";

long int number_of_files = _ZERO;

void      FindDir(unsigned char * path, unsigned char * mask);
short int CheckLogicalDisk(short int number_disk);

int main (int argc, unsigned char * argv[]) {
 if (argc != 3) {
  printf("Incorrect argument!\n"); 
  printf("Enter command: %s [local_disk:] [mask *]\n", argv[0]); 
 }
 else
 if (argc == 3) {
  unsigned char LOCAL_DISK[256] = {'A',':',_ZERO};
  int i;

   if (strcmp(argv[1], "-a") == _ZERO) {
    for (i = 65; i <= 90; i++) {
     if (CheckLogicalDisk(i) == _ZERO) {
      LOCAL_DISK[_ZERO] = (char) i;
      FindDir(&LOCAL_DISK[_ZERO], argv[2]);
     }
    }
   }
   else {
    int length = strlen(argv[1]); 
     if (length > 0 && length < 256) {
      memset(&LOCAL_DISK[_ZERO], _ZERO, 256);
      strcat(&LOCAL_DISK[_ZERO], argv[1]);
      FindDir(&LOCAL_DISK[_ZERO], argv[2]);
     }
     else
      printf("Error: length mask incorrect!\n");
   }

  printf("\n + The search is complete!\n");
  printf(" + Found: %ld files!\n", number_of_files);  
 }
 return 0;
}

short int CheckLogicalDisk(short int number_disk) {
 unsigned char LOGICAL_DISK[] = "+:";
 LOGICAL_DISK[_ZERO] = (unsigned char) number_disk;  

 if (GetDriveType(&LOGICAL_DISK[_ZERO]) == DRIVE_FIXED)
  return _ZERO;
 
 return -1;
}

void FindDir(unsigned char * path, unsigned char * mask) {
 WIN32_FIND_DATA wfd;
 HANDLE hfound;

 char newpath[MAX_PATH]   = {_ZERO};
 char fpath[MAX_PATH]     = {_ZERO};
 char pathifile[MAX_PATH] = {_ZERO};
 char delpath[MAX_PATH]   = {_ZERO};

 strcpy(fpath, path);
 strcat(fpath, slash);
 strcpy(delpath, fpath);
 strcat(fpath, mask);

 if ((hfound = FindFirstFile(fpath, &wfd)) != INVALID_HANDLE_VALUE) {
  do {
   if ((wfd.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY) && (strcmp(wfd.cFileName, t_two) != _ZERO) && (strcmp(wfd.cFileName, t_one) != _ZERO)) {
    strcpy(pathifile, path);
    strcat(pathifile, slash);
    strcat(pathifile, wfd.cFileName);
    printf(" - %s\n", pathifile);
    number_of_files++;
   }
  } while (FindNextFile(hfound, &wfd));
 }

 FindClose(hfound);
 strcpy(fpath, path);
 strcat(fpath, slash);
 strcat(fpath, expansion);

 if ((hfound = FindFirstFile(fpath, &wfd)) != INVALID_HANDLE_VALUE) {
  do {
   if ((wfd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) && (strcmp(wfd.cFileName, t_two) != _ZERO) && (strcmp(wfd.cFileName, t_one) != _ZERO)) {
    strcpy(newpath, path);
    strcat(newpath, slash);
    strcat(newpath, wfd.cFileName);
    FindDir(newpath, mask);
   }
  } while (FindNextFile(hfound, &wfd));
 }
 FindClose(hfound);
}
