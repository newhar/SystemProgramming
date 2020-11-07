# SystemProgramming

1.nilfs 

2.packetforwarding

## Overview

### 1. nilfs2
- Log Structured 파일 시스템인 Nilfs2과 기분 리눅스의 파일시스템인 Ext4의 쓰기 동작 방식을 알아본다.
- Nilfs2 와 Ext4 파일 시스템의 쓰기 동작 패턴을 확인하기 위해 disk I/O 벤치마크를 사용해 실험을 수행한다.
- Nilfs2와 Ext4 파일 시스템의 쓰기 동작 방식을 비교한다.
- 다양한 파일 시스템을 지원하기 위해 Virtual File System에서 사용되는 구조체에 대해 알아본다
- Proc fild System 을 이용하여 데이터를 수집한다.

Report File : 'SystemProgramming/nilfs2/hw1_보고서.pdf' 

### 2.packetforwarding
- netfilter의 hooking을 이용해 skb의 패킷 헤더를 가져와 조건에 따라 포워딩하고 포워딩된 패킷이 제대로 도착했는지 여부를 확인하는 모니터링 모듈을 구현
- IP_layer 네트워크 구현 분석
- Packet forwarding / monitoring 커널 모듈 작성
- Proc file system 이용

Report File : 'SystemProgramming/packetforwarding/2019_SP_2차과제.pdf'

## Environment
- C
- Virtual Machine(Ubuntu)

