; Define constants
IDI_ICON1       EQU 101
IMAGE_ICON      EQU 1
LR_DEFAULTSIZE  EQU 64

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
    sub rsp, 38h  ; 32 bytes shadow space + 16 bytes for alignment + 8 bytes for calling LoadImageW

    ; Call GetModuleHandleW(NULL)
    xor ecx, ecx          ; NULL = 0
    call GetModuleHandleW ; GetModuleHandleW(NULL)
    mov rcx, rax          ; Save handle in RCX

    ; Call LoadImageW
    mov rdx, IDI_ICON1    ; lpszName (resource ID)
    mov r8, IMAGE_ICON    ; uType (IMAGE_ICON)
    xor r9, r9            ; cxDesired = 0
    mov qword ptr [rsp+20h], 0 ; cyDesired = 0
    mov qword ptr [rsp+28h], LR_DEFAULTSIZE ; fuLoad = LR_DEFAULTSIZE
    call LoadImageW       ; LoadImageW(hInst, IDI_ICON1, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE)

    ; Clean up stack and restore registers
    add rsp, 38h          ; Deallocate shadow space
    pop rbp
    pop rdi
    pop rsi
    pop rbx
    ret
LoadIconFromResource endp


end