#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//Takes an array as well as a start, mid and end pointer as well as a temp_array to return through
//Two split sections of array are individually sorted
//returns sorted temp array
void merge(int64_t *arr, size_t begin, size_t mid, size_t end, int64_t *temparr) {
  size_t p1 = begin; //pointer for first half
  size_t p2 = mid; //pointer for second half
  size_t pt = 0; //pointer for temp array
  while (p1 < mid && p2 < end) { //take lowest element from each segment and add to temp
    if (arr[p1] <= arr[p2]) { //part 1 is lower
      temparr[pt] = arr[p1]; //take p1
      p1++; //increment p1 pointer
    } else { //part 2 is lowers
      temparr[pt] = arr[p2]; //take p2
      p2++; //increment p2 pointer
    }
    pt++;
  }
  while (p1 < mid) { //finish taking any remaining elements of p1
    temparr[pt] = arr[p1];
    p1++;
    pt++;
  }
  while (p2 < end) { //finish taking any remaining elements of p2
    temparr[pt] = arr[p2];
    p2++;
    pt++;
  }
}

int cmp_func(const void *left_, const void * right_) { //a comparison function for qsort 
  int64_t left = *(const int64_t *)left_;
  int64_t right = *(const int64_t *)right_;

  if(left < right) { //returns -1, 0 , 1 if < = or >
    return -1;
  } else if (left == right) {
    return 0;
  } else {
    return 1;
  }
}

// helper function for merge sort function
//used to sort when we reach threshold
void sort(int64_t *arr, size_t begin, size_t end, int64_t *tmp) {
  
  for(size_t i = begin; i < end; i++) {
    tmp[i-begin] = arr[i];
  }
  
  qsort(tmp, end-begin, sizeof(int64_t), cmp_func);
}

void merge_sort(int64_t *arr, size_t begin, size_t end, size_t threshold) {
  // sort(arr, begin, end);
  if (end-begin <= threshold) {
    //create temp 
    int64_t* tmp = malloc((end-begin)*sizeof(int64_t));  //temp should come from outside
    sort(arr, begin, end, tmp); //pass in pointer TO temp
    //set arr to the info in temp
    // copy array contents from temp to arr
    for(size_t i = begin; i < end; i++) { //move results into main array
      arr[i] = tmp[i-begin];
    }
    free(tmp);
  }
  else {
    size_t mid = begin + (end-begin)/2; // find mid
    pid_t pid_r; //declare pid for right
    pid_t pid_l = fork(); //init pid_l for left and fork
    //left fork handler
    if (pid_l < 0) { //left fork error case
      perror("Error: Left Fork Broke");
      exit(-1);//error case
    } else if (pid_l == 0) { //we are in left fork
      merge_sort(arr, begin, mid, threshold); //recurse down left
      exit(0); //exit after completing all work
    } else { //right fork handler
      pid_r = fork(); //fork right
      if (pid_r < 0) { //right fork error case
        perror("Error: Right Fork Broke");
        exit(-1);//error case
      } else if (pid_r == 0) { // we are in right fork
         merge_sort(arr, mid, end, threshold);//recurse down right
        exit(0);
      }  
    }  

    //code to wait until children done
    int wstatus;
    //handle left
    // blocks until the process indentified by pid_to_wait_for completes
    pid_t actual_pid_l = waitpid(pid_l, &wstatus, 0);
    if (actual_pid_l != pid_l){
      perror("Error: Left Broke wrong pid");
      exit(-1);
    }
    if (!WIFEXITED(wstatus)) {
      // subprocess crashed, was interrupted, or did not exit normally
      // handle as error
      perror("Error: Left Broke with crash/interrupt/bad exit");
      exit(-1);
    }
    if (WEXITSTATUS(wstatus) != 0) {
      // subprocess returned a non-zero exit code
      // if following standard UNIX conventions, this is also an error
      perror("Error: Left Broke None Zero Exit");
      exit(-1);
    }
    //handle right
    pid_t actual_pid_r = waitpid(pid_r, &wstatus, 0);
    if (actual_pid_r != pid_r){
      perror("Error: right Broke wrong pid");
      exit(-1);
    }
    if (!WIFEXITED(wstatus)) {
      // subprocess crashed, was interrupted, or did not exit normally
      // handle as error
      perror("Error: Right Broke with crash/interrupt/bad exit");
      exit(-1);
    }
    if (WEXITSTATUS(wstatus) != 0) {
      // subprocess returned a non-zero exit code
      // if following standard UNIX conventions, this is also an error
      perror("Error: Right Broke None Zero Exit");
      exit(-1);
    }

    //create temp array
    int64_t* temp = malloc((end-begin)*sizeof(int64_t));
    merge(arr,begin,mid,end,temp); //merge results of forks into temp array
    for (int x = begin; x<end; x++) { //push temp results into main array
      arr[x] = temp[x-begin];
    }
    free(temp);
  } 
}

int is_sorted(int64_t *arr) { //ensures that the array is sorted
  int length = sizeof(arr);
  for(int i = 1; i < length; i++) { // starting at 1 is okay, must have at least 8 entries anyways
    if(arr[i] < arr[i-1]) {
      return -1;
    }
  }
  return 0;
}

int main(int argc, char **argv) {
  // check for correct number of command line arguments
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <filename> <sequential threshold>\n", argv[0]);
    return 1;
  }

  //validate that threshold is a digit
  if(isdigit(*argv[2]) == 0) {
    fprintf(stderr, "Error: Invalid argument\n");
    return 1;
  }

  // process command line arguments
  const char *filename = argv[1];
  char *end;
  size_t threshold = (size_t) strtoul(argv[2], &end, 10);
  if (end != argv[2] + strlen(argv[2])){
    /* report an error (threshold value is invalid) */
    fprintf(stderr,"Error: Bad threshold");
    return 1;
  }
  //open the file
  int fd = open(filename, O_RDWR);
  if (fd < 0) {
    // file couldn't be opened: handle error and exit
    fprintf(stderr,"Error: Bad file open");
    return 1;
  }
  //use fstat to determine the size of the file
  struct stat statbuf;
  int rc = fstat(fd, &statbuf);
  if (rc != 0) {
      // handle fstat error and exit
      fprintf(stderr,"Error: Bad fstat");
      return 1;
  }
  size_t file_size_in_bytes = statbuf.st_size;
  //map the file into memory using mmap
  int64_t *data = mmap(NULL, file_size_in_bytes, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (data == MAP_FAILED) {
      // handle mmap error and exit
    fprintf(stderr,"Error: Bad Map");
    return 1;
  }
  // *data now behaves like a standard array of int64_t. Be careful though! Going off the end
  // of the array will silently extend the file, which can rapidly lead to disk space
  // depletion!
  //sort the data!
  uint64_t num_elements = file_size_in_bytes/8;
  merge_sort(data,0,num_elements,threshold);//TODO: CHECK that file_size_in_bytes makes sence
  //unmap and close the file
  munmap(NULL, file_size_in_bytes); //TODO: CHECK that NULL is correct
  close(fd);
  //exit with a 0 exit code if sort was successful
  return is_sorted(data);
}
