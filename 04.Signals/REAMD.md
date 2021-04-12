## Signal

---

## [1] Signal Concepts

### Signals

```
1. 해당 프로세스에 대한 시스템 인터럽트의 일종이다.
2. 각각의 이름을 가지고 있으며,
3. 비동기적인 이벤트에 대한 핸들링하기 위한 방법이다.
```

> 비동기 이벤트 핸들링을 위한 Software Interrupt

### Conditions that generate a signal

```
1. Signals from terminal
2. Signals from hardware exceptions
3. Signals from software conditions
4. Signals from processes using kill() function
5. Signals from shells using kill command
```

### Actions associated with a signal

```
1. 시그널 확인(Catch signal)
    > 시그널 핸들러를 등록해야 한다.(Register a signal handler)
2. 만약, 시그널 핸들러가 등록되지 않았다면, default action을 취하게 된다.
    > Default action은 일반적으로 해당 프로세스를 종료하는 형태이다.
3. fork()를 하게되면, 자식이 부모의 시그널에 영향을 받는다.
```

즉, 정리를 하게되면,
1. Signal 이벤트를 결정하고,
2. Signal handler 함수를 등록하면, Signal event가 비동기적으로 발생한다. 
3. 그때마다 Signal에 대한 우리가 등록한 handler로 action이 취해지게 된다.

### Major signals

```
주로 사용되는 시그널 이벤트는
SIGALRM  SIGIO  SIGUSR1  
SIGCHLD  SIGKILL  SIGUSR2  
SIGCONT  SIGQUIT
SIGHUP  SIGSTOP
SIGINT  SIGTERM
```

### 시그널 핸들러 등록하기

* #include <signal.h>
* void (*signal(int signo, void (*func)(int))) (int);
* return: previous signal handler if OK, SIG_ERR(-1) on error

```c
#include <stdio.h>
#include <signal.h>
void SigIntHandler(int signo) {
    printf("Received a SIGINT signal\n");
    /* process the signal */
    exit(0);
}
void main() {
    signal(SIGINT, SigIntHandler);
    /* do something */
}
```

### Signal 실습

### Simple example for signal

```
arm-linux-gnueabihf-gcc -o sig1 sig1.c (or make sig1)
./sig1
^C
```

### Another example

```
arm-linux-gnueabihf-gcc -o sig2 sig.c (or make sig2)
./sig2&

pid=`pgrep sig2`
kill -USR1 $pid
kill -USR2 $pid
kill -TERM $pid

cf) &은 background에서 실행한다는 의미
```

## [2] Signal calls for signals

### Send a signal

```
프로세스나 프로세스 그룹에 Signal 보내기
    1. #include <sys/types.h>
    2. #include <signal.h>
    3. int kill(pid_t pid, int signo);
    4. return: 0 if Ok, -1 on error
    5. The first argument, pid
        > If pid > 0, send to the process of which the process ID is pid
        >    pid == 0, send to all processes of which the group ID equals the sender's
        >    pid < 0, send to all processes of which the group ID is the absoulte value of pid

Send a signal to itself
    1. int raise(int signo);
    2. return:0 if OK, -1 on error
```

### Signals과 관계있는 다른 시스템 콜

```
Sleep for the specified number of seconds
    1. #include <unist.h>
    2. unsigned int sleep(unsigned int seconds);
    3. return: 0 or number of seconds until previously set alarm
        (i.e., unslept time)
```

```
Set an alarm clock for delivery of a signal
    1. #include <unistd.h>
    2. unsigned int alarm(unsigned int seconds);
    3. return: 0 or number of seconds until previously set alarm
        (i.e., unslept time)
```

```
Wait for signal
    1. #include <unistd.h>
    2. int pause(void);
    3. return: -1 with errno set to EINTR
```

### Signals sleep 실습

### Make my own sleep system call using signal & pause

```
arm-linux-gnueabihf-gcc -o mysleep mysleep.c (or make mysleep)
./mysleep
```

### An example for periodic alarms

```
arm-linux-gnueabihf-gcc -o alarm alarm.c (or make alarm)
./alarm
```

---

## Interrupted system calls(시스템 콜 인터럽트)

```
If a process catch a signal while it has been blocked in a system call,
    1. The system call returns an error and errno is set to EINTR
    2. It's a good chance to wake up the blocked system call
    3. So, you have to handle it as follows:

    while(1)
    if((n = read(fd, buf, MAX_BUF)) < 0) {
        if(errno == EINTR)
            continue;
        /* handle other errors */
    }
```

### threads에서의 시그널 핸들링

쓰레드에서도 똑같이 시그널이 가능하다.

```
Which thread should handle signals?
    1. To the thread to which the signal applies
    2. To every thread in the process
    3. To certain threads in the process
    4. Assign a specific thread to receive all signals for the process
        > Solaris' implementation: main thread receives signals
```

### Signals in threads 실습

### Signals in threads

```
arm-linux-gnueabihf-gcc -o sig_thread sig_thread.c -lpthread (or make sig_thread)
./sig_thread
```



### Kill another thread(Thread cancellation)

```
Send a cancellation request
    1. #include <pthread.h>
    2. int pthread_cancel(pthrea_t tid);
    3. return: 0 if OK, non-zero on error

Set the type and state of cancellation request
    1. #include <pthread.h>
    2. int pthread_setcancelstate(int state, int *oldstate);
    3. int pthread_setcanceltype(int type, int *oldtype);
    4. return: 0 if OK, non-zero on error
    5. The first argument in pthread_setcancelstate(), state
        > PTHREAD_CANCEL_DISALE, PTHREAD_CANCEL_ENABLE(by default)
    6. The first argument in pthread_setcanceltype(), type
        > PTHREAD_CANCEL_ASYNCHRONOUS, PTHREAD_CANCEL_DIFERRED(by default)
```

### Thread Cancel 실습

```
arm-linux-gnueabihf-gcc -o cancel cancel.c -lpthread (or make cancel)
./cancel
```

## Summary

### Signals(시그널)

* 프로세스에 대한 소프트웨어 인터럽트
* 비동기적인 이벤트에 대한 핸들링이 가능하다.
* SIGINT, SIGALRM, SIGCHLD so on

### System calls for signals(시그널에 관련된 시스템 콜)

* signal : 핸들러 등록
* kill, raise : 시그널 발생
* sleep, alarm, pause : 시그널과 관련된 시스템 콜
* pthread_cancel : 쓰레드에서 시그널을 발생시키는 함수
* pthread_setcancelstate, pthread_setcanceltype
