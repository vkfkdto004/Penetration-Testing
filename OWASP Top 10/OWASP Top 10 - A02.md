# OWASP Top 10 (2021) - A02

| https://owasp.org/Top10/A02_2021-Cryptographic_Failures/
## A02 Crytographic Failures (암호화 실패)
암호학과 관련된 전반적인 내용을 광범위하게 다루고 있으며, 기존에는 민감한 데이터 노출(Sensitive Data Exposure) 이라고 했으나, 암호화 실패(Crytographic Failures)로 명칭이 변경 되었다.

암호화에는 개인정보와 신용카드 번호와 개인 식별 정보(PII)와 같은 민감한 데이터를 전송 중에 보호하는데 사용된다. 그러나 약한 암호화 알고리즘이나 짧은 암호화 키와 같은 요소로 실패할 수 있으며, 이로 인해 공격자가 민감한 데이터를 해독하기 쉬워질 수 있다.
그래서 데이터 같은 법과 규정에 강력하게 영향을 받는 경우라면 더더욱 안전하게 보호하기위한 추가 요구사항을 잘 지켜야한다.
(국내에는 개인정보보호법, 정보통신망법, 신용정보법, ISMP-P 인증, ISO-27001 인증, 주요정보통신기반시설 취약점 분석평가, 전자금융기반시설 취약점 분석평가 등이 있다.)

### 암호화 실패의 예
- 내,외부망 관계없이 데이터가 전송구간에서 평문으로 전송되는 경우(HTTP,FTP,TELNET 등)
- 취약한 암호화 프로토콜을 사용하는 경우(SSL v2.0, SSL v3.0, TLS v1.0, TLS v1.1 등)
- 취약한 암호화 알고리즘을 사용하는 경우(DES, RC4, MD5 등)
- 취약한 암호화 컴포넌트를 사용하는 경우(취약한 버전의 openssl 사용 등)
- 보안 헤더 설정을 통한 HSTS가 누락된 경우
	- HSTS(HTTP Strict Transport Security) : HTTP를 HTTPS로 강제 리다리렉트 하는 것
- 고정된 암호문을 사용하는 경우(Salt, 일회용 난수 미포함)
- 사설 인증서 사용, 인증서와 도메인 불일치
- 암호키 관리가 미흡한 경우(소스코드, 하드코딩 등)



### 예방 방법
- 민감한 데이터를 필요이상으로 저장하지 않는다, 가능한 빨리 폐기하거나 PCI DSS 호환 토큰화를 사용한다.(보관되지 않은 데이터는 크래킹 할 수 없음)
- 저장되어 있는 모든 민감한 데이터를 최신 버전의 암호 프로토콜 및 안전한 암호 알고리즘을 사용하여 암호화 한다.
- FTP, SMTP, TELNET과 같은 레거시 프로토콜을 사용하지 않는것
- 순방향 비밀성(FS) 암호를 사용하는 TLS, 서버에 의한 암호 우선 순위 지정 및 보안 매개변수와 같은 보안 프로토콜을 사용하여 전송중인 모든 데이터를 암호화한다. -> HSTS를 활성화 하여 암호화 시행
- 암호화 시 암호문이 고정되지 않도록 의사 난수 생성기를 포함한다.
- 신뢰할 수 있는 기관에서 발급한 인증서를 사용한다.

