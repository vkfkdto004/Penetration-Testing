# OWASP Top 10 (2021) - A04

| https://owasp.org/Top10/A05_2021-Security_Misconfiguration/
## A05 Security Misconfiguration(보안 설정 오류)
보안 설정 오류는 애플리케이션을 최초 설치하거나 업데이트할 때 보안성을 고려하지 않은 설정으로 인해 취약점이 발생하는 경우로, 설정과 관련된 모든 부분을 포함한다. XXE(XML External Entity)은 애플리케이션의 잘못된 보안 설정으로 인해 발생하는 취약점으로, 개정 시 보안 설정 오류 항목에 병합되었다.
안정적인 보안은 애플리케이션, 프레임워크, 서버, 플랫폼 등이 함께 구성되는데, 보안을 적절하게 설정하고 최적화된 값을 유지하며, 소프트웨어는 최신 상태로 유지해야한다. 보안 설정 오류는 수동 또는 ad hoc(또는 구성되지 않음), 안전하지 않은 기본 구성, 열린 S3 bucket, 잘못 구성된 HTTP 헤더, 민감정보가 포함된 오류 메시지, 패치되지 않은 시스템, 프레임워크 종속성, 컴포넌트를 적시에 업그레이드 하지 않는 것이 일반적이다. 이 취약점을 예방하기 위해선 불필요한 기능, 구성 요소를 최소화하는 것이 좋다. 사용하지 않는 기능 및 프레임워크를 제거하거나 불필요하게 설치 하지 않는 것 또한 중요하다.

## 취약점 예시
- 불필요한 기능을 활성화했거나 설치함 (ex, 불필요 포트, 불필요 페이지, 불필요 계정/권한 등)
- 벤더사에서 기본적으로 제공하는 관리자 계정을 변경하지 않고 그대로 사용하는 경우
- 에러 페이지를 통한 애플리케이션 에러 정보 노출
- 보안 헤더 설정 누락
- 서버 OS, 프레임워크, 라이브러리, 데이터베이스 등의 보안 설정 누락

## 예방 방법
- 애플리케이션 설치 시 불필요한 기능, 구성요소, 샘플, 문서 등을 제거
- 보안 헤더 설정
- 모든 환경에서 애플리케이션 보안 설정을 검증하고 자동화된 프로세스 구현
- 기본 제공하는 관리자 계정 정보를 회사 규정에 맞게 변경
- 서버에서 디렉터리를 나열할 수 있는 디렉터리 목록 페이지 비활성화
