int add(int a, int b)
{
    return a + b;
}

int main(void)
{
    return add(2, 5);
}

/*
 * 00000000  E812000000        call dword 0x17 ; pushes ret addr ; ESP 0x7C00 - 4 = 0x7BFC: 7C05 (EIP: 7C00 + 5) ;
 * 00000005  E9F683FFFF        jmp dword 0xffff8400
 * 0000000A  55                push ebp ; add() ; 7BE8: 7BF8 from EBP
 * 0000000B  89E5              mov ebp,esp ; EBP: 7BE8
 * 0000000D  8B5508            mov edx,[ebp+0x8] ; 7BE8 + 0x8 = 7BF0: 0x2 -> EDX
 * 00000010  8B450C            mov eax,[ebp+0xc] ; 7BE8 + 0xC = 7BF4: 0x5 -> EAX
 * 00000013  01D0              add eax,edx ; EAX: 0x7
 * 00000015  5D                pop ebp ; EBP: 7BF8 (ESP: 7BEC)
 * 00000016  C3                ret ; Jmp to 7C23 (from ESP: 7BEC) -> ESP: 7BF0
 * 00000017  55                push ebp ; main() ; 0x7BF8: 0x00
 * 00000018  89E5              mov ebp,esp ; EBP: 7BF8
 * 0000001A  6A05              push byte +0x5 ; 7BF4: 0x05
 * 0000001C  6A02              push byte +0x2 ; 7BF0: 0x02
 * 0000001E  E8E7FFFFFF        call dword 0xa ; 7BEC: 7C23 (EIP: 7C1E + 5)
 * 00000023  83C408            add esp,byte +0x8 ; ESP: 7BF8
 * 00000026  C9                leave ; updates ESP with current EBP: ESP: 0x7BF0 -> 0x7BF8 and pops old EBP to EBP: 0x0000 & ESP: 0x7BFC
 * 00000027  C3                ret ; pops return address from ESP: 0x7BFC -> 7C05 ESP: 0x7C00
 */