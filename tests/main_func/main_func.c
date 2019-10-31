int main(void)
{
    int val = 3;
    val++;
    return val;
}
/*
 * 00000000  E805000000        call dword 0xa ; pushes ret addr ; ESP 0x7C00 - 4 = 0x7BFC: 7C05 (7C00 + 5)
 * 00000005  E9F683FFFF        jmp dword 0xffff8400 (-31744) <-0x7C00: 31744
 * 0000000A  55                push ebp ; pushes current EBP ; ESP 0x7BFC - 4 = 0x7BF8: 0x0000
 * 0000000B  89E5              mov ebp,esp ; brings current ESP to EBP ; EBP: 0x7BF8
 * 0000000D  83EC10            sub esp,byte +0x10 ; moves ESP down by 16 ; ESP: 0x7BF8 - 16 = 7BE8
 * 00000010  C745FC03000000    mov dword [ebp-0x4],0x3 ; EBP: 0x7BF8 - 4 = 0x7BF4: 3
 * 00000017  8345FC01          add dword [ebp-0x4],byte +0x1 ; 0x7BF4: 4
 * 0000001B  8B45FC            mov eax,[ebp-0x4] ; EAX: 4
 * 0000001E  C9                leave ; updates ESP with current EBP: ESP: 0x7BE8 -> 0x7BF8 and pops old EBP to EBP: 0x0000 & ESP: 0x7BFC
 * 0000001F  C3                ret ; pops return address from ESP: 0x7BFC
 */