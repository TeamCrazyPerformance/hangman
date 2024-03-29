#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#define BUF_SIZE 1024

typedef struct {
    char **data;
    int used;
    int size;
} Array;

void initArray(Array *a, int initialSize) {
  a->data = malloc(initialSize * sizeof(char *));
  a->used = 0;
  a->size = initialSize;
}

void freeArray(Array *a) {
  int i;

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
    int length;

    length = strlen(element);
    a->data[a->used] = malloc(1 + length);
    if (a->data[a->used] != NULL)
      strcpy(a->data[a->used++], element);
  }
  else
    a->data[a->used++] = NULL;
}

int main() {
  system("clear");

  // csv 파일로 부터 단어를 로드
  FILE *file;
  char *line = NULL;
  int length = 0;
  sint readsize;

  int currentScore=0,highScore;

  file = fopen("voca.csv", "r");
  if (file == NULL)
    exit(-1);

  // 단어를 배열로 관리
  Array words, descriptions;
  initArray(&words, 10);
  initArray(&descriptions, 10);

  while ((readsize = getline(&line, &length, file)) != -1) {
    char *w = strtok(line, ",");
    char *d = strtok(NULL, "");
    insertArray(&words, w);
    insertArray(&descriptions, d);
  }

  fclose(file);

  //하이스코어
  int fd = open("highscore", O_RDWR | O_CREAT, 0666);
  char buffer[BUF_SIZE];
  read(fd, buffer, BUF_SIZE);
  highScore = atoi(buffer);

  while(1) {
    // 단어를 랜덤 선택
    srand(time(NULL));
    int random = rand() % words.used;
    char *word = words.data[random];

    // show 이 true 면 글자 보이고 false 면 감추기. 모두 false 로 초기화
    int len = strlen(word);
    int show[len];
    int validGuess = 0; //이번에 맞았는지
    int trials = 5; //남은 시도회수
    int i, j, k, m;

    for (i = 0; i < len; ++i) {
      show[i] = 0;
    }

    // 글자를 맞춰볼때 마다 반복
    int end = 0;

    while (!end) {
      // 맞춘만큼 보여주는 단어
      printf("High Score : %d\n", highScore);
      printf("Current Score : %d\n", currentScore);
      printf("\n%s\n", descriptions.data[random]);
      printf("Answer : ");
      for (j = 0; j < len; ++j) {
        if (show[j]) {
          printf("%c", word[j]);
        } else {
          printf("_");
        }
      }
      printf("\n");

      printf("Guesses Left: %d\n", trials);
      // 다음 시도
      char guess;
      printf("Letter: ");
      fflush(stdout);
      scanf(" %c", &guess);

      // 맞췄으면 글자 보이게
      for (k = 0; k < len; ++k) {
        if (word[k] == guess || word[k] == (guess - 32)) {
          show[k] = 1;
          validGuess = 1;
        }
      }

      if (validGuess == 0)trials--;
      validGuess = 0;
      //이겼는지 확인
      system("clear");
      if (!trials) {
        printf("No more guesses left!\n");
        break;
      }
      end = 1;
      for (m = 0; m < len; ++m) {
        if (!show[m]) {
          end = 0;
          break;
        }
      }
    }

    if (!trials) {
      // 실패
      printf("High Score : %d\n", highScore);
      printf("Current Score : %d\n", currentScore);

      // TODO save highscore
      sprintf(buffer, "%d", highScore);
      write(fd, buffer, BUF_SIZE);
      exit(0);
    } else {
      // 성공
      printf("Correct, the answer is : %s.\n", word);
      currentScore++;

      if(highScore < currentScore) {
        highScore = currentScore;
      }

      sleep(5);
      system("clear");
    }
  }
}
