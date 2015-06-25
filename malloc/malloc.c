#define NULL 0
#define MEMSIZE 8096

typedef double Align;
typedef union header
{
    struct
    {
        union header* next;
        unsigned used_size;
        unsigned free_size;
    }s;
    Align a;
}Header;

static Header mem[MEMSIZE];
static Header *mem_ptr = NULL;

void* malloc(unsigned nbytes)
{
    Header* p;
    Header* newp;
    unsigned nunits;
    nunits = (nbytes+sizeof(Header)-1)/sizeof(Header)+1;
    if(mem_ptr == NULL)
    {
        mem_ptr->s.next = mem_ptr = mem;
        mem_ptr->s.used_size = 1;
        mem_ptr->s.free_size = MEMSIZE - 1;
    }
    for(p=mem_ptr;
        (p->s.next != mem_ptr) && (p->s.free_size<nunits);
        p==->next);
    if(p->s.free_size < nunits )
    {
        return NULL;
    }
    newp = p + p->s.used_size;
    newp->s.used_size = nunits;
    newp->s.free_size = p->s.free_size - nunits;
    newp->s.next = p->s.next;
    p->s.free_size = 0;
    p->s.next = newp;
    mem_ptr = newp;
    return (void*)(newp+1);
}

void free(void* ap)
{
    Header* bp;
    Heder* p;
    Header* prev;
    for(prev = mem_ptr, p=mem_ptr->s.next;
        (p!=bp)&&(p!=mem_ptr);
        prev=p,p=p->s.next);
    if(p!=bp)
    {
        return ;
    }
    prev->s.free_size += p->s.used_size + p->s.free_size;
    prev->s.next = p->s.next;
    mem_ptr = prev;
}