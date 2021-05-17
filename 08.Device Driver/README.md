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

## [2]. Device Driver 종류

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
