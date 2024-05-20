# SQL Injection 의 기초

개인 공부용으로 참고하기 위해서 버그바운티 클럽의 Pentest Gym에서 퍼온 사진과 글을 참고하여 정리한 문서입니다. 

| 출처 : https://www.bugbountyclub.com/pentestgym/view/52

# SQL Injection 이란?
SQL Injection(SQL 삽입)은 웹 애플리케이션이 백엔드에서 구동 중인 데이터베이스에 질의하는 과정에서 샤용되는 SQL 쿼리를 조작하여 데이터베이스를 대상으로 공격자가 의도한 악의적인 행위을 할 수 있는 OWASP Top10 에도 수록되어있는 Injection 웹 기반의 취약점이다.
공격자가 SQL Injection 공격에 성공하게 되면 조직 내부의 민감한 데이터나 개인 정보를 획득 할 수 있고, 심각한 경우에는 조직의 데이터 전체를 장악하거나 완전히 손상 시킬 수 있는 중대한 취약점이다.

# SQL Injection의 동작 원리
우리가 사용하는 웹 애플리케이션들은 대부분 아래의 그림처럼 데이터베이스를 이용해 서비스 제공에 필요한 데이터를 조회하거나 저장, 수정, 삭제한다.
데이터베이스는 웹 애플리케이션과 동일한 서버에 구축되기도 하지만, 물리적 또는 논리적으로 분리된 서버에 구축한다.

![출처 : https://www.bugbountyclub.com/pentestgym](https://www.bugbountyclub.com/uploads/images/training/1644468226.png)

1. 클라이언트가 웹 애플리케이션에서 데이터베이스와 연동된 기능을 사용할 때 사용자의 입력에 의해 입력된 데이터(또는 웹 애플리케이션에 의해 셋팅된 데이터)는 HTTP 요청(GET 매개변수 또는 POST Body 매개변수 등)을 통해 웹 애플리케이션으로 전송
2. 전송된 데이터는 서버측 스크립트에 의해 미리 정의 및 저장되어있던 SQL 쿼리 안의 지정된 위치로 대입
3. 서버측 스크립트는 데이터베이스에 완성된 SQL 쿼리 실행을 요청
4. 데이터베이스는 SQL 쿼리를 실행 한 후 조건에 부합하는 레코드셋이나 Boolean 값 또는 오류를 반환
5. 서버측 스크립트는 데이터베이스에서 반환된 데이터에 맞게 HTTP 응답 메시지를 만들어 클라이언트에게 회신

위 같은 로그인 요청 수행 시 SQL Injection이 수행되는 과정 -> 취약하게 구성된 로그인 기능은 SQL Injection을 통해 인증을 우회하고 다른 사용자의 계정으로 로그인이 가능하다

## 인증 우회 목적의 공격 예시
![](https://www.bugbountyclub.com/uploads/images/training/1644468194.png)

공격자가 입력한 아이디 값
```bash
admin' or '1'='1
```
패스워드 입력 값에는 원하는 임의의 문자열 입력 예제에서는 anything을 입력한다. 공격자가 입력한 아이디와 패스워드의 값은 HTTP 요청을 통해 서버로 전달되고 최종적으로 다음과 같은 쿼리가 생성된다
```bash
SELECT * FROM users WHERE id='admin' or '1'='1' AND password='anything';
```
이 SQL 쿼리는 웹 애플리케이션에서 admin이라는 사용자 계정이 존재한다면 패스워드가 일치하는지 여부과 관계없이 admin 계정으로 로그인 된다.
이유는 쿼리의 논산 연산을 조작해 무조건 TRUE 값을 만들었기 떄문이다.

![](https://www.bugbountyclub.com/uploads/images/training/1616992423.png)
위 사진과 같이 공격자가 조작한 쿼리의 WHERE 절에는 OR와 AND 두개의 논리 연산자가 있고, 다음과 같은 총 3개의 조건식이 있다. AND 연산자가 OR 연산자보다 높다는 것에 유의해야한다.
|구분|내용|조건의 참/거짓 여부|
|:---:|:---:|:---|
|Condition1|id='admin'|TRUE (admin 계정이 있다고 가정하여 TRUE)|
|Condition2|'1'='1'|TRUE|
|Condition3|password='anything'|FALSE|

![](https://www.bugbountyclub.com/uploads/images/training/1616991390.png)


## 공격자가 의도한 새로운 데이터베이스 명령 문장 실행
![](https://www.bugbountyclub.com/uploads/images/training/1644469018.png)
게시판 기능에서 사용자가 특정 게시글을 클릭하면 해당 게시글을 보여주기 위해서 아래 SQL 쿼리가 사용됨
```bash
SELECT brd_no, subject, content, author, post_date FROM board WHERE brd_no=4215;
```
공격자가 세미콜론(;)과 공격자가 의도한 악의적인 SQL 문장을 이용하여 `brd_no` 라는 GET 매개변수 조작, 9999는 존재하지 않는 게시글의 번호로 가정
```bash
/board/view.php?brd_no=9999;Drop%20table%20users;
```
위 요청을 통해 전달된 `brd_no` 매개변수의 값은 게시글을 조회하기 위해 미리 정의되어있던 SQL 쿼리에 포함되고 SQL 쿼리는 데이터베이스에서 실행
하지만 SQL 쿼리는 세미콜론으로 나눠졌기 때문에 한개가 아닌 두개의 쿼리로 나눠졌고, SQL 쿼리와 공격자가 추가한 악의적인 SQL 쿼리가 모두 하나의 트랜젝션에서 실행되게 된다.
```bash
SELECT brd_no, subject, content, author, post_date FROM board WHERE brd_no=9999;Drop table users;
```
만약 사용하는 계정이 테이블 삭제 권한을 가지고 있다면 `Drop table users;` 명령에 의해 users 테이블은 삭제, 서비스에 아주 치명적인 손상을 준다
![](https://www.bugbountyclub.com/uploads/images/training/1644469586.png)


# SQL Injection의 영향
SQL Injection 공격은 인증을 우회하는 등의 웹 애플리케이션의 로직을 파괴하거나 조직의 데이터베이스 정보나 개인정보, 또는 의도적으로 숨긴 데이터 유출 및 데이틔 손상, 손실까지 초래할 수 있다. MSSQL 같은 일부 데이터베이스는 저장 프로시저(Stored Procedure)를 이용하면 원격 명령 실행 등에 노출되어 시스템 손상까지 초래할 수 있다.

# SQL Injection의 예방

## 입력값 검사
HTTP 요청을 통해 전달되는 사용자 데이터에 SQL 구문을 해석 할 수 있는 문자 또는 공격에 사용되는 SQL 구문들의 포함여부를 검사하고, 포함시 요청을 차단하거나 해당 문자를 제거, 또는 다른 문자로 대체해야한다.
- SQL 기호 : `'`, `"`, `;`, `-`,`#`,`/*`
- SQL 구문 : `SELECT`, `INSERT`, `UPDATE`, `DELETE`, `UNION`, `GROUP BY`, `HAVING`, `ORDER BY` 등

## 저장 프로시저 사용
웹 애플리케이션에서 데이터베이스에 접속하여 어떤 작업을 수행 할 때 저장 프로시저를 사용하는 방법. 하지만 저장 프로시저 내에서도 SQL 쿼리가 사용되고, 이 SQL 쿼리가 저장 프로시저내에서 안전하지 않은 방식으로 처리된다면 SQL Injection 공격에 노출될 수 있는 단점이 있다. 또한 저장 프로시저를 호출하는 명령에 매개변수로서 사용자 입력값이 안전하지 않는 방식으로 사용되는 경우에도 마찬가지이다. 공격자는 저장 프로시저 실행 명령을 강제로 종료하고 임의의 저장 프로시저(xp_cmdshell)를 호출하도록 명령을 조작할 수 있다.
sp_boardList 저장 프로시저 실행 명령을 종료시키고 뒤에 xp_cmdshell 저장 프로시저를 통해 C드라이브의 파일과 파일 디렉토리를 출력하는 쉘 명령을 실행하는 경우
```bash
exec sp_boardList '30', 'test'; exec master..xp_cmdshell 'dir C:\'-- 
```

## 매개변수화된 쿼리 적용
매개변수화된 쿼리 (Prepared Statement) : 데이터베이스에서 사용되는 동일하거나 유사한 SQL 쿼리를 효율적으로 반복적으로 실행하기 위해 사용되는 기능

즉, 자주 사용되는 쿼리를 미리 데이터베이스에 저장해 두었다가, 해당 SQL 쿼리 실행에 필요한 값들만 매개변수로 전달하는 실행 방식이다.
매개변수화된 쿼리를 사용하면 공격자는 SQL 쿼리의 구조를 알 수 없고, 어떤 HTTP 매개 변수가 SQL 쿼리의 어느 부분에 전달되는지 알지 못하므로 공격에 대응하여 유용하게 사용될 수 있다.

매개변수화된 쿼리를 사용하지 않고 데이터베이스에 데이터를 삽입하는 PHP 코드
```bash
$conn = mysqli_connect("example.com","spike","ilovejulia","bebop_db");
$sql = "INSERT INTO crews (firstname, lastname, email) VALUES ('$_POST["firstname"]','$_POST["lastname"]','$_POST["email"]')";
$mysqli->query($conn, $sql);
```

매개변수화된 쿼리를 사용한 PHP 코드
```bash
$conn = mysqli_connect("example.com","spike","ilovejulia","bebop_db");
$stmt = $conn->prepare("INSERT INTO crews (firstname, lastname, email) VALUES (?, ?, ?)");
$stmt->bind_param("sss", $firstname, $lastname, $email);
$firstname = $_POST['firstname'];
$lastname = $_POST['lastname'];
$email = $_POST['email'];
$stmt->execute();
```
INSERT 구문을 보면 VALUES에 ? 로 표기를 했다 이를 Placeholder라고 한다. 추후 사용자로부터 전달받은 데이터를 매개변수를 통해 바인딩하여 SQL 쿼리를 실행한다


## 최소 권한 & 최소 기능 사용
간혹 웹 애플리케이션이 사용하는 데이터베이스 계정이 데이터베이스 관리자 계정이거나 관리자급 권한을 보유하고 있는 경우가 있는데, 이는 최소 권한의 원췩에 위배되는 내용이다. 웹 애플리케이션 서비스에 필요한 최소한 권한만을 보유하도록 별도의 데이터베이스 계정을 사용해야한다.
또한 편의상 제공되는 불필요한 기능들을 비활성화 한다

## 데이터베이스 최신 패치 적용
사용중인 데이터베이스의 제작사에서 발표하는 최신 보안 패치를 수시로 확인하고 적용한다.






