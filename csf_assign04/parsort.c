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
}

void merge_sort(int64_t *arr, size_t begin, size_t end, size_t threshold) {
  // TODO: implement
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

int main(int argc, char **argv) {
  // check for correct number of command line arguments
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <filename> <sequential threshold>\n", argv[0]);
    return 1;
  }

  // process command line arguments
  const char *filename = argv[1];
  char *end;
  size_t threshold = (size_t) strtoul(argv[2], &end, 10);
  if (end != argv[2] + strlen(argv[2]))
    /* TODO: report an error (threshold value is invalid) */;

  // TODO: open the file

  // TODO: use fstat to determine the size of the file

  // TODO: map the file into memory using mmap

  // TODO: sort the data!

  // TODO: unmap and close the file

  // TODO: exit with a 0 exit code if sort was successful
}
