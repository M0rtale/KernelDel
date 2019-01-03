#include <ntifs.h>
#include <wdmsec.h>

#define Pool_Tage 'BEEF'

#ifndef _AMD64_
#error only 64bit supported
#endif

static void *operator new(size_t lBlockSize)
{
	auto Address = ExAllocatePoolWithTag(POOL_TYPE(0), lBlockSize, Pool_Tage); // NonPagedPool

	if (Address)
	{
		RtlSecureZeroMemory(Address, lBlockSize);
	}
	
	return Address;
}

static void operator delete(void *pointer)
{
	if (pointer)
	{
		ExFreePoolWithTag(pointer, Pool_Tage);
	}
}