## Cross Compile

---

## [1]. 교차 개발 실습

<img src="https://user-images.githubusercontent.com/66770613/115167016-d0c37e00-a0f0-11eb-895a-fb35888a723f.png" width="700px" height="400px"/>  

### 실습

> 02.process 참고

```
fork.c
  Cross-compile fork.c (or make fork)
  move "fork" to the target board
  $./fork
 
 exit.c
  Cross-compile exit.c (or make fork)
  move "exit" to the target board
  $./exit
  
 wait.c / race.c / worace.c / thread.c (-lpthread)
```

### 실습

> 04. signal 참고

```
sig1.c
  Cross-compile sig1.c (or make sig1)
  $./sig1 (in the target board)
  ^C
  
sig2.c
  Corss-compile sig2.c (or make sig2)
  $./sig2& (in the target board)
  $pid=`pgrep sig2`
  $kill -USR1 $pid
  $kill -USR2 $pid
  $kill -TERM $pid
  
 mysleep.c / alarm.c / sig_thread.c (-lpthread) / cancel.c (-lpthread)
```

## Summary

1. Makefile 수정
2. Compile (arm compile) 
3. Rpi로 이동
4. ssh로 Rpi 접속
5. 실행
