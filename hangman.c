#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

int main() {
  // 맞출 단어 
  char word[128];
  //fflush(stdout);
  
  scanf(" %s", word);

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
      if (word[k] == guess) {
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
