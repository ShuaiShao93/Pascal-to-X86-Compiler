.386
.model flat, stdcall

option casemap:none

includelib E:\masm32\lib\kernel32.lib
includelib E:\masm32\lib\msvcrt.lib

include E:\masm32\include\msvcrt.inc
include E:\masm32\include\kernel32.inc

.DATA
print_int    DB "%d",0
print_char   DB "%c",0
print_string DB "%s",0
scanf_int    DB "%d",0
scanf_char   DB "%c",0
_str0   DB " Please input two letters: ",0
_str1   DB " Please input two integers: ",0
_str2   DB " The ASCII of c1 is: ",0
_str3   DB " The letter you input is out of lowercase's range! ",0
_str4   DB " The ASCII of c1 is: ",0
_str5   DB " The letter you input is out of uppercase's range! ",0
_str6   DB " The letter you input is invalid! ",0
_str7   DB " The result is: ",0

.CODE

start:  
	mov EBP, ESP
	sub ESP, 208
	push offset _str0
	push offset print_string
	call crt_printf
	add ESP, 8
	lea EAX, DWORD PTR [EBP-8]
	push EAX
	push offset scanf_char
	call crt_scanf
	add ESP, 8
	lea EAX, DWORD PTR [EBP-12]
	push EAX
	push offset scanf_char
	call crt_scanf
	add ESP, 8
	push offset _str1
	push offset print_string
	call crt_printf
	add ESP, 8
	lea EAX, DWORD PTR [EBP-16]
	push EAX
	push offset scanf_int
	call crt_scanf
	add ESP, 8
	lea EAX, DWORD PTR [EBP-20]
	push EAX
	push offset scanf_int
	call crt_scanf
	add ESP, 8
	mov EAX, DWORD PTR [EBP-8]
	mov DWORD PTR [EBP-32], EAX
	mov EAX, 97
	mov DWORD PTR [EBP-36], EAX
	mov EAX, DWORD PTR [EBP-32]
	mov EDX, DWORD PTR [EBP-36]
	cmp EAX, EDX
	jl _LABEL3
	mov EAX, DWORD PTR [EBP-8]
	mov DWORD PTR [EBP-40], EAX
	mov EAX, 122
	mov DWORD PTR [EBP-44], EAX
	mov EAX, DWORD PTR [EBP-40]
	mov EDX, DWORD PTR [EBP-44]
	cmp EAX, EDX
	jg _LABEL1
	mov EAX, DWORD PTR [EBP-8]
	mov DWORD PTR [EBP-48], EAX
	mov EAX, 97
	mov DWORD PTR [EBP-52], EAX
	mov EAX, DWORD PTR [EBP-48]
	mov EDX, DWORD PTR [EBP-52]
	sub EAX, EDX
	mov DWORD PTR [EBP-56], EAX
	mov EAX, 97
	mov DWORD PTR [EBP-60], EAX
	mov EAX, DWORD PTR [EBP-56]
	mov EDX, DWORD PTR [EBP-60]
	add EAX, EDX
	mov DWORD PTR [EBP-64], EAX
	mov EAX, DWORD PTR [EBP-64]
	mov DWORD PTR [EBP-28], EAX
	push offset _str2
	push offset print_string
	call crt_printf
	add ESP, 8
	mov EAX, DWORD PTR [EBP-28]
	mov DWORD PTR [EBP-68], EAX
	mov EAX, DWORD PTR [EBP-68]
	push EAX
	push offset print_int
	call crt_printf
	add ESP, 8
	jmp _LABEL2
_LABEL1:  
	push offset _str3
	push offset print_string
	call crt_printf
	add ESP, 8
_LABEL2:  
	jmp _LABEL8
_LABEL3:  
	mov EAX, DWORD PTR [EBP-8]
	mov DWORD PTR [EBP-72], EAX
	mov EAX, 90
	mov DWORD PTR [EBP-76], EAX
	mov EAX, DWORD PTR [EBP-72]
	mov EDX, DWORD PTR [EBP-76]
	cmp EAX, EDX
	jg _LABEL6
	mov EAX, DWORD PTR [EBP-8]
	mov DWORD PTR [EBP-80], EAX
	mov EAX, 65
	mov DWORD PTR [EBP-84], EAX
	mov EAX, DWORD PTR [EBP-80]
	mov EDX, DWORD PTR [EBP-84]
	cmp EAX, EDX
	jl _LABEL4
	mov EAX, DWORD PTR [EBP-8]
	mov DWORD PTR [EBP-88], EAX
	mov EAX, 65
	mov DWORD PTR [EBP-92], EAX
	mov EAX, DWORD PTR [EBP-88]
	mov EDX, DWORD PTR [EBP-92]
	sub EAX, EDX
	mov DWORD PTR [EBP-96], EAX
	mov EAX, 65
	mov DWORD PTR [EBP-100], EAX
	mov EAX, DWORD PTR [EBP-96]
	mov EDX, DWORD PTR [EBP-100]
	add EAX, EDX
	mov DWORD PTR [EBP-104], EAX
	mov EAX, DWORD PTR [EBP-104]
	mov DWORD PTR [EBP-28], EAX
	push offset _str4
	push offset print_string
	call crt_printf
	add ESP, 8
	mov EAX, DWORD PTR [EBP-28]
	mov DWORD PTR [EBP-108], EAX
	mov EAX, DWORD PTR [EBP-108]
	push EAX
	push offset print_int
	call crt_printf
	add ESP, 8
	jmp _LABEL5
_LABEL4:  
	push offset _str5
	push offset print_string
	call crt_printf
	add ESP, 8
_LABEL5:  
	jmp _LABEL7
_LABEL6:  
	push offset _str6
	push offset print_string
	call crt_printf
	add ESP, 8
_LABEL7:  
_LABEL8:  
	mov EAX, DWORD PTR [EBP-8]
	mov DWORD PTR [EBP-112], EAX
	mov EAX, DWORD PTR [EBP-12]
	mov DWORD PTR [EBP-116], EAX
	mov EAX, DWORD PTR [EBP-112]
	mov EDX, DWORD PTR [EBP-116]
	cmp EAX, EDX
	jg _LABEL9
	mov EAX, 97
	mov DWORD PTR [EBP-120], EAX
	mov EAX, DWORD PTR [EBP-16]
	mov DWORD PTR [EBP-124], EAX
	mov EAX, DWORD PTR [EBP-120]
	mov ECX, DWORD PTR [EBP-124]
	cdq 
	imul ECX
	mov DWORD PTR [EBP-128], EAX
	mov EAX, 109
	mov DWORD PTR [EBP-132], EAX
	mov EAX, DWORD PTR [EBP-20]
	mov DWORD PTR [EBP-136], EAX
	mov EAX, DWORD PTR [EBP-132]
	mov ECX, DWORD PTR [EBP-136]
	cdq 
	imul ECX
	mov DWORD PTR [EBP-140], EAX
	mov EAX, DWORD PTR [EBP-128]
	mov EDX, DWORD PTR [EBP-140]
	add EAX, EDX
	mov DWORD PTR [EBP-144], EAX
	mov EAX, 97
	mov DWORD PTR [EBP-148], EAX
	mov EAX, 109
	mov DWORD PTR [EBP-152], EAX
	mov EAX, DWORD PTR [EBP-148]
	mov EDX, DWORD PTR [EBP-152]
	add EAX, EDX
	mov DWORD PTR [EBP-156], EAX
	mov EAX, DWORD PTR [EBP-144]
	mov ECX, DWORD PTR [EBP-156]
	cdq 
	idiv ECX
	mov DWORD PTR [EBP-160], EAX
	mov EAX, DWORD PTR [EBP-160]
	mov DWORD PTR [EBP-24], EAX
	jmp _LABEL10
_LABEL9:  
	mov EAX, 97
	mov DWORD PTR [EBP-164], EAX
	mov EAX, DWORD PTR [EBP-20]
	mov DWORD PTR [EBP-168], EAX
	mov EAX, DWORD PTR [EBP-164]
	mov ECX, DWORD PTR [EBP-168]
	cdq 
	imul ECX
	mov DWORD PTR [EBP-172], EAX
	mov EAX, 109
	mov DWORD PTR [EBP-176], EAX
	mov EAX, DWORD PTR [EBP-16]
	mov DWORD PTR [EBP-180], EAX
	mov EAX, DWORD PTR [EBP-176]
	mov ECX, DWORD PTR [EBP-180]
	cdq 
	imul ECX
	mov DWORD PTR [EBP-184], EAX
	mov EAX, DWORD PTR [EBP-172]
	mov EDX, DWORD PTR [EBP-184]
	add EAX, EDX
	mov DWORD PTR [EBP-188], EAX
	mov EAX, 97
	mov DWORD PTR [EBP-192], EAX
	mov EAX, 109
	mov DWORD PTR [EBP-196], EAX
	mov EAX, DWORD PTR [EBP-192]
	mov EDX, DWORD PTR [EBP-196]
	sub EAX, EDX
	mov DWORD PTR [EBP-200], EAX
	mov EAX, DWORD PTR [EBP-188]
	mov ECX, DWORD PTR [EBP-200]
	cdq 
	idiv ECX
	mov DWORD PTR [EBP-204], EAX
	mov EAX, DWORD PTR [EBP-204]
	mov DWORD PTR [EBP-24], EAX
_LABEL10:  
	push offset _str7
	push offset print_string
	call crt_printf
	add ESP, 8
	mov EAX, DWORD PTR [EBP-24]
	mov DWORD PTR [EBP-208], EAX
	mov EAX, DWORD PTR [EBP-208]
	push EAX
	push offset print_int
	call crt_printf
	add ESP, 8
	mov ESP, EBP
	sub ESP, 4
	pop EAX
	ret 
END start  
