/*
 * Copyright 2017 André Hentschel
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

/* NOTE: The guest side uses mingw's headers. The host side uses Wine's headers. */

#include <windows.h>
#include <stdio.h>

#include "thunk/qemu_windows.h"

#include "windows-user-services.h"
#include "dll_list.h"
#include "qemu_kernel32.h"

#ifndef QEMU_DLL_GUEST
#include <wine/debug.h>
#include "callback_helper.h"
WINE_DEFAULT_DEBUG_CHANNEL(qemu_kernel32);
#endif


struct qemu_FindFirstChangeNotificationA
{
    struct qemu_syscall super;
    uint64_t lpPathName;
    uint64_t bWatchSubtree;
    uint64_t dwNotifyFilter;
};

#ifdef QEMU_DLL_GUEST

WINBASEAPI HANDLE WINAPI FindFirstChangeNotificationA(LPCSTR lpPathName, BOOL bWatchSubtree, DWORD dwNotifyFilter)
{
    struct qemu_FindFirstChangeNotificationA call;
    call.super.id = QEMU_SYSCALL_ID(CALL_FINDFIRSTCHANGENOTIFICATIONA);
    call.lpPathName = (ULONG_PTR)lpPathName;
    call.bWatchSubtree = bWatchSubtree;
    call.dwNotifyFilter = dwNotifyFilter;

    qemu_syscall(&call.super);

    return (HANDLE)(ULONG_PTR)call.super.iret;
}

#else

void qemu_FindFirstChangeNotificationA(struct qemu_syscall *call)
{
    struct qemu_FindFirstChangeNotificationA *c = (struct qemu_FindFirstChangeNotificationA *)call;
    WINE_TRACE("\n");
    c->super.iret = QEMU_H2G(FindFirstChangeNotificationA(QEMU_G2H(c->lpPathName), c->bWatchSubtree, c->dwNotifyFilter));
}

#endif

struct qemu_FindFirstChangeNotificationW
{
    struct qemu_syscall super;
    uint64_t lpPathName;
    uint64_t bWatchSubtree;
    uint64_t dwNotifyFilter;
};

#ifdef QEMU_DLL_GUEST

WINBASEAPI HANDLE WINAPI FindFirstChangeNotificationW(LPCWSTR lpPathName, BOOL bWatchSubtree, DWORD dwNotifyFilter)
{
    struct qemu_FindFirstChangeNotificationW call;
    call.super.id = QEMU_SYSCALL_ID(CALL_FINDFIRSTCHANGENOTIFICATIONW);
    call.lpPathName = (ULONG_PTR)lpPathName;
    call.bWatchSubtree = bWatchSubtree;
    call.dwNotifyFilter = dwNotifyFilter;

    qemu_syscall(&call.super);

    return (HANDLE)(ULONG_PTR)call.super.iret;
}

#else

void qemu_FindFirstChangeNotificationW(struct qemu_syscall *call)
{
    struct qemu_FindFirstChangeNotificationW *c = (struct qemu_FindFirstChangeNotificationW *)call;
    WINE_TRACE("\n");
    c->super.iret = QEMU_H2G(FindFirstChangeNotificationW(QEMU_G2H(c->lpPathName), c->bWatchSubtree, c->dwNotifyFilter));
}

#endif

struct qemu_FindNextChangeNotification
{
    struct qemu_syscall super;
    uint64_t handle;
};

#ifdef QEMU_DLL_GUEST

WINBASEAPI BOOL WINAPI FindNextChangeNotification(HANDLE handle)
{
    struct qemu_FindNextChangeNotification call;
    call.super.id = QEMU_SYSCALL_ID(CALL_FINDNEXTCHANGENOTIFICATION);
    call.handle = (ULONG_PTR)handle;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_FindNextChangeNotification(struct qemu_syscall *call)
{
    struct qemu_FindNextChangeNotification *c = (struct qemu_FindNextChangeNotification *)call;
    WINE_TRACE("\n");
    c->super.iret = FindNextChangeNotification(QEMU_G2H(c->handle));
}

#endif

struct qemu_FindCloseChangeNotification
{
    struct qemu_syscall super;
    uint64_t handle;
};

#ifdef QEMU_DLL_GUEST

WINBASEAPI BOOL WINAPI FindCloseChangeNotification(HANDLE handle)
{
    struct qemu_FindCloseChangeNotification call;
    call.super.id = QEMU_SYSCALL_ID(CALL_FINDCLOSECHANGENOTIFICATION);
    call.handle = (ULONG_PTR)handle;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_FindCloseChangeNotification(struct qemu_syscall *call)
{
    struct qemu_FindCloseChangeNotification *c = (struct qemu_FindCloseChangeNotification *)call;
    WINE_TRACE("\n");
    c->super.iret = FindCloseChangeNotification(QEMU_G2H(c->handle));
}

#endif

struct qemu_ReadDirectoryChangesW
{
    struct qemu_syscall super;
    uint64_t handle;
    uint64_t buffer;
    uint64_t len;
    uint64_t subtree;
    uint64_t filter;
    uint64_t returned;
    uint64_t overlapped;
    uint64_t completion;
};

#ifdef QEMU_DLL_GUEST

WINBASEAPI BOOL WINAPI ReadDirectoryChangesW(HANDLE handle, LPVOID buffer, DWORD len, BOOL subtree, DWORD filter,
        LPDWORD returned, LPOVERLAPPED overlapped, LPOVERLAPPED_COMPLETION_ROUTINE completion)
{
    struct qemu_ReadDirectoryChangesW call;
    call.super.id = QEMU_SYSCALL_ID(CALL_READDIRECTORYCHANGESW);
    call.handle = (ULONG_PTR)handle;
    call.buffer = (ULONG_PTR)buffer;
    call.len = len;
    call.subtree = subtree;
    call.filter = filter;
    call.returned = (ULONG_PTR)returned;
    call.overlapped = (ULONG_PTR)overlapped;
    call.completion = (ULONG_PTR)completion;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_ReadDirectoryChangesW(struct qemu_syscall *call)
{
    struct qemu_ReadDirectoryChangesW *c = (struct qemu_ReadDirectoryChangesW *)call;
    uint64_t guest_completion;
    OVERLAPPED *ov;
    struct qemu_OVERLAPPED *ov32;
    struct callback_entry *wrapper = NULL;
    struct OVERLAPPED_data *ov_wrapper;
    HANDLE guest_event;

    WINE_TRACE("\n");
    guest_completion = c->completion;

#if GUEST_BIT == HOST_BIT
    ov = QEMU_G2H(c->overlapped);
    if (guest_completion && ov)
    {
        wrapper = callback_get(overlapped_wrappers, guest_completion, NULL);
        if (!wrapper)
            WINE_ERR("Failed to get an overlapped IO callback wrapper.\n");
    }

    c->super.iret = ReadDirectoryChangesW(QEMU_G2H(c->handle), QEMU_G2H(c->buffer), c->len, c->subtree,
            c->filter, QEMU_G2H(c->returned), ov, (LPOVERLAPPED_COMPLETION_ROUTINE)wrapper);
    return;
#endif

    ov32 = QEMU_G2H(c->overlapped);
    if (!ov32)
    {
        WINE_TRACE("Synchonous operation, easy...\n");
        c->super.iret = ReadDirectoryChangesW(QEMU_G2H(c->handle), QEMU_G2H(c->buffer), c->len, c->subtree,
                c->filter, QEMU_G2H(c->returned), NULL, NULL);
        return;
    }
    ov_wrapper = alloc_OVERLAPPED_data(ov32, guest_completion, TRUE);
    guest_event = HANDLE_g2h(ov32->hEvent);

    WINE_TRACE("Async operation\n");
    c->super.iret = ReadDirectoryChangesW(QEMU_G2H(c->handle), QEMU_G2H(c->buffer), c->len, c->subtree,
            c->filter, QEMU_G2H(c->returned), &ov_wrapper->ov, NULL);
    WINE_TRACE("result %x\n", (unsigned int)c->super.iret);

    OVERLAPPED_h2g(ov32, &ov_wrapper->ov);
    ov32->hEvent = (ULONG_PTR)guest_event;

    if (c->super.iret)
    {
        if (guest_event != INVALID_HANDLE_VALUE && guest_event)
            ResetEvent(guest_event);

        ov_wrapper->cb_thread = GetCurrentThreadId();

        WINE_TRACE("Async return, starting wait thread.\n");
        if (!QueueUserWorkItem(overlapped32_wait_func, ov_wrapper, 0))
            WINE_ERR("Failed to queue work item\n");
    }
    else
    {
        WINE_TRACE("Error return.\n");
        CloseHandle(ov_wrapper->ov.hEvent);
        HeapFree(GetProcessHeap(), 0, ov_wrapper);
    }
}

#endif

