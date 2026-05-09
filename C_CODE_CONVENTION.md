# Pintos C Code Convention

이 문서는 현재 `pintos` 코드베이스의 스타일을 참고해 만든 C 코드 컨벤션입니다.
목표는 기존 베이스 코드와의 일관성을 유지하면서, 팀 구현 코드에서 흔들리기 쉬운 부분을
하나의 기준으로 고정하는 것입니다.

적용 범위:

- `pintos/threads`
- `pintos/userprog`
- `pintos/vm`
- `pintos/filesys`
- `pintos/devices`
- `pintos/lib`
- `pintos/tests` 내 C 코드

기본 원칙:

- 기존 Pintos 베이스 코드와 자연스럽게 섞이는 스타일을 따른다.
- 읽기 쉬움보다 더 중요한 것은 없다.
- 커널 코드이므로 "짧게 쓰기"보다 "명확하게 쓰기"를 우선한다.
- 인터럽트, 락, 메모리, 자원 해제 조건은 코드와 주석에서 분명해야 한다.

## 1. 파일 구성

소스 파일은 아래 순서를 기본으로 한다.

1. 같은 모듈의 헤더
2. 시스템/표준 라이브러리 헤더
3. 프로젝트 내부 헤더
4. 매크로, 전역 변수, `static` 함수 선언
5. 공개 함수
6. 파일 내부 전용 `static` 함수

예:

```c
#include "threads/thread.h"
#include <debug.h>
#include <stddef.h>
#include <stdio.h>
#include "threads/interrupt.h"
#include "threads/palloc.h"
```

규칙:

- 같은 성격의 include끼리는 묶고, 그룹 사이에는 빈 줄 하나를 둔다.
- `.c` 파일에서만 쓰는 헬퍼 함수는 반드시 `static`으로 선언한다.
- 공개 함수는 헤더에 선언하고, 내부 구현 전용 함수는 헤더에 노출하지 않는다.

## 2. 들여쓰기와 줄바꿈

현재 베이스 코드는 탭 기반 들여쓰기가 가장 강하게 나타난다. 따라서 다음을 표준으로 삼는다.

- 들여쓰기는 탭을 사용한다.
- 같은 블록 안에서 탭과 스페이스를 섞지 않는다.
- 정렬을 위해 필요한 경우에만 최소한의 스페이스를 사용한다.
- 빈 줄에는 공백을 남기지 않는다.
- 파일 끝은 개행 문자 하나로 끝난다.

줄 길이:

- 주석과 일반 코드는 80자를 우선 목표로 한다.
- 불가피한 경우 100자까지 허용한다.
- 100자를 넘기면 줄바꿈을 우선 검토한다.

긴 인자 목록은 다음처럼 줄바꿈한다.

```c
tid_t
thread_create (const char *name, int priority,
		thread_func *function, void *aux) {
	...
}
```

## 3. 중괄호와 공백

Pintos 베이스 코드의 K&R 스타일을 따른다.

함수 정의:

```c
void
thread_init (void) {
	...
}
```

제어문:

```c
if (condition) {
	...
} else {
	...
}
```

공백 규칙:

- 함수 이름과 괄호 사이에 공백을 둔다.
- 제어문 키워드와 괄호 사이에 공백을 둔다.
- 이항 연산자 앞뒤에는 공백을 둔다.
- 콤마 뒤에는 공백을 둔다.
- 캐스팅은 `(type) value` 형태를 사용한다.

예:

```c
struct thread *t = thread_current ();
if (t->priority > thread_current ()->priority)
	thread_yield ();

f->R.rax = syscall_open ((const char *) f->R.rdi);
```

금지:

```c
thread_current();
if(condition){
...
}
```

## 4. 조건문과 반복문

기존 코드에는 한 줄짜리 본문에서 중괄호를 생략한 예가 많다. 하지만 새로 작성하거나 크게 수정하는
코드에서는 다음 기준을 사용한다.

- `if`, `else`, `for`, `while` 본문은 가능하면 중괄호를 사용한다.
- 예외는 즉시 반환하는 한 줄 가드 정도로 제한한다.
- 인터럽트, 락, 메모리 해제, 에러 처리와 관련된 분기에는 중괄호를 생략하지 않는다.

권장:

```c
if (fn_copy == NULL) {
	return TID_ERROR;
}
```

허용:

```c
if (inode == NULL)
	return NULL;
```

## 5. 네이밍

### 5.1 일반 원칙

- 이름은 역할이 드러나게 짓는다.
- 축약형은 코드베이스에서 이미 널리 쓰이는 경우만 사용한다.
- 한 파일 안에서는 같은 개념에 같은 이름을 사용한다.

### 5.2 함수

공개 함수는 모듈 접두사를 사용한다.

- `thread_create`
- `thread_current`
- `process_exec`
- `filesys_open`
- `vm_claim_page`

파일 내부 전용 헬퍼는 짧아도 되지만 동작이 드러나야 한다.

- `do_format`
- `refresh_priority`
- `fd_to_file`

### 5.3 타입과 필드

- `struct`, `enum`, `typedef`는 `snake_case`를 사용한다.
- 구조체 필드는 명사형으로 짓는다.
- 불리언 필드는 참일 때 의미가 드러나게 짓는다.

예:

- `struct child_info`
- `enum thread_status`
- `load_success`
- `removed`
- `writable`

### 5.4 매크로와 상수

- 매크로는 대문자 `SNAKE_CASE`를 사용한다.
- 모듈 문맥이 드러나도록 접두사를 붙인다.

예:

- `THREAD_MAGIC`
- `THREAD_BASIC`
- `TIME_SLICE`
- `PRI_MAX`
- `FD_MAX`

## 6. 주석

현재 베이스 코드는 함수 앞 설명 주석과 블록 설명 주석을 적극적으로 사용한다. 이 패턴을 유지한다.

규칙:

- 공개 함수에는 역할, 입력, 반환값, 실패 조건을 설명하는 주석을 붙인다.
- 인터럽트 컨텍스트, 락 보유 조건, 호출 순서 제약은 반드시 주석으로 남긴다.
- "무엇을 하는지"보다 "왜 이렇게 해야 하는지"를 설명한다.
- 한 줄 설명은 `/* ... */` 또는 짧은 `//`를 허용하되, 파일 전체에서는 한 스타일로 과하게 섞지 않는다.
- 새 주석은 한국어 또는 영어 중 하나로 통일한다.
- 공용 헤더와 범용 라이브러리 코드는 영어 주석을 우선한다.

좋은 예:

```c
/* Called by the timer interrupt handler at each timer tick.
   Thus, this function runs in an external interrupt context. */
```

부족한 예:

```c
/* value 증가 */
```

## 7. 함수 작성 원칙

- 함수는 한 가지 책임만 갖게 작성한다.
- 외부에 노출할 필요가 없는 함수는 `static`으로 제한한다.
- 함수 시작부에서 전제 조건을 빠르게 검증한다.
- 커널 내부 불변식은 `ASSERT`로 검증한다.
- 사용자 입력이나 런타임 실패 가능성은 일반 조건문으로 처리한다.

예:

```c
ASSERT (sema != NULL);
ASSERT (!intr_context ());
```

반환 규칙:

- 성공/실패가 분명한 함수는 `bool`을 우선 사용한다.
- 자원 할당 함수는 실패 시 `NULL`, `TID_ERROR`, `-1` 등 모듈에서 이미 쓰는 실패 값을 따른다.
- 한 모듈 안에서는 같은 종류의 실패를 같은 방식으로 표현한다.

## 8. 변수 선언과 초기화

- 변수는 처음 사용되는 위치와 너무 멀지 않게 선언한다.
- 단, 함수 초반에 의미 있는 로컬 상태를 모아두는 것은 허용한다.
- 포인터, 파일, 페이지, 락 같은 자원 핸들은 선언 직후 초기화한다.
- 새로 할당한 구조체와 배열은 가능한 즉시 초기 상태를 채운다.

예:

```c
struct inode_disk *disk_inode = NULL;
bool success = false;
```

```c
child->tid = TID_ERROR;
child->exit_status = -1;
child->exited = false;
```

## 9. 에러 처리와 자원 해제

Pintos 코드에서는 메모리, 페이지, 파일, 락 해제가 빠지면 바로 디버깅 비용이 커진다. 에러 처리는
반드시 일관되게 작성한다.

규칙:

- `malloc`, `calloc`, `palloc_get_page` 결과는 반드시 검사한다.
- 중간 실패 시 이미 획득한 자원은 역순으로 해제한다.
- 실패 분기가 3개 이상이면 정리용 블록을 두는 방식을 우선 검토한다.
- 성공 경로와 실패 경로가 눈에 잘 들어오도록 배치한다.

예:

```c
fn_copy = palloc_get_page (0);
name_copy = palloc_get_page (0);

if (fn_copy == NULL || name_copy == NULL) {
	if (fn_copy != NULL) {
		palloc_free_page (fn_copy);
	}
	if (name_copy != NULL) {
		palloc_free_page (name_copy);
	}
	return TID_ERROR;
}
```

## 10. 헤더 파일 규칙

- 모든 헤더는 include guard를 사용한다.
- guard 이름은 경로 기반 대문자 이름을 사용한다.
- 헤더에는 필요한 선언만 넣고 구현 세부 사항은 숨긴다.
- 전역 변수는 꼭 필요한 경우에만 `extern`으로 노출한다.

예:

```c
#ifndef THREADS_THREAD_H
#define THREADS_THREAD_H
...
#endif /* threads/thread.h */
```

## 11. 조건부 컴파일

Pintos는 `USERPROG`, `VM`, `EFILESYS` 같은 빌드 조건을 많이 사용하므로 범위를 작게 유지한다.

규칙:

- `#ifdef`는 필요한 최소 범위만 감싼다.
- 조건부 필드, include, 함수 선언은 관련 위치에 가깝게 둔다.
- 한 기능이 통째로 옵션인 경우가 아니면 함수 전체를 크게 감싸기보다 필요한 부분만 분기한다.

예:

```c
#ifdef VM
#include "vm/vm.h"
#endif
```

## 12. Pintos 특화 규칙

### 12.1 스택 사용

`struct thread`가 페이지 하단에 놓이고 커널 스택이 같은 페이지를 쓰기 때문에, 큰 지역 변수는 매우
위험하다.

- 큰 배열이나 큰 구조체를 지역 변수로 두지 않는다.
- 큰 버퍼는 `malloc ()` 또는 `palloc_get_page ()`로 할당한다.
- 재귀 사용은 피한다.

### 12.2 동기화

- 인터럽트 비활성화 구간은 최대한 짧게 유지한다.
- 락/세마포어 관련 함수는 호출 전제 조건을 `ASSERT`와 주석으로 분명히 한다.
- 공유 자료구조를 수정하는 코드는 어떤 락 또는 인터럽트 상태에서 안전한지 드러나야 한다.

### 12.3 리스트와 페이지 자료구조

- `list_entry()` 사용 시 어떤 타입의 어떤 필드를 꺼내는지 명확히 적는다.
- `struct list_elem`는 용도를 이름으로 구분한다.
- 페이지/프레임/파일 디스크립터는 소유권이 어디로 이동하는지 주석이나 이름으로 드러낸다.

## 13. 테스트 코드

`pintos/tests`의 코드는 본 코드보다 짧을 수 있지만, 아래는 동일하게 지킨다.

- 공용 함수/매크로 이름 충돌을 피한다.
- 실패 메시지는 무엇이 틀렸는지 바로 알 수 있게 쓴다.
- 테스트도 자원 누수를 남기지 않는다.

## 14. 기존 코드와의 관계

현재 저장소에는 다음과 같은 흔들림이 일부 존재한다.

- 탭과 스페이스 혼용
- 함수 호출 괄호 앞 공백 누락
- include 정렬 불일치
- 한국어/영어 주석 혼용
- 한 줄 분기에서 중괄호 사용 기준 불일치

이 문서의 기준은 "기존 베이스 코드와 가장 자연스럽게 맞는 방향"을 우선으로 정한 것이다.
기존 레거시 코드를 한 번에 모두 고치기보다는, 새 코드와 수정하는 코드부터 이 기준에 맞춘다.

## 15. 요약 체크리스트

- include는 같은 모듈, 시스템, 프로젝트 순으로 묶었는가
- 들여쓰기는 탭으로 통일했는가
- 함수 정의는 Pintos 스타일로 줄바꿈했는가
- 함수/제어문 괄호 앞 공백을 지켰는가
- `static`으로 숨길 수 있는 함수가 헤더에 노출되지 않았는가
- `ASSERT`와 일반 에러 처리를 구분했는가
- 메모리/파일/페이지 해제가 모든 실패 경로에서 보장되는가
- 인터럽트/락 전제 조건이 코드와 주석에 드러나는가
- 큰 지역 변수를 두지 않았는가
- 새 주석의 언어와 톤이 함수 내부에서 일관적인가


---

## 16. 개발할 때 참고하는 방법

### 구현 전

```
1. 함수 이름 먼저 정하기       → 5.2 네이밍 확인
2. public vs static 결정       → 1번, 7번 확인
3. 에러 처리 경로 먼저 설계    → 9번 확인
4. 동기화 필요 여부 확인       → 12.2 확인
```

### 구현 중

```
1. 함수 작성 전 주석 먼저      → 6번
   - 역할, 입력, 반환값, 실패 조건
2. malloc/palloc 쓸 때         → 해제 경로 바로 작성 (9번)
3. ASSERT vs 에러처리 구분     → 7번
4. 미완성 부분 표시            → TODO(이름) 주석
```

### PR 올리기 전

COMMIT_CONVENTION.md의 PR 체크리스트를 반드시 확인한다.

---

## 17. 보완 사항

### 17.1 페이지/프레임 소유권 명시

Project 3에서 가장 많이 실수하는 부분이다.
함수가 소유권을 가져가는지, 빌려주는 것인지 반드시 주석으로 명시한다.

```c
/* 이 함수는 frame의 소유권을 가져간다.
   호출자는 이후 frame을 직접 해제하면 안 된다. */
void
vm_frame_insert (struct frame *frame) {
    ...
}

/* frame을 반환한다. 소유권은 호출자에게 남는다. */
struct frame *
vm_frame_lookup (void *kaddr) {
    ...
}
```

### 17.2 TODO / FIXME 주석 규칙

미완성 부분은 담당자 이름을 포함해 표시한다.

```c
/* TODO(나코): lazy init 구현 필요 */
/* FIXME(승진): 이 경로에서 lock 해제 누락 가능 */
/* HACK(지운): 임시 처리, spt 연동 후 제거 */
```

### 17.3 커밋 메시지 연동

코드 컨벤션과 커밋 단위는 함께 동작한다.
COMMIT_CONVENTION.md를 참고해 하나의 의도 단위로 커밋한다.

```
feat(vm): vm_alloc_page 구현
feat(vm): vm_claim_page 구현    ← 별도 커밋
fix(vm): frame 해제 누락 수정   ← 별도 커밋
```

### 17.4 인터럽트/락 상태 명시 강화

락과 인터럽트 상태는 함수 진입/종료 시 반드시 주석으로 드러낸다.

```c
/* 락 보유 상태에서 호출해야 한다: vm_lock
   인터럽트 비활성화 상태에서 호출 금지 */
static void
vm_do_claim_page (struct page *page) {
    ASSERT (lock_held_by_current_thread (&vm_lock));
    ...
}
```
