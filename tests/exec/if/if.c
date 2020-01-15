int abs(int i)
{
    if (i >= 0)
    {
        return i;
    }
    else
    {
        return -i;
    }
}

int main(void)
{
    return abs(-3);
}

/*
 * LDFLAG: -Ttext 0x7c00: 31744
 * 00000000  E81A000000        call dword 0x1f ; 0x0000001A: 26 ; 0 + 0x1A + 5 = 1F ; Push 7c05 to ESP: 7CFC ;
 * 00000005  E9F683FFFF        jmp dword 0xF683FFFF ; FFFF83F6: -31754 -> 31749(this EIP) +5(instruction) - 31754 = 0: jump to 0
 * 0000000A  55                push ebp ; OPTIMIZED~
 * 0000000B  89E5              mov ebp,esp
 * 0000000D  837D0800          cmp dword [ebp+0x8],byte +0x0
 * 00000011  7805              js 0x18
 * 00000013  8B4508            mov eax,[ebp+0x8]
 * 00000016  EB05              jmp short 0x1d
 * 00000018  8B4508            mov eax,[ebp+0x8]
 * 0000001B  F7D8              neg eax
 * 0000001D  5D                pop ebp
 * 0000001E  C3                ret ; ~OPTIMIZED
 * 0000001F  55                push ebp ; main() ; ESP: 7BF8: 0000
 * 00000020  89E5              mov ebp,esp ; EBP: 7BF8
 * 00000022  B803000000        mov eax,0x3 ; EAX: 3
 * 00000027  5D                pop ebp ; EBP: 0000 & ESP: 7BFC
 * 00000028  C3                ret ; Pops 7BFC: 7c05 and jumps to it. ESP: 7C00 ;
 */ 