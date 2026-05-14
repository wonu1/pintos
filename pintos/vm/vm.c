/* vm.c: Generic interface for virtual memory objects. */

#include "threads/malloc.h"
#include "vm/vm.h"
#include "vm/inspect.h"

/* Returns a hash value for page p. */
unsigned
page_hash (const struct hash_elem *p_, void *aux UNUSED) {
	const struct page *p = hash_entry(p_, struct page, hash_elem);
	return hash_bytes(&p->va, sizeof p->va);
}

/* Returns true if page a precedes page b. */
bool page_less (const struct hash_elem *a_,
			   const struct hash_elem *b_, void *aux UNUSED) {
	const struct page *a = hash_entry(a_, struct page, hash_elem);
	const struct page *b = hash_entry(b_, struct page, hash_elem);

	return a->va < b->va;
}
/* Initializes the virtual memory subsystem by invoking each subsystem's
 * intialize codes. */
void vm_init(void)
{
	vm_anon_init();
	vm_file_init();
#ifdef EFILESYS /* For project 4 */
	pagecache_init();
#endif
	register_inspect_intr();
	/* DO NOT MODIFY UPPER LINES. */
	/* TODO: Your code goes here. */
}

/* Get the type of the page. This function is useful if you want to know the
 * type of the page after it will be initialized.
 * This function is fully implemented now. */
enum vm_type
page_get_type(struct page *page)
{
	int ty = VM_TYPE(page->operations->type);
	switch (ty)
	{
	case VM_UNINIT:
		return VM_TYPE(page->uninit.type);
	default:
		return ty;
	}
}

/* Helpers */
static struct frame *vm_get_victim(void);
static bool vm_do_claim_page(struct page *page);
static struct frame *vm_evict_frame(void);

/* Create the pending page object with initializer. If you want to create a
 * page, do not create it directly and make it through this function or
 * `vm_alloc_page`. */
bool vm_alloc_page_with_initializer(enum vm_type type, void *upage, bool writable,
									vm_initializer *init, void *aux) {

	ASSERT(VM_TYPE(type) != VM_UNINIT)

	struct supplemental_page_table *spt = &thread_current()->spt;

	/* Check wheter the upage is already occupied or not. */
	if (spt_find_page(spt, upage) == NULL) {
		/* TODO: Create the page, fetch the initialier according to the VM type,
		 * TODO: and then create "uninit" page struct by calling uninit_new. You
		 * TODO: should modify the field after calling the uninit_new. */
		struct page *page = malloc(sizeof(struct page));
		if (page == NULL) {
			return false;
		}
		uninit_new(page, upage, init, type, aux, anon_initializer);
		page->writable = writable;

		/* TODO: Insert the page into the spt. */
		return spt_insert_page(spt, page);
	}
err:
	return false;
}

/* 주어진 supplemental page table에서 va에 대응하는 struct page를 찾습니다. 실패하면 NULL을 반환합니다. */
struct page *
spt_find_page (struct supplemental_page_table *spt, void *va) {

	/* 타입 설계 원칙 : 지역 변수로 Page를 만듦 */
	struct page p;
	struct hash_elem *e;

	p.va = va;
	e = hash_find(&spt->hash_table, &p.hash_elem);

	/* 조건 ? 참일때 실행 : 거짓일 때 실행 */
	return e != NULL ? hash_entry(e, struct page, hash_elem) : NULL;
}

/* Insert PAGE into spt with validation. */
bool spt_insert_page(struct supplemental_page_table *spt UNUSED,
					 struct page *page UNUSED) {
	int succ = false;
	/* TODO: Fill this function. */
	/*주어진 supplemental page table에 struct page를 삽입합니다.
	 이 함수는 해당 virtual address가 주어진 supplemental page table 안에 이미 존재하지 않는지 확인해야 합니다.*/
	if (hash_insert (&spt->hash_table, &page->hash_elem) == NULL) {
		succ = true;
	}
	return succ;
}

void spt_remove_page(struct supplemental_page_table *spt, struct page *page)
{
	vm_dealloc_page(page);
	return true;
}

/* Get the struct frame, that will be evicted. */
static struct frame *
vm_get_victim(void)
{
	struct frame *victim = NULL;
	/* TODO: The policy for eviction is up to you. */

	return victim;
}

/* Evict one page and return the corresponding frame.
 * Return NULL on error.*/
static struct frame *
vm_evict_frame(void)
{
	struct frame *victim UNUSED = vm_get_victim();
	/* TODO: swap out the victim and return the evicted frame. */

	return NULL;
}

/* palloc() and get frame. If there is no available page, evict the page
 * and return it. This always return valid address. That is, if the user pool
 * memory is full, this function evicts the frame to get the available memory
 * space.
 *
 * palloc_get_page를 호출해서 user pool에서 새 physical page를 얻습니다.
 * user pool에서 page를 성공적으로 얻으면 frame도 할당하고,
 * 그 멤버들을 초기화한 뒤 반환합니다.
 * vm_get_frame을 구현한 뒤에는 모든 user space page(PALLOC_USER)를
 * 이 함수를 통해 할당해야 합니다.
 * 지금은 page allocation이 실패했을 때
 * swap out을 처리할 필요가 없습니다.
 * 그런 경우는 일단 PANIC("todo")로 표시해 두면 됩니다.
 * */
static struct frame *
vm_get_frame(void)
{
	struct frame *frame = NULL;
	/* TODO: Fill this function. */
	void *new_page_va = palloc_get_page(PAL_USER);
	if (new_page_va == NULL)
	{
		// page evict 하고 페이지 리턴하기
		// page_evict();
		// if (page_evict() == 실패) { PANIC("todo");}

		// page_evict();
		// frame->kva = new_page_va;

		// if(!page_evict()){
		// 	PANIC("todo");
	}
	else
	{
		// 1) user pool에서 page를 성공적으로 얻으면 frame도 할당하고,
		// 2) 그 멤버들을 초기화한 뒤 반환합니다
	}

	ASSERT(frame != NULL);
	ASSERT(frame->page == NULL);
	return frame;
}

/* Growing the stack. */
static void
vm_stack_growth(void *addr UNUSED)
{
}

/* Handle the fault on write_protected page */
static bool
vm_handle_wp(struct page *page UNUSED)
{
}

/* Return true on success */
bool vm_try_handle_fault (struct intr_frame *f UNUSED, void *addr UNUSED,
						 bool user UNUSED, bool write UNUSED, bool not_present UNUSED) {
	/* 권한이 없는 주소에 접근하려고 할 때*/
	if (not_present == false) {
		process_exit ();
		return false;
	}
	// 현재 thread(process)의 spt의 주소를 설정
	struct supplemental_page_table *spt = &thread_current ()->spt;
	struct page *page = spt_find_page (spt, addr);

	if (page == NULL) {
		vm_stack_growth (); /* TODO: stack 할 때 진행 */
	} else {
		/* read-only 페이지에 쓰려고 할 때 */
		if (write && !page->writable) {
			process_exit ();
			return false;
		}
	
		/* User 모드에서 커널 주소에 접근하려고 할 때*/
		if (user && is_kernel_vaddr(addr)) {
			process_exit ();
			return false;
		}
	}
	
	return vm_do_claim_page(page);
}

/* Free the page.
 * DO NOT MODIFY THIS FUNCTION. */
void vm_dealloc_page(struct page *page)
{
	destroy(page);
	free(page);
}

/* Claim the page that allocate on VA. */
bool vm_claim_page(void *va UNUSED)
{
	struct page *page = NULL;
	/* TODO: Fill this function */

	return vm_do_claim_page(page);
}

/* Claim the PAGE and set up the mmu.
	MMU를 설정해야 합니다.
	즉 page table에 virtual address에서
	physical address로 가는 mapping을 추가해야 합니다.
	반환값은 이 작업이 성공했는지 여부를 나타내야 합니다.
*/
static bool
vm_do_claim_page(struct page *page)
{
	struct frame *frame = vm_get_frame();

	/* Set links */
	frame->page = page;
	page->frame = frame;

	/* TODO: Insert page table entry to map page's VA to frame's PA. */
	// 해당 페이지에 대한
	// pml4_set_page(현재 쓰레드에서 pml4 가져오고, upage, frame에서 가져오는거 아닐까?, read/write);
	// if (pml4_set_page == 실패) return false;

	return swap_in(page, frame->kva);
}

/* Initialize new supplemental page table */
void supplemental_page_table_init(struct supplemental_page_table *spt UNUSED)
{
	hash_init(&spt->hash_table, page_hash, page_less, NULL);
}

/* Copy supplemental page table from src to dst */
bool supplemental_page_table_copy(struct supplemental_page_table *dst UNUSED,
								  struct supplemental_page_table *src UNUSED)
{
}

/* Free the resource hold by the supplemental page table */
void supplemental_page_table_kill(struct supplemental_page_table *spt UNUSED)
{
	/* TODO: Destroy all the supplemental_page_table hold by thread and
	 * TODO: writeback all the modified contents to the storage. */
}
