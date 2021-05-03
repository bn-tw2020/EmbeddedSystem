## System Call

<img width="730" alt="스크린샷 2021-05-03 오전 11 40 29" src="https://user-images.githubusercontent.com/66770613/116837203-5f152500-ac04-11eb-85c8-dd3fe1e8dd54.png">

* 시스템 콜은 유저 레벨과 커널 레벨 사이에 존재합니다.
* 커널 레벨에서의 인터페이스를 제공하는 것
* 유저 레벨에서는 시스템 콜을 통해서 여러 하드웨어 접근이 가능하다.
* 파일 시스템, 디바이스 드라이버, 프로세스 관리 등 리소스 제어가 가능하다.
* 유저 레벨에서는 커널 레벨의 기능을 활용하 수 없다.
  * 컴퓨터 시스템에서의 자원을 접근해서 해당 자원을 제어해야한다면 바로 시스템 콜 인터페이스를 사용하며 된다.

```
1. User mode process와 kernel 간의 interface
  * User mode process는 kernel 영역에 접근할 수 없다.
    - 커널의 자료구조 및 하드웨어에 대한 접근 불가 ⟹ kernel resource 제어 불가능
  * User mode process가 kernel이 가지고 있는 시스템 상태정보를 열람하거나 하드웨어에 접근하여 제어하기 위한 커널과의 통신이 필요하다.
```


## POSIX API & System Calls

### 1. POSIX API

```
1. POSIX API(Application Programming Interface)
  * 유닉스 기반의 표준 운영체제 인터페이스
  * Application이 시스템에 서비스를 요청할 때 어떤 함수를 사용해 하는지 지정
  * 표준을 두어 다른 시스템에 응용 프로그램을 porting 하는 것ㅇ 용이하게 하기 위한 목적
  * open(), close(), write(), read() 등

2. System Call
  * 소프트웨어 인터럽트를 통해 커널에 서비스를 요청
  * 리눅스에서는 POSIX API를 준수하느 라이브러리 함수 안에서 시스템 함수를 호출
```


<img width="730" alt="스크린샷 2021-05-03 오전 11 53 35" src="https://user-images.githubusercontent.com/66770613/116837634-34c46700-ac06-11eb-9bb7-371c3e6525b7.png">

```
1. 응용 프로그램이 표준 라이브러리(POSIX API)를 호출합니다.
2. 표준 라이브러리는 시스템 콜에 인터페이스에 접근합니다.
3. 시스템 콜 인터페이스는 시스템 콜 핸들러와 시스템 콜 서비스 루틴으로 이루어진다.
4. 시스템 콜의 해당하는 번호의 시스템 콜 핸들러가 커널에 등록되어있는 서비스 루틴들이 동작합니다.
```

### 2. System Call

<img width="692" alt="스크린샷 2021-05-03 오후 12 10 24" src="https://user-images.githubusercontent.com/66770613/116838225-9685d080-ac08-11eb-9657-068a8a106ecf.png">


```
1. Linux에서의 system call 처리
  * Interrupt 처리 메케니즘 사용

2. Interrupt
  * 주변 장치와 커널이 통신하는 방식 중 하나로 주변 장치가 자신에게 발생하는 비동기적 사건을 kernel에게 알리는 방법

> 주변 장치의 이벤트가 발생했을 때 그 이벤트에 대한 인터럽트 핸들러를 찾고 핸들러를 통해서 서비스 루틴의 동작이 호출됩니다.
> 이 과정을 통해서 컴퓨터 시스템이 비동기적으로 발생되는 이벤트를 처리할 수 있습니다.
```

<img width="720" alt="스크린샷 2021-05-03 오후 12 16 02" src="https://user-images.githubusercontent.com/66770613/116838450-583ce100-ac09-11eb-8052-d5558c4e64d0.png">


## 실습

* 목적
  * 기존 커널에서 제공하지 않는 서비스를 유저 어플리케이션에서 제공
    * 새로운 시스템 콜을 만듬

* 단계
  * 커널 수정
    * System call 번호 할당
    * System call 호출 테이블 등록
    * System call 호출 함수 구현
    * Kernel 컴파일 및 target board 적재
  * User application 제작
    * 새로 구현한 system call을 사용하는 Application 제작
    * 라이브러리 작성
    * Target board에 적재

## Reference

![시스템 콜 참고 영상](https://www.youtube.com/watch?v=lhToWeuWWfw)