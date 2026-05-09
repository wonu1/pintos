#ifndef VM_ANON_H
#define VM_ANON_H
#include "vm/vm.h"
struct page;
enum vm_type;

struct anon_page
{
    /* TODO ✅
        예를 들어 어떤 page가 anonymous page라면,
        그 struct page는 멤버 중 하나로 struct anon_page anon 필드를 사용합니다.
        anon_page에는 anonymous page를 위해 보관해야 하는 필요한 정보가 들어갑니다.
    */
};

void vm_anon_init(void);
bool anon_initializer(struct page *page, enum vm_type type, void *kva);

#endif
