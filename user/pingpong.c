#include "kernel/types.h"
#include "user/user.h"

int main(){
  int p1[2],p2[2];
  char buffer[] = {'X'};	//来回传输的字符数组
  long length = sizeof(buffer);	//传输字符数组的长度
  pipe(p1);	//父进程写，子进程读的pipe
  pipe(p2);	//子进程写，父进程读的pipe
  if(fork() == 0){
    close(p1[1]);	//关掉不用的p1[1]、p2[0]
    close(p2[0]);
    if(read(p1[0],buffer,length) != length){	//子进程从pipe1的读端，读取字符数组
      printf("a--->b read error!");
      exit(1);
    }
    printf("%d: received ping\n",getpid());	//打印读取到的字符数组
    if(write(p2[1],buffer,length) != length){	//子进程向pipe2的写端，写入字符数组
      printf("a<---b write error!");
      exit(1);
    }
    exit(0);
  }
  close(p1[0]);	//关掉不用的p1[0]、p2[1]
  close(p2[1]);
  if(write(p1[1],buffer,length) != length){	//父进程向pipe1的写端，写入字符数组
    printf("a--->b write error!");
    exit(1);
  }
  if(read(p2[0],buffer,length) != length){	//父进程从pipe2的读端，读取字符数组
    printf("a<---b read error!");
    exit(1);
  }
  printf("%d: received pong\n",getpid());	//打印读取到的字符数组
  wait(0);	//等待子进程退出  
  exit(0);
}
