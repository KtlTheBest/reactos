/*
 * internal executive prototypes
 */

#ifndef __NTOSKRNL_INCLUDE_INTERNAL_EXECUTIVE_H
#define __NTOSKRNL_INCLUDE_INTERNAL_EXECUTIVE_H

#define NTOS_MODE_KERNEL
#include <ntos.h>

typedef struct _WINSTATION_OBJECT
{
  CSHORT Type;
  CSHORT Size;
  KSPIN_LOCK Lock;
  UNICODE_STRING Name;
  LIST_ENTRY DesktopListHead;
  PRTL_ATOM_TABLE AtomTable;
  UINT CaretBlinkRate;
#ifdef __WIN32K__
  PSYSTEM_CURSORINFO SystemCursor;
  PUSER_HANDLE_TABLE HandleTable;
  PWINDOW_OBJECT ShellWindow;
  PWINDOW_OBJECT ShellListView;
  PMENU_OBJECT SystemMenuTemplate;
#else
  PVOID SystemCursor;
  PVOID HandleTable;
  PVOID ShellWindow;
  PVOID ShellListView;
  PVOID SystemMenuTemplate;
#endif
  ULONG Flags;
  struct _DESKTOP_OBJECT* InputDesktop;
  /* FIXME: Clipboard */
  LIST_ENTRY HotKeyListHead;
  FAST_MUTEX HotKeyListLock;
} WINSTATION_OBJECT, *PWINSTATION_OBJECT;

typedef struct _DESKTOP_OBJECT
{
  CSHORT Type;
  CSHORT Size;
  LIST_ENTRY ListEntry;
  KSPIN_LOCK Lock;
  UNICODE_STRING Name;
  /* Pointer to the associated window station. */
  struct _WINSTATION_OBJECT *WindowStation;
#ifdef __WIN32K__
  /* Pointer to the active queue. */
  PUSER_MESSAGE_QUEUE ActiveMessageQueue;
  /* Rectangle of the work area */
  RECT WorkArea;
  /* Pointer of the desktop window. */
  PWINDOW_OBJECT DesktopWindow;
  PWINDOW_OBJECT PrevActiveWindow;
#else
  /* Pointer to the active queue. */
  PVOID ActiveMessageQueue;
  /* Rectangle of the work area */
  LONG WorkArea[4];
  /* Pointer to the desktop window. */
  PVOID DesktopWindow;
  PVOID PrevActiveWindow;
#endif
  /* Thread blocking input */
  PVOID BlockInputThread;
} DESKTOP_OBJECT, *PDESKTOP_OBJECT;


typedef VOID (*PLOOKASIDE_MINMAX_ROUTINE)(
  POOL_TYPE PoolType,
  ULONG Size,
  PUSHORT MinimumDepth,
  PUSHORT MaximumDepth);

/* GLOBAL VARIABLES *********************************************************/

TIME_ZONE_INFORMATION _SystemTimeZoneInfo;
extern POBJECT_TYPE ExEventPairObjectType;


/* INITIALIZATION FUNCTIONS *************************************************/

VOID
ExpWin32kInit(VOID);

VOID 
ExInit2 (VOID);
VOID
ExInit3 (VOID);
VOID 
ExInitTimeZoneInfo (VOID);
VOID 
ExInitializeWorkerThreads(VOID);
VOID
ExpInitLookasideLists(VOID);
VOID
ExpInitializeCallbacks(VOID);

/* OTHER FUNCTIONS **********************************************************/

#ifdef _ENABLE_THRDEVTPAIR
VOID
ExpSwapThreadEventPair(
	IN struct _ETHREAD* Thread,
	IN struct _KEVENT_PAIR* EventPair
	);
#endif /* _ENABLE_THRDEVTPAIR */

LONGLONG
FASTCALL
ExfpInterlockedExchange64(LONGLONG volatile * Destination,
	PLONGLONG Exchange);

#endif /* __NTOSKRNL_INCLUDE_INTERNAL_EXECUTIVE_H */
