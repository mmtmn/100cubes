section .data
    window_title db 'Square Example', 0
    window_width equ 640
    window_height equ 480
    square_size equ 100
    square_color equ 0xFF0000

section .bss
    window resq 1
    renderer resq 1

section .text
    extern SDL_Init
    extern SDL_CreateWindow
    extern SDL_CreateRenderer
    extern SDL_SetRenderDrawColor
    extern SDL_RenderClear
    extern SDL_RenderFillRect
    extern SDL_RenderPresent
    extern SDL_Delay
    extern SDL_DestroyRenderer
    extern SDL_DestroyWindow
    extern SDL_Quit

    global main

main:
    ; Initialize SDL
    push dword 0
    call SDL_Init
    add esp, 4

    ; Create window
    push dword 0
    push dword window_height
    push dword window_width
    push dword window_title
    call SDL_CreateWindow
    add esp, 16
    mov [window], eax

    ; Create renderer
    push dword 0
    push dword -1
    push dword [window]
    call SDL_CreateRenderer
    add esp, 12
    mov [renderer], eax

    ; Set render draw color
    push dword 0
    push dword 0
    push dword 0
    push dword 0xFF
    push dword [renderer]
    call SDL_SetRenderDrawColor
    add esp, 20

    ; Clear renderer
    push dword [renderer]
    call SDL_RenderClear
    add esp, 4

    ; Set square color
    push dword square_color
    push dword [renderer]
    call SDL_SetRenderDrawColor
    add esp, 8

    ; Calculate square position
    mov eax, window_width
    sub eax, square_size
    shr eax, 1
    mov ebx, window_height
    sub ebx, square_size
    shr ebx, 1

    ; Draw square
    push dword square_size
    push dword square_size
    push dword ebx
    push dword eax
    push dword [renderer]
    call SDL_RenderFillRect
    add esp, 20

    ; Present renderer
    push dword [renderer]
    call SDL_RenderPresent
    add esp, 4

    ; Delay
    push dword 2000
    call SDL_Delay
    add esp, 4

    ; Cleanup
    push dword [renderer]
    call SDL_DestroyRenderer
    add esp, 4

    push dword [window]
    call SDL_DestroyWindow
    add esp, 4

    call SDL_Quit

    ; Exit
    mov eax, 1
    xor ebx, ebx
    int 0x80