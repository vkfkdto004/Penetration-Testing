# OWASP Top 10 - 2021

| https://owasp.org/www-project-top-ten/

## 목차
1. Broken Access Control (취약한 액세스 제어, 취약한 접근 제어)
2. Cryptographic Failures
3. Injection
4. Insecure Design
5. Security Misconfiguration
6. Vulnerable and Outdated Components
7. Identification and Authentication Failures
8. Software and Data Integrity Failures
9. Security Logging & Monitoring Failures
10. Server-Side Request Forgery (SSRF)


# OWASP Top 10 이란?
Open Web Application Security Project 의 주요 프로젝트 중에 하나로, 웹 애플리케이션 보안을 향상 시키 위해 개발된 가장 중요한 지침중에 하나이다.
매년 업데이트 되며, 이전 버전에서 식별된 보안 위험을 최신 상황, 기술 동향에 맞게 조정해서 제공한다.
목차에서 나오는 목차는 웹 애플리케이션 보안을 향상 시키기 위한 기준을 제시하고, 개발자와 보안 전문가가 웹 애플리케이션에서 발생할 수 있는 보안 문제를 이해하고 대응하는데 도움을 준다.

---

## A1 Broken Access Control (취약한 액세스 제어)
| Broken Access Control (취약한 액세스 제어) 란?
액세스 제어는 정책을 통해 사용자들이 각자 부여된 권한을 벗어난 행동을 할 수 없도록 하는데, 정해진 정책에 대해서 제대로 적용되지 않아 필요 이상의 권한을 가지는 것을 말한다.
예로 들어,
- 특정 기능이나 역할에 대해서 거기에 맞는 사용자에 대해서만 액세스 권한을 부여하지만 누구나 액세스 할 수 있게 된 경우 (관리자 페이지 노출)
- 인증되지 않은 사용자가 인증된 페이지로, 권한이 필요한 페이지로 부적절하게 강제 브라우정 되는 경우 (부적절한 인가)
- 파라미터나 쿠키 등의 요청을 조작해 권한 상승 또는 타 사용자의 권한을 획득 할 수 있는 경우
- POST,PUT,DELETE API 요청에 대한 액세스 제어가 누락된 경우

| 발생하는 이유?
구성 오류, IDOR(Insecure Direcrt Object References)로 발생할 수 있는데, 이는 응용프로그램이 파일 또는 데이터베이스 레코드와 같은 리소스에 직접적인 참조를 노출시키는 경우이다. 또한 보안되지 않은 세션 관리로 인해 세션 관리에서의 결함으로 공격자가 사용자 세션을 탈취하는 경우 등 여러가지 이유로 발생 할 수 있다.

| 예방 방법
1. 공용 리소스를 제외하고 기본적으로 접근을 거부한다. (화이트리스트 기반)
2. 접근 제어 매커니즘을 애플리케이션 전체에 일괄 적용 시킨다.
3. 공개 인터넷망을 통해서 접속을 허용하는 경우, 패스워드 기반의 사용자 인증 이외에 OTP, 공인인증서 등 방법을 적용한다.
4. 웹 서버의 디렉터리 목록을 비활성화 하고 파일 메타데이터 등 백업 파일이 웹 루트 내에 존재하지 않도록 설정한다.
5. 관리자 인증 후 접속할 수 있는 페이지의 경우, 해당 주소로 직접 입력하여 접속하지 못하도록 관리자 페이지에 대하여 관리자 인증을 위한 세션을 따로 관리한다.



