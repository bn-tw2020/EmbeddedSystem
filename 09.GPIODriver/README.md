## GPIO Driver

---

## [1]. GPIO Driver

* GPIO라는 핀을 통해서 하드웨어를 컨트롤할 수 있습니다.

![image](https://user-images.githubusercontent.com/66770613/119289459-987e0500-bc85-11eb-8582-77a5475a118e.png)  

```
사용할 GPIO핀은 18, 19번이다.
18번으로 LED를 제어하며, 19번으로 Switch제어를 할 것이다.
```

### 회로구성도

![image](https://user-images.githubusercontent.com/66770613/119289582-db3fdd00-bc85-11eb-837b-844c6e63d949.png)  

### Slide Switch 동작원리

![image](https://user-images.githubusercontent.com/66770613/119289732-18a46a80-bc86-11eb-92ae-f1d9774b0fb0.png)  

```
슬라이드 스위치는 세 개의 단자가 존재한다.

좌측부터 번호를 매겨 1번, 2번, 3번이라고 한다면 스위치를 좌측으로 이동시키면 1번과 2번이 서로 연결되고
                                             스위치를 우측으로 이동시키면 2번과 3번이 서로 연결된다.
Example) 1번에 LED에 연결되는 전원 단자를 연결하고 2번에 그라운드를 연결했을 때
         스위치가 좌측에 있을 경우 전류가 흘러 LED의 불빛이 켜지지만
         스위치를 우측으로 이동시키면 전류가 흐르지 않아 LED의 불빛이 꺼집니다.
```

### BCM2835 데이터시트

* 라즈베리파이4 보드가 GPIO로 쓰기위해선 BCM2835 라이브러리를 사용한다.

```
Register View
1. 디바이스 드라이버에서는 GPIO를 제어하는 레지스터에 접근하여 직접 레지스터의 값을 바꿈으로써 특정 GPIO핀의 제어가 가능하다.
2. 라즈베리파이 B+의 Soc인 BCM2835의 데이터시트를 참조하여 필요한 레지스터들을 확인한다.
3. 표의 첫 필드인 Address는 각 레지스터들의 주소가 담긴 필드이다.
4. 이 실습에서 필요한 레지스터 필드의 이름은 GPFSEL1, GPSET0, GPCLR0, GPLEV0으로 각 4개의 레지스터 필드들을 다룬다.
```

![image](https://user-images.githubusercontent.com/66770613/119290165-fc54fd80-bc86-11eb-94ac-5d0f55ecc9c0.png)  

```
1. GPFSEL1
  레지스터주소: 0x7E20 0004
  GPFSEL레지스터 필드는 GPIO핀의 모드를 INPUT이나 OUTPUT으로 지정해주는 필드이다.
    Example) 19번 GPIO를 OUTPUT모드로 지정해주고 싶을 땐 총 32bit중 27~29번째 bit를 001로 설정해준다.
  레지스터는 32bit로 구성되어 있음을 확인가능하며, 32bit의 명령어 형식을 사용하는 것을 알 수 있다.
  32bit임으로 이 필드는 10개의 GPIO에 특정 모드를 할당할 수 있다.
  
2. GPSET0
  레지스터주소: 0x7E20 001C
  GPSET필드는 OUTPUT모드로 설정된 GPIO핀에 해당하는 비트를 SET을 시켜 HIGH값을 출력해내는 필드이다.
    Example) GPFSEL 1필드를 통해 19번 GPIO가 OUTPUT모드로 설정되고, 
             GPSET0필드에서는 19번 GPIO에 해당하는 bit에 1이라는 값을 설정하게되면 19번 GPIO에서는 HIGH값을 출력하게 된다.
             19번 GPIO에 해당하는 bit는 32bit 중 20번째에 해당하는 bit이다.
             이 필드는 32bit로 이루어져 있음으로 31번 GPIO까지 제어가 가능하다(0 ~ 31번)
             
3. GPCLR0
  레지스터주소: 0x7E20 0028
  GPCLR필드는 OUTPUT모드로 설정된 GPIO핀에 해당하는 비트를 SET시켜 LOW값을 출력해내는 필드
  GPIO핀에 LOW를 출력해내는 방법은 이전의 GPSET필드의 방법과 동일하다.
  이 필드는 32bit로 이루어져 있음으로 31번 GPIO까지 제어가 가능하다.
  
4. GPLEV0
  레지스터주소: 0x7E20 0034
  GPLEV필드는 INPUT모드로 설정된 GPIO핀에 입력되는 값을 읽어 들이는 필드이다.
    Example) GPFSEL을 통해 INPUT모드로 설정된 GPIO 19번 핀에 스위치 등으로 인해서 HIGH값을 입력받게 되면
             GPLEV필드 내의 GPIO 19에 해당하는 비트가 0에서 1로 설정되고,
             다시 LOW값이 입력 받게 되면 비트가 1에서 0으로 바뀌게 된다.
```

### 레지스터주소가 매핑된 메모리의 물리적 주소

```
1. BCM2835데이터 시트에 따르면 RAM의 0x2000 0000 ~ 0x20FF FFFF에 해당하는 물리주소에 주변기기들을 다루는 
   레지스터의 주소(0x7E00 0000 ~ 0x7Enn nnnn)들이 MMU를 통해 매핑되어 관리되고 있다.

2. 우리는 레지스터의 주소에 직접접근하여 레지스터를 제어하느 ㄴ것이 아니라 RAM의 물리주소를 통해 레지스터의 주소를 가상주소로 
   매핑하여 레지스터를 제어하는 실습을 진행한다.
   
3. 즉, 우리가 알아야 될 RAM의 물리적인 주소인 0x20000 0000을 알고 있어야 한다. Rpi4는 0xfe00 0000이다.
   주변기기의 물리주소(0x2000 0000)에 0x0020 0000의 주소를 더하게 되면, 0x2020 0000이 되는데, 이 주소가 RAM내의 GPIO 물리주소가 된다.
   Rpi4는 0xfe20 0000이다.
```

### MMU를 통해 매핑된 상태

![image](https://user-images.githubusercontent.com/66770613/119293384-480aa580-bc8d-11eb-9446-cce66a6088d0.png)  

```
1. 우리가 사용하게 되는 메모리 영역들이 사실 상 ARM 가상 주소영역을 통해서 ARM의 MMU형태로 관리되는 물리적 영역을 매핑되는 형태를 사용하게 된다.

2. SDRAM, IO 영역에 해당하는 부분이 VCARM MMU을 통해서 ARM 물리적 주소와 맵핑되어있는 구조이다.

3. 우리가 사용하는 레지스터 주소가 물리적 주소와 가상 메모리와 맵핑되어 있다. 
   가상 메모리 영역에 데이터를 쓰게 되면 맵핑되어 있는 물리적 영역에 데이터를 쓸 수 있게 된다
   역으로, 물리적 메모리에 쓰여진 값들을 맵핑되어 있는 가상 메모리를 통해서 읽어서 시스템에서 쓸수도 있다.

> 요약하면, 가상메모리 영역이 물리적 메모리 영역이 MMU 맵핑을 통해 처리되기 때문에 시스템 메모리 영역까지 자세히 알 필요가 없다.
```

### 실습 시 사용하는 프로그램

```
gpio_dev.c : GPIO 디바이스 드라이버를 생성
gpio_app.c : GPIO 디바이스 드라이버를 활용하는 User의 APP

두 개의 소스 코드를 사용하여 GPIO 18번에는 LED를 
GPIO 19번에는 Switch를 연결하여 GPIO 디바이스 드라이버에서 각 GPIO에 레지스터를 통해 접근하여 제어하는 실습을 진행한다.
```

```
gpio_dev.c

1. 헤더파일
  #include <linux/module.h>
  #include <linux/kernel.h>
  #include <linux/init.h>
  #include <linux/io.h>
  #include <linux/fs.h>
  #include <linux/uaccess.h>
  
module.h, kernel.h, init.h : 모듈프로그래밍을 할 시 필수적인 헤더 파일
io.h : RAM의 물리주소를 가상주소로 매핑시켜주는 함수인 ioremap()을 참조하는 헤더 파일
fs.h : 캐릭터 디바이스 드라이버를 제작할 시 필요로한 헤더파일
uaccess.h : 커널공간과 유저공간의 데이터 송수신을 위한 헤더 파일
            copy_to_user()함수를 참조하는 헤더 파일
            
2. 전역변수로 선언한 변수 및 상수
  GPIO_BASE : GPIO의 물리주소(보드의 버전마다 물리주소가 다름)
  GPFSEL1, GPSET0, GPCLR0, GPLEV0 : GPIO핀을 제어하는 레지스터들의 주소
  *gpio_base : GPIO의 물리주소를 가상주소로 매핑시켜 줄 시 매핑된 주소를 가르키는 포인터 변수
  *gpfsel1, *gpset0, *gpclr0, *gplev0 : GPIO핀을 제어하는 가상주소를 담는 포인터 변수
  
3. init() : 레지스터 가상주소 매핑, GPIO모드 설정, 캐릭터 디바이스 드라이버 생성
4. open() : GPIO 18 LED ON
5. close() : GPIO 19 LED OFF
6. read() : GPIO 19 READ
7. file_operations : 디바이스 파일이 수행할 함수들을 각 시스템 콜에 매핑
8. exit() : 캐릭터 디바이스 드라이버를 커널에서 제거
```

![image](https://user-images.githubusercontent.com/66770613/119294519-99b42f80-bc8f-11eb-91ad-124f33520cd4.png)  

![image](https://user-images.githubusercontent.com/66770613/119294699-08918880-bc90-11eb-97d5-c32cd6e3f6a2.png)  

![image](https://user-images.githubusercontent.com/66770613/119294787-3c6cae00-bc90-11eb-9017-2f12de024e59.png)  

![image](https://user-images.githubusercontent.com/66770613/119294891-763db480-bc90-11eb-9d6b-aaeebe50916d.png)  

![image](https://user-images.githubusercontent.com/66770613/119294989-b2711500-bc90-11eb-9589-18d0a54eaa9d.png)


```
gpio_app.c : GPIO 디바이스 드라이버를 활용하여 GPIO를 제어하는 App
```

![image](https://user-images.githubusercontent.com/66770613/119295093-ec421b80-bc90-11eb-9320-b5229b90d891.png)  

![image](https://user-images.githubusercontent.com/66770613/119295174-1c89ba00-bc91-11eb-9f07-4e6cb9b91e54.png)


### GPIO 디바이스 드라이버 실습 실행

```
1. make 실행
2. gpio_dev_driver.zip파일을 zip 명령어를 사용하여 압축을 푼 뒤 
   example 디렉토리에서 make 명령을 수행시켜서 gpio_app 파일과 gpio_dev.ko파일을 생성시킨다. 
3. 만약, 라즈베리파이가 2이상 버전의 보드를 사용하여 실습을 진행할 경우에는 
   Makefile내용 중 컴파일러(CC)을 arm-linux-gnueabihf-gcc로 변경하고 make를 실시한다. 
4. gpio_app, gpio_dev.ko파일들을 scp명령어를 이용하여 라즈베리파이로 전송시킨다.
   scp [파일명] [target_user명]@[target_user ip]:[target_user_디렉토리]
5. ssh로 Host PC에서 라즈베리파이에 원격제어를 실시
   ssh [target_user명]@[target_user_ip]
6. 모듈을 커널에 등록, 디바이스 파일 생성, gpio_app 생성한다.
7. 이 후의 명령어는 라즈베리파이에서 실행한다.
8. sudo insmod gpio_dev.ko : 커널에 GPIO 디바이스 드라이버 모듈을 적재
9. sudo mknod /dev/gpio_dev c 300 0 : GPIO 디바이스 드라이버가 사용할 디바이스 파일 생성
10. sudo ./gpio_app : gpio_app프로그램을 실행하여 GPIO 18, 19핀들을 제어한다.
```
## [2]. Summary

```
캐릭터 디바이스 드라이버가 가장 기본적인 디바이스 드라이버라고 생각하면된다.  
캐릭터 디바이스 드라이버를 통해서 GPIO을 통해 핀 input, output를 제어할 수 있게 되며  
input, output을 통해 외부 장치를 제어할 수 있다.  
디바이스 드라이버를 하드웨어 장치를 위한 프로그램이라고 생각하면 될 것이다.  
```
