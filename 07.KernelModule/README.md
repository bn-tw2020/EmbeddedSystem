## Kernel Module

---

## [1] Kernel Module

```
Linux kernel
  1. 단일형 커널(Monolithic Kernel)
    운영 체제의 기능의 거의 모든 것이 단일 메모리 공간에서 수행
  2. 커널 기능 확장(e.g, 디바이스 추가)
    커널 컴파일이 요구됨
    
Kernel Module
  1. 커널을 확장하기 위해 사용하는 오브젝트 모듈
  2. 커널이 실행 중에 동적으로 로딩하여 커널과 링크함으로써 커널의 기능을 확장하여 사용할 수 있으며,
     불필요 시에 커널과의 링크를 풀고 메모리에서 제거할 수 있음
     * 커널 컴파일 없이 기능 확장 가능

> Kernel Module이 Monolithic Kernel의 단점을 커버할 수 있다.
```

### Kernel Module

  * 명시적인 커널 모듈 설치 및 제거 과정이 필요
    - insmod /rmmod 명령어를 해야 한다.
    
  * 디바이스 드라이버, 파일시스템, 네트워크 프로토콜 스택 등에 적용
    - 커널 경량화를 위해 반드시 필요
    - 임베디드 시스템: 제한적인 자원으로 인해 커널 등 시스템 소프트웨어 최소화가 필요

  * 커널 모듈은 <span style="color: blue;">커널 모드</span>에서 실행됨으로 커널 프로그래밍에 준하여 작성해야 한다.
  * 주의 사항
    - 무제한적인 메모리 접근 -> 메모리 접근 오류는 시스템에 치명적인 손상을 줄 수 있음.
    - 커널 함수 호출에 따른 에러 코드 검사
    - 실수 연산 및 MMX 연산 사용 불가
    - 제한적인 커널 스택 크기
      + 크기가 큰 배열 사용 회피 -> 동적 할당 방법을 이용
      + 재귀 호출 회피
    - 플랫폼 고려
      + 32bit 및 64bit 환경
      + 바이트 순서 -big-endian 또는 little-endian
      + 프로세서에 특화된 명령어 사용 회피
    - 버전 종속성
      + 커널 버전에 따라 모듈 버전이 다름
    - 커널 버전에 따라 지원되는 커널 함수의 종류나 프로토타입이 다르기 때문에 다른 버전의 커널에 모듈을 링크하려면 모듈 재컴파일이 필요
    - 모듈을 커널에 설치할 때에 버전 검사 수행
      + 커널 버전과 모듈 버전이 일치하지 않는 경우에 모듈을 설치 할 수 없다.
    - 커널 모듈 프로그램의 버전 정의
      + 커널 ver2.0 이상에서는 <linux/module.h> 헤드 파일에서 char kernel_version[] 변수에 커널 버전을 정의
      + 커널 모듈 프로그래밍에서 위의 헤더 파일을 including -> #include <linux/module.h>
      + 커널 버전 정의는 전체 모듈에서 한번만 정의되어야 한다.

#### 커널 모듈 구성

* 커널 기능을 확장한 함수와 자료구조로 구성
  - main() 함수는 없는 독립된 프로그램 모듈
* init_module() / cleanup_module() 함수 이용
  - init_module() 함수: 모듈이 설치될 때에 자동적으로 호출 / 모듈 초기화 등의 기능 수행
  - cleanup_module() 함수: 모듈을 삭제할 때에 자동적으로 호출 / 모듈 제거에 따른 cleanup 작업을 수행

```c++
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
/* global variables */
...
int init_module(void {}
void cleanup_module(void) {}
...
```

* 커널 ver2.4 이상에서는 module_init() / module_exit() 매크로 지원
  + 함수 이름에 의한 종속 관계를 해결
  + module_init() 매크로: startup 함수 등록
  + module_exit() 매크로: cleanup 함수 등록

```
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
...
int module_start() { /* 모듈이 설치될 때에 초기화를 수행하는 코드 */}
int module_exit() { /* 모듈이 제거될 때에 반환작업을 수행하는 코드 */ }
module_init(module_start);
module_exit(module_end);
```

#### 모듈 프로그램의 Makefile

* -c: 링커를 호출하지 않고 오브젝트 모듈만 새엇ㅇ
* -O: c최적화 옵션 지정(최대 O2)
* 컴파일 매크로: __KERNEL__, MODULE, LINUX

```
CC = arm_linux_gcc
INCLUDEDIR = /kernel_source_dir/include
CFLAGS := -c -O -Wall -D__KERNEL__ -DMODULE -DLINUX -I$(INCLUDEDIR)
OBJS = hello.o
all : $(OBJS)
clean : rm -f *.o *.~
```
