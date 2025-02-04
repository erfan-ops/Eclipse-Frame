; Declare external functions
externdef CreateWorkerWLayer:proc
externdef SetParent:proc

; Define the code segment
.code

; Function: SetAsDesktop
; Parameters:
;   hwnd: HWND (passed in rcx)
SetAsDesktop proc
    ; Save non-volatile registers
    push rbx
    push rsi
    push rdi
    push rbp
    sub rsp, 20h  ; Allocate shadow space for function calls

    ; Save the hwnd parameter (rcx) in a non-volatile register
    mov rbx, rcx  ; rbx = hwnd

    ; Call CreateWorkerWLayer to get the WorkerW handle
    call CreateWorkerWLayer  ; Call CreateWorkerWLayer()
    test rax, rax            ; Check if WorkerW handle is NULL
    jz cleanup               ; If NULL, jump to cleanup

    ; Set the parent of the target window to WorkerW
    mov rcx, rbx  ; hwnd (first argument)
    mov rdx, rax  ; workerw (second argument)
    call SetParent  ; Call SetParent(hwnd, workerw)

cleanup:
    ; Clean up stack and restore registers
    add rsp, 20h
    pop rbp
    pop rdi
    pop rsi
    pop rbx
    ret
SetAsDesktop endp

end