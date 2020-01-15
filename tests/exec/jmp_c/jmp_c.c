/* 
 * 0 + 1 + 2 + 3 + 4 + 5 + 6 + 7 + 8 + 9 + 10 = 0x37 (55)
 */
int sum(int a, int b)
{
    int sum;
    sum = 0;
    while (a <= b)
    {
        sum += a;
        a++;
    }
    return sum;
}

int main(void)
{
    return sum(1, 10);
}

/*
 * 00000000  E82B000000        call dword 0x30 ; 0 + 43(0x2B) + 5 = 48(0x30) ; ESP: 7BFC = 7C05
 * 00000005  E9F683FFFF        jmp dword 0xffff8400 ; jmp to 0 ;
 * 0000000A  55                push ebp ; sum() ; 7BE8 = 7BF8 ; <<EBP PUSH>>
 * 0000000B  89E5              mov ebp,esp ; EBP: 7BE8 ; <<EBP: 7BF8>>
 * 0000000D  83EC10            sub esp,byte +0x10 ; ESP = 7BE8 - 16 = 7BDC ;
 * 00000010  C745FC00000000    mov dword [ebp-0x4],0x0 ; sum = 0; 7BE4: 0x0 ;
 * 00000017  EB0A              jmp short 0x23 ; EIP = 0x7C17 + 2 + 0x0A(10) = 0x7C23 ;
 * 00000019  8B4508            mov eax,[ebp+0x8] ; EAX: 0x1 from EBP+0x8:7BF0
 * 0000001C  0145FC            add [ebp-0x4],eax ; EBP-0x4: 0 from (EIP:0x10) ; adds a to sum ;
 * 0000001F  83450801          add dword [ebp+0x8],byte +0x1 ; EBP+0x8:7BF0 ; adds 1 to a ;
 * 00000023  8B4508            mov eax,[ebp+0x8] ; EAX: EBP:7BE8 + 8 = 7BF0: 0x1 ; jumps here from 0x7C17 ;
 * 00000026  3B450C            cmp eax,[ebp+0xc] ; EBP:7BF0 + 12 = 7BF4: 0x10 ; EAX - 7BF4:0x10
 * 00000029  7EEE              jng 0x19 ; jump if not greater (jle) ; 0x29 + 0x2 + 0xEE(-18) = 0x19
 * 0000002B  8B45FC            mov eax,[ebp-0x4] ; EAX: result from 7BE4
 * 0000002E  C9                leave ; moves current EBP to ESP and pops old EBP to EBP ; ESP: 7BE8 -> EBP: 7BF8 from 7BE8 -> ESP: 7BEC;
 * 0000002F  C3                ret ; EIP = 7C3C from 7BEC -> ESP: 7BF0
 * 00000030  55                push ebp ; main() ; <<EBP PUSH>> ESP: 7BF8 = 0x0 ;
 * 00000031  89E5              mov ebp,esp ; <<EBP: 7BF8>>
 * 00000033  6A0A              push byte +0xa ; ESP: 7BF4 = 0xA(10)
 * 00000035  6A01              push byte +0x1 ; ESP: 7BF0 = 0x1
 * 00000037  E8CEFFFFFF        call dword 0xa ; EIP: 7C37(31799) + 5 + FFFFFFCE(-50) = 7C0A ; ESP:7BEC = 7C3C
 * 0000003C  83C408            add esp,byte +0x8 ; ESP: 7BF0 + 8 = 7BF8
 * 0000003F  C9                leave ; ESP: 7BF8 & EBP: 0x0 from 7BF8 -> ESP: 7BFC;
 * 00000040  C3                ret ; EIP: 7C05 from 7BFC -> ESP: 7c00
 */