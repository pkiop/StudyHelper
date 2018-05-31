﻿# 공부도우미

PC로 공부할 때 켜놓으면 얼마나 공부했는지 쉽고 정확하게 알 수 있다.

## 기능

### 공부할 리스트 불러오기

exe파일이 있는 폴더에 양식에 맞게 작성된 list.txt가 있으면 가져와서 리스트에 추가한다.

### 리스트, 공부시간을 저장한 txt 파일을 만들어준다.

프로그램이 종료할 때 저장, 1분마다 자동저장, 직접 저장 할 때마다 추가된 데이터로 txt파일을 만들어 준다. 

### 타임라인

타임라인에 현재 공부하는 걸 적어두면 언제 무슨 공부를 했는지 파악하기 좋다.

### 중단, 재개

공부하다가 일이 생기거나 쉴 때 중단하고 쉬고 재개하면 중단한 이유를 적을 수도 있다. 중단 시간은 체크해서 공부시간에서 빼준다.

### 시간 체크

프로그램 시작시간과 종료시간(저장시간) 비교해서 프로그램 작동시간을 체크하고 중단 시간을 빼준다.

## update 

#### version 1.2
여전히 시간 - 나오는 문제
코드 정렬 필요
이름이 같으면 기존파일이 지워진다. 
00시가 지나면 시간도 -가 된다. day까지 고려하던가 해야한다. 

#### version 1.3

모든 -의 경우 다 수정해 주었다. 

#### version 1.4

수정해야할 것

plan output에서 endl 두번 되는 것 수정. 
초 -되는 것
입력하세요에서 int나 그런걸로 입력 받아서 cache[N - 1][L][R] * (N - 2)를 입력하니까 잘못된입력입니다를 오랜시간동안 내보냈음이 아니라 무한루프 
타임라인에 -초가 아직도 나온다. 

추가할 것
PC모르고 종료하거나 모르고 끄면 자동으로 저장되게 혹은 자동 동기화 기능 - 5분에 한번씩 자동저장등 
공부 한 시간 나오게 

####version 1.5

추가한 것 
자동저장

수정해야할 것

초 -되는 것
입력하세요에서 int나 그런걸로 입력 받아서 cache[N - 1][L][R] * (N - 2)를 입력하니까 잘못된입력입니다를 오랜시간동안 내보냈음이 아니라 무한루프 
타임라인에 -초가 아직도 나온다. 
