# 출제 의도

Xor 역연산 과정을 일일히 계산한다는 것은 꽤나 귀찮은 일인 것을 깨달았으면 좋겠다.

더 나아가서는 문제 해결을 위한 스크립트를 직접 작성해 보게 되는 계기가 되었으면 좋겠다.

# Whoami 문제 풀이

## 1. 문제 파악

처음 아무런 인자 없이 바이너리 실행시 다음과 같은 출력을 얻는다

```
gohome0001@ubuntu:~$ ./whoami
usage : ./whoami your_name
gohome0001@ubuntu:~$
```

그래서 요구사항대로 이름을 넣어서 실행해 보았다.

```
gohome0001@ubuntu:~$ ./whoami gohome0001
Are you an 4ws0me_h@cker?
You're not..
gohome0001@ubuntu:~$
```

내가 `4s0me_h@cker` 인지 아닌지 결과값을 낸다.

```
gohome0001@ubuntu:~/study/pcap$ ./whoami 4ws0me_h@cker
Are you an 4ws0me_h@cker?
You're not..
gohome0001@ubuntu:~/study/pcap$ 
```

인정받으려면, 특정한 문자열을 인자로 넘겨주어야 할 것만 같다.

- 문제에서 argv[1]을 요구. 그렇다면, argv[1] 입력값에 따라 출력이 다를 것이다.

## 2. 문제 분석


### main함수 disassemble
```
(gdb) disass main
Dump of assembler code for function main:
   0x08048524 <+0>:     lea    ecx,[esp+0x4]
   0x08048528 <+4>:     and    esp,0xfffffff0
   0x0804852b <+7>:     push   DWORD PTR [ecx-0x4]
   0x0804852e <+10>:	push   ebp
   0x0804852f <+11>:	mov    ebp,esp
   0x08048531 <+13>:	push   ebx
   0x08048532 <+14>:	push   ecx
   0x08048533 <+15>:	sub    esp,0x30
   0x08048536 <+18>:	mov    ebx,ecx
   0x08048538 <+20>:	cmp    DWORD PTR [ebx],0x1
   0x0804853b <+23>:	jg     0x804855d <main+57>
   0x0804853d <+25>:	mov    eax,DWORD PTR [ebx+0x4]
   0x08048540 <+28>:	mov    eax,DWORD PTR [eax]
   0x08048542 <+30>:	sub    esp,0x8
   0x08048545 <+33>:	push   eax
   0x08048546 <+34>:	push   0x80486b0
   0x0804854b <+39>:	call   0x8048360 <printf@plt>
   0x08048550 <+44>:	add    esp,0x10
   0x08048553 <+47>:	sub    esp,0xc
   0x08048556 <+50>:	push   0x0
   0x08048558 <+52>:	call   0x8048380 <exit@plt>
```
함수 프롤로그 전에, [esp+4] 값에 접근한다. call로 인한 ret 주소가 들어있을 위치각 esp므로, esp+4 는 함수의 첫번째 매개변수를 뜻할 것이다.

즉 main 함수의 첫 매개변수, argc값을 가져와서 1보다 크면 exit(0)을 면하게 된다.

한가지 더, ebp를 push 하기 전에, ecx-0x4값을 스택에 push 한다.
```
   0x0804855d <+57>:	sub    esp,0xc
   0x08048560 <+60>:	lea    eax,[ebp-0x2c]
   0x08048563 <+63>:	push   eax
   0x08048564 <+64>:	call   0x80484cb <init_buf>
```
함수 이름이 init_buf이다. 그리고 지역변수의 주소를 인자로 받는다.<br>

----------

### init_buf 분석

```
(gdb) disass init_buf
Dump of assembler code for function init_buf:
   0x080484cb <+0>:     push   ebp
   0x080484cc <+1>:     mov    ebp,esp
   0x080484ce <+3>:     sub    esp,0x18
   0x080484d1 <+6>:     mov    BYTE PTR [ebp-0x16],0x4d
   0x080484d5 <+10>:	mov    BYTE PTR [ebp-0x15],0x56
   0x080484d9 <+14>:	mov    BYTE PTR [ebp-0x14],0x1e
   0x080484dd <+18>:	mov    BYTE PTR [ebp-0x13],0x11
   0x080484e1 <+22>:	mov    BYTE PTR [ebp-0x12],0x34
   0x080484e5 <+26>:	mov    BYTE PTR [ebp-0x11],0x37
   0x080484e9 <+30>:	mov    BYTE PTR [ebp-0x10],0x2b
   0x080484ed <+34>:	mov    BYTE PTR [ebp-0xf],0x4
   0x080484f1 <+38>:	mov    BYTE PTR [ebp-0xe],0x5
   0x080484f5 <+42>:	mov    BYTE PTR [ebp-0xd],0x44
   0x080484f9 <+46>:	mov    BYTE PTR [ebp-0xc],0x2c
   0x080484fd <+50>:	mov    BYTE PTR [ebp-0xb],0x4
   0x08048501 <+54>:	mov    BYTE PTR [ebp-0xa],0x5d
   0x08048505 <+58>:	mov    BYTE PTR [ebp-0x9],0x0
```
지역변수 공간에 값들을 채워넣는다.
```
   0x08048509 <+62>:	sub    esp,0x4
   0x0804850c <+65>:	push   0xe
   0x0804850e <+67>:	lea    eax,[ebp-0x16]
   0x08048511 <+70>:	push   eax
   0x08048512 <+71>:	push   DWORD PTR [ebp+0x8]
   0x08048515 <+74>:	call   0x80483a0 <strncpy@plt>
   0x0804851a <+79>:	add    esp,0x10
   0x0804851d <+82>:	mov    eax,0x0
   0x08048522 <+87>:	leave  
   0x08048523 <+88>:	ret    
```
strncpy함수를 호출한다. 아까 초기화한 변수의 가장 낮은 주소와, 매개변수를 함수에 넘긴다. 즉 local char[]을 초기화하고, 그 값을 넘긴 char* 에 14byte만큼 초기화한다.

----------
다시 main 함수

```
   0x08048569 <+69>:	add    esp,0x10
   0x0804856c <+72>:	sub    esp,0xc
   0x0804856f <+75>:	push   0x80486c6
   0x08048574 <+80>:	call   0x8048370 <puts@plt>
   0x08048579 <+85>:	add    esp,0x10
```
매개변수 입력 후 첫 출력이므로, "Are you an 4ws0me_h@cker?"를 출력하는 print 일 것이다.

```
(gdb) x/s 0x80486c6
0x80486c6:	"Are you an 4ws0me_h@cker?"
(gdb) 
```
확인해 봐도 맞다.


```
   0x0804857c <+88>:	mov    eax,DWORD PTR [ebx+0x4]
   0x0804857f <+91>:	add    eax,0x4
   0x08048582 <+94>:	mov    eax,DWORD PTR [eax]
   0x08048584 <+96>:	sub    esp,0x4
   0x08048587 <+99>:	push   0xe
   0x08048589 <+101>:	push   eax
   0x0804858a <+102>:	lea    eax,[ebp-0x1e]
   0x0804858d <+105>:	push   eax
   0x0804858e <+106>:	call   0x80483a0 <strncpy@plt>
   0x08048593 <+111>:	add    esp,0x10
```
아까 main 함수 초반에, argv를 ebp+0x4로 접근하게끔 값을 푸쉬했다. 

가져온 주소값에 4를 더했으니, argv[1]의 주소값이 되었음을 알 수 있다.

argv[1]를 14byte 만큼 지역 버퍼 ebp-0x1e에 카피했음을 알 수 있다.
```
   0x08048596 <+114>:	mov    DWORD PTR [ebp-0xc],0x0
   0x0804859d <+121>:	jmp    0x80485d0 <main+172>
   0x0804859f <+123>:	mov    eax,0xc
   0x080485a4 <+128>:	sub    eax,DWORD PTR [ebp-0xc]
   0x080485a7 <+131>:	mov    DWORD PTR [ebp-0x10],eax
   0x080485aa <+134>:	lea    edx,[ebp-0x1e]
   0x080485ad <+137>:	mov    eax,DWORD PTR [ebp-0xc]
   0x080485b0 <+140>:	add    eax,edx
   0x080485b2 <+142>:	movzx  ecx,BYTE PTR [eax]
   0x080485b5 <+145>:	lea    edx,[ebp-0x2c]
   0x080485b8 <+148>:	mov    eax,DWORD PTR [ebp-0x10]
   0x080485bb <+151>:	add    eax,edx
   0x080485bd <+153>:	movzx  eax,BYTE PTR [eax]
   0x080485c0 <+156>:	xor    ecx,eax
   0x080485c2 <+158>:	lea    edx,[ebp-0x1e]
   0x080485c5 <+161>:	mov    eax,DWORD PTR [ebp-0xc]
   0x080485c8 <+164>:	add    eax,edx
   0x080485ca <+166>:	mov    BYTE PTR [eax],cl
   0x080485cc <+168>:	add    DWORD PTR [ebp-0xc],0x1
   0x080485d0 <+172>:	cmp    DWORD PTR [ebp-0xc],0xc
   0x080485d4 <+176>:	jle    0x804859f <main+123>
   0x080485d6 <+178>:	sub    esp,0x4
```
비교, 점프하는 명령어를 보면, 반복문임을 알아챌 수 있다.

ebp-0xc는 카운터 변수, epb-0x1e 는 xor 당하는 포인터, ebp-0x2c는 xor 값을 제공하는 포인터이다.

ebp-0x2c 포인터는 0xc - 카운터변수값 만큼 더해진 위치 값을 가져온다 : 거꾸로 접근

ebp-0x1e 포인터는 카운터변수값만큼 더해진 위치 값을 가져온다 : 순차적을로 접근

```
   0x080485d9 <+181>:	push   0xe
   0x080485db <+183>:	push   0x80486e0
   0x080485e0 <+188>:	lea    eax,[ebp-0x1e]
   0x080485e3 <+191>:	push   eax
   0x080485e4 <+192>:	call   0x80483b0 <strncmp@plt>
   0x080485e9 <+197>:	add    esp,0x10
```
이전에 반복문의 결과값이 들어있는(문자 xor) 버퍼(ebp-0x1e)와 특정 문자열(0x80486e0)을 14byte만큼 비교한다.

```
(gdb) x/s 0x80486e0
0x80486e0:	"4ws0me_h@cker"
```
즉, xor 루틴을 거친 후 '저 문자열' 이 나와야 한다.

```
   0x080485ec <+200>:	test   eax,eax
   0x080485ee <+202>:	je     0x8048602 <main+222>
   0x080485f0 <+204>:	sub    esp,0xc
   0x080485f3 <+207>:	push   0x80486ee
   0x080485f8 <+212>:	call   0x8048370 <puts@plt>
   0x080485fd <+217>:	add    esp,0x10
   0x08048600 <+220>:	jmp    0x8048612 <main+238>
   0x08048602 <+222>:	sub    esp,0xc
   0x08048605 <+225>:	push   0x80486fb
   0x0804860a <+230>:	call   0x8048370 <puts@plt>
   0x0804860f <+235>:	add    esp,0x10
   0x08048612 <+238>:	mov    eax,0x0
   0x08048617 <+243>:	lea    esp,[ebp-0x8]
   0x0804861a <+246>:	pop    ecx
   0x0804861b <+247>:	pop    ebx
   0x0804861c <+248>:	pop    ebp
   0x0804861d <+249>:	lea    esp,[ecx-0x4]
   0x08048620 <+252>:	ret    
```
이후에는 strncmp 결과값인 eax 가 0인지 아닌지를 따져서, 0이면
```
(gdb) x/s 0x80486fb
0x80486fb:	"You are!"
```
인정받는다.

## 3.문제 해결 

문제 해결을 위해서, xor 연산의 특징을 알아야 한다

- xor은 베타적 논리합.
    - A ^ A = 0
    - A ^ B = 1
    - A ^ 0 = A
    - A ^ 1 = B. 왜냐면, A가 1이라면 0, 0이라면 1이 결과값이 됨.

- A ^ B = C 면, A ^ C = B, B ^ C = A가 성립한다.
    - 2진수로 몇자리든 성립함 (비트단위 연산이니까)

즉, disassemble 을 통해 얻은 피연산자, 연산 결과값으로
문제 해결을 위한 입력값을 얻을 수 있다.

파이썬 스크립트를 작성해보면 다음과 같다.
```python
def f(a, b):
    return chr(ord(a)^ord(b))
result = ''
s1 = '\x4d\x56\x1e\x11\x34\x37\x2b\x04\x05\x44\x2c\x04\x5d\x00'
s2 = '4ws0me_h@cker'

for i in range(13):
	s += f(s2[i], s1[12-i])

print(s)
````

정답은 `is_that_tru3?`다.


