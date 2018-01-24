#include <stdlib.h>
//#include <diag/Trace.h>
#define MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#include "FreeRTOS.h"
#include "task.h"
#include "heap_tlsf.h"
#include "debug_console_imx.h"

//#include "heap_tlsf.h"
static uint8_t *Heap_Begin;

//volatile int *DWT_CYCCNT  ;
//volatile int *DWT_CONTROL ;
//volatile int *SCB_DEMCR   ;

void TLSF_init()
{

	//PRINTF("[ TLSF ]------init pool--------\n");
	PRINTF("[ TLSF ]------init pool--------\n");
	int i=0,j=0,res=45504;
	uint32_t fl=0,sl=0;
    struct_tlsf *tlsf;
    struct_head *block,*lastblock;
    size_t size;

	Heap_Begin=(uint8_t*)TLSF_Heap;
	tlsf = (struct_tlsf *)  Heap_Begin;
	
	PRINTF("alamat awal %p\n",Heap_Begin);	
	
	PRINTF("alamat awal %p\n",tlsf);

	PRINTF("besar tlsf %d block header %d\n",sizeof(struct_tlsf),sizeof(struct_head));

	block=((struct_head *)((char *)(Heap_Begin)+(sizeof(struct_tlsf))));


	size=res-sizeof(struct_tlsf);
	block->size=(size-2*Overhead);//-mark-
	PRINTF("block:%d alamat%x %x next:%x \n",block->size,&block->size,&block->prev,&block->next);
	block->prev=NULL;
	block->next=NULL;

	lastblock=((struct_head *)((char *)(&block->next)+(block->size&~3)+4));
	PRINTF("last block %x\n",lastblock);

	lastblock->prev_hdr=block;
	lastblock->size=0x2;

	PRINTF("alamat %x besar %d\n",block,block->size);
	tlsf->fl_bitmap=0;
	for(i=0;i<Realfl;i++)
	{
		for(j=0;j<SLI;j++)
		{
			tlsf->matrix[i][j]=0;
		}
		tlsf->sl_bitmap[i]=0;
	}
	mapping_insert(block->size,&fl,&sl);
	insert_block(block,tlsf,&fl,&sl);
}


inline __attribute__((always_inline)) void insert_block(struct_head *block,struct_tlsf *tlsf,uint32_t* fl,uint32_t* sl)
{
	//PRINTF("[ TLSF ]-------insert block-------\n");
	block->prev=NULL;
	//PRINTF("alamat %x tlsf%x fl%d sl%d \n",block,tlsf,*fl,*sl);
	//PRINTF("block %x\n",tlsf->matrix[*fl][*sl]);

	block->next=tlsf->matrix[*fl][*sl];
	////PRINTF("alamat %x\n",block->free_ptr.next);
	if (tlsf->matrix [*fl][*sl])
	{
		//PRINTF("check insert\n");
	 	tlsf-> matrix [*fl][*sl] ->prev =block;
	}
	tlsf->matrix[*fl][*sl]=block;
	//PRINTF("block2 %x\n",tlsf->matrix[*fl][*sl]);
	//PRINTF("bitmap sl%d",tlsf->sl_bitmap[*fl]);
	tlsf->sl_bitmap[*fl]|=(1<<*sl);
	tlsf->fl_bitmap|=(1<<*fl);
	//PRINTF("bitmap %d temp %d\n",tlsf->sl_bitmap[*fl],tlsf->fl_bitmap);
}

inline __attribute__((always_inline)) struct_head* find_suitable_block(struct_tlsf* tlsf,uint32_t* fl,uint32_t* sl)
{
	//PRINTF("[ TLSF ]------find suitable--------\n");
	uint32_t temp;
	//PRINTF("pool %x\n",tlsf);
	//PRINTF("fl %d sl %d\n",*fl,*sl);
	temp=tlsf->sl_bitmap[*fl]& (~0 << *sl);
	//PRINTF("bitmap %d temp %d\n",tlsf->sl_bitmap[*fl],temp);
	struct_head *block=NULL;
	if(temp)
	{
		*sl=least_bit(temp);
		block=tlsf->matrix[*fl][*sl];
		//PRINTF("block %x\n",&tlsf->matrix[*fl][*sl]);
	}
	else
	{
		*fl=least_bit((tlsf->fl_bitmap&(~0<<(*fl+1))));
		//PRINTF("fl %d ",*fl);
		*sl=least_bit(tlsf->sl_bitmap[*fl]);
		//PRINTF("sl %d\n",*sl);
		block=tlsf->matrix[*fl][*sl];
		//PRINTF("block %x\n",block);
	}
	return block;
}

inline __attribute__((always_inline)) int most_bit(int i)
{
	int r=31;
	if(!i)
		return 0;
	if(!(i&0xffff0000))
	{
		i=i<<16;
		r-=16;
	}
	if(!(i&0xff000000))
	{
		i=i<<8;
		r-=8;
	}
	if(!(i&0xf0000000))
	{
		i=i<<4;
		r-=4;
	}
	if(!(i&0xc0000000))
	{
		i=(i<<2);
		r-=2;
	}
	if(!(i&0x80000000))
	{
		i=i<<1;
		r-=1;
	}
	return r;
}
inline __attribute__((always_inline)) int least_bit(int i)
{
	int r=0;
	if(!i)
		return 0;
	if(!(i&0xffff))
	{
		i>>=16;
		r+=16;
	}
	if(!(i&0xff))
	{
		i>>=8;
		r+=8;
	}
	if(!(i&0xf))
	{
		i>>=4;
		r+=4;
	}
	if(!(i&3))
	{
		i>>=2;
		r+=2;
	}
	if(!(i&1))
	{
		i>>=1;
		r+=1;
	}
	return r;
}

inline __attribute__((always_inline)) void mapping_search(size_t r,uint32_t* fl,uint32_t* sl)
{

	r=r+(1<<(most_bit(r)-TLSF_SL))-1;
	*fl=most_bit(r);
	*sl=(r>>(*fl-TLSF_SL))-(SLI);
	//*sl=(r-(1<<*fl))/(1<<(*fl-TLSF_SL));
	*fl-=TLSF_Offset;
	//PRINTF("search sl:%u fl:%u\n",*sl,*fl);
}

inline __attribute__((always_inline)) void mapping_insert(size_t r,uint32_t* fl,uint32_t* sl)
{
	*fl=most_bit(r);
	*sl=(r>>(*fl-TLSF_SL))-(SLI);
	//*sl=(r-(1<<*fl))/(1<<(*fl-TLSF_SL));
	*fl-=TLSF_Offset;
//	PRINTF("insert sl:%u fl:%u\n",*sl,*fl);
}

inline __attribute__((always_inline)) void unlink_block(struct_head* block,struct_tlsf* tlsf,uint32_t *fl,uint32_t *sl)
{
	////PRINTF("------unlink block--------\n");
	tlsf->matrix[*fl][*sl]=block->next;
	//PRINTF("matrix %d\n",tlsf->matrix[*fl][*sl]);
	if(tlsf->matrix[*fl][*sl])
	{
		//PRINTF("check unlink\n");
		tlsf->matrix[*fl][*sl]->prev=NULL;
	}
	else
	{
		tlsf->sl_bitmap[*fl]=tlsf->sl_bitmap[*fl]&(~(1<<*sl));
		if(tlsf->sl_bitmap[*fl]==0)
		{
			tlsf->fl_bitmap&=(~(1<<*fl));
		}
		//PRINTF("bitmap fl %d sl %d\n",tlsf->fl_bitmap,tlsf->sl_bitmap[*fl]);
	}
	block->prev=NULL;
	block->next=NULL;
}

void* Malloc_TLSF(size_t size,uint32_t fl,uint32_t sl)
{
	//PRINTF("------malloc start--------\n");
	//uint32_t fl=0,sl=0;
	struct_tlsf *tlsf=(struct_tlsf*)Heap_Begin;
	struct_head *free_block,*last_block,*next_block;
	size_t temp;
	int tampung;
	//PRINTF("alamat awal %p\n",Heap_Begin);
	//PRINTF("alamat awal %p\n",tlsf);
	portDISABLE_INTERRUPTS();
	if(size!=0)
	{
		size =(size<16)?16:((size+3)&~3);
		//PRINTF("pool %x size %d besar void %d\n",tlsf,size,sizeof(void*));
		if(fl==NONE && sl==NONE)
		{
			mapping_search(size,&fl,&sl);
		}
		free_block=find_suitable_block(tlsf,&fl,&sl);
		//PRINTF("freeblock %x %x\n",&free_block,free_block);
		if(!free_block)
		{
			portENABLE_INTERRUPTS();
			return NULL;
		}

		unlink_block(free_block,tlsf,&fl,&sl);

		last_block=((struct_head *)((char *)(&free_block->next)+(free_block->size&~3)+4));

		//PRINTF("lastblock %x\n",last_block);
		temp=(free_block->size&~3)-size;
		//PRINTF("besar sisa %d\n",temp);
		if(temp>=sizeof(struct_head))
		{
			temp-=Overhead;
			//PRINTF("besar sisa 2 %d\n",temp);
			next_block=((struct_head *)((char *)(&free_block->next)+(size)+4));
			//PRINTF("alamat blok selanjutnya %x\n",next_block);
			next_block->size=(temp|1);
			//PRINTF("besar size block %d\n",next_block->size);
			last_block->prev_hdr=next_block;
			mapping_insert(temp,&fl,&sl);
			insert_block(next_block,tlsf,&fl,&sl);
			free_block->size=size|(free_block->size&2);
			//PRINTF("size block yang digunakan %d\n",free_block->size);
		}
		else
		{
			last_block->size&=(~2);
			free_block->size&=(~3);
		}
	}
	else
	{
		portENABLE_INTERRUPTS();
		return NULL;
	}
	portENABLE_INTERRUPTS();
	return ((void *)((char*) &free_block->next+4));
}

int Free_TLSF(void* point)
{
	uint32_t fl=0,sl=0;
	struct_tlsf *tlsf=(struct_tlsf*)Heap_Begin;
	struct_head *block,*next_block;
	if(!point)
		return 0;
	//PRINTF("alamat point free %x\n",point);
	//vTaskSuspendAll();
	//taskENTER_CRITICAL();
	//{
	portDISABLE_INTERRUPTS();
	block=(struct_head *) ((char *) point-Overhead);
	//PRINTF("alamat header %x\n",block);

	block->size|= 1;
	block->prev=NULL;
	block->next=NULL;

	next_block=(struct_head *) ((char *) &block->next+(block->size&~3)+4);
	//PRINTF("next block %x\n",next_block);
	if(next_block->size&1)
	{
		mapping_insert(next_block->size,&fl,&sl);
		unlink_block(next_block,tlsf,&fl,&sl);
		block->size+=((next_block->size&~3)+Overhead);
		//PRINTF("[ freeblock ] zize %d\n",block->size);
	}
	if(block->size&2)
	{
		next_block=block->prev_hdr;
		mapping_insert(next_block->size,&fl,&sl);
		unlink_block(next_block,tlsf,&fl,&sl);
		//PRINTF("besar block %d\n",next_block->size);
		next_block->size+=(block->size&~3)+Overhead;
		//PRINTF("[ Prevfree ] zize %d\n",next_block->size);
		block=next_block;
	}
	mapping_insert(block->size,&fl,&sl);
	insert_block(block,tlsf,&fl,&sl);
	next_block=(struct_head *) ((char *) &block->next+(block->size&~3)+4);
	//PRINTF("alamat next block %x\n",next_block);
	next_block->size|=2;
	//PRINTF("block free %d besar next block %d\n",block->size,next_block->size);
	next_block->prev_hdr=block;
	//}
	//taskEXIT_CRITICAL();
	//( void ) xTaskResumeAll();
	portENABLE_INTERRUPTS();
	return 1;
}
