typedef unsigned char   undefined;

typedef unsigned int    ImageBaseOffset32;
typedef unsigned char    bool;
typedef unsigned char    byte;
typedef unsigned int    dword;
float10
typedef long long    longlong;
typedef unsigned char    uchar;
typedef unsigned int    uint;
typedef unsigned long    ulong;
typedef unsigned long long    ulonglong;
typedef unsigned char    undefined1;
typedef unsigned short    undefined2;
typedef unsigned int    undefined4;
typedef unsigned long long    undefined8;
typedef unsigned short    ushort;
typedef short    wchar_t;
typedef unsigned short    word;
typedef union IMAGE_RESOURCE_DIRECTORY_ENTRY_DirectoryUnion IMAGE_RESOURCE_DIRECTORY_ENTRY_DirectoryUnion, *PIMAGE_RESOURCE_DIRECTORY_ENTRY_DirectoryUnion;

typedef struct IMAGE_RESOURCE_DIRECTORY_ENTRY_DirectoryStruct IMAGE_RESOURCE_DIRECTORY_ENTRY_DirectoryStruct, *PIMAGE_RESOURCE_DIRECTORY_ENTRY_DirectoryStruct;

struct IMAGE_RESOURCE_DIRECTORY_ENTRY_DirectoryStruct {
    dword OffsetToDirectory;
    dword DataIsDirectory;
};

union IMAGE_RESOURCE_DIRECTORY_ENTRY_DirectoryUnion {
    dword OffsetToData;
    struct IMAGE_RESOURCE_DIRECTORY_ENTRY_DirectoryStruct IMAGE_RESOURCE_DIRECTORY_ENTRY_DirectoryStruct;
};

typedef struct _s__RTTIBaseClassDescriptor _s__RTTIBaseClassDescriptor, *P_s__RTTIBaseClassDescriptor;

typedef struct TypeDescriptor TypeDescriptor, *PTypeDescriptor;

typedef struct PMD PMD, *PPMD;

typedef struct _s__RTTIClassHierarchyDescriptor _s__RTTIClassHierarchyDescriptor, *P_s__RTTIClassHierarchyDescriptor;

typedef struct _s__RTTIClassHierarchyDescriptor RTTIClassHierarchyDescriptor;

typedef int ptrdiff_t;

typedef struct _s__RTTIBaseClassDescriptor RTTIBaseClassDescriptor;

struct TypeDescriptor {
    void * pVFTable;
    void * spare;
    char[0] name;
};

struct PMD {
    ptrdiff_t mdisp;
    ptrdiff_t pdisp;
    ptrdiff_t vdisp;
};

struct _s__RTTIBaseClassDescriptor {
    struct TypeDescriptor * pTypeDescriptor; // ref to TypeDescriptor (RTTI 0) for class
    dword numContainedBases; // count of extended classes in BaseClassArray (RTTI 2)
    struct PMD where; // member displacement structure
    dword attributes; // bit flags
    RTTIClassHierarchyDescriptor * pClassHierarchyDescriptor; // ref to ClassHierarchyDescriptor (RTTI 3) for class
};

struct _s__RTTIClassHierarchyDescriptor {
    dword signature;
    dword attributes; // bit flags
    dword numBaseClasses; // number of base classes (i.e. rtti1Count)
    RTTIBaseClassDescriptor * * pBaseClassArray; // ref to BaseClassArray (RTTI 2)
};

typedef struct _s_UnwindMapEntry _s_UnwindMapEntry, *P_s_UnwindMapEntry;

typedef int __ehstate_t;

struct _s_UnwindMapEntry {
    __ehstate_t toState;
    void (* action)(void);
};

typedef struct <lambda_dd1c267fe327b8eb9e23cfd134fb4af8> <lambda_dd1c267fe327b8eb9e23cfd134fb4af8>, *P<lambda_dd1c267fe327b8eb9e23cfd134fb4af8>;

struct <lambda_dd1c267fe327b8eb9e23cfd134fb4af8> { // PlaceHolder Class Structure
};

typedef unsigned short    wchar16;
typedef struct _s_UnwindMapEntry UnwindMapEntry;

typedef struct _s_ESTypeList _s_ESTypeList, *P_s_ESTypeList;

typedef struct _s_ESTypeList ESTypeList;

typedef struct _s_HandlerType _s_HandlerType, *P_s_HandlerType;

typedef struct _s_HandlerType HandlerType;

struct _s_HandlerType {
    uint adjectives;
    struct TypeDescriptor * pType;
    ptrdiff_t dispCatchObj;
    void * addressOfHandler;
};

struct _s_ESTypeList {
    int nCount;
    HandlerType * pTypeArray;
};

typedef struct __crt_seh_guarded_call<void> __crt_seh_guarded_call<void>, *P__crt_seh_guarded_call<void>;

struct __crt_seh_guarded_call<void> { // PlaceHolder Class Structure
};

typedef struct __crt_seh_guarded_call<void_(__cdecl*)(int)> __crt_seh_guarded_call<void_(__cdecl*)(int)>, *P__crt_seh_guarded_call<void_(__cdecl*)(int)>;

struct __crt_seh_guarded_call<void_(__cdecl*)(int)> { // PlaceHolder Class Structure
};

typedef struct _s__RTTICompleteObjectLocator _s__RTTICompleteObjectLocator, *P_s__RTTICompleteObjectLocator;

typedef struct _s__RTTICompleteObjectLocator RTTICompleteObjectLocator;

struct _s__RTTICompleteObjectLocator {
    dword signature;
    dword offset; // offset of vbtable within class
    dword cdOffset; // constructor displacement offset
    struct TypeDescriptor * pTypeDescriptor; // ref to TypeDescriptor (RTTI 0) for class
    RTTIClassHierarchyDescriptor * pClassDescriptor; // ref to ClassHierarchyDescriptor (RTTI 3)
};

typedef struct _s_TryBlockMapEntry _s_TryBlockMapEntry, *P_s_TryBlockMapEntry;

typedef struct _s_TryBlockMapEntry TryBlockMapEntry;

struct _s_TryBlockMapEntry {
    __ehstate_t tryLow;
    __ehstate_t tryHigh;
    __ehstate_t catchHigh;
    int nCatches;
    HandlerType * pHandlerArray;
};

typedef struct _s_FuncInfo _s_FuncInfo, *P_s_FuncInfo;

struct _s_FuncInfo {
    uint magicNumber_and_bbtFlags;
    __ehstate_t maxState;
    UnwindMapEntry * pUnwindMap;
    uint nTryBlocks;
    TryBlockMapEntry * pTryBlockMap;
    uint nIPMapEntries;
    void * pIPToStateMap;
    ESTypeList * pESTypeList;
    int EHFlags;
};

typedef ulonglong __uint64;

typedef struct __crt_seh_guarded_call<int> __crt_seh_guarded_call<int>, *P__crt_seh_guarded_call<int>;

struct __crt_seh_guarded_call<int> { // PlaceHolder Class Structure
};

typedef struct _s_FuncInfo FuncInfo;

typedef struct _cpinfo _cpinfo, *P_cpinfo;

typedef uint UINT;

typedef uchar BYTE;

struct _cpinfo {
    UINT MaxCharSize;
    BYTE DefaultChar[2];
    BYTE LeadByte[12];
};

typedef struct _cpinfo * LPCPINFO;

typedef struct _OVERLAPPED _OVERLAPPED, *P_OVERLAPPED;

typedef ulong ULONG_PTR;

typedef union _union_518 _union_518, *P_union_518;

typedef void * HANDLE;

typedef struct _struct_519 _struct_519, *P_struct_519;

typedef void * PVOID;

typedef ulong DWORD;

struct _struct_519 {
    DWORD Offset;
    DWORD OffsetHigh;
};

union _union_518 {
    struct _struct_519 s;
    PVOID Pointer;
};

struct _OVERLAPPED {
    ULONG_PTR Internal;
    ULONG_PTR InternalHigh;
    union _union_518 u;
    HANDLE hEvent;
};

typedef struct _WIN32_FIND_DATAW _WIN32_FIND_DATAW, *P_WIN32_FIND_DATAW;

typedef struct _WIN32_FIND_DATAW * LPWIN32_FIND_DATAW;

typedef struct _FILETIME _FILETIME, *P_FILETIME;

typedef struct _FILETIME FILETIME;

typedef wchar_t WCHAR;

struct _FILETIME {
    DWORD dwLowDateTime;
    DWORD dwHighDateTime;
};

struct _WIN32_FIND_DATAW {
    DWORD dwFileAttributes;
    FILETIME ftCreationTime;
    FILETIME ftLastAccessTime;
    FILETIME ftLastWriteTime;
    DWORD nFileSizeHigh;
    DWORD nFileSizeLow;
    DWORD dwReserved0;
    DWORD dwReserved1;
    WCHAR cFileName[260];
    WCHAR cAlternateFileName[14];
};

typedef struct _SECURITY_ATTRIBUTES _SECURITY_ATTRIBUTES, *P_SECURITY_ATTRIBUTES;

typedef struct _SECURITY_ATTRIBUTES * LPSECURITY_ATTRIBUTES;

typedef void * LPVOID;

typedef int BOOL;

struct _SECURITY_ATTRIBUTES {
    DWORD nLength;
    LPVOID lpSecurityDescriptor;
    BOOL bInheritHandle;
};

typedef enum _FINDEX_INFO_LEVELS {
    FindExInfoBasic=1,
    FindExInfoMaxInfoLevel=2,
    FindExInfoStandard=0
} _FINDEX_INFO_LEVELS;

typedef enum _FINDEX_INFO_LEVELS FINDEX_INFO_LEVELS;

typedef enum _GET_FILEEX_INFO_LEVELS {
    GetFileExInfoStandard=0,
    GetFileExMaxInfoLevel=1
} _GET_FILEEX_INFO_LEVELS;

typedef enum _GET_FILEEX_INFO_LEVELS GET_FILEEX_INFO_LEVELS;

typedef struct _PROCESS_INFORMATION _PROCESS_INFORMATION, *P_PROCESS_INFORMATION;

typedef struct _PROCESS_INFORMATION * LPPROCESS_INFORMATION;

struct _PROCESS_INFORMATION {
    HANDLE hProcess;
    HANDLE hThread;
    DWORD dwProcessId;
    DWORD dwThreadId;
};

typedef enum _FINDEX_SEARCH_OPS {
    FindExSearchLimitToDevices=2,
    FindExSearchLimitToDirectories=1,
    FindExSearchMaxSearchOp=3,
    FindExSearchNameMatch=0
} _FINDEX_SEARCH_OPS;

typedef long LONG;

typedef struct _EXCEPTION_POINTERS _EXCEPTION_POINTERS, *P_EXCEPTION_POINTERS;

typedef LONG (* PTOP_LEVEL_EXCEPTION_FILTER)(struct _EXCEPTION_POINTERS *);

typedef PTOP_LEVEL_EXCEPTION_FILTER LPTOP_LEVEL_EXCEPTION_FILTER;

typedef struct _EXCEPTION_RECORD _EXCEPTION_RECORD, *P_EXCEPTION_RECORD;

typedef struct _EXCEPTION_RECORD EXCEPTION_RECORD;

typedef EXCEPTION_RECORD * PEXCEPTION_RECORD;

typedef struct _CONTEXT _CONTEXT, *P_CONTEXT;

typedef struct _CONTEXT CONTEXT;

typedef CONTEXT * PCONTEXT;

typedef struct _FLOATING_SAVE_AREA _FLOATING_SAVE_AREA, *P_FLOATING_SAVE_AREA;

typedef struct _FLOATING_SAVE_AREA FLOATING_SAVE_AREA;

struct _FLOATING_SAVE_AREA {
    DWORD ControlWord;
    DWORD StatusWord;
    DWORD TagWord;
    DWORD ErrorOffset;
    DWORD ErrorSelector;
    DWORD DataOffset;
    DWORD DataSelector;
    BYTE RegisterArea[80];
    DWORD Cr0NpxState;
};

struct _CONTEXT {
    DWORD ContextFlags;
    DWORD Dr0;
    DWORD Dr1;
    DWORD Dr2;
    DWORD Dr3;
    DWORD Dr6;
    DWORD Dr7;
    FLOATING_SAVE_AREA FloatSave;
    DWORD SegGs;
    DWORD SegFs;
    DWORD SegEs;
    DWORD SegDs;
    DWORD Edi;
    DWORD Esi;
    DWORD Ebx;
    DWORD Edx;
    DWORD Ecx;
    DWORD Eax;
    DWORD Ebp;
    DWORD Eip;
    DWORD SegCs;
    DWORD EFlags;
    DWORD Esp;
    DWORD SegSs;
    BYTE ExtendedRegisters[512];
};

struct _EXCEPTION_RECORD {
    DWORD ExceptionCode;
    DWORD ExceptionFlags;
    struct _EXCEPTION_RECORD * ExceptionRecord;
    PVOID ExceptionAddress;
    DWORD NumberParameters;
    ULONG_PTR ExceptionInformation[15];
};

struct _EXCEPTION_POINTERS {
    PEXCEPTION_RECORD ExceptionRecord;
    PCONTEXT ContextRecord;
};

typedef struct _STARTUPINFOW _STARTUPINFOW, *P_STARTUPINFOW;

typedef WCHAR * LPWSTR;

typedef ushort WORD;

typedef BYTE * LPBYTE;

struct _STARTUPINFOW {
    DWORD cb;
    LPWSTR lpReserved;
    LPWSTR lpDesktop;
    LPWSTR lpTitle;
    DWORD dwX;
    DWORD dwY;
    DWORD dwXSize;
    DWORD dwYSize;
    DWORD dwXCountChars;
    DWORD dwYCountChars;
    DWORD dwFillAttribute;
    DWORD dwFlags;
    WORD wShowWindow;
    WORD cbReserved2;
    LPBYTE lpReserved2;
    HANDLE hStdInput;
    HANDLE hStdOutput;
    HANDLE hStdError;
};

typedef struct _STARTUPINFOW * LPSTARTUPINFOW;

typedef struct _OVERLAPPED * LPOVERLAPPED;

typedef enum _FINDEX_SEARCH_OPS FINDEX_SEARCH_OPS;

typedef struct _RTL_CRITICAL_SECTION _RTL_CRITICAL_SECTION, *P_RTL_CRITICAL_SECTION;

typedef struct _RTL_CRITICAL_SECTION * PRTL_CRITICAL_SECTION;

typedef PRTL_CRITICAL_SECTION LPCRITICAL_SECTION;

typedef struct _RTL_CRITICAL_SECTION_DEBUG _RTL_CRITICAL_SECTION_DEBUG, *P_RTL_CRITICAL_SECTION_DEBUG;

typedef struct _RTL_CRITICAL_SECTION_DEBUG * PRTL_CRITICAL_SECTION_DEBUG;

typedef struct _LIST_ENTRY _LIST_ENTRY, *P_LIST_ENTRY;

typedef struct _LIST_ENTRY LIST_ENTRY;

struct _RTL_CRITICAL_SECTION {
    PRTL_CRITICAL_SECTION_DEBUG DebugInfo;
    LONG LockCount;
    LONG RecursionCount;
    HANDLE OwningThread;
    HANDLE LockSemaphore;
    ULONG_PTR SpinCount;
};

struct _LIST_ENTRY {
    struct _LIST_ENTRY * Flink;
    struct _LIST_ENTRY * Blink;
};

struct _RTL_CRITICAL_SECTION_DEBUG {
    WORD Type;
    WORD CreatorBackTraceIndex;
    struct _RTL_CRITICAL_SECTION * CriticalSection;
    LIST_ENTRY ProcessLocksList;
    DWORD EntryCount;
    DWORD ContentionCount;
    DWORD Flags;
    WORD CreatorBackTraceIndexHigh;
    WORD SpareWORD;
};

typedef struct formatting_buffer formatting_buffer, *Pformatting_buffer;

struct formatting_buffer { // PlaceHolder Class Structure
};

typedef struct common_data<char> common_data<char>, *Pcommon_data<char>;

struct common_data<char> { // PlaceHolder Class Structure
};

typedef struct _iobuf _iobuf, *P_iobuf;

typedef struct _iobuf FILE;

struct _iobuf {
    char * _ptr;
    int _cnt;
    char * _base;
    int _flag;
    int _file;
    int _charbuf;
    int _bufsiz;
    char * _tmpfname;
};

typedef struct lconv lconv, *Plconv;

struct lconv {
    char * decimal_point;
    char * thousands_sep;
    char * grouping;
    char * int_curr_symbol;
    char * currency_symbol;
    char * mon_decimal_point;
    char * mon_thousands_sep;
    char * mon_grouping;
    char * positive_sign;
    char * negative_sign;
    char int_frac_digits;
    char frac_digits;
    char p_cs_precedes;
    char p_sep_by_space;
    char n_cs_precedes;
    char n_sep_by_space;
    char p_sign_posn;
    char n_sign_posn;
    wchar_t * _W_decimal_point;
    wchar_t * _W_thousands_sep;
    wchar_t * _W_int_curr_symbol;
    wchar_t * _W_currency_symbol;
    wchar_t * _W_mon_decimal_point;
    wchar_t * _W_mon_thousands_sep;
    wchar_t * _W_positive_sign;
    wchar_t * _W_negative_sign;
};

typedef ushort wint_t;

typedef struct localerefcount localerefcount, *Plocalerefcount;

typedef struct localerefcount locrefcount;

struct localerefcount {
    char * locale;
    wchar_t * wlocale;
    int * refcount;
    int * wrefcount;
};

typedef uint size_t;

typedef int errno_t;

typedef struct localeinfo_struct localeinfo_struct, *Plocaleinfo_struct;

typedef struct localeinfo_struct * _locale_t;

typedef struct threadlocaleinfostruct threadlocaleinfostruct, *Pthreadlocaleinfostruct;

typedef struct threadlocaleinfostruct * pthreadlocinfo;

typedef struct threadmbcinfostruct threadmbcinfostruct, *Pthreadmbcinfostruct;

typedef struct threadmbcinfostruct * pthreadmbcinfo;

typedef struct __lc_time_data __lc_time_data, *P__lc_time_data;

struct threadlocaleinfostruct {
    int refcount;
    uint lc_codepage;
    uint lc_collate_cp;
    uint lc_time_cp;
    locrefcount lc_category[6];
    int lc_clike;
    int mb_cur_max;
    int * lconv_intl_refcount;
    int * lconv_num_refcount;
    int * lconv_mon_refcount;
    struct lconv * lconv;
    int * ctype1_refcount;
    ushort * ctype1;
    ushort * pctype;
    uchar * pclmap;
    uchar * pcumap;
    struct __lc_time_data * lc_time_curr;
    wchar_t * locale_name[6];
};

struct threadmbcinfostruct {
    int refcount;
    int mbcodepage;
    int ismbcodepage;
    ushort mbulinfo[6];
    uchar mbctype[257];
    uchar mbcasemap[256];
    wchar_t * mblocalename;
};

struct localeinfo_struct {
    pthreadlocinfo locinfo;
    pthreadmbcinfo mbcinfo;
};

struct __lc_time_data {
    char * wday_abbr[7];
    char * wday[7];
    char * month_abbr[12];
    char * month[12];
    char * ampm[2];
    char * ww_sdatefmt;
    char * ww_ldatefmt;
    char * ww_timefmt;
    int ww_caltype;
    int refcount;
    wchar_t * _W_wday_abbr[7];
    wchar_t * _W_wday[7];
    wchar_t * _W_month_abbr[12];
    wchar_t * _W_month[12];
    wchar_t * _W_ampm[2];
    wchar_t * _W_ww_sdatefmt;
    wchar_t * _W_ww_ldatefmt;
    wchar_t * _W_ww_timefmt;
    wchar_t * _W_ww_locale_name;
};

typedef int intptr_t;

typedef size_t rsize_t;

typedef struct exception exception, *Pexception;

struct exception { // PlaceHolder Class Structure
};

typedef struct _IMAGE_SECTION_HEADER _IMAGE_SECTION_HEADER, *P_IMAGE_SECTION_HEADER;

typedef union _union_226 _union_226, *P_union_226;

union _union_226 {
    DWORD PhysicalAddress;
    DWORD VirtualSize;
};

struct _IMAGE_SECTION_HEADER {
    BYTE Name[8];
    union _union_226 Misc;
    DWORD VirtualAddress;
    DWORD SizeOfRawData;
    DWORD PointerToRawData;
    DWORD PointerToRelocations;
    DWORD PointerToLinenumbers;
    WORD NumberOfRelocations;
    WORD NumberOfLinenumbers;
    DWORD Characteristics;
};

typedef WCHAR * PCNZWCH;

typedef union _SLIST_HEADER _SLIST_HEADER, *P_SLIST_HEADER;

typedef double ULONGLONG;

typedef struct _struct_299 _struct_299, *P_struct_299;

typedef struct _SINGLE_LIST_ENTRY _SINGLE_LIST_ENTRY, *P_SINGLE_LIST_ENTRY;

typedef struct _SINGLE_LIST_ENTRY SINGLE_LIST_ENTRY;

struct _SINGLE_LIST_ENTRY {
    struct _SINGLE_LIST_ENTRY * Next;
};

struct _struct_299 {
    SINGLE_LIST_ENTRY Next;
    WORD Depth;
    WORD Sequence;
};

union _SLIST_HEADER {
    ULONGLONG Alignment;
    struct _struct_299 s;
};

typedef char CHAR;

typedef CHAR * LPCSTR;

typedef CHAR * LPSTR;

typedef union _LARGE_INTEGER _LARGE_INTEGER, *P_LARGE_INTEGER;

typedef struct _struct_19 _struct_19, *P_struct_19;

typedef struct _struct_20 _struct_20, *P_struct_20;

typedef double LONGLONG;

struct _struct_20 {
    DWORD LowPart;
    LONG HighPart;
};

struct _struct_19 {
    DWORD LowPart;
    LONG HighPart;
};

union _LARGE_INTEGER {
    struct _struct_19 s;
    struct _struct_20 u;
    LONGLONG QuadPart;
};

typedef union _LARGE_INTEGER LARGE_INTEGER;

typedef struct _IMAGE_SECTION_HEADER * PIMAGE_SECTION_HEADER;

typedef WCHAR * LPWCH;

typedef WCHAR * LPCWSTR;

typedef union _SLIST_HEADER * PSLIST_HEADER;

typedef LARGE_INTEGER * PLARGE_INTEGER;

typedef DWORD LCID;

typedef struct IMAGE_DOS_HEADER IMAGE_DOS_HEADER, *PIMAGE_DOS_HEADER;

struct IMAGE_DOS_HEADER {
    char e_magic[2]; // Magic number
    word e_cblp; // Bytes of last page
    word e_cp; // Pages in file
    word e_crlc; // Relocations
    word e_cparhdr; // Size of header in paragraphs
    word e_minalloc; // Minimum extra paragraphs needed
    word e_maxalloc; // Maximum extra paragraphs needed
    word e_ss; // Initial (relative) SS value
    word e_sp; // Initial SP value
    word e_csum; // Checksum
    word e_ip; // Initial IP value
    word e_cs; // Initial (relative) CS value
    word e_lfarlc; // File address of relocation table
    word e_ovno; // Overlay number
    word e_res[4][4]; // Reserved words
    word e_oemid; // OEM identifier (for e_oeminfo)
    word e_oeminfo; // OEM information; e_oemid specific
    word e_res2[10][10]; // Reserved words
    dword e_lfanew; // File address of new exe header
    byte e_program[64]; // Actual DOS program
};

typedef ULONG_PTR HCRYPTKEY;

typedef uint ALG_ID;

typedef ULONG_PTR HCRYPTPROV;

typedef ULONG_PTR HCRYPTHASH;

typedef ULONG_PTR DWORD_PTR;

typedef ULONG_PTR SIZE_T;

typedef DWORD * LPDWORD;

typedef struct HINSTANCE__ HINSTANCE__, *PHINSTANCE__;

typedef struct HINSTANCE__ * HINSTANCE;

struct HINSTANCE__ {
    int unused;
};

typedef HINSTANCE HMODULE;

typedef struct _FILETIME * LPFILETIME;

typedef int (* FARPROC)(void);

typedef WORD * LPWORD;

typedef BOOL * LPBOOL;

typedef BYTE * PBYTE;

typedef void * LPCVOID;

typedef struct IMAGE_OPTIONAL_HEADER32 IMAGE_OPTIONAL_HEADER32, *PIMAGE_OPTIONAL_HEADER32;

typedef struct IMAGE_DATA_DIRECTORY IMAGE_DATA_DIRECTORY, *PIMAGE_DATA_DIRECTORY;

struct IMAGE_DATA_DIRECTORY {
    ImageBaseOffset32 VirtualAddress;
    dword Size;
};

struct IMAGE_OPTIONAL_HEADER32 {
    word Magic;
    byte MajorLinkerVersion;
    byte MinorLinkerVersion;
    dword SizeOfCode;
    dword SizeOfInitializedData;
    dword SizeOfUninitializedData;
    ImageBaseOffset32 AddressOfEntryPoint;
    ImageBaseOffset32 BaseOfCode;
    ImageBaseOffset32 BaseOfData;
    pointer32 ImageBase;
    dword SectionAlignment;
    dword FileAlignment;
    word MajorOperatingSystemVersion;
    word MinorOperatingSystemVersion;
    word MajorImageVersion;
    word MinorImageVersion;
    word MajorSubsystemVersion;
    word MinorSubsystemVersion;
    dword Win32VersionValue;
    dword SizeOfImage;
    dword SizeOfHeaders;
    dword CheckSum;
    word Subsystem;
    word DllCharacteristics;
    dword SizeOfStackReserve;
    dword SizeOfStackCommit;
    dword SizeOfHeapReserve;
    dword SizeOfHeapCommit;
    dword LoaderFlags;
    dword NumberOfRvaAndSizes;
    struct IMAGE_DATA_DIRECTORY DataDirectory[16];
};

typedef struct IMAGE_RESOURCE_DIRECTORY_ENTRY_NameStruct IMAGE_RESOURCE_DIRECTORY_ENTRY_NameStruct, *PIMAGE_RESOURCE_DIRECTORY_ENTRY_NameStruct;

struct IMAGE_RESOURCE_DIRECTORY_ENTRY_NameStruct {
    dword NameOffset;
    dword NameIsString;
};

typedef struct IMAGE_LOAD_CONFIG_CODE_INTEGRITY IMAGE_LOAD_CONFIG_CODE_INTEGRITY, *PIMAGE_LOAD_CONFIG_CODE_INTEGRITY;

struct IMAGE_LOAD_CONFIG_CODE_INTEGRITY {
    word Flags;
    word Catalog;
    dword CatalogOffset;
    dword Reserved;
};

typedef struct IMAGE_DEBUG_DIRECTORY IMAGE_DEBUG_DIRECTORY, *PIMAGE_DEBUG_DIRECTORY;

struct IMAGE_DEBUG_DIRECTORY {
    dword Characteristics;
    dword TimeDateStamp;
    word MajorVersion;
    word MinorVersion;
    dword Type;
    dword SizeOfData;
    dword AddressOfRawData;
    dword PointerToRawData;
};

typedef struct IMAGE_FILE_HEADER IMAGE_FILE_HEADER, *PIMAGE_FILE_HEADER;

struct IMAGE_FILE_HEADER {
    word Machine; // 332
    word NumberOfSections;
    dword TimeDateStamp;
    dword PointerToSymbolTable;
    dword NumberOfSymbols;
    word SizeOfOptionalHeader;
    word Characteristics;
};

typedef struct IMAGE_NT_HEADERS32 IMAGE_NT_HEADERS32, *PIMAGE_NT_HEADERS32;

struct IMAGE_NT_HEADERS32 {
    char Signature[4];
    struct IMAGE_FILE_HEADER FileHeader;
    struct IMAGE_OPTIONAL_HEADER32 OptionalHeader;
};

typedef union IMAGE_RESOURCE_DIRECTORY_ENTRY IMAGE_RESOURCE_DIRECTORY_ENTRY, *PIMAGE_RESOURCE_DIRECTORY_ENTRY;

typedef union IMAGE_RESOURCE_DIRECTORY_ENTRY_NameUnion IMAGE_RESOURCE_DIRECTORY_ENTRY_NameUnion, *PIMAGE_RESOURCE_DIRECTORY_ENTRY_NameUnion;

union IMAGE_RESOURCE_DIRECTORY_ENTRY_NameUnion {
    struct IMAGE_RESOURCE_DIRECTORY_ENTRY_NameStruct IMAGE_RESOURCE_DIRECTORY_ENTRY_NameStruct;
    dword Name;
    word Id;
};

union IMAGE_RESOURCE_DIRECTORY_ENTRY {
    union IMAGE_RESOURCE_DIRECTORY_ENTRY_NameUnion NameUnion;
    union IMAGE_RESOURCE_DIRECTORY_ENTRY_DirectoryUnion DirectoryUnion;
};

typedef struct IMAGE_SECTION_HEADER IMAGE_SECTION_HEADER, *PIMAGE_SECTION_HEADER;

typedef union Misc Misc, *PMisc;

typedef enum SectionFlags {
    IMAGE_SCN_ALIGN_1024BYTES=11534336,
    IMAGE_SCN_ALIGN_128BYTES=8388608,
    IMAGE_SCN_ALIGN_16BYTES=5242880,
    IMAGE_SCN_ALIGN_1BYTES=1048576,
    IMAGE_SCN_ALIGN_2048BYTES=12582912,
    IMAGE_SCN_ALIGN_256BYTES=9437184,
    IMAGE_SCN_ALIGN_2BYTES=2097152,
    IMAGE_SCN_ALIGN_32BYTES=6291456,
    IMAGE_SCN_ALIGN_4096BYTES=13631488,
    IMAGE_SCN_ALIGN_4BYTES=3145728,
    IMAGE_SCN_ALIGN_512BYTES=10485760,
    IMAGE_SCN_ALIGN_64BYTES=7340032,
    IMAGE_SCN_ALIGN_8192BYTES=14680064,
    IMAGE_SCN_ALIGN_8BYTES=4194304,
    IMAGE_SCN_CNT_CODE=32,
    IMAGE_SCN_CNT_INITIALIZED_DATA=64,
    IMAGE_SCN_CNT_UNINITIALIZED_DATA=128,
    IMAGE_SCN_GPREL=32768,
    IMAGE_SCN_LNK_COMDAT=4096,
    IMAGE_SCN_LNK_INFO=512,
    IMAGE_SCN_LNK_NRELOC_OVFL=16777216,
    IMAGE_SCN_LNK_OTHER=256,
    IMAGE_SCN_LNK_REMOVE=2048,
    IMAGE_SCN_MEM_16BIT=131072,
    IMAGE_SCN_MEM_DISCARDABLE=33554432,
    IMAGE_SCN_MEM_EXECUTE=536870912,
    IMAGE_SCN_MEM_LOCKED=262144,
    IMAGE_SCN_MEM_NOT_CACHED=67108864,
    IMAGE_SCN_MEM_NOT_PAGED=134217728,
    IMAGE_SCN_MEM_PRELOAD=524288,
    IMAGE_SCN_MEM_PURGEABLE=131072,
    IMAGE_SCN_MEM_READ=1073741824,
    IMAGE_SCN_MEM_SHARED=268435456,
    IMAGE_SCN_MEM_WRITE=2147483648,
    IMAGE_SCN_RESERVED_0001=16,
    IMAGE_SCN_RESERVED_0040=1024,
    IMAGE_SCN_TYPE_NO_PAD=8
} SectionFlags;

union Misc {
    dword PhysicalAddress;
    dword VirtualSize;
};

struct IMAGE_SECTION_HEADER {
    char Name[8];
    union Misc Misc;
    ImageBaseOffset32 VirtualAddress;
    dword SizeOfRawData;
    dword PointerToRawData;
    dword PointerToRelocations;
    dword PointerToLinenumbers;
    word NumberOfRelocations;
    word NumberOfLinenumbers;
    enum SectionFlags Characteristics;
};

typedef struct IMAGE_RESOURCE_DATA_ENTRY IMAGE_RESOURCE_DATA_ENTRY, *PIMAGE_RESOURCE_DATA_ENTRY;

struct IMAGE_RESOURCE_DATA_ENTRY {
    dword OffsetToData;
    dword Size;
    dword CodePage;
    dword Reserved;
};

typedef enum IMAGE_GUARD_FLAGS {
    IMAGE_GUARD_CFW_INSTRUMENTED=512,
    IMAGE_GUARD_CF_ENABLE_EXPORT_SUPPRESSION=32768,
    IMAGE_GUARD_CF_EXPORT_SUPPRESSION_INFO_PRESENT=16384,
    IMAGE_GUARD_CF_FUNCTION_TABLE_PRESENT=1024,
    IMAGE_GUARD_CF_FUNCTION_TABLE_SIZE_MASK_1=268435456,
    IMAGE_GUARD_CF_FUNCTION_TABLE_SIZE_MASK_2=536870912,
    IMAGE_GUARD_CF_FUNCTION_TABLE_SIZE_MASK_4=1073741824,
    IMAGE_GUARD_CF_FUNCTION_TABLE_SIZE_MASK_8=2147483648,
    IMAGE_GUARD_CF_INSTRUMENTED=256,
    IMAGE_GUARD_CF_LONGJUMP_TABLE_PRESENT=65536,
    IMAGE_GUARD_DELAYLOAD_IAT_IN_ITS_OWN_SECTION=8192,
    IMAGE_GUARD_PROTECT_DELAYLOAD_IAT=4096,
    IMAGE_GUARD_RF_ENABLE=262144,
    IMAGE_GUARD_RF_INSTRUMENTED=131072,
    IMAGE_GUARD_RF_STRICT=524288,
    IMAGE_GUARD_SECURITY_COOKIE_UNUSED=2048
} IMAGE_GUARD_FLAGS;

typedef struct IMAGE_RESOURCE_DIRECTORY IMAGE_RESOURCE_DIRECTORY, *PIMAGE_RESOURCE_DIRECTORY;

struct IMAGE_RESOURCE_DIRECTORY {
    dword Characteristics;
    dword TimeDateStamp;
    word MajorVersion;
    word MinorVersion;
    word NumberOfNamedEntries;
    word NumberOfIdEntries;
};

typedef struct IMAGE_LOAD_CONFIG_DIRECTORY32 IMAGE_LOAD_CONFIG_DIRECTORY32, *PIMAGE_LOAD_CONFIG_DIRECTORY32;

struct IMAGE_LOAD_CONFIG_DIRECTORY32 {
    dword Size;
    dword TimeDateStamp;
    word MajorVersion;
    word MinorVersion;
    dword GlobalFlagsClear;
    dword GlobalFlagsSet;
    dword CriticalSectionDefaultTimeout;
    dword DeCommitFreeBlockThreshold;
    dword DeCommitTotalFreeThreshold;
    pointer32 LockPrefixTable;
    dword MaximumAllocationSize;
    dword VirtualMemoryThreshold;
    dword ProcessHeapFlags;
    dword ProcessAffinityMask;
    word CsdVersion;
    word DependentLoadFlags;
    pointer32 EditList;
    pointer32 SecurityCookie;
    pointer32 SEHandlerTable;
    dword SEHandlerCount;
    pointer32 GuardCFCCheckFunctionPointer;
    pointer32 GuardCFDispatchFunctionPointer;
    pointer32 GuardCFFunctionTable;
    dword GuardCFFunctionCount;
    enum IMAGE_GUARD_FLAGS GuardFlags;
    struct IMAGE_LOAD_CONFIG_CODE_INTEGRITY CodeIntegrity;
    pointer32 GuardAddressTakenIatEntryTable;
    dword GuardAddressTakenIatEntryCount;
    pointer32 GuardLongJumpTargetTable;
    dword GuardLongJumpTargetCount;
    pointer32 DynamicValueRelocTable;
    pointer32 CHPEMetadataPointer;
    pointer32 GuardRFFailureRoutine;
    pointer32 GuardRFFailureRoutineFunctionPointer;
    dword DynamicValueRelocTableOffset;
    word DynamicValueRelocTableSection;
    word Reserved1;
    pointer32 GuardRFVerifyStackPointerFunctionPointer;
    dword HotPatchTableOffset;
    dword Reserved2;
    dword Reserved3;
};

typedef struct TranslatorGuardRN TranslatorGuardRN, *PTranslatorGuardRN;

struct TranslatorGuardRN { // PlaceHolder Structure
};

typedef struct EHExceptionRecord EHExceptionRecord, *PEHExceptionRecord;

struct EHExceptionRecord { // PlaceHolder Structure
};

typedef struct __crt_signal_action_t __crt_signal_action_t, *P__crt_signal_action_t;

struct __crt_signal_action_t { // PlaceHolder Structure
};

typedef struct <lambda_2fe9b910cf3cbf4a0ab98a02ba45b3ec> <lambda_2fe9b910cf3cbf4a0ab98a02ba45b3ec>, *P<lambda_2fe9b910cf3cbf4a0ab98a02ba45b3ec>;

struct <lambda_2fe9b910cf3cbf4a0ab98a02ba45b3ec> { // PlaceHolder Structure
};

typedef struct __crt_locale_data __crt_locale_data, *P__crt_locale_data;

struct __crt_locale_data { // PlaceHolder Structure
};

typedef struct <lambda_a048d3beccc847880fc8490e18b82769> <lambda_a048d3beccc847880fc8490e18b82769>, *P<lambda_a048d3beccc847880fc8490e18b82769>;

struct <lambda_a048d3beccc847880fc8490e18b82769> { // PlaceHolder Structure
};

typedef struct <lambda_9df27f884b057bc3edfc946cb5b7cf47> <lambda_9df27f884b057bc3edfc946cb5b7cf47>, *P<lambda_9df27f884b057bc3edfc946cb5b7cf47>;

struct <lambda_9df27f884b057bc3edfc946cb5b7cf47> { // PlaceHolder Structure
};

typedef struct <lambda_e69574bed617af4e071282c136b37893> <lambda_e69574bed617af4e071282c136b37893>, *P<lambda_e69574bed617af4e071282c136b37893>;

struct <lambda_e69574bed617af4e071282c136b37893> { // PlaceHolder Structure
};

typedef struct <lambda_be2b3da3f62db62e9dad5dc70221a656> <lambda_be2b3da3f62db62e9dad5dc70221a656>, *P<lambda_be2b3da3f62db62e9dad5dc70221a656>;

struct <lambda_be2b3da3f62db62e9dad5dc70221a656> { // PlaceHolder Structure
};

typedef struct <lambda_9cd88cf8ad10232537feb2133f08c833> <lambda_9cd88cf8ad10232537feb2133f08c833>, *P<lambda_9cd88cf8ad10232537feb2133f08c833>;

struct <lambda_9cd88cf8ad10232537feb2133f08c833> { // PlaceHolder Structure
};

typedef struct _exception _exception, *P_exception;

struct _exception { // PlaceHolder Structure
};

typedef struct <lambda_61cee617f5178ae960314fd4d05640a0> <lambda_61cee617f5178ae960314fd4d05640a0>, *P<lambda_61cee617f5178ae960314fd4d05640a0>;

struct <lambda_61cee617f5178ae960314fd4d05640a0> { // PlaceHolder Structure
};

typedef struct <lambda_51b6e8b1eb166f2a3faf91f424b38130> <lambda_51b6e8b1eb166f2a3faf91f424b38130>, *P<lambda_51b6e8b1eb166f2a3faf91f424b38130>;

struct <lambda_51b6e8b1eb166f2a3faf91f424b38130> { // PlaceHolder Structure
};

typedef struct <lambda_cc0d902bcbbeb830f749456577db4721> <lambda_cc0d902bcbbeb830f749456577db4721>, *P<lambda_cc0d902bcbbeb830f749456577db4721>;

struct <lambda_cc0d902bcbbeb830f749456577db4721> { // PlaceHolder Structure
};

typedef struct <lambda_5b71d36f03204c0beab531769a5b5694> <lambda_5b71d36f03204c0beab531769a5b5694>, *P<lambda_5b71d36f03204c0beab531769a5b5694>;

struct <lambda_5b71d36f03204c0beab531769a5b5694> { // PlaceHolder Structure
};

typedef struct <lambda_f7496a158712204296dd6628a163878e> <lambda_f7496a158712204296dd6628a163878e>, *P<lambda_f7496a158712204296dd6628a163878e>;

struct <lambda_f7496a158712204296dd6628a163878e> { // PlaceHolder Structure
};

typedef struct <lambda_3518db117f0e7cdb002338c5d3c47b6c> <lambda_3518db117f0e7cdb002338c5d3c47b6c>, *P<lambda_3518db117f0e7cdb002338c5d3c47b6c>;

struct <lambda_3518db117f0e7cdb002338c5d3c47b6c> { // PlaceHolder Structure
};

typedef struct <lambda_ae55bdf541ad94d75914d381c370e64d> <lambda_ae55bdf541ad94d75914d381c370e64d>, *P<lambda_ae55bdf541ad94d75914d381c370e64d>;

struct <lambda_ae55bdf541ad94d75914d381c370e64d> { // PlaceHolder Structure
};

typedef struct <lambda_abdedf541bb04549bc734292b4a045d4> <lambda_abdedf541bb04549bc734292b4a045d4>, *P<lambda_abdedf541bb04549bc734292b4a045d4>;

struct <lambda_abdedf541bb04549bc734292b4a045d4> { // PlaceHolder Structure
};

typedef struct __crt_stdio_stream __crt_stdio_stream, *P__crt_stdio_stream;

struct __crt_stdio_stream { // PlaceHolder Structure
};

typedef struct __crt_multibyte_data __crt_multibyte_data, *P__crt_multibyte_data;

struct __crt_multibyte_data { // PlaceHolder Structure
};

typedef struct <lambda_6978c1fb23f02e42e1d9e99668cc68aa> <lambda_6978c1fb23f02e42e1d9e99668cc68aa>, *P<lambda_6978c1fb23f02e42e1d9e99668cc68aa>;

struct <lambda_6978c1fb23f02e42e1d9e99668cc68aa> { // PlaceHolder Structure
};

typedef struct <lambda_8f9ce462984622f9bf76b59e2aaaf805> <lambda_8f9ce462984622f9bf76b59e2aaaf805>, *P<lambda_8f9ce462984622f9bf76b59e2aaaf805>;

struct <lambda_8f9ce462984622f9bf76b59e2aaaf805> { // PlaceHolder Structure
};


// WARNING! conflicting data type names: /Demangler/wchar_t - /wchar_t

typedef struct <lambda_6250bd4b2a391816dd638c3bf72b0bcb> <lambda_6250bd4b2a391816dd638c3bf72b0bcb>, *P<lambda_6250bd4b2a391816dd638c3bf72b0bcb>;

struct <lambda_6250bd4b2a391816dd638c3bf72b0bcb> { // PlaceHolder Structure
};

typedef struct __acrt_ptd __acrt_ptd, *P__acrt_ptd;

struct __acrt_ptd { // PlaceHolder Structure
};

typedef struct EHRegistrationNode EHRegistrationNode, *PEHRegistrationNode;

struct EHRegistrationNode { // PlaceHolder Structure
};

typedef enum _EXCEPTION_DISPOSITION {
} _EXCEPTION_DISPOSITION;

typedef struct __crt_locale_pointers __crt_locale_pointers, *P__crt_locale_pointers;

struct __crt_locale_pointers { // PlaceHolder Structure
};

typedef struct <lambda_ec61778202f4f5fc7e7711acc23c3bca> <lambda_ec61778202f4f5fc7e7711acc23c3bca>, *P<lambda_ec61778202f4f5fc7e7711acc23c3bca>;

struct <lambda_ec61778202f4f5fc7e7711acc23c3bca> { // PlaceHolder Structure
};

typedef struct <lambda_0b5a4a3e68152e1d9b943535f5f47bed> <lambda_0b5a4a3e68152e1d9b943535f5f47bed>, *P<lambda_0b5a4a3e68152e1d9b943535f5f47bed>;

struct <lambda_0b5a4a3e68152e1d9b943535f5f47bed> { // PlaceHolder Structure
};

typedef struct <lambda_b2ea41f6bbb362cd97d94c6828d90b61> <lambda_b2ea41f6bbb362cd97d94c6828d90b61>, *P<lambda_b2ea41f6bbb362cd97d94c6828d90b61>;

struct <lambda_b2ea41f6bbb362cd97d94c6828d90b61> { // PlaceHolder Structure
};

typedef struct <lambda_123407a5e2ac06da108355a851863b7a> <lambda_123407a5e2ac06da108355a851863b7a>, *P<lambda_123407a5e2ac06da108355a851863b7a>;

struct <lambda_123407a5e2ac06da108355a851863b7a> { // PlaceHolder Structure
};

typedef struct string_output_adapter<wchar_t> string_output_adapter<wchar_t>, *Pstring_output_adapter<wchar_t>;

struct string_output_adapter<wchar_t> { // PlaceHolder Structure
};

typedef int (* _onexit_t)(void);




undefined * FUN_00401000(void)

{
  return &DAT_0041f458;
}



void __cdecl print(char *param_1)

{
  undefined4 *puVar1;
  FILE *pFVar2;
  undefined4 uVar3;
  undefined *puVar4;
  
  pFVar2 = (FILE *)FUN_0040568c(1);
  puVar4 = &stack0x00000008;
  uVar3 = 0;
  puVar1 = (undefined4 *)FUN_00401000();
  FUN_00407c04(*puVar1,puVar1[1],pFVar2,(int)param_1,uVar3,puVar4);
  return;
}



// WARNING: Could not reconcile some variable overlaps

char * __fastcall do_encrypt(LPCWSTR file_name,size_t *output_size)

{
  BOOL BVar1;
  char *pcVar2;
  undefined8 *mac_addr;
  undefined4 *puVar3;
  undefined4 *disk_serial;
  DWORD input_file_size;
  size_t _Size;
  char *output_buf;
  int iVar4;
  HANDLE *input_file_handle;
  DWORD total_bytes_read;
  undefined8 *buf1;
  bool bVar5;
  HCRYPTKEY hKey;
  HCRYPTHASH hHash;
  uint bytes_read;
  int offset;
  HCRYPTPROV hProv;
  ushort *temp;
  char read_buf [16];
  
  input_file_handle = (HANDLE *)0x0;
  buf1 = (undefined8 *)0x0;
  hProv = 0;
  hKey = 0;
  hHash = 0;
  CryptAcquireContextW(&hProv,L"DataSafeCryptContainer",(LPCWSTR)0x0,0x18,0x50);
  BVar1 = CryptAcquireContextW(&hProv,L"DataSafeCryptContainer",(LPCWSTR)0x0,0x18,0x48);
  if (BVar1 == 0) {
    GetLastError();
    print("%x");
    goto LAB_004010b6;
  }
  BVar1 = CryptCreateHash(hProv,0x8003,0,0,&hHash);
  if ((BVar1 == 0) || (buf1 = (undefined8 *)allocate(0xe), buf1 == (undefined8 *)0x0))
  goto LAB_004010b6;
  mac_addr = (undefined8 *)get_mac_addr();
  if (mac_addr == (undefined8 *)0x0) {
LAB_00401252:
    free(buf1);
    buf1 = (undefined8 *)0x0;
  }
  else {
    copy_buf_(buf1,0xe,mac_addr,6);
    temp = (ushort *)execute_command(L"wmic bios get serialnumber");
    if (temp == (ushort *)0x0) {
LAB_00401241:
      bVar5 = false;
    }
    else {
      puVar3 = extract_serial(temp);
      free(temp);
      if (puVar3 == (undefined4 *)0x0) goto LAB_00401241;
      temp = (ushort *)lchar_to_dword((ushort *)puVar3);
      if (temp == (ushort *)0xffffffff) {
        bVar5 = false;
        free(puVar3);
      }
      else {
                    // Now buffer will contain mac + bios_serial[0:4]
        copy_buf_((undefined8 *)((int)buf1 + 6),8,(undefined8 *)&temp,4);
        disk_serial = get_disk_serial();
        if (disk_serial == (undefined4 *)0x0) {
          bVar5 = false;
          free(puVar3);
        }
        else {
          temp = (ushort *)lchar_to_dword((ushort *)disk_serial);
          bVar5 = temp != (ushort *)0xffffffff;
          if (bVar5) {
                    // Now buffer will contain mac + bios_serial[0:4] + disk_serial[0:4]
            copy_buf_((undefined8 *)((int)buf1 + 10),4,(undefined8 *)&temp,4);
          }
          free(disk_serial);
          free(puVar3);
        }
      }
    }
    free(mac_addr);
    if (!bVar5) goto LAB_00401252;
  }
  if (((buf1 != (undefined8 *)0x0) && (BVar1 = CryptHashData(hHash,(BYTE *)buf1,0xe,0), BVar1 != 0))
     && (BVar1 = CryptDeriveKey(hProv,0x6610,hHash,0,&hKey), BVar1 != 0
                    // CALG_AES_256 = 0x6610)) {
    input_file_handle = get_file_handle(file_name,0x80000000,3);
    total_bytes_read = 0;
    if (input_file_handle != (HANDLE *)0x0) {
      temp = (ushort *)0x0;
      input_file_size = GetFileSize(*input_file_handle,(LPDWORD)0x0);
                    // align size to 16 bytes
      _Size = (input_file_size & 0xfffffff0) + 0x10;
      output_buf = (char *)allocate(_Size);
      if (output_buf != (char *)0x0) {
        offset = 0;
        read_buf._0_4_ = 0;
        read_buf._4_4_ = 0;
        read_buf._8_4_ = 0;
        read_buf._12_4_ = 0;
        iVar4 = ReadFile(*input_file_handle,read_buf,0x10,&bytes_read,(LPOVERLAPPED)0x0);
        while ((iVar4 != 0 && (bytes_read != 0))) {
          total_bytes_read = total_bytes_read + bytes_read;
          if (total_bytes_read == input_file_size) {
            temp = (ushort *)0x1;
          }
          BVar1 = CryptEncrypt(hKey,0,(BOOL)temp,0,(BYTE *)read_buf,&bytes_read,0x10);
          if (BVar1 == 0) {
            free(output_buf);
            goto LAB_004010b6;
          }
          copy_buffer((undefined8 *)(output_buf + offset),(undefined8 *)read_buf,bytes_read);
          offset = offset + bytes_read;
          read_buf._0_4_ = 0;
          read_buf._4_4_ = 0;
          read_buf._8_4_ = 0;
          read_buf._12_4_ = 0;
          iVar4 = ReadFile(*input_file_handle,read_buf,0x10,&bytes_read,(LPOVERLAPPED)0x0);
        }
        *output_size = _Size;
      }
    }
  }
LAB_004010b6:
  CryptReleaseContext(hProv,0);
  if (hProv != 0) {
    CryptReleaseContext(hProv,0);
  }
  if (hHash != 0) {
    CryptDestroyHash(hHash);
  }
  if (buf1 == (undefined8 *)0x0) {
    free((void *)0x0);
  }
  if (hKey != 0) {
    CryptDestroyKey(hKey);
  }
  if (input_file_handle != (HANDLE *)0x0) {
    CloseHandle(*input_file_handle);
    free(input_file_handle);
  }
  pcVar2 = (char *)FUN_00401f78();
  return pcVar2;
}



HANDLE * __fastcall get_file_handle(LPCWSTR param_1,DWORD param_2,DWORD param_3)

{
  HANDLE *ppvVar1;
  HANDLE pvVar2;
  
  ppvVar1 = (HANDLE *)allocate(4);
  if (ppvVar1 != (HANDLE *)0x0) {
    pvVar2 = CreateFileW(param_1,param_2,0,(LPSECURITY_ATTRIBUTES)0x0,param_3,0x80,(HANDLE)0x0);
    *ppvVar1 = pvVar2;
    if (pvVar2 != (HANDLE)0xffffffff) {
      return ppvVar1;
    }
    free(ppvVar1);
  }
  return (HANDLE *)0x0;
}



undefined4 __fastcall copy_buf_(undefined8 *buf1,uint buf1_len,undefined8 *buf2,uint buf2_len)

{
  undefined4 *puVar1;
  
  if (buf1 == (undefined8 *)0x0) {
    puVar1 = (undefined4 *)FUN_00407f40();
    *puVar1 = 0x16;
    FUN_00407e83();
    return 0x16;
  }
  if ((buf2 != (undefined8 *)0x0) && (buf2_len <= buf1_len)) {
    copy_buffer(buf1,buf2,buf2_len);
    return 0;
  }
  FUN_00402e50((int *)buf1,0,buf1_len);
  if (buf2 == (undefined8 *)0x0) {
    puVar1 = (undefined4 *)FUN_00407f40();
    *puVar1 = 0x16;
    FUN_00407e83();
  }
  else {
    if (buf1_len < buf2_len) {
      puVar1 = (undefined4 *)FUN_00407f40();
      *puVar1 = 0x22;
      FUN_00407e83();
      return 0x22;
    }
  }
  return 0x16;
}



char * __fastcall padded_md5_filename_mac(char *file_name)

{
  ushort uVar1;
  ushort *puVar2;
  undefined8 *pbData;
  int file_name_len_2;
  undefined4 *p_mac_addr;
  BOOL BVar3;
  char *pcVar4;
  undefined4 *puVar5;
  int i;
  uint file_name_len;
  HCRYPTPROV hProv;
  DWORD hash_len;
  HCRYPTHASH hHash;
  byte hash_output [16];
  
  puVar2 = FUN_00402abe(file_name,'\\');
  if (puVar2 != (ushort *)0x0) {
    file_name = (char *)(puVar2 + 1);
  }
  puVar2 = (ushort *)file_name;
  do {
    uVar1 = *puVar2;
    puVar2 = puVar2 + 1;
  } while (uVar1 != 0);
  file_name_len = (int)((int)puVar2 - (int)((ushort *)file_name + 1)) >> 1;
  pbData = (undefined8 *)allocate(file_name_len + 6);
  if (pbData == (undefined8 *)0x0) {
    pcVar4 = (char *)FUN_00401f78();
    return pcVar4;
  }
  file_name_len_2 = copy_str_into_buffer(pbData,(ushort *)file_name,file_name_len);
  if (file_name_len_2 == 0) goto LAB_00401633;
  p_mac_addr = (undefined4 *)get_mac_addr();
  puVar5 = (undefined4 *)((int)pbData + file_name_len);
  if (puVar5 == (undefined4 *)0x0) {
LAB_00401530:
    puVar5 = (undefined4 *)FUN_00407f40();
    *puVar5 = 0x16;
    FUN_00407e83();
  }
  else {
    if (p_mac_addr == (undefined4 *)0x0) {
      *puVar5 = 0;
      *(undefined2 *)(puVar5 + 1) = 0;
      goto LAB_00401530;
    }
                    // Copy MAC address after file name
    *puVar5 = *p_mac_addr;
    *(undefined2 *)(puVar5 + 1) = *(undefined2 *)(p_mac_addr + 1);
  }
  hProv = 0;
  hHash = 0;
  hash_len = 0x10;
  BVar3 = CryptAcquireContextW(&hProv,(LPCWSTR)0x0,(LPCWSTR)0x0,1,0xf0000000);
  if (BVar3 != 0) {
                    // MD5 (0x8003)
    BVar3 = CryptCreateHash(hProv,0x8003,0,0,&hHash);
    if (BVar3 != 0) {
      BVar3 = CryptHashData(hHash,(BYTE *)pbData,file_name_len + 6,0);
      if (BVar3 != 0) {
        BVar3 = CryptGetHashParam(hHash,2,hash_output,&hash_len,0);
        if (BVar3 != 0) {
          puVar5 = (undefined4 *)allocate(0x20);
          if (puVar5 != (undefined4 *)0x0) {
            i = 0;
            *puVar5 = 0;
            puVar5[1] = 0;
            puVar5[2] = 0;
            puVar5[3] = 0;
            puVar5[4] = 0;
            puVar5[5] = 0;
            puVar5[6] = 0;
            puVar5[7] = 0;
                    // Pad MD5 result (nibble to byte)
            if (0 < (int)hash_len) {
              do {
                if (0xf < i) break;
                *(byte *)((int)puVar5 + i * 2) = hash_output[i] >> 4;
                *(byte *)((int)puVar5 + i * 2 + 1) = hash_output[i] & 0xf;
                i = i + 1;
              } while (i < (int)hash_len);
            }
          }
        }
      }
    }
    if (hProv != 0) {
      CryptReleaseContext(hProv,0);
    }
    if (hHash != 0) {
      CryptDestroyHash(hHash);
    }
  }
  if (p_mac_addr != (undefined4 *)0x0) {
    free(p_mac_addr);
  }
LAB_00401633:
  free(pbData);
  pcVar4 = (char *)FUN_00401f78();
  return pcVar4;
}



// WARNING: Function: __alloca_probe replaced with injection: alloca_probe

char * __fastcall encrypt(char *input_file_name,int *output_len)

{
  size_t sVar1;
  undefined4 *padded_md5;
  char *pcVar2;
  char *encrypted_buf;
  ushort *puVar3;
  uint dd_serial_dword;
  undefined4 *buffer;
  undefined4 *puVar4;
  undefined4 bios_serial_dword;
  int iVar5;
  uint uVar6;
  int limit;
  int iVar7;
  uint uVar8;
  undefined4 *diskdrive_serial;
  uint *p_current_src;
  undefined4 *p_current_padded_md5;
  uint *p_current_dest;
  uint garble_buf [625];
  uint temp3;
  char *temp;
  undefined4 *temp2;
  size_t src_offset;
  int counter;
  size_t encrypted_size;
  uint garble_buf_copy [625];
  
  diskdrive_serial = (undefined4 *)0x0;
  padded_md5 = (undefined4 *)padded_md5_filename_mac(input_file_name);
  if (padded_md5 == (undefined4 *)0x0) {
    pcVar2 = (char *)FUN_00401f78();
    return pcVar2;
  }
  encrypted_size = 0;
  encrypted_buf = do_encrypt((LPCWSTR)input_file_name,&encrypted_size);
  if (encrypted_buf != (char *)0x0) {
    puVar3 = (ushort *)execute_command(L"wmic diskdrive get serialnumber");
    if (puVar3 == (ushort *)0x0) {
      diskdrive_serial = (undefined4 *)0x0;
    }
    else {
      diskdrive_serial = extract_serial(puVar3);
      free(puVar3);
      if (diskdrive_serial != (undefined4 *)0x0) {
        dd_serial_dword = lchar_to_dword((ushort *)diskdrive_serial);
        buffer = (undefined4 *)allocate(encrypted_size + 2992);
        if (buffer != (undefined4 *)0x0) {
          *buffer = 0x531b008a;
          puVar3 = (ushort *)execute_command(L"wmic bios get serialnumber");
          if (puVar3 != (ushort *)0x0) {
            puVar4 = extract_serial(puVar3);
            free(puVar3);
            if (puVar4 != (undefined4 *)0x0) {
              bios_serial_dword = lchar_to_dword((ushort *)puVar4);
              garble_buf(garble_buf,bios_serial_dword);
              src_offset = 0;
              limit = 625;
              counter = 0;
              uVar8 = (int)encrypted_size / 0x2e3 + ((int)encrypted_size >> 0x1f);
              p_current_src = garble_buf;
              p_current_dest = garble_buf_copy;
              while (limit != 0) {
                limit = limit + -1;
                *p_current_dest = *p_current_src;
                p_current_src = p_current_src + 1;
                p_current_dest = p_current_dest + 1;
              }
              iVar5 = (uVar8 >> 0x1f) + uVar8;
              uVar8 = iVar5 + 1;
                    // copy padded md5 starting from buf[4] (8 dwords)
              iVar7 = 8;
              p_current_padded_md5 = padded_md5;
              puVar4 = buffer;
              while (puVar4 = puVar4 + 1, iVar7 != 0) {
                iVar7 = iVar7 + -1;
                *puVar4 = *p_current_padded_md5;
                p_current_padded_md5 = p_current_padded_md5 + 1;
              }
              iVar7 = 0x24;
              iVar5 = encrypted_size + iVar5 * -0x2e3;
              do {
                uVar6 = other_garble(garble_buf_copy);
                *(uint *)(iVar7 + (int)buffer) = uVar6;
                if (counter == iVar5) {
                  uVar8 = uVar8 - 1;
                }
                copy_buffer((undefined8 *)(iVar7 + 4 + (int)buffer),
                            (undefined8 *)(encrypted_buf + src_offset),uVar8);
                sVar1 = encrypted_size;
                counter = counter + 1;
                src_offset = src_offset + uVar8;
                iVar7 = iVar7 + 4 + uVar8;
              } while (counter < 739);
              if (src_offset != encrypted_size) {
                print("NOT read enaugh bytes %d , %d");
              }
              iVar5 = sVar1 + 2988;
              *output_len = iVar5;
              iVar7 = 0;
              if (0 < iVar5) {
                do {
                  *(uint *)(iVar7 + (int)buffer) = *(uint *)(iVar7 + (int)buffer) ^ dd_serial_dword;
                  iVar7 = iVar7 + 4;
                } while (iVar7 < iVar5);
              }
              goto LAB_00401895;
            }
          }
          free(buffer);
        }
      }
    }
  }
LAB_00401895:
  free(padded_md5);
  if (encrypted_buf != (char *)0x0) {
    free(encrypted_buf);
  }
  if (diskdrive_serial != (undefined4 *)0x0) {
    free(diskdrive_serial);
  }
  pcVar2 = (char *)FUN_00401f78();
  return pcVar2;
}



undefined4 __cdecl main(int argc,char **argv)

{
  char *output_buffer;
  HANDLE *ppvVar1;
  HANDLE pvVar2;
  DWORD bytes_written;
  int bytes_to_write;
  LPCWSTR lpOutputFileName;
  
  if (argc < 3) {
    print("USAGE: Encrypt <input file name> <output file name>");
    return 0xffffffff;
  }
  argc = 0;
  output_buffer = encrypt(argv[1],&argc);
  lpOutputFileName = (LPCWSTR)argv[2];
  ppvVar1 = (HANDLE *)allocate(4);
  if (ppvVar1 != (HANDLE *)0x0) {
    pvVar2 = CreateFileW(lpOutputFileName,0x40000000,0,(LPSECURITY_ATTRIBUTES)0x0,2,0x80,(HANDLE)0x0
                        );
    bytes_to_write = argc;
    *ppvVar1 = pvVar2;
    if (pvVar2 == (HANDLE)0xffffffff) {
      free(ppvVar1);
      return 0;
    }
    bytes_written = 0;
    WriteFile(*ppvVar1,output_buffer,argc,&bytes_written,(LPOVERLAPPED)0x0);
    if (bytes_written != bytes_to_write) {
      cleanup(lpOutputFileName);
    }
  }
  return 0;
}



void __fastcall garble_buf(undefined4 *buffer,undefined4 initial_value)

{
  *buffer = initial_value;
  buffer[0x270] = 1;
  do {
    buffer[buffer[0x270]] = (buffer + buffer[0x270])[-1] * 0x17b5;
    buffer[0x270] = buffer[0x270] + 1;
  } while ((int)buffer[0x270] < 0x270);
  return;
}



uint __cdecl other_garble(uint *garbled_buf_copy)

{
  uint uVar1;
  int i;
  
  if ((0x26f < (int)garbled_buf_copy[0x270]) || ((int)garbled_buf_copy[0x270] < 0)) {
    if ((0x270 < (int)garbled_buf_copy[0x270]) || ((int)garbled_buf_copy[0x270] < 0)) {
      garble_buf(garbled_buf_copy,0x1105);
    }
    i = 0;
    while (i < 0xe3) {
      garbled_buf_copy[i] =
           (garbled_buf_copy[i] & 0x80000000 | garbled_buf_copy[i + 1] & 0x7fffffff) >> 1 ^
           garbled_buf_copy[i + 0x18d] ^
           *(uint *)(&DAT_0041e8c0 + (garbled_buf_copy[i + 1] & 1) * 4);
      i = i + 1;
    }
    while (i < 0x26f) {
      garbled_buf_copy[i] =
           (garbled_buf_copy[i] & 0x80000000 | garbled_buf_copy[i + 1] & 0x7fffffff) >> 1 ^
           garbled_buf_copy[i + -0xe3] ^
           *(uint *)(&DAT_0041e8c0 + (garbled_buf_copy[i + 1] & 1) * 4);
      i = i + 1;
    }
    garbled_buf_copy[0x26f] =
         (garbled_buf_copy[0x26f] & 0x80000000 | *garbled_buf_copy & 0x7fffffff) >> 1 ^
         garbled_buf_copy[0x18c] ^ *(uint *)(&DAT_0041e8c0 + (*garbled_buf_copy & 1) * 4);
    garbled_buf_copy[0x270] = 0;
  }
  uVar1 = garbled_buf_copy[garbled_buf_copy[0x270]];
  garbled_buf_copy[0x270] = garbled_buf_copy[0x270] + 1;
  uVar1 = uVar1 >> 0xb ^ uVar1;
  uVar1 = (uVar1 & 0x13a58ad) << 7 ^ uVar1;
  uVar1 = (uVar1 & 0x1df8c) << 0xf ^ uVar1;
  return uVar1 >> 0x12 ^ uVar1;
}



undefined * get_mac_addr(void)

{
  char cVar1;
  int *piVar2;
  int iVar3;
  undefined *puVar4;
  int *piVar5;
  size_t local_8;
  
  puVar4 = (undefined *)0x0;
  local_8 = 0x288;
  piVar5 = (int *)allocate(0x288);
  if (piVar5 == (int *)0x0) {
    return (undefined *)0x0;
  }
  iVar3 = GetAdaptersInfo(piVar5,&local_8);
  if (iVar3 == 0x6f) {
    free(piVar5);
    piVar5 = (int *)allocate(local_8);
    if (piVar5 == (int *)0x0) {
      return (undefined *)0x0;
    }
    iVar3 = GetAdaptersInfo(piVar5,&local_8);
  }
  if (iVar3 == 0) {
    cVar1 = *(char *)(piVar5 + 0x65);
    piVar2 = piVar5;
    while ((((cVar1 == 0 && (*(char *)((int)piVar2 + 0x195) == 0)) &&
            (*(char *)((int)piVar2 + 0x196) == 0)) &&
           (((*(char *)((int)piVar2 + 0x197) == 0 && (*(char *)(piVar2 + 0x66) == 0)) &&
            (*(char *)((int)piVar2 + 0x199) == 0))))) {
      piVar2 = (int *)*piVar2;
      if (piVar2 == (int *)0x0) goto LAB_00401ce1;
      cVar1 = *(char *)(piVar2 + 0x65);
    }
    puVar4 = (undefined *)allocate(6);
    if (puVar4 != (undefined *)0x0) {
      *puVar4 = *(undefined *)(piVar2 + 0x65);
      puVar4[1] = *(undefined *)((int)piVar2 + 0x195);
      puVar4[2] = *(undefined *)((int)piVar2 + 0x196);
      puVar4[3] = *(undefined *)((int)piVar2 + 0x197);
      puVar4[4] = *(undefined *)(piVar2 + 0x66);
      puVar4[5] = *(undefined *)((int)piVar2 + 0x199);
    }
  }
LAB_00401ce1:
  free(piVar5);
  return puVar4;
}



undefined4 * __fastcall extract_serial(ushort *param_1)

{
  ushort *puVar1;
  undefined4 *puVar2;
  uint uVar3;
  uint uVar4;
  undefined4 *puVar5;
  undefined4 *puVar6;
  
  puVar1 = FUN_00402b60(param_1,10);
  if (puVar1 != (ushort *)0x0) {
    puVar5 = (undefined4 *)(puVar1 + 1);
    puVar1 = FUN_00402b60((ushort *)puVar5,0x20);
    if (puVar1 != (ushort *)0x0) {
      uVar4 = (int)((int)puVar1 - (int)puVar5) >> 1;
      puVar2 = (undefined4 *)allocate(uVar4 * 2 + 2);
      if (puVar2 != (undefined4 *)0x0) {
        if (0 < (int)uVar4) {
          uVar3 = uVar4 >> 1;
          puVar6 = puVar2;
          while (uVar3 != 0) {
            uVar3 = uVar3 - 1;
            *puVar6 = *puVar5;
            puVar5 = puVar5 + 1;
            puVar6 = puVar6 + 1;
          }
          uVar3 = (uint)((uVar4 & 1) != 0);
          while (uVar3 != 0) {
            uVar3 = uVar3 - 1;
            *(undefined2 *)puVar6 = *(undefined2 *)puVar5;
            puVar5 = (undefined4 *)((int)puVar5 + 2);
            puVar6 = (undefined4 *)((int)puVar6 + 2);
          }
        }
        *(undefined2 *)(uVar4 * 2 + (int)puVar2) = 0;
        return puVar2;
      }
    }
  }
  return (undefined4 *)0x0;
}



undefined4 __fastcall lchar_to_dword(ushort *param_1)

{
  ushort uVar1;
  undefined8 *puVar2;
  int iVar3;
  undefined4 uVar4;
  ushort *puVar5;
  uint uVar6;
  
  uVar4 = 0xffffffff;
  puVar5 = param_1;
  do {
    uVar1 = *puVar5;
    puVar5 = puVar5 + 1;
  } while (uVar1 != 0);
  uVar6 = (int)((int)puVar5 - (int)(param_1 + 1)) >> 1;
  puVar2 = (undefined8 *)allocate(uVar6 + 1);
  if (puVar2 == (undefined8 *)0x0) {
    return 0xffffffff;
  }
  *(undefined *)((int)puVar2 + uVar6) = 0;
  iVar3 = copy_str_into_buffer(puVar2,param_1,uVar6);
  if (iVar3 != 0) {
    uVar4 = *(undefined4 *)puVar2;
  }
  free(puVar2);
  return uVar4;
}



undefined4 * get_disk_serial(void)

{
  ushort *puVar1;
  undefined4 *puVar2;
  
  puVar1 = (ushort *)execute_command(L"wmic diskdrive get serialnumber");
  if (puVar1 == (ushort *)0x0) {
    return (undefined4 *)0x0;
  }
  puVar2 = extract_serial(puVar1);
  free(puVar1);
  return puVar2;
}



int __cdecl FUN_00401e10(wchar_t *param_1,uint param_2,wchar_t *param_3)

{
  undefined4 *puVar1;
  int iVar2;
  __crt_locale_pointers *p_Var3;
  char *pcVar4;
  
  pcVar4 = &stack0x00000010;
  p_Var3 = (__crt_locale_pointers *)0x0;
  puVar1 = (undefined4 *)FUN_00401000();
  iVar2 = ___stdio_common_vswprintf_s(*puVar1,puVar1[1],param_1,param_2,param_3,p_Var3,pcVar4);
  if (iVar2 < 0) {
    iVar2 = -1;
  }
  return iVar2;
}



void * __fastcall execute_command(short *param_1)

{
  short sVar1;
  wchar_t *_Command;
  int iVar2;
  HANDLE *ppvVar3;
  HANDLE hFile;
  BOOL BVar4;
  short *psVar5;
  int iVar6;
  void *lpBuffer;
  short *local_10;
  uint local_c;
  short *local_8;
  
  lpBuffer = (void *)0x0;
  psVar5 = param_1;
  do {
    sVar1 = *psVar5;
    psVar5 = psVar5 + 1;
  } while (sVar1 != 0);
  iVar6 = (int)((int)psVar5 - (int)(param_1 + 1)) >> 1;
  local_8 = param_1;
  _Command = (wchar_t *)allocate(iVar6 * 2 + 0x2e);
  if (_Command == (wchar_t *)0x0) {
    return (void *)0x0;
  }
  iVar2 = FUN_00401e10((wchar_t *)_Command,iVar6 + 0x17,(wchar_t *)L"%s%s%s");
  if (iVar2 != -1) {
    _Command[iVar6 + 0x15] = L'\0';
    __wsystem(_Command);
    ppvVar3 = (HANDLE *)allocate(4);
    if (ppvVar3 != (HANDLE *)0x0) {
      hFile = CreateFileW(L"command_result.txt",0x80000000,0,(LPSECURITY_ATTRIBUTES)0x0,3,0x80,
                          (HANDLE)0x0);
      *ppvVar3 = hFile;
      if (hFile != (HANDLE)0xffffffff) {
        local_8 = (short *)GetFileSize(hFile,(LPDWORD)0x0);
        if (local_8 != (short *)0xffffffff) {
          local_c = (uint)local_8 & 0xfffffffe;
          lpBuffer = allocate(local_c + 2);
          if (lpBuffer != (void *)0x0) {
            *(undefined2 *)(local_c + (int)lpBuffer) = 0;
            BVar4 = ReadFile(*ppvVar3,lpBuffer,(DWORD)local_8,(LPDWORD)&local_10,(LPOVERLAPPED)0x0);
            if ((BVar4 == 0) || (local_10 != local_8)) {
              free(lpBuffer);
              lpBuffer = (void *)0x0;
            }
          }
        }
        CloseHandle(*ppvVar3);
      }
      free(ppvVar3);
    }
    cleanup(L"command_result.txt");
  }
  free(_Command);
  return lpBuffer;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void __fastcall FUN_00401f78(int iParm1)

{
  code *pcVar1;
  BOOL BVar2;
  undefined4 extraout_ECX;
  undefined4 extraout_EDX;
  undefined4 unaff_EBX;
  undefined4 local_4;
  undefined4 unaff_ESI;
  undefined4 unaff_EDI;
  undefined2 in_ES;
  undefined2 in_CS;
  undefined2 in_SS;
  undefined2 in_DS;
  undefined2 in_FS;
  undefined2 in_GS;
  byte in_PF;
  byte in_AF;
  byte in_TF;
  byte in_IF;
  byte in_NT;
  byte in_AC;
  byte in_VIF;
  byte in_VIP;
  byte in_ID;
  undefined4 local_res0;
  
  if (iParm1 == DAT_0041e004) {
    return;
  }
  BVar2 = IsProcessorFeaturePresent(0x17);
  if (BVar2 != 0) {
    pcVar1 = (code *)swi(0x29);
    (*pcVar1)();
    return;
  }
  _DAT_0041ea30 =
       (uint)(in_NT & 1) * 0x4000 | (uint)(in_IF & 1) * 0x200 | (uint)(in_TF & 1) * 0x100 |
       (uint)(BVar2 < 0) * 0x80 | (uint)(BVar2 == 0) * 0x40 | (uint)(in_AF & 1) * 0x10 |
       (uint)(in_PF & 1) * 4 | (uint)(in_ID & 1) * 0x200000 | (uint)(in_VIP & 1) * 0x100000 |
       (uint)(in_VIF & 1) * 0x80000 | (uint)(in_AC & 1) * 0x40000;
  _DAT_0041ea34 = &stack0x00000004;
  _DAT_0041e970 = 0x10001;
  _DAT_0041e920 = 0xc0000409;
  _DAT_0041e924 = 1;
  _DAT_0041e930 = 1;
  DAT_0041e934 = 2;
  _DAT_0041e92c = local_res0;
  _DAT_0041e9fc = in_GS;
  _DAT_0041ea00 = in_FS;
  _DAT_0041ea04 = in_ES;
  _DAT_0041ea08 = in_DS;
  _DAT_0041ea0c = unaff_EDI;
  _DAT_0041ea10 = unaff_ESI;
  _DAT_0041ea14 = unaff_EBX;
  _DAT_0041ea18 = extraout_EDX;
  _DAT_0041ea1c = extraout_ECX;
  _DAT_0041ea20 = BVar2;
  _DAT_0041ea24 = local_4;
  DAT_0041ea28 = local_res0;
  _DAT_0041ea2c = in_CS;
  _DAT_0041ea38 = in_SS;
  FUN_004021da((_EXCEPTION_POINTERS *)&PTR_DAT_004171b0);
  return;
}



// WARNING: Function: __SEH_prolog4 replaced with injection: SEH_prolog4

UINT entry(void)

{
  code *pcVar1;
  bool bVar2;
  uint uVar3;
  int iVar4;
  code **ppcVar5;
  FuncDef5 **ppFVar6;
  char **argv;
  int *piVar7;
  UINT UVar8;
  UINT unaff_ESI;
  undefined4 *in_FS_OFFSET;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  UINT local_24;
  undefined4 local_14;
  
  FUN_004025e2();
  uVar3 = ___scrt_initialize_crt(1);
  if ((char)uVar3 != 0) {
    bVar2 = false;
    uVar3 = FUN_0040235c();
    if (DAT_0041ec3c != 1) {
      if (DAT_0041ec3c == 0) {
        DAT_0041ec3c = 1;
        iVar4 = FUN_00409556((undefined **)&DAT_0041716c,(undefined **)&DAT_00417184);
        if (iVar4 != 0) {
          unaff_ESI = 0xff;
          goto LAB_004021aa;
        }
        FUN_0040950f((undefined **)&DAT_00417160,(undefined **)&DAT_00417168);
        DAT_0041ec3c = 2;
      }
      else {
        bVar2 = true;
      }
      ___scrt_release_startup_lock((char)uVar3);
      ppcVar5 = (code **)FUN_0040269a();
      if (*ppcVar5 != (code *)0x0) {
        uVar3 = FUN_0040246c((int)ppcVar5);
        if ((char)uVar3 != 0) {
          pcVar1 = *ppcVar5;
          uVar11 = 0;
          uVar10 = 2;
          uVar9 = 0;
          _guard_check_icall();
          (*pcVar1)(uVar9,uVar10,uVar11);
        }
      }
      ppFVar6 = (FuncDef5 **)FUN_004026a0();
      if (*ppFVar6 != (FuncDef5 *)0x0) {
        uVar3 = FUN_0040246c((int)ppFVar6);
        if ((char)uVar3 != 0) {
          FUN_004097ea(*ppFVar6);
        }
      }
      argv = (char **)FUN_00409882();
      argv = (char **)*argv;
      piVar7 = (int *)FUN_0040987c();
      FUN_004094f6();
      unaff_ESI = main(*piVar7,argv);
      uVar3 = FUN_004027c5();
      if ((char)uVar3 != 0) {
        if (!bVar2) {
          FUN_004097c5();
        }
        ___scrt_uninitialize_crt(1,0);
LAB_004021aa:
        *in_FS_OFFSET = local_14;
        return unaff_ESI;
      }
      goto LAB_004021c1;
    }
  }
  FUN_004026a6(7);
LAB_004021c1:
  FUN_00409810(unaff_ESI);
  FUN_004097d4(local_24);
  pcVar1 = (code *)swi(3);
  UVar8 = (*pcVar1)();
  return UVar8;
}



void __cdecl FUN_004021da(_EXCEPTION_POINTERS *param_1)

{
  HANDLE hProcess;
  UINT uExitCode;
  
  SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)0x0);
  UnhandledExceptionFilter(param_1);
  uExitCode = 0xc0000409;
  hProcess = GetCurrentProcess();
  TerminateProcess(hProcess,uExitCode);
  return;
}



uint __cdecl FUN_004022fb(uint param_1)

{
  byte bVar1;
  
  bVar1 = 0x20 - ((byte)DAT_0041e004 & 0x1f) & 0x1f;
  return (param_1 >> bVar1 | param_1 << 0x20 - bVar1) ^ DAT_0041e004;
}



// Library Function - Single Match
// Name: ?find_pe_section@@YAPAU_IMAGE_SECTION_HEADER@@QAEI@Z
// Library: Visual Studio 2015 Release
// struct _IMAGE_SECTION_HEADER * __cdecl find_pe_section(unsigned char * const,unsigned int)

_IMAGE_SECTION_HEADER * __cdecl find_pe_section(uchar *param_1,uint param_2)

{
  _IMAGE_SECTION_HEADER *p_Var1;
  _IMAGE_SECTION_HEADER *p_Var2;
  
  param_1 = param_1 + *(int *)(param_1 + 0x3c);
  p_Var1 = (_IMAGE_SECTION_HEADER *)(param_1 + (uint)*(ushort *)(param_1 + 0x14) + 0x18);
  p_Var2 = p_Var1 + (uint)*(ushort *)(param_1 + 6);
  while( true ) {
    if (p_Var1 == p_Var2) {
      return (_IMAGE_SECTION_HEADER *)0x0;
    }
    if ((p_Var1->VirtualAddress <= param_2) && (param_2 < p_Var1->Misc + p_Var1->VirtualAddress))
    break;
    p_Var1 = p_Var1 + 1;
  }
  return p_Var1;
}



// WARNING: Removing unreachable block (ram,0x00402376)
// WARNING: Removing unreachable block (ram,0x0040238a)

uint FUN_0040235c(void)

{
  uint uVar1;
  int in_FS_OFFSET;
  
  uVar1 = ___scrt_is_ucrt_dll_in_use();
  if (uVar1 != 0) {
    LOCK();
    DAT_0041ec40 = DAT_0041ec40 ^
                   (uint)(DAT_0041ec40 == 0) *
                   (DAT_0041ec40 ^ *(uint *)(*(int *)(in_FS_OFFSET + 0x18) + 4));
    uVar1 = 0;
  }
  return uVar1 & 0xffffff00;
}



// Library Function - Single Match
// Name: ___scrt_initialize_crt
// Library: Visual Studio 2015 Release

uint __cdecl ___scrt_initialize_crt(int param_1)

{
  uint uVar1;
  undefined4 uVar2;
  
  if (param_1 == 0) {
    DAT_0041ec44 = 1;
  }
  FUN_0040291b();
  uVar1 = ___vcrt_initialize();
  if ((char)uVar1 != 0) {
    uVar2 = ___acrt_initialize();
    if ((char)uVar2 != 0) {
      return CONCAT31((int3)((uint)uVar2 >> 8),1);
    }
    uVar1 = ___vcrt_uninitialize(0);
  }
  return uVar1 & 0xffffff00;
}



uint FUN_004023c7(int param_1)

{
  code *pcVar1;
  byte bVar2;
  undefined4 in_EAX;
  uint uVar3;
  undefined4 uVar4;
  
  if (DAT_0041ec45 != 0) {
    return CONCAT31((int3)((uint)in_EAX >> 8),1);
  }
  if ((param_1 != 0) && (param_1 != 1)) {
    FUN_004026a6(5);
    pcVar1 = (code *)swi(3);
    uVar4 = (*pcVar1)();
    return uVar4;
  }
  uVar3 = ___scrt_is_ucrt_dll_in_use();
  if ((uVar3 == 0) || (param_1 != 0)) {
    bVar2 = 0x20 - ((byte)DAT_0041e004 & 0x1f) & 0x1f;
    uVar3 = (0xffffffffU >> bVar2 | -1 << 0x20 - bVar2) ^ DAT_0041e004;
    DAT_0041ec48 = uVar3;
    DAT_0041ec4c = uVar3;
    DAT_0041ec50 = uVar3;
    DAT_0041ec54 = uVar3;
    DAT_0041ec58 = uVar3;
    DAT_0041ec5c = uVar3;
LAB_00402458:
    DAT_0041ec45 = 1;
    uVar3 = CONCAT31((int3)(uVar3 >> 8),1);
  }
  else {
    uVar3 = FUN_00409cac(&DAT_0041ec48);
    if (uVar3 == 0) {
      uVar3 = FUN_00409cac(&DAT_0041ec54);
      if (uVar3 == 0) goto LAB_00402458;
    }
    uVar3 = uVar3 & 0xffffff00;
  }
  return uVar3;
}



// WARNING: Function: __SEH_prolog4 replaced with injection: SEH_prolog4

uint __cdecl FUN_0040246c(int param_1)

{
  _IMAGE_SECTION_HEADER *p_Var1;
  uint uVar2;
  undefined4 *in_FS_OFFSET;
  undefined4 local_14;
  
  p_Var1 = find_pe_section((uchar *)&IMAGE_DOS_HEADER_00400000,param_1 - 0x400000);
  if ((p_Var1 == (_IMAGE_SECTION_HEADER *)0x0) || ((int)p_Var1->Characteristics < 0)) {
    uVar2 = (uint)p_Var1 & 0xffffff00;
  }
  else {
    uVar2 = CONCAT31((int3)((uint)p_Var1 >> 8),1);
  }
  *in_FS_OFFSET = local_14;
  return uVar2;
}



// Library Function - Single Match
// Name: ___scrt_release_startup_lock
// Library: Visual Studio 2015 Release

uint __cdecl ___scrt_release_startup_lock(char param_1)

{
  uint uVar1;
  uint uVar2;
  
  uVar2 = ___scrt_is_ucrt_dll_in_use();
  uVar1 = DAT_0041ec40;
  if ((uVar2 != 0) && (param_1 == 0)) {
    DAT_0041ec40 = 0;
    uVar2 = uVar1;
  }
  return uVar2;
}



// Library Function - Single Match
// Name: ___scrt_uninitialize_crt
// Library: Visual Studio 2015 Release

undefined __cdecl ___scrt_uninitialize_crt(undefined4 param_1,char param_2)

{
  if ((DAT_0041ec44 == 0) || (param_2 == 0)) {
    FUN_00409e19((char)param_1);
    ___vcrt_uninitialize(param_1);
  }
  return 1;
}



// Library Function - Single Match
// Name: __onexit
// Library: Visual Studio 2015 Release

_onexit_t __cdecl __onexit(_onexit_t _Func)

{
  int iVar1;
  byte bVar2;
  
  bVar2 = (byte)DAT_0041e004 & 0x1f;
  if (((DAT_0041e004 ^ DAT_0041ec48) >> bVar2 | (DAT_0041e004 ^ DAT_0041ec48) << 0x20 - bVar2) ==
      0xffffffff) {
    iVar1 = __crt_atexit(_Func);
  }
  else {
    iVar1 = FUN_00409cd5(0x48,(char)_Func);
  }
  return (_onexit_t)(~-(uint)(iVar1 != 0) & (uint)_Func);
}



// Library Function - Single Match
// Name: _atexit
// Library: Visual Studio 2015 Release

int __cdecl _atexit(void *param_1)

{
  _onexit_t p_Var1;
  
  p_Var1 = __onexit((_onexit_t)param_1);
  return (uint)(p_Var1 != (_onexit_t)0x0) - 1;
}



uint FUN_00402595(void)

{
  DWORD DVar1;
  uint local_18;
  uint local_14;
  _FILETIME local_10;
  uint local_8;
  
  local_10.dwLowDateTime = 0;
  local_10.dwHighDateTime = 0;
  GetSystemTimeAsFileTime((LPFILETIME)&local_10);
  local_8 = local_10.dwHighDateTime ^ local_10.dwLowDateTime;
  DVar1 = GetCurrentThreadId();
  local_8 = local_8 ^ DVar1;
  DVar1 = GetCurrentProcessId();
  local_8 = local_8 ^ DVar1;
  QueryPerformanceCounter((LARGE_INTEGER *)&local_18);
  return local_14 ^ local_18 ^ local_8 ^ (uint)&local_8;
}



void FUN_004025e2(void)

{
  if ((DAT_0041e004 == 0xbb40e64e) || ((DAT_0041e004 & 0xffff0000) == 0)) {
    DAT_0041e004 = FUN_00402595();
    if (DAT_0041e004 == 0xbb40e64e) {
      DAT_0041e004 = 0xbb40e64f;
    }
    else {
      if ((DAT_0041e004 & 0xffff0000) == 0) {
        DAT_0041e004 = DAT_0041e004 | (DAT_0041e004 | 0x4711) << 0x10;
      }
    }
  }
  DAT_0041e000 = ~DAT_0041e004;
  return;
}



undefined4 FUN_0040262d(void)

{
  return 0;
}



undefined4 FUN_00402630(void)

{
  return 1;
}



undefined4 FUN_00402634(void)

{
  return 0x4000;
}



void FUN_0040263a(void)

{
  InitializeSListHead((PSLIST_HEADER)&ListHead_0041ec60);
  return;
}



undefined FUN_00402646(void)

{
  return 1;
}



void FUN_00402649(void)

{
  code *pcVar1;
  errno_t eVar2;
  
  eVar2 = __controlfp_s((uint *)0x0,0x10000,0x30000);
  if (eVar2 == 0) {
    return;
  }
  FUN_004026a6(7);
  pcVar1 = (code *)swi(3);
  (*pcVar1)();
  return;
}



void _guard_check_icall(void)

{
  return;
}



undefined * FUN_0040266b(void)

{
  return &DAT_0041ec68;
}



void FUN_00402671(void)

{
  uint *puVar1;
  
  puVar1 = (uint *)FUN_00401000();
  *puVar1 = *puVar1 | 4;
  puVar1[1] = puVar1[1];
  puVar1 = (uint *)FUN_0040266b();
  *puVar1 = *puVar1 | 2;
  puVar1[1] = puVar1[1];
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

uint FUN_0040268e(void)

{
  return (uint)(_DAT_0041e00c == 0);
}



undefined * FUN_0040269a(void)

{
  return &DAT_0041f448;
}



undefined * FUN_004026a0(void)

{
  return &DAT_0041f444;
}



void FUN_004026a6(void)

{
  code *pcVar1;
  BOOL BVar2;
  LONG LVar3;
  byte in_PF;
  byte in_AF;
  byte in_TF;
  byte in_IF;
  byte in_NT;
  byte in_AC;
  byte in_VIF;
  byte in_VIP;
  byte in_ID;
  int local_328 [39];
  int *local_278;
  uint local_268;
  EXCEPTION_RECORD local_5c;
  _EXCEPTION_POINTERS local_c;
  
  BVar2 = IsProcessorFeaturePresent(0x17);
  if (BVar2 != 0) {
    pcVar1 = (code *)swi(0x29);
    (*pcVar1)();
    return;
  }
  FUN_00402855();
  local_278 = FUN_00402e50(local_328,0,0x2cc);
  local_268 = (uint)(in_NT & 1) * 0x4000 | (uint)SCARRY4((int)&stack0xfffffcc8,0xc) * 0x800 |
              (uint)(in_IF & 1) * 0x200 | (uint)(in_TF & 1) * 0x100 |
              (uint)((int)&stack0xfffffcd4 < 0) * 0x80 |
              (uint)((undefined *)register0x00000010 == (undefined *)0x32c) * 0x40 |
              (uint)(in_AF & 1) * 0x10 | (uint)(in_PF & 1) * 4 |
              (uint)((undefined *)0xfffffff3 < &stack0xfffffcc8) | (uint)(in_ID & 1) * 0x200000 |
              (uint)(in_VIP & 1) * 0x100000 | (uint)(in_VIF & 1) * 0x80000 |
              (uint)(in_AC & 1) * 0x40000;
  local_328[0] = 0x10001;
  FUN_00402e50((int *)&local_5c,0,0x50);
  local_5c.ExceptionCode = 0x40000015;
  local_5c.ExceptionFlags = 1;
  BVar2 = IsDebuggerPresent();
  local_c.ExceptionRecord = &local_5c;
  local_c.ContextRecord = (PCONTEXT)local_328;
  SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)0x0);
  LVar3 = UnhandledExceptionFilter(&local_c);
  if ((LVar3 == 0) && (BVar2 != 1)) {
    FUN_00402855();
  }
  return;
}



undefined4 thunk_FUN_0040262d(void)

{
  return 0;
}



uint FUN_004027c5(void)

{
  HMODULE pHVar1;
  int *piVar2;
  
  pHVar1 = GetModuleHandleW((LPCWSTR)0x0);
  if ((((pHVar1 != (HMODULE)0x0) && (*(short *)&pHVar1->unused == 0x5a4d)) &&
      (piVar2 = (int *)((int)&pHVar1->unused + (int)pHVar1[0xf]), *piVar2 == 0x4550)) &&
     (((pHVar1 = (HMODULE)0x10b, *(short *)(piVar2 + 6) == 0x10b && (0xe < (uint)piVar2[0x1d])) &&
      (piVar2[0x3a] != 0)))) {
    return 0x101;
  }
  return (uint)pHVar1 & 0xffffff00;
}



// Library Function - Single Match
// Name: ___scrt_unhandled_exception_filter@4
// Library: Visual Studio 2015 Release

undefined4 ___scrt_unhandled_exception_filter_4(int **param_1)

{
  int *piVar1;
  int iVar2;
  code *pcVar3;
  undefined4 uVar4;
  
  piVar1 = *param_1;
  if (((*piVar1 == -0x1f928c9d) && (piVar1[4] == 3)) &&
     ((iVar2 = piVar1[5], iVar2 == 0x19930520 ||
      (((iVar2 == 0x19930521 || (iVar2 == 0x19930522)) || (iVar2 == 0x1994000)))))) {
    _terminate();
    pcVar3 = (code *)swi(3);
    uVar4 = (*pcVar3)();
    return uVar4;
  }
  return 0;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_00402855(void)

{
  _DAT_0041ec70 = 0;
  return;
}



// WARNING: Removing unreachable block (ram,0x0040286d)
// WARNING: Removing unreachable block (ram,0x0040286e)
// WARNING: Removing unreachable block (ram,0x00402874)
// WARNING: Removing unreachable block (ram,0x0040287e)
// WARNING: Removing unreachable block (ram,0x00402885)

void FUN_0040285d(void)

{
  return;
}



// WARNING: This is an inlined function
// WARNING: Unable to track spacebase fully for stack
// WARNING: Variable defined which should be unmapped: param_2
// Library Function - Single Match
// Name: __SEH_prolog4
// Libraries: Visual Studio 2015 Debug, Visual Studio 2015 Release

void __cdecl __SEH_prolog4(undefined4 param_1,int param_2)

{
  undefined4 unaff_EBX;
  undefined4 unaff_ESI;
  undefined4 unaff_EDI;
  int *in_FS_OFFSET;
  undefined4 local_res0;
  uint auStack28 [5];
  undefined local_8 [8];
  
  param_2 = -param_2;
  *(undefined4 *)((int)auStack28 + param_2 + 0x10) = unaff_EBX;
  *(undefined4 *)((int)auStack28 + param_2 + 0xc) = unaff_ESI;
  *(undefined4 *)((int)auStack28 + param_2 + 8) = unaff_EDI;
  *(uint *)((int)auStack28 + param_2 + 4) = DAT_0041e004 ^ (uint)&param_2;
  *(uint *)((int)auStack28 + param_2) = local_res0;
  *(undefined **)in_FS_OFFSET = local_8;
  return;
}



// WARNING: This is an inlined function
// Library Function - Single Match
// Name: __SEH_epilog4
// Libraries: Visual Studio 2015 Debug, Visual Studio 2015 Release

void __SEH_epilog4(void)

{
  undefined4 *unaff_EBP;
  undefined4 *in_FS_OFFSET;
  undefined4 unaff_retaddr;
  
  *in_FS_OFFSET = unaff_EBP[-4];
  *unaff_EBP = unaff_retaddr;
  return;
}



// WARNING: Removing unreachable block (ram,0x00402997)
// WARNING: Removing unreachable block (ram,0x0040295c)
// WARNING: Removing unreachable block (ram,0x00402a0f)

undefined4 FUN_0040291b(void)

{
  int *piVar1;
  uint *puVar2;
  int iVar3;
  uint uVar4;
  BOOL BVar5;
  uint uVar6;
  uint uVar7;
  uint in_XCR0;
  
  DAT_0041ec74 = 0;
  DAT_0041e010 = DAT_0041e010 | 1;
  BVar5 = IsProcessorFeaturePresent(10);
  uVar4 = DAT_0041e010;
  if (BVar5 != 0) {
    DAT_0041e010 = DAT_0041e010 | 2;
    DAT_0041ec74 = 1;
    piVar1 = (int *)cpuid_basic_info(0);
    puVar2 = (uint *)cpuid_Version_info(1);
    uVar7 = puVar2[3];
    if (((piVar1[3] ^ 0x6c65746eU | piVar1[2] ^ 0x49656e69U | piVar1[1] ^ 0x756e6547U) == 0) &&
       (((((uVar6 = *puVar2 & 0xfff3ff0, uVar6 == 0x106c0 || (uVar6 == 0x20660)) ||
          (uVar6 == 0x20670)) || ((uVar6 == 0x30650 || (uVar6 == 0x30660)))) || (uVar6 == 0x30670)))
       ) {
      DAT_0041ec78 = DAT_0041ec78 | 1;
    }
    if (*piVar1 < 7) {
      uVar6 = 0;
    }
    else {
      iVar3 = cpuid_Extended_Feature_Enumeration_info(7);
      uVar6 = *(uint *)(iVar3 + 4);
      if ((uVar6 & 0x200) != 0) {
        DAT_0041ec78 = DAT_0041ec78 | 2;
      }
    }
    if ((uVar7 & 0x100000) != 0) {
      DAT_0041e010 = uVar4 | 6;
      DAT_0041ec74 = 2;
      if ((((uVar7 & 0x8000000) != 0) && ((uVar7 & 0x10000000) != 0)) && ((in_XCR0 & 6) == 6)) {
        DAT_0041e010 = uVar4 | 0xe;
        DAT_0041ec74 = 3;
        if ((uVar6 & 0x20) != 0) {
          DAT_0041e010 = uVar4 | 0x2e;
          DAT_0041ec74 = 5;
        }
      }
    }
  }
  return 0;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address
// Library Function - Single Match
// Name: ___scrt_is_ucrt_dll_in_use
// Library: Visual Studio 2015 Release

uint ___scrt_is_ucrt_dll_in_use(void)

{
  return (uint)(_DAT_0041f440 != 0);
}



ushort * __cdecl FUN_00402abe(char *file_name,ushort separator)

{
  ushort uVar1;
  ushort *puVar2;
  int iVar3;
  ushort *puVar4;
  bool bVar5;
  bool bVar6;
  
  puVar4 = (ushort *)0x0;
  puVar2 = (ushort *)file_name;
  if (DAT_0041ec74 < 2) {
    do {
      uVar1 = *puVar2;
      puVar4 = puVar2 + 1;
      puVar2 = puVar4;
    } while (uVar1 != 0);
    do {
      puVar4 = puVar4 + -1;
      if (puVar4 == (ushort *)file_name) break;
    } while (*puVar4 != separator);
    if (*puVar4 != separator) {
      puVar4 = (ushort *)0x0;
    }
  }
  else {
    while (((uint)((int)file_name + 1U) & 0xe) != 0) {
      if (*(ushort *)file_name == separator) {
        puVar4 = (ushort *)file_name;
      }
      if (*(ushort *)file_name == 0) {
        return puVar4;
      }
      file_name = (char *)((ushort *)file_name + 1);
    }
    bVar5 = separator != 0;
    bVar6 = separator == 0;
    if (bVar6) {
      while (iVar3 = pcmpistri(ZEXT416(0xffff0001),*(undefined *)file_name,0x15), !bVar6
                    // WARNING: Load size is inaccurate) {
        file_name = (char *)((ushort *)file_name + 8);
        bVar6 = (ushort *)file_name == (ushort *)0x0;
      }
      puVar4 = (ushort *)file_name + iVar3;
    }
    else {
      while( true ) {
                    // WARNING: Load size is inaccurate
        iVar3 = pcmpistri(ZEXT216(separator),*(undefined *)file_name,0x41);
        if (bVar5) {
          puVar4 = (ushort *)file_name + iVar3;
        }
        if (bVar6) break;
        bVar5 = (ushort *)0xffffffef < file_name;
        file_name = (char *)((ushort *)file_name + 8);
        bVar6 = (ushort *)file_name == (ushort *)0x0;
      }
    }
  }
  return puVar4;
}



ushort * __cdecl FUN_00402b60(ushort *param_1,ushort param_2)

{
  ushort uVar1;
  int iVar2;
  undefined auVar3 [12];
  undefined auVar4 [16];
  uint uVar5;
  undefined auVar6 [16];
  short sVar7;
  short sVar8;
  int iStack52;
  
  if (0 < DAT_0041ec74) {
    auVar6 = pshuflw(ZEXT216(param_2),ZEXT216(param_2),0);
    do {
      if (((uint)param_1 & 0xfff) < 0xff1) {
        auVar3 = *(undefined *)param_1;
                    // WARNING: Load size is inaccurate
        auVar4 = *(undefined *)param_1;
        sVar7 = SUB162(auVar6,0);
        sVar8 = SUB162(auVar6 >> 0x10,0);
        uVar5 = pmovmskb((uint)param_1 & 0xfff,
                         CONCAT214(-(ushort)((short)((uint)*(undefined4 *)(param_1 + 6) >> 0x10) ==
                                            0),
                                   CONCAT212(-(ushort)((short)*(undefined4 *)(param_1 + 6) == 0),
                                             CONCAT210(-(ushort)(SUB122(auVar3 >> 0x50,0) == 0),
                                                       CONCAT28(-(ushort)(SUB122(auVar3 >> 0x40,0)
                                                                         == 0),
                                                                CONCAT26(-(ushort)(SUB122(auVar3 >> 
                                                  0x30,0) == 0),
                                                  CONCAT24(-(ushort)(SUB122(auVar3 >> 0x20,0) == 0),
                                                           CONCAT22(-(ushort)(SUB122(auVar3 >> 0x10,
                                                                                     0) == 0),
                                                                    -(ushort)(SUB122(auVar3,0) == 0)
                                                                   ))))))) |
                         CONCAT214(-(ushort)(SUB162(auVar4 >> 0x70,0) == sVar8),
                                   CONCAT212(-(ushort)(SUB162(auVar4 >> 0x60,0) == sVar7),
                                             CONCAT210(-(ushort)(SUB162(auVar4 >> 0x50,0) == sVar8),
                                                       CONCAT28(-(ushort)(SUB162(auVar4 >> 0x40,0)
                                                                         == sVar7),
                                                                CONCAT26(-(ushort)(SUB162(auVar4 >> 
                                                  0x30,0) == sVar8),
                                                  CONCAT24(-(ushort)(SUB162(auVar4 >> 0x20,0) ==
                                                                    sVar7),
                                                           CONCAT22(-(ushort)(SUB162(auVar4 >> 0x10,
                                                                                     0) == sVar8),
                                                                    -(ushort)(SUB122(auVar3,0) ==
                                                                             sVar7)))))))));
        if (uVar5 != 0) goto LAB_00402c0d;
        iStack52 = 0x10;
      }
      else {
        if (*param_1 == param_2) {
          return param_1;
        }
        if (*param_1 == 0) {
          return (ushort *)0x0;
        }
        iStack52 = 2;
      }
      param_1 = (ushort *)((int)param_1 + iStack52);
    } while( true );
  }
  uVar1 = *param_1;
  while ((uVar1 != 0 && (uVar1 != param_2))) {
    param_1 = param_1 + 1;
    uVar1 = *param_1;
  }
LAB_00402baf:
  return (ushort *)((uint)(uVar1 != param_2) - 1 & (uint)param_1);
LAB_00402c0d:
  iVar2 = 0;
  if (uVar5 != 0) {
    while ((uVar5 >> iVar2 & 1) == 0) {
      iVar2 = iVar2 + 1;
    }
  }
  param_1 = (ushort *)((int)param_1 + iVar2);
  uVar1 = *param_1;
  goto LAB_00402baf;
}



// Library Function - Single Match
// Name: _ValidateLocalCookies
// Library: Visual Studio 2015 Release

void __cdecl _ValidateLocalCookies(int *param_1)

{
  if (*param_1 != -2) {
    FUN_00401f78();
  }
  FUN_00401f78();
  return;
}



DWORD __cdecl FUN_00402c70(PEXCEPTION_RECORD param_1,PVOID param_2,DWORD param_3)

{
  uint uVar1;
  code *pcVar2;
  int iVar3;
  BOOL BVar4;
  DWORD DVar5;
  PVOID pvVar6;
  uint uVar7;
  PEXCEPTION_RECORD pEVar8;
  PEXCEPTION_RECORD local_20;
  DWORD local_1c;
  _EXCEPTION_RECORD *local_18;
  PVOID local_14;
  DWORD local_10;
  int *local_c;
  char local_5;
  
  local_5 = 0;
  pvVar6 = (PVOID)((int)param_2 + 0x10);
  local_c = (int *)(*(uint *)((int)param_2 + 8) ^ DAT_0041e004);
  local_10 = 1;
  local_14 = pvVar6;
  _ValidateLocalCookies(local_c);
  FUN_00403117(param_3);
  uVar7 = *(uint *)((int)param_2 + 0xc);
  if ((*(byte *)&param_1->ExceptionFlags & 0x66) == 0) {
    local_20 = param_1;
    local_1c = param_3;
    *(PEXCEPTION_RECORD *)((int)param_2 + -4) = (PEXCEPTION_RECORD)&local_20;
    if (uVar7 == 0xfffffffe) {
      return local_10;
    }
    do {
      iVar3 = uVar7 * 3 + 4;
      uVar1 = local_c[iVar3];
      local_18 = (_EXCEPTION_RECORD *)(local_c + iVar3);
      if ((undefined *)local_18->ExceptionFlags != (undefined *)0x0) {
        iVar3 = __EH4_CallFilterFunc_8((undefined *)local_18->ExceptionFlags);
        local_5 = 1;
        if (iVar3 < 0) {
          local_10 = 0;
          goto LAB_00402d2d;
        }
        if (0 < iVar3) {
          if ((param_1->ExceptionCode == 0xe06d7363) &&
             (BVar4 = __IsNonwritableInCurrentImage((PBYTE)&PTR_FUN_00417ab4), BVar4 != 0)) {
            pEVar8 = param_1;
            _guard_check_icall();
            FUN_00404e24((int *)pEVar8);
            pvVar6 = local_14;
          }
          FUN_004030e4(param_2,param_1);
          if (*(uint *)((int)param_2 + 0xc) != uVar7) {
            __EH4_LocalUnwind_16((int)param_2,uVar7,pvVar6,&DAT_0041e004);
          }
          *(uint *)((int)param_2 + 0xc) = uVar1;
          _ValidateLocalCookies(local_c);
          __EH4_TransferToHandler_8((undefined *)local_18->ExceptionRecord);
          pcVar2 = (code *)swi(3);
          DVar5 = (*pcVar2)();
          return DVar5;
        }
      }
      uVar7 = uVar1;
    } while (uVar1 != 0xfffffffe);
    if (local_5 == 0) {
      return local_10;
    }
  }
  else {
    if (uVar7 == 0xfffffffe) {
      return local_10;
    }
    __EH4_LocalUnwind_16((int)param_2,0xfffffffe,pvVar6,&DAT_0041e004);
  }
LAB_00402d2d:
  _ValidateLocalCookies(local_c);
  return local_10;
}



// Library Function - Single Match
// Name: ___vcrt_initialize
// Library: Visual Studio 2015 Release

uint ___vcrt_initialize(void)

{
  uint uVar1;
  
  FUN_004035ad();
  FUN_00403552();
  uVar1 = ___vcrt_initialize_locks();
  if ((char)uVar1 != 0) {
    uVar1 = ___vcrt_initialize_ptd();
    if ((char)uVar1 != 0) {
      return CONCAT31((int3)(uVar1 >> 8),1);
    }
    uVar1 = ___vcrt_uninitialize_locks();
  }
  return uVar1 & 0xffffff00;
}



// Library Function - Single Match
// Name: ___vcrt_uninitialize
// Library: Visual Studio 2015 Release

undefined4 ___vcrt_uninitialize(char param_1)

{
  undefined4 in_EAX;
  
  if (param_1 == 0) {
    ___vcrt_uninitialize_ptd();
    ___vcrt_uninitialize_locks();
    in_EAX = ___vcrt_uninitialize_winapi_thunks(0);
  }
  return CONCAT31((int3)((uint)in_EAX >> 8),1);
}



// Library Function - Single Match
// Name: ___std_type_info_compare
// Library: Visual Studio 2015 Release

uint __cdecl ___std_type_info_compare(int param_1,int param_2)

{
  byte bVar1;
  byte *pbVar2;
  byte *pbVar3;
  bool bVar4;
  
  if (param_1 != param_2) {
    pbVar3 = (byte *)(param_2 + 5);
    pbVar2 = (byte *)(param_1 + 5);
    do {
      bVar1 = *pbVar2;
      bVar4 = bVar1 < *pbVar3;
      if (bVar1 != *pbVar3) {
LAB_00402e44:
        return -(uint)bVar4 | 1;
      }
      if (bVar1 == 0) {
        return 0;
      }
      bVar1 = pbVar2[1];
      bVar4 = bVar1 < pbVar3[1];
      if (bVar1 != pbVar3[1]) goto LAB_00402e44;
      pbVar2 = pbVar2 + 2;
      pbVar3 = pbVar3 + 2;
    } while (bVar1 != 0);
  }
  return 0;
}



int * __cdecl FUN_00402e50(int *param_1,byte param_2,uint param_3)

{
  undefined *puVar1;
  int iVar2;
  int *piVar3;
  undefined auVar4 [16];
  
  if (param_3 == 0) {
    return param_1;
  }
  iVar2 = (uint)param_2 * 0x1010101;
  piVar3 = param_1;
  if (0x20 < param_3) {
    piVar3 = param_1;
    if (0x7f < param_3) {
      piVar3 = param_1;
      if ((DAT_0041ec78 >> 1 & 1) != 0) {
        while (param_3 != 0) {
          param_3 = param_3 - 1;
          *(char *)piVar3 = (char)iVar2;
          piVar3 = (int *)((int)piVar3 + 1);
        }
        return param_1;
      }
      piVar3 = param_1;
      if ((DAT_0041e010 >> 1 & 1) == 0) goto joined_r0x00402f5b;
      auVar4 = CONCAT412(iVar2,CONCAT48(iVar2,CONCAT44(iVar2,iVar2)));
      *param_1 = iVar2;
      param_1[1] = iVar2;
      param_1[2] = iVar2;
      param_1[3] = iVar2;
      piVar3 = (int *)((uint)(param_1 + 4) & 0xfffffff0);
      param_3 = (int)param_1 + (param_3 - (int)piVar3);
      if (0x80 < param_3) {
        do {
                    // WARNING: Store size is inaccurate
          *(undefined *)piVar3 = auVar4;
                    // WARNING: Store size is inaccurate
          *(undefined *)(piVar3 + 4) = auVar4;
                    // WARNING: Store size is inaccurate
          *(undefined *)(piVar3 + 8) = auVar4;
                    // WARNING: Store size is inaccurate
          *(undefined *)(piVar3 + 0xc) = auVar4;
                    // WARNING: Store size is inaccurate
          *(undefined *)(piVar3 + 0x10) = auVar4;
                    // WARNING: Store size is inaccurate
          *(undefined *)(piVar3 + 0x14) = auVar4;
                    // WARNING: Store size is inaccurate
          *(undefined *)(piVar3 + 0x18) = auVar4;
                    // WARNING: Store size is inaccurate
          *(undefined *)(piVar3 + 0x1c) = auVar4;
          piVar3 = piVar3 + 0x20;
          param_3 = param_3 - 0x80;
        } while ((param_3 & 0xffffff00) != 0);
        goto LAB_00402f20;
      }
    }
    if ((DAT_0041e010 >> 1 & 1) != 0) {
      auVar4 = CONCAT412(iVar2,CONCAT48(iVar2,CONCAT44(iVar2,iVar2)));
LAB_00402f20:
      if (0x1f < param_3) {
        do {
                    // WARNING: Store size is inaccurate
          *(undefined *)piVar3 = auVar4;
                    // WARNING: Store size is inaccurate
          *(undefined *)(piVar3 + 4) = auVar4;
          piVar3 = piVar3 + 8;
          param_3 = param_3 - 0x20;
        } while (0x1f < param_3);
        if ((param_3 & 0x1f) == 0) {
          return param_1;
        }
      }
      puVar1 = (undefined *)((int)piVar3 + (param_3 - 0x20));
                    // WARNING: Store size is inaccurate
      *(undefined *)puVar1 = auVar4;
                    // WARNING: Store size is inaccurate
      *(undefined *)(puVar1 + 0x10) = auVar4;
      return param_1;
    }
  }
joined_r0x00402f5b:
  while ((param_3 & 3) != 0) {
    *(char *)piVar3 = (char)iVar2;
    param_3 = param_3 - 1;
    piVar3 = (int *)((int)piVar3 + 1);
  }
  if ((param_3 & 4) != 0) {
    *piVar3 = iVar2;
    piVar3 = piVar3 + 1;
    param_3 = param_3 - 4;
  }
  while ((param_3 & 0xfffffff8) != 0) {
    *piVar3 = iVar2;
    piVar3[1] = iVar2;
    piVar3 = piVar3 + 2;
    param_3 = param_3 - 8;
  }
  return param_1;
}



// Library Function - Single Match
// Name: __local_unwind4
// Library: Visual Studio

void __cdecl __local_unwind4(uint *param_1,int param_2,uint param_3)

{
  undefined4 *puVar1;
  uint uVar2;
  undefined4 *in_FS_OFFSET;
  undefined4 uStack40;
  undefined *puStack36;
  uint local_20;
  uint uStack28;
  int iStack24;
  uint *puStack20;
  
  puStack20 = param_1;
  iStack24 = param_2;
  uStack28 = param_3;
  puStack36 = &LAB_00403040;
  uStack40 = *in_FS_OFFSET;
  local_20 = DAT_0041e004 ^ (uint)&uStack40;
  *(undefined4 **)in_FS_OFFSET = &uStack40;
  while( true ) {
    uVar2 = *(uint *)(param_2 + 0xc);
    if ((uVar2 == 0xfffffffe) || ((param_3 != 0xfffffffe && (uVar2 <= param_3)))) break;
    puVar1 = (undefined4 *)((*(uint *)(param_2 + 8) ^ *param_1) + 0x10 + uVar2 * 0xc);
    *(undefined4 *)(param_2 + 0xc) = *puVar1;
    if (puVar1[1] == 0) {
      __NLG_Notify(0x101);
      FUN_004036fb();
    }
  }
  *in_FS_OFFSET = uStack40;
  return;
}



// Library Function - Single Match
// Name: @_EH4_CallFilterFunc@8
// Library: Visual Studio

void __fastcall __EH4_CallFilterFunc_8(undefined *param_1)

{
  (*(code *)param_1)();
  return;
}



// Library Function - Single Match
// Name: @_EH4_TransferToHandler@8
// Library: Visual Studio

void __fastcall __EH4_TransferToHandler_8(undefined *param_1)

{
  __NLG_Notify(1);
                    // WARNING: Could not recover jumptable at 0x004030df. Too many branches
                    // WARNING: Treating indirect jump as call
  (*(code *)param_1)();
  return;
}



void __fastcall FUN_004030e4(PVOID param_1,PEXCEPTION_RECORD param_2)

{
  RtlUnwind(param_1,(PVOID)0x4030f9,param_2,(PVOID)0x0);
  return;
}



// Library Function - Single Match
// Name: @_EH4_LocalUnwind@16
// Library: Visual Studio

void __fastcall __EH4_LocalUnwind_16(int param_1,uint param_2,undefined4 param_3,uint *param_4)

{
  __local_unwind4(param_4,param_1,param_2);
  return;
}



// WARNING: Removing unreachable block (ram,0x00403126)
// WARNING: Removing unreachable block (ram,0x0040313b)
// WARNING: Removing unreachable block (ram,0x00403140)

void __cdecl FUN_00403117(int param_1)

{
  return;
}



// WARNING: Removing unreachable block (ram,0x00403156)
// WARNING: Removing unreachable block (ram,0x00403168)
// WARNING: Removing unreachable block (ram,0x0040316d)

void __cdecl FUN_00403147(int param_1)

{
  return;
}



void FUN_00403174(undefined *param_1)

{
  if ((param_1 != (undefined *)0x0) && (param_1 != &DAT_0041ec7c)) {
    free(param_1);
  }
  return;
}



// Library Function - Single Match
// Name: ___vcrt_getptd
// Library: Visual Studio 2015 Release

void ___vcrt_getptd(void)

{
  code *pcVar1;
  void *pvVar2;
  int iVar3;
  BOOL BVar4;
  
  pvVar2 = ___vcrt_getptd_noexit();
  if (pvVar2 != (void *)0x0) {
    return;
  }
  iVar3 = FUN_0040f2dd();
  if (iVar3 != 0) {
    FUN_0040f32d(0x16);
  }
  if ((DAT_0041e0f0 & 2) != 0) {
    BVar4 = IsProcessorFeaturePresent(0x17);
    if (BVar4 != 0) {
      pcVar1 = (code *)swi(0x29);
      (*pcVar1)();
      return;
    }
    ___acrt_call_reportfault(3,0x40000015,1);
  }
  FUN_004097d4(3);
  pcVar1 = (code *)swi(3);
  (*pcVar1)();
  return;
}



// Library Function - Single Match
// Name: ___vcrt_getptd_noexit
// Library: Visual Studio 2015 Release

void * ___vcrt_getptd_noexit(void)

{
  DWORD dwErrCode;
  int iVar1;
  void *pvVar2;
  void *pvVar3;
  void *pvVar4;
  
  if (DAT_0041e020 == 0xffffffff) {
    return (void *)0x0;
  }
  dwErrCode = GetLastError();
  pvVar3 = (void *)FUN_00403492(DAT_0041e020);
  if (pvVar3 == (void *)0xffffffff) {
LAB_004031de:
    pvVar3 = (void *)0x0;
    goto LAB_00403224;
  }
  if (pvVar3 != (void *)0x0) goto LAB_00403224;
  iVar1 = FUN_004034cd(DAT_0041e020,(LPVOID)0xffffffff);
  if (iVar1 == 0) goto LAB_004031de;
  pvVar2 = _calloc(1,0x28);
  if (pvVar2 == (void *)0x0) {
LAB_00403206:
    FUN_004034cd(DAT_0041e020,(LPVOID)0x0);
    pvVar3 = (void *)0x0;
    pvVar4 = pvVar2;
  }
  else {
    iVar1 = FUN_004034cd(DAT_0041e020,pvVar2);
    if (iVar1 == 0) goto LAB_00403206;
    pvVar4 = (void *)0x0;
    pvVar3 = pvVar2;
  }
  free(pvVar4);
LAB_00403224:
  SetLastError(dwErrCode);
  return pvVar3;
}



// Library Function - Single Match
// Name: ___vcrt_initialize_ptd
// Library: Visual Studio 2015 Release

uint ___vcrt_initialize_ptd(void)

{
  uint uVar1;
  int iVar2;
  
  uVar1 = FUN_0040341c(FUN_00403174);
  DAT_0041e020 = uVar1;
  if (uVar1 != 0xffffffff) {
    DAT_0041e020 = uVar1;
    iVar2 = FUN_004034cd(uVar1,&DAT_0041ec7c);
    if (iVar2 != 0) {
      return CONCAT31((int3)((uint)iVar2 >> 8),1);
    }
    uVar1 = ___vcrt_uninitialize_ptd();
  }
  return uVar1 & 0xffffff00;
}



// Library Function - Single Match
// Name: ___vcrt_uninitialize_ptd
// Library: Visual Studio 2015 Release

undefined4 ___vcrt_uninitialize_ptd(void)

{
  DWORD DVar1;
  
  DVar1 = DAT_0041e020;
  if (DAT_0041e020 != 0xffffffff) {
    DVar1 = FUN_00403457(DAT_0041e020);
    DAT_0041e020 = 0xffffffff;
  }
  return CONCAT31((int3)(DVar1 >> 8),1);
}



// Library Function - Single Match
// Name: ___vcrt_initialize_locks
// Library: Visual Studio 2015 Release

uint ___vcrt_initialize_locks(void)

{
  int iVar1;
  uint uVar2;
  LPCRITICAL_SECTION p_Var3;
  
  p_Var3 = (LPCRITICAL_SECTION)&DAT_0041eca4;
  uVar2 = 0;
  do {
    iVar1 = FUN_0040350b(p_Var3,4000,0);
    if (iVar1 == 0) {
      uVar2 = ___vcrt_uninitialize_locks();
      return uVar2 & 0xffffff00;
    }
    DAT_0041ecbc = DAT_0041ecbc + 1;
    uVar2 = uVar2 + 0x18;
    p_Var3 = p_Var3 + 1;
  } while (uVar2 < 0x18);
  return CONCAT31((int3)((uint)iVar1 >> 8),1);
}



// Library Function - Single Match
// Name: ___vcrt_uninitialize_locks
// Library: Visual Studio 2015 Release

undefined4 ___vcrt_uninitialize_locks(void)

{
  undefined4 in_EAX;
  undefined4 extraout_EAX;
  int iVar1;
  LPCRITICAL_SECTION lpCriticalSection;
  
  if (DAT_0041ecbc != 0) {
    lpCriticalSection = (LPCRITICAL_SECTION)(&DAT_0041ec8c + DAT_0041ecbc * 0x18);
    iVar1 = DAT_0041ecbc;
    do {
      DeleteCriticalSection(lpCriticalSection);
      DAT_0041ecbc = DAT_0041ecbc + -1;
      lpCriticalSection = (LPCRITICAL_SECTION)&lpCriticalSection[-1].LockSemaphore;
      iVar1 = iVar1 + -1;
      in_EAX = extraout_EAX;
    } while (iVar1 != 0);
  }
  return CONCAT31((int3)((uint)in_EAX >> 8),1);
}



HMODULE __cdecl FUN_004032e9(int *param_1,int *param_2)

{
  HMODULE *ppHVar1;
  LPCWSTR lpLibFileName;
  HMODULE pHVar2;
  HMODULE hLibModule;
  DWORD DVar3;
  int iVar4;
  
  do {
    if (param_1 == param_2) {
      return (HMODULE)0x0;
    }
    ppHVar1 = &hLibModule_0041ecc0 + *param_1;
    hLibModule = *ppHVar1;
    if (hLibModule == (HMODULE)0x0) {
      lpLibFileName = (LPCWSTR)(&PTR_u_api_ms_win_core_fibers_l1_1_1_004171b8)[*param_1];
      hLibModule = LoadLibraryExW(lpLibFileName,(HANDLE)0x0,0x800);
      if (hLibModule == (HMODULE)0x0) {
        DVar3 = GetLastError();
        if (((DVar3 == 0x57) && (iVar4 = _wcsncmp(lpLibFileName,L"api-ms-",7), iVar4 != 0)) &&
           (iVar4 = _wcsncmp(lpLibFileName,L"ext-ms-",7), iVar4 != 0)) {
          hLibModule = LoadLibraryExW(lpLibFileName,(HANDLE)0x0,0);
        }
        else {
          hLibModule = (HMODULE)0x0;
        }
        if (hLibModule == (HMODULE)0x0) {
          *ppHVar1 = (HMODULE)0xffffffff;
          goto LAB_00403396;
        }
      }
      pHVar2 = *ppHVar1;
      *ppHVar1 = hLibModule;
      if (pHVar2 != (HMODULE)0x0) {
        FreeLibrary(hLibModule);
      }
LAB_00403392:
      if (hLibModule != (HMODULE)0x0) {
        return hLibModule;
      }
    }
    else {
      if (hLibModule != (HMODULE)0xffffffff) goto LAB_00403392;
    }
LAB_00403396:
    param_1 = param_1 + 1;
  } while( true );
}



FARPROC __cdecl FUN_004033ad(int param_1,LPCSTR param_2,int *param_3,int *param_4)

{
  uint *puVar1;
  FARPROC pFVar2;
  HMODULE hModule;
  uint uVar3;
  byte bVar4;
  
  puVar1 = &DAT_0041eccc + param_1;
  bVar4 = (byte)DAT_0041e004 & 0x1f;
  pFVar2 = (FARPROC)((DAT_0041e004 ^ *puVar1) >> bVar4 | (DAT_0041e004 ^ *puVar1) << 0x20 - bVar4);
  if (pFVar2 == (FARPROC)0xffffffff) {
    pFVar2 = (FARPROC)0x0;
  }
  else {
    if (pFVar2 == (FARPROC)0x0) {
      hModule = FUN_004032e9(param_3,param_4);
      if ((hModule == (HMODULE)0x0) ||
         (pFVar2 = GetProcAddress(hModule,param_2), pFVar2 == (FARPROC)0x0)) {
        uVar3 = FUN_004022fb(0xffffffff);
        *puVar1 = uVar3;
        pFVar2 = (FARPROC)0x0;
      }
      else {
        uVar3 = FUN_004022fb((uint)pFVar2);
        *puVar1 = uVar3;
      }
    }
  }
  return pFVar2;
}



void __cdecl FUN_0040341c(undefined4 param_1)

{
  FARPROC pFVar1;
  
  pFVar1 = FUN_004033ad(0,"FlsAlloc",(int *)&DAT_00417270,(int *)"FlsAlloc");
  if (pFVar1 != (FARPROC)0x0) {
    _guard_check_icall();
    (*pFVar1)(param_1);
    return;
  }
                    // WARNING: Could not recover jumptable at 0x00403451. Too many branches
                    // WARNING: Treating indirect jump as call
  TlsAlloc();
  return;
}



void __cdecl FUN_00403457(DWORD param_1)

{
  FARPROC pFVar1;
  
  pFVar1 = FUN_004033ad(1,"FlsFree",(int *)&DAT_00417284,(int *)"FlsFree");
  if (pFVar1 == (FARPROC)0x0) {
    TlsFree(param_1);
  }
  else {
    _guard_check_icall();
    (*pFVar1)();
  }
  return;
}



void __cdecl FUN_00403492(DWORD param_1)

{
  FARPROC pFVar1;
  
  pFVar1 = FUN_004033ad(2,"FlsGetValue",(int *)&DAT_00417294,(int *)"FlsGetValue");
  if (pFVar1 == (FARPROC)0x0) {
    TlsGetValue(param_1);
  }
  else {
    _guard_check_icall();
    (*pFVar1)();
  }
  return;
}



void __cdecl FUN_004034cd(DWORD param_1,LPVOID param_2)

{
  FARPROC pFVar1;
  
  pFVar1 = FUN_004033ad(3,"FlsSetValue",(int *)&DAT_004172a8,(int *)"FlsSetValue");
  if (pFVar1 == (FARPROC)0x0) {
    TlsSetValue(param_1,param_2);
  }
  else {
    _guard_check_icall();
    (*pFVar1)();
  }
  return;
}



void __cdecl FUN_0040350b(LPCRITICAL_SECTION param_1,DWORD param_2,undefined4 param_3)

{
  FARPROC pFVar1;
  
  pFVar1 = FUN_004033ad(4,"InitializeCriticalSectionEx",(int *)&DAT_004172bc,
                        (int *)"InitializeCriticalSectionEx");
  if (pFVar1 == (FARPROC)0x0) {
    InitializeCriticalSectionAndSpinCount(param_1,param_2);
  }
  else {
    _guard_check_icall();
    (*pFVar1)(param_1,param_2,param_3);
  }
  return;
}



void FUN_00403552(void)

{
  undefined4 uVar1;
  undefined4 *puVar2;
  int iVar3;
  
  uVar1 = DAT_0041e004;
  puVar2 = &DAT_0041eccc;
  iVar3 = 0;
  do {
    iVar3 = iVar3 + 1;
    *puVar2 = uVar1;
    puVar2 = puVar2 + 1;
  } while (iVar3 != 5);
  return;
}



// Library Function - Single Match
// Name: ___vcrt_uninitialize_winapi_thunks
// Library: Visual Studio 2015 Release

void __cdecl ___vcrt_uninitialize_winapi_thunks(char param_1)

{
  HMODULE *ppHVar1;
  
  if (param_1 == 0) {
    ppHVar1 = &hLibModule_0041ecc0;
    do {
      if (*ppHVar1 != (HMODULE)0x0) {
        if (*ppHVar1 != (HMODULE)0xffffffff) {
          FreeLibrary(*ppHVar1);
        }
        *ppHVar1 = (HMODULE)0x0;
      }
      ppHVar1 = ppHVar1 + 1;
    } while (ppHVar1 != (HMODULE *)&DAT_0041eccc);
  }
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_004035ad(void)

{
  _DAT_0041ece0 = DAT_0041e004;
  return;
}



void __cdecl FUN_004035c0(PVOID param_1)

{
  RtlUnwind(param_1,(PVOID)0x4035d9,(PEXCEPTION_RECORD)0x0,(PVOID)0x0);
  return;
}



// Library Function - Single Match
// Name: __local_unwind2
// Library: Visual Studio

void __cdecl __local_unwind2(int param_1,uint param_2)

{
  uint uVar1;
  uint uVar2;
  undefined4 *in_FS_OFFSET;
  undefined4 local_20;
  undefined *puStack28;
  undefined4 local_18;
  int iStack20;
  
  iStack20 = param_1;
  puStack28 = &LAB_004035e1;
  local_20 = *in_FS_OFFSET;
  uVar2 = DAT_0041e004 ^ (uint)&local_20;
  *(undefined4 **)in_FS_OFFSET = &local_20;
  while( true ) {
    uVar1 = *(uint *)(param_1 + 0xc);
    if ((uVar1 == 0xffffffff) || ((param_2 != 0xffffffff && (uVar1 <= param_2)))) break;
    local_18 = *(undefined4 *)(*(int *)(param_1 + 8) + uVar1 * 0xc);
    *(undefined4 *)(param_1 + 0xc) = local_18;
    if (*(int *)(*(int *)(param_1 + 8) + 4 + uVar1 * 0xc) == 0) {
      __NLG_Notify(0x101);
      FUN_004036fb(uVar2);
    }
  }
  *in_FS_OFFSET = local_20;
  return;
}



// Library Function - Single Match
// Name: __NLG_Notify1
// Library: Visual Studio

undefined4 __fastcall __NLG_Notify1(undefined4 uParm1)

{
  undefined4 in_EAX;
  undefined4 unaff_EBP;
  
  DAT_0041e034 = in_EAX;
  DAT_0041e038 = uParm1;
  DAT_0041e03c = unaff_EBP;
  return in_EAX;
}



// Library Function - Single Match
// Name: __NLG_Notify
// Library: Visual Studio

void __NLG_Notify(ulong param_1)

{
  undefined4 in_EAX;
  undefined4 unaff_EBP;
  
  DAT_0041e034 = in_EAX;
  DAT_0041e038 = param_1;
  DAT_0041e03c = unaff_EBP;
  return;
}



void FUN_004036fb(void)

{
  code *in_EAX;
  
  (*in_EAX)();
  return;
}



void __cdecl FUN_004036fe(int **param_1,int *param_2,int param_3,int param_4)

{
  int *piVar1;
  code *pcVar2;
  int *piVar3;
  int *piVar4;
  int *piVar5;
  int *local_8;
  
  piVar1 = *(int **)(*param_2 + 0xc);
  piVar4 = piVar1;
  piVar5 = piVar1;
  if (-1 < param_4) {
    piVar3 = (int *)((int)piVar1 * 0x14 + *(int *)(*param_2 + 0x10) + 8);
    piVar4 = piVar1;
    piVar5 = piVar1;
    local_8 = piVar1;
    do {
      if (piVar4 == (int *)0xffffffff) goto LAB_0040377a;
      piVar4 = (int *)((int)piVar4 - 1);
      if (((piVar3[-6] < param_3) && (param_3 <= piVar3[-5])) || (piVar4 == (int *)0xffffffff)) {
        param_4 = param_4 + -1;
        piVar5 = local_8;
        local_8 = piVar4;
      }
      piVar3 = piVar3 + -5;
    } while (-1 < param_4);
  }
  if ((piVar5 <= piVar1) && ((int *)((int)piVar4 + 1U) <= piVar5)) {
    *param_1 = param_2;
    param_1[2] = param_2;
    param_1[1] = (int *)((int)piVar4 + 1U);
    param_1[3] = piVar5;
    return;
  }
LAB_0040377a:
  _terminate();
  pcVar2 = (code *)swi(3);
  (*pcVar2)();
  return;
}



undefined4 __cdecl
FUN_00403780(undefined4 param_1,undefined4 param_2,undefined4 param_3,int param_4,int param_5)

{
  undefined4 uVar1;
  int **in_FS_OFFSET;
  int *local_1c;
  code *local_18;
  uint local_14;
  undefined4 local_10;
  undefined4 local_c;
  int local_8;
  
  local_14 = DAT_0041e004 ^ (uint)&local_1c;
  local_10 = param_2;
  local_8 = param_4 + 1;
  local_18 = FID_conflict_CatchGuardHandler;
  local_c = param_1;
  local_1c = *in_FS_OFFSET;
  *(int ***)in_FS_OFFSET = &local_1c;
  uVar1 = __CallSettingFrame_12(param_3,param_1,param_5);
  *in_FS_OFFSET = local_1c;
  return uVar1;
}



undefined4 __cdecl
FUN_004037de(undefined4 *param_1,undefined4 *param_2,undefined4 param_3,undefined4 param_4,
            undefined4 param_5,undefined4 param_6,undefined4 param_7)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 **in_FS_OFFSET;
  undefined4 *local_3c;
  code *local_38;
  uint local_34;
  undefined4 local_30;
  undefined4 *local_2c;
  undefined4 local_28;
  undefined4 local_24;
  undefined *local_20;
  undefined *local_1c;
  int local_18;
  undefined4 *local_14;
  undefined4 local_10;
  undefined4 local_c;
  code *local_8;
  
  local_1c = &stack0xfffffffc;
  local_20 = &stack0xffffffc0;
  if (param_1 == (undefined4 *)0x123) {
    *param_2 = 0x40388d;
    uVar1 = 1;
  }
  else {
    local_38 = TranslatorGuardHandler;
    local_34 = DAT_0041e004 ^ (uint)&local_3c;
    local_30 = param_5;
    local_2c = param_2;
    local_28 = param_6;
    local_24 = param_7;
    local_18 = 0;
    local_3c = *in_FS_OFFSET;
    *(undefined4 ***)in_FS_OFFSET = &local_3c;
    local_c = 1;
    local_14 = param_1;
    local_10 = param_3;
    iVar2 = ___vcrt_getptd();
    local_8 = *(code **)(iVar2 + 8);
    _guard_check_icall();
    (*local_8)(*param_1,&local_14);
    if (local_18 == 0) {
      *in_FS_OFFSET = local_3c;
    }
    else {
      *local_3c = **in_FS_OFFSET;
      *in_FS_OFFSET = local_3c;
    }
    uVar1 = 0;
  }
  return uVar1;
}



// Library Function - Single Match
// Name: ?_JumpToContinuation@@YGXPAXPAUEHRegistrationNode@@@Z
// Library: Visual Studio
// void __stdcall _JumpToContinuation(void *,struct EHRegistrationNode *)

void _JumpToContinuation(void *param_1,EHRegistrationNode *param_2)

{
  undefined4 *in_FS_OFFSET;
  
  *in_FS_OFFSET = *(undefined4 *)*in_FS_OFFSET;
                    // WARNING: Could not recover jumptable at 0x004038e2. Too many branches
                    // WARNING: Treating indirect jump as call
  (*(code *)param_1)();
  return;
}



// Library Function - Single Match
// Name: ?_UnwindNestedFrames@@YGXPAUEHRegistrationNode@@PAUEHExceptionRecord@@@Z
// Library: Visual Studio 2012 Release
// void __stdcall _UnwindNestedFrames(struct EHRegistrationNode *,struct EHExceptionRecord *)

void _UnwindNestedFrames(EHRegistrationNode *param_1,EHExceptionRecord *param_2)

{
  undefined4 *puVar1;
  undefined4 *in_FS_OFFSET;
  
  puVar1 = (undefined4 *)*in_FS_OFFSET;
  RtlUnwind(param_1,(PVOID)0x403913,(PEXCEPTION_RECORD)param_2,(PVOID)0x0);
  *(uint *)(param_2 + 4) = *(uint *)(param_2 + 4) & 0xfffffffd;
  *puVar1 = *in_FS_OFFSET;
  *(undefined4 **)in_FS_OFFSET = puVar1;
  return;
}



// Library Function - Multiple Matches With Different Base Names
// Name: 
?CatchGuardHandler@@YA?AW4_EXCEPTION_DISPOSITION@@PAUEHExceptionRecord@@PAUCatchGuardRN@@PAX2@Z,
// __CatchGuardHandler
// Libraries: Visual Studio 2008 Release, Visual Studio 2010 Release, Visual Studio 2012 Release,
// Visual Studio 2015 Release

void __cdecl FID_conflict_CatchGuardHandler(int *param_1,undefined8 *param_2,int param_3)

{
  FUN_00401f78();
  FUN_004047a9(param_1,*(undefined8 **)(param_2 + 2),param_3,0,*(uint **)((int)param_2 + 0xc),
               *(int *)((int)param_2 + 0x14),param_2,0);
  return;
}



// Library Function - Single Match
// Name: __CreateFrameInfo
// Library: Visual Studio 2015 Release

undefined4 * __cdecl __CreateFrameInfo(undefined4 *param_1,undefined4 param_2)

{
  int iVar1;
  
  *param_1 = param_2;
  iVar1 = ___vcrt_getptd();
  param_1[1] = *(undefined4 *)(iVar1 + 0x24);
  iVar1 = ___vcrt_getptd();
  *(undefined4 **)(iVar1 + 0x24) = param_1;
  return param_1;
}



void __cdecl FUN_00403991(int param_1)

{
  code *pcVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  
  iVar2 = ___vcrt_getptd();
  iVar2 = *(int *)(iVar2 + 0x24);
  iVar3 = ___vcrt_getptd();
  if (param_1 == iVar2) {
    *(undefined4 *)(iVar3 + 0x24) = *(undefined4 *)(param_1 + 4);
    return;
  }
  iVar2 = *(int *)(iVar3 + 0x24);
  do {
    piVar4 = (int *)(iVar2 + 4);
    iVar2 = *piVar4;
    if (iVar2 == 0) {
      _terminate();
      pcVar1 = (code *)swi(3);
      (*pcVar1)();
      return;
    }
  } while (param_1 != iVar2);
  *piVar4 = *(int *)(param_1 + 4);
  return;
}



// Library Function - Single Match
// Name: 
?TranslatorGuardHandler@@YA?AW4_EXCEPTION_DISPOSITION@@PAUEHExceptionRecord@@PAUTranslatorGuardRN@@PAX2@Z
// Library: Visual Studio 2012 Release
// enum _EXCEPTION_DISPOSITION __cdecl TranslatorGuardHandler(struct EHExceptionRecord *,struct
// TranslatorGuardRN *,void *,void *)

_EXCEPTION_DISPOSITION __cdecl
TranslatorGuardHandler
          (EHExceptionRecord *param_1,TranslatorGuardRN *param_2,void *param_3,void *param_4)

{
  _EXCEPTION_DISPOSITION _Var1;
  code *UNRECOVERED_JUMPTABLE;
  
  FUN_00401f78();
  if ((*(uint *)(param_1 + 4) & 0x66) != 0) {
    *(undefined4 *)(param_2 + 0x24) = 1;
    return 1;
  }
  FUN_004047a9((int *)param_1,*(undefined8 **)(param_2 + 0x10),(int)param_3,0,
               *(uint **)(param_2 + 0xc),*(int *)(param_2 + 0x14),*(undefined8 **)(param_2 + 0x18),1
              );
  if (*(int *)(param_2 + 0x24) == 0) {
    _UnwindNestedFrames((EHRegistrationNode *)param_2,param_1);
  }
  FUN_004037de((undefined4 *)0x123,&UNRECOVERED_JUMPTABLE,0,0,0,0,0);
                    // WARNING: Could not recover jumptable at 0x00403a6b. Too many branches
                    // WARNING: Treating indirect jump as call
  _Var1 = (*UNRECOVERED_JUMPTABLE)();
  return _Var1;
}



// Library Function - Multiple Matches With Different Base Names
// Name: ___CxxFrameHandler, ___CxxFrameHandler2, ___CxxFrameHandler3
// Library: Visual Studio

undefined4
FID_conflict____CxxFrameHandler3(int *param_1,undefined8 *param_2,int param_3,undefined4 param_4)

{
  uint *in_EAX;
  undefined4 uVar1;
  
  uVar1 = FUN_004047a9(param_1,param_2,param_3,param_4,in_EAX,0,(undefined8 *)0x0,0);
  return uVar1;
}



ulonglong __cdecl copy_buffer(undefined8 *param_1,undefined8 *param_2,uint param_3)

{
  undefined8 uVar1;
  undefined auVar2 [16];
  undefined auVar3 [16];
  undefined auVar4 [16];
  undefined auVar5 [16];
  undefined auVar6 [16];
  undefined auVar7 [16];
  undefined4 uVar8;
  int iVar9;
  uint uVar10;
  uint uVar11;
  int iVar12;
  undefined8 *puVar13;
  undefined4 *puVar14;
  undefined4 *puVar15;
  undefined4 *puVar16;
  undefined4 *puVar17;
  undefined8 *puVar18;
  undefined auVar19 [16];
  ulonglong uVar20;
  
  if ((param_2 < param_1) && (param_1 < (undefined8 *)(param_3 + (int)param_2))) {
    puVar14 = (undefined4 *)((int)param_2 + param_3);
    puVar16 = (undefined4 *)((int)param_1 + param_3);
    uVar10 = param_3;
    uVar11 = param_3;
    if (0x1f < param_3) {
      uVar10 = param_3;
      if ((DAT_0041e010 >> 1 & 1) == 0) {
        uVar11 = param_3;
        if (((uint)puVar16 & 3) != 0) {
          uVar11 = (uint)puVar16 & 3;
          param_3 = param_3 - uVar11;
          do {
            *(undefined *)((int)puVar16 - 1) = *(undefined *)((int)puVar14 + -1);
            puVar14 = (undefined4 *)((int)puVar14 + -1);
            puVar16 = (undefined4 *)((int)puVar16 - 1);
            uVar11 = uVar11 - 1;
          } while (uVar11 != 0);
        }
        uVar10 = param_3;
        if (0x1f < param_3) {
          uVar10 = param_3 >> 2;
          while( true ) {
            if (uVar10 == 0) break;
            uVar10 = uVar10 - 1;
            puVar16[-1] = puVar14[-1];
            puVar14 = puVar14 + -1;
            puVar16 = puVar16 + -1;
          }
          switch(param_3 & 3) {
          case 0:
            return CONCAT44(param_3,param_1) & 0x3ffffffff;
          case 1:
            *(undefined *)((int)puVar16 - 1) = *(undefined *)((int)puVar14 + -1);
            return CONCAT44(param_3,param_1) & 0x3ffffffff;
          case 2:
            *(undefined *)((int)puVar16 - 1) = *(undefined *)((int)puVar14 + -1);
            *(undefined *)((int)puVar16 - 2) = *(undefined *)((int)puVar14 + -2);
            return CONCAT44(param_3,param_1) & 0x3ffffffff;
          case 3:
            *(undefined *)((int)puVar16 - 1) = *(undefined *)((int)puVar14 + -1);
            *(undefined *)((int)puVar16 - 2) = *(undefined *)((int)puVar14 + -2);
            *(undefined *)((int)puVar16 - 3) = *(undefined *)((int)puVar14 + -3);
            return CONCAT44(param_3,param_1) & 0x3ffffffff;
          }
        }
      }
      else {
        while (puVar15 = puVar14, puVar17 = puVar16, ((uint)puVar16 & 0xf) != 0) {
          puVar14 = (undefined4 *)((int)puVar14 + -1);
          puVar16 = (undefined4 *)((int)puVar16 + -1);
          *(undefined *)puVar16 = *(undefined *)puVar14;
          uVar10 = uVar10 - 1;
        }
        do {
          puVar14 = puVar15;
          puVar16 = puVar17;
          if (uVar10 < 0x80) break;
          puVar14 = puVar15 + -0x20;
          puVar16 = puVar17 + -0x20;
                    // WARNING: Load size is inaccurate
                    // WARNING: Load size is inaccurate
          auVar19 = *(undefined *)(puVar15 + -0x1c);
                    // WARNING: Load size is inaccurate
          auVar2 = *(undefined *)(puVar15 + -0x18);
                    // WARNING: Load size is inaccurate
          auVar3 = *(undefined *)(puVar15 + -0x14);
                    // WARNING: Load size is inaccurate
          auVar4 = *(undefined *)(puVar15 + -0x10);
                    // WARNING: Load size is inaccurate
          auVar5 = *(undefined *)(puVar15 + -0xc);
                    // WARNING: Load size is inaccurate
          auVar6 = *(undefined *)(puVar15 + -8);
                    // WARNING: Load size is inaccurate
          auVar7 = *(undefined *)(puVar15 + -4);
                    // WARNING: Store size is inaccurate
          *(undefined *)puVar16 = *(undefined *)puVar14;
                    // WARNING: Store size is inaccurate
          *(undefined *)(puVar17 + -0x1c) = auVar19;
                    // WARNING: Store size is inaccurate
          *(undefined *)(puVar17 + -0x18) = auVar2;
                    // WARNING: Store size is inaccurate
          *(undefined *)(puVar17 + -0x14) = auVar3;
                    // WARNING: Store size is inaccurate
          *(undefined *)(puVar17 + -0x10) = auVar4;
                    // WARNING: Store size is inaccurate
          *(undefined *)(puVar17 + -0xc) = auVar5;
                    // WARNING: Store size is inaccurate
          *(undefined *)(puVar17 + -8) = auVar6;
                    // WARNING: Store size is inaccurate
          *(undefined *)(puVar17 + -4) = auVar7;
          uVar10 = uVar10 - 0x80;
          puVar15 = puVar14;
          puVar17 = puVar16;
        } while ((uVar10 & 0xffffff80) != 0);
        puVar15 = puVar14;
        puVar17 = puVar16;
        uVar11 = param_3;
        if (0x1f < uVar10) {
          do {
            puVar14 = puVar15 + -8;
            puVar16 = puVar17 + -8;
                    // WARNING: Load size is inaccurate
                    // WARNING: Load size is inaccurate
            auVar19 = *(undefined *)(puVar15 + -4);
                    // WARNING: Store size is inaccurate
            *(undefined *)puVar16 = *(undefined *)puVar14;
                    // WARNING: Store size is inaccurate
            *(undefined *)(puVar17 + -4) = auVar19;
            uVar10 = uVar10 - 0x20;
            puVar15 = puVar14;
            puVar17 = puVar16;
            uVar11 = param_3;
          } while ((uVar10 & 0xffffffe0) != 0);
        }
      }
    }
    while ((uVar10 & 0xfffffffc) != 0) {
      puVar16 = puVar16 + -1;
      puVar14 = puVar14 + -1;
      *puVar16 = *puVar14;
      uVar10 = uVar10 - 4;
    }
    while (uVar10 != 0) {
      puVar16 = (undefined4 *)((int)puVar16 - 1);
      puVar14 = (undefined4 *)((int)puVar14 + -1);
      *(undefined *)puVar16 = *(undefined *)puVar14;
      uVar10 = uVar10 - 1;
    }
    return CONCAT44(uVar11,param_1);
  }
  uVar10 = param_3;
  puVar18 = param_1;
  if (0x1f < param_3) {
    if (param_3 < 0x80) {
      uVar10 = param_3;
      puVar18 = param_1;
      if ((DAT_0041e010 >> 1 & 1) != 0) {
LAB_00403f7d:
        if (uVar10 == 0) goto LAB_00403fe0;
        param_3 = uVar10 >> 5;
        while (param_3 != 0) {
                    // WARNING: Load size is inaccurate
                    // WARNING: Load size is inaccurate
          auVar19 = *(undefined *)(param_2 + 2);
                    // WARNING: Store size is inaccurate
          *(undefined *)puVar18 = *(undefined *)param_2;
                    // WARNING: Store size is inaccurate
          *(undefined *)(puVar18 + 2) = auVar19;
          param_2 = param_2 + 4;
          puVar18 = puVar18 + 4;
          param_3 = param_3 - 1;
        }
        goto LAB_00403fab;
      }
LAB_00403cd7:
      uVar10 = (uint)param_1 & 3;
      puVar18 = param_1;
      while (uVar10 != 0) {
        *(undefined *)puVar18 = *(undefined *)param_2;
        param_3 = param_3 - 1;
        param_2 = (undefined8 *)((int)param_2 + 1);
        puVar18 = (undefined8 *)((int)puVar18 + 1);
        uVar10 = (uint)puVar18 & 3;
      }
    }
    else {
      uVar10 = param_3;
      puVar18 = param_1;
      if ((DAT_0041ec78 >> 1 & 1) != 0) {
        while (uVar10 != 0) {
          uVar10 = uVar10 - 1;
          *(undefined *)puVar18 = *(undefined *)param_2;
          param_2 = (undefined8 *)((int)param_2 + 1);
          puVar18 = (undefined8 *)((int)puVar18 + 1);
        }
        return CONCAT44(param_3,param_1);
      }
      if (((((uint)param_1 ^ (uint)param_2) & 0xf) == 0) && ((DAT_0041e010 >> 1 & 1) != 0)) {
        puVar18 = param_1;
        if (((uint)param_2 & 0xf) != 0) {
          uVar11 = 0x10 - ((uint)param_2 & 0xf);
          param_3 = param_3 - uVar11;
          uVar10 = uVar11 & 3;
          puVar18 = param_1;
          while (uVar10 != 0) {
            *(undefined *)puVar18 = *(undefined *)param_2;
            param_2 = (undefined8 *)((int)param_2 + 1);
            puVar18 = (undefined8 *)((int)puVar18 + 1);
            uVar10 = uVar10 - 1;
          }
          uVar11 = uVar11 >> 2;
          while (uVar11 != 0) {
            *(uint *)puVar18 = *(uint *)param_2;
            param_2 = (undefined8 *)((int)param_2 + 4);
            puVar18 = (undefined8 *)((int)puVar18 + 4);
            uVar11 = uVar11 - 1;
          }
        }
        uVar10 = param_3 & 0x7f;
        param_3 = param_3 >> 7;
        while (param_3 != 0) {
                    // WARNING: Load size is inaccurate
                    // WARNING: Load size is inaccurate
          auVar19 = *(undefined *)(param_2 + 2);
                    // WARNING: Load size is inaccurate
          auVar2 = *(undefined *)(param_2 + 4);
                    // WARNING: Load size is inaccurate
          auVar3 = *(undefined *)(param_2 + 6);
                    // WARNING: Store size is inaccurate
          *(undefined *)puVar18 = *(undefined *)param_2;
                    // WARNING: Store size is inaccurate
          *(undefined *)(puVar18 + 2) = auVar19;
                    // WARNING: Store size is inaccurate
          *(undefined *)(puVar18 + 4) = auVar2;
                    // WARNING: Store size is inaccurate
          *(undefined *)(puVar18 + 6) = auVar3;
                    // WARNING: Load size is inaccurate
                    // WARNING: Load size is inaccurate
          auVar19 = *(undefined *)(param_2 + 10);
                    // WARNING: Load size is inaccurate
          auVar2 = *(undefined *)(param_2 + 0xc);
                    // WARNING: Load size is inaccurate
          auVar3 = *(undefined *)(param_2 + 0xe);
                    // WARNING: Store size is inaccurate
          *(undefined *)(puVar18 + 8) = *(undefined *)(param_2 + 8);
                    // WARNING: Store size is inaccurate
          *(undefined *)(puVar18 + 10) = auVar19;
                    // WARNING: Store size is inaccurate
          *(undefined *)(puVar18 + 0xc) = auVar2;
                    // WARNING: Store size is inaccurate
          *(undefined *)(puVar18 + 0xe) = auVar3;
          param_2 = param_2 + 0x10;
          puVar18 = puVar18 + 0x10;
          param_3 = param_3 - 1;
        }
        goto LAB_00403f7d;
      }
      if (((DAT_0041ec78 & 1) == 0) || (((uint)param_1 & 3) != 0)) goto LAB_00403cd7;
      puVar18 = param_1;
      if (((uint)param_2 & 3) == 0) {
        if (((uint)param_1 >> 2 & 1) != 0) {
          uVar8 = *(undefined4 *)param_2;
          param_3 = param_3 - 4;
          param_2 = (undefined8 *)((int)param_2 + 4);
          *(undefined4 *)param_1 = uVar8;
          param_1 = (undefined8 *)((int)param_1 + 4);
        }
        if (((uint)param_1 >> 3 & 1) != 0) {
          uVar1 = *param_2;
          param_3 = param_3 - 8;
          param_2 = param_2 + 1;
          *param_1 = uVar1;
          param_1 = param_1 + 1;
        }
        if (((uint)param_2 & 7) == 0) {
                    // WARNING: Load size is inaccurate
          puVar18 = param_2 + -1;
          auVar19 = *(undefined *)(param_2 + -1);
          do {
            puVar13 = puVar18;
                    // WARNING: Load size is inaccurate
            auVar2 = *(undefined *)(puVar13 + 2);
            param_3 = param_3 - 0x30;
                    // WARNING: Load size is inaccurate
            auVar3 = *(undefined *)(puVar13 + 4);
                    // WARNING: Load size is inaccurate
            auVar4 = *(undefined *)(puVar13 + 6);
                    // WARNING: Store size is inaccurate
            *(undefined *)param_1 = SUB3216(ZEXT2032(CONCAT416(0x40,auVar19)) >> 0x40,0);
                    // WARNING: Store size is inaccurate
            *(undefined *)(param_1 + 2) = SUB3216(ZEXT2032(CONCAT416(0x40,auVar2)) >> 0x40,0);
                    // WARNING: Store size is inaccurate
            *(undefined *)(param_1 + 4) = SUB3216(ZEXT2032(CONCAT416(0x40,auVar3)) >> 0x40,0);
            param_1 = param_1 + 6;
            puVar18 = puVar13 + 6;
            auVar19 = auVar4;
          } while (0x2f < param_3);
          puVar13 = puVar13 + 7;
        }
        else {
          if (((uint)param_2 >> 3 & 1) == 0) {
                    // WARNING: Load size is inaccurate
            iVar9 = (int)param_2 + -4;
            auVar19 = *(undefined *)((int)param_2 + -4);
            do {
              iVar12 = iVar9;
                    // WARNING: Load size is inaccurate
              auVar2 = *(undefined *)(iVar12 + 0x10);
              param_3 = param_3 - 0x30;
                    // WARNING: Load size is inaccurate
              auVar3 = *(undefined *)(iVar12 + 0x20);
                    // WARNING: Load size is inaccurate
              auVar4 = *(undefined *)(iVar12 + 0x30);
                    // WARNING: Store size is inaccurate
              *(undefined *)param_1 = SUB2016(CONCAT416(0x20,auVar19) >> 0x20,0);
                    // WARNING: Store size is inaccurate
              *(undefined *)(param_1 + 2) = SUB2016(CONCAT416(0x20,auVar2) >> 0x20,0);
                    // WARNING: Store size is inaccurate
              *(undefined *)(param_1 + 4) = SUB2016(CONCAT416(0x20,auVar3) >> 0x20,0);
              param_1 = param_1 + 6;
              iVar9 = iVar12 + 0x30;
              auVar19 = auVar4;
            } while (0x2f < param_3);
            puVar13 = (undefined8 *)(iVar12 + 0x34);
          }
          else {
                    // WARNING: Load size is inaccurate
            iVar9 = (int)param_2 + -0xc;
            auVar19 = *(undefined *)((int)param_2 + -0xc);
            do {
              iVar12 = iVar9;
                    // WARNING: Load size is inaccurate
              auVar2 = *(undefined *)(iVar12 + 0x10);
              param_3 = param_3 - 0x30;
                    // WARNING: Load size is inaccurate
              auVar3 = *(undefined *)(iVar12 + 0x20);
                    // WARNING: Load size is inaccurate
              auVar4 = *(undefined *)(iVar12 + 0x30);
                    // WARNING: Store size is inaccurate
              *(undefined *)param_1 = SUB3216(ZEXT2032(CONCAT416(0x60,auVar19)) >> 0x60,0);
                    // WARNING: Store size is inaccurate
              *(undefined *)(param_1 + 2) = SUB3216(ZEXT2032(CONCAT416(0x60,auVar2)) >> 0x60,0);
                    // WARNING: Store size is inaccurate
              *(undefined *)(param_1 + 4) = SUB3216(ZEXT2032(CONCAT416(0x60,auVar3)) >> 0x60,0);
              param_1 = param_1 + 6;
              iVar9 = iVar12 + 0x30;
              auVar19 = auVar4;
            } while (0x2f < param_3);
            puVar13 = (undefined8 *)(iVar12 + 0x3c);
          }
        }
        while (0xf < param_3) {
                    // WARNING: Load size is inaccurate
          auVar19 = *(undefined *)puVar13;
          param_3 = param_3 - 0x10;
          puVar13 = puVar13 + 2;
                    // WARNING: Store size is inaccurate
          *(undefined *)param_1 = auVar19;
          param_1 = param_1 + 2;
        }
        if ((param_3 >> 2 & 1) != 0) {
          uVar8 = *(undefined4 *)puVar13;
          param_3 = param_3 - 4;
          puVar13 = (undefined8 *)((int)puVar13 + 4);
          *(undefined4 *)param_1 = uVar8;
          param_1 = (undefined8 *)((int)param_1 + 4);
        }
        if ((param_3 >> 3 & 1) != 0) {
          param_3 = param_3 - 8;
          *param_1 = *puVar13;
        }
                    // WARNING: Could not recover jumptable at 0x00403cd5. Too many branches
                    // WARNING: Treating indirect jump as call
        uVar20 = (*(code *)(&switchdataD_00403d14)[param_3])();
        return uVar20;
      }
    }
    uVar10 = param_3;
    if (0x1f < param_3) {
      uVar10 = param_3 >> 2;
      while (uVar10 != 0) {
        uVar10 = uVar10 - 1;
        *(uint *)puVar18 = *(uint *)param_2;
        param_2 = (undefined8 *)((int)param_2 + 4);
        puVar18 = (undefined8 *)((int)puVar18 + 4);
      }
      switch(param_3 & 3) {
      case 0:
        return CONCAT44(param_3,param_1) & 0x3ffffffff;
      case 1:
        *(undefined *)puVar18 = *(undefined *)param_2;
        return CONCAT44(param_3,param_1) & 0x3ffffffff;
      case 2:
        *(undefined *)puVar18 = *(undefined *)param_2;
        *(undefined *)((int)puVar18 + 1) = *(undefined *)((int)param_2 + 1);
        return CONCAT44(param_3,param_1) & 0x3ffffffff;
      case 3:
        *(undefined *)puVar18 = *(undefined *)param_2;
        *(undefined *)((int)puVar18 + 1) = *(undefined *)((int)param_2 + 1);
        *(undefined *)((int)puVar18 + 2) = *(undefined *)((int)param_2 + 2);
        return CONCAT44(param_3,param_1) & 0x3ffffffff;
      }
    }
  }
LAB_00403fab:
  if ((uVar10 & 0x1f) != 0) {
    uVar11 = (uVar10 & 0x1f) >> 2;
    while (uVar11 != 0) {
      param_3 = *(uint *)param_2;
      *(uint *)puVar18 = param_3;
      puVar18 = (undefined8 *)((int)puVar18 + 4);
      param_2 = (undefined8 *)((int)param_2 + 4);
      uVar11 = uVar11 - 1;
    }
    uVar10 = uVar10 & 3;
    while (uVar10 != 0) {
      *(undefined *)puVar18 = *(undefined *)param_2;
      param_2 = (undefined8 *)((int)param_2 + 1);
      puVar18 = (undefined8 *)((int)puVar18 + 1);
      uVar10 = uVar10 - 1;
    }
  }
LAB_00403fe0:
  return CONCAT44(param_3,param_1);
}



// WARNING: Function: __SEH_prolog4 replaced with injection: SEH_prolog4

int __cdecl FUN_00404024(int param_1,undefined8 *param_2,uint *param_3,byte *param_4)

{
  code *pcVar1;
  int iVar2;
  undefined8 *puVar3;
  int iVar4;
  undefined4 *in_FS_OFFSET;
  uint uVar5;
  undefined4 local_14;
  
  pcVar1 = DAT_0041ed20;
  iVar4 = 0;
  if (((param_3[1] == 0) || (*(char *)(param_3[1] + 8) == 0)) ||
     ((param_3[2] == 0 && (-1 < (int)*param_3)))) {
    iVar4 = 0;
    goto LAB_0040414d;
  }
  uVar5 = *param_3;
  if (-1 < (int)uVar5) {
    param_2 = (undefined8 *)((int)param_2 + param_3[2] + 0xc);
  }
  if ((((char)uVar5 < 0) && ((*param_4 & 0x10) != 0)) && (DAT_0041ed20 != (code *)0x0)) {
    _guard_check_icall();
    iVar2 = (*pcVar1)();
LAB_0040409b:
    if ((iVar2 == 0) || (param_2 == (undefined8 *)0x0)) {
LAB_0040415d:
      _terminate();
      pcVar1 = (code *)swi(3);
      iVar4 = (*pcVar1)();
      return iVar4;
    }
    *(int *)param_2 = iVar2;
  }
  else {
    if ((uVar5 & 8) != 0) {
      iVar2 = *(int *)(param_1 + 0x18);
      goto LAB_0040409b;
    }
    if ((*param_4 & 1) == 0) {
      if (*(int *)(param_4 + 0x18) == 0) {
        if ((*(int *)(param_1 + 0x18) != 0) && (param_2 != (undefined8 *)0x0)) {
          uVar5 = *(uint *)(param_4 + 0x14);
          puVar3 = (undefined8 *)___AdjustPointer(*(int *)(param_1 + 0x18),(int *)(param_4 + 8));
          FUN_00405050(param_2,puVar3,uVar5);
          goto LAB_0040414d;
        }
        goto LAB_0040415d;
      }
      if ((*(int *)(param_1 + 0x18) == 0) || (param_2 == (undefined8 *)0x0)) goto LAB_0040415d;
      iVar4 = (uint)((*param_4 & 4) != 0) + 1;
      goto LAB_0040414d;
    }
    if ((*(int *)(param_1 + 0x18) == 0) || (param_2 == (undefined8 *)0x0)) goto LAB_0040415d;
    FUN_00405050(param_2,*(undefined8 **)(param_1 + 0x18),*(uint *)(param_4 + 0x14));
    if ((*(int *)(param_4 + 0x14) != 4) || (*(int *)param_2 == 0)) goto LAB_0040414d;
    iVar2 = *(int *)param_2;
  }
  iVar2 = ___AdjustPointer(iVar2,(int *)(param_4 + 8));
  *(int *)param_2 = iVar2;
LAB_0040414d:
  *in_FS_OFFSET = local_14;
  return iVar4;
}



// WARNING: Function: __SEH_prolog4 replaced with injection: SEH_prolog4

void __cdecl FUN_00404163(int param_1,undefined8 *param_2,uint *param_3,byte *param_4)

{
  int iVar1;
  undefined8 *puVar2;
  undefined4 *in_FS_OFFSET;
  undefined4 uVar3;
  undefined4 local_14;
  
  puVar2 = param_2;
  if (-1 < (int)*param_3) {
    puVar2 = (undefined8 *)((int)param_2 + param_3[2] + 0xc);
  }
  iVar1 = FUN_00404024(param_1,param_2,param_3,param_4);
  if (iVar1 == 1) {
    iVar1 = ___AdjustPointer(*(int *)(param_1 + 0x18),(int *)(param_4 + 8));
    FUN_00404d49(puVar2,*(undefined **)(param_4 + 0x18),iVar1);
  }
  else {
    if (iVar1 == 2) {
      uVar3 = 1;
      iVar1 = ___AdjustPointer(*(int *)(param_1 + 0x18),(int *)(param_4 + 8));
      FUN_00404d59(puVar2,*(undefined **)(param_4 + 0x18),iVar1,uVar3);
    }
  }
  *in_FS_OFFSET = local_14;
  return;
}



void __cdecl
FUN_004041fc(EHExceptionRecord *param_1,undefined8 *param_2,undefined4 param_3,undefined4 param_4,
            int param_5,uint *param_6,byte *param_7,int *param_8,int param_9,undefined8 *param_10)

{
  void *pvVar1;
  
  if (param_7 != (byte *)0x0) {
    FUN_00404163((int)param_1,param_2,param_6,param_7);
  }
  if (param_10 == (undefined8 *)0x0) {
    param_10 = param_2;
  }
  _UnwindNestedFrames((EHRegistrationNode *)param_10,param_1);
  FUN_00404bc7((int)param_2,param_4,param_5,*param_8);
  FUN_00404e16((int)param_2,param_5,param_8[1] + 1);
  pvVar1 = (void *)FUN_00404967((int)param_1,(int)param_2,param_3,param_5,param_6[3],param_9,0x100);
  if (pvVar1 != (void *)0x0) {
    _JumpToContinuation(pvVar1,(EHRegistrationNode *)param_2);
  }
  return;
}



// WARNING: Could not reconcile some variable overlaps

void __cdecl
FUN_0040427c(int *param_1,undefined8 *param_2,undefined4 param_3,undefined4 param_4,uint *param_5,
            char param_6,int param_7,undefined8 *param_8)

{
  code *pcVar1;
  uint *puVar2;
  int iVar3;
  byte **ppbVar4;
  char cVar5;
  int iVar6;
  uint uVar7;
  undefined4 uVar8;
  int iVar9;
  int *local_58;
  uint local_54 [2];
  uint local_4c;
  uint *local_48;
  undefined4 local_44;
  int *local_40;
  byte **local_3c;
  int local_38;
  int local_34;
  int *local_30;
  undefined4 local_2c;
  int local_28;
  int local_24;
  uint *local_20;
  uint local_1c;
  byte **local_18;
  uint *local_14;
  undefined4 local_10;
  int local_c;
  char local_5;
  
  iVar9 = 0;
  local_2c = local_2c & 0xffffff00;
  local_5 = 0;
  local_c = FUN_00404df9((int)param_2,param_4,(int)param_5);
  if ((local_c < -1) || ((int)param_5[1] <= local_c)) goto LAB_00404604;
  if (((*param_1 == -0x1f928c9d) && (param_1[4] == 3)) &&
     ((((param_1[5] == 0x19930520 || (param_1[5] == 0x19930521)) || (param_1[5] == 0x19930522)) &&
      (param_1[7] == 0)))) {
    iVar6 = ___vcrt_getptd();
    if (*(int *)(iVar6 + 0x10) == 0) {
      return;
    }
    iVar6 = ___vcrt_getptd();
    param_1 = *(int **)(iVar6 + 0x10);
    iVar6 = ___vcrt_getptd();
    local_2c = CONCAT31(local_2c._1_3_,1);
    local_10 = *(undefined4 *)(iVar6 + 0x14);
    if ((param_1 == (int *)0x0) ||
       ((((*param_1 == -0x1f928c9d && (param_1[4] == 3)) &&
         ((param_1[5] == 0x19930520 || ((param_1[5] == 0x19930521 || (param_1[5] == 0x19930522))))))
        && (param_1[7] == 0)))) goto LAB_00404604;
    iVar6 = ___vcrt_getptd();
    if (*(int *)(iVar6 + 0x1c) != 0) {
      iVar6 = ___vcrt_getptd();
      local_20 = *(uint **)(iVar6 + 0x1c);
      iVar6 = ___vcrt_getptd();
      *(undefined4 *)(iVar6 + 0x1c) = 0;
      cVar5 = FUN_00404caf(param_1,local_20);
      puVar2 = local_20;
      if (cVar5 == 0) {
        if (0 < (int)*local_20) {
          local_20 = (uint *)0x0;
          do {
            uVar7 = FUN_0040491b(*(void **)((int)local_20 + puVar2[1] + 4),
                                 (int)&class_std__bad_exception_RTTI_Type_Descriptor);
            if ((char)uVar7 != 0) {
              FUN_00404e24(param_1);
              FUN_004048c6(local_54);
              FUN_00404fe3((int *)local_54,&DAT_0041d024);
              param_5 = puVar2;
              goto LAB_004045c5;
            }
            iVar9 = iVar9 + 1;
            local_20 = local_20 + 4;
          } while (iVar9 < (int)*puVar2);
        }
        goto LAB_00404604;
      }
    }
  }
  else {
    local_10 = param_3;
  }
  local_48 = param_5;
  local_44 = param_4;
  if (((*param_1 == -0x1f928c9d) && (param_1[4] == 3)) &&
     ((param_1[5] == 0x19930520 || ((param_1[5] == 0x19930521 || (param_1[5] == 0x19930522)))))) {
    if (param_5[3] != 0) {
      FUN_004036fe(&local_58,(int *)&local_48,local_c,param_7);
      local_30 = local_58;
      local_1c = local_54[0];
      if (local_54[0] < local_4c) {
        local_20 = (uint *)(local_54[0] * 0x14);
        iVar9 = local_c;
        local_1c = local_54[0];
        do {
          local_40 = (int *)(*(int *)(*local_30 + 0x10) + (int)local_20);
          if ((*local_40 == iVar9 || *local_40 < iVar9) && (iVar9 <= local_40[1])) {
            local_14 = (uint *)local_40[4];
            local_38 = local_40[3];
            local_28 = 0;
            if (local_38 != 0) {
              iVar6 = **(int **)(param_1[7] + 0xc);
              local_3c = (byte **)(*(int **)(param_1[7] + 0xc) + 1);
              iVar3 = iVar6;
              ppbVar4 = local_3c;
              local_34 = iVar6;
              do {
                while (local_18 = ppbVar4, local_24 = iVar3, 0 < local_24) {
                  iVar9 = ___TypeMatch((byte *)local_14,*local_18,(byte *)param_1[7]);
                  if (iVar9 != 0) {
                    local_5 = 1;
                    FUN_004041fc((EHExceptionRecord *)param_1,param_2,local_10,param_4,(int)param_5,
                                 local_14,*local_18,local_40,param_7,param_8);
                    iVar9 = local_c;
                    goto LAB_0040450c;
                  }
                  iVar3 = local_24 + -1;
                  iVar6 = local_34;
                  ppbVar4 = local_18 + 1;
                }
                local_14 = local_14 + 4;
                local_28 = local_28 + 1;
                iVar9 = local_c;
                iVar3 = iVar6;
                ppbVar4 = local_3c;
              } while (local_28 != local_38);
            }
          }
LAB_0040450c:
          local_20 = local_20 + 5;
          local_1c = local_1c + 1;
        } while (local_1c < local_4c);
      }
    }
    if (param_6 != 0) {
      FUN_00404e24(param_1);
    }
    if (((local_5 == 0) && (0x19930520 < (*param_5 & 0x1fffffff))) &&
       ((param_5[7] != 0 || (((param_5[8] >> 2 & 1) != 0 && (param_7 == 0)))))) {
      if ((param_5[8] >> 2 & 1) != 0) goto LAB_00404604;
      cVar5 = FUN_00404caf(param_1,param_5[7]);
      if (cVar5 == 0) {
LAB_004045c5:
        iVar9 = ___vcrt_getptd();
        *(int **)(iVar9 + 0x10) = param_1;
        iVar9 = ___vcrt_getptd();
        *(undefined4 *)(iVar9 + 0x14) = local_10;
        if (param_8 == (undefined8 *)0x0) {
          param_8 = param_2;
        }
        _UnwindNestedFrames((EHRegistrationNode *)param_8,(EHExceptionRecord *)param_1);
        FUN_00404baf((int)param_2,param_4,(int)param_5);
        uVar8 = FUN_00404d6c((int)param_5);
        FUN_00404b2b(uVar8);
        goto LAB_00404604;
      }
    }
  }
  else {
    if (param_5[3] != 0) {
      if (param_6 != 0) goto LAB_00404604;
      FUN_0040460a(param_1,param_2,local_10,param_4,(int)param_5,local_c,param_7,param_8);
    }
  }
  iVar9 = ___vcrt_getptd();
  if (*(int *)(iVar9 + 0x1c) == 0) {
    return;
  }
LAB_00404604:
  _terminate();
  pcVar1 = (code *)swi(3);
  (*pcVar1)();
  return;
}



void __cdecl
FUN_0040460a(int *param_1,undefined8 *param_2,undefined4 param_3,undefined4 param_4,int param_5,
            int param_6,int param_7,undefined8 *param_8)

{
  code *pcVar1;
  int iVar2;
  PVOID pvVar3;
  uint *puVar4;
  int *piVar5;
  int *local_24;
  uint local_20;
  uint local_18;
  int local_14;
  undefined4 local_10;
  int local_c;
  int *local_8;
  
  if (*param_1 != -0x7ffffffd) {
    iVar2 = ___vcrt_getptd();
    if (*(int *)(iVar2 + 8) != 0) {
      pvVar3 = EncodePointer((PVOID)0x0);
      iVar2 = ___vcrt_getptd();
      if ((((*(PVOID *)(iVar2 + 8) != pvVar3) && (*param_1 != -0x1fbcb0b3)) &&
          (*param_1 != -0x1fbcbcae)) &&
         (iVar2 = FUN_004037de(param_1,(undefined4 *)param_2,param_3,param_4,param_5,param_7,param_8
                              ), iVar2 != 0)) {
        return;
      }
    }
    local_14 = param_5;
    local_10 = param_4;
    if (*(int *)(param_5 + 0xc) == 0) {
      _terminate();
      pcVar1 = (code *)swi(3);
      (*pcVar1)();
      return;
    }
    FUN_004036fe(&local_24,&local_14,param_6,param_7);
    local_8 = local_24;
    if (local_20 < local_18) {
      local_c = local_20 * 0x14;
      do {
        piVar5 = (int *)(*(int *)(*local_24 + 0x10) + local_c);
        if ((*piVar5 == param_6 || *piVar5 < param_6) && (param_6 <= piVar5[1])) {
          puVar4 = (uint *)(piVar5[3] * 0x10 + piVar5[4] + -0x10);
          if (((puVar4[1] == 0) || (local_24 = local_8, *(char *)(puVar4[1] + 8) == 0)) &&
             (local_24 = local_8, (*(byte *)puVar4 & 0x40) == 0)) {
            FUN_004041fc((EHExceptionRecord *)param_1,param_2,param_3,param_4,param_5,puVar4,
                         (byte *)0x0,piVar5,param_7,param_8);
            local_24 = local_8;
          }
        }
        local_c = local_c + 0x14;
        local_20 = local_20 + 1;
      } while (local_20 < local_18);
    }
  }
  return;
}



// Library Function - Single Match
// Name: ___TypeMatch
// Library: Visual Studio 2015 Release

undefined4 __cdecl ___TypeMatch(byte *param_1,byte *param_2,byte *param_3)

{
  byte bVar1;
  int iVar2;
  byte *pbVar3;
  uint uVar4;
  byte *pbVar5;
  undefined4 uVar6;
  bool bVar7;
  
  iVar2 = *(int *)(param_1 + 4);
  if (((iVar2 == 0) || (pbVar5 = (byte *)(iVar2 + 8), *pbVar5 == 0)) ||
     (((*param_1 & 0x80) != 0 && ((*param_2 & 0x10) != 0)))) {
    uVar6 = 1;
  }
  else {
    uVar6 = 0;
    if (iVar2 != *(int *)(param_2 + 4)) {
      pbVar3 = (byte *)(*(int *)(param_2 + 4) + 8);
      do {
        bVar1 = *pbVar5;
        bVar7 = bVar1 < *pbVar3;
        if (bVar1 != *pbVar3) {
LAB_0040476c:
          uVar4 = -(uint)bVar7 | 1;
          goto LAB_00404771;
        }
        if (bVar1 == 0) break;
        bVar1 = pbVar5[1];
        bVar7 = bVar1 < pbVar3[1];
        if (bVar1 != pbVar3[1]) goto LAB_0040476c;
        pbVar5 = pbVar5 + 2;
        pbVar3 = pbVar3 + 2;
      } while (bVar1 != 0);
      uVar4 = 0;
LAB_00404771:
      if (uVar4 != 0) {
        return 0;
      }
    }
    if ((((*param_2 & 2) == 0) || ((*param_1 & 8) != 0)) &&
       ((((*param_3 & 1) == 0 || ((*param_1 & 1) != 0)) &&
        (((*param_3 & 2) == 0 || ((*param_1 & 2) != 0)))))) {
      uVar6 = 1;
    }
  }
  return uVar6;
}



undefined4 __cdecl
FUN_004047a9(int *param_1,undefined8 *param_2,int param_3,undefined4 param_4,uint *param_5,
            int param_6,undefined8 *param_7,byte param_8)

{
  code *pcVar1;
  int iVar2;
  undefined4 uVar3;
  uint uVar4;
  
  FUN_00403117(param_3);
  iVar2 = ___vcrt_getptd();
  if ((((*(int *)(iVar2 + 0x20) != 0) || (*param_1 == -0x1f928c9d)) || (*param_1 == -0x7fffffda)) ||
     (((*param_5 & 0x1fffffff) < 0x19930522 || ((*(byte *)(param_5 + 8) & 1) == 0)))) {
    if ((*(byte *)(param_1 + 1) & 0x66) == 0) {
      if (((param_5[3] != 0) || ((0x19930520 < (*param_5 & 0x1fffffff) && (param_5[7] != 0)))) ||
         ((0x19930521 < (*param_5 & 0x1fffffff) && ((param_5[8] >> 2 & 1) != 0)))) {
        if ((((*param_1 == -0x1f928c9d) && (2 < (uint)param_1[4])) &&
            (0x19930522 < (uint)param_1[5])) &&
           (pcVar1 = *(code **)(param_1[7] + 8), pcVar1 != (code *)0x0)) {
          uVar4 = (uint)param_8;
          _guard_check_icall();
          uVar3 = (*pcVar1)(param_1,param_2,param_3,param_4,param_5,param_6,param_7,uVar4);
          return uVar3;
        }
        FUN_0040427c(param_1,param_2,param_3,param_4,param_5,param_8,param_6,param_7);
      }
    }
    else {
      if ((param_5[1] != 0) && (param_6 == 0)) {
        FUN_00404baf((int)param_2,param_4,(int)param_5);
      }
    }
  }
  return 1;
}



undefined4 * __thiscall FUN_004048ab(void *this,exception *param_1)

{
  exception((exception *)this,param_1);
  *(undefined4 *)this = 0x417a9c;
  return (undefined4 *)this;
}



undefined4 * __fastcall FUN_004048c6(undefined4 *param_1)

{
  param_1[1] = 0;
  param_1[2] = 0;
  param_1[1] = 0x417aa4;
  *param_1 = 0x417a9c;
  return param_1;
}



// Library Function - Single Match
// Name: ??0exception@std@@QAE@ABV01@@Z
// Library: Visual Studio 2015 Release
// public: __thiscall std::exception::exception(class std::exception const &)

void __thiscall exception(exception *this,exception *param_1)

{
  *(undefined4 *)this = 0x417a7c;
  *(char **)(this + 4) = (char *)0x0;
  *(undefined4 *)(this + 8) = 0;
  ___std_exception_copy((char **)(param_1 + 4),(char **)(this + 4));
  return;
}



uint __thiscall FUN_0040491b(void *this,int param_1)

{
  uint uVar1;
  
  uVar1 = ___std_type_info_compare((int)this + 4,param_1 + 4);
  return -uVar1 & 0xffffff00 | (uint)(byte)(1 - (uVar1 != 0));
}



// Library Function - Multiple Matches With Same Base Name
// Name: `scalar_deleting_destructor'
// Library: Visual Studio 2015 Release

undefined4 * __thiscall _scalar_deleting_destructor_(void *this,byte param_1)

{
  *(undefined4 *)this = 0x417a7c;
  ___std_exception_destroy((void **)((int)this + 4));
  if ((param_1 & 1) != 0) {
    FUN_00416378(this);
  }
  return (undefined4 *)this;
}



// WARNING: Function: __SEH_prolog4 replaced with injection: SEH_prolog4

undefined4 __cdecl
FUN_00404967(int param_1,int param_2,undefined4 param_3,undefined4 param_4,undefined4 param_5,
            int param_6,int param_7)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 *in_FS_OFFSET;
  undefined4 local_50 [2];
  undefined4 local_48;
  undefined4 local_44;
  undefined4 local_40;
  undefined4 local_3c;
  undefined4 *local_38;
  undefined4 local_34;
  undefined4 local_20;
  undefined4 local_14;
  undefined4 uStack12;
  undefined *local_8;
  
  local_8 = &DAT_0041cf68;
  uStack12 = 0x404973;
  local_20 = param_5;
  local_44 = 0;
  local_34 = *(undefined4 *)(param_2 + -4);
  local_38 = __CreateFrameInfo(local_50,*(undefined4 *)(param_1 + 0x18));
  iVar1 = ___vcrt_getptd();
  local_3c = *(undefined4 *)(iVar1 + 0x10);
  iVar1 = ___vcrt_getptd();
  local_40 = *(undefined4 *)(iVar1 + 0x14);
  iVar1 = ___vcrt_getptd();
  *(int *)(iVar1 + 0x10) = param_1;
  iVar1 = ___vcrt_getptd();
  *(undefined4 *)(iVar1 + 0x14) = param_3;
  local_48 = 1;
  local_8 = (undefined *)0x1;
  uVar2 = FUN_00403780(param_2,param_4,param_5,param_6,param_7);
  local_8 = (undefined *)0xfffffffe;
  local_48 = 0;
  local_20 = uVar2;
  FUN_00404aaf();
  *in_FS_OFFSET = local_14;
  return uVar2;
}



void FUN_00404aaf(void)

{
  int iVar1;
  int unaff_EBX;
  int unaff_EBP;
  int *unaff_EDI;
  
  *(undefined4 *)(*(int *)(unaff_EBP + 0xc) + -4) = *(undefined4 *)(unaff_EBP + -0x30);
  FUN_00403991(*(int *)(unaff_EBP + -0x34));
  iVar1 = ___vcrt_getptd();
  *(undefined4 *)(iVar1 + 0x10) = *(undefined4 *)(unaff_EBP + -0x38);
  iVar1 = ___vcrt_getptd();
  *(undefined4 *)(iVar1 + 0x14) = *(undefined4 *)(unaff_EBP + -0x3c);
  if ((((*unaff_EDI == -0x1f928c9d) && (unaff_EDI[4] == 3)) &&
      ((unaff_EDI[5] == 0x19930520 || ((unaff_EDI[5] == 0x19930521 || (unaff_EDI[5] == 0x19930522)))
       ))) && ((*(int *)(unaff_EBP + -0x40) == 0 && (unaff_EBX != 0)))) {
    iVar1 = __IsExceptionObjectToBeDestroyed(unaff_EDI[6]);
    if (iVar1 != 0) {
      FUN_00404e24(unaff_EDI);
    }
  }
  return;
}



// WARNING: Function: __EH_prolog3_catch replaced with injection: EH_prolog3

void FUN_00404b2b(undefined4 param_1)

{
  code *pcVar1;
  int iVar2;
  
  iVar2 = ___vcrt_getptd();
  if (*(int *)(iVar2 + 0x1c) == 0) {
    FUN_00404ddc();
    iVar2 = ___vcrt_getptd();
    *(undefined4 *)(iVar2 + 0x1c) = param_1;
    FUN_00404fe3((int *)0x0,(byte *)0x0);
  }
  _terminate();
  pcVar1 = (code *)swi(3);
  (*pcVar1)();
  return;
}



void Catch_All_00404b4b(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_EBP;
  
  iVar2 = ___vcrt_getptd();
  *(undefined4 *)(iVar2 + 0x1c) = *(undefined4 *)(unaff_EBP + 8);
  FUN_00404fe3((int *)0x0,(byte *)0x0);
  _terminate();
  pcVar1 = (code *)swi(3);
  (*pcVar1)();
  return;
}



// Library Function - Single Match
// Name: ?ExFilterRethrow@@YAHPAU_EXCEPTION_POINTERS@@@Z
// Library: Visual Studio 2015 Release
// int __cdecl ExFilterRethrow(struct _EXCEPTION_POINTERS *)

int __cdecl ExFilterRethrow(_EXCEPTION_POINTERS *param_1)

{
  PEXCEPTION_RECORD pEVar1;
  int iVar2;
  
  pEVar1 = param_1->ExceptionRecord;
  if ((((pEVar1->ExceptionCode == 0xe06d7363) && (pEVar1->NumberParameters == 3)) &&
      ((pEVar1->ExceptionInformation[0] == 0x19930520 ||
       ((pEVar1->ExceptionInformation[0] == 0x19930521 ||
        (pEVar1->ExceptionInformation[0] == 0x19930522)))))) &&
     (pEVar1->ExceptionInformation[2] == 0)) {
    iVar2 = ___vcrt_getptd();
    *(undefined4 *)(iVar2 + 0x20) = 1;
    return 1;
  }
  return 0;
}



void __cdecl FUN_00404baf(int param_1,undefined4 param_2,int param_3)

{
  FUN_00404bc7(param_1,param_2,param_3,-1);
  return;
}



// WARNING: Function: __SEH_prolog4 replaced with injection: SEH_prolog4

void __cdecl FUN_00404bc7(int param_1,undefined4 param_2,int param_3,int param_4)

{
  code *pcVar1;
  int iVar2;
  int iVar3;
  undefined4 *in_FS_OFFSET;
  undefined4 local_14;
  
  iVar2 = FUN_00404df9(param_1,param_2,param_3);
  iVar3 = ___vcrt_getptd();
  *(int *)(iVar3 + 0x18) = *(int *)(iVar3 + 0x18) + 1;
  while (iVar3 = iVar2, iVar3 != param_4) {
    if ((iVar3 < 0) || (*(int *)(param_3 + 4) <= iVar3)) goto LAB_00404ca9;
    iVar2 = *(int *)(*(int *)(param_3 + 8) + iVar3 * 8);
    if (*(int *)(*(int *)(param_3 + 8) + 4 + iVar3 * 8) != 0) {
      FUN_00404e16(param_1,param_3,iVar2);
      __CallSettingFrame_12(*(undefined4 *)(*(int *)(param_3 + 8) + 4 + iVar3 * 8),param_1,0x103);
    }
  }
  FUN_00404c95();
  if (iVar3 == param_4) {
    FUN_00404e16(param_1,param_3,iVar3);
    *in_FS_OFFSET = local_14;
    return;
  }
LAB_00404ca9:
  _terminate();
  pcVar1 = (code *)swi(3);
  (*pcVar1)();
  return;
}



void FUN_00404c95(void)

{
  int iVar1;
  
  iVar1 = ___vcrt_getptd();
  if (0 < *(int *)(iVar1 + 0x18)) {
    iVar1 = ___vcrt_getptd();
    *(int *)(iVar1 + 0x18) = *(int *)(iVar1 + 0x18) + -1;
  }
  return;
}



uint FUN_00404caf(int param_1,int *param_2)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  code *pcVar4;
  byte **in_EAX;
  int iVar5;
  undefined4 uVar6;
  byte **ppbVar7;
  byte bVar8;
  int iVar9;
  int local_c;
  int local_8;
  
  if (param_2 == (int *)0x0) {
    _terminate();
    pcVar4 = (code *)swi(3);
    uVar6 = (*pcVar4)();
    return uVar6;
  }
  iVar9 = *param_2;
  bVar8 = 0;
  if (0 < iVar9) {
    local_8 = 0;
    piVar1 = *(int **)(*(int *)(param_1 + 0x1c) + 0xc);
    iVar2 = *piVar1;
    in_EAX = (byte **)(piVar1 + 1);
    bVar8 = 0;
    do {
      if (0 < iVar2) {
        iVar3 = param_2[1];
        ppbVar7 = in_EAX;
        local_c = iVar2;
        do {
          iVar5 = ___TypeMatch((byte *)(iVar3 + local_8),*ppbVar7,*(byte **)(param_1 + 0x1c));
          if (iVar5 != 0) {
            bVar8 = 1;
            break;
          }
          local_c = local_c + -1;
          ppbVar7 = ppbVar7 + 1;
        } while (0 < local_c);
      }
      local_8 = local_8 + 0x10;
      iVar9 = iVar9 + -1;
    } while (iVar9 != 0);
  }
  return (uint)in_EAX & 0xffffff00 | (uint)bVar8;
}



void FUN_00404d49(undefined4 param_1,undefined *param_2,undefined4 param_3)

{
  (*(code *)param_2)(param_3);
  return;
}



void FUN_00404d59(undefined4 param_1,undefined *param_2,undefined4 param_3,undefined4 param_4)

{
  (*(code *)param_2)(param_3,param_4);
  return;
}



undefined4 __cdecl FUN_00404d6c(int param_1)

{
  return *(undefined4 *)(param_1 + 0x1c);
}



char * __fastcall FUN_00404d77(int param_1)

{
  char *pcVar1;
  
  pcVar1 = *(char **)(param_1 + 4);
  if (pcVar1 == (char *)0x0) {
    pcVar1 = "Unknown exception";
  }
  return pcVar1;
}



// WARNING: Restarted to delay deadcode elimination for space: stack
// Library Function - Single Match
// Name: __CallSettingFrame@12
// Library: Visual Studio

void __CallSettingFrame_12(undefined4 param_1,undefined4 param_2,int param_3)

{
  code *pcVar1;
  
  pcVar1 = (code *)__NLG_Notify1(param_3);
  (*pcVar1)();
  if (param_3 == 0x100) {
    param_3 = 2;
  }
  __NLG_Notify1(param_3);
  return;
}



void FUN_00404ddc(void)

{
  code *pcVar1;
  int iVar2;
  
  iVar2 = ___vcrt_getptd();
  pcVar1 = *(code **)(iVar2 + 4);
  if (pcVar1 != (code *)0x0) {
    _guard_check_icall();
    (*pcVar1)();
  }
  _terminate();
  pcVar1 = (code *)swi(3);
  (*pcVar1)();
  return;
}



int __cdecl FUN_00404df9(int param_1,undefined4 param_2,int param_3)

{
  if (*(int *)(param_3 + 4) < 0x81) {
    return (int)*(char *)(param_1 + 8);
  }
  return *(int *)(param_1 + 8);
}



void __cdecl FUN_00404e16(int param_1,undefined4 param_2,undefined4 param_3)

{
  *(undefined4 *)(param_1 + 8) = param_3;
  return;
}



// WARNING: Function: __SEH_prolog4 replaced with injection: SEH_prolog4

void __cdecl FUN_00404e24(int *param_1)

{
  byte *pbVar1;
  code *pcVar2;
  undefined4 *in_FS_OFFSET;
  int *piVar3;
  undefined4 local_14;
  
  if ((((param_1 != (int *)0x0) && (*param_1 == -0x1f928c9d)) && (param_1[4] == 3)) &&
     ((((param_1[5] == 0x19930520 || (param_1[5] == 0x19930521)) || (param_1[5] == 0x19930522)) &&
      (pbVar1 = (byte *)param_1[7], pbVar1 != (byte *)0x0)))) {
    if (*(undefined **)(pbVar1 + 4) == (undefined *)0x0) {
      if (((*pbVar1 & 0x10) != 0) && (piVar3 = *(int **)param_1[6], piVar3 != (int *)0x0)) {
        pcVar2 = *(code **)(*piVar3 + 8);
        _guard_check_icall();
        (*pcVar2)(piVar3);
      }
    }
    else {
      FUN_00404ec4(param_1[6],*(undefined **)(pbVar1 + 4));
    }
  }
  *in_FS_OFFSET = local_14;
  return;
}



void FUN_00404ec4(undefined4 param_1,undefined *param_2)

{
  (*(code *)param_2)();
  return;
}



// Library Function - Single Match
// Name: __IsExceptionObjectToBeDestroyed
// Library: Visual Studio 2015 Release

undefined4 __cdecl __IsExceptionObjectToBeDestroyed(int param_1)

{
  int *piVar1;
  int iVar2;
  
  iVar2 = ___vcrt_getptd();
  piVar1 = *(int **)(iVar2 + 0x24);
  while( true ) {
    if (piVar1 == (int *)0x0) {
      return 1;
    }
    if (*piVar1 == param_1) break;
    piVar1 = (int *)piVar1[1];
  }
  return 0;
}



// Library Function - Single Match
// Name: ___AdjustPointer
// Library: Visual Studio 2015 Release

int __cdecl ___AdjustPointer(int param_1,int *param_2)

{
  int iVar1;
  int iVar2;
  
  iVar1 = param_2[1];
  iVar2 = *param_2 + param_1;
  if (-1 < iVar1) {
    iVar2 = iVar2 + *(int *)(*(int *)(iVar1 + param_1) + param_2[2]) + iVar1;
  }
  return iVar2;
}



// Library Function - Single Match
// Name: ___FrameUnwindFilter
// Library: Visual Studio 2015 Release

undefined4 ___FrameUnwindFilter(int **param_1)

{
  int *piVar1;
  int iVar2;
  undefined4 uVar3;
  
  piVar1 = *param_1;
  if ((*piVar1 == -0x1fbcbcae) || (*piVar1 == -0x1fbcb0b3)) {
    iVar2 = ___vcrt_getptd();
    if (0 < *(int *)(iVar2 + 0x18)) {
      iVar2 = ___vcrt_getptd();
      *(int *)(iVar2 + 0x18) = *(int *)(iVar2 + 0x18) + -1;
    }
  }
  else {
    if (*piVar1 == -0x1f928c9d) {
      iVar2 = ___vcrt_getptd();
      *(undefined4 *)(iVar2 + 0x18) = 0;
      uVar3 = _terminate();
      return uVar3;
    }
  }
  return 0;
}



// Library Function - Single Match
// Name: ___std_exception_copy
// Library: Visual Studio 2015 Release

void __cdecl ___std_exception_copy(char **param_1,char **param_2)

{
  char *pcVar1;
  char cVar2;
  char *_Dst;
  char *pcVar3;
  char *pcVar4;
  
  if ((*(char *)(param_1 + 1) == 0) || (pcVar3 = *param_1, pcVar3 == (char *)0x0)) {
    *param_2 = *param_1;
    *(undefined *)(param_2 + 1) = 0;
  }
  else {
    pcVar1 = pcVar3 + 1;
    do {
      cVar2 = *pcVar3;
      pcVar3 = pcVar3 + 1;
    } while (cVar2 != 0);
    _Dst = (char *)allocate((size_t)(pcVar3 + (1 - (int)pcVar1)));
    pcVar4 = _Dst;
    if (_Dst != (char *)0x0) {
      _strcpy_s(_Dst,(rsize_t)(pcVar3 + (1 - (int)pcVar1)),*param_1);
      pcVar4 = (char *)0x0;
      *param_2 = _Dst;
      *(undefined *)(param_2 + 1) = 1;
    }
    free(pcVar4);
  }
  return;
}



// Library Function - Single Match
// Name: ___std_exception_destroy
// Library: Visual Studio 2015 Release

void __cdecl ___std_exception_destroy(void **param_1)

{
  if (*(char *)(param_1 + 1) != 0) {
    free(*param_1);
  }
  *param_1 = (void *)0x0;
  *(undefined *)(param_1 + 1) = 0;
  return;
}



void FUN_00404fe3(int *param_1,byte *param_2)

{
  code *pcVar1;
  int iVar2;
  DWORD *pDVar3;
  DWORD *pDVar4;
  int *piVar5;
  DWORD local_24;
  DWORD local_20;
  DWORD local_14;
  ULONG_PTR local_10;
  int *local_c;
  byte *local_8;
  
  iVar2 = 8;
  pDVar3 = &DAT_00417ab8;
  pDVar4 = &local_24;
  while (iVar2 != 0) {
    iVar2 = iVar2 + -1;
    *pDVar4 = *pDVar3;
    pDVar3 = pDVar3 + 1;
    pDVar4 = pDVar4 + 1;
  }
  if ((param_2 != (byte *)0x0) && ((*param_2 & 0x10) != 0)) {
    piVar5 = (int *)(*param_1 + -4);
    pcVar1 = *(code **)(*piVar5 + 0x20);
    param_2 = *(byte **)(*piVar5 + 0x18);
    _guard_check_icall();
    (*pcVar1)(piVar5);
  }
  local_c = param_1;
  if ((param_2 != (byte *)0x0) && ((*param_2 & 8) != 0)) {
    local_10 = 0x1994000;
  }
  local_8 = param_2;
  RaiseException(local_24,local_20,local_14,&local_10);
  return;
}



ulonglong __cdecl FUN_00405050(undefined8 *param_1,undefined8 *param_2,uint param_3)

{
  undefined8 uVar1;
  undefined auVar2 [16];
  undefined auVar3 [16];
  undefined auVar4 [16];
  undefined auVar5 [16];
  undefined auVar6 [16];
  undefined auVar7 [16];
  undefined4 uVar8;
  int iVar9;
  uint uVar10;
  uint uVar11;
  int iVar12;
  undefined8 *puVar13;
  undefined4 *puVar14;
  undefined4 *puVar15;
  undefined4 *puVar16;
  undefined4 *puVar17;
  undefined8 *puVar18;
  undefined auVar19 [16];
  ulonglong uVar20;
  
  if ((param_2 < param_1) && (param_1 < (undefined8 *)(param_3 + (int)param_2))) {
    puVar14 = (undefined4 *)((int)param_2 + param_3);
    puVar16 = (undefined4 *)((int)param_1 + param_3);
    uVar10 = param_3;
    uVar11 = param_3;
    if (0x1f < param_3) {
      uVar10 = param_3;
      if ((DAT_0041e010 >> 1 & 1) == 0) {
        uVar11 = param_3;
        if (((uint)puVar16 & 3) != 0) {
          uVar11 = (uint)puVar16 & 3;
          param_3 = param_3 - uVar11;
          do {
            *(undefined *)((int)puVar16 - 1) = *(undefined *)((int)puVar14 + -1);
            puVar14 = (undefined4 *)((int)puVar14 + -1);
            puVar16 = (undefined4 *)((int)puVar16 - 1);
            uVar11 = uVar11 - 1;
          } while (uVar11 != 0);
        }
        uVar10 = param_3;
        if (0x1f < param_3) {
          uVar10 = param_3 >> 2;
          while( true ) {
            if (uVar10 == 0) break;
            uVar10 = uVar10 - 1;
            puVar16[-1] = puVar14[-1];
            puVar14 = puVar14 + -1;
            puVar16 = puVar16 + -1;
          }
          switch(param_3 & 3) {
          case 0:
            return CONCAT44(param_3,param_1) & 0x3ffffffff;
          case 1:
            *(undefined *)((int)puVar16 - 1) = *(undefined *)((int)puVar14 + -1);
            return CONCAT44(param_3,param_1) & 0x3ffffffff;
          case 2:
            *(undefined *)((int)puVar16 - 1) = *(undefined *)((int)puVar14 + -1);
            *(undefined *)((int)puVar16 - 2) = *(undefined *)((int)puVar14 + -2);
            return CONCAT44(param_3,param_1) & 0x3ffffffff;
          case 3:
            *(undefined *)((int)puVar16 - 1) = *(undefined *)((int)puVar14 + -1);
            *(undefined *)((int)puVar16 - 2) = *(undefined *)((int)puVar14 + -2);
            *(undefined *)((int)puVar16 - 3) = *(undefined *)((int)puVar14 + -3);
            return CONCAT44(param_3,param_1) & 0x3ffffffff;
          }
        }
      }
      else {
        while (puVar15 = puVar14, puVar17 = puVar16, ((uint)puVar16 & 0xf) != 0) {
          puVar14 = (undefined4 *)((int)puVar14 + -1);
          puVar16 = (undefined4 *)((int)puVar16 + -1);
          *(undefined *)puVar16 = *(undefined *)puVar14;
          uVar10 = uVar10 - 1;
        }
        do {
          puVar14 = puVar15;
          puVar16 = puVar17;
          if (uVar10 < 0x80) break;
          puVar14 = puVar15 + -0x20;
          puVar16 = puVar17 + -0x20;
                    // WARNING: Load size is inaccurate
                    // WARNING: Load size is inaccurate
          auVar19 = *(undefined *)(puVar15 + -0x1c);
                    // WARNING: Load size is inaccurate
          auVar2 = *(undefined *)(puVar15 + -0x18);
                    // WARNING: Load size is inaccurate
          auVar3 = *(undefined *)(puVar15 + -0x14);
                    // WARNING: Load size is inaccurate
          auVar4 = *(undefined *)(puVar15 + -0x10);
                    // WARNING: Load size is inaccurate
          auVar5 = *(undefined *)(puVar15 + -0xc);
                    // WARNING: Load size is inaccurate
          auVar6 = *(undefined *)(puVar15 + -8);
                    // WARNING: Load size is inaccurate
          auVar7 = *(undefined *)(puVar15 + -4);
                    // WARNING: Store size is inaccurate
          *(undefined *)puVar16 = *(undefined *)puVar14;
                    // WARNING: Store size is inaccurate
          *(undefined *)(puVar17 + -0x1c) = auVar19;
                    // WARNING: Store size is inaccurate
          *(undefined *)(puVar17 + -0x18) = auVar2;
                    // WARNING: Store size is inaccurate
          *(undefined *)(puVar17 + -0x14) = auVar3;
                    // WARNING: Store size is inaccurate
          *(undefined *)(puVar17 + -0x10) = auVar4;
                    // WARNING: Store size is inaccurate
          *(undefined *)(puVar17 + -0xc) = auVar5;
                    // WARNING: Store size is inaccurate
          *(undefined *)(puVar17 + -8) = auVar6;
                    // WARNING: Store size is inaccurate
          *(undefined *)(puVar17 + -4) = auVar7;
          uVar10 = uVar10 - 0x80;
          puVar15 = puVar14;
          puVar17 = puVar16;
        } while ((uVar10 & 0xffffff80) != 0);
        puVar15 = puVar14;
        puVar17 = puVar16;
        uVar11 = param_3;
        if (0x1f < uVar10) {
          do {
            puVar14 = puVar15 + -8;
            puVar16 = puVar17 + -8;
                    // WARNING: Load size is inaccurate
                    // WARNING: Load size is inaccurate
            auVar19 = *(undefined *)(puVar15 + -4);
                    // WARNING: Store size is inaccurate
            *(undefined *)puVar16 = *(undefined *)puVar14;
                    // WARNING: Store size is inaccurate
            *(undefined *)(puVar17 + -4) = auVar19;
            uVar10 = uVar10 - 0x20;
            puVar15 = puVar14;
            puVar17 = puVar16;
            uVar11 = param_3;
          } while ((uVar10 & 0xffffffe0) != 0);
        }
      }
    }
    while ((uVar10 & 0xfffffffc) != 0) {
      puVar16 = puVar16 + -1;
      puVar14 = puVar14 + -1;
      *puVar16 = *puVar14;
      uVar10 = uVar10 - 4;
    }
    while (uVar10 != 0) {
      puVar16 = (undefined4 *)((int)puVar16 - 1);
      puVar14 = (undefined4 *)((int)puVar14 + -1);
      *(undefined *)puVar16 = *(undefined *)puVar14;
      uVar10 = uVar10 - 1;
    }
    return CONCAT44(uVar11,param_1);
  }
  uVar10 = param_3;
  puVar18 = param_1;
  if (0x1f < param_3) {
    if (param_3 < 0x80) {
      uVar10 = param_3;
      puVar18 = param_1;
      if ((DAT_0041e010 >> 1 & 1) != 0) {
LAB_0040551d:
        if (uVar10 == 0) goto LAB_00405580;
        param_3 = uVar10 >> 5;
        while (param_3 != 0) {
                    // WARNING: Load size is inaccurate
                    // WARNING: Load size is inaccurate
          auVar19 = *(undefined *)(param_2 + 2);
                    // WARNING: Store size is inaccurate
          *(undefined *)puVar18 = *(undefined *)param_2;
                    // WARNING: Store size is inaccurate
          *(undefined *)(puVar18 + 2) = auVar19;
          param_2 = param_2 + 4;
          puVar18 = puVar18 + 4;
          param_3 = param_3 - 1;
        }
        goto LAB_0040554b;
      }
LAB_00405277:
      uVar10 = (uint)param_1 & 3;
      puVar18 = param_1;
      while (uVar10 != 0) {
        *(undefined *)puVar18 = *(undefined *)param_2;
        param_3 = param_3 - 1;
        param_2 = (undefined8 *)((int)param_2 + 1);
        puVar18 = (undefined8 *)((int)puVar18 + 1);
        uVar10 = (uint)puVar18 & 3;
      }
    }
    else {
      uVar10 = param_3;
      puVar18 = param_1;
      if ((DAT_0041ec78 >> 1 & 1) != 0) {
        while (uVar10 != 0) {
          uVar10 = uVar10 - 1;
          *(undefined *)puVar18 = *(undefined *)param_2;
          param_2 = (undefined8 *)((int)param_2 + 1);
          puVar18 = (undefined8 *)((int)puVar18 + 1);
        }
        return CONCAT44(param_3,param_1);
      }
      if (((((uint)param_1 ^ (uint)param_2) & 0xf) == 0) && ((DAT_0041e010 >> 1 & 1) != 0)) {
        puVar18 = param_1;
        if (((uint)param_2 & 0xf) != 0) {
          uVar11 = 0x10 - ((uint)param_2 & 0xf);
          param_3 = param_3 - uVar11;
          uVar10 = uVar11 & 3;
          puVar18 = param_1;
          while (uVar10 != 0) {
            *(undefined *)puVar18 = *(undefined *)param_2;
            param_2 = (undefined8 *)((int)param_2 + 1);
            puVar18 = (undefined8 *)((int)puVar18 + 1);
            uVar10 = uVar10 - 1;
          }
          uVar11 = uVar11 >> 2;
          while (uVar11 != 0) {
            *(uint *)puVar18 = *(uint *)param_2;
            param_2 = (undefined8 *)((int)param_2 + 4);
            puVar18 = (undefined8 *)((int)puVar18 + 4);
            uVar11 = uVar11 - 1;
          }
        }
        uVar10 = param_3 & 0x7f;
        param_3 = param_3 >> 7;
        while (param_3 != 0) {
                    // WARNING: Load size is inaccurate
                    // WARNING: Load size is inaccurate
          auVar19 = *(undefined *)(param_2 + 2);
                    // WARNING: Load size is inaccurate
          auVar2 = *(undefined *)(param_2 + 4);
                    // WARNING: Load size is inaccurate
          auVar3 = *(undefined *)(param_2 + 6);
                    // WARNING: Store size is inaccurate
          *(undefined *)puVar18 = *(undefined *)param_2;
                    // WARNING: Store size is inaccurate
          *(undefined *)(puVar18 + 2) = auVar19;
                    // WARNING: Store size is inaccurate
          *(undefined *)(puVar18 + 4) = auVar2;
                    // WARNING: Store size is inaccurate
          *(undefined *)(puVar18 + 6) = auVar3;
                    // WARNING: Load size is inaccurate
                    // WARNING: Load size is inaccurate
          auVar19 = *(undefined *)(param_2 + 10);
                    // WARNING: Load size is inaccurate
          auVar2 = *(undefined *)(param_2 + 0xc);
                    // WARNING: Load size is inaccurate
          auVar3 = *(undefined *)(param_2 + 0xe);
                    // WARNING: Store size is inaccurate
          *(undefined *)(puVar18 + 8) = *(undefined *)(param_2 + 8);
                    // WARNING: Store size is inaccurate
          *(undefined *)(puVar18 + 10) = auVar19;
                    // WARNING: Store size is inaccurate
          *(undefined *)(puVar18 + 0xc) = auVar2;
                    // WARNING: Store size is inaccurate
          *(undefined *)(puVar18 + 0xe) = auVar3;
          param_2 = param_2 + 0x10;
          puVar18 = puVar18 + 0x10;
          param_3 = param_3 - 1;
        }
        goto LAB_0040551d;
      }
      if (((DAT_0041ec78 & 1) == 0) || (((uint)param_1 & 3) != 0)) goto LAB_00405277;
      puVar18 = param_1;
      if (((uint)param_2 & 3) == 0) {
        if (((uint)param_1 >> 2 & 1) != 0) {
          uVar8 = *(undefined4 *)param_2;
          param_3 = param_3 - 4;
          param_2 = (undefined8 *)((int)param_2 + 4);
          *(undefined4 *)param_1 = uVar8;
          param_1 = (undefined8 *)((int)param_1 + 4);
        }
        if (((uint)param_1 >> 3 & 1) != 0) {
          uVar1 = *param_2;
          param_3 = param_3 - 8;
          param_2 = param_2 + 1;
          *param_1 = uVar1;
          param_1 = param_1 + 1;
        }
        if (((uint)param_2 & 7) == 0) {
                    // WARNING: Load size is inaccurate
          puVar18 = param_2 + -1;
          auVar19 = *(undefined *)(param_2 + -1);
          do {
            puVar13 = puVar18;
                    // WARNING: Load size is inaccurate
            auVar2 = *(undefined *)(puVar13 + 2);
            param_3 = param_3 - 0x30;
                    // WARNING: Load size is inaccurate
            auVar3 = *(undefined *)(puVar13 + 4);
                    // WARNING: Load size is inaccurate
            auVar4 = *(undefined *)(puVar13 + 6);
                    // WARNING: Store size is inaccurate
            *(undefined *)param_1 = SUB3216(ZEXT2032(CONCAT416(0x40,auVar19)) >> 0x40,0);
                    // WARNING: Store size is inaccurate
            *(undefined *)(param_1 + 2) = SUB3216(ZEXT2032(CONCAT416(0x40,auVar2)) >> 0x40,0);
                    // WARNING: Store size is inaccurate
            *(undefined *)(param_1 + 4) = SUB3216(ZEXT2032(CONCAT416(0x40,auVar3)) >> 0x40,0);
            param_1 = param_1 + 6;
            puVar18 = puVar13 + 6;
            auVar19 = auVar4;
          } while (0x2f < param_3);
          puVar13 = puVar13 + 7;
        }
        else {
          if (((uint)param_2 >> 3 & 1) == 0) {
                    // WARNING: Load size is inaccurate
            iVar9 = (int)param_2 + -4;
            auVar19 = *(undefined *)((int)param_2 + -4);
            do {
              iVar12 = iVar9;
                    // WARNING: Load size is inaccurate
              auVar2 = *(undefined *)(iVar12 + 0x10);
              param_3 = param_3 - 0x30;
                    // WARNING: Load size is inaccurate
              auVar3 = *(undefined *)(iVar12 + 0x20);
                    // WARNING: Load size is inaccurate
              auVar4 = *(undefined *)(iVar12 + 0x30);
                    // WARNING: Store size is inaccurate
              *(undefined *)param_1 = SUB2016(CONCAT416(0x20,auVar19) >> 0x20,0);
                    // WARNING: Store size is inaccurate
              *(undefined *)(param_1 + 2) = SUB2016(CONCAT416(0x20,auVar2) >> 0x20,0);
                    // WARNING: Store size is inaccurate
              *(undefined *)(param_1 + 4) = SUB2016(CONCAT416(0x20,auVar3) >> 0x20,0);
              param_1 = param_1 + 6;
              iVar9 = iVar12 + 0x30;
              auVar19 = auVar4;
            } while (0x2f < param_3);
            puVar13 = (undefined8 *)(iVar12 + 0x34);
          }
          else {
                    // WARNING: Load size is inaccurate
            iVar9 = (int)param_2 + -0xc;
            auVar19 = *(undefined *)((int)param_2 + -0xc);
            do {
              iVar12 = iVar9;
                    // WARNING: Load size is inaccurate
              auVar2 = *(undefined *)(iVar12 + 0x10);
              param_3 = param_3 - 0x30;
                    // WARNING: Load size is inaccurate
              auVar3 = *(undefined *)(iVar12 + 0x20);
                    // WARNING: Load size is inaccurate
              auVar4 = *(undefined *)(iVar12 + 0x30);
                    // WARNING: Store size is inaccurate
              *(undefined *)param_1 = SUB3216(ZEXT2032(CONCAT416(0x60,auVar19)) >> 0x60,0);
                    // WARNING: Store size is inaccurate
              *(undefined *)(param_1 + 2) = SUB3216(ZEXT2032(CONCAT416(0x60,auVar2)) >> 0x60,0);
                    // WARNING: Store size is inaccurate
              *(undefined *)(param_1 + 4) = SUB3216(ZEXT2032(CONCAT416(0x60,auVar3)) >> 0x60,0);
              param_1 = param_1 + 6;
              iVar9 = iVar12 + 0x30;
              auVar19 = auVar4;
            } while (0x2f < param_3);
            puVar13 = (undefined8 *)(iVar12 + 0x3c);
          }
        }
        while (0xf < param_3) {
                    // WARNING: Load size is inaccurate
          auVar19 = *(undefined *)puVar13;
          param_3 = param_3 - 0x10;
          puVar13 = puVar13 + 2;
                    // WARNING: Store size is inaccurate
          *(undefined *)param_1 = auVar19;
          param_1 = param_1 + 2;
        }
        if ((param_3 >> 2 & 1) != 0) {
          uVar8 = *(undefined4 *)puVar13;
          param_3 = param_3 - 4;
          puVar13 = (undefined8 *)((int)puVar13 + 4);
          *(undefined4 *)param_1 = uVar8;
          param_1 = (undefined8 *)((int)param_1 + 4);
        }
        if ((param_3 >> 3 & 1) != 0) {
          param_3 = param_3 - 8;
          *param_1 = *puVar13;
        }
                    // WARNING: Could not recover jumptable at 0x00405275. Too many branches
                    // WARNING: Treating indirect jump as call
        uVar20 = (*(code *)(&switchdataD_004052b4)[param_3])();
        return uVar20;
      }
    }
    uVar10 = param_3;
    if (0x1f < param_3) {
      uVar10 = param_3 >> 2;
      while (uVar10 != 0) {
        uVar10 = uVar10 - 1;
        *(uint *)puVar18 = *(uint *)param_2;
        param_2 = (undefined8 *)((int)param_2 + 4);
        puVar18 = (undefined8 *)((int)puVar18 + 4);
      }
      switch(param_3 & 3) {
      case 0:
        return CONCAT44(param_3,param_1) & 0x3ffffffff;
      case 1:
        *(undefined *)puVar18 = *(undefined *)param_2;
        return CONCAT44(param_3,param_1) & 0x3ffffffff;
      case 2:
        *(undefined *)puVar18 = *(undefined *)param_2;
        *(undefined *)((int)puVar18 + 1) = *(undefined *)((int)param_2 + 1);
        return CONCAT44(param_3,param_1) & 0x3ffffffff;
      case 3:
        *(undefined *)puVar18 = *(undefined *)param_2;
        *(undefined *)((int)puVar18 + 1) = *(undefined *)((int)param_2 + 1);
        *(undefined *)((int)puVar18 + 2) = *(undefined *)((int)param_2 + 2);
        return CONCAT44(param_3,param_1) & 0x3ffffffff;
      }
    }
  }
LAB_0040554b:
  if ((uVar10 & 0x1f) != 0) {
    uVar11 = (uVar10 & 0x1f) >> 2;
    while (uVar11 != 0) {
      param_3 = *(uint *)param_2;
      *(uint *)puVar18 = param_3;
      puVar18 = (undefined8 *)((int)puVar18 + 4);
      param_2 = (undefined8 *)((int)param_2 + 4);
      uVar11 = uVar11 - 1;
    }
    uVar10 = uVar10 & 3;
    while (uVar10 != 0) {
      *(undefined *)puVar18 = *(undefined *)param_2;
      param_2 = (undefined8 *)((int)param_2 + 1);
      puVar18 = (undefined8 *)((int)puVar18 + 1);
      uVar10 = uVar10 - 1;
    }
  }
LAB_00405580:
  return CONCAT44(param_3,param_1);
}



undefined * __cdecl FUN_0040568c(int param_1)

{
  return &DAT_0041e040 + param_1 * 0x38;
}



// Library Function - Single Match
// Name: __lock_file
// Library: Visual Studio 2015 Release

void __cdecl __lock_file(FILE *_File)

{
  EnterCriticalSection((LPCRITICAL_SECTION)(_File + 1));
  return;
}



void __cdecl FUN_004056fc(int param_1)

{
  LeaveCriticalSection((LPCRITICAL_SECTION)(param_1 + 0x20));
  return;
}



// WARNING: Function: __SEH_prolog4 replaced with injection: SEH_prolog4
// WARNING: Function: __SEH_epilog4 replaced with injection: EH_epilog3
// Library Function - Multiple Matches With Same Base Name
// Name: operator()<>
// Library: Visual Studio 2015 Release

undefined4 operator____(FILE **param_1,FILE **param_2)

{
  undefined4 uVar1;
  
  __lock_file(*param_1);
  uVar1 = FUN_00405fa7(param_2);
  FUN_00405755();
  return uVar1;
}



void FUN_00405755(void)

{
  int unaff_EBP;
  
  FUN_004056fc(**(int **)(unaff_EBP + 0x10));
  return;
}



// WARNING: Type propagation algorithm not settling

void __cdecl
FUN_00405761(uint param_1,undefined4 param_2,undefined4 *******param_3,int param_4,int param_5,
            undefined4 *param_6,undefined4 param_7)

{
  undefined4 *************pppppppppppppuVar1;
  undefined4 *puVar2;
  uint uVar3;
  int local_488;
  undefined local_484 [8];
  char local_47c;
  uint local_478;
  undefined4 *************local_474;
  int local_470;
  int local_46c;
  undefined4 local_468;
  undefined4 *************local_464;
  uint local_460 [16];
  undefined local_420 [1040];
  undefined4 local_10;
  uint local_8;
  
  local_8 = DAT_0041e004 ^ (uint)&stack0xfffffffc;
  local_464 = (undefined4 *************)param_3;
  if ((param_5 == 0) || ((param_4 != 0 && (param_3 == (undefined4 *******)0x0)))) {
    puVar2 = (undefined4 *)FUN_00407f40();
    *puVar2 = 0x16;
    FUN_00407e83();
    goto LAB_004058bc;
  }
  FUN_00405ed6(&local_488,param_6);
  pppppppppppppuVar1 = local_464;
  local_478 = param_1 & 2;
  local_474 = local_464;
  local_470 = param_4;
  local_46c = 0;
  if ((local_478 != 0) || (local_468 = 0, local_464 == (undefined4 *************)0x0)) {
    local_468 = 1;
  }
  local_464 = &local_474;
  output_adapter_data__(local_460,&local_464,param_1,param_2,param_5,local_484,param_7);
  local_10 = 0;
  uVar3 = FUN_004063a9(local_460);
  if (pppppppppppppuVar1 != (undefined4 *************)0x0) {
    if ((param_1 & 1) == 0) {
      if (local_478 == 0) {
        if (param_4 != 0) {
          if (local_46c != param_4) goto LAB_00405891;
LAB_004058e1:
          *(undefined2 *)((int)pppppppppppppuVar1 + param_4 * 2 + -2) = 0;
        }
      }
      else {
        if (param_4 != 0) {
          if (-1 < (int)uVar3) {
            if (local_46c == param_4) goto LAB_004058e1;
            goto LAB_00405891;
          }
          *(undefined2 *)pppppppppppppuVar1 = 0;
        }
      }
    }
    else {
      if (((param_4 != 0) || (uVar3 == 0)) && (local_46c != param_4)) {
LAB_00405891:
        *(undefined2 *)((int)pppppppppppppuVar1 + local_46c * 2) = 0;
      }
    }
  }
  FUN_00405f69((int)local_420);
  if (local_47c != 0) {
    *(uint *)(local_488 + 0x350) = *(uint *)(local_488 + 0x350) & 0xfffffffd;
  }
LAB_004058bc:
  FUN_00401f78();
  return;
}



// Library Function - Single Match
// Name: ??$common_vsprintf_s@_W@@YAH_KQA_WIQB_WQAU__crt_locale_pointers@@QAD@Z
// Library: Visual Studio 2015 Release
// int __cdecl common_vsprintf_s<wchar_t>(unsigned __int64,wchar_t * const,unsigned int,wchar_t
// const * const,struct __crt_locale_pointers * const,char * const)

int __cdecl
common_vsprintf_s_wchar_t_
          (__uint64 param_1,wchar_t *param_2,uint param_3,wchar_t *param_4,
          __crt_locale_pointers *param_5,char *param_6)

{
  undefined4 *puVar1;
  int iVar2;
  
  if (param_4 == (wchar_t *)0x0) {
    puVar1 = (undefined4 *)FUN_00407f40();
    *puVar1 = 0x16;
    FUN_00407e83();
    return -1;
  }
  if ((param_2 == (wchar_t *)0x0) || (param_3 == 0)) {
    puVar1 = (undefined4 *)FUN_00407f40();
    *puVar1 = 0x16;
  }
  else {
    iVar2 = FUN_00405761((uint)param_1,param_1._4_4_,(undefined4 *******)param_2,param_3,
                         (int)param_4,(undefined4 *)param_5,param_6);
    if (iVar2 < 0) {
      *(undefined2 *)param_2 = 0;
    }
    if (iVar2 != -2) {
      return iVar2;
    }
    puVar1 = (undefined4 *)FUN_00407f40();
    *puVar1 = 0x22;
  }
  FUN_00407e83();
  return -1;
}



// Library Function - Single Match
// Name: ??$count@D@formatting_buffer@__crt_stdio_output@@QBEIXZ
// Libraries: Visual Studio 2015 Debug, Visual Studio 2015 Release
// public: unsigned int __thiscall __crt_stdio_output::formatting_buffer::count<char>(void)const 

uint __thiscall count_char_(formatting_buffer *this)

{
  if (*(int *)(this + 0x404) == 0) {
    return 0x200;
  }
  return *(uint *)(this + 0x400) >> 1;
}



uint __fastcall FUN_0040597c(int param_1)

{
  if (*(int *)(param_1 + 0x404) == 0) {
    return 0x100;
  }
  return *(uint *)(param_1 + 0x400) >> 2;
}



uint __thiscall FUN_00405995(void *pvParm1,uint param_1)

{
  undefined4 in_EAX;
  undefined4 *puVar1;
  uint uVar2;
  uint extraout_EAX;
  bool bVar3;
  void *local_8;
  
  if (param_1 < 0x80000000) {
    param_1 = param_1 * 2;
    if (((*(int *)((int)pvParm1 + 0x404) == 0) && (param_1 < 0x401)) ||
       (param_1 < *(uint *)((int)pvParm1 + 0x400) || param_1 == *(uint *)((int)pvParm1 + 0x400))) {
      uVar2 = CONCAT31((int3)((uint)in_EAX >> 8),1);
    }
    else {
      local_8 = pvParm1;
      local_8 = __malloc_base(param_1);
      bVar3 = local_8 != (LPVOID)0x0;
      if (bVar3) {
        operator_((void *)((int)pvParm1 + 0x404),&local_8);
        *(uint *)((int)pvParm1 + 0x400) = param_1;
      }
      FID_conflict__free(local_8);
      uVar2 = extraout_EAX & 0xffffff00 | (uint)bVar3;
    }
  }
  else {
    local_8 = pvParm1;
    puVar1 = (undefined4 *)FUN_00407f40();
    *puVar1 = 0xc;
    uVar2 = (uint)puVar1 & 0xffffff00;
  }
  return uVar2;
}



uint __thiscall FUN_00405a14(void *pvParm1,uint param_1)

{
  undefined4 in_EAX;
  undefined4 *puVar1;
  uint uVar2;
  uint extraout_EAX;
  bool bVar3;
  void *local_8;
  
  if (param_1 < 0x40000000) {
    param_1 = param_1 << 2;
    if (((*(int *)((int)pvParm1 + 0x404) == 0) && (param_1 < 0x401)) ||
       (param_1 < *(uint *)((int)pvParm1 + 0x400) || param_1 == *(uint *)((int)pvParm1 + 0x400))) {
      uVar2 = CONCAT31((int3)((uint)in_EAX >> 8),1);
    }
    else {
      local_8 = pvParm1;
      local_8 = __malloc_base(param_1);
      bVar3 = local_8 != (LPVOID)0x0;
      if (bVar3) {
        operator_((void *)((int)pvParm1 + 0x404),&local_8);
        *(uint *)((int)pvParm1 + 0x400) = param_1;
      }
      FID_conflict__free(local_8);
      uVar2 = extraout_EAX & 0xffffff00 | (uint)bVar3;
    }
  }
  else {
    local_8 = pvParm1;
    puVar1 = (undefined4 *)FUN_00407f40();
    *puVar1 = 0xc;
    uVar2 = (uint)puVar1 & 0xffffff00;
  }
  return uVar2;
}



uint __cdecl FUN_00405a94(undefined4 param_1,undefined4 param_2,char param_3,int param_4)

{
  uint uVar1;
  
  uVar1 = param_4 - 2;
  if (uVar1 != 0) {
    uVar1 = param_4 - 3;
    if ((uVar1 == 0) || (uVar1 = param_4 - 0xc, uVar1 == 0)) {
LAB_00405abd:
      return CONCAT31((int3)(uVar1 >> 8),1);
    }
    if (param_4 != 0xd) {
      uVar1 = uVar1 & 0xffffff00;
      if ((param_3 != 'c') && (param_3 != 's')) goto LAB_00405abd;
    }
  }
  return uVar1 & 0xffffff00;
}



uint __cdecl FUN_00405ac5(uint param_1,undefined4 param_2,short param_3,int param_4)

{
  byte bVar1;
  int iVar2;
  
  if (param_4 == 2) {
    return 0;
  }
  iVar2 = param_4 + -3;
  if (((iVar2 != 0) && (iVar2 = param_4 + -0xc, iVar2 != 0)) && (param_4 != 0xd)) {
    bVar1 = 1;
    if ((param_3 == 99) || (param_3 == 0x73)) {
      bVar1 = 0;
    }
    return (uint)(bVar1 ^ (param_1 & 4) != 0);
  }
  return CONCAT31((int3)((uint)iVar2 >> 8),1);
}



// Library Function - Single Match
// Name: ??$scratch_data@D@formatting_buffer@__crt_stdio_output@@QAEPADXZ
// Libraries: Visual Studio 2015 Debug, Visual Studio 2015 Release
// public: char * __thiscall __crt_stdio_output::formatting_buffer::scratch_data<char>(void)

char * __thiscall scratch_data_char_(formatting_buffer *this)

{
  int iVar1;
  uint uVar2;
  
  iVar1 = *(int *)(this + 0x404);
  uVar2 = count_char_(this);
  if (iVar1 == 0) {
    this = this + uVar2;
  }
  else {
    this = (formatting_buffer *)(uVar2 + iVar1);
  }
  return (char *)this;
}



void __thiscall FUN_00405b31(void *this,uint param_1,uint param_2,byte param_3)

{
  uint uVar1;
  formatting_buffer *pfVar2;
  char cVar3;
  formatting_buffer *pfVar4;
  
  pfVar4 = *(formatting_buffer **)((int)this + 0x444);
  if (*(formatting_buffer **)((int)this + 0x444) == (formatting_buffer *)0x0) {
    pfVar4 = (formatting_buffer *)((int)this + 0x40);
  }
  uVar1 = count_char_((formatting_buffer *)((int)this + 0x40));
  pfVar4 = pfVar4 + (uVar1 - 1);
  *(formatting_buffer **)((int)this + 0x34) = pfVar4;
  pfVar2 = pfVar4;
  while ((0 < *(int *)((int)this + 0x28) || (param_1 != 0))) {
    *(int *)((int)this + 0x28) = *(int *)((int)this + 0x28) + -1;
    cVar3 = (char)(param_1 % param_2) + '0';
    if ('9' < cVar3) {
      cVar3 = cVar3 + (param_3 ^ 1) * ' ' + '\a';
    }
    **(char **)((int)this + 0x34) = cVar3;
    *(int *)((int)this + 0x34) = *(int *)((int)this + 0x34) + -1;
    pfVar2 = *(formatting_buffer **)((int)this + 0x34);
    param_1 = param_1 / param_2;
  }
  *(formatting_buffer **)((int)this + 0x38) = pfVar4 + -(int)pfVar2;
  *(int *)((int)this + 0x34) = *(int *)((int)this + 0x34) + 1;
  return;
}



void __thiscall FUN_00405ba4(void *this,uint param_1,uint param_2,byte param_3)

{
  char cVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  
  iVar3 = *(int *)((int)this + 0x444);
  if (*(int *)((int)this + 0x444) == 0) {
    iVar3 = (int)this + 0x40;
  }
  uVar2 = FUN_0040597c((int)this + 0x40);
  iVar4 = iVar3 + uVar2 * 2 + -2;
  *(int *)((int)this + 0x34) = iVar4;
  iVar3 = iVar4;
  while ((0 < *(int *)((int)this + 0x28) || (param_1 != 0))) {
    *(int *)((int)this + 0x28) = *(int *)((int)this + 0x28) + -1;
    uVar2 = param_1 % param_2 + 0x30 & 0xffff;
    cVar1 = (char)uVar2;
    if (0x39 < uVar2) {
      cVar1 = (param_3 ^ 1) * ' ' + '\a' + cVar1;
    }
    **(short **)((int)this + 0x34) = (short)cVar1;
    *(int *)((int)this + 0x34) = *(int *)((int)this + 0x34) + -2;
    iVar3 = *(int *)((int)this + 0x34);
    param_1 = param_1 / param_2;
  }
  *(int *)((int)this + 0x38) = iVar4 - iVar3 >> 1;
  *(int *)((int)this + 0x34) = *(int *)((int)this + 0x34) + 2;
  return;
}



void __thiscall
FUN_00405c2b(void *this,undefined4 param_1,undefined4 param_2,uint param_3,byte param_4)

{
  uint uVar1;
  char extraout_CL;
  char cVar2;
  formatting_buffer *pfVar3;
  formatting_buffer *pfVar4;
  longlong lVar5;
  
  pfVar4 = *(formatting_buffer **)((int)this + 0x444);
  if (*(formatting_buffer **)((int)this + 0x444) == (formatting_buffer *)0x0) {
    pfVar4 = (formatting_buffer *)((int)this + 0x40);
  }
  uVar1 = count_char_((formatting_buffer *)((int)this + 0x40));
  pfVar4 = pfVar4 + (uVar1 - 1);
  *(formatting_buffer **)((int)this + 0x34) = pfVar4;
  lVar5 = CONCAT44(param_2,param_1);
  pfVar3 = pfVar4;
  while( true ) {
    if ((*(int *)((int)this + 0x28) < 1) && (lVar5 == 0)) break;
    *(int *)((int)this + 0x28) = *(int *)((int)this + 0x28) + -1;
    lVar5 = __aulldvrm((uint)lVar5,(uint)((ulonglong)lVar5 >> 0x20),param_3,0);
    cVar2 = extraout_CL + '0';
    if ('9' < cVar2) {
      cVar2 = cVar2 + (param_4 ^ 1) * ' ' + '\a';
    }
    **(char **)((int)this + 0x34) = cVar2;
    *(int *)((int)this + 0x34) = *(int *)((int)this + 0x34) + -1;
    pfVar3 = *(formatting_buffer **)((int)this + 0x34);
  }
  *(formatting_buffer **)((int)this + 0x38) = pfVar4 + -(int)pfVar3;
  *(int *)((int)this + 0x34) = *(int *)((int)this + 0x34) + 1;
  return;
}



void __thiscall
FUN_00405cbb(void *this,undefined4 param_1,undefined4 param_2,uint param_3,byte param_4)

{
  char cVar1;
  uint uVar2;
  int iVar3;
  int extraout_ECX;
  int iVar4;
  longlong lVar5;
  
  iVar3 = *(int *)((int)this + 0x444);
  if (*(int *)((int)this + 0x444) == 0) {
    iVar3 = (int)this + 0x40;
  }
  uVar2 = FUN_0040597c((int)this + 0x40);
  iVar4 = iVar3 + uVar2 * 2 + -2;
  *(int *)((int)this + 0x34) = iVar4;
  lVar5 = CONCAT44(param_2,param_1);
  iVar3 = iVar4;
  while( true ) {
    if ((*(int *)((int)this + 0x28) < 1) && (lVar5 == 0)) break;
    *(int *)((int)this + 0x28) = *(int *)((int)this + 0x28) + -1;
    lVar5 = __aulldvrm((uint)lVar5,(uint)((ulonglong)lVar5 >> 0x20),param_3,0);
    uVar2 = extraout_ECX + 0x30U & 0xffff;
    cVar1 = (char)uVar2;
    if (0x39 < uVar2) {
      cVar1 = (param_4 ^ 1) * ' ' + '\a' + cVar1;
    }
    **(short **)((int)this + 0x34) = (short)cVar1;
    *(int *)((int)this + 0x34) = *(int *)((int)this + 0x34) + -2;
    iVar3 = *(int *)((int)this + 0x34);
  }
  *(int *)((int)this + 0x38) = iVar4 - iVar3 >> 1;
  *(int *)((int)this + 0x34) = *(int *)((int)this + 0x34) + 2;
  return;
}



void __cdecl FUN_00405d5f(void *param_1,byte param_2,int param_3,int *param_4)

{
  uint uVar1;
  int iVar2;
  int iVar3;
  
  iVar3 = 0;
  if (0 < param_3) {
    do {
      uVar1 = FUN_0040792f(param_1,param_2);
      if ((char)uVar1 == 0) {
        *param_4 = -1;
        iVar2 = -1;
      }
      else {
        *param_4 = *param_4 + 1;
        iVar2 = *param_4;
      }
    } while ((iVar2 != -1) && (iVar3 = iVar3 + 1, iVar3 < param_3));
  }
  return;
}



// Library Function - Single Match
// Name: 
??$write_multiple_characters@V?$string_output_adapter@_W@__crt_stdio_output@@D@__crt_stdio_output@@YAXABV?$string_output_adapter@_W@0@DHQAH@Z
// Library: Visual Studio 2015 Release
// void __cdecl __crt_stdio_output::write_multiple_characters<class
// __crt_stdio_output::string_output_adapter<wchar_t>,char>(class
// __crt_stdio_output::string_output_adapter<wchar_t> const &,char,int,int * const)

void __cdecl
write_multiple_characters_class___crt_stdio_output__string_output_adapter_wchar_t__char_
          (string_output_adapter_wchar_t_ *param_1,char param_2,int param_3,int *param_4)

{
  int iVar1;
  
  iVar1 = 0;
  if (0 < param_3) {
    do {
      write_character(param_1,(wchar_t)param_2,param_4);
      if (*param_4 == -1) {
        return;
      }
      iVar1 = iVar1 + 1;
    } while (iVar1 < param_3);
  }
  return;
}



// Library Function - Single Match
// Name: ??0?$common_data@D@__crt_stdio_output@@IAE@XZ
// Library: Visual Studio 2015 Release
// protected: __thiscall __crt_stdio_output::common_data<char>::common_data<char>(void)

void __thiscall common_data_char_(common_data_char_ *this)

{
  *(undefined4 *)this = 0;
  *(undefined4 *)(this + 4) = 0;
  *(undefined4 *)(this + 8) = 0;
  *(undefined4 *)(this + 0xc) = 0;
  *(undefined4 *)(this + 0x10) = 0;
  *(undefined4 *)(this + 0x14) = 0;
  *(undefined4 *)(this + 0x18) = 0;
  *(undefined4 *)(this + 0x1c) = 0;
  *(undefined4 *)(this + 0x20) = 0;
  *(undefined4 *)(this + 0x24) = 0;
  *(undefined4 *)(this + 0x28) = 0;
  *(undefined2 *)(this + 0x30) = 0;
  *(undefined4 *)(this + 0x38) = 0;
  this[0x3c] = (common_data_char_)0x0;
  *(undefined4 *)(this + 0x440) = 0;
  *(undefined4 *)(this + 0x444) = 0;
  return;
}



undefined4 * __fastcall FUN_00405e12(undefined4 *param_1)

{
  *param_1 = 0;
  param_1[1] = 0;
  param_1[2] = 0;
  param_1[3] = 0;
  *(undefined2 *)((int)param_1 + 0x32) = 0;
  param_1[4] = 0;
  param_1[5] = 0;
  param_1[6] = 0;
  param_1[7] = 0;
  param_1[8] = 0;
  param_1[9] = 0;
  param_1[10] = 0;
  *(undefined *)(param_1 + 0xc) = 0;
  param_1[0xe] = 0;
  *(undefined *)(param_1 + 0xf) = 0;
  param_1[0x110] = 0;
  param_1[0x111] = 0;
  return param_1;
}



// Library Function - Multiple Matches With Same Base Name
// Name: output_adapter_data<>
// Library: Visual Studio 2015 Release

undefined4 * __thiscall
output_adapter_data__
          (void *this,undefined4 *param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
          undefined4 param_5,undefined4 param_6)

{
  common_data_char_((common_data_char_ *)this);
  *(undefined4 *)((int)this + 0x448) = *param_1;
  *(undefined4 *)this = param_2;
  *(undefined4 *)((int)this + 4) = param_3;
  *(undefined4 *)((int)this + 8) = param_5;
  *(undefined4 *)((int)this + 0x10) = param_4;
  *(undefined4 *)((int)this + 0x14) = param_6;
  return (undefined4 *)this;
}



// Library Function - Multiple Matches With Same Base Name
// Name: output_adapter_data<>
// Library: Visual Studio 2015 Release

undefined4 * __thiscall
output_adapter_data__
          (void *this,undefined4 *param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
          undefined4 param_5,undefined4 param_6)

{
  FUN_00405e12((undefined4 *)this);
  *(undefined4 *)((int)this + 0x448) = *param_1;
  *(undefined4 *)this = param_2;
  *(undefined4 *)((int)this + 4) = param_3;
  *(undefined4 *)((int)this + 8) = param_5;
  *(undefined4 *)((int)this + 0x10) = param_4;
  *(undefined4 *)((int)this + 0x14) = param_6;
  return (undefined4 *)this;
}



uint * __thiscall FUN_00405ed6(void *this,undefined4 *param_1)

{
  undefined **ppuVar1;
  undefined *puVar2;
  uint uVar3;
  
  *(undefined *)((int)this + 0xc) = 0;
  ppuVar1 = (undefined **)((int)this + 4);
  if (param_1 == (undefined4 *)0x0) {
    if (DAT_0041ef80 != 0) {
      uVar3 = FUN_0040b5c7();
      *(uint *)this = uVar3;
      *ppuVar1 = *(undefined **)(uVar3 + 0x4c);
      *(int *)((int)this + 8) = *(int *)(uVar3 + 0x48);
      ___acrt_update_locale_info(uVar3,ppuVar1);
      FUN_0040b846(*(int *)this,(int *)((int)this + 8));
      uVar3 = *(uint *)(*(int *)this + 0x350);
      if ((uVar3 & 2) != 0) {
        return (uint *)this;
      }
      *(uint *)(*(int *)this + 0x350) = uVar3 | 2;
      *(undefined *)((int)this + 0xc) = 1;
      return (uint *)this;
    }
    *ppuVar1 = PTR_PTR_DAT_0041e1f8;
    puVar2 = PTR_DAT_0041e1fc;
  }
  else {
    *ppuVar1 = (undefined *)*param_1;
    puVar2 = (undefined *)param_1[1];
  }
  *(undefined **)((int)this + 8) = puVar2;
  return (uint *)this;
}



void __fastcall FUN_00405f69(int param_1)

{
  FID_conflict__free(*(void **)(param_1 + 0x404));
  *(undefined4 *)(param_1 + 0x404) = 0;
  return;
}



// Library Function - Multiple Matches With Same Base Name
// Name: operator=
// Library: Visual Studio 2015 Release

void ** __thiscall operator_(void *this,void **param_1)

{
  FID_conflict__free(*(void **)this);
  *(undefined4 *)this = 0;
  *(void **)this = *param_1;
  *param_1 = (void *)0x0;
  return (void **)this;
}



void __fastcall FUN_00405fa7(FILE **param_1)

{
  FILE *pFVar1;
  uint uVar2;
  int local_478;
  undefined local_474 [8];
  char local_46c;
  uint local_468;
  char *local_464;
  uint local_460 [16];
  undefined local_420 [1040];
  undefined4 local_10;
  uint local_8;
  
  local_8 = DAT_0041e004 ^ (uint)&stack0xfffffffc;
  pFVar1 = (FILE *)(*param_1)->_ptr;
  uVar2 = FUN_0040c2cc(pFVar1);
  local_468 = local_468 & 0xffffff00 | uVar2 & 0xff;
  FUN_00405ed6(&local_478,(undefined4 *)param_1[1]->_ptr);
  local_464 = (*param_1)->_ptr;
  output_adapter_data__
            (local_460,&local_464,param_1[2]->_ptr,param_1[2]->_cnt,param_1[3]->_ptr,local_474,
             param_1[4]->_ptr);
  local_10 = 0;
  FUN_00406295(local_460);
  FUN_00405f69((int)local_420);
  if (local_46c != 0) {
    *(uint *)(local_478 + 0x350) = *(uint *)(local_478 + 0x350) & 0xfffffffd;
  }
  ___acrt_stdio_end_temporary_buffering_nolock((char)local_468,pFVar1);
  FUN_00401f78();
  return;
}



void __cdecl FUN_00406068(char *param_1,int *param_2)

{
  char cVar1;
  char *pcVar2;
  
  cVar1 = *param_1;
  while ((cVar1 != 0 && (cVar1 != ***(char ***)(*param_2 + 0x88)))) {
    param_1 = param_1 + 1;
    cVar1 = *param_1;
  }
  if (cVar1 != 0) {
    do {
      param_1 = param_1 + 1;
      cVar1 = *param_1;
      pcVar2 = param_1;
      if ((cVar1 == 0) || (pcVar2 = param_1, cVar1 == 'e')) break;
      pcVar2 = param_1;
    } while (cVar1 != 'E');
    do {
      pcVar2 = pcVar2;
      pcVar2 = pcVar2 + -1;
    } while (*pcVar2 == '0');
    if (*pcVar2 == ***(char ***)(*param_2 + 0x88)) {
      pcVar2 = pcVar2 + -2;
    }
    do {
      cVar1 = *param_1;
      pcVar2 = pcVar2 + 1;
      param_1 = param_1 + 1;
      *pcVar2 = cVar1;
    } while (cVar1 != 0);
  }
  return;
}



uint FUN_004060c8(char param_1,int param_2)

{
  uint uVar1;
  
  if ((byte)(param_1 - 0x20U) < 0x5b) {
    uVar1 = (uint)*(byte *)((int)&DAT_00417ab8 + (int)param_1) & 0xf;
  }
  else {
    uVar1 = 0;
  }
  return (uint)((byte)(&DAT_00417ad8)[uVar1 * 8 + param_2] >> 4);
}



uint FUN_00406107(ushort param_1,int param_2)

{
  uint uVar1;
  
  if ((ushort)(param_1 - 0x20) < 0x5b) {
    uVar1 = (uint)(byte)(&DAT_00417b18)[(uint)param_1] & 0xf;
  }
  else {
    uVar1 = 0;
  }
  return (uint)((byte)(&DAT_00417b38)[param_2 + uVar1 * 9] >> 4);
}



void __cdecl FUN_0040614b(byte *param_1,int *param_2)

{
  byte bVar1;
  uint uVar2;
  int iVar3;
  byte bVar4;
  bool bVar5;
  
  uVar2 = FUN_0040ac85((int)(char)*param_1);
  bVar5 = uVar2 == 0x65;
  while (!bVar5) {
    param_1 = param_1 + 1;
    iVar3 = FUN_0040aad7((uint)*param_1);
    bVar5 = iVar3 == 0;
  }
  uVar2 = FUN_0040ac85((int)(char)*param_1);
  if (uVar2 == 0x78) {
    param_1 = param_1 + 2;
  }
  bVar4 = *param_1;
  *param_1 = ***(byte ***)(*param_2 + 0x88);
  do {
    param_1 = param_1 + 1;
    bVar1 = *param_1;
    *param_1 = bVar4;
    bVar5 = bVar4 != 0;
    bVar4 = bVar1;
  } while (bVar5);
  return;
}



uint __thiscall FUN_004061a7(void *this,long *param_1)

{
  int iVar1;
  long lVar2;
  uint uVar3;
  int *piVar4;
  int *piVar5;
  int *local_8;
  
  piVar5 = *(int **)((int)this + 0xc);
  if (piVar5 == (int *)0x0) {
    local_8 = (int *)this;
    piVar5 = (int *)FUN_00407f40();
    *(int **)((int)this + 0xc) = piVar5;
  }
  iVar1 = *piVar5;
  *piVar5 = 0;
  local_8 = (int *)0x0;
  lVar2 = FID_conflict__wcstol((wchar_t *)(*(int *)((int)this + 0x10) + -1),(wchar_t **)&local_8,10)
  ;
  *param_1 = lVar2;
  piVar4 = *(int **)((int)this + 0xc);
  if (piVar4 == (int *)0x0) {
    piVar4 = (int *)FUN_00407f40();
    *(int **)((int)this + 0xc) = piVar4;
  }
  if ((*piVar4 == 0x22) || (piVar4 = local_8, local_8 < *(int **)((int)this + 0x10))) {
    uVar3 = (uint)piVar4 & 0xffffff00;
  }
  else {
    *(int **)((int)this + 0x10) = local_8;
    uVar3 = CONCAT31((int3)((uint)local_8 >> 8),1);
  }
  if ((*piVar5 == 0) && (iVar1 != 0)) {
    *piVar5 = iVar1;
  }
  return uVar3;
}



uint __thiscall FUN_0040621d(void *this,long *param_1)

{
  int iVar1;
  long lVar2;
  uint uVar3;
  int *piVar4;
  int *piVar5;
  int *local_8;
  
  piVar5 = *(int **)((int)this + 0xc);
  if (piVar5 == (int *)0x0) {
    local_8 = (int *)this;
    piVar5 = (int *)FUN_00407f40();
    *(int **)((int)this + 0xc) = piVar5;
  }
  iVar1 = *piVar5;
  *piVar5 = 0;
  local_8 = (int *)0x0;
  lVar2 = FID_conflict__wcstol((wchar_t *)(*(int *)((int)this + 0x10) + -2),(wchar_t **)&local_8,10)
  ;
  *param_1 = lVar2;
  piVar4 = *(int **)((int)this + 0xc);
  if (piVar4 == (int *)0x0) {
    piVar4 = (int *)FUN_00407f40();
    *(int **)((int)this + 0xc) = piVar4;
  }
  if ((*piVar4 == 0x22) || (piVar4 = local_8, local_8 < *(int **)((int)this + 0x10))) {
    uVar3 = (uint)piVar4 & 0xffffff00;
  }
  else {
    *(int **)((int)this + 0x10) = local_8;
    uVar3 = CONCAT31((int3)((uint)local_8 >> 8),1);
  }
  if ((*piVar5 == 0) && (iVar1 != 0)) {
    *piVar5 = iVar1;
  }
  return uVar3;
}



uint __fastcall FUN_00406295(uint *param_1)

{
  char cVar1;
  uint uVar2;
  undefined4 *puVar3;
  undefined4 uVar4;
  
  uVar2 = FUN_00407805((FILE **)(param_1 + 0x112));
  if ((char)uVar2 != 0) {
    if (param_1[4] != 0) {
LAB_0040636e:
      do {
        param_1[0x114] = param_1[0x114] + 1;
        if (param_1[0x114] == 2) {
          return param_1[6];
        }
        param_1[0xe] = 0;
        param_1[7] = 0;
LAB_0040635b:
        cVar1 = *(char *)param_1[4];
        *(char *)((int)param_1 + 0x31) = cVar1;
        if (cVar1 != 0) {
          param_1[4] = param_1[4] + 1;
          if ((int)param_1[6] < 0) goto LAB_0040636e;
          uVar2 = FUN_004060c8(*(char *)((int)param_1 + 0x31),param_1[7]);
          param_1[7] = uVar2;
          switch(uVar2) {
          case 0:
            uVar2 = FUN_0040654b((int)param_1);
            cVar1 = (char)uVar2;
            break;
          case 1:
            param_1[10] = 0xffffffff;
            param_1[9] = 0;
            *(undefined *)(param_1 + 0xc) = 0;
            param_1[8] = 0;
            param_1[0xb] = 0;
            *(undefined *)(param_1 + 0xf) = 0;
            goto LAB_0040635b;
          case 2:
            uVar4 = state_case_flag((int)param_1);
            cVar1 = (char)uVar4;
            break;
          case 3:
            uVar2 = FUN_00406dfb(param_1);
            cVar1 = (char)uVar2;
            break;
          case 4:
            goto switchD_004062fe_caseD_4;
          case 5:
            uVar2 = FUN_0040664d(param_1);
            cVar1 = (char)uVar2;
            break;
          case 6:
            cVar1 = FUN_0040669a();
            break;
          case 7:
            uVar2 = FUN_00406960(param_1);
            cVar1 = (char)uVar2;
            break;
          case 0xbad1abe1:
            goto LAB_004062c8;
          default:
            goto switchD_004062fe_caseD_9;
          }
          if (cVar1 == 0) {
            return 0xffffffff;
          }
          goto LAB_0040635b;
        }
        param_1[4] = param_1[4] + 1;
      } while( true );
    }
switchD_004062fe_caseD_9:
    puVar3 = (undefined4 *)FUN_00407f40();
    *puVar3 = 0x16;
    FUN_00407e83();
  }
LAB_004062c8:
  return 0xffffffff;
switchD_004062fe_caseD_4:
  param_1[10] = 0;
  goto LAB_0040635b;
}



uint __fastcall FUN_004063a9(uint *param_1)

{
  short sVar1;
  char cVar2;
  undefined4 *puVar3;
  uint uVar4;
  undefined4 uVar5;
  
  cVar2 = FUN_00407826();
  if (cVar2 != 0) {
    if (param_1[4] == 0) {
switchD_00406413_caseD_9:
      puVar3 = (undefined4 *)FUN_00407f40();
      *puVar3 = 0x16;
      FUN_00407e83();
    }
    else {
      do {
        param_1[0x114] = param_1[0x114] + 1;
        if (param_1[0x114] == 2) {
          return param_1[6];
        }
        param_1[0xe] = 0;
        param_1[7] = 0;
LAB_00406470:
        sVar1 = *(short *)param_1[4];
        *(short *)((int)param_1 + 0x32) = sVar1;
        if (sVar1 != 0) {
          param_1[4] = param_1[4] + 2;
          if ((int)param_1[6] < 0) goto LAB_00406487;
          uVar4 = FUN_00406107(*(ushort *)((int)param_1 + 0x32),param_1[7]);
          param_1[7] = uVar4;
          switch(uVar4) {
          case 0:
            uVar4 = state_case_normal((int)param_1);
            cVar2 = (char)uVar4;
            break;
          case 1:
            param_1[10] = 0xffffffff;
            param_1[9] = 0;
            *(undefined *)(param_1 + 0xc) = 0;
            param_1[8] = 0;
            param_1[0xb] = 0;
            *(undefined *)(param_1 + 0xf) = 0;
            goto LAB_00406470;
          case 2:
            uVar5 = state_case_flag((int)param_1);
            cVar2 = (char)uVar5;
            break;
          case 3:
            uVar4 = FUN_00406e26(param_1);
            cVar2 = (char)uVar4;
            break;
          case 4:
            goto switchD_00406413_caseD_4;
          case 5:
            uVar4 = FUN_00406673(param_1);
            cVar2 = (char)uVar4;
            break;
          case 6:
            uVar4 = FUN_004067e8(param_1);
            cVar2 = (char)uVar4;
            break;
          case 7:
            cVar2 = FUN_00406b94(param_1);
            break;
          case 0xbad1abe1:
            goto LAB_004063dc;
          default:
            goto switchD_00406413_caseD_9;
          }
          if (cVar2 == 0) {
            return 0xffffffff;
          }
          goto LAB_00406470;
        }
        param_1[4] = param_1[4] + 2;
LAB_00406487:
        cVar2 = FUN_00407841();
      } while (cVar2 != 0);
    }
  }
LAB_004063dc:
  return 0xffffffff;
switchD_00406413_caseD_4:
  param_1[10] = 0;
  goto LAB_00406470;
}



// Library Function - Multiple Matches With Same Base Name
// Name: state_case_flag
// Libraries: Visual Studio 2015 Debug, Visual Studio 2015 Release

undefined4 __fastcall state_case_flag(int param_1)

{
  int iVar1;
  int iVar2;
  
  iVar1 = (int)*(char *)(param_1 + 0x31);
  iVar2 = iVar1 + -0x20;
  if (iVar2 == 0) {
    *(uint *)(param_1 + 0x20) = *(uint *)(param_1 + 0x20) | 2;
  }
  else {
    iVar2 = iVar1 + -0x23;
    if (iVar2 == 0) {
      *(uint *)(param_1 + 0x20) = *(uint *)(param_1 + 0x20) | 0x20;
    }
    else {
      iVar2 = iVar1 + -0x2b;
      if (iVar2 == 0) {
        *(uint *)(param_1 + 0x20) = *(uint *)(param_1 + 0x20) | 1;
      }
      else {
        iVar2 = iVar1 + -0x2d;
        if (iVar2 == 0) {
          *(uint *)(param_1 + 0x20) = *(uint *)(param_1 + 0x20) | 4;
        }
        else {
          iVar2 = iVar1 + -0x30;
          if (iVar2 == 0) {
            *(uint *)(param_1 + 0x20) = *(uint *)(param_1 + 0x20) | 8;
          }
        }
      }
    }
  }
  return CONCAT31((int3)((uint)iVar2 >> 8),1);
}



// Library Function - Multiple Matches With Same Base Name
// Name: state_case_flag
// Libraries: Visual Studio 2015 Debug, Visual Studio 2015 Release

undefined4 __fastcall state_case_flag(int param_1)

{
  uint uVar1;
  int iVar2;
  
  uVar1 = (uint)*(ushort *)(param_1 + 0x32);
  iVar2 = uVar1 - 0x20;
  if (iVar2 == 0) {
    *(uint *)(param_1 + 0x20) = *(uint *)(param_1 + 0x20) | 2;
  }
  else {
    iVar2 = uVar1 - 0x23;
    if (iVar2 == 0) {
      *(uint *)(param_1 + 0x20) = *(uint *)(param_1 + 0x20) | 0x20;
    }
    else {
      iVar2 = uVar1 - 0x2b;
      if (iVar2 == 0) {
        *(uint *)(param_1 + 0x20) = *(uint *)(param_1 + 0x20) | 1;
      }
      else {
        iVar2 = uVar1 - 0x2d;
        if (iVar2 == 0) {
          *(uint *)(param_1 + 0x20) = *(uint *)(param_1 + 0x20) | 4;
        }
        else {
          iVar2 = uVar1 - 0x30;
          if (iVar2 == 0) {
            *(uint *)(param_1 + 0x20) = *(uint *)(param_1 + 0x20) | 8;
          }
        }
      }
    }
  }
  return CONCAT31((int3)((uint)iVar2 >> 8),1);
}



uint __fastcall FUN_0040654b(int param_1)

{
  uint uVar1;
  undefined4 *puVar2;
  
  uVar1 = FUN_00406589(param_1);
  if ((char)uVar1 == 0) {
    puVar2 = (undefined4 *)FUN_00407f40();
    *puVar2 = 0x16;
    uVar1 = FUN_00407e83();
    return uVar1 & 0xffffff00;
  }
  return CONCAT31((int3)(uVar1 >> 8),1);
}



// Library Function - Multiple Matches With Same Base Name
// Name: state_case_normal
// Library: Visual Studio 2015 Release

uint __fastcall state_case_normal(int param_1)

{
  undefined4 uVar1;
  undefined4 *puVar2;
  uint uVar3;
  
  uVar1 = state_case_normal_common(param_1);
  if ((char)uVar1 == 0) {
    puVar2 = (undefined4 *)FUN_00407f40();
    *puVar2 = 0x16;
    uVar3 = FUN_00407e83();
    return uVar3 & 0xffffff00;
  }
  return CONCAT31((int3)((uint)uVar1 >> 8),1);
}



uint __fastcall FUN_00406589(int param_1)

{
  uint uVar1;
  
  uVar1 = FUN_004065e0(param_1);
  if ((char)uVar1 != 0) {
    uVar1 = FUN_0040792f((void *)(param_1 + 0x448),*(byte *)(param_1 + 0x31));
    if ((char)uVar1 == 0) {
      *(undefined4 *)(param_1 + 0x18) = 0xffffffff;
    }
    else {
      *(int *)(param_1 + 0x18) = *(int *)(param_1 + 0x18) + 1;
    }
    uVar1 = CONCAT31((int3)(uVar1 >> 8),1);
  }
  return uVar1;
}



// Library Function - Multiple Matches With Same Base Name
// Name: state_case_normal_common
// Library: Visual Studio 2015 Release

undefined __fastcall state_case_normal_common(int param_1)

{
  *(undefined *)(param_1 + 0x3c) = 1;
  write_character((string_output_adapter_wchar_t_ *)(param_1 + 0x448),
                  SUB21(*(undefined2 *)(param_1 + 0x32),0),(int *)(param_1 + 0x18));
  return 1;
}



uint __fastcall FUN_004065e0(int param_1)

{
  byte bVar1;
  char cVar2;
  uint uVar3;
  char *pcVar4;
  undefined4 *puVar5;
  
  bVar1 = *(byte *)(param_1 + 0x31);
  *(undefined *)(param_1 + 0x3c) = 0;
  pcVar4 = (char *)FUN_00407be2(***(int ***)(param_1 + 8),(int)(char)bVar1,0x8000);
  if (pcVar4 != (char *)0x0) {
    uVar3 = FUN_0040792f((void *)(param_1 + 0x448),bVar1);
    if ((char)uVar3 == 0) {
      *(undefined4 *)(param_1 + 0x18) = 0xffffffff;
    }
    else {
      *(int *)(param_1 + 0x18) = *(int *)(param_1 + 0x18) + 1;
    }
    cVar2 = **(char **)(param_1 + 0x10);
    pcVar4 = *(char **)(param_1 + 0x10) + 1;
    *(char *)(param_1 + 0x31) = cVar2;
    *(char **)(param_1 + 0x10) = pcVar4;
    if (cVar2 == 0) {
      puVar5 = (undefined4 *)FUN_00407f40();
      *puVar5 = 0x16;
      uVar3 = FUN_00407e83();
      return uVar3 & 0xffffff00;
    }
  }
  return CONCAT31((int3)((uint)pcVar4 >> 8),1);
}



uint __fastcall FUN_0040664d(void *param_1)

{
  long *plVar1;
  int iVar2;
  uint uVar3;
  
  plVar1 = (long *)((int)param_1 + 0x28);
  if (*(char *)((int)param_1 + 0x31) != '*') {
    uVar3 = FUN_004061a7(param_1,plVar1);
    return uVar3;
  }
  *(int *)((int)param_1 + 0x14) = *(int *)((int)param_1 + 0x14) + 4;
  iVar2 = *(int *)(*(int *)((int)param_1 + 0x14) + -4);
  *plVar1 = iVar2;
  if (iVar2 < 0) {
    *plVar1 = -1;
  }
  return CONCAT31((int3)((uint)iVar2 >> 8),1);
}



uint __fastcall FUN_00406673(void *param_1)

{
  long *plVar1;
  int iVar2;
  uint uVar3;
  
  plVar1 = (long *)((int)param_1 + 0x28);
  if (*(short *)((int)param_1 + 0x32) != 0x2a) {
    uVar3 = FUN_0040621d(param_1,plVar1);
    return uVar3;
  }
  *(int *)((int)param_1 + 0x14) = *(int *)((int)param_1 + 0x14) + 4;
  iVar2 = *(int *)(*(int *)((int)param_1 + 0x14) + -4);
  *plVar1 = iVar2;
  if (iVar2 < 0) {
    *plVar1 = -1;
  }
  return CONCAT31((int3)((uint)iVar2 >> 8),1);
}



uint __fastcall FUN_0040669a(uint *puParm1)

{
  char cVar1;
  char *pcVar2;
  uint in_EAX;
  uint uVar3;
  undefined4 *puVar4;
  char *pcStack4;
  
  cVar1 = *(char *)((int)puParm1 + 0x31);
  pcStack4 = (char *)(in_EAX & 0xffffff00);
  if (cVar1 == 'F') {
    pcStack4 = (char *)(*puParm1 & 8);
    if (pcStack4 == (char *)0x0) {
      puParm1[7] = 7;
      uVar3 = FUN_00406960(puParm1);
      return uVar3;
    }
  }
  else {
    if (cVar1 == 'N') {
      pcStack4 = (char *)(*puParm1 & 8);
      if (pcStack4 == (char *)0x0) {
        puParm1[7] = 8;
        goto LAB_004066d2;
      }
    }
    else {
      if (puParm1[0xb] != 0) {
LAB_004066d2:
        puVar4 = (undefined4 *)FUN_00407f40();
        *puVar4 = 0x16;
        uVar3 = FUN_00407e83();
        return uVar3 & 0xffffff00;
      }
      if (cVar1 < 'k') {
        if (cVar1 == 'j') {
          puParm1[0xb] = 5;
          goto LAB_004067e5;
        }
        if (cVar1 == 'I') {
          pcVar2 = (char *)puParm1[4];
          cVar1 = *pcVar2;
          if ((cVar1 == '3') && (pcVar2[1] == '2')) {
            pcStack4 = pcVar2 + 2;
            puParm1[0xb] = 10;
            *(char **)(puParm1 + 4) = pcStack4;
          }
          else {
            if ((cVar1 == '6') && (pcVar2[1] == '4')) {
              pcStack4 = pcVar2 + 2;
              puParm1[0xb] = 0xb;
              *(char **)(puParm1 + 4) = pcStack4;
            }
            else {
              if ((((cVar1 == 'd') || (cVar1 == 'i')) || (cVar1 == 'o')) ||
                 (((cVar1 == 'u' || (cVar1 == 'x')) || (cVar1 == 'X')))) {
                puParm1[0xb] = 9;
              }
            }
          }
          goto LAB_004067e5;
        }
        if (cVar1 == 'L') {
          puParm1[0xb] = 8;
          goto LAB_004067e5;
        }
        if (cVar1 == 'T') {
          puParm1[0xb] = 0xd;
          goto LAB_004067e5;
        }
        if (cVar1 != 'h') goto LAB_004067e5;
        if (*(char *)puParm1[4] == 'h') {
          *(char **)(puParm1 + 4) = (char *)puParm1[4] + 1;
          pcStack4 = (char *)0x1;
        }
        else {
          pcStack4 = (char *)0x2;
        }
      }
      else {
        if (cVar1 != 'l') {
          if (cVar1 == 't') {
            puParm1[0xb] = 7;
          }
          else {
            if (cVar1 == 'w') {
              puParm1[0xb] = 0xc;
            }
            else {
              if (cVar1 == 'z') {
                puParm1[0xb] = 6;
              }
            }
          }
          goto LAB_004067e5;
        }
        if (*(char *)puParm1[4] == 'l') {
          *(char **)(puParm1 + 4) = (char *)puParm1[4] + 1;
          pcStack4 = &DAT_00000004;
        }
        else {
          pcStack4 = (char *)0x3;
        }
      }
      *(char **)(puParm1 + 0xb) = pcStack4;
    }
  }
LAB_004067e5:
  return CONCAT31((int3)((uint)pcStack4 >> 8),1);
}



uint __fastcall FUN_004067e8(uint *param_1)

{
  ushort uVar1;
  ushort *puVar2;
  uint uVar3;
  undefined4 *puVar4;
  ushort *puStack8;
  
  uVar1 = *(ushort *)((int)param_1 + 0x32);
  puStack8 = (ushort *)(uint)uVar1;
  if (uVar1 == 0x46) {
    puStack8 = (ushort *)(*param_1 & 8);
    if (puStack8 == (ushort *)0x0) {
      param_1[7] = 7;
      uVar3 = FUN_00406b94(param_1);
      return uVar3;
    }
  }
  else {
    if (uVar1 == 0x4e) {
      puStack8 = (ushort *)(*param_1 & 8);
      if (puStack8 == (ushort *)0x0) {
        param_1[7] = 8;
        goto LAB_00406827;
      }
    }
    else {
      if (param_1[0xb] != 0) {
LAB_00406827:
        puVar4 = (undefined4 *)FUN_00407f40();
        *puVar4 = 0x16;
        uVar3 = FUN_00407e83();
        return uVar3 & 0xffffff00;
      }
      if (uVar1 < 0x6b) {
        if (uVar1 == 0x6a) {
          param_1[0xb] = 5;
          goto LAB_0040695c;
        }
        if (uVar1 == 0x49) {
          puVar2 = (ushort *)param_1[4];
          uVar1 = *puVar2;
          puStack8 = (ushort *)(uint)uVar1;
          if ((uVar1 == 0x33) && (puVar2[1] == 0x32)) {
            puStack8 = puVar2 + 2;
            param_1[0xb] = 10;
            *(ushort **)(param_1 + 4) = puStack8;
          }
          else {
            if ((uVar1 == 0x36) && (puVar2[1] == 0x34)) {
              puStack8 = puVar2 + 2;
              param_1[0xb] = 0xb;
              *(ushort **)(param_1 + 4) = puStack8;
            }
            else {
              if ((((uVar1 == 100) || (uVar1 == 0x69)) || (uVar1 == 0x6f)) ||
                 (((uVar1 == 0x75 || (uVar1 == 0x78)) || (uVar1 == 0x58)))) {
                param_1[0xb] = 9;
              }
            }
          }
          goto LAB_0040695c;
        }
        if (uVar1 == 0x4c) {
          param_1[0xb] = 8;
          goto LAB_0040695c;
        }
        if (uVar1 == 0x54) {
          param_1[0xb] = 0xd;
          goto LAB_0040695c;
        }
        if (uVar1 != 0x68) goto LAB_0040695c;
        if (*(short *)param_1[4] == 0x68) {
          *(short **)(param_1 + 4) = (short *)param_1[4] + 1;
          puStack8 = (ushort *)0x1;
        }
        else {
          puStack8 = (ushort *)0x2;
        }
      }
      else {
        if (uVar1 != 0x6c) {
          if (uVar1 == 0x74) {
            param_1[0xb] = 7;
          }
          else {
            if (uVar1 == 0x77) {
              param_1[0xb] = 0xc;
            }
            else {
              if (uVar1 == 0x7a) {
                param_1[0xb] = 6;
              }
            }
          }
          goto LAB_0040695c;
        }
        if (*(short *)param_1[4] == 0x6c) {
          *(short **)(param_1 + 4) = (short *)param_1[4] + 1;
          puStack8 = (ushort *)&DAT_00000004;
        }
        else {
          puStack8 = (ushort *)0x3;
        }
      }
      *(ushort **)(param_1 + 0xb) = puStack8;
    }
  }
LAB_0040695c:
  return CONCAT31((int3)((uint)puStack8 >> 8),1);
}



// WARNING: Could not reconcile some variable overlaps

uint __fastcall FUN_00406960(uint *param_1)

{
  uint *puVar1;
  char cVar2;
  bool bVar3;
  bool bVar4;
  undefined uVar5;
  uint uVar6;
  int iVar7;
  int iVar8;
  byte bVar9;
  int local_c;
  undefined4 local_8;
  
  uVar6 = SEXT14(*(char *)((int)param_1 + 0x31));
  if ((int)uVar6 < 0x65) {
    if (uVar6 == 100) {
LAB_00406a12:
      param_1[8] = param_1[8] | 0x10;
LAB_00406a16:
      bVar9 = 0;
      uVar6 = 10;
LAB_00406a19:
      local_8 = param_1;
      uVar6 = FUN_0040734e(param_1,uVar6,bVar9);
    }
    else {
      if ((int)uVar6 < 0x59) {
        if (uVar6 == 0x58) {
          bVar9 = 1;
LAB_004069be:
          uVar6 = 0x10;
          goto LAB_00406a19;
        }
        if (uVar6 != 0x41) {
          if (uVar6 == 0x43) goto LAB_004069d2;
          if ((int)uVar6 < 0x45) goto LAB_004069b5;
          if (0x47 < (int)uVar6) {
            if (uVar6 != 0x53) goto LAB_004069b5;
            goto LAB_004069a6;
          }
        }
LAB_00406a22:
        local_8 = param_1;
        uVar6 = FUN_00406f5e(param_1);
      }
      else {
        if (uVar6 == 0x5a) {
          local_8 = param_1;
          uVar6 = FUN_00406ea6(param_1);
        }
        else {
          if (uVar6 == 0x61) goto LAB_00406a22;
          uVar6 = uVar6 - 99;
          if (uVar6 != 0) goto LAB_004069b5;
LAB_004069d2:
          local_8 = param_1;
          uVar6 = FUN_00407226(param_1);
        }
      }
    }
  }
  else {
    if ((int)uVar6 < 0x71) {
      if (uVar6 == 0x70) {
        local_8 = param_1;
        uVar6 = FUN_004076aa(param_1);
      }
      else {
        if ((int)uVar6 < 0x68) goto LAB_00406a22;
        if (uVar6 == 0x69) goto LAB_00406a12;
        if (uVar6 == 0x6e) {
          local_8 = param_1;
          uVar6 = type_case_n((int)param_1);
        }
        else {
          if (uVar6 != 0x6f) goto LAB_004069b5;
          local_8 = param_1;
          uVar6 = type_case_o(param_1);
        }
      }
    }
    else {
      if (uVar6 != 0x73) {
        if (uVar6 != 0x75) {
          uVar6 = uVar6 - 0x78;
          if (uVar6 != 0) goto LAB_004069b5;
          bVar9 = 0;
          goto LAB_004069be;
        }
        goto LAB_00406a16;
      }
LAB_004069a6:
      local_8 = param_1;
      uVar6 = FUN_004076da(param_1);
    }
  }
  if ((char)uVar6 == 0) {
LAB_004069b5:
    return uVar6 & 0xffffff00;
  }
  if (*(char *)(param_1 + 0xc) != 0) goto LAB_00406b8c;
  iVar7 = 0;
  local_8 = (uint *)((uint)local_8._3_1_ << 0x18);
  uVar6 = param_1[8];
  local_c = 0;
  if ((uVar6 >> 4 & 1) != 0) {
    if ((uVar6 >> 6 & 1) == 0) {
      if ((uVar6 & 1) == 0) {
        if ((uVar6 >> 1 & 1) != 0) {
          local_8 = (uint *)CONCAT31(local_8._1_3_,0x20);
          iVar7 = 1;
          local_c = 1;
        }
        goto LAB_00406aa8;
      }
      local_8 = (uint *)CONCAT31(local_8._1_3_,0x2b);
    }
    else {
      local_8 = (uint *)CONCAT31(local_8._1_3_,0x2d);
    }
    local_c = 1;
    iVar7 = 1;
  }
LAB_00406aa8:
  cVar2 = *(char *)((int)param_1 + 0x31);
  if (((cVar2 == 'x') || (cVar2 == 'X')) && ((uVar6 >> 5 & 1) != 0)) {
    bVar3 = true;
  }
  else {
    bVar3 = false;
  }
  if ((cVar2 == 'a') || (cVar2 == 'A')) {
    bVar4 = true;
  }
  else {
    bVar4 = false;
  }
  if ((bVar3) || (bVar4)) {
    *(undefined *)((int)&local_8 + iVar7) = 0x30;
    if ((cVar2 == 'X') || (cVar2 == 'A')) {
      uVar5 = 0x58;
    }
    else {
      uVar5 = 0x78;
    }
    *(undefined *)((int)&local_8 + iVar7 + 1) = uVar5;
    iVar7 = iVar7 + 2;
    local_c = iVar7;
  }
  puVar1 = param_1 + 6;
  iVar8 = (param_1[9] - param_1[0xe]) - iVar7;
  if ((*(byte *)(param_1 + 8) & 0xc) == 0) {
    FUN_00405d5f(param_1 + 0x112,0x20,iVar8,(int *)puVar1);
    iVar7 = local_c;
  }
  FUN_00407aa3(param_1 + 0x112,(byte *)&local_8,iVar7,(int *)puVar1,(int **)(param_1 + 3));
  if (((param_1[8] >> 3 & 1) != 0) && ((param_1[8] >> 2 & 1) == 0)) {
    FUN_00405d5f(param_1 + 0x112,0x30,iVar8,(int *)puVar1);
  }
  uVar6 = FUN_00407965((int)param_1);
  if ((-1 < (int)*puVar1) && (uVar6 = param_1[8] >> 2, (uVar6 & 1) != 0)) {
    uVar6 = FUN_00405d5f(param_1 + 0x112,0x20,iVar8,(int *)puVar1);
  }
LAB_00406b8c:
  return CONCAT31((int3)(uVar6 >> 8),1);
}



void __fastcall FUN_00406b94(uint *param_1)

{
  uint *this;
  short sVar1;
  bool bVar2;
  bool bVar3;
  char cVar4;
  undefined4 uVar5;
  uint uVar6;
  int iVar7;
  undefined2 uVar8;
  ushort uVar9;
  uint uVar10;
  byte bVar11;
  uint local_14;
  uint local_10;
  undefined2 local_c;
  uint local_8;
  
  local_8 = DAT_0041e004 ^ (uint)&stack0xfffffffc;
  uVar9 = *(ushort *)((int)param_1 + 0x32);
  if (uVar9 < 0x65) {
    if (uVar9 == 100) {
LAB_00406c58:
      param_1[8] = param_1[8] | 0x10;
LAB_00406c5c:
      bVar11 = 0;
      uVar10 = 10;
LAB_00406c5f:
      uVar10 = FUN_004074a1(param_1,uVar10,bVar11);
      cVar4 = (char)uVar10;
    }
    else {
      if (uVar9 < 0x59) {
        if (uVar9 == 0x58) {
          bVar11 = 1;
LAB_00406bff:
          uVar10 = 0x10;
          goto LAB_00406c5f;
        }
        if (uVar9 != 0x41) {
          if (uVar9 == 0x43) goto LAB_00406c13;
          if (uVar9 < 0x45) goto LAB_00406deb;
          if (0x47 < uVar9) {
            if (uVar9 != 0x53) goto LAB_00406deb;
            goto LAB_00406be7;
          }
        }
LAB_00406c68:
        uVar5 = FUN_004070b7(param_1);
        cVar4 = (char)uVar5;
      }
      else {
        if (uVar9 == 0x5a) {
          uVar5 = FUN_00406f02(param_1);
          cVar4 = (char)uVar5;
        }
        else {
          if (uVar9 == 0x61) goto LAB_00406c68;
          if (uVar9 != 99) goto LAB_00406deb;
LAB_00406c13:
          uVar5 = FUN_004072b6(param_1);
          cVar4 = (char)uVar5;
        }
      }
    }
  }
  else {
    if (uVar9 < 0x71) {
      if (uVar9 == 0x70) {
        cVar4 = FUN_004076c2(param_1);
      }
      else {
        if (uVar9 < 0x65) goto LAB_00406deb;
        if (uVar9 < 0x68) goto LAB_00406c68;
        if (uVar9 == 0x69) goto LAB_00406c58;
        if (uVar9 == 0x6e) {
          uVar10 = type_case_n((int)param_1);
          cVar4 = (char)uVar10;
        }
        else {
          if (uVar9 != 0x6f) goto LAB_00406deb;
          cVar4 = type_case_o(param_1);
        }
      }
    }
    else {
      if (uVar9 != 0x73) {
        if (uVar9 != 0x75) {
          if (uVar9 != 0x78) goto LAB_00406deb;
          bVar11 = 0;
          goto LAB_00406bff;
        }
        goto LAB_00406c5c;
      }
LAB_00406be7:
      uVar5 = FUN_0040774b(param_1);
      cVar4 = (char)uVar5;
    }
  }
  if ((cVar4 == 0) || (*(char *)(param_1 + 0xc) != 0)) goto LAB_00406deb;
  uVar6 = 0;
  local_10 = 0;
  local_c = 0;
  uVar10 = param_1[8];
  local_14 = 0;
  if ((uVar10 >> 4 & 1) != 0) {
    if ((uVar10 >> 6 & 1) == 0) {
      if ((uVar10 & 1) == 0) {
        if ((uVar10 >> 1 & 1) != 0) {
          local_10 = 0x20;
          uVar6 = 1;
          local_14 = 1;
        }
        goto LAB_00406cf6;
      }
      uVar9 = 0x2b;
    }
    else {
      uVar9 = 0x2d;
    }
    local_10 = (uint)uVar9;
    local_14 = 1;
    uVar6 = 1;
  }
LAB_00406cf6:
  sVar1 = *(short *)((int)param_1 + 0x32);
  if (((sVar1 == 0x78) || (sVar1 == 0x58)) && ((uVar10 >> 5 & 1) != 0)) {
    bVar2 = true;
  }
  else {
    bVar2 = false;
  }
  if ((sVar1 == 0x61) || (sVar1 == 0x41)) {
    bVar3 = true;
  }
  else {
    bVar3 = false;
  }
  if ((bVar2) || (bVar3)) {
    *(undefined2 *)((int)&local_10 + uVar6 * 2) = 0x30;
    if ((sVar1 == 0x58) || (uVar8 = 0x78, sVar1 == 0x41)) {
      uVar8 = 0x58;
    }
    *(undefined2 *)((int)&local_10 + uVar6 * 2 + 2) = uVar8;
    uVar6 = uVar6 + 2;
    local_14 = uVar6;
  }
  this = param_1 + 0x112;
  iVar7 = (param_1[9] - param_1[0xe]) - uVar6;
  if ((*(byte *)(param_1 + 8) & 0xc) == 0) {
    write_multiple_characters_class___crt_stdio_output__string_output_adapter_wchar_t__char_
              ((string_output_adapter_wchar_t_ *)this,' ',iVar7,(int *)(param_1 + 6));
    uVar6 = local_14;
  }
  FUN_00407acf(this,&local_10,uVar6,(int *)(param_1 + 6));
  if (((param_1[8] >> 3 & 1) != 0) && ((param_1[8] >> 2 & 1) == 0)) {
    write_multiple_characters_class___crt_stdio_output__string_output_adapter_wchar_t__char_
              ((string_output_adapter_wchar_t_ *)this,'0',iVar7,(int *)(param_1 + 6));
  }
  FUN_00407a0e((int)param_1);
  if ((-1 < (int)param_1[6]) && ((param_1[8] >> 2 & 1) != 0)) {
    write_multiple_characters_class___crt_stdio_output__string_output_adapter_wchar_t__char_
              ((string_output_adapter_wchar_t_ *)this,' ',iVar7,(int *)(param_1 + 6));
  }
LAB_00406deb:
  FUN_00401f78();
  return;
}



uint __fastcall FUN_00406dfb(void *param_1)

{
  long *plVar1;
  uint uVar2;
  int iVar3;
  
  plVar1 = (long *)((int)param_1 + 0x24);
  if (*(char *)((int)param_1 + 0x31) != '*') {
    uVar2 = FUN_004061a7(param_1,plVar1);
    return uVar2;
  }
  *(int *)((int)param_1 + 0x14) = *(int *)((int)param_1 + 0x14) + 4;
  iVar3 = *(int *)(*(int *)((int)param_1 + 0x14) + -4);
  *plVar1 = iVar3;
  if (iVar3 < 0) {
    *(uint *)((int)param_1 + 0x20) = *(uint *)((int)param_1 + 0x20) | 4;
    iVar3 = -iVar3;
    *plVar1 = iVar3;
  }
  return CONCAT31((int3)((uint)iVar3 >> 8),1);
}



uint __fastcall FUN_00406e26(void *param_1)

{
  long *plVar1;
  uint uVar2;
  int iVar3;
  
  plVar1 = (long *)((int)param_1 + 0x24);
  if (*(short *)((int)param_1 + 0x32) != 0x2a) {
    uVar2 = FUN_0040621d(param_1,plVar1);
    return uVar2;
  }
  *(int *)((int)param_1 + 0x14) = *(int *)((int)param_1 + 0x14) + 4;
  iVar3 = *(int *)(*(int *)((int)param_1 + 0x14) + -4);
  *plVar1 = iVar3;
  if (iVar3 < 0) {
    *(uint *)((int)param_1 + 0x20) = *(uint *)((int)param_1 + 0x20) | 4;
    iVar3 = -iVar3;
    *plVar1 = iVar3;
  }
  return CONCAT31((int3)((uint)iVar3 >> 8),1);
}



undefined4 __cdecl FUN_00406e52(undefined4 param_1)

{
  undefined4 uStack8;
  
  switch(param_1) {
  case 0:
  case 3:
  case 6:
  case 7:
  case 9:
  case 10:
    uStack8 = 4;
    break;
  case 1:
    return 1;
  case 2:
    uStack8 = 2;
    break;
  case 4:
  case 5:
  case 0xb:
    uStack8 = 8;
    break;
  default:
    return 0;
  }
  return uStack8;
}



undefined4 __fastcall FUN_00406ea6(undefined4 *param_1)

{
  ushort uVar1;
  ushort *puVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  
  param_1[5] = param_1[5] + 4;
  puVar2 = *(ushort **)(param_1[5] + -4);
  if ((puVar2 == (ushort *)0x0) || (iVar3 = *(int *)(puVar2 + 2), iVar3 == 0)) {
    param_1[0xd] = 0x417ba4;
    uVar5 = 6;
  }
  else {
    uVar4 = FUN_00405a94(*param_1,param_1[1],*(char *)((int)param_1 + 0x31),param_1[0xb]);
    param_1[0xd] = iVar3;
    uVar1 = *puVar2;
    uVar5 = (uint)uVar1;
    if ((char)uVar4 != 0) {
      *(undefined *)(param_1 + 0xf) = 1;
      uVar5 = (uint)(uVar1 >> 1);
      goto LAB_00406ef9;
    }
  }
  *(undefined *)(param_1 + 0xf) = 0;
LAB_00406ef9:
  param_1[0xe] = uVar5;
  return CONCAT31((int3)(uVar5 >> 8),1);
}



undefined4 __fastcall FUN_00406f02(uint *param_1)

{
  ushort uVar1;
  ushort *puVar2;
  uint uVar3;
  uint uVar4;
  
  param_1[5] = param_1[5] + 4;
  puVar2 = *(ushort **)(param_1[5] - 4);
  if ((puVar2 == (ushort *)0x0) || (uVar4 = *(uint *)(puVar2 + 2), uVar4 == 0)) {
    param_1[0xd] = 0x417ba4;
    uVar4 = 6;
  }
  else {
    uVar3 = FUN_00405ac5(*param_1,param_1[1],*(short *)((int)param_1 + 0x32),param_1[0xb]);
    param_1[0xd] = uVar4;
    uVar1 = *puVar2;
    uVar4 = (uint)uVar1;
    if ((char)uVar3 != 0) {
      *(undefined *)(param_1 + 0xf) = 1;
      uVar4 = (uint)(uVar1 >> 1);
      goto LAB_00406f55;
    }
  }
  *(undefined *)(param_1 + 0xf) = 0;
LAB_00406f55:
  param_1[0xe] = uVar4;
  return CONCAT31((int3)(uVar4 >> 8),1);
}



undefined4 __fastcall FUN_00406f5e(uint *param_1)

{
  uint *this;
  char *pcVar1;
  char cVar2;
  uint *puVar3;
  int iVar4;
  char *pcVar5;
  undefined8 *puVar6;
  uint uVar7;
  uint uVar8;
  uint uVar9;
  undefined4 *puVar10;
  uint uVar11;
  uint *local_c;
  uint *local_8;
  
  param_1[8] = param_1[8] | 0x10;
  uVar11 = param_1[10];
  if ((int)uVar11 < 0) {
    if ((*(char *)((int)param_1 + 0x31) == 'a') || (*(char *)((int)param_1 + 0x31) == 'A')) {
      uVar11 = 0xd;
    }
    else {
      uVar11 = 6;
    }
    param_1[10] = uVar11;
  }
  else {
    if ((uVar11 == 0) &&
       ((*(char *)((int)param_1 + 0x31) == 'g' ||
        (uVar11 = 0, *(char *)((int)param_1 + 0x31) == 'G')))) {
      param_1[10] = 1;
      uVar11 = 1;
    }
  }
  this = param_1 + 0x10;
  local_c = param_1;
  local_8 = param_1;
  cVar2 = FUN_00405995(uVar11 + 0x15d);
  if (cVar2 == 0) {
    uVar11 = count_char_((formatting_buffer *)this);
    param_1[10] = uVar11 - 0x15d;
  }
  puVar3 = (uint *)param_1[0x111];
  if ((uint *)param_1[0x111] == (uint *)0x0) {
    puVar3 = this;
  }
  *(uint **)(param_1 + 0xd) = puVar3;
  param_1[5] = param_1[5] + 8;
  local_c = *(uint **)(param_1[5] - 8);
  local_8 = *(uint **)(param_1[5] - 4);
  uVar11 = count_char_((formatting_buffer *)this);
  puVar3 = (uint *)param_1[0x111];
  if ((uint *)param_1[0x111] == (uint *)0x0) {
    puVar3 = this;
  }
  puVar10 = (undefined4 *)param_1[2];
  iVar4 = (int)*(char *)((int)param_1 + 0x31);
  uVar9 = param_1[1];
  uVar8 = *param_1;
  uVar7 = param_1[10];
  puVar6 = (undefined8 *)scratch_data_char_((formatting_buffer *)this);
  pcVar5 = (char *)count_char_((formatting_buffer *)this);
  FUN_0040c12a((double *)&local_c,(byte *)puVar3,pcVar5,puVar6,uVar11,iVar4,uVar7,uVar8,uVar9,
               puVar10);
  uVar11 = param_1[8] >> 5;
  if (((uVar11 & 1) != 0) && (param_1[10] == 0)) {
    uVar11 = FUN_0040614b((byte *)param_1[0xd],(int *)param_1[2]);
  }
  uVar11 = uVar11 & 0xffffff00;
  if (((*(char *)((int)param_1 + 0x31) == 'g') || (*(char *)((int)param_1 + 0x31) == 'G')) &&
     (uVar11 = param_1[8] >> 5, (uVar11 & 1) == 0)) {
    uVar11 = FUN_00406068((char *)param_1[0xd],(int *)param_1[2]);
  }
  pcVar5 = (char *)param_1[0xd];
  cVar2 = *pcVar5;
  if (cVar2 == '-') {
    param_1[8] = param_1[8] | 0x40;
    pcVar5 = pcVar5 + 1;
    *(char **)(param_1 + 0xd) = pcVar5;
    cVar2 = *pcVar5;
  }
  if (((cVar2 == 'i') || (cVar2 == 'I')) || ((cVar2 == 'n' || (cVar2 == 'N')))) {
    param_1[8] = param_1[8] & 0xfffffff7;
    *(undefined *)((int)param_1 + 0x31) = 0x73;
  }
  pcVar1 = pcVar5 + 1;
  do {
    cVar2 = *pcVar5;
    pcVar5 = pcVar5 + 1;
  } while (cVar2 != 0);
  *(char **)(param_1 + 0xe) = pcVar5 + -(int)pcVar1;
  return CONCAT31((int3)(uVar11 >> 8),1);
}



undefined4 __fastcall FUN_004070b7(uint *param_1)

{
  uint *this;
  char *pcVar1;
  ushort uVar2;
  char cVar3;
  uint *puVar4;
  int iVar5;
  char *pcVar6;
  undefined8 *puVar7;
  uint uVar8;
  uint uVar9;
  uint uVar10;
  uint uVar11;
  undefined4 *puVar12;
  uint *local_c;
  uint *local_8;
  
  param_1[8] = param_1[8] | 0x10;
  uVar11 = param_1[10];
  if ((int)uVar11 < 0) {
    if ((*(short *)((int)param_1 + 0x32) == 0x61) || (*(short *)((int)param_1 + 0x32) == 0x41)) {
      uVar11 = 0xd;
    }
    else {
      uVar11 = 6;
    }
    param_1[10] = uVar11;
  }
  else {
    if ((uVar11 == 0) &&
       ((*(short *)((int)param_1 + 0x32) == 0x67 ||
        (uVar11 = 0, *(short *)((int)param_1 + 0x32) == 0x47)))) {
      param_1[10] = 1;
      uVar11 = 1;
    }
  }
  this = param_1 + 0x10;
  local_c = param_1;
  local_8 = param_1;
  cVar3 = FUN_00405995(uVar11 + 0x15d);
  if (cVar3 == 0) {
    uVar11 = count_char_((formatting_buffer *)this);
    param_1[10] = uVar11 - 0x15d;
  }
  puVar4 = (uint *)param_1[0x111];
  if ((uint *)param_1[0x111] == (uint *)0x0) {
    puVar4 = this;
  }
  *(uint **)(param_1 + 0xd) = puVar4;
  param_1[5] = param_1[5] + 8;
  local_c = *(uint **)(param_1[5] - 8);
  local_8 = *(uint **)(param_1[5] - 4);
  uVar11 = count_char_((formatting_buffer *)this);
  puVar4 = (uint *)param_1[0x111];
  if ((uint *)param_1[0x111] == (uint *)0x0) {
    puVar4 = this;
  }
  puVar12 = (undefined4 *)param_1[2];
  iVar5 = (int)*(char *)((int)param_1 + 0x32);
  uVar10 = param_1[1];
  uVar9 = *param_1;
  uVar8 = param_1[10];
  puVar7 = (undefined8 *)scratch_data_char_((formatting_buffer *)this);
  pcVar6 = (char *)count_char_((formatting_buffer *)this);
  FUN_0040c12a((double *)&local_c,(byte *)puVar4,pcVar6,puVar7,uVar11,iVar5,uVar8,uVar9,uVar10,
               puVar12);
  if (((param_1[8] >> 5 & 1) != 0) && (param_1[10] == 0)) {
    FUN_0040614b((byte *)param_1[0xd],(int *)param_1[2]);
  }
  uVar2 = *(ushort *)((int)param_1 + 0x32);
  uVar11 = (uint)uVar2;
  if (((uVar2 == 0x67) || (uVar2 == 0x47)) && (uVar11 = param_1[8] >> 5, (uVar11 & 1) == 0)) {
    uVar11 = FUN_00406068((char *)param_1[0xd],(int *)param_1[2]);
  }
  pcVar6 = (char *)param_1[0xd];
  cVar3 = *pcVar6;
  uVar11 = uVar11 & 0xffffff00;
  if (cVar3 == '-') {
    param_1[8] = param_1[8] | 0x40;
    pcVar6 = pcVar6 + 1;
    *(char **)(param_1 + 0xd) = pcVar6;
    cVar3 = *pcVar6;
  }
  if (((cVar3 == 'i') || (cVar3 == 'I')) || ((cVar3 == 'n' || (cVar3 == 'N')))) {
    param_1[8] = param_1[8] & 0xfffffff7;
    uVar11 = 0;
    *(undefined2 *)((int)param_1 + 0x32) = 0x73;
  }
  pcVar1 = pcVar6 + 1;
  do {
    cVar3 = *pcVar6;
    pcVar6 = pcVar6 + 1;
  } while (cVar3 != 0);
  *(char **)(param_1 + 0xe) = pcVar6 + -(int)pcVar1;
  return CONCAT31((int3)(uVar11 >> 8),1);
}



undefined4 __fastcall FUN_00407226(undefined4 *param_1)

{
  uint uVar1;
  int iVar2;
  int *piVar3;
  int *this;
  ushort uVar4;
  
  this = param_1 + 0x10;
  uVar1 = FUN_00405a94(*param_1,param_1[1],*(char *)((int)param_1 + 0x31),param_1[0xb]);
  if ((char)uVar1 == 0) {
    piVar3 = (int *)param_1[0x111];
    if ((int *)param_1[0x111] == (int *)0x0) {
      piVar3 = this;
    }
    param_1[5] = param_1[5] + 4;
    *(formatting_buffer *)piVar3 = *(formatting_buffer *)(param_1[5] + -4);
    param_1[0xe] = 1;
  }
  else {
    param_1[5] = param_1[5] + 4;
    uVar4 = *(ushort *)(param_1[5] + -4);
    piVar3 = (int *)param_1[0x111];
    if ((int *)param_1[0x111] == (int *)0x0) {
      piVar3 = this;
    }
    uVar1 = count_char_((formatting_buffer *)this);
    iVar2 = FUN_0040afbe(param_1 + 0xe,piVar3,uVar1,uVar4);
    if (iVar2 != 0) {
      *(undefined *)(param_1 + 0xc) = 1;
    }
  }
  piVar3 = (int *)param_1[0x111];
  if (piVar3 != (int *)0x0) {
    this = piVar3;
  }
  *(int **)(param_1 + 0xd) = this;
  return CONCAT31((int3)((uint)piVar3 >> 8),1);
}



// WARNING: Could not reconcile some variable overlaps

undefined4 __fastcall FUN_004072b6(uint *param_1)

{
  uint uVar1;
  uint *puVar2;
  uint *puVar3;
  uint *local_8;
  
  *(undefined *)(param_1 + 0xf) = 1;
  puVar3 = param_1 + 0x10;
  param_1[5] = param_1[5] + 4;
  local_8._0_2_ = *(ushort *)(param_1[5] - 4);
  local_8 = param_1;
  uVar1 = FUN_00405ac5(*param_1,param_1[1],*(short *)((int)param_1 + 0x32),param_1[0xb]);
  if ((char)uVar1 == 0) {
    local_8._0_2_ = (ushort)local_8 & 0xff;
    local_8 = (uint *)((uint)local_8 & 0xffff0000 | (uint)(ushort)local_8);
    puVar2 = (uint *)param_1[0x111];
    if ((uint *)param_1[0x111] == (uint *)0x0) {
      puVar2 = puVar3;
    }
    uVar1 = FUN_0040ad03((ushort *)puVar2,(byte *)&local_8,*(byte **)(*(int *)param_1[2] + 4),
                         (int *)param_1[2]);
    if ((int)uVar1 < 0) {
      *(undefined *)(param_1 + 0xc) = 1;
    }
  }
  else {
    puVar2 = (uint *)param_1[0x111];
    if ((uint *)param_1[0x111] == (uint *)0x0) {
      puVar2 = puVar3;
    }
    *(ushort *)puVar2 = (ushort)local_8;
  }
  puVar2 = (uint *)param_1[0x111];
  if (puVar2 != (uint *)0x0) {
    puVar3 = puVar2;
  }
  *(uint **)(param_1 + 0xd) = puVar3;
  param_1[0xe] = 1;
  return CONCAT31((int3)((uint)puVar2 >> 8),1);
}



uint __thiscall FUN_0040734e(void *this,uint param_1,byte param_2)

{
  int iVar1;
  undefined4 *puVar2;
  uint uVar3;
  char *pcVar4;
  uint uVar5;
  uint uVar6;
  bool bVar7;
  
  iVar1 = FUN_00406e52(*(undefined4 *)((int)this + 0x2c));
  if (iVar1 == 1) {
    *(int *)((int)this + 0x14) = *(int *)((int)this + 0x14) + 4;
    if ((*(uint *)((int)this + 0x20) >> 4 & 1) == 0) {
      uVar3 = (uint)*(byte *)(*(int *)((int)this + 0x14) + -4);
    }
    else {
      uVar3 = SEXT14(*(char *)(*(int *)((int)this + 0x14) + -4));
    }
  }
  else {
    if (iVar1 == 2) {
      *(int *)((int)this + 0x14) = *(int *)((int)this + 0x14) + 4;
      if ((*(uint *)((int)this + 0x20) >> 4 & 1) == 0) {
        uVar3 = (uint)*(ushort *)(*(int *)((int)this + 0x14) + -4);
      }
      else {
        uVar3 = SEXT24(*(short *)(*(int *)((int)this + 0x14) + -4));
      }
    }
    else {
      if (iVar1 != 4) {
        if (iVar1 != 8) {
          puVar2 = (undefined4 *)FUN_00407f40();
          *puVar2 = 0x16;
          uVar3 = FUN_00407e83();
          return uVar3 & 0xffffff00;
        }
        *(int *)((int)this + 0x14) = *(int *)((int)this + 0x14) + 8;
        uVar3 = *(uint *)(*(int *)((int)this + 0x14) + -8);
        uVar6 = *(uint *)(*(int *)((int)this + 0x14) + -4);
        goto LAB_00407404;
      }
      *(int *)((int)this + 0x14) = *(int *)((int)this + 0x14) + 4;
      if ((*(uint *)((int)this + 0x20) >> 4 & 1) == 0) {
        uVar3 = *(uint *)(*(int *)((int)this + 0x14) + -4);
        uVar6 = 0;
        goto LAB_00407404;
      }
      uVar3 = *(uint *)(*(int *)((int)this + 0x14) + -4);
    }
  }
  uVar6 = (int)uVar3 >> 0x1f;
LAB_00407404:
  uVar5 = *(uint *)((int)this + 0x20);
  if ((((uVar5 >> 4 & 1) != 0) && ((int)uVar6 < 1)) && ((int)uVar6 < 0)) {
    bVar7 = uVar3 != 0;
    uVar3 = -uVar3;
    uVar6 = -(uVar6 + (uint)bVar7);
    uVar5 = uVar5 | 0x40;
    *(uint *)((int)this + 0x20) = uVar5;
  }
  if (*(int *)((int)this + 0x28) < 0) {
    *(undefined4 *)((int)this + 0x28) = 1;
  }
  else {
    *(uint *)((int)this + 0x20) = uVar5 & 0xfffffff7;
    FUN_00405995(*(undefined4 *)((int)this + 0x28));
  }
  if ((uVar3 | uVar6) == 0) {
    *(uint *)((int)this + 0x20) = *(uint *)((int)this + 0x20) & 0xffffffdf;
  }
  *(undefined *)((int)this + 0x3c) = 0;
  if (iVar1 == 8) {
    FUN_00405c2b(this,uVar3,uVar6,param_1,param_2);
  }
  else {
    FUN_00405b31(this,uVar3,param_1,param_2);
  }
  pcVar4 = (char *)(*(uint *)((int)this + 0x20) >> 7);
  if ((((uint)pcVar4 & 1) != 0) &&
     ((*(int *)((int)this + 0x38) == 0 || (pcVar4 = *(char **)((int)this + 0x34), *pcVar4 != '0'))))
  {
    *(int *)((int)this + 0x34) = *(int *)((int)this + 0x34) + -1;
    **(undefined **)((int)this + 0x34) = 0x30;
    *(int *)((int)this + 0x38) = *(int *)((int)this + 0x38) + 1;
  }
  return CONCAT31((int3)((uint)pcVar4 >> 8),1);
}



uint __thiscall FUN_004074a1(void *this,uint param_1,byte param_2)

{
  int iVar1;
  undefined4 *puVar2;
  uint uVar3;
  short *psVar4;
  uint uVar5;
  uint uVar6;
  bool bVar7;
  
  iVar1 = FUN_00406e52(*(undefined4 *)((int)this + 0x2c));
  if (iVar1 == 1) {
    *(int *)((int)this + 0x14) = *(int *)((int)this + 0x14) + 4;
    if ((*(uint *)((int)this + 0x20) >> 4 & 1) == 0) {
      uVar3 = (uint)*(byte *)(*(int *)((int)this + 0x14) + -4);
    }
    else {
      uVar3 = SEXT14(*(char *)(*(int *)((int)this + 0x14) + -4));
    }
  }
  else {
    if (iVar1 == 2) {
      *(int *)((int)this + 0x14) = *(int *)((int)this + 0x14) + 4;
      if ((*(uint *)((int)this + 0x20) >> 4 & 1) == 0) {
        uVar3 = (uint)*(ushort *)(*(int *)((int)this + 0x14) + -4);
      }
      else {
        uVar3 = SEXT24(*(short *)(*(int *)((int)this + 0x14) + -4));
      }
    }
    else {
      if (iVar1 != 4) {
        if (iVar1 != 8) {
          puVar2 = (undefined4 *)FUN_00407f40();
          *puVar2 = 0x16;
          uVar3 = FUN_00407e83();
          return uVar3 & 0xffffff00;
        }
        *(int *)((int)this + 0x14) = *(int *)((int)this + 0x14) + 8;
        uVar3 = *(uint *)(*(int *)((int)this + 0x14) + -8);
        uVar6 = *(uint *)(*(int *)((int)this + 0x14) + -4);
        goto LAB_00407557;
      }
      *(int *)((int)this + 0x14) = *(int *)((int)this + 0x14) + 4;
      if ((*(uint *)((int)this + 0x20) >> 4 & 1) == 0) {
        uVar3 = *(uint *)(*(int *)((int)this + 0x14) + -4);
        uVar6 = 0;
        goto LAB_00407557;
      }
      uVar3 = *(uint *)(*(int *)((int)this + 0x14) + -4);
    }
  }
  uVar6 = (int)uVar3 >> 0x1f;
LAB_00407557:
  uVar5 = *(uint *)((int)this + 0x20);
  if ((((uVar5 >> 4 & 1) != 0) && ((int)uVar6 < 1)) && ((int)uVar6 < 0)) {
    bVar7 = uVar3 != 0;
    uVar3 = -uVar3;
    uVar6 = -(uVar6 + (uint)bVar7);
    uVar5 = uVar5 | 0x40;
    *(uint *)((int)this + 0x20) = uVar5;
  }
  if (*(int *)((int)this + 0x28) < 0) {
    *(undefined4 *)((int)this + 0x28) = 1;
  }
  else {
    *(uint *)((int)this + 0x20) = uVar5 & 0xfffffff7;
    FUN_00405a14(*(undefined4 *)((int)this + 0x28));
  }
  if ((uVar3 | uVar6) == 0) {
    *(uint *)((int)this + 0x20) = *(uint *)((int)this + 0x20) & 0xffffffdf;
  }
  *(undefined *)((int)this + 0x3c) = 1;
  if (iVar1 == 8) {
    FUN_00405cbb(this,uVar3,uVar6,param_1,param_2);
  }
  else {
    FUN_00405ba4(this,uVar3,param_1,param_2);
  }
  psVar4 = (short *)(*(uint *)((int)this + 0x20) >> 7);
  if ((((uint)psVar4 & 1) != 0) &&
     ((*(int *)((int)this + 0x38) == 0 || (psVar4 = *(short **)((int)this + 0x34), *psVar4 != 0x30))
     )) {
    *(int *)((int)this + 0x34) = *(int *)((int)this + 0x34) + -2;
    **(undefined2 **)((int)this + 0x34) = 0x30;
    *(int *)((int)this + 0x38) = *(int *)((int)this + 0x38) + 1;
  }
  return CONCAT31((int3)((uint)psVar4 >> 8),1);
}



// Library Function - Multiple Matches With Same Base Name
// Name: type_case_n
// Library: Visual Studio 2015 Release

uint __fastcall type_case_n(int param_1)

{
  uint *puVar1;
  int iVar2;
  undefined4 *puVar3;
  uint uVar4;
  
  *(int *)(param_1 + 0x14) = *(int *)(param_1 + 0x14) + 4;
  puVar1 = *(uint **)(*(int *)(param_1 + 0x14) + -4);
  iVar2 = __get_printf_count_output();
  if (iVar2 == 0) {
LAB_00407611:
    puVar3 = (undefined4 *)FUN_00407f40();
    *puVar3 = 0x16;
    uVar4 = FUN_00407e83();
    uVar4 = uVar4 & 0xffffff00;
  }
  else {
    iVar2 = FUN_00406e52(*(undefined4 *)(param_1 + 0x2c));
    if (iVar2 == 1) {
      uVar4 = 0;
      *(undefined *)puVar1 = *(undefined *)(param_1 + 0x18);
    }
    else {
      if (iVar2 == 2) {
        uVar4 = (uint)*(ushort *)(param_1 + 0x18);
        *(ushort *)puVar1 = *(ushort *)(param_1 + 0x18);
      }
      else {
        if (iVar2 == 4) {
          uVar4 = *(uint *)(param_1 + 0x18);
          *puVar1 = uVar4;
        }
        else {
          if (iVar2 != 8) goto LAB_00407611;
          uVar4 = *(uint *)(param_1 + 0x18);
          *puVar1 = uVar4;
          puVar1[1] = (int)uVar4 >> 0x1f;
        }
      }
    }
    *(undefined *)(param_1 + 0x30) = 1;
    uVar4 = CONCAT31((int3)(uVar4 >> 8),1);
  }
  return uVar4;
}



// Library Function - Multiple Matches With Same Base Name
// Name: type_case_o
// Libraries: Visual Studio 2015 Debug, Visual Studio 2015 Release

void __fastcall type_case_o(void *param_1)

{
  if ((*(uint *)((int)param_1 + 0x20) >> 5 & 1) != 0) {
    *(uint *)((int)param_1 + 0x20) = *(uint *)((int)param_1 + 0x20) | 0x80;
  }
  FUN_0040734e(param_1,8,0);
  return;
}



// Library Function - Multiple Matches With Same Base Name
// Name: type_case_o
// Libraries: Visual Studio 2015 Debug, Visual Studio 2015 Release

void __fastcall type_case_o(void *param_1)

{
  if ((*(uint *)((int)param_1 + 0x20) >> 5 & 1) != 0) {
    *(uint *)((int)param_1 + 0x20) = *(uint *)((int)param_1 + 0x20) | 0x80;
  }
  FUN_004074a1(param_1,8,0);
  return;
}



void __fastcall FUN_004076aa(void *param_1)

{
  *(undefined4 *)((int)param_1 + 0x28) = 8;
  *(undefined4 *)((int)param_1 + 0x2c) = 10;
  FUN_0040734e(param_1,0x10,1);
  return;
}



void __fastcall FUN_004076c2(void *param_1)

{
  *(undefined4 *)((int)param_1 + 0x28) = 8;
  *(undefined4 *)((int)param_1 + 0x2c) = 10;
  FUN_004074a1(param_1,0x10,1);
  return;
}



undefined4 __fastcall FUN_004076da(undefined4 *param_1)

{
  uint uVar1;
  undefined4 uVar2;
  int iVar3;
  char *pcVar4;
  
  param_1[5] = param_1[5] + 4;
  iVar3 = param_1[10];
  pcVar4 = *(char **)(param_1[5] + -4);
  *(char **)(param_1 + 0xd) = pcVar4;
  if (iVar3 == -1) {
    iVar3 = 0x7fffffff;
  }
  uVar1 = FUN_00405a94(*param_1,param_1[1],*(char *)((int)param_1 + 0x31),param_1[0xb]);
  if ((char)uVar1 == 0) {
    if ((wchar_t *)pcVar4 == (wchar_t *)0x0) {
      pcVar4 = "(null)";
      param_1[0xd] = 0x417ba4;
    }
    uVar2 = FUN_0040afdb(pcVar4,iVar3);
  }
  else {
    if ((wchar_t *)pcVar4 == (wchar_t *)0x0) {
      param_1[0xd] = 0x417b94;
      pcVar4 = (char *)L"(null)";
    }
    *(undefined *)(param_1 + 0xf) = 1;
    uVar2 = FUN_0040b0fd(pcVar4,iVar3);
  }
  param_1[0xe] = uVar2;
  return CONCAT31((int3)((uint)uVar2 >> 8),1);
}



undefined4 __fastcall FUN_0040774b(uint *param_1)

{
  uint uVar1;
  uint uVar2;
  wchar_t *pwVar3;
  
  param_1[5] = param_1[5] + 4;
  uVar2 = param_1[10];
  pwVar3 = *(wchar_t **)(param_1[5] - 4);
  *(wchar_t **)(param_1 + 0xd) = pwVar3;
  if (uVar2 == 0xffffffff) {
    uVar2 = 0x7fffffff;
  }
  uVar1 = FUN_00405ac5(*param_1,param_1[1],*(short *)((int)param_1 + 0x32),param_1[0xb]);
  if ((char)uVar1 == 0) {
    if (pwVar3 == (wchar_t *)0x0) {
      param_1[0xd] = 0x417ba4;
    }
    uVar2 = FUN_004077be(param_1,uVar2);
  }
  else {
    if (pwVar3 == (wchar_t *)0x0) {
      pwVar3 = L"(null)";
      param_1[0xd] = 0x417b94;
    }
    *(undefined *)(param_1 + 0xf) = 1;
    uVar2 = FUN_0040b0fd(pwVar3,uVar2);
  }
  param_1[0xe] = uVar2;
  return CONCAT31((int3)(uVar2 >> 8),1);
}



int __thiscall FUN_004077be(void *this,int param_1)

{
  uint uVar1;
  byte *pbVar2;
  int iVar3;
  
  iVar3 = 0;
  pbVar2 = *(byte **)((int)this + 0x34);
  if (0 < param_1) {
    do {
      if (*pbVar2 == 0) {
        return iVar3;
      }
      uVar1 = FUN_00407be2(***(int ***)((int)this + 8),(uint)*pbVar2,0x8000);
      if (uVar1 != 0) {
        pbVar2 = pbVar2 + 1;
      }
      pbVar2 = pbVar2 + 1;
      iVar3 = iVar3 + 1;
    } while (iVar3 < param_1);
  }
  return iVar3;
}



uint __fastcall FUN_00407805(FILE **param_1)

{
  undefined4 *puVar1;
  uint uVar2;
  
  if (*param_1 == (FILE *)0x0) {
    puVar1 = (undefined4 *)FUN_00407f40();
    *puVar1 = 0x16;
    uVar2 = FUN_00407e83();
    return uVar2 & 0xffffff00;
  }
  uVar2 = FUN_00407863(*param_1);
  return uVar2;
}



uint __fastcall FUN_00407826(int *piParm1)

{
  undefined4 in_EAX;
  undefined4 *puVar1;
  uint uVar2;
  
  if (*piParm1 == 0) {
    puVar1 = (undefined4 *)FUN_00407f40();
    *puVar1 = 0x16;
    uVar2 = FUN_00407e83();
    return uVar2 & 0xffffff00;
  }
  return CONCAT31((int3)((uint)in_EAX >> 8),1);
}



uint __fastcall FUN_00407841(int iParm1)

{
  undefined4 in_EAX;
  undefined4 *puVar1;
  uint uVar2;
  
  if ((*(int *)(iParm1 + 0x1c) != 0) && (*(int *)(iParm1 + 0x1c) != 7)) {
    puVar1 = (undefined4 *)FUN_00407f40();
    *puVar1 = 0x16;
    uVar2 = FUN_00407e83();
    return uVar2 & 0xffffff00;
  }
  return CONCAT31((int3)((uint)in_EAX >> 8),1);
}



uint __cdecl FUN_00407863(FILE *param_1)

{
  uint uVar1;
  undefined4 *puVar2;
  undefined *puVar3;
  undefined *puVar4;
  
  uVar1 = (uint)param_1->_flag >> 0xc;
  if ((uVar1 & 1) == 0) {
    uVar1 = __fileno(param_1);
    puVar3 = &DAT_0041e100;
    if ((uVar1 == 0xffffffff) || (uVar1 == 0xfffffffe)) {
      puVar4 = &DAT_0041e100;
    }
    else {
      puVar4 = (undefined *)((uVar1 & 0x3f) * 0x38 + (&DAT_0041f080)[(int)uVar1 >> 6]);
    }
    if (puVar4[0x29] == 0) {
      if ((uVar1 != 0xffffffff) && (uVar1 != 0xfffffffe)) {
        puVar3 = (undefined *)((uVar1 & 0x3f) * 0x38 + (&DAT_0041f080)[(int)uVar1 >> 6]);
      }
      if ((puVar3[0x2d] & 1) == 0) goto LAB_004078e5;
    }
    puVar2 = (undefined4 *)FUN_00407f40();
    *puVar2 = 0x16;
    uVar1 = FUN_00407e83();
    uVar1 = uVar1 & 0xffffff00;
  }
  else {
LAB_004078e5:
    uVar1 = CONCAT31((int3)(uVar1 >> 8),1);
  }
  return uVar1;
}



// Library Function - Single Match
// Name: ?write_character@?$string_output_adapter@_W@__crt_stdio_output@@QBE_N_WQAH@Z
// Library: Visual Studio 2015 Release
// public: bool __thiscall
// __crt_stdio_output::string_output_adapter<wchar_t>::write_character(wchar_t,int * const)const 

bool __thiscall write_character(string_output_adapter_wchar_t_ *this,wchar_t param_1,int *param_2)

{
  int iVar1;
  undefined uVar2;
  undefined in_stack_00000005;
  
  iVar1 = *(int *)this;
  if (*(int *)(iVar1 + 8) == *(int *)(iVar1 + 4)) {
    if (*(char *)(iVar1 + 0xc) == 0) {
      *param_2 = -1;
    }
    else {
      *param_2 = *param_2 + 1;
    }
    uVar2 = *(undefined *)(*(int *)this + 0xc);
  }
  else {
    *param_2 = *param_2 + 1;
    *(int *)(*(int *)this + 8) = *(int *)(*(int *)this + 8) + 1;
    *(undefined2 *)**(undefined4 **)this = _param_1;
    **(int **)this = **(int **)this + 2;
    uVar2 = 1;
  }
  return (bool)uVar2;
}



uint __thiscall FUN_0040792f(void *this,byte param_1)

{
  uint uVar1;
  
  if (((*(uint *)(*(int *)this + 0xc) >> 0xc & 1) == 0) || (*(int *)(*(int *)this + 4) != 0)) {
    uVar1 = FUN_0040c28e(param_1,*(FILE **)this);
    uVar1 = uVar1 & 0xffffff00 | (uint)(uVar1 != 0xffffffff);
  }
  else {
    uVar1 = CONCAT31((int3)((uint)*(int *)this >> 8),1);
  }
  return uVar1;
}



void __fastcall FUN_00407965(int param_1)

{
  ushort uVar1;
  int iVar2;
  int iVar3;
  ushort *puVar4;
  int local_14;
  int local_10 [2];
  uint local_8;
  
  local_8 = DAT_0041e004 ^ (uint)&stack0xfffffffc;
  if ((*(char *)(param_1 + 0x3c) == 0) || (*(int *)(param_1 + 0x38) < 1)) {
    FUN_00407aa3((void *)(param_1 + 0x448),*(byte **)(param_1 + 0x34),*(int *)(param_1 + 0x38),
                 (int *)(param_1 + 0x18),(int **)(param_1 + 0xc));
  }
  else {
    puVar4 = *(ushort **)(param_1 + 0x34);
    iVar3 = 0;
    if (*(int *)(param_1 + 0x38) != 0) {
      do {
        uVar1 = *puVar4;
        puVar4 = puVar4 + 1;
        local_14 = 0;
        iVar2 = FUN_0040afbe(&local_14,local_10,6,uVar1);
        if ((iVar2 != 0) || (local_14 == 0)) {
          *(undefined4 *)(param_1 + 0x18) = 0xffffffff;
          break;
        }
        FUN_00407aa3((void *)(param_1 + 0x448),(byte *)local_10,local_14,(int *)(param_1 + 0x18),
                     (int **)(param_1 + 0xc));
        iVar3 = iVar3 + 1;
      } while (iVar3 != *(int *)(param_1 + 0x38));
    }
  }
  FUN_00401f78();
  return;
}



undefined4 __fastcall FUN_00407a0e(int param_1)

{
  uint uVar1;
  uint3 extraout_var;
  uint uVar2;
  byte *pbVar3;
  int iVar4;
  uint local_8;
  
  if ((*(char *)(param_1 + 0x3c) == 0) && (uVar2 = *(uint *)(param_1 + 0x38), 0 < (int)uVar2)) {
    pbVar3 = *(byte **)(param_1 + 0x34);
    iVar4 = 0;
    if (uVar2 != 0) {
      do {
        local_8 = local_8 & 0xffff0000;
        uVar1 = FUN_0040ad03((ushort *)&local_8,pbVar3,*(byte **)(**(int **)(param_1 + 8) + 4),
                             *(int **)(param_1 + 8));
        if ((int)uVar1 < 1) {
          *(int *)(param_1 + 0x18) = -1;
          uVar2 = uVar1;
          break;
        }
        write_character((string_output_adapter_wchar_t_ *)(param_1 + 0x448),SUB41(local_8,0),
                        (int *)(param_1 + 0x18));
        uVar2 = (uint)extraout_var << 8;
        pbVar3 = pbVar3 + uVar1;
        iVar4 = iVar4 + 1;
      } while (iVar4 != *(int *)(param_1 + 0x38));
    }
  }
  else {
    uVar2 = FUN_00407acf((void *)(param_1 + 0x448),*(undefined8 **)(param_1 + 0x34),
                         *(uint *)(param_1 + 0x38),(int *)(param_1 + 0x18));
  }
  return CONCAT31((int3)(uVar2 >> 8),1);
}



void __thiscall FUN_00407aa3(void *this,byte *param_1,int param_2,int *param_3,int **param_4)

{
  int iVar1;
  uint uVar2;
  int *piVar3;
  byte *pbVar4;
  int *piVar5;
  
  if (((*(uint *)(*(int *)this + 0xc) >> 0xc & 1) != 0) && (*(int *)(*(int *)this + 4) == 0)) {
    *param_3 = *param_3 + param_2;
    return;
  }
  piVar5 = *param_4;
  if (piVar5 == (int *)0x0) {
    piVar5 = (int *)FUN_00407f40();
    *param_4 = piVar5;
  }
  iVar1 = *piVar5;
  pbVar4 = param_1 + param_2;
  *piVar5 = 0;
  if (param_1 != pbVar4) {
    do {
      uVar2 = FUN_0040792f(this,*param_1);
      if ((char)uVar2 == 0) {
        piVar3 = *param_4;
        if (piVar3 == (int *)0x0) {
          piVar3 = (int *)FUN_00407f40();
          *param_4 = piVar3;
        }
        if (*piVar3 != 0x2a) {
          *param_3 = -1;
          break;
        }
        uVar2 = FUN_0040792f(this,0x3f);
        if ((char)uVar2 != 0) goto LAB_00407bb6;
        *param_3 = -1;
      }
      else {
LAB_00407bb6:
        *param_3 = *param_3 + 1;
      }
      param_1 = param_1 + 1;
    } while (param_1 != pbVar4);
  }
  if ((*piVar5 == 0) && (iVar1 != 0)) {
    *piVar5 = iVar1;
  }
  return;
}



void __thiscall FUN_00407acf(void *this,undefined8 *param_1,uint param_2,int *param_3)

{
  undefined4 *puVar1;
  uint uVar2;
  
  if (param_2 == 0) {
    return;
  }
  puVar1 = *(undefined4 **)this;
  if (puVar1[2] == puVar1[1]) {
    if (*(char *)(puVar1 + 3) == 0) {
LAB_00407b33:
      *param_3 = -1;
      return;
    }
  }
  else {
    uVar2 = puVar1[1] - puVar1[2];
    if (param_2 <= uVar2) {
      uVar2 = param_2;
    }
    copy_buffer((undefined8 *)*puVar1,param_1,uVar2 * 2);
    **(int **)this = **(int **)this + uVar2 * 2;
    *(int *)(*(int *)this + 8) = *(int *)(*(int *)this + 8) + uVar2;
    if (*(char *)(*(int *)this + 0xc) == 0) {
      if (uVar2 == param_2) {
        *param_3 = *param_3 + uVar2;
        return;
      }
      goto LAB_00407b33;
    }
  }
  *param_3 = *param_3 + param_2;
  return;
}



uint __cdecl FUN_00407be2(int param_1,int param_2,uint param_3)

{
  if (param_2 + 1U < 0x101) {
    return (uint)*(ushort *)(param_1 + param_2 * 2) & param_3;
  }
  return 0;
}



undefined4 __cdecl
FUN_00407c04(undefined4 param_1,undefined4 param_2,FILE *param_3,int param_4,undefined4 param_5,
            undefined4 param_6)

{
  undefined4 *puVar1;
  undefined4 uVar2;
  FILE *local_3c;
  undefined4 *local_38;
  undefined4 *local_34;
  int *local_30;
  undefined4 *local_2c;
  undefined4 local_28;
  undefined4 local_24;
  FILE *local_20;
  FILE *local_1c;
  undefined4 local_18;
  int local_14;
  undefined4 local_10;
  FILE *local_c [2];
  
  local_18 = param_6;
  local_10 = param_5;
  local_28 = param_1;
  local_14 = param_4;
  local_c[0] = param_3;
  local_24 = param_2;
  if ((param_3 == (FILE *)0x0) || (param_4 == 0)) {
    puVar1 = (undefined4 *)FUN_00407f40();
    *puVar1 = 0x16;
    FUN_00407e83();
    uVar2 = 0xffffffff;
  }
  else {
    local_3c = (FILE *)local_c;
    local_1c = param_3;
    local_38 = &local_10;
    local_34 = &local_28;
    local_30 = &local_14;
    local_2c = &local_18;
    local_20 = param_3;
    uVar2 = operator____(&local_20,&local_3c);
  }
  return uVar2;
}



// Library Function - Single Match
// Name: ___stdio_common_vswprintf_s
// Library: Visual Studio 2015 Release

void __cdecl
___stdio_common_vswprintf_s
          (undefined4 param_1,undefined4 param_2,wchar_t *param_3,uint param_4,wchar_t *param_5,
          __crt_locale_pointers *param_6,char *param_7)

{
  common_vsprintf_s_wchar_t_(CONCAT44(param_2,param_1),param_3,param_4,param_5,param_6,param_7);
  return;
}



void __cdecl free(void *param_1)

{
  FID_conflict__free(param_1);
  return;
}



// Library Function - Multiple Matches With Different Base Names
// Name: ?<lambda_invoker_cdecl>@<lambda_e0128dce7362637f77e3b418282c6919>@@CAPAXI@Z, _malloc
// Library: Visual Studio 2015 Release

void * __cdecl allocate(size_t _Size)

{
  LPVOID pvVar1;
  
  pvVar1 = __malloc_base(_Size);
  return pvVar1;
}



// Library Function - Single Match
// Name: ___acrt_call_reportfault
// Library: Visual Studio 2015 Release

void ___acrt_call_reportfault(int param_1,DWORD param_2,DWORD param_3)

{
  BOOL BVar1;
  LONG LVar2;
  byte in_PF;
  byte in_AF;
  byte in_TF;
  byte in_IF;
  byte in_NT;
  byte in_AC;
  byte in_VIF;
  byte in_VIP;
  byte in_ID;
  _EXCEPTION_POINTERS local_32c;
  EXCEPTION_RECORD local_324;
  int local_2d4 [39];
  PCONTEXT local_224;
  uint local_214;
  uint local_8;
  
  local_8 = DAT_0041e004 ^ (uint)&stack0xfffffffc;
  if (param_1 != -1) {
    FUN_00402855();
  }
  FUN_00402e50((int *)&local_324,0,0x50);
  FUN_00402e50(local_2d4,0,0x2cc);
  local_32c.ExceptionRecord = &local_324;
  local_32c.ContextRecord = (PCONTEXT)local_2d4;
  local_214 = (uint)(in_NT & 1) * 0x4000 | (uint)SCARRY4((int)&stack0xfffffcb8,0x18) * 0x800 |
              (uint)(in_IF & 1) * 0x200 | (uint)(in_TF & 1) * 0x100 |
              (uint)((int)&stack0xfffffcd0 < 0) * 0x80 |
              (uint)((undefined *)register0x00000010 == (undefined *)0x330) * 0x40 |
              (uint)(in_AF & 1) * 0x10 | (uint)(in_PF & 1) * 4 |
              (uint)((undefined *)0xffffffe7 < &stack0xfffffcb8) | (uint)(in_ID & 1) * 0x200000 |
              (uint)(in_VIP & 1) * 0x100000 | (uint)(in_VIF & 1) * 0x80000 |
              (uint)(in_AC & 1) * 0x40000;
  local_2d4[0] = 0x10001;
  local_324.ExceptionCode = param_2;
  local_324.ExceptionFlags = param_3;
  local_224 = local_32c.ContextRecord;
  BVar1 = IsDebuggerPresent();
  SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)0x0);
  LVar2 = UnhandledExceptionFilter(&local_32c);
  if (((LVar2 == 0) && (BVar1 == 0)) && (param_1 != -1)) {
    FUN_00402855();
  }
  FUN_00401f78();
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void __cdecl FUN_00407e10(undefined4 param_1)

{
  _DAT_0041ed30 = param_1;
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void __cdecl
FUN_00407e1f(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
            undefined4 param_5)

{
  uint uVar1;
  byte bVar2;
  code *pcVar3;
  
  uVar1 = FUN_0040b71e();
  if (((uVar1 == 0) || (pcVar3 = *(code **)(uVar1 + 0x35c), pcVar3 == (code *)0x0)) &&
     (bVar2 = (byte)DAT_0041e004 & 0x1f,
     pcVar3 = (code *)((DAT_0041e004 ^ _DAT_0041ed30) >> bVar2 |
                      (DAT_0041e004 ^ _DAT_0041ed30) << 0x20 - bVar2), pcVar3 == (code *)0x0)) {
    FUN_00407e93();
    pcVar3 = (code *)swi(3);
    (*pcVar3)(param_2,param_3,param_4,param_5);
    return;
  }
  _guard_check_icall();
  (*pcVar3)(param_1,param_2,param_3,param_4,param_5);
  return;
}



void FUN_00407e83(void)

{
  FUN_00407e1f(0,0,0,0,0);
  return;
}



void FUN_00407e93(void)

{
  code *pcVar1;
  BOOL BVar2;
  HANDLE hProcess;
  UINT uExitCode;
  
  BVar2 = IsProcessorFeaturePresent(0x17);
  if (BVar2 != 0) {
    pcVar1 = (code *)swi(0x29);
    (*pcVar1)();
    return;
  }
  ___acrt_call_reportfault(2,0xc0000417,1);
  uExitCode = 0xc0000417;
  hProcess = GetCurrentProcess();
  TerminateProcess(hProcess,uExitCode);
  return;
}



// Library Function - Multiple Matches With Different Base Names
// Name: ___acrt_errno_from_os_error, __get_errno_from_oserr
// Libraries: Visual Studio 2012 Release, Visual Studio 2015 Release

int __cdecl FID_conflict____acrt_errno_from_os_error(ulong param_1)

{
  uint uVar1;
  
  uVar1 = 0;
  do {
    if (param_1 == (&DAT_00417bb0)[uVar1 * 2]) {
      return (&DAT_00417bb4)[uVar1 * 2];
    }
    uVar1 = uVar1 + 1;
  } while (uVar1 < 0x2d);
  if (param_1 - 0x13 < 0x12) {
    return 0xd;
  }
  return (-(uint)(0xe < param_1 - 0xbc) & 0xe) + 8;
}



// Library Function - Single Match
// Name: ___acrt_errno_map_os_error
// Library: Visual Studio 2015 Release

void __cdecl ___acrt_errno_map_os_error(ulong param_1)

{
  ulong *puVar1;
  int iVar2;
  int *piVar3;
  
  puVar1 = (ulong *)FUN_00407f2d();
  *puVar1 = param_1;
  iVar2 = FID_conflict____acrt_errno_from_os_error(param_1);
  piVar3 = (int *)FUN_00407f40();
  *piVar3 = iVar2;
  return;
}



undefined * FUN_00407f2d(void)

{
  uint uVar1;
  
  uVar1 = FUN_0040b71e();
  if (uVar1 == 0) {
    return &DAT_0041e0ec;
  }
  return (undefined *)(uVar1 + 0x14);
}



undefined * FUN_00407f40(void)

{
  uint uVar1;
  
  uVar1 = FUN_0040b71e();
  if (uVar1 == 0) {
    return &DAT_0041e0e8;
  }
  return (undefined *)(uVar1 + 0x10);
}



undefined4 __cdecl cleanup(LPCWSTR file_name)

{
  BOOL BVar1;
  DWORD DVar2;
  
  BVar1 = DeleteFileW(file_name);
  if (BVar1 == 0) {
    DVar2 = GetLastError();
    ___acrt_errno_map_os_error(DVar2);
    return 0xffffffff;
  }
  return 0;
}



// WARNING: Function: __EH_prolog3_GS replaced with injection: EH_prolog3

void __cdecl FUN_00407f7b(undefined8 *param_1,ushort *param_2,uint param_3,undefined4 *param_4)

{
  char cVar1;
  ushort uVar2;
  undefined4 *puVar3;
  ushort *puVar4;
  int iVar5;
  DWORD DVar6;
  uint uVar7;
  uint uVar8;
  int local_40;
  int local_3c;
  char local_34;
  int local_30;
  undefined8 *local_2c;
  int local_28;
  int local_24;
  ushort *local_20;
  CHAR local_1c [20];
  undefined4 local_8;
  undefined4 uStack4;
  
  uStack4 = 0x30;
  local_8 = 0x407f87;
  uVar8 = 0;
  local_2c = param_1;
  local_20 = param_2;
  local_24 = 0;
  if ((param_1 != (undefined8 *)0x0) && (param_3 == 0)) goto LAB_0040821e;
  if (param_2 == (ushort *)0x0) {
    puVar3 = (undefined4 *)FUN_00407f40();
    *puVar3 = 0x16;
    FUN_00407e83();
    goto LAB_0040821e;
  }
  FUN_00405ed6(&local_40,param_4);
  local_8 = 0;
  uVar7 = *(uint *)(local_3c + 8);
  if (uVar7 == 0xfde9) {
    local_30 = 0;
    local_2c = (undefined8 *)0x0;
    FUN_0040c466(param_1,&local_20,param_3,&local_30);
  }
  else {
    if (param_1 == (undefined8 *)0x0) {
      if (*(int *)(local_3c + 0xa8) == 0) {
        uVar2 = *local_20;
        puVar4 = local_20;
        while (uVar2 != 0) {
          if (0xff < uVar2) goto LAB_004081fe;
          puVar4 = puVar4 + 1;
          uVar2 = *puVar4;
        }
      }
      else {
        iVar5 = FUN_0040c3b9(uVar7,0,(LPCWSTR)local_20,-1,(LPSTR)0x0,0,0,(uint)&local_24);
        if ((iVar5 == 0) || (local_24 != 0)) {
LAB_004081fe:
          puVar3 = (undefined4 *)FUN_00407f40();
          *puVar3 = 0x2a;
        }
      }
    }
    else {
      if (*(int *)(local_3c + 0xa8) == 0) {
        if (param_3 != 0) {
          do {
            if (0xff < *local_20) goto LAB_00408198;
            *(undefined *)((int)param_1 + uVar8) = *(undefined *)local_20;
            uVar2 = *local_20;
            local_20 = local_20 + 1;
          } while ((uVar2 != 0) && (uVar8 = uVar8 + 1, uVar8 < param_3));
        }
      }
      else {
        if (*(int *)(local_3c + 4) == 1) {
          puVar4 = local_20;
          uVar8 = param_3;
          if (param_3 != 0) {
            do {
              if (*puVar4 == 0) break;
              puVar4 = puVar4 + 1;
              uVar8 = uVar8 - 1;
            } while (uVar8 != 0);
            if ((uVar8 != 0) && (*puVar4 == 0)) {
              param_3 = ((int)((int)puVar4 - (int)local_20) >> 1) + 1;
            }
          }
          iVar5 = FUN_0040c3b9(uVar7,0,(LPCWSTR)local_20,param_3,(LPSTR)param_1,param_3,0,
                               (uint)&local_24);
          if (iVar5 != 0) {
joined_r0x004080d2:
            if (local_24 == 0) goto LAB_0040820c;
          }
LAB_00408198:
          puVar3 = (undefined4 *)FUN_00407f40();
          *puVar3 = 0x2a;
        }
        else {
          uVar8 = FUN_0040c3b9(uVar7,0,(LPCWSTR)local_20,-1,(LPSTR)param_1,param_3,0,(uint)&local_24
                              );
          if (uVar8 != 0) goto joined_r0x004080d2;
          if ((local_24 != 0) || (DVar6 = GetLastError(), DVar6 != 0x7a)) goto LAB_00408198;
          if (param_3 != 0) {
            do {
              iVar5 = *(int *)(local_3c + 4);
              if (5 < iVar5) {
                iVar5 = 5;
              }
              uVar7 = FUN_0040c3b9(*(uint *)(local_3c + 8),0,(LPCWSTR)local_20,1,local_1c,iVar5,0,
                                   (uint)&local_24);
              if ((((uVar7 == 0) || (local_24 != 0)) || ((int)uVar7 < 0)) || (5 < uVar7))
              goto LAB_004081fe;
              if (param_3 < uVar7 + uVar8) break;
              local_28 = 0;
              if (0 < (int)uVar7) {
                do {
                  cVar1 = local_1c[local_28];
                  *(char *)((int)local_2c + uVar8) = cVar1;
                  if (cVar1 == 0) goto LAB_0040820c;
                  local_28 = local_28 + 1;
                  uVar8 = uVar8 + 1;
                } while (local_28 < (int)uVar7);
              }
              local_20 = local_20 + 1;
            } while (uVar8 < param_3);
          }
        }
      }
    }
  }
LAB_0040820c:
  if (local_34 != 0) {
    *(uint *)(local_40 + 0x350) = *(uint *)(local_40 + 0x350) & 0xfffffffd;
  }
LAB_0040821e:
  FUN_00416238();
  return;
}



void __cdecl copy_str_into_buffer(undefined8 *param_1,ushort *param_2,uint param_3)

{
  FUN_00407f7b(param_1,param_2,param_3,(undefined4 *)0x0);
  return;
}



undefined __cdecl FUN_0040823e(byte param_1,uint param_2)

{
  if ((param_1 & 4) != 0) {
    return 1;
  }
  if ((param_1 & 1) == 0) {
    return 0;
  }
  if ((param_1 & 2) == 0) {
    if (param_2 < 0x80000000) {
      return 0;
    }
  }
  else {
    if (param_2 < 0x80000001) {
      return 0;
    }
  }
  return 1;
}



// Library Function - Multiple Matches With Same Base Name
// Name: make_c_string_character_source<>
// Library: Visual Studio 2015 Release

void __cdecl
make_c_string_character_source__(undefined4 *param_1,undefined4 param_2,undefined4 *param_3)

{
  *param_1 = param_2;
  *(undefined4 **)(param_1 + 1) = param_3;
  if (param_3 != (undefined4 *)0x0) {
    *param_3 = param_2;
  }
  return;
}



uint __cdecl
FUN_0040828a(undefined4 *param_1,byte *param_2,byte **param_3,uint param_4,byte param_5)

{
  byte bVar1;
  char cVar2;
  int iVar3;
  undefined4 *puVar4;
  byte bVar5;
  uint uVar6;
  uint uVar7;
  int local_2c;
  ushort *local_28 [2];
  char local_20;
  byte *local_1c;
  uint local_18;
  uint local_14;
  uint local_10;
  uint local_c;
  uint local_8;
  
  cVar2 = FUN_00407826();
  if (cVar2 != 0) {
    if ((param_4 == 0) || ((1 < (int)param_4 && ((int)param_4 < 0x25)))) {
      FUN_00405ed6(&local_2c,param_1);
      local_10 = 0;
      local_1c = param_2;
      do {
        bVar5 = *param_2;
        param_2 = param_2 + 1;
        local_8 = local_8 & 0xffffff00 | (uint)bVar5;
        iVar3 = __ischartype_l((uint)bVar5,8,local_28);
      } while (iVar3 != 0);
      local_c = (uint)param_5;
      if (bVar5 == 0x2d) {
        local_c = local_c | 2;
LAB_0040832b:
        bVar5 = *param_2;
        param_2 = param_2 + 1;
        local_8 = local_8 & 0xffffff00 | (uint)bVar5;
      }
      else {
        if (bVar5 == 0x2b) goto LAB_0040832b;
      }
      if ((param_4 == 0) || (param_4 == 0x10)) {
        if ((byte)(bVar5 - 0x30) < 10) {
          iVar3 = (int)(char)bVar5 + -0x30;
        }
        else {
          if ((byte)(bVar5 + 0x9f) < 0x1a) {
            iVar3 = (int)(char)bVar5 + -0x57;
          }
          else {
            if ((byte)(bVar5 + 0xbf) < 0x1a) {
              iVar3 = (int)(char)bVar5 + -0x37;
            }
            else {
              iVar3 = -1;
            }
          }
        }
        if (iVar3 == 0) {
          bVar1 = *param_2;
          local_14 = local_14 & 0xffffff00 | (uint)bVar1;
          if ((bVar1 == 0x78) || (bVar1 == 0x58)) {
            if (param_4 == 0) {
              param_4 = 0x10;
            }
            bVar5 = param_2[1];
            param_2 = param_2 + 2;
            local_8 = local_8 & 0xffffff00 | (uint)bVar5;
          }
          else {
            if (param_4 == 0) {
              param_4 = 8;
            }
            param_2 = param_2 + 1;
            FUN_00408af3(&param_2,bVar1);
          }
        }
        else {
          if (param_4 == 0) {
            param_4 = 10;
          }
        }
      }
      local_14 = (uint)(0xffffffff / (ulonglong)param_4);
      local_18 = (uint)(0xffffffff % (ulonglong)param_4);
      while( true ) {
        if ((byte)(bVar5 - 0x30) < 10) {
          uVar6 = (int)(char)bVar5 - 0x30;
        }
        else {
          if ((byte)(bVar5 + 0x9f) < 0x1a) {
            uVar6 = (int)(char)bVar5 - 0x57;
          }
          else {
            if ((byte)(bVar5 + 0xbf) < 0x1a) {
              uVar6 = (int)(char)bVar5 - 0x37;
            }
            else {
              uVar6 = 0xffffffff;
            }
          }
        }
        if ((uVar6 == 0xffffffff) || (param_4 <= uVar6)) break;
        if ((local_10 < local_14) || ((local_10 == local_14 && (uVar6 <= local_18)))) {
          uVar7 = 8;
          local_10 = local_10 * param_4 + uVar6;
        }
        else {
          uVar7 = 0xc;
        }
        bVar5 = *param_2;
        param_2 = param_2 + 1;
        local_8 = local_8 & 0xffffff00 | (uint)bVar5;
        local_c = local_c | uVar7;
      }
      FUN_00408af3(&param_2,(char)local_8);
      uVar7 = local_c;
      uVar6 = local_10;
      if ((local_c & 8) == 0) {
        uVar6 = 0;
        param_2 = local_1c;
      }
      else {
        cVar2 = FUN_0040823e((byte)local_c,local_10);
        if (cVar2 == 0) {
          if ((uVar7 & 2) != 0) {
            uVar6 = -uVar6;
          }
        }
        else {
          puVar4 = (undefined4 *)FUN_00407f40();
          *puVar4 = 0x22;
          if ((uVar7 & 1) == 0) {
            uVar6 = 0xffffffff;
          }
          else {
            if ((uVar7 & 2) == 0) {
              uVar6 = 0x7fffffff;
            }
            else {
              uVar6 = 0x80000000;
            }
          }
        }
      }
      if (local_20 != 0) {
        *(uint *)(local_2c + 0x350) = *(uint *)(local_2c + 0x350) & 0xfffffffd;
      }
      goto LAB_004082c3;
    }
    puVar4 = (undefined4 *)FUN_00407f40();
    *puVar4 = 0x16;
    FUN_00407e83();
  }
  uVar6 = 0;
LAB_004082c3:
  if (param_3 != (byte **)0x0) {
    *param_3 = param_2;
  }
  return uVar6;
}



// WARNING: Could not reconcile some variable overlaps

uint __cdecl
FUN_004084b1(undefined4 *param_1,ushort *param_2,ushort **param_3,uint param_4,byte param_5)

{
  char cVar1;
  uint uVar2;
  uint uVar3;
  undefined4 *puVar4;
  int iVar5;
  uint uVar6;
  ushort uVar7;
  byte bVar8;
  int local_20 [3];
  char local_14;
  ushort *local_10;
  uint local_c;
  undefined4 local_8;
  
  cVar1 = FUN_00407826();
  if (cVar1 != 0) {
    if ((param_4 == 0) || ((1 < (int)param_4 && ((int)param_4 < 0x25)))) {
      FUN_00405ed6(local_20,param_1);
      local_c = 0;
      local_10 = param_2;
      do {
        uVar7 = *param_2;
        param_2 = param_2 + 1;
        uVar6 = FUN_0040c58c(uVar7,8);
      } while (uVar6 != 0);
      if (uVar7 == 0x2d) {
        param_5 = param_5 | 2;
LAB_00408545:
        uVar7 = *param_2;
        param_2 = param_2 + 1;
      }
      else {
        if (uVar7 == 0x2b) goto LAB_00408545;
      }
      uVar6 = (uint)uVar7;
      local_8 = 0x19;
      iVar5 = 0x30;
      if ((param_4 != 0) && (param_4 != 0x10)) goto LAB_004087aa;
      if (uVar7 < 0x30) {
LAB_0040871d:
        if (((uVar7 < 0x41) || (0x5a < uVar7)) && (0x19 < (ushort)(uVar7 - 0x61))) {
          iVar5 = -1;
        }
        else {
          uVar2 = uVar6;
          if ((ushort)(uVar7 - 0x61) < 0x1a) {
            uVar2 = uVar6 - 0x20;
          }
          iVar5 = uVar2 - 0x37;
        }
      }
      else {
        if (uVar7 < 0x3a) {
LAB_00408584:
          iVar5 = uVar6 - iVar5;
        }
        else {
          iVar5 = 0xff10;
          if (uVar7 < 0xff10) {
            iVar5 = 0x660;
            if ((uVar7 < 0x660) ||
               ((0x669 < uVar7 &&
                ((iVar5 = 0x6f0, uVar7 < 0x6f0 ||
                 ((0x6f9 < uVar7 &&
                  ((iVar5 = 0x966, uVar7 < 0x966 ||
                   ((0x96f < uVar7 &&
                    ((iVar5 = 0x9e6, uVar7 < 0x9e6 ||
                     ((0x9ef < uVar7 &&
                      ((iVar5 = 0xa66, uVar7 < 0xa66 ||
                       ((0xa6f < uVar7 &&
                        ((iVar5 = 0xae6, uVar7 < 0xae6 ||
                         ((0xaef < uVar7 &&
                          ((iVar5 = 0xb66, uVar7 < 0xb66 ||
                           ((0xb6f < uVar7 &&
                            ((iVar5 = 0xc66, uVar7 < 0xc66 ||
                             ((0xc6f < uVar7 &&
                              ((iVar5 = 0xce6, uVar7 < 0xce6 ||
                               ((0xcef < uVar7 &&
                                ((iVar5 = 0xd66, uVar7 < 0xd66 ||
                                 ((0xd6f < uVar7 &&
                                  ((iVar5 = 0xe50, uVar7 < 0xe50 ||
                                   ((0xe59 < uVar7 &&
                                    ((iVar5 = 0xed0, uVar7 < 0xed0 ||
                                     ((0xed9 < uVar7 &&
                                      ((iVar5 = 0xf20, uVar7 < 0xf20 ||
                                       ((0xf29 < uVar7 &&
                                        ((iVar5 = 0x1040, uVar7 < 0x1040 ||
                                         ((0x1049 < uVar7 &&
                                          ((iVar5 = 0x17e0, uVar7 < 0x17e0 ||
                                           ((0x17e9 < uVar7 &&
                                            ((iVar5 = 0x1810, uVar7 < 0x1810 || (0x1819 < uVar7)))))
                                           )))))))))))))))))))))))))))))))))))))))))))))))))))))))))
            goto LAB_0040871d;
            goto LAB_00408584;
          }
          if (uVar7 < 0xff1a) goto LAB_00408584;
          iVar5 = -1;
        }
        if (iVar5 == -1) goto LAB_0040871d;
      }
      if (iVar5 == 0) {
        uVar7 = *param_2;
        if ((uVar7 == 0x78) || (uVar7 == 0x58)) {
          if (param_4 == 0) {
            param_4 = 0x10;
          }
          uVar6 = (uint)param_2[1];
          param_2 = param_2 + 2;
        }
        else {
          if (param_4 == 0) {
            param_4 = 8;
          }
          param_2 = param_2 + 1;
          FUN_00408b1c(&param_2,uVar7);
        }
      }
      else {
        if (param_4 == 0) {
          param_4 = 10;
        }
      }
LAB_004087aa:
      uVar2 = (uint)(0xffffffff / (ulonglong)param_4);
      do {
        uVar7 = (ushort)uVar6;
        if (uVar7 < 0x30) goto LAB_00408a18;
        if (uVar7 < 0x3a) {
          uVar3 = uVar6 - 0x30;
LAB_00408a13:
          if (uVar3 == 0xffffffff) goto LAB_00408a18;
        }
        else {
          if (0xff0f < uVar7) {
            if (uVar7 < 0xff1a) {
              uVar3 = uVar6 - 0xff10;
            }
            else {
              uVar3 = 0xffffffff;
            }
            goto LAB_00408a13;
          }
          if (0x65f < uVar7) {
            if (uVar7 < 0x66a) {
              uVar3 = uVar6 - 0x660;
            }
            else {
              if (uVar7 < 0x6f0) goto LAB_00408a18;
              if (uVar7 < 0x6fa) {
                uVar3 = uVar6 - 0x6f0;
              }
              else {
                if (uVar7 < 0x966) goto LAB_00408a18;
                if (uVar7 < 0x970) {
                  uVar3 = uVar6 - 0x966;
                }
                else {
                  if (uVar7 < 0x9e6) goto LAB_00408a18;
                  if (uVar7 < 0x9f0) {
                    uVar3 = uVar6 - 0x9e6;
                  }
                  else {
                    if (uVar7 < 0xa66) goto LAB_00408a18;
                    if (uVar7 < 0xa70) {
                      uVar3 = uVar6 - 0xa66;
                    }
                    else {
                      if (uVar7 < 0xae6) goto LAB_00408a18;
                      if (uVar7 < 0xaf0) {
                        uVar3 = uVar6 - 0xae6;
                      }
                      else {
                        if (uVar7 < 0xb66) goto LAB_00408a18;
                        if (uVar7 < 0xb70) {
                          uVar3 = uVar6 - 0xb66;
                        }
                        else {
                          if (uVar7 < 0xc66) goto LAB_00408a18;
                          if (uVar7 < 0xc70) {
                            uVar3 = uVar6 - 0xc66;
                          }
                          else {
                            if (uVar7 < 0xce6) goto LAB_00408a18;
                            if (uVar7 < 0xcf0) {
                              uVar3 = uVar6 - 0xce6;
                            }
                            else {
                              if (uVar7 < 0xd66) goto LAB_00408a18;
                              if (uVar7 < 0xd70) {
                                uVar3 = uVar6 - 0xd66;
                              }
                              else {
                                if (uVar7 < 0xe50) goto LAB_00408a18;
                                if (uVar7 < 0xe5a) {
                                  uVar3 = uVar6 - 0xe50;
                                }
                                else {
                                  if (uVar7 < 0xed0) goto LAB_00408a18;
                                  if (uVar7 < 0xeda) {
                                    uVar3 = uVar6 - 0xed0;
                                  }
                                  else {
                                    if (uVar7 < 0xf20) goto LAB_00408a18;
                                    if (uVar7 < 0xf2a) {
                                      uVar3 = uVar6 - 0xf20;
                                    }
                                    else {
                                      if (uVar7 < 0x1040) goto LAB_00408a18;
                                      if (uVar7 < 0x104a) {
                                        uVar3 = uVar6 - 0x1040;
                                      }
                                      else {
                                        if (uVar7 < 0x17e0) goto LAB_00408a18;
                                        if (uVar7 < 0x17ea) {
                                          uVar3 = uVar6 - 0x17e0;
                                        }
                                        else {
                                          if ((uVar7 < 0x1810) || (0x1819 < uVar7))
                                          goto LAB_00408a18;
                                          uVar3 = uVar6 - 0x1810;
                                        }
                                      }
                                    }
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
            goto LAB_00408a13;
          }
LAB_00408a18:
          if (((uVar7 < 0x41) || (0x5a < uVar7)) && ((ushort)local_8 < (ushort)(uVar7 - 0x61))) {
            uVar3 = 0xffffffff;
          }
          else {
            if ((ushort)(uVar7 - 0x61) <= (ushort)local_8) {
              uVar6 = uVar6 - 0x20;
            }
            uVar3 = uVar6 - 0x37;
          }
        }
        if ((uVar3 == 0xffffffff) || (param_4 <= uVar3)) goto LAB_00408a7f;
        if ((local_c < uVar2) ||
           ((local_c == uVar2 && (uVar3 <= (uint)(0xffffffff % (ulonglong)param_4))))) {
          bVar8 = 8;
          local_c = local_c * param_4 + uVar3;
        }
        else {
          bVar8 = 0xc;
        }
        uVar6 = (uint)*param_2;
        param_2 = param_2 + 1;
        param_5 = param_5 | bVar8;
      } while( true );
    }
    puVar4 = (undefined4 *)FUN_00407f40();
    *puVar4 = 0x16;
    FUN_00407e83();
  }
  uVar6 = 0;
LAB_004084e9:
  if (param_3 != (ushort **)0x0) {
    *param_3 = param_2;
  }
  return uVar6;
LAB_00408a7f:
  FUN_00408b1c(&param_2,uVar7);
  uVar6 = local_c;
  if ((param_5 & 8) == 0) {
    uVar6 = 0;
    param_2 = local_10;
  }
  else {
    cVar1 = FUN_0040823e(param_5,local_c);
    if (cVar1 == 0) {
      if ((param_5 & 2) != 0) {
        uVar6 = -uVar6;
      }
    }
    else {
      puVar4 = (undefined4 *)FUN_00407f40();
      *puVar4 = 0x22;
      if ((param_5 & 1) == 0) {
        uVar6 = 0xffffffff;
      }
      else {
        if ((param_5 & 2) == 0) {
          uVar6 = 0x7fffffff;
        }
        else {
          uVar6 = 0x80000000;
        }
      }
    }
  }
  if (local_14 != 0) {
    *(uint *)(local_20[0] + 0x350) = *(uint *)(local_20[0] + 0x350) & 0xfffffffd;
  }
  goto LAB_004084e9;
}



void __thiscall FUN_00408af3(void *this,char param_1)

{
  int iVar1;
  undefined4 *puVar2;
  
  iVar1 = *(int *)this;
  *(char **)this = (char *)(iVar1 + -1);
  if ((param_1 != 0) && (*(char *)(iVar1 + -1) != param_1)) {
    puVar2 = (undefined4 *)FUN_00407f40();
    *puVar2 = 0x16;
    FUN_00407e83();
  }
  return;
}



void __thiscall FUN_00408b1c(void *this,short param_1)

{
  int iVar1;
  undefined4 *puVar2;
  
  iVar1 = *(int *)this;
  *(short **)this = (short *)(iVar1 + -2);
  if ((param_1 != 0) && (*(short *)(iVar1 + -2) != param_1)) {
    puVar2 = (undefined4 *)FUN_00407f40();
    *puVar2 = 0x16;
    FUN_00407e83();
  }
  return;
}



void __cdecl FUN_00408b4a(int param_1,uint param_2,ushort **param_3)

{
  ushort *puVar1;
  
  if (param_3 == (ushort **)0x0) {
    puVar1 = ___pctype_func();
  }
  else {
    puVar1 = *(ushort **)*param_3;
  }
  FUN_00407be2((int)puVar1,param_1,param_2);
  return;
}



// Library Function - Single Match
// Name: __ischartype_l
// Library: Visual Studio 2015 Release

void __cdecl __ischartype_l(int param_1,uint param_2,ushort **param_3)

{
  if ((param_3 == (ushort **)0x0) || (*(int *)(*param_3 + 2) < 2)) {
    FUN_00408b4a(param_1,param_2,param_3);
  }
  else {
    FUN_0040c5f7(param_1,param_2,param_3);
  }
  return;
}



// Library Function - Multiple Matches With Different Base Names
// Name: __strtoi64, __wcstoi64, _strtoimax, _strtol, _strtoll, _wcstoimax, _wcstol, _wcstoll
// Library: Visual Studio 2015 Release

long __cdecl FID_conflict__wcstol(wchar_t *_Str,wchar_t **_EndPtr,int _Radix)

{
  uint uVar1;
  byte *extraout_var;
  byte **extraout_var_00;
  byte bVar2;
  
  bVar2 = 1;
  make_c_string_character_source__((undefined4 *)&stack0xffffffe8,_Str,_EndPtr);
  uVar1 = FUN_0040828a((undefined4 *)0x0,extraout_var,extraout_var_00,_Radix,bVar2);
  return uVar1;
}



// Library Function - Multiple Matches With Different Base Names
// Name: __strtoi64, __wcstoi64, _strtoimax, _strtol, _strtoll, _wcstoimax, _wcstol, _wcstoll
// Library: Visual Studio 2015 Release

long __cdecl FID_conflict__wcstol(wchar_t *_Str,wchar_t **_EndPtr,int _Radix)

{
  uint uVar1;
  ushort *extraout_var;
  ushort **extraout_var_00;
  byte bVar2;
  
  bVar2 = 1;
  make_c_string_character_source__((undefined4 *)&stack0xffffffe8,_Str,_EndPtr);
  uVar1 = FUN_004084b1((undefined4 *)0x0,extraout_var,extraout_var_00,_Radix,bVar2);
  return uVar1;
}



// Library Function - Single Match
// Name: __wsystem
// Library: Visual Studio 2015 Release

int __cdecl __wsystem(wchar_t *_Command)

{
  code *pcVar1;
  errno_t eVar2;
  int *piVar3;
  undefined4 *puVar4;
  HANDLE pvVar5;
  int iVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  ushort *local_1c;
  ushort *local_18;
  undefined *local_14;
  wchar_t *local_10;
  undefined4 local_c;
  uint local_8;
  
  local_8 = DAT_0041e004 ^ (uint)&stack0xfffffffc;
  local_1c = (ushort *)0x0;
  eVar2 = FID_conflict___dupenv_s((wchar_t **)&local_1c,(size_t *)0x0,L"COMSPEC");
  if ((eVar2 != 0) && (eVar2 == 0x16)) {
    uVar10 = 0;
    uVar9 = 0;
    uVar8 = 0;
    uVar7 = 0;
    FUN_00407e93();
    pcVar1 = (code *)swi(3);
    iVar6 = (*pcVar1)(uVar7,uVar8,uVar9,uVar10);
    return iVar6;
  }
  if (_Command == (wchar_t *)0x0) {
    if (local_1c != (ushort *)0x0) {
      __waccess_s((wchar_t *)local_1c,0);
    }
    goto LAB_00408cd6;
  }
  local_18 = (wchar_t *)local_1c;
  local_14 = &DAT_00417d38;
  local_10 = _Command;
  local_c = 0;
  if (local_1c != (ushort *)0x0) {
    piVar3 = (int *)FUN_00407f40();
    iVar6 = *piVar3;
    puVar4 = (undefined4 *)FUN_00407f40();
    *puVar4 = 0;
    pvVar5 = FUN_0040ce68(0,(ushort *)local_18,(wchar_t **)&local_18,(wint_t **)0x0);
    piVar3 = (int *)FUN_00407f40();
    if (pvVar5 != (HANDLE)0xffffffff) {
      *piVar3 = iVar6;
      goto LAB_00408cd6;
    }
    if ((*piVar3 != 2) && (piVar3 = (int *)FUN_00407f40(), *piVar3 != 0xd)) goto LAB_00408cd6;
    piVar3 = (int *)FUN_00407f40();
    *piVar3 = iVar6;
  }
  local_18 = L"cmd.exe";
  FUN_0040d172(0,(ushort *)L"cmd.exe",(wchar_t **)&local_18,(wint_t **)0x0);
LAB_00408cd6:
  FID_conflict__free(local_1c);
  iVar6 = FUN_00401f78();
  return iVar6;
}



undefined4 __cdecl FUN_00408d08(uint param_1,uint *param_2)

{
  uint *puVar1;
  code *pcVar2;
  uint *puVar3;
  uint **ppuVar4;
  uint *puVar5;
  uint *puVar6;
  undefined4 uVar7;
  uint uVar8;
  
  ppuVar4 = (uint **)FUN_0040b71e();
  if (ppuVar4 == (uint **)0x0) {
    return 0;
  }
  puVar1 = *ppuVar4;
  puVar6 = puVar1;
  while (puVar6 != puVar1 + 0x24) {
    if (*puVar6 == param_1) goto LAB_00408d40;
    puVar6 = puVar6 + 3;
  }
  puVar6 = (uint *)0x0;
LAB_00408d40:
  if (puVar6 == (uint *)0x0) {
    return 0;
  }
  pcVar2 = (code *)puVar6[2];
  if (pcVar2 == (code *)0x0) {
    return 0;
  }
  if (pcVar2 == (code *)0x5) {
    puVar6[2] = 0;
    return 1;
  }
  if (pcVar2 == (code *)0x1) {
    return 0xffffffff;
  }
  puVar3 = ppuVar4[1];
  ppuVar4[1] = param_2;
  if (puVar6[1] != 8) {
    uVar8 = puVar6[1];
    puVar6[2] = 0;
    _guard_check_icall();
    (*pcVar2)(uVar8);
    goto LAB_00408e4d;
  }
  puVar5 = puVar1 + 9;
  while (puVar5 != puVar1 + 0x24) {
    puVar5[2] = 0;
    puVar5 = puVar5 + 3;
  }
  puVar1 = ppuVar4[2];
  if (*puVar6 < 0xc0000092) {
    if (*puVar6 == 0xc0000091) {
      puVar5 = (uint *)0x84;
    }
    else {
      if (*puVar6 == 0xc000008d) {
        puVar5 = (uint *)0x82;
      }
      else {
        if (*puVar6 == 0xc000008e) {
          puVar5 = (uint *)0x83;
        }
        else {
          if (*puVar6 == 0xc000008f) {
            puVar5 = (uint *)0x86;
          }
          else {
            puVar5 = puVar1;
            if (*puVar6 != 0xc0000090) goto LAB_00408e2a;
            puVar5 = (uint *)0x81;
          }
        }
      }
    }
LAB_00408e27:
    ppuVar4[2] = puVar5;
  }
  else {
    if (*puVar6 == 0xc0000092) {
      puVar5 = (uint *)0x8a;
      goto LAB_00408e27;
    }
    if (*puVar6 == 0xc0000093) {
      puVar5 = (uint *)0x85;
      goto LAB_00408e27;
    }
    if (*puVar6 == 0xc00002b4) {
      puVar5 = (uint *)0x8e;
      goto LAB_00408e27;
    }
    puVar5 = puVar1;
    if (*puVar6 == 0xc00002b5) {
      puVar5 = (uint *)0x8d;
      goto LAB_00408e27;
    }
  }
LAB_00408e2a:
  uVar7 = 8;
  _guard_check_icall();
  (*pcVar2)(uVar7,puVar5);
  ppuVar4[2] = puVar1;
LAB_00408e4d:
  ppuVar4[1] = puVar3;
  return 0xffffffff;
}



undefined4 FUN_00408e62(void)

{
  return DAT_0041ed34;
}



void __cdecl FUN_00408e68(undefined4 param_1)

{
  DAT_0041ed34 = param_1;
  return;
}



// Library Function - Single Match
// Name: ??$__crt_fast_encode_pointer@P6AHPAU_exception@@@Z@@YAP6AHPAU_exception@@@ZQ6AH0@Z@Z
// Library: Visual Studio 2015 Release
// int (__cdecl*__cdecl __crt_fast_encode_pointer<int (__cdecl*)(struct _exception *)>(int
// (__cdecl*const)(struct _exception *)))(struct _exception *)

FuncDef4 * __cdecl __crt_fast_encode_pointer_int____cdecl___struct__exception___(FuncDef5 *param_1)

{
  byte bVar1;
  
  bVar1 = 0x20 - ((byte)DAT_0041e004 & 0x1f) & 0x1f;
  return (FuncDef4 *)(((uint)param_1 >> bVar1 | (int)param_1 << 0x20 - bVar1) ^ DAT_0041e004);
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

uint FUN_00408e96(void)

{
  uint uVar1;
  byte bVar2;
  
  bVar2 = (byte)DAT_0041e004 & 0x1f;
  uVar1 = (DAT_0041e004 ^ _DAT_0041ed38) >> bVar2 | (DAT_0041e004 ^ _DAT_0041ed38) << 0x20 - bVar2;
  return uVar1 & 0xffffff00 | (uint)(uVar1 != 0);
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void __cdecl FUN_00408eae(undefined4 param_1)

{
  _DAT_0041ed38 = param_1;
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 __cdecl FUN_00408ebd(undefined4 param_1)

{
  undefined4 uVar1;
  byte bVar2;
  code *pcVar3;
  
  bVar2 = (byte)DAT_0041e004 & 0x1f;
  pcVar3 = (code *)((DAT_0041e004 ^ _DAT_0041ed38) >> bVar2 |
                   (DAT_0041e004 ^ _DAT_0041ed38) << 0x20 - bVar2);
  if (pcVar3 == (code *)0x0) {
    uVar1 = 0;
  }
  else {
    _guard_check_icall();
    uVar1 = (*pcVar3)(param_1);
  }
  return uVar1;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address
// Library Function - Single Match
// Name: ___setusermatherr
// Library: Visual Studio 2015 Release

void __cdecl ___setusermatherr(FuncDef5 *param_1)

{
  _DAT_0041ed38 = __crt_fast_encode_pointer_int____cdecl___struct__exception___(param_1);
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

int __cdecl FUN_00408f04(int param_1)

{
  LPCWSTR pWVar1;
  int iVar2;
  LPCWSTR *ppWVar3;
  undefined4 *puVar4;
  LPCWSTR *_Memory;
  short *local_14;
  uint local_10;
  LPCWSTR *local_c;
  uint local_8;
  
  if (param_1 == 0) {
    iVar2 = 0;
  }
  else {
    if ((param_1 == 2) || (param_1 == 1)) {
      iVar2 = 0;
      GetModuleFileNameW((HMODULE)0x0,(LPWSTR)&DAT_0041ed40,0x104);
      _DAT_0041ef68 = &DAT_0041ed40;
      local_14 = DAT_0041ef7c;
      if ((DAT_0041ef7c == (short *)0x0) || (*DAT_0041ef7c == 0)) {
        local_14 = (short *)&DAT_0041ed40;
      }
      local_8 = 0;
      local_10 = 0;
      FUN_00409035(local_14,(short **)0x0,(short *)0x0,(int *)&local_8,(int *)&local_10);
      ppWVar3 = (LPCWSTR *)FUN_004091cc(local_8,local_10,2);
      if (ppWVar3 == (LPCWSTR *)0x0) {
        puVar4 = (undefined4 *)FUN_00407f40();
        iVar2 = 0xc;
        *puVar4 = 0xc;
        _Memory = ppWVar3;
      }
      else {
        FUN_00409035(local_14,ppWVar3,(short *)(ppWVar3 + local_8),(int *)&local_8,(int *)&local_10)
        ;
        if (param_1 == 1) {
          _DAT_0041ef6c = local_8 - 1;
          _Memory = (LPCWSTR *)0x0;
          _DAT_0041ef74 = ppWVar3;
        }
        else {
          local_c = (LPCWSTR *)0x0;
          iVar2 = FUN_0040d907(ppWVar3,(LPCWSTR *)&local_c);
          _Memory = local_c;
          if (iVar2 == 0) {
            _DAT_0041ef6c = 0;
            pWVar1 = *local_c;
            while (pWVar1 != (LPCWSTR)0x0) {
              local_c = local_c + 1;
              _DAT_0041ef6c = _DAT_0041ef6c + 1;
              pWVar1 = *local_c;
            }
            local_c = (LPCWSTR *)0x0;
            iVar2 = 0;
            _DAT_0041ef74 = _Memory;
          }
          FID_conflict__free(local_c);
          local_c = (LPCWSTR *)0x0;
          _Memory = ppWVar3;
        }
      }
      FID_conflict__free(_Memory);
    }
    else {
      puVar4 = (undefined4 *)FUN_00407f40();
      iVar2 = 0x16;
      *puVar4 = 0x16;
      FUN_00407e83();
    }
  }
  return iVar2;
}



void __cdecl FUN_00409035(short *param_1,short **param_2,short *param_3,int *param_4,int *param_5)

{
  bool bVar1;
  bool bVar2;
  short sVar3;
  short *psVar4;
  uint uVar5;
  
  *param_5 = 0;
  *param_4 = 1;
  if (param_2 != (short **)0x0) {
    *param_2 = param_3;
    param_2 = param_2 + 1;
  }
  bVar1 = false;
  bVar2 = false;
  do {
    sVar3 = 0x22;
    if (*param_1 == 0x22) {
      bVar1 = !bVar1;
      bVar2 = bVar1;
    }
    else {
      *param_5 = *param_5 + 1;
      if (param_3 != (short *)0x0) {
        *param_3 = *param_1;
        param_3 = param_3 + 1;
      }
      sVar3 = *param_1;
      if (sVar3 == 0) goto LAB_004090c2;
    }
    param_1 = param_1 + 1;
  } while ((bVar2) || ((sVar3 != 0x20 && (sVar3 != 9))));
  if (param_3 != (short *)0x0) {
    param_3[-1] = 0;
  }
LAB_004090c2:
  bVar1 = false;
  while( true ) {
    sVar3 = *param_1;
    if (sVar3 != 0) {
      while ((sVar3 == 0x20 || (sVar3 == 9))) {
        param_1 = param_1 + 1;
        sVar3 = *param_1;
      }
    }
    if (sVar3 == 0) break;
    if (param_2 != (short **)0x0) {
      *param_2 = param_3;
      param_2 = param_2 + 1;
    }
    *param_4 = *param_4 + 1;
    while( true ) {
      sVar3 = *param_1;
      uVar5 = 0;
      bVar2 = true;
      psVar4 = param_1;
      while (sVar3 == 0x5c) {
        psVar4 = psVar4 + 1;
        uVar5 = uVar5 + 1;
        sVar3 = *psVar4;
      }
      param_1 = psVar4;
      if (sVar3 == 0x22) {
        param_1 = psVar4;
        if (((uVar5 & 1) == 0) && ((!bVar1 || (param_1 = psVar4 + 1, *param_1 != 0x22)))) {
          bVar2 = false;
          bVar1 = !bVar1;
          param_1 = psVar4;
        }
        uVar5 = uVar5 >> 1;
      }
      while (uVar5 != 0) {
        uVar5 = uVar5 - 1;
        if (param_3 != (short *)0x0) {
          *param_3 = 0x5c;
          param_3 = param_3 + 1;
        }
        *param_5 = *param_5 + 1;
      }
      sVar3 = *param_1;
      if ((sVar3 == 0) || ((!bVar1 && ((sVar3 == 0x20 || (sVar3 == 9)))))) break;
      if (bVar2) {
        if (param_3 != (short *)0x0) {
          *param_3 = sVar3;
          param_3 = param_3 + 1;
        }
        *param_5 = *param_5 + 1;
      }
      param_1 = param_1 + 1;
    }
    if (param_3 != (short *)0x0) {
      *param_3 = 0;
      param_3 = param_3 + 1;
    }
    *param_5 = *param_5 + 1;
  }
  if (param_2 != (short **)0x0) {
    *param_2 = (short *)0x0;
  }
  *param_4 = *param_4 + 1;
  return;
}



LPVOID __cdecl FUN_004091cc(uint param_1,uint param_2,uint param_3)

{
  LPVOID pvVar1;
  
  if ((param_1 < 0x3fffffff) && (param_2 < (uint)(0xffffffff / (ulonglong)param_3))) {
    if (param_2 * param_3 < ~(param_1 * 4)) {
      pvVar1 = __calloc_base(param_1 * 4 + param_2 * param_3,1);
      FID_conflict__free((void *)0x0);
      return pvVar1;
    }
  }
  return (LPVOID)0x0;
}



void __cdecl FUN_0040921b(int param_1)

{
  FUN_00408f04(param_1);
  return;
}



int FUN_00409226(void)

{
  int iVar1;
  
  iVar1 = DAT_0041ef50;
  if (DAT_0041ef50 == 0) {
    if (DAT_0041ef4c != 0) {
      iVar1 = FUN_00409252();
      if (iVar1 == 0) {
        return DAT_0041ef50;
      }
      iVar1 = FUN_004093de();
      if (iVar1 == 0) {
        return DAT_0041ef50;
      }
    }
    iVar1 = 0;
  }
  return iVar1;
}



undefined4 FUN_00409252(void)

{
  undefined8 *_Memory;
  wchar_t **ppwVar1;
  undefined4 uVar2;
  
  if (DAT_0041ef50 != (wchar_t **)0x0) {
    return 0;
  }
  _Memory = FUN_0040e1a6();
  if (_Memory == (undefined8 *)0x0) {
    uVar2 = 0xffffffff;
  }
  else {
    ppwVar1 = FUN_004092a0((wchar_t *)_Memory);
    if (ppwVar1 == (wchar_t **)0x0) {
      uVar2 = 0xffffffff;
    }
    else {
      uVar2 = 0;
      DAT_0041ef50 = ppwVar1;
      DAT_0041ef54 = ppwVar1;
    }
    FID_conflict__free((void *)0x0);
  }
  FID_conflict__free(_Memory);
  return uVar2;
}



wchar_t ** __cdecl FUN_004092a0(wchar_t *param_1)

{
  uint _SizeInWords;
  wchar_t wVar1;
  wchar_t wVar2;
  code *pcVar3;
  errno_t eVar4;
  wchar_t *pwVar5;
  wchar_t *_Dst;
  int iVar6;
  wchar_t **ppwVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  wchar_t **local_c;
  
  iVar6 = 0;
  wVar2 = *param_1;
  _Dst = param_1;
  while (wVar2 != L'\0') {
    if (wVar2 != L'=') {
      iVar6 = iVar6 + 1;
    }
    pwVar5 = _Dst;
    do {
      wVar2 = *pwVar5;
      pwVar5 = pwVar5 + 1;
    } while (wVar2 != L'\0');
    _Dst = _Dst + ((int)((int)pwVar5 - (int)(_Dst + 1)) >> 1) + 1;
    wVar2 = *_Dst;
  }
  ppwVar7 = (wchar_t **)__calloc_base(iVar6 + 1,4);
  if (ppwVar7 != (wchar_t **)0x0) {
    wVar2 = *param_1;
    local_c = ppwVar7;
    while (wVar2 != L'\0') {
      _Dst = param_1;
      do {
        wVar1 = *_Dst;
        _Dst = _Dst + 1;
      } while (wVar1 != L'\0');
      _SizeInWords = ((int)((int)_Dst - (int)(param_1 + 1)) >> 1) + 1;
      if (wVar2 != L'=') {
        _Dst = (wchar_t *)__calloc_base(_SizeInWords,2);
        if (_Dst == (wchar_t *)0x0) {
          free_environment__(ppwVar7);
          ppwVar7 = (wchar_t **)0x0;
          FID_conflict__free((void *)0x0);
          break;
        }
        eVar4 = _wcscpy_s(_Dst,_SizeInWords,param_1);
        if (eVar4 != 0) {
          uVar11 = 0;
          uVar10 = 0;
          uVar9 = 0;
          uVar8 = 0;
          FUN_00407e93();
          pcVar3 = (code *)swi(3);
          ppwVar7 = (wchar_t **)(*pcVar3)(uVar8,uVar9,uVar10,uVar11);
          return ppwVar7;
        }
        *local_c = _Dst;
        local_c = local_c + 1;
        FID_conflict__free((void *)0x0);
      }
      param_1 = param_1 + _SizeInWords;
      wVar2 = *param_1;
    }
  }
  FID_conflict__free((void *)0x0);
  return ppwVar7;
}



// Library Function - Multiple Matches With Same Base Name
// Name: free_environment<>
// Library: Visual Studio 2015 Release

void __cdecl free_environment__(void **param_1)

{
  void *_Memory;
  void **ppvVar1;
  
  if (param_1 != (void **)0x0) {
    _Memory = *param_1;
    ppvVar1 = param_1;
    while (_Memory != (void *)0x0) {
      FID_conflict__free(_Memory);
      ppvVar1 = ppvVar1 + 1;
      _Memory = *ppvVar1;
    }
    FID_conflict__free(param_1);
  }
  return;
}



undefined4 FUN_004093de(void)

{
  LPCSTR pCVar1;
  uint uVar2;
  ushort *_Memory;
  int iVar3;
  undefined4 uVar4;
  LPCSTR *ppCVar5;
  
  if (DAT_0041ef4c == (LPCSTR *)0x0) {
LAB_00409454:
    uVar4 = 0xffffffff;
  }
  else {
    pCVar1 = *DAT_0041ef4c;
    ppCVar5 = DAT_0041ef4c;
    while (pCVar1 != (LPCSTR)0x0) {
      uVar2 = FUN_0040e0f3(0,0,pCVar1,-1,(LPWSTR)0x0,0);
      if (uVar2 == 0) goto LAB_00409454;
      _Memory = (ushort *)__calloc_base(uVar2,2);
      if ((_Memory == (ushort *)0x0) ||
         (iVar3 = FUN_0040e0f3(0,0,*ppCVar5,-1,(LPWSTR)_Memory,uVar2), iVar3 == 0)) {
        FID_conflict__free(_Memory);
        goto LAB_00409454;
      }
      FUN_0040e57d(_Memory,0);
      FID_conflict__free((void *)0x0);
      ppCVar5 = ppCVar5 + 1;
      pCVar1 = *ppCVar5;
    }
    uVar4 = 0;
  }
  return uVar4;
}



void __thiscall FUN_0040945b(void *this,undefined *param_1)

{
  void *pvVar1;
  void *pvVar2;
  
  pvVar1 = (void *)((int)this + 4);
  while (this != pvVar1) {
    pvVar2 = this;
    _guard_check_icall();
    (*(code *)param_1)(pvVar2);
    this = (void *)((int)this + 4);
  }
  return;
}



// Library Function - Multiple Matches With Same Base Name
// Name: uninitialize_environment_internal<>
// Library: Visual Studio 2015 Release

void __cdecl uninitialize_environment_internal__(void **param_1)

{
  if ((void **)*param_1 != DAT_0041ef58) {
    free_environment__((void **)*param_1);
  }
  return;
}



// Library Function - Multiple Matches With Same Base Name
// Name: uninitialize_environment_internal<>
// Library: Visual Studio 2015 Release

void __cdecl uninitialize_environment_internal__(void **param_1)

{
  if ((void **)*param_1 != DAT_0041ef54) {
    free_environment__((void **)*param_1);
  }
  return;
}



int thunk_FUN_00409226(void)

{
  int iVar1;
  
  iVar1 = DAT_0041ef50;
  if (DAT_0041ef50 == 0) {
    if (DAT_0041ef4c != 0) {
      iVar1 = FUN_00409252();
      if (iVar1 == 0) {
        return DAT_0041ef50;
      }
      iVar1 = FUN_004093de();
      if (iVar1 == 0) {
        return DAT_0041ef50;
      }
    }
    iVar1 = 0;
  }
  return iVar1;
}



void FUN_004094bf(void)

{
  FUN_0040945b(&DAT_0041ef4c,uninitialize_environment_internal__);
  FUN_0040945b(&DAT_0041ef50,uninitialize_environment_internal__);
  free_environment__(DAT_0041ef58);
  free_environment__(DAT_0041ef54);
  return;
}



void FUN_004094f6(void)

{
  if (DAT_0041ef54 == 0) {
    DAT_0041ef54 = FUN_00409226();
  }
  return;
}



undefined4 thunk_FUN_00409252(void)

{
  undefined8 *_Memory;
  wchar_t **ppwVar1;
  undefined4 uVar2;
  
  if (DAT_0041ef50 != (wchar_t **)0x0) {
    return 0;
  }
  _Memory = FUN_0040e1a6();
  if (_Memory == (undefined8 *)0x0) {
    uVar2 = 0xffffffff;
  }
  else {
    ppwVar1 = FUN_004092a0((wchar_t *)_Memory);
    if (ppwVar1 == (wchar_t **)0x0) {
      uVar2 = 0xffffffff;
    }
    else {
      uVar2 = 0;
      DAT_0041ef50 = ppwVar1;
      DAT_0041ef54 = ppwVar1;
    }
    FID_conflict__free((void *)0x0);
  }
  FID_conflict__free(_Memory);
  return uVar2;
}



void __cdecl FUN_0040950f(undefined **param_1,undefined **param_2)

{
  code *pcVar1;
  uint uVar2;
  uint uVar3;
  
  uVar3 = 0;
  uVar2 = ~-(uint)(param_2 < param_1) & (uint)((int)param_2 + (3 - (int)param_1)) >> 2;
  if (uVar2 != 0) {
    do {
      pcVar1 = (code *)*param_1;
      if (pcVar1 != (code *)0x0) {
        _guard_check_icall();
        (*pcVar1)();
      }
      param_1 = (code **)param_1 + 1;
      uVar3 = uVar3 + 1;
    } while (uVar3 != uVar2);
  }
  return;
}



int __cdecl FUN_00409556(undefined **param_1,undefined **param_2)

{
  code *pcVar1;
  int iVar2;
  
  do {
    if (param_1 == param_2) {
      return 0;
    }
    pcVar1 = (code *)*param_1;
    if (pcVar1 != (code *)0x0) {
      _guard_check_icall();
      iVar2 = (*pcVar1)();
      if (iVar2 != 0) {
        return iVar2;
      }
    }
    param_1 = (code **)param_1 + 1;
  } while( true );
}



// WARNING: Function: __SEH_prolog4 replaced with injection: SEH_prolog4
// WARNING: Function: __SEH_epilog4 replaced with injection: EH_epilog3
// Library Function - Multiple Matches With Same Base Name
// Name: operator()<>
// Library: Visual Studio 2015 Release

void operator____(int *param_1,int **param_2)

{
  ___acrt_lock(*param_1);
  FUN_004095c7(param_2);
  FUN_004095bb();
  return;
}



void FUN_004095bb(void)

{
  int unaff_EBP;
  
  ___acrt_unlock(**(int **)(unaff_EBP + 0x10));
  return;
}



// WARNING: Function: __SEH_prolog4 replaced with injection: SEH_prolog4
// WARNING: Function: __SEH_epilog4 replaced with injection: EH_epilog3

void __fastcall FUN_004095c7(int **param_1)

{
  byte bVar1;
  uint uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined uVar5;
  undefined4 uVar6;
  
  if (DAT_0041ef64 != 0) {
    return;
  }
  DAT_0041ef5c = 1;
  if (**param_1 == 0) {
    bVar1 = (byte)DAT_0041e004 & 0x1f;
    if (DAT_0041ef60 != DAT_0041e004) {
      uVar2 = DAT_0041e004 ^ DAT_0041ef60;
      uVar6 = 0;
      uVar4 = 0;
      uVar3 = 0;
      _guard_check_icall();
      (*(code *)(uVar2 >> bVar1 | uVar2 << 0x20 - bVar1))(uVar3,uVar4,uVar6);
    }
    uVar5 = 0x8c;
  }
  else {
    if (**param_1 != 1) goto LAB_00409635;
    uVar5 = 0x98;
  }
  FUN_00409c7d(uVar5);
LAB_00409635:
  if (**param_1 == 0) {
    FUN_0040950f((undefined **)&DAT_00417188,(undefined **)&DAT_00417198);
  }
  FUN_0040950f((undefined **)&DAT_0041719c,(undefined **)&DAT_004171a0);
  if (*param_1[1] == 0) {
    DAT_0041ef64 = 1;
    *(undefined *)param_1[2] = 1;
  }
  return;
}



uint __cdecl FUN_00409697(int param_1)

{
  return (uint)(param_1 == -0x1f928c9d);
}



void __cdecl FUN_004096aa(UINT param_1,undefined4 param_2,int param_3)

{
  code *pcVar1;
  uint uVar2;
  int *local_1c;
  int *local_18;
  undefined *local_14;
  int local_10;
  undefined4 local_c;
  undefined local_5;
  
  if (param_3 == 0) {
    uVar2 = FUN_004027c5();
    if ((char)uVar2 != 0) {
      FUN_00409752(param_1);
    }
  }
  local_1c = &param_2;
  local_5 = 0;
  local_18 = &param_3;
  local_14 = &local_5;
  local_c = 2;
  local_10 = 2;
  operator____(&local_10,&local_1c);
  if (param_3 != 0) {
    return;
  }
  FUN_00409710(param_1);
  pcVar1 = (code *)swi(3);
  (*pcVar1)();
  return;
}



void FUN_00409710(UINT param_1)

{
  code *pcVar1;
  int iVar2;
  HANDLE hProcess;
  int in_FS_OFFSET;
  UINT uExitCode;
  
  iVar2 = FUN_0040e628();
  if ((iVar2 != 1) && ((*(uint *)(*(int *)(in_FS_OFFSET + 0x30) + 0x68) >> 8 & 1) == 0)) {
    uExitCode = param_1;
    hProcess = GetCurrentProcess();
    TerminateProcess(hProcess,uExitCode);
  }
  FUN_00409752(param_1);
  ExitProcess(param_1);
  pcVar1 = (code *)swi(3);
  (*pcVar1)();
  return;
}



void __cdecl FUN_00409752(undefined4 param_1)

{
  BOOL BVar1;
  FARPROC pFVar2;
  HMODULE local_8;
  
  local_8 = (HMODULE)0x0;
  BVar1 = GetModuleHandleExW(0,L"mscoree.dll",&local_8);
  if ((BVar1 != 0) && (pFVar2 = GetProcAddress(local_8,"CorExitProcess"), pFVar2 != (FARPROC)0x0)) {
    _guard_check_icall();
    (*pFVar2)(param_1);
  }
  if (local_8 != (HMODULE)0x0) {
    FreeLibrary(local_8);
  }
  return;
}



void __cdecl FUN_004097a7(undefined4 param_1)

{
  DAT_0041ef60 = param_1;
  return;
}



void FUN_004097b6(void)

{
  FUN_004096aa(0,2,1);
  return;
}



void FUN_004097c5(void)

{
  FUN_004096aa(0,0,1);
  return;
}



void __cdecl FUN_004097d4(UINT param_1)

{
  FUN_004096aa(param_1,2,0);
  return;
}



// WARNING: Function: __SEH_prolog4 replaced with injection: SEH_prolog4

void __cdecl FUN_004097ea(FuncDef5 *param_1)

{
  code *pcVar1;
  uint uVar2;
  
  if (DAT_0041ef60 == (FuncDef4 *)DAT_0041e004) {
    DAT_0041ef60 = __crt_fast_encode_pointer_int____cdecl___struct__exception___(param_1);
    return;
  }
  uVar2 = FUN_0040b5c7();
  pcVar1 = *(code **)(uVar2 + 0xc);
  if (pcVar1 != (code *)0x0) {
    _guard_check_icall();
    (*pcVar1)();
  }
  FUN_00409f3e();
  pcVar1 = (code *)swi(3);
  (*pcVar1)();
  return;
}



void __cdecl FUN_00409810(UINT param_1)

{
  FUN_004096aa(param_1,0,0);
  return;
}



// Library Function - Single Match
// Name: __set_fmode
// Library: Visual Studio 2015 Release

errno_t __cdecl __set_fmode(int _Mode)

{
  undefined4 *puVar1;
  
  if (((_Mode != 0x4000) && (_Mode != 0x8000)) && (_Mode != 0x10000)) {
    puVar1 = (undefined4 *)FUN_00407f40();
    *puVar1 = 0x16;
    FUN_00407e83();
    return 0x16;
  }
  DAT_0041f404 = _Mode;
  return 0;
}



undefined * FUN_0040987c(void)

{
  return &DAT_0041ef6c;
}



undefined * FUN_00409882(void)

{
  return &DAT_0041ef74;
}



// WARNING: Function: __SEH_prolog4 replaced with injection: SEH_prolog4
// WARNING: Function: __SEH_epilog4 replaced with injection: EH_epilog3

void FUN_00409888(int *param_1)

{
  undefined **ppuVar1;
  undefined **ppuVar2;
  
  ___acrt_lock(*param_1);
  ppuVar2 = (undefined **)&DAT_0041f298;
  while (ppuVar2 != (undefined **)&DAT_0041f29c) {
    if ((undefined **)*ppuVar2 != &PTR_DAT_0041e140) {
      ppuVar1 = __updatetlocinfoEx_nolock(ppuVar2,&PTR_DAT_0041e140);
      *(undefined ***)ppuVar2 = ppuVar1;
    }
    ppuVar2 = ppuVar2 + 1;
  }
  FUN_004098e0();
  return;
}



void FUN_004098e0(void)

{
  int unaff_EBP;
  
  ___acrt_unlock(**(int **)(unaff_EBP + 0x10));
  return;
}



undefined4 FUN_004098ec(void)

{
  undefined4 uVar1;
  
  uVar1 = DAT_0041ef80;
  DAT_0041ef80 = 1;
  return uVar1;
}



void FUN_004098f7(void)

{
  int local_10;
  undefined4 local_c;
  
  local_c = 4;
  local_10 = 4;
  FUN_00409888(&local_10);
  return;
}



// Library Function - Single Match
// Name: __configthreadlocale
// Library: Visual Studio 2015 Release

int __cdecl __configthreadlocale(int _Flag)

{
  uint uVar1;
  uint uVar2;
  undefined4 *puVar3;
  uint uVar4;
  
  uVar2 = FUN_0040b5c7();
  uVar1 = *(uint *)(uVar2 + 0x350);
  if (_Flag == -1) {
    DAT_0041e780 = 0xffffffff;
  }
  else {
    if (_Flag != 0) {
      if (_Flag == 1) {
        uVar4 = uVar1 | 2;
      }
      else {
        if (_Flag != 2) {
          puVar3 = (undefined4 *)FUN_00407f40();
          *puVar3 = 0x16;
          FUN_00407e83();
          return -1;
        }
        uVar4 = uVar1 & 0xfffffffd;
      }
      *(uint *)(uVar2 + 0x350) = uVar4;
    }
  }
  return (uint)((uVar1 & 2) == 0) + 1;
}



undefined4 FUN_00409982(void)

{
  return DAT_0041ef84;
}



// Library Function - Single Match
// Name: __set_new_mode
// Library: Visual Studio 2015 Release

undefined4 __cdecl __set_new_mode(int param_1)

{
  undefined4 *puVar1;
  undefined4 uVar2;
  
  if ((param_1 != 0) && (param_1 != 1)) {
    puVar1 = (undefined4 *)FUN_00407f40();
    *puVar1 = 0x16;
    FUN_00407e83();
    return 0xffffffff;
  }
  uVar2 = DAT_0041ef84;
  DAT_0041ef84 = param_1;
  return uVar2;
}



undefined * FUN_004099b8(void)

{
  return &DAT_0041ef88;
}



// WARNING: Function: __SEH_prolog4 replaced with injection: SEH_prolog4
// WARNING: Function: __SEH_epilog4 replaced with injection: EH_epilog3
// Library Function - Multiple Matches With Same Base Name
// Name: operator()<>
// Library: Visual Studio 2015 Release

undefined4 operator____(int *param_1,uint **param_2)

{
  undefined4 uVar1;
  
  ___acrt_lock(*param_1);
  uVar1 = FUN_00409b8b(param_2);
  FUN_00409a03();
  return uVar1;
}



void FUN_00409a03(void)

{
  int unaff_EBP;
  
  ___acrt_unlock(**(int **)(unaff_EBP + 0x10));
  return;
}



// WARNING: Function: __SEH_prolog4 replaced with injection: SEH_prolog4
// WARNING: Function: __SEH_epilog4 replaced with injection: EH_epilog3
// Library Function - Multiple Matches With Same Base Name
// Name: operator()<>
// Library: Visual Studio 2015 Release

undefined4 operator____(int *param_1,uint **param_2)

{
  undefined4 uVar1;
  
  ___acrt_lock(*param_1);
  uVar1 = FUN_00409a60(param_2);
  FUN_00409a54();
  return uVar1;
}



void FUN_00409a54(void)

{
  int unaff_EBP;
  
  ___acrt_unlock(**(int **)(unaff_EBP + 0x10));
  return;
}



undefined4 __fastcall FUN_00409a60(uint **param_1)

{
  uint *puVar1;
  void *pvVar2;
  FuncDef4 *pFVar3;
  undefined4 uVar4;
  byte bVar5;
  FuncDef4 **ppFVar6;
  void *pvVar7;
  uint uVar8;
  FuncDef4 **ppFVar9;
  uint uVar10;
  FuncDef4 **ppFVar11;
  
  puVar1 = (uint *)**param_1;
  if (puVar1 == (uint *)0x0) {
LAB_00409b81:
    uVar4 = 0xffffffff;
  }
  else {
    bVar5 = (byte)DAT_0041e004 & 0x1f;
    ppFVar11 = (FuncDef4 **)
               ((puVar1[1] ^ (uint)DAT_0041e004) >> bVar5 |
               (puVar1[1] ^ (uint)DAT_0041e004) << 0x20 - bVar5);
    ppFVar9 = (FuncDef4 **)
              ((puVar1[2] ^ (uint)DAT_0041e004) >> bVar5 |
              (puVar1[2] ^ (uint)DAT_0041e004) << 0x20 - bVar5);
    pvVar7 = (void *)((*puVar1 ^ (uint)DAT_0041e004) >> bVar5 |
                     (*puVar1 ^ (uint)DAT_0041e004) << 0x20 - bVar5);
    pvVar2 = pvVar7;
    if (ppFVar11 == ppFVar9) {
      uVar8 = (int)((int)ppFVar9 - (int)pvVar7) >> 2;
      uVar10 = 0x200;
      if (uVar8 < 0x201) {
        uVar10 = uVar8;
      }
      uVar10 = uVar10 + uVar8;
      if (uVar10 == 0) {
        uVar10 = 0x20;
      }
      if (uVar10 < uVar8) {
LAB_00409add:
        uVar10 = uVar8 + 4;
        pvVar2 = __recalloc_base(pvVar7,uVar10,4);
        FID_conflict__free((void *)0x0);
        if (pvVar2 == (LPVOID)0x0) goto LAB_00409b81;
      }
      else {
        pvVar2 = __recalloc_base(pvVar7,uVar10,4);
        FID_conflict__free((void *)0x0);
        if (pvVar2 == (LPVOID)0x0) goto LAB_00409add;
      }
      pFVar3 = DAT_0041e004;
      ppFVar11 = (FuncDef4 **)((int)pvVar2 + uVar8 * 4);
      ppFVar9 = (FuncDef4 **)((int)pvVar2 + uVar10 * 4);
      uVar10 = ~-(uint)(ppFVar9 < ppFVar11) & (uint)((int)ppFVar9 + (3 - (int)ppFVar11)) >> 2;
      if (uVar10 != 0) {
        uVar8 = 0;
        ppFVar6 = ppFVar11;
        do {
          uVar8 = uVar8 + 1;
          *ppFVar6 = pFVar3;
          ppFVar6 = ppFVar6 + 1;
        } while (uVar8 != uVar10);
      }
    }
    pFVar3 = __crt_fast_encode_pointer_int____cdecl___struct__exception___((FuncDef5 *)*param_1[1]);
    *ppFVar11 = pFVar3;
    uVar10 = FUN_004022fb((uint)pvVar2);
    *(uint *)**param_1 = uVar10;
    uVar10 = FUN_004022fb((uint)(ppFVar11 + 1));
    *(uint *)(**param_1 + 4) = uVar10;
    uVar10 = FUN_004022fb((uint)ppFVar9);
    *(uint *)(**param_1 + 8) = uVar10;
    uVar4 = 0;
  }
  return uVar4;
}



undefined4 __fastcall FUN_00409b8b(uint **param_1)

{
  uint uVar1;
  undefined4 uVar2;
  uint uVar3;
  uint *puVar4;
  sbyte sVar5;
  uint uVar6;
  uint *puVar7;
  uint *_Memory;
  uint *puVar8;
  
  uVar1 = DAT_0041e004;
  puVar8 = (uint *)**param_1;
  if (puVar8 == (uint *)0x0) {
    uVar2 = 0xffffffff;
  }
  else {
    uVar6 = DAT_0041e004 & 0x1f;
    sVar5 = (sbyte)uVar6;
    _Memory = (uint *)((*puVar8 ^ DAT_0041e004) >> sVar5 | (*puVar8 ^ DAT_0041e004) << 0x20 - sVar5)
    ;
    puVar8 = (uint *)((puVar8[1] ^ DAT_0041e004) >> sVar5 |
                     (puVar8[1] ^ DAT_0041e004) << 0x20 - sVar5);
    if ((_Memory != (uint *)0x0) &&
       (uVar3 = DAT_0041e004, puVar8 = puVar8, _Memory != (uint *)0xffffffff)) {
      while (puVar8 = puVar8 + -1, _Memory <= puVar8) {
        if (*puVar8 != uVar1) {
          uVar3 = *puVar8 ^ uVar3;
          *puVar8 = uVar1;
          _guard_check_icall();
          (*(code *)(uVar3 >> (sbyte)uVar6 | uVar3 << 0x20 - (sbyte)uVar6))();
          uVar6 = DAT_0041e004 & 0x1f;
          uVar3 = *(uint *)**param_1 ^ DAT_0041e004;
          sVar5 = (sbyte)uVar6;
          puVar7 = (uint *)(uVar3 >> sVar5 | uVar3 << 0x20 - sVar5);
          uVar3 = ((uint *)**param_1)[1] ^ DAT_0041e004;
          puVar4 = (uint *)(uVar3 >> sVar5 | uVar3 << 0x20 - sVar5);
          if ((puVar7 != _Memory) || (uVar3 = DAT_0041e004, puVar4 != puVar8)) {
            uVar3 = DAT_0041e004;
            puVar8 = puVar4;
            puVar8 = puVar4;
            _Memory = puVar7;
          }
        }
      }
      if (_Memory != (uint *)0xffffffff) {
        FID_conflict__free(_Memory);
        uVar3 = DAT_0041e004;
      }
      *(uint *)**param_1 = uVar3;
      *(uint *)(**param_1 + 4) = uVar3;
      *(uint *)(**param_1 + 8) = uVar3;
    }
    uVar2 = 0;
  }
  return uVar2;
}



// Library Function - Single Match
// Name: __crt_atexit
// Library: Visual Studio 2015 Release

void __crt_atexit(undefined4 param_1)

{
  FUN_00409cd5(0x8c,(undefined1)param_1);
  return;
}



void FUN_00409c7d(undefined1 param_1)

{
  int local_14;
  uint *local_10;
  undefined4 local_c;
  
  local_10 = (uint *)&param_1;
  local_c = 2;
  local_14 = 2;
  operator____(&local_14,&local_10);
  return;
}



undefined4 __cdecl FUN_00409cac(int *param_1)

{
  int iVar1;
  
  iVar1 = DAT_0041e004;
  if (param_1 == (int *)0x0) {
    return 0xffffffff;
  }
  if (*param_1 == param_1[2]) {
    *param_1 = DAT_0041e004;
    param_1[1] = iVar1;
    param_1[2] = iVar1;
  }
  return 0;
}



void FUN_00409cd5(undefined1 param_1,undefined1 param_2)

{
  uint *local_18;
  undefined1 *local_14;
  int local_10;
  undefined4 local_c;
  
  local_18 = (uint *)&param_1;
  local_14 = &param_2;
  local_c = 2;
  local_10 = 2;
  operator____(&local_10,&local_18);
  return;
}



undefined FUN_00409d70(void)

{
  FUN_00409dd9((int **)&DAT_0041f29c);
  return 1;
}



void FUN_00409dd9(int **param_1)

{
  int iVar1;
  
  LOCK();
  iVar1 = **param_1 + -1;
  **param_1 = iVar1;
  if ((iVar1 == 0) && (*param_1 != (int *)&DAT_0041e208)) {
    FID_conflict__free(*param_1);
    *(undefined **)param_1 = &DAT_0041e208;
  }
  return;
}



// Library Function - Single Match
// Name: ___acrt_initialize
// Library: Visual Studio 2015 Release

void ___acrt_initialize(void)

{
  FUN_0040f0f2(&PTR_LAB_00417e08,&PTR_u_api_ms_win_core_datetime_l1_1_1_00417e88);
  return;
}



undefined __cdecl FUN_00409e19(char param_1)

{
  undefined uVar1;
  
  if (param_1 != 0) {
    if (DAT_0041ed28 != 0) {
      FUN_0040a868();
    }
    return 1;
  }
  uVar1 = FUN_0040f15a((int)&PTR_LAB_00417e08,(int)&PTR_u_api_ms_win_core_datetime_l1_1_1_00417e88);
  return uVar1;
}



// Library Function - Single Match
// Name: __controlfp_s
// Libraries: Visual Studio 2012 Release, Visual Studio 2015 Release

errno_t __cdecl __controlfp_s(uint *_CurrentState,uint _NewValue,uint _Mask)

{
  uint uVar1;
  undefined4 *puVar2;
  errno_t eVar3;
  
  _Mask = _Mask & 0xfff7ffff;
  if ((_NewValue & _Mask & 0xfcf0fce0) == 0) {
    if (_CurrentState == (uint *)0x0) {
      FUN_0040f69b(_NewValue,_Mask);
    }
    else {
      uVar1 = FUN_0040f69b(_NewValue,_Mask);
      *_CurrentState = uVar1;
    }
    eVar3 = 0;
  }
  else {
    if (_CurrentState != (uint *)0x0) {
      uVar1 = FUN_0040f69b(0,0);
      *_CurrentState = uVar1;
    }
    puVar2 = (undefined4 *)FUN_00407f40();
    eVar3 = 0x16;
    *puVar2 = 0x16;
    FUN_00407e83();
  }
  return eVar3;
}



// WARNING: Function: __SEH_prolog4 replaced with injection: SEH_prolog4
// Library Function - Single Match
// Name: _terminate
// Library: Visual Studio 2015 Release

void _terminate(void)

{
  code *pcVar1;
  uint uVar2;
  
  uVar2 = FUN_0040b5c7();
  pcVar1 = *(code **)(uVar2 + 0xc);
  if (pcVar1 != (code *)0x0) {
    _guard_check_icall();
    (*pcVar1)();
  }
  FUN_00409f3e();
  pcVar1 = (code *)swi(3);
  (*pcVar1)();
  return;
}



// Library Function - Single Match
// Name: _strcpy_s
// Library: Visual Studio 2015 Release

errno_t __cdecl _strcpy_s(char *_Dst,rsize_t _SizeInBytes,char *_Src)

{
  char cVar1;
  errno_t *peVar2;
  char *pcVar3;
  errno_t eVar4;
  
  if ((_Dst != (char *)0x0) && (_SizeInBytes != 0)) {
    if (_Src != (char *)0x0) {
      pcVar3 = _Dst;
      do {
        cVar1 = (_Src + -(int)_Dst)[(int)pcVar3];
        *pcVar3 = cVar1;
        pcVar3 = pcVar3 + 1;
        if (cVar1 == 0) break;
        _SizeInBytes = _SizeInBytes - 1;
      } while (_SizeInBytes != 0);
      if (_SizeInBytes != 0) {
        return 0;
      }
      *_Dst = 0;
      peVar2 = (errno_t *)FUN_00407f40();
      eVar4 = 0x22;
      goto LAB_00409f09;
    }
    *_Dst = 0;
  }
  peVar2 = (errno_t *)FUN_00407f40();
  eVar4 = 0x16;
LAB_00409f09:
  *peVar2 = eVar4;
  FUN_00407e83();
  return eVar4;
}



void FUN_00409f3e(void)

{
  code *pcVar1;
  int iVar2;
  BOOL BVar3;
  
  iVar2 = FUN_0040f2dd();
  if (iVar2 != 0) {
    FUN_0040f32d(0x16);
  }
  if ((DAT_0041e0f0 & 2) != 0) {
    BVar3 = IsProcessorFeaturePresent(0x17);
    if (BVar3 != 0) {
      pcVar1 = (code *)swi(0x29);
      (*pcVar1)();
      return;
    }
    ___acrt_call_reportfault(3,0x40000015,1);
  }
  FUN_004097d4(3);
  pcVar1 = (code *)swi(3);
  (*pcVar1)();
  return;
}



// Library Function - Single Match
// Name: _calloc
// Library: Visual Studio 2015 Release

void * __cdecl _calloc(size_t _Count,size_t _Size)

{
  LPVOID pvVar1;
  
  pvVar1 = __calloc_base(_Count,_Size);
  return pvVar1;
}



// Library Function - Single Match
// Name: _wcsncmp
// Library: Visual Studio 2015 Release

int __cdecl _wcsncmp(wchar_t *_Str1,wchar_t *_Str2,size_t _MaxCount)

{
  if (_MaxCount != 0) {
    while (((_MaxCount = _MaxCount - 1, _MaxCount != 0 && (*_Str1 != 0)) && (*_Str1 == *_Str2))) {
      _Str1 = (wchar_t *)((ushort *)_Str1 + 1);
      _Str2 = (wchar_t *)((ushort *)_Str2 + 1);
    }
    return (uint)(ushort)*_Str1 - (uint)(ushort)*_Str2;
  }
  return _MaxCount;
}



// Library Function - Single Match
// Name: __calloc_base
// Library: Visual Studio 2015 Release

LPVOID __cdecl __calloc_base(uint param_1,uint param_2)

{
  int iVar1;
  LPVOID pvVar2;
  undefined4 *puVar3;
  SIZE_T dwBytes;
  
  if ((param_1 == 0) || (param_2 <= 0xffffffe0 / param_1)) {
    dwBytes = param_1 * param_2;
    if (dwBytes == 0) {
      dwBytes = 1;
    }
    do {
      pvVar2 = HeapAlloc(hHeap_0041f410,8,dwBytes);
      if (pvVar2 != (LPVOID)0x0) {
        return pvVar2;
      }
      iVar1 = FUN_00409982();
    } while ((iVar1 != 0) && (iVar1 = FUN_0040f19a(dwBytes), iVar1 != 0));
  }
  puVar3 = (undefined4 *)FUN_00407f40();
  *puVar3 = 0xc;
  return (LPVOID)0x0;
}



// Library Function - Multiple Matches With Different Base Names
// Name: __free_base, _free
// Libraries: Visual Studio 2012 Release, Visual Studio 2015 Release

void __cdecl FID_conflict__free(void *_Memory)

{
  BOOL BVar1;
  int *piVar2;
  DWORD DVar3;
  int iVar4;
  
  if (_Memory != (void *)0x0) {
    BVar1 = HeapFree(hHeap_0041f410,0,_Memory);
    if (BVar1 == 0) {
      piVar2 = (int *)FUN_00407f40();
      DVar3 = GetLastError();
      iVar4 = FID_conflict____acrt_errno_from_os_error(DVar3);
      *piVar2 = iVar4;
    }
  }
  return;
}



void FUN_0040a0df(void)

{
  FUN_0040a1f6(1,"CompareStringEx",(int *)&DAT_004183a8,(int *)"CompareStringEx");
  return;
}



void FUN_0040a0f9(void)

{
  FUN_0040a1f6(0x14,"LCMapStringEx",(int *)&DAT_00418430,(int *)"LCMapStringEx");
  return;
}



void FUN_0040a113(void)

{
  FUN_0040a1f6(0x16,"LocaleNameToLCID",(int *)&DAT_00418448,(int *)"LocaleNameToLCID");
  return;
}



HMODULE __cdecl FUN_0040a12d(int *param_1,int *param_2)

{
  HMODULE *ppHVar1;
  LPCWSTR lpLibFileName;
  HMODULE pHVar2;
  HMODULE hLibModule;
  DWORD DVar3;
  int iVar4;
  
  do {
    if (param_1 == param_2) {
      return (HMODULE)0x0;
    }
    ppHVar1 = &hLibModule_0041efa8 + *param_1;
    hLibModule = *ppHVar1;
    if (hLibModule == (HMODULE)0x0) {
      lpLibFileName = (LPCWSTR)(&PTR_u_api_ms_win_core_datetime_l1_1_1_00417e88)[*param_1];
      hLibModule = LoadLibraryExW(lpLibFileName,(HANDLE)0x0,0x800);
      if (hLibModule == (HMODULE)0x0) {
        DVar3 = GetLastError();
        if (((DVar3 == 0x57) && (iVar4 = _wcsncmp(lpLibFileName,L"api-ms-",7), iVar4 != 0)) &&
           (iVar4 = _wcsncmp(lpLibFileName,L"ext-ms-",7), iVar4 != 0)) {
          hLibModule = LoadLibraryExW(lpLibFileName,(HANDLE)0x0,0);
        }
        else {
          hLibModule = (HMODULE)0x0;
        }
        if (hLibModule == (HMODULE)0x0) {
          *ppHVar1 = (HMODULE)0xffffffff;
          goto LAB_0040a1dd;
        }
      }
      pHVar2 = *ppHVar1;
      *ppHVar1 = hLibModule;
      if (pHVar2 != (HMODULE)0x0) {
        FreeLibrary(hLibModule);
      }
LAB_0040a1d9:
      if (hLibModule != (HMODULE)0x0) {
        return hLibModule;
      }
    }
    else {
      if (hLibModule != (HMODULE)0xffffffff) goto LAB_0040a1d9;
    }
LAB_0040a1dd:
    param_1 = param_1 + 1;
  } while( true );
}



FARPROC __cdecl FUN_0040a1f6(int param_1,LPCSTR param_2,int *param_3,int *param_4)

{
  uint *puVar1;
  FARPROC pFVar2;
  HMODULE hModule;
  uint uVar3;
  byte bVar4;
  
  puVar1 = &DAT_0041eff8 + param_1;
  bVar4 = (byte)DAT_0041e004 & 0x1f;
  pFVar2 = (FARPROC)((DAT_0041e004 ^ *puVar1) >> bVar4 | (DAT_0041e004 ^ *puVar1) << 0x20 - bVar4);
  if (pFVar2 == (FARPROC)0xffffffff) {
    pFVar2 = (FARPROC)0x0;
  }
  else {
    if (pFVar2 == (FARPROC)0x0) {
      hModule = FUN_0040a12d(param_3,param_4);
      if ((hModule == (HMODULE)0x0) ||
         (pFVar2 = GetProcAddress(hModule,param_2), pFVar2 == (FARPROC)0x0)) {
        bVar4 = 0x20 - ((byte)DAT_0041e004 & 0x1f) & 0x1f;
        *puVar1 = (0xffffffffU >> bVar4 | -1 << 0x20 - bVar4) ^ DAT_0041e004;
        pFVar2 = (FARPROC)0x0;
      }
      else {
        uVar3 = FUN_004022fb((uint)pFVar2);
        *puVar1 = uVar3;
      }
    }
  }
  return pFVar2;
}



int FUN_0040a279(undefined4 param_1)

{
  FARPROC pFVar1;
  int iVar2;
  undefined4 uVar3;
  
  pFVar1 = FUN_0040a1f6(0x1c,"AppPolicyGetProcessTerminationMethod",(int *)&DAT_00418464,
                        (int *)"AppPolicyGetProcessTerminationMethod");
  if (pFVar1 == (FARPROC)0x0) {
    iVar2 = -0x3ffffddb;
  }
  else {
    uVar3 = 0xfffffffa;
    _guard_check_icall();
    iVar2 = (*pFVar1)(uVar3,param_1);
  }
  return iVar2;
}



void FUN_0040a2b9(int param_1,DWORD param_2,PCNZWCH param_3,int param_4,PCNZWCH param_5,int param_6,
                 undefined4 param_7,undefined4 param_8,undefined4 param_9)

{
  code *pcVar1;
  LCID Locale;
  
  pcVar1 = (code *)FUN_0040a0df();
  if (pcVar1 == (code *)0x0) {
    Locale = FUN_0040a4bd(param_1,0);
    CompareStringW(Locale,param_2,param_3,param_4,param_5,param_6);
  }
  else {
    _guard_check_icall();
    (*pcVar1)(param_1,param_2,param_3,param_4,param_5,param_6,param_7,param_8,param_9);
  }
  return;
}



void FUN_0040a316(undefined4 param_1)

{
  FARPROC pFVar1;
  
  pFVar1 = FUN_0040a1f6(3,"FlsAlloc",(int *)&DAT_004183c0,(int *)"FlsAlloc");
  if (pFVar1 == (FARPROC)0x0) {
    TlsAlloc();
  }
  else {
    _guard_check_icall();
    (*pFVar1)(param_1);
  }
  return;
}



void FUN_0040a355(DWORD param_1)

{
  FARPROC pFVar1;
  
  pFVar1 = FUN_0040a1f6(4,"FlsFree",(int *)&DAT_004183d4,(int *)"FlsFree");
  if (pFVar1 != (FARPROC)0x0) {
    _guard_check_icall();
    (*pFVar1)(param_1);
    return;
  }
                    // WARNING: Could not recover jumptable at 0x0040a38e. Too many branches
                    // WARNING: Treating indirect jump as call
  TlsFree(param_1);
  return;
}



void FUN_0040a394(DWORD param_1)

{
  FARPROC pFVar1;
  
  pFVar1 = FUN_0040a1f6(5,"FlsGetValue",(int *)&DAT_004183e4,(int *)"FlsGetValue");
  if (pFVar1 != (FARPROC)0x0) {
    _guard_check_icall();
    (*pFVar1)(param_1);
    return;
  }
                    // WARNING: Could not recover jumptable at 0x0040a3cd. Too many branches
                    // WARNING: Treating indirect jump as call
  TlsGetValue(param_1);
  return;
}



void FUN_0040a3d3(DWORD param_1,LPVOID param_2)

{
  FARPROC pFVar1;
  
  pFVar1 = FUN_0040a1f6(6,"FlsSetValue",(int *)&DAT_004183f8,(int *)"FlsSetValue");
  if (pFVar1 != (FARPROC)0x0) {
    _guard_check_icall();
    (*pFVar1)(param_1,param_2);
    return;
  }
                    // WARNING: Could not recover jumptable at 0x0040a40f. Too many branches
                    // WARNING: Treating indirect jump as call
  TlsSetValue(param_1,param_2);
  return;
}



void FUN_0040a415(LPCRITICAL_SECTION param_1,DWORD param_2,undefined4 param_3)

{
  FARPROC pFVar1;
  
  pFVar1 = FUN_0040a1f6(0x12,"InitializeCriticalSectionEx",(int *)&DAT_0041840c,
                        (int *)"InitializeCriticalSectionEx");
  if (pFVar1 == (FARPROC)0x0) {
    InitializeCriticalSectionAndSpinCount(param_1,param_2);
  }
  else {
    _guard_check_icall();
    (*pFVar1)(param_1,param_2,param_3);
  }
  return;
}



void FUN_0040a460(int param_1,DWORD param_2,LPCWSTR param_3,int param_4,LPWSTR param_5,int param_6,
                 undefined4 param_7,undefined4 param_8,undefined4 param_9)

{
  code *pcVar1;
  LCID Locale;
  
  pcVar1 = (code *)FUN_0040a0f9();
  if (pcVar1 == (code *)0x0) {
    Locale = FUN_0040a4bd(param_1,0);
    LCMapStringW(Locale,param_2,param_3,param_4,param_5,param_6);
  }
  else {
    _guard_check_icall();
    (*pcVar1)(param_1,param_2,param_3,param_4,param_5,param_6,param_7,param_8,param_9);
  }
  return;
}



void FUN_0040a4bd(int param_1,undefined4 param_2)

{
  code *pcVar1;
  
  pcVar1 = (code *)FUN_0040a113();
  if (pcVar1 == (code *)0x0) {
    FID_conflict__AtlDownlevelLocaleNameToLCID(param_1);
  }
  else {
    _guard_check_icall();
    (*pcVar1)(param_1,param_2);
  }
  return;
}



// Library Function - Single Match
// Name: ___acrt_uninitialize_winapi_thunks
// Library: Visual Studio 2015 Release

undefined __cdecl ___acrt_uninitialize_winapi_thunks(char param_1)

{
  HMODULE *ppHVar1;
  
  if (param_1 == 0) {
    ppHVar1 = &hLibModule_0041efa8;
    do {
      if (*ppHVar1 != (HMODULE)0x0) {
        if (*ppHVar1 != (HMODULE)0xffffffff) {
          FreeLibrary(*ppHVar1);
        }
        *ppHVar1 = (HMODULE)0x0;
      }
      ppHVar1 = ppHVar1 + 1;
    } while (ppHVar1 != (HMODULE *)&DAT_0041eff8);
  }
  return 1;
}



// WARNING: Function: __SEH_prolog4 replaced with injection: SEH_prolog4
// WARNING: Function: __SEH_epilog4 replaced with injection: EH_epilog3

int FUN_0040a54f(void)

{
  int iVar1;
  int iVar2;
  int local_20;
  
  local_20 = 0;
  ___acrt_lock(8);
  iVar2 = 3;
  while (iVar2 != DAT_0041ed24) {
    iVar1 = *(int *)(DAT_0041ed28 + iVar2 * 4);
    if (iVar1 != 0) {
      if ((*(uint *)(iVar1 + 0xc) >> 0xd & 1) != 0) {
        iVar1 = FUN_0040fb8f(*(FILE **)(DAT_0041ed28 + iVar2 * 4));
        if (iVar1 != -1) {
          local_20 = local_20 + 1;
        }
      }
      DeleteCriticalSection((LPCRITICAL_SECTION)(*(int *)(DAT_0041ed28 + iVar2 * 4) + 0x20));
      FID_conflict__free(*(void **)(DAT_0041ed28 + iVar2 * 4));
      *(undefined4 *)(DAT_0041ed28 + iVar2 * 4) = 0;
    }
    iVar2 = iVar2 + 1;
  }
  FUN_0040a5e7();
  return local_20;
}



void FUN_0040a5e7(void)

{
  ___acrt_unlock(8);
  return;
}



// WARNING: Function: __SEH_prolog4 replaced with injection: SEH_prolog4
// WARNING: Function: __SEH_epilog4 replaced with injection: EH_epilog3

void FUN_0040a5f0(FILE **param_1,FILE **param_2)

{
  uint uVar1;
  int iVar2;
  
  __lock_file(*param_1);
  uVar1 = FUN_0040a763((int *)(*param_2)->_ptr,(int *)param_2[1]);
  if (((char)uVar1 != 0) &&
     ((*(char *)&param_2[2]->_ptr != 0 || ((*(uint *)((*param_2)->_ptr + 0xc) >> 1 & 1) != 0)))) {
    iVar2 = FUN_0040a820((FILE *)(*param_2)->_ptr);
    if (iVar2 == -1) {
      param_2[3]->_ptr = (char *)0xffffffff;
    }
    else {
      param_2[1]->_ptr = param_2[1]->_ptr + 1;
    }
  }
  FUN_0040a666();
  return;
}



void FUN_0040a666(void)

{
  int unaff_EBP;
  
  FUN_004056fc(**(int **)(unaff_EBP + 0x10));
  return;
}



// WARNING: Function: __SEH_prolog4 replaced with injection: SEH_prolog4
// WARNING: Function: __SEH_epilog4 replaced with injection: EH_epilog3

void FUN_0040a672(int *param_1,int **param_2)

{
  FILE **ppFVar1;
  uint uVar2;
  FILE **ppFVar3;
  FILE *local_40;
  int *local_3c;
  int *local_38;
  int *local_34;
  FILE **local_30;
  FILE *local_2c;
  FILE *local_28;
  FILE local_24;
  
  local_24._tmpfname = &DAT_0041d1e0;
  local_24._bufsiz = 0x40a67e;
  ___acrt_lock(*param_1);
  local_24._tmpfname = (char *)0x0;
  ppFVar1 = DAT_0041ed28 + DAT_0041ed24;
  ppFVar3 = DAT_0041ed28;
  while (ppFVar3 != ppFVar1) {
    local_24._ptr = *ppFVar3;
    local_30 = ppFVar3;
    uVar2 = FUN_0040a763((int *)local_24._ptr,*param_2);
    if ((char)uVar2 != 0) {
      local_34 = param_2[2];
      local_38 = param_2[1];
      local_3c = *param_2;
      local_40 = &local_24;
      local_28 = local_24._ptr;
      local_2c = local_24._ptr;
      FUN_0040a5f0(&local_2c,&local_40);
    }
    ppFVar3 = ppFVar3 + 1;
  }
  local_24._tmpfname = (char *)0xfffffffe;
  local_30 = ppFVar3;
  FUN_0040a708();
  return;
}



void FUN_0040a708(void)

{
  int unaff_EBP;
  
  ___acrt_unlock(**(int **)(unaff_EBP + 0x10));
  return;
}



int __cdecl FUN_0040a714(char param_1)

{
  int *local_24;
  char *local_20;
  int *local_1c;
  int local_18;
  undefined4 local_14;
  int local_10;
  int local_c [2];
  
  local_c[0] = 0;
  local_24 = local_c;
  local_10 = 0;
  local_20 = &param_1;
  local_1c = &local_10;
  local_14 = 8;
  local_18 = 8;
  FUN_0040a672(&local_18,&local_24);
  if (param_1 == 0) {
    local_c[0] = local_10;
  }
  return local_c[0];
}



uint __cdecl FUN_0040a763(int *param_1,int *param_2)

{
  uint *puVar1;
  int iVar2;
  
  if (param_1 != (int *)0x0) {
    puVar1 = (uint *)(param_1 + 3);
    param_1 = (int *)(*puVar1 >> 0xd);
    if (((uint)param_1 & 1) != 0) {
      iVar2 = FUN_0040a796(*puVar1);
      if ((char)iVar2 != 0) {
        return CONCAT31((int3)((uint)iVar2 >> 8),1);
      }
      *param_2 = *param_2 + 1;
      param_1 = param_2;
    }
  }
  return (uint)param_1 & 0xffffff00;
}



int __cdecl FUN_0040a796(uint param_1)

{
  uint3 uVar1;
  
  uVar1 = (uint3)((param_1 & 0xffffff03) >> 8);
  if ((((char)(param_1 & 0xffffff03) != 2) || ((param_1 & 0xc0) == 0)) && ((param_1 & 0x800) == 0))
  {
    return (uint)uVar1 << 8;
  }
  return CONCAT31(uVar1,1);
}



undefined4 __cdecl FUN_0040a7bb(FILE *param_1)

{
  uint *puVar1;
  uint uVar2;
  char *pcVar3;
  undefined8 *puVar4;
  char *pcVar5;
  
  puVar1 = (uint *)&param_1->_flag;
  if ((((byte)*puVar1 & 3) == 2) && ((*puVar1 & 0xc0) != 0)) {
    puVar4 = (undefined8 *)param_1->_cnt;
    pcVar3 = param_1->_ptr + -(int)puVar4;
    *(undefined8 **)&param_1->_ptr = puVar4;
    param_1->_base = (char *)0x0;
    if (0 < (int)pcVar3) {
      pcVar5 = pcVar3;
      uVar2 = __fileno(param_1);
      pcVar5 = (char *)FUN_004104a5(uVar2,puVar4,(uint)pcVar5);
      if (pcVar3 != pcVar5) {
        LOCK();
        *puVar1 = *puVar1 | 0x10;
        return 0xffffffff;
      }
      if ((*puVar1 >> 2 & 1) != 0) {
        LOCK();
        *puVar1 = *puVar1 & 0xfffffffd;
      }
    }
  }
  return 0;
}



int __cdecl FUN_0040a820(FILE *param_1)

{
  int iVar1;
  uint uVar2;
  undefined4 uVar3;
  
  if (param_1 == (FILE *)0x0) {
    iVar1 = FUN_0040a714(0);
    return iVar1;
  }
  iVar1 = FUN_0040a7bb(param_1);
  if (iVar1 == 0) {
    if (((uint)param_1->_flag >> 0xb & 1) != 0) {
      uVar2 = __fileno(param_1);
      iVar1 = FUN_0040fca1(uVar2);
      if (iVar1 != 0) goto LAB_0040a862;
    }
    uVar3 = 0;
  }
  else {
LAB_0040a862:
    uVar3 = 0xffffffff;
  }
  return uVar3;
}



void FUN_0040a868(void)

{
  FUN_0040a714(1);
  return;
}



// Library Function - Single Match
// Name: ___acrt_stdio_free_buffer_nolock
// Library: Visual Studio 2015 Release

void __cdecl ___acrt_stdio_free_buffer_nolock(undefined4 *param_1)

{
  uint *puVar1;
  
  puVar1 = param_1 + 3;
  if (((*puVar1 >> 0xd & 1) != 0) && ((*puVar1 >> 6 & 1) != 0)) {
    FID_conflict__free((void *)param_1[1]);
    LOCK();
    *puVar1 = *puVar1 & 0xfffffebf;
    param_1[1] = 0;
    *param_1 = 0;
    param_1[2] = 0;
  }
  return;
}



void FUN_0040a8b1(void)

{
  byte bVar1;
  undefined *hFile;
  DWORD DVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  _STARTUPINFOW local_4c;
  undefined4 *local_8;
  
  GetStartupInfoW((LPSTARTUPINFOW)&local_4c);
  if ((local_4c.cbReserved2 != 0) && (local_4c.lpReserved2 != (uint *)0x0)) {
    uVar4 = *local_4c.lpReserved2;
    local_8 = (undefined4 *)((int)local_4c.lpReserved2 + uVar4 + 4);
    if (0x1fff < (int)uVar4) {
      uVar4 = 0x2000;
    }
    FUN_0040e70d(uVar4);
    if ((int)DAT_0041f280 < (int)uVar4) {
      uVar4 = DAT_0041f280;
    }
    uVar5 = 0;
    if (uVar4 != 0) {
      do {
        hFile = (undefined *)*local_8;
        if ((((hFile != (undefined *)0xffffffff) && (hFile != &DAT_fffffffe)) &&
            (bVar1 = *(byte *)(uVar5 + 4 + (int)local_4c.lpReserved2), (bVar1 & 1) != 0)) &&
           (((bVar1 & 8) != 0 || (DVar2 = GetFileType(hFile), DVar2 != 0)))) {
          iVar3 = (uVar5 & 0x3f) * 0x38 + (&DAT_0041f080)[(int)uVar5 >> 6];
          *(undefined4 *)(iVar3 + 0x18) = *local_8;
          *(undefined *)(iVar3 + 0x28) = *(undefined *)(uVar5 + 4 + (int)local_4c.lpReserved2);
        }
        uVar5 = uVar5 + 1;
        local_8 = local_8 + 1;
      } while (uVar5 != uVar4);
    }
  }
  return;
}



void FUN_0040a969(void)

{
  HANDLE hFile;
  uint uVar1;
  int iVar2;
  uint uVar3;
  DWORD nStdHandle;
  
  uVar3 = 0;
  do {
    iVar2 = (uVar3 & 0x3f) * 0x38 + (&DAT_0041f080)[(int)uVar3 >> 6];
    if ((*(int *)(iVar2 + 0x18) == -1) || (*(int *)(iVar2 + 0x18) == -2)) {
      *(undefined *)(iVar2 + 0x28) = 0x81;
      if (uVar3 == 0) {
        nStdHandle = 0xfffffff6;
      }
      else {
        if (uVar3 == 1) {
          nStdHandle = 0xfffffff5;
        }
        else {
          nStdHandle = 0xfffffff4;
        }
      }
      hFile = GetStdHandle(nStdHandle);
      if ((hFile == (HANDLE)0xffffffff) || (hFile == (HANDLE)0x0)) {
        uVar1 = 0;
      }
      else {
        uVar1 = GetFileType(hFile);
      }
      if (uVar1 == 0) {
        *(byte *)(iVar2 + 0x28) = *(byte *)(iVar2 + 0x28) | 0x40;
        *(undefined4 *)(iVar2 + 0x18) = 0xfffffffe;
        if (DAT_0041ed28 != 0) {
          *(undefined4 *)(*(int *)(DAT_0041ed28 + uVar3 * 4) + 0x10) = 0xfffffffe;
        }
      }
      else {
        *(HANDLE *)(iVar2 + 0x18) = hFile;
        if ((uVar1 & 0xff) == 2) {
          *(byte *)(iVar2 + 0x28) = *(byte *)(iVar2 + 0x28) | 0x40;
        }
        else {
          if ((uVar1 & 0xff) == 3) {
            *(byte *)(iVar2 + 0x28) = *(byte *)(iVar2 + 0x28) | 8;
          }
        }
      }
    }
    else {
      *(byte *)(iVar2 + 0x28) = *(byte *)(iVar2 + 0x28) | 0x80;
    }
    uVar3 = uVar3 + 1;
  } while (uVar3 != 3);
  return;
}



void FUN_0040aa6a(void)

{
  ___acrt_unlock(7);
  return;
}



// Library Function - Single Match
// Name: __isdigit_l
// Library: Visual Studio 2015 Release

int __cdecl __isdigit_l(int _C,_locale_t _Locale)

{
  int iVar1;
  int local_14;
  ushort *local_10 [2];
  char local_8;
  
  FUN_00405ed6(&local_14,(undefined4 *)_Locale);
  iVar1 = __ischartype_l(_C,4,local_10);
  if (local_8 != 0) {
    *(uint *)(local_14 + 0x350) = *(uint *)(local_14 + 0x350) & 0xfffffffd;
  }
  return iVar1;
}



void __cdecl FUN_0040aad7(int param_1)

{
  if (DAT_0041ef80 != 0) {
    __isdigit_l(param_1,(_locale_t)0x0);
    return;
  }
  FUN_00407be2((int)PTR_DAT_0041e140,param_1,4);
  return;
}



// WARNING: Could not reconcile some variable overlaps

uint __cdecl FUN_0040ab08(uint param_1,uint param_2,undefined4 *param_3)

{
  byte bVar1;
  uint uVar2;
  int iVar3;
  undefined4 *puVar4;
  int local_20;
  ushort *local_1c [2];
  char local_14;
  uint local_10;
  undefined2 local_c;
  undefined local_a;
  ushort local_8;
  undefined local_6;
  
  FUN_00405ed6(&local_20,param_3);
  if (param_1 < 0x100) {
    uVar2 = FUN_0040ac4f(param_1,local_1c);
    if ((char)uVar2 != 0) {
      bVar1 = *(byte *)(*(int *)(local_1c[0] + 0x4a) + param_1);
      if (local_14 == 0) {
        return (uint)bVar1;
      }
      *(uint *)(local_20 + 0x350) = *(uint *)(local_20 + 0x350) & 0xfffffffd;
      return (uint)bVar1;
    }
    if (local_14 == 0) {
      return param_1;
    }
    *(uint *)(local_20 + 0x350) = *(uint *)(local_20 + 0x350) & 0xfffffffd;
    return param_1;
  }
  local_8 = 0;
  local_6 = 0;
  if (1 < *(int *)(local_1c[0] + 2)) {
    local_10 = (int)param_1 >> 8;
    iVar3 = FUN_00410771(local_10 & 0xff,local_1c);
    if (iVar3 != 0) {
      local_8 = CONCAT11((byte)param_1,(char)local_10);
      local_6 = 0;
      iVar3 = 2;
      goto LAB_0040abca;
    }
  }
  puVar4 = (undefined4 *)FUN_00407f40();
  *puVar4 = 0x2a;
  iVar3 = 1;
  local_8 = (ushort)(byte)param_1;
LAB_0040abca:
  local_c = L'\0';
  local_a = 0;
  iVar3 = ___acrt_LCMapStringA
                    (local_1c,*(int *)(local_1c[0] + 0x54),param_2,(char *)&local_8,iVar3,&local_c,3
                     ,*(uint *)(local_1c[0] + 4),1);
  if (iVar3 == 0) {
    if (local_14 != 0) {
      *(uint *)(local_20 + 0x350) = *(uint *)(local_20 + 0x350) & 0xfffffffd;
    }
  }
  else {
    if (iVar3 == 1) {
      param_1 = (uint)(byte)local_c;
      if (local_14 != 0) {
        *(uint *)(local_20 + 0x350) = *(uint *)(local_20 + 0x350) & 0xfffffffd;
      }
    }
    else {
      param_1 = (uint)CONCAT11((byte)local_c,local_c._1_1_);
      if (local_14 != 0) {
        *(uint *)(local_20 + 0x350) = *(uint *)(local_20 + 0x350) & 0xfffffffd;
      }
    }
  }
  return param_1;
}



uint __cdecl FUN_0040ac4f(int param_1,ushort **param_2)

{
  uint uVar1;
  
  uVar1 = __ischartype_l(param_1,1,param_2);
  return uVar1 & 0xffffff00 | (uint)(uVar1 != 0);
}



void __cdecl FUN_0040ac6b(uint param_1,undefined4 *param_2)

{
  FUN_0040ab08(param_1,0x100,param_2);
  return;
}



uint __cdecl FUN_0040ac85(uint param_1)

{
  if (DAT_0041ef80 == 0) {
    if (param_1 - 0x41 < 0x1a) {
      param_1 = param_1 + 0x20;
    }
  }
  else {
    param_1 = FUN_0040ac6b(param_1,(undefined4 *)0x0);
  }
  return param_1;
}



// Library Function - Single Match
// Name: __malloc_base
// Library: Visual Studio 2015 Release

LPVOID __cdecl __malloc_base(SIZE_T param_1)

{
  int iVar1;
  LPVOID pvVar2;
  undefined4 *puVar3;
  
  if (param_1 < 0xffffffe1) {
    if (param_1 == 0) {
      param_1 = 1;
    }
    do {
      pvVar2 = HeapAlloc(hHeap_0041f410,0,param_1);
      if (pvVar2 != (LPVOID)0x0) {
        return pvVar2;
      }
      iVar1 = FUN_00409982();
    } while ((iVar1 != 0) && (iVar1 = FUN_0040f19a(param_1), iVar1 != 0));
  }
  puVar3 = (undefined4 *)FUN_00407f40();
  *puVar3 = 0xc;
  return (LPVOID)0x0;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

uint __cdecl FUN_0040ad03(ushort *param_1,byte *param_2,byte *param_3,undefined4 *param_4)

{
  byte *pbVar1;
  uint uVar2;
  int iVar3;
  undefined4 *puVar4;
  int local_14;
  int local_10 [2];
  char local_8;
  
  if ((param_2 == (byte *)0x0) || (param_3 == (byte *)0x0)) {
    _DAT_0041f284 = 0;
    _DAT_0041f288 = 0;
    return 0;
  }
  if (*param_2 == 0) {
    if (param_1 == (ushort *)0x0) {
      return 0;
    }
    *param_1 = 0;
    return 0;
  }
  FUN_00405ed6(&local_14,param_4);
  if (*(int *)(local_10[0] + 8) == 0xfde9) {
    uVar2 = FUN_00410a10(param_1,param_2,param_3,(byte **)&DAT_0041f284);
    if (-1 < (int)uVar2) goto LAB_0040ae1c;
  }
  else {
    if (*(int *)(local_10[0] + 0xa8) == 0) {
      if (param_1 != (ushort *)0x0) {
        *param_1 = (ushort)*param_2;
      }
      uVar2 = 1;
      goto LAB_0040ae1c;
    }
    iVar3 = FUN_00410771((uint)*param_2,local_10);
    if (iVar3 != 0) {
      if (*(int *)(local_10[0] + 4) < 2) {
LAB_0040add9:
        pbVar1 = *(byte **)(local_10[0] + 4);
LAB_0040addc:
        if ((param_3 < pbVar1) || (param_2[1] == 0)) goto LAB_0040ae0e;
      }
      else {
        pbVar1 = *(byte **)(local_10[0] + 4);
        if ((int)param_3 < (int)*(byte **)(local_10[0] + 4)) goto LAB_0040addc;
        iVar3 = FUN_0040e0f3(*(uint *)(local_10[0] + 8),9,(LPCSTR)param_2,*(int *)(local_10[0] + 4),
                             (LPWSTR)param_1,(uint)(param_1 != (ushort *)0x0));
        if (iVar3 == 0) goto LAB_0040add9;
      }
      uVar2 = *(uint *)(local_10[0] + 4);
      goto LAB_0040ae1c;
    }
    uVar2 = 1;
    iVar3 = FUN_0040e0f3(*(uint *)(local_10[0] + 8),9,(LPCSTR)param_2,1,(LPWSTR)param_1,
                         (uint)(param_1 != (ushort *)0x0));
    if (iVar3 != 0) goto LAB_0040ae1c;
LAB_0040ae0e:
    puVar4 = (undefined4 *)FUN_00407f40();
    *puVar4 = 0x2a;
  }
  uVar2 = 0xffffffff;
LAB_0040ae1c:
  if (local_8 != 0) {
    *(uint *)(local_14 + 0x350) = *(uint *)(local_14 + 0x350) & 0xfffffffd;
    return uVar2;
  }
  return uVar2;
}



void __cdecl FUN_0040ae47(ushort *param_1,byte *param_2,byte *param_3)

{
  FUN_0040ad03(param_1,param_2,param_3,(undefined4 *)0x0);
  return;
}



undefined4 __cdecl
FUN_0040ae61(int *param_1,int *param_2,uint param_3,ushort param_4,undefined4 *param_5)

{
  undefined4 *puVar1;
  int iVar2;
  DWORD DVar3;
  undefined4 uVar4;
  int local_1c;
  int local_18;
  char local_10;
  undefined4 local_c;
  int local_8;
  
  if ((param_2 == (int *)0x0) && (param_3 != 0)) {
    if (param_1 != (int *)0x0) {
      *param_1 = 0;
    }
    return 0;
  }
  if (param_1 != (int *)0x0) {
    *param_1 = -1;
  }
  if (0x7fffffff < param_3) {
    puVar1 = (undefined4 *)FUN_00407f40();
    *puVar1 = 0x16;
    FUN_00407e83();
    return 0x16;
  }
  FUN_00405ed6(&local_1c,param_5);
  uVar4 = 0;
  if (*(uint *)(local_18 + 8) == 0xfde9) {
    local_c = 0;
    local_8 = 0;
    iVar2 = FUN_00410b4f((byte *)param_2,(uint)param_4,&local_c);
    if (param_1 != (int *)0x0) {
      *param_1 = iVar2;
    }
    if (4 < iVar2) {
      puVar1 = (undefined4 *)FUN_00407f40();
      uVar4 = *puVar1;
    }
    goto LAB_0040af2c;
  }
  if (*(int *)(local_18 + 0xa8) == 0) {
    if (param_4 < 0x100) {
      if (param_2 != (int *)0x0) {
        if (param_3 == 0) goto LAB_0040afaa;
        *(char *)param_2 = (char)param_4;
      }
      if (param_1 != (int *)0x0) {
        *param_1 = 1;
      }
      goto LAB_0040af2c;
    }
    if ((param_2 != (int *)0x0) && (param_3 != 0)) {
      FUN_00402e50(param_2,0,param_3);
    }
  }
  else {
    local_8 = 0;
    iVar2 = FUN_0040c3b9(*(uint *)(local_18 + 8),0,(LPCWSTR)&param_4,1,(LPSTR)param_2,param_3,0,
                         (uint)&local_8);
    if (iVar2 == 0) {
      DVar3 = GetLastError();
      if (DVar3 == 0x7a) {
        if ((param_2 != (int *)0x0) && (param_3 != 0)) {
          FUN_00402e50(param_2,0,param_3);
        }
LAB_0040afaa:
        puVar1 = (undefined4 *)FUN_00407f40();
        uVar4 = 0x22;
        *puVar1 = 0x22;
        FUN_00407e83();
        goto LAB_0040af2c;
      }
    }
    else {
      if (local_8 == 0) {
        if (param_1 != (int *)0x0) {
          *param_1 = iVar2;
        }
        goto LAB_0040af2c;
      }
    }
  }
  puVar1 = (undefined4 *)FUN_00407f40();
  uVar4 = 0x2a;
  *puVar1 = 0x2a;
LAB_0040af2c:
  if (local_10 != 0) {
    *(uint *)(local_1c + 0x350) = *(uint *)(local_1c + 0x350) & 0xfffffffd;
  }
  return uVar4;
}



void __cdecl FUN_0040afbe(int *param_1,int *param_2,uint param_3,ushort param_4)

{
  FUN_0040ae61(param_1,param_2,param_3,param_4,(undefined4 *)0x0);
  return;
}



char * FUN_0040afdb(char *param_1,char *param_2)

{
  undefined auVar1 [16];
  char *pcVar2;
  int iVar3;
  char *pcVar4;
  char *pcVar5;
  undefined auVar6 [32];
  undefined in_YMM1 [32];
  
  if (DAT_0041ec74 < 5) {
    if (DAT_0041ec74 < 1) {
      pcVar4 = param_1;
      if (param_1 != param_2 + (int)param_1) {
        do {
          if (*pcVar4 == 0) break;
          pcVar4 = pcVar4 + 1;
        } while (pcVar4 != param_2 + (int)param_1);
      }
      pcVar4 = pcVar4 + -(int)param_1;
    }
    else {
      pcVar5 = (char *)(-(uint)(((uint)param_1 & 0xf) != 0) & 0x10 - ((uint)param_1 & 0xf));
      if (param_2 < pcVar5) {
        pcVar5 = param_2;
      }
      pcVar2 = param_1;
      if (param_1 != pcVar5 + (int)param_1) {
        do {
          if (*pcVar2 == 0) break;
          pcVar2 = pcVar2 + 1;
        } while (pcVar2 != pcVar5 + (int)param_1);
      }
      pcVar4 = pcVar2 + -(int)param_1;
      if (pcVar4 == pcVar5) {
        pcVar5 = pcVar2 + ((uint)(param_2 + -(int)pcVar5) & 0xfffffff0);
        pcVar4 = pcVar2;
        while (pcVar4 != pcVar5) {
                    // WARNING: Load size is inaccurate
          auVar1 = *(undefined *)pcVar4;
          pcVar2 = (char *)pmovmskb(pcVar2,CONCAT115(-(SUB161(auVar1 >> 0x78,0) == 0),
                                                     CONCAT114(-(SUB161(auVar1 >> 0x70,0) == 0),
                                                               CONCAT113(-(SUB161(auVar1 >> 0x68,0)
                                                                          == 0),CONCAT112(-(SUB161(
                                                  auVar1 >> 0x60,0) == 0),
                                                  CONCAT111(-(SUB161(auVar1 >> 0x58,0) == 0),
                                                            CONCAT110(-(SUB161(auVar1 >> 0x50,0) ==
                                                                       0),CONCAT19(-(SUB161(auVar1 
                                                  >> 0x48,0) == 0),
                                                  CONCAT18(-(SUB161(auVar1 >> 0x40,0) == 0),
                                                           CONCAT17(-(SUB161(auVar1 >> 0x38,0) == 0)
                                                                    ,CONCAT16(-(SUB161(auVar1 >> 
                                                  0x30,0) == 0),
                                                  CONCAT15(-(SUB161(auVar1 >> 0x28,0) == 0),
                                                           CONCAT14(-(SUB161(auVar1 >> 0x20,0) == 0)
                                                                    ,CONCAT13(-(SUB161(auVar1 >> 
                                                  0x18,0) == 0),
                                                  CONCAT12(-(SUB161(auVar1 >> 0x10,0) == 0),
                                                           CONCAT11(-(SUB161(auVar1 >> 8,0) == 0),
                                                                    -(SUB161(auVar1,0) == 0)))))))))
                                                  ))))))));
          if (pcVar2 != (char *)0x0) break;
          pcVar4 = pcVar4 + 0x10;
        }
        while ((pcVar4 != param_2 + (int)param_1 && (*pcVar4 != 0))) {
          pcVar4 = pcVar4 + 1;
        }
        pcVar4 = pcVar4 + -(int)param_1;
      }
    }
  }
  else {
    pcVar5 = (char *)(-(uint)(((uint)param_1 & 0x1f) != 0) & 0x20 - ((uint)param_1 & 0x1f));
    if (param_2 < pcVar5) {
      pcVar5 = param_2;
    }
    pcVar2 = param_1;
    if (param_1 != pcVar5 + (int)param_1) {
      do {
        if (*pcVar2 == 0) break;
        pcVar2 = pcVar2 + 1;
      } while (pcVar2 != pcVar5 + (int)param_1);
    }
    pcVar4 = pcVar2 + -(int)param_1;
    if (pcVar4 == pcVar5) {
      pcVar4 = pcVar2 + ((uint)(param_2 + -(int)pcVar5) & 0xffffffe0);
      auVar1 = vpxor_avx(SUB3216(in_YMM1,0),SUB3216(in_YMM1,0));
      while (pcVar2 != pcVar4) {
                    // WARNING: Load size is inaccurate
        auVar6 = vpcmpeqb_avx2(ZEXT1632(auVar1),*(undefined *)pcVar2);
        iVar3 = vpmovmskb_avx2(auVar6);
        if (iVar3 != 0) break;
        pcVar2 = pcVar2 + 0x20;
      }
      while ((pcVar2 != param_2 + (int)param_1 && (*pcVar2 != 0))) {
        pcVar2 = pcVar2 + 1;
      }
      pcVar4 = pcVar2 + -(int)param_1;
      vzeroupper_avx();
    }
  }
  return pcVar4;
}



uint FUN_0040b0fd(short *param_1,uint param_2)

{
  short *psVar1;
  undefined auVar2 [16];
  int iVar3;
  short *psVar4;
  uint uVar5;
  uint uVar6;
  undefined auVar7 [32];
  undefined in_YMM1 [32];
  
  if (DAT_0041ec74 < 5) {
    if (DAT_0041ec74 < 1) {
      psVar4 = param_1;
      if (param_1 != param_1 + param_2) {
        do {
          if (*psVar4 == 0) break;
          psVar4 = psVar4 + 1;
        } while (psVar4 != param_1 + param_2);
      }
      psVar4 = (short *)((int)psVar4 - (int)param_1);
      goto LAB_0040b2a5;
    }
    if (((uint)param_1 & 1) == 0) {
      uVar6 = (-(uint)(((uint)param_1 & 0xf) != 0) & 0x10 - ((uint)param_1 & 0xf)) >> 1;
      if (param_2 < uVar6) {
        uVar6 = param_2;
      }
      psVar4 = param_1;
      if (param_1 != param_1 + uVar6) {
        do {
          if (*psVar4 == 0) break;
          psVar4 = psVar4 + 1;
        } while (psVar4 != param_1 + uVar6);
      }
      uVar5 = (int)((int)psVar4 - (int)param_1) >> 1;
      if (uVar5 != uVar6) {
        return uVar5;
      }
      psVar4 = param_1 + uVar5;
      iVar3 = (param_2 - uVar6 & 0xfffffff0) + uVar5;
      psVar1 = param_1 + iVar3;
      while (psVar4 != psVar1) {
                    // WARNING: Load size is inaccurate
        auVar2 = *(undefined *)psVar4;
        iVar3 = pmovmskb(iVar3,CONCAT214(-(ushort)(SUB162(auVar2 >> 0x70,0) == 0),
                                         CONCAT212(-(ushort)(SUB162(auVar2 >> 0x60,0) == 0),
                                                   CONCAT210(-(ushort)(SUB162(auVar2 >> 0x50,0) == 0
                                                                      ),
                                                             CONCAT28(-(ushort)(SUB162(auVar2 >> 
                                                  0x40,0) == 0),
                                                  CONCAT26(-(ushort)(SUB162(auVar2 >> 0x30,0) == 0),
                                                           CONCAT24(-(ushort)(SUB162(auVar2 >> 0x20,
                                                                                     0) == 0),
                                                                    CONCAT22(-(ushort)(SUB162(auVar2
                                                                                              >> 
                                                  0x10,0) == 0),-(ushort)(SUB162(auVar2,0) == 0)))))
                                                  ))));
        if (iVar3 != 0) break;
        psVar4 = psVar4 + 8;
      }
      while ((psVar4 != param_1 + param_2 && (*psVar4 != 0))) {
        psVar4 = psVar4 + 1;
      }
    }
    else {
      psVar4 = param_1;
      if (param_1 != param_1 + param_2) {
        do {
          if (*psVar4 == 0) break;
          psVar4 = psVar4 + 1;
        } while (psVar4 != param_1 + param_2);
      }
    }
  }
  else {
    if (((uint)param_1 & 1) == 0) {
      uVar6 = (-(uint)(((uint)param_1 & 0x1f) != 0) & 0x20 - ((uint)param_1 & 0x1f)) >> 1;
      if (param_2 < uVar6) {
        uVar6 = param_2;
      }
      psVar4 = param_1;
      if (param_1 != param_1 + uVar6) {
        do {
          if (*psVar4 == 0) break;
          psVar4 = psVar4 + 1;
        } while (psVar4 != param_1 + uVar6);
      }
      uVar5 = (int)((int)psVar4 - (int)param_1) >> 1;
      if (uVar5 != uVar6) {
        return uVar5;
      }
      psVar4 = param_1 + uVar5;
      auVar2 = vpxor_avx(SUB3216(in_YMM1,0),SUB3216(in_YMM1,0));
      while (psVar4 != param_1 + (param_2 - uVar6 & 0xffffffe0) + uVar5) {
                    // WARNING: Load size is inaccurate
        auVar7 = vpcmpeqw_avx2(ZEXT1632(auVar2),*(undefined *)psVar4);
        iVar3 = vpmovmskb_avx2(auVar7);
        if (iVar3 != 0) break;
        psVar4 = psVar4 + 0x10;
      }
      while ((psVar4 != param_1 + param_2 && (*psVar4 != 0))) {
        psVar4 = psVar4 + 1;
      }
      vzeroupper_avx();
      return (int)((int)psVar4 - (int)param_1) >> 1;
    }
    psVar4 = param_1;
    if (param_1 != param_1 + param_2) {
      do {
        if (*psVar4 == 0) break;
        psVar4 = psVar4 + 1;
      } while (psVar4 != param_1 + param_2);
    }
  }
  psVar4 = (short *)((int)psVar4 - (int)param_1);
LAB_0040b2a5:
  return (int)psVar4 >> 1;
}



// WARNING: Function: __SEH_prolog4 replaced with injection: SEH_prolog4
// WARNING: Function: __SEH_epilog4 replaced with injection: EH_epilog3
// Library Function - Single Match
// Name: 
??$?RV<lambda_9df27f884b057bc3edfc946cb5b7cf47>@@AAV<lambda_e69574bed617af4e071282c136b37893>@@V<lambda_cc0d902bcbbeb830f749456577db4721>@@@?$__crt_seh_guarded_call@X@@QAEX$$QAV<lambda_9df27f884b057bc3edfc946cb5b7cf47>@@AAV<lambda_e69574bed617af4e071282c136b37893>@@$$QAV<lambda_cc0d902bcbbeb830f749456577db4721>@@@Z
// Library: Visual Studio 2015 Release
// public: void __thiscall __crt_seh_guarded_call<void>::operator()<class
// <lambda_9df27f884b057bc3edfc946cb5b7cf47>,class <lambda_e69574bed617af4e071282c136b37893>&,class
// <lambda_cc0d902bcbbeb830f749456577db4721>>(class
// <lambda_9df27f884b057bc3edfc946cb5b7cf47>&&,class
// <lambda_e69574bed617af4e071282c136b37893>&,class <lambda_cc0d902bcbbeb830f749456577db4721>&&)

void __thiscall

operator___class__lambda_9df27f884b057bc3edfc946cb5b7cf47__class__lambda_e69574bed617af4e071282c136b37893___class__lambda_cc0d902bcbbeb830f749456577db4721___
          (__crt_seh_guarded_call_void_ *this,_lambda_9df27f884b057bc3edfc946cb5b7cf47_ *param_1,
          _lambda_e69574bed617af4e071282c136b37893_ *param_2,
          _lambda_cc0d902bcbbeb830f749456577db4721_ *param_3)

{
  ___acrt_lock(*(int *)param_1);
  LOCK();
  **(int **)(**(int **)param_2 + 0x48) = **(int **)(**(int **)param_2 + 0x48) + 1;
  FUN_0040b2e9();
  return;
}



void FUN_0040b2e9(void)

{
  int unaff_EBP;
  
  ___acrt_unlock(**(int **)(unaff_EBP + 0x10));
  return;
}



// WARNING: Function: __SEH_prolog4 replaced with injection: SEH_prolog4
// WARNING: Function: __SEH_epilog4 replaced with injection: EH_epilog3
// Library Function - Single Match
// Name: 
??$?RV<lambda_51b6e8b1eb166f2a3faf91f424b38130>@@AAV<lambda_6250bd4b2a391816dd638c3bf72b0bcb>@@V<lambda_0b5a4a3e68152e1d9b943535f5f47bed>@@@?$__crt_seh_guarded_call@X@@QAEX$$QAV<lambda_51b6e8b1eb166f2a3faf91f424b38130>@@AAV<lambda_6250bd4b2a391816dd638c3bf72b0bcb>@@$$QAV<lambda_0b5a4a3e68152e1d9b943535f5f47bed>@@@Z
// Library: Visual Studio 2015 Release
// public: void __thiscall __crt_seh_guarded_call<void>::operator()<class
// <lambda_51b6e8b1eb166f2a3faf91f424b38130>,class <lambda_6250bd4b2a391816dd638c3bf72b0bcb>&,class
// <lambda_0b5a4a3e68152e1d9b943535f5f47bed>>(class
// <lambda_51b6e8b1eb166f2a3faf91f424b38130>&&,class
// <lambda_6250bd4b2a391816dd638c3bf72b0bcb>&,class <lambda_0b5a4a3e68152e1d9b943535f5f47bed>&&)

void __thiscall

operator___class__lambda_51b6e8b1eb166f2a3faf91f424b38130__class__lambda_6250bd4b2a391816dd638c3bf72b0bcb___class__lambda_0b5a4a3e68152e1d9b943535f5f47bed___
          (__crt_seh_guarded_call_void_ *this,_lambda_51b6e8b1eb166f2a3faf91f424b38130_ *param_1,
          _lambda_6250bd4b2a391816dd638c3bf72b0bcb_ *param_2,
          _lambda_0b5a4a3e68152e1d9b943535f5f47bed_ *param_3)

{
  int *_Memory;
  int iVar1;
  
  ___acrt_lock(*(int *)param_1);
  _Memory = *(int **)(**(int **)param_2 + 0x48);
  if (_Memory != (int *)0x0) {
    LOCK();
    iVar1 = *_Memory;
    *_Memory = iVar1 + -1;
    if ((iVar1 + -1 == 0) && (_Memory != (int *)&DAT_0041e208)) {
      FID_conflict__free(_Memory);
    }
  }
  FUN_0040b34a();
  return;
}



void FUN_0040b34a(void)

{
  int unaff_EBP;
  
  ___acrt_unlock(**(int **)(unaff_EBP + 0x10));
  return;
}



// WARNING: Function: __SEH_prolog4 replaced with injection: SEH_prolog4
// WARNING: Function: __SEH_epilog4 replaced with injection: EH_epilog3
// Library Function - Single Match
// Name: 
??$?RV<lambda_5b71d36f03204c0beab531769a5b5694>@@AAV<lambda_be2b3da3f62db62e9dad5dc70221a656>@@V<lambda_8f9ce462984622f9bf76b59e2aaaf805>@@@?$__crt_seh_guarded_call@X@@QAEX$$QAV<lambda_5b71d36f03204c0beab531769a5b5694>@@AAV<lambda_be2b3da3f62db62e9dad5dc70221a656>@@$$QAV<lambda_8f9ce462984622f9bf76b59e2aaaf805>@@@Z
// Library: Visual Studio 2015 Release
// public: void __thiscall __crt_seh_guarded_call<void>::operator()<class
// <lambda_5b71d36f03204c0beab531769a5b5694>,class <lambda_be2b3da3f62db62e9dad5dc70221a656>&,class
// <lambda_8f9ce462984622f9bf76b59e2aaaf805>>(class
// <lambda_5b71d36f03204c0beab531769a5b5694>&&,class
// <lambda_be2b3da3f62db62e9dad5dc70221a656>&,class <lambda_8f9ce462984622f9bf76b59e2aaaf805>&&)

void __thiscall

operator___class__lambda_5b71d36f03204c0beab531769a5b5694__class__lambda_be2b3da3f62db62e9dad5dc70221a656___class__lambda_8f9ce462984622f9bf76b59e2aaaf805___
          (__crt_seh_guarded_call_void_ *this,_lambda_5b71d36f03204c0beab531769a5b5694_ *param_1,
          _lambda_be2b3da3f62db62e9dad5dc70221a656_ *param_2,
          _lambda_8f9ce462984622f9bf76b59e2aaaf805_ *param_3)

{
  ___acrt_lock(*(int *)param_1);
  replace_current_thread_locale_nolock(**(__acrt_ptd ***)param_2,(__crt_locale_data *)0x0);
  FUN_0040b395();
  return;
}



void FUN_0040b395(void)

{
  int unaff_EBP;
  
  ___acrt_unlock(**(int **)(unaff_EBP + 0x10));
  return;
}



// WARNING: Function: __SEH_prolog4 replaced with injection: SEH_prolog4
// WARNING: Function: __SEH_epilog4 replaced with injection: EH_epilog3
// Library Function - Single Match
// Name: 
??$?RV<lambda_3518db117f0e7cdb002338c5d3c47b6c>@@AAV<lambda_b2ea41f6bbb362cd97d94c6828d90b61>@@V<lambda_abdedf541bb04549bc734292b4a045d4>@@@?$__crt_seh_guarded_call@X@@QAEX$$QAV<lambda_3518db117f0e7cdb002338c5d3c47b6c>@@AAV<lambda_b2ea41f6bbb362cd97d94c6828d90b61>@@$$QAV<lambda_abdedf541bb04549bc734292b4a045d4>@@@Z
// Library: Visual Studio 2015 Release
// public: void __thiscall __crt_seh_guarded_call<void>::operator()<class
// <lambda_3518db117f0e7cdb002338c5d3c47b6c>,class <lambda_b2ea41f6bbb362cd97d94c6828d90b61>&,class
// <lambda_abdedf541bb04549bc734292b4a045d4>>(class
// <lambda_3518db117f0e7cdb002338c5d3c47b6c>&&,class
// <lambda_b2ea41f6bbb362cd97d94c6828d90b61>&,class <lambda_abdedf541bb04549bc734292b4a045d4>&&)

void __thiscall

operator___class__lambda_3518db117f0e7cdb002338c5d3c47b6c__class__lambda_b2ea41f6bbb362cd97d94c6828d90b61___class__lambda_abdedf541bb04549bc734292b4a045d4___
          (__crt_seh_guarded_call_void_ *this,_lambda_3518db117f0e7cdb002338c5d3c47b6c_ *param_1,
          _lambda_b2ea41f6bbb362cd97d94c6828d90b61_ *param_2,
          _lambda_abdedf541bb04549bc734292b4a045d4_ *param_3)

{
  ___acrt_lock(*(int *)param_1);
  replace_current_thread_locale_nolock
            (**(__acrt_ptd ***)param_2,*(__crt_locale_data **)**(__crt_locale_data ***)(param_2 + 4)
            );
  FUN_0040b3e5();
  return;
}



void FUN_0040b3e5(void)

{
  int unaff_EBP;
  
  ___acrt_unlock(**(int **)(unaff_EBP + 0x10));
  return;
}



void __cdecl FUN_0040b3f1(undefined4 *param_1)

{
  undefined4 local_18;
  undefined4 **local_14;
  undefined4 **local_10;
  undefined *local_c;
  __crt_seh_guarded_call_void_ local_5;
  
  param_1[6] = 1;
  *param_1 = 0x417d40;
  param_1[0xd4] = 1;
  param_1[0x12] = 0x41e208;
  *(undefined2 *)(param_1 + 0x1b) = 0x43;
  *(undefined2 *)((int)param_1 + 0x172) = 0x43;
  param_1[0xd3] = 0;
  local_14 = &param_1;
  local_c = (undefined *)0x5;
  local_18 = 5;
    
  operator___class__lambda_9df27f884b057bc3edfc946cb5b7cf47__class__lambda_e69574bed617af4e071282c136b37893___class__lambda_cc0d902bcbbeb830f749456577db4721___
            (&local_5,(_lambda_9df27f884b057bc3edfc946cb5b7cf47_ *)&local_18,
             (_lambda_e69574bed617af4e071282c136b37893_ *)&local_14,
             (_lambda_cc0d902bcbbeb830f749456577db4721_ *)&local_c);
  local_10 = &param_1;
  local_c = &stack0x00000008;
  local_18 = 4;
  local_14 = (undefined4 **)0x4;
    
  operator___class__lambda_3518db117f0e7cdb002338c5d3c47b6c__class__lambda_b2ea41f6bbb362cd97d94c6828d90b61___class__lambda_abdedf541bb04549bc734292b4a045d4___
            (&local_5,(_lambda_3518db117f0e7cdb002338c5d3c47b6c_ *)&local_14,
             (_lambda_b2ea41f6bbb362cd97d94c6828d90b61_ *)&local_10,
             (_lambda_abdedf541bb04549bc734292b4a045d4_ *)&local_18);
  return;
}



void FUN_0040b48c(undefined4 *param_1)

{
  if (param_1 != (undefined4 *)0x0) {
    FUN_0040b4ad(param_1);
    FID_conflict__free(param_1);
  }
  return;
}



void __cdecl FUN_0040b4ad(undefined4 *param_1)

{
  undefined4 local_14;
  undefined4 **local_10;
  undefined4 local_c;
  __crt_seh_guarded_call_void_ local_5;
  
  if ((undefined *)*param_1 != &DAT_00417d40) {
    FID_conflict__free((undefined *)*param_1);
  }
  FID_conflict__free((void *)param_1[0xf]);
  FID_conflict__free((void *)param_1[0xc]);
  FID_conflict__free((void *)param_1[0xd]);
  FID_conflict__free((void *)param_1[0xe]);
  FID_conflict__free((void *)param_1[10]);
  FID_conflict__free((void *)param_1[0xb]);
  FID_conflict__free((void *)param_1[0x10]);
  FID_conflict__free((void *)param_1[0x11]);
  FID_conflict__free((void *)param_1[0xd8]);
  local_10 = &param_1;
  local_c = 5;
  local_14 = 5;
    
  operator___class__lambda_51b6e8b1eb166f2a3faf91f424b38130__class__lambda_6250bd4b2a391816dd638c3bf72b0bcb___class__lambda_0b5a4a3e68152e1d9b943535f5f47bed___
            (&local_5,(_lambda_51b6e8b1eb166f2a3faf91f424b38130_ *)&local_14,
             (_lambda_6250bd4b2a391816dd638c3bf72b0bcb_ *)&local_10,
             (_lambda_0b5a4a3e68152e1d9b943535f5f47bed_ *)&local_c);
  local_10 = &param_1;
  local_14 = 4;
  local_c = 4;
    
  operator___class__lambda_5b71d36f03204c0beab531769a5b5694__class__lambda_be2b3da3f62db62e9dad5dc70221a656___class__lambda_8f9ce462984622f9bf76b59e2aaaf805___
            (&local_5,(_lambda_5b71d36f03204c0beab531769a5b5694_ *)&local_c,
             (_lambda_be2b3da3f62db62e9dad5dc70221a656_ *)&local_10,
             (_lambda_8f9ce462984622f9bf76b59e2aaaf805_ *)&local_14);
  return;
}



// Library Function - Single Match
// Name: ?replace_current_thread_locale_nolock@@YAXQAU__acrt_ptd@@QAU__crt_locale_data@@@Z
// Library: Visual Studio 2015 Release
// void __cdecl replace_current_thread_locale_nolock(struct __acrt_ptd * const,struct
// __crt_locale_data * const)

void __cdecl replace_current_thread_locale_nolock(__acrt_ptd *param_1,__crt_locale_data *param_2)

{
  undefined **ppuVar1;
  
  if (*(int *)(param_1 + 0x4c) != 0) {
    ___acrt_release_locale_ref(*(int *)(param_1 + 0x4c));
    ppuVar1 = *(undefined ***)(param_1 + 0x4c);
    if (((ppuVar1 != DAT_0041f298) && (ppuVar1 != &PTR_DAT_0041e140)) &&
       (ppuVar1[3] == (undefined *)0x0)) {
      ___acrt_free_locale(ppuVar1);
    }
  }
  *(__crt_locale_data **)(param_1 + 0x4c) = param_2;
  if (param_2 != (__crt_locale_data *)0x0) {
    ___acrt_add_locale_ref((int)param_2);
  }
  return;
}



uint FUN_0040b5c7(void)

{
  code *pcVar1;
  DWORD dwErrCode;
  int iVar2;
  uint uVar3;
  undefined4 *_Memory;
  
  dwErrCode = GetLastError();
  if ((DAT_0041e138 == 0xffffffff) ||
     (_Memory = (undefined4 *)FUN_0040a394(DAT_0041e138), _Memory == (undefined4 *)0x0)) {
    iVar2 = FUN_0040a3d3(DAT_0041e138,(LPVOID)0xffffffff);
    if (iVar2 != 0) {
      _Memory = (undefined4 *)__calloc_base(1,0x364);
      if (_Memory == (undefined4 *)0x0) {
        FUN_0040a3d3(DAT_0041e138,(LPVOID)0x0);
        _Memory = (undefined4 *)0x0;
      }
      else {
        iVar2 = FUN_0040a3d3(DAT_0041e138,_Memory);
        if (iVar2 != 0) {
          FUN_0040b3f1(_Memory);
          FID_conflict__free((void *)0x0);
          goto LAB_0040b669;
        }
        FUN_0040a3d3(DAT_0041e138,(LPVOID)0x0);
      }
      FID_conflict__free(_Memory);
    }
  }
  else {
    if (_Memory != (undefined4 *)0xffffffff) goto LAB_0040b669;
  }
  _Memory = (undefined4 *)0x0;
LAB_0040b669:
  SetLastError(dwErrCode);
  uVar3 = -(uint)(_Memory != (undefined4 *)0x0) & (uint)_Memory;
  if (uVar3 == 0) {
    FUN_00409f3e();
    pcVar1 = (code *)swi(3);
    uVar3 = (*pcVar1)();
    return uVar3;
  }
  return uVar3;
}



undefined4 * FUN_0040b684(void)

{
  code *pcVar1;
  int iVar2;
  undefined4 *_Memory;
  
  if ((DAT_0041e138 == 0xffffffff) ||
     (_Memory = (undefined4 *)FUN_0040a394(DAT_0041e138), _Memory == (undefined4 *)0x0)) {
    iVar2 = FUN_0040a3d3(DAT_0041e138,(LPVOID)0xffffffff);
    if (iVar2 == 0) goto LAB_0040b718;
    _Memory = (undefined4 *)__calloc_base(1,0x364);
    if (_Memory == (undefined4 *)0x0) {
      FUN_0040a3d3(DAT_0041e138,(LPVOID)0x0);
    }
    else {
      iVar2 = FUN_0040a3d3(DAT_0041e138,_Memory);
      if (iVar2 != 0) {
        FUN_0040b3f1(_Memory);
        FID_conflict__free((void *)0x0);
        goto LAB_0040b710;
      }
      FUN_0040a3d3(DAT_0041e138,(LPVOID)0x0);
    }
    FID_conflict__free(_Memory);
  }
  else {
    if (_Memory == (undefined4 *)0xffffffff) goto LAB_0040b718;
LAB_0040b710:
    if (_Memory != (undefined4 *)0x0) {
      return _Memory;
    }
  }
LAB_0040b718:
  FUN_00409f3e();
  pcVar1 = (code *)swi(3);
  _Memory = (undefined4 *)(*pcVar1)();
  return _Memory;
}



uint FUN_0040b71e(void)

{
  DWORD dwErrCode;
  int iVar1;
  undefined4 *_Memory;
  
  dwErrCode = GetLastError();
  if ((DAT_0041e138 == 0xffffffff) ||
     (_Memory = (undefined4 *)FUN_0040a394(DAT_0041e138), _Memory == (undefined4 *)0x0)) {
    iVar1 = FUN_0040a3d3(DAT_0041e138,(LPVOID)0xffffffff);
    if (iVar1 != 0) {
      _Memory = (undefined4 *)__calloc_base(1,0x364);
      if (_Memory == (undefined4 *)0x0) {
        FUN_0040a3d3(DAT_0041e138,(LPVOID)0x0);
        _Memory = (undefined4 *)0x0;
      }
      else {
        iVar1 = FUN_0040a3d3(DAT_0041e138,_Memory);
        if (iVar1 != 0) {
          FUN_0040b3f1(_Memory);
          FID_conflict__free((void *)0x0);
          goto LAB_0040b7c0;
        }
        FUN_0040a3d3(DAT_0041e138,(LPVOID)0x0);
      }
      FID_conflict__free(_Memory);
    }
  }
  else {
    if (_Memory != (undefined4 *)0xffffffff) goto LAB_0040b7c0;
  }
  _Memory = (undefined4 *)0x0;
LAB_0040b7c0:
  SetLastError(dwErrCode);
  return -(uint)(_Memory != (undefined4 *)0x0) & (uint)_Memory;
}



undefined4 FUN_0040b7ff(void)

{
  DWORD DVar1;
  
  DVar1 = DAT_0041e138;
  if (DAT_0041e138 != 0xffffffff) {
    DVar1 = FUN_0040a355(DAT_0041e138);
    DAT_0041e138 = 0xffffffff;
  }
  return CONCAT31((int3)(DVar1 >> 8),1);
}



// Library Function - Single Match
// Name: ___acrt_update_locale_info
// Library: Visual Studio 2015 Release

void __cdecl ___acrt_update_locale_info(int param_1,undefined **param_2)

{
  undefined **ppuVar1;
  
  if ((*param_2 != DAT_0041f298) && ((*(uint *)(param_1 + 0x350) & DAT_0041e780) == 0)) {
    ppuVar1 = FUN_0040efa3();
    *(undefined ***)param_2 = ppuVar1;
  }
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void __cdecl FUN_0040b846(int param_1,int *param_2)

{
  int iVar1;
  
  if ((*param_2 != _DAT_0041f29c) && ((*(uint *)(param_1 + 0x350) & DAT_0041e780) == 0)) {
    iVar1 = FUN_0040de6a();
    *param_2 = iVar1;
  }
  return;
}



undefined4 __cdecl FUN_0040b873(uint *param_1)

{
  uint uVar1;
  undefined4 uStack16;
  
  uVar1 = param_1[1];
  if ((uVar1 >> 0x14 & 0x7ff) == 0x7ff) {
    if ((*param_1 | uVar1 & 0xfffff) == 0) {
      uStack16 = 1;
    }
    else {
      if (((((int)uVar1 < 1) && ((int)uVar1 < 0)) && (*param_1 == 0)) &&
         ((uVar1 & 0xfffff) == 0x80000)) {
        uStack16 = 4;
      }
      else {
        if ((uVar1 & 0x80000) == 0) {
          uStack16 = 3;
        }
        else {
          uStack16 = 2;
        }
      }
    }
  }
  else {
    uStack16 = 0;
  }
  return uStack16;
}



int __cdecl
FUN_0040b8d5(uint *param_1,byte *param_2,uint param_3,undefined8 *param_4,uint param_5,uint param_6,
            byte param_7,int param_8,undefined4 *param_9)

{
  int *piVar1;
  char cVar2;
  short sVar3;
  ushort uVar4;
  undefined4 *puVar5;
  byte *pbVar6;
  int *piVar7;
  uint uVar8;
  char extraout_CL;
  uint uVar9;
  uint uVar10;
  uint extraout_ECX;
  uint extraout_ECX_00;
  int *piVar11;
  int iVar12;
  char *pcVar13;
  bool bVar14;
  undefined8 uVar15;
  int local_3c;
  int local_38;
  char local_30;
  int *local_28;
  int local_20;
  uint local_18;
  uint local_14;
  int *local_10;
  int local_c;
  byte local_5;
  
  if ((int)param_6 < 0) {
    param_6 = 0;
  }
  *param_2 = 0;
  FUN_00405ed6(&local_3c,param_9);
  if (param_3 <= param_6 + 0xb) {
    puVar5 = (undefined4 *)FUN_00407f40();
    iVar12 = 0x22;
    *puVar5 = 0x22;
    FUN_00407e83();
    goto LAB_0040bbd7;
  }
  uVar9 = param_1[1];
  if ((uVar9 >> 0x14 & 0x7ff) == 0x7ff) {
    iVar12 = FUN_0040bbf0(param_1,(char *)param_2,param_3,param_4,param_5,param_6,0,param_8,
                          (undefined4 *)0x0);
    if (iVar12 == 0) {
      pbVar6 = FUN_00416610(param_2,0x65);
      if (pbVar6 != (byte *)0x0) {
        *pbVar6 = (param_7 ^ 1) * ' ' + 0x50;
        pbVar6[3] = 0;
      }
      iVar12 = 0;
    }
    else {
      *param_2 = 0;
    }
    goto LAB_0040bbd7;
  }
  if (((int)uVar9 < 1) && ((int)uVar9 < 0)) {
    *param_2 = 0x2d;
    param_2 = param_2 + 1;
    uVar9 = param_1[1];
  }
  local_28 = (int *)(param_2 + 1);
  local_5 = param_7 ^ 1;
  local_14 = 0x3ff;
  local_20 = (uint)local_5 * 0x20 + 7;
  if ((uVar9 & 0x7ff00000) == 0) {
    *param_2 = 0x30;
    uVar9 = *param_1 | param_1[1] & 0xfffff;
    local_14 = uVar9;
    if (uVar9 != 0) {
      local_14 = 0x3fe;
      goto LAB_0040b9e4;
    }
  }
  else {
    *param_2 = 0x31;
LAB_0040b9e4:
    uVar9 = 0;
  }
  piVar11 = (int *)(param_2 + 2);
  if (param_6 == 0) {
    cVar2 = (char)uVar9;
  }
  else {
    cVar2 = ***(char ***)(local_38 + 0x88);
  }
  *(char *)local_28 = cVar2;
  local_18 = param_1[1] & 0xfffff;
  if ((local_18 != 0) || (local_10 = piVar11, uVar9 <= *param_1 && *param_1 != uVar9)) {
    local_c = 0x30;
    local_18 = 0xf0000;
    local_10 = (int *)uVar9;
    do {
      sVar3 = (short)local_c;
      if ((int)param_6 < 1) break;
      sVar3 = __aullshr();
      cVar2 = (char)(sVar3 + 0x30U);
      if (0x39 < (ushort)(sVar3 + 0x30U)) {
        cVar2 = cVar2 + (char)local_20;
      }
      local_10 = (int *)((uint)local_10 >> 4 | local_18 << 0x1c);
      *(char *)piVar11 = cVar2;
      piVar11 = (int *)((int)piVar11 + 1);
      local_18 = local_18 >> 4;
      local_c = local_c + -4;
      param_6 = param_6 - 1;
      sVar3 = (short)local_c;
    } while (-1 < sVar3);
    local_10 = piVar11;
    if ((-1 < sVar3) && (local_10 = piVar11, uVar4 = __aullshr(), piVar1 = piVar11, 8 < uVar4)) {
      while( true ) {
        piVar7 = (int *)((int)piVar1 + -1);
        cVar2 = *(char *)piVar7;
        if ((cVar2 != 'f') && (cVar2 != 'F')) break;
        *(char *)piVar7 = '0';
        piVar1 = piVar7;
      }
      if (piVar7 == local_28) {
        *(char *)((int)piVar1 + -2) = *(char *)((int)piVar1 + -2) + 1;
      }
      else {
        if (cVar2 == '9') {
          cVar2 = (char)local_20 + ':';
        }
        else {
          cVar2 = cVar2 + 1;
        }
        *(char *)piVar7 = cVar2;
      }
    }
  }
  if (0 < (int)param_6) {
    FUN_00402e50(piVar11,0x30,param_6);
    piVar11 = (int *)((int)piVar11 + param_6);
    local_10 = piVar11;
  }
  if (*(char *)local_28 == 0) {
    local_10 = local_28;
    piVar11 = local_28;
  }
  *(byte *)piVar11 = local_5 * ' ' + 'P';
  uVar8 = __aullshr();
  uVar8 = uVar8 & 0x7ff;
  uVar10 = uVar8 - local_14;
  uVar9 = -(uint)(uVar8 < local_14);
  pcVar13 = (char *)((int)local_10 + 2);
  if (uVar8 < local_14) {
    bVar14 = uVar10 != 0;
    uVar10 = -uVar10;
    uVar9 = -(uVar9 + (uint)bVar14);
    uVar8 = 0x2d;
  }
  else {
    uVar8 = CONCAT31((int3)((uint)param_1 >> 8),0x2b);
  }
  *(char *)((int)local_10 + 1) = (char)uVar8;
  *pcVar13 = '0';
  local_28 = (int *)pcVar13;
  if ((-1 < (int)uVar9) &&
     ((uVar8 = 1000, 0 < (int)uVar9 || (local_28 = (int *)pcVar13, 999 < uVar10)))) {
    uVar8 = 1000;
    local_28 = (int *)pcVar13;
    uVar15 = __alldvrm(uVar10,uVar9,1000,0);
    local_20 = (int)((ulonglong)uVar15 >> 0x20);
    uVar9 = 1000;
    *(char *)local_28 = (char)uVar15 + '0';
    pcVar13 = (char *)local_28 + 1;
    uVar10 = extraout_ECX;
  }
  if (((int *)pcVar13 != local_28) || ((-1 < (int)uVar9 && ((0 < (int)uVar9 || (99 < uVar10)))))) {
    uVar15 = __alldvrm(uVar10,uVar9,100,0);
    local_20 = (int)((ulonglong)uVar15 >> 0x20);
    *pcVar13 = (char)uVar15 + '0';
    pcVar13 = pcVar13 + 1;
    uVar10 = extraout_ECX_00;
    uVar9 = uVar8;
  }
  cVar2 = (char)uVar10;
  if (((int *)pcVar13 != local_28) || ((-1 < (int)uVar9 && ((0 < (int)uVar9 || (9 < uVar10)))))) {
    uVar15 = __alldvrm(uVar10,uVar9,10,0);
    *pcVar13 = (char)uVar15 + '0';
    pcVar13 = pcVar13 + 1;
    cVar2 = extraout_CL;
  }
  *pcVar13 = cVar2 + '0';
  pcVar13[1] = 0;
  iVar12 = 0;
LAB_0040bbd7:
  if (local_30 != 0) {
    *(uint *)(local_3c + 0x350) = *(uint *)(local_3c + 0x350) & 0xfffffffd;
  }
  return iVar12;
}



void __cdecl
FUN_0040bbf0(uint *param_1,char *param_2,uint param_3,undefined8 *param_4,uint param_5,int param_6,
            char param_7,int param_8,undefined4 *param_9)

{
  int iVar1;
  uint uVar2;
  int local_10 [3];
  
  uVar2 = param_6 + 3U;
  if (param_5 <= param_6 + 3U) {
    uVar2 = param_5;
  }
  FUN_00411188(*param_1,param_1[1],param_6 + 1,local_10,param_4,uVar2);
  uVar2 = 0xffffffff;
  if (param_3 != 0xffffffff) {
    uVar2 = (param_3 - (uint)(local_10[0] == 0x2d)) - (uint)(0 < param_6);
  }
  iVar1 = FUN_00410c28((undefined8 *)(param_2 + (uint)(0 < param_6) + (uint)(local_10[0] == 0x2d)),
                       uVar2,param_6 + 1,(int)local_10);
  if (iVar1 == 0) {
    FUN_0040bc94(param_2,param_3,param_6,param_7,param_8,local_10,0,param_9);
  }
  else {
    *param_2 = 0;
  }
  return;
}



undefined4 __cdecl
FUN_0040bc94(char *param_1,uint param_2,int param_3,char param_4,int param_5,int *param_6,
            byte param_7,undefined4 *param_8)

{
  code *pcVar1;
  undefined4 *puVar2;
  char *_SizeInBytes;
  errno_t eVar3;
  int iVar4;
  char cVar5;
  int iVar6;
  undefined4 uVar7;
  char *_Dst;
  undefined4 uVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  int local_14;
  int local_10;
  char local_8;
  
  iVar6 = param_3;
  if (param_3 < 1) {
    iVar6 = 0;
  }
  if (iVar6 + 9U < param_2) {
    FUN_00405ed6(&local_14,param_8);
    if (param_7 != 0) {
      shift_bytes(param_1,param_2,param_1 + (uint)(*param_6 == 0x2d),(uint)(0 < param_3));
    }
    _Dst = param_1;
    if (*param_6 == 0x2d) {
      *param_1 = '-';
      _Dst = param_1 + 1;
    }
    if (0 < param_3) {
      *_Dst = _Dst[1];
      _Dst = _Dst + 1;
      *_Dst = ***(char ***)(local_10 + 0x88);
    }
    _Dst = _Dst + ((uint)param_7 ^ 1) + param_3;
    _SizeInBytes = (char *)0xffffffff;
    if (param_2 != 0xffffffff) {
      _SizeInBytes = param_1 + (param_2 - (int)_Dst);
    }
    eVar3 = _strcpy_s(_Dst,(rsize_t)_SizeInBytes,"e+000");
    if (eVar3 != 0) {
      uVar10 = 0;
      uVar9 = 0;
      uVar8 = 0;
      uVar7 = 0;
      FUN_00407e93();
      pcVar1 = (code *)swi(3);
      uVar7 = (*pcVar1)(uVar7,uVar8,uVar9,uVar10);
      return uVar7;
    }
    if (param_4 != 0) {
      *_Dst = 'E';
    }
    if (*(char *)param_6[2] != '0') {
      iVar6 = param_6[1] + -1;
      if (iVar6 < 0) {
        iVar6 = -iVar6;
        _Dst[1] = '-';
      }
      if (99 < iVar6) {
        iVar4 = iVar6 / 100;
        iVar6 = iVar6 % 100;
        _Dst[2] = _Dst[2] + (char)iVar4;
      }
      cVar5 = (char)iVar6;
      if (9 < iVar6) {
        cVar5 = (char)(iVar6 % 10);
        _Dst[3] = _Dst[3] + (char)(iVar6 / 10);
      }
      _Dst[4] = _Dst[4] + cVar5;
    }
    if ((param_5 == 2) && (_Dst[2] == '0')) {
      FUN_00405050((undefined8 *)(_Dst + 2),(undefined8 *)(_Dst + 3),3);
    }
    if (local_8 != 0) {
      *(uint *)(local_14 + 0x350) = *(uint *)(local_14 + 0x350) & 0xfffffffd;
    }
    uVar7 = 0;
  }
  else {
    puVar2 = (undefined4 *)FUN_00407f40();
    uVar7 = 0x22;
    *puVar2 = 0x22;
    FUN_00407e83();
  }
  return uVar7;
}



// Library Function - Single Match
// Name: ?fp_format_f@@YAHQBNQADI1IHQAU__crt_locale_pointers@@@Z
// Library: Visual Studio 2015 Release
// int __cdecl fp_format_f(double const * const,char * const,unsigned int,char * const,unsigned
// int,int,struct __crt_locale_pointers * const)

int __cdecl
fp_format_f(double *param_1,char *param_2,uint param_3,char *param_4,uint param_5,int param_6,
           __crt_locale_pointers *param_7)

{
  int iVar1;
  uint uVar2;
  int local_10;
  int local_c;
  undefined4 uStack8;
  
  local_10 = 0;
  local_c = 0;
  uStack8 = 0;
  FUN_00411188(*(uint *)param_1,*(uint *)((int)param_1 + 4),param_6,&local_10,(undefined8 *)param_4,
               param_5);
  uVar2 = 0xffffffff;
  if (param_3 != 0xffffffff) {
    uVar2 = param_3 - (uint)(local_10 == 0x2d);
  }
  iVar1 = FUN_00410c28((undefined8 *)(param_2 + (uint)(local_10 == 0x2d)),uVar2,local_c + param_6,
                       (int)&local_10);
  if (iVar1 == 0) {
    iVar1 = FUN_0040be6b(param_2,param_3,param_6,&local_10,0,(undefined4 *)param_7);
  }
  else {
    *param_2 = 0;
  }
  return iVar1;
}



undefined4 __cdecl
FUN_0040be6b(char *param_1,uint param_2,uint param_3,int *param_4,char param_5,undefined4 *param_6)

{
  int iVar1;
  uint uVar2;
  char *pcVar3;
  int local_14;
  int local_10;
  char local_8;
  
  FUN_00405ed6(&local_14,param_6);
  if ((param_5 != 0) && (param_4[1] - 1U == param_3)) {
    *(undefined2 *)(param_1 + (uint)(*param_4 == 0x2d) + (param_4[1] - 1U)) = 0x30;
  }
  pcVar3 = param_1;
  if (*param_4 == 0x2d) {
    *param_1 = '-';
    pcVar3 = param_1 + 1;
  }
  iVar1 = param_4[1];
  if (iVar1 < 1) {
    shift_bytes(param_1,param_2,pcVar3,1);
    *pcVar3 = '0';
    iVar1 = 1;
  }
  pcVar3 = pcVar3 + iVar1;
  if (0 < (int)param_3) {
    shift_bytes(param_1,param_2,pcVar3,1);
    *pcVar3 = ***(char ***)(local_10 + 0x88);
    if (param_4[1] < 0) {
      uVar2 = -param_4[1];
      if ((param_5 != 0) || ((int)uVar2 < (int)param_3)) {
        param_3 = uVar2;
      }
      shift_bytes(param_1,param_2,pcVar3 + 1,param_3);
      FUN_00402e50((int *)(pcVar3 + 1),0x30,param_3);
    }
  }
  if (local_8 != 0) {
    *(uint *)(local_14 + 0x350) = *(uint *)(local_14 + 0x350) & 0xfffffffd;
  }
  return 0;
}



void __cdecl
FUN_0040bf3e(uint *param_1,char *param_2,uint param_3,undefined8 *param_4,rsize_t param_5,
            uint param_6,char param_7,int param_8,undefined4 *param_9)

{
  undefined8 *puVar1;
  uint uVar2;
  int iVar3;
  undefined8 *puVar4;
  bool bVar5;
  int local_14;
  int local_10;
  undefined4 uStack12;
  int local_8;
  
  local_14 = 0;
  local_10 = 0;
  uStack12 = 0;
  FUN_00411188(*param_1,param_1[1],param_6,&local_14,param_4,param_5);
  bVar5 = local_14 == 0x2d;
  local_8 = local_10 + -1;
  uVar2 = 0xffffffff;
  if (param_3 != 0xffffffff) {
    uVar2 = param_3 - (uint)bVar5;
  }
  iVar3 = FUN_00410c28((undefined8 *)(param_2 + (uint)bVar5),uVar2,param_6,(int)&local_14);
  if (iVar3 == 0) {
    iVar3 = local_10 + -1;
    if ((iVar3 < -4) || ((int)param_6 <= iVar3)) {
      FUN_0040bc94(param_2,param_3,param_6,param_7,param_8,&local_14,1,param_9);
    }
    else {
      puVar1 = (undefined8 *)(param_2 + (uint)bVar5);
      if (local_8 < iVar3) {
        do {
          puVar4 = puVar1;
          puVar1 = (undefined8 *)((int)puVar4 + 1);
        } while (*(char *)puVar4 != 0);
        *(undefined *)((int)puVar4 + -1) = 0;
      }
      FUN_0040be6b(param_2,param_3,param_6,&local_14,1,param_9);
    }
  }
  else {
    *param_2 = 0;
  }
  return;
}



void __cdecl FUN_0040c001(int param_1,byte param_2,char *param_3,char *param_4,byte param_5)

{
  char *pcVar1;
  int iVar2;
  char cVar3;
  code *pcVar4;
  int iVar5;
  errno_t eVar6;
  char *pcVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  char *local_48;
  undefined *local_44;
  undefined *local_40;
  undefined *local_3c;
  undefined *local_38;
  undefined *local_34;
  undefined *local_30;
  undefined *local_2c;
  char *local_28;
  undefined *local_24;
  char *local_20;
  undefined *local_1c;
  char *local_18;
  undefined *local_14;
  char *local_10;
  undefined *local_c;
  uint local_8;
  
  local_8 = DAT_0041e004 ^ (uint)&stack0xfffffffc;
  if (param_4 < (char *)((uint)param_2 + 4)) {
    *param_3 = 0;
  }
  else {
    if (param_2 != 0) {
      *param_3 = '-';
      param_3 = param_3 + 1;
      param_4 = param_4 + -1;
      *param_3 = 0;
    }
    local_28 = "NAN(SNAN)";
    local_48 = "INF";
    local_44 = &DAT_00418490;
    local_40 = &DAT_00418494;
    local_3c = &DAT_00418494;
    local_30 = &DAT_0041849c;
    local_2c = &DAT_0041849c;
    iVar5 = ((uint)param_5 ^ 1) * 2;
    local_1c = &DAT_0041849c;
    local_c = &DAT_0041849c;
    local_38 = &DAT_00418498;
    local_34 = &DAT_00418498;
    local_24 = &DAT_00418498;
    iVar2 = param_1 * 4 + -4;
    local_20 = "nan(snan)";
    local_18 = "NAN(IND)";
    local_14 = &DAT_00418498;
    local_10 = "nan(ind)";
    pcVar7 = (&local_48)[iVar2 + iVar5];
    pcVar1 = pcVar7 + 1;
    do {
      cVar3 = *pcVar7;
      pcVar7 = pcVar7 + 1;
    } while (cVar3 != 0);
    eVar6 = _strcpy_s(param_3,(rsize_t)param_4,
                      (&local_48)[(uint)(param_4 <= pcVar7 + -(int)pcVar1) + iVar2 + iVar5]);
    if (eVar6 != 0) {
      uVar11 = 0;
      uVar10 = 0;
      uVar9 = 0;
      uVar8 = 0;
      FUN_00407e93();
      pcVar4 = (code *)swi(3);
      (*pcVar4)(uVar8,uVar9,uVar10,uVar11);
      return;
    }
  }
  FUN_00401f78();
  return;
}



// Library Function - Single Match
// Name: ?shift_bytes@@YAXQADI0H@Z
// Library: Visual Studio 2015 Release
// void __cdecl shift_bytes(char * const,unsigned int,char * const,int)

void __cdecl shift_bytes(char *param_1,uint param_2,char *param_3,int param_4)

{
  char cVar1;
  char *pcVar2;
  
  if (param_4 != 0) {
    pcVar2 = param_3;
    do {
      cVar1 = *pcVar2;
      pcVar2 = pcVar2 + 1;
    } while (cVar1 != 0);
    FUN_00405050((undefined8 *)(param_3 + param_4),(undefined8 *)param_3,
                 (uint)(pcVar2 + (1 - (int)(param_3 + 1))));
  }
  return;
}



int __cdecl
FUN_0040c12a(double *param_1,byte *param_2,char *param_3,undefined8 *param_4,uint param_5,
            int param_6,uint param_7,uint param_8,undefined4 param_9,undefined4 *param_10)

{
  undefined4 *puVar1;
  int iVar2;
  byte local_c;
  byte local_8;
  
  if ((((param_2 == (byte *)0x0) || (param_3 == (char *)0x0)) || (param_4 == (undefined8 *)0x0)) ||
     (param_5 == 0)) {
    puVar1 = (undefined4 *)FUN_00407f40();
    iVar2 = 0x16;
    *puVar1 = 0x16;
    FUN_00407e83();
  }
  else {
    if (((param_6 == 0x41) || (param_6 == 0x45)) ||
       ((param_6 == 0x46 || (local_8 = 0, param_6 == 0x47)))) {
      local_8 = 1;
    }
    if (((param_8 & 8) == 0) && (iVar2 = FUN_0040b873((uint *)param_1), iVar2 != 0)) {
      if ((*(int *)((int)param_1 + 4) < 1) && (*(int *)((int)param_1 + 4) < 0)) {
        local_c = 1;
      }
      else {
        local_c = 0;
      }
      iVar2 = FUN_0040c001(iVar2,local_c,(char *)param_2,param_3,local_8);
    }
    else {
      if ((param_8 & 0x10) == 0) {
        iVar2 = 2;
      }
      else {
        iVar2 = 3;
      }
      if (param_6 < 0x62) {
        if ((param_6 == 0x61) || (param_6 == 0x41)) {
          iVar2 = FUN_0040b8d5((uint *)param_1,param_2,(uint)param_3,param_4,param_5,param_7,local_8
                               ,iVar2,param_10);
          return iVar2;
        }
        param_6 = param_6 + -0x45;
      }
      else {
        param_6 = param_6 + -0x65;
      }
      if (param_6 == 0) {
        iVar2 = FUN_0040bbf0((uint *)param_1,(char *)param_2,(uint)param_3,param_4,param_5,param_7,
                             local_8,iVar2,param_10);
      }
      else {
        if (param_6 == 1) {
          iVar2 = fp_format_f(param_1,(char *)param_2,(uint)param_3,(char *)param_4,param_5,param_7,
                              (__crt_locale_pointers *)param_10);
        }
        else {
          iVar2 = FUN_0040bf3e((uint *)param_1,(char *)param_2,(uint)param_3,param_4,param_5,param_7
                               ,local_8,iVar2,param_10);
        }
      }
    }
  }
  return iVar2;
}



// Library Function - Single Match
// Name: __fileno
// Library: Visual Studio 2015 Release

int __cdecl __fileno(FILE *_File)

{
  undefined4 *puVar1;
  
  if (_File == (FILE *)0x0) {
    puVar1 = (undefined4 *)FUN_00407f40();
    *puVar1 = 0x16;
    FUN_00407e83();
    return -1;
  }
  return _File->_file;
}



uint __cdecl FUN_0040c28e(byte param_1,FILE *param_2)

{
  char **ppcVar1;
  uint uVar2;
  
  ppcVar1 = &param_2->_base;
  *ppcVar1 = *ppcVar1 + -1;
  if ((int)*ppcVar1 < 0) {
    uVar2 = FUN_0041274c(param_1,param_2);
    return uVar2;
  }
  *param_2->_ptr = param_1;
  param_2->_ptr = param_2->_ptr + 1;
  return (uint)param_1;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address
// Library Function - Single Match
// Name: __get_printf_count_output
// Library: Visual Studio 2015 Release

int __cdecl __get_printf_count_output(void)

{
  return (uint)(_DAT_0041f28c == (DAT_0041e004 | 1));
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

uint __cdecl FUN_0040c2cc(FILE *param_1)

{
  FILE **ppFVar1;
  int _FileHandle;
  char *pcVar2;
  FILE *pFVar3;
  char **ppcVar4;
  
  _FileHandle = __fileno(param_1);
  pFVar3 = (FILE *)__isatty(_FileHandle);
  if (pFVar3 == (FILE *)0x0) {
LAB_0040c378:
    return (uint)pFVar3 & 0xffffff00;
  }
  pFVar3 = (FILE *)FUN_0040568c(1);
  if (param_1 == pFVar3) {
    ppcVar4 = (char **)&DAT_0041f290;
  }
  else {
    pFVar3 = (FILE *)FUN_0040568c(2);
    if (param_1 != pFVar3) goto LAB_0040c378;
    ppcVar4 = (char **)&DAT_0041f294;
  }
  _DAT_0041ed2c = _DAT_0041ed2c + 1;
  ppFVar1 = (FILE **)&param_1->_flag;
  pFVar3 = *ppFVar1;
  if (((uint)pFVar3 & 0x4c0) != 0) goto LAB_0040c378;
  LOCK();
  *ppFVar1 = (FILE *)((uint)*ppFVar1 | 0x282);
  pcVar2 = *ppcVar4;
  if (pcVar2 == (char *)0x0) {
    pcVar2 = (char *)__malloc_base(0x1000);
    *ppcVar4 = pcVar2;
    FID_conflict__free((void *)0x0);
    pcVar2 = *ppcVar4;
    if (pcVar2 == (char *)0x0) {
      param_1->_base = (char *)0x2;
      *(int **)&param_1->_cnt = &param_1->_charbuf;
      *(int **)&param_1->_ptr = &param_1->_charbuf;
      param_1->_bufsiz = 2;
      goto LAB_0040c35d;
    }
  }
  *(char **)&param_1->_cnt = pcVar2;
  pcVar2 = *ppcVar4;
  param_1->_ptr = pcVar2;
  param_1->_base = (char *)0x1000;
  param_1->_bufsiz = 0x1000;
LAB_0040c35d:
  return CONCAT31((int3)((uint)pcVar2 >> 8),1);
}



// Library Function - Single Match
// Name: ___acrt_stdio_end_temporary_buffering_nolock
// Library: Visual Studio 2015 Release

void __cdecl ___acrt_stdio_end_temporary_buffering_nolock(char param_1,FILE *param_2)

{
  uint *puVar1;
  
  if ((param_1 != 0) && (puVar1 = (uint *)&param_2->_flag, (*puVar1 >> 9 & 1) != 0)) {
    FUN_0040a7bb(param_2);
    LOCK();
    *puVar1 = *puVar1 & 0xfffffd7f;
    param_2->_bufsiz = 0;
    param_2->_cnt = 0;
    param_2->_ptr = (char *)0x0;
  }
  return;
}



void __cdecl
FUN_0040c3b9(uint param_1,uint param_2,LPCWSTR param_3,int param_4,LPSTR param_5,int param_6,
            uint param_7,uint param_8)

{
  byte bVar1;
  DWORD dwFlags;
  bool bVar2;
  
  dwFlags = 0;
  if ((param_1 == 65000) || (bVar1 = 0, param_1 == 0xfde9)) {
    bVar1 = 1;
  }
  if (param_1 < 0xc436) {
    if ((param_1 == 0xc435) || (param_1 == 0x2a)) goto LAB_0040c433;
    if (0xc42b < param_1) {
      if ((param_1 < 0xc42f) || (param_1 == 0xc431)) goto LAB_0040c433;
      bVar2 = param_1 == 0xc433;
LAB_0040c428:
      if (bVar2) goto LAB_0040c433;
    }
  }
  else {
    if (param_1 == 0xd698) goto LAB_0040c433;
    if (0xdea9 < param_1) {
      if ((param_1 < 0xdeb4) || (param_1 == 65000)) goto LAB_0040c433;
      bVar2 = param_1 == 0xfde9;
      goto LAB_0040c428;
    }
  }
  dwFlags = param_2 & 0xffffff7f;
LAB_0040c433:
  WideCharToMultiByte(param_1,dwFlags,param_3,param_4,param_5,param_6,
                      (LPCSTR)(~-(uint)bVar1 & param_7),(LPBOOL)(~-(uint)bVar1 & param_8));
  return;
}



void __cdecl FUN_0040c466(undefined8 *param_1,ushort **param_2,uint param_3,int *param_4)

{
  uint uVar1;
  ushort *puVar2;
  int iVar3;
  undefined8 *puVar4;
  ushort *puVar5;
  undefined8 *local_24;
  ushort *local_14;
  undefined8 local_10;
  uint local_8;
  
  local_8 = DAT_0041e004 ^ (uint)&stack0xfffffffc;
  puVar5 = *param_2;
  local_24 = param_1;
  puVar2 = puVar5;
  if (param_1 == (undefined8 *)0x0) {
    while ((iVar3 = FUN_004127ad((byte *)&local_10,(uint)*puVar5,param_4), iVar3 != -1 &&
           ((iVar3 == 0 || (*(char *)((int)&local_14 + iVar3 + 3) != 0))))) {
      puVar5 = puVar5 + 1;
    }
  }
  else {
    while( true ) {
      local_14 = puVar2;
      puVar4 = param_1;
      if (param_3 < 4) {
        puVar4 = &local_10;
      }
      uVar1 = FUN_004127ad((byte *)puVar4,(uint)*puVar5,param_4);
      if (uVar1 == 0xffffffff) {
        *param_2 = local_14;
        goto LAB_0040c555;
      }
      if (puVar4 != param_1) {
        puVar2 = local_14;
        if (param_3 < uVar1) goto LAB_0040c50d;
        copy_buffer(param_1,puVar4,uVar1);
      }
      if ((uVar1 != 0) && (*(char *)((int)param_1 + (uVar1 - 1)) == 0)) break;
      puVar5 = puVar5 + 1;
      if (uVar1 != 0) {
        local_14 = puVar5;
      }
      param_3 = param_3 - uVar1;
      param_1 = (undefined8 *)((int)param_1 + uVar1);
      puVar2 = local_14;
    }
    puVar2 = (ushort *)0x0;
LAB_0040c50d:
    *param_2 = puVar2;
  }
LAB_0040c555:
  FUN_00401f78(local_24);
  return;
}



// Library Function - Single Match
// Name: ___pctype_func
// Library: Visual Studio 2015 Release

ushort * __cdecl ___pctype_func(void)

{
  uint uVar1;
  ushort **local_8;
  
  uVar1 = FUN_0040b5c7();
  local_8 = *(ushort ***)(uVar1 + 0x4c);
  ___acrt_update_locale_info(uVar1,(undefined **)&local_8);
  return *local_8;
}



// WARNING: Could not reconcile some variable overlaps

uint __cdecl FUN_0040c58c(ushort param_1,ushort param_2)

{
  int iVar1;
  uint local_c;
  uint local_8;
  
  if (param_1 != 0xffff) {
    if (param_1 < 0x100) {
      return (uint)(*(ushort *)(PTR_DAT_0041e204 + (uint)param_1 * 2) & param_2);
    }
    local_c = (uint)param_1;
    local_8 = 0;
    iVar1 = FUN_00412849(1,(LPCWSTR)&local_c,1,(LPWORD)&local_8);
    if (iVar1 != 0) {
      return (uint)((ushort)local_8 & param_2);
    }
  }
  return 0;
}



void __cdecl FUN_0040c5f7(int param_1,undefined4 param_2,undefined4 *param_3)

{
  int iVar1;
  int local_24;
  int local_20 [2];
  char local_18;
  CHAR local_14;
  CHAR local_13;
  undefined local_12;
  undefined4 local_10;
  undefined2 local_c;
  uint local_8;
  
  local_8 = DAT_0041e004 ^ (uint)&stack0xfffffffc;
  FUN_00405ed6(&local_24,param_3);
  if ((param_1 < -1) || (0xff < param_1)) {
    iVar1 = FUN_00410771(param_1 >> 8 & 0xff,local_20);
    if (iVar1 == 0) {
      local_13 = 0;
      iVar1 = 1;
      local_14 = (CHAR)param_1;
    }
    else {
      local_12 = 0;
      iVar1 = 2;
      local_14 = (CHAR)((uint)param_1 >> 8);
      local_13 = (CHAR)param_1;
    }
    local_10 = 0;
    local_c = 0;
    iVar1 = FUN_0040ebb3(local_20,1,&local_14,iVar1,(LPWORD)&local_10,*(uint *)(local_20[0] + 8),1);
    if (iVar1 == 0) {
      if (local_18 != 0) {
        *(uint *)(local_24 + 0x350) = *(uint *)(local_24 + 0x350) & 0xfffffffd;
      }
      goto LAB_0040c6bd;
    }
  }
  if (local_18 != 0) {
    *(uint *)(local_24 + 0x350) = *(uint *)(local_24 + 0x350) & 0xfffffffd;
  }
LAB_0040c6bd:
  FUN_00401f78();
  return;
}



// WARNING: Function: __SEH_prolog4 replaced with injection: SEH_prolog4
// WARNING: Function: __SEH_epilog4 replaced with injection: EH_epilog3
// Library Function - Single Match
// Name: ??$common_dupenv_s@_W@@YAHQAPA_WQAIQB_WHQBDH@Z
// Library: Visual Studio 2015 Release
// int __cdecl common_dupenv_s<wchar_t>(wchar_t * * const,unsigned int * const,wchar_t const *
// const,int,char const * const,int)

int __cdecl
common_dupenv_s_wchar_t_
          (wchar_t **param_1,uint *param_2,wchar_t *param_3,int param_4,char *param_5,int param_6)

{
  int iVar1;
  
  ___acrt_lock(0xb);
  iVar1 = common_dupenv_s_nolock_wchar_t_(param_1,param_2,param_3,param_4,param_5,param_6);
  FUN_0040c71e();
  return iVar1;
}



void FUN_0040c71e(void)

{
  ___acrt_unlock(0xb);
  return;
}



// Library Function - Single Match
// Name: ??$common_dupenv_s_nolock@_W@@YAHQAPA_WQAIQB_WHQBDH@Z
// Library: Visual Studio 2015 Release
// int __cdecl common_dupenv_s_nolock<wchar_t>(wchar_t * * const,unsigned int * const,wchar_t const
// * const,int,char const * const,int)

int __cdecl
common_dupenv_s_nolock_wchar_t_
          (wchar_t **param_1,uint *param_2,wchar_t *param_3,int param_4,char *param_5,int param_6)

{
  size_t _Count;
  WCHAR WVar1;
  code *pcVar2;
  PCNZWCH _Src;
  wchar_t *_Dst;
  undefined4 *puVar3;
  errno_t eVar4;
  int iVar5;
  PCNZWCH pWVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  
  if (param_1 != (wchar_t **)0x0) {
    *param_1 = (wchar_t *)0x0;
    if (param_2 != (uint *)0x0) {
      *param_2 = 0;
    }
    if (param_3 != (wchar_t *)0x0) {
      _Src = FUN_0040c7d5((ushort *)param_3);
      if (_Src != (PCNZWCH)0x0) {
        pWVar6 = _Src;
        do {
          WVar1 = *pWVar6;
          pWVar6 = pWVar6 + 1;
        } while (WVar1 != L'\0');
        _Count = ((int)((int)pWVar6 - (int)(_Src + 1)) >> 1) + 1;
        _Dst = (wchar_t *)_calloc(_Count,2);
        *(wchar_t **)param_1 = _Dst;
        if (_Dst == (wchar_t *)0x0) {
          puVar3 = (undefined4 *)FUN_00407f40();
          *puVar3 = 0xc;
          return 0xc;
        }
        eVar4 = _wcscpy_s(_Dst,_Count,_Src);
        if (eVar4 != 0) {
          uVar10 = 0;
          uVar9 = 0;
          uVar8 = 0;
          uVar7 = 0;
          FUN_00407e93();
          pcVar2 = (code *)swi(3);
          iVar5 = (*pcVar2)(uVar7,uVar8,uVar9,uVar10);
          return iVar5;
        }
        if (param_2 != (uint *)0x0) {
          *param_2 = _Count;
        }
      }
      return 0;
    }
  }
  puVar3 = (undefined4 *)FUN_00407f40();
  *puVar3 = 0x16;
  FUN_00407e83();
  return 0x16;
}



PCNZWCH __cdecl FUN_0040c7d5(ushort *param_1)

{
  ushort uVar1;
  WCHAR WVar2;
  PCNZWCH pWVar3;
  int iVar4;
  PCNZWCH pWVar5;
  ushort *puVar6;
  uint uVar7;
  PCNZWCH *ppWVar8;
  
  ppWVar8 = (PCNZWCH *)thunk_FUN_00409226();
  if ((ppWVar8 != (PCNZWCH *)0x0) && (param_1 != (ushort *)0x0)) {
    puVar6 = param_1;
    do {
      uVar1 = *puVar6;
      puVar6 = puVar6 + 1;
    } while (uVar1 != 0);
    uVar7 = (int)((int)puVar6 - (int)(param_1 + 1)) >> 1;
    while (pWVar3 = *ppWVar8, pWVar3 != (PCNZWCH)0x0) {
      pWVar5 = pWVar3;
      do {
        WVar2 = *pWVar5;
        pWVar5 = pWVar5 + 1;
      } while (WVar2 != L'\0');
      if (((uVar7 < (uint)((int)((int)pWVar5 - (int)(pWVar3 + 1)) >> 1)) && (pWVar3[uVar7] == L'='))
         && (iVar4 = FUN_00412862(pWVar3,param_1,uVar7), iVar4 == 0)) {
        return *ppWVar8 + uVar7 + 1;
      }
      ppWVar8 = ppWVar8 + 1;
    }
  }
  return (PCNZWCH)0x0;
}



// Library Function - Multiple Matches With Different Base Names
// Name: __dupenv_s, __wdupenv_s
// Library: Visual Studio 2015 Release

errno_t __cdecl
FID_conflict___dupenv_s(wchar_t **_Buffer,size_t *_BufferSizeInWords,wchar_t *_VarName)

{
  int iVar1;
  
  iVar1 = common_dupenv_s_wchar_t_
                    ((wchar_t **)_Buffer,_BufferSizeInWords,(wchar_t *)_VarName,1,(char *)0x0,0);
  return iVar1;
}



// WARNING: Function: __SEH_prolog4 replaced with injection: SEH_prolog4
// WARNING: Function: __SEH_epilog4 replaced with injection: EH_epilog3
// Library Function - Multiple Matches With Same Base Name
// Name: operator()<>
// Libraries: Visual Studio 2015 Debug, Visual Studio 2015 Release

uint operator____(int *param_1,uint **param_2)

{
  uint uVar1;
  uint uVar2;
  
  ___acrt_lock(*param_1);
  uVar1 = FUN_0040cced(param_2);
  uVar2 = FUN_0040c8be();
  return uVar2 & 0xffffff00 | uVar1 & 0xff;
}



void FUN_0040c8be(void)

{
  int unaff_EBP;
  
  ___acrt_unlock(**(int **)(unaff_EBP + 0x10));
  return;
}



HANDLE __cdecl FUN_0040cad5(uint param_1,LPCWSTR param_2,wchar_t **param_3,wint_t **param_4)

{
  code *pcVar1;
  char cVar2;
  undefined4 *puVar3;
  int iVar4;
  BOOL BVar5;
  DWORD DVar6;
  HANDLE hObject;
  HANDLE pvVar7;
  uint uVar8;
  _STARTUPINFOW local_70;
  _PROCESS_INFORMATION local_2c;
  HANDLE local_1c;
  HANDLE local_18;
  WORD local_14;
  LPWSTR local_10;
  LPBYTE local_c;
  void *local_8;
  
  if ((param_2 == (LPCWSTR)0x0) || (param_3 == (wchar_t **)0x0)) {
LAB_0040cae3:
    puVar3 = (undefined4 *)FUN_00407f40();
    *puVar3 = 0x16;
    FUN_00407e83();
    return (HANDLE)0xffffffff;
  }
  if (4 < param_1) {
    puVar3 = (undefined4 *)FUN_00407f2d();
    *puVar3 = 0;
    goto LAB_0040cae3;
  }
  local_10 = (LPWSTR)0x0;
  local_8 = (void *)0x0;
  iVar4 = FID_conflict____acrt_pack_narrow_command_line_and_environment
                    (param_3,param_4,&local_10,&local_8);
  if (iVar4 == -1) {
    FID_conflict__free(local_8);
    local_8 = (void *)0x0;
    FID_conflict__free(local_10);
    return (HANDLE)0xffffffff;
  }
  local_c = (LPBYTE)0x0;
  cVar2 = FUN_0040ce2d(0xf4,0xec,param_1 != 4);
  if (cVar2 == 0) {
    FID_conflict__free(local_c);
    local_c = (LPBYTE)0x0;
    FID_conflict__free(local_8);
    local_8 = (void *)0x0;
    FID_conflict__free(local_10);
    return (HANDLE)0xffffffff;
  }
  uVar8 = 0;
  if (param_1 == 4) {
    uVar8 = 8;
  }
  puVar3 = (undefined4 *)FUN_00407f2d();
  *puVar3 = 0;
  FUN_00402e50((int *)&local_70,0,0x44);
  local_70.cbReserved2 = local_14;
  local_70.lpReserved2 = local_c;
  local_70.cb = 0x44;
  BVar5 = CreateProcessW(param_2,local_10,(LPSECURITY_ATTRIBUTES)0x0,(LPSECURITY_ATTRIBUTES)0x0,1,
                         uVar8 | 0x400,local_8,(LPCWSTR)0x0,(LPSTARTUPINFOW)&local_70,
                         (LPPROCESS_INFORMATION)&local_2c);
  hObject = local_2c.hProcess;
  if (BVar5 != 0) {
    if (param_1 == 2) {
      FUN_004097d4(0);
      pcVar1 = (code *)swi(3);
      hObject = (HANDLE)(*pcVar1)();
      return hObject;
    }
    if (param_1 != 0) {
      if (param_1 != 4) {
        if (local_2c.hThread != (HANDLE)0xffffffff) {
          CloseHandle(local_2c.hThread);
        }
        FID_conflict__free(local_c);
        local_c = (LPBYTE)0x0;
        FID_conflict__free(local_8);
        local_8 = (void *)0x0;
        FID_conflict__free(local_10);
        return local_2c.hProcess;
      }
      if (local_2c.hThread != (HANDLE)0xffffffff) {
        CloseHandle(local_2c.hThread);
      }
      if (local_2c.hProcess != (HANDLE)0xffffffff) {
        CloseHandle(local_2c.hProcess);
      }
      pvVar7 = (HANDLE)0x0;
      goto LAB_0040cc8a;
    }
    WaitForSingleObject(local_2c.hProcess,0xffffffff);
    BVar5 = GetExitCodeProcess(local_2c.hProcess,(LPDWORD)&local_18);
    if (BVar5 != 0) {
      local_1c = local_18;
      if (local_2c.hThread != (HANDLE)0xffffffff) {
        CloseHandle(local_2c.hThread);
      }
      pvVar7 = local_1c;
      if (hObject != (HANDLE)0xffffffff) {
        CloseHandle(hObject);
        pvVar7 = local_1c;
      }
      goto LAB_0040cc8a;
    }
  }
  DVar6 = GetLastError();
  ___acrt_errno_map_os_error(DVar6);
  if (local_2c.hThread != (HANDLE)0xffffffff) {
    CloseHandle(local_2c.hThread);
  }
  pvVar7 = (HANDLE)0xffffffff;
  if (hObject != (HANDLE)0xffffffff) {
    CloseHandle(hObject);
    pvVar7 = (HANDLE)0xffffffff;
  }
LAB_0040cc8a:
  FID_conflict__free(local_c);
  local_c = (LPBYTE)0x0;
  FID_conflict__free(local_8);
  local_8 = (void *)0x0;
  FID_conflict__free(local_10);
  return pvVar7;
}



uint __fastcall FUN_0040cced(uint **param_1)

{
  uint *puVar1;
  uint extraout_EAX;
  int iVar2;
  undefined4 uVar3;
  uint *puVar4;
  byte bVar5;
  undefined4 *puVar6;
  uint uVar7;
  uint uVar8;
  uint uVar9;
  uint uVar10;
  undefined4 *local_c;
  
  *(undefined4 *)**param_1 = 0;
  *(undefined4 *)*param_1[1] = 0;
  uVar7 = DAT_0041f280;
  uVar8 = DAT_0041f280;
  while ((uVar7 != 0 &&
         (uVar8 = uVar8 - 1,
         *(char *)((&DAT_0041f080)[uVar8 >> 6] + 0x28 + (uVar8 & 0x3f) * 0x38) != 0))) {
    uVar7 = uVar7 - 1;
  }
  if (uVar7 < 0x3332) {
    uVar8 = uVar7 * 5 + 4 & 0xffff;
    puVar1 = (uint *)__calloc_base(uVar8,1);
    if (puVar1 == (uint *)0x0) {
      puVar6 = (undefined4 *)FUN_00407f40();
      bVar5 = 0;
      *puVar6 = 0xc;
    }
    else {
      puVar4 = puVar1 + 1;
      *puVar1 = uVar7;
      local_c = (undefined4 *)((int)puVar4 + uVar7);
      uVar9 = 0;
      puVar6 = local_c;
      if (uVar7 != 0) {
        do {
          iVar2 = (uVar9 & 0x3f) * 0x38 + (&DAT_0041f080)[uVar9 >> 6];
          bVar5 = *(byte *)(iVar2 + 0x28);
          if ((bVar5 & 0x10) == 0) {
            *(byte *)((int)puVar4 + uVar9) = bVar5;
            uVar3 = *(undefined4 *)(iVar2 + 0x18);
          }
          else {
            *(undefined *)((int)puVar4 + uVar9) = 0;
            uVar3 = 0xffffffff;
          }
          *puVar6 = uVar3;
          uVar9 = uVar9 + 1;
          puVar6 = puVar6 + 1;
        } while (uVar9 != uVar7);
      }
      if (*(char *)param_1[2] == 0) {
        uVar9 = 0;
        while( true ) {
          uVar10 = uVar7;
          if (2 < uVar7) {
            uVar10 = 3;
          }
          if (uVar9 == uVar10) break;
          *(undefined *)puVar4 = 0;
          *local_c = 0xffffffff;
          uVar9 = uVar9 + 1;
          puVar4 = (uint *)((int)puVar4 + 1);
          local_c = local_c + 1;
        }
      }
      *(uint **)**param_1 = puVar1;
      bVar5 = 1;
      *(uint *)*param_1[1] = uVar8;
    }
    FID_conflict__free((void *)0x0);
    uVar7 = extraout_EAX & 0xffffff00 | (uint)bVar5;
  }
  else {
    puVar6 = (undefined4 *)FUN_00407f40();
    *puVar6 = 0xc;
    uVar7 = (uint)puVar6 & 0xffffff00;
  }
  return uVar7;
}



void FUN_0040ce2d(undefined1 param_1,undefined1 param_2,undefined1 param_3)

{
  uint *local_1c;
  undefined1 *local_18;
  undefined1 *local_14;
  int local_10;
  undefined4 local_c;
  
  local_1c = (uint *)&param_1;
  local_18 = &param_2;
  local_14 = &param_3;
  local_c = 7;
  local_10 = 7;
  operator____(&local_10,&local_1c);
  return;
}



HANDLE __cdecl FUN_0040ce68(uint param_1,ushort *param_2,wchar_t **param_3,wint_t **param_4)

{
  uint _SizeInWords;
  ushort uVar1;
  code *pcVar2;
  undefined4 *puVar3;
  ushort *file_name;
  ushort *puVar4;
  errno_t eVar5;
  HANDLE pvVar6;
  wchar_t *_Dst;
  int iVar7;
  ushort *_Dst_00;
  wchar_t *_Src;
  undefined4 uVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  
  if ((((param_2 == (ushort *)0x0) || (*param_2 == 0)) || (param_3 == (wchar_t **)0x0)) ||
     ((*param_3 == (wchar_t *)0x0 || (**param_3 == L'\0')))) {
    puVar3 = (undefined4 *)FUN_00407f40();
    *puVar3 = 0x16;
    FUN_00407e83();
    return (HANDLE)0xffffffff;
  }
  file_name = FUN_00402abe((char *)param_2,0x5c);
  puVar4 = FUN_00402abe((char *)param_2,0x2f);
  if (puVar4 == (ushort *)0x0) {
    _Dst_00 = param_2;
    if ((file_name == (ushort *)0x0) &&
       (file_name = FUN_00402abe((char *)param_2,0x3a), _Dst_00 = param_2,
       file_name == (ushort *)0x0)) {
      file_name = param_2;
      do {
        uVar1 = *file_name;
        file_name = file_name + 1;
      } while (uVar1 != 0);
      _SizeInWords = ((int)((int)file_name - (int)(param_2 + 1)) >> 1) + 3;
      _Dst_00 = (ushort *)__calloc_base(_SizeInWords,2);
      if (_Dst_00 == (ushort *)0x0) {
        FID_conflict__free((void *)0x0);
        return (HANDLE)0xffffffff;
      }
      eVar5 = _wcscpy_s((wchar_t *)_Dst_00,_SizeInWords,L".\\");
      if ((eVar5 != 0) ||
         (eVar5 = _wcscat_s((wchar_t *)_Dst_00,_SizeInWords,(wchar_t *)param_2), eVar5 != 0))
      goto LAB_0040cac8;
      FID_conflict__free((void *)0x0);
      file_name = _Dst_00 + 2;
    }
  }
  else {
    if ((file_name == (ushort *)0x0) || (_Dst_00 = param_2, file_name < puVar4)) {
      _Dst_00 = param_2;
      file_name = puVar4;
    }
  }
  file_name = FUN_00402abe((char *)file_name,0x2e);
  if (file_name == (ushort *)0x0) {
    file_name = _Dst_00;
    do {
      uVar1 = *file_name;
      file_name = file_name + 1;
    } while (uVar1 != 0);
    iVar7 = (int)((int)file_name - (int)(_Dst_00 + 1)) >> 1;
    _SizeInWords = iVar7 + 5;
    _Dst = (wchar_t *)__calloc_base(_SizeInWords,2);
    if (_Dst == (wchar_t *)0x0) {
      pvVar6 = (HANDLE)0xffffffff;
LAB_0040cab3:
      FID_conflict__free(_Dst);
      goto LAB_0040caba;
    }
    eVar5 = _wcscpy_s(_Dst,_SizeInWords,(wchar_t *)_Dst_00);
    if (eVar5 != 0) {
LAB_0040cac8:
      uVar11 = 0;
      uVar10 = 0;
      uVar9 = 0;
      uVar8 = 0;
      FUN_00407e93();
      pcVar2 = (code *)swi(3);
      pvVar6 = (HANDLE)(*pcVar2)(uVar8,uVar9,uVar10,uVar11);
      return pvVar6;
    }
    puVar3 = (undefined4 *)FUN_00407f40();
    _Src = L".com";
    uVar8 = *puVar3;
    do {
      eVar5 = _wcscpy_s(_Dst + iVar7,5,_Src);
      if (eVar5 != 0) goto LAB_0040cac8;
      eVar5 = __waccess_s(_Dst,0);
      if (eVar5 == 0) {
        puVar3 = (undefined4 *)FUN_00407f40();
        *puVar3 = uVar8;
        pvVar6 = FUN_0040cad5(param_1,_Dst,param_3,param_4);
        goto LAB_0040cab3;
      }
      _Src = (wchar_t *)((int)_Src + 10);
    } while ((undefined **)_Src != &PTR_u_ja_JP_004191cc);
    FID_conflict__free(_Dst);
  }
  else {
    eVar5 = __waccess_s((wchar_t *)_Dst_00,0);
    if (eVar5 == 0) {
      pvVar6 = FUN_0040cad5(param_1,(LPCWSTR)_Dst_00,param_3,param_4);
      goto LAB_0040caba;
    }
  }
  pvVar6 = (HANDLE)0xffffffff;
LAB_0040caba:
  FID_conflict__free((void *)(-(uint)(param_2 != _Dst_00) & (uint)_Dst_00));
  return pvVar6;
}



void __cdecl FUN_0040d172(uint param_1,ushort *param_2,wchar_t **param_3,wint_t **param_4)

{
  ushort uVar1;
  wchar_t wVar2;
  int iVar3;
  code *pcVar4;
  bool bVar5;
  undefined4 *puVar6;
  int *piVar7;
  HANDLE pvVar8;
  int *piVar9;
  ushort *_Memory;
  errno_t eVar10;
  ushort *puVar11;
  ushort *puVar12;
  undefined4 uVar13;
  wchar_t *pwVar14;
  undefined4 uVar15;
  undefined4 uVar16;
  undefined4 uVar17;
  wchar_t local_20;
  undefined2 local_1e;
  wchar_t *local_1c;
  char local_15;
  wchar_t local_14;
  undefined2 local_12;
  undefined2 local_10;
  undefined2 local_e;
  undefined2 local_c;
  uint local_8;
  
  local_8 = DAT_0041e004 ^ (uint)&stack0xfffffffc;
  if ((((param_2 == (ushort *)0x0) || (*param_2 == 0)) || (param_3 == (wchar_t **)0x0)) ||
     (*param_3 == (wchar_t *)0x0)) {
    puVar6 = (undefined4 *)FUN_00407f40();
    *puVar6 = 0x16;
    FUN_00407e83();
  }
  else {
    wVar2 = **param_3;
    piVar7 = (int *)FUN_00407f40();
    if (wVar2 == L'\0') {
      *piVar7 = 0x16;
      FUN_00407e83();
    }
    else {
      iVar3 = *piVar7;
      *piVar7 = 0;
      pvVar8 = FUN_0040ce68(param_1,param_2,param_3,param_4);
      if (((pvVar8 == (HANDLE)0xffffffff) && (piVar9 = (int *)FUN_00407f40(), *piVar9 == 2)) &&
         ((_Memory = FUN_00402b60(param_2,0x5c), _Memory == (ushort *)0x0 &&
          ((_Memory = FUN_00402b60(param_2,0x2f), _Memory == (ushort *)0x0 && (param_2[1] != 0x3a)))
          ))) {
        local_14 = L'P';
        local_12 = 0x41;
        local_10 = 0x54;
        local_e = 0x48;
        local_c = 0;
        local_1c = (wchar_t *)0x0;
        eVar10 = FID_conflict___dupenv_s(&local_1c,(size_t *)0x0,&local_14);
        if (eVar10 == 0) {
          if (local_1c != (wchar_t *)0x0) {
            _Memory = (ushort *)__calloc_base(0x104,2);
            pwVar14 = local_1c;
            if (_Memory != (ushort *)0x0) {
              do {
                do {
                  pwVar14 = (wchar_t *)FUN_00413016(pwVar14,(short *)_Memory,0x103);
                  if ((pwVar14 == (wchar_t *)0x0) || (*_Memory == 0)) goto LAB_0040d12a;
                  puVar12 = _Memory;
                  do {
                    uVar1 = *puVar12;
                    puVar12 = puVar12 + 1;
                  } while (uVar1 != 0);
                  puVar11 = FUN_00402abe((char *)_Memory,0x5c);
                  if ((_Memory + ((int)((int)puVar12 - (int)(_Memory + 1)) >> 1) + -1 != puVar11) &&
                     (puVar11 = FUN_00402abe((char *)_Memory,0x2f),
                     _Memory + ((int)((int)puVar12 - (int)(_Memory + 1)) >> 1) + -1 != puVar11)) {
                    local_20 = L'\\';
                    local_1e = 0;
                    eVar10 = _wcscat_s((wchar_t *)_Memory,0x104,&local_20);
                    if (eVar10 != 0) goto LAB_0040d165;
                  }
                  puVar12 = _Memory;
                  do {
                    uVar1 = *puVar12;
                    puVar12 = puVar12 + 1;
                  } while (uVar1 != 0);
                  puVar11 = param_2;
                  do {
                    uVar1 = *puVar11;
                    puVar11 = puVar11 + 1;
                  } while (uVar1 != 0);
                  if (0x103 < (uint)(((int)((int)puVar11 - (int)(param_2 + 1)) >> 1) +
                                    ((int)((int)puVar12 - (int)(_Memory + 1)) >> 1)))
                  goto LAB_0040d12a;
                  eVar10 = _wcscat_s((wchar_t *)_Memory,0x104,(wchar_t *)param_2);
                  if (eVar10 != 0) goto LAB_0040d165;
                  puVar6 = (undefined4 *)FUN_00407f40();
                  *puVar6 = 0;
                  pvVar8 = FUN_0040ce68(param_1,_Memory,param_3,param_4);
                  if (pvVar8 != (HANDLE)0xffffffff) goto LAB_0040d12a;
                  piVar9 = (int *)FUN_00407f40();
                } while ((*piVar9 == 2) || (piVar9 = (int *)FUN_00407f2d(), *piVar9 == 0x15));
                puVar12 = _Memory + 1;
                puVar11 = FUN_00402b60(_Memory,0x2f);
                if ((puVar11 == _Memory) &&
                   (puVar11 = FUN_00402b60(puVar12,0x2f), puVar11 == puVar12)) {
                  local_15 = 1;
                }
                else {
                  local_15 = 0;
                }
                puVar11 = FUN_00402b60(_Memory,0x5c);
                if ((puVar11 == _Memory) &&
                   (puVar11 = FUN_00402b60(puVar12,0x5c), puVar11 == puVar12)) {
                  bVar5 = true;
                }
                else {
                  bVar5 = false;
                }
              } while ((local_15 != 0) || (bVar5));
            }
LAB_0040d12a:
            FID_conflict__free(_Memory);
          }
        }
        else {
          if (eVar10 == 0x16) {
LAB_0040d165:
            uVar17 = 0;
            uVar16 = 0;
            uVar15 = 0;
            uVar13 = 0;
            FUN_00407e93();
            pcVar4 = (code *)swi(3);
            (*pcVar4)(uVar13,uVar15,uVar16,uVar17);
            return;
          }
        }
        FID_conflict__free(local_1c);
        local_1c = (wchar_t *)0x0;
      }
      if ((*piVar7 == 0) && (iVar3 != 0)) {
        *piVar7 = iVar3;
      }
    }
  }
  FUN_00401f78();
  return;
}



// Library Function - Single Match
// Name: __waccess_s
// Library: Visual Studio 2015 Release

errno_t __cdecl __waccess_s(wchar_t *_Filename,int _AccessMode)

{
  undefined4 *puVar1;
  BOOL BVar2;
  DWORD DVar3;
  errno_t eVar4;
  byte local_2c [36];
  uint local_8;
  
  local_8 = DAT_0041e004 ^ (uint)&stack0xfffffffc;
  if (_Filename == (wchar_t *)0x0) {
    puVar1 = (undefined4 *)FUN_00407f2d();
    *puVar1 = 0;
    puVar1 = (undefined4 *)FUN_00407f40();
    *puVar1 = 0x16;
    FUN_00407e83();
  }
  else {
    if ((_AccessMode & 0xfffffff9U) == 0) {
      BVar2 = GetFileAttributesExW(_Filename,GetFileExInfoStandard,local_2c);
      if (BVar2 == 0) {
        DVar3 = GetLastError();
        ___acrt_errno_map_os_error(DVar3);
      }
      else {
        if ((((local_2c[0] & 0x10) != 0) || ((local_2c[0] & 1) == 0)) ||
           (((uint)_AccessMode >> 1 & 1) == 0)) goto LAB_0040d22e;
        puVar1 = (undefined4 *)FUN_00407f2d();
        *puVar1 = 5;
        puVar1 = (undefined4 *)FUN_00407f40();
        *puVar1 = 0xd;
      }
      FUN_00407f40();
    }
    else {
      puVar1 = (undefined4 *)FUN_00407f2d();
      *puVar1 = 0;
      puVar1 = (undefined4 *)FUN_00407f40();
      *puVar1 = 0x16;
      FUN_00407e83();
    }
  }
LAB_0040d22e:
  eVar4 = FUN_00401f78();
  return eVar4;
}



// Library Function - Single Match
// Name: _wcscat_s
// Library: Visual Studio 2015 Release

errno_t __cdecl _wcscat_s(wchar_t *_Dst,rsize_t _SizeInWords,wchar_t *_Src)

{
  wchar_t wVar1;
  errno_t *peVar2;
  wchar_t *pwVar3;
  errno_t eStack20;
  
  if ((_Dst != (wchar_t *)0x0) && (_SizeInWords != 0)) {
    pwVar3 = _Dst;
    if (_Src != (wchar_t *)0x0) {
      do {
        if (*pwVar3 == L'\0') break;
        pwVar3 = pwVar3 + 1;
        _SizeInWords = _SizeInWords - 1;
      } while (_SizeInWords != 0);
      if (_SizeInWords != 0) {
        _Src = (wchar_t *)((int)_Src - (int)pwVar3);
        do {
          wVar1 = *(wchar_t *)((int)_Src + (int)pwVar3);
          *pwVar3 = wVar1;
          pwVar3 = pwVar3 + 1;
          if (wVar1 == L'\0') break;
          _SizeInWords = _SizeInWords - 1;
        } while (_SizeInWords != 0);
        if (_SizeInWords != 0) {
          return 0;
        }
        *_Dst = L'\0';
        peVar2 = (errno_t *)FUN_00407f40();
        eStack20 = 0x22;
        goto LAB_0040d266;
      }
    }
    *_Dst = L'\0';
  }
  peVar2 = (errno_t *)FUN_00407f40();
  eStack20 = 0x16;
LAB_0040d266:
  *peVar2 = eStack20;
  FUN_00407e83();
  return eStack20;
}



// Library Function - Single Match
// Name: _wcscpy_s
// Library: Visual Studio 2015 Release

errno_t __cdecl _wcscpy_s(wchar_t *_Dst,rsize_t _SizeInWords,wchar_t *_Src)

{
  wchar_t wVar1;
  errno_t *peVar2;
  wchar_t *pwVar3;
  errno_t eVar4;
  
  if ((_Dst != (wchar_t *)0x0) && (_SizeInWords != 0)) {
    if (_Src != (wchar_t *)0x0) {
      pwVar3 = _Dst;
      do {
        wVar1 = *(wchar_t *)((int)((int)_Src - (int)_Dst) + (int)pwVar3);
        *pwVar3 = wVar1;
        pwVar3 = pwVar3 + 1;
        if (wVar1 == L'\0') break;
        _SizeInWords = _SizeInWords - 1;
      } while (_SizeInWords != 0);
      if (_SizeInWords != 0) {
        return 0;
      }
      *_Dst = L'\0';
      peVar2 = (errno_t *)FUN_00407f40();
      eVar4 = 0x22;
      goto LAB_0040d2d9;
    }
    *_Dst = L'\0';
  }
  peVar2 = (errno_t *)FUN_00407f40();
  eVar4 = 0x16;
LAB_0040d2d9:
  *peVar2 = eVar4;
  FUN_00407e83();
  return eVar4;
}



undefined4 __cdecl FUN_0040d316(short *param_1,int param_2,int param_3,int param_4)

{
  short sVar1;
  undefined4 *puVar2;
  int iVar3;
  short *psVar4;
  int iVar5;
  undefined4 uVar6;
  
  if (param_4 == 0) {
    if (param_1 == (short *)0x0) {
      if (param_2 == 0) {
        return 0;
      }
    }
    else {
LAB_0040d34c:
      if (param_2 != 0) {
        if (param_4 == 0) {
          *param_1 = 0;
          return 0;
        }
        if (param_3 != 0) {
          iVar3 = param_4;
          psVar4 = param_1;
          iVar5 = param_2;
          if (param_4 == -1) {
            do {
              sVar1 = *(short *)((param_3 - (int)param_1) + (int)psVar4);
              *psVar4 = sVar1;
              psVar4 = psVar4 + 1;
              if (sVar1 == 0) break;
              iVar5 = iVar5 + -1;
            } while (iVar5 != 0);
          }
          else {
            do {
              sVar1 = *(short *)((param_3 - (int)param_1) + (int)psVar4);
              *psVar4 = sVar1;
              psVar4 = psVar4 + 1;
              if ((sVar1 == 0) || (iVar5 = iVar5 + -1, iVar5 == 0)) break;
              iVar3 = iVar3 + -1;
            } while (iVar3 != 0);
            if (iVar3 == 0) {
              *psVar4 = 0;
            }
          }
          if (iVar5 != 0) {
            return 0;
          }
          if (param_4 == -1) {
            param_1[param_2 + -1] = 0;
            return 0x50;
          }
          *param_1 = 0;
          puVar2 = (undefined4 *)FUN_00407f40();
          uVar6 = 0x22;
          goto LAB_0040d33a;
        }
        *param_1 = 0;
      }
    }
  }
  else {
    if (param_1 != (short *)0x0) goto LAB_0040d34c;
  }
  puVar2 = (undefined4 *)FUN_00407f40();
  uVar6 = 0x16;
LAB_0040d33a:
  *puVar2 = uVar6;
  FUN_00407e83();
  return uVar6;
}



void __cdecl FUN_0040d3e1(short *param_1,int param_2,int param_3,int param_4)

{
  FUN_0040d316(param_1,param_2,param_3,param_4);
  return;
}



uint __cdecl FUN_0040d3ec(uint param_1,uint param_2)

{
  if (param_2 >= param_1 && param_2 != param_1) {
    return 0xffffffff;
  }
  return (uint)(param_2 < param_1);
}



void __cdecl FUN_0040d404(LPCWSTR *param_1,LPCWSTR *param_2)

{
  LPCWSTR pWVar1;
  WCHAR WVar2;
  LPCWSTR pWVar3;
  code *pcVar4;
  undefined4 *puVar5;
  LPCWSTR pWVar6;
  int iVar7;
  LPCWSTR pWVar8;
  uint uVar9;
  LPCWSTR *ppWVar10;
  LPCWSTR *_Memory;
  uint uVar11;
  LPCWSTR pWVar12;
  undefined4 uVar13;
  undefined4 uVar14;
  undefined4 uVar15;
  undefined4 uVar16;
  LPCWSTR *local_30;
  LPCWSTR *local_2c;
  undefined4 local_28;
  LPCWSTR *local_24;
  LPCWSTR local_20;
  LPCWSTR local_1c;
  LPCWSTR *local_18;
  uint local_14;
  LPCWSTR local_10;
  undefined2 local_c;
  uint local_8;
  
  local_8 = DAT_0041e004 ^ (uint)&stack0xfffffffc;
  local_24 = param_2;
  local_18 = param_1;
  if (param_2 == (LPCWSTR *)0x0) {
    puVar5 = (undefined4 *)FUN_00407f40();
    *puVar5 = 0x16;
    FUN_00407e83();
  }
  else {
    pWVar12 = (LPCWSTR)0x0;
    *param_2 = (LPCWSTR)0x0;
    pWVar6 = *param_1;
    local_30 = (LPCWSTR *)0x0;
    local_2c = (LPCWSTR *)0x0;
    local_28 = 0;
    local_24 = param_2;
    ppWVar10 = local_30;
    while (pWVar6 != (LPCWSTR)0x0) {
      local_10 = (LPCWSTR)0x3f002a;
      local_c = 0;
      local_30 = ppWVar10;
      local_18 = param_1;
      pWVar6 = FUN_0040eb65(pWVar6,(short *)&local_10);
      if (pWVar6 == (LPCWSTR)0x0) {
        local_14 = FUN_0040d632(*param_1,0,0,&local_30);
      }
      else {
        local_14 = FUN_0040d6e8(*param_1,pWVar6,&local_30);
      }
      ppWVar10 = local_30;
      if (local_14 != 0) goto LAB_0040d5d7;
      param_1 = local_18 + 1;
      pWVar6 = *param_1;
      ppWVar10 = local_30;
    }
    local_14 = 0;
    local_20 = (LPCWSTR)(((int)(LPCWSTR *)((int)local_2c - (int)ppWVar10) >> 2) + 1);
    uVar11 = ~-(uint)(local_2c < ppWVar10) &
             (int)(LPCWSTR *)((int)local_2c - (int)ppWVar10) + 3U >> 2;
    local_18 = ppWVar10;
    if (uVar11 != 0) {
      uVar9 = 0;
      local_18 = ppWVar10;
      do {
        pWVar6 = *local_18;
        local_1c = pWVar6 + 1;
        do {
          WVar2 = *pWVar6;
          pWVar6 = pWVar6 + 1;
        } while (WVar2 != L'\0');
        local_14 = local_14 + 1 + ((int)((int)pWVar6 - (int)local_1c) >> 1);
        local_18 = local_18 + 1;
        uVar9 = uVar9 + 1;
      } while (uVar9 != uVar11);
    }
    local_30 = ppWVar10;
    pWVar6 = (LPCWSTR)FUN_004091cc((uint)local_20,local_14,2);
    if (pWVar6 == (LPCWSTR)0x0) {
      local_14 = -1;
    }
    else {
      pWVar1 = pWVar6 + (int)local_20 * 2;
      local_20 = pWVar1;
      local_18 = ppWVar10;
      if (ppWVar10 != local_2c) {
        local_10 = (LPCWSTR)((int)pWVar6 - (int)ppWVar10);
        local_18 = ppWVar10;
        local_20 = pWVar1;
        do {
          pWVar3 = *local_18;
          pWVar8 = pWVar3;
          do {
            WVar2 = *pWVar8;
            pWVar8 = pWVar8 + 1;
          } while (WVar2 != L'\0');
          local_1c = (LPCWSTR)(((int)((int)pWVar8 - (int)(pWVar3 + 1)) >> 1) + 1);
          iVar7 = FUN_0040d3e1(local_20,local_14 - ((int)((int)local_20 - (int)pWVar1) >> 1),
                               (int)pWVar3,(int)local_1c);
          if (iVar7 != 0) {
            uVar16 = 0;
            uVar15 = 0;
            uVar14 = 0;
            uVar13 = 0;
            FUN_00407e93();
            pcVar4 = (code *)swi(3);
            (*pcVar4)(uVar13,uVar14,uVar15,uVar16);
            return;
          }
          *(LPCWSTR *)((int)local_10 + (int)local_18) = local_20;
          local_18 = local_18 + 1;
          local_20 = local_20 + (int)local_1c;
        } while (local_18 != local_2c);
      }
      local_14 = 0;
      *local_24 = pWVar6;
    }
    FID_conflict__free((void *)0x0);
LAB_0040d5d7:
    pWVar6 = (LPCWSTR)(~-(uint)(local_2c < ppWVar10) &
                      (uint)((int)local_2c + (3 - (int)ppWVar10)) >> 2);
    _Memory = ppWVar10;
    local_24 = ppWVar10;
    local_10 = pWVar6;
    if (pWVar6 != (LPCWSTR)0x0) {
      do {
        FID_conflict__free(*ppWVar10);
        pWVar12 = (LPCWSTR)((int)pWVar12 + 1);
        ppWVar10 = ppWVar10 + 1;
        _Memory = local_30;
      } while (pWVar12 != pWVar6);
    }
    FID_conflict__free(_Memory);
  }
  FUN_00401f78();
  return;
}



int __cdecl FUN_0040d632(short *param_1,int param_2,uint param_3,void **param_4)

{
  short sVar1;
  code *pcVar2;
  int iVar3;
  short *_Memory;
  uint uVar4;
  uint uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  
  _Memory = param_1;
  do {
    sVar1 = *_Memory;
    _Memory = _Memory + 1;
  } while (sVar1 != 0);
  uVar4 = ((int)((int)_Memory - (int)(param_1 + 1)) >> 1) + 1;
  if (~param_3 < uVar4) {
    iVar3 = 0xc;
  }
  else {
    uVar5 = param_3 + 1 + uVar4;
    _Memory = (short *)__calloc_base(uVar5,2);
    if (param_3 != 0) {
      iVar3 = FUN_0040d3e1(_Memory,uVar5,param_2,param_3);
      if (iVar3 != 0) goto LAB_0040d6db;
    }
    iVar3 = FUN_0040d3e1(_Memory + param_3,uVar5 - param_3,(int)param_1,uVar4);
    if (iVar3 != 0) {
LAB_0040d6db:
      uVar9 = 0;
      uVar8 = 0;
      uVar7 = 0;
      uVar6 = 0;
      FUN_00407e93();
      pcVar2 = (code *)swi(3);
      iVar3 = (*pcVar2)(uVar6,uVar7,uVar8,uVar9);
      return iVar3;
    }
    iVar3 = FUN_0040d880(param_4);
    if (iVar3 == 0) {
      *(short **)param_4[1] = _Memory;
      param_4[1] = (void *)((int)param_4[1] + 4);
      iVar3 = 0;
    }
    else {
      FID_conflict__free(_Memory);
    }
    FID_conflict__free((void *)0x0);
  }
  return iVar3;
}



void __cdecl FUN_0040d6e8(LPCWSTR param_1,LPCWSTR param_2,void **param_3)

{
  char cVar1;
  HANDLE hFindFile;
  int iVar2;
  BOOL BVar3;
  int iVar4;
  LPCWSTR extraout_EDX;
  ulonglong uVar5;
  _WIN32_FIND_DATAW local_258;
  uint local_8;
  
  local_8 = DAT_0041e004 ^ (uint)&stack0xfffffffc;
  if (param_2 != param_1) {
    do {
      cVar1 = FUN_0040d85c(*param_2);
      param_2 = extraout_EDX;
      if (cVar1 != 0) break;
      param_2 = extraout_EDX + -1;
    } while (param_2 != param_1);
  }
  if ((*param_2 == L':') && (param_2 != param_1 + 1)) {
    FUN_0040d632(param_1,0,0,param_3);
  }
  else {
    uVar5 = FUN_0040d85c(*param_2);
    hFindFile = FindFirstFileExW(param_1,FindExInfoStandard,&local_258,FindExSearchNameMatch,
                                 (LPVOID)0x0,0);
    if (hFindFile == (HANDLE)0xffffffff) {
      FUN_0040d632(param_1,0,0,param_3);
    }
    else {
      iVar4 = (int)((int)param_3[1] - (int)*param_3) >> 2;
      do {
        if (((local_258.cFileName[0] != L'.') ||
            ((local_258.cFileName[1] != L'\0' &&
             ((local_258.cFileName[1] != L'.' || (local_258.cFileName[2] != L'\0')))))) &&
           (iVar2 = FUN_0040d632(local_258.cFileName,(int)param_1,
                                 -(uint)((uVar5 & 0xff) != 0) &
                                 ((int)(uVar5 >> 0x20) - (int)param_1 >> 1) + 1U,param_3),
           iVar2 != 0)) goto LAB_0040d842;
        BVar3 = FindNextFileW(hFindFile,(LPWIN32_FIND_DATAW)&local_258);
      } while (BVar3 != 0);
      iVar2 = (int)((int)param_3[1] - (int)*param_3) >> 2;
      if (iVar4 != iVar2) {
        FUN_00413070((char *)((int)*param_3 + iVar4 * 4),iVar2 - iVar4,4,FUN_0040d3ec);
      }
LAB_0040d842:
      FindClose(hFindFile);
    }
  }
  FUN_00401f78();
  return;
}



undefined FUN_0040d85c(short param_1)

{
  undefined uVar1;
  
  if (((param_1 == 0x2f) || (param_1 == 0x5c)) || (param_1 == 0x3a)) {
    uVar1 = 1;
  }
  else {
    uVar1 = 0;
  }
  return uVar1;
}



undefined4 __fastcall FUN_0040d880(void **param_1)

{
  void *pvVar1;
  undefined4 uVar2;
  LPVOID pvVar3;
  uint uVar4;
  
  if (param_1[1] == param_1[2]) {
    if (*param_1 == (void *)0x0) {
      pvVar3 = __calloc_base(4,4);
      *param_1 = pvVar3;
      FID_conflict__free((void *)0x0);
      pvVar1 = *param_1;
      if (pvVar1 != (void *)0x0) {
        param_1[1] = pvVar1;
        param_1[2] = (void *)((int)pvVar1 + 0x10);
        goto LAB_0040d88e;
      }
    }
    else {
      uVar4 = (int)((int)param_1[2] - (int)*param_1) >> 2;
      if (uVar4 < 0x80000000) {
        pvVar3 = __recalloc_base(*param_1,uVar4 * 2,4);
        if (pvVar3 == (LPVOID)0x0) {
          uVar2 = 0xc;
        }
        else {
          *param_1 = pvVar3;
          param_1[1] = (LPVOID)((int)pvVar3 + uVar4 * 4);
          param_1[2] = (LPVOID)((int)pvVar3 + uVar4 * 8);
          uVar2 = 0;
        }
        FID_conflict__free((void *)0x0);
        return uVar2;
      }
    }
    uVar2 = 0xc;
  }
  else {
LAB_0040d88e:
    uVar2 = 0;
  }
  return uVar2;
}



void __cdecl FUN_0040d907(LPCWSTR *param_1,LPCWSTR *param_2)

{
  FUN_0040d404(param_1,param_2);
  return;
}



// WARNING: Function: __SEH_prolog4 replaced with injection: SEH_prolog4
// WARNING: Function: __SEH_epilog4 replaced with injection: EH_epilog3
// Library Function - Multiple Matches With Same Base Name
// Name: operator()<>
// Library: Visual Studio 2015 Release

void operator____(int *param_1,int **param_2)

{
  ___acrt_lock(*param_1);
  FUN_0040d955(param_2);
  FUN_0040d949();
  return;
}



void FUN_0040d949(void)

{
  int unaff_EBP;
  
  ___acrt_unlock(**(int **)(unaff_EBP + 0x10));
  return;
}



void __fastcall FUN_0040d955(int **param_1)

{
  int iVar1;
  
  _memcpy_s(DAT_0041f2a0,0x101,(void *)(*(int *)(**param_1 + 0x48) + 0x18),0x101);
  _memcpy_s(DAT_0041f2a4,0x100,(void *)(*(int *)(**param_1 + 0x48) + 0x119),0x100);
  LOCK();
  iVar1 = **(int **)*param_1[1] + -1;
  **(int **)*param_1[1] = iVar1;
  if ((iVar1 == 0) && (*(undefined **)(void **)*param_1[1] != &DAT_0041e208)) {
    FID_conflict__free(*(void **)*param_1[1]);
  }
  *(undefined4 *)*param_1[1] = *(undefined4 *)(**param_1 + 0x48);
  LOCK();
  **(int **)(**param_1 + 0x48) = **(int **)(**param_1 + 0x48) + 1;
  return;
}



// Library Function - Single Match
// Name: ?CPtoLocaleName@@YAPB_WH@Z
// Library: Visual Studio 2015 Release
// wchar_t const * __cdecl CPtoLocaleName(int)

wchar_t * __cdecl CPtoLocaleName(int param_1)

{
  if (param_1 == 0x3a4) {
    return (wchar_t *)L"ja-JP";
  }
  if (param_1 == 0x3a8) {
    return (wchar_t *)L"zh-CN";
  }
  if (param_1 == 0x3b5) {
    return (wchar_t *)L"ko-KR";
  }
  if (param_1 != 0x3b6) {
    return (wchar_t *)0x0;
  }
  return (wchar_t *)L"zh-TW";
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address
// Library Function - Single Match
// Name: ?getSystemCP@@YAHH@Z
// Library: Visual Studio 2015 Release
// int __cdecl getSystemCP(int)

int __cdecl getSystemCP(int param_1)

{
  int local_14;
  int local_10;
  char local_8;
  
  FUN_00405ed6(&local_14,(undefined4 *)0x0);
  _DAT_0041f2a8 = 0;
  if (param_1 == -2) {
    _DAT_0041f2a8 = 1;
    param_1 = GetOEMCP();
  }
  else {
    if (param_1 == -3) {
      _DAT_0041f2a8 = 1;
      param_1 = GetACP();
    }
    else {
      if (param_1 == -4) {
        _DAT_0041f2a8 = 1;
        param_1 = *(UINT *)(local_10 + 8);
      }
    }
  }
  if (local_8 != 0) {
    *(uint *)(local_14 + 0x350) = *(uint *)(local_14 + 0x350) & 0xfffffffd;
  }
  return param_1;
}



// Library Function - Single Match
// Name: ?setSBCS@@YAXPAU__crt_multibyte_data@@@Z
// Library: Visual Studio 2015 Release
// void __cdecl setSBCS(struct __crt_multibyte_data *)

void __cdecl setSBCS(__crt_multibyte_data *param_1)

{
  int iVar1;
  __crt_multibyte_data *p_Var2;
  int *piVar3;
  
  piVar3 = (int *)(param_1 + 0x18);
  FUN_00402e50(piVar3,0,0x101);
  *(undefined4 *)(param_1 + 4) = 0;
  *(undefined4 *)(param_1 + 8) = 0;
  *(undefined4 *)(param_1 + 0x21c) = 0;
  iVar1 = 0x101;
  *(undefined4 *)(param_1 + 0xc) = 0;
  *(undefined4 *)(param_1 + 0x10) = 0;
  *(undefined4 *)(param_1 + 0x14) = 0;
  do {
    *(undefined *)piVar3 = (&DAT_0041e208 + -(int)param_1)[(int)piVar3];
    piVar3 = (int *)((int)piVar3 + 1);
    iVar1 = iVar1 + -1;
  } while (iVar1 != 0);
  p_Var2 = param_1 + 0x119;
  iVar1 = 0x100;
  do {
    *p_Var2 = p_Var2[(int)(&DAT_0041e208 + -(int)param_1)];
    p_Var2 = p_Var2 + 1;
    iVar1 = iVar1 + -1;
  } while (iVar1 != 0);
  return;
}



void __cdecl FUN_0040daee(int param_1)

{
  byte bVar1;
  char cVar2;
  BOOL BVar3;
  uint uVar4;
  undefined uVar5;
  byte *pbVar6;
  _cpinfo local_71c;
  WORD local_708 [512];
  WCHAR local_308 [128];
  WCHAR local_208 [128];
  CHAR local_108 [256];
  uint local_8;
  
  local_8 = DAT_0041e004 ^ (uint)&stack0xfffffffc;
  if ((*(int *)(param_1 + 4) == 0xfde9) ||
     (BVar3 = GetCPInfo(*(UINT *)(param_1 + 4),(LPCPINFO)&local_71c), BVar3 == 0)) {
    uVar4 = 0;
    do {
      if (uVar4 - 0x41 < 0x1a) {
        pbVar6 = (byte *)(param_1 + 0x19 + uVar4);
        *pbVar6 = *pbVar6 | 0x10;
        cVar2 = (char)uVar4 + ' ';
      }
      else {
        if (uVar4 - 0x61 < 0x1a) {
          pbVar6 = (byte *)(param_1 + 0x19 + uVar4);
          *pbVar6 = *pbVar6 | 0x20;
          cVar2 = (char)uVar4 + -0x20;
        }
        else {
          cVar2 = 0;
        }
      }
      *(char *)(param_1 + 0x119 + uVar4) = cVar2;
      uVar4 = uVar4 + 1;
    } while (uVar4 < 0x100);
  }
  else {
    uVar4 = 0;
    do {
      local_108[uVar4] = (CHAR)uVar4;
      uVar4 = uVar4 + 1;
    } while (uVar4 < 0x100);
    pbVar6 = local_71c.LeadByte;
    local_108[0] = ' ';
    while (local_71c.LeadByte[0] != 0) {
      bVar1 = pbVar6[1];
      uVar4 = (uint)local_71c.LeadByte[0];
      while ((uVar4 <= (uint)bVar1 && (uVar4 < 0x100))) {
        local_108[uVar4] = ' ';
        uVar4 = uVar4 + 1;
      }
      pbVar6 = pbVar6 + 2;
      local_71c.LeadByte[0] = *pbVar6;
    }
    FUN_0040ebb3((undefined4 *)0x0,1,local_108,0x100,local_708,*(uint *)(param_1 + 4),0);
    ___acrt_LCMapStringA
              ((undefined4 *)0x0,*(int *)(param_1 + 0x21c),0x100,local_108,0x100,local_208,0x100,
               *(uint *)(param_1 + 4),0);
    ___acrt_LCMapStringA
              ((undefined4 *)0x0,*(int *)(param_1 + 0x21c),0x200,local_108,0x100,local_308,0x100,
               *(uint *)(param_1 + 4),0);
    uVar4 = 0;
    do {
      if ((local_708[uVar4] & 1) == 0) {
        if ((local_708[uVar4] & 2) == 0) {
          uVar5 = 0;
        }
        else {
          pbVar6 = (byte *)(param_1 + 0x19 + uVar4);
          *pbVar6 = *pbVar6 | 0x20;
          uVar5 = *(undefined *)((int)local_308 + uVar4);
        }
      }
      else {
        pbVar6 = (byte *)(param_1 + 0x19 + uVar4);
        *pbVar6 = *pbVar6 | 0x10;
        uVar5 = *(undefined *)((int)local_208 + uVar4);
      }
      *(undefined *)(param_1 + 0x119 + uVar4) = uVar5;
      uVar4 = uVar4 + 1;
    } while (uVar4 < 0x100);
  }
  FUN_00401f78();
  return;
}



int __cdecl FUN_0040dc71(int param_1,char param_2,int param_3,int **param_4)

{
  int *piVar1;
  int iVar2;
  undefined4 *puVar3;
  int iVar4;
  undefined4 *puVar5;
  undefined4 *_Memory;
  int *local_18;
  int ***local_14;
  int local_10;
  int local_c [2];
  
  FUN_0040dd8c(param_3,param_4);
  local_10 = getSystemCP(param_1);
  if (local_10 == *(int *)(*(int *)(param_3 + 0x48) + 4)) {
    return 0;
  }
  puVar3 = (undefined4 *)__malloc_base(0x220);
  if (puVar3 != (undefined4 *)0x0) {
    iVar4 = 0x88;
    _Memory = *(undefined4 **)(param_3 + 0x48);
    puVar5 = puVar3;
    while (iVar4 != 0) {
      iVar4 = iVar4 + -1;
      *puVar5 = *_Memory;
      _Memory = _Memory + 1;
      puVar5 = puVar5 + 1;
    }
    *puVar3 = 0;
    iVar4 = FUN_0040de7d(local_10,(__crt_multibyte_data *)puVar3);
    if (iVar4 != -1) {
      if (param_2 == 0) {
        FUN_004098ec();
      }
      piVar1 = *(int **)(param_3 + 0x48);
      LOCK();
      iVar2 = *piVar1;
      *piVar1 = *piVar1 + -1;
      if ((iVar2 == 1) && (*(undefined **)(param_3 + 0x48) != &DAT_0041e208)) {
        FID_conflict__free(*(void **)(param_3 + 0x48));
      }
      *puVar3 = 1;
      _Memory = (undefined4 *)0x0;
      *(undefined4 **)(param_3 + 0x48) = puVar3;
      if (((*(byte *)(param_3 + 0x350) & 2) == 0) && (((byte)DAT_0041e780 & 1) == 0)) {
        local_18 = &param_3;
        local_14 = &param_4;
        local_10 = 5;
        local_c[0] = 5;
        operator____(local_c,&local_18);
        if (param_2 != 0) {
          PTR_DAT_0041e1fc = (undefined *)*param_4;
        }
      }
      goto LAB_0040dce8;
    }
    _Memory = (undefined4 *)FUN_00407f40();
    *_Memory = 0x16;
  }
  iVar4 = -1;
  _Memory = puVar3;
LAB_0040dce8:
  FID_conflict__free(_Memory);
  return iVar4;
}



// WARNING: Function: __SEH_prolog4 replaced with injection: SEH_prolog4
// WARNING: Function: __SEH_epilog4 replaced with injection: EH_epilog3

int * __cdecl FUN_0040dd8c(int param_1,int **param_2)

{
  int iVar1;
  code *pcVar2;
  int *_Memory;
  
  if (((*(uint *)(param_1 + 0x350) & DAT_0041e780) == 0) || (*(int *)(param_1 + 0x4c) == 0)) {
    ___acrt_lock(5);
    _Memory = *(int **)(param_1 + 0x48);
    if (_Memory != *param_2) {
      if (_Memory != (int *)0x0) {
        LOCK();
        iVar1 = *_Memory;
        *_Memory = iVar1 + -1;
        if ((iVar1 + -1 == 0) && (_Memory != (int *)&DAT_0041e208)) {
          FID_conflict__free(_Memory);
        }
      }
      _Memory = *param_2;
      *(int **)(param_1 + 0x48) = _Memory;
      LOCK();
      *_Memory = *_Memory + 1;
    }
    FUN_0040de0b();
  }
  else {
    _Memory = *(int **)(param_1 + 0x48);
  }
  if (_Memory != (int *)0x0) {
    return _Memory;
  }
  FUN_00409f3e();
  pcVar2 = (code *)swi(3);
  _Memory = (int *)(*pcVar2)();
  return _Memory;
}



void FUN_0040de0b(void)

{
  ___acrt_unlock(5);
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 FUN_0040de22(void)

{
  int in_EAX;
  undefined4 *puVar1;
  
  if (DAT_0041f2ac == 0) {
    _DAT_0041f29c = &DAT_0041e208;
    DAT_0041f2a4 = &DAT_0041e530;
    DAT_0041f2a0 = &DAT_0041e428;
    puVar1 = FUN_0040b684();
    in_EAX = FUN_0040dc71(-3,1,(int)puVar1,(int **)&DAT_0041f29c);
    DAT_0041f2ac = 1;
  }
  return CONCAT31((int3)((uint)in_EAX >> 8),1);
}



void FUN_0040de6a(void)

{
  uint uVar1;
  int **ppiVar2;
  
  ppiVar2 = (int **)&DAT_0041f29c;
  uVar1 = FUN_0040b5c7();
  FUN_0040dd8c(uVar1,ppiVar2);
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void __cdecl FUN_0040de7d(int param_1,__crt_multibyte_data *param_2)

{
  byte bVar1;
  undefined2 uVar2;
  uint CodePage;
  uint uVar3;
  BOOL BVar4;
  byte *pbVar5;
  __crt_multibyte_data *p_Var6;
  wchar_t *pwVar7;
  int iVar8;
  byte *pbVar9;
  undefined2 *puVar10;
  undefined2 *puVar11;
  uint uVar12;
  int local_20;
  _cpinfo local_1c;
  uint local_8;
  
  local_8 = DAT_0041e004 ^ (uint)&stack0xfffffffc;
  CodePage = getSystemCP(param_1);
  if (CodePage != 0) {
    uVar12 = 0;
    uVar3 = 0;
    local_20 = 0;
LAB_0040deb1:
    if (*(uint *)((int)&DAT_0041e638 + uVar3) != CodePage) goto code_r0x0040debd;
    FUN_00402e50((int *)(param_2 + 0x18),0,0x101);
    pbVar5 = (byte *)(local_20 * 0x30 + 0x41e648);
    do {
      bVar1 = *pbVar5;
      pbVar9 = pbVar5;
      while ((bVar1 != 0 && (bVar1 = pbVar9[1], bVar1 != 0))) {
        uVar3 = (uint)*pbVar9;
        while ((uVar3 <= (uint)bVar1 && (uVar3 < 0x100))) {
          *(byte *)(param_2 + uVar3 + 0x19) = (byte)param_2[uVar3 + 0x19] | (&DAT_0041e630)[uVar12];
          uVar3 = uVar3 + 1;
          bVar1 = pbVar9[1];
        }
        pbVar9 = pbVar9 + 2;
        bVar1 = *pbVar9;
      }
      uVar12 = uVar12 + 1;
      pbVar5 = pbVar5 + 8;
    } while (uVar12 < 4);
    *(uint *)(param_2 + 4) = CodePage;
    *(undefined4 *)(param_2 + 8) = 1;
    pwVar7 = CPtoLocaleName(CodePage);
    *(wchar_t **)(param_2 + 0x21c) = pwVar7;
    puVar10 = (undefined2 *)(param_2 + 0xc);
    puVar11 = (undefined2 *)(local_20 * 0x30 + 0x41e63c);
    iVar8 = 6;
    do {
      uVar2 = *puVar11;
      puVar11 = puVar11 + 1;
      *puVar10 = uVar2;
      puVar10 = puVar10 + 1;
      iVar8 = iVar8 + -1;
    } while (iVar8 != 0);
    goto LAB_0040df0d;
  }
LAB_0040e058:
  setSBCS(param_2);
LAB_0040e061:
  FUN_00401f78();
  return;
code_r0x0040debd:
  local_20 = local_20 + 1;
  uVar3 = uVar3 + 0x30;
  if (0xef < uVar3) goto code_r0x0040decb;
  goto LAB_0040deb1;
code_r0x0040decb:
  if ((CodePage == 65000) || (BVar4 = IsValidCodePage(CodePage & 0xffff), BVar4 == 0))
  goto LAB_0040e061;
  if (CodePage == 0xfde9) {
    *(undefined4 *)(param_2 + 4) = 0xfde9;
    *(undefined4 *)(param_2 + 0x21c) = 0;
    *(undefined4 *)(param_2 + 0x18) = 0;
    *(undefined2 *)(param_2 + 0x1c) = 0;
  }
  else {
    BVar4 = GetCPInfo(CodePage,(LPCPINFO)&local_1c);
    if (BVar4 == 0) {
      if (_DAT_0041f2a8 == 0) goto LAB_0040e061;
      goto LAB_0040e058;
    }
    FUN_00402e50((int *)(param_2 + 0x18),0,0x101);
    *(uint *)(param_2 + 4) = CodePage;
    *(undefined4 *)(param_2 + 0x21c) = 0;
    if (local_1c.MaxCharSize == 2) {
      pbVar5 = local_1c.LeadByte;
      while ((local_1c.LeadByte[0] != 0 && (bVar1 = pbVar5[1], bVar1 != 0))) {
        CodePage = (uint)*pbVar5;
        while (CodePage <= (uint)bVar1) {
          *(byte *)(param_2 + CodePage + 0x19) = (byte)param_2[CodePage + 0x19] | 4;
          CodePage = CodePage + 1;
        }
        pbVar5 = pbVar5 + 2;
        local_1c.LeadByte[0] = *pbVar5;
      }
      p_Var6 = param_2 + 0x1a;
      iVar8 = 0xfe;
      do {
        *(byte *)p_Var6 = (byte)*p_Var6 | 8;
        p_Var6 = p_Var6 + 1;
        iVar8 = iVar8 + -1;
      } while (iVar8 != 0);
      pwVar7 = CPtoLocaleName(*(int *)(param_2 + 4));
      *(wchar_t **)(param_2 + 0x21c) = pwVar7;
      uVar12 = 1;
    }
  }
  *(uint *)(param_2 + 8) = uVar12;
  *(undefined4 *)(param_2 + 0xc) = 0;
  *(undefined4 *)(param_2 + 0x10) = 0;
  *(undefined4 *)(param_2 + 0x14) = 0;
LAB_0040df0d:
  FUN_0040daee((int)param_2);
  goto LAB_0040e061;
}



// Library Function - Single Match
// Name: _memcpy_s
// Libraries: Visual Studio 2012 Release, Visual Studio 2015 Debug, Visual Studio 2015 Release

errno_t __cdecl _memcpy_s(void *_Dst,rsize_t _DstSize,void *_Src,rsize_t _MaxCount)

{
  errno_t eVar1;
  undefined4 *puVar2;
  errno_t *peVar3;
  
  if (_MaxCount == 0) {
    eVar1 = 0;
  }
  else {
    if (_Dst == (void *)0x0) {
      puVar2 = (undefined4 *)FUN_00407f40();
      eVar1 = 0x16;
      *puVar2 = 0x16;
      FUN_00407e83();
    }
    else {
      if ((_Src == (void *)0x0) || (_DstSize < _MaxCount)) {
        FUN_00402e50((int *)_Dst,0,_DstSize);
        if (_Src == (void *)0x0) {
          peVar3 = (errno_t *)FUN_00407f40();
          eVar1 = 0x16;
        }
        else {
          if (_MaxCount <= _DstSize) {
            return 0x16;
          }
          peVar3 = (errno_t *)FUN_00407f40();
          eVar1 = 0x22;
        }
        *peVar3 = eVar1;
        FUN_00407e83();
      }
      else {
        copy_buffer((undefined8 *)_Dst,(undefined8 *)_Src,_MaxCount);
        eVar1 = 0;
      }
    }
  }
  return eVar1;
}



void __cdecl
FUN_0040e0f3(uint param_1,DWORD param_2,LPCSTR param_3,int param_4,LPWSTR param_5,int param_6)

{
  if (param_1 < 0xc436) {
    if (((param_1 != 0xc435) && (param_1 != 0x2a)) &&
       ((param_1 < 0xc42c || (((0xc42e < param_1 && (param_1 != 0xc431)) && (param_1 != 0xc433))))))
    goto LAB_0040e155;
LAB_0040e16b:
    param_2 = 0;
  }
  else {
    if (param_1 != 0xd698) {
      if (param_1 < 0xdeaa) goto LAB_0040e155;
      if ((param_1 < 0xdeb4) || (param_1 == 65000)) goto LAB_0040e16b;
      if (param_1 != 0xfde9) goto LAB_0040e155;
    }
    param_2 = param_2 & 8;
  }
LAB_0040e155:
  MultiByteToWideChar(param_1,param_2,param_3,param_4,param_5,param_6);
  return;
}



// Library Function - Single Match
// Name: ?find_end_of_double_null_terminated_sequence@@YAPB_WQB_W@Z
// Libraries: Visual Studio 2015 Debug, Visual Studio 2015 Release
// wchar_t const * __cdecl find_end_of_double_null_terminated_sequence(wchar_t const * const)

wchar_t * __cdecl find_end_of_double_null_terminated_sequence(wchar_t *param_1)

{
  short sVar1;
  short *psVar2;
  
  sVar1 = *(short *)param_1;
  while (sVar1 != 0) {
    psVar2 = (short *)param_1;
    do {
      sVar1 = *psVar2;
      psVar2 = psVar2 + 1;
    } while (sVar1 != 0);
    param_1 = (wchar_t *)
              ((short *)param_1 + ((int)((int)psVar2 - (int)((short *)param_1 + 1)) >> 1) + 1);
    sVar1 = *(short *)param_1;
  }
  return (wchar_t *)((short *)param_1 + 1);
}



undefined8 * FUN_0040e1a6(void)

{
  undefined8 *puVar1;
  wchar_t *pwVar2;
  undefined8 *puVar3;
  
  puVar1 = (undefined8 *)GetEnvironmentStringsW();
  if (puVar1 == (undefined8 *)0x0) {
    puVar3 = (undefined8 *)0x0;
  }
  else {
    pwVar2 = find_end_of_double_null_terminated_sequence((wchar_t *)puVar1);
    puVar3 = (undefined8 *)__malloc_base((uint)(pwVar2 + -(int)puVar1) & 0xfffffffe);
    if (puVar3 != (undefined8 *)0x0) {
      copy_buffer(puVar3,puVar1,(uint)(pwVar2 + -(int)puVar1) & 0xfffffffe);
    }
    FID_conflict__free((void *)0x0);
    FreeEnvironmentStringsW((LPWCH)puVar1);
  }
  return puVar3;
}



LPVOID __cdecl FUN_0040e44b(wchar_t **param_1)

{
  wchar_t *pwVar1;
  uint _SizeInWords;
  wchar_t wVar2;
  code *pcVar3;
  wchar_t **ppwVar4;
  LPVOID pvVar5;
  LPVOID pvVar6;
  errno_t eVar7;
  int iVar8;
  wchar_t *pwVar9;
  LPVOID pvVar10;
  undefined4 uVar11;
  undefined4 uVar12;
  undefined4 uVar13;
  undefined4 uVar14;
  
  if (param_1 == (wchar_t **)0x0) {
    pvVar5 = (LPVOID)0x0;
  }
  else {
    iVar8 = 0;
    pwVar9 = *param_1;
    ppwVar4 = param_1;
    while (pwVar9 != (wchar_t *)0x0) {
      ppwVar4 = ppwVar4 + 1;
      iVar8 = iVar8 + 1;
      pwVar9 = *ppwVar4;
    }
    pvVar5 = __calloc_base(iVar8 + 1,4);
    if (pvVar5 == (LPVOID)0x0) {
LAB_0040e4f9:
      FUN_00409f3e();
LAB_0040e4fe:
      uVar14 = 0;
      uVar13 = 0;
      uVar12 = 0;
      uVar11 = 0;
      FUN_00407e93();
      pcVar3 = (code *)swi(3);
      pvVar5 = (LPVOID)(*pcVar3)(uVar11,uVar12,uVar13,uVar14);
      return pvVar5;
    }
    pwVar9 = *param_1;
    if (pwVar9 != (wchar_t *)0x0) {
      pvVar10 = (LPVOID)((int)pvVar5 - (int)param_1);
      do {
        pwVar1 = pwVar9 + 1;
        do {
          wVar2 = *pwVar9;
          pwVar9 = pwVar9 + 1;
        } while (wVar2 != L'\0');
        _SizeInWords = ((int)((int)pwVar9 - (int)pwVar1) >> 1) + 1;
        pvVar6 = __calloc_base(_SizeInWords,2);
        *(LPVOID *)((int)pvVar10 + (int)param_1) = pvVar6;
        FID_conflict__free((void *)0x0);
        if (*(int *)((int)pvVar10 + (int)param_1) == 0) goto LAB_0040e4f9;
        eVar7 = _wcscpy_s(*(wchar_t **)((int)pvVar10 + (int)param_1),_SizeInWords,*param_1);
        if (eVar7 != 0) goto LAB_0040e4fe;
        param_1 = param_1 + 1;
        pwVar9 = *param_1;
      } while (pwVar9 != (wchar_t *)0x0);
    }
    FID_conflict__free((void *)0x0);
  }
  return pvVar5;
}



void FUN_0040e50b(void)

{
  if (DAT_0041ef50 == DAT_0041ef54) {
    DAT_0041ef50 = (wchar_t **)FUN_0040e44b(DAT_0041ef50);
  }
  return;
}



int __cdecl FUN_0040e525(PCNZWCH param_1,uint param_2)

{
  ushort *puVar1;
  ushort **ppuVar2;
  int iVar3;
  ushort **ppuVar4;
  
  ppuVar2 = DAT_0041ef50;
  puVar1 = *DAT_0041ef50;
  ppuVar4 = DAT_0041ef50;
  while( true ) {
    if (puVar1 == (ushort *)0x0) {
      return -((int)((int)ppuVar4 - (int)ppuVar2) >> 2);
    }
    iVar3 = FUN_00412862(param_1,puVar1,param_2);
    if ((iVar3 == 0) && (((*ppuVar4)[param_2] == 0x3d || ((*ppuVar4)[param_2] == 0)))) break;
    ppuVar4 = ppuVar4 + 1;
    puVar1 = *ppuVar4;
  }
  return (int)((int)ppuVar4 - (int)ppuVar2) >> 2;
}



undefined4 __cdecl FUN_0040e57d(ushort *param_1,int param_2)

{
  uint uVar1;
  ushort uVar2;
  ushort uVar3;
  code *pcVar4;
  undefined4 *puVar5;
  ushort *_Memory;
  int iVar6;
  uint _SizeInWords;
  wchar_t *_Dst;
  errno_t eVar7;
  BOOL BVar8;
  uint uVar9;
  ushort *puVar10;
  undefined4 uVar11;
  int *piVar12;
  undefined4 uVar13;
  undefined4 uVar14;
  undefined4 uVar15;
  
  if (param_1 == (ushort *)0x0) {
    puVar5 = (undefined4 *)FUN_00407f40();
    *puVar5 = 0x16;
    return 0xffffffff;
  }
  _Memory = FUN_00402b60(param_1,0x3d);
  if ((_Memory == (ushort *)0x0) || (_Memory == param_1)) {
LAB_0040e422:
    puVar5 = (undefined4 *)FUN_00407f40();
    *puVar5 = 0x16;
  }
  else {
    uVar3 = _Memory[1];
    FUN_0040e50b();
    uVar11 = 0;
    if (DAT_0041ef50 == (int *)0x0) {
      if ((param_2 == 0) || (DAT_0041ef4c == (LPVOID)0x0)) {
        if (uVar3 == 0) {
          uVar11 = 0;
          _Memory = param_1;
          goto LAB_0040e430;
        }
        if (DAT_0041ef4c == (LPVOID)0x0) {
          DAT_0041ef4c = __calloc_base(1,4);
          FID_conflict__free((void *)0x0);
          if (DAT_0041ef4c == (LPVOID)0x0) goto LAB_0040e42d;
          if (DAT_0041ef50 != (int *)0x0) goto LAB_0040e2e0;
        }
        DAT_0041ef50 = (int *)__calloc_base(1,4);
        FID_conflict__free((void *)0x0);
      }
      else {
        iVar6 = thunk_FUN_00409226();
        if (iVar6 == 0) goto LAB_0040e422;
        FUN_0040e50b();
      }
      if (DAT_0041ef50 == (int *)0x0) goto LAB_0040e42d;
    }
LAB_0040e2e0:
    piVar12 = DAT_0041ef50;
    uVar9 = (int)((int)_Memory - (int)param_1) >> 1;
    _SizeInWords = FUN_0040e525((PCNZWCH)param_1,uVar9);
    if ((-1 < (int)_SizeInWords) && (*piVar12 != 0)) {
      FID_conflict__free((void *)piVar12[_SizeInWords]);
      if (uVar3 == 0) {
        while (piVar12[_SizeInWords] != 0) {
          piVar12[_SizeInWords] = piVar12[_SizeInWords + 1];
          _SizeInWords = _SizeInWords + 1;
        }
        piVar12 = (int *)__recalloc_base(piVar12,_SizeInWords,4);
        FID_conflict__free((void *)0x0);
        _Memory = param_1;
        if (piVar12 != (int *)0x0) {
LAB_0040e399:
          DAT_0041ef50 = piVar12;
        }
      }
      else {
        *(ushort **)(piVar12 + _SizeInWords) = param_1;
        _Memory = (ushort *)0x0;
      }
      if (param_2 != 0) {
        puVar10 = param_1;
        do {
          uVar2 = *puVar10;
          puVar10 = puVar10 + 1;
        } while (uVar2 != 0);
        _SizeInWords = ((int)((int)puVar10 - (int)(param_1 + 1)) >> 1) + 2;
        _Dst = (wchar_t *)__calloc_base(_SizeInWords,2);
        if (_Dst != (wchar_t *)0x0) {
          eVar7 = _wcscpy_s(_Dst,_SizeInWords,(wchar_t *)param_1);
          if (eVar7 != 0) {
            uVar15 = 0;
            uVar14 = 0;
            uVar13 = 0;
            uVar11 = 0;
            FUN_00407e93();
            pcVar4 = (code *)swi(3);
            uVar11 = (*pcVar4)(uVar11,uVar13,uVar14,uVar15);
            return uVar11;
          }
          (_Dst + uVar9 + 1)[-1] = L'\0';
          BVar8 = SetEnvironmentVariableW
                            (_Dst,(LPCWSTR)(-(uint)(uVar3 != 0) & (uint)(_Dst + uVar9 + 1)));
          if (BVar8 == 0) {
            puVar5 = (undefined4 *)FUN_00407f40();
            uVar11 = 0xffffffff;
            *puVar5 = 0x2a;
          }
        }
        FID_conflict__free(_Dst);
      }
      goto LAB_0040e430;
    }
    _Memory = param_1;
    if (uVar3 == 0) goto LAB_0040e430;
    uVar1 = -_SizeInWords + 2;
    if ((-_SizeInWords <= uVar1) && (uVar1 < 0x3fffffff)) {
      piVar12 = (int *)__recalloc_base(piVar12,uVar1,4);
      FID_conflict__free((void *)0x0);
      if (piVar12 != (int *)0x0) {
        *(ushort **)(piVar12 + _SizeInWords * 0x3fffffff) = param_1;
        piVar12[_SizeInWords * 0x3fffffff + 1] = 0;
        _Memory = (ushort *)0x0;
        goto LAB_0040e399;
      }
    }
  }
LAB_0040e42d:
  uVar11 = 0xffffffff;
  _Memory = param_1;
LAB_0040e430:
  FID_conflict__free(_Memory);
  return uVar11;
}



// Library Function - Single Match
// Name: ___acrt_lock
// Library: Visual Studio 2015 Release

void __cdecl ___acrt_lock(int param_1)

{
  EnterCriticalSection((LPCRITICAL_SECTION)(&DAT_0041f2b0 + param_1 * 0x18));
  return;
}



undefined4 FUN_0040e5e0(void)

{
  undefined4 in_EAX;
  undefined4 extraout_EAX;
  int iVar1;
  LPCRITICAL_SECTION lpCriticalSection;
  
  if (DAT_0041f400 != 0) {
    lpCriticalSection = (LPCRITICAL_SECTION)(&DAT_0041f298 + DAT_0041f400 * 6);
    iVar1 = DAT_0041f400;
    do {
      DeleteCriticalSection(lpCriticalSection);
      DAT_0041f400 = DAT_0041f400 + -1;
      lpCriticalSection = (LPCRITICAL_SECTION)&lpCriticalSection[-1].LockSemaphore;
      iVar1 = iVar1 + -1;
      in_EAX = extraout_EAX;
    } while (iVar1 != 0);
  }
  return CONCAT31((int3)((uint)in_EAX >> 8),1);
}



// Library Function - Single Match
// Name: ___acrt_unlock
// Library: Visual Studio 2015 Release

void __cdecl ___acrt_unlock(int param_1)

{
  LeaveCriticalSection((LPCRITICAL_SECTION)(&DAT_0041f2b0 + param_1 * 0x18));
  return;
}



undefined4 FUN_0040e628(void)

{
  int in_FS_OFFSET;
  int local_8;
  
  local_8 = 0;
  if ((-1 < *(int *)(*(int *)(*(int *)(in_FS_OFFSET + 0x30) + 0x10) + 8)) &&
     (FUN_0040a279(&local_8), local_8 == 1)) {
    return 0;
  }
  return 1;
}



undefined4 * FUN_0040e65b(void)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  
  puVar2 = (undefined4 *)__calloc_base(0x40,0x38);
  if (puVar2 == (undefined4 *)0x0) {
    puVar2 = (undefined4 *)0x0;
  }
  else {
    if (puVar2 != puVar2 + 0x380) {
      puVar3 = puVar2 + 8;
      do {
        FUN_0040a415((LPCRITICAL_SECTION)(puVar3 + -8),4000,0);
        puVar3[-2] = 0xffffffff;
        *puVar3 = 0;
        puVar3[1] = 0;
        puVar1 = puVar3 + 6;
        puVar3[2] = 0xa0a0000;
        *(undefined *)(puVar3 + 3) = 10;
        *(byte *)((int)puVar3 + 0xd) = *(byte *)((int)puVar3 + 0xd) & 0xf8;
        *(undefined4 *)((int)puVar3 + 0xe) = 0;
        *(undefined *)((int)puVar3 + 0x12) = 0;
        puVar3 = puVar3 + 0xe;
      } while (puVar1 != puVar2 + 0x380);
    }
  }
  FID_conflict__free((void *)0x0);
  return puVar2;
}



// Library Function - Single Match
// Name: ___acrt_lowio_destroy_handle_array
// Library: Visual Studio 2015 Release

void __cdecl ___acrt_lowio_destroy_handle_array(LPCRITICAL_SECTION param_1)

{
  LPCRITICAL_SECTION lpCriticalSection;
  
  if (param_1 != (LPCRITICAL_SECTION)0x0) {
    lpCriticalSection = param_1;
    if (param_1 != (LPCRITICAL_SECTION)&param_1[0x95].RecursionCount) {
      do {
        DeleteCriticalSection(lpCriticalSection);
        lpCriticalSection = (LPCRITICAL_SECTION)&lpCriticalSection[2].RecursionCount;
      } while (lpCriticalSection != (LPCRITICAL_SECTION)&param_1[0x95].RecursionCount);
    }
    FID_conflict__free(param_1);
  }
  return;
}



// WARNING: Function: __SEH_prolog4 replaced with injection: SEH_prolog4
// WARNING: Function: __SEH_epilog4 replaced with injection: EH_epilog3

undefined4 __cdecl FUN_0040e70d(uint param_1)

{
  undefined4 *puVar1;
  int iVar2;
  undefined4 uVar3;
  int iVar4;
  
  if (param_1 < 0x2000) {
    uVar3 = 0;
    ___acrt_lock(7);
    iVar4 = 0;
    iVar2 = DAT_0041f280;
    while (iVar2 <= (int)param_1) {
      if ((&DAT_0041f080)[iVar4] == 0) {
        puVar1 = FUN_0040e65b();
        *(undefined4 **)(&DAT_0041f080 + iVar4) = puVar1;
        if (puVar1 == (undefined4 *)0x0) {
          uVar3 = 0xc;
          break;
        }
        iVar2 = DAT_0041f280 + 0x40;
        DAT_0041f280 = iVar2;
      }
      iVar4 = iVar4 + 1;
    }
    FUN_0040e798();
  }
  else {
    puVar1 = (undefined4 *)FUN_00407f40();
    uVar3 = 9;
    *puVar1 = 9;
    FUN_00407e83();
  }
  return uVar3;
}



void FUN_0040e798(void)

{
  ___acrt_unlock(7);
  return;
}



// Library Function - Single Match
// Name: ___acrt_lowio_lock_fh
// Library: Visual Studio 2015 Release

void __cdecl ___acrt_lowio_lock_fh(uint param_1)

{
  EnterCriticalSection
            ((LPCRITICAL_SECTION)((param_1 & 0x3f) * 0x38 + (&DAT_0041f080)[(int)param_1 >> 6]));
  return;
}



// Library Function - Single Match
// Name: ___acrt_lowio_unlock_fh
// Library: Visual Studio 2015 Release

void __cdecl ___acrt_lowio_unlock_fh(uint param_1)

{
  LeaveCriticalSection
            ((LPCRITICAL_SECTION)((param_1 & 0x3f) * 0x38 + (&DAT_0041f080)[(int)param_1 >> 6]));
  return;
}



// Library Function - Single Match
// Name: __free_osfhnd
// Library: Visual Studio 2015 Release

int __cdecl __free_osfhnd(int param_1)

{
  int iVar1;
  undefined4 *puVar2;
  int iVar3;
  DWORD nStdHandle;
  
  if ((-1 < param_1) && ((uint)param_1 < DAT_0041f280)) {
    iVar3 = (param_1 & 0x3fU) * 0x38;
    if (((*(byte *)(iVar3 + 0x28 + (&DAT_0041f080)[param_1 >> 6]) & 1) != 0) &&
       (*(int *)(iVar3 + 0x18 + (&DAT_0041f080)[param_1 >> 6]) != -1)) {
      iVar1 = FUN_00408e62();
      if (iVar1 == 1) {
        if (param_1 == 0) {
          nStdHandle = 0xfffffff6;
        }
        else {
          if (param_1 == 1) {
            nStdHandle = 0xfffffff5;
          }
          else {
            if (param_1 != 2) goto LAB_0040e84d;
            nStdHandle = 0xfffffff4;
          }
        }
        SetStdHandle(nStdHandle,(HANDLE)0x0);
      }
LAB_0040e84d:
      *(undefined4 *)(iVar3 + 0x18 + (&DAT_0041f080)[param_1 >> 6]) = 0xffffffff;
      return 0;
    }
  }
  puVar2 = (undefined4 *)FUN_00407f40();
  *puVar2 = 9;
  puVar2 = (undefined4 *)FUN_00407f2d();
  *puVar2 = 0;
  return -1;
}



// Library Function - Single Match
// Name: __get_osfhandle
// Library: Visual Studio 2015 Release

intptr_t __cdecl __get_osfhandle(int _FileHandle)

{
  undefined4 *puVar1;
  int iVar2;
  
  if (_FileHandle == -2) {
    puVar1 = (undefined4 *)FUN_00407f2d();
    *puVar1 = 0;
    puVar1 = (undefined4 *)FUN_00407f40();
    *puVar1 = 9;
  }
  else {
    if ((-1 < _FileHandle) && ((uint)_FileHandle < DAT_0041f280)) {
      iVar2 = (_FileHandle & 0x3fU) * 0x38;
      if ((*(byte *)((&DAT_0041f080)[_FileHandle >> 6] + 0x28 + iVar2) & 1) != 0) {
        return *(intptr_t *)((&DAT_0041f080)[_FileHandle >> 6] + 0x18 + iVar2);
      }
    }
    puVar1 = (undefined4 *)FUN_00407f2d();
    *puVar1 = 0;
    puVar1 = (undefined4 *)FUN_00407f40();
    *puVar1 = 9;
    FUN_00407e83();
  }
  return -1;
}



// Library Function - Single Match
// Name: ___acrt_locale_free_monetary
// Library: Visual Studio 2015 Release

void __cdecl ___acrt_locale_free_monetary(int param_1)

{
  if (param_1 != 0) {
    if (*(undefined **)(param_1 + 0xc) != PTR_DAT_0041e734) {
      FID_conflict__free(*(undefined **)(param_1 + 0xc));
    }
    if (*(undefined **)(param_1 + 0x10) != PTR_DAT_0041e738) {
      FID_conflict__free(*(undefined **)(param_1 + 0x10));
    }
    if (*(undefined **)(param_1 + 0x14) != PTR_DAT_0041e73c) {
      FID_conflict__free(*(undefined **)(param_1 + 0x14));
    }
    if (*(undefined **)(param_1 + 0x18) != PTR_DAT_0041e740) {
      FID_conflict__free(*(undefined **)(param_1 + 0x18));
    }
    if (*(undefined **)(param_1 + 0x1c) != PTR_DAT_0041e744) {
      FID_conflict__free(*(undefined **)(param_1 + 0x1c));
    }
    if (*(undefined **)(param_1 + 0x20) != PTR_DAT_0041e748) {
      FID_conflict__free(*(undefined **)(param_1 + 0x20));
    }
    if (*(undefined **)(param_1 + 0x24) != PTR_DAT_0041e74c) {
      FID_conflict__free(*(undefined **)(param_1 + 0x24));
    }
    if (*(undefined **)(param_1 + 0x38) != PTR_DAT_0041e760) {
      FID_conflict__free(*(undefined **)(param_1 + 0x38));
    }
    if (*(undefined **)(param_1 + 0x3c) != PTR_DAT_0041e764) {
      FID_conflict__free(*(undefined **)(param_1 + 0x3c));
    }
    if (*(undefined **)(param_1 + 0x40) != PTR_DAT_0041e768) {
      FID_conflict__free(*(undefined **)(param_1 + 0x40));
    }
    if (*(undefined **)(param_1 + 0x44) != PTR_DAT_0041e76c) {
      FID_conflict__free(*(undefined **)(param_1 + 0x44));
    }
    if (*(undefined **)(param_1 + 0x48) != PTR_DAT_0041e770) {
      FID_conflict__free(*(undefined **)(param_1 + 0x48));
    }
    if (*(undefined **)(param_1 + 0x4c) != PTR_DAT_0041e774) {
      FID_conflict__free(*(undefined **)(param_1 + 0x4c));
    }
  }
  return;
}



// Library Function - Single Match
// Name: ___acrt_locale_free_numeric
// Library: Visual Studio 2015 Release

void __cdecl ___acrt_locale_free_numeric(void **param_1)

{
  if (param_1 != (void **)0x0) {
    if ((undefined *)*param_1 != PTR_DAT_0041e728) {
      FID_conflict__free(*param_1);
    }
    if ((undefined *)param_1[1] != PTR_DAT_0041e72c) {
      FID_conflict__free(param_1[1]);
    }
    if ((undefined *)param_1[2] != PTR_DAT_0041e730) {
      FID_conflict__free(param_1[2]);
    }
    if ((undefined *)param_1[0xc] != PTR_DAT_0041e758) {
      FID_conflict__free(param_1[0xc]);
    }
    if ((undefined *)param_1[0xd] != PTR_DAT_0041e75c) {
      FID_conflict__free(param_1[0xd]);
    }
  }
  return;
}



void __cdecl FUN_0040ea49(void **param_1,uint param_2)

{
  uint uVar1;
  uint uVar2;
  
  uVar2 = 0;
  uVar1 = ~-(uint)(param_1 + param_2 < param_1) & param_2 & 0x3fffffff;
  if (uVar1 != 0) {
    do {
      FID_conflict__free(*param_1);
      uVar2 = uVar2 + 1;
      param_1 = param_1 + 1;
    } while (uVar2 != uVar1);
  }
  return;
}



// Library Function - Single Match
// Name: ___acrt_locale_free_time
// Library: Visual Studio 2015 Release

void __cdecl ___acrt_locale_free_time(void **param_1)

{
  if (param_1 != (void **)0x0) {
    FUN_0040ea49(param_1,7);
    FUN_0040ea49(param_1 + 7,7);
    FUN_0040ea49(param_1 + 0xe,0xc);
    FUN_0040ea49(param_1 + 0x1a,0xc);
    FUN_0040ea49(param_1 + 0x26,2);
    FID_conflict__free(param_1[0x28]);
    FID_conflict__free(param_1[0x29]);
    FID_conflict__free(param_1[0x2a]);
    FUN_0040ea49(param_1 + 0x2d,7);
    FUN_0040ea49(param_1 + 0x34,7);
    FUN_0040ea49(param_1 + 0x3b,0xc);
    FUN_0040ea49(param_1 + 0x47,0xc);
    FUN_0040ea49(param_1 + 0x53,2);
    FID_conflict__free(param_1[0x55]);
    FID_conflict__free(param_1[0x56]);
    FID_conflict__free(param_1[0x57]);
    FID_conflict__free(param_1[0x58]);
  }
  return;
}



short * __cdecl FUN_0040eb65(short *param_1,short *param_2)

{
  short sVar1;
  short sVar2;
  short *psVar3;
  
  if (*param_1 != 0) {
    do {
      if (*param_2 != 0) {
        sVar1 = *param_1;
        psVar3 = param_2;
        sVar2 = *param_2;
        do {
          if (sVar2 == sVar1) {
            return param_1;
          }
          psVar3 = psVar3 + 1;
          sVar2 = *psVar3;
          sVar1 = *param_1;
        } while (sVar2 != 0);
      }
      param_1 = param_1 + 1;
    } while (*param_1 != 0);
  }
  return (short *)0x0;
}



// WARNING: Function: __alloca_probe_16 replaced with injection: alloca_probe

void __cdecl
FUN_0040ebb3(undefined4 *param_1,DWORD param_2,LPCSTR param_3,int param_4,LPWORD param_5,
            uint param_6,int param_7)

{
  uint uVar1;
  int cchSrc;
  int *piVar2;
  int *lpSrcStr;
  int local_20;
  int local_1c;
  char local_14;
  int local_10;
  uint local_c;
  uint local_8;
  
  local_8 = DAT_0041e004 ^ (uint)&stack0xfffffffc;
  FUN_00405ed6(&local_20,param_1);
  if (param_6 == 0) {
    param_6 = *(uint *)(local_1c + 8);
  }
  local_10 = FUN_0040e0f3(param_6,(uint)(param_7 != 0) * 8 + 1,param_3,param_4,(LPWSTR)0x0,0);
  if (local_10 == 0) goto LAB_0040ec92;
  local_c = local_10 * 2;
  uVar1 = -(uint)(local_c < local_c + 8) & local_c + 8;
  if (uVar1 == 0) {
    lpSrcStr = (int *)0x0;
  }
  else {
    if (uVar1 < 0x401) {
      piVar2 = (int *)&stack0xffffffd4;
      lpSrcStr = (int *)&stack0xffffffd4;
      if ((undefined *)register0x00000010 != (undefined *)0x2c) {
LAB_0040ec4c:
        lpSrcStr = piVar2 + 2;
      }
    }
    else {
      lpSrcStr = (int *)__malloc_base(uVar1);
      if (lpSrcStr != (int *)0x0) {
        *lpSrcStr = 0xdddd;
        piVar2 = lpSrcStr;
        goto LAB_0040ec4c;
      }
    }
  }
  if (lpSrcStr != (int *)0x0) {
    FUN_00402e50(lpSrcStr,0,local_c);
    cchSrc = FUN_0040e0f3(param_6,1,param_3,param_4,(LPWSTR)lpSrcStr,local_10);
    if (cchSrc != 0) {
      GetStringTypeW(param_2,(LPCWSTR)lpSrcStr,cchSrc,param_5);
    }
  }
  FUN_0040ecb8((int)lpSrcStr);
LAB_0040ec92:
  if (local_14 != 0) {
    *(uint *)(local_20 + 0x350) = *(uint *)(local_20 + 0x350) & 0xfffffffd;
  }
  FUN_00401f78();
  return;
}



void __cdecl FUN_0040ecb8(int param_1)

{
  if ((param_1 != 0) && (*(int *)(param_1 + -8) == 0xdddd)) {
    FID_conflict__free((int *)(param_1 + -8));
  }
  return;
}



// Library Function - Single Match
// Name: ___acrt_add_locale_ref
// Library: Visual Studio 2015 Release

void __cdecl ___acrt_add_locale_ref(int param_1)

{
  int *piVar1;
  int **ppiVar2;
  int iVar3;
  
  LOCK();
  *(int *)(param_1 + 0xc) = *(int *)(param_1 + 0xc) + 1;
  piVar1 = *(int **)(param_1 + 0x7c);
  if (piVar1 != (int *)0x0) {
    LOCK();
    *piVar1 = *piVar1 + 1;
  }
  piVar1 = *(int **)(param_1 + 0x84);
  if (piVar1 != (int *)0x0) {
    LOCK();
    *piVar1 = *piVar1 + 1;
  }
  piVar1 = *(int **)(param_1 + 0x80);
  if (piVar1 != (int *)0x0) {
    LOCK();
    *piVar1 = *piVar1 + 1;
  }
  piVar1 = *(int **)(param_1 + 0x8c);
  if (piVar1 != (int *)0x0) {
    LOCK();
    *piVar1 = *piVar1 + 1;
  }
  ppiVar2 = (int **)(param_1 + 0x28);
  iVar3 = 6;
  do {
    if ((ppiVar2[-2] != (int *)&DAT_0041e200) && (piVar1 = *ppiVar2, piVar1 != (int *)0x0)) {
      LOCK();
      *piVar1 = *piVar1 + 1;
    }
    if ((ppiVar2[-3] != (int *)0x0) && (piVar1 = ppiVar2[-1], piVar1 != (int *)0x0)) {
      LOCK();
      *piVar1 = *piVar1 + 1;
    }
    ppiVar2 = ppiVar2 + 4;
    iVar3 = iVar3 + -1;
  } while (iVar3 != 0);
  ___acrt_locale_add_lc_time_reference(*(undefined ***)(param_1 + 0x9c));
  return;
}



// Library Function - Single Match
// Name: ___acrt_free_locale
// Library: Visual Studio 2015 Release

void __cdecl ___acrt_free_locale(void *param_1)

{
  int *_Memory;
  void **ppvVar1;
  int **ppiVar2;
  int local_8;
  
  if ((((*(undefined ***)((int)param_1 + 0x88) != (undefined **)0x0) &&
       (*(undefined ***)((int)param_1 + 0x88) != &PTR_DAT_0041e728)) &&
      (*(int **)((int)param_1 + 0x7c) != (int *)0x0)) && (**(int **)((int)param_1 + 0x7c) == 0)) {
    _Memory = *(int **)((int)param_1 + 0x84);
    if ((_Memory != (int *)0x0) && (*_Memory == 0)) {
      FID_conflict__free(_Memory);
      ___acrt_locale_free_monetary(*(int *)((int)param_1 + 0x88));
    }
    _Memory = *(int **)((int)param_1 + 0x80);
    if ((_Memory != (int *)0x0) && (*_Memory == 0)) {
      FID_conflict__free(_Memory);
      ___acrt_locale_free_numeric(*(void ***)((int)param_1 + 0x88));
    }
    FID_conflict__free(*(void **)((int)param_1 + 0x7c));
    FID_conflict__free(*(void **)((int)param_1 + 0x88));
  }
  if ((*(int **)((int)param_1 + 0x8c) != (int *)0x0) && (**(int **)((int)param_1 + 0x8c) == 0)) {
    FID_conflict__free((void *)(*(int *)((int)param_1 + 0x90) + -0xfe));
    FID_conflict__free((void *)(*(int *)((int)param_1 + 0x94) + -0x80));
    FID_conflict__free((void *)(*(int *)((int)param_1 + 0x98) + -0x80));
    FID_conflict__free(*(void **)((int)param_1 + 0x8c));
  }
  ___acrt_locale_free_lc_time_if_unreferenced(*(undefined ***)((int)param_1 + 0x9c));
  ppvVar1 = (void **)((int)param_1 + 0xa0);
  local_8 = 6;
  ppiVar2 = (int **)((int)param_1 + 0x28);
  do {
    if (((ppiVar2[-2] != (int *)&DAT_0041e200) && (_Memory = *ppiVar2, _Memory != (int *)0x0)) &&
       (*_Memory == 0)) {
      FID_conflict__free(_Memory);
      FID_conflict__free(*ppvVar1);
    }
    if (((ppiVar2[-3] != (int *)0x0) && (_Memory = ppiVar2[-1], _Memory != (int *)0x0)) &&
       (*_Memory == 0)) {
      FID_conflict__free(_Memory);
    }
    ppvVar1 = ppvVar1 + 1;
    ppiVar2 = ppiVar2 + 4;
    local_8 = local_8 + -1;
  } while (local_8 != 0);
  FID_conflict__free(param_1);
  return;
}



// Library Function - Single Match
// Name: ___acrt_locale_add_lc_time_reference
// Library: Visual Studio 2015 Release

undefined * __cdecl ___acrt_locale_add_lc_time_reference(undefined **param_1)

{
  undefined *puVar1;
  
  if ((param_1 != (undefined **)0x0) && (param_1 != &PTR_DAT_004184d8)) {
    LOCK();
    param_1 = param_1 + 0x2c;
    puVar1 = *param_1;
    *param_1 = *param_1 + 1;
    return puVar1 + 1;
  }
  return (undefined *)0x7fffffff;
}



// Library Function - Single Match
// Name: ___acrt_locale_free_lc_time_if_unreferenced
// Library: Visual Studio 2015 Release

void __cdecl ___acrt_locale_free_lc_time_if_unreferenced(undefined **param_1)

{
  if (((param_1 != (undefined **)0x0) && (param_1 != &PTR_DAT_004184d8)) &&
     (param_1[0x2c] == (undefined *)0x0)) {
    ___acrt_locale_free_time(param_1);
    FID_conflict__free(param_1);
  }
  return;
}



// Library Function - Single Match
// Name: ___acrt_locale_release_lc_time_reference
// Library: Visual Studio 2015 Release

undefined * __cdecl ___acrt_locale_release_lc_time_reference(undefined **param_1)

{
  undefined *puVar1;
  
  if ((param_1 != (undefined **)0x0) && (param_1 != &PTR_DAT_004184d8)) {
    LOCK();
    param_1 = param_1 + 0x2c;
    puVar1 = *param_1;
    *param_1 = *param_1 + -1;
    return puVar1 + -1;
  }
  return (undefined *)0x7fffffff;
}



// Library Function - Single Match
// Name: ___acrt_release_locale_ref
// Library: Visual Studio 2015 Release

void __cdecl ___acrt_release_locale_ref(int param_1)

{
  int *piVar1;
  int **ppiVar2;
  int iVar3;
  
  if (param_1 != 0) {
    LOCK();
    *(int *)(param_1 + 0xc) = *(int *)(param_1 + 0xc) + -1;
    piVar1 = *(int **)(param_1 + 0x7c);
    if (piVar1 != (int *)0x0) {
      LOCK();
      *piVar1 = *piVar1 + -1;
    }
    piVar1 = *(int **)(param_1 + 0x84);
    if (piVar1 != (int *)0x0) {
      LOCK();
      *piVar1 = *piVar1 + -1;
    }
    piVar1 = *(int **)(param_1 + 0x80);
    if (piVar1 != (int *)0x0) {
      LOCK();
      *piVar1 = *piVar1 + -1;
    }
    piVar1 = *(int **)(param_1 + 0x8c);
    if (piVar1 != (int *)0x0) {
      LOCK();
      *piVar1 = *piVar1 + -1;
    }
    ppiVar2 = (int **)(param_1 + 0x28);
    iVar3 = 6;
    do {
      if ((ppiVar2[-2] != (int *)&DAT_0041e200) && (piVar1 = *ppiVar2, piVar1 != (int *)0x0)) {
        LOCK();
        *piVar1 = *piVar1 + -1;
      }
      if ((ppiVar2[-3] != (int *)0x0) && (piVar1 = ppiVar2[-1], piVar1 != (int *)0x0)) {
        LOCK();
        *piVar1 = *piVar1 + -1;
      }
      ppiVar2 = ppiVar2 + 4;
      iVar3 = iVar3 + -1;
    } while (iVar3 != 0);
    ___acrt_locale_release_lc_time_reference(*(undefined ***)(param_1 + 0x9c));
  }
  return;
}



// WARNING: Function: __SEH_prolog4 replaced with injection: SEH_prolog4
// WARNING: Function: __SEH_epilog4 replaced with injection: EH_epilog3

undefined ** FUN_0040efa3(void)

{
  code *pcVar1;
  uint uVar2;
  undefined **ppuVar3;
  
  uVar2 = FUN_0040b5c7();
  if (((*(uint *)(uVar2 + 0x350) & DAT_0041e780) == 0) ||
     (ppuVar3 = *(undefined ***)(undefined **)(uVar2 + 0x4c), ppuVar3 == (undefined **)0x0)) {
    ___acrt_lock(4);
    ppuVar3 = __updatetlocinfoEx_nolock((undefined **)(uVar2 + 0x4c),DAT_0041f298);
    FUN_0040f003();
    if (ppuVar3 == (undefined **)0x0) {
      FUN_00409f3e();
      pcVar1 = (code *)swi(3);
      ppuVar3 = (undefined **)(*pcVar1)();
      return ppuVar3;
    }
  }
  return ppuVar3;
}



void FUN_0040f003(void)

{
  ___acrt_unlock(4);
  return;
}



// Library Function - Single Match
// Name: __updatetlocinfoEx_nolock
// Library: Visual Studio 2015 Release

undefined ** __cdecl __updatetlocinfoEx_nolock(undefined **param_1,undefined **param_2)

{
  undefined **ppuVar1;
  
  if ((param_2 == (undefined **)0x0) || (param_1 == (undefined **)0x0)) {
    param_2 = (undefined **)0x0;
  }
  else {
    ppuVar1 = (undefined **)*param_1;
    if (ppuVar1 != param_2) {
      *(undefined ***)param_1 = param_2;
      ___acrt_add_locale_ref((int)param_2);
      if (((ppuVar1 != (undefined **)0x0) &&
          (___acrt_release_locale_ref((int)ppuVar1), ppuVar1[3] == (undefined *)0x0)) &&
         (ppuVar1 != &PTR_DAT_0041e140)) {
        ___acrt_free_locale(ppuVar1);
      }
    }
  }
  return param_2;
}



// Library Function - Single Match
// Name: __recalloc_base
// Library: Visual Studio 2015 Release

LPVOID __cdecl __recalloc_base(void *param_1,uint param_2,uint param_3)

{
  undefined4 *puVar1;
  LPVOID pvVar2;
  size_t sVar3;
  
  if ((param_2 == 0) || (param_3 <= 0xffffffe0 / param_2)) {
    if (param_1 == (void *)0x0) {
      sVar3 = 0;
    }
    else {
      sVar3 = __msize(param_1);
    }
    param_2 = param_2 * param_3;
    pvVar2 = __realloc_base(param_1,param_2);
    if ((pvVar2 != (LPVOID)0x0) && (sVar3 < param_2)) {
      FUN_00402e50((int *)((int)pvVar2 + sVar3),0,param_2 - sVar3);
    }
  }
  else {
    puVar1 = (undefined4 *)FUN_00407f40();
    *puVar1 = 0xc;
    pvVar2 = (LPVOID)0x0;
  }
  return pvVar2;
}



uint FUN_0040f0f2(code **param_1,code **param_2)

{
  code *pcVar1;
  code **in_EAX;
  code **ppcVar2;
  undefined4 uVar3;
  
  ppcVar2 = param_1;
  if (param_1 != param_2) {
    do {
      pcVar1 = *ppcVar2;
      if (pcVar1 != (code *)0x0) {
        _guard_check_icall();
        in_EAX = (code **)(*pcVar1)();
        if ((char)in_EAX == 0) break;
      }
      ppcVar2 = ppcVar2 + 2;
    } while (ppcVar2 != param_2);
    if (ppcVar2 != param_2) {
      if (ppcVar2 != param_1) {
        ppcVar2 = ppcVar2 + -1;
        do {
          if ((ppcVar2[-1] != (code *)0x0) && (pcVar1 = *ppcVar2, pcVar1 != (code *)0x0)) {
            uVar3 = 0;
            _guard_check_icall();
            (*pcVar1)(uVar3);
          }
          in_EAX = ppcVar2 + -1;
          ppcVar2 = ppcVar2 + -2;
        } while (in_EAX != param_1);
      }
      return (uint)in_EAX & 0xffffff00;
    }
  }
  return CONCAT31((int3)((uint)in_EAX >> 8),1);
}



undefined __cdecl FUN_0040f15a(int param_1,int param_2)

{
  code *pcVar1;
  undefined4 uVar2;
  
  if (param_1 != param_2) {
    do {
      pcVar1 = *(code **)(param_2 + -4);
      if (pcVar1 != (code *)0x0) {
        uVar2 = 0;
        _guard_check_icall();
        (*pcVar1)(uVar2);
      }
      param_2 = param_2 + -8;
    } while (param_2 != param_1);
  }
  return 1;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void __cdecl FUN_0040f18b(undefined4 param_1)

{
  _DAT_0041f414 = param_1;
  return;
}



undefined4 __cdecl FUN_0040f19a(undefined4 param_1)

{
  code *pcVar1;
  int iVar2;
  
  pcVar1 = (code *)__query_new_handler();
  if (pcVar1 != (code *)0x0) {
    _guard_check_icall();
    iVar2 = (*pcVar1)(param_1);
    if (iVar2 != 0) {
      return 1;
    }
  }
  return 0;
}



// WARNING: Function: __SEH_prolog4 replaced with injection: SEH_prolog4
// WARNING: Function: __SEH_epilog4 replaced with injection: EH_epilog3
// WARNING: Globals starting with '_' overlap smaller symbols at the same address
// Library Function - Single Match
// Name: __query_new_handler
// Library: Visual Studio 2015 Release

uint __query_new_handler(void)

{
  byte bVar1;
  uint uVar2;
  
  ___acrt_lock(0);
  bVar1 = (byte)DAT_0041e004 & 0x1f;
  uVar2 = DAT_0041e004 ^ _DAT_0041f414;
  FUN_0040f210();
  return uVar2 >> bVar1 | uVar2 << 0x20 - bVar1;
}



void FUN_0040f210(void)

{
  ___acrt_unlock(0);
  return;
}



// WARNING: Function: __SEH_prolog4 replaced with injection: SEH_prolog4
// WARNING: Function: __SEH_epilog4 replaced with injection: EH_epilog3
// WARNING: Globals starting with '_' overlap smaller symbols at the same address
// Library Function - Single Match
// Name: 
??$?RV<lambda_a048d3beccc847880fc8490e18b82769>@@AAV<lambda_ec61778202f4f5fc7e7711acc23c3bca>@@V<lambda_f7496a158712204296dd6628a163878e>@@@?$__crt_seh_guarded_call@P6AXH@Z@@QAEP6AXH@Z$$QAV<lambda_a048d3beccc847880fc8490e18b82769>@@AAV<lambda_ec61778202f4f5fc7e7711acc23c3bca>@@$$QAV<lambda_f7496a158712204296dd6628a163878e>@@@Z
// Library: Visual Studio 2015 Release
// public: void (__cdecl*__thiscall __crt_seh_guarded_call<void (__cdecl*)(int)>::operator()<class
// <lambda_a048d3beccc847880fc8490e18b82769>,class <lambda_ec61778202f4f5fc7e7711acc23c3bca>&,class
// <lambda_f7496a158712204296dd6628a163878e>>(class
// <lambda_a048d3beccc847880fc8490e18b82769>&&,class
// <lambda_ec61778202f4f5fc7e7711acc23c3bca>&,class
// <lambda_f7496a158712204296dd6628a163878e>&&))(int)

FuncDef6 * __thiscall

operator___class__lambda_a048d3beccc847880fc8490e18b82769__class__lambda_ec61778202f4f5fc7e7711acc23c3bca___class__lambda_f7496a158712204296dd6628a163878e___
          (__crt_seh_guarded_call_void____cdecl___int__ *this,
          _lambda_a048d3beccc847880fc8490e18b82769_ *param_1,
          _lambda_ec61778202f4f5fc7e7711acc23c3bca_ *param_2,
          _lambda_f7496a158712204296dd6628a163878e_ *param_3)

{
  byte bVar1;
  uint uVar2;
  
  ___acrt_lock(*(int *)param_1);
  bVar1 = (byte)DAT_0041e004 & 0x1f;
  uVar2 = DAT_0041e004 ^ _DAT_0041f420;
  FUN_0040f267();
  return (FuncDef6 *)(uVar2 >> bVar1 | uVar2 << 0x20 - bVar1);
}



void FUN_0040f267(void)

{
  int unaff_EBP;
  
  ___acrt_unlock(**(int **)(unaff_EBP + 0x10));
  return;
}



// Library Function - Single Match
// Name: ?get_global_action_nolock@@YAPAP6AXH@ZH@Z
// Library: Visual Studio 2015 Release
// void (__cdecl** __cdecl get_global_action_nolock(int))(int)

FuncDef7 * __cdecl get_global_action_nolock(int param_1)

{
  if (param_1 == 2) {
    return (FuncDef7 *)&DAT_0041f418;
  }
  if (param_1 != 6) {
    if (param_1 == 0xf) {
      return (FuncDef7 *)&DAT_0041f424;
    }
    if (param_1 == 0x15) {
      return (FuncDef7 *)&DAT_0041f41c;
    }
    if (param_1 != 0x16) {
      return (FuncDef7 *)0x0;
    }
  }
  return (FuncDef7 *)&DAT_0041f420;
}



// Library Function - Single Match
// Name: ?siglookup@@YAPAU__crt_signal_action_t@@HQAU1@@Z
// Library: Visual Studio 2015 Release
// struct __crt_signal_action_t * __cdecl siglookup(int,struct __crt_signal_action_t * const)

__crt_signal_action_t * __cdecl siglookup(int param_1,__crt_signal_action_t *param_2)

{
  __crt_signal_action_t *p_Var1;
  
  p_Var1 = param_2 + 0x90;
  if (param_2 != p_Var1) {
    do {
      if (*(int *)(param_2 + 4) == param_1) {
        return param_2;
      }
      param_2 = param_2 + 0xc;
    } while (param_2 != p_Var1);
  }
  return (__crt_signal_action_t *)0x0;
}



void FUN_0040f2dd(void)

{
  undefined4 local_10;
  undefined4 local_c;
  __crt_seh_guarded_call_void____cdecl___int__ local_5;
  
  local_c = 3;
  local_10 = 3;
    
  operator___class__lambda_a048d3beccc847880fc8490e18b82769__class__lambda_ec61778202f4f5fc7e7711acc23c3bca___class__lambda_f7496a158712204296dd6628a163878e___
            (&local_5,(_lambda_a048d3beccc847880fc8490e18b82769_ *)&local_10,
             (_lambda_ec61778202f4f5fc7e7711acc23c3bca_ *)&local_5,
             (_lambda_f7496a158712204296dd6628a163878e_ *)&local_c);
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void __cdecl FUN_0040f306(undefined4 param_1)

{
  _DAT_0041f418 = param_1;
  _DAT_0041f41c = param_1;
  _DAT_0041f420 = param_1;
  _DAT_0041f424 = param_1;
  return;
}



int FUN_0040f324(void)

{
  uint uVar1;
  
  uVar1 = FUN_0040b5c7();
  return uVar1 + 8;
}



// WARNING: Function: __SEH_prolog4 replaced with injection: SEH_prolog4
// WARNING: Function: __SEH_epilog4 replaced with injection: EH_epilog3

undefined4 __cdecl FUN_0040f32d(int param_1)

{
  __crt_signal_action_t *p_Var1;
  undefined4 *puVar2;
  uint uVar3;
  undefined4 uVar4;
  char extraout_CL;
  byte bVar5;
  char cVar6;
  __crt_signal_action_t *p_Var7;
  code **ppcVar8;
  code *pcVar9;
  undefined4 uVar10;
  int iVar11;
  undefined4 local_34;
  __crt_signal_action_t *local_30;
  __crt_signal_action_t **local_24;
  char local_1d;
  
  local_24 = (__crt_signal_action_t **)0x0;
  local_34 = 0;
  local_1d = 1;
  if (param_1 < 9) {
    if (param_1 == 8) goto LAB_0040f389;
    if (param_1 != 2) {
      if (param_1 == 4) goto LAB_0040f389;
      if (param_1 != 6) goto LAB_0040f3ad;
    }
LAB_0040f37c:
    ppcVar8 = (code **)get_global_action_nolock(param_1);
    cVar6 = extraout_CL;
  }
  else {
    if (param_1 != 0xb) {
      if ((param_1 != 0xf) && ((param_1 < 0x15 || (0x16 < param_1)))) goto LAB_0040f3ad;
      goto LAB_0040f37c;
    }
LAB_0040f389:
    local_24 = (__crt_signal_action_t **)FUN_0040b71e();
    if (local_24 == (__crt_signal_action_t **)0x0) {
      return 0xffffffff;
    }
    p_Var1 = siglookup(param_1,*local_24);
    if (p_Var1 == (__crt_signal_action_t *)0x0) {
LAB_0040f3ad:
      puVar2 = (undefined4 *)FUN_00407f40();
      *puVar2 = 0x16;
      FUN_00407e83();
      return 0xffffffff;
    }
    ppcVar8 = (code **)(p_Var1 + 8);
    cVar6 = 0;
    local_1d = 0;
  }
  local_30 = (__crt_signal_action_t *)0x0;
  if (cVar6 != 0) {
    ___acrt_lock(3);
    cVar6 = local_1d;
  }
  pcVar9 = *ppcVar8;
  if (cVar6 != 0) {
    bVar5 = (byte)DAT_0041e004 & 0x1f;
    pcVar9 = (code *)(((uint)pcVar9 ^ (uint)DAT_0041e004) >> bVar5 |
                     ((uint)pcVar9 ^ (uint)DAT_0041e004) << 0x20 - bVar5);
    cVar6 = local_1d;
  }
  if (pcVar9 == (code *)0x1) goto LAB_0040f482;
  if (pcVar9 == (code *)0x0) {
    if (cVar6 != 0) {
      ___acrt_unlock(3);
    }
    FUN_004097d4(3);
    pcVar9 = (code *)swi(3);
    uVar4 = (*pcVar9)();
    return uVar4;
  }
  if (((param_1 == 8) || (param_1 == 0xb)) || (param_1 == 4)) {
    local_30 = local_24[1];
    local_24[1] = (__crt_signal_action_t *)0x0;
    if (param_1 == 8) {
      puVar2 = (undefined4 *)FUN_0040f324();
      local_34 = *puVar2;
      puVar2 = (undefined4 *)FUN_0040f324();
      *puVar2 = 0x8c;
      goto LAB_0040f44f;
    }
  }
  else {
LAB_0040f44f:
    if (param_1 == 8) {
      p_Var1 = *local_24;
      p_Var7 = p_Var1 + 0x24;
      while (p_Var7 != p_Var1 + 0x90) {
        *(undefined4 *)(p_Var7 + 8) = 0;
        p_Var7 = p_Var7 + 0xc;
      }
      goto LAB_0040f482;
    }
  }
  *ppcVar8 = DAT_0041e004;
LAB_0040f482:
  FUN_0040f4b7();
  if (pcVar9 != (code *)0x1) {
    if (param_1 == 8) {
      uVar3 = FUN_0040b5c7();
      uVar4 = *(undefined4 *)(uVar3 + 8);
      uVar10 = 8;
      _guard_check_icall();
      (*pcVar9)(uVar10,uVar4);
    }
    else {
      iVar11 = param_1;
      _guard_check_icall();
      (*pcVar9)(iVar11);
      if ((param_1 != 0xb) && (param_1 != 4)) {
        return 0;
      }
    }
    local_24[1] = local_30;
    if (param_1 == 8) {
      uVar3 = FUN_0040b5c7();
      *(undefined4 *)(uVar3 + 8) = local_34;
    }
  }
  return 0;
}



void FUN_0040f4b7(void)

{
  int unaff_EBP;
  
  if (*(char *)(unaff_EBP + -0x19) != 0) {
    ___acrt_unlock(3);
  }
  return;
}



uint __cdecl FUN_0040f514(uint param_1)

{
  uint uVar1;
  uint uVar2;
  
  uVar1 = (param_1 & 0x10) << 3;
  if ((param_1 & 8) != 0) {
    uVar1 = uVar1 | 0x200;
  }
  if ((param_1 & 4) != 0) {
    uVar1 = uVar1 | 0x400;
  }
  if ((param_1 & 2) != 0) {
    uVar1 = uVar1 | 0x800;
  }
  if ((param_1 & 1) != 0) {
    uVar1 = uVar1 | 0x1000;
  }
  if ((param_1 & 0x80000) != 0) {
    uVar1 = uVar1 | 0x100;
  }
  uVar2 = param_1 & 0x300;
  if (uVar2 != 0) {
    if (uVar2 == 0x100) {
      uVar1 = uVar1 | 0x2000;
    }
    else {
      if (uVar2 == 0x200) {
        uVar1 = uVar1 | 0x4000;
      }
      else {
        if (uVar2 == 0x300) {
          uVar1 = uVar1 | 0x6000;
        }
      }
    }
  }
  param_1 = param_1 & 0x3000000;
  if (param_1 == 0x1000000) {
    uVar1 = uVar1 | 0x8040;
  }
  else {
    if (param_1 == 0x2000000) {
      return uVar1 | 0x40;
    }
    if (param_1 == 0x3000000) {
      return uVar1 | 0x8000;
    }
  }
  return uVar1;
}



uint FUN_0040f5bb(void)

{
  uint uVar1;
  uint uVar2;
  uint in_MXCSR;
  ushort in_FPUStatusWord;
  
  if (DAT_0041ec74 < 1) {
    uVar1 = 0;
    if ((in_FPUStatusWord & 0x3f) != 0) {
      uVar1 = ((uint)in_FPUStatusWord & 1) << 4;
      if ((in_FPUStatusWord & 4) != 0) {
        uVar1 = uVar1 | 8;
      }
      if ((in_FPUStatusWord & 8) != 0) {
        uVar1 = uVar1 | 4;
      }
      if ((in_FPUStatusWord & 0x10) != 0) {
        uVar1 = uVar1 | 2;
      }
      if ((in_FPUStatusWord & 0x20) != 0) {
        uVar1 = uVar1 | 1;
      }
      if ((in_FPUStatusWord & 2) != 0) {
        uVar1 = uVar1 | 0x80000;
      }
    }
  }
  else {
    uVar1 = 0;
    uVar2 = 0;
    if ((in_FPUStatusWord & 0x3f) != 0) {
      uVar2 = ((uint)in_FPUStatusWord & 1) << 4;
      if ((in_FPUStatusWord & 4) != 0) {
        uVar2 = uVar2 | 8;
      }
      if ((in_FPUStatusWord & 8) != 0) {
        uVar2 = uVar2 | 4;
      }
      if ((in_FPUStatusWord & 0x10) != 0) {
        uVar2 = uVar2 | 2;
      }
      if ((in_FPUStatusWord & 0x20) != 0) {
        uVar2 = uVar2 | 1;
      }
      if ((in_FPUStatusWord & 2) != 0) {
        uVar2 = uVar2 | 0x80000;
      }
    }
    if ((in_MXCSR & 0x3f) != 0) {
      uVar1 = (in_MXCSR & 1) << 4;
      if ((in_MXCSR & 4) != 0) {
        uVar1 = uVar1 | 8;
      }
      if ((in_MXCSR & 8) != 0) {
        uVar1 = uVar1 | 4;
      }
      if ((in_MXCSR & 0x10) != 0) {
        uVar1 = uVar1 | 2;
      }
      if ((in_MXCSR & 0x20) != 0) {
        uVar1 = uVar1 | 1;
      }
      if ((in_MXCSR & 2) != 0) {
        uVar1 = uVar1 | 0x80000;
      }
    }
    uVar1 = uVar1 | uVar2;
  }
  return uVar1;
}



uint FUN_0040f69b(uint param_1,uint param_2)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  uint in_MXCSR;
  ushort in_FPUControlWord;
  
  uVar3 = (uint)in_FPUControlWord;
  uVar1 = (uVar3 & 1) << 4;
  if ((in_FPUControlWord & 4) != 0) {
    uVar1 = uVar1 | 8;
  }
  if ((in_FPUControlWord & 8) != 0) {
    uVar1 = uVar1 | 4;
  }
  if ((in_FPUControlWord & 0x10) != 0) {
    uVar1 = uVar1 | 2;
  }
  if ((in_FPUControlWord & 0x20) != 0) {
    uVar1 = uVar1 | 1;
  }
  if ((in_FPUControlWord & 2) != 0) {
    uVar1 = uVar1 | 0x80000;
  }
  uVar2 = uVar3 & 0xc00;
  if ((in_FPUControlWord & 0xc00) != 0) {
    if (uVar2 == 0x400) {
      uVar1 = uVar1 | 0x100;
    }
    else {
      if (uVar2 == 0x800) {
        uVar1 = uVar1 | 0x200;
      }
      else {
        if (uVar2 == 0xc00) {
          uVar1 = uVar1 | 0x300;
        }
      }
    }
  }
  if ((in_FPUControlWord & 0x300) == 0) {
    uVar1 = uVar1 | 0x20000;
  }
  else {
    if ((uVar3 & 0x300) == 0x200) {
      uVar1 = uVar1 | 0x10000;
    }
  }
  if ((in_FPUControlWord & 0x1000) != 0) {
    uVar1 = uVar1 | 0x40000;
  }
  uVar3 = ~param_2 & uVar1 | param_1 & param_2;
  if (uVar3 != uVar1) {
    uVar1 = FUN_0040f99a(uVar3);
    uVar3 = (uVar1 & 1) << 4;
    if ((uVar1 & 4) != 0) {
      uVar3 = uVar3 | 8;
    }
    if ((uVar1 & 8) != 0) {
      uVar3 = uVar3 | 4;
    }
    if ((uVar1 & 0x10) != 0) {
      uVar3 = uVar3 | 2;
    }
    if ((uVar1 & 0x20) != 0) {
      uVar3 = uVar3 | 1;
    }
    if ((uVar1 & 2) != 0) {
      uVar3 = uVar3 | 0x80000;
    }
    uVar2 = uVar1 & 0xc00;
    if ((uVar1 & 0xc00) != 0) {
      if (uVar2 == 0x400) {
        uVar3 = uVar3 | 0x100;
      }
      else {
        if (uVar2 == 0x800) {
          uVar3 = uVar3 | 0x200;
        }
        else {
          if (uVar2 == 0xc00) {
            uVar3 = uVar3 | 0x300;
          }
        }
      }
    }
    if ((uVar1 & 0x300) == 0) {
      uVar3 = uVar3 | 0x20000;
    }
    else {
      if ((uVar1 & 0x300) == 0x200) {
        uVar3 = uVar3 | 0x10000;
      }
    }
    if ((uVar1 & 0x1000) != 0) {
      uVar3 = uVar3 | 0x40000;
    }
  }
  if (0 < DAT_0041ec74) {
    uVar1 = in_MXCSR >> 3 & 0x10;
    if ((in_MXCSR & 0x200) != 0) {
      uVar1 = uVar1 | 8;
    }
    if ((in_MXCSR & 0x400) != 0) {
      uVar1 = uVar1 | 4;
    }
    if ((in_MXCSR & 0x800) != 0) {
      uVar1 = uVar1 | 2;
    }
    if ((in_MXCSR & 0x1000) != 0) {
      uVar1 = uVar1 | 1;
    }
    if ((in_MXCSR & 0x100) != 0) {
      uVar1 = uVar1 | 0x80000;
    }
    uVar2 = in_MXCSR & 0x6000;
    if (uVar2 != 0) {
      if (uVar2 == 0x2000) {
        uVar1 = uVar1 | 0x100;
      }
      else {
        if (uVar2 == 0x4000) {
          uVar1 = uVar1 | 0x200;
        }
        else {
          if (uVar2 == 0x6000) {
            uVar1 = uVar1 | 0x300;
          }
        }
      }
    }
    uVar2 = in_MXCSR & 0x8040;
    if (uVar2 == 0x40) {
      uVar1 = uVar1 | 0x2000000;
    }
    else {
      if (uVar2 == 0x8000) {
        uVar1 = uVar1 | 0x3000000;
      }
      else {
        if (uVar2 == 0x8040) {
          uVar1 = uVar1 | 0x1000000;
        }
      }
    }
    uVar2 = ~(param_2 & 0x308031f) & uVar1 | param_2 & 0x308031f & param_1;
    if (uVar2 != uVar1) {
      uVar1 = FUN_0040f514(uVar2);
      FUN_00413822(uVar1);
      uVar1 = in_MXCSR >> 3 & 0x10;
      if ((in_MXCSR & 0x200) != 0) {
        uVar1 = uVar1 | 8;
      }
      if ((in_MXCSR & 0x400) != 0) {
        uVar1 = uVar1 | 4;
      }
      if ((in_MXCSR & 0x800) != 0) {
        uVar1 = uVar1 | 2;
      }
      if ((in_MXCSR & 0x1000) != 0) {
        uVar1 = uVar1 | 1;
      }
      if ((in_MXCSR & 0x100) != 0) {
        uVar1 = uVar1 | 0x80000;
      }
      uVar2 = in_MXCSR & 0x6000;
      if (uVar2 != 0) {
        if (uVar2 == 0x2000) {
          uVar1 = uVar1 | 0x100;
        }
        else {
          if (uVar2 == 0x4000) {
            uVar1 = uVar1 | 0x200;
          }
          else {
            if (uVar2 == 0x6000) {
              uVar1 = uVar1 | 0x300;
            }
          }
        }
      }
      uVar2 = in_MXCSR & 0x8040;
      if (uVar2 == 0x40) {
        uVar1 = uVar1 | 0x2000000;
      }
      else {
        if (uVar2 == 0x8000) {
          uVar1 = uVar1 | 0x3000000;
        }
        else {
          if (uVar2 == 0x8040) {
            uVar1 = uVar1 | 0x1000000;
          }
        }
      }
    }
    uVar2 = uVar1 ^ uVar3;
    uVar3 = uVar1 | uVar3;
    if ((uVar2 & 0x8031f) != 0) {
      uVar3 = uVar3 | 0x80000000;
    }
  }
  return uVar3;
}



uint __cdecl FUN_0040f99a(uint param_1)

{
  uint uVar1;
  uint uVar2;
  
  uVar1 = param_1 >> 4 & 1;
  if ((param_1 & 8) != 0) {
    uVar1 = uVar1 | 4;
  }
  if ((param_1 & 4) != 0) {
    uVar1 = uVar1 | 8;
  }
  if ((param_1 & 2) != 0) {
    uVar1 = uVar1 | 0x10;
  }
  if ((param_1 & 1) != 0) {
    uVar1 = uVar1 | 0x20;
  }
  if ((param_1 & 0x80000) != 0) {
    uVar1 = uVar1 | 2;
  }
  uVar2 = param_1 & 0x300;
  if (uVar2 != 0) {
    if (uVar2 == 0x100) {
      uVar1 = uVar1 | 0x400;
    }
    else {
      if (uVar2 == 0x200) {
        uVar1 = uVar1 | 0x800;
      }
      else {
        if (uVar2 == 0x300) {
          uVar1 = uVar1 | 0xc00;
        }
      }
    }
  }
  if ((param_1 & 0x30000) == 0) {
    uVar1 = uVar1 | 0x300;
  }
  else {
    if ((param_1 & 0x30000) == 0x10000) {
      uVar1 = uVar1 | 0x200;
    }
  }
  if ((param_1 & 0x40000) != 0) {
    uVar1 = uVar1 | 0x1000;
  }
  return uVar1;
}



undefined4 __cdecl FUN_0040fa38(int param_1)

{
  ushort uVar1;
  int iVar2;
  int iVar3;
  ushort uVar4;
  ushort *puVar5;
  int local_10;
  int local_c;
  int local_8;
  
  local_c = 0;
  local_10 = 0xe3;
  while( true ) {
    local_8 = 0x55;
    iVar2 = (local_10 + local_c) / 2;
    puVar5 = (ushort *)(&PTR_DAT_0041a338)[iVar2 * 2];
    iVar3 = param_1 - (int)puVar5;
    do {
      uVar4 = *(ushort *)(iVar3 + (int)puVar5);
      if ((ushort)(uVar4 - 0x41) < 0x1a) {
        uVar4 = uVar4 + 0x20;
      }
      uVar1 = *puVar5;
      if ((ushort)(uVar1 - 0x41) < 0x1a) {
        uVar1 = uVar1 + 0x20;
      }
      puVar5 = puVar5 + 1;
      local_8 = local_8 + -1;
    } while (((local_8 != 0) && (uVar4 != 0)) && (uVar4 == uVar1));
    if ((uint)uVar4 == (uint)uVar1) break;
    if ((int)((uint)uVar4 - (uint)uVar1) < 0) {
      local_10 = iVar2 + -1;
    }
    else {
      local_c = iVar2 + 1;
    }
    if (local_10 < local_c) {
      return 0xffffffff;
    }
  }
  return *(undefined4 *)(&UNK_0041a33c + iVar2 * 8);
}



// Library Function - Multiple Matches With Different Base Names
// Name: ?_AtlDownlevelLocaleNameToLCID@ATL@@YAKPB_W@Z, ___acrt_DownlevelLocaleNameToLCID,
// ___crtDownlevelLocaleNameToLCID
// Library: Visual Studio 2015 Release

undefined4 __cdecl FID_conflict__AtlDownlevelLocaleNameToLCID(int param_1)

{
  uint uVar1;
  
  if (param_1 != 0) {
    uVar1 = FUN_0040fa38(param_1);
    if ((-1 < (int)uVar1) && (uVar1 < 0xe4)) {
      return *(undefined4 *)(&DAT_00419218 + uVar1 * 8);
    }
  }
  return 0;
}



// Library Function - Single Match
// Name: __fclose_nolock
// Library: Visual Studio 2015 Release

int __cdecl __fclose_nolock(FILE *_File)

{
  undefined4 *puVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  
  if (_File == (FILE *)0x0) {
    puVar1 = (undefined4 *)FUN_00407f40();
    *puVar1 = 0x16;
    FUN_00407e83();
    iVar2 = -1;
  }
  else {
    iVar2 = -1;
    if (((uint)_File->_flag >> 0xd & 1) != 0) {
      iVar2 = FUN_0040a7bb(_File);
      ___acrt_stdio_free_buffer_nolock((undefined4 *)_File);
      uVar3 = __fileno(_File);
      iVar4 = FUN_004139c6(uVar3);
      if (iVar4 < 0) {
        iVar2 = -1;
      }
      else {
        if (_File->_tmpfname != (char *)0x0) {
          FID_conflict__free(_File->_tmpfname);
          _File->_tmpfname = (char *)0x0;
        }
      }
    }
    __acrt_stdio_free_stream(SUB41(_File,0));
  }
  return iVar2;
}



// WARNING: Function: __SEH_prolog4 replaced with injection: SEH_prolog4
// WARNING: Function: __SEH_epilog4 replaced with injection: EH_epilog3

int __cdecl FUN_0040fb8f(FILE *param_1)

{
  undefined4 *puVar1;
  int iVar2;
  
  if (param_1 == (FILE *)0x0) {
    puVar1 = (undefined4 *)FUN_00407f40();
    *puVar1 = 0x16;
    FUN_00407e83();
  }
  else {
    if (((uint)param_1->_flag >> 0xc & 1) == 0) {
      __lock_file(param_1);
      iVar2 = __fclose_nolock(param_1);
      FUN_0040fbff();
      return iVar2;
    }
    __acrt_stdio_free_stream(SUB41(param_1,0));
  }
  return -1;
}



void FUN_0040fbff(void)

{
  int unaff_EBP;
  
  FUN_004056fc(*(int *)(unaff_EBP + -0x20));
  return;
}



// WARNING: Function: __SEH_prolog4 replaced with injection: SEH_prolog4
// WARNING: Function: __SEH_epilog4 replaced with injection: EH_epilog3
// Library Function - Single Match
// Name: 
??$?RV<lambda_61cee617f5178ae960314fd4d05640a0>@@AAV<lambda_6978c1fb23f02e42e1d9e99668cc68aa>@@V<lambda_9cd88cf8ad10232537feb2133f08c833>@@@?$__crt_seh_guarded_call@H@@QAEH$$QAV<lambda_61cee617f5178ae960314fd4d05640a0>@@AAV<lambda_6978c1fb23f02e42e1d9e99668cc68aa>@@$$QAV<lambda_9cd88cf8ad10232537feb2133f08c833>@@@Z
// Library: Visual Studio 2015 Release
// public: int __thiscall __crt_seh_guarded_call<int>::operator()<class
// <lambda_61cee617f5178ae960314fd4d05640a0>,class <lambda_6978c1fb23f02e42e1d9e99668cc68aa>&,class
// <lambda_9cd88cf8ad10232537feb2133f08c833>>(class
// <lambda_61cee617f5178ae960314fd4d05640a0>&&,class
// <lambda_6978c1fb23f02e42e1d9e99668cc68aa>&,class <lambda_9cd88cf8ad10232537feb2133f08c833>&&)

int __thiscall

operator___class__lambda_61cee617f5178ae960314fd4d05640a0__class__lambda_6978c1fb23f02e42e1d9e99668cc68aa___class__lambda_9cd88cf8ad10232537feb2133f08c833___
          (__crt_seh_guarded_call_int_ *this,_lambda_61cee617f5178ae960314fd4d05640a0_ *param_1,
          _lambda_6978c1fb23f02e42e1d9e99668cc68aa_ *param_2,
          _lambda_9cd88cf8ad10232537feb2133f08c833_ *param_3)

{
  uint _FileHandle;
  HANDLE hFile;
  BOOL BVar1;
  DWORD *pDVar2;
  DWORD DVar3;
  undefined4 *puVar4;
  int iVar5;
  
  iVar5 = 0;
  ___acrt_lowio_lock_fh(*(uint *)param_1);
  _FileHandle = **(uint **)param_2;
  if ((*(byte *)((&DAT_0041f080)[(int)_FileHandle >> 6] + 0x28 + (_FileHandle & 0x3f) * 0x38) & 1)
      != 0) {
    hFile = (HANDLE)__get_osfhandle(_FileHandle);
    BVar1 = FlushFileBuffers(hFile);
    if (BVar1 != 0) goto LAB_0040fc79;
    pDVar2 = (DWORD *)FUN_00407f2d();
    DVar3 = GetLastError();
    *pDVar2 = DVar3;
  }
  puVar4 = (undefined4 *)FUN_00407f40();
  *puVar4 = 9;
  iVar5 = -1;
LAB_0040fc79:
  FUN_0040fc95();
  return iVar5;
}



void FUN_0040fc95(void)

{
  int unaff_EBP;
  
  ___acrt_lowio_unlock_fh(**(uint **)(unaff_EBP + 0x10));
  return;
}



int __cdecl FUN_0040fca1(uint param_1)

{
  int iVar1;
  undefined4 *puVar2;
  uint local_14;
  uint *local_10;
  uint local_c;
  __crt_seh_guarded_call_int_ local_5;
  
  if (param_1 == 0xfffffffe) {
    puVar2 = (undefined4 *)FUN_00407f40();
    *puVar2 = 9;
  }
  else {
    if (((-1 < (int)param_1) && (param_1 < DAT_0041f280)) &&
       ((*(byte *)((&DAT_0041f080)[(int)param_1 >> 6] + 0x28 + (param_1 & 0x3f) * 0x38) & 1) != 0))
    {
      local_10 = &param_1;
      local_c = param_1;
      local_14 = param_1;
      iVar1 = 
              operator___class__lambda_61cee617f5178ae960314fd4d05640a0__class__lambda_6978c1fb23f02e42e1d9e99668cc68aa___class__lambda_9cd88cf8ad10232537feb2133f08c833___
                        (&local_5,(_lambda_61cee617f5178ae960314fd4d05640a0_ *)&local_14,
                         (_lambda_6978c1fb23f02e42e1d9e99668cc68aa_ *)&local_10,
                         (_lambda_9cd88cf8ad10232537feb2133f08c833_ *)&local_c);
      return iVar1;
    }
    puVar2 = (undefined4 *)FUN_00407f40();
    *puVar2 = 9;
    FUN_00407e83();
  }
  return -1;
}



void __cdecl FUN_0040fd20(DWORD *param_1,uint param_2,undefined8 *param_3,int param_4)

{
  int iVar1;
  undefined *puVar2;
  byte bVar3;
  undefined8 **ppuVar4;
  ushort *puVar5;
  BOOL BVar6;
  undefined4 *this;
  int iVar7;
  int iVar8;
  DWORD DVar9;
  undefined8 *puVar10;
  undefined8 *puVar11;
  byte *pbVar12;
  undefined4 local_90;
  undefined4 local_8c;
  undefined4 local_88;
  undefined4 local_84;
  int local_80;
  UINT local_7c;
  undefined8 *local_78;
  HANDLE local_74;
  undefined8 *local_70;
  int local_6c;
  undefined8 *local_68;
  undefined8 *local_64;
  DWORD local_60;
  DWORD local_5c;
  DWORD local_58;
  int local_54;
  int local_50;
  undefined4 local_4c;
  int local_48;
  int local_44;
  char local_3c;
  undefined8 *local_38;
  undefined8 *local_34;
  undefined2 local_30;
  uint local_2c;
  undefined8 *local_28;
  byte local_21;
  CHAR local_20 [8];
  undefined local_18;
  byte local_17;
  undefined8 local_10;
  uint local_8;
  
  local_8 = DAT_0041e004 ^ (uint)&stack0xfffffffc;
  local_50 = (param_2 & 0x3f) * 0x38;
  local_54 = (int)param_2 >> 6;
  local_70 = param_3;
  local_74 = *(HANDLE *)(local_50 + 0x18 + (&DAT_0041f080)[local_54]);
  local_68 = (undefined8 *)(param_4 + (int)param_3);
  local_7c = GetConsoleCP();
  iVar8 = 0;
  FUN_00405ed6(&local_48,(undefined4 *)0x0);
  local_60 = 0;
  local_80 = *(int *)(local_44 + 8);
  local_5c = 0;
  local_58 = 0;
  local_28 = local_70;
  puVar11 = local_70;
  if ((undefined8 *)(param_4 + (int)param_3) <= local_70) {
LAB_004100a4:
    if (local_3c != 0) {
      *(uint *)(local_48 + 0x350) = *(uint *)(local_48 + 0x350) & 0xfffffffd;
    }
    *param_1 = local_60;
    param_1[1] = local_5c;
    param_1[2] = local_58;
    FUN_00401f78();
    return;
  }
LAB_0040fd9e:
  puVar10 = local_28;
  DVar9 = local_5c;
  local_30 = local_30 & 0xff | (ushort)*(byte *)puVar11 << 8;
  local_4c = 0;
  local_2c = 1;
  local_34 = (undefined8 *)(&DAT_0041f080)[local_54];
  if (local_80 == 0xfde9) {
    local_6c = (int)local_34 + local_50 + 0x2e;
    local_2c = 0;
    do {
      if (*(char *)(local_6c + local_2c) == 0) break;
      local_2c = local_2c + 1;
    } while ((int)local_2c < 5);
    puVar11 = (undefined8 *)((int)local_68 - (int)local_28);
    if ((int)local_2c < 1) {
      local_34 = (undefined8 *)((int)(char)(&DAT_0041e798)[(uint)*(byte *)local_28] + 1);
      if ((int)puVar11 < (int)local_34) {
        if (0 < (int)puVar11) {
          do {
            iVar7 = local_50 + iVar8;
            puVar2 = (undefined *)(iVar8 + (int)local_28);
            iVar8 = iVar8 + 1;
            *(undefined *)(iVar7 + 0x2e + (&DAT_0041f080)[local_54]) = *puVar2;
          } while (iVar8 < (int)puVar11);
        }
        goto LAB_00410046;
      }
      local_90 = 0;
      this = &local_90;
      local_8c = 0;
      local_38 = local_28;
      ppuVar4 = &local_38;
      puVar10 = local_28;
      puVar11 = local_34;
    }
    else {
      local_38 = (undefined8 *)
                 ((int)(char)(&DAT_0041e798)[(uint)*(byte *)(local_50 + 0x2e + (int)local_34)] + 1);
      local_34 = (undefined8 *)((int)local_38 - local_2c);
      if ((int)puVar11 < (int)local_34) {
        if (0 < (int)puVar11) {
          do {
            iVar7 = local_50 + iVar8;
            puVar2 = (undefined *)(iVar8 + (int)local_28);
            iVar8 = iVar8 + 1;
            *(undefined *)(iVar7 + local_2c + 0x2e + (&DAT_0041f080)[local_54]) = *puVar2;
          } while (iVar8 < (int)puVar11);
        }
LAB_00410046:
        local_5c = local_5c + (int)puVar11;
        goto LAB_004100a4;
      }
      iVar7 = 0;
      if (0 < (int)local_2c) {
        do {
          *(undefined *)((int)&local_10 + iVar7) = *(undefined *)(local_6c + iVar7);
          iVar7 = iVar7 + 1;
        } while (iVar7 < (int)local_2c);
      }
      if (0 < (int)local_34) {
        copy_buffer((undefined8 *)((int)&local_10 + local_2c),local_28,(uint)local_34);
      }
      if (0 < (int)local_2c) {
        iVar7 = 0;
        do {
          iVar1 = local_50 + iVar7;
          iVar7 = iVar7 + 1;
          *(undefined *)(iVar1 + 0x2e + (&DAT_0041f080)[local_54]) = 0;
          puVar10 = local_28;
        } while (iVar7 < (int)local_2c);
      }
      local_78 = &local_10;
      local_88 = 0;
      this = &local_88;
      local_84 = 0;
      ppuVar4 = &local_78;
      puVar11 = local_38;
    }
    local_2c = (uint)(puVar11 == (undefined8 *)&DAT_00000004) + 1;
    iVar7 = FUN_00410a52(this,(ushort *)&local_4c,(byte **)ppuVar4,local_2c,(byte **)this);
    if (iVar7 == -1) goto LAB_004100a4;
    puVar11 = (undefined8 *)((int)puVar10 + (int)local_34 + -1);
  }
  else {
    bVar3 = *(byte *)(local_50 + 0x2d + (int)local_34);
    if ((bVar3 & 4) == 0) {
      local_21 = *(byte *)puVar11;
      puVar5 = ___pctype_func();
      if ((short)puVar5[(uint)local_21] < 0) {
        local_38 = (undefined8 *)((int)puVar11 + 1);
        if (local_38 < local_68) {
          iVar7 = FUN_0040ae47((ushort *)&local_4c,(byte *)puVar11,(byte *)0x2);
          puVar11 = local_38;
          if (iVar7 != -1) goto LAB_0040ff75;
        }
        else {
          *(byte *)(local_50 + 0x2e + (&DAT_0041f080)[local_54]) = local_21;
          pbVar12 = (byte *)(local_50 + 0x2d + (&DAT_0041f080)[local_54]);
          *pbVar12 = *pbVar12 | 4;
          local_5c = DVar9 + 1;
        }
        goto LAB_004100a4;
      }
      pbVar12 = (byte *)0x1;
      puVar10 = puVar11;
    }
    else {
      local_18 = *(undefined *)(local_50 + 0x2e + (int)local_34);
      local_17 = *(byte *)puVar11;
      pbVar12 = (byte *)0x2;
      *(byte *)(local_50 + 0x2d + (int)local_34) = bVar3 & 0xfb;
      puVar10 = (undefined8 *)&local_18;
    }
    iVar7 = FUN_0040ae47((ushort *)&local_4c,(byte *)puVar10,pbVar12);
    if (iVar7 == -1) goto LAB_004100a4;
  }
LAB_0040ff75:
  puVar11 = (undefined8 *)((int)puVar11 + 1);
  local_28 = puVar11;
  local_38 = (undefined8 *)FUN_0040c3b9(local_7c,0,(LPCWSTR)&local_4c,local_2c,local_20,5,0,0);
  if (local_38 == (undefined8 *)0x0) goto LAB_004100a4;
  BVar6 = WriteFile(local_74,local_20,(DWORD)local_38,(LPDWORD)&local_64,(LPOVERLAPPED)0x0);
  if (BVar6 == 0) {
LAB_00410098:
    local_60 = GetLastError();
    goto LAB_004100a4;
  }
  DVar9 = (local_58 - (int)local_70) + (int)puVar11;
  local_5c = DVar9;
  if (local_64 < local_38) goto LAB_004100a4;
  if (local_30._1_1_ == '\n') {
    local_30 = 0xd;
    local_5c = DVar9;
    BVar6 = WriteFile(local_74,&local_30,1,(LPDWORD)&local_64,(LPOVERLAPPED)0x0);
    if (BVar6 == 0) goto LAB_00410098;
    if (local_64 == (undefined8 *)0x0) goto LAB_004100a4;
    local_58 = local_58 + 1;
    DVar9 = DVar9 + 1;
  }
  local_5c = DVar9;
  if (local_68 <= puVar11) goto LAB_004100a4;
  goto LAB_0040fd9e;
}



DWORD * __cdecl FUN_004100cc(DWORD *param_1,ushort *param_2,int param_3)

{
  ushort uVar1;
  ushort *puVar2;
  uint uVar3;
  DWORD DVar4;
  
  *param_1 = 0;
  param_1[1] = 0;
  param_1[2] = 0;
  puVar2 = (ushort *)(param_3 + (int)param_2);
  if (param_2 < puVar2) {
    do {
      uVar1 = *param_2;
      uVar3 = FUN_00413ce2(uVar1);
      if ((ushort)uVar3 != uVar1) {
LAB_00410125:
        DVar4 = GetLastError();
        *param_1 = DVar4;
        return param_1;
      }
      param_1[1] = param_1[1] + 2;
      if (uVar1 == 10) {
        uVar3 = FUN_00413ce2(0xd);
        if ((short)uVar3 != 0xd) goto LAB_00410125;
        param_1[1] = param_1[1] + 1;
        param_1[2] = param_1[2] + 1;
      }
      param_2 = param_2 + 1;
    } while (param_2 < puVar2);
  }
  return param_1;
}



uint __cdecl FUN_00410136(uint param_1)

{
  uint uVar1;
  int iVar2;
  int iVar3;
  DWORD local_8;
  
  uVar1 = __isatty(param_1);
  if (uVar1 != 0) {
    iVar3 = (int)param_1 >> 6;
    iVar2 = (param_1 & 0x3f) * 0x38;
    uVar1 = (&DAT_0041f080)[iVar3];
    if ((*(char *)(uVar1 + 0x28 + iVar2) < 0) &&
       (((uVar1 = FUN_0040b5c7(), *(int *)(*(int *)(uVar1 + 0x4c) + 0xa8) != 0 ||
         (uVar1 = (&DAT_0041f080)[iVar3], *(char *)(uVar1 + 0x29 + iVar2) != 0)) &&
        (uVar1 = GetConsoleMode(*(HANDLE *)((&DAT_0041f080)[iVar3] + 0x18 + iVar2),&local_8),
        uVar1 != 0)))) {
      return CONCAT31((int3)(uVar1 >> 8),1);
    }
  }
  return uVar1 & 0xffffff00;
}



// WARNING: Function: __alloca_probe replaced with injection: alloca_probe

void __cdecl FUN_004101a9(DWORD *param_1,uint param_2,char *param_3,int param_4)

{
  char cVar1;
  char *hFile;
  BOOL BVar2;
  DWORD DVar3;
  char *pcVar4;
  char *pcVar5;
  char *local_140c;
  char local_1408 [5119];
  char local_9;
  uint local_8;
  
  local_8 = DAT_0041e004 ^ (uint)&stack0xfffffffc;
  hFile = *(char **)((&DAT_0041f080)[(int)param_2 >> 6] + 0x18 + (param_2 & 0x3f) * 0x38);
  pcVar4 = param_3 + param_4;
  *param_1 = 0;
  param_1[1] = 0;
  param_1[2] = 0;
  local_140c = hFile;
  if (pcVar4 <= param_3) {
LAB_00410273:
    FUN_00401f78();
    return;
  }
  do {
    pcVar5 = local_1408;
    do {
      if (pcVar4 <= param_3) break;
      cVar1 = *param_3;
      param_3 = param_3 + 1;
      if (cVar1 == '\n') {
        param_1[2] = param_1[2] + 1;
        *pcVar5 = '\r';
        pcVar5 = pcVar5 + 1;
      }
      *pcVar5 = cVar1;
      pcVar5 = pcVar5 + 1;
    } while (pcVar5 < &local_9);
    BVar2 = WriteFile(hFile,local_1408,(DWORD)(pcVar5 + -(int)local_1408),(LPDWORD)&local_140c,
                      (LPOVERLAPPED)0x0);
    if (BVar2 == 0) {
      DVar3 = GetLastError();
      *param_1 = DVar3;
      goto LAB_00410273;
    }
    *(char **)(param_1 + 1) = local_140c + (int)param_1[1];
    if ((local_140c < pcVar5 + -(int)local_1408) || (pcVar4 <= param_3)) goto LAB_00410273;
  } while( true );
}



// WARNING: Function: __alloca_probe replaced with injection: alloca_probe

void __cdecl FUN_00410286(DWORD *param_1,uint param_2,short *param_3,int param_4)

{
  short sVar1;
  BOOL BVar2;
  DWORD DVar3;
  short *psVar4;
  short *psVar5;
  short *local_1410;
  HANDLE local_140c;
  short local_1408 [2559];
  short local_a;
  uint local_8;
  
  local_8 = DAT_0041e004 ^ (uint)&stack0xfffffffc;
  local_140c = *(HANDLE *)((&DAT_0041f080)[(int)param_2 >> 6] + 0x18 + (param_2 & 0x3f) * 0x38);
  psVar4 = (short *)(param_4 + (int)param_3);
  *param_1 = 0;
  param_1[1] = 0;
  param_1[2] = 0;
  do {
    if (psVar4 <= param_3) break;
    psVar5 = local_1408;
    do {
      if (psVar4 <= param_3) break;
      sVar1 = *param_3;
      param_3 = param_3 + 1;
      if (sVar1 == 10) {
        param_1[2] = param_1[2] + 2;
        *psVar5 = 0xd;
        psVar5 = psVar5 + 1;
      }
      *psVar5 = sVar1;
      psVar5 = psVar5 + 1;
    } while (psVar5 < &local_a);
    BVar2 = WriteFile(local_140c,local_1408,(DWORD)(short *)((int)psVar5 - (int)local_1408),
                      (LPDWORD)&local_1410,(LPOVERLAPPED)0x0);
    if (BVar2 == 0) {
      DVar3 = GetLastError();
      *param_1 = DVar3;
      break;
    }
    param_1[1] = param_1[1] + (int)local_1410;
  } while ((short *)((int)psVar5 - (int)local_1408) <= local_1410);
  FUN_00401f78();
  return;
}



// WARNING: Function: __alloca_probe replaced with injection: alloca_probe

void __cdecl FUN_00410371(DWORD *param_1,uint param_2,WCHAR *param_3,int param_4)

{
  WCHAR WVar1;
  WCHAR *pWVar2;
  uint uVar3;
  BOOL BVar4;
  DWORD DVar5;
  uint uVar6;
  WCHAR *pWVar7;
  DWORD local_1418;
  HANDLE local_1414;
  WCHAR *local_1410;
  CHAR local_140c [3416];
  WCHAR local_6b4 [852];
  WCHAR local_c [2];
  uint local_8;
  
  local_8 = DAT_0041e004 ^ (uint)&stack0xfffffffc;
  local_1414 = *(HANDLE *)((&DAT_0041f080)[(int)param_2 >> 6] + 0x18 + (param_2 & 0x3f) * 0x38);
  local_1410 = (WCHAR *)(param_4 + (int)param_3);
  *param_1 = 0;
  param_1[1] = 0;
  param_1[2] = 0;
  pWVar7 = param_3;
  if (param_3 < local_1410) {
    do {
      pWVar2 = local_6b4;
      do {
        if (local_1410 <= pWVar7) break;
        WVar1 = *pWVar7;
        pWVar7 = pWVar7 + 1;
        if (WVar1 == L'\n') {
          *pWVar2 = L'\r';
          pWVar2 = pWVar2 + 1;
        }
        *pWVar2 = WVar1;
        pWVar2 = pWVar2 + 1;
      } while (pWVar2 < local_c);
      uVar3 = FUN_0040c3b9(0xfde9,0,local_6b4,(int)((int)pWVar2 - (int)local_6b4) >> 1,local_140c,
                           0xd55,0,0);
      if (uVar3 == 0) {
LAB_0041048a:
        DVar5 = GetLastError();
        *param_1 = DVar5;
        break;
      }
      uVar6 = 0;
      if (uVar3 != 0) {
        do {
          BVar4 = WriteFile(local_1414,local_140c + uVar6,uVar3 - uVar6,&local_1418,
                            (LPOVERLAPPED)0x0);
          if (BVar4 == 0) goto LAB_0041048a;
          uVar6 = uVar6 + local_1418;
        } while (uVar6 < uVar3);
      }
      *(WCHAR **)(param_1 + 1) = (WCHAR *)((int)pWVar7 - (int)param_3);
    } while (pWVar7 < local_1410);
  }
  FUN_00401f78();
  return;
}



// WARNING: Function: __SEH_prolog4 replaced with injection: SEH_prolog4
// WARNING: Function: __SEH_epilog4 replaced with injection: EH_epilog3

int __cdecl FUN_004104a5(uint param_1,undefined8 *param_2,uint param_3)

{
  undefined4 *puVar1;
  int iVar2;
  int iVar3;
  
  if (param_1 == 0xfffffffe) {
    puVar1 = (undefined4 *)FUN_00407f2d();
    *puVar1 = 0;
    puVar1 = (undefined4 *)FUN_00407f40();
    *puVar1 = 9;
  }
  else {
    if ((-1 < (int)param_1) && (param_1 < DAT_0041f280)) {
      iVar2 = (param_1 & 0x3f) * 0x38;
      if ((*(byte *)((&DAT_0041f080)[(int)param_1 >> 6] + 0x28 + iVar2) & 1) != 0) {
        ___acrt_lowio_lock_fh(param_1);
        iVar3 = -1;
        if ((*(byte *)((&DAT_0041f080)[(int)param_1 >> 6] + 0x28 + iVar2) & 1) == 0) {
          puVar1 = (undefined4 *)FUN_00407f40();
          *puVar1 = 9;
          puVar1 = (undefined4 *)FUN_00407f2d();
          *puVar1 = 0;
        }
        else {
          iVar3 = FUN_0041058d(param_1,param_2,param_3);
        }
        FUN_00410564();
        return iVar3;
      }
    }
    puVar1 = (undefined4 *)FUN_00407f2d();
    *puVar1 = 0;
    puVar1 = (undefined4 *)FUN_00407f40();
    *puVar1 = 9;
    FUN_00407e83();
  }
  return -1;
}



void FUN_00410564(void)

{
  uint unaff_ESI;
  
  ___acrt_lowio_unlock_fh(unaff_ESI);
  return;
}



int __cdecl FUN_0041058d(uint param_1,undefined8 *param_2,uint param_3)

{
  char cVar1;
  undefined8 *puVar2;
  uint uVar3;
  DWORD *pDVar4;
  BOOL BVar5;
  undefined4 *puVar6;
  DWORD local_2c;
  DWORD local_28;
  undefined4 uStack36;
  DWORD local_20;
  DWORD local_1c;
  DWORD local_18;
  uint local_14;
  int local_10;
  int local_c;
  undefined8 *local_8;
  
  local_8 = param_2;
  local_14 = param_3;
  if (param_3 == 0) {
    return 0;
  }
  if (param_2 == (undefined8 *)0x0) {
LAB_004105b3:
    puVar6 = (undefined4 *)FUN_00407f2d();
    *puVar6 = 0;
    puVar6 = (undefined4 *)FUN_00407f40();
    *puVar6 = 0x16;
    FUN_00407e83();
    return -1;
  }
  local_c = (int)param_1 >> 6;
  local_10 = (param_1 & 0x3f) * 0x38;
  cVar1 = *(char *)((&DAT_0041f080)[local_c] + 0x29 + local_10);
  if (((cVar1 == 2) || (cVar1 == 1)) && ((~param_3 & 1) == 0)) goto LAB_004105b3;
  if ((*(byte *)((&DAT_0041f080)[local_c] + 0x28 + local_10) & 0x20) != 0) {
    __lseeki64_nolock(param_1,0,2);
  }
  local_20 = 0;
  local_1c = 0;
  local_18 = 0;
  uVar3 = FUN_00410136(param_1);
  puVar2 = local_8;
  if ((char)uVar3 == 0) {
    if (*(char *)((&DAT_0041f080)[local_c] + 0x28 + local_10) < 0) {
      if (cVar1 == 0) {
        pDVar4 = (DWORD *)FUN_004101a9(&local_2c,param_1,(char *)local_8,local_14);
      }
      else {
        if (cVar1 == 1) {
          pDVar4 = (DWORD *)FUN_00410371(&local_2c,param_1,(WCHAR *)local_8,local_14);
        }
        else {
          if (cVar1 != 2) goto LAB_00410700;
          pDVar4 = (DWORD *)FUN_00410286(&local_2c,param_1,(short *)local_8,local_14);
        }
      }
    }
    else {
      local_2c = 0;
      local_28 = 0;
      uStack36 = 0;
      BVar5 = WriteFile(*(HANDLE *)((&DAT_0041f080)[local_c] + 0x18 + local_10),local_8,local_14,
                        &local_28,(LPOVERLAPPED)0x0);
      if (BVar5 == 0) {
        local_2c = GetLastError();
      }
      pDVar4 = &local_2c;
    }
  }
  else {
    if (cVar1 == 0) {
      pDVar4 = (DWORD *)FUN_0040fd20(&local_2c,param_1,local_8,local_14);
    }
    else {
      if (1 < (byte)(cVar1 - 1U)) goto LAB_00410700;
      pDVar4 = FUN_004100cc(&local_2c,(ushort *)local_8,local_14);
    }
  }
  local_20 = *pDVar4;
  local_1c = pDVar4[1];
  local_18 = pDVar4[2];
LAB_00410700:
  if (local_1c != 0) {
    return local_1c - local_18;
  }
  if (local_20 != 0) {
    if (local_20 == 5) {
      puVar6 = (undefined4 *)FUN_00407f40();
      *puVar6 = 9;
      puVar6 = (undefined4 *)FUN_00407f2d();
      *puVar6 = 5;
      return -1;
    }
    ___acrt_errno_map_os_error(local_20);
    return -1;
  }
  if (((*(byte *)((&DAT_0041f080)[local_c] + 0x28 + local_10) & 0x40) != 0) &&
     (*(char *)puVar2 == 0x1a)) {
    return 0;
  }
  puVar6 = (undefined4 *)FUN_00407f40();
  *puVar6 = 0x1c;
  puVar6 = (undefined4 *)FUN_00407f2d();
  *puVar6 = 0;
  return -1;
}



void __cdecl FUN_00410771(int param_1,undefined4 *param_2)

{
  int local_14;
  int *local_10;
  char local_8;
  
  FUN_00405ed6(&local_14,param_2);
  FUN_00407be2(*local_10,param_1,0x8000);
  if (local_8 != 0) {
    *(uint *)(local_14 + 0x350) = *(uint *)(local_14 + 0x350) & 0xfffffffd;
  }
  return;
}



// WARNING: Function: __alloca_probe_16 replaced with injection: alloca_probe

void __cdecl
FUN_004107ad(int *param_1,int param_2,uint param_3,char *param_4,int param_5,LPWSTR param_6,
            int param_7,uint param_8,int param_9)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  undefined4 *puVar4;
  undefined4 *puVar5;
  undefined4 *puVar6;
  int local_c;
  uint local_8;
  
  local_8 = DAT_0041e004 ^ (uint)&stack0xfffffffc;
  iVar3 = param_5;
  if (0 < param_5) {
    iVar1 = ___strncnt(param_4,param_5);
    iVar3 = iVar1 + 1;
    if (param_5 <= iVar1) {
      iVar3 = iVar1;
    }
  }
  if (param_8 == 0) {
    param_8 = *(uint *)(*param_1 + 8);
  }
  local_c = FUN_0040e0f3(param_8,(uint)(param_9 != 0) * 8 + 1,param_4,iVar3,(LPWSTR)0x0,0);
  if (local_c == 0) goto LAB_00410974;
  uVar2 = local_c * 2 + 8;
  uVar2 = -(uint)((uint)(local_c * 2) < uVar2) & uVar2;
  if (uVar2 == 0) {
    puVar4 = (undefined4 *)0x0;
  }
  else {
    if (uVar2 < 0x401) {
      puVar6 = (undefined4 *)&stack0xffffffe8;
      puVar4 = (undefined4 *)&stack0xffffffe8;
      if ((undefined *)register0x00000010 != (undefined *)0x18) {
LAB_00410857:
        puVar4 = puVar6 + 2;
      }
    }
    else {
      puVar4 = (undefined4 *)__malloc_base(uVar2);
      if (puVar4 != (undefined4 *)0x0) {
        *puVar4 = 0xdddd;
        puVar6 = puVar4;
        goto LAB_00410857;
      }
    }
  }
  if (((puVar4 != (undefined4 *)0x0) &&
      (iVar1 = FUN_0040e0f3(param_8,1,param_4,iVar3,(LPWSTR)puVar4,local_c), iVar3 = local_c,
      iVar1 != 0)) &&
     (iVar1 = FUN_0040a460(param_2,param_3,(LPCWSTR)puVar4,local_c,(LPWSTR)0x0,0,0,0,0), iVar1 != 0)
     ) {
    if ((param_3 & 0x400) == 0) {
      uVar2 = iVar1 * 2 + 8;
      uVar2 = -(uint)((uint)(iVar1 * 2) < uVar2) & uVar2;
      if (uVar2 == 0) {
        puVar6 = (undefined4 *)0x0;
LAB_00410924:
        if ((puVar6 != (undefined4 *)0x0) &&
           (iVar3 = FUN_0040a460(param_2,param_3,(LPCWSTR)puVar4,local_c,(LPWSTR)puVar6,iVar1,0,0,0)
           , iVar3 != 0)) {
          if (param_7 == 0) {
            param_7 = 0;
            param_6 = (LPWSTR)0x0;
          }
          iVar3 = FUN_0040c3b9(param_8,0,(LPCWSTR)puVar6,iVar1,(LPSTR)param_6,param_7,0,0);
          if (iVar3 != 0) {
            FUN_0040ecb8((int)puVar6);
            goto LAB_0041096b;
          }
        }
      }
      else {
        if (uVar2 < 0x401) {
          puVar5 = (undefined4 *)&stack0xffffffe8;
          puVar6 = (undefined4 *)&stack0xffffffe8;
          if ((undefined *)register0x00000010 != (undefined *)0x18) {
LAB_0041091d:
            puVar6 = puVar5 + 2;
            goto LAB_00410924;
          }
        }
        else {
          puVar6 = (undefined4 *)__malloc_base(uVar2);
          if (puVar6 != (undefined4 *)0x0) {
            *puVar6 = 0xdddd;
            puVar5 = puVar6;
            goto LAB_0041091d;
          }
        }
      }
      FUN_0040ecb8((int)puVar6);
    }
    else {
      if ((param_7 != 0) && (iVar1 <= param_7)) {
        FUN_0040a460(param_2,param_3,(LPCWSTR)puVar4,iVar3,param_6,param_7,0,0,0);
      }
    }
  }
LAB_0041096b:
  FUN_0040ecb8((int)puVar4);
LAB_00410974:
  FUN_00401f78();
  return;
}



// Library Function - Single Match
// Name: ___acrt_LCMapStringA
// Library: Visual Studio 2015 Release

void __cdecl
___acrt_LCMapStringA
          (undefined4 *param_1,int param_2,uint param_3,char *param_4,int param_5,LPWSTR param_6,
          int param_7,uint param_8,int param_9)

{
  int local_14;
  int local_10 [2];
  char local_8;
  
  FUN_00405ed6(&local_14,param_1);
  FUN_004107ad(local_10,param_2,param_3,param_4,param_5,param_6,param_7,param_8,param_9);
  if (local_8 != 0) {
    *(uint *)(local_14 + 0x350) = *(uint *)(local_14 + 0x350) & 0xfffffffd;
  }
  return;
}



int FUN_004109e4(char *param_1)

{
  int iVar1;
  
  if (*param_1 == 0) {
    iVar1 = 1;
  }
  else {
    if (param_1[1] == 0) {
      iVar1 = 2;
    }
    else {
      iVar1 = (uint)(param_1[2] != 0) + 3;
    }
  }
  return iVar1;
}



uint __cdecl FUN_00410a10(undefined2 *param_1,byte *param_2,byte *param_3,byte **param_4)

{
  uint uVar1;
  undefined2 uVar2;
  uint local_8;
  
  uVar1 = FUN_00413d32((uint)&local_8,param_2,param_3,param_4);
  if (uVar1 < 5) {
    uVar2 = (undefined2)local_8;
    if (0xffff < local_8) {
      uVar2 = 0xfffd;
    }
    if (param_1 != (undefined2 *)0x0) {
      *param_1 = uVar2;
    }
  }
  return uVar1;
}



int __thiscall FUN_00410a52(void *this,ushort *param_1,byte **param_2,uint param_3,byte **param_4)

{
  int iVar1;
  byte *pbVar2;
  undefined4 *puVar3;
  ushort uVar4;
  ushort *puVar5;
  int iVar6;
  byte *pbVar7;
  void *local_c;
  void *pvStack8;
  
  pbVar7 = *param_2;
  if (param_1 == (ushort *)0x0) {
    iVar1 = 0;
    local_c = this;
    pvStack8 = this;
    while( true ) {
      pbVar2 = (byte *)FUN_004109e4((char *)pbVar7);
      iVar6 = FUN_00413d32(0,pbVar7,pbVar2,param_4);
      if (iVar6 == -1) break;
      if (iVar6 == 0) {
        return iVar1;
      }
      if (iVar6 == 4) {
        iVar1 = iVar1 + 1;
      }
      pbVar7 = pbVar7 + iVar6;
      iVar1 = iVar1 + 1;
    }
    puVar3 = (undefined4 *)FUN_00407f40();
    *puVar3 = 0x2a;
  }
  else {
    puVar5 = param_1;
    local_c = this;
    pvStack8 = this;
    if (param_3 != 0) {
      do {
        pbVar2 = (byte *)FUN_004109e4((char *)pbVar7);
        iVar1 = FUN_00413d32((uint)&local_c,pbVar7,pbVar2,param_4);
        if (iVar1 == -1) {
          *param_2 = pbVar7;
          puVar3 = (undefined4 *)FUN_00407f40();
          *puVar3 = 0x2a;
          return -1;
        }
        if (iVar1 == 0) {
          pbVar7 = (byte *)0x0;
          *puVar5 = 0;
          break;
        }
        uVar4 = (ushort)local_c;
        if ((void *)0xffff < local_c) {
          if (param_3 < 2) break;
          local_c = (void *)((int)local_c + -0x10000);
          param_3 = param_3 - 1;
          *puVar5 = (ushort)((uint)local_c >> 10) | 0xd800;
          puVar5 = puVar5 + 1;
          uVar4 = (ushort)local_c & 0x3ff | 0xdc00;
        }
        *puVar5 = uVar4;
        pbVar7 = pbVar7 + iVar1;
        puVar5 = puVar5 + 1;
        param_3 = param_3 - 1;
      } while (param_3 != 0);
    }
    iVar6 = (int)((int)puVar5 - (int)param_1) >> 1;
    *param_2 = pbVar7;
  }
  return iVar6;
}



int __cdecl FUN_00410b4f(byte *param_1,uint param_2,undefined4 *param_3)

{
  byte bVar1;
  int iVar2;
  byte bVar3;
  int iVar4;
  
  if (param_1 == (byte *)0x0) {
    *param_3 = 0;
    param_3[1] = 0;
    return 1;
  }
  if (param_2 == 0) {
    *param_1 = (byte)param_2;
    *param_3 = 0;
    param_3[1] = 0;
    return 1;
  }
  if ((param_2 & 0xffffff80) == 0) {
    *param_1 = (byte)param_2;
    return 1;
  }
  if ((param_2 & 0xfffff800) == 0) {
    bVar3 = 0xc0;
    iVar4 = 1;
    iVar2 = iVar4;
  }
  else {
    if ((param_2 & 0xffff0000) == 0) {
      if ((0xd7ff < param_2) && (param_2 < 0xe000)) {
LAB_00410bf1:
        iVar2 = FUN_00410bfe(param_3);
        return iVar2;
      }
      iVar4 = 2;
      bVar3 = 0xe0;
      iVar2 = iVar4;
    }
    else {
      if (((param_2 & 0xffe00000) != 0) || (0x10ffff < param_2)) goto LAB_00410bf1;
      iVar4 = 3;
      bVar3 = 0xf0;
      iVar2 = iVar4;
    }
  }
  do {
    bVar1 = (byte)param_2;
    param_2 = param_2 >> 6;
    param_1[iVar4] = bVar1 & 0x3f | 0x80;
    iVar4 = iVar4 + -1;
  } while (iVar4 != 0);
  *param_1 = (byte)param_2 | bVar3;
  *param_3 = 0;
  param_3[1] = 0;
  return iVar2 + 1;
}



undefined4 __cdecl FUN_00410bfe(undefined4 *param_1)

{
  undefined4 *puVar1;
  
  *param_1 = 0;
  param_1[1] = 0;
  puVar1 = (undefined4 *)FUN_00407f40();
  *puVar1 = 0x2a;
  return 0xffffffff;
}



void __cdecl FUN_00410c1d(byte *param_1,uint param_2,undefined4 *param_3)

{
  FUN_00410b4f(param_1,param_2,param_3);
  return;
}



undefined4 __cdecl FUN_00410c28(undefined8 *param_1,uint param_2,int param_3,int param_4)

{
  undefined4 *puVar1;
  int iVar2;
  char *pcVar3;
  undefined8 *puVar4;
  char cVar5;
  char *pcVar6;
  undefined4 uVar7;
  
  if ((param_1 != (undefined8 *)0x0) && (param_2 != 0)) {
    *(undefined *)param_1 = 0;
    iVar2 = param_3;
    if (param_3 < 1) {
      iVar2 = 0;
    }
    if (param_2 <= iVar2 + 1U) {
      puVar1 = (undefined4 *)FUN_00407f40();
      uVar7 = 0x22;
      goto LAB_00410c3c;
    }
    if (param_4 != 0) {
      pcVar3 = (char *)((int)param_1 + 1);
      pcVar6 = *(char **)(param_4 + 8);
      *(undefined *)param_1 = 0x30;
      if (0 < param_3) {
        do {
          cVar5 = *pcVar6;
          if (cVar5 == 0) {
            cVar5 = '0';
          }
          else {
            pcVar6 = pcVar6 + 1;
          }
          *pcVar3 = cVar5;
          pcVar3 = pcVar3 + 1;
          param_3 = param_3 + -1;
        } while (0 < param_3);
      }
      *pcVar3 = 0;
      if ((-1 < param_3) && ('4' < *pcVar6)) {
        while( true ) {
          pcVar3 = pcVar3 + -1;
          if (*pcVar3 != '9') break;
          *pcVar3 = '0';
        }
        *pcVar3 = *pcVar3 + 1;
      }
      if (*(char *)param_1 == '1') {
        *(int *)(param_4 + 4) = *(int *)(param_4 + 4) + 1;
      }
      else {
        puVar4 = (undefined8 *)((int)param_1 + 1);
        do {
          cVar5 = *(char *)puVar4;
          puVar4 = (undefined8 *)((int)puVar4 + 1);
        } while (cVar5 != 0);
        FUN_00405050(param_1,(undefined8 *)((int)param_1 + 1),(int)puVar4 + (1 - ((int)param_1 + 2))
                    );
      }
      return 0;
    }
  }
  puVar1 = (undefined4 *)FUN_00407f40();
  uVar7 = 0x16;
LAB_00410c3c:
  *puVar1 = uVar7;
  FUN_00407e83();
  return uVar7;
}



// WARNING: Removing unreachable block (ram,0x00411041)
// WARNING: Removing unreachable block (ram,0x0041101c)
// WARNING: Could not reconcile some variable overlaps

ulonglong __cdecl FUN_00410cf0(uint *param_1,uint *param_2)

{
  longlong lVar1;
  uint *puVar2;
  uint uVar3;
  uint extraout_ECX;
  uint *puVar4;
  uint extraout_ECX_00;
  uint uVar5;
  uint uVar6;
  int iVar7;
  uint uVar8;
  undefined4 uVar9;
  uint uVar10;
  bool bVar11;
  undefined8 uVar12;
  ulonglong uVar13;
  ulonglong uVar14;
  ulonglong uVar15;
  undefined local_21c [460];
  uint *local_50;
  uint local_4c;
  undefined8 local_48;
  uint *local_40;
  uint local_3c;
  uint local_38;
  uint local_34;
  int local_30;
  int local_28;
  uint local_24;
  uint *local_20;
  uint *local_1c;
  uint local_18;
  int local_14;
  int local_10;
  uint local_c;
  uint local_8;
  
  uVar8 = *param_1;
  if ((uVar8 != 0) && (local_38 = *param_2, local_38 != 0)) {
    uVar6 = local_38 - 1;
    uVar3 = uVar8 - 1;
    if (uVar6 == 0) {
      local_c = param_2[1];
      if (local_c == 1) {
        uVar8 = param_1[1];
        *param_1 = 0;
        _memcpy_s(param_1 + 1,0x1cc,local_21c,0);
        return (ulonglong)uVar8;
      }
      if (uVar3 == 0) {
        uVar8 = param_1[1];
        *param_1 = 0;
        _memcpy_s(param_1 + 1,0x1cc,local_21c,0);
        uVar6 = uVar8 % local_c;
        param_1[1] = uVar6;
        *param_1 = (uint)(uVar6 != 0);
        return (ulonglong)uVar8 / (ulonglong)local_c;
      }
      uVar6 = 0;
      local_10 = 0;
      local_28 = 0;
      local_1c = (uint *)0x0;
      if (uVar3 != 0xffffffff) {
        local_20 = param_1 + uVar8;
        puVar4 = param_1;
        do {
          puVar2 = puVar4;
          puVar4 = puVar2;
          uVar12 = __aulldvrm(*local_20,uVar6,local_c,0);
          local_48._4_4_ = (int)((ulonglong)uVar12 >> 0x20);
          local_28 = local_10;
          local_20 = local_20 + -1;
          uVar8 = uVar8 - 1;
          uVar6 = extraout_ECX;
          local_1c = puVar2;
          local_10 = (int)uVar12;
        } while (uVar8 != 0);
      }
      *param_1 = 0;
      _memcpy_s(param_1 + 1,0x1cc,local_21c,0);
      param_1[1] = uVar6;
      *(uint **)(param_1 + 2) = local_1c;
      *param_1 = (uint)(local_1c != (uint *)0x0) + 1;
      return CONCAT44(local_28,local_10);
    }
    if (uVar6 <= uVar3) {
      iVar7 = uVar3 - uVar6;
      if (iVar7 <= (int)uVar3) {
        puVar4 = param_1 + uVar8;
        puVar2 = param_2 + local_38;
        uVar8 = uVar3;
        do {
          if (*puVar2 != *puVar4) {
            if (*puVar4 <= *puVar2) goto LAB_00410e8e;
            break;
          }
          uVar8 = uVar8 - 1;
          puVar2 = puVar2 + -1;
          puVar4 = puVar4 + -1;
        } while (iVar7 <= (int)uVar8);
      }
      iVar7 = iVar7 + 1;
LAB_00410e8e:
      if (iVar7 != 0) {
        local_34 = param_2[local_38];
        local_24 = param_2[local_38 - 1];
        local_10 = 0x1f;
        if (local_34 != 0) {
          while (local_34 >> local_10 == 0) {
            local_10 = local_10 + -1;
          }
        }
        if (local_34 == 0) {
          local_10 = 0x20;
        }
        else {
          local_10 = 0x1f - local_10;
        }
        local_30 = 0x20 - local_10;
        if (local_10 != 0) {
          uVar8 = local_24 >> ((byte)local_30 & 0x1f);
          local_24 = local_24 << ((byte)local_10 & 0x1f);
          local_34 = local_34 << ((byte)local_10 & 0x1f) | uVar8;
          if (2 < local_38) {
            local_24 = local_24 | param_2[local_38 - 2] >> ((byte)local_30 & 0x1f);
          }
        }
        local_20 = (uint *)0x0;
        local_1c = (uint *)(iVar7 + -1);
        uVar9 = 0;
        local_c = uVar3;
        if (-1 < (int)local_1c) {
          local_3c = (int)local_1c + local_38;
          local_40 = param_1 + iVar7;
          local_50 = param_1 + (local_3c - 1);
          local_c = uVar3;
          do {
            if (local_c < local_3c) {
              local_4c = 0;
            }
            else {
              local_4c = local_50[2];
            }
            uVar8 = local_50[1];
            local_18 = *local_50;
            local_28 = 0;
            local_8 = local_4c;
            if (local_10 != 0) {
              uVar8 = local_18 >> ((byte)local_30 & 0x1f);
              local_8 = local_4c;
              uVar12 = __allshl();
              local_8 = (uint)((ulonglong)uVar12 >> 0x20);
              uVar8 = uVar8 | (uint)uVar12;
              local_18 = local_18 << ((byte)local_10 & 0x1f);
              if (2 < local_3c) {
                local_18 = local_18 |
                           param_1[local_38 + (int)local_1c + -2] >> ((byte)local_30 & 0x1f);
              }
            }
            uVar13 = __aulldvrm(uVar8,local_8,local_34,0);
            uVar15 = (ulonglong)extraout_ECX_00;
            local_28 = 0;
            local_48 = uVar13;
            if ((int)(uVar13 >> 0x20) != 0) {
              local_48 = uVar13;
              uVar14 = __allmul((uint)(uVar13 - 0xffffffff),(uint)(uVar13 - 0xffffffff >> 0x20),
                                local_34,0);
              local_14 = (int)(uVar13 >> 0x20);
              local_8 = (uint)uVar13;
              uVar15 = uVar14 + uVar15;
              local_28 = (int)(uVar15 >> 0x20);
              local_48 = 0xffffffff;
            }
            local_14 = (int)(local_48 >> 0x20);
            uVar8 = (uint)uVar15;
            uVar13 = local_48;
            if ((int)(uVar15 >> 0x20) == 0) {
              do {
                uVar6 = (uint)local_48;
                local_8 = local_18;
                uVar15 = __allmul(local_24,0,uVar6,(uint)(local_48 >> 0x20));
                if (uVar15 <= CONCAT44(uVar8,local_8)) break;
                bVar11 = uVar6 != 0;
                uVar6 = uVar6 - 1;
                local_14 = local_14 + -1 + (uint)bVar11;
                local_48 = CONCAT44(local_14,uVar6);
                bVar11 = CARRY4(uVar8,local_34);
                uVar8 = uVar8 + local_34;
                local_28 = local_28 + (uint)bVar11;
              } while (local_28 == 0);
              uVar13 = CONCAT44(local_14,uVar6);
            }
            local_48._4_4_ = (int)(local_48 >> 0x20);
            if (uVar13 != 0) {
              uVar6 = 0;
              uVar8 = 0;
              if (local_38 != 0) {
                puVar2 = param_2 + 1;
                local_18 = local_38;
                puVar4 = local_40;
                do {
                  lVar1 = (local_48 & 0xffffffff) * (ulonglong)*puVar2;
                  uVar3 = (uint)lVar1;
                  uVar10 = uVar6 + uVar3;
                  uVar6 = uVar8 + (int)((ulonglong)lVar1 >> 0x20) + local_48._4_4_ * *puVar2 +
                          (uint)CARRY4(uVar6,uVar3);
                  uVar8 = 0;
                  if (*puVar4 < uVar10) {
                    bVar11 = 0xfffffffe < uVar6;
                    uVar6 = uVar6 + 1;
                    uVar8 = (uint)bVar11;
                  }
                  *puVar4 = *puVar4 - uVar10;
                  puVar4 = puVar4 + 1;
                  puVar2 = puVar2 + 1;
                  local_18 = local_18 - 1;
                } while (local_18 != 0);
              }
              if ((uVar8 != 0) || (local_4c < uVar6)) {
                if (local_38 != 0) {
                  uVar6 = 0;
                  puVar2 = local_40;
                  uVar8 = local_38;
                  puVar4 = param_2;
                  do {
                    puVar4 = puVar4 + 1;
                    uVar3 = *puVar2;
                    uVar10 = *puVar4;
                    uVar5 = uVar3 + *puVar4;
                    *puVar2 = uVar5 + uVar6;
                    uVar6 = (uint)CARRY4(uVar3,uVar10) + (uint)CARRY4(uVar5,uVar6);
                    uVar8 = uVar8 - 1;
                    puVar2 = puVar2 + 1;
                  } while (uVar8 != 0);
                }
                uVar13 = uVar13 - 1;
              }
              local_c = local_3c - 1;
            }
            local_14 = (int)(uVar13 >> 0x20);
            local_40 = local_40 + -1;
            local_1c = (uint *)((int)local_1c + -1);
            local_50 = local_50 + -1;
            local_3c = local_3c - 1;
            uVar9 = local_20;
            local_20 = (uint *)(int)uVar13;
          } while (-1 < (int)local_1c);
        }
        uVar8 = local_c + 1;
        if (uVar8 < *param_1) {
          puVar4 = param_1 + local_c + 2;
          uVar6 = uVar8;
          do {
            *puVar4 = 0;
            puVar4 = puVar4 + 1;
            uVar6 = uVar6 + 1;
          } while (uVar6 < *param_1);
        }
        *param_1 = uVar8;
        while ((uVar8 != 0 && (param_1[uVar8] == 0))) {
          uVar8 = uVar8 - 1;
          *param_1 = uVar8;
        }
        return CONCAT44(uVar9,local_20);
      }
    }
  }
  return 0;
}



// WARNING: Type propagation algorithm not settling
// WARNING: Could not reconcile some variable overlaps

void __cdecl
FUN_00411188(uint param_1,uint param_2,int param_3,undefined4 *param_4,undefined8 *param_5,
            rsize_t param_6)

{
  undefined8 *puVar1;
  byte bVar2;
  undefined8 **ppuVar3;
  ulonglong uVar4;
  code *pcVar5;
  longlong lVar6;
  bool bVar7;
  int iVar8;
  char *_Src;
  rsize_t _MaxCount;
  uint uVar9;
  errno_t eVar10;
  uint uVar11;
  undefined8 *puVar12;
  undefined8 *puVar13;
  undefined8 *_Src_00;
  uint uVar14;
  undefined8 *puVar15;
  undefined8 **ppuVar16;
  ulonglong uVar17;
  undefined4 uVar18;
  undefined4 uVar19;
  undefined8 local_97c;
  undefined8 local_964 [57];
  uint local_798;
  bool local_790;
  undefined8 *local_788;
  undefined4 *local_784;
  undefined8 *local_780;
  undefined8 local_77c;
  undefined8 *local_774;
  undefined8 *local_770;
  uint local_76c;
  undefined8 *local_768;
  undefined8 *local_764;
  undefined8 *local_760;
  undefined8 **local_75c;
  undefined8 *local_758;
  undefined8 *local_754;
  undefined8 *local_750;
  undefined8 *local_74c;
  undefined8 *local_748;
  undefined8 local_744 [57];
  undefined8 *local_578;
  uint local_574;
  undefined4 local_570;
  undefined8 *local_3a8;
  undefined8 **local_3a4;
  undefined8 *local_1d8;
  undefined8 local_1d4;
  uint local_8;
  
  local_8 = DAT_0041e004 ^ (uint)&stack0xfffffffc;
  local_784 = param_4;
  local_770 = param_5;
  FUN_00413ee6(&local_798);
  local_790 = ((byte)local_798 & 0x1f) != 0x1f;
  if (local_790) {
    FUN_00413f50(&local_798);
  }
  uVar14 = param_1;
  uVar18 = 0x20;
  if (((int)param_2 < 1) && ((int)param_2 < 0)) {
    uVar18 = 0x2d;
  }
  *local_784 = uVar18;
  *(undefined8 **)(local_784 + 2) = local_770;
  if (((param_2 & 0x7ff00000) == 0) && ((param_1 | param_2 & 0xfffff) == 0)) {
    _Src = "0";
    local_784[1] = 0;
  }
  else {
    iVar8 = FUN_0040b873(&param_1);
    if (iVar8 != 0) {
      local_784[1] = 1;
    }
    if (iVar8 == 1) {
      _Src = "1#INF";
    }
    else {
      if (iVar8 == 2) {
        _Src = "1#QNAN";
      }
      else {
        if (iVar8 == 3) {
          _Src = "1#SNAN";
        }
        else {
          if (iVar8 != 4) {
            local_788 = (undefined8 *)0x0;
            local_780 = (undefined8 *)(param_3 + 1);
            local_77c = CONCAT44(param_2,uVar14) & 0x7fffffffffffffff;
            local_77c._4_4_ = (uint)(local_77c >> 0x20);
            uVar11 = local_77c._4_4_ >> 0x14;
            if (uVar11 != 0) {
              uVar9 = 0;
              iVar8 = 0x100000;
            }
            else {
              iVar8 = 0;
              uVar9 = uVar11;
            }
            local_760 = (undefined8 *)((uint)local_77c + uVar9);
            local_1d4._4_4_ =
                 (local_77c._4_4_ & 0xfffff) + iVar8 + (uint)CARRY4((uint)local_77c,uVar9);
            local_74c = (undefined8 *)(uVar11 + (uint)(uVar11 == 0));
            param_1 = uVar14;
            FUN_00413fa0();
            FUN_004140b0();
            local_76c = FUN_00416550();
            if ((local_76c == 0x7fffffff) || (local_76c == 0x80000000)) {
              local_76c = 0;
            }
            local_1d4._0_4_ = local_760;
            uVar14 = (uint)(local_1d4._4_4_ != 0);
            local_3a8 = (undefined8 *)0x0;
            _Src_00 = (undefined8 *)(uVar14 + 1);
            if (local_74c < (undefined8 *)0x433) {
              if (local_74c == (undefined8 *)0x35) {
LAB_00411862:
                puVar15 = (&local_1d8)[(int)_Src_00];
                local_77c = local_77c & 0xffffffff;
                iVar8 = 0x1f;
                if (puVar15 != (undefined8 *)0x0) {
                  while ((uint)puVar15 >> iVar8 == 0) {
                    iVar8 = iVar8 + -1;
                  }
                }
                if (puVar15 == (undefined8 *)0x0) {
                  iVar8 = 0;
                }
                else {
                  iVar8 = iVar8 + 1;
                }
                local_75c = &local_1d8 + (int)_Src_00;
                puVar15 = _Src_00;
                ppuVar16 = local_75c;
                do {
                  if (puVar15 < _Src_00) {
                    local_750 = *(undefined8 **)((int)&local_1d4 + (int)puVar15 * 4);
                  }
                  else {
                    local_750 = (undefined8 *)0x0;
                  }
                  if ((undefined8 *)((int)puVar15 + -1) < _Src_00) {
                    puVar12 = *ppuVar16;
                  }
                  else {
                    puVar12 = (undefined8 *)0x0;
                  }
                  ppuVar16 = ppuVar16 + -1;
                  *(uint *)((int)&local_1d4 + (int)puVar15 * 4) =
                       (uint)puVar12 >> 0x1f | (int)local_750 * 2;
                  puVar15 = (undefined8 *)((int)puVar15 + -1);
                } while (puVar15 != (undefined8 *)0xffffffff);
                local_1d8 = _Src_00;
                if (iVar8 == 0x20) {
                  local_1d8 = (undefined8 *)(uVar14 + 2);
                }
                uVar14 = 0x434 - (int)local_74c;
                uVar11 = uVar14 >> 5;
                local_760 = _Src_00;
                FUN_00402e50((int *)&local_574,0,uVar11 * 4);
                *(int *)((int)&local_574 + uVar11 * 4) = 1 << ((byte)uVar14 & 0x1f);
              }
              else {
                local_574 = 0;
                local_570 = 0x100000;
                local_578 = (undefined8 *)0x2;
                if (local_1d4._4_4_ == 0) goto LAB_00411862;
                iVar8 = 0;
                do {
                  if (*(int *)((int)&local_574 + iVar8) != *(int *)((int)&local_1d4 + iVar8))
                  goto LAB_00411862;
                  iVar8 = iVar8 + 4;
                } while (iVar8 != 8);
                local_77c = local_77c & 0xffffffff;
                iVar8 = 0x1f;
                if (local_1d4._4_4_ != 0) {
                  while (local_1d4._4_4_ >> iVar8 == 0) {
                    iVar8 = iVar8 + -1;
                  }
                }
                if (local_1d4._4_4_ == 0) {
                  iVar8 = 0;
                }
                else {
                  iVar8 = iVar8 + 1;
                }
                local_75c = &local_1d8 + (int)_Src_00;
                puVar15 = _Src_00;
                ppuVar16 = local_75c;
                do {
                  if (puVar15 < _Src_00) {
                    local_750 = *(undefined8 **)((int)&local_1d4 + (int)puVar15 * 4);
                  }
                  else {
                    local_750 = (undefined8 *)0x0;
                  }
                  if ((undefined8 *)((int)puVar15 + -1) < _Src_00) {
                    puVar12 = *ppuVar16;
                  }
                  else {
                    puVar12 = (undefined8 *)0x0;
                  }
                  ppuVar16 = ppuVar16 + -1;
                  *(uint *)((int)&local_1d4 + (int)puVar15 * 4) =
                       (uint)puVar12 >> 0x1e | (int)local_750 << 2;
                  puVar15 = (undefined8 *)((int)puVar15 + -1);
                } while (puVar15 != (undefined8 *)0xffffffff);
                local_1d8 = _Src_00;
                if (0x20U - iVar8 < 2) {
                  local_1d8 = (undefined8 *)(uVar14 + 2);
                }
                uVar14 = 0x435 - (int)local_74c;
                uVar11 = uVar14 >> 5;
                local_760 = _Src_00;
                FUN_00402e50((int *)&local_574,0,uVar11 * 4);
                *(int *)((int)&local_574 + uVar11 * 4) = 1 << ((byte)uVar14 & 0x1f);
              }
              local_578 = (undefined8 *)(uVar11 + 1);
              local_3a8 = local_578;
              _memcpy_s(&local_3a4,0x1cc,&local_574,(int)local_578 * 4);
            }
            else {
              local_574 = 0;
              local_570 = 0x100000;
              local_578 = (undefined8 *)0x2;
              if (local_1d4._4_4_ == 0) {
LAB_00411584:
                _Src = (char *)((int)local_74c + -0x432);
                local_74c = (undefined8 *)((uint)_Src & 0x1f);
                puVar12 = (undefined8 *)((uint)_Src >> 5);
                local_760 = (undefined8 *)(0x20 - (int)local_74c);
                local_754 = puVar12;
                local_1d8 = _Src_00;
                iVar8 = __allshl();
                puVar15 = (&local_1d8)[(int)_Src_00];
                local_774 = (undefined8 *)(iVar8 + -1);
                local_77c = local_77c & 0xffffffff;
                iVar8 = 0x1f;
                if (puVar15 != (undefined8 *)0x0) {
                  while ((uint)puVar15 >> iVar8 == 0) {
                    iVar8 = iVar8 + -1;
                  }
                }
                local_75c = (undefined8 **)~(uint)local_774;
                if (puVar15 == (undefined8 *)0x0) {
                  iVar8 = 0;
                }
                else {
                  iVar8 = iVar8 + 1;
                }
                local_764 = (undefined8 *)((int)_Src_00 + (int)puVar12);
                local_758 = (undefined8 *)(0x20 - iVar8);
                if ((local_764 == (undefined8 *)0x73) && (local_758 < local_74c)) {
                  bVar7 = true;
                }
                else {
                  bVar7 = false;
                }
                if (((undefined8 *)0x73 < local_764) || (bVar7)) {
                  local_578 = (undefined8 *)0x0;
                  local_1d8 = (undefined8 *)0x0;
                  _memcpy_s(&local_1d4,0x1cc,&local_574,0);
                }
                else {
                  if ((undefined8 *)0x71 < local_764) {
                    local_764 = (undefined8 *)0x72;
                  }
                  local_768 = local_764;
                  if (local_764 != (undefined8 *)0xffffffff) {
                    puVar13 = (undefined8 *)((int)local_764 - (int)local_754);
                    puVar15 = (undefined8 *)((int)&local_1d4 + (int)puVar13 * 4);
                    local_768 = local_764;
                    do {
                      puVar12 = local_754;
                      if (local_768 < local_754) break;
                      if (puVar13 < _Src_00) {
                        local_750 = *(undefined8 **)puVar15;
                      }
                      else {
                        local_750 = (undefined8 *)0x0;
                      }
                      if ((undefined8 *)((int)puVar13 + -1) < _Src_00) {
                        _Src = *(char **)((int)puVar15 + -4);
                      }
                      else {
                        _Src = (char *)0x0;
                      }
                      puVar15 = (undefined8 *)((int)puVar15 + -4);
                      *(uint *)((int)&local_1d4 + (int)local_768 * 4) =
                           ((uint)_Src & (uint)local_75c) >> ((byte)local_760 & 0x1f) |
                           ((uint)local_750 & (uint)local_774) << ((byte)local_74c & 0x1f);
                      local_768 = (undefined8 *)((int)local_768 + -1);
                      puVar13 = (undefined8 *)((int)puVar13 + -1);
                      _Src_00 = local_1d8;
                      puVar12 = local_754;
                    } while (local_768 != (undefined8 *)0xffffffff);
                  }
                  if (puVar12 != (undefined8 *)0x0) {
                    _Src_00 = &local_1d4;
                    while (puVar12 != (undefined8 *)0x0) {
                      puVar12 = (undefined8 *)((int)puVar12 + -1);
                      *(undefined4 *)_Src_00 = 0;
                      _Src_00 = (undefined8 *)((int)_Src_00 + 4);
                    }
                  }
                  local_1d8 = local_764;
                  if (local_758 < local_74c) {
                    local_1d8 = (undefined8 *)((int)local_764 + 1);
                  }
                }
                local_574 = 2;
              }
              else {
                iVar8 = 0;
                do {
                  if (*(int *)((int)&local_574 + iVar8) != *(int *)((int)&local_1d4 + iVar8))
                  goto LAB_00411584;
                  iVar8 = iVar8 + 4;
                } while (iVar8 != 8);
                _Src = (char *)((int)local_74c + -0x431);
                local_74c = (undefined8 *)((uint)_Src & 0x1f);
                puVar12 = (undefined8 *)((uint)_Src >> 5);
                local_774 = (undefined8 *)(0x20 - (int)local_74c);
                local_750 = puVar12;
                local_1d8 = _Src_00;
                iVar8 = __allshl();
                puVar15 = (&local_1d8)[(int)_Src_00];
                local_75c = (undefined8 **)(iVar8 + -1);
                local_77c = local_77c & 0xffffffff;
                iVar8 = 0x1f;
                if (puVar15 != (undefined8 *)0x0) {
                  while ((uint)puVar15 >> iVar8 == 0) {
                    iVar8 = iVar8 + -1;
                  }
                }
                local_760 = (undefined8 *)~(uint)local_75c;
                if (puVar15 == (undefined8 *)0x0) {
                  iVar8 = 0;
                }
                else {
                  iVar8 = iVar8 + 1;
                }
                local_768 = (undefined8 *)((int)_Src_00 + (int)puVar12);
                local_758 = (undefined8 *)(0x20 - iVar8);
                if ((local_768 == (undefined8 *)0x73) && (local_758 < local_74c)) {
                  bVar7 = true;
                }
                else {
                  bVar7 = false;
                }
                if (((undefined8 *)0x73 < local_768) || (bVar7)) {
                  local_578 = (undefined8 *)0x0;
                  local_1d8 = (undefined8 *)0x0;
                  _memcpy_s(&local_1d4,0x1cc,&local_574,0);
                }
                else {
                  if ((undefined8 *)0x71 < local_768) {
                    local_768 = (undefined8 *)0x72;
                  }
                  local_764 = local_768;
                  if (local_768 != (undefined8 *)0xffffffff) {
                    puVar13 = (undefined8 *)((int)local_768 - (int)local_750);
                    puVar15 = (undefined8 *)((int)&local_1d4 + (int)puVar13 * 4);
                    local_764 = local_768;
                    do {
                      puVar12 = local_750;
                      if (local_764 < local_750) break;
                      if (puVar13 < _Src_00) {
                        local_754 = *(undefined8 **)puVar15;
                      }
                      else {
                        local_754 = (undefined8 *)0x0;
                      }
                      if ((undefined8 *)((int)puVar13 + -1) < _Src_00) {
                        _Src = *(char **)((int)puVar15 + -4);
                      }
                      else {
                        _Src = (char *)0x0;
                      }
                      puVar15 = (undefined8 *)((int)puVar15 + -4);
                      *(uint *)((int)&local_1d4 + (int)local_764 * 4) =
                           ((uint)_Src & (uint)local_760) >> ((byte)local_774 & 0x1f) |
                           ((uint)local_754 & (uint)local_75c) << ((byte)local_74c & 0x1f);
                      local_764 = (undefined8 *)((int)local_764 + -1);
                      puVar13 = (undefined8 *)((int)puVar13 + -1);
                      _Src_00 = local_1d8;
                      puVar12 = local_750;
                    } while (local_764 != (undefined8 *)0xffffffff);
                  }
                  if (puVar12 != (undefined8 *)0x0) {
                    _Src_00 = &local_1d4;
                    while (puVar12 != (undefined8 *)0x0) {
                      puVar12 = (undefined8 *)((int)puVar12 + -1);
                      *(undefined4 *)_Src_00 = 0;
                      _Src_00 = (undefined8 *)((int)_Src_00 + 4);
                    }
                  }
                  local_1d8 = local_768;
                  if (local_758 < local_74c) {
                    local_1d8 = (undefined8 *)((int)local_768 + 1);
                  }
                }
                local_574 = 4;
              }
              local_3a8 = (undefined8 *)0x1;
              local_570 = 0;
              local_578 = (undefined8 *)0x1;
              _memcpy_s(&local_3a4,0x1cc,&local_574,4);
            }
            local_760 = (undefined8 *)0xa;
            if ((int)local_76c < 0) {
              local_764 = (undefined8 *)(-local_76c / 10);
              local_77c._4_4_ = -local_76c % 10;
              local_77c = local_77c & 0xffffffff | (ulonglong)local_77c._4_4_ << 0x20;
              if (local_764 != (undefined8 *)0x0) {
                do {
                  local_74c = local_764;
                  if ((undefined8 *)0x26 < local_764) {
                    local_74c = (undefined8 *)0x26;
                  }
                  bVar2 = (&DAT_0041bb97)[(int)local_74c * 4];
                  uVar14 = (uint)(byte)(&DAT_0041bb96)[(int)local_74c * 4] * 4;
                  local_578 = (undefined8 *)
                              ((uint)(byte)(&DAT_0041bb96)[(int)local_74c * 4] + (uint)bVar2);
                  FUN_00402e50((int *)&local_574,0,uVar14);
                  copy_buffer((undefined8 *)((int)&local_574 + uVar14),
                              (undefined8 *)
                              (&UNK_0041b290 +
                              (uint)*(ushort *)(&UNK_0041bb94 + (int)local_74c * 4) * 4),
                              (uint)bVar2 << 2);
                  _Src_00 = (undefined8 *)local_1d4;
                  if (local_578 < (undefined8 *)0x2) {
                    if (local_574 == 0) {
LAB_00411e71:
                      local_1d8 = (undefined8 *)0x0;
                      _MaxCount = 0;
                      _Src_00 = local_964;
                      goto LAB_004120fe;
                    }
                    if ((local_574 == 1) || (local_1d8 == (undefined8 *)0x0)) goto LAB_0041210f;
                    uVar14 = 0;
                    _Src_00 = (undefined8 *)0x0;
                    do {
                      lVar6 = (ulonglong)local_574 *
                              (ulonglong)*(uint *)((int)&local_1d4 + (int)_Src_00 * 4);
                      uVar11 = (uint)lVar6;
                      *(uint *)((int)&local_1d4 + (int)_Src_00 * 4) = uVar11 + uVar14;
                      uVar14 = (int)((ulonglong)lVar6 >> 0x20) + (uint)CARRY4(uVar11,uVar14);
                      _Src_00 = (undefined8 *)((int)_Src_00 + 1);
                    } while (_Src_00 != local_1d8);
LAB_00411ec9:
                    if (uVar14 == 0) goto LAB_0041210f;
                    if (local_1d8 < (undefined8 *)0x73) {
                      *(uint *)((int)&local_1d4 + (int)local_1d8 * 4) = uVar14;
                      local_1d8 = (undefined8 *)((int)local_1d8 + 1);
                      goto LAB_0041210f;
                    }
LAB_004121ad:
                    local_1d8 = (undefined8 *)0x0;
                    _memcpy_s(&local_1d4,0x1cc,local_964,0);
                    bVar7 = false;
                  }
                  else {
                    if (local_1d8 < (undefined8 *)0x2) {
                      local_788 = (undefined8 *)local_1d4;
                      local_1d8 = local_578;
                      _memcpy_s(&local_1d4,0x1cc,&local_574,(int)local_578 << 2);
                      if (_Src_00 != (undefined8 *)0x0) {
                        if ((_Src_00 != (undefined8 *)0x1) && (local_1d8 != (undefined8 *)0x0)) {
                          uVar14 = 0;
                          _Src_00 = (undefined8 *)0x0;
                          do {
                            lVar6 = ZEXT48(local_788) *
                                    (ulonglong)*(uint *)((int)&local_1d4 + (int)_Src_00 * 4);
                            uVar11 = (uint)lVar6;
                            *(uint *)((int)&local_1d4 + (int)_Src_00 * 4) = uVar11 + uVar14;
                            uVar14 = (int)((ulonglong)lVar6 >> 0x20) + (uint)CARRY4(uVar11,uVar14);
                            _Src_00 = (undefined8 *)((int)_Src_00 + 1);
                          } while (_Src_00 != local_1d8);
                          goto LAB_00411ec9;
                        }
                        goto LAB_0041210f;
                      }
                      goto LAB_00411e71;
                    }
                    local_754 = &local_574;
                    if (local_1d8 <= local_578) {
                      local_754 = &local_1d4;
                    }
                    local_774 = &local_1d4;
                    _Src_00 = local_1d8;
                    local_768 = local_578;
                    if (local_1d8 <= local_578) {
                      local_774 = &local_574;
                      _Src_00 = local_578;
                      local_768 = local_1d8;
                    }
                    local_1d8 = (undefined8 *)0x0;
                    puVar15 = (undefined8 *)0x0;
                    local_748 = (undefined8 *)0x0;
                    if (local_768 != (undefined8 *)0x0) {
                      do {
                        if (*(int *)((int)local_754 + (int)puVar15 * 4) == 0) {
                          if (puVar15 == local_1d8) {
                            *(undefined4 *)((int)local_744 + (int)puVar15 * 4) = 0;
                            local_1d8 = (undefined8 *)((int)puVar15 + 1);
                            local_748 = local_1d8;
                          }
                        }
                        else {
                          local_758 = (undefined8 *)0x0;
                          local_750 = (undefined8 *)0x0;
                          puVar12 = puVar15;
                          if (_Src_00 != (undefined8 *)0x0) {
                            do {
                              puVar13 = local_750;
                              if (puVar12 == (undefined8 *)0x73) break;
                              if (puVar12 == local_1d8) {
                                *(undefined4 *)((int)local_744 + (int)puVar12 * 4) = 0;
                                local_748 = (undefined8 *)
                                            ((char *)((int)local_758 + 1) + (int)puVar15);
                              }
                              lVar6 = (ulonglong)*(uint *)((int)local_774 + (int)local_758 * 4) *
                                      (ulonglong)*(uint *)((int)local_754 + (int)puVar15 * 4);
                              uVar11 = (uint)lVar6;
                              uVar9 = uVar11 + (int)local_750;
                              puVar13 = (undefined8 *)((int)local_744 + (int)puVar12 * 4);
                              uVar14 = *(uint *)puVar13;
                              *(uint *)puVar13 = *(uint *)puVar13 + uVar9;
                              puVar13 = (undefined8 *)
                                        ((int)((ulonglong)lVar6 >> 0x20) +
                                         (uint)CARRY4(uVar11,(uint)local_750) +
                                        (uint)CARRY4(uVar14,uVar9));
                              local_758 = (undefined8 *)((int)local_758 + 1);
                              puVar12 = (undefined8 *)((int)puVar12 + 1);
                              local_1d8 = local_748;
                              local_788 = puVar13;
                              local_750 = puVar13;
                            } while (local_758 != _Src_00);
                            do {
                              if (puVar13 == (undefined8 *)0x0) break;
                              if (puVar12 == (undefined8 *)0x73) goto LAB_004121ad;
                              if (puVar12 == local_1d8) {
                                *(undefined4 *)((int)local_744 + (int)puVar12 * 4) = 0;
                                local_748 = (undefined8 *)((int)puVar12 + 1);
                              }
                              puVar1 = (undefined8 *)((int)local_744 + (int)puVar12 * 4);
                              uVar14 = *(uint *)puVar1;
                              *(uint *)puVar1 = *(uint *)puVar1 + (int)puVar13;
                              puVar13 = (undefined8 *)(uint)CARRY4(uVar14,(uint)puVar13);
                              puVar12 = (undefined8 *)((int)puVar12 + 1);
                              local_1d8 = local_748;
                            } while( true );
                          }
                          if (puVar12 == (undefined8 *)0x73) goto LAB_004121ad;
                        }
                        puVar15 = (undefined8 *)((int)puVar15 + 1);
                      } while (puVar15 != local_768);
                    }
                    _MaxCount = (int)local_1d8 << 2;
                    _Src_00 = local_744;
LAB_004120fe:
                    _memcpy_s(&local_1d4,0x1cc,_Src_00,_MaxCount);
LAB_0041210f:
                    bVar7 = true;
                  }
                  if (!bVar7) goto LAB_004121fb;
                  local_764 = (undefined8 *)((int)local_764 - (int)local_74c);
                } while (local_764 != (undefined8 *)0x0);
              }
              if (local_77c._4_4_ != 0) {
                uVar14 = *(uint *)(&DAT_0041bc2c + local_77c._4_4_ * 2);
                local_77c = local_77c & 0xffffffff | (ulonglong)uVar14 << 0x20;
                if (uVar14 == 0) {
LAB_004121fb:
                  local_1d8 = (undefined8 *)0x0;
                  _Src_00 = &local_1d4;
                  goto LAB_00412208;
                }
                if ((uVar14 != 1) && (local_1d8 != (undefined8 *)0x0)) {
                  uVar11 = 0;
                  _Src_00 = (undefined8 *)0x0;
                  do {
                    lVar6 = (ulonglong)uVar14 *
                            (ulonglong)*(uint *)((int)&local_1d4 + (int)_Src_00 * 4);
                    uVar9 = (uint)lVar6;
                    *(uint *)((int)&local_1d4 + (int)_Src_00 * 4) = uVar9 + uVar11;
                    uVar11 = (int)((ulonglong)lVar6 >> 0x20) + (uint)CARRY4(uVar9,uVar11);
                    _Src_00 = (undefined8 *)((int)_Src_00 + 1);
                  } while (_Src_00 != local_1d8);
                  if (uVar11 != 0) {
                    if ((undefined8 *)0x72 < local_1d8) goto LAB_004121fb;
                    *(uint *)((int)&local_1d4 + (int)local_1d8 * 4) = uVar11;
                    local_1d8 = (undefined8 *)((int)local_1d8 + 1);
                  }
                }
              }
            }
            else {
              local_774 = (undefined8 *)(local_76c / 10);
              local_788 = (undefined8 *)(local_76c % 10);
              while (local_774 != (undefined8 *)0x0) {
                local_754 = local_774;
                if ((undefined8 *)0x26 < local_774) {
                  local_754 = (undefined8 *)0x26;
                }
                bVar2 = (&DAT_0041bb97)[(int)local_754 * 4];
                uVar14 = (uint)(byte)(&DAT_0041bb96)[(int)local_754 * 4] * 4;
                local_578 = (undefined8 *)
                            ((uint)(byte)(&DAT_0041bb96)[(int)local_754 * 4] + (uint)bVar2);
                FUN_00402e50((int *)&local_574,0,uVar14);
                copy_buffer((undefined8 *)((int)&local_574 + uVar14),
                            (undefined8 *)
                            (&UNK_0041b290 +
                            (uint)*(ushort *)(&UNK_0041bb94 + (int)local_754 * 4) * 4),
                            (uint)bVar2 << 2);
                ppuVar16 = local_3a4;
                if (local_578 < (undefined8 *)0x2) {
                  if (local_574 == 0) {
                    _MaxCount = 0;
                    local_748 = (undefined8 *)0x0;
                    local_3a8 = (undefined8 *)0x0;
LAB_00411cc0:
                    _Src_00 = local_744;
                    goto LAB_00411cc7;
                  }
                  if ((local_574 == 1) || (local_3a8 == (undefined8 *)0x0)) goto LAB_00411cd8;
                  ppuVar16 = (undefined8 **)0x0;
                  _Src_00 = (undefined8 *)0x0;
                  do {
                    ppuVar3 = *(undefined8 ***)(&local_3a4 + (int)_Src_00);
                    uVar14 = (uint)((ulonglong)local_574 * ZEXT48(ppuVar3));
                    *(undefined8 ***)(&local_3a4 + (int)_Src_00) =
                         (undefined8 **)(uVar14 + (int)ppuVar16);
                    ppuVar16 = (undefined8 **)
                               ((int)((ulonglong)local_574 * ZEXT48(ppuVar3) >> 0x20) +
                               (uint)CARRY4(uVar14,(uint)ppuVar16));
                    _Src_00 = (undefined8 *)((int)_Src_00 + 1);
                  } while (_Src_00 != local_3a8);
LAB_00411b17:
                  if (ppuVar16 == (undefined8 **)0x0) goto LAB_00411cd8;
                  if (local_3a8 < (undefined8 *)0x73) {
                    *(undefined8 ***)(&local_3a4 + (int)local_3a8) = ppuVar16;
                    local_3a8 = (undefined8 *)((int)local_3a8 + 1);
                    goto LAB_00411cd8;
                  }
                  local_578 = (undefined8 *)0x0;
                  _Src_00 = &local_574;
LAB_00411d3b:
                  local_3a8 = (undefined8 *)0x0;
                  _memcpy_s(&local_3a4,0x1cc,_Src_00,0);
                  bVar7 = false;
                }
                else {
                  if ((undefined8 *)0x1 < local_3a8) {
                    local_74c = &local_574;
                    if (local_3a8 <= local_578) {
                      local_74c = &local_3a4;
                    }
                    local_750 = &local_3a4;
                    _Src_00 = local_3a8;
                    local_764 = local_578;
                    if (local_3a8 <= local_578) {
                      local_750 = &local_574;
                      _Src_00 = local_578;
                      local_764 = local_3a8;
                    }
                    local_3a8 = (undefined8 *)0x0;
                    puVar15 = (undefined8 *)0x0;
                    local_748 = (undefined8 *)0x0;
                    if (local_764 != (undefined8 *)0x0) {
                      do {
                        if (*(int *)((int)local_74c + (int)puVar15 * 4) == 0) {
                          if (puVar15 == local_3a8) {
                            *(undefined4 *)((int)local_744 + (int)puVar15 * 4) = 0;
                            local_3a8 = (undefined8 *)((int)puVar15 + 1);
                            local_748 = local_3a8;
                          }
                        }
                        else {
                          local_758 = (undefined8 *)0x0;
                          local_768 = (undefined8 *)0x0;
                          puVar12 = puVar15;
                          if (_Src_00 != (undefined8 *)0x0) {
                            do {
                              puVar13 = local_768;
                              if (puVar12 == (undefined8 *)0x73) break;
                              if (puVar12 == local_3a8) {
                                *(undefined4 *)((int)local_744 + (int)puVar12 * 4) = 0;
                                local_748 = (undefined8 *)
                                            ((char *)((int)local_758 + 1) + (int)puVar15);
                              }
                              lVar6 = (ulonglong)*(uint *)((int)local_750 + (int)local_758 * 4) *
                                      (ulonglong)*(uint *)((int)local_74c + (int)puVar15 * 4);
                              uVar11 = (uint)lVar6;
                              uVar9 = uVar11 + (int)local_768;
                              puVar13 = (undefined8 *)((int)local_744 + (int)puVar12 * 4);
                              uVar14 = *(uint *)puVar13;
                              *(uint *)puVar13 = *(uint *)puVar13 + uVar9;
                              puVar13 = (undefined8 *)
                                        ((int)((ulonglong)lVar6 >> 0x20) +
                                         (uint)CARRY4(uVar11,(uint)local_768) +
                                        (uint)CARRY4(uVar14,uVar9));
                              local_758 = (undefined8 *)((int)local_758 + 1);
                              puVar12 = (undefined8 *)((int)puVar12 + 1);
                              local_77c = local_77c & 0xffffffff | ZEXT48(puVar13) << 0x20;
                              local_3a8 = local_748;
                              local_768 = puVar13;
                            } while (local_758 != _Src_00);
                            do {
                              if (puVar13 == (undefined8 *)0x0) break;
                              if (puVar12 == (undefined8 *)0x73) goto LAB_00411d26;
                              if (puVar12 == local_3a8) {
                                *(undefined4 *)((int)local_744 + (int)puVar12 * 4) = 0;
                                local_748 = (undefined8 *)((int)puVar12 + 1);
                              }
                              puVar1 = (undefined8 *)((int)local_744 + (int)puVar12 * 4);
                              uVar14 = *(uint *)puVar1;
                              *(uint *)puVar1 = *(uint *)puVar1 + (int)puVar13;
                              puVar13 = (undefined8 *)(uint)CARRY4(uVar14,(uint)puVar13);
                              puVar12 = (undefined8 *)((int)puVar12 + 1);
                              local_3a8 = local_748;
                            } while( true );
                          }
                          if (puVar12 == (undefined8 *)0x73) {
LAB_00411d26:
                            _Src_00 = local_964;
                            goto LAB_00411d3b;
                          }
                        }
                        puVar15 = (undefined8 *)((int)puVar15 + 1);
                      } while (puVar15 != local_764);
                    }
                    _MaxCount = (int)local_3a8 << 2;
                    goto LAB_00411cc0;
                  }
                  local_75c = local_3a4;
                  local_3a8 = local_578;
                  _memcpy_s(&local_3a4,0x1cc,&local_574,(int)local_578 << 2);
                  if (ppuVar16 != (undefined8 **)0x0) {
                    if ((ppuVar16 != (undefined8 **)0x1) && (local_3a8 != (undefined8 *)0x0)) {
                      ppuVar16 = (undefined8 **)0x0;
                      _Src_00 = (undefined8 *)0x0;
                      do {
                        ppuVar3 = *(undefined8 ***)(&local_3a4 + (int)_Src_00);
                        uVar14 = (uint)(ZEXT48(local_75c) * ZEXT48(ppuVar3));
                        *(undefined8 ***)(&local_3a4 + (int)_Src_00) =
                             (undefined8 **)(uVar14 + (int)ppuVar16);
                        ppuVar16 = (undefined8 **)
                                   ((int)(ZEXT48(local_75c) * ZEXT48(ppuVar3) >> 0x20) +
                                   (uint)CARRY4(uVar14,(uint)ppuVar16));
                        _Src_00 = (undefined8 *)((int)_Src_00 + 1);
                      } while (_Src_00 != local_3a8);
                      goto LAB_00411b17;
                    }
                    goto LAB_00411cd8;
                  }
                  local_578 = (undefined8 *)0x0;
                  local_3a8 = (undefined8 *)0x0;
                  _MaxCount = 0;
                  _Src_00 = &local_574;
LAB_00411cc7:
                  _memcpy_s(&local_3a4,0x1cc,_Src_00,_MaxCount);
LAB_00411cd8:
                  bVar7 = true;
                }
                if (!bVar7) goto LAB_00411d60;
                local_774 = (undefined8 *)((int)local_774 - (int)local_754);
              }
              if (local_788 != (undefined8 *)0x0) {
                local_788 = *(undefined8 **)(&DAT_0041bc2c + (int)local_788 * 2);
                if (local_788 == (undefined8 *)0x0) {
LAB_00411d60:
                  local_3a8 = (undefined8 *)0x0;
                  _Src_00 = &local_3a4;
LAB_00412208:
                  _memcpy_s(_Src_00,0x1cc,local_964,0);
                }
                else {
                  if ((local_788 != (undefined8 *)0x1) && (local_3a8 != (undefined8 *)0x0)) {
                    ppuVar16 = (undefined8 **)0x0;
                    _Src_00 = (undefined8 *)0x0;
                    do {
                      ppuVar3 = *(undefined8 ***)(&local_3a4 + (int)_Src_00);
                      uVar14 = (uint)(ZEXT48(local_788) * ZEXT48(ppuVar3));
                      *(undefined8 ***)(&local_3a4 + (int)_Src_00) =
                           (undefined8 **)(uVar14 + (int)ppuVar16);
                      ppuVar16 = (undefined8 **)
                                 ((int)(ZEXT48(local_788) * ZEXT48(ppuVar3) >> 0x20) +
                                 (uint)CARRY4(uVar14,(uint)ppuVar16));
                      _Src_00 = (undefined8 *)((int)_Src_00 + 1);
                    } while (_Src_00 != local_3a8);
                    if (ppuVar16 != (undefined8 **)0x0) {
                      if ((undefined8 *)0x72 < local_3a8) goto LAB_00411d60;
                      *(undefined8 ***)(&local_3a4 + (int)local_3a8) = ppuVar16;
                      local_3a8 = (undefined8 *)((int)local_3a8 + 1);
                    }
                  }
                }
              }
            }
            _Src_00 = local_770;
            local_754 = local_770;
            if (local_1d8 != (undefined8 *)0x0) {
              uVar14 = 0;
              puVar15 = (undefined8 *)0x0;
              do {
                lVar6 = (ulonglong)*(uint *)((int)&local_1d4 + (int)puVar15 * 4) * 10;
                uVar11 = (uint)lVar6;
                *(uint *)((int)&local_1d4 + (int)puVar15 * 4) = uVar11 + uVar14;
                uVar14 = (int)((ulonglong)lVar6 >> 0x20) + (uint)CARRY4(uVar11,uVar14);
                puVar15 = (undefined8 *)((int)puVar15 + 1);
              } while (puVar15 != local_1d8);
              local_77c = local_77c & 0xffffffff | (ulonglong)uVar14 << 0x20;
              if (uVar14 != 0) {
                if (local_1d8 < (undefined8 *)0x73) {
                  *(uint *)((int)&local_1d4 + (int)local_1d8 * 4) = uVar14;
                  local_1d8 = (undefined8 *)((int)local_1d8 + 1);
                }
                else {
                  local_1d8 = (undefined8 *)0x0;
                  _memcpy_s(&local_1d4,0x1cc,local_964,0);
                }
              }
            }
            uVar17 = FUN_00410cf0((uint *)&local_1d8,(uint *)&local_3a8);
            if ((int)uVar17 == 10) {
              local_76c = local_76c + 1;
              local_754 = (undefined8 *)((int)_Src_00 + 1);
              *(char *)_Src_00 = '1';
              if (local_3a8 != (undefined8 *)0x0) {
                ppuVar16 = (undefined8 **)0x0;
                puVar15 = (undefined8 *)0x0;
                do {
                  ppuVar3 = *(undefined8 ***)(&local_3a4 + (int)puVar15);
                  uVar14 = (uint)(ZEXT48(ppuVar3) * 10);
                  *(undefined8 ***)(&local_3a4 + (int)puVar15) =
                       (undefined8 **)(uVar14 + (int)ppuVar16);
                  ppuVar16 = (undefined8 **)
                             ((int)(ZEXT48(ppuVar3) * 10 >> 0x20) +
                             (uint)CARRY4(uVar14,(uint)ppuVar16));
                  puVar15 = (undefined8 *)((int)puVar15 + 1);
                } while (puVar15 != local_3a8);
                if (ppuVar16 != (undefined8 **)0x0) {
                  if (local_3a8 < (undefined8 *)0x73) {
                    *(undefined8 ***)(&local_3a4 + (int)local_3a8) = ppuVar16;
                    local_3a8 = (undefined8 *)((int)local_3a8 + 1);
                  }
                  else {
                    local_3a8 = (undefined8 *)0x0;
                    _memcpy_s(&local_3a4,0x1cc,local_964,0);
                  }
                }
              }
LAB_0041236e:
              _Src_00 = (undefined8 *)((int)_Src_00 + 1);
              uVar14 = local_76c;
            }
            else {
              if ((int)uVar17 != 0) {
                *(char *)_Src_00 = (char)uVar17 + '0';
                goto LAB_0041236e;
              }
              uVar14 = local_76c - 1;
            }
            local_784[1] = uVar14;
            if ((-1 < (int)uVar14) && (local_780 < 0x80000000)) {
              local_780 = (undefined8 *)((int)local_780 + uVar14);
            }
            uVar14 = param_6 - 1;
            if (local_780 <= param_6 - 1) {
              uVar14 = (uint)local_780;
            }
            local_780 = (undefined8 *)(uVar14 + (int)local_770);
            if (_Src_00 != local_780) {
              do {
                if (local_1d8 == (undefined8 *)0x0) break;
                uVar14 = 0;
                puVar15 = (undefined8 *)0x0;
                do {
                  lVar6 = (ulonglong)*(uint *)((int)&local_1d4 + (int)puVar15 * 4) * 1000000000;
                  uVar11 = (uint)lVar6;
                  *(uint *)((int)&local_1d4 + (int)puVar15 * 4) = uVar11 + uVar14;
                  uVar14 = (int)((ulonglong)lVar6 >> 0x20) + (uint)CARRY4(uVar11,uVar14);
                  puVar15 = (undefined8 *)((int)puVar15 + 1);
                } while (puVar15 != local_1d8);
                if (uVar14 != 0) {
                  if (local_1d8 < (undefined8 *)0x73) {
                    *(uint *)((int)&local_1d4 + (int)local_1d8 * 4) = uVar14;
                    local_1d8 = (undefined8 *)((int)local_1d8 + 1);
                  }
                  else {
                    local_1d8 = (undefined8 *)0x0;
                    _memcpy_s(&local_1d4,0x1cc,local_964,0);
                  }
                }
                uVar17 = FUN_00410cf0((uint *)&local_1d8,(uint *)&local_3a8);
                puVar15 = (undefined8 *)&DAT_00000008;
                puVar12 = (undefined8 *)((int)local_780 - (int)_Src_00);
                do {
                  uVar4 = uVar17 & 0xffffffff;
                  uVar17 = uVar4 / ZEXT48(local_760);
                  if (puVar15 <= puVar12) {
                    *(char *)((int)puVar15 + (int)_Src_00) = (char)(uVar4 % ZEXT48(local_760)) + '0'
                    ;
                  }
                  puVar15 = (undefined8 *)((int)puVar15 + -1);
                } while (puVar15 != (undefined8 *)0xffffffff);
                if ((undefined8 *)0x9 < puVar12) {
                  puVar12 = (undefined8 *)0x9;
                }
                _Src_00 = (undefined8 *)((int)_Src_00 + (int)puVar12);
              } while (_Src_00 != local_780);
            }
            *(char *)_Src_00 = 0;
            goto LAB_0041247e;
          }
          _Src = "1#IND";
        }
      }
    }
  }
  eVar10 = _strcpy_s((char *)local_770,param_6,_Src);
  if (eVar10 != 0) {
    local_97c = 0;
    uVar19 = 0;
    uVar18 = 0;
    FUN_00407e93();
    pcVar5 = (code *)swi(3);
    (*pcVar5)(uVar18,uVar19,local_97c);
    return;
  }
LAB_0041247e:
  if (local_790 != false) {
    FUN_00413f03((int *)&local_798);
  }
  FUN_00401f78();
  return;
}



uint __cdecl FUN_004125f9(undefined param_1,FILE *param_2)

{
  uint uVar1;
  undefined *puVar2;
  uint *puVar3;
  int iVar4;
  char *pcVar5;
  char *pcVar6;
  __int64 _Var7;
  
  uVar1 = __fileno(param_2);
  if ((param_2->_flag & 0xc0U) == 0) {
    iVar4 = FUN_004104a5(uVar1,(undefined8 *)&param_1,1);
    uVar1 = -(iVar4 + -1) & 0xffffff00U | (uint)(byte)(1 - (iVar4 + -1 != 0));
  }
  else {
    pcVar6 = (char *)0x0;
    pcVar5 = param_2->_ptr + -param_2->_cnt;
    param_2->_ptr = (char *)(param_2->_cnt + 1);
    param_2->_base = (char *)(param_2->_bufsiz + -1);
    if ((int)pcVar5 < 1) {
      if ((uVar1 == 0xffffffff) || (uVar1 == 0xfffffffe)) {
        puVar2 = &DAT_0041e100;
      }
      else {
        puVar2 = (undefined *)((uVar1 & 0x3f) * 0x38 + (&DAT_0041f080)[(int)uVar1 >> 6]);
      }
      if (((puVar2[0x28] & 0x20) != 0) && (_Var7 = FUN_00413cac(uVar1,0,0,2), _Var7 == -1)) {
        puVar3 = (uint *)&param_2->_flag;
        LOCK();
        *puVar3 = *puVar3 | 0x10;
        return CONCAT31((int3)((uint)puVar3 >> 8),1);
      }
    }
    else {
      pcVar6 = (char *)FUN_004104a5(uVar1,(undefined8 *)param_2->_cnt,(uint)pcVar5);
    }
    *(undefined *)param_2->_cnt = param_1;
    uVar1 = (uint)param_2 & 0xffffff00 | (uint)(pcVar6 == pcVar5);
  }
  return uVar1;
}



uint __cdecl FUN_004126c5(int *param_1)

{
  uint uVar1;
  int *hFile;
  byte bVar2;
  uint local_14;
  uint local_10;
  uint local_c;
  uint local_8;
  
  if (((uint)param_1[3] >> 3 & 1) == 0) {
    if ((((param_1[3] & 0xc0U) == 0) || (*param_1 != param_1[1])) &&
       (hFile = (int *)__get_osfhandle(param_1[4]), param_1 = hFile, hFile != (int *)0xffffffff)) {
      bVar2 = 1;
      param_1 = (int *)SetFilePointerEx(hFile,0,(PLARGE_INTEGER)&local_c,1);
      if ((param_1 != (int *)0x0) &&
         (param_1 = (int *)GetFileSizeEx(hFile,(PLARGE_INTEGER)&local_14), param_1 != (int *)0x0)) {
        if ((local_c != local_14) || (local_c = local_8, local_8 != local_10)) {
          bVar2 = 0;
          local_8 = local_c;
        }
        return local_8 & 0xffffff00 | (uint)bVar2;
      }
    }
    uVar1 = (uint)param_1 & 0xffffff00;
  }
  else {
    uVar1 = CONCAT31((uint3)((uint)param_1[3] >> 0xb),1);
  }
  return uVar1;
}



uint __cdecl FUN_0041274c(byte param_1,FILE *param_2)

{
  int _FileHandle;
  undefined4 *puVar1;
  uint uVar2;
  FILE *pFVar3;
  
  _FileHandle = __fileno(param_2);
  if ((param_2->_flag & 6U) == 0) {
    puVar1 = (undefined4 *)FUN_00407f40();
    *puVar1 = 9;
  }
  else {
    if (((uint)param_2->_flag >> 0xc & 1) == 0) {
      if ((param_2->_flag & 1U) != 0) {
        uVar2 = FUN_004126c5((int *)param_2);
        param_2->_base = (char *)0x0;
        if ((char)uVar2 == 0) goto LAB_0041250f;
        param_2->_ptr = (char *)param_2->_cnt;
        LOCK();
        param_2->_flag = param_2->_flag & 0xfffffffe;
      }
      LOCK();
      param_2->_flag = param_2->_flag | 2;
      LOCK();
      param_2->_flag = param_2->_flag & 0xfffffff7;
      param_2->_base = (char *)0x0;
      if (((param_2->_flag & 0x4c0U) == 0) &&
         (((pFVar3 = (FILE *)FUN_0040568c(1), param_2 != pFVar3 &&
           (pFVar3 = (FILE *)FUN_0040568c(2), param_2 != pFVar3)) ||
          (_FileHandle = __isatty(_FileHandle), _FileHandle == 0)))) {
        FUN_004141cd((undefined4 *)param_2);
      }
      uVar2 = FUN_004125f9(param_1,param_2);
      if ((char)uVar2 == 0) {
        LOCK();
        param_2->_flag = param_2->_flag | 0x10;
        return 0xffffffff;
      }
      return (uint)param_1;
    }
    puVar1 = (undefined4 *)FUN_00407f40();
    *puVar1 = 0x22;
  }
LAB_0041250f:
  LOCK();
  param_2->_flag = param_2->_flag | 0x10;
  return 0xffffffff;
}



// Library Function - Single Match
// Name: __isatty
// Library: Visual Studio 2015 Release

int __cdecl __isatty(int _FileHandle)

{
  undefined4 *puVar1;
  
  if (_FileHandle == -2) {
    puVar1 = (undefined4 *)FUN_00407f40();
    *puVar1 = 9;
  }
  else {
    if ((-1 < _FileHandle) && ((uint)_FileHandle < DAT_0041f280)) {
      return (uint)*(byte *)((&DAT_0041f080)[_FileHandle >> 6] + 0x28 + (_FileHandle & 0x3fU) * 0x38
                            ) & 0x40;
    }
    puVar1 = (undefined4 *)FUN_00407f40();
    *puVar1 = 9;
    FUN_00407e83();
  }
  return 0;
}



undefined4 __cdecl FUN_004127ad(byte *param_1,uint param_2,int *param_3)

{
  ushort uVar1;
  undefined4 uVar2;
  undefined4 local_c;
  undefined4 local_8;
  
  if (param_3 == (int *)0x0) {
    param_3 = &DAT_0041f428;
  }
  uVar1 = (short)(param_2 & 0xffff) + 0x2400;
  if (*param_3 == 0) {
    if (0x3ff < uVar1) {
      if (0x3ff < (ushort)((short)param_2 + 0x2800U)) {
        uVar2 = FUN_00410c1d(param_1,param_2 & 0xffff,param_3);
        return uVar2;
      }
      *param_3 = ((param_2 & 0x27ff) + 0x40) * 0x400;
      return 0;
    }
  }
  else {
    if (uVar1 < 0x400) {
      local_c = 0;
      local_8 = 0;
      uVar2 = FUN_00410c1d(param_1,(param_2 & 0x23ff) + *param_3,&local_c);
      *param_3 = 0;
      param_3[1] = 0;
      return uVar2;
    }
  }
  uVar2 = FUN_00410bfe(param_3);
  return uVar2;
}



void __cdecl FUN_00412849(DWORD param_1,LPCWSTR param_2,int param_3,LPWORD param_4)

{
  GetStringTypeW(param_1,param_2,param_3,param_4);
  return;
}



int __cdecl FUN_00412862(PCNZWCH param_1,ushort *param_2,uint param_3)

{
  ushort uVar1;
  undefined4 *puVar2;
  int iVar3;
  ushort uVar4;
  
  if (DAT_0041ef80 == 0) {
    if ((param_1 == (PCNZWCH)0x0) || (param_2 == (ushort *)0x0)) {
      puVar2 = (undefined4 *)FUN_00407f40();
      *puVar2 = 0x16;
      FUN_00407e83();
      iVar3 = 0x7fffffff;
    }
    else {
      if (param_3 < 0x80000000) {
        param_1 = (PCNZWCH)((int)param_1 - (int)param_2);
        do {
          uVar4 = *(ushort *)((int)param_1 + (int)param_2);
          if ((ushort)(uVar4 - 0x41) < 0x1a) {
            uVar4 = uVar4 + 0x20;
          }
          uVar1 = *param_2;
          if ((ushort)(uVar1 - 0x41) < 0x1a) {
            uVar1 = uVar1 + 0x20;
          }
          param_2 = param_2 + 1;
          param_3 = param_3 - 1;
        } while (((param_3 != 0) && (uVar4 != 0)) && (uVar4 == uVar1));
        iVar3 = (uint)uVar4 - (uint)uVar1;
      }
      else {
        puVar2 = (undefined4 *)FUN_00407f40();
        *puVar2 = 0x16;
        FUN_00407e83();
        iVar3 = 0x7fffffff;
      }
    }
  }
  else {
    iVar3 = FUN_00412929(param_1,param_2,param_3,(undefined4 *)0x0);
  }
  return iVar3;
}



int __cdecl FUN_00412929(PCNZWCH param_1,ushort *param_2,uint param_3,undefined4 *param_4)

{
  int iVar1;
  int iVar2;
  undefined4 *puVar3;
  ushort uVar4;
  ushort uVar5;
  int local_14;
  int local_10;
  char local_8;
  
  if (param_3 == 0) {
    iVar1 = 0;
  }
  else {
    if (param_1 == (PCNZWCH)0x0) {
      puVar3 = (undefined4 *)FUN_00407f40();
      *puVar3 = 0x16;
      FUN_00407e83();
      iVar1 = 0x7fffffff;
    }
    else {
      if (param_2 == (ushort *)0x0) {
        puVar3 = (undefined4 *)FUN_00407f40();
        *puVar3 = 0x16;
        FUN_00407e83();
        iVar1 = 0x7fffffff;
      }
      else {
        iVar1 = 0x7fffffff;
        if (param_3 < 0x80000000) {
          FUN_00405ed6(&local_14,param_4);
          if (*(int *)(local_10 + 0xa4) == 0) {
            param_1 = (PCNZWCH)((int)param_1 - (int)param_2);
            do {
              uVar4 = *(ushort *)((int)param_1 + (int)param_2);
              if ((ushort)(uVar4 - 0x41) < 0x1a) {
                uVar4 = uVar4 + 0x20;
              }
              uVar5 = *param_2;
              if ((ushort)(uVar5 - 0x41) < 0x1a) {
                uVar5 = uVar5 + 0x20;
              }
              param_2 = param_2 + 1;
              param_3 = param_3 - 1;
            } while (((param_3 != 0) && (uVar4 != 0)) && (uVar4 == uVar5));
            iVar1 = (uint)uVar4 - (uint)uVar5;
          }
          else {
            iVar2 = FUN_00414226(*(int *)(local_10 + 0xa4),0x1001,param_1,param_3,(PCNZWCH)param_2,
                                 param_3);
            if (iVar2 == 0) {
              puVar3 = (undefined4 *)FUN_00407f40();
              *puVar3 = 0x16;
            }
            else {
              iVar1 = iVar2 + -2;
            }
          }
          if (local_8 != 0) {
            *(uint *)(local_14 + 0x350) = *(uint *)(local_14 + 0x350) & 0xfffffffd;
          }
        }
        else {
          puVar3 = (undefined4 *)FUN_00407f40();
          *puVar3 = 0x16;
          FUN_00407e83();
        }
      }
    }
  }
  return iVar1;
}



undefined4 __cdecl FUN_00412a47(wchar_t **param_1,wchar_t **param_2)

{
  wchar_t wVar1;
  code *pcVar2;
  undefined4 *puVar3;
  wchar_t *_Src;
  errno_t eVar4;
  wchar_t *pwVar5;
  wchar_t *pwVar6;
  wchar_t **ppwVar7;
  uint uVar8;
  undefined4 uVar9;
  wchar_t *_Dst;
  undefined4 uVar10;
  undefined4 uVar11;
  undefined4 uVar12;
  wchar_t **local_8;
  
  *param_2 = (wchar_t *)0x0;
  uVar8 = 0;
  local_8 = param_1;
  pwVar5 = *param_1;
  ppwVar7 = param_1;
  if (pwVar5 != (wchar_t *)0x0) {
    do {
      _Src = pwVar5 + 1;
      do {
        wVar1 = *pwVar5;
        pwVar5 = pwVar5 + 1;
      } while (wVar1 != L'\0');
      ppwVar7 = ppwVar7 + 1;
      uVar8 = uVar8 + ((int)((int)pwVar5 - (int)_Src) >> 1) + 1;
      pwVar5 = *ppwVar7;
    } while (pwVar5 != (wchar_t *)0x0);
    if (1 < uVar8) goto LAB_00412a91;
  }
  uVar8 = 1;
LAB_00412a91:
  pwVar5 = (wchar_t *)__calloc_base(uVar8,2);
  if (pwVar5 == (wchar_t *)0x0) {
    ___acrt_errno_map_os_error(8);
    puVar3 = (undefined4 *)FUN_00407f40();
    uVar9 = 0xc;
    *puVar3 = 0xc;
  }
  else {
    _Src = *param_1;
    _Dst = pwVar5;
    if (_Src != (wchar_t *)0x0) {
      do {
        eVar4 = _wcscpy_s(_Dst,uVar8 - ((int)((int)_Dst - (int)pwVar5) >> 1),_Src);
        if (eVar4 != 0) {
          uVar12 = 0;
          uVar11 = 0;
          uVar10 = 0;
          uVar9 = 0;
          FUN_00407e93();
          pcVar2 = (code *)swi(3);
          uVar9 = (*pcVar2)(uVar9,uVar10,uVar11,uVar12);
          return uVar9;
        }
        pwVar6 = *local_8;
        _Src = pwVar6 + 1;
        do {
          wVar1 = *pwVar6;
          pwVar6 = pwVar6 + 1;
        } while (wVar1 != L'\0');
        local_8 = local_8 + 1;
        pwVar6 = _Dst + ((int)((int)pwVar6 - (int)_Src) >> 1);
        *pwVar6 = L' ';
        _Dst = pwVar6 + 1;
        _Src = *local_8;
      } while (_Src != (wchar_t *)0x0);
      *pwVar6 = L'\0';
    }
    *param_2 = pwVar5;
    uVar9 = 0;
  }
  FID_conflict__free((void *)0x0);
  return uVar9;
}



void __cdecl FUN_00412b37(wint_t **param_1,undefined4 *param_2)

{
  wint_t *pwVar1;
  wchar_t wVar2;
  wint_t wVar3;
  short sVar4;
  ushort uVar5;
  code *pcVar6;
  uint uVar7;
  errno_t eVar8;
  wchar_t *pwVar9;
  undefined8 *_Memory;
  wchar_t *pwVar10;
  int iVar11;
  undefined4 *puVar12;
  wint_t *_Src;
  undefined8 *_Dst;
  ushort *puVar13;
  undefined8 *puVar14;
  wint_t **ppwVar15;
  undefined4 uVar16;
  undefined4 uVar17;
  undefined4 uVar18;
  undefined4 uVar19;
  undefined8 *local_40;
  wint_t *local_3c;
  undefined8 **local_38;
  wint_t **local_34;
  wchar_t *local_30;
  rsize_t local_2c;
  uint local_28;
  char local_21;
  ushort local_20;
  wint_t local_1e;
  undefined2 local_1c;
  undefined2 local_1a;
  undefined2 local_18;
  undefined2 local_16;
  undefined2 local_14;
  undefined2 local_12;
  undefined2 local_10;
  undefined2 local_e;
  undefined2 local_c;
  uint local_8;
  
  local_8 = DAT_0041e004 ^ (uint)&stack0xfffffffc;
  local_34 = param_1;
  *param_2 = 0;
  if (param_1 != (wint_t **)0x0) {
    local_20 = 0x53;
    local_1e = 0x79;
    local_1c = 0x73;
    local_18 = 0x65;
    local_16 = 0x6d;
    local_14 = 0x52;
    local_12 = 0x6f;
    local_c = 0;
    local_1a = 0x74;
    local_30 = (wchar_t *)0x0;
    local_10 = local_12;
    local_e = local_1a;
    eVar8 = FID_conflict___dupenv_s(&local_30,(size_t *)0x0,(wchar_t *)&local_20);
    if (eVar8 == 0) {
      if (local_30 == (wchar_t *)0x0) {
        local_2c = 0xb;
      }
      else {
        pwVar9 = local_30;
        do {
          wVar2 = *pwVar9;
          pwVar9 = pwVar9 + 1;
        } while (wVar2 != L'\0');
        local_2c = ((int)((int)pwVar9 - (int)(local_30 + 1)) >> 1) + 0xc;
      }
      _Src = *param_1;
      local_28 = 2;
      while (_Src != (wint_t *)0x0) {
        pwVar1 = _Src + 1;
        do {
          wVar3 = *_Src;
          _Src = _Src + 1;
        } while (wVar3 != 0);
        param_1 = param_1 + 1;
        local_28 = local_28 + ((int)((int)_Src - (int)pwVar1) >> 1) + 1;
        _Src = *param_1;
      }
      _Memory = FUN_0040e1a6();
      if (_Memory != (undefined8 *)0x0) {
        sVar4 = *(short *)_Memory;
        puVar14 = _Memory;
        while (sVar4 != 0x3d) {
          _Dst = puVar14;
          do {
            sVar4 = *(short *)_Dst;
            _Dst = (undefined8 *)((int)_Dst + 2);
          } while (sVar4 != 0);
          puVar14 = (undefined8 *)
                    ((int)puVar14 + ((int)((int)_Dst - (int)((int)puVar14 + 2)) >> 1) * 2 + 2);
          sVar4 = *(short *)puVar14;
        }
        local_38 = &local_40;
        local_40 = puVar14;
        pwVar10 = operator__((_lambda_dd1c267fe327b8eb9e23cfd134fb4af8_ *)&local_38);
        _Src = *local_34;
        ppwVar15 = local_34;
        local_38 = (undefined8 **)((int)(pwVar10 + -(int)puVar14) >> 1);
        while (_Src != (wint_t *)0x0) {
          puVar13 = &local_20;
          local_3c = &local_1e;
          do {
            uVar5 = *puVar13;
            puVar13 = puVar13 + 1;
          } while (uVar5 != 0);
          iVar11 = FUN_004135d8(_Src,&local_20,(int)((int)puVar13 - (int)local_3c) >> 1);
          if (iVar11 == 0) {
            local_21 = 1;
            goto LAB_00412cda;
          }
          ppwVar15 = ppwVar15 + 1;
          _Src = *ppwVar15;
        }
        local_21 = 0;
        local_28 = local_2c + local_28;
LAB_00412cda:
        local_28 = (int)(undefined8 **)((int)(pwVar10 + -(int)puVar14) >> 1) + local_28;
        puVar14 = (undefined8 *)__calloc_base(local_28,2);
        if (puVar14 == (undefined8 *)0x0) {
          ___acrt_errno_map_os_error(0xe);
          puVar12 = (undefined4 *)FUN_00407f40();
          *puVar12 = 0xc;
        }
        else {
          _Dst = puVar14;
          if (local_38 != (undefined8 **)0x0) {
            uVar7 = (int)local_38 * 2;
            copy_buffer(puVar14,local_40,uVar7);
            local_28 = local_28 - (int)local_38;
            _Dst = (undefined8 *)(uVar7 + (int)puVar14);
          }
          _Src = *local_34;
          while (_Src != (wint_t *)0x0) {
            eVar8 = _wcscpy_s((wchar_t *)_Dst,local_28,(wchar_t *)_Src);
            if (eVar8 != 0) goto LAB_00412e1a;
            _Src = *local_34;
            local_3c = _Src + 1;
            do {
              wVar3 = *_Src;
              _Src = _Src + 1;
            } while (wVar3 != 0);
            iVar11 = ((int)((int)_Src - (int)local_3c) >> 1) + 1;
            local_28 = local_28 - iVar11;
            _Dst = (undefined8 *)((int)_Dst + iVar11 * 2);
            local_34 = local_34 + 1;
            _Src = *local_34;
          }
          if (local_21 == 0) {
            eVar8 = _wcscpy_s((wchar_t *)_Dst,local_2c,(wchar_t *)&local_20);
            if (((eVar8 != 0) || (eVar8 = _wcscat_s((wchar_t *)_Dst,local_2c,L"="), eVar8 != 0)) ||
               ((local_30 != (wchar_t *)0x0 &&
                (eVar8 = _wcscat_s((wchar_t *)_Dst,local_2c,local_30), eVar8 != 0))))
            goto LAB_00412e1a;
            _Dst = (undefined8 *)((int)_Dst + local_2c * 2);
          }
          if (_Dst == puVar14) {
            *(undefined2 *)_Dst = 0;
            _Dst = (undefined8 *)((int)_Dst + 2);
          }
          *(undefined2 *)_Dst = 0;
          *(undefined8 **)param_2 = puVar14;
        }
        FID_conflict__free((void *)0x0);
      }
      FID_conflict__free(_Memory);
    }
    else {
      if (eVar8 == 0x16) {
LAB_00412e1a:
        uVar19 = 0;
        uVar18 = 0;
        uVar17 = 0;
        uVar16 = 0;
        FUN_00407e93();
        pcVar6 = (code *)swi(3);
        (*pcVar6)(uVar16,uVar17,uVar18,uVar19);
        return;
      }
      FUN_00407f40();
    }
    FID_conflict__free(local_30);
  }
  FUN_00401f78();
  return;
}



// Library Function - Single Match
// Name: ??R<lambda_dd1c267fe327b8eb9e23cfd134fb4af8>@@QBEPA_WXZ
// Libraries: Visual Studio 2015 Debug, Visual Studio 2015 Release
// public: wchar_t * __thiscall <lambda_dd1c267fe327b8eb9e23cfd134fb4af8>::operator()(void)const 

wchar_t * __thiscall operator__(_lambda_dd1c267fe327b8eb9e23cfd134fb4af8_ *this)

{
  short sVar1;
  short *psVar2;
  short *psVar3;
  
  psVar2 = **(short ***)this;
  sVar1 = *psVar2;
  while ((((sVar1 == 0x3d && (psVar2[1] != 0)) && (psVar2[2] == 0x3a)) && (psVar2[3] == 0x3d))) {
    psVar3 = psVar2 + 4;
    do {
      sVar1 = *psVar3;
      psVar3 = psVar3 + 1;
    } while (sVar1 != 0);
    psVar2 = psVar2 + ((int)((int)psVar3 - (int)(psVar2 + 5)) >> 1) + 5;
    sVar1 = *psVar2;
  }
  return (wchar_t *)psVar2;
}



// Library Function - Multiple Matches With Different Base Names
// Name: ___acrt_pack_narrow_command_line_and_environment,
// ___acrt_pack_wide_command_line_and_environment
// Library: Visual Studio 2015 Release

undefined4 __cdecl
FID_conflict____acrt_pack_narrow_command_line_and_environment
          (wchar_t **param_1,wint_t **param_2,wchar_t **param_3,void **param_4)

{
  int iVar1;
  undefined4 uVar2;
  void *local_c;
  wchar_t *local_8;
  
  uVar2 = 0;
  local_8 = (wchar_t *)0x0;
  iVar1 = FUN_00412a47(param_1,&local_8);
  if (iVar1 == 0) {
    local_c = (void *)0x0;
    iVar1 = FUN_00412b37(param_2,&local_c);
    if (iVar1 == 0) {
      *param_3 = local_8;
      *param_4 = local_c;
      FID_conflict__free((void *)0x0);
      FID_conflict__free((void *)0x0);
    }
    else {
      FID_conflict__free(local_c);
      FID_conflict__free(local_8);
      uVar2 = 0xffffffff;
    }
  }
  else {
    FID_conflict__free(local_8);
    uVar2 = 0xffffffff;
  }
  return uVar2;
}



uint __cdecl FUN_00413016(short *param_1,short *param_2,uint param_3)

{
  short *psVar1;
  short sVar2;
  undefined4 *puVar3;
  short *psVar4;
  
  if (param_2 == (short *)0x0) {
    puVar3 = (undefined4 *)FUN_00407f40();
    *puVar3 = 0x16;
    return 0;
  }
  if (param_3 != 0) {
    *param_2 = 0;
  }
  if (param_3 < 2) {
    puVar3 = (undefined4 *)FUN_00407f40();
    *puVar3 = 0x16;
    return 0;
  }
  sVar2 = *param_1;
  while (sVar2 == 0x3b) {
    param_1 = param_1 + 1;
    sVar2 = *param_1;
  }
  psVar1 = param_2 + (param_3 - 1);
  psVar4 = param_1;
  while (sVar2 != 0) {
    if (sVar2 == 0x3b) goto LAB_00412ff7;
    psVar4 = psVar4 + 1;
    if (sVar2 == 0x22) {
      sVar2 = *psVar4;
      while ((sVar2 != 0 && (sVar2 != 0x22))) {
        *param_2 = sVar2;
        psVar4 = psVar4 + 1;
        param_2 = param_2 + 1;
        if (param_2 == psVar1) {
          *param_2 = 0;
          puVar3 = (undefined4 *)FUN_00407f40();
          *puVar3 = 0x22;
          return 0;
        }
        sVar2 = *psVar4;
      }
      if (sVar2 != 0) {
        psVar4 = psVar4 + 1;
      }
    }
    else {
      *param_2 = sVar2;
      param_2 = param_2 + 1;
      if (param_2 == psVar1) {
        *param_2 = 0;
        puVar3 = (undefined4 *)FUN_00407f40();
        *puVar3 = 0x22;
        return 0;
      }
    }
    sVar2 = *psVar4;
  }
LAB_00412fff:
  *param_2 = 0;
  return -(uint)(psVar4 != param_1) & (uint)psVar4;
LAB_00412ff7:
  do {
    psVar4 = psVar4 + 1;
  } while (*psVar4 == 0x3b);
  goto LAB_00412fff;
}



// Library Function - Single Match
// Name: ?swap@@YAXPAD0I@Z
// Library: Visual Studio 2015 Release
// void __cdecl swap(char *,char *,unsigned int)

void __cdecl swap(char *param_1,char *param_2,uint param_3)

{
  char *pcVar1;
  char cVar2;
  char *pcVar3;
  
  if ((param_1 != param_2) && (param_3 != 0)) {
    pcVar3 = param_2;
    do {
      pcVar1 = pcVar3 + 1;
      cVar2 = pcVar1[(int)(param_1 + -(int)param_2 + -1)];
      pcVar1[(int)(param_1 + -(int)param_2 + -1)] = *pcVar3;
      *pcVar3 = cVar2;
      param_3 = param_3 - 1;
      pcVar3 = pcVar1;
    } while (param_3 != 0);
  }
  return;
}



void __cdecl FUN_00413070(char *param_1,uint param_2,uint param_3,undefined *param_4)

{
  char cVar1;
  undefined4 *puVar2;
  int iVar3;
  char *pcVar4;
  char *pcVar5;
  uint uVar6;
  char *pcVar7;
  char *pcVar8;
  char *pcVar9;
  char *local_114;
  int local_110;
  char *local_108;
  char *local_fc;
  char *apcStack248 [30];
  char *apcStack128 [30];
  uint local_8;
  
  local_8 = DAT_0041e004 ^ (uint)&stack0xfffffffc;
  local_108 = param_1;
  if ((((param_1 == (char *)0x0) && (param_2 != 0)) || (param_3 == 0)) ||
     (param_4 == (undefined *)0x0)) {
    puVar2 = (undefined4 *)FUN_00407f40();
    *puVar2 = 0x16;
    FUN_00407e83();
  }
  else {
    local_110 = 0;
    if (1 < param_2) {
      pcVar4 = param_1 + (param_2 - 1) * param_3;
LAB_004130f2:
      while (uVar6 = (uint)(pcVar4 + -(int)local_108) / param_3 + 1, 8 < uVar6) {
        pcVar8 = local_108 + (uVar6 >> 1) * param_3;
        pcVar7 = local_108;
        pcVar5 = pcVar8;
        _guard_check_icall();
        iVar3 = (*(code *)param_4)(pcVar7,pcVar5);
        if (0 < iVar3) {
          swap(local_108,pcVar8,param_3);
        }
        pcVar7 = local_108;
        pcVar5 = pcVar4;
        _guard_check_icall();
        iVar3 = (*(code *)param_4)(pcVar7,pcVar5);
        if (0 < iVar3) {
          swap(local_108,pcVar4,param_3);
        }
        pcVar7 = pcVar8;
        pcVar5 = pcVar4;
        _guard_check_icall();
        iVar3 = (*(code *)param_4)(pcVar7,pcVar5);
        local_114 = local_108;
        local_fc = pcVar4;
        if (0 < iVar3) {
          swap(pcVar8,pcVar4,param_3);
          local_114 = local_108;
          local_fc = pcVar4;
        }
LAB_00413290:
        if (local_114 < pcVar8) {
          do {
            local_114 = local_114 + param_3;
            if (pcVar8 <= local_114) goto LAB_004132d1;
            pcVar7 = local_114;
            pcVar5 = pcVar8;
            _guard_check_icall();
            iVar3 = (*(code *)param_4)(pcVar7,pcVar5);
          } while (iVar3 < 1);
          pcVar7 = local_fc;
          if (pcVar8 <= local_114) goto LAB_004132d1;
        }
        else {
LAB_004132d1:
          do {
            local_114 = local_114 + param_3;
            pcVar7 = local_fc;
            if (pcVar4 < local_114) break;
            pcVar7 = local_114;
            pcVar5 = pcVar8;
            _guard_check_icall();
            iVar3 = (*(code *)param_4)(pcVar7,pcVar5);
            pcVar7 = local_fc;
          } while (iVar3 < 1);
        }
        do {
          local_fc = pcVar7;
          pcVar7 = local_fc + -param_3;
          if (pcVar7 <= pcVar8) break;
          pcVar5 = pcVar7;
          pcVar9 = pcVar8;
          _guard_check_icall();
          iVar3 = (*(code *)param_4)(pcVar5,pcVar9);
        } while (0 < iVar3);
        if (local_114 <= pcVar7) {
          if (local_114 != pcVar7) {
            pcVar5 = pcVar7;
            uVar6 = param_3;
            do {
              pcVar9 = pcVar5 + 1;
              cVar1 = pcVar9[(int)(local_114 + -(int)pcVar7 + -1)];
              pcVar9[(int)(local_114 + -(int)pcVar7 + -1)] = *pcVar5;
              *pcVar5 = cVar1;
              uVar6 = uVar6 - 1;
              pcVar5 = pcVar9;
            } while (uVar6 != 0);
          }
          local_fc = pcVar7;
          if (pcVar8 == pcVar7) {
            pcVar8 = local_114;
            local_fc = pcVar7;
          }
          goto LAB_00413290;
        }
        if (pcVar8 < local_fc) {
          do {
            local_fc = local_fc + -param_3;
            if (local_fc <= pcVar8) goto LAB_004133f1;
            pcVar7 = local_fc;
            pcVar5 = pcVar8;
            _guard_check_icall();
            iVar3 = (*(code *)param_4)(pcVar7,pcVar5);
          } while (iVar3 == 0);
          if (local_fc <= pcVar8) goto LAB_004133f1;
        }
        else {
LAB_004133f1:
          do {
            local_fc = local_fc + -param_3;
            if (local_fc <= local_108) break;
            pcVar7 = local_fc;
            pcVar5 = pcVar8;
            _guard_check_icall();
            iVar3 = (*(code *)param_4)(pcVar7,pcVar5);
          } while (iVar3 == 0);
        }
        if ((int)(local_fc + -(int)local_108) < (int)(pcVar4 + -(int)local_114)) goto LAB_00413477;
        if (local_108 < local_fc) {
          apcStack128[local_110] = local_108;
          apcStack248[local_110] = local_fc;
          local_110 = local_110 + 1;
        }
        local_108 = local_114;
        if (pcVar4 <= local_114) goto LAB_004131b5;
      }
      while (pcVar7 = local_108, pcVar8 = local_108, local_108 < pcVar4) {
        while (pcVar8 = pcVar8 + param_3, pcVar8 <= pcVar4) {
          pcVar5 = pcVar8;
          pcVar9 = pcVar7;
          _guard_check_icall();
          iVar3 = (*(code *)param_4)(pcVar5,pcVar9);
          if (0 < iVar3) {
            pcVar7 = pcVar8;
          }
        }
        if (pcVar7 != pcVar4) {
          pcVar8 = pcVar4;
          uVar6 = param_3;
          do {
            cVar1 = (pcVar7 + -(int)pcVar4)[(int)pcVar8];
            (pcVar8 + 1)[(int)(pcVar7 + -(int)pcVar4 + -1)] = *pcVar8;
            *pcVar8 = cVar1;
            uVar6 = uVar6 - 1;
            pcVar8 = pcVar8 + 1;
          } while (uVar6 != 0);
        }
        pcVar4 = pcVar4 + -param_3;
      }
      goto LAB_004131b5;
    }
  }
LAB_004130be:
  FUN_00401f78();
  return;
LAB_00413477:
  if (local_114 < pcVar4) {
    apcStack128[local_110] = local_114;
    apcStack248[local_110] = pcVar4;
    local_110 = local_110 + 1;
  }
  pcVar4 = local_fc;
  if (local_fc <= local_108) {
LAB_004131b5:
    iVar3 = local_110 + -1;
    if (0 < local_110) {
      local_108 = apcStack128[iVar3];
      local_110 = iVar3;
      pcVar4 = apcStack248[iVar3];
      goto LAB_004130f2;
    }
    goto LAB_004130be;
  }
  goto LAB_004130f2;
}



// Library Function - Single Match
// Name: _strpbrk
// Library: Visual Studio

char * __cdecl _strpbrk(char *_Str,char *_Control)

{
  byte bVar1;
  char *pcVar2;
  byte *pbVar3;
  byte abStack40 [32];
  
  while (bVar1 = *_Control, bVar1 != 0) {
    _Control = (char *)((byte *)_Control + 1);
    abStack40[(int)(uint)bVar1 >> 3] = abStack40[(int)(uint)bVar1 >> 3] | 1 << ((uint)bVar1 & 7);
  }
  do {
    pbVar3 = (byte *)_Str;
    pcVar2 = (char *)(uint)*pbVar3;
    if (*pbVar3 == 0) {
      return pcVar2;
    }
    _Str = (char *)(pbVar3 + 1);
  } while ((abStack40[(int)pcVar2 >> 3] >> ((uint)pcVar2 & 7) & 1) == 0);
  return (char *)pbVar3;
}



// Library Function - Single Match
// Name: __towlower_l
// Library: Visual Studio 2015 Release

wint_t __cdecl __towlower_l(wint_t _C,_locale_t _Locale)

{
  uint uVar1;
  int iVar2;
  ushort uVar3;
  undefined2 in_stack_00000006;
  int local_1c;
  int local_18;
  char local_10;
  ushort local_8 [2];
  
  uVar3 = 0xffff;
  if (_C != 0xffff) {
    FUN_00405ed6(&local_1c,(undefined4 *)_Locale);
    if (*(int *)(local_18 + 0xa8) == 0) {
      uVar3 = _C;
      if ((ushort)(_C - 0x41) < 0x1a) {
        uVar3 = _C + 0x20;
      }
    }
    else {
      if (_C < 0x100) {
        uVar1 = FUN_0040c58c(_C,1);
        uVar3 = _C;
        if (uVar1 != 0) {
          uVar3 = (ushort)*(byte *)(*(int *)(local_18 + 0x94) + (__C & 0xffff));
        }
      }
      else {
        iVar2 = ___acrt_LCMapStringW
                          (*(int *)(local_18 + 0xa8),0x100,(LPCWSTR)&_C,1,(LPWSTR)local_8,1);
        uVar3 = local_8[0];
        if (iVar2 == 0) {
          uVar3 = (ushort)__C;
        }
      }
    }
    if (local_10 != 0) {
      *(uint *)(local_1c + 0x350) = *(uint *)(local_1c + 0x350) & 0xfffffffd;
    }
  }
  return (wint_t)uVar3;
}



int __cdecl FUN_004135d8(wint_t *param_1,ushort *param_2,int param_3)

{
  ushort uVar1;
  int iVar2;
  undefined4 *puVar3;
  ushort uVar4;
  
  if (DAT_0041ef80 == 0) {
    iVar2 = 0;
    if (param_3 != 0) {
      if ((param_1 == (wint_t *)0x0) || (param_2 == (ushort *)0x0)) {
        puVar3 = (undefined4 *)FUN_00407f40();
        *puVar3 = 0x16;
        FUN_00407e83();
        iVar2 = 0x7fffffff;
      }
      else {
        param_1 = (wint_t *)((int)param_1 - (int)param_2);
        do {
          uVar4 = *(ushort *)((int)param_1 + (int)param_2);
          if ((ushort)(uVar4 - 0x41) < 0x1a) {
            uVar4 = uVar4 + 0x20;
          }
          uVar1 = *param_2;
          if ((ushort)(uVar1 - 0x41) < 0x1a) {
            uVar1 = uVar1 + 0x20;
          }
          param_2 = param_2 + 1;
          param_3 = param_3 + -1;
        } while (((param_3 != 0) && (uVar4 != 0)) && (uVar4 == uVar1));
        iVar2 = (uint)uVar4 - (uint)uVar1;
      }
    }
  }
  else {
    iVar2 = FUN_00413689(param_1,param_2,param_3,(undefined4 *)0x0);
  }
  return iVar2;
}



int __cdecl FUN_00413689(wint_t *param_1,wint_t *param_2,int param_3,undefined4 *param_4)

{
  ushort uVar1;
  ushort uVar2;
  undefined4 *puVar3;
  int iVar4;
  int local_18;
  localeinfo_struct local_14;
  char local_c;
  
  iVar4 = 0;
  if (param_3 != 0) {
    if ((param_1 == (wint_t *)0x0) || (param_2 == (wint_t *)0x0)) {
      puVar3 = (undefined4 *)FUN_00407f40();
      *puVar3 = 0x16;
      FUN_00407e83();
      iVar4 = 0x7fffffff;
    }
    else {
      FUN_00405ed6(&local_18,param_4);
      if ((local_14.locinfo)->locale_name[2] == (wchar_t *)0x0) {
        param_1 = (wint_t *)((int)param_1 - (int)param_2);
        do {
          uVar1 = *(ushort *)((int)param_1 + (int)param_2);
          if ((ushort)(uVar1 - 0x41) < 0x1a) {
            uVar1 = uVar1 + 0x20;
          }
          uVar2 = *param_2;
          if ((ushort)(uVar2 - 0x41) < 0x1a) {
            uVar2 = uVar2 + 0x20;
          }
          param_2 = param_2 + 1;
          param_3 = param_3 + -1;
        } while (((param_3 != 0) && (uVar1 != 0)) && (uVar1 == uVar2));
      }
      else {
        do {
          uVar1 = __towlower_l(*param_1,(_locale_t)&local_14);
          uVar2 = __towlower_l(*param_2,(_locale_t)&local_14);
          param_3 = param_3 + -1;
          param_1 = param_1 + 1;
          param_2 = param_2 + 1;
          if ((param_3 == 0) || (uVar1 == 0)) break;
        } while (uVar1 == uVar2);
      }
      iVar4 = (uint)uVar1 - (uint)uVar2;
      if (local_c != 0) {
        *(uint *)(local_18 + 0x350) = *(uint *)(local_18 + 0x350) & 0xfffffffd;
      }
    }
  }
  return iVar4;
}



// Library Function - Single Match
// Name: __msize
// Library: Visual Studio 2015 Release

size_t __cdecl __msize(void *_Memory)

{
  undefined4 *puVar1;
  SIZE_T SVar2;
  
  if (_Memory == (void *)0x0) {
    puVar1 = (undefined4 *)FUN_00407f40();
    *puVar1 = 0x16;
    FUN_00407e83();
    return 0xffffffff;
  }
  SVar2 = HeapSize(hHeap_0041f410,0,_Memory);
  return SVar2;
}



// Library Function - Single Match
// Name: __realloc_base
// Library: Visual Studio 2015 Release

LPVOID __cdecl __realloc_base(void *param_1,uint param_2)

{
  LPVOID pvVar1;
  undefined4 *puVar2;
  int iVar3;
  
  if (param_1 == (void *)0x0) {
    pvVar1 = __malloc_base(param_2);
  }
  else {
    if (param_2 == 0) {
      FID_conflict__free(param_1);
    }
    else {
      if (param_2 < 0xffffffe1) {
        do {
          pvVar1 = HeapReAlloc(hHeap_0041f410,0,param_1,param_2);
          if (pvVar1 != (LPVOID)0x0) {
            return pvVar1;
          }
          iVar3 = FUN_00409982();
        } while ((iVar3 != 0) && (iVar3 = FUN_0040f19a(param_2), iVar3 != 0));
      }
      puVar2 = (undefined4 *)FUN_00407f40();
      *puVar2 = 0xc;
    }
    pvVar1 = (LPVOID)0x0;
  }
  return pvVar1;
}



// WARNING: Function: __SEH_prolog4 replaced with injection: SEH_prolog4
// WARNING: Function: __SEH_epilog4 replaced with injection: EH_epilog3
// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_00413822(undefined4 param_1)

{
  return;
}



int FUN_00413898(void)

{
  short in_FPUStatusWord;
  
  return (int)in_FPUStatusWord;
}



// Library Function - Single Match
// Name: __ctrlfp
// Library: Visual Studio 2015 Release

int __ctrlfp(void)

{
  short in_FPUControlWord;
  
  return (int)in_FPUControlWord;
}



// Library Function - Single Match
// Name: __set_statfp
// Library: Visual Studio 2015 Release

void __set_statfp(undefined4 param_1)

{
  return;
}



int FUN_00413932(void)

{
  short in_FPUStatusWord;
  
  return (int)in_FPUStatusWord;
}



// WARNING: Function: __SEH_prolog4 replaced with injection: SEH_prolog4
// WARNING: Function: __SEH_epilog4 replaced with injection: EH_epilog3
// Library Function - Single Match
// Name: 
??$?RV<lambda_123407a5e2ac06da108355a851863b7a>@@AAV<lambda_2fe9b910cf3cbf4a0ab98a02ba45b3ec>@@V<lambda_ae55bdf541ad94d75914d381c370e64d>@@@?$__crt_seh_guarded_call@H@@QAEH$$QAV<lambda_123407a5e2ac06da108355a851863b7a>@@AAV<lambda_2fe9b910cf3cbf4a0ab98a02ba45b3ec>@@$$QAV<lambda_ae55bdf541ad94d75914d381c370e64d>@@@Z
// Library: Visual Studio 2015 Release
// public: int __thiscall __crt_seh_guarded_call<int>::operator()<class
// <lambda_123407a5e2ac06da108355a851863b7a>,class <lambda_2fe9b910cf3cbf4a0ab98a02ba45b3ec>&,class
// <lambda_ae55bdf541ad94d75914d381c370e64d>>(class
// <lambda_123407a5e2ac06da108355a851863b7a>&&,class
// <lambda_2fe9b910cf3cbf4a0ab98a02ba45b3ec>&,class <lambda_ae55bdf541ad94d75914d381c370e64d>&&)

int __thiscall

operator___class__lambda_123407a5e2ac06da108355a851863b7a__class__lambda_2fe9b910cf3cbf4a0ab98a02ba45b3ec___class__lambda_ae55bdf541ad94d75914d381c370e64d___
          (__crt_seh_guarded_call_int_ *this,_lambda_123407a5e2ac06da108355a851863b7a_ *param_1,
          _lambda_2fe9b910cf3cbf4a0ab98a02ba45b3ec_ *param_2,
          _lambda_ae55bdf541ad94d75914d381c370e64d_ *param_3)

{
  uint _FileHandle;
  int iVar1;
  undefined4 *puVar2;
  
  ___acrt_lowio_lock_fh(*(uint *)param_1);
  _FileHandle = **(uint **)param_2;
  if ((*(byte *)((&DAT_0041f080)[(int)_FileHandle >> 6] + 0x28 + (_FileHandle & 0x3f) * 0x38) & 1)
      == 0) {
    puVar2 = (undefined4 *)FUN_00407f40();
    *puVar2 = 9;
    iVar1 = -1;
  }
  else {
    iVar1 = __close_nolock(_FileHandle);
  }
  FUN_004139ba();
  return iVar1;
}



void FUN_004139ba(void)

{
  int unaff_EBP;
  
  ___acrt_lowio_unlock_fh(**(uint **)(unaff_EBP + 0x10));
  return;
}



int __cdecl FUN_004139c6(uint param_1)

{
  int iVar1;
  undefined4 *puVar2;
  uint local_14;
  uint *local_10;
  uint local_c;
  __crt_seh_guarded_call_int_ local_5;
  
  if (param_1 == 0xfffffffe) {
    puVar2 = (undefined4 *)FUN_00407f2d();
    *puVar2 = 0;
    puVar2 = (undefined4 *)FUN_00407f40();
    *puVar2 = 9;
  }
  else {
    if (((-1 < (int)param_1) && (param_1 < DAT_0041f280)) &&
       ((*(byte *)((&DAT_0041f080)[(int)param_1 >> 6] + 0x28 + (param_1 & 0x3f) * 0x38) & 1) != 0))
    {
      local_10 = &param_1;
      local_c = param_1;
      local_14 = param_1;
      iVar1 = 
              operator___class__lambda_123407a5e2ac06da108355a851863b7a__class__lambda_2fe9b910cf3cbf4a0ab98a02ba45b3ec___class__lambda_ae55bdf541ad94d75914d381c370e64d___
                        (&local_5,(_lambda_123407a5e2ac06da108355a851863b7a_ *)&local_14,
                         (_lambda_2fe9b910cf3cbf4a0ab98a02ba45b3ec_ *)&local_10,
                         (_lambda_ae55bdf541ad94d75914d381c370e64d_ *)&local_c);
      return iVar1;
    }
    puVar2 = (undefined4 *)FUN_00407f2d();
    *puVar2 = 0;
    puVar2 = (undefined4 *)FUN_00407f40();
    *puVar2 = 9;
    FUN_00407e83();
  }
  return -1;
}



// Library Function - Single Match
// Name: __close_nolock
// Library: Visual Studio 2015 Release

int __cdecl __close_nolock(int _FileHandle)

{
  intptr_t iVar1;
  intptr_t iVar2;
  HANDLE hObject;
  BOOL BVar3;
  int iVar4;
  DWORD DVar5;
  
  iVar1 = __get_osfhandle(_FileHandle);
  if (iVar1 != -1) {
    if (((_FileHandle == 1) && ((*(byte *)(DAT_0041f080 + 0x98) & 1) != 0)) ||
       ((_FileHandle == 2 && ((*(byte *)(DAT_0041f080 + 0x60) & 1) != 0)))) {
      iVar1 = __get_osfhandle(2);
      iVar2 = __get_osfhandle(1);
      if (iVar2 == iVar1) goto LAB_00413a6b;
    }
    hObject = (HANDLE)__get_osfhandle(_FileHandle);
    BVar3 = CloseHandle(hObject);
    if (BVar3 == 0) {
      DVar5 = GetLastError();
      goto LAB_00413abd;
    }
  }
LAB_00413a6b:
  DVar5 = 0;
LAB_00413abd:
  __free_osfhnd(_FileHandle);
  *(undefined *)((&DAT_0041f080)[_FileHandle >> 6] + 0x28 + (_FileHandle & 0x3fU) * 0x38) = 0;
  if (DVar5 == 0) {
    iVar4 = 0;
  }
  else {
    ___acrt_errno_map_os_error(DVar5);
    iVar4 = -1;
  }
  return iVar4;
}



// Library Function - Single Match
// Name: ?__acrt_stdio_free_stream@@YAXV__crt_stdio_stream@@@Z
// Library: Visual Studio 2015 Release
// void __cdecl __acrt_stdio_free_stream(class __crt_stdio_stream)

void __cdecl __acrt_stdio_free_stream(__crt_stdio_stream param_1)

{
  undefined3 in_stack_00000005;
  
  *_param_1 = 0;
  _param_1[1] = 0;
  _param_1[2] = 0;
  _param_1[4] = 0xffffffff;
  _param_1[5] = 0;
  _param_1[6] = 0;
  _param_1[7] = 0;
  _param_1[3] = 0;
  return;
}



// WARNING: Function: __SEH_prolog4 replaced with injection: SEH_prolog4
// WARNING: Function: __SEH_epilog4 replaced with injection: EH_epilog3

__int64 __cdecl FUN_00413b2c(uint param_1,undefined4 param_2,undefined4 param_3,int param_4)

{
  undefined4 *puVar1;
  int iVar2;
  __int64 _Var3;
  
  if (param_1 == 0xfffffffe) {
    puVar1 = (undefined4 *)FUN_00407f2d();
    *puVar1 = 0;
    puVar1 = (undefined4 *)FUN_00407f40();
    *puVar1 = 9;
  }
  else {
    if ((-1 < (int)param_1) && (param_1 < DAT_0041f280)) {
      iVar2 = (param_1 & 0x3f) * 0x38;
      if ((*(byte *)((&DAT_0041f080)[(int)param_1 >> 6] + 0x28 + iVar2) & 1) != 0) {
        ___acrt_lowio_lock_fh(param_1);
        _Var3 = -1;
        if ((*(byte *)((&DAT_0041f080)[(int)param_1 >> 6] + 0x28 + iVar2) & 1) == 0) {
          puVar1 = (undefined4 *)FUN_00407f40();
          *puVar1 = 9;
          puVar1 = (undefined4 *)FUN_00407f2d();
          *puVar1 = 0;
        }
        else {
          _Var3 = common_lseek_nolock___int64_(param_1,CONCAT44(param_3,param_2),param_4);
        }
        FUN_00413c01();
        return _Var3;
      }
    }
    puVar1 = (undefined4 *)FUN_00407f2d();
    *puVar1 = 0;
    puVar1 = (undefined4 *)FUN_00407f40();
    *puVar1 = 9;
    FUN_00407e83();
  }
  return -1;
}



void FUN_00413c01(void)

{
  uint unaff_EDI;
  
  ___acrt_lowio_unlock_fh(unaff_EDI);
  return;
}



// Library Function - Single Match
// Name: ??$common_lseek_nolock@_J@@YA_JH_JH@Z
// Library: Visual Studio 2015 Release
// __int64 __cdecl common_lseek_nolock<__int64>(int,__int64,int)

__int64 __cdecl common_lseek_nolock___int64_(int param_1,__int64 param_2,int param_3)

{
  byte *pbVar1;
  HANDLE hFile;
  undefined4 *puVar2;
  BOOL BVar3;
  DWORD DVar4;
  uint local_c;
  uint local_8;
  
  hFile = (HANDLE)__get_osfhandle(param_1);
  if (hFile == (HANDLE)0xffffffff) {
    puVar2 = (undefined4 *)FUN_00407f40();
    *puVar2 = 9;
  }
  else {
    BVar3 = SetFilePointerEx(hFile,param_2,(PLARGE_INTEGER)&local_c,param_3);
    if (BVar3 == 0) {
      DVar4 = GetLastError();
      ___acrt_errno_map_os_error(DVar4);
    }
    else {
      if ((local_c & local_8) != 0xffffffff) {
        pbVar1 = (byte *)((&DAT_0041f080)[param_1 >> 6] + 0x28 + (param_1 & 0x3fU) * 0x38);
        *pbVar1 = *pbVar1 & 0xfd;
        goto LAB_00413ca6;
      }
    }
  }
  local_c = 0xffffffff;
  local_8 = 0xffffffff;
LAB_00413ca6:
  return CONCAT44(local_8,local_c);
}



__int64 __cdecl FUN_00413cac(uint param_1,undefined4 param_2,undefined4 param_3,int param_4)

{
  __int64 _Var1;
  
  _Var1 = FUN_00413b2c(param_1,param_2,param_3,param_4);
  return _Var1;
}



// Library Function - Single Match
// Name: __lseeki64_nolock
// Library: Visual Studio 2015 Release

longlong __cdecl __lseeki64_nolock(int _FileHandle,longlong _Offset,int _Origin)

{
  __int64 _Var1;
  
  _Var1 = common_lseek_nolock___int64_(_FileHandle,_Offset,_Origin);
  return (longlong)_Var1;
}



uint __cdecl FUN_00413ce2(ushort param_1)

{
  uint uVar1;
  DWORD local_8;
  
  uVar1 = FUN_004142e8();
  if ((uVar1 != 0) && (uVar1 = FUN_0041431e(&param_1,1,&local_8), uVar1 != 0)) {
    return uVar1 & 0xffff0000 | (uint)param_1;
  }
  return 0xffff;
}



// Library Function - Single Match
// Name: ___strncnt
// Library: Visual Studio 2015 Release

void __cdecl ___strncnt(char *param_1,int param_2)

{
  char cVar1;
  int iVar2;
  
  iVar2 = 0;
  cVar1 = *param_1;
  while ((cVar1 != 0 && (iVar2 != param_2))) {
    iVar2 = iVar2 + 1;
    cVar1 = param_1[iVar2];
  }
  return;
}



// WARNING: Could not reconcile some variable overlaps

void __cdecl FUN_00413d32(uint param_1,byte *param_2,byte *param_3,byte **param_4)

{
  byte bVar1;
  ushort uVar2;
  byte bVar3;
  byte *pbVar4;
  byte *pbVar5;
  byte *pbVar6;
  byte **ppbVar7;
  byte local_28;
  byte *local_20;
  byte *local_1c;
  byte local_16;
  byte local_15;
  undefined4 local_14;
  undefined4 local_10;
  undefined4 local_c;
  uint local_8;
  
  local_8 = DAT_0041e004 ^ (uint)&stack0xfffffffc;
  if (param_4 == (byte **)0x0) {
    param_4 = (byte **)&DAT_0041f430;
  }
  pbVar6 = param_2;
  if (param_2 == (byte *)0x0) {
    param_3 = (byte *)0x1;
    pbVar6 = &DAT_0041b288;
  }
  ppbVar7 = (byte **)(-(uint)(param_2 != (byte *)0x0) & param_1);
  if (param_3 == (byte *)0x0) goto LAB_00413ed5;
  local_28 = (byte)*(short *)((int)param_4 + 6);
  if (*(short *)((int)param_4 + 6) == 0) {
    local_16 = *pbVar6;
    pbVar6 = pbVar6 + 1;
    if (-1 < (char)local_16) {
      if (ppbVar7 != (byte **)0x0) {
        *ppbVar7 = (byte *)(uint)local_16;
      }
      goto LAB_00413ed5;
    }
    if ((local_16 & 0xe0) == 0xc0) {
      bVar3 = 2;
    }
    else {
      if ((local_16 & 0xf0) == 0xe0) {
        bVar3 = 3;
      }
      else {
        if ((local_16 & 0xf8) != 0xf0) goto LAB_00413ece;
        bVar3 = 4;
      }
    }
    uVar2 = (ushort)bVar3;
    pbVar5 = (byte *)((1 << (7 - bVar3 & 0x1f)) - 1U & (uint)local_16);
    local_15 = bVar3;
    local_28 = bVar3;
LAB_00413e1b:
    pbVar4 = (byte *)((uint)uVar2 & 0xff);
    local_20 = param_3;
    if (pbVar4 < param_3) {
      local_20 = pbVar4;
    }
    local_1c = pbVar6 + -(int)param_2;
    while (local_1c < local_20) {
      bVar1 = *pbVar6;
      pbVar6 = pbVar6 + 1;
      local_1c = local_1c + 1;
      if ((bVar1 & 0xc0) != 0x80) goto LAB_00413ece;
      pbVar5 = (byte *)((int)pbVar5 << 6 | (uint)bVar1 & 0x3f);
    }
    if (local_20 < pbVar4) {
      *(ushort *)(param_4 + 1) = (ushort)bVar3;
      *param_4 = pbVar5;
      *(ushort *)((int)param_4 + 6) = (ushort)(byte)(local_28 - (char)local_20);
      goto LAB_00413ed5;
    }
    if (((pbVar5 < (byte *)0xd800) || ((byte *)0xdfff < pbVar5)) && (pbVar5 < (byte *)0x110000)) {
      local_14 = 0x80;
      local_10 = 0x800;
      local_c = 0x10000;
      if ((&local_1c)[(uint)bVar3] <= pbVar5) {
        if (ppbVar7 != (byte **)0x0) {
          *ppbVar7 = pbVar5;
        }
        *param_4 = (byte *)0x0;
        param_4[1] = (byte *)0x0;
        goto LAB_00413ed5;
      }
    }
  }
  else {
    bVar3 = *(byte *)(param_4 + 1);
    pbVar5 = *param_4;
    if ((byte)(bVar3 - 2) < 3) {
      uVar2 = *(ushort *)((int)param_4 + 6);
      if (((byte)uVar2 != 0) && ((byte)uVar2 < bVar3)) goto LAB_00413e1b;
    }
  }
LAB_00413ece:
  FUN_00410bfe(param_4);
LAB_00413ed5:
  FUN_00401f78();
  return;
}



undefined4 __cdecl FUN_00413ee6(undefined4 *param_1)

{
  undefined4 uVar1;
  
  uVar1 = FUN_00414684();
  *param_1 = uVar1;
  uVar1 = FUN_00414706();
  param_1[1] = uVar1;
  return 0;
}



undefined4 __cdecl FUN_00413f03(int *param_1)

{
  int iVar1;
  undefined4 uVar2;
  int local_c;
  int local_8;
  
  FUN_004147c0(*param_1);
  FUN_0041482a(param_1[1]);
  local_c = 0;
  local_8 = 0;
  iVar1 = FUN_00413ee6(&local_c);
  if (((iVar1 == 0) && (*param_1 == local_c)) && (param_1[1] == local_8)) {
    uVar2 = 0;
  }
  else {
    uVar2 = 1;
  }
  return uVar2;
}



undefined4 __cdecl FUN_00413f50(uint *param_1)

{
  int iVar1;
  uint local_c;
  uint local_8;
  
  local_c = 0;
  local_8 = 0;
  iVar1 = FUN_00413ee6(&local_c);
  if (iVar1 == 0) {
    param_1[1] = local_8;
    *param_1 = local_c;
    local_c = local_c | 0x1f;
    iVar1 = FUN_00413f03((int *)&local_c);
    if (iVar1 == 0) {
      FUN_0040f5bb();
      return 0;
    }
  }
  return 1;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_00413fa0(void)

{
  bool bVar1;
  float10 in_ST0;
  uint in_MXCSR;
  ushort in_FPUControlWord;
  
  if (_DAT_0041f450 != 0) {
    bVar1 = (in_MXCSR & 0x7f80) == 0x1f80;
    if (bVar1) {
      bVar1 = (in_FPUControlWord & 0x7f) == 0x7f;
    }
    if (bVar1) {
      start(SUB84((double)in_ST0,0),(int)((ulonglong)(double)in_ST0 >> 0x20));
      return;
    }
  }
  FUN_004150c8(SUB84((double)in_ST0,0),(uint)((ulonglong)(double)in_ST0 >> 0x20));
  FUN_00413ff8();
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined8 __fastcall FUN_00413ff8(undefined4 uParm1,uint uParm2,int param_1,uint param_2)

{
  uint in_EAX;
  uint uVar1;
  undefined4 uVar2;
  bool in_ZF;
  ushort in_FPUControlWord;
  ushort in_FPUStatusWord;
  uint local_4;
  
  local_4 = uParm2 & 0xffff0000 | (uint)in_FPUControlWord;
  if (in_ZF) {
    if (((in_EAX & 0xfffff) == 0) && (param_1 == 0)) {
      param_2 = in_EAX & 0x80000000;
      uVar1 = param_2;
      goto joined_r0x00414047;
    }
    param_2 = FUN_0041506c();
  }
  else {
    if ((param_2 & 0x7ff00000) == 0) {
      if (((param_2 & 0xfffff) == 0) && (param_1 == 0)) {
        param_2 = 2;
        goto LAB_00414091;
      }
      uVar1 = param_2 & 0x80000000;
    }
    else {
      uVar1 = param_2 & 0x80000000;
    }
joined_r0x00414047:
    if (uVar1 == 0) {
      if (_DAT_0041f438 == 0) {
        uVar1 = param_2 & 0xffff0000 | (uint)in_FPUControlWord;
        if (in_FPUControlWord != 0x27f) {
          if (((in_FPUControlWord & 0x20) != 0) &&
             (uVar1 = param_2 & 0xffff0000 | (uint)in_FPUStatusWord, (in_FPUStatusWord & 0x20) != 0)
             ) {
            uVar2 = __startOneArgErrorHandling();
            return CONCAT44(local_4,uVar2);
          }
          uVar1 = uVar1 & 0xffff0020;
        }
        return CONCAT44(local_4,uVar1);
      }
      goto LAB_004150de;
    }
    param_2 = 1;
  }
LAB_00414091:
  if (_DAT_0041f438 == 0) {
    uVar2 = __startOneArgErrorHandling();
    return CONCAT44(local_4,uVar2);
  }
LAB_004150de:
  return CONCAT44(local_4,param_2);
}



// WARNING: Removing unreachable block (ram,0x00414149)
// WARNING: Globals starting with '_' overlap smaller symbols at the same address

float10 FUN_004140b0(double param_1)

{
  double dVar1;
  int iVar2;
  uint uVar3;
  bool in_PF;
  bool bVar4;
  unkbyte10 Var5;
  float10 fVar6;
  uint in_MXCSR;
  ushort in_FPUControlWord;
  undefined auVar7 [16];
  undefined auVar8 [16];
  undefined auVar9 [16];
  uint *puVar10;
  undefined4 uVar11;
  undefined4 uVar12;
  
  if (_DAT_0041f450 != 0) {
    bVar4 = (in_MXCSR & 0x7f80) == 0x1f80;
    if (bVar4) {
      bVar4 = (in_FPUControlWord & 0x7f) == 0x7f;
    }
    if (bVar4) {
      auVar7 = psrlq(ZEXT816((ulonglong)param_1),0x34);
      iVar2 = SUB164(auVar7,0);
      auVar9 = psubd(CONCAT88(0x433,0x433),
                     (auVar7 & (undefined  [16])0xffffffffffffffff | ZEXT816(0) << 0x40) &
                     (undefined  [16])0xffffffffffffffff);
      auVar8 = psrlq(ZEXT816((ulonglong)param_1),auVar9);
      if ((auVar7 & (undefined  [16])0x800) == (undefined  [16])0x0) {
        auVar7 = psllq(auVar8,auVar9);
        if (iVar2 < 0x3ff) {
          return (float10)(double)(-(ulonglong)(0.00000000 < param_1) & 0x3ff0000000000000);
        }
        if (iVar2 < 0x433) {
          return (float10)(SUB168(auVar7,0) + (double)-(ulonglong)(SUB168(auVar7,0) < param_1));
        }
      }
      else {
        if (iVar2 < 0xbff) {
          return (float10)-0.00000000;
        }
        auVar7 = psllq(auVar8,auVar9);
        if (iVar2 < 0xc33) {
          return (float10)SUB168(auVar7,0);
        }
      }
      return (float10)param_1;
    }
  }
  uVar3 = __ctrlfp(0x1b3f,0xffff);
  if ((param_1._6_2_ & 0x7ff0) == 0x7ff0) {
    iVar2 = __sptype(SUB84(param_1,0),(uint)((ulonglong)param_1 >> 0x20));
    if (2 < iVar2 - 1U) {
      uVar11 = SUB84(param_1 + 1.00000000,0);
      uVar12 = (undefined4)((ulonglong)(param_1 + 1.00000000) >> 0x20);
      puVar10 = (uint *)&DAT_00000008;
LAB_00415481:
      Var5 = __except1(puVar10,0xc,param_1,CONCAT44(uVar12,uVar11),uVar3);
      return (float10)Var5;
    }
    __ctrlfp(uVar3,0xffff);
    dVar1 = param_1;
  }
  else {
    fVar6 = __frnd(param_1);
    dVar1 = (double)fVar6;
    if ((in_PF) && ((uVar3 & 0x20) == 0)) {
      uVar11 = SUB84((double)fVar6,0);
      uVar12 = (undefined4)((ulonglong)(double)fVar6 >> 0x20);
      puVar10 = (uint *)&DAT_00000010;
      goto LAB_00415481;
    }
    __ctrlfp(uVar3,0xffff);
  }
  return (float10)dVar1;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void __cdecl FUN_004141cd(undefined4 *param_1)

{
  uint *puVar1;
  LPVOID pvVar2;
  undefined4 uVar3;
  
  _DAT_0041ed2c = _DAT_0041ed2c + 1;
  uVar3 = 0x1000;
  pvVar2 = __malloc_base(0x1000);
  *(LPVOID *)(param_1 + 1) = pvVar2;
  FID_conflict__free((void *)0x0);
  puVar1 = param_1 + 3;
  if (param_1[1] == 0) {
    LOCK();
    *puVar1 = *puVar1 | 0x400;
    *(undefined4 **)(param_1 + 1) = param_1 + 5;
    uVar3 = 2;
  }
  else {
    LOCK();
    *puVar1 = *puVar1 | 0x40;
  }
  param_1[6] = uVar3;
  param_1[2] = 0;
  *param_1 = param_1[1];
  return;
}



int __cdecl
FUN_00414226(int param_1,DWORD param_2,PCNZWCH param_3,int param_4,PCNZWCH param_5,int param_6)

{
  int iVar1;
  
  if (0 < param_4) {
    param_4 = FUN_0040b0fd(param_3,param_4);
  }
  if (0 < param_6) {
    param_6 = FUN_0040b0fd(param_5,param_6);
  }
  if ((param_4 == 0) || (param_6 == 0)) {
    if (param_4 - param_6 == 0) {
      iVar1 = 2;
    }
    else {
      iVar1 = (param_4 - param_6 >> 0x1f & 0xfffffffeU) + 3;
    }
  }
  else {
    iVar1 = FUN_0040a2b9(param_1,param_2,param_3,param_4,param_5,param_6,0,0,0);
  }
  return iVar1;
}



// Library Function - Single Match
// Name: ___acrt_LCMapStringW
// Library: Visual Studio 2015 Release

void __cdecl
___acrt_LCMapStringW
          (int param_1,DWORD param_2,LPCWSTR param_3,int param_4,LPWSTR param_5,int param_6)

{
  int iVar1;
  int iVar2;
  
  iVar2 = param_4;
  if (0 < param_4) {
    iVar1 = FUN_0040b0fd(param_3,param_4);
    iVar2 = iVar1 + 1;
    if (param_4 <= iVar1) {
      iVar2 = iVar1;
    }
  }
  FUN_0040a460(param_1,param_2,param_3,iVar2,param_5,param_6,0,0,0);
  return;
}



void FUN_004142c9(void)

{
  hConsoleOutput_0041e8a0 =
       CreateFileW(L"CONOUT$",0x40000000,3,(LPSECURITY_ATTRIBUTES)0x0,3,0,(HANDLE)0x0);
  return;
}



uint FUN_004142e8(void)

{
  if ((undefined *)hConsoleOutput_0041e8a0 == &DAT_fffffffe) {
    FUN_004142c9();
  }
  return (uint)(hConsoleOutput_0041e8a0 != (HANDLE)0xffffffff);
}



void FUN_00414307(void)

{
  if ((hConsoleOutput_0041e8a0 != (HANDLE)0xffffffff) &&
     ((undefined *)hConsoleOutput_0041e8a0 != &DAT_fffffffe)) {
    CloseHandle(hConsoleOutput_0041e8a0);
  }
  return;
}



BOOL __cdecl FUN_0041431e(void *param_1,DWORD param_2,LPDWORD param_3)

{
  BOOL BVar1;
  DWORD DVar2;
  
  BVar1 = WriteConsoleW(hConsoleOutput_0041e8a0,param_1,param_2,param_3,(LPVOID)0x0);
  if (BVar1 == 0) {
    DVar2 = GetLastError();
    if (DVar2 == 6) {
      FUN_00414307();
      FUN_004142c9();
      BVar1 = WriteConsoleW(hConsoleOutput_0041e8a0,param_1,param_2,param_3,(LPVOID)0x0);
    }
  }
  return BVar1;
}



uint __cdecl FUN_00414373(uint param_1)

{
  ushort uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  
  uVar4 = 0;
  uVar1 = (ushort)param_1 & 0x8040;
  if (uVar1 == 0x8000) {
    uVar3 = 0xc00;
  }
  else {
    if (uVar1 == 0x40) {
      uVar3 = 0x800;
    }
    else {
      uVar3 = 0x400;
      if (uVar1 != 0x8040) {
        uVar3 = 0;
      }
    }
  }
  uVar2 = param_1 & 0x6000;
  if (uVar2 != 0) {
    if (uVar2 == 0x2000) {
      uVar4 = 0x100;
    }
    else {
      if (uVar2 == 0x4000) {
        uVar4 = 0x200;
      }
      else {
        if (uVar2 == 0x6000) {
          uVar4 = 0x300;
        }
      }
    }
  }
  return (param_1 >> 10 & 1) << 2 |
         (param_1 >> 8 & 1) << 5 | (param_1 >> 7 & 1) << 4 | (param_1 >> 9 & 1) << 3 |
         (param_1 >> 0xb & 1) * 2 | param_1 >> 0xc & 1 | uVar4 | uVar3;
}



uint __cdecl FUN_0041442f(uint param_1)

{
  uint uVar1;
  uint uVar2;
  uint local_8;
  
  local_8 = 0x1000;
  uVar2 = 0;
  if ((param_1 & 0x300) == 0) {
    local_8 = 0x2000;
  }
  else {
    if ((param_1 & 0x300) != 0x200) {
      local_8 = 0;
    }
  }
  uVar1 = param_1 & 0xc00;
  if (uVar1 != 0) {
    if (uVar1 == 0x400) {
      uVar2 = 0x100;
    }
    else {
      if (uVar1 == 0x800) {
        uVar2 = 0x200;
      }
      else {
        if (uVar1 == 0xc00) {
          uVar2 = 0x300;
        }
      }
    }
  }
  return (param_1 >> 4 & 1) * 2 |
         (param_1 >> 1 & 1) << 5 | (param_1 >> 2 & 1) << 3 | (param_1 >> 3 & 1) << 2 |
         (param_1 & 1) << 4 | param_1 >> 5 & 1 | (param_1 & 0x1000) << 2 | uVar2 | local_8;
}



uint __cdecl FUN_004144ea(uint param_1)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  
  uVar1 = param_1 >> 2;
  uVar3 = param_1 & 0xc00;
  uVar4 = 0;
  if (uVar3 == 0x400) {
    uVar3 = 0x8040;
  }
  else {
    if (uVar3 == 0x800) {
      uVar3 = 0x40;
    }
    else {
      if (uVar3 == 0xc00) {
        uVar3 = 0x8000;
      }
      else {
        uVar3 = 0;
      }
    }
  }
  uVar2 = (param_1 & 0xc0000000) >> 0x16;
  if (uVar2 != 0) {
    if (uVar2 == 0x100) {
      uVar4 = 0x2000;
    }
    else {
      if (uVar2 == 0x200) {
        uVar4 = 0x4000;
      }
      else {
        if (uVar2 == 0x300) {
          uVar4 = 0x6000;
        }
      }
    }
  }
  return ((uVar1 & 0x8000000) >> 0x1b) << 8 |
         ((uVar1 & 0x800000) >> 0x17) << 0xb | ((uVar1 & 0x1000000) >> 0x18) << 10 |
         ((uVar1 & 0x2000000) >> 0x19) << 9 | ((uVar1 & 0x4000000) >> 0x1a) << 7 |
         ((uVar1 & 0x400000) >> 0x16) << 0xc | uVar3 | uVar4;
}



uint __cdecl FUN_004145b7(uint param_1)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  
  uVar3 = param_1 >> 2;
  uVar4 = 0;
  if ((param_1 & 0x3000) == 0) {
    uVar2 = 0x300;
  }
  else {
    uVar2 = uVar4;
    if ((param_1 & 0x3000) == 0x1000) {
      uVar2 = 0x200;
    }
  }
  uVar1 = (param_1 & 0xc00000) >> 0xe;
  if (uVar1 != 0) {
    if (uVar1 == 0x100) {
      uVar4 = 0x400;
    }
    else {
      if (uVar1 == 0x200) {
        uVar4 = 0x800;
      }
      else {
        if (uVar1 == 0x300) {
          uVar4 = 0xc00;
        }
      }
    }
  }
  return ((uVar3 & 0x20000) >> 0x11) << 2 |
         ((uVar3 & 0x8000) >> 0xf) << 4 | ((uVar3 & 0x10000) >> 0x10) << 3 |
         ((uVar3 & 0x80000) >> 0x13) * 2 | (uVar3 & 0x40000) >> 0x12 |
         ((uVar3 & 0x4000) >> 0xe) << 5 | param_1 >> 2 & 0x1000 | uVar2 | uVar4;
}



uint FUN_00414684(void)

{
  uint uVar1;
  uint uVar2;
  int iVar3;
  uint *puVar4;
  uint in_MXCSR;
  ushort in_FPUControlWord;
  uint local_24 [8];
  
  iVar3 = 7;
  puVar4 = local_24;
  while (iVar3 != 0) {
    iVar3 = iVar3 + -1;
    *puVar4 = 0;
    puVar4 = puVar4 + 1;
  }
  local_24[0] = local_24[0] & 0xffff0000 | (uint)in_FPUControlWord;
  uVar1 = FUN_0041442f((uint)in_FPUControlWord & 7999);
  if (DAT_0041ec74 < 1) {
    in_MXCSR = 0;
  }
  else {
    in_MXCSR = in_MXCSR & 0xffc0;
  }
  uVar2 = FUN_00414373(in_MXCSR);
  return uVar2 | ((((uVar2 & 0x3f) << 2 | uVar2 & 0xffffff00) << 6 | uVar1 & 0x3f) << 2 |
                 uVar1 & 0x300) << 0xe | uVar1;
}



uint FUN_00414706(void)

{
  uint uVar1;
  uint uVar2;
  uint in_MXCSR;
  ushort in_FPUStatusWord;
  
  uVar1 = (uint)in_FPUStatusWord & 0x3f;
  uVar1 = (uVar1 >> 1 & 1) << 5 | (uVar1 >> 2 & 1) << 3 | (uVar1 >> 3 & 1) << 2 |
          (uVar1 >> 4 & 1) * 2 | ((uint)in_FPUStatusWord & 1) << 4 | uVar1 >> 5;
  if (DAT_0041ec74 < 1) {
    uVar2 = 0;
  }
  else {
    uVar2 = in_MXCSR & 0x3f;
  }
  uVar2 = (uVar2 >> 1 & 1) << 5 | (uVar2 >> 2 & 1) << 3 | (uVar2 >> 3 & 1) << 2 |
          (uVar2 >> 4 & 1) * 2 | (uVar2 & 1) << 4 | uVar2 >> 5;
  return (uVar2 << 8 | uVar1) << 0x10 | uVar2 | uVar1;
}



void FUN_004147c0(uint param_1)

{
  uint uVar1;
  int iVar2;
  undefined4 *puVar3;
  undefined2 in_FPUControlWord;
  undefined4 local_24;
  
  uVar1 = FUN_004145b7(param_1);
  iVar2 = 7;
  puVar3 = &local_24;
  while (iVar2 != 0) {
    iVar2 = iVar2 + -1;
    *puVar3 = 0;
    puVar3 = puVar3 + 1;
  }
  local_24 = CONCAT22(local_24._2_2_,in_FPUControlWord);
  local_24 = local_24 ^ (uVar1 & 0xffff ^ local_24) & 7999;
  FUN_004144ea(param_1);
  return;
}



void FUN_0041482a(undefined4 param_1)

{
  int iVar1;
  undefined4 *puVar2;
  undefined4 local_24 [8];
  
  iVar1 = 7;
  puVar2 = local_24;
  while (iVar1 != 0) {
    iVar1 = iVar1 + -1;
    *puVar2 = 0;
    puVar2 = puVar2 + 1;
  }
  return;
}



// Library Function - Single Match
// Name: start
// Library: Visual Studio

float10 start(undefined8 param_1)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  int iVar4;
  int iVar5;
  double dVar6;
  double dVar7;
  undefined in_XMM0 [16];
  undefined auVar8 [16];
  double dVar9;
  double dVar10;
  undefined auVar11 [16];
  undefined auVar12 [16];
  undefined auVar13 [16];
  double dVar14;
  undefined auVar15 [16];
  ulonglong uVar16;
  double local_c;
  
  iVar4 = 0;
  while( true ) {
    auVar13 = psrlq(in_XMM0,0x34);
    auVar11 = CONCAT88(0x3fdbc00000000000,0x3fdbc00000000000);
    auVar8 = orpd(in_XMM0 & (undefined  [16])0xffffffffffffffff,
                  CONCAT88(0x3ff0000000000000,0x3ff0000000000000));
    auVar12 = addpd(CONCAT88(0x428ffffffffff810,0x428ffffffffff810),auVar8);
    uVar1 = (uint)SUB162(auVar12,0) & 0x7f0;
    dVar6 = (double)(SUB168(auVar8 >> 0x40,0) & 0x7fffffff80000000);
    auVar12 = subpd(auVar8,(auVar8 & (undefined  [16])0xffffffffffffffff |
                           ZEXT816((ulonglong)dVar6) << 0x40) & (undefined  [16])0xffffffffffffffff)
    ;
    auVar15 = subpd(CONCAT88(dVar6 * *(double *)(&DAT_0041c348 + uVar1),
                             (double)(SUB168(auVar8,0) & 0x7fffffff80000000) *
                             *(double *)(&DAT_0041c340 + uVar1)),auVar11);
    dVar6 = SUB168(auVar12,0) * *(double *)(&DAT_0041c340 + uVar1);
    dVar9 = SUB168(auVar12 >> 0x40,0) * *(double *)(&DAT_0041c348 + uVar1);
    auVar8 = addpd(CONCAT88(dVar9,dVar6),auVar15);
    uVar2 = (uint)SUB162(auVar13,0) & 0xfff;
    uVar3 = uVar2 - 1;
    if (uVar3 < 0x7fe) {
      iVar4 = (uVar2 - 0x3ff) + iVar4;
      dVar14 = (double)iVar4;
      iVar5 = 0;
      if (uVar1 + iVar4 * 0x400 == 0) {
        iVar5 = 0x10;
      }
      dVar7 = SUB168(auVar8,0);
      dVar10 = SUB168(auVar8 >> 0x40,0);
      auVar8 = addpd(CONCAT88(dVar10 * -3.07179526,dVar7 * 21.53547326),
                     CONCAT88(0x3ffc6a02dc9635d2,0xc025c9806a358455));
      auVar12 = addpd(CONCAT88(*(undefined8 *)(&DAT_0041bf38 + uVar1),
                               *(double *)(&DAT_0041bf30 + uVar1) + SUB168(auVar15,0)),
                      CONCAT88(dVar14 * 0.00000000,dVar14 * 0.30103000));
      auVar12 = addpd(auVar12,CONCAT88((ulonglong)dVar9 & *(ulonglong *)(&DAT_0041bd68 + iVar5),
                                       (ulonglong)dVar6 & *(ulonglong *)(&DAT_0041bd60 + iVar5)));
      auVar8 = addpd(CONCAT88(SUB168(auVar8 >> 0x40,0) * dVar10,SUB168(auVar8,0) * dVar7),
                     CONCAT88(0xbff27af2dc77b135,0x4016ab9f7e1899f7));
      dVar6 = SUB168(auVar8 >> 0x40,0) * dVar10 * dVar10;
      auVar8 = addpd(CONCAT88(dVar6,SUB168(auVar8,0) * dVar7 * dVar7 * dVar7 * dVar7 * dVar7),
                     CONCAT88(dVar10 * 0.00000000,dVar7 * 0.00161610));
      dVar6 = (double)movlpd(local_c,CONCAT88(dVar6,dVar6 + SUB168(auVar8,0) +
                                                    SUB168(auVar12 >> 0x40,0) + SUB168(auVar12,0)));
      return (float10)dVar6;
    }
    auVar8 = movlpd(auVar8,param_1);
    uVar16 = -(ulonglong)(SUB168(auVar8,0) == 0.00000000);
    auVar12 = ZEXT816(uVar16);
    if ((short)uVar16 != 0) break;
    if (uVar3 != 0xffffffff) {
      if (uVar3 < 0x7ff) {
        auVar8 = movlpd(auVar8,param_1);
        auVar12 = CONCAT88(0xfffffffffffff,0xfffffffffffff);
        auVar8 = orpd((auVar8 & (undefined  [16])0xffffffffffffffff |
                      ZEXT816(SUB168(auVar8 >> 0x40,0) & 0xfffffffffffff) << 0x40) &
                      (undefined  [16])0xffffffffffffffff,
                      CONCAT88(0x3ff0000000000000,0x3ff0000000000000));
        if (SUB168(auVar8,0) == 1.00000000) {
          return (float10)INFINITY;
        }
        iVar4 = 0x3e9;
      }
      else {
        if (0x7fe < ((uint)SUB162(auVar13,0) & 0x7ff)) {
          auVar11 = movlpd(auVar11,param_1);
          movlpd(auVar8,param_1);
          auVar8 = psrlq(auVar11,0x20);
          if ((SUB164(auVar11,0) | SUB164(auVar8,0) & 0xfffff) != 0) {
            iVar4 = 0x3e9;
            goto LAB_00414b6a;
          }
        }
        auVar12 = ZEXT816(0xfff8000000000000);
        iVar4 = 9;
      }
      goto LAB_00414b6a;
    }
    auVar12 = movlpd(auVar12,0x4330000000000000);
    in_XMM0 = CONCAT88(SUB168(auVar8 >> 0x40,0),SUB168(auVar8,0) * SUB168(auVar12,0));
    iVar4 = -0x34;
  }
  movlpd(auVar11,0x3ff0000000000000);
  auVar12 = movlpd(auVar12,0xfff0000000000000);
  iVar4 = 8;
LAB_00414b6a:
  local_c = (double)movlpd(local_c,auVar12);
  FUN_00415223(&param_1,&param_1,&local_c,iVar4);
  return (float10)local_c;
}



undefined4 FUN_0041506c(void)

{
  uint in_EAX;
  
  if ((in_EAX & 0x80000) != 0) {
    return 0;
  }
  return 0;
}



uint __cdecl FUN_004150c8(undefined4 param_1,uint param_2)

{
  if ((param_2 & 0x7ff00000) != 0x7ff00000) {
    return param_2 & 0x7ff00000;
  }
  return param_2;
}



float10 __fastcall
FUN_004151d0(undefined4 uParm1,uint uParm2,ushort param_1,undefined4 param_2,undefined4 param_3,
            undefined4 param_4,undefined4 param_5,undefined4 param_6)

{
  float10 in_ST0;
  int local_24 [2];
  undefined4 uStack28;
  undefined4 uStack24;
  undefined4 local_14;
  undefined4 local_10;
  double dStack12;
  
  local_14 = param_5;
  local_10 = param_6;
  dStack12 = (double)in_ST0;
  uStack28 = param_3;
  uStack24 = param_4;
  FUN_004156c6(uParm2,local_24,&param_1);
  return (float10)dStack12;
}



// Library Function - Single Match
// Name: __startOneArgErrorHandling
// Library: Visual Studio

float10 __fastcall
__startOneArgErrorHandling
          (undefined4 uParm1,uint uParm2,ushort param_1,undefined4 param_2,undefined4 param_3,
          undefined4 param_4)

{
  float10 in_ST0;
  int local_24 [2];
  undefined4 local_1c;
  undefined4 local_18;
  double local_c;
  
  local_c = (double)in_ST0;
  local_1c = param_3;
  local_18 = param_4;
  FUN_004156c6(uParm2,local_24,&param_1);
  return (float10)local_c;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void __cdecl FUN_00415223(undefined8 *param_1,undefined8 *param_2,undefined8 *param_3,int param_4)

{
  undefined8 uVar1;
  int iVar2;
  code *pcVar3;
  undefined4 *puVar4;
  undefined4 local_24;
  char *local_20;
  undefined8 local_1c;
  undefined8 local_14;
  undefined8 local_c;
  
  if (_DAT_0041f43c == 0) {
    pcVar3 = FUN_00408ebd;
  }
  else {
    pcVar3 = (code *)DecodePointer(Ptr_0041f44c);
  }
  if (0x1a < param_4) {
    if (param_4 != 0x1b) {
      if (param_4 == 0x1c) {
        local_20 = "pow";
      }
      else {
        if (param_4 == 0x31) {
          local_20 = "sqrt";
        }
        else {
          if (param_4 == 0x3a) {
            local_20 = "acos";
          }
          else {
            if (param_4 != 0x3d) {
              if ((param_4 != 1000) && (param_4 != 0x3e9)) {
                return;
              }
              uVar1 = *param_1;
              goto LAB_0041532b;
            }
            local_20 = "asin";
          }
        }
      }
      goto LAB_0041538b;
    }
    local_24 = 2;
LAB_004153d1:
    local_20 = "pow";
    goto LAB_004153d8;
  }
  if (param_4 == 0x1a) {
    uVar1 = 0x3ff0000000000000;
LAB_0041532b:
    *param_3 = uVar1;
    return;
  }
  if (0xe < param_4) {
    if (param_4 == 0xf) {
      local_20 = &DAT_0041bf00;
    }
    else {
      if (param_4 == 0x18) {
        local_24 = 3;
        goto LAB_004153d1;
      }
      if (param_4 != 0x19) {
        return;
      }
      local_20 = &DAT_0041bf04;
    }
    local_24 = 4;
    local_1c = *param_1;
    local_14 = *param_2;
    puVar4 = &local_24;
    local_c = *param_3;
    _guard_check_icall();
    (*pcVar3)(puVar4);
    goto LAB_0041540e;
  }
  if (param_4 == 0xe) {
    local_24 = 3;
    local_20 = "exp";
  }
  else {
    if (param_4 != 2) {
      if (param_4 == 3) {
        local_20 = "log";
      }
      else {
        if (param_4 == 8) {
          local_24 = 2;
          local_20 = "log10";
          goto LAB_004153d8;
        }
        if (param_4 != 9) {
          return;
        }
        local_20 = "log10";
      }
LAB_0041538b:
      local_24 = 1;
      local_1c = *param_1;
      local_14 = *param_2;
      puVar4 = &local_24;
      local_c = *param_3;
      _guard_check_icall();
      iVar2 = (*pcVar3)(puVar4);
      if (iVar2 == 0) {
        puVar4 = (undefined4 *)FUN_00407f40();
        *puVar4 = 0x21;
      }
      goto LAB_0041540e;
    }
    local_24 = 2;
    local_20 = "log";
  }
LAB_004153d8:
  local_1c = *param_1;
  local_14 = *param_2;
  puVar4 = &local_24;
  local_c = *param_3;
  _guard_check_icall();
  iVar2 = (*pcVar3)(puVar4);
  if (iVar2 == 0) {
    puVar4 = (undefined4 *)FUN_00407f40();
    *puVar4 = 0x22;
  }
LAB_0041540e:
  *param_3 = local_c;
  return;
}



// Library Function - Single Match
// Name: ___ascii_strnicmp
// Library: Visual Studio

int __cdecl ___ascii_strnicmp(char *_Str1,char *_Str2,size_t _MaxCount)

{
  byte bVar1;
  byte bVar2;
  bool bVar3;
  
  if (_MaxCount != 0) {
    do {
      bVar2 = *_Str1;
      bVar1 = *_Str2;
      if ((bVar2 == 0) || (bVar1 == 0)) break;
      _Str1 = (char *)((byte *)_Str1 + 1);
      _Str2 = (char *)((byte *)_Str2 + 1);
      if ((0x40 < bVar2) && (bVar2 < 0x5b)) {
        bVar2 = bVar2 + 0x20;
      }
      if ((0x40 < bVar1) && (bVar1 < 0x5b)) {
        bVar1 = bVar1 + 0x20;
      }
      bVar3 = bVar2 < bVar1;
      if (bVar2 != bVar1) goto LAB_00415521;
      _MaxCount = _MaxCount - 1;
    } while (_MaxCount != 0);
    _MaxCount = 0;
    bVar3 = bVar2 < bVar1;
    if (bVar2 != bVar1) {
LAB_00415521:
      _MaxCount = 0xffffffff;
      if (!bVar3) {
        _MaxCount = 1;
      }
    }
  }
  return _MaxCount;
}



// Library Function - Single Match
// Name: __d_inttype
// Library: Visual Studio 2015 Release

undefined4 __d_inttype(double param_1)

{
  uint uVar1;
  undefined4 uVar2;
  bool in_PF;
  
  uVar1 = __fpclass(param_1);
  if (((uVar1 & 0x90) != 0) || (__frnd(param_1), in_PF)) {
    uVar2 = 0;
  }
  else {
    __frnd(param_1 * 0.50000000);
    if (in_PF) {
      uVar2 = 1;
    }
    else {
      uVar2 = 2;
    }
  }
  return uVar2;
}



// Library Function - Single Match
// Name: __powhlp
// Library: Visual Studio 2015 Release

undefined4 __powhlp(int param_1,int iParm2,int param_2,int iParm4,undefined8 *param_3)

{
  double dVar1;
  undefined8 uVar2;
  int iVar3;
  bool in_PF;
  
  dVar1 = ABS((double)CONCAT44(iParm2,param_1));
  if (iParm4 == 0x7ff00000) {
    if (param_2 != 0) goto LAB_0041562f;
    uVar2 = 0x3ff0000000000000;
    if (in_PF) {
      if ((ushort)((ushort)(1.00000000 < dVar1) << 8 | (ushort)(dVar1 == 1.00000000) << 0xe) == 0) {
        uVar2 = 0;
      }
      goto LAB_004156c0;
    }
  }
  else {
    if ((iParm4 == -0x100000) && (param_2 == 0)) {
      if (in_PF) {
        uVar2 = 0x3ff0000000000000;
        if ((ushort)((ushort)(1.00000000 < dVar1) << 8 | (ushort)(dVar1 == 1.00000000) << 0xe) == 0)
        {
          uVar2 = 0x7ff0000000000000;
        }
      }
      else {
        uVar2 = 0;
      }
      goto LAB_004156c0;
    }
LAB_0041562f:
    if (iParm2 != 0x7ff00000) {
      if (iParm2 != -0x100000) {
        return 0;
      }
      if (param_1 != 0) {
        return 0;
      }
      iVar3 = __d_inttype(CONCAT44(iParm4,param_2));
      uVar2 = 0;
      if ((ushort)((ushort)((double)CONCAT44(iParm4,param_2) < 0.00000000) << 8 |
                  (ushort)((double)CONCAT44(iParm4,param_2) == 0.00000000) << 0xe) == 0) {
        uVar2 = 0x7ff0000000000000;
        if (iVar3 == 1) {
          uVar2 = 0xfff0000000000000;
        }
      }
      else {
        if (in_PF) {
          uVar2 = 0x3ff0000000000000;
        }
        else {
          if (iVar3 == 1) {
            uVar2 = 0x8000000000000000;
          }
        }
      }
      goto LAB_004156c0;
    }
    if (param_1 != 0) {
      return 0;
    }
    if ((ushort)((ushort)((double)CONCAT44(iParm4,param_2) < 0.00000000) << 8 |
                (ushort)((double)CONCAT44(iParm4,param_2) == 0.00000000) << 0xe) != 0) {
      uVar2 = 0;
      if (in_PF) {
        uVar2 = 0x3ff0000000000000;
      }
      goto LAB_004156c0;
    }
  }
  uVar2 = 0x7ff0000000000000;
LAB_004156c0:
  *param_3 = uVar2;
  return 0;
}



void __cdecl FUN_004156c6(uint param_1,int *param_2,ushort *param_3)

{
  int iVar1;
  uint uVar2;
  uint *puVar3;
  uint local_94;
  undefined1 local_90 [48];
  undefined8 local_60;
  uint local_50;
  uint local_14;
  
  local_14 = DAT_0041e004 ^ (uint)&stack0xfffffff0;
  local_94 = (uint)*param_3;
  iVar1 = *param_2;
  if (iVar1 == 1) {
LAB_0041572e:
    puVar3 = (uint *)&DAT_00000008;
LAB_00415730:
    iVar1 = FUN_004158e8(puVar3,param_2 + 6,local_94);
    if (iVar1 == 0) {
      if (((param_1 == 0x10) || (param_1 == 0x16)) || (param_1 == 0x1d)) {
        local_60 = *(undefined8 *)(param_2 + 4);
        local_50 = local_50 & 0xffffffe3 | 3;
      }
      else {
        local_50 = local_50 & 0xfffffffe;
      }
      FUN_00415ad5((undefined1 (*) [10])local_90,&local_94,puVar3,param_1,(int)(param_2 + 2),
                   (undefined8 *)(param_2 + 6));
    }
  }
  else {
    if (iVar1 == 2) {
      puVar3 = (uint *)&DAT_00000004;
      goto LAB_00415730;
    }
    if (iVar1 == 3) {
      puVar3 = (uint *)0x11;
      goto LAB_00415730;
    }
    if (iVar1 == 4) {
      puVar3 = (uint *)0x12;
      goto LAB_00415730;
    }
    if (iVar1 == 5) goto LAB_0041572e;
    if (iVar1 == 8) {
      puVar3 = (uint *)&DAT_00000010;
      goto LAB_00415730;
    }
  }
  __ctrlfp(local_94,0xffff);
  if (*param_2 != 8) {
    uVar2 = FUN_00408e96();
    if ((char)uVar2 != 0) {
      iVar1 = FUN_00408ebd(param_2);
      if (iVar1 != 0) goto LAB_004157bd;
    }
  }
  __set_errno_from_matherr(*param_2);
LAB_004157bd:
  FUN_00401f78();
  return;
}



// Library Function - Single Match
// Name: __frnd
// Library: Visual Studio 2015 Release

float10 __cdecl __frnd(double param_1)

{
  return (float10)ROUND(param_1);
}



// Library Function - Single Match
// Name: __errcode
// Library: Visual Studio 2015 Release

int __cdecl __errcode(uint param_1)

{
  int iStack8;
  
  if ((param_1 & 0x20) == 0) {
    if ((param_1 & 8) != 0) {
      return 1;
    }
    if ((param_1 & 4) == 0) {
      if ((param_1 & 1) == 0) {
        return (param_1 & 2) * 2;
      }
      iStack8 = 3;
    }
    else {
      iStack8 = 2;
    }
  }
  else {
    iStack8 = 5;
  }
  return iStack8;
}



// Library Function - Single Match
// Name: __except1
// Library: Visual Studio 2015 Release

void __cdecl
__except1(uint *param_1,uint param_2,undefined8 param_3,undefined8 param_4,uint param_5)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  int unaff_EDI;
  undefined1 local_90 [64];
  uint local_50;
  uint local_14;
  
  uVar3 = param_5;
  local_14 = DAT_0041e004 ^ (uint)&stack0xfffffff0;
  iVar1 = FUN_004158e8(param_1,&param_4,param_5);
  if (iVar1 == 0) {
    local_50 = local_50 & 0xfffffffe;
    FUN_00415af8((undefined1 (*) [10])local_90,&param_5,param_1,param_2,(int)&param_3,&param_4,
                 (undefined8 *)0x0,unaff_EDI);
    uVar3 = param_5;
  }
  iVar1 = __errcode((uint)param_1);
  uVar2 = FUN_00408e96();
  if (((char)uVar2 == 0) || (iVar1 == 0)) {
    __set_errno_from_matherr(iVar1);
    __ctrlfp();
  }
  else {
    __umatherr(iVar1,param_2,(int)param_3,(int)((ulonglong)param_3 >> 0x20),0,0,(int)param_4,
               (int)((ulonglong)param_4 >> 0x20),uVar3);
  }
  FUN_00401f78();
  return;
}



uint FUN_004158e8(uint param_1,double *param_2,uint param_3)

{
  double dVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  bool in_PF;
  bool bVar6;
  float10 fVar7;
  float10 fVar8;
  uint local_14;
  byte bStack16;
  undefined uStack15;
  ushort uStack14;
  uint local_8;
  
  uVar5 = param_1 & 0x1f;
  if (((param_1 & 8) != 0) && ((param_3 & 1) != 0)) {
    __set_statfp(1);
    uVar5 = param_1 & 0x17;
    goto LAB_00415ab2;
  }
  if ((param_1 & param_3 & 4) != 0) {
    __set_statfp(4);
    uVar5 = param_1 & 0x1b;
    goto LAB_00415ab2;
  }
  if (((param_1 & 1) == 0) || ((param_3 & 8) == 0)) {
    if (((param_1 & 2) != 0) && ((param_3 & 0x10) != 0)) {
      uVar5 = param_1 >> 4 & 1;
      if (in_PF) {
        fVar7 = (float10)FUN_00415ebf(*param_2,&local_8);
        iVar2 = local_8 - 0x600;
        dVar1 = (double)fVar7;
        local_14 = SUB84(dVar1,0);
        bStack16 = (byte)((ulonglong)dVar1 >> 0x20);
        uStack15 = (undefined)((ulonglong)dVar1 >> 0x28);
        uStack14 = (ushort)((ulonglong)dVar1 >> 0x30);
        fVar8 = (float10)0;
        if (iVar2 < -0x432) {
          fVar8 = fVar8 * fVar7;
          uVar5 = 1;
        }
        else {
          bVar6 = (ushort)((ushort)(fVar8 < fVar7) << 8 | (ushort)(fVar8 == fVar7) << 0xe) == 0;
          local_8 = (uint)bVar6;
          uStack14 = uStack14 & 0xf | 0x10;
          if (iVar2 < -0x3fd) {
            iVar2 = -0x3fd - iVar2;
            uVar3 = (uint)((ulonglong)dVar1 >> 0x20) & 0xfffff | 0x100000;
            do {
              if (((local_14 & 1) != 0) && (uVar5 == 0)) {
                uVar5 = 1;
              }
              local_14 = local_14 >> 1;
              if ((bStack16 & 1) != 0) {
                local_14 = local_14 | 0x80000000;
              }
              uVar4 = uVar3 >> 1;
              bStack16 = (byte)uVar4;
              uStack15 = (undefined)(uVar4 >> 8);
              uStack14 = (ushort)(uVar3 >> 0x11);
              iVar2 = iVar2 + -1;
              uVar3 = uVar4;
            } while (iVar2 != 0);
          }
          fVar8 = (float10)(double)CONCAT26(uStack14,CONCAT15(uStack15,CONCAT14(bStack16,local_14)))
          ;
          if (bVar6) {
            fVar8 = -fVar8;
          }
        }
        *param_2 = (double)fVar8;
      }
      else {
        uVar5 = 1;
      }
      if (uVar5 != 0) {
        __set_statfp(0x10);
      }
      uVar5 = param_1 & 0x1d;
    }
    goto LAB_00415ab2;
  }
  __set_statfp(8);
  uVar5 = param_3 & 0xc00;
  if (uVar5 == 0) {
    if (in_PF) {
LAB_004159bf:
      dVar1 = INFINITY;
      goto LAB_004159c5;
    }
LAB_004159b7:
    dVar1 = INFINITY;
LAB_004159c7:
    *param_2 = dVar1;
  }
  else {
    if (uVar5 == 0x400) {
      if (in_PF) goto LAB_004159bf;
      dVar1 = 
      179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.00000000
      ;
      goto LAB_004159c7;
    }
    if (uVar5 == 0x800) {
      if (!in_PF) goto LAB_004159b7;
      dVar1 = 
      179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.00000000
      ;
LAB_004159c5:
      dVar1 = -dVar1;
      goto LAB_004159c7;
    }
    if (uVar5 == 0xc00) {
      dVar1 = 
      179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.00000000
      ;
      if (!in_PF) goto LAB_004159c7;
      goto LAB_004159c5;
    }
  }
  uVar5 = param_1 & 0x1e;
LAB_00415ab2:
  if (((param_1 & 0x10) != 0) && ((param_3 & 0x20) != 0)) {
    __set_statfp(0x20);
    uVar5 = uVar5 & 0xffffffef;
  }
  return (uint)(uVar5 == 0);
}



void __cdecl
FUN_00415ad5(undefined1 (*param_1) [10],uint *param_2,uint *param_3,uint param_4,int param_5,
            undefined8 *param_6)

{
  int unaff_EBP;
  
  FUN_00415af8(param_1,param_2,param_3,param_4,param_5,param_6,(undefined8 *)0x0,unaff_EBP);
  return;
}



undefined1 (*) [10] __cdecl
FUN_00415af8(undefined1 (*__return_storage_ptr__) [10],uint *param_1,uint *param_2,uint param_3,
            int param_4,undefined8 *param_5,undefined8 *param_6,int param_7)

{
  uint uVar1;
  undefined1 (*pauVar2) [10];
  DWORD dwExceptionCode;
  
  *(undefined4 *)(*__return_storage_ptr__ + 4) = 0;
  dwExceptionCode = 0xc000000d;
  *(undefined4 *)(*__return_storage_ptr__ + 8) = 0;
  *(undefined4 *)(__return_storage_ptr__ + 1 + 2) = 0;
  if (((uint)param_2 & 0x10) != 0) {
    dwExceptionCode = 0xc000008f;
    *(uint *)(*__return_storage_ptr__ + 4) = *(uint *)(*__return_storage_ptr__ + 4) | 1;
  }
  if (((uint)param_2 & 2) != 0) {
    dwExceptionCode = 0xc0000093;
    *(uint *)(*__return_storage_ptr__ + 4) = *(uint *)(*__return_storage_ptr__ + 4) | 2;
  }
  if (((uint)param_2 & 1) != 0) {
    dwExceptionCode = 0xc0000091;
    *(uint *)(*__return_storage_ptr__ + 4) = *(uint *)(*__return_storage_ptr__ + 4) | 4;
  }
  if (((uint)param_2 & 4) != 0) {
    dwExceptionCode = 0xc000008e;
    *(uint *)(*__return_storage_ptr__ + 4) = *(uint *)(*__return_storage_ptr__ + 4) | 8;
  }
  if (((uint)param_2 & 8) != 0) {
    dwExceptionCode = 0xc0000090;
    *(uint *)(*__return_storage_ptr__ + 4) = *(uint *)(*__return_storage_ptr__ + 4) | 0x10;
  }
  *(uint *)(*__return_storage_ptr__ + 8) =
       *(uint *)(*__return_storage_ptr__ + 8) ^
       (~(*param_1 << 4) ^ *(uint *)(*__return_storage_ptr__ + 8)) & 0x10;
  *(uint *)(*__return_storage_ptr__ + 8) =
       *(uint *)(*__return_storage_ptr__ + 8) ^
       (~(*param_1 * 2) ^ *(uint *)(*__return_storage_ptr__ + 8)) & 8;
  *(uint *)(*__return_storage_ptr__ + 8) =
       *(uint *)(*__return_storage_ptr__ + 8) ^
       (~(*param_1 >> 1) ^ *(uint *)(*__return_storage_ptr__ + 8)) & 4;
  *(uint *)(*__return_storage_ptr__ + 8) =
       *(uint *)(*__return_storage_ptr__ + 8) ^
       (~(*param_1 >> 3) ^ *(uint *)(*__return_storage_ptr__ + 8)) & 2;
  *(uint *)(*__return_storage_ptr__ + 8) =
       *(uint *)(*__return_storage_ptr__ + 8) ^
       (~(*param_1 >> 5) ^ *(uint *)(*__return_storage_ptr__ + 8)) & 1;
  uVar1 = FUN_00413932();
  if ((uVar1 & 1) != 0) {
    *(uint *)(__return_storage_ptr__ + 1 + 2) = *(uint *)(__return_storage_ptr__ + 1 + 2) | 0x10;
  }
  if ((uVar1 & 4) != 0) {
    *(uint *)(__return_storage_ptr__ + 1 + 2) = *(uint *)(__return_storage_ptr__ + 1 + 2) | 8;
  }
  if ((uVar1 & 8) != 0) {
    *(uint *)(__return_storage_ptr__ + 1 + 2) = *(uint *)(__return_storage_ptr__ + 1 + 2) | 4;
  }
  if ((uVar1 & 0x10) != 0) {
    *(uint *)(__return_storage_ptr__ + 1 + 2) = *(uint *)(__return_storage_ptr__ + 1 + 2) | 2;
  }
  if ((uVar1 & 0x20) != 0) {
    *(uint *)(__return_storage_ptr__ + 1 + 2) = *(uint *)(__return_storage_ptr__ + 1 + 2) | 1;
  }
  uVar1 = *param_1 & 0xc00;
  if (uVar1 == 0) {
    *(uint *)*__return_storage_ptr__ = *(uint *)*__return_storage_ptr__ & 0xfffffffc;
  }
  else {
    if (uVar1 == 0x400) {
      uVar1 = *(uint *)*__return_storage_ptr__ & 0xfffffffd | 1;
    }
    else {
      if (uVar1 != 0x800) {
        if (uVar1 == 0xc00) {
          *(uint *)*__return_storage_ptr__ = *(uint *)*__return_storage_ptr__ | 3;
        }
        goto LAB_00415c5a;
      }
      uVar1 = *(uint *)*__return_storage_ptr__ & 0xfffffffe | 2;
    }
    *(uint *)*__return_storage_ptr__ = uVar1;
  }
LAB_00415c5a:
  uVar1 = *param_1 & 0x300;
  if (uVar1 == 0) {
    uVar1 = *(uint *)*__return_storage_ptr__ & 0xffffffeb | 8;
LAB_00415c90:
    *(uint *)*__return_storage_ptr__ = uVar1;
  }
  else {
    if (uVar1 == 0x200) {
      uVar1 = *(uint *)*__return_storage_ptr__ & 0xffffffe7 | 4;
      goto LAB_00415c90;
    }
    if (uVar1 == 0x300) {
      *(uint *)*__return_storage_ptr__ = *(uint *)*__return_storage_ptr__ & 0xffffffe3;
    }
  }
  *(uint *)*__return_storage_ptr__ =
       *(uint *)*__return_storage_ptr__ ^
       (param_3 << 5 ^ *(uint *)*__return_storage_ptr__) & 0x1ffe0;
  *(uint *)(__return_storage_ptr__ + 3 + 2) = *(uint *)(__return_storage_ptr__ + 3 + 2) | 1;
  if (param_6 == (undefined8 *)0x0) {
    *(uint *)(__return_storage_ptr__ + 3 + 2) =
         *(uint *)(__return_storage_ptr__ + 3 + 2) & 0xffffffe3 | 2;
    *(undefined8 *)(__return_storage_ptr__ + 1 + 6) = *(undefined8 *)param_4;
    *(uint *)(__return_storage_ptr__ + 9 + 6) = *(uint *)(__return_storage_ptr__ + 9 + 6) | 1;
    *(uint *)(__return_storage_ptr__ + 9 + 6) =
         *(uint *)(__return_storage_ptr__ + 9 + 6) & 0xffffffe3 | 2;
    *(undefined8 *)(__return_storage_ptr__ + 8) = *param_5;
  }
  else {
    *(uint *)(__return_storage_ptr__ + 3 + 2) =
         *(uint *)(__return_storage_ptr__ + 3 + 2) & 0xffffffe1;
    *(undefined4 *)(__return_storage_ptr__ + 1 + 6) = *(undefined4 *)param_4;
    *(uint *)(__return_storage_ptr__ + 9 + 6) = *(uint *)(__return_storage_ptr__ + 9 + 6) | 1;
    *(uint *)(__return_storage_ptr__ + 9 + 6) =
         *(uint *)(__return_storage_ptr__ + 9 + 6) & 0xffffffe1;
    *(undefined4 *)(__return_storage_ptr__ + 8) = *(undefined4 *)param_5;
  }
  FUN_00413898();
  RaiseException(dwExceptionCode,0,1,(ULONG_PTR *)&__return_storage_ptr__);
  uVar1 = *(uint *)(*__return_storage_ptr__ + 8);
  if ((uVar1 & 0x10) != 0) {
    *param_1 = *param_1 & 0xfffffffe;
    uVar1 = *(uint *)(*__return_storage_ptr__ + 8);
  }
  if ((uVar1 & 8) != 0) {
    *param_1 = *param_1 & 0xfffffffb;
    uVar1 = *(uint *)(*__return_storage_ptr__ + 8);
  }
  if ((uVar1 & 4) != 0) {
    *param_1 = *param_1 & 0xfffffff7;
    uVar1 = *(uint *)(*__return_storage_ptr__ + 8);
  }
  if ((uVar1 & 2) != 0) {
    *param_1 = *param_1 & 0xffffffef;
    uVar1 = *(uint *)(*__return_storage_ptr__ + 8);
  }
  if ((uVar1 & 1) != 0) {
    *param_1 = *param_1 & 0xffffffdf;
  }
  uVar1 = *(uint *)*__return_storage_ptr__ & 3;
  if (uVar1 == 0) {
    *param_1 = *param_1 & 0xfffff3ff;
  }
  else {
    if (uVar1 == 1) {
      uVar1 = *param_1 & 0xfffff7ff | 0x400;
    }
    else {
      if (uVar1 != 2) {
        if (uVar1 == 3) {
          *param_1 = *param_1 | 0xc00;
        }
        goto LAB_00415da6;
      }
      uVar1 = *param_1 & 0xfffffbff | 0x800;
    }
    *param_1 = uVar1;
  }
LAB_00415da6:
  uVar1 = *(uint *)*__return_storage_ptr__ >> 2 & 7;
  if (uVar1 == 0) {
    pauVar2 = (undefined1 (*) [10])(*param_1 & 0xfffff3ff | 0x300);
  }
  else {
    if (uVar1 != 1) {
      pauVar2 = (undefined1 (*) [10])(uVar1 - 2);
      if (pauVar2 == (undefined1 (*) [10])0x0) {
        *param_1 = *param_1 & 0xfffff3ff;
      }
      goto LAB_00415dd7;
    }
    pauVar2 = (undefined1 (*) [10])(*param_1 & 0xfffff3ff | 0x200);
  }
  *(undefined1 (**) [10])param_1 = pauVar2;
LAB_00415dd7:
  if (param_6 == (undefined8 *)0x0) {
    *param_5 = *(undefined8 *)(__return_storage_ptr__ + 8);
  }
  else {
    *(undefined4 *)param_5 = *(undefined4 *)(__return_storage_ptr__ + 8);
  }
  return pauVar2;
}



// Library Function - Single Match
// Name: __set_errno_from_matherr
// Library: Visual Studio 2015 Release

void __cdecl __set_errno_from_matherr(int param_1)

{
  undefined4 *puVar1;
  
  if (param_1 == 1) {
    puVar1 = (undefined4 *)FUN_00407f40();
    *puVar1 = 0x21;
  }
  else {
    if (param_1 - 2U < 2) {
      puVar1 = (undefined4 *)FUN_00407f40();
      *puVar1 = 0x22;
      return;
    }
  }
  return;
}



// Library Function - Single Match
// Name: __umatherr
// Library: Visual Studio 2015 Release

float10 __cdecl
__umatherr(int param_1,int param_2,undefined4 param_3,undefined4 param_4,undefined4 param_5,
          undefined4 param_6,undefined4 param_7,undefined4 param_8,undefined4 param_9)

{
  double dVar1;
  int iVar2;
  int local_24;
  undefined *local_20;
  undefined4 local_1c;
  undefined4 local_18;
  undefined4 local_14;
  undefined4 local_10;
  undefined4 local_c;
  undefined4 uStack8;
  
  local_20 = (undefined *)0x0;
  iVar2 = 0;
  do {
    if ((&DAT_0041c758)[iVar2 * 2] == param_2) {
      local_20 = (&PTR_DAT_0041c75c)[iVar2 * 2];
      break;
    }
    iVar2 = iVar2 + 1;
  } while (iVar2 < 0x1d);
  if (local_20 == (undefined *)0x0) {
    __ctrlfp(param_9,0xffff);
    __set_errno_from_matherr(param_1);
    dVar1 = (double)CONCAT44(param_8,param_7);
  }
  else {
    local_1c = param_3;
    local_18 = param_4;
    local_14 = param_5;
    local_10 = param_6;
    local_c = param_7;
    local_24 = param_1;
    uStack8 = param_8;
    __ctrlfp(param_9,0xffff);
    iVar2 = FUN_00408ebd(&local_24);
    if (iVar2 == 0) {
      __set_errno_from_matherr(param_1);
    }
    dVar1 = (double)CONCAT44(uStack8,local_c);
  }
  return (float10)dVar1;
}



float10 FUN_00415ebf(int param_1,uint uParm2,int *param_2)

{
  uint uVar1;
  double dVar2;
  int iVar3;
  ushort uVar4;
  bool in_PF;
  float10 fVar5;
  float10 fVar6;
  
  dVar2 = (double)CONCAT17(uParm2._3_1_,CONCAT16(uParm2._2_1_,CONCAT24((undefined2)uParm2,param_1)))
  ;
  fVar5 = (float10)dVar2;
  fVar6 = (float10)0;
  if (in_PF) {
    uVar4 = (ushort)(uParm2 >> 0x10);
    if (((uParm2 & 0x7ff00000) == 0) && (((uParm2 & 0xfffff) != 0 || (param_1 != 0)))) {
      iVar3 = -0x3fd;
      if ((uParm2 & 0x100000) == 0) {
        do {
          uVar1 = uParm2 * 2;
          uParm2._0_2_ = (undefined2)uVar1;
          uParm2 = uVar1;
          if (param_1 < 0) {
            uParm2 = uVar1 | 1;
          }
          param_1 = param_1 * 2;
          iVar3 = iVar3 + -1;
        } while ((uVar1 >> 0x10 & 0x10) == 0);
        uVar4 = (ushort)(uVar1 >> 0x10);
      }
      uParm2._2_1_ = (undefined)(uVar4 & 0xffef);
      uParm2._3_1_ = (byte)((uVar4 & 0xffef) >> 8);
      if ((ushort)((ushort)(fVar6 < fVar5) << 8 | (ushort)(fVar6 == fVar5) << 0xe) == 0) {
        uParm2._3_1_ = uParm2._3_1_ | 0x80;
      }
      fVar6 = __set_exp(CONCAT17(uParm2._3_1_,
                                 CONCAT16(uParm2._2_1_,CONCAT24((undefined2)uParm2,param_1))),0);
    }
    else {
      fVar6 = __set_exp((ulonglong)dVar2,0);
      iVar3 = ((uint)(uVar4 >> 4) & 0x7ff) - 0x3fe;
    }
  }
  else {
    iVar3 = 0;
  }
  *param_2 = iVar3;
  return fVar6;
}



// Library Function - Single Match
// Name: __set_exp
// Library: Visual Studio 2015 Release

float10 __cdecl __set_exp(ulonglong param_1,short param_2)

{
  double local_c;
  
  local_c = (double)(param_1 & 0xffffffffffff |
                    (ulonglong)(ushort)((param_2 + 0x3fe) * 0x10 | param_1._6_2_ & 0x800f) << 0x30);
  return (float10)local_c;
}



// Library Function - Single Match
// Name: __sptype
// Library: Visual Studio 2015 Release

undefined4 __cdecl __sptype(int param_1,uint param_2)

{
  undefined4 uStack8;
  
  if (param_2 == 0x7ff00000) {
    if (param_1 == 0) {
      return 1;
    }
  }
  else {
    if ((param_2 == 0xfff00000) && (param_1 == 0)) {
      return 2;
    }
  }
  if ((param_2._2_2_ & 0x7ff8) == 0x7ff8) {
    uStack8 = 3;
  }
  else {
    if (((param_2._2_2_ & 0x7ff8) != 0x7ff0) || (((param_2 & 0x7ffff) == 0 && (param_1 == 0)))) {
      return 0;
    }
    uStack8 = 4;
  }
  return uStack8;
}



// Library Function - Single Match
// Name: __fpclass
// Library: Visual Studio 2015 Release

int __cdecl __fpclass(double _X)

{
  int iVar1;
  bool in_PF;
  
  if ((_X._6_2_ & 0x7ff0) == 0x7ff0) {
    iVar1 = __sptype(_X._0_4_,(uint)((ulonglong)_X >> 0x20));
    if (iVar1 == 1) {
      return 0x200;
    }
    if (iVar1 == 2) {
      iVar1 = 4;
    }
    else {
      if (iVar1 != 3) {
        return 1;
      }
      iVar1 = 2;
    }
    return iVar1;
  }
  if ((((ulonglong)_X & 0x7ff0000000000000) == 0) &&
     ((((ulonglong)_X & 0xfffff00000000) != 0 || (_X._0_4_ != 0)))) {
    return (-(uint)(((ulonglong)_X & 0x8000000000000000) != 0) & 0xffffff90) + 0x80;
  }
  if (!in_PF) {
    return (-(uint)(((ulonglong)_X & 0x8000000000000000) != 0) & 0xffffffe0) + 0x40;
  }
  return (-(uint)(((ulonglong)_X & 0x8000000000000000) != 0) & 0xffffff08) + 0x100;
}



BOOL IsProcessorFeaturePresent(DWORD ProcessorFeature)

{
  BOOL BVar1;
  
                    // WARNING: Could not recover jumptable at 0x004160ce. Too many branches
                    // WARNING: Treating indirect jump as call
  BVar1 = IsProcessorFeaturePresent(ProcessorFeature);
  return BVar1;
}



// Library Function - Single Match
// Name: __FindPESection
// Library: Visual Studio 2015 Release

PIMAGE_SECTION_HEADER __cdecl __FindPESection(PBYTE pImageBase,DWORD_PTR rva)

{
  PIMAGE_SECTION_HEADER p_Var1;
  uint uVar2;
  
  uVar2 = 0;
  pImageBase = pImageBase + *(int *)(pImageBase + 0x3c);
  p_Var1 = (PIMAGE_SECTION_HEADER)(pImageBase + (uint)*(ushort *)(pImageBase + 0x14) + 0x18);
  if (*(ushort *)(pImageBase + 6) != 0) {
    do {
      if ((p_Var1->VirtualAddress <= rva) && (rva < p_Var1->Misc + p_Var1->VirtualAddress)) {
        return p_Var1;
      }
      uVar2 = uVar2 + 1;
      p_Var1 = p_Var1 + 1;
    } while (uVar2 < (uint)*(ushort *)(pImageBase + 6));
  }
  return (PIMAGE_SECTION_HEADER)0x0;
}



// Library Function - Single Match
// Name: __IsNonwritableInCurrentImage
// Library: Visual Studio 2015 Release

BOOL __cdecl __IsNonwritableInCurrentImage(PBYTE pTarget)

{
  uint uVar1;
  int iVar2;
  PIMAGE_SECTION_HEADER p_Var3;
  int **in_FS_OFFSET;
  uint uStack44;
  uint *local_1c;
  int *local_14;
  code *pcStack16;
  uint local_c;
  undefined4 local_8;
  
  pcStack16 = FUN_00402c70;
  local_14 = *in_FS_OFFSET;
  local_c = DAT_0041e004 ^ 0x41d480;
  uStack44 = DAT_0041e004 ^ (uint)&stack0xfffffffc;
  local_1c = &uStack44;
  *(int ***)in_FS_OFFSET = &local_14;
  local_8 = 0;
  iVar2 = FUN_004161f0((short *)&IMAGE_DOS_HEADER_00400000);
  if (iVar2 != 0) {
    p_Var3 = __FindPESection((PBYTE)&IMAGE_DOS_HEADER_00400000,(DWORD_PTR)(pTarget + -0x400000));
    if (p_Var3 != (PIMAGE_SECTION_HEADER)0x0) {
      uVar1 = p_Var3->Characteristics;
      *in_FS_OFFSET = local_14;
      return ~(uVar1 >> 0x1f) & 1;
    }
  }
  *in_FS_OFFSET = local_14;
  return 0;
}



undefined4 __cdecl FUN_004161f0(short *param_1)

{
  if (((*param_1 == 0x5a4d) && (*(int *)(*(int *)(param_1 + 0x1e) + (int)param_1) == 0x4550)) &&
     (*(short *)((int *)(*(int *)(param_1 + 0x1e) + (int)param_1) + 6) == 0x10b)) {
    return 1;
  }
  return 0;
}



void FUN_00416238(void)

{
  undefined4 *unaff_EBP;
  undefined4 *in_FS_OFFSET;
  undefined4 unaff_retaddr;
  
  FUN_00401f78();
  *in_FS_OFFSET = unaff_EBP[-3];
  *unaff_EBP = unaff_retaddr;
  return;
}



// WARNING: This is an inlined function
// WARNING: Unable to track spacebase fully for stack
// WARNING: Variable defined which should be unmapped: param_1
// Library Function - Single Match
// Name: __EH_prolog3_GS
// Libraries: Visual Studio 2015 Debug, Visual Studio 2015 Release

void __cdecl __EH_prolog3_GS(int param_1)

{
  undefined4 unaff_EBX;
  undefined4 unaff_ESI;
  undefined4 unaff_EDI;
  int *in_FS_OFFSET;
  undefined4 local_res0;
  uint auStack28 [5];
  undefined local_8 [8];
  
  param_1 = -param_1;
  *(undefined4 *)((int)auStack28 + param_1 + 0x10) = unaff_EBX;
  *(undefined4 *)((int)auStack28 + param_1 + 0xc) = unaff_ESI;
  *(undefined4 *)((int)auStack28 + param_1 + 8) = unaff_EDI;
  *(uint *)((int)auStack28 + param_1 + 4) = DAT_0041e004 ^ (uint)&param_1;
  *(uint *)((int)auStack28 + param_1) = local_res0;
  *(undefined **)in_FS_OFFSET = local_8;
  return;
}



// WARNING: This is an inlined function
// WARNING: Unable to track spacebase fully for stack
// WARNING: Variable defined which should be unmapped: param_1
// Library Function - Single Match
// Name: __EH_prolog3_catch
// Libraries: Visual Studio 2015 Debug, Visual Studio 2015 Release

void __cdecl __EH_prolog3_catch(int param_1)

{
  undefined4 unaff_EBX;
  undefined4 unaff_ESI;
  undefined4 unaff_EDI;
  int *in_FS_OFFSET;
  undefined4 local_res0;
  uint auStack28 [5];
  undefined local_8 [8];
  
  param_1 = -param_1;
  *(undefined4 *)((int)auStack28 + param_1 + 0x10) = unaff_EBX;
  *(undefined4 *)((int)auStack28 + param_1 + 0xc) = unaff_ESI;
  *(undefined4 *)((int)auStack28 + param_1 + 8) = unaff_EDI;
  *(uint *)((int)auStack28 + param_1 + 4) = DAT_0041e004 ^ (uint)&param_1;
  *(uint *)((int)auStack28 + param_1) = local_res0;
  *(undefined **)in_FS_OFFSET = local_8;
  return;
}



// Library Function - Single Match
// Name: __aulldvrm
// Library: Visual Studio

undefined8 __aulldvrm(uint param_1,uint param_2,uint param_3,uint param_4)

{
  ulonglong uVar1;
  longlong lVar2;
  uint uVar3;
  int iVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  uint uVar9;
  
  uVar3 = param_1;
  uVar8 = param_4;
  uVar6 = param_2;
  uVar9 = param_3;
  if (param_4 == 0) {
    uVar3 = param_2 / param_3;
    iVar4 = (int)(((ulonglong)param_2 % (ulonglong)param_3 << 0x20 | (ulonglong)param_1) /
                 (ulonglong)param_3);
  }
  else {
    do {
      uVar5 = uVar8 >> 1;
      uVar9 = uVar9 >> 1 | (uint)((uVar8 & 1) != 0) << 0x1f;
      uVar7 = uVar6 >> 1;
      uVar3 = uVar3 >> 1 | (uint)((uVar6 & 1) != 0) << 0x1f;
      uVar8 = uVar5;
      uVar6 = uVar7;
    } while (uVar5 != 0);
    uVar1 = CONCAT44(uVar7,uVar3) / (ulonglong)uVar9;
    iVar4 = (int)uVar1;
    lVar2 = (ulonglong)param_3 * (uVar1 & 0xffffffff);
    uVar3 = (uint)((ulonglong)lVar2 >> 0x20);
    uVar8 = uVar3 + iVar4 * param_4;
    if (((CARRY4(uVar3,iVar4 * param_4)) || (param_2 < uVar8)) ||
       ((param_2 <= uVar8 && (param_1 < (uint)lVar2)))) {
      iVar4 = iVar4 + -1;
    }
    uVar3 = 0;
  }
  return CONCAT44(uVar3,iVar4);
}



// Library Function - Multiple Matches With Same Base Name
// Name: `scalar_deleting_destructor'
// Library: Visual Studio 2015 Release

undefined4 * __thiscall _scalar_deleting_destructor_(void *this,byte param_1)

{
  *(undefined4 *)this = 0x41c8fc;
  if ((param_1 & 1) != 0) {
    FUN_00416378(this);
  }
  return (undefined4 *)this;
}



void __cdecl FUN_00416378(void *param_1)

{
  free(param_1);
  return;
}



// Library Function - Single Match
// Name: __allmul
// Library: Visual Studio

ulonglong __allmul(uint param_1,uint param_2,uint param_3,uint param_4)

{
  if ((param_4 | param_2) == 0) {
    return (ulonglong)param_1 * (ulonglong)param_3;
  }
  return (ulonglong)param_1 * (ulonglong)param_3 & 0xffffffff |
         (ulonglong)
         ((int)((ulonglong)param_1 * (ulonglong)param_3 >> 0x20) +
         param_2 * param_3 + param_1 * param_4) << 0x20;
}



// Library Function - Single Match
// Name: __alldvrm
// Library: Visual Studio

undefined8 __alldvrm(uint param_1,uint param_2,uint param_3,uint param_4)

{
  ulonglong uVar1;
  longlong lVar2;
  uint uVar3;
  int iVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  bool bVar9;
  bool bVar10;
  
  bVar10 = (int)param_2 < 0;
  if (bVar10) {
    bVar9 = param_1 != 0;
    param_1 = -param_1;
    param_2 = -(uint)bVar9 - param_2;
  }
  if ((int)param_4 < 0) {
    bVar10 = (bool)(bVar10 + 1);
    bVar9 = param_3 != 0;
    param_3 = -param_3;
    param_4 = -(uint)bVar9 - param_4;
  }
  uVar3 = param_1;
  uVar5 = param_3;
  uVar6 = param_2;
  uVar8 = param_4;
  if (param_4 == 0) {
    uVar3 = param_2 / param_3;
    iVar4 = (int)(((ulonglong)param_2 % (ulonglong)param_3 << 0x20 | (ulonglong)param_1) /
                 (ulonglong)param_3);
  }
  else {
    do {
      uVar8 = uVar8 >> 1;
      uVar5 = uVar5 >> 1 | (uint)((uVar8 & 1) != 0) << 0x1f;
      uVar7 = uVar6 >> 1;
      uVar3 = uVar3 >> 1 | (uint)((uVar6 & 1) != 0) << 0x1f;
      uVar6 = uVar7;
      uVar8 = uVar8;
    } while (uVar8 != 0);
    uVar1 = CONCAT44(uVar7,uVar3) / (ulonglong)uVar5;
    iVar4 = (int)uVar1;
    lVar2 = (ulonglong)param_3 * (uVar1 & 0xffffffff);
    uVar3 = (uint)((ulonglong)lVar2 >> 0x20);
    uVar5 = uVar3 + iVar4 * param_4;
    if (((CARRY4(uVar3,iVar4 * param_4)) || (param_2 < uVar5)) ||
       ((param_2 <= uVar5 && (param_1 < (uint)lVar2)))) {
      iVar4 = iVar4 + -1;
    }
    uVar3 = 0;
  }
  if (bVar10 == true) {
    bVar10 = iVar4 != 0;
    iVar4 = -iVar4;
    uVar3 = -(uint)bVar10 - uVar3;
  }
  return CONCAT44(uVar3,iVar4);
}



// Library Function - Single Match
// Name: __aullshr
// Library: Visual Studio

ulonglong __fastcall __aullshr(byte bParm1,uint uParm2)

{
  uint in_EAX;
  
  if (0x3f < bParm1) {
    return 0;
  }
  if (bParm1 < 0x20) {
    return CONCAT44(uParm2 >> (bParm1 & 0x1f),
                    in_EAX >> (bParm1 & 0x1f) | uParm2 << 0x20 - (bParm1 & 0x1f));
  }
  return (ulonglong)(uParm2 >> (bParm1 & 0x1f));
}



// WARNING: This is an inlined function
// WARNING: Function: __alloca_probe replaced with injection: alloca_probe

uint __alloca_probe_16(void)

{
  uint in_EAX;
  uint uVar1;
  
  uVar1 = 4 - in_EAX & 0xf;
  return in_EAX + uVar1 | -(uint)CARRY4(in_EAX,uVar1);
}



// WARNING: This is an inlined function
// WARNING: Function: __alloca_probe replaced with injection: alloca_probe

uint __alloca_probe_8(void)

{
  uint in_EAX;
  uint uVar1;
  
  uVar1 = 4 - in_EAX & 7;
  return in_EAX + uVar1 | -(uint)CARRY4(in_EAX,uVar1);
}



// WARNING: This is an inlined function
// Library Function - Single Match
// Name: __alloca_probe
// Libraries: Visual Studio 2015 Debug, Visual Studio 2015 Release

void __alloca_probe(void)

{
  undefined *in_EAX;
  undefined4 *puVar1;
  undefined4 local_res0;
  undefined auStack4 [4];
  
  puVar1 = (undefined4 *)((uint)auStack4 & 0xfffff000);
  while ((undefined4 *)
         ((uint)(&stack0x00000000 + -(int)in_EAX) & ~-(uint)(register0x00000010 < in_EAX)) < puVar1)
  {
    puVar1 = puVar1 + -0x400;
  }
  *(undefined4 *)((uint)(&stack0x00000000 + -(int)in_EAX) & ~-(uint)(register0x00000010 < in_EAX)) =
       local_res0;
  return;
}



// Library Function - Single Match
// Name: __allshl
// Library: Visual Studio

longlong __fastcall __allshl(byte bParm1,int iParm2)

{
  uint in_EAX;
  
  if (0x3f < bParm1) {
    return 0;
  }
  if (bParm1 < 0x20) {
    return CONCAT44(iParm2 << (bParm1 & 0x1f) | in_EAX >> 0x20 - (bParm1 & 0x1f),
                    in_EAX << (bParm1 & 0x1f));
  }
  return (ulonglong)(in_EAX << (bParm1 & 0x1f)) << 0x20;
}



undefined8 __fastcall FUN_00416550(undefined4 uParm1,undefined4 uParm2)

{
  ulonglong uVar1;
  uint uVar2;
  bool bVar3;
  float fVar4;
  float10 in_ST0;
  uint local_20;
  float fStack28;
  
  if (DAT_0041ec74 == 0) {
    uVar1 = (ulonglong)ROUND(in_ST0);
    local_20 = (uint)uVar1;
    fStack28 = (float)(uVar1 >> 0x20);
    fVar4 = (float)in_ST0;
    if ((local_20 != 0) || (fVar4 = fStack28, (uVar1 & 0x7fffffff00000000) != 0)) {
      if ((int)fVar4 < 0) {
        uVar2 = (uint)(0x80000000 < ((uint)(float)(in_ST0 - (float10)uVar1) ^ 0x80000000));
        bVar3 = CARRY4(local_20,uVar2);
        local_20 = local_20 + uVar2;
        fStack28 = (float)((int)fStack28 + (uint)bVar3);
      }
      else {
        uVar2 = (uint)(0x80000000 < (uint)(float)(in_ST0 - (float10)uVar1));
        bVar3 = local_20 < uVar2;
        local_20 = local_20 - uVar2;
        fStack28 = (float)((int)fStack28 - (uint)bVar3);
      }
    }
    return CONCAT44(fStack28,local_20);
  }
  return CONCAT44(uParm2,(int)in_ST0);
}



void __cdecl free(void *param_1)

{
  FID_conflict__free(param_1);
  return;
}



byte * __cdecl FUN_00416610(byte *param_1,byte param_2)

{
  char *pcVar1;
  undefined auVar2 [16];
  char *pcVar3;
  char *pcVar4;
  char *pcVar5;
  char *pcVar6;
  char *pcVar7;
  char *pcVar8;
  char *pcVar9;
  char *pcVar10;
  char *pcVar11;
  char *pcVar12;
  char *pcVar13;
  char *pcVar14;
  char *pcVar15;
  char *pcVar16;
  char *pcVar17;
  byte bVar18;
  uint uVar19;
  char *pcVar20;
  uint uVar21;
  int iVar22;
  byte *pbVar23;
  undefined4 unaff_EBX;
  uint uVar24;
  byte *pbVar25;
  char cVar27;
  char cVar28;
  char cVar29;
  char cVar30;
  char cVar31;
  char cVar32;
  char cVar33;
  char cVar34;
  char cVar35;
  char cVar36;
  char cVar37;
  char cVar38;
  char cVar39;
  char cVar40;
  char cVar41;
  undefined auVar26 [16];
  undefined auVar42 [16];
  
  if (DAT_0041ec74 != 0) {
    if (DAT_0041ec74 < 2) {
      auVar42 = pshuflw(ZEXT216(CONCAT11(param_2,param_2)),ZEXT216(CONCAT11(param_2,param_2)),0);
      auVar42 = movlhps(auVar42,auVar42);
      uVar21 = (uint)param_1 & 0xf;
      uVar19 = -1 << (sbyte)uVar21;
      param_1 = param_1 + -uVar21;
      pbVar23 = (byte *)0x0;
      while( true ) {
                    // WARNING: Load size is inaccurate
        auVar2 = *(undefined *)param_1;
        cVar27 = SUB161(auVar2 >> 8,0);
        cVar28 = SUB161(auVar2 >> 0x10,0);
        cVar29 = SUB161(auVar2 >> 0x18,0);
        cVar30 = SUB161(auVar2 >> 0x20,0);
        cVar31 = SUB161(auVar2 >> 0x28,0);
        cVar32 = SUB161(auVar2 >> 0x30,0);
        cVar33 = SUB161(auVar2 >> 0x38,0);
        cVar34 = SUB161(auVar2 >> 0x40,0);
        cVar35 = SUB161(auVar2 >> 0x48,0);
        cVar36 = SUB161(auVar2 >> 0x50,0);
        cVar37 = SUB161(auVar2 >> 0x58,0);
        cVar38 = SUB161(auVar2 >> 0x60,0);
        cVar39 = SUB161(auVar2 >> 0x68,0);
        cVar40 = SUB161(auVar2 >> 0x70,0);
        cVar41 = SUB161(auVar2 >> 0x78,0);
        auVar26 = CONCAT115(-(cVar41 == SUB161(auVar42 >> 0x78,0)),
                            CONCAT114(-(cVar40 == SUB161(auVar42 >> 0x70,0)),
                                      CONCAT113(-(cVar39 == SUB161(auVar42 >> 0x68,0)),
                                                CONCAT112(-(cVar38 == SUB161(auVar42 >> 0x60,0)),
                                                          CONCAT111(-(cVar37 ==
                                                                     SUB161(auVar42 >> 0x58,0)),
                                                                    CONCAT110(-(cVar36 ==
                                                                               SUB161(auVar42 >>
                                                                                      0x50,0)),
                                                                              CONCAT19(-(cVar35 ==
                                                                                        SUB161(
                                                  auVar42 >> 0x48,0)),
                                                  CONCAT18(-(cVar34 == SUB161(auVar42 >> 0x40,0)),
                                                           CONCAT17(-(cVar33 ==
                                                                     SUB161(auVar42 >> 0x38,0)),
                                                                    CONCAT16(-(cVar32 ==
                                                                              SUB161(auVar42 >> 0x30
                                                                                     ,0)),
                                                                             CONCAT15(-(cVar31 ==
                                                                                       SUB161(
                                                  auVar42 >> 0x28,0)),
                                                  CONCAT14(-(cVar30 == SUB161(auVar42 >> 0x20,0)),
                                                           CONCAT13(-(cVar29 ==
                                                                     SUB161(auVar42 >> 0x18,0)),
                                                                    CONCAT12(-(cVar28 ==
                                                                              SUB161(auVar42 >> 0x10
                                                                                     ,0)),
                                                                             CONCAT11(-(cVar27 ==
                                                                                       SUB161(
                                                  auVar42 >> 8,0)),
                                                  -(SUB161(auVar2,0) == SUB161(auVar42,0))))))))))))
                                                  )))));
        uVar21 = pmovmskb(uVar21,CONCAT115(-(cVar41 == 0),
                                           CONCAT114(-(cVar40 == 0),
                                                     CONCAT113(-(cVar39 == 0),
                                                               CONCAT112(-(cVar38 == 0),
                                                                         CONCAT111(-(cVar37 == 0),
                                                                                   CONCAT110(-(
                                                  cVar36 == 0),
                                                  CONCAT19(-(cVar35 == 0),
                                                           CONCAT18(-(cVar34 == 0),
                                                                    CONCAT17(-(cVar33 == 0),
                                                                             CONCAT16(-(cVar32 == 0)
                                                                                      ,CONCAT15(-(
                                                  cVar31 == 0),
                                                  CONCAT14(-(cVar30 == 0),
                                                           CONCAT13(-(cVar29 == 0),
                                                                    CONCAT12(-(cVar28 == 0),
                                                                             CONCAT11(-(cVar27 == 0)
                                                                                      ,-(SUB161(
                                                  auVar2,0) == 0)))))))))))))))));
        uVar21 = uVar21 & uVar19;
        if (uVar21 != 0) break;
        uVar21 = pmovmskb(0,auVar26);
        uVar21 = uVar21 & uVar19;
        iVar22 = 0x1f;
        if (uVar21 != 0) {
          while (uVar21 >> iVar22 == 0) {
            iVar22 = iVar22 + -1;
          }
        }
        if (uVar21 != 0) {
          pbVar23 = param_1 + iVar22;
        }
        uVar19 = 0xffffffff;
        param_1 = param_1 + 0x10;
      }
      uVar24 = pmovmskb(unaff_EBX,auVar26);
      uVar19 = (uVar21 * 2 & uVar21 * -2) - 1 & uVar24 & uVar19;
      iVar22 = 0x1f;
      if (uVar19 != 0) {
        while (uVar19 >> iVar22 == 0) {
          iVar22 = iVar22 + -1;
        }
      }
      pbVar25 = param_1 + iVar22;
      if (uVar19 == 0) {
        pbVar25 = pbVar23;
      }
      return pbVar25;
    }
    if (param_2 == 0) {
      pcVar20 = (char *)((uint)param_1 & 0xfffffff0);
      uVar19 = pmovmskb(param_1,CONCAT115(-(pcVar20[0xf] == 0),
                                          CONCAT114(-(pcVar20[0xe] == 0),
                                                    CONCAT113(-(pcVar20[0xd] == 0),
                                                              CONCAT112(-(pcVar20[0xc] == 0),
                                                                        CONCAT111(-(pcVar20[0xb] ==
                                                                                   0),CONCAT110(-(
                                                  pcVar20[10] == 0),
                                                  CONCAT19(-(pcVar20[9] == 0),
                                                           CONCAT18(-(pcVar20[8] == 0),
                                                                    CONCAT17(-(pcVar20[7] == 0),
                                                                             CONCAT16(-(pcVar20[6]
                                                                                       == 0),
                                                  CONCAT15(-(pcVar20[5] == 0),
                                                           CONCAT14(-(pcVar20[4] == 0),
                                                                    CONCAT13(-(pcVar20[3] == 0),
                                                                             CONCAT12(-(pcVar20[2]
                                                                                       == 0),
                                                  CONCAT11(-(pcVar20[1] == 0),-(*pcVar20 == 0)))))))
                                                  ))))))))));
      uVar19 = uVar19 & -1 << ((byte)param_1 & 0xf);
      while (uVar19 == 0) {
        pcVar17 = pcVar20 + 0x1f;
        pcVar16 = pcVar20 + 0x1e;
        pcVar15 = pcVar20 + 0x1d;
        pcVar14 = pcVar20 + 0x1c;
        pcVar13 = pcVar20 + 0x1b;
        pcVar12 = pcVar20 + 0x1a;
        pcVar11 = pcVar20 + 0x19;
        pcVar10 = pcVar20 + 0x18;
        pcVar9 = pcVar20 + 0x17;
        pcVar8 = pcVar20 + 0x16;
        pcVar7 = pcVar20 + 0x15;
        pcVar6 = pcVar20 + 0x14;
        pcVar5 = pcVar20 + 0x13;
        pcVar4 = pcVar20 + 0x12;
        pcVar3 = pcVar20 + 0x11;
        pcVar1 = pcVar20 + 0x10;
        pcVar20 = pcVar20 + 0x10;
        uVar19 = pmovmskb(0,CONCAT115(-(*pcVar17 == 0),
                                      CONCAT114(-(*pcVar16 == 0),
                                                CONCAT113(-(*pcVar15 == 0),
                                                          CONCAT112(-(*pcVar14 == 0),
                                                                    CONCAT111(-(*pcVar13 == 0),
                                                                              CONCAT110(-(*pcVar12 
                                                  == 0),CONCAT19(-(*pcVar11 == 0),
                                                                 CONCAT18(-(*pcVar10 == 0),
                                                                          CONCAT17(-(*pcVar9 == 0),
                                                                                   CONCAT16(-(*
                                                  pcVar8 == 0),
                                                  CONCAT15(-(*pcVar7 == 0),
                                                           CONCAT14(-(*pcVar6 == 0),
                                                                    CONCAT13(-(*pcVar5 == 0),
                                                                             CONCAT12(-(*pcVar4 == 0
                                                                                       ),CONCAT11(-(
                                                  *pcVar3 == 0),-(*pcVar1 == 0)))))))))))))))));
      }
      iVar22 = 0;
      if (uVar19 != 0) {
        while ((uVar19 >> iVar22 & 1) == 0) {
          iVar22 = iVar22 + 1;
        }
      }
      pbVar23 = (byte *)(pcVar20 + iVar22);
    }
    else {
      pbVar23 = (byte *)0x0;
      uVar19 = (uint)param_1 & 0xf;
      while (uVar19 != 0) {
        if ((uint)*param_1 == (uint)param_2) {
          pbVar23 = param_1;
        }
        if (*param_1 == 0) {
          return pbVar23;
        }
        param_1 = param_1 + 1;
        uVar19 = (uint)param_1 & 0xf;
      }
      do {
        pbVar25 = param_1 + 0x10;
                    // WARNING: Load size is inaccurate
        iVar22 = pcmpistri(ZEXT416((uint)param_2),*(undefined *)param_1,0x40);
        if ((byte *)0xffffffef < param_1) {
          pbVar23 = pbVar25 + iVar22 + -0x10;
        }
        param_1 = pbVar25;
      } while (pbVar25 != (byte *)0x0);
    }
    return pbVar23;
  }
  iVar22 = -1;
  do {
    pbVar23 = param_1;
    if (iVar22 == 0) break;
    iVar22 = iVar22 + -1;
    pbVar23 = param_1 + 1;
    bVar18 = *param_1;
    param_1 = pbVar23;
  } while (bVar18 != 0);
  iVar22 = -(iVar22 + 1);
  pbVar23 = pbVar23 + -1;
  do {
    pbVar25 = pbVar23;
    if (iVar22 == 0) break;
    iVar22 = iVar22 + -1;
    pbVar25 = pbVar23 + -1;
    bVar18 = *pbVar23;
    pbVar23 = pbVar25;
  } while (param_2 != bVar18);
  pbVar25 = pbVar25 + 1;
  if (*pbVar25 != param_2) {
    pbVar25 = (byte *)0x0;
  }
  return pbVar25;
}



void Unwind_00416896(void)

{
  uint *puVar1;
  int unaff_EBP;
  
  if (*(char *)(unaff_EBP + -0x30) != 0) {
    puVar1 = (uint *)(*(int *)(unaff_EBP + -0x3c) + 0x350);
    *puVar1 = *puVar1 & 0xfffffffd;
  }
  return;
}


