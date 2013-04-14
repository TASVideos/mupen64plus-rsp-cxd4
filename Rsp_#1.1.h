#ifndef __RSP_1_1_H__
#define __RSP_1_1_H__

#if defined(__cplusplus)
extern "C" {
#endif

#define PLUGIN_TYPE_RSP             1
#define PLUGIN_TYPE_GFX             2
#define PLUGIN_TYPE_AUDIO           3
#define PLUGIN_TYPE_CONTROLLER      4

struct HWND__ {int unused;};
typedef struct HWND__ *HWND; /* Use void pointers on GCC or old Windows. */
struct HINSTANCE__ {int unused;};
typedef struct HINSTANCE__ *HINSTANCE;
struct HMENU__ {int unused;};
typedef struct HMENU__ *HMENU;
struct HDC__ {int unused;};
typedef struct HDC__ *HDC;

typedef struct {
	unsigned short Version;        /* Should be set to 0x0101 */
	unsigned short Type;           /* Set to PLUGIN_TYPE_RSP */
	char Name[100];      /* Name of the DLL */

	/* If DLL supports memory these memory options then set them to TRUE or FALSE
	   if it does not support it */
	int NormalMemory;   /* a normal BYTE array */ 
	int MemoryBswaped;  /* a normal BYTE array where the memory has been pre
	                          bswap on a dword (32 bits) boundry */
} PLUGIN_INFO;

typedef struct {
	HINSTANCE hInst;
	int MemoryBswaped;    /* If this is set to TRUE, then the memory has been pre
	                          bswap on a dword (32 bits) boundry */
	unsigned char *RDRAM;
	unsigned char *DMEM;
	unsigned char *IMEM;

	unsigned long *MI_INTR_REG;

	unsigned long *SP_MEM_ADDR_REG;
	unsigned long *SP_DRAM_ADDR_REG;
	unsigned long *SP_RD_LEN_REG;
	unsigned long *SP_WR_LEN_REG;
	unsigned long *SP_STATUS_REG;
	unsigned long *SP_DMA_FULL_REG;
	unsigned long *SP_DMA_BUSY_REG;
	unsigned long *SP_PC_REG;
	unsigned long *SP_SEMAPHORE_REG;

	unsigned long *DPC_START_REG;
	unsigned long *DPC_END_REG;
	unsigned long *DPC_CURRENT_REG;
	unsigned long *DPC_STATUS_REG;
	unsigned long *DPC_CLOCK_REG;
	unsigned long *DPC_BUFBUSY_REG;
	unsigned long *DPC_PIPEBUSY_REG;
	unsigned long *DPC_TMEM_REG;

	void (*CheckInterrupts)( void );
	void (*ProcessDList)( void );
	void (*ProcessAList)( void );
	void (*ProcessRdpList)( void );
	void (*ShowCFB)( void );
} RSP_INFO;

typedef struct tagRECT {
    long left;
    long top;
    long right;
    long bottom;
} RECT;

typedef struct tagPAINTSTRUCT {
    HDC hdc;
    int fErase;
    RECT rcPaint;
    int fRestore;
    int fIncUpdate;
    unsigned char rgbReserved[32];
} PAINTSTRUCT;

typedef struct {
	/* Menu */
	/* Items should have an ID between 5001 and 5100 */
	HMENU hRSPMenu;
	void (*ProcessMenuItem) ( int ID );

	/* Break Points */
	int UseBPoints;
	char BPPanelName[20];
	void (*Add_BPoint)     (void);
	void (*CreateBPPanel)  (HWND hDlg, RECT rcBox);
	void (*HideBPPanel)    (void);
	void (*PaintBPPanel)   (PAINTSTRUCT ps);
	void (*ShowBPPanel)    (void);
	void (*RefreshBpoints) (HWND hList);
	void (*RemoveBpoint)   (HWND hList, int index);
	void (*RemoveAllBpoint)(void);
	
	/* RSP command Window */
	void (*Enter_RSP_Commands_Window)(void);

} RSPDEBUG_INFO;

typedef struct {
	void (*UpdateBreakPoints)(void);
	void (*UpdateMemory)(void);
	void (*UpdateR4300iRegisters)(void);
	void (*Enter_BPoint_Window)(void);
	void (*Enter_R4300i_Commands_Window)(void);
	void (*Enter_R4300i_Register_Window)(void);
	void (*Enter_RSP_Commands_Window) (void);
	void (*Enter_Memory_Window)(void);
} DEBUG_INFO;

/******************************************************************
  Function: CloseDLL
  Purpose:  This function is called when the emulator is closing
            down allowing the dll to de-initialise.
  input:    none
  output:   none
*******************************************************************/ 
__declspec(dllexport) void CloseDLL(void);

/******************************************************************
  Function: DllAbout
  Purpose:  This function is optional function that is provided
            to give further information about the DLL.
  input:    a handle to the window that calls this function
  output:   none
*******************************************************************/ 
__declspec(dllexport) void DllAbout(HWND hParent);

/******************************************************************
  Function: DllConfig
  Purpose:  This function is optional function that is provided
            to allow the user to configure the dll
  input:    a handle to the window that calls this function
  output:   none
*******************************************************************/
__declspec(dllexport) void DllConfig(HWND hParent);

/******************************************************************
  Function: DllTest
  Purpose:  This function is optional function that is provided
            to allow the user to test the dll
  input:    a handle to the window that calls this function
  output:   none
*******************************************************************/ 
__declspec(dllexport) void DllTest(HWND hParent);

/******************************************************************
  Function: DoRspCycles
  Purpose:  This function is to allow the RSP to run in parrel with
            the r4300 switching control back to the r4300 once the
			function ends.
  input:    The number of cylces that is meant to be executed
  output:   The number of cycles that was executed. This value can
            be greater than the number of cycles that the RSP 
			should have performed.
			(this value is ignored if the RSP is stoped)
*******************************************************************/ 
__declspec(dllexport) unsigned int DoRspCycles(unsigned int Cycles);

/******************************************************************
  Function: GetDllInfo
  Purpose:  This function allows the emulator to gather information
            about the dll by filling in the PluginInfo structure.
  input:    a pointer to a PLUGIN_INFO stucture that needs to be
            filled by the function. (see def above)
  output:   none
*******************************************************************/ 
__declspec(dllexport) void GetDllInfo(PLUGIN_INFO *PluginInfo);

/******************************************************************
  Function: GetRspDebugInfo
  Purpose:  This function allows the emulator to gather information
            about the debug capabilities of the dll by filling in
			the DebugInfo structure.
  input:    a pointer to a RSPDEBUG_INFO stucture that needs to be
            filled by the function. (see def above)
  output:   none
*******************************************************************/ 
__declspec(dllexport) void GetRspDebugInfo(RSPDEBUG_INFO *RSPDebugInfo);

/******************************************************************
  Function: InitiateRSP
  Purpose:  This function is called when the DLL is started to give
            information from the emulator that the n64 RSP 
			interface needs
  input:    Rsp_Info is passed to this function which is defined
            above.
			CycleCount is the number of cycles between switching
			control between the RSP and r4300i core.
  output:   none
*******************************************************************/ 
__declspec(dllexport) void InitiateRSP(RSP_INFO Rsp_Info, unsigned int *CycleCount);

/******************************************************************
  Function: InitiateRSPDebugger
  Purpose:  This function is called when the DLL is started to give
            information from the emulator that the n64 RSP 
			interface needs to intergrate the debugger with the
			rest of the emulator.
  input:    DebugInfo is passed to this function which is defined
            above.
  output:   none
*******************************************************************/ 
__declspec(dllexport) void InitiateRSPDebugger(DEBUG_INFO DebugInfo);

/******************************************************************
  Function: RomClosed
  Purpose:  This function is called when a rom is closed.
  input:    none
  output:   none
*******************************************************************/ 
__declspec(dllexport) void RomClosed(void);

#if defined(__cplusplus)
}
#endif

#endif // __RSP_1_1_H__
