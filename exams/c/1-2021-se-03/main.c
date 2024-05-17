#include <stdio.h>
#include <err.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/types.h>

off_t get_file_size(int fd);

off_t get_file_size(int fd) {
  struct stat info;
  if (fstat(fd, &info) == -1) err(1, "failed to fstat");
  return info.st_size;
}

int main(int argc, char* argv[]) {
  if (argc != 3) errx(1, "provide 2 args");

  int in = open(argv[1], O_RDONLY);
  if (in == -1) err(1, "failed to open");
  int out = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0644);
  if (out == -1) err(1, "failed to open");

  off_t in_size = get_file_size(in);
  uint16_t arr[in_size];

  uint16_t buf;
  ssize_t offset;
  int counter = 0;
  while((offset = read(in, &buf, sizeof(uint16_t))) > 0) {
    arr[counter] = buf;
    counter++;
  }
  if (offset == -1) err(1, "failed to read");

  const char ndef[] = "#ifndef NUMBERS_H\n";
  if (write(out, &ndef, sizeof(ndef)) == -1) err(1, "failed to write"); 
  const char def[] = "#define NUMBERS_H\n";
  if (write(out, &def, sizeof(def)) == -1) err(1, "failed to write"); 

  const char intro[] = "\nconst int arr[] = {";
  if (write(out, &intro, sizeof(intro)) == -1) err(1, "failed to write"); 

  const char del[] = ", ";
  for (int i = 0; i < in_size; i++) {
    char num[16]; 
    sprintf(num, "%u", arr[i]);
    if (write(out, &num, sizeof(num)) == -1) err(1, "failed to write"); 
    if (i != in_size - 1) {
      if (write(out, &del, sizeof(del)) == -1) err(1, "failed to write"); 
    }
  }

  const char outro[] = "};\n\n";
  if (write(out, &outro, sizeof(outro)) == -1) err(1, "failed to write"); 

  const char end[] = "#endif // NUMBERS_H\n";
  if (write(out, &end, sizeof(end)) == -1) err(1, "failed to write"); 

  close(in);
  close(out);

  return 0;
}
