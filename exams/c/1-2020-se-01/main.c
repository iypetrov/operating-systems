#include <stdio.h>
#include <err.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>

uint16_t get_count(int fd);

uint16_t get_count(int fd) {
  uint8_t byte;

  if (read(fd, &byte, sizeof(uint8_t)) == -1) err(1, "failed to write");
  if (read(fd, &byte, sizeof(uint8_t)) == -1) err(1, "failed to write");
  if (read(fd, &byte, sizeof(uint8_t)) == -1) err(1, "failed to write");
  if (read(fd, &byte, sizeof(uint8_t)) == -1) err(1, "failed to write");

  uint16_t count;
  if (read(fd, &count, sizeof(uint16_t)) == -1) err(1, "failed to write");

  if (read(fd, &byte, sizeof(uint8_t)) == -1) err(1, "failed to write");
  if (read(fd, &byte, sizeof(uint8_t)) == -1) err(1, "failed to write");
  if (read(fd, &byte, sizeof(uint8_t)) == -1) err(1, "failed to write");
  if (read(fd, &byte, sizeof(uint8_t)) == -1) err(1, "failed to write");
  if (read(fd, &byte, sizeof(uint8_t)) == -1) err(1, "failed to write");
  if (read(fd, &byte, sizeof(uint8_t)) == -1) err(1, "failed to write");
  if (read(fd, &byte, sizeof(uint8_t)) == -1) err(1, "failed to write");
  if (read(fd, &byte, sizeof(uint8_t)) == -1) err(1, "failed to write");
  if (read(fd, &byte, sizeof(uint8_t)) == -1) err(1, "failed to write");
  if (read(fd, &byte, sizeof(uint8_t)) == -1) err(1, "failed to write");

  return count;
}

int main(int argc, char* argv[]) {
  if (argc != 7) errx(1, "provide 6 args"); 

  int affix = open(argv[1], O_RDONLY);
  if (affix == -1) err(1, "failed to open affix");
  int postfix = open(argv[2], O_RDONLY);
  if (postfix == -1) err(1, "failed to open postfix");
  int prefix = open(argv[3], O_RDONLY);
  if (prefix == -1) err(1, "failed to open prefix");
  int infix = open(argv[4], O_RDONLY);
  if (infix == -1) err(1, "failed to open infix");
  int suffix = open(argv[5], O_RDONLY);
  if (suffix == -1) err(1, "failed to open suffix");
  int crucifixus = open(argv[6], O_WRONLY | O_TRUNC | O_CREAT, 0644);
  if (crucifixus == -1) err(1, "failed to open crucifixus");

  uint16_t affix_cnt = get_count(affix);
  dprintf(1, "affix_cnt -> %d\n", affix_cnt);
  uint16_t affix_arr[affix_cnt];
  for (int i = 0; i < affix_cnt; i++) {
    uint16_t affix_buf;
    if (read(affix, &affix_buf, sizeof(uint16_t)) == -1) err(1, "failed to read");
    affix_arr[i] = affix_buf;
    dprintf(1, "%d ", affix_arr[i]);
  }
  dprintf(1, "\n\n");

  uint16_t postfix_cnt = get_count(postfix);
  dprintf(1, "postfix_cnt -> %d\n", postfix_cnt);
  uint32_t postfix_arr[postfix_cnt];
  for (int i = 0; i < postfix_cnt; i++) {
    uint32_t postfix_buf;
    if (read(postfix, &postfix_buf, sizeof(uint32_t)) == -1) err(1, "failed to read");
    postfix_arr[i] = postfix_buf;
    dprintf(1, "%d ", postfix_arr[i]);
  }
  dprintf(1, "\n\n");

  uint16_t prefix_cnt = get_count(prefix);
  dprintf(1, "prefix_cnt -> %d\n", prefix_cnt);
  uint8_t prefix_arr[prefix_cnt];
  for (int i = 0; i < prefix_cnt; i++) {
    uint8_t prefix_buf;
    if (read(prefix, &prefix_buf, sizeof(uint8_t)) == -1) err(1, "failed to read");
    prefix_arr[i] = prefix_buf;
    dprintf(1, "%d ", prefix_arr[i]);
  }
  dprintf(1, "\n\n");

  uint16_t infix_cnt = get_count(infix);
  dprintf(1, "infix_cnt -> %d\n", infix_cnt);
  uint16_t infix_arr[infix_cnt];
  for (int i = 0; i < infix_cnt; i++) {
    uint16_t infix_buf;
    if (read(infix, &infix_buf, sizeof(uint16_t)) == -1) err(1, "failed to read");
    infix_arr[i] = infix_buf;
    dprintf(1, "%d ", infix_arr[i]);
  }
  dprintf(1, "\n\n");

  uint16_t suffix_cnt = get_count(suffix);
  dprintf(1, "suffix_cnt -> %d\n", suffix_cnt);
  uint64_t suffix_arr[suffix_cnt];
  for (int i = 0; i < suffix_cnt; i++) {
    uint64_t suffix_buf;
    if (read(suffix, &suffix_buf, sizeof(uint64_t)) == -1) err(1, "failed to read");
    suffix_arr[i] = suffix_buf;
    dprintf(1, "%ld ", suffix_arr[i]);
  }
  dprintf(1, "\n\n");

  dprintf(1, "output:\n\n");
  if (affix_cnt % 8 != 0) errx(1, "affix not in correct format"); 
  for (int i = 0; i < affix_cnt; i+=8) {
    dprintf(1, "postfix:\n");
    dprintf(1, "%d %d\n", affix_arr[i], affix_arr[i + 1]);
    for (int j = affix_arr[i]; j < affix_arr[i] + affix_arr[i + 1]; j++) {
      dprintf(1, "%d -> %d\n", j, postfix_arr[j]);
      if(write(crucifixus, &postfix_arr[j], sizeof(uint32_t)) == -1) err(1, "failed to write");
    }
    dprintf(1, "prefix:\n");
    dprintf(1, "%d %d\n", affix_arr[i + 2], affix_arr[i + 3]);
    for (int j = affix_arr[i + 2]; j < affix_arr[i + 2] + affix_arr[i + 3]; j++) {
      dprintf(1, "%d -> %d\n", j, prefix_arr[j]);
      if(write(crucifixus, &prefix_arr[j] , sizeof(uint8_t)) == -1) err(1, "failed to write");
    }
    dprintf(1, "infix:\n");
    dprintf(1, "%d %d\n", affix_arr[i + 4], affix_arr[i + 5]);
    for (int j = affix_arr[i + 4]; j < affix_arr[i + 4] + affix_arr[i + 5]; j++) {
      dprintf(1, "%d -> %d\n", j, infix_arr[j]);
      if(write(crucifixus, &infix_arr[j], 2 * sizeof(uint16_t)) == -1) err(1, "failed to write");
    }
    dprintf(1, "suffix:\n");
    dprintf(1, "%d %d\n", affix_arr[i + 6], affix_arr[i + 7]);
    for (int j = affix_arr[i + 6]; j < affix_arr[i + 6] + affix_arr[i + 7]; j++) {
      dprintf(1, "%d -> %ld\n", j, suffix_arr[j]);
      if(write(crucifixus, &suffix_arr[j], sizeof(uint64_t)) == -1) err(1, "failed to write");
    }
    dprintf(1, "\n");
  }

  close(affix);
  close(postfix);
  close(prefix);
  close(infix);
  close(suffix);
  close(crucifixus);

  return 0;
}
