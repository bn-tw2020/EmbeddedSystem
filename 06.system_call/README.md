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

* 응용 프로그램이
