#define NULL 0
#define MEMSIZE 8096

typedef double Align;
typedef union header
{
	struct
	{
		union header* next;/*指向下一个节点*/
		unsigned used_size;/*已经被分配的大小*/
		unsigned free_size;/*剩余空闲大小*/
	}s;
	Align a;/*不知道这是什么鬼, 但是这个union至少有double那么大*/
}Header;

static Header mem[MEMSIZE];/*开一个大数组*/
static Header *mem_ptr = NULL;/*这个也不知道什么鬼*/

void* malloc(unsigned nbytes)
{
	Header* p;
	Header* newp;
	unsigned nunits;
	/*计算需要多少个单元*/
	nunits = (nbytes + sizeof(Header) - 1) / sizeof(Header) + 1;
	/*如果第一次分配, 此时mem_ptr为空*/
	if (mem_ptr == NULL)
	{
		/*让mem_ptr及其next指针指向mem*/
		mem_ptr->s.next = mem_ptr = mem;
		/*并记录已经使用的单元*/
		mem_ptr->s.used_size = 1;
		/*记录空闲空间减少1*/
		mem_ptr->s.free_size = MEMSIZE - 1;
	}
	/*不管是不是第一次分配, 让p指向mem_ptr, 然后一直往后移动, 寻找第一个自由*/
	/*空间大于需要分配的空间的节点*/
	for (p = mem_ptr;
		(p->s.next != mem_ptr) && (p->s.free_size < nunits);
		p = p->s.next);
	/*因为p->s.next == mem_ptr时会退出循环, 此时应该是第一次分配, 但是空间仍*/
	/*不够, 所以分配失败, 返回NULL*/
	if (p->s.free_size < nunits)
	{
		return NULL;
	}
	/*其余情况应该是能分配的*/
	/*首先, 一个新的指针指向该节点后一些位置, 避开已占用内存*/
	newp = p + p->s.used_size;
	/*然后新建一个节点, 指定已经使用的大小为现在说分配的大小*/
	newp->s.used_size = nunits;
	/*指定剩余空间大小为前驱节点的剩余空间大小减去要分配的大小*/
	newp->s.free_size = p->s.free_size - nunits;
	/*插入节点*/
	newp->s.next = p->s.next;
	p->s.free_size = 0;
	p->s.next = newp;
	/*newp应该是最后一个节点, 这里让mem_ptr指向newp是为毛?*/
	mem_ptr = newp;
	return (void*)(newp + 1);/*避开新节点所占用的内存*/
}

void free(void* ap)
{
	/*令bp为ap所在的节点的指针*/
	Header* bp=((Header*)(ap)-1);
	Header* p;
	Header* prev;
	/*从mem_ptr开始找, 一直往下, 找到所在节点或找不到*/
	for (prev = mem_ptr, p = mem_ptr->s.next;
		(p != bp) && (p != mem_ptr);
		prev = p, p = p->s.next);
	if (p != bp)
	{
		/*找不到直接返回了*/
		return;
	}
	/*找到了就释放内存, 所谓释放就是让该节点的前驱的自由空间增大*/
	prev->s.free_size += p->s.used_size + p->s.free_size;
	/*并且删除节点*/
	prev->s.next = p->s.next;
	mem_ptr = prev;
}

int main()
{

}