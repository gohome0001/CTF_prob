# WARNING : 라이트업이 완성되지 않은 상태입니다.

# Observer - Reversing

## 문제의도
- Ollydbg 등 windows용 디버거를 사용해 `동적으로 명령어를 패치하는` 것에 대해 알았으면 했다.
- 포면적인 문제 해결이 전체 문제를 해결하는 것이 아님을 알았으면 좋겠다.
    - 명령어 패치에서, 단순 jmp로는 flag 값 얻을 수 없음.
    
---------------

## 키워드

- Windows 디버거 - 디버깅을 위해, 어셈블리 명령어를 Single Stepping 하며 실행할 수 있는 프로그램이다
- 명령어 동적 패치 - 메모리상에 로딩된 실행 파일의 명령어를 수정하는 것을 말한다.

------

## 문제내용
```
Sometimes, You need an Observer.
But after then, It won't help you :p

The main funcion is at 0x004015BE

hint : Debug it if you want~
```
exe 실행파일 `Observer`

---

## 풀이  

처음 실행파일을 실행해 보면 다음과 같은 문구가 뜨며, 아무 키 입력시 프로그램이 종료된다.

    No Observer found..

옵저버? 프로그램 실행에서 옵저버는 무엇을 뜻하는 것일까?

힌트를 보면, 옵저버가 의미하는 것이 디버거임을 추측할 수 있다.

`디버거`란, 디버깅 작업을 하기 편하도록, 어셈블리 명령어를 하나 하나 실행할 수 있게끔 해준다. 이 외에도 디버거별로 다양한 기능들을 제공한다.
- 디버거의 예로 Ollydbg, Immunity Debugger, gdb 등이 있다

이번에는 Ollydbg를 사용해 보도록 하겠다.

프로그램을 로딩한 후, Ollydbg의 강력한 기능 중 하나인 Search for all referenced string 을 사용해보자.

<!--사진-->

위에 보면, 아까 본 'No Observer found...' 문자열도 보이고, 문제 해결에 도움이 될 것 같은 문자열들이 보인다.

디버거 없이 실행했을 때 왜 'No Observer found'가 출력되었는지 알기 위해 그 문자열이 호출된 함수를 분석해 보자!

----------

### 함수 A
- 그냥 실행했을 때, 'No Observer found'를 출력해주는 함수


```
CPU Disasm
Address   Hex dump          Command                                                  Comments
00401500  /$  55            PUSH EBP                                                 ; IDP.00401500(guessed void)
00401501  |.  89E5          MOV EBP,ESP
00401503  |.  83EC 18       SUB ESP,18
00401506  |.  A1 34614000   MOV EAX,DWORD PTR DS:[<&KERNEL32.IsDebuggerPresent>]
0040150B  |.  FFD0          CALL EAX
```
IsDebuggerPresent 함수를 실행한다
```
0040150D  |.  85C0          TEST EAX,EAX
0040150F  |.  74 02         JE SHORT 00401513
00401511  |.  EB 1D         JMP SHORT 00401530
00401513  |>  C70424 004040 MOV DWORD PTR SS:[ESP],OFFSET 00404000                   ; ASCII "No Observer found.."
0040151A  |.  E8 29120000   CALL <JMP.&msvcrt.puts>
0040151F  |.  E8 2C120000   CALL <JMP.&msvcrt.getchar>
00401524  |.  C70424 010000 MOV DWORD PTR SS:[ESP],1
0040152B  |.  E8 10120000   CALL <JMP.&msvcrt.exit>
00401530  |>  C9            LEAVE
00401531  \.  C3            RETN
```
함수 결과값이 0이면 'No Observer found' 를 출력하며 종료하고, 1이면 종료하지 않고 리턴한다.

IsDebuggerPresent 함수는 디버거로 실행되는지 확인하고, 맞으면 1 아니면 0을 출력한다.

즉, 디버거가 문제에서 말하는 Observer 임을 알 수 있다.

결국 처음에 프로그램이 그냥 종료되어버린 이유는 디버거로 실행하지 않았기 때문이다.

------

### 함수 B

------

### 함수 C

------

### Main 함수

------


이렇게 체크하는 함수 2개의 기능인, IsDebuggerPresent()함수를 확인해 종료하는 루틴을 패치한다

이러면 Observer Not Found 메세지가 안 뜨게 하면서, 프로그램의 감시에서 벗어나는 과정을 거치면, 문자열 하나가 출력된다.

FLAG : `amu#ehdAhwid22b3`

플래그값에 의미는 딱히 없다 (rand로 돌려서 source 문자열 만들었음..)