#include <stdio.h>
#include <stdbool.h>

#define TS_SYNC_CODE 0x47

bool is_stream_of_188bytes(char* stream_ptr, uint32_t size)
{
  char* cur_ptr = stream_ptr;

  // Find 1st sync code (0x47)
  while (*cur_ptr != TS_SYNC_CODE) {
    cur_ptr = cur_ptr + 1;

    // Reach the end of stream
    if (cur_ptr == stream_ptr + size)
      return false;
  }

  // Verify integration
  if (*(cur_ptr + 188) == TS_SYNC_CODE
      && *(cur_ptr + 188*2) == TS_SYNC_CODE
      && *(cur_ptr + 188*3) == TS_SYNC_CODE) {
    return true;
  } else {
    return false;
  }
}

int main(int argc, char* argv[])
{
  if (argc < 2) {
    printf("Usage: ./main [filename]\n");
    return -1;
  }

  FILE* fp = fopen(argv[1], "rb");
  if (!fp) {
    perror("fopen");
    return -1;
  }

  #define BUFFER_SIZE 1024
  char buffer[BUFFER_SIZE] = {0};
  uint32_t size_read = fread(buffer, BUFFER_SIZE, 1, fp);

  bool result = is_stream_of_188bytes(buffer, size_read);
  printf("result = %d\n", result);

  fclose(fp);

  return 0;
}
