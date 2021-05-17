## Device Driver

---

## [1] 디바이스 드라이버

```
1. 물리적인 하드웨어 장치를 다루고 관리하는 소프트웨어
2. 유저 어플리케이션이 드라이버에게 요청을 하면, 드라이버는 하드웨어를 구동시켜 목적을 달성
3. major number와 minor number를 이용하여 각각의 디바이스들을 구분하여 사용
4. 디바이스와 시스템 메모리 간에 data의 전달을 담당하는 kernel 내부 기능
```

<div style="text-align:center;"><img src="https://user-images.githubusercontent.com/66770613/118423719-96a1c800-b700-11eb-89e2-5115bf8133ed.png"></div>  


* 프로그램들이 system call interface을 통해서 커널의 자원을 사용할 수 있다.
* 리눅스 시스템의 경우에는 디바이스들을 파일 시스템으로 관리를 한다.
* device driver interface을 통해서 char, block, network driver인지 결정을 한다.
* 드라이버 코드들이 파일 시스템으로 연결되는대 맵핑되는 역활이 DDI가 된다.

```
Example
taskA가 char Driver을 통해서 키보드와 터미널을 제어한다고 해보자
유저 프로그램은 system call을 통해 터미널과 키보드를 접근을 할 것이다
system call interface을 통해서 file system에 접근을 할 것이고
char driver로 연결이 된다. function들에 대해 mapping이 된다.
최종적으로 char driver에 의해서 키보드와 터미널에 제어가 된다.

이 모든 것이 유저 프로그램부터 하드웨어 레벨의 물리적인 디바이스까지 중간의 커널단에서 system call과 file system, driver 맵핑 관계로 연결이 된다.
system call을 통해서 커널의 명령들을 트리거 할 수 있는데, 트리거 할 수 있는 대상이 device file(device driver)이다.
device driver에 해당하는 system call을 받게 되면, 거기에 따라 mapping되는 driver의 function이 실행된다.
```

## [2] Device Driver 종류

* Char Driver
  * device를 file처럼 취급하고 접근하여 직접 read/write를 수행
  * data 형태는 stream 방식으로 전송
  * Example) console, keyboard, serial port driver ...
  * 등록 함수 명: register_chrdev()

* Block Driver
  * disk와 같이 file system을 기반으로 일정한 block 단위로 data read/write를 수행
  * Example) floppy disk, hard disk, CD-ROM driver 등
  * 등록 함수 명: register_blkdev()

* Network Driver
  * network의 physical layer와 frame 단위의 데이터를 송수신
  * Example) Ethernet device driver(eth0)
  * 등록 함수 명: register_netdev()
 
 ### 기본적인 디바이스 드라이버
 
 ```c++
 /* Header Files */
 #include <linux/kernel.h> // printf와 같은 기능의 커널 코드를 위한 함수
 #include <linux/module.h> // 필요한 header 모듈 소스가 include 해야함
 #include <linux/fs.h>     // "file system" 헤더는 디바이스 드라이버를 쓰는데 필요한 헤더 파일
 #include <linux/init.h>   // module_init()과 module_exit()가 정의됨
 
 /* Module 설치 시 초기화 수행 */
 module_init(init_function)     // insmod로 모듈을 적재하면 이 함수가 불려진다. 이 함수 안에서 register_chrdev() 커널 함수를 불러서 모듈을 적재함
 /* Module 제거 시 반환 작업 수행 */
 module_exit(cleanup_function)  // rmmod로 적재된 모듈을 삭제할 경우 이 함수가 불러진다. unregister_chrdev() 커널 함수를 불러서 모듈을 삭제함.
 
 /* Function Prototypes */
 static int device_open();    
 static int device_release();
 static ssize_t device_read();
 static ssize_t device_write();
 static int init_function();
 static void cleanup_function();
 
 
 /* Device Operations: application에서 쓰는 함수와 driver에서 쓰는 함수를 mapping */
 static struct file_operations device_fops = {
    open: device_open,
    release: device_release,
    read: device_read,
    write: device_write
  } ;
 ```
 
 1. 디바이스 드라이버 작성
 2. mknod을 통해서 디바이스에 대한 파일을 생성
 3. insmod을 통해서 모듈 적재한다.
 4. 그러면 mknod을 통한 파일과 insmod을 통해서 적재한 것이 mapping이 된다.


### Makefile

```
* Compile Options

%arm-linux-gcc -c -D__KERNEL__ -DMODULE -Wall -02 -o test_dd.o
-c: 모듈은 test_dd.o로 컴파일 되어야 하므로 -c 옵션 사용
-__KERNEL__ _DMODULE: 커널에서 동작하고 모듈을 사용된다는 뜻
-Wall: 경고를 다 출력해 준다는 의미
-02: Compile과정에서 inline함수를 확장할 때 최적화 함
```

## [3] Driver를 커널에 모듈로 등록 및 해제

```
Char Device Driver 등록 방법
 1. 외부와 device driver는 file interface(node를 의미)를 통해 연결 => mknod를 통해
 2. Device driver는 자신을 구별하기 위해 고유의 major number를 사용

장치의 등록과 해제
 1. 등록 : int register_chrdev(unsigned int major, const char *name, struct file_operations *fops)
       - Major: 등록할 major number. 0이면 사용하지 않는 번호 중 자동으로 할당
       - Name: device의 이름
       - Fops: device에 대한 file 연산 함수들

 2. 해제 : int unregister_chrdev(unsigned int major, const char *name)
```

<div style="text-align:center;"><img src="https://user-images.githubusercontent.com/66770613/118427452-2bf48a80-b708-11eb-988f-929f63737940.png"></div>  


### Major number와 Minor number

```
1. 장치를 구분하는 방법으로 둘을 이용하여 특정 장치를 구별
2. Major number: 커널에서 디바이스 드라이버를 구분하는 사용
3. Minor number: 디바이스 드라이버 내에서 필요한 경우 장치를 구분하기 위해 사용
4. 새로운 디바이스는 새로운 major number을 가져야 함
5. register_chrdev()로 장치를 등록할 때 mjaor number를 지정
6. 같은 major number가 등록되어 있으면 실패 (실패는 0 아래의 값을 반환한다)
7. Major와 Minor번호는 파일의 정보를 담고 있는 inode의 i_rdev에 16bit로 저장된다.
   상위 8bit는 major, 하위 8bit는 minor이다.
```

### mknod 명령으로 디바이스 드라이버에 접근할 수 있는 장치 파일 생성

* mknod [device file name] [type] [major] [minor]
 + Example) %mknod test_dd c 252 0 ==> c는 char device drive를 의미. block dvice driver은 b

### mdev_t: 장치의 major, minor number를 표현하는 자료 구조

* MAJOR(): kdev_t에서 major number를 얻어내는 매크로
 + Example) MAJOR(inode->i_rdev);

* MINOR(): kdev_t에서 minor number를 얻어내는 매크로
* cat /porc/devices 명령으로 현재 로드된 디바이스 드라이버 확인


### 커널에 등록할 때 쓰이는 명령어
```
insmod: 모듈을 설치(install)
rmmod: 실행 중인 모듈을 제거(unload)
lsmod: Load된 module들의 정보를 표시
depmod: 모듈들의 symbol들을 이용하여 Makefile과 유사한 dependency file을 생성
modprobe: depmod명령으로 생성된 dependency를 이용하여 지정된 디렉토리의 모듈들과 연관된 모듈들을 자동으로 load
modinfo: 목적 파일을 검사해서 관련된 정보를 표시
```

## [4] 실습

```
1. 만든 드라이버 소스를 test_dd.c / test application은 test_app.c, 그리고 makefile은 Makefile로 작성 한 후 저장한다.
2. make 명령어를 이용하여 위의 2개의 files을 컴파일 한다.
   %make
3. 생성된 test_dd.o와 test_app를 target board(Raspberry Pi)로 전송한다.
```

## [5] Summary

<div style="text-align:center;"><img src="https://user-images.githubusercontent.com/66770613/118428128-b8537d00-b709-11eb-8f0d-3e752c28c72e.png"></div>  


