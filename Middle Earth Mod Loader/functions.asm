EXTERN functionPointer0:QWORD
EXTERN functionPointer1:QWORD
EXTERN functionPointer2:QWORD
EXTERN functionPointer3:QWORD
EXTERN functionPointer4:QWORD
EXTERN functionPointer5:QWORD
EXTERN functionPointer6:QWORD
EXTERN functionPointer7:QWORD
EXTERN functionPointer8:QWORD
EXTERN functionPointer9:QWORD
EXTERN functionPointer10:QWORD
EXTERN functionPointer11:QWORD
EXTERN functionPointer12:QWORD
EXTERN functionPointer13:QWORD
EXTERN functionPointer14:QWORD
EXTERN functionPointer15:QWORD
EXTERN functionPointer16:QWORD
EXTERN functionPointer17:QWORD
EXTERN functionPointer18:QWORD
EXTERN functionPointer19:QWORD
EXTERN functionPointer20:QWORD
EXTERN functionPointer21:QWORD
EXTERN functionPointer22:QWORD
EXTERN functionPointer23:QWORD
EXTERN functionPointer24:QWORD
EXTERN functionPointer25:QWORD
EXTERN functionPointer26:QWORD
EXTERN functionPointer27:QWORD
EXTERN functionPointer28:QWORD
EXTERN functionPointer29:QWORD
EXTERN functionPointer30:QWORD
EXTERN functionPointer31:QWORD
EXTERN functionPointer32:QWORD
EXTERN functionPointer33:QWORD
EXTERN functionPointer34:QWORD
EXTERN functionPointer35:QWORD
EXTERN functionPointer36:QWORD
EXTERN functionPointer37:QWORD
EXTERN functionPointer38:QWORD
EXTERN functionPointer39:QWORD
EXTERN functionPointer40:QWORD
EXTERN functionPointer41:QWORD
EXTERN functionPointer42:QWORD
EXTERN functionPointer43:QWORD
EXTERN functionPointer44:QWORD
EXTERN functionPointer45:QWORD
EXTERN functionPointer46:QWORD
EXTERN functionPointer47:QWORD
EXTERN functionPointer48:QWORD
EXTERN functionPointer49:QWORD
EXTERN functionPointer50:QWORD
EXTERN functionPointer51:QWORD
EXTERN functionPointer52:QWORD
EXTERN functionPointer53:QWORD
EXTERN functionPointer54:QWORD
EXTERN functionPointer55:QWORD
EXTERN functionPointer56:QWORD
EXTERN functionPointer57:QWORD
EXTERN functionPointer58:QWORD
EXTERN functionPointer59:QWORD
EXTERN functionPointer60:QWORD
EXTERN functionPointer61:QWORD
EXTERN functionPointer62:QWORD
EXTERN functionPointer63:QWORD
EXTERN functionPointer64:QWORD
EXTERN functionPointer65:QWORD
EXTERN functionPointer66:QWORD
EXTERN functionPointer67:QWORD
EXTERN functionPointer68:QWORD
EXTERN functionPointer69:QWORD
EXTERN functionPointer70:QWORD
EXTERN functionPointer71:QWORD
EXTERN functionPointer72:QWORD

EXTERN IsGamePaused:PROC
EXTERN activeWindowJmpAddr:QWORD
EXTERN origFuncAddr:QWORD

.code

ReplaceGetActiveWindow proc
    call    IsGamePaused
    cmp     rax, 1
    je      SetNullHandle

    jmp     [origFuncAddr]

    SetNullHandle:
    mov     rax, 0
    jmp     [activeWindowJmpAddr]
ReplaceGetActiveWindow endp

ProxyFunction0 proc
    jmp [functionPointer0]
ProxyFunction0 endp

ProxyFunction1 proc
    jmp [functionPointer1]
ProxyFunction1 endp

ProxyFunction2 proc
    jmp [functionPointer2]
ProxyFunction2 endp

ProxyFunction3 proc
    jmp [functionPointer3]
ProxyFunction3 endp

ProxyFunction4 proc
    jmp [functionPointer4]
ProxyFunction4 endp

ProxyFunction5 proc
    jmp [functionPointer5]
ProxyFunction5 endp

ProxyFunction6 proc
    jmp [functionPointer6]
ProxyFunction6 endp

ProxyFunction7 proc
    jmp [functionPointer7]
ProxyFunction7 endp

ProxyFunction8 proc
    jmp [functionPointer8]
ProxyFunction8 endp

ProxyFunction9 proc
    jmp [functionPointer9]
ProxyFunction9 endp

ProxyFunction10 proc
    jmp [functionPointer10]
ProxyFunction10 endp

ProxyFunction11 proc
    jmp [functionPointer11]
ProxyFunction11 endp

ProxyFunction12 proc
    jmp [functionPointer12]
ProxyFunction12 endp

ProxyFunction13 proc
    jmp [functionPointer13]
ProxyFunction13 endp

ProxyFunction14 proc
    jmp [functionPointer14]
ProxyFunction14 endp

ProxyFunction15 proc
    jmp [functionPointer15]
ProxyFunction15 endp

ProxyFunction16 proc
    jmp [functionPointer16]
ProxyFunction16 endp

ProxyFunction17 proc
    jmp [functionPointer17]
ProxyFunction17 endp

ProxyFunction18 proc
    jmp [functionPointer18]
ProxyFunction18 endp

ProxyFunction19 proc
    jmp [functionPointer19]
ProxyFunction19 endp

ProxyFunction20 proc
    jmp [functionPointer20]
ProxyFunction20 endp

ProxyFunction21 proc
    jmp [functionPointer21]
ProxyFunction21 endp

ProxyFunction22 proc
    jmp [functionPointer22]
ProxyFunction22 endp

ProxyFunction23 proc
    jmp [functionPointer23]
ProxyFunction23 endp

ProxyFunction24 proc
    jmp [functionPointer24]
ProxyFunction24 endp

ProxyFunction25 proc
    jmp [functionPointer25]
ProxyFunction25 endp

;BinkNextFrame - Load Plugins
ProxyFunction26 proc
    jmp [functionPointer26]
ProxyFunction26 endp

ProxyFunction27 proc
    jmp [functionPointer27]
ProxyFunction27 endp

ProxyFunction28 proc
    jmp [functionPointer28]
ProxyFunction28 endp

ProxyFunction29 proc
    jmp [functionPointer29]
ProxyFunction29 endp

ProxyFunction30 proc
    jmp [functionPointer30]
ProxyFunction30 endp

ProxyFunction31 proc
    jmp [functionPointer31]
ProxyFunction31 endp

ProxyFunction32 proc
    jmp [functionPointer32]
ProxyFunction32 endp

ProxyFunction33 proc
    jmp [functionPointer33]
ProxyFunction33 endp

ProxyFunction34 proc
    jmp [functionPointer34]
ProxyFunction34 endp

ProxyFunction35 proc
    jmp [functionPointer35]
ProxyFunction35 endp

ProxyFunction36 proc
    jmp [functionPointer36]
ProxyFunction36 endp

ProxyFunction37 proc
    jmp [functionPointer37]
ProxyFunction37 endp

ProxyFunction38 proc
    jmp [functionPointer38]
ProxyFunction38 endp

ProxyFunction39 proc
    jmp [functionPointer39]
ProxyFunction39 endp

ProxyFunction40 proc
    jmp [functionPointer40]
ProxyFunction40 endp

ProxyFunction41 proc
    jmp [functionPointer41]
ProxyFunction41 endp

ProxyFunction42 proc
    jmp [functionPointer42]
ProxyFunction42 endp

ProxyFunction43 proc
    jmp [functionPointer43]
ProxyFunction43 endp

ProxyFunction44 proc
    jmp [functionPointer44]
ProxyFunction44 endp

ProxyFunction45 proc
    jmp [functionPointer45]
ProxyFunction45 endp

ProxyFunction46 proc
    jmp [functionPointer46]
ProxyFunction46 endp

ProxyFunction47 proc
    jmp [functionPointer47]
ProxyFunction47 endp

ProxyFunction48 proc
    jmp [functionPointer48]
ProxyFunction48 endp

ProxyFunction49 proc
    jmp [functionPointer49]
ProxyFunction49 endp

ProxyFunction50 proc
    jmp [functionPointer50]
ProxyFunction50 endp

ProxyFunction51 proc
    jmp [functionPointer51]
ProxyFunction51 endp

ProxyFunction52 proc
    jmp [functionPointer52]
ProxyFunction52 endp

ProxyFunction53 proc
    jmp [functionPointer53]
ProxyFunction53 endp

ProxyFunction54 proc
    jmp [functionPointer54]
ProxyFunction54 endp

ProxyFunction55 proc
    jmp [functionPointer55]
ProxyFunction55 endp

ProxyFunction56 proc
    jmp [functionPointer56]
ProxyFunction56 endp

ProxyFunction57 proc
    jmp [functionPointer57]
ProxyFunction57 endp

ProxyFunction58 proc
    jmp [functionPointer58]
ProxyFunction58 endp

ProxyFunction59 proc
    jmp [functionPointer59]
ProxyFunction59 endp

ProxyFunction60 proc
    jmp [functionPointer60]
ProxyFunction60 endp

ProxyFunction61 proc
    jmp [functionPointer61]
ProxyFunction61 endp

ProxyFunction62 proc
    jmp [functionPointer62]
ProxyFunction62 endp

ProxyFunction63 proc
    jmp [functionPointer63]
ProxyFunction63 endp

ProxyFunction64 proc
    jmp [functionPointer64]
ProxyFunction64 endp

ProxyFunction65 proc
    jmp [functionPointer65]
ProxyFunction65 endp

ProxyFunction66 proc
    jmp [functionPointer66]
ProxyFunction66 endp

ProxyFunction67 proc
    jmp [functionPointer67]
ProxyFunction67 endp

ProxyFunction68 proc
    jmp [functionPointer68]
ProxyFunction68 endp

ProxyFunction69 proc
    jmp [functionPointer69]
ProxyFunction69 endp

ProxyFunction70 proc
    jmp [functionPointer70]
ProxyFunction70 endp

ProxyFunction71 proc
    jmp [functionPointer71]
ProxyFunction71 endp

ProxyFunction72 proc
    jmp [functionPointer72]
ProxyFunction72 endp

END