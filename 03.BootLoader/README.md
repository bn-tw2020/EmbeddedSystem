## BootLoader

### 부트로더란

```
운영체제가 시동되기 이전에 미리 실행되면서 커널이 올바르게 시동되기 위해 필요한 모든 관련 작업을 마무리하고
최종적으로 운영체제를 시동시키기 위한 목적을 가진 프로그램이다.
```

### 라즈베리파이의 부팅 절차

![스크린샷 2021-04-05 오전 9 34 04](https://user-images.githubusercontent.com/66770613/113525831-1dd42a00-95f2-11eb-8898-64882058910e.png)

```
1. 보드에 전원이 들어오고 GPU가 활성화 된다(ARM Core - 비활성화 상태)
2. SoC내의 ROM에 있는 첫 번째 부트로더(Firmware)를 읽어들인다.
3. 첫 번째 부트로더가 SD카드 내의 FAT32(boot partition)에서 두 번째 부트로더인 bootcode.bin를 호출한다.
4. 두 번째 부트로더가 SD카드 내에 있는 config.txt를 읽고 실행한다.
5. 두 번째 부트로더가 세 번째 부트로더 start.elf를 호출, 실행하며, ARM-Core를 활성화시킨다.
6. ARM-Core가 활성화되면 네 번째 부트로더 kernel.img를 호출, 실행한다.

-> 따라서, 라즈베리파이는 구동하는 필요한 최소 파일은 bootcode.bin, start.elf, kernel.img이다.
```

### Role of Bootloader

```
bootcode.bin(두 번째 부트로더)
    ARM Core와 RAM의 비활성화 상태에서 실행된다.
    그로인해, 가장 먼저 활성화되는 GPU의 L2캐시에 로드되어 GPU에서 실행된다.
    두번째 부트로더인 bootcode.bin는 램을 활성화하고 SD카드로부터 GPU펌웨어(start.elf)를 읽어들인다.

start.elf(세 번째 부트로더)
    GPU의 펌웨어이며, 일종의 BIOS라고 할 수 있다.
    GPU와 ARM Core에서 사용하는 RAM을 배타적으로 할당.
    디바이스의 각종 주파수와 같은 하드웨어 제어 설정을 한다.(ARM Core를 깨우는 작업)

kernel.img(네 번쨰 부트로더)
    ARM Core활성화 시에 RAM에 로드되며 ARM Core는 kernel.img의 명령을 실행한다.

config.txt
    라즈베리파이의 하드웨어는 /boot 디렉터리에 있는 config.txt파일에 담겨있는 설정 값들에 의해 제어된다.
    이 파일은 라즈베리파이가 다양한 입/출력들을 어떻게 설정하고 SoC칩과 연결된 메모리 모듈이 실행되어야 하는속도를 알려준다.
```

## Cross Kernel Compile

```
Target System's Kernel Complie이며, Target System은 Rpi이다.
Rpi는 커널을 컴파일한다는 것 자체가 리소스가 낮고 효율적이지 못해서 느린다.
그래서 크로스 컴파일을 하게 된다.
크로스 컴파일을 하기 위해선 호스트PC와 타켓PC가 연결시키고,
타겟시스템의 컴파일 환경을 호스트PC에 구축을 하고(Cross Compile) 커널 소스를 CrossCompile를 하면(kernel.img)
타겟PC에 kernel.img을 부팅하게 하면 된다.
```

### 환경 구축에 필요한 툴 설치

```
sudo apt install make
    Makefile을 참조하여 빌드하는 명령어
    Makefile은 컴파일시 다양한 옵션을 정리해놓은 파일로 쉘스크립트로 구성됨.

sudo apt install git
    외부(다른 서버) 저장소에 위치한 파일/디렉토리 복사

sudo apt-get install bison flex libssl-dev bc
    크로스컴파일시 시스템에 필요한 툴

sudo clone https://github.com/raspberrypi/tools ~/tools
    크로스컴파일에 필요한 툴체인 설치
```

### 환경구축

```
1. vi ~/.bashrc
    가장 끝 줄(명령모드 G)에 밑의 한 개의 라인 추가
    :KERNEL = kernel7
    rpi 2이상 kernel7, rpi 1이하 kernel

2. echo PATH=\$PATH:~/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabinf-Raspbian/bin >> ~/.bashrc
    PATH 환경변수를 업데이트 시켜 크로스 컴파일에 필요한 파일 위치를 인식시키기
    호스트 시스템이 32bit : gcc-linaro-arm-linux-gnueabihf-Raspbian
                64bit : gcc-linaro-arm-linux-gnueabihf-Raspbian
    getconf LONG_BIT >> OS의 명령어형식 bit수 확인 명령어

3. source ~/.bashrc
    .bashrc파일의 스크립트 수행
    해당 명령어를 사용한 뒤 echo $KERNEL or echo $PATH를 사용하여 정상등록 확인
```

### 커널소스 내려받기

* 방법 (1)

```
git clone --depth=1 --branch rpi-[vir] https://github.com/raspberrypi/linux.git ~/linux

depth = 1 (최신 커널 소스를 내려 받기, 해당 옵션이 없을 시 저장소 전체를 내려 받음)
branch[vir] : [vir]의 커널소스 내려 받기
vir은 3.18.y이상으로 내려받기
```

* 방법 (2)
    * 웹 브라우저를 사용하여 내려받기
    * URL : github.com/raspberrypi/linux 접속
    * 3.18.y이상의 branch로 내려받기
    * unzip linux-rpi-[ver].zip명령어를 통해 압축해제


    * .config파일 생성
        * sudo make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf-bcm2709_defconfig
    
    
    * 타겟시스템의 커널컴파일 실행
        * make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- zlmage modules dtbs
        * Target시스템의 img file, modules, *.file, *.dtb, *.dtbo 생성
        * dtb(device tree blob) : 임베디드보드 하드웨어 설정내용을 커널로 넘기기 위한 구조체
    

    * 타겟 시스템의 모듈 install
        * mkdir ~/modules
            * 모듈 인스톨 시 저장해둘 dir
        * make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf INSTALL_MOD_PATH=~/modules modules_install
            * 타켓시스템 모듈 인스톨 수행
            * INSTALL_MOD_PATH변수는 명령어 수행시 생성파일들 저장할 경로
        * rm build source
            * 불필요한 링크파일 삭제


    * 타겟 시스템으로 컴파일된 파일 및 디렉토리 전송
        * scp linux-rpi-[ver]/arch/arm/boot/zlmage pi@xx.xx.xx.xx:/home/pi
        * scp linux-rpi-[ver]/arch/arm/boot/dts/*.dt pi@xx.xx.xx.xx:/home/pi
        * scp linux-rpi-[ver]/arch/arm/boot/dts/overlays/*.dtb* pi@xx.xx.xx.xx:/home/pi
        * scp linux-rpi-[ver]/arch/arm/boot/dts/overlays/README pi@xx.xx.xx.xx:/home/pi
        * scp -r ~/modules/lib/modules/[ver]/ pi@xx.xx.xx.xx:/home/pi

    
    * 타겟 내부에서 호스트에게서 받은 파일들을 해당경로로 이동(작업 디렉토리 : /home/pi)
        * mv ./zlmage /boot/kernel7.img
            * 생성한 커널이미지를 boot내의 kernel7.img로 교체
        * mv ./[modules ver]/ /lib/modules/.
            * 생성한 모듈 이동
        * mv ./*.dtb /boot/.
        * mv ./*.dtb* /boot/overlays/.s
        * mv ./README /boot/oerlays/.
        * sudo reboot
            * 타겟머신 리부팅
        * uname -r
            * 변경된 커널버전 확인하기

## Summary

1. Cross Kernel Compile
2. Cross Compile 환경구축
3. Kernel src 준비
4. Cross Compile
5. img -> TargetPC로 전송
6. 알맞은 위치에 배치
7. 재부팅
