# Pintos Commit Convention

이 문서는 팀 4명이 같은 방식으로 커밋 메시지를 작성하기 위한 컨벤션입니다.
코드 리뷰와 머지 과정에서 변경 의도를 빠르게 파악할 수 있도록 합니다.

---

## 1. 커밋 메시지 형식

```
<type>(<scope>): <subject>

[body]

[footer]
```

- `type`과 `scope`는 필수, `body`와 `footer`는 선택
- 제목(subject)은 50자 이내, 명령형으로 작성
- 본문(body)은 72자 줄바꿈, "무엇을"보다 "왜"를 설명

---

## 2. Type 종류

| Type | 설명 | 예시 |
|------|------|------|
| `feat` | 새 기능 구현 | `feat(vm): vm_claim_page 구현` |
| `fix` | 버그 수정 | `fix(userprog): fd_table 누수 수정` |
| `docs` | 주석, 문서 수정 | `docs(thread): thread_create 주석 보완` |
| `refactor` | 동작 변경 없는 코드 개선 | `refactor(vm): do_fork 에러 처리 정리` |
| `test` | 테스트 추가/수정 | `test(vm): page_fault 테스트 추가` |
| `chore` | 빌드, 설정 변경 | `chore: Makefile 경로 수정` |
| `wip` | 작업 중 (머지 금지) | `wip(vm): lazy init 진행 중` |

---

## 3. Scope 종류

Pintos 디렉토리 기준으로 작성한다.

| Scope | 해당 범위 |
|-------|----------|
| `vm` | pintos/vm |
| `userprog` | pintos/userprog |
| `thread` | pintos/threads |
| `filesys` | pintos/filesys |
| `device` | pintos/devices |
| `lib` | pintos/lib |

---

## 4. Subject 작성 규칙

- 명령형으로 시작한다 ("구현", "수정", "추가", "제거")
- 마침표를 붙이지 않는다
- 50자를 넘기지 않는다
- 한국어 또는 영어 중 팀이 정한 한 가지로 통일한다

좋은 예:
```
feat(vm): vm_alloc_page 구현
fix(userprog): exit status 저장 누락 수정
docs(vm): supplementary page table 주석 추가
```

나쁜 예:
```
수정함
고침
feat: 여러가지 수정하고 기능도 추가하고 버그도 잡음.
```

---

## 5. Body 작성 규칙

제목만으로 의도가 충분히 전달되면 생략 가능하다.
아래 경우에는 반드시 작성한다.

- 왜 이렇게 구현했는지 설명이 필요한 경우
- 이전 동작과 달라진 점이 있는 경우
- 다른 팀원이 이해하지 못할 수 있는 복잡한 로직

예:
```
fix(vm): page fault 핸들러에서 spt 조회 실패 시 exit 추가

기존에는 spt에 없는 주소 접근 시 커널 패닉이 발생했음.
유저 프로그램의 잘못된 접근으로 판단하고 exit(-1) 처리함.
```

---

## 6. Footer 작성 규칙

이슈 번호 연결이나 브레이킹 체인지 표시에 사용한다.

```
fix(userprog): wait syscall 반환값 수정

Closes #12
```

---

## 7. TODO / FIXME 주석 규칙

커밋 단위로 해결하지 못한 부분은 코드에 표시한다.
담당자 이름을 반드시 포함한다.

```c
/* TODO(나코): lazy init 구현 필요 */
/* FIXME(승진): 이 경로에서 lock 해제 누락 가능 */
/* HACK(지운): 임시 처리, 추후 spt 연동 후 제거 */
```

`wip` 커밋과 함께 쓰면 어디까지 했는지 추적하기 쉽다.

---

## 8. 커밋 단위 기준

커밋은 **"하나의 의도"** 단위로 쪼갠다.

좋은 예:
```
feat(vm): vm_alloc_page 구현
feat(vm): vm_claim_page 구현
fix(vm): frame 해제 누락 수정
```

나쁜 예:
```
여러 파일 수정
vm 작업
```

**커밋 가능 기준:**
- 이 커밋 하나를 팀원에게 한 문장으로 설명할 수 있다
- 이 커밋이 잘못됐을 때 이것만 되돌릴 수 있다
- 빌드가 깨지지 않는다 (wip 제외)

---

## 9. 브랜치 전략

```
main
 └── {이름}/{기능}
      예: nako/vm-claim-page
          seungjin/lazy-init
          jiun/swap-in
```

- 머지 조건: 팀원 전원이 코드를 설명할 수 있을 때
- wip 커밋이 있는 브랜치는 머지 금지
- 머지 후 브랜치 삭제

---

## 10. PR 체크리스트

PR 올리기 전 반드시 확인한다.

```
## 코드 컨벤션
- [ ] include 순서 (모듈 → 시스템 → 프로젝트)
- [ ] 탭 들여쓰기 통일
- [ ] 함수/제어문 괄호 앞 공백
- [ ] static 함수 헤더 미노출
- [ ] ASSERT vs 에러처리 구분

## 자원 관리
- [ ] malloc/palloc 모든 실패 경로에서 해제
- [ ] 파일/락 해제 누락 없음
- [ ] 큰 지역 변수 없음

## 동기화
- [ ] 인터럽트/락 전제 조건 주석
- [ ] 공유 자료구조 수정 시 락 보유 여부

## 이해 확인
- [ ] 팀원 전원이 이 코드를 말로 설명할 수 있는가
- [ ] wip 커밋이 없는가
- [ ] TODO/FIXME 중 이번 PR에서 해결해야 할 것은 없는가
```
