#include <stdio.h>
#include "../lib/go_concurrency.xh"
#include <stdlib.h>
#include <pthread.h>

void *spawn_test(int a, int b) {
  printf("%d,%d\n",a,b);
}

int main(){
  Channel<int>* ch = chan_open<int>();
  spawn(spawn_test, 5, 4);
  select {
    case <-ch: printf("Channel One\n");
    default: printf("Default\n");
  }
  chan_close<int>(ch);
}


