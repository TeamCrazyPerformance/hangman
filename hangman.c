#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#define BUF_SIZE 1024

typedef struct {
    char **data;
    size_t used;
    size_t size;
} Array;

void initArray(Array *a, size_t initialSize) {
  a->data = malloc(initialSize * sizeof(char *));
  a->used = 0;
  a->size = initialSize;
}

void freeArray(Array *a) {
  size_t i;

  for (i = 0; i < a->used; ++i)
    free(a->data[i]);

  free(a->data);
  free(a);
}

void insertArray(Array *a, char* element) {
  if (a->used == a->size) {
    void *pointer;

    a->size *= 2;
    pointer = realloc(a->data, a->size * sizeof(char *));
    if (a->data == NULL) {
      freeArray(a);
      exit(1);
    }
    a->data = pointer;
  }

  if (element != NULL) {
    size_t length;

    length = strlen(element);
    a->data[a->used] = malloc(1 + length);
    if (a->data[a->used] != NULL)
      strcpy(a->data[a->used++], element);
  }
  else
    a->data[a->used++] = NULL;
}

int main() {
  // csv 파일로 부터 단어를 로드
  FILE *file;
  char *line = NULL;
  size_t length = 0;
  ssize_t read;

  file = fopen("voca.csv", "r");
  if (file == NULL)
    exit(-1);

  // 단어를 배열로 관리
  Array words, descriptions;
  initArray(&words, 10);
  initArray(&descriptions, 10);

  while ((read = getline(&line, &length, file)) != -1) {
    char *w = strtok(line, ",");
    char *d = strtok(NULL, ",");
    insertArray(&words, w);
    insertArray(&descriptions, d);
  }

  fclose(file);

  // 단어를 랜덤 선택
  srand(time(NULL));
  int random = rand() % words.used;//TODO 갯수
  char *word = words.data[random];

  // show 이 true 면 글자 보이고 false 면 감추기. 모두 false 로 초기화
  int len = strlen(word);
  int show[len];
  int validGuess=0; //이번에 맞았는지
  int trials=5; //남은 시도회수
  int i,j,k,m;

  for (i=0; i < len; ++i) {
    show[i] = 0;
  }

  // 글자를 맞춰볼때 마다 반복
  int end = 0;
  while (!end) {
    // 맞춘만큼 보여주는 단어
    printf("%s\n", descriptions.data[random]);
    printf("Answer : ");
    for(j=0; j < len; ++j) {
      if (show[j]) {
        printf("%c", word[j]);
      }
      else {
        printf("_");
      }
    }
    printf("\n");

    printf("Guesses Left: %d\n",trials);
    // 다음 시도
    char guess;
    printf("Letter: ");
    fflush(stdout);
    scanf(" %c", &guess);

    // 맞췄으면 글자 보이게
    for(k=0; k < len; ++k) {
      if (word[k] == guess||word[k]==(guess-32)||word[k]==(guess+32)) {
        show[k] = 1;
        validGuess=1;
      }
    }
    if(!validGuess){trials--;validGuess=0;}
    //이겼는지 확인
    if(!trials){
      printf("No more guesses left!\n");
      return 0;
    }
    end = 1;
    for(m = 0; m < len; ++m) {
      if (!show[m]) {
        end = 0;
        break;
      }
    }
  }
  // 성공
  printf("Correct, the answer is : %s.\n", word);
  return 0;
}
