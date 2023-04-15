	.file	"Arduino-usbserial.c"
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__SREG__ = 0x3f
__tmp_reg__ = 0
__zero_reg__ = 1
	.text
.Ltext0:
	.cfi_sections	.debug_frame
	.section	.text.RingBuffer_Remove,"ax",@progbits
	.type	RingBuffer_Remove, @function
RingBuffer_Remove:
.LVL0:
.LFB76:
	.file 1 "Lib/LightweightRingBuff.h"
	.loc 1 183 3 view -0
	.cfi_startproc
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	.loc 1 183 3 is_stmt 0 view .LVU1
	movw r18,r24
	.loc 1 184 4 is_stmt 1 view .LVU2
	.loc 1 184 34 is_stmt 0 view .LVU3
	movw r26,r24
	subi r26,126
	sbci r27,-1
	.loc 1 184 20 view .LVU4
	ld r30,X+
	ld r31,X
	sbiw r26,1
	ld r24,Z+
.LVL1:
	.loc 1 186 4 is_stmt 1 view .LVU5
	.loc 1 186 25 is_stmt 0 view .LVU6
	movw r20,r18
	subi r20,-128
	sbci r21,-1
	.loc 1 186 7 view .LVU7
	cp r30,r20
	cpc r31,r21
	breq .L2
	st X+,r30
	st X,r31
.L3:
	.loc 1 189 4 is_stmt 1 view .LVU8
.LBB34:
	.loc 1 189 4 view .LVU9
	in r20,__SREG__
.LVL2:
.LBB35:
.LBI35:
	.file 2 "/usr/local/Cellar/avr-gcc@9/9.4.0/avr/include/util/atomic.h"
	.loc 2 48 27 view .LVU10
.LBB36:
	.loc 2 50 5 view .LVU11
/* #APP */
 ;  50 "/usr/local/Cellar/avr-gcc@9/9.4.0/avr/include/util/atomic.h" 1
	cli
 ;  0 "" 2
	.loc 2 51 5 view .LVU12
.LVL3:
	.loc 2 51 5 is_stmt 0 view .LVU13
/* #NOAPP */
.LBE36:
.LBE35:
	.loc 1 189 4 is_stmt 1 view .LVU14
	.loc 1 191 5 view .LVU15
	.loc 1 191 18 is_stmt 0 view .LVU16
	movw r30,r18
	subi r30,124
	sbci r31,-1
	ld r25,Z
	subi r25,lo8(-(-1))
	st Z,r25
	.loc 1 189 4 is_stmt 1 view .LVU17
.LVL4:
	.loc 1 189 4 view .LVU18
.LBB37:
.LBI37:
	.loc 2 68 24 view .LVU19
.LBB38:
	.loc 2 70 5 view .LVU20
	.loc 2 70 10 is_stmt 0 view .LVU21
	out __SREG__,r20
	.loc 2 71 5 is_stmt 1 view .LVU22
.LVL5:
	.loc 2 71 5 is_stmt 0 view .LVU23
.LBE38:
.LBE37:
.LBE34:
	.loc 1 194 4 is_stmt 1 view .LVU24
/* epilogue start */
	.loc 1 195 3 is_stmt 0 view .LVU25
	ret
.LVL6:
.L2:
	.loc 1 187 6 is_stmt 1 view .LVU26
	.loc 1 187 18 is_stmt 0 view .LVU27
	st X+,r18
	st X,r19
	rjmp .L3
	.cfi_endproc
.LFE76:
	.size	RingBuffer_Remove, .-RingBuffer_Remove
	.section	.text.SetupHardware,"ax",@progbits
.global	SetupHardware
	.type	SetupHardware, @function
SetupHardware:
.LFB90:
	.file 3 "Arduino-usbserial.c"
	.loc 3 140 1 is_stmt 1 view -0
	.cfi_startproc
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	.loc 3 142 2 view .LVU29
	.loc 3 142 8 is_stmt 0 view .LVU30
	in r24,0x34
	andi r24,lo8(-9)
	out 0x34,r24
	.loc 3 143 2 is_stmt 1 view .LVU31
.LBB45:
.LBI45:
	.file 4 "/usr/local/Cellar/avr-gcc@9/9.4.0/avr/include/avr/wdt.h"
	.loc 4 447 6 view .LVU32
.LBE45:
	.loc 4 449 2 view .LVU33
.LBB47:
.LBB46:
	.loc 4 469 9 view .LVU34
	.loc 4 470 3 view .LVU35
/* #APP */
 ;  470 "/usr/local/Cellar/avr-gcc@9/9.4.0/avr/include/avr/wdt.h" 1
	in __tmp_reg__,__SREG__
	cli
	wdr
	lds r24,96
	ori r24,24
	sts 96,r24
	sts 96,__zero_reg__
	out __SREG__,__tmp_reg__
	
 ;  0 "" 2
/* #NOAPP */
.LBE46:
.LBE47:
	.loc 3 146 2 view .LVU36
.LBB48:
.LBI48:
	.file 5 "../../LUFA/Drivers/Peripheral/Serial.h"
	.loc 5 99 23 view .LVU37
.LVL7:
.LBB49:
	.loc 5 102 5 view .LVU38
	.loc 5 102 12 is_stmt 0 view .LVU39
	ldi r24,lo8(103)
	ldi r25,0
	sts 204+1,r25
	sts 204,r24
	.loc 5 104 5 is_stmt 1 view .LVU40
	.loc 5 104 12 is_stmt 0 view .LVU41
	ldi r24,lo8(6)
	sts 202,r24
	.loc 5 105 5 is_stmt 1 view .LVU42
	.loc 5 105 12 is_stmt 0 view .LVU43
	sts 200,__zero_reg__
	.loc 5 106 5 is_stmt 1 view .LVU44
	.loc 5 106 12 is_stmt 0 view .LVU45
	ldi r24,lo8(24)
	sts 201,r24
	.loc 5 108 5 is_stmt 1 view .LVU46
	.loc 5 108 11 is_stmt 0 view .LVU47
	sbi 0xa,3
	.loc 5 109 5 is_stmt 1 view .LVU48
	.loc 5 109 11 is_stmt 0 view .LVU49
	sbi 0xb,2
.LVL8:
	.loc 5 109 11 view .LVU50
.LBE49:
.LBE48:
	.loc 3 147 2 is_stmt 1 view .LVU51
.LBB50:
.LBI50:
	.file 6 "./Board/LEDs.h"
	.loc 6 67 23 view .LVU52
.LBB51:
	.loc 6 69 5 view .LVU53
	.loc 6 69 11 is_stmt 0 view .LVU54
	in r24,0xa
	ori r24,lo8(48)
	out 0xa,r24
	.loc 6 70 5 is_stmt 1 view .LVU55
	.loc 6 70 11 is_stmt 0 view .LVU56
	in r24,0xb
	ori r24,lo8(48)
	out 0xb,r24
.LBE51:
.LBE50:
	.loc 3 148 2 is_stmt 1 view .LVU57
	call USB_Init
.LVL9:
	.loc 3 151 2 view .LVU58
	.loc 3 151 9 is_stmt 0 view .LVU59
	ldi r24,lo8(4)
	out 0x25,r24
	.loc 3 154 2 is_stmt 1 view .LVU60
	.loc 3 154 22 is_stmt 0 view .LVU61
	sbi 0xb,7
	.loc 3 155 2 is_stmt 1 view .LVU62
	.loc 3 155 22 is_stmt 0 view .LVU63
	sbi 0xa,7
/* epilogue start */
	.loc 3 156 1 view .LVU64
	ret
	.cfi_endproc
.LFE90:
	.size	SetupHardware, .-SetupHardware
	.section	.text.startup.main,"ax",@progbits
.global	main
	.type	main, @function
main:
.LFB89:
	.loc 3 81 1 is_stmt 1 view -0
	.cfi_startproc
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	.loc 3 82 2 view .LVU66
	call SetupHardware
.LVL10:
	.loc 3 84 2 view .LVU67
.LBB110:
.LBI110:
	.loc 1 85 22 view .LVU68
.LBE110:
	.loc 1 87 4 view .LVU69
.LBB116:
.LBB111:
	.loc 1 87 4 view .LVU70
	in r18,__SREG__
.LVL11:
.LBB112:
.LBI112:
	.loc 2 48 27 view .LVU71
.LBB113:
	.loc 2 50 5 view .LVU72
/* #APP */
 ;  50 "/usr/local/Cellar/avr-gcc@9/9.4.0/avr/include/util/atomic.h" 1
	cli
 ;  0 "" 2
	.loc 2 51 5 view .LVU73
.LVL12:
	.loc 2 51 5 is_stmt 0 view .LVU74
/* #NOAPP */
.LBE113:
.LBE112:
	.loc 1 87 4 is_stmt 1 view .LVU75
	.loc 1 89 5 view .LVU76
	.loc 1 89 17 is_stmt 0 view .LVU77
	ldi r24,lo8(USBtoUSART_Buffer)
	ldi r25,hi8(USBtoUSART_Buffer)
	sts USBtoUSART_Buffer+128+1,r25
	sts USBtoUSART_Buffer+128,r24
	.loc 1 90 5 is_stmt 1 view .LVU78
	.loc 1 90 17 is_stmt 0 view .LVU79
	sts USBtoUSART_Buffer+130+1,r25
	sts USBtoUSART_Buffer+130,r24
	.loc 1 87 4 is_stmt 1 view .LVU80
.LVL13:
	.loc 1 87 4 view .LVU81
.LBB114:
.LBI114:
	.loc 2 68 24 view .LVU82
.LBB115:
	.loc 2 70 5 view .LVU83
	.loc 2 70 10 is_stmt 0 view .LVU84
	out __SREG__,r18
	.loc 2 71 5 is_stmt 1 view .LVU85
.LVL14:
	.loc 2 71 5 is_stmt 0 view .LVU86
.LBE115:
.LBE114:
.LBE111:
.LBE116:
	.loc 3 85 2 is_stmt 1 view .LVU87
.LBB117:
.LBI117:
	.loc 1 85 22 view .LVU88
.LBE117:
	.loc 1 87 4 view .LVU89
.LBB123:
.LBB118:
	.loc 1 87 4 view .LVU90
	in r18,__SREG__
.LVL15:
.LBB119:
.LBI119:
	.loc 2 48 27 view .LVU91
.LBB120:
	.loc 2 50 5 view .LVU92
/* #APP */
 ;  50 "/usr/local/Cellar/avr-gcc@9/9.4.0/avr/include/util/atomic.h" 1
	cli
 ;  0 "" 2
	.loc 2 51 5 view .LVU93
.LVL16:
	.loc 2 51 5 is_stmt 0 view .LVU94
/* #NOAPP */
.LBE120:
.LBE119:
	.loc 1 87 4 is_stmt 1 view .LVU95
	.loc 1 89 5 view .LVU96
	.loc 1 89 17 is_stmt 0 view .LVU97
	ldi r24,lo8(USARTtoUSB_Buffer)
	ldi r25,hi8(USARTtoUSB_Buffer)
	sts USARTtoUSB_Buffer+128+1,r25
	sts USARTtoUSB_Buffer+128,r24
	.loc 1 90 5 is_stmt 1 view .LVU98
	.loc 1 90 17 is_stmt 0 view .LVU99
	sts USARTtoUSB_Buffer+130+1,r25
	sts USARTtoUSB_Buffer+130,r24
	.loc 1 87 4 is_stmt 1 view .LVU100
.LVL17:
	.loc 1 87 4 view .LVU101
.LBB121:
.LBI121:
	.loc 2 68 24 view .LVU102
.LBB122:
	.loc 2 70 5 view .LVU103
	.loc 2 70 10 is_stmt 0 view .LVU104
	out __SREG__,r18
	.loc 2 71 5 is_stmt 1 view .LVU105
.LVL18:
	.loc 2 71 5 is_stmt 0 view .LVU106
.LBE122:
.LBE121:
.LBE118:
.LBE123:
	.loc 3 87 2 is_stmt 1 view .LVU107
/* #APP */
 ;  87 "Arduino-usbserial.c" 1
	sei
 ;  0 "" 2
/* #NOAPP */
.LBB124:
.LBB125:
.LBB126:
.LBB127:
.LBB128:
	.loc 1 113 11 is_stmt 0 view .LVU108
	ldi r16,lo8(USBtoUSART_Buffer)
	ldi r17,hi8(USBtoUSART_Buffer)
.LBE128:
.LBE127:
.LBE126:
.LBE125:
	.loc 3 109 33 view .LVU109
	ldi r29,lo8(3)
.LVL19:
.L22:
	.loc 3 109 33 view .LVU110
.LBE124:
	.loc 3 89 2 is_stmt 1 view .LVU111
.LBB177:
	.loc 3 92 3 view .LVU112
.LBB136:
.LBI125:
	.loc 1 127 22 view .LVU113
	.loc 1 127 22 is_stmt 0 view .LVU114
.LBE136:
.LBE177:
	.loc 1 129 4 is_stmt 1 view .LVU115
.LBB178:
.LBB137:
.LBB135:
.LBI126:
	.loc 1 107 34 view .LVU116
.LBB134:
	.loc 1 109 4 view .LVU117
	.loc 1 111 4 view .LVU118
.LBB133:
	.loc 1 111 4 view .LVU119
	in r25,__SREG__
.LVL20:
.LBB129:
.LBI129:
	.loc 2 48 27 view .LVU120
.LBB130:
	.loc 2 50 5 view .LVU121
/* #APP */
 ;  50 "/usr/local/Cellar/avr-gcc@9/9.4.0/avr/include/util/atomic.h" 1
	cli
 ;  0 "" 2
	.loc 2 51 5 view .LVU122
.LVL21:
	.loc 2 51 5 is_stmt 0 view .LVU123
/* #NOAPP */
.LBE130:
.LBE129:
	.loc 1 111 4 is_stmt 1 view .LVU124
	.loc 1 113 5 view .LVU125
	.loc 1 113 11 is_stmt 0 view .LVU126
	lds r24,USBtoUSART_Buffer+132
.LVL22:
	.loc 1 111 4 is_stmt 1 view .LVU127
	.loc 1 111 4 view .LVU128
.LBB131:
.LBI131:
	.loc 2 68 24 view .LVU129
.LBB132:
	.loc 2 70 5 view .LVU130
	.loc 2 70 10 is_stmt 0 view .LVU131
	out __SREG__,r25
	.loc 2 71 5 is_stmt 1 view .LVU132
.LVL23:
	.loc 2 71 5 is_stmt 0 view .LVU133
.LBE132:
.LBE131:
.LBE133:
	.loc 1 116 4 is_stmt 1 view .LVU134
	.loc 1 116 4 is_stmt 0 view .LVU135
.LBE134:
.LBE135:
.LBE137:
	.loc 3 92 6 view .LVU136
	cpi r24,lo8(-128)
	breq .L7
.LBB138:
	.loc 3 94 4 is_stmt 1 view .LVU137
	.loc 3 94 27 is_stmt 0 view .LVU138
	ldi r24,lo8(VirtualSerial_CDC_Interface)
	ldi r25,hi8(VirtualSerial_CDC_Interface)
	call CDC_Device_ReceiveByte
.LVL24:
	.loc 3 97 4 is_stmt 1 view .LVU139
	.loc 3 97 7 is_stmt 0 view .LVU140
	sbrc r25,7
	rjmp .L7
	.loc 3 98 6 is_stmt 1 view .LVU141
.LVL25:
.LBB139:
.LBI139:
	.loc 1 158 22 view .LVU142
.LBB140:
	.loc 1 161 4 view .LVU143
	.loc 1 161 11 is_stmt 0 view .LVU144
	lds r30,USBtoUSART_Buffer+128
	lds r31,USBtoUSART_Buffer+128+1
.LBE140:
.LBE139:
	.loc 3 98 6 view .LVU145
	st Z,r24
.LBB148:
.LBB146:
	.loc 1 163 4 is_stmt 1 view .LVU146
	.loc 1 163 8 is_stmt 0 view .LVU147
	lds r24,USBtoUSART_Buffer+128
	lds r25,USBtoUSART_Buffer+128+1
.LVL26:
	.loc 1 163 8 view .LVU148
	movw r18,r24
	subi r18,-1
	sbci r19,-1
	.loc 1 163 7 view .LVU149
	subi r24,lo8(USBtoUSART_Buffer+127)
	sbci r25,hi8(USBtoUSART_Buffer+127)
	brne .+2
	rjmp .L9
	sts USBtoUSART_Buffer+128+1,r19
	sts USBtoUSART_Buffer+128,r18
.L10:
	.loc 1 166 4 is_stmt 1 view .LVU150
.LBB141:
	.loc 1 166 4 view .LVU151
	in r25,__SREG__
.LVL27:
.LBB142:
.LBI142:
	.loc 2 48 27 view .LVU152
.LBB143:
	.loc 2 50 5 view .LVU153
/* #APP */
 ;  50 "/usr/local/Cellar/avr-gcc@9/9.4.0/avr/include/util/atomic.h" 1
	cli
 ;  0 "" 2
	.loc 2 51 5 view .LVU154
.LVL28:
	.loc 2 51 5 is_stmt 0 view .LVU155
/* #NOAPP */
.LBE143:
.LBE142:
	.loc 1 166 4 is_stmt 1 view .LVU156
	.loc 1 168 5 view .LVU157
	.loc 1 168 18 is_stmt 0 view .LVU158
	lds r24,USBtoUSART_Buffer+132
	subi r24,lo8(-(1))
	sts USBtoUSART_Buffer+132,r24
	.loc 1 166 4 is_stmt 1 view .LVU159
.LVL29:
	.loc 1 166 4 view .LVU160
.LBB144:
.LBI144:
	.loc 2 68 24 view .LVU161
.LBB145:
	.loc 2 70 5 view .LVU162
	.loc 2 70 10 is_stmt 0 view .LVU163
	out __SREG__,r25
	.loc 2 71 5 is_stmt 1 view .LVU164
.LVL30:
.L7:
	.loc 2 71 5 is_stmt 0 view .LVU165
.LBE145:
.LBE144:
.LBE141:
.LBE146:
.LBE148:
.LBE138:
	.loc 3 102 3 is_stmt 1 view .LVU166
.LBB150:
.LBI150:
	.loc 1 107 34 view .LVU167
.LBB151:
	.loc 1 109 4 view .LVU168
	.loc 1 111 4 view .LVU169
.LBB152:
	.loc 1 111 4 view .LVU170
	in r24,__SREG__
.LVL31:
.LBB153:
.LBI153:
	.loc 2 48 27 view .LVU171
.LBB154:
	.loc 2 50 5 view .LVU172
/* #APP */
 ;  50 "/usr/local/Cellar/avr-gcc@9/9.4.0/avr/include/util/atomic.h" 1
	cli
 ;  0 "" 2
	.loc 2 51 5 view .LVU173
.LVL32:
	.loc 2 51 5 is_stmt 0 view .LVU174
/* #NOAPP */
.LBE154:
.LBE153:
	.loc 1 111 4 is_stmt 1 view .LVU175
	.loc 1 113 5 view .LVU176
	.loc 1 113 11 is_stmt 0 view .LVU177
	lds r28,USARTtoUSB_Buffer+132
.LVL33:
	.loc 1 111 4 is_stmt 1 view .LVU178
	.loc 1 111 4 view .LVU179
.LBB155:
.LBI155:
	.loc 2 68 24 view .LVU180
.LBB156:
	.loc 2 70 5 view .LVU181
	.loc 2 70 10 is_stmt 0 view .LVU182
	out __SREG__,r24
	.loc 2 71 5 is_stmt 1 view .LVU183
.LVL34:
	.loc 2 71 5 is_stmt 0 view .LVU184
.LBE156:
.LBE155:
.LBE152:
	.loc 1 116 4 is_stmt 1 view .LVU185
	.loc 1 116 4 is_stmt 0 view .LVU186
.LBE151:
.LBE150:
	.loc 3 103 3 is_stmt 1 view .LVU187
	.loc 3 103 6 is_stmt 0 view .LVU188
	sbic 0x15,0
	rjmp .L11
	.loc 3 103 29 discriminator 1 view .LVU189
	cpi r28,lo8(97)
	brlo .L12
.L11:
	.loc 3 105 4 is_stmt 1 view .LVU190
	.loc 3 105 10 is_stmt 0 view .LVU191
	sbi 0x15,0
	.loc 3 107 4 is_stmt 1 view .LVU192
	.loc 3 107 7 is_stmt 0 view .LVU193
	lds r24,USARTtoUSB_Buffer+132
	tst r24
	breq .L14
	.loc 3 108 5 is_stmt 1 view .LVU194
.LVL35:
.LBB157:
.LBI157:
	.loc 6 73 23 view .LVU195
.LBB158:
	.loc 6 75 5 view .LVU196
	.loc 6 75 11 is_stmt 0 view .LVU197
	cbi 0xb,5
.LVL36:
	.loc 6 75 11 view .LVU198
.LBE158:
.LBE157:
	.loc 3 109 5 is_stmt 1 view .LVU199
	.loc 3 109 33 is_stmt 0 view .LVU200
	sts PulseMSRemaining,r29
.L14:
	.loc 3 113 10 is_stmt 1 view .LVU201
	.loc 3 113 22 is_stmt 0 view .LVU202
.LVL37:
	subi r28,1
	brcc .L15
	.loc 3 117 4 is_stmt 1 view .LVU203
	.loc 3 117 24 is_stmt 0 view .LVU204
	lds r24,PulseMSRemaining
	.loc 3 117 7 view .LVU205
	tst r24
	breq .L17
	.loc 3 117 59 discriminator 1 view .LVU206
	lds r24,PulseMSRemaining
	.loc 3 117 41 discriminator 1 view .LVU207
	subi r24,lo8(-(-1))
	.loc 3 117 36 discriminator 1 view .LVU208
	sts PulseMSRemaining,r24
	cpse r24,__zero_reg__
	rjmp .L17
	.loc 3 118 6 is_stmt 1 view .LVU209
.LVL38:
.LBB159:
.LBI159:
	.loc 6 78 23 view .LVU210
.LBB160:
	.loc 6 80 5 view .LVU211
	.loc 6 80 11 is_stmt 0 view .LVU212
	sbi 0xb,5
.LVL39:
.L17:
	.loc 6 80 11 view .LVU213
.LBE160:
.LBE159:
	.loc 3 121 4 is_stmt 1 view .LVU214
	.loc 3 121 24 is_stmt 0 view .LVU215
	lds r24,PulseMSRemaining+1
	.loc 3 121 7 view .LVU216
	tst r24
	breq .L12
	.loc 3 121 59 discriminator 1 view .LVU217
	lds r24,PulseMSRemaining+1
	.loc 3 121 41 discriminator 1 view .LVU218
	subi r24,lo8(-(-1))
	.loc 3 121 36 discriminator 1 view .LVU219
	sts PulseMSRemaining+1,r24
	cpse r24,__zero_reg__
	rjmp .L12
	.loc 3 122 6 is_stmt 1 view .LVU220
.LVL40:
.LBB161:
.LBI161:
	.loc 6 78 23 view .LVU221
.LBB162:
	.loc 6 80 5 view .LVU222
	.loc 6 80 11 is_stmt 0 view .LVU223
	sbi 0xb,4
.LVL41:
.L12:
	.loc 6 80 11 view .LVU224
.LBE162:
.LBE161:
	.loc 3 126 3 is_stmt 1 view .LVU225
.LBB163:
.LBI163:
	.loc 1 144 22 view .LVU226
	.loc 1 144 22 is_stmt 0 view .LVU227
.LBE163:
.LBE178:
	.loc 1 146 4 is_stmt 1 view .LVU228
.LBB179:
.LBB171:
.LBB164:
.LBI164:
	.loc 1 107 34 view .LVU229
.LBB165:
	.loc 1 109 4 view .LVU230
	.loc 1 111 4 view .LVU231
.LBB166:
	.loc 1 111 4 view .LVU232
	in r25,__SREG__
.LVL42:
.LBB167:
.LBI167:
	.loc 2 48 27 view .LVU233
.LBB168:
	.loc 2 50 5 view .LVU234
/* #APP */
 ;  50 "/usr/local/Cellar/avr-gcc@9/9.4.0/avr/include/util/atomic.h" 1
	cli
 ;  0 "" 2
	.loc 2 51 5 view .LVU235
.LVL43:
	.loc 2 51 5 is_stmt 0 view .LVU236
/* #NOAPP */
.LBE168:
.LBE167:
	.loc 1 111 4 is_stmt 1 view .LVU237
	.loc 1 113 5 view .LVU238
	.loc 1 113 11 is_stmt 0 view .LVU239
	lds r24,USBtoUSART_Buffer+132
.LVL44:
	.loc 1 111 4 is_stmt 1 view .LVU240
	.loc 1 111 4 view .LVU241
.LBB169:
.LBI169:
	.loc 2 68 24 view .LVU242
.LBB170:
	.loc 2 70 5 view .LVU243
	.loc 2 70 10 is_stmt 0 view .LVU244
	out __SREG__,r25
	.loc 2 71 5 is_stmt 1 view .LVU245
.LVL45:
	.loc 2 71 5 is_stmt 0 view .LVU246
.LBE170:
.LBE169:
.LBE166:
	.loc 1 116 4 is_stmt 1 view .LVU247
	.loc 1 116 4 is_stmt 0 view .LVU248
.LBE165:
.LBE164:
.LBE171:
	.loc 3 126 6 view .LVU249
	tst r24
	breq .L20
	.loc 3 127 5 is_stmt 1 view .LVU250
	.loc 3 127 19 is_stmt 0 view .LVU251
	ldi r24,lo8(USBtoUSART_Buffer)
	ldi r25,hi8(USBtoUSART_Buffer)
	call RingBuffer_Remove
.LVL46:
.LBB172:
.LBI172:
	.loc 5 140 23 is_stmt 1 view .LVU252
.LBB173:
	.loc 5 142 5 view .LVU253
.L21:
	.loc 5 142 37 view .LVU254
	.loc 5 142 11 view .LVU255
	.loc 5 142 14 is_stmt 0 view .LVU256
	lds r25,200
	.loc 5 142 11 view .LVU257
	sbrs r25,5
	rjmp .L21
	.loc 5 143 5 is_stmt 1 view .LVU258
	.loc 5 143 10 is_stmt 0 view .LVU259
	sts 206,r24
.LVL47:
	.loc 5 143 10 view .LVU260
.LBE173:
.LBE172:
	.loc 3 129 6 is_stmt 1 view .LVU261
.LBB174:
.LBI174:
	.loc 6 73 23 view .LVU262
.LBB175:
	.loc 6 75 5 view .LVU263
	.loc 6 75 11 is_stmt 0 view .LVU264
	cbi 0xb,4
.LVL48:
	.loc 6 75 11 view .LVU265
.LBE175:
.LBE174:
	.loc 3 130 4 is_stmt 1 view .LVU266
	.loc 3 130 32 is_stmt 0 view .LVU267
	sts PulseMSRemaining+1,r29
.L20:
	.loc 3 133 3 is_stmt 1 view .LVU268
	ldi r24,lo8(VirtualSerial_CDC_Interface)
	ldi r25,hi8(VirtualSerial_CDC_Interface)
	call CDC_Device_USBTask
.LVL49:
	.loc 3 134 3 view .LVU269
	call USB_USBTask
.LVL50:
.LBE179:
	.loc 3 89 8 view .LVU270
	.loc 3 90 2 is_stmt 0 view .LVU271
	rjmp .L22
.LVL51:
.L9:
.LBB180:
.LBB176:
.LBB149:
.LBB147:
	.loc 1 164 6 is_stmt 1 view .LVU272
	.loc 1 164 17 is_stmt 0 view .LVU273
	sts USBtoUSART_Buffer+128+1,r17
	sts USBtoUSART_Buffer+128,r16
	rjmp .L10
.LVL52:
.L15:
	.loc 1 164 17 view .LVU274
.LBE147:
.LBE149:
.LBE176:
	.loc 3 114 6 is_stmt 1 view .LVU275
	ldi r24,lo8(USARTtoUSB_Buffer)
	ldi r25,hi8(USARTtoUSB_Buffer)
	call RingBuffer_Remove
.LVL53:
	mov r22,r24
	ldi r24,lo8(VirtualSerial_CDC_Interface)
	ldi r25,hi8(VirtualSerial_CDC_Interface)
	call CDC_Device_SendByte
.LVL54:
	rjmp .L14
.LBE180:
	.cfi_endproc
.LFE89:
	.size	main, .-main
	.section	.text.EVENT_USB_Device_ConfigurationChanged,"ax",@progbits
.global	EVENT_USB_Device_ConfigurationChanged
	.type	EVENT_USB_Device_ConfigurationChanged, @function
EVENT_USB_Device_ConfigurationChanged:
.LFB91:
	.loc 3 160 1 view -0
	.cfi_startproc
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	.loc 3 161 2 view .LVU277
	ldi r24,lo8(VirtualSerial_CDC_Interface)
	ldi r25,hi8(VirtualSerial_CDC_Interface)
	jmp CDC_Device_ConfigureEndpoints
.LVL55:
	.cfi_endproc
.LFE91:
	.size	EVENT_USB_Device_ConfigurationChanged, .-EVENT_USB_Device_ConfigurationChanged
	.section	.text.EVENT_USB_Device_UnhandledControlRequest,"ax",@progbits
.global	EVENT_USB_Device_UnhandledControlRequest
	.type	EVENT_USB_Device_UnhandledControlRequest, @function
EVENT_USB_Device_UnhandledControlRequest:
.LFB92:
	.loc 3 166 1 view -0
	.cfi_startproc
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	.loc 3 167 2 view .LVU279
	ldi r24,lo8(VirtualSerial_CDC_Interface)
	ldi r25,hi8(VirtualSerial_CDC_Interface)
	jmp CDC_Device_ProcessControlRequest
.LVL56:
	.cfi_endproc
.LFE92:
	.size	EVENT_USB_Device_UnhandledControlRequest, .-EVENT_USB_Device_UnhandledControlRequest
	.section	.text.EVENT_CDC_Device_LineEncodingChanged,"ax",@progbits
.global	EVENT_CDC_Device_LineEncodingChanged
	.type	EVENT_CDC_Device_LineEncodingChanged, @function
EVENT_CDC_Device_LineEncodingChanged:
.LVL57:
.LFB93:
	.loc 3 175 1 view -0
	.cfi_startproc
	.loc 3 175 1 is_stmt 0 view .LVU281
	push r8
.LCFI0:
	.cfi_def_cfa_offset 3
	.cfi_offset 8, -2
	push r9
.LCFI1:
	.cfi_def_cfa_offset 4
	.cfi_offset 9, -3
	push r10
.LCFI2:
	.cfi_def_cfa_offset 5
	.cfi_offset 10, -4
	push r11
.LCFI3:
	.cfi_def_cfa_offset 6
	.cfi_offset 11, -5
	push r15
.LCFI4:
	.cfi_def_cfa_offset 7
	.cfi_offset 15, -6
	push r16
.LCFI5:
	.cfi_def_cfa_offset 8
	.cfi_offset 16, -7
	push r17
.LCFI6:
	.cfi_def_cfa_offset 9
	.cfi_offset 17, -8
	push r28
.LCFI7:
	.cfi_def_cfa_offset 10
	.cfi_offset 28, -9
	push r29
.LCFI8:
	.cfi_def_cfa_offset 11
	.cfi_offset 29, -10
	rcall .
	rcall .
	push __tmp_reg__
.LCFI9:
	.cfi_def_cfa_offset 16
	in r28,__SP_L__
	in r29,__SP_H__
.LCFI10:
	.cfi_def_cfa_register 28
/* prologue: function */
/* frame size = 5 */
/* stack size = 14 */
.L__stack_usage = 14
	movw r16,r24
	.loc 3 176 2 is_stmt 1 view .LVU282
.LVL58:
	.loc 3 178 2 view .LVU283
	.loc 3 178 46 is_stmt 0 view .LVU284
	movw r26,r24
	adiw r26,20
	ld r24,X
.LVL59:
	.loc 3 178 2 view .LVU285
	ldi r25,lo8(48)
	mov r15,r25
	cpi r24,lo8(1)
	breq .L37
	.loc 3 184 15 view .LVU286
	ldi r18,lo8(32)
	mov r15,r18
	.loc 3 178 2 view .LVU287
	cpi r24,lo8(2)
	breq .L37
	.loc 3 176 10 view .LVU288
	mov r15,__zero_reg__
.L37:
.LVL60:
	.loc 3 188 2 is_stmt 1 view .LVU289
	.loc 3 188 5 is_stmt 0 view .LVU290
	movw r30,r16
	ldd r24,Z+19
	cpi r24,lo8(2)
	brne .L38
	.loc 3 189 4 is_stmt 1 view .LVU291
	.loc 3 189 15 is_stmt 0 view .LVU292
	set
	bld r15,3
.LVL61:
.L38:
	.loc 3 191 2 is_stmt 1 view .LVU293
	.loc 3 191 46 is_stmt 0 view .LVU294
	movw r26,r16
	adiw r26,21
	ld r24,X
	.loc 3 191 2 view .LVU295
	cpi r24,lo8(7)
	brne .+2
	rjmp .L39
	.loc 3 191 2 view .LVU296
	cpi r24,lo8(8)
	brne .+2
	rjmp .L40
	cpi r24,lo8(6)
	brne .L41
	.loc 3 194 4 is_stmt 1 view .LVU297
	.loc 3 194 15 is_stmt 0 view .LVU298
	set
	bld r15,1
.LVL62:
	.loc 3 195 4 is_stmt 1 view .LVU299
.L41:
	.loc 3 205 2 view .LVU300
	.loc 3 205 9 is_stmt 0 view .LVU301
	sts 201,__zero_reg__
	.loc 3 206 2 is_stmt 1 view .LVU302
	.loc 3 206 9 is_stmt 0 view .LVU303
	sts 200,__zero_reg__
	.loc 3 207 2 is_stmt 1 view .LVU304
	.loc 3 207 9 is_stmt 0 view .LVU305
	sts 202,__zero_reg__
	.loc 3 210 2 is_stmt 1 view .LVU306
	.loc 3 210 48 is_stmt 0 view .LVU307
	movw r30,r16
	ldd r18,Z+15
	ldd r19,Z+16
	ldd r20,Z+17
	ldd r21,Z+18
	.loc 3 210 9 view .LVU308
	cp r18,__zero_reg__
	ldi r31,-31
	cpc r19,r31
	cpc r20,__zero_reg__
	cpc r21,__zero_reg__
	brne .+2
	rjmp .L46
	.loc 3 212 7 discriminator 2 view .LVU309
	movw r8,r18
	movw r10,r20
	lsr r11
	ror r10
	ror r9
	ror r8
	movw r26,r10
	movw r24,r8
	subi r24,-128
	sbci r25,123
	sbci r26,-31
	sbci r27,-1
	std Y+1,r24
	std Y+2,r25
	std Y+3,r26
	std Y+4,r27
	movw r22,r24
	movw r24,r26
	call __udivmodsi4
	.loc 3 210 9 discriminator 2 view .LVU310
	subi r18,1
	sbc r19,__zero_reg__
.L42:
	.loc 3 210 9 discriminator 4 view .LVU311
	sts 204+1,r19
	sts 204,r18
	.loc 3 214 2 is_stmt 1 discriminator 4 view .LVU312
	.loc 3 214 9 is_stmt 0 discriminator 4 view .LVU313
	sts 202,r15
	.loc 3 215 2 is_stmt 1 discriminator 4 view .LVU314
	.loc 3 215 9 is_stmt 0 discriminator 4 view .LVU315
	movw r30,r16
	ldd r24,Z+15
	ldd r25,Z+16
	ldd r26,Z+17
	ldd r27,Z+18
	cp r24,__zero_reg__
	sbci r25,-31
	cpc r26,__zero_reg__
	cpc r27,__zero_reg__
	breq .L47
	.loc 3 215 9 view .LVU316
	ldi r24,lo8(2)
.L43:
	.loc 3 215 9 discriminator 4 view .LVU317
	sts 200,r24
	.loc 3 216 2 is_stmt 1 discriminator 4 view .LVU318
	.loc 3 216 9 is_stmt 0 discriminator 4 view .LVU319
	ldi r24,lo8(-104)
	sts 201,r24
/* epilogue start */
	.loc 3 217 1 discriminator 4 view .LVU320
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop r29
	pop r28
	pop r17
	pop r16
.LVL63:
	.loc 3 217 1 discriminator 4 view .LVU321
	pop r15
.LVL64:
	.loc 3 217 1 discriminator 4 view .LVU322
	pop r11
	pop r10
	pop r9
	pop r8
	ret
.LVL65:
.L39:
	.loc 3 197 4 is_stmt 1 view .LVU323
	.loc 3 197 15 is_stmt 0 view .LVU324
	set
	bld r15,2
.LVL66:
	.loc 3 198 4 is_stmt 1 view .LVU325
	rjmp .L41
.L40:
	.loc 3 200 4 view .LVU326
	.loc 3 200 15 is_stmt 0 view .LVU327
	mov r27,r15
	ori r27,lo8(6)
	mov r15,r27
.LVL67:
	.loc 3 201 4 is_stmt 1 view .LVU328
	rjmp .L41
.L46:
	.loc 3 210 9 is_stmt 0 view .LVU329
	ldi r18,lo8(16)
	ldi r19,0
	rjmp .L42
.L47:
	.loc 3 215 9 view .LVU330
	ldi r24,0
	rjmp .L43
	.cfi_endproc
.LFE93:
	.size	EVENT_CDC_Device_LineEncodingChanged, .-EVENT_CDC_Device_LineEncodingChanged
	.section	.text.__vector_23,"ax",@progbits
.global	__vector_23
	.type	__vector_23, @function
__vector_23:
.LFB94:
	.loc 3 223 1 is_stmt 1 view -0
	.cfi_startproc
	__gcc_isr 1
	push r24
.LCFI11:
	.cfi_def_cfa_offset 3
	.cfi_offset 24, -2
	push r25
.LCFI12:
	.cfi_def_cfa_offset 4
	.cfi_offset 25, -3
	push r30
.LCFI13:
	.cfi_def_cfa_offset 5
	.cfi_offset 30, -4
	push r31
.LCFI14:
	.cfi_def_cfa_offset 6
	.cfi_offset 31, -5
/* prologue: Signal */
/* frame size = 0 */
/* stack size = 4...8 */
.L__stack_usage = 4 + __gcc_isr.n_pushed
	.loc 3 224 2 view .LVU332
	.loc 3 224 10 is_stmt 0 view .LVU333
	lds r24,206
.LVL68:
	.loc 3 226 2 is_stmt 1 view .LVU334
	.loc 3 226 6 is_stmt 0 view .LVU335
	in r25,0x1e
	.loc 3 226 5 view .LVU336
	cpi r25,lo8(4)
	brne .L48
	.loc 3 227 4 is_stmt 1 view .LVU337
.LVL69:
.LBB188:
.LBI188:
	.loc 1 158 22 view .LVU338
.LBB189:
	.loc 1 161 4 view .LVU339
	.loc 1 161 11 is_stmt 0 view .LVU340
	lds r30,USARTtoUSB_Buffer+128
	lds r31,USARTtoUSB_Buffer+128+1
	.loc 1 161 16 view .LVU341
	st Z,r24
	.loc 1 163 4 is_stmt 1 view .LVU342
	.loc 1 163 8 is_stmt 0 view .LVU343
	lds r30,USARTtoUSB_Buffer+128
	lds r31,USARTtoUSB_Buffer+128+1
	movw r24,r30
.LVL70:
	.loc 1 163 8 view .LVU344
	adiw r24,1
	.loc 1 163 7 view .LVU345
	ldi r18,hi8(USARTtoUSB_Buffer+127)
	cpi r30,lo8(USARTtoUSB_Buffer+127)
	cpc r31,r18
	breq .L50
	sts USARTtoUSB_Buffer+128+1,r25
	sts USARTtoUSB_Buffer+128,r24
.L51:
	.loc 1 166 4 is_stmt 1 view .LVU346
.LBB190:
	.loc 1 166 4 view .LVU347
	in r25,__SREG__
.LVL71:
.LBB191:
.LBI191:
	.loc 2 48 27 view .LVU348
.LBB192:
	.loc 2 50 5 view .LVU349
/* #APP */
 ;  50 "/usr/local/Cellar/avr-gcc@9/9.4.0/avr/include/util/atomic.h" 1
	cli
 ;  0 "" 2
	.loc 2 51 5 view .LVU350
.LVL72:
	.loc 2 51 5 is_stmt 0 view .LVU351
/* #NOAPP */
.LBE192:
.LBE191:
	.loc 1 166 4 is_stmt 1 view .LVU352
	.loc 1 168 5 view .LVU353
	.loc 1 168 18 is_stmt 0 view .LVU354
	lds r24,USARTtoUSB_Buffer+132
	subi r24,lo8(-(1))
	sts USARTtoUSB_Buffer+132,r24
	.loc 1 166 4 is_stmt 1 view .LVU355
.LVL73:
	.loc 1 166 4 view .LVU356
.LBB193:
.LBI193:
	.loc 2 68 24 view .LVU357
.LBB194:
	.loc 2 70 5 view .LVU358
	.loc 2 70 10 is_stmt 0 view .LVU359
	out __SREG__,r25
	.loc 2 71 5 is_stmt 1 view .LVU360
.LVL74:
.L48:
/* epilogue start */
	.loc 2 71 5 is_stmt 0 view .LVU361
.LBE194:
.LBE193:
.LBE190:
.LBE189:
.LBE188:
	.loc 3 228 1 view .LVU362
	pop r31
	pop r30
	pop r25
	pop r24
	__gcc_isr 2
	reti
.LVL75:
.L50:
.LBB196:
.LBB195:
	.loc 1 164 6 is_stmt 1 view .LVU363
	.loc 1 164 17 is_stmt 0 view .LVU364
	ldi r24,lo8(USARTtoUSB_Buffer)
	ldi r25,hi8(USARTtoUSB_Buffer)
	std Z+2,r25
	std Z+1,r24
	.loc 1 164 17 view .LVU365
	rjmp .L51
	__gcc_isr 0,r18
.LBE195:
.LBE196:
	.cfi_endproc
.LFE94:
	.size	__vector_23, .-__vector_23
	.section	.text.EVENT_CDC_Device_ControLineStateChanged,"ax",@progbits
.global	EVENT_CDC_Device_ControLineStateChanged
	.type	EVENT_CDC_Device_ControLineStateChanged, @function
EVENT_CDC_Device_ControLineStateChanged:
.LVL76:
.LFB95:
	.loc 3 235 1 is_stmt 1 view -0
	.cfi_startproc
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	.loc 3 236 2 view .LVU367
	.loc 3 238 2 view .LVU368
	.loc 3 238 5 is_stmt 0 view .LVU369
	movw r30,r24
	ldd r24,Z+13
.LVL77:
	.loc 3 238 5 view .LVU370
	sbrs r24,0
	rjmp .L53
	.loc 3 239 4 is_stmt 1 view .LVU371
	.loc 3 239 24 is_stmt 0 view .LVU372
	cbi 0xb,7
	ret
.L53:
	.loc 3 241 4 is_stmt 1 view .LVU373
	.loc 3 241 24 is_stmt 0 view .LVU374
	sbi 0xb,7
/* epilogue start */
	.loc 3 242 1 view .LVU375
	ret
	.cfi_endproc
.LFE95:
	.size	EVENT_CDC_Device_ControLineStateChanged, .-EVENT_CDC_Device_ControLineStateChanged
.global	VirtualSerial_CDC_Interface
	.data
	.type	VirtualSerial_CDC_Interface, @object
	.size	VirtualSerial_CDC_Interface, 22
VirtualSerial_CDC_Interface:
	.byte	0
	.byte	3
	.word	64
	.byte	0
	.byte	4
	.word	64
	.byte	0
	.byte	2
	.word	8
	.byte	0
	.zero	9
	.comm	PulseMSRemaining,3,1
	.comm	USARTtoUSB_Buffer,133,1
	.comm	USBtoUSART_Buffer,133,1
	.text
.Letext0:
	.file 7 "/usr/local/Cellar/avr-gcc@9/9.4.0/avr/include/stdint.h"
	.file 8 "../../LUFA/Drivers/USB/HighLevel/../LowLevel/../HighLevel/StdRequestType.h"
	.file 9 "../../LUFA/Drivers/USB/HighLevel/../LowLevel/../HighLevel/DeviceStandardReq.h"
	.file 10 "../../LUFA/Drivers/USB/HighLevel/USBTask.h"
	.file 11 "../../LUFA/Drivers/USB/HighLevel/../LowLevel/Device.h"
	.file 12 "../../LUFA/Drivers/USB/Class/Device/../Common/CDC.h"
	.file 13 "/usr/local/Cellar/avr-gcc@9/9.4.0/avr/include/stdio.h"
	.file 14 "../../LUFA/Drivers/USB/Class/Device/CDC.h"
	.file 15 "../../LUFA/Drivers/USB/HighLevel/../LowLevel/USBController.h"
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.long	0xf72
	.word	0x2
	.long	.Ldebug_abbrev0
	.byte	0x4
	.uleb128 0x1
	.long	.LASF114
	.byte	0xc
	.long	.LASF115
	.long	.LASF116
	.long	.Ldebug_ranges0+0x118
	.long	0
	.long	0
	.long	.Ldebug_line0
	.uleb128 0x2
	.byte	0x1
	.byte	0x6
	.long	.LASF0
	.uleb128 0x3
	.long	.LASF2
	.byte	0x7
	.byte	0x7e
	.byte	0x16
	.long	0x41
	.uleb128 0x4
	.long	0x30
	.uleb128 0x2
	.byte	0x1
	.byte	0x8
	.long	.LASF1
	.uleb128 0x3
	.long	.LASF3
	.byte	0x7
	.byte	0x7f
	.byte	0x14
	.long	0x54
	.uleb128 0x5
	.byte	0x2
	.byte	0x5
	.string	"int"
	.uleb128 0x3
	.long	.LASF4
	.byte	0x7
	.byte	0x80
	.byte	0x16
	.long	0x67
	.uleb128 0x2
	.byte	0x2
	.byte	0x7
	.long	.LASF5
	.uleb128 0x2
	.byte	0x4
	.byte	0x5
	.long	.LASF6
	.uleb128 0x3
	.long	.LASF7
	.byte	0x7
	.byte	0x82
	.byte	0x16
	.long	0x86
	.uleb128 0x4
	.long	0x75
	.uleb128 0x2
	.byte	0x4
	.byte	0x7
	.long	.LASF8
	.uleb128 0x2
	.byte	0x8
	.byte	0x5
	.long	.LASF9
	.uleb128 0x2
	.byte	0x8
	.byte	0x7
	.long	.LASF10
	.uleb128 0x6
	.long	.LASF29
	.byte	0x7
	.byte	0x1
	.long	0x41
	.byte	0xb
	.byte	0x72
	.byte	0x9
	.long	0xd2
	.uleb128 0x7
	.long	.LASF11
	.byte	0
	.uleb128 0x7
	.long	.LASF12
	.byte	0x1
	.uleb128 0x7
	.long	.LASF13
	.byte	0x2
	.uleb128 0x7
	.long	.LASF14
	.byte	0x3
	.uleb128 0x7
	.long	.LASF15
	.byte	0x4
	.uleb128 0x7
	.long	.LASF16
	.byte	0x5
	.byte	0
	.uleb128 0x8
	.byte	0x8
	.byte	0x8
	.byte	0xa2
	.byte	0xc
	.long	0x127
	.uleb128 0x9
	.long	.LASF17
	.byte	0x8
	.byte	0xa4
	.byte	0xe
	.long	0x30
	.byte	0x2
	.byte	0x23
	.uleb128 0
	.uleb128 0x9
	.long	.LASF18
	.byte	0x8
	.byte	0xa5
	.byte	0xe
	.long	0x30
	.byte	0x2
	.byte	0x23
	.uleb128 0x1
	.uleb128 0x9
	.long	.LASF19
	.byte	0x8
	.byte	0xa6
	.byte	0xe
	.long	0x5b
	.byte	0x2
	.byte	0x23
	.uleb128 0x2
	.uleb128 0x9
	.long	.LASF20
	.byte	0x8
	.byte	0xa7
	.byte	0xe
	.long	0x5b
	.byte	0x2
	.byte	0x23
	.uleb128 0x4
	.uleb128 0x9
	.long	.LASF21
	.byte	0x8
	.byte	0xa8
	.byte	0xe
	.long	0x5b
	.byte	0x2
	.byte	0x23
	.uleb128 0x6
	.byte	0
	.uleb128 0x3
	.long	.LASF22
	.byte	0x8
	.byte	0xa9
	.byte	0x6
	.long	0xd2
	.uleb128 0xa
	.long	.LASF23
	.byte	0x9
	.byte	0x64
	.byte	0x13
	.long	0x30
	.byte	0x1
	.byte	0x1
	.uleb128 0xa
	.long	.LASF24
	.byte	0x9
	.byte	0x75
	.byte	0x11
	.long	0x14f
	.byte	0x1
	.byte	0x1
	.uleb128 0x2
	.byte	0x1
	.byte	0x2
	.long	.LASF25
	.uleb128 0xb
	.long	0x14f
	.uleb128 0x4
	.long	0x14f
	.uleb128 0xa
	.long	.LASF26
	.byte	0x9
	.byte	0x7f
	.byte	0x11
	.long	0x14f
	.byte	0x1
	.byte	0x1
	.uleb128 0xa
	.long	.LASF27
	.byte	0xa
	.byte	0x53
	.byte	0x19
	.long	0x156
	.byte	0x1
	.byte	0x1
	.uleb128 0xa
	.long	.LASF28
	.byte	0xa
	.byte	0x5b
	.byte	0x21
	.long	0x127
	.byte	0x1
	.byte	0x1
	.uleb128 0xc
	.byte	0x2
	.uleb128 0x6
	.long	.LASF30
	.byte	0x7
	.byte	0x1
	.long	0x41
	.byte	0xc
	.byte	0x9b
	.byte	0x8
	.long	0x1b1
	.uleb128 0x7
	.long	.LASF31
	.byte	0
	.uleb128 0x7
	.long	.LASF32
	.byte	0x1
	.uleb128 0x7
	.long	.LASF33
	.byte	0x2
	.byte	0
	.uleb128 0x6
	.long	.LASF34
	.byte	0x7
	.byte	0x1
	.long	0x41
	.byte	0xc
	.byte	0xa3
	.byte	0x8
	.long	0x1e2
	.uleb128 0x7
	.long	.LASF35
	.byte	0
	.uleb128 0x7
	.long	.LASF36
	.byte	0x1
	.uleb128 0x7
	.long	.LASF37
	.byte	0x2
	.uleb128 0x7
	.long	.LASF38
	.byte	0x3
	.uleb128 0x7
	.long	.LASF39
	.byte	0x4
	.byte	0
	.uleb128 0xd
	.long	.LASF117
	.byte	0xe
	.byte	0xd
	.byte	0xf4
	.byte	0x8
	.long	0x26d
	.uleb128 0xe
	.string	"buf"
	.byte	0xd
	.byte	0xf5
	.byte	0x8
	.long	0x26d
	.byte	0x2
	.byte	0x23
	.uleb128 0
	.uleb128 0x9
	.long	.LASF40
	.byte	0xd
	.byte	0xf6
	.byte	0x10
	.long	0x41
	.byte	0x2
	.byte	0x23
	.uleb128 0x2
	.uleb128 0x9
	.long	.LASF41
	.byte	0xd
	.byte	0xf7
	.byte	0xa
	.long	0x30
	.byte	0x2
	.byte	0x23
	.uleb128 0x3
	.uleb128 0xf
	.long	.LASF42
	.byte	0xd
	.word	0x107
	.byte	0x6
	.long	0x54
	.byte	0x2
	.byte	0x23
	.uleb128 0x4
	.uleb128 0x10
	.string	"len"
	.byte	0xd
	.word	0x108
	.byte	0x6
	.long	0x54
	.byte	0x2
	.byte	0x23
	.uleb128 0x6
	.uleb128 0x10
	.string	"put"
	.byte	0xd
	.word	0x109
	.byte	0x8
	.long	0x29a
	.byte	0x2
	.byte	0x23
	.uleb128 0x8
	.uleb128 0x10
	.string	"get"
	.byte	0xd
	.word	0x10a
	.byte	0x8
	.long	0x2b0
	.byte	0x2
	.byte	0x23
	.uleb128 0xa
	.uleb128 0xf
	.long	.LASF43
	.byte	0xd
	.word	0x10b
	.byte	0x8
	.long	0x18a
	.byte	0x2
	.byte	0x23
	.uleb128 0xc
	.byte	0
	.uleb128 0x11
	.byte	0x2
	.long	0x273
	.uleb128 0x2
	.byte	0x1
	.byte	0x8
	.long	.LASF44
	.uleb128 0x4
	.long	0x273
	.uleb128 0x12
	.byte	0x1
	.long	0x54
	.long	0x294
	.uleb128 0x13
	.long	0x273
	.uleb128 0x13
	.long	0x294
	.byte	0
	.uleb128 0x11
	.byte	0x2
	.long	0x1e2
	.uleb128 0x11
	.byte	0x2
	.long	0x27f
	.uleb128 0x12
	.byte	0x1
	.long	0x54
	.long	0x2b0
	.uleb128 0x13
	.long	0x294
	.byte	0
	.uleb128 0x11
	.byte	0x2
	.long	0x2a0
	.uleb128 0x14
	.long	0x294
	.long	0x2c1
	.uleb128 0x15
	.byte	0
	.uleb128 0x16
	.long	.LASF45
	.byte	0xd
	.word	0x197
	.byte	0x17
	.long	0x2b6
	.byte	0x1
	.byte	0x1
	.uleb128 0x8
	.byte	0xd
	.byte	0xe
	.byte	0x64
	.byte	0xb
	.long	0x370
	.uleb128 0x9
	.long	.LASF46
	.byte	0xe
	.byte	0x66
	.byte	0xf
	.long	0x30
	.byte	0x2
	.byte	0x23
	.uleb128 0
	.uleb128 0x9
	.long	.LASF47
	.byte	0xe
	.byte	0x68
	.byte	0xf
	.long	0x30
	.byte	0x2
	.byte	0x23
	.uleb128 0x1
	.uleb128 0x9
	.long	.LASF48
	.byte	0xe
	.byte	0x69
	.byte	0xf
	.long	0x5b
	.byte	0x2
	.byte	0x23
	.uleb128 0x2
	.uleb128 0x9
	.long	.LASF49
	.byte	0xe
	.byte	0x6a
	.byte	0xf
	.long	0x14f
	.byte	0x2
	.byte	0x23
	.uleb128 0x4
	.uleb128 0x9
	.long	.LASF50
	.byte	0xe
	.byte	0x6c
	.byte	0xf
	.long	0x30
	.byte	0x2
	.byte	0x23
	.uleb128 0x5
	.uleb128 0x9
	.long	.LASF51
	.byte	0xe
	.byte	0x6d
	.byte	0xf
	.long	0x5b
	.byte	0x2
	.byte	0x23
	.uleb128 0x6
	.uleb128 0x9
	.long	.LASF52
	.byte	0xe
	.byte	0x6e
	.byte	0xf
	.long	0x14f
	.byte	0x2
	.byte	0x23
	.uleb128 0x8
	.uleb128 0x9
	.long	.LASF53
	.byte	0xe
	.byte	0x70
	.byte	0xf
	.long	0x30
	.byte	0x2
	.byte	0x23
	.uleb128 0x9
	.uleb128 0x9
	.long	.LASF54
	.byte	0xe
	.byte	0x71
	.byte	0xf
	.long	0x5b
	.byte	0x2
	.byte	0x23
	.uleb128 0xa
	.uleb128 0x9
	.long	.LASF55
	.byte	0xe
	.byte	0x72
	.byte	0xf
	.long	0x14f
	.byte	0x2
	.byte	0x23
	.uleb128 0xc
	.byte	0
	.uleb128 0x4
	.long	0x2d0
	.uleb128 0x8
	.byte	0x2
	.byte	0xe
	.byte	0x78
	.byte	0x6
	.long	0x39d
	.uleb128 0x9
	.long	.LASF56
	.byte	0xe
	.byte	0x7a
	.byte	0xf
	.long	0x30
	.byte	0x2
	.byte	0x23
	.uleb128 0
	.uleb128 0x9
	.long	.LASF57
	.byte	0xe
	.byte	0x7d
	.byte	0xf
	.long	0x30
	.byte	0x2
	.byte	0x23
	.uleb128 0x1
	.byte	0
	.uleb128 0x8
	.byte	0x7
	.byte	0xe
	.byte	0x83
	.byte	0x6
	.long	0x3e3
	.uleb128 0x9
	.long	.LASF58
	.byte	0xe
	.byte	0x85
	.byte	0x10
	.long	0x75
	.byte	0x2
	.byte	0x23
	.uleb128 0
	.uleb128 0x9
	.long	.LASF59
	.byte	0xe
	.byte	0x86
	.byte	0x10
	.long	0x30
	.byte	0x2
	.byte	0x23
	.uleb128 0x4
	.uleb128 0x9
	.long	.LASF60
	.byte	0xe
	.byte	0x89
	.byte	0x10
	.long	0x30
	.byte	0x2
	.byte	0x23
	.uleb128 0x5
	.uleb128 0x9
	.long	.LASF61
	.byte	0xe
	.byte	0x8c
	.byte	0x10
	.long	0x30
	.byte	0x2
	.byte	0x23
	.uleb128 0x6
	.byte	0
	.uleb128 0x8
	.byte	0x9
	.byte	0xe
	.byte	0x76
	.byte	0x5
	.long	0x40b
	.uleb128 0x9
	.long	.LASF62
	.byte	0xe
	.byte	0x81
	.byte	0x8
	.long	0x375
	.byte	0x2
	.byte	0x23
	.uleb128 0
	.uleb128 0x9
	.long	.LASF63
	.byte	0xe
	.byte	0x8d
	.byte	0x8
	.long	0x39d
	.byte	0x2
	.byte	0x23
	.uleb128 0x2
	.byte	0
	.uleb128 0x8
	.byte	0x16
	.byte	0xe
	.byte	0x62
	.byte	0xc
	.long	0x433
	.uleb128 0x9
	.long	.LASF64
	.byte	0xe
	.byte	0x73
	.byte	0x7
	.long	0x370
	.byte	0x2
	.byte	0x23
	.uleb128 0
	.uleb128 0x9
	.long	.LASF65
	.byte	0xe
	.byte	0x90
	.byte	0x7
	.long	0x3e3
	.byte	0x2
	.byte	0x23
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.long	.LASF66
	.byte	0xe
	.byte	0x93
	.byte	0x6
	.long	0x40b
	.uleb128 0x8
	.byte	0x85
	.byte	0x1
	.byte	0x46
	.byte	0xb
	.long	0x487
	.uleb128 0x9
	.long	.LASF67
	.byte	0x1
	.byte	0x48
	.byte	0x15
	.long	0x487
	.byte	0x2
	.byte	0x23
	.uleb128 0
	.uleb128 0xe
	.string	"In"
	.byte	0x1
	.byte	0x49
	.byte	0x15
	.long	0x497
	.byte	0x3
	.byte	0x23
	.uleb128 0x80
	.uleb128 0xe
	.string	"Out"
	.byte	0x1
	.byte	0x4a
	.byte	0x15
	.long	0x497
	.byte	0x3
	.byte	0x23
	.uleb128 0x82
	.uleb128 0x9
	.long	.LASF68
	.byte	0x1
	.byte	0x4b
	.byte	0x15
	.long	0x30
	.byte	0x3
	.byte	0x23
	.uleb128 0x84
	.byte	0
	.uleb128 0x14
	.long	0x30
	.long	0x497
	.uleb128 0x17
	.long	0x67
	.byte	0x7f
	.byte	0
	.uleb128 0x11
	.byte	0x2
	.long	0x30
	.uleb128 0x3
	.long	.LASF69
	.byte	0x1
	.byte	0x4c
	.byte	0x5
	.long	0x43f
	.uleb128 0x18
	.long	.LASF70
	.byte	0x3
	.byte	0x28
	.byte	0xc
	.long	0x49d
	.byte	0x1
	.byte	0x5
	.byte	0x3
	.long	USBtoUSART_Buffer
	.uleb128 0x18
	.long	.LASF71
	.byte	0x3
	.byte	0x2b
	.byte	0xc
	.long	0x49d
	.byte	0x1
	.byte	0x5
	.byte	0x3
	.long	USARTtoUSB_Buffer
	.uleb128 0x8
	.byte	0x3
	.byte	0x3
	.byte	0x2e
	.byte	0xa
	.long	0x506
	.uleb128 0x9
	.long	.LASF72
	.byte	0x3
	.byte	0x30
	.byte	0xa
	.long	0x30
	.byte	0x2
	.byte	0x23
	.uleb128 0
	.uleb128 0x9
	.long	.LASF73
	.byte	0x3
	.byte	0x31
	.byte	0xa
	.long	0x30
	.byte	0x2
	.byte	0x23
	.uleb128 0x1
	.uleb128 0x9
	.long	.LASF74
	.byte	0x3
	.byte	0x32
	.byte	0xa
	.long	0x30
	.byte	0x2
	.byte	0x23
	.uleb128 0x2
	.byte	0
	.uleb128 0xb
	.long	0x4cf
	.uleb128 0x18
	.long	.LASF75
	.byte	0x3
	.byte	0x33
	.byte	0x3
	.long	0x506
	.byte	0x1
	.byte	0x5
	.byte	0x3
	.long	PulseMSRemaining
	.uleb128 0x18
	.long	.LASF76
	.byte	0x3
	.byte	0x39
	.byte	0x1c
	.long	0x433
	.byte	0x1
	.byte	0x5
	.byte	0x3
	.long	VirtualSerial_CDC_Interface
	.uleb128 0x19
	.byte	0x1
	.long	.LASF78
	.byte	0x3
	.byte	0xea
	.byte	0x6
	.byte	0x1
	.long	.LFB95
	.long	.LFE95
	.byte	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 2
	.byte	0x1
	.long	0x575
	.uleb128 0x1a
	.long	.LASF82
	.byte	0x3
	.byte	0xea
	.byte	0x50
	.long	0x57b
	.long	.LLST45
	.long	.LVUS45
	.uleb128 0x1b
	.long	.LASF77
	.byte	0x3
	.byte	0xec
	.byte	0x7
	.long	0x14f
	.long	.LLST46
	.long	.LVUS46
	.byte	0
	.uleb128 0x11
	.byte	0x2
	.long	0x433
	.uleb128 0x4
	.long	0x575
	.uleb128 0x1c
	.byte	0x1
	.long	.LASF79
	.byte	0x3
	.byte	0xde
	.byte	0x1
	.byte	0x1
	.long	.LFB94
	.long	.LFE94
	.long	.LLST39
	.byte	0x1
	.long	0x634
	.uleb128 0x1b
	.long	.LASF80
	.byte	0x3
	.byte	0xe0
	.byte	0xa
	.long	0x30
	.long	.LLST40
	.long	.LVUS40
	.uleb128 0x1d
	.long	0xdb7
	.long	.LBI188
	.byte	.LVU338
	.long	.Ldebug_ranges0+0x100
	.byte	0x3
	.byte	0xe3
	.byte	0x4
	.uleb128 0x1e
	.long	0xdd1
	.long	.LLST41
	.long	.LVUS41
	.uleb128 0x1f
	.long	0xdc5
	.uleb128 0x20
	.long	0xddd
	.long	.LBB190
	.long	.LBE190
	.uleb128 0x21
	.long	0xdde
	.long	.LLST42
	.long	.LVUS42
	.uleb128 0x21
	.long	0xdea
	.long	.LLST43
	.long	.LVUS43
	.uleb128 0x22
	.long	0xed1
	.long	.LBI191
	.byte	.LVU348
	.long	.LBB191
	.long	.LBE191
	.byte	0x1
	.byte	0xa6
	.byte	0x4
	.uleb128 0x23
	.long	0xeb0
	.long	.LBI193
	.byte	.LVU357
	.long	.LBB193
	.long	.LBE193
	.byte	0x1
	.byte	0xa6
	.byte	0x4
	.uleb128 0x1e
	.long	0xebe
	.long	.LLST44
	.long	.LVUS44
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x1c
	.byte	0x1
	.long	.LASF81
	.byte	0x3
	.byte	0xae
	.byte	0x6
	.byte	0x1
	.long	.LFB93
	.long	.LFE93
	.long	.LLST36
	.byte	0x1
	.long	0x678
	.uleb128 0x1a
	.long	.LASF82
	.byte	0x3
	.byte	0xae
	.byte	0x4d
	.long	0x57b
	.long	.LLST37
	.long	.LVUS37
	.uleb128 0x1b
	.long	.LASF83
	.byte	0x3
	.byte	0xb0
	.byte	0xa
	.long	0x30
	.long	.LLST38
	.long	.LVUS38
	.byte	0
	.uleb128 0x19
	.byte	0x1
	.long	.LASF84
	.byte	0x3
	.byte	0xa5
	.byte	0x6
	.byte	0x1
	.long	.LFB92
	.long	.LFE92
	.byte	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 2
	.byte	0x1
	.long	0x6ad
	.uleb128 0x24
	.long	.LVL56
	.byte	0x1
	.long	0xf11
	.uleb128 0x25
	.byte	0x6
	.byte	0x68
	.byte	0x93
	.uleb128 0x1
	.byte	0x69
	.byte	0x93
	.uleb128 0x1
	.byte	0x5
	.byte	0x3
	.long	VirtualSerial_CDC_Interface
	.byte	0
	.byte	0
	.uleb128 0x19
	.byte	0x1
	.long	.LASF85
	.byte	0x3
	.byte	0x9f
	.byte	0x6
	.byte	0x1
	.long	.LFB91
	.long	.LFE91
	.byte	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 2
	.byte	0x1
	.long	0x6e2
	.uleb128 0x24
	.long	.LVL55
	.byte	0x1
	.long	0xf1f
	.uleb128 0x25
	.byte	0x6
	.byte	0x68
	.byte	0x93
	.uleb128 0x1
	.byte	0x69
	.byte	0x93
	.uleb128 0x1
	.byte	0x5
	.byte	0x3
	.long	VirtualSerial_CDC_Interface
	.byte	0
	.byte	0
	.uleb128 0x19
	.byte	0x1
	.long	.LASF86
	.byte	0x3
	.byte	0x8b
	.byte	0x6
	.byte	0x1
	.long	.LFB90
	.long	.LFE90
	.byte	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 2
	.byte	0x1
	.long	0x779
	.uleb128 0x26
	.long	0xedf
	.long	.LBI45
	.byte	.LVU32
	.long	.Ldebug_ranges0+0
	.byte	0x3
	.byte	0x8f
	.byte	0x2
	.long	0x726
	.uleb128 0x20
	.long	0xf01
	.long	.LBB46
	.long	.LBE46
	.uleb128 0x27
	.long	0xf02
	.byte	0
	.byte	0
	.uleb128 0x28
	.long	0xc94
	.long	.LBI48
	.byte	.LVU37
	.long	.LBB48
	.long	.LBE48
	.byte	0x3
	.byte	0x92
	.byte	0x2
	.long	0x75a
	.uleb128 0x1e
	.long	0xca2
	.long	.LLST5
	.long	.LVUS5
	.uleb128 0x1e
	.long	0xcae
	.long	.LLST6
	.long	.LVUS6
	.byte	0
	.uleb128 0x22
	.long	0xcf1
	.long	.LBI50
	.byte	.LVU52
	.long	.LBB50
	.long	.LBE50
	.byte	0x3
	.byte	0x93
	.byte	0x2
	.uleb128 0x29
	.long	.LVL9
	.long	0xf2d
	.byte	0
	.uleb128 0x2a
	.byte	0x1
	.long	.LASF118
	.byte	0x3
	.byte	0x50
	.byte	0x5
	.byte	0x1
	.long	0x54
	.long	.LFB89
	.long	.LFE89
	.byte	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 2
	.byte	0x1
	.long	0xc79
	.uleb128 0x2b
	.long	.Ldebug_ranges0+0x48
	.long	0xb79
	.uleb128 0x1b
	.long	.LASF87
	.byte	0x3
	.byte	0x66
	.byte	0x14
	.long	0x30
	.long	.LLST13
	.long	.LVUS13
	.uleb128 0x2b
	.long	.Ldebug_ranges0+0xb0
	.long	0x873
	.uleb128 0x1b
	.long	.LASF80
	.byte	0x3
	.byte	0x5e
	.byte	0xc
	.long	0x48
	.long	.LLST18
	.long	.LVUS18
	.uleb128 0x26
	.long	0xdb7
	.long	.LBI139
	.byte	.LVU142
	.long	.Ldebug_ranges0+0xc8
	.byte	0x3
	.byte	0x62
	.byte	0x6
	.long	0x85a
	.uleb128 0x1e
	.long	0xdd1
	.long	.LLST19
	.long	.LVUS19
	.uleb128 0x1f
	.long	0xdc5
	.uleb128 0x20
	.long	0xddd
	.long	.LBB141
	.long	.LBE141
	.uleb128 0x21
	.long	0xdde
	.long	.LLST20
	.long	.LVUS20
	.uleb128 0x21
	.long	0xdea
	.long	.LLST21
	.long	.LVUS21
	.uleb128 0x22
	.long	0xed1
	.long	.LBI142
	.byte	.LVU152
	.long	.LBB142
	.long	.LBE142
	.byte	0x1
	.byte	0xa6
	.byte	0x4
	.uleb128 0x23
	.long	0xeb0
	.long	.LBI144
	.byte	.LVU161
	.long	.LBB144
	.long	.LBE144
	.byte	0x1
	.byte	0xa6
	.byte	0x4
	.uleb128 0x1e
	.long	0xebe
	.long	.LLST22
	.long	.LVUS22
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x2c
	.long	.LVL24
	.long	0xf3c
	.uleb128 0x25
	.byte	0x6
	.byte	0x68
	.byte	0x93
	.uleb128 0x1
	.byte	0x69
	.byte	0x93
	.uleb128 0x1
	.byte	0x5
	.byte	0x3
	.long	VirtualSerial_CDC_Interface
	.byte	0
	.byte	0
	.uleb128 0x26
	.long	0xe17
	.long	.LBI125
	.byte	.LVU113
	.long	.Ldebug_ranges0+0x78
	.byte	0x3
	.byte	0x5c
	.byte	0x9
	.long	0x914
	.uleb128 0x1f
	.long	0xe29
	.uleb128 0x1d
	.long	0xe36
	.long	.LBI126
	.byte	.LVU116
	.long	.Ldebug_ranges0+0x98
	.byte	0x1
	.byte	0x81
	.byte	0xc
	.uleb128 0x1f
	.long	0xe48
	.uleb128 0x2d
	.long	.Ldebug_ranges0+0x98
	.uleb128 0x21
	.long	0xe54
	.long	.LLST14
	.long	.LVUS14
	.uleb128 0x2e
	.long	0xe60
	.long	.Ldebug_ranges0+0x98
	.uleb128 0x21
	.long	0xe61
	.long	.LLST15
	.long	.LVUS15
	.uleb128 0x21
	.long	0xe6d
	.long	.LLST16
	.long	.LVUS16
	.uleb128 0x22
	.long	0xed1
	.long	.LBI129
	.byte	.LVU120
	.long	.LBB129
	.long	.LBE129
	.byte	0x1
	.byte	0x6f
	.byte	0x4
	.uleb128 0x23
	.long	0xeb0
	.long	.LBI131
	.byte	.LVU129
	.long	.LBB131
	.long	.LBE131
	.byte	0x1
	.byte	0x6f
	.byte	0x4
	.uleb128 0x1e
	.long	0xebe
	.long	.LLST17
	.long	.LVUS17
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x28
	.long	0xe36
	.long	.LBI150
	.byte	.LVU167
	.long	.LBB150
	.long	.LBE150
	.byte	0x3
	.byte	0x66
	.byte	0x22
	.long	0x9a0
	.uleb128 0x1f
	.long	0xe48
	.uleb128 0x21
	.long	0xe54
	.long	.LLST23
	.long	.LVUS23
	.uleb128 0x20
	.long	0xe60
	.long	.LBB152
	.long	.LBE152
	.uleb128 0x21
	.long	0xe61
	.long	.LLST24
	.long	.LVUS24
	.uleb128 0x21
	.long	0xe6d
	.long	.LLST25
	.long	.LVUS25
	.uleb128 0x22
	.long	0xed1
	.long	.LBI153
	.byte	.LVU171
	.long	.LBB153
	.long	.LBE153
	.byte	0x1
	.byte	0x6f
	.byte	0x4
	.uleb128 0x23
	.long	0xeb0
	.long	.LBI155
	.byte	.LVU180
	.long	.LBB155
	.long	.LBE155
	.byte	0x1
	.byte	0x6f
	.byte	0x4
	.uleb128 0x1e
	.long	0xebe
	.long	.LLST26
	.long	.LVUS26
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x28
	.long	0xcd6
	.long	.LBI157
	.byte	.LVU195
	.long	.LBB157
	.long	.LBE157
	.byte	0x3
	.byte	0x6c
	.byte	0x5
	.long	0x9c7
	.uleb128 0x1e
	.long	0xce4
	.long	.LLST27
	.long	.LVUS27
	.byte	0
	.uleb128 0x28
	.long	0xcbb
	.long	.LBI159
	.byte	.LVU210
	.long	.LBB159
	.long	.LBE159
	.byte	0x3
	.byte	0x76
	.byte	0x6
	.long	0x9ee
	.uleb128 0x1e
	.long	0xcc9
	.long	.LLST28
	.long	.LVUS28
	.byte	0
	.uleb128 0x28
	.long	0xcbb
	.long	.LBI161
	.byte	.LVU221
	.long	.LBB161
	.long	.LBE161
	.byte	0x3
	.byte	0x7a
	.byte	0x6
	.long	0xa15
	.uleb128 0x1e
	.long	0xcc9
	.long	.LLST29
	.long	.LVUS29
	.byte	0
	.uleb128 0x26
	.long	0xdf8
	.long	.LBI163
	.byte	.LVU226
	.long	.Ldebug_ranges0+0xe8
	.byte	0x3
	.byte	0x7e
	.byte	0x9
	.long	0xab8
	.uleb128 0x1f
	.long	0xe0a
	.uleb128 0x23
	.long	0xe36
	.long	.LBI164
	.byte	.LVU229
	.long	.LBB164
	.long	.LBE164
	.byte	0x1
	.byte	0x92
	.byte	0xc
	.uleb128 0x1f
	.long	0xe48
	.uleb128 0x21
	.long	0xe54
	.long	.LLST30
	.long	.LVUS30
	.uleb128 0x20
	.long	0xe60
	.long	.LBB166
	.long	.LBE166
	.uleb128 0x21
	.long	0xe61
	.long	.LLST31
	.long	.LVUS31
	.uleb128 0x21
	.long	0xe6d
	.long	.LLST32
	.long	.LVUS32
	.uleb128 0x22
	.long	0xed1
	.long	.LBI167
	.byte	.LVU233
	.long	.LBB167
	.long	.LBE167
	.byte	0x1
	.byte	0x6f
	.byte	0x4
	.uleb128 0x23
	.long	0xeb0
	.long	.LBI169
	.byte	.LVU242
	.long	.LBB169
	.long	.LBE169
	.byte	0x1
	.byte	0x6f
	.byte	0x4
	.uleb128 0x1e
	.long	0xebe
	.long	.LLST33
	.long	.LVUS33
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x28
	.long	0xc79
	.long	.LBI172
	.byte	.LVU252
	.long	.LBB172
	.long	.LBE172
	.byte	0x3
	.byte	0x7f
	.byte	0x5
	.long	0xadf
	.uleb128 0x1e
	.long	0xc87
	.long	.LLST34
	.long	.LVUS34
	.byte	0
	.uleb128 0x28
	.long	0xcd6
	.long	.LBI174
	.byte	.LVU262
	.long	.LBB174
	.long	.LBE174
	.byte	0x3
	.byte	0x81
	.byte	0x6
	.long	0xb06
	.uleb128 0x1e
	.long	0xce4
	.long	.LLST35
	.long	.LVUS35
	.byte	0
	.uleb128 0x2f
	.long	.LVL46
	.long	0xcfb
	.long	0xb1f
	.uleb128 0x25
	.byte	0x6
	.byte	0x68
	.byte	0x93
	.uleb128 0x1
	.byte	0x69
	.byte	0x93
	.uleb128 0x1
	.byte	0x2
	.byte	0x80
	.sleb128 0
	.byte	0
	.uleb128 0x2f
	.long	.LVL49
	.long	0xf4b
	.long	0xb3b
	.uleb128 0x25
	.byte	0x6
	.byte	0x68
	.byte	0x93
	.uleb128 0x1
	.byte	0x69
	.byte	0x93
	.uleb128 0x1
	.byte	0x5
	.byte	0x3
	.long	VirtualSerial_CDC_Interface
	.byte	0
	.uleb128 0x29
	.long	.LVL50
	.long	0xf59
	.uleb128 0x2f
	.long	.LVL53
	.long	0xcfb
	.long	0xb60
	.uleb128 0x25
	.byte	0x6
	.byte	0x68
	.byte	0x93
	.uleb128 0x1
	.byte	0x69
	.byte	0x93
	.uleb128 0x1
	.byte	0x5
	.byte	0x3
	.long	USARTtoUSB_Buffer
	.byte	0
	.uleb128 0x2c
	.long	.LVL54
	.long	0xf67
	.uleb128 0x25
	.byte	0x6
	.byte	0x68
	.byte	0x93
	.uleb128 0x1
	.byte	0x69
	.byte	0x93
	.uleb128 0x1
	.byte	0x5
	.byte	0x3
	.long	VirtualSerial_CDC_Interface
	.byte	0
	.byte	0
	.uleb128 0x26
	.long	0xe7b
	.long	.LBI110
	.byte	.LVU68
	.long	.Ldebug_ranges0+0x18
	.byte	0x3
	.byte	0x54
	.byte	0x2
	.long	0xbf4
	.uleb128 0x1f
	.long	0xe89
	.uleb128 0x20
	.long	0xe95
	.long	.LBB111
	.long	.LBE111
	.uleb128 0x21
	.long	0xe96
	.long	.LLST7
	.long	.LVUS7
	.uleb128 0x21
	.long	0xea2
	.long	.LLST8
	.long	.LVUS8
	.uleb128 0x22
	.long	0xed1
	.long	.LBI112
	.byte	.LVU71
	.long	.LBB112
	.long	.LBE112
	.byte	0x1
	.byte	0x57
	.byte	0x4
	.uleb128 0x23
	.long	0xeb0
	.long	.LBI114
	.byte	.LVU82
	.long	.LBB114
	.long	.LBE114
	.byte	0x1
	.byte	0x57
	.byte	0x4
	.uleb128 0x1e
	.long	0xebe
	.long	.LLST9
	.long	.LVUS9
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x26
	.long	0xe7b
	.long	.LBI117
	.byte	.LVU88
	.long	.Ldebug_ranges0+0x30
	.byte	0x3
	.byte	0x55
	.byte	0x2
	.long	0xc6f
	.uleb128 0x1f
	.long	0xe89
	.uleb128 0x20
	.long	0xe95
	.long	.LBB118
	.long	.LBE118
	.uleb128 0x21
	.long	0xe96
	.long	.LLST10
	.long	.LVUS10
	.uleb128 0x21
	.long	0xea2
	.long	.LLST11
	.long	.LVUS11
	.uleb128 0x22
	.long	0xed1
	.long	.LBI119
	.byte	.LVU91
	.long	.LBB119
	.long	.LBE119
	.byte	0x1
	.byte	0x57
	.byte	0x4
	.uleb128 0x23
	.long	0xeb0
	.long	.LBI121
	.byte	.LVU102
	.long	.LBB121
	.long	.LBE121
	.byte	0x1
	.byte	0x57
	.byte	0x4
	.uleb128 0x1e
	.long	0xebe
	.long	.LLST12
	.long	.LVUS12
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x29
	.long	.LVL10
	.long	0x6e2
	.byte	0
	.uleb128 0x30
	.long	.LASF88
	.byte	0x5
	.byte	0x8c
	.byte	0x17
	.byte	0x1
	.byte	0x3
	.long	0xc94
	.uleb128 0x31
	.long	.LASF90
	.byte	0x5
	.byte	0x8c
	.byte	0x30
	.long	0x27a
	.byte	0
	.uleb128 0x30
	.long	.LASF89
	.byte	0x5
	.byte	0x63
	.byte	0x17
	.byte	0x1
	.byte	0x3
	.long	0xcbb
	.uleb128 0x31
	.long	.LASF91
	.byte	0x5
	.byte	0x63
	.byte	0x32
	.long	0x81
	.uleb128 0x31
	.long	.LASF92
	.byte	0x5
	.byte	0x64
	.byte	0x2e
	.long	0x15b
	.byte	0
	.uleb128 0x30
	.long	.LASF93
	.byte	0x6
	.byte	0x4e
	.byte	0x17
	.byte	0x1
	.byte	0x3
	.long	0xcd6
	.uleb128 0x31
	.long	.LASF94
	.byte	0x6
	.byte	0x4e
	.byte	0x36
	.long	0x3c
	.byte	0
	.uleb128 0x30
	.long	.LASF95
	.byte	0x6
	.byte	0x49
	.byte	0x17
	.byte	0x1
	.byte	0x3
	.long	0xcf1
	.uleb128 0x31
	.long	.LASF94
	.byte	0x6
	.byte	0x49
	.byte	0x35
	.long	0x3c
	.byte	0
	.uleb128 0x32
	.long	.LASF119
	.byte	0x6
	.byte	0x43
	.byte	0x17
	.byte	0x1
	.byte	0x3
	.uleb128 0x33
	.long	.LASF120
	.byte	0x1
	.byte	0xb6
	.byte	0x21
	.byte	0x1
	.long	0x30
	.long	.LFB76
	.long	.LFE76
	.byte	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 2
	.byte	0x1
	.long	0xdac
	.uleb128 0x1a
	.long	.LASF67
	.byte	0x1
	.byte	0xb6
	.byte	0x45
	.long	0xdb2
	.long	.LLST0
	.long	.LVUS0
	.uleb128 0x1b
	.long	.LASF96
	.byte	0x1
	.byte	0xb8
	.byte	0x14
	.long	0x30
	.long	.LLST1
	.long	.LVUS1
	.uleb128 0x34
	.long	.LBB34
	.long	.LBE34
	.uleb128 0x1b
	.long	.LASF97
	.byte	0x1
	.byte	0xbd
	.byte	0x4
	.long	0x30
	.long	.LLST2
	.long	.LVUS2
	.uleb128 0x1b
	.long	.LASF98
	.byte	0x1
	.byte	0xbd
	.byte	0x4
	.long	0x30
	.long	.LLST3
	.long	.LVUS3
	.uleb128 0x22
	.long	0xed1
	.long	.LBI35
	.byte	.LVU10
	.long	.LBB35
	.long	.LBE35
	.byte	0x1
	.byte	0xbd
	.byte	0x4
	.uleb128 0x23
	.long	0xeb0
	.long	.LBI37
	.byte	.LVU19
	.long	.LBB37
	.long	.LBE37
	.byte	0x1
	.byte	0xbd
	.byte	0x4
	.uleb128 0x1e
	.long	0xebe
	.long	.LLST4
	.long	.LVUS4
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x11
	.byte	0x2
	.long	0x49d
	.uleb128 0x4
	.long	0xdac
	.uleb128 0x30
	.long	.LASF99
	.byte	0x1
	.byte	0x9e
	.byte	0x16
	.byte	0x1
	.byte	0x3
	.long	0xdf8
	.uleb128 0x31
	.long	.LASF67
	.byte	0x1
	.byte	0x9e
	.byte	0x3a
	.long	0xdb2
	.uleb128 0x31
	.long	.LASF96
	.byte	0x1
	.byte	0x9f
	.byte	0x3e
	.long	0x3c
	.uleb128 0x35
	.uleb128 0x36
	.long	.LASF97
	.byte	0x1
	.byte	0xa6
	.byte	0x4
	.long	0x30
	.uleb128 0x36
	.long	.LASF98
	.byte	0x1
	.byte	0xa6
	.byte	0x4
	.long	0x30
	.byte	0
	.byte	0
	.uleb128 0x37
	.long	.LASF100
	.byte	0x1
	.byte	0x90
	.byte	0x16
	.byte	0x1
	.long	0x14f
	.byte	0x3
	.long	0xe17
	.uleb128 0x31
	.long	.LASF67
	.byte	0x1
	.byte	0x90
	.byte	0x3b
	.long	0xdb2
	.byte	0
	.uleb128 0x37
	.long	.LASF101
	.byte	0x1
	.byte	0x7f
	.byte	0x16
	.byte	0x1
	.long	0x14f
	.byte	0x3
	.long	0xe36
	.uleb128 0x31
	.long	.LASF67
	.byte	0x1
	.byte	0x7f
	.byte	0x3a
	.long	0xdb2
	.byte	0
	.uleb128 0x37
	.long	.LASF102
	.byte	0x1
	.byte	0x6b
	.byte	0x22
	.byte	0x1
	.long	0x30
	.byte	0x3
	.long	0xe7b
	.uleb128 0x31
	.long	.LASF67
	.byte	0x1
	.byte	0x6b
	.byte	0x48
	.long	0xdb2
	.uleb128 0x36
	.long	.LASF68
	.byte	0x1
	.byte	0x6d
	.byte	0x15
	.long	0x30
	.uleb128 0x35
	.uleb128 0x36
	.long	.LASF97
	.byte	0x1
	.byte	0x6f
	.byte	0x4
	.long	0x30
	.uleb128 0x36
	.long	.LASF98
	.byte	0x1
	.byte	0x6f
	.byte	0x4
	.long	0x30
	.byte	0
	.byte	0
	.uleb128 0x30
	.long	.LASF103
	.byte	0x1
	.byte	0x55
	.byte	0x16
	.byte	0x1
	.byte	0x3
	.long	0xeb0
	.uleb128 0x31
	.long	.LASF67
	.byte	0x1
	.byte	0x55
	.byte	0x3e
	.long	0xdb2
	.uleb128 0x35
	.uleb128 0x36
	.long	.LASF97
	.byte	0x1
	.byte	0x57
	.byte	0x4
	.long	0x30
	.uleb128 0x36
	.long	.LASF98
	.byte	0x1
	.byte	0x57
	.byte	0x4
	.long	0x30
	.byte	0
	.byte	0
	.uleb128 0x30
	.long	.LASF104
	.byte	0x2
	.byte	0x44
	.byte	0x18
	.byte	0x1
	.byte	0x3
	.long	0xecb
	.uleb128 0x38
	.string	"__s"
	.byte	0x2
	.byte	0x44
	.byte	0x33
	.long	0xecb
	.byte	0
	.uleb128 0x11
	.byte	0x2
	.long	0x3c
	.uleb128 0x39
	.long	.LASF121
	.byte	0x2
	.byte	0x30
	.byte	0x1b
	.byte	0x1
	.long	0x30
	.byte	0x3
	.uleb128 0x3a
	.long	.LASF105
	.byte	0x4
	.word	0x1bf
	.byte	0x6
	.byte	0x1
	.byte	0x3
	.long	0xf11
	.uleb128 0x3b
	.long	0xf01
	.uleb128 0x3c
	.long	.LASF106
	.byte	0x4
	.word	0x1c3
	.byte	0x1a
	.long	0x30
	.byte	0
	.uleb128 0x35
	.uleb128 0x3c
	.long	.LASF106
	.byte	0x4
	.word	0x1d5
	.byte	0x1a
	.long	0x30
	.byte	0
	.byte	0
	.uleb128 0x3d
	.byte	0x1
	.byte	0x1
	.long	.LASF107
	.long	.LASF107
	.byte	0xe
	.byte	0xa5
	.byte	0x9
	.uleb128 0x3d
	.byte	0x1
	.byte	0x1
	.long	.LASF108
	.long	.LASF108
	.byte	0xe
	.byte	0x9e
	.byte	0x9
	.uleb128 0x3e
	.byte	0x1
	.byte	0x1
	.long	.LASF109
	.long	.LASF109
	.byte	0xf
	.word	0x123
	.byte	0x9
	.uleb128 0x3e
	.byte	0x1
	.byte	0x1
	.long	.LASF110
	.long	.LASF110
	.byte	0xe
	.word	0x106
	.byte	0xc
	.uleb128 0x3d
	.byte	0x1
	.byte	0x1
	.long	.LASF111
	.long	.LASF111
	.byte	0xe
	.byte	0xac
	.byte	0x9
	.uleb128 0x3d
	.byte	0x1
	.byte	0x1
	.long	.LASF112
	.long	.LASF112
	.byte	0xa
	.byte	0xb2
	.byte	0x9
	.uleb128 0x3d
	.byte	0x1
	.byte	0x1
	.long	.LASF113
	.long	.LASF113
	.byte	0xe
	.byte	0xe9
	.byte	0xc
	.byte	0
	.section	.debug_abbrev,"",@progbits
.Ldebug_abbrev0:
	.uleb128 0x1
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1b
	.uleb128 0xe
	.uleb128 0x55
	.uleb128 0x6
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x52
	.uleb128 0x1
	.uleb128 0x10
	.uleb128 0x6
	.byte	0
	.byte	0
	.uleb128 0x2
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.byte	0
	.byte	0
	.uleb128 0x3
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x4
	.uleb128 0x26
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x5
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.byte	0
	.byte	0
	.uleb128 0x6
	.uleb128 0x4
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x7
	.uleb128 0x28
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1c
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x8
	.uleb128 0x13
	.byte	0x1
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x9
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xa
	.byte	0
	.byte	0
	.uleb128 0xa
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3c
	.uleb128 0xc
	.byte	0
	.byte	0
	.uleb128 0xb
	.uleb128 0x35
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xc
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0xd
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xe
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xa
	.byte	0
	.byte	0
	.uleb128 0xf
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xa
	.byte	0
	.byte	0
	.uleb128 0x10
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xa
	.byte	0
	.byte	0
	.uleb128 0x11
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x12
	.uleb128 0x15
	.byte	0x1
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x13
	.uleb128 0x5
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x14
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x15
	.uleb128 0x21
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x16
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3c
	.uleb128 0xc
	.byte	0
	.byte	0
	.uleb128 0x17
	.uleb128 0x21
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x18
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x2
	.uleb128 0xa
	.byte	0
	.byte	0
	.uleb128 0x19
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0xa
	.uleb128 0x2117
	.uleb128 0xc
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1a
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x6
	.uleb128 0x2137
	.uleb128 0x6
	.byte	0
	.byte	0
	.uleb128 0x1b
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x6
	.uleb128 0x2137
	.uleb128 0x6
	.byte	0
	.byte	0
	.uleb128 0x1c
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0x6
	.uleb128 0x2117
	.uleb128 0xc
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1d
	.uleb128 0x1d
	.byte	0x1
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x52
	.uleb128 0x1
	.uleb128 0x2138
	.uleb128 0xb
	.uleb128 0x55
	.uleb128 0x6
	.uleb128 0x58
	.uleb128 0xb
	.uleb128 0x59
	.uleb128 0xb
	.uleb128 0x57
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x1e
	.uleb128 0x5
	.byte	0
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x6
	.uleb128 0x2137
	.uleb128 0x6
	.byte	0
	.byte	0
	.uleb128 0x1f
	.uleb128 0x5
	.byte	0
	.uleb128 0x31
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x20
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.byte	0
	.byte	0
	.uleb128 0x21
	.uleb128 0x34
	.byte	0
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x6
	.uleb128 0x2137
	.uleb128 0x6
	.byte	0
	.byte	0
	.uleb128 0x22
	.uleb128 0x1d
	.byte	0
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x52
	.uleb128 0x1
	.uleb128 0x2138
	.uleb128 0xb
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x58
	.uleb128 0xb
	.uleb128 0x59
	.uleb128 0xb
	.uleb128 0x57
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x23
	.uleb128 0x1d
	.byte	0x1
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x52
	.uleb128 0x1
	.uleb128 0x2138
	.uleb128 0xb
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x58
	.uleb128 0xb
	.uleb128 0x59
	.uleb128 0xb
	.uleb128 0x57
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x24
	.uleb128 0x4109
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x2115
	.uleb128 0xc
	.uleb128 0x31
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x25
	.uleb128 0x410a
	.byte	0
	.uleb128 0x2
	.uleb128 0xa
	.uleb128 0x2111
	.uleb128 0xa
	.byte	0
	.byte	0
	.uleb128 0x26
	.uleb128 0x1d
	.byte	0x1
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x52
	.uleb128 0x1
	.uleb128 0x2138
	.uleb128 0xb
	.uleb128 0x55
	.uleb128 0x6
	.uleb128 0x58
	.uleb128 0xb
	.uleb128 0x59
	.uleb128 0xb
	.uleb128 0x57
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x27
	.uleb128 0x34
	.byte	0
	.uleb128 0x31
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x28
	.uleb128 0x1d
	.byte	0x1
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x52
	.uleb128 0x1
	.uleb128 0x2138
	.uleb128 0xb
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x58
	.uleb128 0xb
	.uleb128 0x59
	.uleb128 0xb
	.uleb128 0x57
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x29
	.uleb128 0x4109
	.byte	0
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x31
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x2a
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0xa
	.uleb128 0x2117
	.uleb128 0xc
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x2b
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x55
	.uleb128 0x6
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x2c
	.uleb128 0x4109
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x31
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x2d
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x55
	.uleb128 0x6
	.byte	0
	.byte	0
	.uleb128 0x2e
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x55
	.uleb128 0x6
	.byte	0
	.byte	0
	.uleb128 0x2f
	.uleb128 0x4109
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x30
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x20
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x31
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x32
	.uleb128 0x2e
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x20
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x33
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0xa
	.uleb128 0x2117
	.uleb128 0xc
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x34
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.byte	0
	.byte	0
	.uleb128 0x35
	.uleb128 0xb
	.byte	0x1
	.byte	0
	.byte	0
	.uleb128 0x36
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x37
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x20
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x38
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x39
	.uleb128 0x2e
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x20
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x3a
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x20
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x3b
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x3c
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x3d
	.uleb128 0x2e
	.byte	0
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3c
	.uleb128 0xc
	.uleb128 0x2007
	.uleb128 0xe
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x3e
	.uleb128 0x2e
	.byte	0
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3c
	.uleb128 0xc
	.uleb128 0x2007
	.uleb128 0xe
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_loc,"",@progbits
.Ldebug_loc0:
.LVUS45:
	.uleb128 0
	.uleb128 .LVU370
	.uleb128 .LVU370
	.uleb128 0
.LLST45:
	.long	.LVL76
	.long	.LVL77
	.word	0x6
	.byte	0x68
	.byte	0x93
	.uleb128 0x1
	.byte	0x69
	.byte	0x93
	.uleb128 0x1
	.long	.LVL77
	.long	.LFE95
	.word	0x6
	.byte	0x6e
	.byte	0x93
	.uleb128 0x1
	.byte	0x6f
	.byte	0x93
	.uleb128 0x1
	.long	0
	.long	0
.LVUS46:
	.uleb128 .LVU368
	.uleb128 .LVU370
	.uleb128 .LVU370
	.uleb128 0
.LLST46:
	.long	.LVL76
	.long	.LVL77
	.word	0x7
	.byte	0x88
	.sleb128 13
	.byte	0x94
	.byte	0x1
	.byte	0x31
	.byte	0x1a
	.byte	0x9f
	.long	.LVL77
	.long	.LFE95
	.word	0x5
	.byte	0x88
	.sleb128 0
	.byte	0x31
	.byte	0x1a
	.byte	0x9f
	.long	0
	.long	0
.LLST39:
	.long	.LFB94
	.long	.LCFI11
	.word	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 2
	.long	.LCFI11
	.long	.LCFI12
	.word	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 3
	.long	.LCFI12
	.long	.LCFI13
	.word	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 4
	.long	.LCFI13
	.long	.LCFI14
	.word	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 5
	.long	.LCFI14
	.long	.LFE94
	.word	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 6
	.long	0
	.long	0
.LVUS40:
	.uleb128 .LVU334
	.uleb128 .LVU344
.LLST40:
	.long	.LVL68
	.long	.LVL70
	.word	0x1
	.byte	0x68
	.long	0
	.long	0
.LVUS41:
	.uleb128 .LVU338
	.uleb128 .LVU344
.LLST41:
	.long	.LVL69
	.long	.LVL70
	.word	0x1
	.byte	0x68
	.long	0
	.long	0
.LVUS42:
	.uleb128 .LVU348
	.uleb128 .LVU361
.LLST42:
	.long	.LVL71
	.long	.LVL74
	.word	0x1
	.byte	0x69
	.long	0
	.long	0
.LVUS43:
	.uleb128 .LVU351
	.uleb128 .LVU356
	.uleb128 .LVU356
	.uleb128 .LVU361
.LLST43:
	.long	.LVL72
	.long	.LVL73
	.word	0x2
	.byte	0x31
	.byte	0x9f
	.long	.LVL73
	.long	.LVL74
	.word	0x2
	.byte	0x30
	.byte	0x9f
	.long	0
	.long	0
.LVUS44:
	.uleb128 .LVU358
	.uleb128 .LVU361
.LLST44:
	.long	.LVL73
	.long	.LVL74
	.word	0x6
	.byte	0xf2
	.long	.Ldebug_info0+1503
	.sleb128 0
	.long	0
	.long	0
.LLST36:
	.long	.LFB93
	.long	.LCFI0
	.word	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 2
	.long	.LCFI0
	.long	.LCFI1
	.word	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 3
	.long	.LCFI1
	.long	.LCFI2
	.word	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 4
	.long	.LCFI2
	.long	.LCFI3
	.word	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 5
	.long	.LCFI3
	.long	.LCFI4
	.word	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 6
	.long	.LCFI4
	.long	.LCFI5
	.word	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 7
	.long	.LCFI5
	.long	.LCFI6
	.word	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 8
	.long	.LCFI6
	.long	.LCFI7
	.word	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 9
	.long	.LCFI7
	.long	.LCFI8
	.word	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 10
	.long	.LCFI8
	.long	.LCFI9
	.word	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 11
	.long	.LCFI9
	.long	.LCFI10
	.word	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 16
	.long	.LCFI10
	.long	.LFE93
	.word	0x2
	.byte	0x8c
	.sleb128 16
	.long	0
	.long	0
.LVUS37:
	.uleb128 0
	.uleb128 .LVU285
	.uleb128 .LVU285
	.uleb128 .LVU321
	.uleb128 .LVU321
	.uleb128 .LVU323
	.uleb128 .LVU323
	.uleb128 0
.LLST37:
	.long	.LVL57
	.long	.LVL59
	.word	0x6
	.byte	0x68
	.byte	0x93
	.uleb128 0x1
	.byte	0x69
	.byte	0x93
	.uleb128 0x1
	.long	.LVL59
	.long	.LVL63
	.word	0x6
	.byte	0x60
	.byte	0x93
	.uleb128 0x1
	.byte	0x61
	.byte	0x93
	.uleb128 0x1
	.long	.LVL63
	.long	.LVL65
	.word	0x6
	.byte	0x6e
	.byte	0x93
	.uleb128 0x1
	.byte	0x6f
	.byte	0x93
	.uleb128 0x1
	.long	.LVL65
	.long	.LFE93
	.word	0x6
	.byte	0x60
	.byte	0x93
	.uleb128 0x1
	.byte	0x61
	.byte	0x93
	.uleb128 0x1
	.long	0
	.long	0
.LVUS38:
	.uleb128 .LVU283
	.uleb128 .LVU289
	.uleb128 .LVU289
	.uleb128 .LVU322
	.uleb128 .LVU323
	.uleb128 0
.LLST38:
	.long	.LVL58
	.long	.LVL60
	.word	0x2
	.byte	0x30
	.byte	0x9f
	.long	.LVL60
	.long	.LVL64
	.word	0x1
	.byte	0x5f
	.long	.LVL65
	.long	.LFE93
	.word	0x1
	.byte	0x5f
	.long	0
	.long	0
.LVUS5:
	.uleb128 .LVU38
	.uleb128 .LVU50
.LLST5:
	.long	.LVL7
	.long	.LVL8
	.word	0x4
	.byte	0xa
	.word	0x2580
	.byte	0x9f
	.long	0
	.long	0
.LVUS6:
	.uleb128 .LVU38
	.uleb128 .LVU50
.LLST6:
	.long	.LVL7
	.long	.LVL8
	.word	0x2
	.byte	0x30
	.byte	0x9f
	.long	0
	.long	0
.LVUS13:
	.uleb128 .LVU186
	.uleb128 .LVU272
	.uleb128 .LVU274
	.uleb128 0
.LLST13:
	.long	.LVL34
	.long	.LVL51
	.word	0x1
	.byte	0x6c
	.long	.LVL52
	.long	.LFE89
	.word	0x1
	.byte	0x6c
	.long	0
	.long	0
.LVUS18:
	.uleb128 .LVU139
	.uleb128 .LVU148
.LLST18:
	.long	.LVL24
	.long	.LVL26
	.word	0x6
	.byte	0x68
	.byte	0x93
	.uleb128 0x1
	.byte	0x69
	.byte	0x93
	.uleb128 0x1
	.long	0
	.long	0
.LVUS19:
	.uleb128 .LVU142
	.uleb128 .LVU148
.LLST19:
	.long	.LVL25
	.long	.LVL26
	.word	0x1
	.byte	0x68
	.long	0
	.long	0
.LVUS20:
	.uleb128 .LVU152
	.uleb128 .LVU165
.LLST20:
	.long	.LVL27
	.long	.LVL30
	.word	0x1
	.byte	0x69
	.long	0
	.long	0
.LVUS21:
	.uleb128 .LVU155
	.uleb128 .LVU160
	.uleb128 .LVU160
	.uleb128 .LVU165
.LLST21:
	.long	.LVL28
	.long	.LVL29
	.word	0x2
	.byte	0x31
	.byte	0x9f
	.long	.LVL29
	.long	.LVL30
	.word	0x2
	.byte	0x30
	.byte	0x9f
	.long	0
	.long	0
.LVUS22:
	.uleb128 .LVU162
	.uleb128 .LVU165
.LLST22:
	.long	.LVL29
	.long	.LVL30
	.word	0x6
	.byte	0xf2
	.long	.Ldebug_info0+2054
	.sleb128 0
	.long	0
	.long	0
.LVUS14:
	.uleb128 .LVU127
	.uleb128 .LVU135
.LLST14:
	.long	.LVL22
	.long	.LVL23
	.word	0x1
	.byte	0x68
	.long	0
	.long	0
.LVUS15:
	.uleb128 .LVU120
	.uleb128 .LVU133
.LLST15:
	.long	.LVL20
	.long	.LVL23
	.word	0x1
	.byte	0x69
	.long	0
	.long	0
.LVUS16:
	.uleb128 .LVU123
	.uleb128 .LVU128
	.uleb128 .LVU128
	.uleb128 0
.LLST16:
	.long	.LVL21
	.long	.LVL22
	.word	0x2
	.byte	0x31
	.byte	0x9f
	.long	.LVL22
	.long	.LFE89
	.word	0x2
	.byte	0x30
	.byte	0x9f
	.long	0
	.long	0
.LVUS17:
	.uleb128 .LVU130
	.uleb128 .LVU161
	.uleb128 .LVU161
	.uleb128 .LVU165
	.uleb128 .LVU180
	.uleb128 .LVU242
	.uleb128 .LVU242
	.uleb128 .LVU272
	.uleb128 .LVU272
	.uleb128 .LVU274
	.uleb128 .LVU274
	.uleb128 0
.LLST17:
	.long	.LVL22
	.long	.LVL29
	.word	0x6
	.byte	0xf2
	.long	.Ldebug_info0+2238
	.sleb128 0
	.long	.LVL29
	.long	.LVL30
	.word	0x6
	.byte	0xf2
	.long	.Ldebug_info0+2054
	.sleb128 0
	.long	.LVL33
	.long	.LVL44
	.word	0x6
	.byte	0xf2
	.long	.Ldebug_info0+2380
	.sleb128 0
	.long	.LVL44
	.long	.LVL51
	.word	0x6
	.byte	0xf2
	.long	.Ldebug_info0+2659
	.sleb128 0
	.long	.LVL51
	.long	.LVL52
	.word	0x6
	.byte	0xf2
	.long	.Ldebug_info0+2238
	.sleb128 0
	.long	.LVL52
	.long	.LFE89
	.word	0x6
	.byte	0xf2
	.long	.Ldebug_info0+2380
	.sleb128 0
	.long	0
	.long	0
.LVUS23:
	.uleb128 .LVU178
	.uleb128 .LVU186
.LLST23:
	.long	.LVL33
	.long	.LVL34
	.word	0x1
	.byte	0x6c
	.long	0
	.long	0
.LVUS24:
	.uleb128 .LVU171
	.uleb128 .LVU184
.LLST24:
	.long	.LVL31
	.long	.LVL34
	.word	0x1
	.byte	0x68
	.long	0
	.long	0
.LVUS25:
	.uleb128 .LVU174
	.uleb128 .LVU179
	.uleb128 .LVU179
	.uleb128 .LVU272
	.uleb128 .LVU274
	.uleb128 0
.LLST25:
	.long	.LVL32
	.long	.LVL33
	.word	0x2
	.byte	0x31
	.byte	0x9f
	.long	.LVL33
	.long	.LVL51
	.word	0x2
	.byte	0x30
	.byte	0x9f
	.long	.LVL52
	.long	.LFE89
	.word	0x2
	.byte	0x30
	.byte	0x9f
	.long	0
	.long	0
.LVUS26:
	.uleb128 .LVU181
	.uleb128 .LVU242
	.uleb128 .LVU242
	.uleb128 .LVU272
	.uleb128 .LVU274
	.uleb128 0
.LLST26:
	.long	.LVL33
	.long	.LVL44
	.word	0x6
	.byte	0xf2
	.long	.Ldebug_info0+2380
	.sleb128 0
	.long	.LVL44
	.long	.LVL51
	.word	0x6
	.byte	0xf2
	.long	.Ldebug_info0+2659
	.sleb128 0
	.long	.LVL52
	.long	.LFE89
	.word	0x6
	.byte	0xf2
	.long	.Ldebug_info0+2380
	.sleb128 0
	.long	0
	.long	0
.LVUS27:
	.uleb128 .LVU195
	.uleb128 .LVU198
.LLST27:
	.long	.LVL35
	.long	.LVL36
	.word	0x3
	.byte	0x8
	.byte	0x20
	.byte	0x9f
	.long	0
	.long	0
.LVUS28:
	.uleb128 .LVU210
	.uleb128 .LVU213
.LLST28:
	.long	.LVL38
	.long	.LVL39
	.word	0x3
	.byte	0x8
	.byte	0x20
	.byte	0x9f
	.long	0
	.long	0
.LVUS29:
	.uleb128 .LVU221
	.uleb128 .LVU224
.LLST29:
	.long	.LVL40
	.long	.LVL41
	.word	0x2
	.byte	0x40
	.byte	0x9f
	.long	0
	.long	0
.LVUS30:
	.uleb128 .LVU240
	.uleb128 .LVU248
.LLST30:
	.long	.LVL44
	.long	.LVL45
	.word	0x1
	.byte	0x68
	.long	0
	.long	0
.LVUS31:
	.uleb128 .LVU233
	.uleb128 .LVU246
.LLST31:
	.long	.LVL42
	.long	.LVL45
	.word	0x1
	.byte	0x69
	.long	0
	.long	0
.LVUS32:
	.uleb128 .LVU236
	.uleb128 .LVU241
	.uleb128 .LVU241
	.uleb128 .LVU272
.LLST32:
	.long	.LVL43
	.long	.LVL44
	.word	0x2
	.byte	0x31
	.byte	0x9f
	.long	.LVL44
	.long	.LVL51
	.word	0x2
	.byte	0x30
	.byte	0x9f
	.long	0
	.long	0
.LVUS33:
	.uleb128 .LVU243
	.uleb128 .LVU272
.LLST33:
	.long	.LVL44
	.long	.LVL51
	.word	0x6
	.byte	0xf2
	.long	.Ldebug_info0+2659
	.sleb128 0
	.long	0
	.long	0
.LVUS34:
	.uleb128 .LVU252
	.uleb128 .LVU260
.LLST34:
	.long	.LVL46
	.long	.LVL47
	.word	0x1
	.byte	0x68
	.long	0
	.long	0
.LVUS35:
	.uleb128 .LVU262
	.uleb128 .LVU265
.LLST35:
	.long	.LVL47
	.long	.LVL48
	.word	0x2
	.byte	0x40
	.byte	0x9f
	.long	0
	.long	0
.LVUS7:
	.uleb128 .LVU71
	.uleb128 .LVU86
.LLST7:
	.long	.LVL11
	.long	.LVL14
	.word	0x1
	.byte	0x62
	.long	0
	.long	0
.LVUS8:
	.uleb128 .LVU74
	.uleb128 .LVU81
	.uleb128 .LVU81
	.uleb128 0
.LLST8:
	.long	.LVL12
	.long	.LVL13
	.word	0x2
	.byte	0x31
	.byte	0x9f
	.long	.LVL13
	.long	.LFE89
	.word	0x2
	.byte	0x30
	.byte	0x9f
	.long	0
	.long	0
.LVUS9:
	.uleb128 .LVU83
	.uleb128 .LVU102
	.uleb128 .LVU102
	.uleb128 .LVU110
	.uleb128 .LVU129
	.uleb128 .LVU161
	.uleb128 .LVU161
	.uleb128 .LVU165
	.uleb128 .LVU180
	.uleb128 .LVU242
	.uleb128 .LVU242
	.uleb128 .LVU272
	.uleb128 .LVU272
	.uleb128 .LVU274
	.uleb128 .LVU274
	.uleb128 0
.LLST9:
	.long	.LVL13
	.long	.LVL17
	.word	0x6
	.byte	0xf2
	.long	.Ldebug_info0+2976
	.sleb128 0
	.long	.LVL17
	.long	.LVL19
	.word	0x6
	.byte	0xf2
	.long	.Ldebug_info0+3099
	.sleb128 0
	.long	.LVL22
	.long	.LVL29
	.word	0x6
	.byte	0xf2
	.long	.Ldebug_info0+2238
	.sleb128 0
	.long	.LVL29
	.long	.LVL30
	.word	0x6
	.byte	0xf2
	.long	.Ldebug_info0+2054
	.sleb128 0
	.long	.LVL33
	.long	.LVL44
	.word	0x6
	.byte	0xf2
	.long	.Ldebug_info0+2380
	.sleb128 0
	.long	.LVL44
	.long	.LVL51
	.word	0x6
	.byte	0xf2
	.long	.Ldebug_info0+2659
	.sleb128 0
	.long	.LVL51
	.long	.LVL52
	.word	0x6
	.byte	0xf2
	.long	.Ldebug_info0+2238
	.sleb128 0
	.long	.LVL52
	.long	.LFE89
	.word	0x6
	.byte	0xf2
	.long	.Ldebug_info0+2380
	.sleb128 0
	.long	0
	.long	0
.LVUS10:
	.uleb128 .LVU91
	.uleb128 .LVU106
.LLST10:
	.long	.LVL15
	.long	.LVL18
	.word	0x1
	.byte	0x62
	.long	0
	.long	0
.LVUS11:
	.uleb128 .LVU94
	.uleb128 .LVU101
	.uleb128 .LVU101
	.uleb128 0
.LLST11:
	.long	.LVL16
	.long	.LVL17
	.word	0x2
	.byte	0x31
	.byte	0x9f
	.long	.LVL17
	.long	.LFE89
	.word	0x2
	.byte	0x30
	.byte	0x9f
	.long	0
	.long	0
.LVUS12:
	.uleb128 .LVU103
	.uleb128 .LVU110
	.uleb128 .LVU129
	.uleb128 .LVU161
	.uleb128 .LVU161
	.uleb128 .LVU165
	.uleb128 .LVU180
	.uleb128 .LVU242
	.uleb128 .LVU242
	.uleb128 .LVU272
	.uleb128 .LVU272
	.uleb128 .LVU274
	.uleb128 .LVU274
	.uleb128 0
.LLST12:
	.long	.LVL17
	.long	.LVL19
	.word	0x6
	.byte	0xf2
	.long	.Ldebug_info0+3099
	.sleb128 0
	.long	.LVL22
	.long	.LVL29
	.word	0x6
	.byte	0xf2
	.long	.Ldebug_info0+2238
	.sleb128 0
	.long	.LVL29
	.long	.LVL30
	.word	0x6
	.byte	0xf2
	.long	.Ldebug_info0+2054
	.sleb128 0
	.long	.LVL33
	.long	.LVL44
	.word	0x6
	.byte	0xf2
	.long	.Ldebug_info0+2380
	.sleb128 0
	.long	.LVL44
	.long	.LVL51
	.word	0x6
	.byte	0xf2
	.long	.Ldebug_info0+2659
	.sleb128 0
	.long	.LVL51
	.long	.LVL52
	.word	0x6
	.byte	0xf2
	.long	.Ldebug_info0+2238
	.sleb128 0
	.long	.LVL52
	.long	.LFE89
	.word	0x6
	.byte	0xf2
	.long	.Ldebug_info0+2380
	.sleb128 0
	.long	0
	.long	0
.LVUS0:
	.uleb128 0
	.uleb128 .LVU5
	.uleb128 .LVU5
	.uleb128 0
.LLST0:
	.long	.LVL0
	.long	.LVL1
	.word	0x6
	.byte	0x68
	.byte	0x93
	.uleb128 0x1
	.byte	0x69
	.byte	0x93
	.uleb128 0x1
	.long	.LVL1
	.long	.LFE76
	.word	0x6
	.byte	0x62
	.byte	0x93
	.uleb128 0x1
	.byte	0x63
	.byte	0x93
	.uleb128 0x1
	.long	0
	.long	0
.LVUS1:
	.uleb128 .LVU5
	.uleb128 0
.LLST1:
	.long	.LVL1
	.long	.LFE76
	.word	0x1
	.byte	0x68
	.long	0
	.long	0
.LVUS2:
	.uleb128 .LVU10
	.uleb128 .LVU23
.LLST2:
	.long	.LVL2
	.long	.LVL5
	.word	0x1
	.byte	0x64
	.long	0
	.long	0
.LVUS3:
	.uleb128 .LVU13
	.uleb128 .LVU18
	.uleb128 .LVU18
	.uleb128 .LVU26
.LLST3:
	.long	.LVL3
	.long	.LVL4
	.word	0x2
	.byte	0x31
	.byte	0x9f
	.long	.LVL4
	.long	.LVL6
	.word	0x2
	.byte	0x30
	.byte	0x9f
	.long	0
	.long	0
.LVUS4:
	.uleb128 .LVU20
	.uleb128 .LVU26
.LLST4:
	.long	.LVL4
	.long	.LVL6
	.word	0x6
	.byte	0xf2
	.long	.Ldebug_info0+3402
	.sleb128 0
	.long	0
	.long	0
	.section	.debug_aranges,"",@progbits
	.long	0x54
	.word	0x2
	.long	.Ldebug_info0
	.byte	0x4
	.byte	0
	.word	0
	.word	0
	.long	.LFB76
	.long	.LFE76-.LFB76
	.long	.LFB90
	.long	.LFE90-.LFB90
	.long	.LFB89
	.long	.LFE89-.LFB89
	.long	.LFB91
	.long	.LFE91-.LFB91
	.long	.LFB92
	.long	.LFE92-.LFB92
	.long	.LFB93
	.long	.LFE93-.LFB93
	.long	.LFB94
	.long	.LFE94-.LFB94
	.long	.LFB95
	.long	.LFE95-.LFB95
	.long	0
	.long	0
	.section	.debug_ranges,"",@progbits
.Ldebug_ranges0:
	.long	.LBB45
	.long	.LBE45
	.long	.LBB47
	.long	.LBE47
	.long	0
	.long	0
	.long	.LBB110
	.long	.LBE110
	.long	.LBB116
	.long	.LBE116
	.long	0
	.long	0
	.long	.LBB117
	.long	.LBE117
	.long	.LBB123
	.long	.LBE123
	.long	0
	.long	0
	.long	.LBB124
	.long	.LBE124
	.long	.LBB177
	.long	.LBE177
	.long	.LBB178
	.long	.LBE178
	.long	.LBB179
	.long	.LBE179
	.long	.LBB180
	.long	.LBE180
	.long	0
	.long	0
	.long	.LBB125
	.long	.LBE125
	.long	.LBB136
	.long	.LBE136
	.long	.LBB137
	.long	.LBE137
	.long	0
	.long	0
	.long	.LBB126
	.long	.LBE126
	.long	.LBB135
	.long	.LBE135
	.long	0
	.long	0
	.long	.LBB138
	.long	.LBE138
	.long	.LBB176
	.long	.LBE176
	.long	0
	.long	0
	.long	.LBB139
	.long	.LBE139
	.long	.LBB148
	.long	.LBE148
	.long	.LBB149
	.long	.LBE149
	.long	0
	.long	0
	.long	.LBB163
	.long	.LBE163
	.long	.LBB171
	.long	.LBE171
	.long	0
	.long	0
	.long	.LBB188
	.long	.LBE188
	.long	.LBB196
	.long	.LBE196
	.long	0
	.long	0
	.long	.LFB76
	.long	.LFE76
	.long	.LFB90
	.long	.LFE90
	.long	.LFB89
	.long	.LFE89
	.long	.LFB91
	.long	.LFE91
	.long	.LFB92
	.long	.LFE92
	.long	.LFB93
	.long	.LFE93
	.long	.LFB94
	.long	.LFE94
	.long	.LFB95
	.long	.LFE95
	.long	0
	.long	0
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.section	.debug_str,"MS",@progbits,1
.LASF20:
	.string	"wIndex"
.LASF120:
	.string	"RingBuffer_Remove"
.LASF76:
	.string	"VirtualSerial_CDC_Interface"
.LASF119:
	.string	"LEDs_Init"
.LASF121:
	.string	"__iCliRetVal"
.LASF79:
	.string	"__vector_23"
.LASF40:
	.string	"unget"
.LASF10:
	.string	"long long unsigned int"
.LASF100:
	.string	"RingBuffer_IsEmpty"
.LASF11:
	.string	"DEVICE_STATE_Unattached"
.LASF13:
	.string	"DEVICE_STATE_Default"
.LASF26:
	.string	"USB_CurrentlySelfPowered"
.LASF3:
	.string	"int16_t"
.LASF9:
	.string	"long long int"
.LASF0:
	.string	"signed char"
.LASF58:
	.string	"BaudRateBPS"
.LASF110:
	.string	"CDC_Device_ReceiveByte"
.LASF91:
	.string	"BaudRate"
.LASF35:
	.string	"CDC_PARITY_None"
.LASF89:
	.string	"Serial_Init"
.LASF6:
	.string	"long int"
.LASF73:
	.string	"RxLEDPulse"
.LASF43:
	.string	"udata"
.LASF72:
	.string	"TxLEDPulse"
.LASF22:
	.string	"USB_Request_Header_t"
.LASF59:
	.string	"CharFormat"
.LASF19:
	.string	"wValue"
.LASF52:
	.string	"DataOUTEndpointDoubleBank"
.LASF4:
	.string	"uint16_t"
.LASF29:
	.string	"USB_Device_States_t"
.LASF47:
	.string	"DataINEndpointNumber"
.LASF94:
	.string	"LEDMask"
.LASF17:
	.string	"bmRequestType"
.LASF57:
	.string	"DeviceToHost"
.LASF98:
	.string	"__ToDo"
.LASF105:
	.string	"wdt_disable"
.LASF5:
	.string	"unsigned int"
.LASF71:
	.string	"USARTtoUSB_Buffer"
.LASF113:
	.string	"CDC_Device_SendByte"
.LASF39:
	.string	"CDC_PARITY_Space"
.LASF8:
	.string	"long unsigned int"
.LASF30:
	.string	"CDC_LineEncodingFormats_t"
.LASF38:
	.string	"CDC_PARITY_Mark"
.LASF97:
	.string	"sreg_save"
.LASF42:
	.string	"size"
.LASF31:
	.string	"CDC_LINEENCODING_OneStopBit"
.LASF106:
	.string	"temp_reg"
.LASF95:
	.string	"LEDs_TurnOnLEDs"
.LASF15:
	.string	"DEVICE_STATE_Configured"
.LASF24:
	.string	"USB_RemoteWakeupEnabled"
.LASF34:
	.string	"CDC_LineEncodingParity_t"
.LASF64:
	.string	"Config"
.LASF84:
	.string	"EVENT_USB_Device_UnhandledControlRequest"
.LASF99:
	.string	"RingBuffer_Insert"
.LASF51:
	.string	"DataOUTEndpointSize"
.LASF16:
	.string	"DEVICE_STATE_Suspended"
.LASF46:
	.string	"ControlInterfaceNumber"
.LASF36:
	.string	"CDC_PARITY_Odd"
.LASF28:
	.string	"USB_ControlRequest"
.LASF101:
	.string	"RingBuffer_IsFull"
.LASF48:
	.string	"DataINEndpointSize"
.LASF107:
	.string	"CDC_Device_ProcessControlRequest"
.LASF50:
	.string	"DataOUTEndpointNumber"
.LASF45:
	.string	"__iob"
.LASF68:
	.string	"Count"
.LASF33:
	.string	"CDC_LINEENCODING_TwoStopBits"
.LASF109:
	.string	"USB_Init"
.LASF53:
	.string	"NotificationEndpointNumber"
.LASF14:
	.string	"DEVICE_STATE_Addressed"
.LASF12:
	.string	"DEVICE_STATE_Powered"
.LASF49:
	.string	"DataINEndpointDoubleBank"
.LASF85:
	.string	"EVENT_USB_Device_ConfigurationChanged"
.LASF80:
	.string	"ReceivedByte"
.LASF83:
	.string	"ConfigMask"
.LASF86:
	.string	"SetupHardware"
.LASF111:
	.string	"CDC_Device_USBTask"
.LASF87:
	.string	"BufferCount"
.LASF25:
	.string	"_Bool"
.LASF1:
	.string	"unsigned char"
.LASF117:
	.string	"__file"
.LASF18:
	.string	"bRequest"
.LASF54:
	.string	"NotificationEndpointSize"
.LASF102:
	.string	"RingBuffer_GetCount"
.LASF93:
	.string	"LEDs_TurnOffLEDs"
.LASF7:
	.string	"uint32_t"
.LASF37:
	.string	"CDC_PARITY_Even"
.LASF112:
	.string	"USB_USBTask"
.LASF92:
	.string	"DoubleSpeed"
.LASF44:
	.string	"char"
.LASF108:
	.string	"CDC_Device_ConfigureEndpoints"
.LASF65:
	.string	"State"
.LASF77:
	.string	"CurrentDTRState"
.LASF70:
	.string	"USBtoUSART_Buffer"
.LASF32:
	.string	"CDC_LINEENCODING_OneAndAHalfStopBits"
.LASF78:
	.string	"EVENT_CDC_Device_ControLineStateChanged"
.LASF114:
	.string	"GNU C99 9.4.0 -mn-flash=1 -mno-skip-bug -mmcu=avr35 -gdwarf-2 -Os -std=gnu99 -funsigned-char -funsigned-bitfields -ffunction-sections -fno-inline-small-functions -fpack-struct -fshort-enums -fno-strict-aliasing"
.LASF74:
	.string	"PingPongLEDPulse"
.LASF62:
	.string	"ControlLineStates"
.LASF75:
	.string	"PulseMSRemaining"
.LASF104:
	.string	"__iRestore"
.LASF103:
	.string	"RingBuffer_InitBuffer"
.LASF69:
	.string	"RingBuff_t"
.LASF115:
	.string	"Arduino-usbserial.c"
.LASF63:
	.string	"LineEncoding"
.LASF116:
	.string	"/Users/damienhu/Documents/Git/arduino-atmega16u2/Projects/arduino-usbserial"
.LASF67:
	.string	"Buffer"
.LASF27:
	.string	"USB_IsInitialized"
.LASF61:
	.string	"DataBits"
.LASF23:
	.string	"USB_ConfigurationNumber"
.LASF2:
	.string	"uint8_t"
.LASF96:
	.string	"Data"
.LASF55:
	.string	"NotificationEndpointDoubleBank"
.LASF41:
	.string	"flags"
.LASF21:
	.string	"wLength"
.LASF82:
	.string	"CDCInterfaceInfo"
.LASF60:
	.string	"ParityType"
.LASF81:
	.string	"EVENT_CDC_Device_LineEncodingChanged"
.LASF90:
	.string	"DataByte"
.LASF118:
	.string	"main"
.LASF88:
	.string	"Serial_TxByte"
.LASF66:
	.string	"USB_ClassInfo_CDC_Device_t"
.LASF56:
	.string	"HostToDevice"
	.ident	"GCC: (Homebrew AVR GCC 9.4.0) 9.4.0"
.global __do_copy_data
.global __do_clear_bss
