; Define constants
IDI_ICON1       EQU 101
IMAGE_ICON      EQU 1
LR_DEFAULTSIZE  EQU 64
NULL            EQU 0

; Define HANDLE type
HANDLE typedef QWORD

; Define HICON type
HICON typedef HANDLE

; Declare external functions
externdef GetModuleHandleW:proc
externdef LoadImageW:proc

; Define the code segment
.code

; Function: LoadIconFromResource
; Returns: HICON (RAX)
LoadIconFromResource proc
    ; Save non-volatile registers
    push rbx
    push rsi
    push rdi
    push rbp
    mov rbp, rsp
    sub rsp, 30h  ; 32 bytes shadow space + 16 bytes for alignment

    ; Call GetModuleHandleW(NULL)
    xor ecx, ecx          ; NULL = 0
    call GetModuleHandleW ; GetModuleHandleW(NULL)
    mov rbx, rax          ; Save handle in RBX

    ; Ensure stack alignment (sub rsp, 8) before calling LoadImageW
    sub rsp, 8

    ; Call LoadImageW
    mov rcx, rbx          ; hInst (module handle)
    mov rdx, IDI_ICON1    ; lpszName (resource ID)
    mov r8, IMAGE_ICON    ; uType (IMAGE_ICON)
    xor r9, r9            ; cxDesired = 0
    mov qword ptr [rsp+20h], 0 ; cyDesired = 0
    mov qword ptr [rsp+28h], LR_DEFAULTSIZE ; fuLoad = LR_DEFAULTSIZE
    call LoadImageW       ; LoadImageW(hInst, IDI_ICON1, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE)

    ; Restore stack alignment
    add rsp, 8

    ; Clean up stack and restore registers
    add rsp, 30h          ; Deallocate shadow space
    pop rbp
    pop rdi
    pop rsi
    pop rbx
    ret
LoadIconFromResource endp


end