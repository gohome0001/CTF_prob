# 출제 의도

이 문제는 linux 유틸리티 `ltrace` 사용법과, 프로그램에서 call 되는 C 라이브러리 함수의 기능에 대해 알 수 있으면 어렵지 않게 해결할 수 있는 문제이다. 물론 단순 디버깅으로도 해결할 수도 있는 문제이다.

문제 제목과 내용이 문제 해결에 있어서 수수께끼 같지만, 큰 실마리가 된다는 것을 알았으면 좋겠다.

주어진 키워드를 통해 조금 더 쉽게 문제를 해결할 수 있다는 것을 알았으면 좋겠다.

리눅스에서 제공하는 기본 명령어들, 여러 기능들에 대해 익숙해질 수 있는 계기가 되었으면 좋겠다.

# 문제 내용
```
I made a program that runs on my computer. but, my computer has been broken now.. 
can't you run it for me?
```

자기 컴퓨터에서 돌아가는 프로그램을 만들었는데, 컴퓨터가 고장났다고 한다.

자기 대신 실행해 줄수 없냐고 물어본다.

자기 컴퓨터에서 실행되었던 프로그램이 내 컴퓨터에서도 별 탈 없이 실행될 수 있을까?

# 문제 풀이

## 키워드

키워드는 다음과 같다

- trace - 리눅스에서 프로그램 추적과 관련된 유틸리티 등에 대한 힌드!
- 내컴퓨터에선 돌아가는 프로그램 - 자기 컴퓨터 환경에서 돌아가는 프로그램

## 파일 분석

이 문제는 파일이 정상 실행되도록 내 환경을 맞춰 주어야 한다.

- 주어진 파일을 실행해 보면, 화면에 아무것도 출력하지 않은 채 그냥 끝나버린다.
```
gohome0001@ubuntu:~/study/pcap$ ./traceME
gohome0001@ubuntu:~/study/pcap$ 
```

파일을 디버깅 해서 실마리를 찾을 수도 있지만, 리눅스의 ltrace라는 툴을 사용하기로 했다.

### ltrace란?
- ltrace는 리눅스에서 프로그램을 실해하고, 끝날때까지 사용된 동적 라이브러리 콜을 관찰, 추적할 수 있는 유틸리티 이다.

- 호출되는 라이브러리 함수와 넘겨지는 매개변수 값을 그대로 볼 수 있어, 디버깅 할 떄에 많이 쓰인다.

### ltrace 사용법
```
gohome0001@ubuntu:~/study/pcap$ ltrace 실행파일명
```

주어진 파일을 ltrace로 실행해 보았다.
```
gohome0001@ubuntu:~/study/pcap$ ltrace ./traceME
__libc_start_main(0x804852b, 1, 0xffffd0e4, 0x8048620 <unfinished ...>
open("./my/dir", 1, 036770654060)                                                                                            = -1
exit(1 <no return ...>
+++ exited (status 1) +++
gohome0001@ubuntu:~/study/pcap$ 
```

결과를 보면, `open` 함수를 호출하고 있다.

- open 함수는 저수준 파일 입출력 함수이다.
- file descriptor값을 return 한다. 실패시 -1을 return 한다.
- 매개변수로 open하려는 파일 path와 모드 (읽기, 쓰기 등), 기타 옵션들을 줄 수 있다.

즉, `./my/dir` 파일을 열려고 한 것을 볼 수 있다.

함수 결과값을 보면. 파일이 없어 `open`에 실패하고, 그로 인해 프로그램이 종료했음을 추측할 수 있다. 그러면 한번 `./my/dir` 파일을 만든 후 시도해 보자.
```
gohome0001@ubuntu:~/study/pcap$ mkdir my
gohome0001@ubuntu:~/study/pcap$ vim ./my/dir
gohome0001@ubuntu:~/study/pcap$ ltrace ./traceME
__libc_start_main(0x804852b, 1, 0xffffd0e4, 0x8048620 <unfinished ...>
open("./my/dir", 1, 036770654060)                                                                                            = 3
getenv("MYENV")                                                                                                              = nil
exit(1 <no return ...>
+++ exited (status 1) +++
gohome0001@ubuntu:~/study/pcap$ 
```
`open` 함수는 성공적으로 작동하였다. 그 다음에는, `getenv` 함수를 호출하고 있다. 
- getenv() 함수는 매개변수로 환경변수 이름 문자열을 넘기면, 해당 환경변수의 주소를 리턴하는 함수이다. 실패 시 NULL 포인터를 리턴한다.

MYENV라는 이름의 환경변수 주소를 불러들여오고 있다. 결과값을 보아서는 실패한 모양이다.

이번에도 역시 MYENV라는 환경변수를 만든 후 다시 시도해 보자.
```
gohome0001@ubuntu:~/study/pcap$ export MYENV=1
gohome0001@ubuntu:~/study/pcap$ ltrace ./traceME
__libc_start_main(0x804852b, 1, 0xffffd0e4, 0x8048620 <unfinished ...>
open("./my/dir", 1, 036770654060)                                                                                            = 3
getenv("MYENV")                                                                                                              = "1"
getuid()                                                                                                                     = 1000
exit(1 <no return ...>
+++ exited (status 1) +++
gohome0001@ubuntu:~/study/pcap$ 
```

음.. 이번에는 `getuid` 함수를 호출한다
- getuid 함수는 프로세스의 ruid를 return 하는 함수이다. 
- ruid는 Real User ID로, 프로세스의 real uid를 리턴한다. ruid는 euid때문에 생긴 개념이다.
- euid는 Effective User ID로, 프로세스의 권한으로 설정된 uid를 뜻한다. euid는 ruid보다 권한이 낮거나 같다야 한다.

이번에는 디버거를 사용해야 할 거 같다. 왜 프로그램이 종료하는지 알기 힘들기 때문이다.

gdb를 사용하면 다음과 같은 부분을 찾을 수 있다.

```
   0x08048585 <+90>:	call   0x80483c0 <getuid@plt>
   0x0804858a <+95>:	mov    %eax,-0xc(%ebp)
   0x0804858d <+98>:	cmpl   $0x3ec,-0xc(%ebp)
```

함수 return값을 0x3ec (10진수로는 1004) 와 비교한다. 

위의 결과를 보면, 리턴값이 1000이다. ruid값을 1004로 맞춰주어야 될 것 같다.

```
gohome0001@ubuntu:~/study/pcap$ ls -l traceME
-rwxrwxr-x 1 gohome0001 gohome0001 7604 Sep  5 23:31 traceME
gohome0001@ubuntu:~/study/pcap$ 
```

`setuid`가 설정되어 있지 않은 파일이라, uid가 1004인 사용자로 실행하면 될 것 같다. 

- setuid : 리눅스에서 파일을 소유한 사용자의 권한으로 프로그램이 실행되도록 설정된 파일을 말한다. ls 명령 결과에서 실행 권한을 나타내는 x가 s로 바뀌어 표시된다.

uid가 1004인 계정을 생성한 뒤, 다시 실행해 보자.

```
root@ubuntu:~/# useradd -u 1004 myaccount
root@ubuntu:~/# passwd myaccount
root@ubuntu:~/# Enter new UNIX password:
root@ubuntu:~/# Retype new UNIX password:
root@ubuntu:~/# password updated successfully
root@ubuntu:~/#
```
그전에, chmod 명령어로 ./my/dir 파일과 traceME파일에 다른 사용자가 읽기/실행할 수 있게 권한을 설정해 주어야 한다.

- chmod : 리눅스에서 파일의 접근 권한을 수정하는 명령어. 

```
root@ubuntu:~/# chmod o+r ./my/dir
root@ubuntu:~/# chmod o+x ./traceME
```

myaccount 로 로그인 후..
```
myaccount@ubuntu:/home/gohome0001/study/pcap$ export MYENV=1
myaccount@ubuntu:/home/gohome0001/study/pcap$ ltrace ./traceME
__libc_start_main(0x804852b, 1, 0xffffd0e4, 0x8048620 <unfinished ...>
open("./my/dir", 1, 036770654060)                                                                                            = 3
getenv("MYENV")                                                                                                              = "1"
getuid()                                                                                                                     = 1004
strcmp("./traceME", "qux")
= -1
exit(1 <no return ...>
+++ exited (status 1) +++
myaccount@ubuntu:/home/gohome0001/study/pcap$
```

음.. 이번에도 디버깅이 필요해 보인다.
strcmp 함수가 실행되는 곳을 찾아보자.

```as,
0x080485a9 <+126>:	mov    0x4(%ebx),%eax
0x080485ac <+129>:	mov    (%eax),%eax
0x080485ae <+131>:	sub    $0x8,%esp
0x080485b1 <+134>:	push   $0x80486af
0x080485b6 <+139>:	push   %eax
0x080485b7 <+140>:	call   0x80483a0 <strcmp@plt>
```
strcmp 함수가 호출되는 부분이다.

저 "./traceME" 문자열이 커맨드라인 인수인 `argv[0]`임을 알 수 있다.

```
(gdb) x/s 0x80486af
0x80486af:	"qux"
```

비교되늰 문자열도 "qux" 이다.

이 루틴을 통과하려면, 파일에 링크를 걸어 argv[0] 문자열을 "qux"로 바꾸면 된다.

또한, PATH 환경변수에 링크 파일이 있는 디렉토리를 추개하 주어야 한다. 그렇지 않으면 "qux"가 아니라 "./qux" 로 실행해야 되기 떄문이다.

```
gohome0001@ubuntu:~/study/pcap$ su myaccount
Password: 
myaccount@ubuntu:/home/gohome0001/study/pcap$ ln -s ./traceME qux
myaccount@ubuntu:/home/gohome0001/study/pcap$ export PATH=$PATH":/home/gohome0001/study/pcap"
myaccount@ubuntu:/home/gohome0001/study/pcap$ ltrace qux
__libc_start_main(0x804852b, 1, 0xffffd704, 0x8048620 <unfinished ...>
open("./my/dir", 1, 036770654060)                                                                                            = 3
getenv("MYENV")                                                                                                              = "1"
getuid()                                                                                                                     = 1004
strcmp("qux", "qux")                                                                                                         = 0
printf("%s", "foobarbaz?\n"foobarbaz?
)                                                                                                 = 11
puts("check it carefully!"check it carefully!
)                                                                                                  = 20
+++ exited (status 0) +++
myaccount@ubuntu:/home/gohome0001/study/pcap$
```

ltrace로 확인해 보니, 조건에 걸리는 경우가 없었다. 디버거로 확인해 보아도 알 수 있다.

### FLAG : foobarbaz?








