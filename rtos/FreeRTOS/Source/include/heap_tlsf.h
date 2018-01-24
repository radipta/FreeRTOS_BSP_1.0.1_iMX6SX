/*
 * heap_tlsf.h
 *
 *  Created on: Dec 4, 2016
 *      Author: alvin
 */

#ifndef HEAP_TLSF_H_
#define HEAP_TLSF_H_

//#include "deprecated_definitions.h"	//removing include deprecated definition
#ifndef portENTER_CRITICAL
	#include "portmacro.h"
#endif

#define TLSF_SL 3
#define SLI (1<<TLSF_SL)
#define TLSF_MAXSIZE 15
#define NONE TLSF_MAXSIZE+1
#define TLSF_Offset 4
#define Realfl TLSF_MAXSIZE-TLSF_Offset
#define Realsl (1<<SLI)
#define head_size	(sizeof (freeptr_t))
#define Overhead	(sizeof (struct_head))

typedef struct header_struct {
    struct header_struct *prev_hdr;
    size_t size;
    struct header_struct *prev;
    struct header_struct *next;
} struct_head;

typedef struct TLSF_struct {
    uint32_t fl_bitmap;
    uint32_t sl_bitmap[Realfl+2];
    struct_head *matrix[Realfl+2][SLI];
} struct_tlsf;


//static uint8_t ucHeap2[ configTOTAL_HEAP_SIZE ];
//static uint8_t ucHeap2[(size_t)45504];
static uint8_t ucHeap2[(size_t)8192];
//static uint8_t ucHeap2[5000];
#define TLSF_Heap (((portPOINTER_SIZE_TYPE)&ucHeap2[portBYTE_ALIGNMENT]) & (~((portPOINTER_SIZE_TYPE )portBYTE_ALIGNMENT_MASK )));

void TLSF_init();
inline __attribute__((always_inline)) void insert_block(struct_head *block,struct_tlsf *tlsf,uint32_t* fl,uint32_t* sl)PRIVILEGED_FUNCTION;
inline __attribute__((always_inline)) struct_head* find_suitable_block(struct_tlsf* tlsf,uint32_t* fl,uint32_t* sl)PRIVILEGED_FUNCTION;
inline __attribute__((always_inline)) void mapping_search(size_t r,uint32_t* fl,uint32_t* sl)PRIVILEGED_FUNCTION;
inline __attribute__((always_inline)) void mapping_insert(size_t r,uint32_t* fl,uint32_t* sl)PRIVILEGED_FUNCTION;
void* Malloc_TLSF(size_t size,uint32_t fl,uint32_t sl) PRIVILEGED_FUNCTION;
int Free_TLSF(void* point) PRIVILEGED_FUNCTION;
inline __attribute__((always_inline)) void unlink_block(struct_head* block,struct_tlsf* tlsf,uint32_t *fl,uint32_t *sl)PRIVILEGED_FUNCTION;
inline __attribute__((always_inline)) int most_bit(int i)PRIVILEGED_FUNCTION;
inline __attribute__((always_inline)) int least_bit(int i)PRIVILEGED_FUNCTION;



#endif /* HEAP_TLSF_H_ */
