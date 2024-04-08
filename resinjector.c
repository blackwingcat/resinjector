#include "stdafx.h"

/*
typedef struct tagVS_FIXEDFILEINFO {
  DWORD dwSignature;
  DWORD dwStrucVersion;
  DWORD dwFileVersionMS;
  DWORD dwFileVersionLS;
  DWORD dwProductVersionMS;
  DWORD dwProductVersionLS;
  DWORD dwFileFlagsMask;
  DWORD dwFileFlags;
  DWORD dwFileOS;
  DWORD dwFileType;
  DWORD dwFileSubtype;
  DWORD dwFileDateMS;
  DWORD dwFileDateLS;
} VS_FIXEDFILEINFO;
*/
typedef struct
{
    WORD wLength;
    WORD wValueLength;
    WORD wType;
    wchar_t szKey[15];
  	WORD Padding1;
    VS_FIXEDFILEINFO Value;
    WORD Padding2;
    WORD Children;
} VS_VERSIONINFOA;

int main(int argc, char* argv[])
{
	int i;
	int *updatePointer = 0;
	char AssemblyPath[640] = {0};
	VS_VERSIONINFOA vi = {0};
	lstrcpyW(vi.szKey,L"VS_VERSION_INFO");
	vi.Value.dwSignature = 0xfeef04bd;
	vi.Value.dwStrucVersion = 0x00010000;
	vi.wLength = sizeof(vi);
	vi.wValueLength = sizeof(vi.Value);
	vi.wType = 0;
	vi.Value.dwFileVersionMS =0x00010000;
	vi.Value.dwFileVersionLS =0x00000000;
	vi.Value.dwProductVersionMS =0x00010000;
	vi.Value.dwProductVersionLS =0x00000000;
 	vi.Value.dwFileFlagsMask= 0x3f;
/*	typedef enum _FileFlags : uint32_t { // FLAGS
			DEBUG        = 0x00000001,
			PRERELEASE   = 0x00000002,
			PATCHED      = 0x00000004,
			PRIVATEBUILD = 0x00000008,
			INFOINFERRED = 0x00000010,
			SPECIALBUILD = 0x00000020,
	} Flags; */
	vi.Value.dwFileFlags = 0;
 	//vi.Value.dwFileOS = 0x040004;// NT_WINDOWS32
 	vi.Value.dwFileOS = 0x000004;// WINDOWS32
	/* typedef enum _Type : uint32_t  {
			UNKNOWN_TYPE = 0x00000000,
			APP          = 0x00000001,
			DLL          = 0x00000002,
			DRV          = 0x00000003,
			FONT         = 0x00000004,
			VXD          = 0x00000005,
			STATIC_LIB   = 0x00000007,
		} Type;*/
	vi.Value.dwFileType = 1;
		/* typedef enum _SubType : uint32_t {
			UNKNOWN_SUB_TYPE      = 0x00000000,

			DRV_PRINTER           = 0x00000001,
			DRV_KEYBOARD          = 0x00000002,
			DRV_LANGUAGE          = 0x00000003,
			DRV_DISPLAY           = 0x00000004,
			DRV_MOUSE             = 0x00000005,
			DRV_NETWORK           = 0x00000006,
			DRV_SYSTEM            = 0x00000007,
			DRV_INSTALLABLE       = 0x00000008,
			DRV_SOUND             = 0x00000009,
			DRV_COMM              = 0x0000000A,
			DRV_VERSIONED_PRINTER = 0x0000000C,

			FONT_RASTER           = 0x00000001,
			FONT_VECTOR           = 0x00000002,
			FONT_TRUETYPE         = 0x00000003,
		} SubType;*/
		vi.Value.dwFileSubtype = 0;
	vi.Value.dwFileDateMS = 0;
	vi.Value.dwFileDateLS = 0;
	printf("resource inector v0.1\n");
	for(i=1;i<argc;i++){
		if(*argv[i]=='/' ||*argv[i]=='-'){
			//option
		} else {
			lstrcpyA(AssemblyPath,argv[i]);
		}
	}
	if(*AssemblyPath == 0) {
		printf("no input file\n");
		return 0;
	}
	updatePointer = (int *)BeginUpdateResource(AssemblyPath, false);
	BYTE *manifestByteArray = NULL;
	bool result = false;
	BYTE nullp [] = "";
	manifestByteArray = nullp;
	if (updatePointer == NULL) {
		printf("can not open exe\n");
		return 0;
	}
	try{
		int _langID = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US);
        if (UpdateResource(updatePointer, RT_VERSION, MAKEINTRESOURCE(VS_VERSION_INFO), _langID, &vi, vi.wLength) != 1)
//		if (UpdateResource(updatePointer, (const char *)24, (const char *)ResourceName, 0, manifestByteArray, mBALength != 1) )
		{
			printf("can not udate res\n");
			return 0;
		}
	} catch (...) {
		result =true;
	}
	if (updatePointer != 0) {
		if(1 != EndUpdateResource(updatePointer, result)){
			printf("err.\n");
		}
		else printf("end.\n");
	} else {
			printf("can not finalize res\n");	
	}
	return 0;
}
