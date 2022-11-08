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

/*
TODO:
  Understand Implication of little Endian
  Implement File Read Structure
    open the file
      int fd = open(filename, O_RDWR);
      if (fd < 0) {
        // file couldn't be opened: handle error and exit
      }
    use fstat to determine the size of the file
      struct stat statbuf;
      int rc = fstat(fd, &statbuf);
      if (rc != 0) {
          // handle fstat error and exit
      }
      size_t file_size_in_bytes = statbuf.st_size;
    map the file into memory using mmap
      int64_t *data = mmap(NULL, file_size_in_bytes, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)
      if (data == MAP_FAILED) {
          // handle mmap error and exit
      }
      // *data now behaves like a standard array of int64_t. Be careful though! Going off the end
      // of the array will silently extend the file, which can rapidly lead to disk space
      // depletion!
    call to sort the data
      self explanatory
    unmap and close the file
    ensure that we handle all errors properly
      check each above step
  Implement Regular Sequential Merge Sort in C
  Convert Merge to Parrallel 



*/


void merge(int64_t *arr, size_t begin, size_t mid, size_t end, int64_t *temparr) {
  // TODO: implement
  size_t p1 = begin; //pointer for first half
  size_t p2 = mid; //pointer for second half
  size_t pt = 0; //pointer for temp array
  while (p1 < mid && p2 < end) {
    if (arr[p1] <= arr[p2]) {
      temparr[pt] = arr[p1];
      p1++;
    } else {
      temparr[pt] = arr[p2];
      p2++;
    }
    pt++;
  }
  while (p1 < mid) {
    temparr[pt] = arr[p1];
    p1++;
    pt++;
  }
  while (p2 < end) {
    temparr[pt] = arr[p2];
    p2++;
    pt++;
  }
}

int cmp_func(const void *left_, const void * right_) {
  int64_t left = *(const int64_t *)left_;
  int64_t right = *(const int64_t *)right_;

  if(left < right) {
    return -1;
  } else if (left == right) {
    return 0;
  } else {
    return 1;
  }
}

// helper function for merge sort function
void sort(int64_t *arr, size_t begin, size_t end, int64_t *tmp) {
  
  for(size_t i = begin; i < end; i++) {
    tmp[i-begin] = arr[i+begin];
  }
  
  qsort(tmp, end-begin, sizeof(int64_t), cmp_func);
}

void merge_sort(int64_t *arr, size_t begin, size_t end, size_t threshold) {
  // sort(arr, begin, end);
  if (end-begin <= threshold) {
    //create temp 
    int64_t tmp[end-begin]; //temp should come from outside
    sort(arr, begin, end,tmp); //pass in pointer TO temp
    //set arr to the info in temp
      // copy array contents from temp to arr
    for(size_t i = begin; i < end; i++) {
      arr[i+begin] = tmp[i];
    }
  }
  else {
    size_t mid = begin + (end-begin)/2;//TODO: check for an off by one error
    // pid_t pid = fork();
    // if (pid == -1) {
    //   exit(1);//error case
    // } else if (pid == 0) {
    //   merge_sort(arr, mid, end, threshold);
    //   exit(0);
    // }
    // merge_sort(arr, begin, mid, threshold);


    pid_t pid_l = fork();
    pid_t pid_r;
    if (pid_l == -1) { //left fork handler
      exit(1);//error case
    } else if (pid_l == 0) {
      merge_sort(arr, mid, end, threshold);
      exit(0);
    } else { //right fork handler
      pid_r = fork();
      if (pid_r == -1) {
        exit(1);//error case
      } else if (pid_r == 0) {
        merge_sort(arr, begin, mid, threshold);
        exit(0);
      }  
    }  

    int wstatus;
    //handle left
    // blocks until the process indentified by pid_to_wait_for completes
    pid_t actual_pid_l = waitpid(pid_l, &wstatus, 0);
    if (!WIFEXITED(wstatus)) {
    // subprocess crashed, was interrupted, or did not exit normally
    // handle as error
    }
    if (WEXITSTATUS(wstatus) != 0) {
        // subprocess returned a non-zero exit code
        // if following standard UNIX conventions, this is also an error
    }
    //handle right
    pid_t actual_pid_r = waitpid(pid_r, &wstatus, 0);
    if (!WIFEXITED(wstatus)) {
    // subprocess crashed, was interrupted, or did not exit normally
    // handle as error
    }
    if (WEXITSTATUS(wstatus) != 0) {
        // subprocess returned a non-zero exit code
        // if following standard UNIX conventions, this is also an error
    }

    

    //serial
    //merge_sort(arr, begin, mid, threshold);
    //merge_sort(arr, mid, end, threshold);


    int64_t temp[end-begin];
    merge(arr,begin,mid,end,temp);
    for (int x = begin; x<end; x++) {
      arr[x] = temp[x-begin];
    }
  
  }
  
  /*
  if (number of elements is at or below the threshold)
    sort the elements sequentially
  else {
    in parallel {
      recursively sort the left half of the sequence
      recursively sort the right half of the sequence
    }
    merge the sorted sequences into a temp array
    copy the contents of the temp array back to the original array
  }
  */
 
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

  if(isdigit(argv[2]) == 0) {
    fprintf(stderr, "Invalid argument\n", argv[0]);
    return 1;
  }

  // process command line arguments
  const char *filename = argv[1];
  char *end;
  size_t threshold = (size_t) strtoul(argv[2], &end, 10);
  if (end != argv[2] + strlen(argv[2]))
    /* TODO: report an error (threshold value is invalid) */
    return 1;

  // TODO: open the file
  int fd = open(filename, O_RDWR);
  if (fd < 0) {
    // file couldn't be opened: handle error and exit
    return 1;
  }
  // TODO: use fstat to determine the size of the file
  struct stat statbuf;
  int rc = fstat(fd, &statbuf);
  if (rc != 0) {
      // handle fstat error and exit
      return 1;
  }
  size_t file_size_in_bytes = statbuf.st_size;
  // TODO: map the file into memory using mmap
  int64_t *data = mmap(NULL, file_size_in_bytes, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (data == MAP_FAILED) {
      // handle mmap error and exit
      return 1;
  }
  // *data now behaves like a standard array of int64_t. Be careful though! Going off the end
  // of the array will silently extend the file, which can rapidly lead to disk space
  // depletion!
  // TODO: sort the data!
  uint64_t num_elements = file_size_in_bytes/8;
  merge_sort(data,0,num_elements,threshold);//TODO: CHECK that file_size_in_bytes makes sence
  // TODO: unmap and close the file
  munmap(NULL, file_size_in_bytes); //TODO: CHECK that NULL is correct
  close(fd);
  for (int x = 0; x<num_elements; x++) {
    printf("%ld\n",data[x]);
  }
  // TODO: exit with a 0 exit code if sort was successful
  return is_sorted(data);
}
