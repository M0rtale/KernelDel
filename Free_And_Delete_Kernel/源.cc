#include "Free_Delete.h"

Free_Delete *_Free_Delete = nullptr;


void UnloadDriver(PDRIVER_OBJECT pDriverObject)
{
	DbgPrintEx(0, 0, "\nUnload called.");
	delete _Free_Delete;
	DbgPrintEx(0, 0, "\nUnload over.");
}

extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegistryPath)
{
	DbgPrintEx(0, 0, "\nDriver Loaded");
	
	pDriverObject->DriverUnload = UnloadDriver;

	_Free_Delete = new Free_Delete();
	_Free_Delete->Delete_Model(L"\\??\\C:\\D.EXE");

	return STATUS_SUCCESS;

}