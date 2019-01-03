#include "Free_Delete.h"

bool Free_Delete::Delete_Model(wchar_t *Path)
{
	HANDLE hFile;
	UNICODE_STRING uPath;
	OBJECT_ATTRIBUTES oaFileObject;
	IO_STATUS_BLOCK ioBlock;
	DEVICE_OBJECT *device = 0;
	
	PEPROCESS peProc = PsGetCurrentProcess();
	//R3 Context
	KeAttachProcess(peProc);

	RtlInitUnicodeString(&uPath, Path); // copy path to a unicode string

	InitializeObjectAttributes(&oaFileObject, &uPath, OBJ_CASE_INSENSITIVE, 0, 0);
	NTSTATUS result = IoCreateFileSpecifyDeviceObjectHint(
		&hFile,
		SYNCHRONIZE | FILE_WRITE_ATTRIBUTES | FILE_READ_ATTRIBUTES | FILE_READ_DATA,
		&oaFileObject,
		&ioBlock,
		0,
		0,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		FILE_OPEN,
		FILE_NON_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_ALERT,
		0,
		0,
		CreateFileTypeNone,
		0,
		IO_IGNORE_SHARE_ACCESS_CHECK,
		device
		);
	if (!NT_SUCCESS(result))
	{
		DbgPrintEx(0, 0, "\nIoCreateFileSpecifyDeviceObjectHint Failed...");
		ZwClose(hFile);
		KeDetachProcess();
		return false;
	}

	PVOID object = 0;
	result = ObReferenceObjectByHandle(hFile, 0, 0, 0, &object, 0);

	if (!NT_SUCCESS(result))
	{
		DbgPrintEx(0, 0, "\nObReferenceObjectByHandle Failed...");
		ZwClose(hFile);
		KeDetachProcess();
		return false;
	}

	//CopyPasta
	((FILE_OBJECT*)object)->SectionObjectPointer->ImageSectionObject = 0;
	((FILE_OBJECT*)object)->DeleteAccess = 1; // Remove all access, might give access denied for higher altitude
	result = ZwDeleteFile(&oaFileObject);

	ObDereferenceObject(object);
	ZwClose(hFile);

	if (!NT_SUCCESS(result))
	{
		DbgPrintEx(0, 0, "\nZwDeleteFile Failed...");
		ZwClose(hFile);
		KeDetachProcess();
		return false;
	}

	result = ZwDeleteFile(&oaFileObject);
	if (NT_SUCCESS(result))
	{
		KeDetachProcess();
		return true;
	}
}