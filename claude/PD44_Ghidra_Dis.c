// Function: thunk_FUN_rom_000200

/* WARNING: This function may have set the stack pointer */
/* WARNING: Removing unreachable block (rom,0x000216) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void thunk_FUN_rom_000200(void)

{
  code *UNRECOVERED_JUMPTABLE;
  
  SPLIM = 0x27e0;
  _DAT_ram_085c = 0x20a;
  FUN_rom_000222();
  _DAT_ram_085c = 0x210;
  FUN_rom_000232(0xfbe,0);
  _DAT_ram_085c = 0x21e;
  FUN_rom_000afe();
  break();
  PC = 0;
  UNRECOVERED_JUMPTABLE = (code *)PC;
                    /* WARNING: Could not recover jumptable at 0x000220. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*UNRECOVERED_JUMPTABLE)();
  return;
}





// Function: FUN_rom_000200

/* WARNING: This function may have set the stack pointer */
/* WARNING: Removing unreachable block (rom,0x000216) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_000200(void)

{
  code *UNRECOVERED_JUMPTABLE;
  
  SPLIM = 0x27e0;
  _DAT_ram_085c = 0x20a;
  FUN_rom_000222();
  _DAT_ram_085c = 0x210;
  FUN_rom_000232(0xfbe,0);
  _DAT_ram_085c = 0x21e;
  FUN_rom_000afe();
  break();
  PC = 0;
  UNRECOVERED_JUMPTABLE = (code *)PC;
                    /* WARNING: Could not recover jumptable at 0x000220. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*UNRECOVERED_JUMPTABLE)();
  return;
}





// Function: FUN_rom_000222

undefined2 FUN_rom_000222(void)

{
  uint uVar1;
  
  uVar1 = CORCON;
  CORCON = uVar1 & 0xfffb;
  Ram0034 = 0;
  uVar1 = CORCON;
  CORCON = uVar1 | 4;
  return 0;
}





// Function: FUN_rom_000232

undefined2 FUN_rom_000232(uint param_1,undefined2 param_2)

{
  uint uVar1;
  byte bVar2;
  int iVar3;
  int iVar4;
  undefined4 uVar5;
  undefined1 *puVar6;
  int iVar7;
  bool bVar8;
  
  Ram0032 = param_2;
  iVar4 = 0;
  while( true ) {
    bVar2 = TBLPAG;
    puVar6 = *(undefined1 **)((uint3)bVar2 << 0x10 | (uint3)param_1 & 0xfffffe);
    if (puVar6 == (undefined1 *)0x0) break;
    iVar7 = Ram0032;
    Ram0032 = iVar7 + iVar4 + (uint)(0xfffd < param_1);
    bVar2 = TBLPAG;
    iVar7 = *(int *)((uint3)bVar2 << 0x10 | (uint3)(param_1 + 2) & 0xfffffe);
    uVar1 = param_1 + 4;
    iVar3 = Ram0032;
    Ram0032 = iVar3 + iVar4 + (uint)(0xfffd < param_1 + 2);
    bVar2 = TBLPAG;
    param_1 = param_1 + 6;
    iVar3 = Ram0032;
    Ram0032 = iVar3 + iVar4 + (uint)(0xfffd < uVar1);
    if ((*(uint *)((uint3)bVar2 << 0x10 | (uint3)uVar1 & 0xfffffe) & 0x7f) == 0) {
      do {
        *puVar6 = 0;
        bVar8 = iVar7 != 0;
        iVar7 = iVar7 + -1;
        puVar6 = puVar6 + 1;
      } while (bVar8 && iVar7 != 0);
    }
    else {
      uVar5 = FUN_rom_00026c(iVar4);
      param_1 = (uint)((ulong)uVar5 >> 0x10);
      iVar4 = (int)uVar5;
    }
  }
  return 0;
}





// Function: FUN_rom_00026c

void FUN_rom_00026c(int param_1,uint param_2,undefined1 *param_3,int param_4,int param_5)

{
  undefined1 uVar1;
  uint uVar2;
  undefined1 uVar3;
  int iVar4;
  uint uVar5;
  undefined1 *puVar6;
  int iVar7;
  
  while( true ) {
    uVar1 = TBLPAG;
    uVar5 = param_2 + 2;
    uVar2 = *(uint *)(CONCAT12(uVar1,param_2) & 0xfffffe);
    if ((param_2 & 1) != 0) {
      uVar2 = uVar2 >> 8;
    }
    *param_3 = (char)uVar2;
    if (param_4 == 1) goto LAB_rom_00028a;
    uVar1 = TBLPAG;
    uVar2 = *(uint *)(CONCAT12(uVar1,uVar5) & 0xfffffe);
    if ((uVar5 & 1) != 0) {
      uVar2 = uVar2 >> 8;
    }
    param_3[1] = (char)uVar2;
    iVar7 = param_4 + -2;
    if (iVar7 == 0) break;
    puVar6 = param_3 + 2;
    if (param_5 != 0) {
      uVar3 = TBLPAG;
      uVar1 = 0;
      if ((param_2 & 1) == 0) {
        uVar1 = *(undefined1 *)(CONCAT12(uVar3,param_2) | 1);
      }
      puVar6 = param_3 + 3;
      param_3[2] = uVar1;
      iVar7 = param_4 + -3;
      if (iVar7 == 0) break;
    }
    iVar4 = Ram0032;
    Ram0032 = iVar4 + param_1 + (uint)(0xfffd < param_2);
    param_2 = uVar5;
    param_3 = puVar6;
    param_4 = iVar7;
  }
  uVar5 = param_2 + 1;
LAB_rom_00028a:
  iVar7 = Ram0032;
  Ram0032 = iVar7 + param_1 + (uint)(0xfffe < uVar5);
  return;
}





// Function: FUN_rom_0002b8

void FUN_rom_0002b8(int param_1,undefined1 param_2,int param_3)

{
  int iVar1;
  
  for (iVar1 = 0; iVar1 != param_3; iVar1 = iVar1 + 1) {
    *(undefined1 *)(iVar1 + param_1) = param_2;
  }
  return;
}





// Function: FUN_rom_0004ee

undefined4 FUN_rom_0004ee(void)

{
  byte bVar1;
  uint uVar2;
  
  bVar1 = Peripherals::GPIOD.PORTD._1_1_;
  uVar2 = bVar1 & 4;
  bVar1 = Peripherals::GPIOC.PORTC._1_1_;
  if ((bVar1 & 0x20) != 0) {
    uVar2 = uVar2 + 2;
  }
  bVar1 = Peripherals::GPIOC.PORTC._1_1_;
  if ((bVar1 & 0x40) != 0) {
    uVar2 = uVar2 + 1;
  }
  Peripherals::I2C2.I2C2ADD = uVar2 + 0x58;
  return CONCAT22(uVar2,uVar2 + 0x58);
}





// Function: FUN_rom_00050c

undefined1 FUN_rom_00050c(void)

{
  ushort uVar1;
  byte bVar2;
  undefined1 uVar3;
  
  FUN_rom_0004ee();
  uVar1 = Peripherals::I2C2.I2C2CON;
  Peripherals::I2C2.I2C2CON = uVar1 | 0x40;
  uVar1 = Peripherals::I2C2.I2C2CON;
  Peripherals::I2C2.I2C2CON = uVar1 | 0x1000;
  uVar1 = Peripherals::I2C2.I2C2CON;
  Peripherals::I2C2.I2C2CON = uVar1 | 0x8000;
  bVar2 = Peripherals::INTERRUPT.IPC12._0_1_;
  Peripherals::INTERRUPT.IPC12._0_1_ = bVar2 | 0x70;
  uVar3 = Peripherals::INTERRUPT.IPC12._0_1_;
  uVar3 = Peripherals::INTERRUPT.IPC12._0_1_;
  uVar1 = Peripherals::INTERRUPT.IFS3;
  Peripherals::INTERRUPT.IFS3 = uVar1 & 0xfffd;
  uVar1 = Peripherals::INTERRUPT.IEC3;
  Peripherals::INTERRUPT.IEC3 = uVar1 | 2;
  return 0x70;
}





// Function: FUN_rom_00051e

void FUN_rom_00051e(void)

{
  DAT_ram_0852 = 0;
  FUN_rom_00050c();
  return;
}





// Function: FUN_rom_000836

void FUN_rom_000836(void)

{
  ushort uVar1;
  
  uVar1 = Peripherals::GPIOD.LATD;
  Peripherals::GPIOD.LATD = uVar1 & 0xfffb;
  uVar1 = Peripherals::GPIOD.TRISD;
  Peripherals::GPIOD.TRISD = uVar1 & 0xfffb;
  uVar1 = Peripherals::GPIOC.TRISC;
  Peripherals::GPIOC.TRISC = uVar1 | 0x4000;
  uVar1 = Peripherals::GPIOC.TRISC;
  Peripherals::GPIOC.TRISC = uVar1 | 0x2000;
  uVar1 = Peripherals::GPIOD.TRISD;
  Peripherals::GPIOD.TRISD = uVar1 | 0x400;
  uVar1 = Peripherals::GPIOF.TRISF;
  Peripherals::GPIOF.TRISF = uVar1 | 0x20;
  uVar1 = Peripherals::GPIOF.TRISF;
  Peripherals::GPIOF.TRISF = uVar1 | 0x10;
  uVar1 = Peripherals::GPIOG.LATG;
  Peripherals::GPIOG.LATG = uVar1 | 0x200;
  uVar1 = Peripherals::GPIOG.TRISG;
  Peripherals::GPIOG.TRISG = uVar1 & 0xfdff;
  return;
}





// Function: FUN_rom_00084a

undefined8 FUN_rom_00084a(void)

{
  byte bVar1;
  ushort uVar2;
  undefined1 uVar3;
  
  Peripherals::CLOCK.PLLFBD = 0x30;
  bVar1 = Peripherals::CLOCK.CLKDIV._0_1_;
  Peripherals::CLOCK.CLKDIV._0_1_ = bVar1 & 0xe0 | 2;
  bVar1 = Peripherals::CLOCK.CLKDIV._0_1_;
  Peripherals::CLOCK.CLKDIV._0_1_ = bVar1 & 0x3f;
  Peripherals::CLOCK.OSCCON._1_1_ = 0x78;
  Peripherals::CLOCK.OSCCON._1_1_ = 0x9a;
  Peripherals::CLOCK.OSCCON._1_1_ = 3;
  Peripherals::CLOCK.OSCCON._0_1_ = 0x46;
  Peripherals::CLOCK.OSCCON._0_1_ = 0x57;
  Peripherals::CLOCK.OSCCON._0_1_ = 1;
  do {
    bVar1 = Peripherals::CLOCK.OSCCON._1_1_;
  } while ((bVar1 & 0x70) != 0x30);
  do {
    bVar1 = Peripherals::CLOCK.OSCCON._0_1_;
  } while ((bVar1 & 0x20) == 0);
  uVar2 = Peripherals::CLOCK.ACLKCON;
  Peripherals::CLOCK.ACLKCON = uVar2 & 0xff7f;
  uVar2 = Peripherals::CLOCK.ACLKCON;
  Peripherals::CLOCK.ACLKCON = uVar2 | 0x40;
  uVar2 = Peripherals::CLOCK.ACLKCON;
  Peripherals::CLOCK.ACLKCON = uVar2 | 0x2000;
  bVar1 = Peripherals::CLOCK.ACLKCON._1_1_;
  Peripherals::CLOCK.ACLKCON._1_1_ = bVar1 | 7;
  uVar3 = Peripherals::CLOCK.ACLKCON._1_1_;
  uVar3 = Peripherals::CLOCK.ACLKCON._1_1_;
  uVar2 = Peripherals::CLOCK.ACLKCON;
  Peripherals::CLOCK.ACLKCON = uVar2 | 0x8000;
  do {
    bVar1 = Peripherals::CLOCK.ACLKCON._1_1_;
  } while ((bVar1 & 0x40) == 0);
  return CONCAT26(0x30,(uint6)CONCAT14(bVar1,0x300001));
}





// Function: FUN_rom_00089e

undefined3 FUN_rom_00089e(void)

{
  ushort uVar1;
  undefined1 uVar2;
  uint uVar3;
  
  Peripherals::TIMER1.T1CON = 0;
  Peripherals::TIMER1.TMR1 = 0;
  Peripherals::TIMER1.PR1 = 0xc35;
  uVar2 = Peripherals::INTERRUPT.IPC0._1_1_;
  uVar3 = CONCAT11(0xc,uVar2) & 0xff8f;
  Peripherals::INTERRUPT.IPC0._1_1_ = (byte)uVar3 | 0x10;
  uVar1 = Peripherals::INTERRUPT.IFS0;
  Peripherals::INTERRUPT.IFS0 = uVar1 & 0xfff7;
  uVar1 = Peripherals::INTERRUPT.IEC0;
  Peripherals::INTERRUPT.IEC0 = uVar1 | 8;
  uVar2 = Peripherals::TIMER1.T1CON._0_1_;
  uVar3 = CONCAT11((char)(uVar3 >> 8),uVar2) & 0xffcf | 0x10;
  Peripherals::TIMER1.T1CON._0_1_ = (char)uVar3;
  uVar1 = Peripherals::TIMER1.T1CON;
  Peripherals::TIMER1.T1CON = uVar1 | 0x8000;
  return CONCAT12(0xcf,uVar3);
}





// Function: FUN_rom_0008c4

void FUN_rom_0008c4(void)

{
  FUN_rom_00089e();
  FUN_rom_00051e();
  FUN_rom_000cea();
  return;
}





// Function: FUN_rom_0008d2

undefined2 FUN_rom_0008d2(void)

{
  undefined2 local_2 [2];
  undefined4 uStack_6;
  
  uStack_6 = 0x8de;
  FUN_rom_000e64(local_2);
  return local_2[0];
}





// Function: FUN_rom_0008e4

undefined4 FUN_rom_0008e4(void)

{
  uint uVar1;
  undefined4 uVar2;
  uint uVar3;
  undefined2 uStack_2;
  undefined1 auStack_4 [2052];
  
  uStack_2 = 0;
  uVar2 = 0x1000;
  FUN_rom_0002b8(auStack_4,0,0x800);
  while (uVar1 = (uint)uVar2, uVar3 = (uint)((ulong)uVar2 >> 0x10),
        uVar3 < (uVar1 < 0x5bff) || uVar1 == 0x5bff && uVar3 == uVar1 < 0x5bff) {
    FUN_rom_000e64(auStack_4,0x200,uVar1);
    uVar2 = CONCAT22((int)((ulong)uVar2 >> 0x10) + (uint)(0xfbff < (uint)uVar2),(uint)uVar2 + 0x400)
    ;
    FUN_rom_000f96(auStack_4,&uStack_2);
  }
  return CONCAT22(0xf7f8,uStack_2);
}





// Function: FUN_rom_00094a

undefined2 FUN_rom_00094a(void)

{
  undefined1 *puVar1;
  undefined1 *puVar2;
  int iVar3;
  int iVar4;
  undefined2 uVar5;
  undefined1 *puVar6;
  
  puVar1 = (undefined1 *)FUN_rom_0008d2(0x5bfe,0);
  puVar2 = (undefined1 *)FUN_rom_0008e4();
  iVar3 = FUN_rom_0008d2(0x5bfa,0);
  iVar4 = FUN_rom_0008d2(0xa7fa,0);
  FUN_rom_000f16();
  puVar6 = puVar2;
  if ((puVar1 == puVar2) && (puVar6 = &DAT_ram_1234, iVar3 == iVar4)) {
    FUN_rom_000efe(puVar2);
    FUN_rom_0008d2(0x5bfc,0);
    FUN_rom_000ef6();
    uVar5 = 1;
  }
  else {
    FUN_rom_000efe(puVar6);
    FUN_rom_0008d2(0xa7fc,0);
    FUN_rom_000ef6();
    uVar5 = 0;
  }
  return uVar5;
}





// Function: FUN_rom_0009a4

void FUN_rom_0009a4(void)

{
  int iVar1;
  undefined1 extraout_var;
  undefined1 extraout_var_00;
  
  iVar1 = FUN_rom_00094a();
  if (iVar1 != 0) {
    FUN_rom_000ebe(2);
    FUN_rom_000eba(1);
    FUN_rom_000ec2(CONCAT11(extraout_var,2),0);
    thunk_FUN_rom_001070();
    return;
  }
  FUN_rom_000ebe(3);
  FUN_rom_000eba(1);
  FUN_rom_000ec2(CONCAT11(extraout_var_00,3),0,0);
  thunk_FUN_rom_005c70();
  return;
}





// Function: FUN_rom_0009e8

void FUN_rom_0009e8(void)

{
  byte bVar1;
  ushort uVar2;
  byte extraout_var;
  int iVar3;
  
  bVar1 = Peripherals::CLOCK.RCON._0_1_;
  if ((bVar1 & 1) == 0) {
    bVar1 = Peripherals::CLOCK.RCON._0_1_;
    if ((bVar1 & 2) == 0) goto LAB_rom_000a26;
  }
  FUN_rom_000ebe(0);
  FUN_rom_000eba(0);
  FUN_rom_000ec2((uint)extraout_var << 8,0);
  FUN_rom_000ef6(0);
  FUN_rom_000efe(0);
  FUN_rom_000f06(0);
  FUN_rom_000f0e(0);
  uVar2 = Peripherals::CLOCK.RCON;
  Peripherals::CLOCK.RCON = uVar2 & 0xfffe;
  uVar2 = Peripherals::CLOCK.RCON;
  Peripherals::CLOCK.RCON = uVar2 & 0xfffd;
LAB_rom_000a26:
  Peripherals::CLOCK.RCON = 0;
  iVar3 = FUN_rom_000eb6();
  if (iVar3 == 1) {
    FUN_rom_000836();
    FUN_rom_0008c4();
    return;
  }
  FUN_rom_0009a4();
  return;
}





// Function: FUN_rom_000a36

/* WARNING: Heritage AFTER dead removal. Example location: W1 : 0x000a54 */
/* WARNING: Restarted to delay deadcode elimination for space: ram */

void FUN_rom_000a36(int param_1)

{
  uint uVar1;
  undefined4 in_W3W2;
  uint in_W4;
  int in_W5;
  uint uVar2;
  int iVar3;
  uint *puVar4;
  undefined4 uVar5;
  uint auStack_2 [1024];
  uint uStack_802;
  int iStack_804;
  
  uVar5 = in_W3W2;
  uStack_802 = in_W4;
  iStack_804 = in_W5;
  while( true ) {
    uVar1 = iStack_804 + (int)((ulong)uVar5 >> 0x10) + (uint)CARRY2(uStack_802,(uint)uVar5);
    uVar2 = (uint)((ulong)in_W3W2 >> 0x10);
    if (uVar1 <= uVar2 && uVar1 + ((uint)in_W3W2 < uStack_802 + (uint)uVar5) <= uVar2) break;
    FUN_rom_000e9c((uint)in_W3W2,uVar1,0x200);
    FUN_rom_0002b8(auStack_2,uVar1,0x800);
    iVar3 = ((((int)((ulong)in_W3W2 >> 0x10) - (int)((ulong)uVar5 >> 0x10)) -
             (uint)((uint)in_W3W2 < (uint)uVar5)) * 0x200 | (uint)in_W3W2 - (uint)uVar5 >> 7) +
            param_1;
    puVar4 = auStack_2;
    do {
      FUN_rom_000c68(iVar3);
      iVar3 = iVar3 + 1;
      puVar4 = puVar4 + 0x80;
    } while (puVar4 != &uStack_802);
    FUN_rom_000de6(auStack_2,uVar1,(int)in_W3W2);
    in_W3W2 = CONCAT22((int)((ulong)in_W3W2 >> 0x10) + (uint)(0xfbff < (uint)in_W3W2),
                       (uint)in_W3W2 + 0x400);
  }
  return;
}





// Function: FUN_rom_000afe

void FUN_rom_000afe(void)

{
  byte bVar1;
  undefined1 uVar2;
  undefined1 extraout_var;
  uint uVar3;
  undefined2 extraout_W1;
  
  bVar1 = SPLIM.SR._0_1_;
  SPLIM.SR._0_1_ = bVar1 | 0xe0;
  uVar2 = SPLIM.SR._0_1_;
  uVar2 = SPLIM.SR._0_1_;
  FUN_rom_00084a(0xe0);
  DAT_ram_0858 = 0;
  FUN_rom_0009e8();
  bVar1 = SPLIM.SR._0_1_;
  uVar3 = CONCAT11(extraout_var,bVar1) & 0xff1f;
  SPLIM.SR._0_1_ = bVar1 & 0x1f;
  do {
    FUN_rom_0004ee(uVar3);
    FUN_rom_000a36(8,extraout_W1,0x1000,0,0x4c00,0);
    uVar3 = FUN_rom_0009a4();
  } while( true );
}





// Function: FUN_rom_000b32

uint FUN_rom_000b32(void)

{
  byte bVar1;
  ushort uVar2;
  ushort uVar3;
  
  uVar2 = Peripherals::GPIOG.LATG;
  Peripherals::GPIOG.LATG = uVar2 & 0xfdff;
  uVar2 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = 0xd7;
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  uVar2 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = 0;
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  uVar2 = Peripherals::SPI2.SPI2BUF;
  uVar3 = Peripherals::GPIOG.LATG;
  Peripherals::GPIOG.LATG = uVar3 | 0x200;
  return uVar2 & 0xff;
}





// Function: FUN_rom_000b52

byte FUN_rom_000b52(void)

{
  byte bVar1;
  ushort uVar2;
  
  do {
    DAT_ram_085a = FUN_rom_000b32();
  } while (-1 < DAT_ram_085a);
  uVar2 = Peripherals::GPIOG.LATG;
  Peripherals::GPIOG.LATG = uVar2 & 0xfdff;
  uVar2 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = 0x3d;
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  uVar2 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = 0x2a;
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  uVar2 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = 0x80;
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  uVar2 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = 0xa6;
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  uVar2 = Peripherals::GPIOG.LATG;
  Peripherals::GPIOG.LATG = uVar2 | 0x200;
  return bVar1;
}





// Function: FUN_rom_000c68

void FUN_rom_000c68(void)

{
  byte bVar1;
  ushort uVar2;
  int iVar3;
  ulong in_W1W0;
  bool bVar4;
  
  do {
    DAT_ram_085a = FUN_rom_000b32();
  } while (-1 < DAT_ram_085a);
  uVar2 = Peripherals::GPIOG.LATG;
  Peripherals::GPIOG.LATG = uVar2 & 0xfdff;
  uVar2 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = 0xd2;
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  uVar2 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = (uint)(in_W1W0 >> 8) & 0xff;
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  uVar2 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = (ushort)(in_W1W0 & 0xffff00ff);
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  uVar2 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = 0;
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  uVar2 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = 0;
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  uVar2 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = 0;
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  uVar2 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = 0;
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  uVar2 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = 0;
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  iVar3 = 0;
  do {
    uVar2 = Peripherals::SPI2.SPI2BUF;
    Peripherals::SPI2.SPI2BUF = 0;
    do {
      bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
    } while ((bVar1 & 1) == 0);
    uVar2 = Peripherals::SPI2.SPI2BUF;
    *(char *)((int)((in_W1W0 & 0xffff00ff) >> 0x10) + iVar3) = (char)uVar2;
    bVar4 = iVar3 != 0xff;
    iVar3 = iVar3 + 1;
  } while (bVar4);
  uVar2 = Peripherals::GPIOG.LATG;
  Peripherals::GPIOG.LATG = uVar2 | 0x200;
  return;
}





// Function: FUN_rom_000cea

void FUN_rom_000cea(void)

{
  ushort uVar1;
  
  Peripherals::SPI2.SPI2STAT = 0;
  Peripherals::SPI2.SPI2CON1 = 0x122;
  Peripherals::SPI2.SPI2CON2 = 0;
  uVar1 = Peripherals::SPI2.SPI2STAT;
  Peripherals::SPI2.SPI2STAT = uVar1 | 0x8000;
  DAT_ram_085a = FUN_rom_000b32(0x122);
  if ((DAT_ram_085a & 1) == 0) {
    FUN_rom_000b52();
  }
  return;
}





// Function: FUN_rom_000de6

undefined2 FUN_rom_000de6(undefined3 *param_1)

{
  undefined4 in_W3W2;
  undefined2 uVar1;
  int iVar2;
  bool bVar3;
  uint local_2;
  undefined2 local_4;
  undefined2 local_8;
  
  iVar2 = 0;
  do {
    uVar1 = CONCAT11(*(undefined1 *)((int)param_1 + 3),(char)((uint3)*param_1 >> 0x10));
    FUN_rom_000f4e((int)((ulong)in_W3W2 >> 0x10),(int)in_W3W2,uVar1);
    if (((uint)in_W3W2 & 0x1f) == 0x18) {
      local_4 = (undefined2)((ulong)in_W3W2 >> 0x10);
      local_2 = (uint)in_W3W2;
      local_8 = uVar1;
    }
    if (iVar2 != 0) {
      if ((iVar2 + 1U & 0x3f) == 0) {
        FUN_rom_000f4e(local_4,local_2,local_8);
        FUN_rom_000f36(0x4001);
      }
    }
    in_W3W2 = CONCAT22((int)((ulong)in_W3W2 >> 0x10) + (uint)(0xfffd < (uint)in_W3W2),
                       (uint)in_W3W2 + 2);
    param_1 = param_1 + 1;
    bVar3 = iVar2 != 0x1ff;
    iVar2 = iVar2 + 1;
  } while (bVar3);
  return 0x200;
}





// Function: FUN_rom_000e64

undefined1 * FUN_rom_000e64(undefined1 *param_1,int param_2)

{
  undefined2 uVar1;
  undefined1 extraout_W1byte;
  byte bVar2;
  undefined4 in_W3W2;
  undefined1 *puVar3;
  int iVar4;
  bool bVar5;
  
  puVar3 = param_1;
  for (iVar4 = 0; bVar5 = iVar4 - param_2 < 0,
      bVar5 && !SBORROW2(iVar4,param_2) || !bVar5 && SBORROW2(iVar4,param_2); iVar4 = iVar4 + 1) {
    uVar1 = FUN_rom_000f56((int)((ulong)in_W3W2 >> 0x10),(int)in_W3W2);
    *puVar3 = (char)uVar1;
    bVar2 = (byte)((uint)uVar1 >> 8);
    puVar3[1] = bVar2;
    puVar3[2] = extraout_W1byte;
    param_1 = (undefined1 *)((uint)bVar2 << 8);
    puVar3[3] = 0;
    puVar3 = puVar3 + 4;
    in_W3W2 = CONCAT22((int)((ulong)in_W3W2 >> 0x10) + (uint)(0xfffd < (uint)in_W3W2),
                       (uint)in_W3W2 + 2);
  }
  return param_1;
}





// Function: FUN_rom_000e9c

ulong FUN_rom_000e9c(undefined2 param_1,undefined2 param_2,int param_3)

{
  bool bVar1;
  
  bVar1 = param_3 == 0x200;
  if (bVar1) {
    FUN_rom_000f64();
  }
  return (ulong)CONCAT12(bVar1,(uint)bVar1);
}





// Function: FUN_rom_000eb6

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined2 FUN_rom_000eb6(void)

{
  return _DAT_ram_27fe;
}





// Function: FUN_rom_000eba

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_000eba(undefined2 param_1)

{
  _DAT_ram_27fc = param_1;
  return;
}





// Function: FUN_rom_000ebe

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_000ebe(undefined2 param_1)

{
  _DAT_ram_27fe = param_1;
  return;
}





// Function: FUN_rom_000ec2

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_000ec2(uint param_1,uint param_2,uint param_3)

{
  uint uVar1;
  
  uVar1 = _DAT_ram_27fa & 0xf1;
  if ((char)param_1 != -1) {
    uVar1 = _DAT_ram_27fa & 0xc1 | (param_1 & 3) << 4;
  }
  if ((char)param_2 != -1) {
    uVar1 = uVar1 & 0xff3f | (param_2 & 3) << 6;
  }
  if ((char)param_3 != -1) {
    uVar1 = uVar1 & 0xfffe | param_3 & 1;
  }
  _DAT_ram_27fa = uVar1;
  return;
}





// Function: FUN_rom_000ef2

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined2 FUN_rom_000ef2(void)

{
  return _DAT_ram_27fa;
}





// Function: FUN_rom_000ef6

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_000ef6(undefined2 param_1)

{
  _DAT_ram_27f8 = param_1;
  return;
}





// Function: FUN_rom_000efa

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined2 FUN_rom_000efa(void)

{
  return _DAT_ram_27f8;
}





// Function: FUN_rom_000efe

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_000efe(undefined2 param_1)

{
  _DAT_ram_27f6 = param_1;
  return;
}





// Function: FUN_rom_000f02

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined2 FUN_rom_000f02(void)

{
  return _DAT_ram_27f6;
}





// Function: FUN_rom_000f06

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_000f06(undefined2 param_1)

{
  _DAT_ram_27f4 = param_1;
  return;
}





// Function: FUN_rom_000f0a

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined2 FUN_rom_000f0a(void)

{
  return _DAT_ram_27f4;
}





// Function: FUN_rom_000f0e

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_000f0e(undefined2 param_1)

{
  _DAT_ram_27f2 = param_1;
  return;
}





// Function: FUN_rom_000f12

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined2 FUN_rom_000f12(void)

{
  return _DAT_ram_27f2;
}





// Function: FUN_rom_000f16

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_000f16(undefined2 param_1)

{
  _DAT_ram_27f0 = param_1;
  return;
}





// Function: FUN_rom_000f36

undefined2 FUN_rom_000f36(ushort param_1)

{
  ushort uVar1;
  
  Peripherals::NVM.NVMCON = param_1;
  DISICNT = 6;
  Peripherals::NVM.NVMKEY = 0x55;
  Peripherals::NVM.NVMKEY = 0xaa;
  uVar1 = Peripherals::NVM.NVMCON;
  Peripherals::NVM.NVMCON = uVar1 | 0x8000;
  do {
    uVar1 = Peripherals::NVM.NVMCON;
  } while ((uVar1 & 0x8000) != 0);
  return 0xaa;
}





// Function: FUN_rom_000f4e

void FUN_rom_000f4e(undefined2 param_1,uint param_2,undefined2 param_3,undefined2 param_4)

{
  byte bVar1;
  undefined1 uVar2;
  
  Ram0032 = param_1;
  bVar1 = TBLPAG;
  *(undefined2 *)((uint3)bVar1 << 0x10 | (uint3)param_2 & 0xfffffe) = param_4;
  uVar2 = TBLPAG;
  *(undefined2 *)(CONCAT12(uVar2,param_2) | 1) = param_3;
  return;
}





// Function: FUN_rom_000f56

ulong FUN_rom_000f56(undefined2 param_1,uint param_2)

{
  byte bVar1;
  undefined1 uVar2;
  
  Ram0032 = param_1;
  bVar1 = TBLPAG;
  uVar2 = TBLPAG;
  return (ulong)CONCAT12(*(undefined1 *)(CONCAT12(uVar2,param_2) | 1),
                         *(undefined2 *)((uint3)bVar1 << 0x10 | (uint3)param_2 & 0xfffffe));
}





// Function: FUN_rom_000f64

undefined2 FUN_rom_000f64(undefined2 param_1,uint param_2,ushort param_3)

{
  byte bVar1;
  undefined2 uVar2;
  ushort uVar3;
  
  uVar2 = Ram0032;
  Peripherals::NVM.NVMCON = param_3;
  Ram0032 = param_1;
  bVar1 = TBLPAG;
  *(uint *)((uint3)bVar1 << 0x10 | (uint3)param_2 & 0xfffffe) = param_2;
  DISICNT = 6;
  Peripherals::NVM.NVMKEY = 0x55;
  Peripherals::NVM.NVMKEY = 0xaa;
  uVar3 = Peripherals::NVM.NVMCON;
  Peripherals::NVM.NVMCON = uVar3 | 0x8000;
  do {
    uVar3 = Peripherals::NVM.NVMCON;
  } while ((uVar3 & 0x8000) != 0);
  Ram0032 = uVar2;
  return 0xaa;
}





// Function: FUN_rom_000f84

uint FUN_rom_000f84(byte param_1,uint *param_2)

{
  uint uVar1;
  
  uVar1 = *(uint *)((uint)(byte)(param_1 ^ (byte)*param_2) * 2 + -0x7d3a);
  *param_2 = *param_2 >> 8 ^ uVar1;
  return uVar1;
}





// Function: FUN_rom_000f96

void FUN_rom_000f96(int param_1,undefined2 param_2)

{
  undefined4 in_W3W2;
  uint uVar1;
  uint uVar2;
  uint uVar3;
  bool bVar4;
  
  uVar1 = 0;
  uVar2 = 0;
  while( true ) {
    uVar3 = (uint)((ulong)in_W3W2 >> 0x10);
    if (uVar3 <= uVar2 && uVar3 + (uVar1 < (uint)in_W3W2) <= uVar2) break;
    FUN_rom_000f84(*(undefined1 *)(param_1 + uVar1),param_2);
    bVar4 = 0xfffe < uVar1;
    uVar1 = uVar1 + 1;
    uVar2 = uVar2 + bVar4;
  }
  return;
}





// Function: thunk_FUN_rom_001070

/* WARNING: This function may have set the stack pointer */
/* WARNING: Removing unreachable block (rom,0x001086) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void thunk_FUN_rom_001070(void)

{
  code *UNRECOVERED_JUMPTABLE;
  
  SPLIM = 0x27e0;
  _DAT_ram_1e58 = 0x107a;
  FUN_rom_001092();
  _DAT_ram_1e58 = 0x1080;
  FUN_rom_0010a2(0x5842,0);
  _DAT_ram_1e58 = 0x108e;
  FUN_rom_005a1c();
  break();
  PC = 0;
  UNRECOVERED_JUMPTABLE = (code *)PC;
                    /* WARNING: Could not recover jumptable at 0x001090. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*UNRECOVERED_JUMPTABLE)();
  return;
}





// Function: FUN_rom_001070

/* WARNING: This function may have set the stack pointer */
/* WARNING: Removing unreachable block (rom,0x001086) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_001070(void)

{
  code *UNRECOVERED_JUMPTABLE;
  
  SPLIM = 0x27e0;
  _DAT_ram_1e58 = 0x107a;
  FUN_rom_001092();
  _DAT_ram_1e58 = 0x1080;
  FUN_rom_0010a2(0x5842,0);
  _DAT_ram_1e58 = 0x108e;
  FUN_rom_005a1c();
  break();
  PC = 0;
  UNRECOVERED_JUMPTABLE = (code *)PC;
                    /* WARNING: Could not recover jumptable at 0x001090. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*UNRECOVERED_JUMPTABLE)();
  return;
}





// Function: FUN_rom_001092

undefined2 FUN_rom_001092(void)

{
  uint uVar1;
  
  uVar1 = CORCON;
  CORCON = uVar1 & 0xfffb;
  Ram0034 = 0;
  uVar1 = CORCON;
  CORCON = uVar1 | 4;
  return 0;
}





// Function: FUN_rom_0010a2

undefined2 FUN_rom_0010a2(uint param_1,undefined2 param_2)

{
  uint uVar1;
  byte bVar2;
  int iVar3;
  int iVar4;
  undefined4 uVar5;
  undefined1 *puVar6;
  int iVar7;
  bool bVar8;
  
  Ram0032 = param_2;
  iVar4 = 0;
  while( true ) {
    bVar2 = TBLPAG;
    puVar6 = *(undefined1 **)((uint3)bVar2 << 0x10 | (uint3)param_1 & 0xfffffe);
    if (puVar6 == (undefined1 *)0x0) break;
    iVar7 = Ram0032;
    Ram0032 = iVar7 + iVar4 + (uint)(0xfffd < param_1);
    bVar2 = TBLPAG;
    iVar7 = *(int *)((uint3)bVar2 << 0x10 | (uint3)(param_1 + 2) & 0xfffffe);
    uVar1 = param_1 + 4;
    iVar3 = Ram0032;
    Ram0032 = iVar3 + iVar4 + (uint)(0xfffd < param_1 + 2);
    bVar2 = TBLPAG;
    param_1 = param_1 + 6;
    iVar3 = Ram0032;
    Ram0032 = iVar3 + iVar4 + (uint)(0xfffd < uVar1);
    if ((*(uint *)((uint3)bVar2 << 0x10 | (uint3)uVar1 & 0xfffffe) & 0x7f) == 0) {
      do {
        *puVar6 = 0;
        bVar8 = iVar7 != 0;
        iVar7 = iVar7 + -1;
        puVar6 = puVar6 + 1;
      } while (bVar8 && iVar7 != 0);
    }
    else {
      uVar5 = FUN_rom_0010dc(iVar4);
      param_1 = (uint)((ulong)uVar5 >> 0x10);
      iVar4 = (int)uVar5;
    }
  }
  return 0;
}





// Function: FUN_rom_0010dc

void FUN_rom_0010dc(int param_1,uint param_2,undefined1 *param_3,int param_4,int param_5)

{
  undefined1 uVar1;
  uint uVar2;
  undefined1 uVar3;
  int iVar4;
  uint uVar5;
  undefined1 *puVar6;
  int iVar7;
  
  while( true ) {
    uVar1 = TBLPAG;
    uVar5 = param_2 + 2;
    uVar2 = *(uint *)(CONCAT12(uVar1,param_2) & 0xfffffe);
    if ((param_2 & 1) != 0) {
      uVar2 = uVar2 >> 8;
    }
    *param_3 = (char)uVar2;
    if (param_4 == 1) goto LAB_rom_0010fa;
    uVar1 = TBLPAG;
    uVar2 = *(uint *)(CONCAT12(uVar1,uVar5) & 0xfffffe);
    if ((uVar5 & 1) != 0) {
      uVar2 = uVar2 >> 8;
    }
    param_3[1] = (char)uVar2;
    iVar7 = param_4 + -2;
    if (iVar7 == 0) break;
    puVar6 = param_3 + 2;
    if (param_5 != 0) {
      uVar3 = TBLPAG;
      uVar1 = 0;
      if ((param_2 & 1) == 0) {
        uVar1 = *(undefined1 *)(CONCAT12(uVar3,param_2) | 1);
      }
      puVar6 = param_3 + 3;
      param_3[2] = uVar1;
      iVar7 = param_4 + -3;
      if (iVar7 == 0) break;
    }
    iVar4 = Ram0032;
    Ram0032 = iVar4 + param_1 + (uint)(0xfffd < param_2);
    param_2 = uVar5;
    param_3 = puVar6;
    param_4 = iVar7;
  }
  uVar5 = param_2 + 1;
LAB_rom_0010fa:
  iVar7 = Ram0032;
  Ram0032 = iVar7 + param_1 + (uint)(0xfffe < uVar5);
  return;
}





// Function: FUN_rom_001100

undefined4 FUN_rom_001100(uint param_1,int param_2)

{
  ulong in_W1W0;
  long lVar1;
  
  lVar1 = (in_W1W0 & 0xffff) * (ulong)param_1;
  return CONCAT22(param_2 * (int)in_W1W0 +
                  (int)(in_W1W0 >> 0x10) * param_1 + (int)((ulong)lVar1 >> 0x10),(int)lVar1);
}





// Function: FUN_rom_00110c

undefined4 FUN_rom_00110c(int param_1,uint param_2,int param_3,uint param_4)

{
  uint uVar1;
  undefined4 uVar2;
  bool bVar3;
  
  uVar1 = param_2 ^ param_4;
  if ((int)param_2 < 0) {
    bVar3 = param_1 != 0;
    param_1 = -param_1;
    param_2 = -(uint)bVar3 - param_2;
  }
  if ((int)param_4 < 0) {
    bVar3 = param_3 != 0;
    param_3 = -param_3;
    param_4 = -(uint)bVar3 - param_4;
  }
  uVar2 = FUN_rom_00112a(param_1,param_2,param_3,param_4);
  if ((int)uVar1 < 0) {
    uVar2 = CONCAT22(-(uint)((int)uVar2 != 0) - (int)((ulong)uVar2 >> 0x10),-(int)uVar2);
  }
  return uVar2;
}





// Function: FUN_rom_00112a

void FUN_rom_00112a(uint param_1,uint param_2,uint param_3,uint param_4)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  int iVar6;
  ulong uVar7;
  int iVar8;
  
  uVar7 = 0;
  iVar8 = 0x20;
  do {
    uVar3 = param_1 << 1;
    uVar4 = param_1 & 0x8000;
    uVar1 = (int)uVar7 << 1 | (uint)((param_2 & 0x8000) != 0);
    uVar2 = (int)(uVar7 >> 0x10) << 1 | (uint)((uVar7 & 0x8000) != 0);
    uVar5 = uVar1 - param_3;
    uVar1 = (uint)(uVar1 < param_3);
    iVar6 = (uVar2 - param_4) - uVar1;
    uVar7 = CONCAT22(iVar6,uVar5);
    param_1 = uVar3 | 1;
    if (uVar2 < param_4 || uVar2 < param_4 + uVar1) {
      uVar7 = CONCAT22(iVar6 + param_4 + (uint)CARRY2(uVar5,param_3),uVar5 + param_3);
      param_1 = uVar3;
    }
    iVar8 = iVar8 + -1;
    param_2 = param_2 << 1 | (uint)(uVar4 != 0);
  } while (iVar8 != 0);
  return;
}





// Function: FUN_rom_0013c2

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_0013c2(void)

{
  undefined2 *puVar1;
  bool bVar2;
  
  _DAT_ram_19b0 = &DAT_ram_1bf4;
  _DAT_ram_19b2 = &DAT_ram_1bf2;
  _DAT_ram_19b4 = &DAT_ram_1bee;
  _DAT_ram_19b6 = &DAT_ram_1bec;
  _DAT_ram_19ba = &DAT_ram_1946;
  _DAT_ram_19bc = &DAT_ram_1944;
  _DAT_ram_19be = &DAT_ram_1942;
  _DAT_ram_19d0 = &DAT_ram_1be8;
  _DAT_ram_19d2 = &DAT_ram_1be6;
  _DAT_ram_19d4 = &DAT_ram_1be4;
  _DAT_ram_19d6 = &DAT_ram_19a4;
  _DAT_ram_19d8 = &DAT_ram_194c;
  _DAT_ram_19da = &DAT_ram_194a;
  _DAT_ram_19dc = &DAT_ram_1bd2;
  _DAT_ram_19de = &DAT_ram_1bc8;
  _DAT_ram_19f0 = &DAT_ram_1be2;
  _DAT_ram_19f2 = &DAT_ram_1bde;
  _DAT_ram_19f4 = &DAT_ram_1bc0;
  _DAT_ram_19f6 = &DAT_ram_1bd0;
  _DAT_ram_19f8 = &DAT_ram_1bce;
  _DAT_ram_19fa = &DAT_ram_197a;
  _DAT_ram_1a10 = &DAT_ram_1bea;
  _DAT_ram_1a12 = &DAT_ram_1bb2;
  _DAT_ram_1a14 = &DAT_ram_1bc6;
  _DAT_ram_1a16 = &DAT_ram_1bb6;
  _DAT_ram_1a18 = &DAT_ram_1bb4;
  _DAT_ram_1a1a = &DAT_ram_19a2;
  _DAT_ram_1a1c = &DAT_ram_19a0;
  _DAT_ram_1a1e = &DAT_ram_199e;
  _DAT_ram_1a20 = &DAT_ram_199c;
  _DAT_ram_1a22 = &DAT_ram_199a;
  _DAT_ram_1a24 = &DAT_ram_1998;
  _DAT_ram_1a26 = &DAT_ram_1996;
  _DAT_ram_1a28 = &DAT_ram_1994;
  _DAT_ram_1a2a = &DAT_ram_1bca;
  _DAT_ram_1a30 = &DAT_ram_1bd4;
  _DAT_ram_1a32 = &DAT_ram_19a6;
  _DAT_ram_1a34 = &DAT_ram_1bdc;
  _DAT_ram_1a36 = &DAT_ram_19ae;
  _DAT_ram_1a38 = &DAT_ram_1bda;
  _DAT_ram_1a3a = &DAT_ram_19ac;
  _DAT_ram_1a3c = &DAT_ram_1bd8;
  _DAT_ram_1a3e = &DAT_ram_19aa;
  _DAT_ram_1a40 = &DAT_ram_1bd6;
  _DAT_ram_1a42 = &DAT_ram_19a8;
  _DAT_ram_1a46 = &DAT_ram_1992;
  _DAT_ram_1a48 = &DAT_ram_1990;
  _DAT_ram_1a70 = &DAT_ram_1bbc;
  _DAT_ram_1a72 = &DAT_ram_1bba;
  _DAT_ram_1a74 = &DAT_ram_1bb8;
  _DAT_ram_1a76 = &DAT_ram_198e;
  _DAT_ram_1a78 = &DAT_ram_198c;
  _DAT_ram_1b90 = &DAT_ram_198a;
  _DAT_ram_1b92 = &DAT_ram_1988;
  _DAT_ram_1b94 = &DAT_ram_1986;
  _DAT_ram_1b96 = &DAT_ram_1984;
  _DAT_ram_1b98 = &DAT_ram_1982;
  _DAT_ram_1b9c = &DAT_ram_197e;
  puVar1 = (undefined2 *)&DAT_ram_19b0;
  do {
    *(undefined2 *)*puVar1 = 0;
    bVar2 = puVar1 != (undefined2 *)0x1bae;
    puVar1 = puVar1 + 1;
  } while (bVar2);
  _DAT_ram_1bf4 = 1;
  _DAT_ram_197c = 0x5dc;
  _DAT_ram_19a2 = 0x15c0;
  _DAT_ram_19a0 = 0x21c0;
  _DAT_ram_199e = 0xb00;
  _DAT_ram_199c = 0xd00;
  _DAT_ram_1998 = 0xdc0;
  _DAT_ram_1996 = 0x15c0;
  _DAT_ram_1994 = 0x15c0;
  _DAT_ram_1bca = &Peripherals::TIMER3;
  _DAT_ram_1bc6 = 0x3133;
  _DAT_ram_1bb6 = 0x640;
  _DAT_ram_1bb4 = 0x1180;
  _DAT_ram_1992 = 0x135;
  _DAT_ram_1990 = 0x13b;
  _DAT_ram_198a = 0xef;
  _DAT_ram_1988 = 0xff;
  _DAT_ram_1986 = 0x1f;
  _DAT_ram_1984 = 0x3fff;
  _DAT_ram_1982 = 0x3fff;
  _DAT_ram_197e = 0x1f;
  _DAT_ram_199a = 0x10ab;
  _DAT_ram_194e = 0;
  _DAT_ram_1948 = 0;
  return 0x1f10ab;
}





// Function: FUN_rom_00150c

undefined3 FUN_rom_00150c(void)

{
  ushort uVar1;
  byte bVar2;
  uint uVar3;
  
  FUN_rom_0013c2();
  Peripherals::I2C2.I2C2CON = 0x9040;
  Peripherals::I2C2.I2C2ADD = 0x58;
  bVar2 = Peripherals::INTERRUPT.IPC12._0_1_;
  uVar3 = bVar2 & 0xff8f | 0x30;
  Peripherals::INTERRUPT.IPC12._0_1_ = (char)uVar3;
  Peripherals::INTERRUPT.IFS3 = 0;
  uVar1 = Peripherals::INTERRUPT.IEC3;
  Peripherals::INTERRUPT.IEC3 = uVar1 | 2;
  return CONCAT12(0x8f,uVar3);
}





// Function: FUN_rom_001526

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_001526(void)

{
  uint uVar1;
  ushort uVar2;
  
  if (_DAT_ram_1e40 == 0x21) {
    uVar2 = Peripherals::I2C2.I2C2RCV;
    DAT_ram_1979 = (undefined1)uVar2;
  }
  else {
    uVar2 = Peripherals::I2C2.I2C2RCV;
    (&DAT_ram_1958)[_DAT_ram_1e40] = (char)uVar2;
    _DAT_ram_1e40 = _DAT_ram_1e40 + 1;
  }
  _DAT_ram_1e3e = 0;
  if (DAT_ram_1958 == '\0') {
    if (1 < _DAT_ram_1e40) {
      uVar2 = Peripherals::I2C2.I2C2RCV;
      if (uVar2 == 0x13) {
        _DAT_ram_1940 = _DAT_ram_1940 | 2;
      }
      uVar2 = Peripherals::I2C2.I2C2RCV;
      if (uVar2 == 0x14) {
        _DAT_ram_1940 = _DAT_ram_1940 | 4;
      }
      uVar2 = Peripherals::I2C2.I2C2RCV;
      if (uVar2 == 0x15) {
        _DAT_ram_1940 = _DAT_ram_1940 | 8;
      }
      goto LAB_rom_00156a;
    }
  }
  _DAT_ram_1940 = 0;
LAB_rom_00156a:
  if (_DAT_ram_1e40 == 4) {
    if (DAT_ram_1958 == ';') {
      uVar1 = (uint)DAT_ram_195a * 0x100 + (uint)DAT_ram_195b;
      if (uVar1 < 0xf7ff) {
        _DAT_ram_1922 = uVar1 + 0x800;
      }
      else {
        _DAT_ram_1922 = -0x801;
      }
      _DAT_ram_1928 = _DAT_ram_1928 | 2;
      return;
    }
  }
  else if (_DAT_ram_1e40 == 3) {
    if (DAT_ram_1958 == ' ') {
      if (DAT_ram_195a < 0xb) {
        if (DAT_ram_1959 == '\x02') {
          uVar1 = (uint)DAT_ram_195a;
          if (DAT_ram_195a < 8) {
            _DAT_ram_160c = 1;
          }
          else {
            _DAT_ram_160c = 2;
            uVar1 = uVar1 - 8;
          }
          _DAT_ram_160a = uVar1 << 5;
          _DAT_ram_1928 = _DAT_ram_1928 | 0x20;
          return;
        }
        if (DAT_ram_1959 == '\x03') {
          uVar1 = (uint)DAT_ram_195a;
          if (DAT_ram_195a < 8) {
            _DAT_ram_160c = _DAT_ram_1e40;
          }
          else {
            _DAT_ram_160c = 4;
            uVar1 = uVar1 - 8;
          }
          _DAT_ram_160a = uVar1 << 5;
          _DAT_ram_1928 = _DAT_ram_1928 | 0x20;
        }
      }
    }
  }
  return;
}





// Function: FUN_rom_0015ea

bool FUN_rom_0015ea(int param_1,int param_2)

{
  ushort uVar1;
  int iVar2;
  uint uVar3;
  
  uVar1 = Peripherals::I2C2.I2C2ADD;
  iVar2 = uVar1 * 2;
  for (uVar3 = 0; uVar3 < param_2 - 1U; uVar3 = uVar3 + 1) {
    iVar2 = iVar2 + (uint)*(byte *)(param_1 + uVar3);
  }
  return (char)(*(char *)(param_1 + param_2 + -1) + (char)iVar2) != '\0';
}





// Function: FUN_rom_001616

undefined2 FUN_rom_001616(uint param_1)

{
  ushort uVar1;
  
  if (param_1 == 4) {
    uVar1 = 0xfa;
  }
  else if (param_1 < 4) {
    if (param_1 == 1) {
      uVar1 = 0xfe;
    }
    else {
      if (param_1 != 2) {
        return 1;
      }
      uVar1 = 0xce;
    }
  }
  else if (param_1 == 8) {
    uVar1 = 0x55;
  }
  else {
    if (param_1 != 0x10) {
      return 1;
    }
    uVar1 = 0xaa;
  }
  Peripherals::I2C2.I2C2TRN = uVar1;
  return 0;
}





// Function: FUN_rom_001652

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ulong FUN_rom_001652(void)

{
  ushort uVar1;
  uint uVar2;
  ulong uVar3;
  
  if (_DAT_ram_1e40 < 3) {
    if (_DAT_ram_1e40 != 0) {
      _DAT_ram_1950 = _DAT_ram_1950 | 1;
    }
  }
  else {
    _DAT_ram_192c = _DAT_ram_1e40 - 2;
    if (0xe < _DAT_ram_192c && _DAT_ram_1e40 != 0x11) {
      _DAT_ram_1950 = _DAT_ram_1950 | 2;
    }
  }
  uVar3 = FUN_rom_001616(_DAT_ram_1950);
  if ((char)uVar3 != '\0') {
    if (_DAT_ram_1e3e <= _DAT_ram_192c * 2 && _DAT_ram_192c * 2 - _DAT_ram_1e3e != 0) {
      if ((_DAT_ram_1e3e & 1) == 0) {
        uVar2 = (uint)**(byte **)(&DAT_ram_19b0 +
                                 (uint)(byte)(&DAT_ram_1959)[_DAT_ram_1e3e >> 1] * 2);
      }
      else {
        if ((_DAT_ram_1e3e & 1) != 1) {
          return CONCAT22(_DAT_ram_1e3e,_DAT_ram_1e3e) & 0xffff0001;
        }
        uVar2 = **(uint **)(&DAT_ram_19b0 + (uint)(byte)(&DAT_ram_1959)[_DAT_ram_1e3e >> 1] * 2) >>
                8;
      }
      Peripherals::I2C2.I2C2TRN = uVar2;
      uVar1 = Peripherals::I2C2.I2C2TRN;
      _DAT_ram_1956 = _DAT_ram_1956 + uVar1;
      return CONCAT22(&DAT_ram_19b0,uVar1);
    }
    _DAT_ram_1950 = _DAT_ram_1950 | 2;
    _DAT_ram_1940 = 0;
    uVar3 = CONCAT22(_DAT_ram_1e3e,-_DAT_ram_1956) & 0xffff00ff;
    Peripherals::I2C2.I2C2TRN = -_DAT_ram_1956 & 0xff;
  }
  return uVar3;
}





// Function: FUN_rom_0016bc

undefined4 FUN_rom_0016bc(uint param_1)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  undefined4 uVar4;
  
  param_1 = param_1 & 0xff;
  uVar2 = (uint)(byte)(&DAT_ram_1958)[param_1];
  uVar1 = uVar2 - 5;
  uVar3 = -(uint)(uVar2 < 5);
  if (uVar3 < (uVar1 < 0x38) || uVar2 == 0x3d && -(uint)(uVar1 < 0x38) == (uint)(uVar2 < 5)) {
                    /* WARNING: Could not emulate address calculation at 0x0016ce */
                    /* WARNING: Treating indirect jump as call */
    uVar4 = (*(code *)((int3)(int)uVar1 * 2 + 0x16e2))
                      (uVar1,uVar3,
                       (uint)(byte)(&DAT_ram_1959)[param_1] +
                       (uint)(byte)(&DAT_ram_195a)[param_1] * 0x100);
    return uVar4;
  }
  return CONCAT22(uVar3,uVar1);
}





// Function: FUN_rom_00184a

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_rom_00184a(uint param_1)

{
  byte bVar1;
  byte bVar2;
  uint uVar3;
  TIMER1 *pTVar4;
  uint uVar5;
  int *piVar6;
  undefined2 uVar7;
  TIMER1 *pTVar8;
  
  param_1 = param_1 & 0xff;
  bVar1 = (&DAT_ram_195a)[param_1];
  uVar3 = (uint)(byte)(&DAT_ram_1959)[param_1] + (uint)bVar1 * 0x100;
  bVar2 = (&DAT_ram_1958)[param_1];
  pTVar8 = (TIMER1 *)CONCAT11(bVar1,bVar2);
  if (bVar2 == 0x30) {
    if ((uVar3 & 0xf010) != 0) goto LAB_rom_001948;
    goto LAB_rom_001944;
  }
  if (bVar2 < 0x30) {
    if (bVar2 != 0x13) {
      if (bVar2 < 0x13) {
        if (((bVar2 != 6) && (bVar2 != 7)) && (bVar2 != 5)) goto LAB_rom_001944;
      }
      else if (bVar2 != 0x17) {
        if (bVar2 < 0x17) {
          if (bVar2 == 0x16) goto LAB_rom_0018c8;
        }
        else if ((bVar2 == 0x23) || (bVar2 == 0x24)) goto LAB_rom_0018c8;
        goto LAB_rom_001944;
      }
    }
LAB_rom_0018c8:
    if (uVar3 == 0) goto LAB_rom_001944;
    goto LAB_rom_001948;
  }
  pTVar4 = _DAT_ram_1a1c;
  if (bVar2 == 0x35) {
LAB_rom_00191c:
    pTVar8 = pTVar4;
    if (uVar3 <= pTVar8->TMR1 - 0x100) {
LAB_rom_001944:
      uVar7 = 0;
      goto LAB_rom_00194a;
    }
  }
  else {
    if (bVar2 < 0x35) {
      if (bVar2 == 0x32) {
        if (uVar3 < 0x3134) {
          uVar5 = 0x2fff;
LAB_rom_00190c:
          if (uVar5 < uVar3) goto LAB_rom_001944;
        }
      }
      else if (bVar2 < 0x32) {
        pTVar8 = (TIMER1 *)CONCAT11(bVar1,1);
        if (DAT_ram_15f6 != '\x01') {
          if ((_DAT_ram_1bea & 0x400) == 0) {
            if (uVar3 != 0) goto LAB_rom_001944;
          }
          else if ((uVar3 < 0x100) || (uVar3 == 0x8000)) goto LAB_rom_001944;
        }
      }
      else {
        if (bVar2 == 0x33) {
          uVar5 = 0x640;
        }
        else {
          if (bVar2 != 0x34) goto LAB_rom_001944;
          uVar5 = 0x1180;
        }
        if (uVar3 <= uVar5) {
          uVar5 = 0x3f;
          goto LAB_rom_00190c;
        }
      }
      goto LAB_rom_001948;
    }
    pTVar4 = _DAT_ram_1a20;
    if (bVar2 == 0x37) goto LAB_rom_00191c;
    piVar6 = _DAT_ram_1a1a;
    if ((bVar2 < 0x37) || (piVar6 = _DAT_ram_1a1e, bVar2 == 0x38)) {
      pTVar8 = &Peripherals::TIMER1;
      if (*piVar6 + 0x100U <= uVar3) goto LAB_rom_001944;
    }
    else if ((bVar2 != 0x3d) ||
            ((pTVar8 = (TIMER1 *)(uVar3 & 0xff), pTVar8 != (TIMER1 *)0x0 &&
             ((pTVar8 < (TIMER1 *)0xf0 || (pTVar8 == (TIMER1 *)0xfa)))))) goto LAB_rom_001944;
  }
LAB_rom_001948:
  uVar7 = 1;
LAB_rom_00194a:
  return CONCAT26(pTVar8,CONCAT24(uVar7,CONCAT22(&DAT_ram_1958,uVar3)));
}





// Function: FUN_rom_001982

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* WARNING: Restarted to delay deadcode elimination for space: ram */

ulong FUN_rom_001982(void)

{
  ushort uVar1;
  uint uVar2;
  ulong uVar3;
  
  if (_DAT_ram_1e40 == 3) {
    if (DAT_ram_1959 < 0x10) {
      _DAT_ram_192e = 0;
      goto LAB_rom_0019c2;
    }
  }
  else if (_DAT_ram_1e40 == 4) {
    if ((DAT_ram_195a != 0) && ((uint)DAT_ram_1959 + (uint)DAT_ram_195a < 0x101)) {
      _DAT_ram_192e = 1;
      goto LAB_rom_0019c2;
    }
  }
  else if (_DAT_ram_1e40 < 5) {
    if (_DAT_ram_1e40 != 0) {
      _DAT_ram_1950 = _DAT_ram_1950 | 1;
    }
    goto LAB_rom_0019c2;
  }
  _DAT_ram_1950 = _DAT_ram_1950 | 2;
LAB_rom_0019c2:
  uVar3 = FUN_rom_001616(_DAT_ram_1950);
  if ((char)uVar3 == '\0') {
    return uVar3;
  }
  if (_DAT_ram_192e == 0) {
    if (0xf < _DAT_ram_1e3e) {
      _DAT_ram_1950 = _DAT_ram_1950 | 2;
      Peripherals::I2C2.I2C2TRN = -_DAT_ram_1956 & 0xff;
      return CONCAT22(_DAT_ram_1e3e,-_DAT_ram_1956) & 0xffff00ff;
    }
    uVar2 = (uint)DAT_ram_1959 << 4;
  }
  else {
    if (DAT_ram_195a <= _DAT_ram_1e3e) {
      _DAT_ram_1950 = _DAT_ram_1950 | 2;
      Peripherals::I2C2.I2C2TRN = -_DAT_ram_1956 & 0xff;
      return CONCAT22(_DAT_ram_1e3e,-_DAT_ram_1956) & 0xffff00ff;
    }
    uVar2 = (uint)DAT_ram_1959;
  }
  Peripherals::I2C2.I2C2TRN = (uint)(byte)(&DAT_ram_160e)[uVar2 + _DAT_ram_1e3e];
  uVar1 = Peripherals::I2C2.I2C2TRN;
  _DAT_ram_1956 = uVar1 + _DAT_ram_1956;
  return CONCAT22(&DAT_ram_160e,_DAT_ram_1956);
}





// Function: FUN_rom_001a12

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ushort FUN_rom_001a12(void)

{
  uint uVar1;
  uint uVar2;
  ushort uVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  
  if (_DAT_ram_1e40 < 4) {
    if (_DAT_ram_1e40 != 0) {
      _DAT_ram_1950 = _DAT_ram_1950 | 1;
    }
  }
  else if (0xfe < (DAT_ram_1959 + _DAT_ram_1e40) - 4 && DAT_ram_1959 + _DAT_ram_1e40 != 0x103) {
    _DAT_ram_1950 = _DAT_ram_1950 | 2;
  }
  uVar3 = FUN_rom_001616(_DAT_ram_1950);
  if ((char)uVar3 != '\0') {
    uVar1 = _DAT_ram_1e40 - 3;
    uVar2 = (uint)DAT_ram_1959;
    uVar7 = _DAT_ram_1bf4 & 0x100;
    uVar4 = (uint)((byte)(_DAT_ram_1928 >> 4) & 1);
    uVar5 = 0;
    while (uVar5 < uVar1) {
      if ((uVar7 != 0) || (uVar6 = uVar1, 199 < uVar2 + uVar5)) {
        (&DAT_ram_160e)[uVar2 + uVar5] = (&DAT_ram_195a)[uVar5];
        uVar4 = 1;
        uVar6 = uVar5;
      }
      uVar5 = uVar6 + 1;
    }
    _DAT_ram_1928 = _DAT_ram_1928 & 0xffef | uVar4 << 4;
    if (uVar4 == 0) {
      uVar3 = 0xaa;
    }
    else {
      uVar3 = 0x31;
    }
    _DAT_ram_1950 = _DAT_ram_1950 | 2;
    Peripherals::I2C2.I2C2TRN = uVar3;
  }
  return uVar3;
}





// Function: FUN_rom_001a92

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ushort FUN_rom_001a92(void)

{
  ushort uVar1;
  undefined1 *puVar2;
  undefined1 extraout_var;
  undefined1 *puVar3;
  uint extraout_W2;
  uint uVar4;
  
  if (_DAT_ram_1e40 != 8) {
    if (_DAT_ram_1e40 < 9) {
      if (_DAT_ram_1e40 != 0) {
        _DAT_ram_1950 = _DAT_ram_1950 | 1;
      }
    }
    else {
      _DAT_ram_1950 = _DAT_ram_1950 | 2;
    }
  }
  uVar1 = FUN_rom_001616(_DAT_ram_1950);
  if ((char)uVar1 == '\0') {
    return uVar1;
  }
  _DAT_ram_1950 = _DAT_ram_1950 | 2;
  if (((DAT_ram_1959 != '\x05') || (DAT_ram_195a != -0x5a)) || (DAT_ram_195c != '\x01')) {
    _DAT_ram_1e1a = _DAT_ram_1e1a & 0xfdff;
    FUN_rom_005a88(0x19ff,CONCAT11(extraout_var,0xff),extraout_W2 & 0xff00);
    _DAT_ram_1bbc = FUN_rom_005ab8();
    Peripherals::I2C2.I2C2TRN = 0xce;
    return 0xce;
  }
  puVar2 = &DAT_ram_195b;
  puVar3 = (undefined1 *)CONCAT11(extraout_var,2);
  uVar4 = extraout_W2;
  if (DAT_ram_195b == '\x02') {
    puVar2 = (undefined1 *)(uint)DAT_ram_195d;
    puVar3 = (undefined1 *)(_DAT_ram_1bba >> 8);
    uVar4 = _DAT_ram_1bba;
    if ((puVar2 == puVar3) && (puVar2 = (undefined1 *)0x0, DAT_ram_195e == (char)_DAT_ram_1bba)) {
      _DAT_ram_1e1a = _DAT_ram_1e1a | 0x200;
      FUN_rom_005a88(0xff,0xff,1);
      _DAT_ram_1bbc = FUN_rom_005ab8();
      uVar1 = 0x31;
      goto LAB_rom_001b0e;
    }
  }
  _DAT_ram_1e1a = _DAT_ram_1e1a & 0xfdff;
  FUN_rom_005a88(CONCAT11((char)((uint)puVar2 >> 8),0xff),CONCAT11((char)((uint)puVar3 >> 8),0xff),
                 uVar4 & 0xff00);
  _DAT_ram_1bbc = FUN_rom_005ab8();
  uVar1 = 0xce;
LAB_rom_001b0e:
  Peripherals::I2C2.I2C2TRN = uVar1;
  return uVar1;
}





// Function: FUN_rom_001b2a

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined2 FUN_rom_001b2a(void)

{
  undefined2 uVar1;
  
  if (_DAT_ram_1e40 == 4) {
    if ((_DAT_ram_1e1a & 0x200) == 0) {
      _DAT_ram_1950 = _DAT_ram_1950 | 0x10;
    }
  }
  else if (_DAT_ram_1e40 < 5) {
    if (_DAT_ram_1e40 != 0) {
      _DAT_ram_1950 = _DAT_ram_1950 | 1;
    }
  }
  else {
    _DAT_ram_1950 = _DAT_ram_1950 | 2;
  }
  uVar1 = FUN_rom_001616(_DAT_ram_1950);
  if ((char)uVar1 != '\0') {
    _DAT_ram_1950 = _DAT_ram_1950 | 2;
    if ((DAT_ram_1959 == 'P') && (DAT_ram_195a == 'H')) {
      _DAT_ram_1e1a = _DAT_ram_1e1a | 1;
      Peripherals::I2C2.I2C2TRN = 0x31;
      return 0x31;
    }
    uVar1 = 0xce;
    Peripherals::I2C2.I2C2TRN = 0xce;
  }
  return uVar1;
}





// Function: FUN_rom_001b6a

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ushort FUN_rom_001b6a(void)

{
  byte bVar1;
  ushort uVar2;
  uint uVar3;
  int iVar4;
  uint uVar5;
  undefined1 *puVar6;
  
  if (_DAT_ram_1e40 - 7 < 0x10 || _DAT_ram_1e40 == 0x17) {
    if (DAT_ram_1959 < 0x11) {
      if ((_DAT_ram_1e1a & 0x200) == 0) {
        _DAT_ram_1950 = _DAT_ram_1950 | 0x10;
      }
    }
    else {
LAB_rom_001b86:
      _DAT_ram_1950 = _DAT_ram_1950 | 2;
    }
  }
  else {
    if (0x17 < _DAT_ram_1e40) goto LAB_rom_001b86;
    if (_DAT_ram_1e40 != 0) {
      _DAT_ram_1950 = _DAT_ram_1950 | 1;
    }
  }
  uVar2 = FUN_rom_001616(_DAT_ram_1950);
  bVar1 = DAT_ram_1959;
  if ((char)uVar2 == '\0') {
    return uVar2;
  }
  _DAT_ram_1950 = _DAT_ram_1950 | 2;
  uVar3 = DAT_ram_1959 + 4;
  iVar4 = 0;
  for (uVar5 = 1; uVar5 <= uVar3; uVar5 = uVar5 + 1) {
    iVar4 = iVar4 + (uint)(byte)(&DAT_ram_1958)[uVar5];
  }
  _DAT_ram_1956 = -iVar4 & 0xff;
  if (_DAT_ram_1956 != (byte)(&DAT_ram_195d)[DAT_ram_1959]) {
    Peripherals::I2C2.I2C2TRN = 0xce;
    return 0xce;
  }
  uVar5 = _DAT_ram_1956;
  if (DAT_ram_195c == '\0') {
    if ((_DAT_ram_1e40 < 8) || (DAT_ram_1959 == 0)) {
      uVar2 = 0xce;
      goto LAB_rom_001c3e;
    }
    for (uVar5 = 5; uVar5 <= uVar3; uVar5 = uVar5 + 1) {
      *(undefined1 *)(uVar5 + 0x1709) = (&DAT_ram_1958)[uVar5];
    }
    _DAT_ram_1920 = (uint)DAT_ram_195a * 0x100 + (uint)DAT_ram_195b;
    if (_DAT_ram_1920 < 0xf7ff) {
      _DAT_ram_1920 = _DAT_ram_1920 + 0x800;
    }
    else {
      _DAT_ram_1920 = 0xf7ff;
    }
    uVar5 = _DAT_ram_1920 & 0xff;
    puVar6 = (undefined1 *)(uVar5 + 0x171e);
    _DAT_ram_1608 = (uint)bVar1;
    for (uVar3 = 0; uVar3 < bVar1; uVar3 = uVar3 + 1) {
      _DAT_ram_1930 = uVar3 + uVar5;
      *puVar6 = (&DAT_ram_170e)[uVar3];
      puVar6 = puVar6 + 1;
    }
    uVar5 = _DAT_ram_1930;
    if (_DAT_ram_1930 == 0xff) {
      _DAT_ram_1928 = _DAT_ram_1928 | 1;
    }
  }
  FUN_rom_005a88(0xff,uVar5 & 0xff00);
  uVar2 = 0x31;
LAB_rom_001c3e:
  Peripherals::I2C2.I2C2TRN = uVar2;
  return uVar2;
}





// Function: FUN_rom_001c48

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* WARNING: Restarted to delay deadcode elimination for space: ram */

ulong FUN_rom_001c48(void)

{
  ushort uVar1;
  ulong uVar2;
  
  if (_DAT_ram_1e40 == 5) {
    if (DAT_ram_1959 < 0x11) goto LAB_rom_001c66;
  }
  else if (_DAT_ram_1e40 < 6) {
    if (_DAT_ram_1e40 != 0) {
      _DAT_ram_1950 = _DAT_ram_1950 | 1;
    }
    goto LAB_rom_001c66;
  }
  _DAT_ram_1950 = _DAT_ram_1950 | 2;
LAB_rom_001c66:
  uVar2 = FUN_rom_001616(_DAT_ram_1950);
  if ((char)uVar2 != '\0') {
    if (_DAT_ram_1e3e == DAT_ram_1959) {
      _DAT_ram_1950 = _DAT_ram_1950 | 2;
      Peripherals::I2C2.I2C2TRN = -_DAT_ram_1956 & 0xff;
      return CONCAT22(_DAT_ram_1e3e,-_DAT_ram_1956) & 0xffff00ff;
    }
    Peripherals::I2C2.I2C2TRN = (uint)*(byte *)(_DAT_ram_1e3e + 0x181e);
    uVar1 = Peripherals::I2C2.I2C2TRN;
    _DAT_ram_1956 = uVar1 + _DAT_ram_1956;
    uVar2 = CONCAT22(_DAT_ram_1e3e,_DAT_ram_1956);
  }
  return uVar2;
}





// Function: FUN_rom_001c94

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_001c94(void)

{
  char cVar1;
  uint uVar2;
  
  if (_DAT_ram_1e40 != 2) {
    if (_DAT_ram_1e40 < 3) {
      if (_DAT_ram_1e40 != 0) {
        _DAT_ram_1950 = _DAT_ram_1950 | 8;
      }
    }
    else {
      _DAT_ram_1950 = _DAT_ram_1950 | 2;
    }
  }
  cVar1 = FUN_rom_001616(_DAT_ram_1950);
  if (cVar1 != '\0') {
    _DAT_ram_1950 = _DAT_ram_1950 | 2;
    Peripherals::I2C2.I2C2TRN = 0x31;
    uVar2 = _DAT_ram_1e1a & 0x100;
    if (uVar2 == 0) {
      _DAT_ram_1e1a = _DAT_ram_1e1a | 0x100;
      _DAT_ram_191e = 0x800;
      _DAT_ram_1606 = uVar2;
      FUN_rom_005a88(0x8ff,1);
    }
  }
  return;
}





// Function: FUN_rom_001cd4

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_001cd4(void)

{
  ushort uVar1;
  undefined4 uVar2;
  
  if (_DAT_ram_1e40 == 3) {
    if (DAT_ram_1959 == -0x20) goto LAB_rom_001ce8;
  }
  else if (_DAT_ram_1e40 == 0) goto LAB_rom_001ce8;
  _DAT_ram_1950 = _DAT_ram_1950 | 8;
LAB_rom_001ce8:
  uVar2 = FUN_rom_001616(_DAT_ram_1950);
  if ((char)uVar2 != '\0') {
    if (_DAT_ram_1e3e == 10) {
      Peripherals::I2C2.I2C2TRN = -_DAT_ram_1956 & 0xff;
      _DAT_ram_1950 = _DAT_ram_1950 | 2;
      return 0xa000a;
    }
    Peripherals::I2C2.I2C2TRN = (uint)(byte)(&DAT_ram_1d03)[_DAT_ram_1e3e];
    uVar1 = Peripherals::I2C2.I2C2TRN;
    _DAT_ram_1956 = uVar1 + _DAT_ram_1956;
    uVar2 = CONCAT22(_DAT_ram_1e3e,_DAT_ram_1956);
  }
  return uVar2;
}





// Function: FUN_rom_001d12

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined2 FUN_rom_001d12(void)

{
  undefined2 uVar1;
  
  if (_DAT_ram_1e40 != 5) {
    if (_DAT_ram_1e40 < 6) {
      if (_DAT_ram_1e40 != 0) {
        _DAT_ram_1950 = _DAT_ram_1950 | 1;
      }
    }
    else {
      _DAT_ram_1950 = _DAT_ram_1950 | 2;
    }
  }
  uVar1 = FUN_rom_001616(_DAT_ram_1950);
  if ((char)uVar1 != '\0') {
    _DAT_ram_1950 = _DAT_ram_1950 | 2;
    if ((DAT_ram_1959 == 'P') && (DAT_ram_195a == 'H')) {
      if (DAT_ram_195b == '\x01') {
        _DAT_ram_1bf4 = _DAT_ram_1bf4 | 0x100;
LAB_rom_001d50:
        Peripherals::I2C2.I2C2TRN = 0x31;
        return 0x31;
      }
      if (DAT_ram_195b == '\0') {
        _DAT_ram_1bf4 = _DAT_ram_1bf4 & 0xfeff;
        goto LAB_rom_001d50;
      }
    }
    _DAT_ram_1bf4 = _DAT_ram_1bf4 & 0xfeff;
    uVar1 = 0xce;
    Peripherals::I2C2.I2C2TRN = 0xce;
  }
  return uVar1;
}





// Function: FUN_rom_001d5e

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ushort FUN_rom_001d5e(void)

{
  ushort uVar1;
  int iVar2;
  bool bVar3;
  
  if ((_DAT_ram_1e40 < 2) && (_DAT_ram_1e40 != 0)) {
    _DAT_ram_1950 = _DAT_ram_1950 | 8;
  }
  uVar1 = FUN_rom_001616(_DAT_ram_1950);
  if ((char)uVar1 != '\0') {
    if ((_DAT_ram_1bf4 & 0x100) != 0) {
      if (_DAT_ram_1e40 == 0xb) {
        _DAT_ram_1950 = _DAT_ram_1950 | 4;
        iVar2 = 0;
        do {
          (&DAT_ram_1cfa)[iVar2] = (&DAT_ram_1959)[iVar2];
          bVar3 = iVar2 != 8;
          iVar2 = iVar2 + 1;
        } while (bVar3);
        _DAT_ram_1e18 = _DAT_ram_1e18 | 2;
        uVar1 = 0x31;
      }
      else {
        if (((&DAT_ram_1cf8)[_DAT_ram_1e3e] != '\n') || (*(char *)(_DAT_ram_1e3e + 0x1cf9) != '\r'))
        {
          Peripherals::I2C2.I2C2TRN = (uint)(byte)(&DAT_ram_1cfa)[_DAT_ram_1e3e];
          uVar1 = Peripherals::I2C2.I2C2TRN;
          _DAT_ram_1956 = _DAT_ram_1956 + uVar1;
          return uVar1;
        }
        _DAT_ram_1950 = _DAT_ram_1950 | 4;
        uVar1 = -_DAT_ram_1956 & 0xff;
      }
      Peripherals::I2C2.I2C2TRN = uVar1;
      return uVar1;
    }
    uVar1 = 0x55;
    Peripherals::I2C2.I2C2TRN = 0x55;
  }
  return uVar1;
}





// Function: FUN_rom_001dc8

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ushort FUN_rom_001dc8(void)

{
  byte bVar1;
  ushort uVar2;
  
  if ((_DAT_ram_1e40 < 2) && (_DAT_ram_1e40 != 0)) {
    _DAT_ram_1950 = _DAT_ram_1950 | 8;
  }
  uVar2 = FUN_rom_001616(_DAT_ram_1950);
  if ((char)uVar2 != '\0') {
    if ((_DAT_ram_1bf4 & 0x100) != 0) {
      if (_DAT_ram_1e40 == 0x16) {
        DAT_ram_15f7 = DAT_ram_1959;
        DAT_ram_15f8 = DAT_ram_195a;
        DAT_ram_15f9 = DAT_ram_195b;
        DAT_ram_15fa = DAT_ram_195c;
        DAT_ram_15e6 = DAT_ram_195d;
        DAT_ram_15e7 = DAT_ram_195e;
        DAT_ram_15e8 = DAT_ram_195f;
        DAT_ram_15e9 = DAT_ram_1960;
        DAT_ram_15ea = DAT_ram_1961;
        DAT_ram_15eb = DAT_ram_1962;
        DAT_ram_15ec = DAT_ram_1963;
        DAT_ram_15ed = DAT_ram_1964;
        DAT_ram_15ee = DAT_ram_1965;
        DAT_ram_15ef = DAT_ram_1966;
        DAT_ram_15f0 = DAT_ram_1967;
        DAT_ram_15f1 = DAT_ram_1968;
        DAT_ram_15f2 = DAT_ram_1969;
        DAT_ram_15f3 = DAT_ram_196a;
        DAT_ram_15f4 = DAT_ram_196b;
        DAT_ram_15f5 = DAT_ram_196c;
        _DAT_ram_1928 = _DAT_ram_1928 | 0x40;
        _DAT_ram_125a = _DAT_ram_125a | 0x40;
        uVar2 = 0x31;
      }
      else {
        if (_DAT_ram_1e3e != 0x14) {
          bVar1 = DAT_ram_15f7;
          if ((((_DAT_ram_1e3e != 0) && (bVar1 = DAT_ram_15f8, _DAT_ram_1e3e != 1)) &&
              (bVar1 = DAT_ram_15f9, _DAT_ram_1e3e != 2)) &&
             (bVar1 = DAT_ram_15fa, _DAT_ram_1e3e != 3)) {
            bVar1 = (&DAT_ram_15e2)[_DAT_ram_1e3e];
          }
          Peripherals::I2C2.I2C2TRN = (uint)bVar1;
          uVar2 = Peripherals::I2C2.I2C2TRN;
          _DAT_ram_1956 = _DAT_ram_1956 + uVar2;
          return uVar2;
        }
        uVar2 = -_DAT_ram_1956 & 0xff;
      }
      _DAT_ram_1950 = _DAT_ram_1950 | 4;
      Peripherals::I2C2.I2C2TRN = uVar2;
      return uVar2;
    }
    uVar2 = 0x55;
    Peripherals::I2C2.I2C2TRN = 0x55;
  }
  return uVar2;
}





// Function: FUN_rom_001e8a

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ushort FUN_rom_001e8a(void)

{
  ushort uVar1;
  uint uVar2;
  
  if (_DAT_ram_1e3e < 0x16) {
    if (DAT_ram_195a == '\0') {
      uVar2 = FUN_rom_0042b0();
      Peripherals::I2C2.I2C2TRN = uVar2 & 0xff;
    }
    else {
      Peripherals::I2C2.I2C2TRN = 0;
    }
    uVar1 = Peripherals::I2C2.I2C2TRN;
    _DAT_ram_1956 = _DAT_ram_1956 + uVar1;
    return uVar1;
  }
  _DAT_ram_1950 = _DAT_ram_1950 | 2;
  Peripherals::I2C2.I2C2TRN = -_DAT_ram_1956 & 0xffU;
  return -_DAT_ram_1956 & 0xffU;
}





// Function: FUN_rom_001eb0

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ushort FUN_rom_001eb0(void)

{
  ushort uVar1;
  uint uVar2;
  
  if (DAT_ram_1959 == '\x02') {
    if (_DAT_ram_1e3e < 0x1c) {
LAB_rom_001ec8:
      if (DAT_ram_195a < 0xb) {
        uVar2 = FUN_rom_0042b8(_DAT_ram_1e3e);
        Peripherals::I2C2.I2C2TRN = uVar2 & 0xff;
      }
      else {
        Peripherals::I2C2.I2C2TRN = 0;
      }
      uVar1 = Peripherals::I2C2.I2C2TRN;
      _DAT_ram_1956 = _DAT_ram_1956 + uVar1;
      return uVar1;
    }
  }
  else if ((DAT_ram_1959 == '\x03') && (_DAT_ram_1e3e < 0x1d)) goto LAB_rom_001ec8;
  _DAT_ram_1950 = _DAT_ram_1950 | 2;
  Peripherals::I2C2.I2C2TRN = -_DAT_ram_1956 & 0xffU;
  return -_DAT_ram_1956 & 0xffU;
}





// Function: FUN_rom_001eee

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ushort FUN_rom_001eee(void)

{
  ushort uVar1;
  uint uVar2;
  
  if (_DAT_ram_1e3e < 0x18) {
    if (DAT_ram_195a == '\0') {
      uVar2 = FUN_rom_0042c0();
      Peripherals::I2C2.I2C2TRN = uVar2 & 0xff;
    }
    else {
      Peripherals::I2C2.I2C2TRN = 0;
    }
    uVar1 = Peripherals::I2C2.I2C2TRN;
    _DAT_ram_1956 = _DAT_ram_1956 + uVar1;
    return uVar1;
  }
  _DAT_ram_1950 = _DAT_ram_1950 | 2;
  Peripherals::I2C2.I2C2TRN = -_DAT_ram_1956 & 0xffU;
  return -_DAT_ram_1956 & 0xffU;
}





// Function: FUN_rom_001f18

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined2 FUN_rom_001f18(void)

{
  undefined2 uVar1;
  
  if (_DAT_ram_1e40 == 3) {
    if (DAT_ram_1959 == '\x01') goto LAB_rom_001f36;
  }
  else if (_DAT_ram_1e40 < 4) {
    if (_DAT_ram_1e40 != 0) {
      _DAT_ram_1950 = _DAT_ram_1950 | 1;
    }
    goto LAB_rom_001f36;
  }
  _DAT_ram_1950 = _DAT_ram_1950 | 2;
LAB_rom_001f36:
  uVar1 = FUN_rom_001616(_DAT_ram_1950);
  if ((char)uVar1 != '\0') {
    _DAT_ram_1950 = _DAT_ram_1950 | 2;
    if ((_DAT_ram_1bf4 & 0x100) != 0) {
      _DAT_ram_1e1a = _DAT_ram_1e1a | 0x48;
      Peripherals::I2C2.I2C2TRN = 0x31;
      return 0x31;
    }
    uVar1 = 0xaa;
    Peripherals::I2C2.I2C2TRN = 0xaa;
  }
  return uVar1;
}





// Function: FUN_rom_001f54

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined2 FUN_rom_001f54(void)

{
  undefined2 uVar1;
  
  if (_DAT_ram_1e40 == 3) {
    if ((((_DAT_ram_1e18 & 0x100) == 0) && (_DAT_ram_1e22 != 5)) && (DAT_ram_1959 == -0x56))
    goto LAB_rom_001f7a;
  }
  else if (_DAT_ram_1e40 < 4) {
    if (_DAT_ram_1e40 != 0) {
      _DAT_ram_1950 = _DAT_ram_1950 | 1;
    }
    goto LAB_rom_001f7a;
  }
  _DAT_ram_1950 = _DAT_ram_1950 | 2;
LAB_rom_001f7a:
  uVar1 = FUN_rom_001616(_DAT_ram_1950);
  if ((char)uVar1 != '\0') {
    _DAT_ram_1950 = _DAT_ram_1950 | 2;
    _DAT_ram_1e18 = _DAT_ram_1e18 | 0x100;
    _DAT_ram_1bcc = _DAT_ram_1bca;
    uVar1 = 0x31;
    Peripherals::I2C2.I2C2TRN = 0x31;
  }
  return uVar1;
}





// Function: FUN_rom_001f90

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined2 FUN_rom_001f90(void)

{
  undefined2 uVar1;
  
  if (_DAT_ram_1e40 == 3) {
    if (DAT_ram_1959 == '\x01') goto LAB_rom_001fae;
  }
  else if (_DAT_ram_1e40 < 4) {
    if (_DAT_ram_1e40 != 0) {
      _DAT_ram_1950 = _DAT_ram_1950 | 1;
    }
    goto LAB_rom_001fae;
  }
  _DAT_ram_1950 = _DAT_ram_1950 | 2;
LAB_rom_001fae:
  uVar1 = FUN_rom_001616(_DAT_ram_1950);
  if ((char)uVar1 != '\0') {
    _DAT_ram_1950 = _DAT_ram_1950 | 2;
    if ((_DAT_ram_1bf4 & 0x100) != 0) {
      *_DAT_ram_1a10 = 0;
      *_DAT_ram_1a12 = 0;
      DAT_ram_15f6 = 0;
      _DAT_ram_1bea = _DAT_ram_1bea & 0xfff7;
      _DAT_ram_1928 = _DAT_ram_1928 | 0x40;
      Peripherals::I2C2.I2C2TRN = 0x31;
      return 0x31;
    }
    uVar1 = 0xaa;
    Peripherals::I2C2.I2C2TRN = 0xaa;
  }
  return uVar1;
}





// Function: FUN_rom_001fd6

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_001fd6(void)

{
  byte bVar1;
  
  _DAT_ram_1bf0 =
       _DAT_ram_1bf0 & 0xfff1 | _DAT_ram_1e20 >> 3 & 2 | (_DAT_ram_1e20 & 1) << 2 |
       ((_DAT_ram_1e20 >> 1 | _DAT_ram_1e20 >> 3) & 1) << 3;
  if ((_DAT_ram_1e20 & 4) != 0) {
    bVar1 = Peripherals::GPIOD.LATD._0_1_;
    if ((bVar1 & 0x20) != 0) {
      _DAT_ram_1bec = _DAT_ram_1bec | 0x100;
      if ((_DAT_ram_1bea & 0x20) != 0) {
        _DAT_ram_1bf0 = _DAT_ram_1bf0 | 0x40;
      }
      goto LAB_rom_002010;
    }
  }
  _DAT_ram_1bec = _DAT_ram_1bec & 0xfeff;
LAB_rom_002010:
  if ((_DAT_ram_1e1c & 2) == 0) {
    _DAT_ram_1bf2 = _DAT_ram_1bf2 & 0xfffd;
    if ((_DAT_ram_1bea & 0x800) == 0) {
      _DAT_ram_1bf0 = _DAT_ram_1bf0 | 0x10;
    }
  }
  else {
    _DAT_ram_1bf2 = _DAT_ram_1bf2 | 2;
    _DAT_ram_1bf0 = _DAT_ram_1bf0 & 0xffef;
  }
  _DAT_ram_1bbc = FUN_rom_005ab8();
  _DAT_ram_1bba = FUN_rom_005ac0();
  _DAT_ram_1bb8 = FUN_rom_005ac8();
  _DAT_ram_198e = FUN_rom_005ad0();
  _DAT_ram_198c = FUN_rom_005ad8();
  return;
}





// Function: FUN_rom_002042

/* WARNING: Heritage AFTER dead removal. Example location: W1 : 0x00209a */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* WARNING: Restarted to delay deadcode elimination for space: ram */

ulonglong FUN_rom_002042(void)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  
  iVar1 = _DAT_ram_1e22;
  if (_DAT_ram_1e22 == 2) {
    _DAT_ram_1be0 = _DAT_ram_1260 << 2;
    if (((_DAT_ram_1bf4 & 0x100) != 0) || (_DAT_ram_1be2 = _DAT_ram_1be0, _DAT_ram_1d1a == 0)) {
      _DAT_ram_1be2 = _DAT_ram_125e << 2;
    }
    _DAT_ram_1d0e = (uint)((ulong)((long)_DAT_ram_1d1e * 0x1e80) >> 10);
    _DAT_ram_1bde = _DAT_ram_1d0e;
    if (_DAT_ram_1d0e < 0x21) {
      _DAT_ram_1bde = 0x20;
    }
    uVar5 = _DAT_ram_1bde;
    uVar2 = FUN_rom_001100(_DAT_ram_1be0,(int)((ulong)((long)_DAT_ram_1d1e * 0x1e80) >> 0x10),
                           _DAT_ram_1bde);
    iVar3 = (int)((ulong)uVar2 >> 0x10);
    _DAT_ram_1bbe = iVar3 << 2 | (uint)uVar2 >> 0xe;
    if (_DAT_ram_1bbe < 7) {
      _DAT_ram_1bbe = 6;
    }
    uVar2 = FUN_rom_001100(_DAT_ram_1be2,iVar3,uVar5);
    _DAT_ram_1bc0 = (int)((ulong)uVar2 >> 0x10) << 2 | (uint)uVar2 >> 0xe;
    if (_DAT_ram_1bc0 < 7) {
      _DAT_ram_1bc0 = 6;
    }
  }
  else {
    _DAT_ram_1be2 = 0;
    _DAT_ram_1bde = 0;
    _DAT_ram_1bc0 = 0;
    _DAT_ram_1be0 = 0;
    _DAT_ram_1bbe = 0;
  }
  if (((_DAT_ram_1bf2 & 1) != 0) && (iVar1 == 2)) {
    uVar5 = 0xffcc;
    if (_DAT_ram_199e < 0xffcc) {
      uVar5 = _DAT_ram_199e + 0x33;
    }
    uVar4 = 0;
    if (0x33 < _DAT_ram_199c) {
      uVar4 = _DAT_ram_199c - 0x33;
    }
    if (_DAT_ram_1be2 < _DAT_ram_199e) {
      _DAT_ram_1bec = _DAT_ram_1bec | 8;
    }
    else if (((_DAT_ram_1264 & 8) == 0) && (uVar5 <= _DAT_ram_1be2)) {
      _DAT_ram_1bec = _DAT_ram_1bec & 0xfff7;
    }
    if (_DAT_ram_199c < _DAT_ram_1be2) {
      _DAT_ram_1bec = _DAT_ram_1bec | 4;
    }
    else if ((_DAT_ram_1be2 < 0xc99) && (_DAT_ram_1be2 <= uVar4)) {
      _DAT_ram_1bec = _DAT_ram_1bec & 0xfffb;
    }
  }
  uVar2 = CONCAT22(_DAT_ram_199a,(int)((ulong)_DAT_ram_199a * 0x3d >> 0x10) << 10);
  if (((_DAT_ram_1bf2 & 1) != 0) && (iVar1 == 2)) {
    uVar2 = CONCAT22(_DAT_ram_199a,_DAT_ram_1bde);
    if (_DAT_ram_199a < _DAT_ram_1bde) {
      _DAT_ram_1bec = _DAT_ram_1bec | 0x40;
      goto LAB_rom_002146;
    }
    if ((uint)((ulong)_DAT_ram_199a * 0x3d >> 6) < _DAT_ram_1bde) goto LAB_rom_002146;
  }
  _DAT_ram_1bec = _DAT_ram_1bec & 0xffbf;
LAB_rom_002146:
  if ((_DAT_ram_1bf4 & 0x100) == 0) {
    _DAT_ram_197a = 0;
  }
  else {
    _DAT_ram_197a = _DAT_ram_1be0;
  }
  return (ulonglong)CONCAT24(_DAT_ram_1bf4,uVar2) & 0xffff0100ffffffff;
}





// Function: FUN_rom_00215a

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_00215a(void)

{
  IC1 *pIVar1;
  
  _DAT_ram_1bdc = (IC1 *)(_DAT_ram_1e16 * 0x40);
  _DAT_ram_1bda = (IC1 *)(_DAT_ram_1d16 * 0x40);
  _DAT_ram_1bd8 = (IC1 *)(_DAT_ram_1e12 * 0x40);
  _DAT_ram_1bd6 = _DAT_ram_1e46 << 6;
  if (_DAT_ram_1cc0 != 0) {
    return;
  }
  pIVar1 = (IC1 *)0x0;
  if (&Peripherals::IC1 < _DAT_ram_1998) {
    pIVar1 = _DAT_ram_1998 + -0x50;
  }
  if (((_DAT_ram_1e20 & 8) == 0) && (_DAT_ram_1bdc < _DAT_ram_1998)) {
    if (_DAT_ram_1bdc < pIVar1 || (int)_DAT_ram_1bdc - (int)pIVar1 == 0) {
      _DAT_ram_1bec = _DAT_ram_1bec & 0xffef;
    }
  }
  else {
    _DAT_ram_1bec = _DAT_ram_1bec | 0x10;
  }
  pIVar1 = (IC1 *)0x0;
  if (&Peripherals::IC1 < _DAT_ram_1996) {
    pIVar1 = _DAT_ram_1996 + -0x50;
  }
  if (_DAT_ram_1bda < _DAT_ram_1996) {
    if (_DAT_ram_1bda < pIVar1 || (int)_DAT_ram_1bda - (int)pIVar1 == 0) {
      _DAT_ram_194e = _DAT_ram_194e & 0xfeff;
    }
  }
  else {
    _DAT_ram_194e = _DAT_ram_194e | 0x100;
  }
  pIVar1 = (IC1 *)0x0;
  if (&Peripherals::IC1 < _DAT_ram_1994) {
    pIVar1 = _DAT_ram_1994 + -0x50;
  }
  if (_DAT_ram_1bd8 < _DAT_ram_1994) {
    if (_DAT_ram_1bd8 < pIVar1 || (int)_DAT_ram_1bd8 - (int)pIVar1 == 0) {
      _DAT_ram_194e = _DAT_ram_194e & 0xfdff;
    }
  }
  else {
    _DAT_ram_194e = _DAT_ram_194e | 0x200;
  }
  if (((_DAT_ram_1e20 & 2) == 0) && ((_DAT_ram_194e & 0x300) == 0)) {
    _DAT_ram_1bec = _DAT_ram_1bec & 0xffdf;
  }
  else {
    _DAT_ram_1bec = _DAT_ram_1bec | 0x20;
  }
  if ((_DAT_ram_1e20 & 8) == 0) {
    _DAT_ram_1e18 = _DAT_ram_1e18 & 0xfdff;
  }
  else {
    _DAT_ram_1e18 = _DAT_ram_1e18 | 0x200;
  }
  if ((_DAT_ram_1264 & 0x4000) == 0) {
    if (-1 < _DAT_ram_1e1a) {
      _DAT_ram_1bf0 = _DAT_ram_1bf0 & 0xfcff;
      return;
    }
    _DAT_ram_1bf0 = _DAT_ram_1bf0 & 0xfeff;
    _DAT_ram_1bf0 = _DAT_ram_1bf0 | 0x200;
    return;
  }
  _DAT_ram_1bf0 = _DAT_ram_1bf0 & 0xfdff | 0x100;
  return;
}





// Function: FUN_rom_0021fe

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_0021fe(void)

{
  int iVar1;
  
  iVar1 = _DAT_ram_1932;
  if (_DAT_ram_1bf0 != _DAT_ram_1932) {
    _DAT_ram_1932 = _DAT_ram_1bf0;
    _DAT_ram_1e1c = _DAT_ram_1e1c & 0xfff7;
  }
  if (_DAT_ram_1bf0 == 0) {
    _DAT_ram_194e = _DAT_ram_194e & 0xfffe;
  }
  else if (_DAT_ram_1bf0 != 0x10) {
    _DAT_ram_194e = _DAT_ram_194e | 1;
  }
  if (_DAT_ram_1bec == 0) {
    _DAT_ram_194e = _DAT_ram_194e & 0xfffd;
    return CONCAT22(_DAT_ram_1bf0,iVar1);
  }
  _DAT_ram_194e = _DAT_ram_194e | 2;
  return CONCAT22(_DAT_ram_1bf0,iVar1);
}





// Function: FUN_rom_002224

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

uint FUN_rom_002224(void)

{
  long lVar1;
  uint uVar2;
  uint uVar3;
  
  _DAT_ram_1be6 = _DAT_ram_1bf2 & 2;
  uVar2 = _DAT_ram_1be6;
  _DAT_ram_1be8 = _DAT_ram_1be6;
  if ((_DAT_ram_1bf2 & 2) == 0) goto LAB_rom_0022ca;
  _DAT_ram_1be8 = _DAT_ram_1d14;
  uVar2 = _DAT_ram_1d14;
  if (_DAT_ram_1d14 < 0xc81) {
    uVar2 = 0xc80;
  }
  if (_DAT_ram_1bbe < 0x8c) {
    uVar3 = _DAT_ram_1bbe + 0xf;
  }
  else {
    uVar3 = (int)((ulong)_DAT_ram_1bbe * 0x472 >> 0x10) << 6 |
            (uint)((ulong)_DAT_ram_1bbe * 0x472 >> 10) & 0x3f;
  }
  lVar1 = CONCAT22(_DAT_ram_1bbe >> 5,_DAT_ram_1bbe << 0xb);
  isDivideOverflow(lVar1,uVar2);
  _DAT_ram_1be6 = (uint)(lVar1 / (long)(int)uVar2);
  lVar1 = CONCAT22(uVar3 >> 5,uVar3 << 0xb);
  isDivideOverflow(lVar1,uVar2);
  uVar2 = (uint)(lVar1 / (long)(int)uVar2);
  if ((_DAT_ram_1bbe < 0x8c) && ((_DAT_ram_1bf2 & 0x400) == 0)) {
    uVar2 = uVar2 + 7;
  }
  if (_DAT_ram_1d10 < 0xf001) {
    if (_DAT_ram_1d10 < _DAT_ram_1bbe) {
      if (_DAT_ram_1d10 < 0xf) {
LAB_rom_0022a2:
        _DAT_ram_1be4 = _DAT_ram_1d10;
      }
      else {
        _DAT_ram_1be4 = _DAT_ram_1bbe;
      }
    }
    else {
      _DAT_ram_1be4 = uVar3;
      if (_DAT_ram_1d10 <= uVar3) goto LAB_rom_0022a2;
    }
  }
  if ((_DAT_ram_1be6 <= _DAT_ram_1d12) && (_DAT_ram_1be6 = uVar2, _DAT_ram_1d12 <= uVar2)) {
    _DAT_ram_1be6 = _DAT_ram_1d12;
  }
  uVar2 = _DAT_ram_1be4;
  if (_DAT_ram_1be6 < 4) {
    _DAT_ram_1be6 = 4;
  }
LAB_rom_0022ca:
  _DAT_ram_1be4 = uVar2;
  uVar2 = _DAT_ram_1be6;
  if (((_DAT_ram_1bf2 & 2) != 0) && (_DAT_ram_1cc0 == 0)) {
    uVar2 = 0xffff;
    if (_DAT_ram_19a2 < 0xfedf) {
      uVar2 = _DAT_ram_19a2 + 0x120;
    }
    if (_DAT_ram_1be8 < _DAT_ram_19a2) {
      _DAT_ram_1bec = _DAT_ram_1bec | 2;
    }
    else if (uVar2 < _DAT_ram_1be8) {
      _DAT_ram_1bec = _DAT_ram_1bec & 0xfffd;
    }
    uVar3 = 0;
    if (0xa0 < _DAT_ram_19a0) {
      uVar3 = _DAT_ram_19a0 - 0xa0;
    }
    if (_DAT_ram_19a0 < _DAT_ram_1be8) {
      _DAT_ram_1bec = _DAT_ram_1bec | 1;
      return _DAT_ram_19a0;
    }
    uVar2 = _DAT_ram_19a0;
    if (_DAT_ram_1be8 < uVar3) {
      _DAT_ram_1bec = _DAT_ram_1bec & 0xfffe;
    }
  }
  return uVar2;
}





// Function: FUN_rom_00230c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_00230c(void)

{
  ushort uVar1;
  ulong uVar2;
  uint uVar3;
  int iVar4;
  
  _DAT_ram_19ae = _DAT_ram_1e16;
  if (_DAT_ram_1e16 != 0) {
    _DAT_ram_19ae =
         ((int)((ulong)_DAT_ram_1e16 * 0x4a7 >> 0x10) << 8 |
         (uint)((ulong)_DAT_ram_1e16 * 0x4a7 >> 8) & 0xff) + 1;
  }
  _DAT_ram_19a6 = 0;
  uVar3 = 800;
  if (800 < _DAT_ram_1bd4) {
    uVar1 = Peripherals::OC2.OC2RS;
    iVar4 = (int)((ulong)uVar1 * 0x20c4 >> 0x10);
    _DAT_ram_19a6 = iVar4 + 1;
    uVar3 = 0xfe;
    if (0xfd < _DAT_ram_19a6 && iVar4 != 0xfd) {
      _DAT_ram_19a6 = 0xff;
      uVar3 = _DAT_ram_19a6;
    }
  }
  if (_DAT_ram_1d16 == 0) {
    _DAT_ram_19ac = _DAT_ram_1d16;
  }
  else {
    _DAT_ram_19ac =
         ((int)((ulong)_DAT_ram_1d16 * 0x2f1 >> 0x10) << 8 |
         (uint)((ulong)_DAT_ram_1d16 * 0x2f1 >> 8) & 0xff) + 1;
    uVar3 = _DAT_ram_19ac;
  }
  if (_DAT_ram_1e12 == 0) {
    _DAT_ram_19aa = _DAT_ram_1e12;
  }
  else {
    _DAT_ram_19aa =
         ((int)((ulong)_DAT_ram_1e12 * 0x2f1 >> 0x10) << 8 |
         (uint)((ulong)_DAT_ram_1e12 * 0x2f1 >> 8) & 0xff) + 1;
    uVar3 = _DAT_ram_19aa;
  }
  if (_DAT_ram_1e46 != 0) {
    uVar2 = (ulong)_DAT_ram_1e46 * 0x3a8 >> 8;
    _DAT_ram_19a8 = (int)uVar2 + 1;
    return CONCAT22((int)(uVar2 >> 0x10),_DAT_ram_19a8);
  }
  _DAT_ram_19a8 = _DAT_ram_1e46;
  return CONCAT22(_DAT_ram_1e46,uVar3);
}





// Function: FUN_rom_002398

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_002398(void)

{
  byte bVar1;
  uint uVar2;
  undefined1 *puVar3;
  uint uVar4;
  undefined1 *puVar5;
  
  if ((_DAT_ram_1bf2 & 0x402) == 2) {
    if (_DAT_ram_1934 == 1) {
      uVar2 = _DAT_ram_1be8 - 0xaa0;
      uVar4 = 800;
LAB_rom_0023c6:
      if (uVar4 < uVar2) goto LAB_rom_0023ca;
    }
    else {
      if (_DAT_ram_1934 == 2) {
        uVar2 = _DAT_ram_1be8 - 0xd20;
        uVar4 = 0x400;
        goto LAB_rom_0023c6;
      }
      if (_DAT_ram_1934 == 3) {
        uVar2 = _DAT_ram_1be8 + 0xea20;
        uVar4 = 0x10e0;
        goto LAB_rom_0023c6;
      }
LAB_rom_0023ca:
      _DAT_ram_1934 = 0;
    }
    if ((_DAT_ram_1934 == 0) && (0xb3f < _DAT_ram_1be8)) {
      if (_DAT_ram_1be8 < 0xd81) {
        _DAT_ram_1934 = 1;
      }
      else if (_DAT_ram_1be8 < 0x1081) {
        _DAT_ram_1934 = 2;
      }
      else if ((0x167f < _DAT_ram_1be8) && (_DAT_ram_1be8 < 0x2621)) {
        _DAT_ram_1934 = 3;
      }
    }
  }
  else {
    if ((_DAT_ram_1bf2 & 0x402) == 0x402) {
      if (_DAT_ram_1934 == 2) {
        puVar3 = (undefined1 *)(_DAT_ram_1be8 - 0xe60);
        puVar5 = &DAT_ram_19a0;
LAB_rom_00241a:
        if (puVar5 < puVar3) {
          _DAT_ram_1934 = 0;
        }
      }
      else if (_DAT_ram_1934 == 3) {
        puVar3 = (undefined1 *)(_DAT_ram_1be8 + 0xda80);
        puVar5 = (undefined1 *)0xfa0;
        goto LAB_rom_00241a;
      }
      if ((_DAT_ram_1934 == 0) && (0xeff < _DAT_ram_1be8)) {
        if (_DAT_ram_1be8 < 0x26c1) {
          _DAT_ram_1934 = 2;
        }
        else if (_DAT_ram_1be8 < 0x3481) {
          _DAT_ram_1934 = 3;
        }
      }
      bVar1 = (byte)_DAT_ram_1934 | 4;
      goto LAB_rom_00244e;
    }
    _DAT_ram_1934 = 0;
  }
  bVar1 = (byte)_DAT_ram_1934;
LAB_rom_00244e:
  _DAT_ram_1bf2 = CONCAT11(bVar1,DAT_ram_1bf2);
  return;
}





// Function: FUN_rom_002452

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_002452(void)

{
  byte bVar1;
  ushort uVar2;
  
  bVar1 = Peripherals::GPIOD.LATD._0_1_;
  if (((bVar1 & 8) == 0) && ((_DAT_ram_1e1c & 2) != 0)) {
    uVar2 = Peripherals::GPIOD.LATD;
    Peripherals::GPIOD.LATD = uVar2 | 4;
    return;
  }
  uVar2 = Peripherals::GPIOD.LATD;
  Peripherals::GPIOD.LATD = uVar2 & 0xfffb;
  return;
}





// Function: FUN_rom_002464

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_002464(undefined2 param_1,uint param_2)

{
  uint uVar1;
  
  uVar1 = _DAT_ram_1bd2;
  if ((_DAT_ram_1bf2 & 2) == 0) {
    _DAT_ram_1bd2 = 0;
    _DAT_ram_1bc8 = 0;
  }
  else {
    if (_DAT_ram_1bc8 <= _DAT_ram_1be4) {
      _DAT_ram_1bc8 = _DAT_ram_1be4;
    }
    param_2 = _DAT_ram_1be6;
    if (_DAT_ram_1bd2 <= _DAT_ram_1be6) {
      _DAT_ram_1bd2 = _DAT_ram_1be6;
      return CONCAT22(_DAT_ram_1be6,uVar1);
    }
  }
  return CONCAT22(param_2,_DAT_ram_1bd2);
}





// Function: FUN_rom_002486

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_002486(undefined2 param_1,uint param_2)

{
  uint uVar1;
  
  uVar1 = _DAT_ram_1bd0;
  if ((_DAT_ram_1bf2 & 1) == 0) {
    _DAT_ram_1bd0 = 0;
  }
  else {
    param_2 = _DAT_ram_1bde;
    if (_DAT_ram_1bd0 <= _DAT_ram_1bde) {
      _DAT_ram_1bd0 = _DAT_ram_1bde;
      return CONCAT22(_DAT_ram_1bde,uVar1);
    }
  }
  return CONCAT22(param_2,_DAT_ram_1bd0);
}





// Function: FUN_rom_00249c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_00249c(undefined2 param_1,uint param_2)

{
  uint uVar1;
  
  uVar1 = _DAT_ram_1bce;
  if ((_DAT_ram_1bf2 & 1) == 0) {
    _DAT_ram_1bce = 0;
  }
  else {
    param_2 = _DAT_ram_1bc0;
    if (_DAT_ram_1bce <= _DAT_ram_1bc0) {
      _DAT_ram_1bce = _DAT_ram_1bc0;
      return CONCAT22(_DAT_ram_1bc0,uVar1);
    }
  }
  return CONCAT22(param_2,_DAT_ram_1bce);
}





// Function: FUN_rom_0024b2

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_rom_0024b2(uint param_1,uint param_2,uint param_3,uint param_4)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  
  iVar3 = _DAT_ram_194c;
  uVar2 = _DAT_ram_194a;
  uVar4 = _DAT_ram_1936;
  uVar5 = _DAT_ram_1938;
  if ((_DAT_ram_192a & 1) == 0) {
    if ((_DAT_ram_1bf2 & 2) == 0) {
      _DAT_ram_1936 = 0;
      _DAT_ram_1938 = 0;
      _DAT_ram_194a = 0;
      _DAT_ram_194c = 0;
      param_1 = 0;
      _DAT_ram_19a4 = 0;
      uVar4 = _DAT_ram_1936;
      uVar5 = _DAT_ram_1938;
    }
    else {
      param_1 = _DAT_ram_1936 + 1;
      param_2 = _DAT_ram_1938 + (0xfffe < _DAT_ram_1936);
      param_3 = _DAT_ram_1936;
      param_4 = _DAT_ram_1938;
      uVar4 = param_1;
      uVar5 = param_2;
      if (((param_1 < 0x9c3) <= param_2 && (_DAT_ram_1936 != 0x9c2 || param_2 != param_1 < 0x9c3))
         && (uVar4 = param_1, uVar5 = param_2, _DAT_ram_1940 == 0)) {
        param_2 = _DAT_ram_1938 - (_DAT_ram_1936 < 0x9c3);
        param_1 = _DAT_ram_19a4;
        uVar4 = _DAT_ram_1936 - 0x9c3;
        uVar5 = param_2;
        if (_DAT_ram_19a4 != 0xffff) {
          _DAT_ram_19a4 = _DAT_ram_19a4 + 1;
          iVar1 = _DAT_ram_194a + _DAT_ram_1be4;
          _DAT_ram_194c = _DAT_ram_194c + (uint)CARRY2(_DAT_ram_194a,_DAT_ram_1be4);
          _DAT_ram_194a = iVar1;
          _DAT_ram_1936 = _DAT_ram_1936 - 0x9c3;
          _DAT_ram_1938 = param_2;
          return CONCAT26(_DAT_ram_194c,CONCAT24(iVar1,CONCAT22(iVar3,uVar2)));
        }
      }
    }
  }
  _DAT_ram_1938 = uVar5;
  _DAT_ram_1936 = uVar4;
  return CONCAT26(param_2,CONCAT24(param_1,CONCAT22(param_4,param_3)));
}





// Function: FUN_rom_002506

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_002506(void)

{
  ushort uVar1;
  byte bVar2;
  uint uVar3;
  ushort uVar4;
  
  bVar2 = Peripherals::I2C2.I2C2STAT._0_1_;
  if ((bVar2 & 8) == 0) {
    bVar2 = Peripherals::I2C2.I2C2STAT._0_1_;
    if ((bVar2 & 0x10) != 0) {
      bVar2 = Peripherals::I2C2.I2C2STAT._0_1_;
      if ((bVar2 & 4) != 0) {
        _DAT_ram_194e = _DAT_ram_194e & 0xff7f;
      }
    }
  }
  else {
    _DAT_ram_194e = _DAT_ram_194e | 0x80;
  }
  bVar2 = Peripherals::GPIOG.PORTG._0_1_;
  if ((bVar2 & 4) != 0) {
    bVar2 = Peripherals::GPIOG.PORTG._0_1_;
    if ((bVar2 & 8) != 0) {
      if ((_DAT_ram_194e & 0x80) == 0) {
        _DAT_ram_193a = 0;
        goto LAB_rom_002536;
      }
    }
  }
  _DAT_ram_193a = _DAT_ram_193a + 1;
LAB_rom_002536:
  uVar3 = _DAT_ram_193a;
  uVar4 = 9999;
  if (9999 < _DAT_ram_193a) {
    _DAT_ram_193a = 0;
    _DAT_ram_194e = _DAT_ram_194e & 0xff7f;
    uVar4 = Peripherals::INTERRUPT.IEC3;
    Peripherals::INTERRUPT.IEC3 = uVar4 & 0xfffd;
    Peripherals::I2C2.I2C2CON = 0;
    Peripherals::I2C2.I2C2CON = 0x9040;
    uVar4 = Peripherals::I2C2.I2C2RCV;
    uVar1 = Peripherals::GPIOF.TRISF;
    Peripherals::GPIOF.TRISF = uVar1 | 0x10;
    uVar1 = Peripherals::GPIOF.TRISF;
    Peripherals::GPIOF.TRISF = uVar1 | 0x20;
    _DAT_ram_1e40 = 0;
    _DAT_ram_1e3e = 0;
    Peripherals::INTERRUPT.IFS3 = 0;
    uVar1 = Peripherals::INTERRUPT.IEC3;
    Peripherals::INTERRUPT.IEC3 = uVar1 | 2;
  }
  return CONCAT22(uVar3,uVar4);
}





// Function: FUN_rom_00255a

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_00255a(void)

{
  char cVar1;
  
  if ((_DAT_ram_1bf0 & 0x4e) == 0) {
    _DAT_ram_1bf0 = _DAT_ram_1bf0 & 0xfffe;
  }
  else if ((((_DAT_ram_1bea & 0x20) != 0) || ((_DAT_ram_1bf0 & 0x40) == 0)) &&
          (cVar1 = Peripherals::PWM2.IOCON2._1_1_, cVar1 < '\0')) {
    _DAT_ram_1bf0 = _DAT_ram_1bf0 | 1;
  }
  if ((_DAT_ram_1e1c & 0x6000) != 0) {
    _DAT_ram_1bf0 = _DAT_ram_1bf0 | 1;
  }
  return CONCAT22(0x6000,_DAT_ram_1e1c & 0x6000);
}





// Function: FUN_rom_00257e

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int FUN_rom_00257e(void)

{
  uint uVar1;
  
  uVar1 = _DAT_ram_1262;
  _DAT_ram_1bea = _DAT_ram_1bea | 4;
  _DAT_ram_1262 = _DAT_ram_1262 & 0xfffb;
  if (_DAT_ram_1268 == 3) {
    _DAT_ram_1262 = _DAT_ram_1262 | 2;
    return _DAT_ram_1268;
  }
  if (_DAT_ram_1268 == 4) {
    _DAT_ram_1262 = uVar1 & 0xfff9;
  }
  return _DAT_ram_1268;
}





// Function: FUN_rom_002594

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_002594(void)

{
  ushort uVar1;
  uint uVar2;
  
  if (((_DAT_ram_1bf2 & 2) != 0) && (_DAT_ram_1cc0 == 0)) {
    if (((_DAT_ram_194e & 1) == 0) ||
       (((_DAT_ram_1e1c & 8) != 0 || (uVar2 = _DAT_ram_194e, (_DAT_ram_1bea & 1) == 0)))) {
      if (((_DAT_ram_194e & 2) == 0) || ((_DAT_ram_1bea & 2) == 0)) {
        if ((_DAT_ram_1bea & 4) == 0) goto LAB_rom_0025f8;
        if ((_DAT_ram_1262 & 2) == 0) {
          if ((_DAT_ram_1e18 & 0x800) == 0) goto LAB_rom_0025d6;
        }
        else if ((_DAT_ram_1e18 & 0x800) != 0) {
LAB_rom_0025d6:
          _DAT_ram_1262 = _DAT_ram_1262 | 4;
        }
        if ((_DAT_ram_1262 & 4) == 0) goto LAB_rom_0025f4;
        if ((_DAT_ram_1262 & 2) == 0) {
          if ((_DAT_ram_1e18 & 0x800) == 0) goto LAB_rom_0025f4;
        }
        else if ((_DAT_ram_1e18 & 0x800) != 0) goto LAB_rom_0025f4;
        _DAT_ram_1bea = _DAT_ram_1bea & 0xfffb;
        goto LAB_rom_0025f0;
      }
      uVar2 = _DAT_ram_1e22;
      if ((_DAT_ram_1e22 != 2) && (uVar2 = 0, (_DAT_ram_1bec & 0xc) != 0)) goto LAB_rom_0025f0;
    }
    FUN_rom_00257e(uVar2);
  }
LAB_rom_0025f0:
  if ((_DAT_ram_1bea & 4) == 0) {
LAB_rom_0025f8:
    uVar1 = Peripherals::GPIOE.LATE;
    Peripherals::GPIOE.LATE = uVar1 & 0xff7f;
    return;
  }
LAB_rom_0025f4:
  uVar1 = Peripherals::GPIOE.LATE;
  Peripherals::GPIOE.LATE = uVar1 | 0x80;
  return;
}





// Function: FUN_rom_0025fc

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_0025fc(void)

{
  byte bVar1;
  uint uVar2;
  uint uVar3;
  bool bVar4;
  
  _DAT_ram_1952 = 0x5dc;
  if (_DAT_ram_1e22 == 2) {
    if ((_DAT_ram_194e & 4) == 0) {
      _DAT_ram_194e = _DAT_ram_194e | 4;
LAB_rom_002616:
      _DAT_ram_197c = 0x5dc;
    }
  }
  else if ((_DAT_ram_194e & 4) != 0) {
    _DAT_ram_194e = _DAT_ram_194e & 0xfffb;
    goto LAB_rom_002616;
  }
  if (((_DAT_ram_1e18 & 1) != 0) || (uVar3 = _DAT_ram_1bb2, _DAT_ram_1bb2 <= _DAT_ram_1bb0)) {
    uVar3 = _DAT_ram_1bb0 + 0x96;
  }
  if (((_DAT_ram_1e1a & 0x2000) != 0) && (uVar3 < 0x2ee1)) {
    uVar3 = 12000;
  }
  if ((_DAT_ram_1bd4 < 200) || (bVar1 = Peripherals::GPIOD.LATD._0_1_, (bVar1 & 0x20) == 0))
  goto LAB_rom_00268c;
  if (_DAT_ram_1bd4 < uVar3) {
    if (uVar3 - _DAT_ram_1bd4 < 1000) {
      uVar3 = _DAT_ram_193c + 1;
      bVar4 = _DAT_ram_193c == 0x30;
      _DAT_ram_193c = uVar3;
      if (uVar3 < 0x31 || bVar4) goto LAB_rom_00268c;
    }
    else {
      uVar3 = _DAT_ram_193c + 1;
      bVar4 = _DAT_ram_193c == 8;
      _DAT_ram_193c = uVar3;
      if (uVar3 < 9 || bVar4) goto LAB_rom_00268c;
    }
    _DAT_ram_193c = 0;
    _DAT_ram_1e3c = _DAT_ram_1e3c + 1;
  }
  else {
    if (_DAT_ram_1bd4 - uVar3 < 1000) {
      uVar3 = _DAT_ram_193c + 1;
      bVar4 = _DAT_ram_193c == 0x30;
      _DAT_ram_193c = uVar3;
      if (uVar3 < 0x31 || bVar4) goto LAB_rom_00268c;
    }
    else {
      uVar3 = _DAT_ram_193c + 1;
      bVar4 = _DAT_ram_193c == 8;
      _DAT_ram_193c = uVar3;
      if (uVar3 < 9 || bVar4) goto LAB_rom_00268c;
    }
    _DAT_ram_193c = 0;
    _DAT_ram_1e3c = _DAT_ram_1e3c - 1;
  }
LAB_rom_00268c:
  FUN_rom_005312(&DAT_ram_1e3c);
  uVar2 = _DAT_ram_1e3c;
  uVar3 = 2000;
  if (2000 < _DAT_ram_1e3c) {
    uVar3 = 0x7d1;
    _DAT_ram_1e3c = uVar3;
  }
  return CONCAT22(uVar2,uVar3);
}





// Function: FUN_rom_0026a0

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_0026a0(undefined2 param_1,undefined2 param_2)

{
  uint uVar1;
  uint uVar2;
  
  if ((_DAT_ram_1bea & 8) == 0) {
    _DAT_ram_1bf2 = _DAT_ram_1bf2 & 0xff7f;
  }
  else if ((_DAT_ram_1e18 & 1) == 0) {
    uVar1 = _DAT_ram_1e54;
    uVar2 = _DAT_ram_1e3c;
    if (_DAT_ram_1e54 <= _DAT_ram_1e3c) {
      uVar1 = _DAT_ram_1bb2;
      uVar2 = _DAT_ram_1bb0;
      if (_DAT_ram_1bb0 < _DAT_ram_1bb2) {
        _DAT_ram_1bf2 = _DAT_ram_1bf2 & 0xff7f;
        return CONCAT22(_DAT_ram_1bb0,_DAT_ram_1bb2);
      }
    }
    _DAT_ram_1bf2 = _DAT_ram_1bf2 | 0x80;
    return CONCAT22(uVar2,uVar1);
  }
  return CONCAT22(param_2,param_1);
}





// Function: FUN_rom_0026d6

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_0026d6(void)

{
  if ((((_DAT_ram_1bf0 & 1) != 0) || ((_DAT_ram_1e1a & 0xa0) != 0)) ||
     (DAT_ram_1bef = 0, (_DAT_ram_1e20 & 0x5c0) != 0)) {
    DAT_ram_1bef = 0x80;
  }
  if ((_DAT_ram_1bf0 & 2) != 0) {
    DAT_ram_1bee = 1;
    return;
  }
  if ((_DAT_ram_1e20 & 0x80) != 0) {
    DAT_ram_1bee = 2;
    return;
  }
  if ((_DAT_ram_1bf0 & 0x100) == 0) {
    if ((_DAT_ram_1bf0 & 0x200) == 0) {
      if ((_DAT_ram_1e1a & 0x20) != 0) goto LAB_rom_00270a;
      if ((_DAT_ram_1e1a & 0x80) == 0) {
        if ((_DAT_ram_1bf0 & 0x1000) != 0) {
          DAT_ram_1bee = 7;
          return;
        }
        if ((_DAT_ram_1bf0 & 0x2000) != 0) {
          DAT_ram_1bee = 8;
          return;
        }
        if ((_DAT_ram_1bf0 & 0x40) != 0) {
          DAT_ram_1bee = 0xe;
          return;
        }
        if (_DAT_ram_1e22 == 5) {
          DAT_ram_1bee = 0x14;
          return;
        }
        if ((_DAT_ram_1e20 & 0x540) != 0) {
          DAT_ram_1bee = 0xf;
          return;
        }
        if ((_DAT_ram_1bf0 & 0x10) == 0) {
          DAT_ram_1bee = 0;
          return;
        }
        DAT_ram_1bee = 0xb;
        return;
      }
    }
    DAT_ram_1bee = 5;
  }
  else {
LAB_rom_00270a:
    DAT_ram_1bee = 4;
  }
  DAT_ram_1bef = DAT_ram_1bef;
  return;
}





// Function: FUN_rom_002746

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_002746(void)

{
  if (_DAT_ram_193e != _DAT_ram_1bec) {
    if (((_DAT_ram_193e & 0x10) != (_DAT_ram_1bec & 0x10)) && ((_DAT_ram_1bec & 0x10) != 0)) {
      if (DAT_ram_1946 != -1) {
        DAT_ram_1946 = DAT_ram_1946 + '\x01';
      }
    }
    if (((_DAT_ram_193e & 0x20) != (_DAT_ram_1bec & 0x20)) && ((_DAT_ram_1bec & 0x20) != 0)) {
      if (DAT_ram_1947 != -1) {
        DAT_ram_1947 = DAT_ram_1947 + '\x01';
      }
    }
    if (((_DAT_ram_193e & 0x40) != (_DAT_ram_1bec & 0x40)) && ((_DAT_ram_1bec & 0x40) != 0)) {
      if (DAT_ram_1944 != -1) {
        DAT_ram_1944 = DAT_ram_1944 + '\x01';
      }
    }
    if (((_DAT_ram_193e & 0x80) != (_DAT_ram_1bec & 0x80)) && ((_DAT_ram_1bec & 0x80) != 0)) {
      if (DAT_ram_1945 != -1) {
        DAT_ram_1945 = DAT_ram_1945 + '\x01';
      }
    }
    if (((_DAT_ram_193e & 0x100) != (_DAT_ram_1bec & 0x100)) && ((_DAT_ram_1bec & 0x100) != 0)) {
      if (DAT_ram_1942 != -1) {
        DAT_ram_1942 = DAT_ram_1942 + '\x01';
      }
    }
    _DAT_ram_193e = _DAT_ram_1bec;
  }
  return;
}





// Function: FUN_rom_0027ce

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ushort FUN_rom_0027ce(void)

{
  undefined1 uVar1;
  byte bVar2;
  char cVar3;
  uint uVar4;
  int iVar5;
  ushort uVar6;
  uint uVar7;
  byte *pbVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  byte *pbVar11;
  bool bVar12;
  
  if (_DAT_ram_1e40 - 5 < 0x18 || _DAT_ram_1e40 == 0x1d) {
    uVar7 = 1;
    uVar4 = 2;
    do {
      if (_DAT_ram_1e40 == uVar4 + 3) {
        uVar7 = 0;
      }
      bVar12 = uVar4 != 0x1a;
      uVar4 = uVar4 + 3;
    } while (bVar12);
    _DAT_ram_1950 = _DAT_ram_1950 & 0xfffe | uVar7;
    if (uVar7 == 0) {
      pbVar8 = &DAT_ram_93b8;
      do {
        bVar2 = (&DAT_ram_1958)[*pbVar8];
        if (bVar2 < 0x18) {
          if ((0x15 < bVar2) || ((4 < bVar2 && ((bVar2 < 8 || (bVar2 == 0x13))))))
          goto LAB_rom_00282a;
LAB_rom_00282e:
          iVar5 = 1;
        }
        else {
          if ((bVar2 < 0x23) || ((0x24 < bVar2 && (0xc < (byte)(bVar2 - 0x30) && bVar2 != 0x3d))))
          goto LAB_rom_00282e;
LAB_rom_00282a:
          iVar5 = 0;
        }
        _DAT_ram_1950 = _DAT_ram_1950 & 0xfffd | iVar5 * 2;
      } while ((*pbVar8 + 4 < _DAT_ram_1e40) && (pbVar8 = pbVar8 + 1, iVar5 == 0));
    }
  }
  else if (_DAT_ram_1e40 < 0x1e) {
    if (_DAT_ram_1e40 != 0) {
      _DAT_ram_1950 = _DAT_ram_1950 | 1;
    }
  }
  else {
    _DAT_ram_1950 = _DAT_ram_1950 | 2;
  }
  uVar6 = FUN_rom_001616(_DAT_ram_1950);
  uVar4 = _DAT_ram_1e40;
  if ((char)uVar6 != '\0') {
    uVar9 = 0x93b80000;
    do {
      uVar1 = *(undefined1 *)((ulong)uVar9 >> 0x10);
      uVar10 = CONCAT31((int3)((ulong)uVar9 >> 8),uVar1);
      cVar3 = FUN_rom_00184a(uVar1);
      if (cVar3 != '\0') {
        _DAT_ram_1950 = _DAT_ram_1950 | 2;
      }
      uVar9 = CONCAT22((int)((ulong)uVar10 >> 0x10) + 1,(uint)uVar10);
    } while (((uint)uVar10 & 0xff) + 4 < uVar4);
    uVar6 = 0xce;
    if ((_DAT_ram_1950 & 2) == 0) {
      pbVar8 = &DAT_ram_93b8;
      do {
        pbVar11 = pbVar8 + 2;
        uVar4 = (uint)*pbVar8;
        FUN_rom_0016bc(*pbVar8);
        pbVar8 = pbVar11;
      } while (uVar4 + 4 < _DAT_ram_1e40);
      uVar6 = 0x31;
    }
    Peripherals::I2C2.I2C2TRN = uVar6;
    _DAT_ram_1950 = _DAT_ram_1950 | 2;
  }
  return uVar6;
}





// Function: FUN_rom_00289a

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_00289a(void)

{
  char cVar1;
  
  if (_DAT_ram_1e40 != 4) {
    if (_DAT_ram_1e40 < 5) {
      if (_DAT_ram_1e40 != 0) {
        _DAT_ram_1950 = _DAT_ram_1950 | 1;
      }
    }
    else {
      _DAT_ram_1950 = _DAT_ram_1950 | 2;
    }
  }
  cVar1 = FUN_rom_001616(_DAT_ram_1950);
  if (cVar1 != '\0') {
    if (DAT_ram_1959 == '\x01') {
      FUN_rom_001eee();
      return;
    }
    if (DAT_ram_1959 == '\0') {
      FUN_rom_001e8a();
      return;
    }
    if ((DAT_ram_1959 == '\x02') || (DAT_ram_1959 == '\x03')) {
      FUN_rom_001eb0();
      return;
    }
    Peripherals::I2C2.I2C2TRN = 0;
  }
  return;
}





// Function: FUN_rom_0028d6

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ushort FUN_rom_0028d6(void)

{
  uint uVar1;
  ushort uVar2;
  
  if (_DAT_ram_1e40 != 0) {
    _DAT_ram_1956 = 0;
    _DAT_ram_1950 = 0;
    uVar1 = FUN_rom_0015ea(&DAT_ram_1958);
    _DAT_ram_1950 = _DAT_ram_1950 & 0xfffb | (uVar1 & 1) << 2;
  }
  uVar2 = 0xfa;
  if ((_DAT_ram_1950 & 4) == 0) {
    if (DAT_ram_1958 == 0x24) {
      uVar2 = FUN_rom_001d12();
      goto LAB_rom_0029aa;
    }
    if (DAT_ram_1958 < 0x24) {
      if (DAT_ram_1958 == 0x12) {
        uVar2 = FUN_rom_001f90();
        goto LAB_rom_0029aa;
      }
      if (DAT_ram_1958 < 0x12) {
        if (DAT_ram_1958 == 1) {
          uVar2 = FUN_rom_0027ce();
          goto LAB_rom_0029aa;
        }
        if (DAT_ram_1958 == 0) {
          uVar2 = FUN_rom_001652();
          goto LAB_rom_0029aa;
        }
        if (DAT_ram_1958 == 0x11) {
          uVar2 = FUN_rom_001f54();
          goto LAB_rom_0029aa;
        }
      }
      else {
        if (DAT_ram_1958 == 0x21) {
          uVar2 = FUN_rom_001f18();
          goto LAB_rom_0029aa;
        }
        if (DAT_ram_1958 < 0x21) {
          if (DAT_ram_1958 == 0x20) {
            uVar2 = FUN_rom_00289a();
            goto LAB_rom_0029aa;
          }
        }
        else {
          if (DAT_ram_1958 == 0x22) {
            uVar2 = FUN_rom_001982();
            goto LAB_rom_0029aa;
          }
          if (DAT_ram_1958 == 0x23) {
            uVar2 = FUN_rom_001a12();
            goto LAB_rom_0029aa;
          }
        }
      }
    }
    else {
      if (DAT_ram_1958 == 0x3d) {
        uVar2 = FUN_rom_001c94();
        goto LAB_rom_0029aa;
      }
      if (DAT_ram_1958 < 0x3d) {
        if (DAT_ram_1958 == 0x3b) {
          uVar2 = FUN_rom_001c48();
          goto LAB_rom_0029aa;
        }
        if (0x3a < DAT_ram_1958) {
          uVar2 = FUN_rom_001b2a();
          goto LAB_rom_0029aa;
        }
        if (DAT_ram_1958 == 0x3a) {
          uVar2 = FUN_rom_001b6a();
          goto LAB_rom_0029aa;
        }
      }
      else {
        if (DAT_ram_1958 == 0xe2) {
          uVar2 = FUN_rom_001d5e();
          goto LAB_rom_0029aa;
        }
        if (DAT_ram_1958 < 0xe2) {
          if (DAT_ram_1958 == 0xe0) {
            uVar2 = FUN_rom_001cd4();
            goto LAB_rom_0029aa;
          }
        }
        else {
          if (DAT_ram_1958 == 0xe3) {
            uVar2 = FUN_rom_001dc8();
            goto LAB_rom_0029aa;
          }
          if (DAT_ram_1958 == 0xff) {
            uVar2 = FUN_rom_001a92();
            goto LAB_rom_0029aa;
          }
        }
      }
    }
    uVar2 = 0x55;
  }
  Peripherals::I2C2.I2C2TRN = uVar2;
LAB_rom_0029aa:
  _DAT_ram_1e40 = 0;
  _DAT_ram_1e3e = _DAT_ram_1e3e + 1;
  return uVar2;
}





// Function: FUN_rom_0029fa

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined2 FUN_rom_0029fa(void)

{
  undefined2 uVar1;
  
  FUN_rom_002452();
  FUN_rom_00230c();
  FUN_rom_002746();
  FUN_rom_0025fc();
  FUN_rom_0026a0();
  FUN_rom_002398();
  uVar1 = 0x5db;
  if (_DAT_ram_197c < 0x5dc) {
    uVar1 = 0x5dc;
    _DAT_ram_1bb0 = 0x5dc;
  }
  else {
    _DAT_ram_1bb0 = _DAT_ram_197c;
  }
  if ((_DAT_ram_192a & 1) == 0) {
    FUN_rom_001fd6();
    FUN_rom_002042();
    FUN_rom_002224();
    FUN_rom_00215a();
    FUN_rom_0021fe();
    FUN_rom_002464();
    FUN_rom_002486();
    FUN_rom_00249c();
    FUN_rom_00255a();
    uVar1 = FUN_rom_002594();
  }
  return uVar1;
}





// Function: FUN_rom_002a30

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* WARNING: Restarted to delay deadcode elimination for space: ram */

void FUN_rom_002a30(void)

{
  if ((_DAT_ram_125a & 0x40) != 0) {
    _DAT_ram_125a = _DAT_ram_125a & 0xffbf;
    if ((((DAT_ram_15ee == 0) && (DAT_ram_15ef == ' ')) && (DAT_ram_15f0 == 0)) &&
       (DAT_ram_15f1 == 0)) {
      _DAT_ram_1d24 = 0;
    }
    else {
      _DAT_ram_1d24 = ((int)((uint)DAT_ram_15f1 * 0x100 + (uint)DAT_ram_15f0) / 0x7a & 0xffU) + 1;
    }
    _DAT_ram_1e44 = CONCAT11(0x15,DAT_ram_15f3) * 0x100 + (uint)DAT_ram_15f2;
    if (_DAT_ram_1e44 - 0x321 < 0x9e || _DAT_ram_1e44 == 0x3bf) {
      Peripherals::PWM5.PDC5 = _DAT_ram_1e44;
    }
    _DAT_ram_1e42 = CONCAT11(0x15,DAT_ram_15f5) * 0x100 + (uint)DAT_ram_15f4;
    _DAT_ram_1d32 = CONCAT11(0x15,DAT_ram_15e7) * 0x100 + (uint)DAT_ram_15e6;
    _DAT_ram_1d30 = ((int)((uint)DAT_ram_15e9 * 0x100 + (uint)DAT_ram_15e8) >> 4) + 1;
    _DAT_ram_1d2e = CONCAT11(0x15,DAT_ram_15eb) * 0x100 + (uint)DAT_ram_15ea;
    _DAT_ram_1d2c = ((int)((uint)DAT_ram_15ed * 0x100 + (uint)DAT_ram_15ec) >> 4) + 1;
    _DAT_ram_1d2a = CONCAT11(0x15,DAT_ram_15f8) * 0x100 + (uint)DAT_ram_15f7;
    _DAT_ram_1d28 = ((int)((uint)DAT_ram_15fa * 0x100 + (uint)DAT_ram_15f9) >> 4) + 1;
    _DAT_ram_1d26 = CONCAT11(0x15,DAT_ram_15ef) * 0x100 + (uint)DAT_ram_15ee;
    return;
  }
  return;
}





// Function: FUN_rom_002afc

/* WARNING: Heritage AFTER dead removal. Example location: W1 : 0x002b20 */
/* WARNING: Removing unreachable block (rom,0x002b18) */
/* WARNING: Removing unreachable block (rom,0x002b42) */
/* WARNING: Removing unreachable block (rom,0x002b48) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* WARNING: Restarted to delay deadcode elimination for space: ram */

void FUN_rom_002afc(void)

{
  ushort uVar1;
  
  uVar1 = Peripherals::ADC.ADCBUF5;
  *(ushort *)(&DAT_ram_0880 + _DAT_ram_1252 * 2) = uVar1;
  do {
  } while( true );
}





// Function: FUN_rom_002b70

/* WARNING: Heritage AFTER dead removal. Example location: W1 : 0x002bac */
/* WARNING: Removing unreachable block (rom,0x002b96) */
/* WARNING: Removing unreachable block (rom,0x002bc2) */
/* WARNING: Removing unreachable block (rom,0x002bbe) */
/* WARNING: Removing unreachable block (rom,0x002bc4) */
/* WARNING: Removing unreachable block (rom,0x002be0) */
/* WARNING: Removing unreachable block (rom,0x002bdc) */
/* WARNING: Removing unreachable block (rom,0x002be2) */
/* WARNING: Removing unreachable block (rom,0x002c1c) */
/* WARNING: Removing unreachable block (rom,0x002c1e) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* WARNING: Restarted to delay deadcode elimination for space: ram */

void FUN_rom_002b70(void)

{
  ushort uVar1;
  
  uVar1 = Peripherals::ADC.ADCBUF4;
  *(ulong *)(&DAT_ram_110a + _DAT_ram_1252 * 4) = (ulong)uVar1;
  do {
  } while( true );
}





// Function: FUN_rom_002c32

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_002c32(void)

{
  ushort uVar1;
  uint uVar2;
  bool bVar3;
  
  uVar1 = Peripherals::ADC.ADCBUF11;
  _DAT_ram_1210 = _DAT_ram_1210 + uVar1;
  uVar1 = Peripherals::ADC.ADCBUF10;
  _DAT_ram_120e = _DAT_ram_120e + uVar1;
  uVar1 = Peripherals::ADC.ADCBUF1;
  _DAT_ram_120c = _DAT_ram_120c + uVar1;
  uVar2 = _DAT_ram_120a + 1;
  bVar3 = _DAT_ram_120a != 0x1e;
  _DAT_ram_120a = uVar2;
  if (0x1e < uVar2 && bVar3) {
    _DAT_ram_120a = 0;
    _DAT_ram_1e48 = _DAT_ram_1210 >> 7;
    _DAT_ram_1e4c = _DAT_ram_120e >> 7;
    _DAT_ram_1e4a = _DAT_ram_120c >> 7;
    _DAT_ram_1210 = 0;
    _DAT_ram_120e = 0;
    _DAT_ram_120c = 0;
  }
  return;
}





// Function: FUN_rom_002c78

/* WARNING: Heritage AFTER dead removal. Example location: W1 : 0x002c92 */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* WARNING: Restarted to delay deadcode elimination for space: ram */

void FUN_rom_002c78(void)

{
  undefined4 in_W1W0;
  long lVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  
  if (_DAT_ram_1268 == 4) {
    lVar1 = (long)_DAT_ram_1d44 * 0x3b05;
  }
  else {
    _DAT_ram_1212 = 0;
    iVar4 = _DAT_ram_1d44;
    uVar2 = FUN_rom_001100(_DAT_ram_1d44,(int)((ulong)in_W1W0 >> 0x10),0xf0a,0);
    iVar3 = (int)((ulong)uVar2 >> 0x10);
    _DAT_ram_1214 = iVar3 << 1 | (uint)uVar2 >> 0xf;
    if (_DAT_ram_1d4a != 0x6b3) goto LAB_rom_002cbc;
    lVar1 = FUN_rom_001100(iVar4,iVar3,0x3b05,0);
  }
  _DAT_ram_1214 = (int)((ulong)lVar1 >> 0x10) << 1 | (uint)lVar1 >> 0xf;
LAB_rom_002cbc:
  if ((int)_DAT_ram_1214 < 0x4e) {
    if ((int)_DAT_ram_1214 < 0) {
      _DAT_ram_1214 = 0;
    }
  }
  else {
    _DAT_ram_1214 = 0x4d;
  }
  _DAT_ram_1d3a = _DAT_ram_1214;
  return;
}





// Function: FUN_rom_002cd6

/* WARNING: Heritage AFTER dead removal. Example location: W1 : 0x002d1a */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* WARNING: Restarted to delay deadcode elimination for space: ram */

void FUN_rom_002cd6(void)

{
  uint uVar1;
  uint uVar2;
  int iVar3;
  long lVar4;
  undefined4 uVar5;
  int iVar6;
  int iVar7;
  long lVar8;
  uint uVar9;
  
  lVar4 = (long)(_DAT_ram_1da0 >> 2) * 0xc03;
  lVar8 = lVar4 + -0x60180;
  iVar7 = -((int)((ulong)lVar8 >> 0x10) * 0x40 | (uint)lVar8 >> 10);
  _DAT_ram_1d50 = iVar7 + 0x400;
  if (iVar7 == 0 || _DAT_ram_1d50 < 0x400) {
    if ((iVar7 + 0x301U < 0x8000 || !SBORROW2(_DAT_ram_1d50,0xff)) &&
        ((int)(iVar7 + 0x301U) < 1 || SBORROW2(_DAT_ram_1d50,0xff))) {
      _DAT_ram_1d50 = 0x100;
    }
  }
  else {
    _DAT_ram_1d50 = 0x400;
  }
  iVar7 = _DAT_ram_1d50;
  uVar5 = FUN_rom_001100(_DAT_ram_1d50,(int)((ulong)lVar4 >> 0x10),12000);
  iVar3 = (int)((ulong)uVar5 >> 0x10);
  _DAT_ram_1d56 = iVar3 << 6 | (uint)uVar5 >> 10;
  _DAT_ram_1d58 = (int)_DAT_ram_1d56 >> 0xf;
  uVar9 = _DAT_ram_1d56;
  uVar5 = FUN_rom_001100(iVar7,iVar3,0xaf0);
  iVar7 = _DAT_ram_1218;
  uVar2 = _DAT_ram_1216;
  iVar6 = (int)((ulong)uVar5 >> 0x10);
  uVar1 = iVar6 << 6 | (uint)uVar5 >> 10;
  iVar3 = _DAT_ram_1d44 - _DAT_ram_1d36;
  _DAT_ram_1d54 = uVar1;
  uVar5 = FUN_rom_001100(iVar3,iVar6,uVar9);
  iVar6 = (int)((ulong)uVar5 >> 0x10);
  _DAT_ram_121c = (iVar6 + iVar7 + (uint)CARRY2((uint)uVar5,uVar2)) * 8 | (uint)uVar5 + uVar2 >> 0xd
  ;
  if ((int)_DAT_ram_121c < 0) {
    _DAT_ram_121a = 0;
  }
  else {
    _DAT_ram_121a = _DAT_ram_121c;
    if (4000 < (int)_DAT_ram_121c) {
      _DAT_ram_121a = 4000;
    }
  }
  lVar8 = (long)(int)(_DAT_ram_121a - _DAT_ram_121c) * (long)_DAT_ram_1d52;
  uVar9 = _DAT_ram_121a;
  uVar5 = FUN_rom_001100(iVar3,iVar6,uVar1);
  uVar1 = (uint)uVar5 + (uint)lVar8;
  _DAT_ram_1216 = uVar1 + uVar2;
  _DAT_ram_1218 =
       (int)((ulong)uVar5 >> 0x10) + (int)((ulong)lVar8 >> 0x10) +
       (uint)CARRY2((uint)uVar5,(uint)lVar8) + iVar7 + (uint)CARRY2(uVar1,uVar2);
  if (_DAT_ram_1216 == 0 && _DAT_ram_1218 == 500 || _DAT_ram_1218 < 500) {
    if (_DAT_ram_1218 < 0) {
      _DAT_ram_1216 = 0;
      _DAT_ram_1218 = 0;
    }
  }
  else {
    _DAT_ram_1216 = 0;
    _DAT_ram_1218 = 500;
  }
  _DAT_ram_1d38 = uVar9;
  return;
}





// Function: FUN_rom_002dc6

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_002dc6(void)

{
  byte bVar1;
  uint uVar2;
  bool bVar3;
  
  bVar1 = Peripherals::GPIOD.PORTD._0_1_;
  uVar2 = CONCAT11((byte)(_DAT_ram_1e1c >> 9),(char)(_DAT_ram_1e1c >> 1)) & 0xff01;
  if ((byte)uVar2 == (bVar1 & 1)) {
    _DAT_ram_121e = 0;
  }
  else {
    uVar2 = _DAT_ram_121e + 1;
    bVar3 = _DAT_ram_121e != 1;
    _DAT_ram_121e = uVar2;
    if (1 < uVar2 && bVar3) {
      bVar1 = Peripherals::GPIOD.PORTD._0_1_;
      uVar2 = (bVar1 & 1) * 2;
      _DAT_ram_1e1c = _DAT_ram_1e1c & 0xfffd | uVar2;
      _DAT_ram_121e = 0;
      return CONCAT22(uVar2,_DAT_ram_1e1c);
    }
  }
  return CONCAT22(CONCAT11(2,bVar1 & 1),uVar2);
}





// Function: FUN_rom_002df6

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_002df6(void)

{
  byte bVar1;
  
  bVar1 = Peripherals::GPIOD.PORTD._1_1_;
  if ((DAT_ram_1e1d & 8) == (bVar1 & 8)) {
    _DAT_ram_1d3e = 3;
    if ((_DAT_ram_1e1c & 0x800) == 0) {
      _DAT_ram_1d3e = 0x15e;
    }
  }
  else {
    if (_DAT_ram_1d3e == 0) {
      bVar1 = Peripherals::GPIOD.PORTD._1_1_;
      _DAT_ram_1e1c = _DAT_ram_1e1c & 0xf7ff | (bVar1 >> 3 & 1) << 0xb;
      return;
    }
    _DAT_ram_1d3e = _DAT_ram_1d3e + -1;
  }
  return;
}





// Function: FUN_rom_002e2c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_002e2c(void)

{
  if ((_DAT_ram_1e22 == 0) && (_DAT_ram_1e10 != 0)) {
    _DAT_ram_1e10 = _DAT_ram_1e10 + -1;
  }
  return;
}





// Function: FUN_rom_002e3c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ulong FUN_rom_002e3c(undefined2 param_1,uint param_2)

{
  byte bVar1;
  uint uVar2;
  uint uVar3;
  
  uVar3 = _DAT_ram_1e22;
  if (_DAT_ram_1e22 == 2) {
    param_2 = _DAT_ram_1e20 & 1;
    uVar3 = _DAT_ram_1e20;
    if (param_2 == 0) {
      if (_DAT_ram_1d42 < 0x1402) {
        _DAT_ram_1220 = param_2;
        return CONCAT22(_DAT_ram_1e20,0x1401) & 0x1ffff;
      }
      uVar2 = _DAT_ram_1220 + 1;
      if (0x17 < uVar2 && _DAT_ram_1220 != 0x17) {
        _DAT_ram_1264 = _DAT_ram_1264 | 0x80;
      }
      uVar3 = 0x1781;
      if (0x1781 < _DAT_ram_1d42) {
LAB_rom_002e6a:
        _DAT_ram_1264 = _DAT_ram_1264 | 0x80;
        _DAT_ram_1220 = uVar2;
        return CONCAT22(_DAT_ram_1e20,uVar3) & 0x1ffff;
      }
      bVar1 = Peripherals::GPIOD.LATD._0_1_;
      uVar3 = CONCAT11(0x17,bVar1);
      _DAT_ram_1220 = uVar2;
      if (((bVar1 & 0x10) != 0) && ((_DAT_ram_1264 & 0x400) == 0)) goto LAB_rom_002e6a;
      goto LAB_rom_002e74;
    }
  }
  _DAT_ram_1220 = 0;
LAB_rom_002e74:
  return CONCAT22(param_2,uVar3);
}





// Function: FUN_rom_002e76

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_002e76(undefined2 param_1,uint param_2)

{
  uint uVar1;
  bool bVar2;
  
  uVar1 = _DAT_ram_1e22;
  if (_DAT_ram_1e22 == 2) {
    uVar1 = _DAT_ram_1e20;
    if ((_DAT_ram_1e20 & 1) == 0) {
      uVar1 = _DAT_ram_1268;
      if (_DAT_ram_1268 != 3) {
LAB_rom_002ea4:
        _DAT_ram_1222 = _DAT_ram_1e20 & 1;
        return CONCAT22(param_2,uVar1);
      }
      if ((_DAT_ram_1d42 <= _DAT_ram_1bb4) &&
         (uVar1 = _DAT_ram_1bb4, param_2 = _DAT_ram_1d42, _DAT_ram_1d38 < 1)) goto LAB_rom_002ea4;
      param_2 = _DAT_ram_1222 + 1;
      uVar1 = 0x225;
      bVar2 = _DAT_ram_1222 != 0x224;
      _DAT_ram_1222 = param_2;
      if (0x224 < param_2 && bVar2) {
        _DAT_ram_1264 = _DAT_ram_1264 | 0x100;
        return CONCAT22(param_2,0x225);
      }
      goto LAB_rom_002eaa;
    }
  }
  _DAT_ram_1222 = 0;
LAB_rom_002eaa:
  return CONCAT22(param_2,uVar1);
}





// Function: FUN_rom_002eac

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int FUN_rom_002eac(void)

{
  byte bVar1;
  ushort uVar2;
  int iVar3;
  bool bVar4;
  
  if (_DAT_ram_1e22 == 2) {
    if (_DAT_ram_1268 == 3) {
      uVar2 = Peripherals::ADC.ADCBUF5;
      iVar3 = 0x2eb;
      if ((0x2eb < uVar2) &&
         (iVar3 = 14999,
         0x7fff < _DAT_ram_1da4 + 0xc569U && SBORROW2(_DAT_ram_1da4,14999) ||
         0 < (int)(_DAT_ram_1da4 + 0xc569U) && !SBORROW2(_DAT_ram_1da4,14999))) {
LAB_rom_002ef8:
        uVar2 = Peripherals::GPIOF.LATF;
        Peripherals::GPIOF.LATF = uVar2 | 1;
        return iVar3;
      }
    }
    else {
      iVar3 = _DAT_ram_1268;
      if (_DAT_ram_1268 != 4) goto LAB_rom_002f08;
      if (_DAT_ram_1be8 < 0x1fd1) {
        if (_DAT_ram_1be8 < 0x1f70) {
          _DAT_ram_1d5a = 0x2198;
        }
      }
      else {
        _DAT_ram_1d5a = 0x251c;
      }
      uVar2 = Peripherals::ADC.ADCBUF5;
      iVar3 = 0x16b;
      if (((0x16b < uVar2) && (iVar3 = _DAT_ram_1d5a, _DAT_ram_1d5a < _DAT_ram_1da4)) &&
         ((_DAT_ram_1264 & 0x800) != 0)) goto LAB_rom_002ef8;
    }
    if (((_DAT_ram_125a & 0x10) == 0) &&
       (iVar3 = 8000,
       (_DAT_ram_1da4 + 0xe0c0U < 0x8000 || !SBORROW2(_DAT_ram_1da4,8000)) &&
       ((int)(_DAT_ram_1da4 + 0xe0c0U) < 1 || SBORROW2(_DAT_ram_1da4,8000)))) {
LAB_rom_002f08:
      uVar2 = Peripherals::GPIOF.LATF;
      Peripherals::GPIOF.LATF = uVar2 & 0xfffe;
      return iVar3;
    }
  }
  else {
    bVar1 = Peripherals::GPIOF.LATF._0_1_;
    iVar3 = CONCAT11((char)((uint)_DAT_ram_1e22 >> 8),bVar1);
    if (((bVar1 & 1) == 0) &&
       (iVar3 = _DAT_ram_1224 + 1, bVar4 = _DAT_ram_1224 != 4, _DAT_ram_1224 = iVar3,
       bVar4 && 4 < iVar3)) {
      _DAT_ram_1224 = 0;
      uVar2 = Peripherals::GPIOF.LATF;
      Peripherals::GPIOF.LATF = uVar2 | 1;
    }
  }
  return iVar3;
}





// Function: FUN_rom_002f20

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

uint FUN_rom_002f20(void)

{
  uint uVar1;
  
  uVar1 = _DAT_ram_1bb6;
  if (_DAT_ram_1bb6 <= _DAT_ram_1d42) {
    uVar1 = 0x6e;
    if (_DAT_ram_1d40 < 0x6f) {
      uVar1 = 0xc4d;
      if (_DAT_ram_1d4e != 0xc4d) {
        _DAT_ram_126a = _DAT_ram_126a | 8;
      }
    }
  }
  return uVar1;
}





// Function: FUN_rom_002f48

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_002f48(void)

{
  uint uVar1;
  byte bVar2;
  bool bVar3;
  
  bVar2 = Peripherals::GPIOD.PORTD._0_1_;
  _DAT_ram_1228 = bVar2 >> 6 & 1;
  if (_DAT_ram_1228 != _DAT_ram_122a) {
    _DAT_ram_1226 = _DAT_ram_1226 + 1;
    _DAT_ram_122a = _DAT_ram_1228;
  }
  uVar1 = _DAT_ram_122e + 1;
  bVar3 = _DAT_ram_122e != 0x1f2;
  _DAT_ram_122e = uVar1;
  if (0x1f2 < uVar1 && bVar3) {
    if (_DAT_ram_1e22 == 2) {
      bVar2 = Peripherals::GPIOD.LATD._0_1_;
      if ((bVar2 & 0x20) == 0) {
        _DAT_ram_122c = 0;
      }
      if (_DAT_ram_1226 < 5) {
        uVar1 = _DAT_ram_122c + 1;
        bVar3 = _DAT_ram_122c != 0x4e;
        _DAT_ram_122c = uVar1;
        if (0x4e < uVar1 && bVar3) {
          _DAT_ram_1e20 = _DAT_ram_1e20 | 4;
        }
      }
      else if (7 < _DAT_ram_1226) {
        _DAT_ram_122c = 0;
        _DAT_ram_1e20 = _DAT_ram_1e20 & 0xfffb;
      }
    }
    else {
      _DAT_ram_122c = 0;
    }
    _DAT_ram_1bd4 = _DAT_ram_1226 * 0x96;
    _DAT_ram_1226 = 0;
    _DAT_ram_122e = 0;
    _DAT_ram_1e1c = _DAT_ram_1e1c | 0x100;
  }
  return;
}





// Function: FUN_rom_002fa6

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

long FUN_rom_002fa6(undefined2 param_1,uint param_2)

{
  int iVar1;
  bool bVar2;
  
  if ((_DAT_ram_125a & 0x20) == 0) {
    _DAT_ram_126a = _DAT_ram_126a & 0xfbdf;
    iVar1 = 0;
    _DAT_ram_1230 = 0;
    _DAT_ram_1232 = 0;
    goto LAB_rom_003050;
  }
  if (((_DAT_ram_1bea & 0x200) == 0) || ((_DAT_ram_1266 & 0x80) == 0)) {
LAB_rom_002fdc:
    _DAT_ram_126a = _DAT_ram_126a & 0xffef;
  }
  else if (_DAT_ram_1268 == 4) {
    if (0x3f < _DAT_ram_1bbe) goto LAB_rom_002fd0;
    param_2 = 0x402;
    if ((_DAT_ram_1bf2 & 0x402) == 2) {
      _DAT_ram_126a = _DAT_ram_126a | 0x10;
    }
  }
  else {
LAB_rom_002fd0:
    param_2 = _DAT_ram_1bbe;
    if ((0x45 < _DAT_ram_1bbe) || (_DAT_ram_1268 == 3)) goto LAB_rom_002fdc;
  }
  if ((_DAT_ram_126a & 0x10) == 0) {
    _DAT_ram_126a = _DAT_ram_126a & 0xffdf;
    _DAT_ram_1232 = 0;
  }
  else {
    param_2 = _DAT_ram_1be4;
    if (_DAT_ram_1be4 < 0xb) {
      if (_DAT_ram_1d44 < 0x1a) {
        if (_DAT_ram_1be4 < 8) {
          iVar1 = _DAT_ram_1232 + 1;
          param_2 = 1000;
          bVar2 = _DAT_ram_1232 != 999;
          _DAT_ram_1232 = iVar1;
          if (bVar2 && 999 < iVar1) {
            _DAT_ram_1232 = 1000;
            _DAT_ram_126a = _DAT_ram_126a | 0x20;
          }
        }
        goto LAB_rom_00300e;
      }
    }
    _DAT_ram_126a = _DAT_ram_126a & 0xffdf;
    _DAT_ram_1232 = 0;
  }
LAB_rom_00300e:
  iVar1 = _DAT_ram_1268;
  if (_DAT_ram_1268 == 3) {
    iVar1 = 0x600;
    param_2 = _DAT_ram_1264 & 0x600;
    if (param_2 == 0) {
      if (_DAT_ram_1be4 < 0xd) {
        if (_DAT_ram_1d44 < 0x1a) {
          param_2 = 0;
          iVar1 = _DAT_ram_1d44;
          if (_DAT_ram_1be4 < 10) {
            iVar1 = _DAT_ram_1230 + 1;
            param_2 = 1000;
            bVar2 = _DAT_ram_1230 != 999;
            _DAT_ram_1230 = iVar1;
            if (bVar2 && 999 < iVar1) {
              _DAT_ram_1230 = 1000;
              _DAT_ram_126a = _DAT_ram_126a | 0x400;
              return CONCAT22(1000,iVar1);
            }
          }
LAB_rom_003050:
          return CONCAT22(param_2,iVar1);
        }
      }
      _DAT_ram_126a = _DAT_ram_126a & 0xfbff;
      _DAT_ram_1230 = param_2;
      return ((ulong)_DAT_ram_1264 & 0x600) << 0x10;
    }
  }
  _DAT_ram_126a = _DAT_ram_126a & 0xfbff;
  _DAT_ram_1230 = 0;
  return CONCAT22(param_2,iVar1);
}





// Function: FUN_rom_003052

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_003052(void)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  
  uVar3 = _DAT_ram_125a & 0x120;
  if ((uVar3 == 0) && (_DAT_ram_1268 == 0)) {
    uVar3 = _DAT_ram_1e22;
    if (_DAT_ram_1e22 == 2) {
      if (_DAT_ram_1234 == 0xd || _DAT_ram_1234 + 1 < 0xe) {
        uVar3 = 10000;
        _DAT_ram_1da2 = 10000;
        _DAT_ram_1234 = _DAT_ram_1234 + 1;
      }
      else {
        _DAT_ram_1234 = 0xf;
        if (0x7fff < _DAT_ram_1da0 - 0x17eU && SBORROW2(_DAT_ram_1da0,0x17e) ||
            0 < (int)(_DAT_ram_1da0 - 0x17eU) && !SBORROW2(_DAT_ram_1da0,0x17e)) {
          uVar3 = 0xd48;
          _DAT_ram_1da2 = 0xd48;
        }
        else {
          uVar3 = 10000;
        }
        _DAT_ram_125a = _DAT_ram_125a | 0x100;
        _DAT_ram_1236 = uVar3;
      }
      goto LAB_rom_00308c;
    }
  }
  _DAT_ram_1234 = 0;
LAB_rom_00308c:
  uVar2 = _DAT_ram_1236;
  if ((0xd48 < (int)_DAT_ram_1236) && ((_DAT_ram_125a & 0x100) != 0)) {
    uVar3 = _DAT_ram_1236 - 0x32;
    uVar1 = _DAT_ram_1236 - 0xd7a;
    _DAT_ram_1236 = uVar3;
    if ((uVar1 < 0x8000 || !SBORROW2(uVar3,0xd48)) && ((int)uVar1 < 1 || SBORROW2(uVar3,0xd48))) {
      _DAT_ram_1236 = 0xd48;
    }
    _DAT_ram_1da2 = _DAT_ram_1236;
    uVar3 = _DAT_ram_1236;
  }
  return CONCAT22(uVar2,uVar3);
}





// Function: FUN_rom_0030aa

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_0030aa(undefined2 param_1,undefined2 param_2)

{
  byte bVar1;
  uint uVar2;
  
  if (_DAT_ram_1e22 == 1) {
    _DAT_ram_1bec = _DAT_ram_1bec & 0xff7f;
    return CONCAT22(param_2,1);
  }
  uVar2 = _DAT_ram_1e18;
  if ((_DAT_ram_1e18 & 0x800) != 0) {
    uVar2 = _DAT_ram_1d4e;
    if (_DAT_ram_1d4e == 0xc4d) {
      bVar1 = Peripherals::GPIOD.LATD._0_1_;
      if ((bVar1 & 0x10) == 0) {
        if (_DAT_ram_1268 == 3) {
          _DAT_ram_1bec = _DAT_ram_1bec | 0x80;
          return 0xc4d0c4d;
        }
      }
    }
  }
  if ((_DAT_ram_1268 == 4) || ((_DAT_ram_1e18 & 0x800) == 0)) {
    _DAT_ram_1bec = _DAT_ram_1bec & 0xff7f;
  }
  return CONCAT22(uVar2,_DAT_ram_1268);
}





// Function: FUN_rom_0030e2

undefined1 FUN_rom_0030e2(uint param_1,uint param_2,uint param_3,undefined1 param_4)

{
  if (param_1 < param_2) {
    if (param_1 <= param_3) {
      param_4 = 0;
    }
  }
  else {
    param_4 = 1;
  }
  return param_4;
}





// Function: FUN_rom_0030f8

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int FUN_rom_0030f8(void)

{
  ushort uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  bool bVar6;
  
  uVar2 = _DAT_ram_1e18;
  uVar4 = _DAT_ram_1266 & 1;
  uVar1 = Peripherals::ADC.ADCBUF13;
  uVar5 = _DAT_ram_1e18 >> 0xb & 1;
  uVar3 = FUN_rom_0030e2(uVar1,0x26c,0x136,uVar4);
  if (uVar5 == (uVar3 & 0xff)) {
    _DAT_ram_1238 = 0;
  }
  else {
    uVar5 = _DAT_ram_1238 + 1;
    bVar6 = _DAT_ram_1238 != 0x35;
    _DAT_ram_1238 = uVar5;
    if (0x35 < uVar5 && bVar6) {
      uVar1 = Peripherals::ADC.ADCBUF13;
      uVar4 = FUN_rom_0030e2(uVar1,0x26c,0x136,uVar4);
      uVar4 = uVar4 & 1;
      _DAT_ram_1e18 = uVar2 & 0xf7ff | uVar4 << 0xb;
      _DAT_ram_1238 = 0;
      if ((_DAT_ram_1bf2 & 0x10) == 0) {
        if (uVar4 == 0) {
          _DAT_ram_1262 = _DAT_ram_1262 & 0xfff7 | 0x40;
        }
      }
      else if (uVar4 != 0) {
        _DAT_ram_1262 = _DAT_ram_1262 & 0xffbf;
        _DAT_ram_1262 = _DAT_ram_1262 | 8;
      }
    }
  }
  if (_DAT_ram_1e22 != 3) {
    _DAT_ram_1262 = _DAT_ram_1262 & 0xffb7;
  }
  if ((_DAT_ram_1e18 & 0x800) == 0) {
    _DAT_ram_1bf2 = _DAT_ram_1bf2 | 0x10;
  }
  else {
    _DAT_ram_1bf2 = _DAT_ram_1bf2 & 0xffef;
  }
  return _DAT_ram_1e22;
}





// Function: FUN_rom_00316e

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_00316e(void)

{
  byte bVar1;
  ushort uVar2;
  uint uVar3;
  char cVar4;
  undefined2 uVar5;
  
  uVar2 = Peripherals::ADC.ADCBUF15;
  cVar4 = FUN_rom_0030e2(uVar2,0x2c9,0x26c,_DAT_ram_1266 >> 10 & 1);
  if (cVar4 == '\0') {
    bVar1 = Peripherals::GPIOD.LATD._0_1_;
    if (((bVar1 & 8) == 0) && ((_DAT_ram_1e18 & 0x100) == 0)) {
      if (_DAT_ram_1e22 != 5) {
        _DAT_ram_123a = _DAT_ram_123a + 1;
        goto LAB_rom_003196;
      }
    }
  }
  _DAT_ram_123a = 0;
LAB_rom_003196:
  uVar3 = _DAT_ram_123a;
  if (0x2ed < _DAT_ram_123a) {
    _DAT_ram_123a = 0x2ee;
    return CONCAT22(uVar3,0x2ee);
  }
  uVar5 = 499;
  if (499 < _DAT_ram_123a) {
    _DAT_ram_1bcc = _DAT_ram_1bca;
    _DAT_ram_1e18 = _DAT_ram_1e18 | 0x100;
    uVar5 = _DAT_ram_1bca;
  }
  return CONCAT22(uVar3,uVar5);
}





// Function: FUN_rom_0031b2

uint FUN_rom_0031b2(uint param_1,uint param_2,uint *param_3,uint param_4)

{
  uint uVar1;
  
  if (param_4 == param_1) {
    *param_3 = 0;
  }
  else {
    uVar1 = *param_3;
    *param_3 = uVar1 + 1;
    if (param_2 <= uVar1 + 1) {
      *param_3 = 0;
      param_4 = param_1;
    }
  }
  return param_4 & 0xff;
}





// Function: FUN_rom_0031cc

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_0031cc(void)

{
  ushort uVar1;
  byte bVar2;
  char cVar3;
  uint uVar4;
  undefined2 uVar5;
  
  if ((((_DAT_ram_1e22 == 2) && (_DAT_ram_1e0e == 0)) && ((_DAT_ram_1e20 & 0x10) == 0)) &&
     (((_DAT_ram_125a & 4) == 0 && ((_DAT_ram_1e1c & 0x800) != 0)))) {
    bVar2 = Peripherals::PWM1.IOCON1._1_1_;
    if ((((bVar2 & 2) != 0) &&
        ((bVar2 = Peripherals::PWM1.IOCON1._1_1_, (bVar2 & 1) != 0 &&
         (bVar2 = Peripherals::PWM2.IOCON2._1_1_, (bVar2 & 2) != 0)))) &&
       (bVar2 = Peripherals::PWM2.IOCON2._1_1_, (bVar2 & 1) != 0)) {
      _DAT_ram_123c = 0;
      return;
    }
    if (_DAT_ram_1268 == 3 || _DAT_ram_1268 == 4) {
      uVar1 = Peripherals::ADC.ADCBUF5;
      uVar4 = _DAT_ram_1e1c >> 2 & 1;
      uVar5 = 0;
      cVar3 = FUN_rom_0030e2(uVar1,0x15f,0x17f,_DAT_ram_1266 >> 0xf);
      if (cVar3 == '\0') {
        uVar5 = 1;
      }
      uVar4 = FUN_rom_0031b2(uVar5,5,&DAT_ram_123c,uVar4);
      _DAT_ram_1e1c = _DAT_ram_1e1c & 0xfffb | (uVar4 & 1) << 2;
      return;
    }
  }
  _DAT_ram_123c = 0;
  return;
}





// Function: FUN_rom_003242

int FUN_rom_003242(int param_1,int param_2)

{
  bool bVar1;
  
  if (param_2 < param_1) {
    param_2 = param_1 + -1;
  }
  else {
    bVar1 = -1 < param_1 - param_2;
    if ((bVar1 || !SBORROW2(param_1,param_2)) && (!bVar1 || SBORROW2(param_1,param_2))) {
      param_2 = param_1 + 1;
    }
  }
  return param_2;
}





// Function: FUN_rom_003254

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_003254(void)

{
  if ((_DAT_ram_125a & 0x20) == 0) {
LAB_rom_0032bc:
    _DAT_ram_1244 = _DAT_ram_1d4e;
  }
  else {
    _DAT_ram_1242 = _DAT_ram_1d44 * 0x17 >> 10;
    if (_DAT_ram_1242 < 0) {
      _DAT_ram_1242 = 0;
    }
    if (10 < _DAT_ram_1242) {
      _DAT_ram_1242 = 10;
    }
    if ((_DAT_ram_1bea & 0x100) == 0) {
LAB_rom_003286:
      if ((_DAT_ram_1bea & 0x80) == 0) {
LAB_rom_0032a2:
        if ((_DAT_ram_1bea & 0x40) != 0) {
          if (_DAT_ram_1d4e == 0xc4d) {
            _DAT_ram_123e = _DAT_ram_1242 + 0xc69;
            goto LAB_rom_0032b6;
          }
        }
        goto LAB_rom_0032bc;
      }
      if (_DAT_ram_1d4e != 0xc4d) goto LAB_rom_0032a2;
      _DAT_ram_1244 = ((uint)(&DAT_ram_d000 + _DAT_ram_1bc6) >> 2) + 0xbff;
    }
    else {
      if (_DAT_ram_1d4e != 0xc4d) goto LAB_rom_003286;
      _DAT_ram_1240 = _DAT_ram_1242 + 0xc11;
LAB_rom_0032b6:
      _DAT_ram_1244 = FUN_rom_003242(_DAT_ram_1244);
    }
    _DAT_ram_1d3a = 0;
  }
  _DAT_ram_1246 = (int)((long)_DAT_ram_1244 * (long)_DAT_ram_1d32 >> 0xd) + _DAT_ram_1d30;
  _DAT_ram_1d4c = (_DAT_ram_1246 - _DAT_ram_1d3a) - _DAT_ram_1d38;
  return;
}





// Function: FUN_rom_0032e0

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int FUN_rom_0032e0(void)

{
  int iVar1;
  
  iVar1 = _DAT_ram_1e22;
  if (_DAT_ram_1e22 == 2) {
    iVar1 = 0x800;
    if ((_DAT_ram_1e1c & 0x800) == 0) {
      _DAT_ram_1d4e = 0xffb0;
      iVar1 = _DAT_ram_1268 + -3;
      if (iVar1 == 0 || _DAT_ram_1268 == 4) {
        _DAT_ram_125a = _DAT_ram_125a | 0x10;
        _DAT_ram_1e0e = 0;
      }
      else {
        _DAT_ram_1e22 = 3;
        iVar1 = FUN_rom_004b50();
      }
    }
  }
  if (((_DAT_ram_125a & 0x10) != 0) &&
     (((_DAT_ram_1e1c & 0x800) != 0 || ((_DAT_ram_1e1a & 2) != 0)))) {
    _DAT_ram_125a = _DAT_ram_125a & 0xffef;
    FUN_rom_004b50();
    iVar1 = 0x9c4;
    _DAT_ram_1e10 = 0x9c4;
    _DAT_ram_1e22 = 0;
  }
  return iVar1;
}





// Function: FUN_rom_003322

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_003322(void)

{
  ushort uVar1;
  char cVar2;
  uint uVar3;
  undefined2 uVar4;
  
  if (((_DAT_ram_125a & 0x10) == 0) || (_DAT_ram_1e22 != 2)) {
    _DAT_ram_1e18 = _DAT_ram_1e18 & 0xefff;
    _DAT_ram_1248 = 0;
    return;
  }
  uVar1 = Peripherals::ADC.ADCBUF5;
  uVar3 = _DAT_ram_1e18 >> 0xc & 1;
  uVar4 = 0;
  cVar2 = FUN_rom_0030e2(uVar1,0x15f,0x17f,_DAT_ram_1266 >> 0xf);
  if (cVar2 == '\0') {
    uVar4 = 1;
  }
  uVar3 = FUN_rom_0031b2(uVar4,5,&DAT_ram_1248,uVar3);
  _DAT_ram_1e18 = _DAT_ram_1e18 & 0xefff | (uVar3 & 1) << 0xc;
  if ((uVar3 & 1) == 0) {
    return;
  }
  _DAT_ram_192a = _DAT_ram_192a | 1;
  FUN_rom_004b50();
  if ((_DAT_ram_1e1c & 0x802) != 2) {
    _DAT_ram_1e22 = 3;
    return;
  }
  if (_DAT_ram_1e20 != 0) {
    _DAT_ram_1e22 = 3;
    return;
  }
  _DAT_ram_1264 = _DAT_ram_1264 | 0x2000;
  _DAT_ram_1e22 = 3;
  return;
}





// Function: FUN_rom_003386

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

uint FUN_rom_003386(void)

{
  ushort uVar1;
  uint uVar2;
  
  uVar2 = _DAT_ram_1264;
  if (((_DAT_ram_1264 & 0x180) == 0) && ((_DAT_ram_1e1c & 4) == 0)) {
    return _DAT_ram_1264 & 0x180;
  }
  if (_DAT_ram_1e22 == 2) {
    _DAT_ram_1264 = _DAT_ram_1264 & 0xfe7f;
    _DAT_ram_1e1c = _DAT_ram_1e1c & 0xfffb;
    _DAT_ram_1e20 = _DAT_ram_1e20 | 1;
    if (((uVar2 & 0x400) == 0) && (((uVar2 & 0x200) != 0 || (_DAT_ram_1268 == 4)))) {
      _DAT_ram_1bf0 = _DAT_ram_1bf0 | 0x1000;
    }
    else {
      _DAT_ram_1bf0 = _DAT_ram_1bf0 | 0x2000;
    }
    uVar1 = Peripherals::ADC.ADCBUF4;
    _DAT_ram_1926 =
         (int)((ulong)uVar1 * 0x1e80 >> 0x10) << 6 | (uint)((ulong)uVar1 * 0x1e80 >> 10) & 0x3f;
    _DAT_ram_1924 =
         (int)((ulong)_DAT_ram_1be0 * (ulong)_DAT_ram_1926 >> 0x10) << 2 |
         (uint)((ulong)_DAT_ram_1be0 * (ulong)_DAT_ram_1926 >> 0xe) & 3;
    _DAT_ram_1262 = _DAT_ram_1262 | 0x1000;
    _DAT_ram_1bf0 = _DAT_ram_1bf0 | 1;
    _DAT_ram_1bf2 = _DAT_ram_1bf2 & 0xfffe;
    _DAT_ram_1bce = _DAT_ram_1924;
    _DAT_ram_1bd0 = _DAT_ram_1926;
    _DAT_ram_1bde = _DAT_ram_1926;
    FUN_rom_001fd6();
    _DAT_ram_192a = _DAT_ram_192a | 1;
    FUN_rom_004b50();
    _DAT_ram_1e22 = 3;
    return 3;
  }
  _DAT_ram_1264 = _DAT_ram_1264 & 0xfe7f;
  _DAT_ram_1e1c = _DAT_ram_1e1c & 0xfffb;
  return _DAT_ram_1e22;
}





// Function: FUN_rom_0033f4

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

byte FUN_rom_0033f4(void)

{
  ushort uVar1;
  char cVar2;
  undefined1 uVar3;
  byte bVar4;
  byte bVar5;
  
  cVar2 = Peripherals::PWM1.PWMCON1._1_1_;
  if (cVar2 < '\0') {
    bVar5 = Peripherals::CMP3.CMPCON3._0_1_;
    if ((bVar5 & 8) == 0) {
      _DAT_ram_125a = _DAT_ram_125a | 4;
      if (_DAT_ram_1e22 == 2) {
        _DAT_ram_1e20 = _DAT_ram_1e20 | 1;
        bVar5 = Peripherals::GPIOD.LATD._0_1_;
        if ((bVar5 & 0x10) == 0) {
          _DAT_ram_1bf0 = _DAT_ram_1bf0 | 0x2000;
        }
        else {
          _DAT_ram_1bf0 = _DAT_ram_1bf0 | 0x1000;
        }
        uVar1 = Peripherals::ADC.ADCBUF4;
        _DAT_ram_1926 =
             (int)((ulong)uVar1 * 0x1e80 >> 0x10) << 6 | (uint)((ulong)uVar1 * 0x1e80 >> 10) & 0x3f;
        _DAT_ram_1924 =
             (int)((ulong)_DAT_ram_1be0 * (ulong)_DAT_ram_1926 >> 0x10) << 2 |
             (uint)((ulong)_DAT_ram_1be0 * (ulong)_DAT_ram_1926 >> 0xe) & 3;
        _DAT_ram_1262 = _DAT_ram_1262 | 0x1000;
        _DAT_ram_1bf0 = _DAT_ram_1bf0 | 1;
        _DAT_ram_1bf2 = _DAT_ram_1bf2 & 0xfffe;
        _DAT_ram_1bce = _DAT_ram_1924;
        _DAT_ram_1bd0 = _DAT_ram_1926;
        _DAT_ram_1bde = _DAT_ram_1926;
        FUN_rom_001fd6();
        _DAT_ram_192a = _DAT_ram_192a | 1;
        FUN_rom_004b50();
        _DAT_ram_1e22 = 3;
      }
      uVar1 = Peripherals::PWM1.PWMCON1;
      Peripherals::PWM1.PWMCON1 = uVar1 & 0xefff;
      uVar1 = Peripherals::PWM1.PWMCON1;
      Peripherals::PWM1.PWMCON1 = uVar1 | 0x1000;
      bVar5 = Peripherals::PWM1.FCLCON1._0_1_;
      Peripherals::PWM1.FCLCON1._0_1_ = bVar5 | 3;
      uVar3 = Peripherals::PWM1.FCLCON1._0_1_;
      uVar3 = Peripherals::PWM1.FCLCON1._0_1_;
      uVar1 = Peripherals::PWM2.PWMCON2;
      Peripherals::PWM2.PWMCON2 = uVar1 & 0xefff;
      uVar1 = Peripherals::PWM2.PWMCON2;
      Peripherals::PWM2.PWMCON2 = uVar1 | 0x1000;
      bVar5 = Peripherals::PWM2.FCLCON2._0_1_;
      Peripherals::PWM2.FCLCON2._0_1_ = bVar5 | 3;
      uVar3 = Peripherals::PWM2.FCLCON2._0_1_;
      uVar3 = Peripherals::PWM2.FCLCON2._0_1_;
      uVar1 = Peripherals::PWM3.PWMCON3;
      Peripherals::PWM3.PWMCON3 = uVar1 & 0xefff;
      uVar1 = Peripherals::PWM3.PWMCON3;
      Peripherals::PWM3.PWMCON3 = uVar1 | 0x1000;
      bVar5 = Peripherals::PWM3.FCLCON3._0_1_;
      Peripherals::PWM3.FCLCON3._0_1_ = bVar5 | 3;
      uVar3 = Peripherals::PWM3.FCLCON3._0_1_;
      uVar3 = Peripherals::PWM3.FCLCON3._0_1_;
      DAT_ram_124a = 1;
      return 1;
    }
  }
  if (DAT_ram_124a == 1) {
    DAT_ram_124a = 2;
    bVar5 = Peripherals::PWM1.FCLCON1._0_1_;
    Peripherals::PWM1.FCLCON1._0_1_ = bVar5 & 0xfc;
    bVar5 = Peripherals::PWM1.FCLCON1._0_1_;
    Peripherals::PWM1.FCLCON1._0_1_ = bVar5 & 0xfc;
    return bVar5 & 0xfc;
  }
  if (DAT_ram_124a == 2) {
    DAT_ram_124a = 3;
    bVar5 = Peripherals::PWM2.FCLCON2._0_1_;
    Peripherals::PWM2.FCLCON2._0_1_ = bVar5 & 0xfc;
    bVar5 = Peripherals::PWM2.FCLCON2._0_1_;
    Peripherals::PWM2.FCLCON2._0_1_ = bVar5 & 0xfc;
    return bVar5 & 0xfc;
  }
  bVar5 = DAT_ram_124a;
  if (DAT_ram_124a == 3) {
    DAT_ram_124a = 0;
    bVar5 = Peripherals::PWM3.FCLCON3._0_1_;
    Peripherals::PWM3.FCLCON3._0_1_ = bVar5 & 0xfc;
    bVar4 = Peripherals::PWM3.FCLCON3._0_1_;
    bVar5 = bVar4 & 0xfc;
    Peripherals::PWM3.FCLCON3._0_1_ = bVar4 & 0xfc;
  }
  return bVar5;
}





// Function: FUN_rom_0034ee

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_0034ee(int param_1)

{
  ushort uVar1;
  int iVar2;
  bool bVar3;
  
  if (_DAT_ram_1d5c == -0x50) {
    _DAT_ram_124e = 0;
    _DAT_ram_124c = 0;
  }
  iVar2 = _DAT_ram_124e;
  if ((_DAT_ram_124e - 300U < 0x8000 || !SBORROW2(_DAT_ram_124e,300)) &&
      ((int)(_DAT_ram_124e - 300U) < 1 || SBORROW2(_DAT_ram_124e,300))) {
    _DAT_ram_124e = _DAT_ram_124e + 1;
    _DAT_ram_1d5c = (int)((long)_DAT_ram_124e * 0x1c) + 0x50;
    iVar2 = (int)((ulong)((long)_DAT_ram_124e * 0x1c) >> 0x10);
    if (param_1 < _DAT_ram_1d5c) {
      _DAT_ram_125a = _DAT_ram_125a | 0x20;
      _DAT_ram_1d5c = param_1;
      if (param_1 == 0x6b3) {
        uVar1 = Peripherals::GPIOD.LATD;
        Peripherals::GPIOD.LATD = uVar1 | 0x10;
        iVar2 = _DAT_ram_124c + 1;
        bVar3 = _DAT_ram_124c != 0x49;
        _DAT_ram_124c = iVar2;
        if (bVar3 && 0x49 < iVar2) {
          _DAT_ram_124e = 0;
          _DAT_ram_1262 = _DAT_ram_1262 & 0xdfff;
          _DAT_ram_124c = 0;
          _DAT_ram_1268 = 3;
          _DAT_ram_126e = 0x8000;
          _DAT_ram_1270 = 0x166;
          _DAT_ram_1262 = _DAT_ram_1262 | 0x100;
        }
      }
      else if (param_1 == 0xc4d) {
        _DAT_ram_124e = 0;
        _DAT_ram_1262 = _DAT_ram_1262 & 0xdfff;
        _DAT_ram_1268 = 3;
        uVar1 = Peripherals::GPIOD.LATD;
        Peripherals::GPIOD.LATD = uVar1 & 0xffef;
        _DAT_ram_126e = 0;
        _DAT_ram_1270 = 0;
      }
    }
  }
  return CONCAT22(iVar2,_DAT_ram_1d5c);
}





// Function: FUN_rom_003558

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_003558(void)

{
  int iVar1;
  ushort uVar2;
  bool bVar3;
  
  if (0x7fff < _DAT_ram_1d4e - 0xb11U && SBORROW2(_DAT_ram_1d4e,0xb11) ||
      0 < (int)(_DAT_ram_1d4e - 0xb11U) && !SBORROW2(_DAT_ram_1d4e,0xb11)) {
    _DAT_ram_1d5e = 0x8659;
    _DAT_ram_1250 = 0;
  }
  else {
    if ((_DAT_ram_1e18 & 0x800) == 0) {
      if ((_DAT_ram_1e1e & 6) == 0) {
        _DAT_ram_1d5e = 0x8659;
        goto LAB_rom_003598;
      }
    }
    _DAT_ram_1d5e = 0x4fa;
    if ((_DAT_ram_126a & 8) != 0) {
      if (_DAT_ram_1250 + 1 < 0x226 || _DAT_ram_1250 == 0x225) {
        _DAT_ram_1d5e = 0;
        _DAT_ram_1250 = _DAT_ram_1250 + 1;
      }
      else {
        _DAT_ram_1250 = 0x226;
      }
    }
  }
LAB_rom_003598:
  _DAT_ram_1d60 = 0;
  iVar1 = _DAT_ram_1270 - (uint)(_DAT_ram_126e < _DAT_ram_1d5e);
  _DAT_ram_1d4e = _DAT_ram_1d4a - (iVar1 * 4 | _DAT_ram_126e - _DAT_ram_1d5e >> 0xe);
  bVar3 = -1 < _DAT_ram_1d4e - _DAT_ram_1d4a;
  if ((bVar3 || SBORROW2(_DAT_ram_1d4e,_DAT_ram_1d4a)) &&
      (!bVar3 || !SBORROW2(_DAT_ram_1d4e,_DAT_ram_1d4a))) {
    _DAT_ram_126e = 0;
    _DAT_ram_1270 = 0;
    _DAT_ram_1d4e = _DAT_ram_1d4a;
    _DAT_ram_1e0e = 5;
    _DAT_ram_126a = _DAT_ram_126a & 0xfff7;
    _DAT_ram_1e1e = _DAT_ram_1e1e & 0xfff9;
    uVar2 = Peripherals::GPIOD.LATD;
    Peripherals::GPIOD.LATD = uVar2 & 0xffef;
    _DAT_ram_1264 = _DAT_ram_1264 & 0xfdff;
    return CONCAT22(iVar1 >> 0xe,5);
  }
  _DAT_ram_1264 = _DAT_ram_1264 | 0x200;
  _DAT_ram_126e = _DAT_ram_126e - _DAT_ram_1d5e;
  _DAT_ram_1270 = iVar1;
  return CONCAT22(iVar1 >> 0xe,_DAT_ram_1d4e);
}





// Function: FUN_rom_0035d4

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_0035d4(void)

{
  int iVar1;
  uint uVar2;
  bool bVar3;
  
  if (_DAT_ram_1d4e < 0xb9a) {
    _DAT_ram_1d62 = 0x9547;
  }
  else {
    _DAT_ram_1d62 = 0x4c75;
  }
  _DAT_ram_1d64 = 0;
  bVar3 = CARRY2(_DAT_ram_1d62,_DAT_ram_126e);
  _DAT_ram_126e = _DAT_ram_1d62 + _DAT_ram_126e;
  _DAT_ram_1270 = _DAT_ram_1270 + (uint)bVar3;
  uVar2 = (uint)(_DAT_ram_126e < 0x7fff);
  if (_DAT_ram_126e == 0x7fff && _DAT_ram_1270 - 0x166U == uVar2 ||
      (int)((_DAT_ram_1270 - 0x166U) - uVar2) < 0 !=
      (SBORROW2(_DAT_ram_1270,0x166) || SBORROW2(_DAT_ram_1270,uVar2 + 0x166))) {
    _DAT_ram_1264 = _DAT_ram_1264 | 0x400;
  }
  else {
    _DAT_ram_126e = 0x8000;
    _DAT_ram_1270 = 0x166;
    if ((_DAT_ram_1d44 - 0xc0U < 0x8000 || !SBORROW2(_DAT_ram_1d44,0xc0)) &&
        ((int)(_DAT_ram_1d44 - 0xc0U) < 1 || SBORROW2(_DAT_ram_1d44,0xc0))) {
      uVar2 = (int)((ulong)((long)_DAT_ram_1d44 * -0x2a6) >> 0x10) << 8 |
              (uint)((ulong)((long)_DAT_ram_1d44 * -0x2a6) >> 8) & 0xff;
      iVar1 = uVar2 + 0x1418;
      _DAT_ram_1d34 = 0x13ec;
      if (((uVar2 + 0x2c < 0x8000 || !SBORROW2(iVar1,0x13ec)) &&
           ((int)(uVar2 + 0x2c) < 1 || SBORROW2(iVar1,0x13ec))) &&
         (_DAT_ram_1d34 = iVar1,
         (uVar2 + 0x1bd < 0x8000 || !SBORROW2(iVar1,0x125b)) &&
         ((int)(uVar2 + 0x1bd) < 1 || SBORROW2(iVar1,0x125b)))) {
        _DAT_ram_1d34 = 0x125c;
      }
      _DAT_ram_1e0e = 5;
      _DAT_ram_126a = _DAT_ram_126a | 2;
      _DAT_ram_125a = _DAT_ram_125a & 0xfffd;
      _DAT_ram_125a = _DAT_ram_125a | 1;
      _DAT_ram_1264 = _DAT_ram_1264 | 0x800;
    }
    _DAT_ram_1d20 = 0x32;
    _DAT_ram_1268 = 4;
    _DAT_ram_1264 = _DAT_ram_1264 & 0xfbff;
  }
  _DAT_ram_1d4e = _DAT_ram_1d4a - (_DAT_ram_1270 << 2 | _DAT_ram_126e >> 0xe);
  return;
}





// Function: FUN_rom_003666

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_003666(void)

{
  int iVar1;
  uint uVar2;
  
  if (((_DAT_ram_1e18 & 0x800) != 0) && ((_DAT_ram_1e1e & 6) != 0)) {
    _DAT_ram_1e1e = _DAT_ram_1e1e & 0xfff9;
  }
  if ((_DAT_ram_1e20 & 1) == 0) {
    if (((((_DAT_ram_1e18 & 0x800) == 0) && ((_DAT_ram_1e1a & 0xa0) == 0)) ||
        (_DAT_ram_1bb6 <= _DAT_ram_1d42)) && (_DAT_ram_1d20 == 0)) {
      if ((_DAT_ram_1264 & 0x800) != 0) {
        uVar2 = (int)((ulong)((long)_DAT_ram_1d44 * -0xd3d) >> 0x10) << 8 |
                (uint)((ulong)((long)_DAT_ram_1d44 * -0xd3d) >> 8) & 0xff;
        iVar1 = uVar2 + 0x33a6;
        _DAT_ram_1d34 = 0x34bc;
        if (((uVar2 - 0x116 < 0x8000 || !SBORROW2(iVar1,0x34bc)) &&
             ((int)(uVar2 - 0x116) < 1 || SBORROW2(iVar1,0x34bc))) &&
           (_DAT_ram_1d34 = iVar1,
           (uVar2 + 0xf53 < 0x8000 || !SBORROW2(iVar1,0x2453)) &&
           ((int)(uVar2 + 0xf53) < 1 || SBORROW2(iVar1,0x2453)))) {
          _DAT_ram_1d34 = 0x2454;
        }
        _DAT_ram_126a = _DAT_ram_126a | 2;
        _DAT_ram_125a = _DAT_ram_125a & 0xfffe;
        _DAT_ram_125a = _DAT_ram_125a | 2;
        _DAT_ram_1264 = _DAT_ram_1264 & 0xf7ff;
      }
      _DAT_ram_1268 = 3;
    }
    else if (((_DAT_ram_1e4e < 0xc1) &&
             ((_DAT_ram_1d44 - 0xc0U < 0x8000 || !SBORROW2(_DAT_ram_1d44,0xc0)) &&
              ((int)(_DAT_ram_1d44 - 0xc0U) < 1 || SBORROW2(_DAT_ram_1d44,0xc0)))) &&
            ((_DAT_ram_1264 & 0x800) == 0)) {
      uVar2 = (int)((ulong)((long)_DAT_ram_1d44 * -0x2a6) >> 0x10) << 8 |
              (uint)((ulong)((long)_DAT_ram_1d44 * -0x2a6) >> 8) & 0xff;
      iVar1 = uVar2 + 0x1418;
      _DAT_ram_1d34 = 0x13ec;
      if (((uVar2 + 0x2c < 0x8000 || !SBORROW2(iVar1,0x13ec)) &&
           ((int)(uVar2 + 0x2c) < 1 || SBORROW2(iVar1,0x13ec))) &&
         (_DAT_ram_1d34 = iVar1,
         (uVar2 + 0x1bd < 0x8000 || !SBORROW2(iVar1,0x125b)) &&
         ((int)(uVar2 + 0x1bd) < 1 || SBORROW2(iVar1,0x125b)))) {
        _DAT_ram_1d34 = 0x125c;
      }
      _DAT_ram_1e0e = 5;
      _DAT_ram_126a = _DAT_ram_126a | 2;
      _DAT_ram_125a = _DAT_ram_125a & 0xfffd;
      _DAT_ram_125a = _DAT_ram_125a | 1;
      _DAT_ram_1264 = _DAT_ram_1264 | 0x800;
    }
  }
  else {
    _DAT_ram_125a = 0;
    _DAT_ram_126a = 0;
  }
  _DAT_ram_1262 = _DAT_ram_1262 & 0xffdf;
  _DAT_ram_1d4a = 0x6b3;
  return;
}





// Function: FUN_rom_00371a

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

uint FUN_rom_00371a(void)

{
  byte bVar1;
  ushort uVar2;
  uint uVar3;
  uint uVar4;
  
  _DAT_ram_1d4a = 0xc4d;
  uVar3 = 0x800;
  uVar4 = _DAT_ram_1e18 & 0x800;
  if (uVar4 == 0) {
    _DAT_ram_1262 = _DAT_ram_1262 & 0xffdf | 0x100;
  }
  if (((_DAT_ram_1262 & 0x100) != 0) && (uVar4 != 0)) {
    uVar3 = 0xc0;
    if ((_DAT_ram_1d44 - 0xc0U < 0x8000 || !SBORROW2(_DAT_ram_1d44,0xc0)) &&
        ((int)(_DAT_ram_1d44 - 0xc0U) < 1 || SBORROW2(_DAT_ram_1d44,0xc0))) {
      _DAT_ram_1262 = _DAT_ram_1262 | 0x20;
    }
    _DAT_ram_1262 = _DAT_ram_1262 & 0xfeff;
  }
  if ((_DAT_ram_1e20 & 1) == 0) {
    if ((_DAT_ram_125a & 0x10) == 0) {
      if ((((uVar4 == 0) || ((_DAT_ram_126a & 8) != 0)) && (_DAT_ram_1d4e != 0xc4d)) &&
         ((_DAT_ram_1e1a & 0xa0) == 0)) {
        _DAT_ram_1262 = _DAT_ram_1262 & 0xffdf;
        uVar4 = FUN_rom_003558();
        _DAT_ram_1264 = _DAT_ram_1264 & 0xfbff;
        return uVar4;
      }
      if (((_DAT_ram_1262 & 0x20) != 0) || ((_DAT_ram_1e1a & 0xa0) != 0)) {
        uVar4 = FUN_rom_0035d4();
        uVar2 = Peripherals::GPIOD.LATD;
        Peripherals::GPIOD.LATD = uVar2 | 0x10;
        return uVar4;
      }
      bVar1 = Peripherals::GPIOD.LATD._0_1_;
      if ((bVar1 & 0x10) == 0) {
        _DAT_ram_1d4a = 0xc4d;
        return (uint)bVar1;
      }
      if (0x7fff < _DAT_ram_1d44 - 0xd2U && SBORROW2(_DAT_ram_1d44,0xd2) ||
          0 < (int)(_DAT_ram_1d44 - 0xd2U) && !SBORROW2(_DAT_ram_1d44,0xd2)) {
        _DAT_ram_1d4a = 0xc4d;
        return 0xd2;
      }
      uVar4 = FUN_rom_0035d4();
      return uVar4;
    }
  }
  else {
    _DAT_ram_125a = 0;
    _DAT_ram_126a = 0;
  }
  return uVar3;
}





// Function: FUN_rom_00378a

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_00378a(void)

{
  if ((_DAT_ram_1262 & 0x2000) == 0) {
    if ((_DAT_ram_1da0 < 0x300) || ((_DAT_ram_125a & 0x80) != 0)) {
      FUN_rom_004b7c();
    }
    else {
      _DAT_ram_125a = _DAT_ram_125a | 0x80;
    }
  }
  _DAT_ram_1d4e = FUN_rom_0034ee(_DAT_ram_1d4a);
  return;
}





// Function: FUN_rom_0037aa

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_0037aa(void)

{
  if ((_DAT_ram_1e22 == 2) && ((_DAT_ram_1e1c & 0x800) != 0)) {
    if (_DAT_ram_1268 == 3) {
      FUN_rom_00371a();
      return;
    }
    if (_DAT_ram_1268 == 4) {
      FUN_rom_003666();
    }
    else if (_DAT_ram_1268 == 0) {
      FUN_rom_00378a();
      return;
    }
  }
  return;
}





// Function: FUN_rom_0037ca

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_0037ca(void)

{
  ushort uVar1;
  int iVar2;
  int iVar3;
  
  if ((_DAT_ram_1da4 < 8000) || (iVar2 = 7999, iVar3 = _DAT_ram_1da4, (_DAT_ram_126a & 0x20) != 0))
  {
    iVar2 = 0x13b;
    iVar3 = _DAT_ram_1d94;
    if ((_DAT_ram_1d94 - 0x13bU < 0x8000 || !SBORROW2(_DAT_ram_1d94,0x13b)) &&
        ((int)(_DAT_ram_1d94 - 0x13bU) < 1 || SBORROW2(_DAT_ram_1d94,0x13b))) {
      iVar2 = _DAT_ram_1d4a;
      iVar3 = _DAT_ram_1d4e;
      if (_DAT_ram_1d4e == _DAT_ram_1d4a) {
        if (_DAT_ram_1e4e < 0x12a) {
          uVar1 = Peripherals::GPIOF.LATF;
          Peripherals::GPIOF.LATF = uVar1 & 0xffbf;
        }
        return CONCAT22(_DAT_ram_1e4e,0x129);
      }
    }
  }
  uVar1 = Peripherals::GPIOF.LATF;
  Peripherals::GPIOF.LATF = uVar1 | 0x40;
  return CONCAT22(iVar3,iVar2);
}





// Function: FUN_rom_0038a6

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_0038a6(void)

{
  ushort uVar1;
  
  Peripherals::SPI2.SPI2STAT = 0;
  Peripherals::SPI2.SPI2CON1 = 0x122;
  Peripherals::SPI2.SPI2CON2 = 0;
  uVar1 = Peripherals::SPI2.SPI2STAT;
  Peripherals::SPI2.SPI2STAT = uVar1 | 0x8000;
  _DAT_ram_1292 = 0;
  return 0x122;
}





// Function: FUN_rom_0038b6

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_0038b6(ushort *param_1)

{
  byte bVar1;
  ushort uVar2;
  ushort uVar3;
  
  uVar2 = Peripherals::GPIOG.LATG;
  Peripherals::GPIOG.LATG = uVar2 & 0xfdff;
  uVar2 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = 0xd7;
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  _DAT_ram_1394 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = 0;
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  uVar2 = Peripherals::SPI2.SPI2BUF;
  *param_1 = uVar2;
  uVar3 = Peripherals::GPIOG.LATG;
  Peripherals::GPIOG.LATG = uVar3 | 0x200;
  return CONCAT22(param_1,uVar2);
}





// Function: FUN_rom_0038dc

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

byte FUN_rom_0038dc(void)

{
  byte bVar1;
  ushort uVar2;
  
  do {
    FUN_rom_0038b6(&DAT_ram_1292);
  } while ((_DAT_ram_1292 & 0x80) == 0);
  uVar2 = Peripherals::GPIOG.LATG;
  Peripherals::GPIOG.LATG = uVar2 & 0xfdff;
  uVar2 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = 0x3d;
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  uVar2 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = 0x2a;
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  uVar2 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = 0x80;
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  _DAT_ram_1394 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = 0xa6;
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  uVar2 = Peripherals::GPIOG.LATG;
  Peripherals::GPIOG.LATG = uVar2 | 0x200;
  return bVar1;
}





// Function: FUN_rom_003922

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_003922(uint param_1)

{
  byte bVar1;
  ushort uVar2;
  
  do {
    FUN_rom_0038b6(&DAT_ram_1292);
  } while ((_DAT_ram_1292 & 0x80) == 0);
  uVar2 = Peripherals::GPIOG.LATG;
  Peripherals::GPIOG.LATG = uVar2 & 0xfdff;
  uVar2 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = 0x81;
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  uVar2 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = param_1 >> 8 & 3;
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  uVar2 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = param_1 & 0xff;
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  _DAT_ram_1394 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = 0;
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  uVar2 = Peripherals::GPIOG.LATG;
  Peripherals::GPIOG.LATG = uVar2 | 0x200;
  return;
}





// Function: FUN_rom_00396e

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ushort FUN_rom_00396e(int param_1,uint param_2)

{
  byte bVar1;
  ushort uVar2;
  ushort uVar3;
  uint uVar4;
  
  do {
    FUN_rom_0038b6(&DAT_ram_1292);
  } while ((_DAT_ram_1292 & 0x80) == 0);
  uVar3 = Peripherals::GPIOG.LATG;
  Peripherals::GPIOG.LATG = uVar3 & 0xfdff;
  uVar3 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = 0x84;
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  uVar3 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = 0;
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  uVar3 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = 0;
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  _DAT_ram_1394 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = 0;
  uVar3 = _DAT_ram_1394;
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
    uVar3 = CONCAT11((char)(uVar3 >> 8),bVar1);
  } while ((bVar1 & 1) == 0);
  for (uVar4 = 0; uVar4 < param_2; uVar4 = uVar4 + 1) {
    _DAT_ram_1394 = Peripherals::SPI2.SPI2BUF;
    Peripherals::SPI2.SPI2BUF = (uint)*(byte *)(param_1 + uVar4);
    do {
      bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
      uVar3 = (ushort)bVar1;
    } while ((bVar1 & 1) == 0);
  }
  uVar2 = Peripherals::GPIOG.LATG;
  Peripherals::GPIOG.LATG = uVar2 | 0x200;
  return uVar3;
}





// Function: FUN_rom_0039d0

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_0039d0(uint param_1)

{
  byte bVar1;
  ushort uVar2;
  
  do {
    FUN_rom_0038b6(&DAT_ram_1292);
  } while ((_DAT_ram_1292 & 0x80) == 0);
  uVar2 = Peripherals::GPIOG.LATG;
  Peripherals::GPIOG.LATG = uVar2 & 0xfdff;
  uVar2 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = 0x88;
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  uVar2 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = param_1 >> 8 & 3;
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  uVar2 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = param_1 & 0xff;
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  _DAT_ram_1394 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = 0;
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  uVar2 = Peripherals::GPIOG.LATG;
  Peripherals::GPIOG.LATG = uVar2 | 0x200;
  return;
}





// Function: FUN_rom_003a1c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_003a1c(int param_1,uint param_2)

{
  byte bVar1;
  ushort uVar2;
  uint uVar3;
  ulong in_W3W2;
  
  do {
    FUN_rom_0038b6(&DAT_ram_1292);
  } while ((_DAT_ram_1292 & 0x80) == 0);
  uVar2 = Peripherals::GPIOG.LATG;
  Peripherals::GPIOG.LATG = uVar2 & 0xfdff;
  uVar2 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = 0xd2;
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  uVar2 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = (uint)in_W3W2 >> 8 & 3;
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  uVar2 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = (ushort)(in_W3W2 & 0xffff00ff);
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  uVar2 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = (ushort)((in_W3W2 & 0xffff00ff) >> 0x10);
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  uVar2 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = 0;
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  uVar2 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = 0;
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  uVar2 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = 0;
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  _DAT_ram_1394 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = 0;
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  for (uVar3 = 0; uVar3 < param_2; uVar3 = uVar3 + 1) {
    _DAT_ram_1394 = Peripherals::SPI2.SPI2BUF;
    Peripherals::SPI2.SPI2BUF = 0;
    do {
      bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
    } while ((bVar1 & 1) == 0);
    uVar2 = Peripherals::SPI2.SPI2BUF;
    *(char *)(param_1 + uVar3) = (char)uVar2;
  }
  uVar2 = Peripherals::GPIOG.LATG;
  Peripherals::GPIOG.LATG = uVar2 | 0x200;
  return;
}





// Function: FUN_rom_003aba

void FUN_rom_003aba(void)

{
  undefined1 *puVar1;
  int iVar2;
  bool bVar3;
  
  puVar1 = &DAT_ram_1498;
  do {
    *puVar1 = 0;
    bVar3 = puVar1 != (undefined1 *)0x1597;
    puVar1 = puVar1 + 1;
  } while (bVar3);
  iVar2 = 0;
  do {
    FUN_rom_003922(iVar2);
    FUN_rom_00396e(&DAT_ram_1498,0x100);
    FUN_rom_0039d0(iVar2);
    bVar3 = iVar2 != 7;
    iVar2 = iVar2 + 1;
  } while (bVar3);
  return;
}





// Function: FUN_rom_003ae0

undefined1 FUN_rom_003ae0(void)

{
  DAT_ram_1288 = DAT_ram_15d0;
  DAT_ram_1289 = DAT_ram_15d1;
  DAT_ram_1286 = DAT_ram_15d2;
  DAT_ram_1287 = DAT_ram_15d3;
  DAT_ram_1284 = DAT_ram_15d4;
  DAT_ram_1285 = DAT_ram_15d5;
  DAT_ram_1282 = DAT_ram_15d6;
  DAT_ram_1283 = DAT_ram_15d7;
  DAT_ram_1926 = DAT_ram_15d8;
  DAT_ram_1927 = DAT_ram_15d9;
  DAT_ram_1924 = DAT_ram_15da;
  DAT_ram_1925 = DAT_ram_15db;
  DAT_ram_1280 = DAT_ram_15dc;
  DAT_ram_1281 = DAT_ram_15dd;
  DAT_ram_127e = DAT_ram_15de;
  DAT_ram_127f = DAT_ram_15df;
  DAT_ram_127c = DAT_ram_15e0;
  DAT_ram_127d = DAT_ram_15e1;
  DAT_ram_127a = DAT_ram_15e2;
  DAT_ram_127b = DAT_ram_15e3;
  DAT_ram_1278 = DAT_ram_15e4;
  DAT_ram_1279 = DAT_ram_15e5;
  return DAT_ram_15e5;
}





// Function: FUN_rom_003b3a

bool FUN_rom_003b3a(int param_1,uint param_2,uint param_3,char param_4)

{
  char cVar1;
  char *pcVar2;
  
  param_2 = param_2 & 0xff;
  cVar1 = '\0';
  pcVar2 = (char *)(param_1 + param_2);
  for (; param_2 <= (param_3 & 0xff); param_2 = param_2 + 1) {
    cVar1 = cVar1 + *pcVar2;
    pcVar2 = pcVar2 + 2;
  }
  return -cVar1 != param_4;
}





// Function: FUN_rom_003b5e

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_003b5e(void)

{
  uint uVar1;
  undefined1 *puVar2;
  undefined1 extraout_var;
  uint uVar3;
  uint extraout_W1;
  undefined1 extraout_var_00;
  undefined1 extraout_var_01;
  undefined1 extraout_var_02;
  undefined1 extraout_var_03;
  undefined1 extraout_var_04;
  undefined1 extraout_var_05;
  undefined1 extraout_var_06;
  undefined1 extraout_var_07;
  undefined1 extraout_var_08;
  undefined1 extraout_var_09;
  undefined1 extraout_var_10;
  undefined1 extraout_var_11;
  int iVar4;
  bool bVar5;
  
  if ((_DAT_ram_1290 & 2) == 0) {
    _DAT_ram_1290 = _DAT_ram_1290 | 2;
    FUN_rom_0038b6(&DAT_ram_1292);
    FUN_rom_003a1c(&DAT_ram_160e,0x100,6,0);
    if ((DAT_ram_160e == -1) && (DAT_ram_160f == -1)) {
      _DAT_ram_1290 = _DAT_ram_1290 | 8;
    }
    else {
      _DAT_ram_1290 = _DAT_ram_1290 & 0xfff7;
    }
    if (((_DAT_ram_1292 & 1) == 0) || ((_DAT_ram_1290 & 8) != 0)) {
      FUN_rom_0038dc();
      FUN_rom_003aba();
      DAT_ram_128a = 0;
      DAT_ram_128b = 0;
      DAT_ram_128c = 0;
      _DAT_ram_1396 = 0;
      puVar2 = &DAT_ram_160e;
      do {
        *puVar2 = 0;
        bVar5 = puVar2 != (undefined1 *)0x170d;
        puVar2 = puVar2 + 1;
      } while (bVar5);
      puVar2 = &DAT_ram_15d0;
      do {
        *puVar2 = 0;
        bVar5 = puVar2 != &DAT_ram_15e5;
        puVar2 = puVar2 + 1;
      } while (bVar5);
      puVar2 = &DAT_ram_1598;
      do {
        *puVar2 = 0;
        bVar5 = puVar2 != (undefined1 *)0x15af;
        puVar2 = puVar2 + 1;
      } while (bVar5);
    }
    else {
      FUN_rom_003a1c(&DAT_ram_15d0,0x16,0,0);
      FUN_rom_003a1c(&DAT_ram_1598,0x18,5,0);
      FUN_rom_003a1c(&DAT_ram_15e6,0x20,7,0);
      DAT_ram_128a = DAT_ram_1598;
      DAT_ram_128b = DAT_ram_1599;
      DAT_ram_128c = DAT_ram_159a;
      _DAT_ram_1396 = (uint)DAT_ram_159b;
      if ((DAT_ram_15e6 == '\0') && (DAT_ram_15e7 == '\0')) {
        _DAT_ram_125a = _DAT_ram_125a & 0xffbf;
      }
      else {
        _DAT_ram_125a = _DAT_ram_125a | 0x40;
      }
      uVar1 = CONCAT11(extraout_var,1);
      if (DAT_ram_15f6 == '\x01') {
        _DAT_ram_1bb2 = 0x8000;
        _DAT_ram_1e18 = _DAT_ram_1e18 | 1;
        _DAT_ram_1bea = _DAT_ram_1bea | 0x408;
      }
      iVar4 = 0;
      do {
        uVar1 = FUN_rom_003b3a(&DAT_ram_160e,uVar1 & 0xff00,6,CONCAT11(0x16,DAT_ram_1615));
        uVar3 = _DAT_ram_128e & 0xfffe;
        _DAT_ram_128e = uVar3 | uVar1 & 1;
        uVar1 = FUN_rom_003b3a(&DAT_ram_160e,CONCAT11((char)(uVar3 >> 8),8),
                               CONCAT11(extraout_var_00,0x26),CONCAT11(0x16,DAT_ram_1635));
        uVar3 = _DAT_ram_128e & 0xfffd;
        _DAT_ram_128e = uVar3 | (uVar1 & 1) * 2;
        uVar1 = FUN_rom_003b3a(&DAT_ram_160e,CONCAT11((char)(uVar3 >> 8),0x28),
                               CONCAT11(extraout_var_01,0x6e),CONCAT11(0x16,DAT_ram_167d));
        uVar3 = _DAT_ram_128e & 0xfffb;
        _DAT_ram_128e = uVar3 | (uVar1 & 1) << 2;
        uVar1 = FUN_rom_003b3a(&DAT_ram_160e,CONCAT11((char)(uVar3 >> 8),0x70),
                               CONCAT11(extraout_var_02,0x73),CONCAT11(0x16,DAT_ram_1682));
        uVar3 = _DAT_ram_128e & 0xfff7;
        _DAT_ram_128e = uVar3 | (uVar1 & 1) << 3;
        uVar1 = FUN_rom_003b3a(&DAT_ram_160e,CONCAT11((char)(uVar3 >> 8),0x75),
                               CONCAT11(extraout_var_03,0x8c),CONCAT11(0x16,DAT_ram_1681));
        uVar3 = _DAT_ram_128e & 0xffef;
        _DAT_ram_128e = uVar3 | (uVar1 & 1) << 4;
        uVar1 = FUN_rom_003b3a(&DAT_ram_160e,CONCAT11((char)(uVar3 >> 8),0x8d),
                               CONCAT11(extraout_var_04,0x90),CONCAT11(0x16,DAT_ram_169f));
        uVar3 = _DAT_ram_128e & 0xffdf;
        _DAT_ram_128e = uVar3 | (uVar1 & 1) << 5;
        uVar1 = FUN_rom_003b3a(&DAT_ram_160e,CONCAT11((char)(uVar3 >> 8),0x92),
                               CONCAT11(extraout_var_05,0x9e),CONCAT11(0x16,DAT_ram_169e));
        uVar3 = _DAT_ram_128e & 0xffbf;
        _DAT_ram_128e = uVar3 | (uVar1 & 1) << 6;
        uVar1 = FUN_rom_003b3a(&DAT_ram_160e,CONCAT11((char)(uVar3 >> 8),0x9f),
                               CONCAT11(extraout_var_06,0xa2),CONCAT11(0x16,DAT_ram_16b1));
        uVar3 = _DAT_ram_128e & 0xff7f;
        _DAT_ram_128e = uVar3 | (uVar1 & 1) << 7;
        uVar1 = FUN_rom_003b3a(&DAT_ram_160e,CONCAT11((char)(uVar3 >> 8),0xa4),
                               CONCAT11(extraout_var_07,0xb0),CONCAT11(0x16,DAT_ram_16b0));
        _DAT_ram_128e = _DAT_ram_128e & 0xfeff | (uVar1 & 1) << 8;
        uVar1 = FUN_rom_003b3a(&DAT_ram_160e,CONCAT11((char)(_DAT_ram_128e >> 8),0xb1),
                               CONCAT11(extraout_var_08,0xb4),CONCAT11(0x16,DAT_ram_16c3));
        _DAT_ram_128e = _DAT_ram_128e & 0xfdff | (uVar1 & 1) << 9;
        uVar1 = FUN_rom_003b3a(&DAT_ram_160e,CONCAT11((char)(_DAT_ram_128e >> 8),0xb6),
                               CONCAT11(extraout_var_09,199),CONCAT11(0x16,DAT_ram_16c2));
        _DAT_ram_128e = _DAT_ram_128e & 0xfbff | (uVar1 & 1) << 10;
        uVar1 = FUN_rom_003b3a(&DAT_ram_160e,CONCAT11((char)(_DAT_ram_128e >> 8),200),
                               CONCAT11(extraout_var_10,0xcb),CONCAT11(0x16,DAT_ram_16da));
        _DAT_ram_128e = _DAT_ram_128e & 0xf7ff | (uVar1 & 1) << 0xb;
        uVar1 = FUN_rom_003b3a(&DAT_ram_160e,CONCAT11((char)(_DAT_ram_128e >> 8),0xcd),
                               CONCAT11(extraout_var_11,0xff),CONCAT11(0x16,DAT_ram_16d9));
        _DAT_ram_128e = _DAT_ram_128e & 0xefff | (uVar1 & 1) << 0xc;
        if (_DAT_ram_128e == 0) break;
        FUN_rom_003a1c(&DAT_ram_160e,0x100,6,0);
        bVar5 = iVar4 != 2;
        uVar1 = extraout_W1;
        iVar4 = iVar4 + 1;
      } while (bVar5);
    }
    FUN_rom_003ae0();
  }
  return;
}





// Function: FUN_rom_003d60

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined1 FUN_rom_003d60(void)

{
  DAT_ram_1498 = DAT_ram_1396;
  DAT_ram_1499 = DAT_ram_128a;
  DAT_ram_149a = DAT_ram_128b;
  DAT_ram_149b = DAT_ram_128c;
  DAT_ram_149c = DAT_ram_1bf4;
  DAT_ram_149d = DAT_ram_1bf5;
  DAT_ram_149e = DAT_ram_1bf2;
  DAT_ram_149f = DAT_ram_1bf3;
  DAT_ram_14a0 = DAT_ram_1bee;
  DAT_ram_14a1 = DAT_ram_1bef;
  DAT_ram_14a2 = DAT_ram_1bec;
  DAT_ram_14a3 = DAT_ram_1bed;
  DAT_ram_14a4 = (char)_DAT_ram_1bbc;
  DAT_ram_14a5 = (char)((uint)_DAT_ram_1bbc >> 8);
  DAT_ram_14a6 = (char)_DAT_ram_1bba;
  DAT_ram_14a7 = (char)((uint)_DAT_ram_1bba >> 8);
  DAT_ram_14a8 = (char)_DAT_ram_1bb8;
  DAT_ram_14a9 = (char)((uint)_DAT_ram_1bb8 >> 8);
  DAT_ram_14aa = DAT_ram_1bea;
  DAT_ram_14ab = DAT_ram_1beb;
  DAT_ram_14ac = (char)_DAT_ram_1bc6;
  DAT_ram_14ad = (char)((uint)_DAT_ram_1bc6 >> 8);
  DAT_ram_14ae = (char)_DAT_ram_1bb6;
  DAT_ram_14af = (char)((uint)_DAT_ram_1bb6 >> 8);
  DAT_ram_14b0 = (char)_DAT_ram_1bb4;
  DAT_ram_14b1 = (char)((uint)_DAT_ram_1bb4 >> 8);
  DAT_ram_14b2 = DAT_ram_1bca;
  DAT_ram_14b3 = DAT_ram_1bcb;
  return DAT_ram_1bcb;
}





// Function: FUN_rom_003dd2

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined1 FUN_rom_003dd2(void)

{
  DAT_ram_1498 = DAT_ram_1396;
  DAT_ram_1499 = DAT_ram_1be8;
  DAT_ram_149a = DAT_ram_1be9;
  DAT_ram_149b = DAT_ram_1be6;
  DAT_ram_149c = DAT_ram_1be7;
  DAT_ram_149d = DAT_ram_1be4;
  DAT_ram_149e = DAT_ram_1be5;
  DAT_ram_149f = DAT_ram_1be2;
  DAT_ram_14a0 = DAT_ram_1be3;
  DAT_ram_14a1 = DAT_ram_1bde;
  DAT_ram_14a2 = DAT_ram_1bdf;
  DAT_ram_14a3 = DAT_ram_1bd2;
  DAT_ram_14a4 = DAT_ram_1bd3;
  DAT_ram_14a5 = (char)_DAT_ram_1bc8;
  DAT_ram_14a6 = (char)((uint)_DAT_ram_1bc8 >> 8);
  DAT_ram_14a7 = DAT_ram_1bd0;
  DAT_ram_14a8 = DAT_ram_1bd1;
  DAT_ram_14a9 = DAT_ram_1bdc;
  DAT_ram_14aa = DAT_ram_1bdd;
  DAT_ram_14ab = DAT_ram_1bda;
  DAT_ram_14ac = DAT_ram_1bdb;
  DAT_ram_14ad = DAT_ram_1bd8;
  DAT_ram_14ae = DAT_ram_1bd9;
  DAT_ram_14af = DAT_ram_1bd6;
  DAT_ram_14b0 = DAT_ram_1bd7;
  DAT_ram_14b1 = (char)_DAT_ram_1bb2;
  DAT_ram_14b2 = (char)((uint)_DAT_ram_1bb2 >> 8);
  DAT_ram_14b3 = DAT_ram_1bd4;
  DAT_ram_14b4 = DAT_ram_1bd5;
  return DAT_ram_1bd5;
}





// Function: FUN_rom_003e48

undefined2 FUN_rom_003e48(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  bool bVar4;
  undefined1 auStack_2 [8];
  undefined1 local_a;
  undefined1 local_b;
  undefined4 local_26;
  
  DAT_ram_1598 = DAT_ram_128a;
  DAT_ram_1599 = DAT_ram_128b;
  DAT_ram_159a = DAT_ram_128c;
  DAT_ram_159b = DAT_ram_1396;
  iVar1 = 0;
  iVar2 = 1;
  do {
    local_26 = 0x3e6c;
    iVar3 = iVar2;
    FUN_rom_003a1c(auStack_2,0x20,1,iVar2 << 5);
    *(undefined1 *)(iVar1 + 0x159c) = local_a;
    *(undefined1 *)(iVar1 + 0x159d) = local_b;
    iVar2 = iVar3 + 1;
    iVar1 = iVar1 + 2;
  } while (iVar3 != 7);
  iVar2 = 0;
  do {
    local_26 = 0x3e90;
    FUN_rom_003a1c(auStack_2,0x20,2,iVar2 << 5);
    *(undefined1 *)(iVar2 * 2 + 0x15aa) = local_a;
    *(undefined1 *)(iVar2 * 2 + 0x15ab) = local_b;
    bVar4 = iVar2 != 2;
    iVar2 = iVar2 + 1;
  } while (bVar4);
  return 0x17;
}





// Function: FUN_rom_003eac

void FUN_rom_003eac(void)

{
  FUN_rom_003e48();
  FUN_rom_003922(5);
  FUN_rom_00396e(&DAT_ram_1598,0x18);
  FUN_rom_0039d0(5);
  return;
}





// Function: FUN_rom_003ebc

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

byte FUN_rom_003ebc(byte param_1)

{
  uint uVar1;
  bool bVar2;
  
  if ((_DAT_ram_1bf2 & 1) != 0) {
    if ((_DAT_ram_1bf4 & 0x100) == 0) {
      param_1 = Peripherals::GPIOF.LATF._0_1_;
      if ((param_1 & 2) == 0) {
        _DAT_ram_1276 = _DAT_ram_1276 + (0xfffe < _DAT_ram_1274);
        param_1 = 0xdf;
        bVar2 = _DAT_ram_1274 != 0x93de;
        uVar1 = (uint)(_DAT_ram_1274 + 1 < 0x93df);
        _DAT_ram_1274 = _DAT_ram_1274 + 1;
        if ((3 < _DAT_ram_1276 && uVar1 + 4 <= _DAT_ram_1276) &&
            (bVar2 || _DAT_ram_1276 - 4 != uVar1)) {
          _DAT_ram_1274 = 0;
          _DAT_ram_1276 = 0;
          if ((_DAT_ram_128c < 0xff || _DAT_ram_128c < (_DAT_ram_128a < 0xfffe) + 0xff) ||
              _DAT_ram_128a == 0xfffe && _DAT_ram_128c - 0xff == (uint)(_DAT_ram_128a < 0xfffe)) {
            bVar2 = 0xfffe < _DAT_ram_128a;
            _DAT_ram_128a = _DAT_ram_128a + 1;
            _DAT_ram_128c = _DAT_ram_128c + bVar2;
          }
          uVar1 = _DAT_ram_1272 + 1;
          param_1 = 0x9f;
          bVar2 = _DAT_ram_1272 != 0x59e;
          _DAT_ram_1272 = uVar1;
          if (0x59e < uVar1 && bVar2) {
            _DAT_ram_1272 = _DAT_ram_1bf4 & 0x100;
            param_1 = FUN_rom_003eac();
          }
        }
      }
    }
  }
  return param_1;
}





// Function: FUN_rom_003f0e

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_003f0e(void)

{
  byte bVar1;
  undefined1 *puVar2;
  undefined1 *puVar3;
  int iVar4;
  int iVar5;
  uint uVar6;
  undefined1 *puVar7;
  bool bVar8;
  undefined1 local_2 [68];
  
  if (((((_DAT_ram_192a & 1) != 0) && (uVar6 = _DAT_ram_1bf4 & 0x100, uVar6 == 0)) &&
      (bVar1 = Peripherals::GPIOF.LATF._0_1_, (bVar1 & 2) == 0)) && (DAT_ram_1bee != '\0')) {
    _DAT_ram_192a = _DAT_ram_192a & 0xfffe;
    FUN_rom_003a1c(&DAT_ram_1498,0x100,1,0);
    if ((((_DAT_ram_1e1c & 2) == 0) || ((_DAT_ram_1bf2 & 8) == 0)) || (_DAT_ram_1e22 == 5)) {
      FUN_rom_003a1c(&DAT_ram_1498,0x100,1,0);
      FUN_rom_003d60();
      FUN_rom_003922(1);
      FUN_rom_00396e(&DAT_ram_1498,0x100);
      FUN_rom_0039d0(1);
      FUN_rom_003a1c(&DAT_ram_1498,0x100,3,0);
      FUN_rom_003dd2();
    }
    else {
      _DAT_ram_1396 = _DAT_ram_1396 + 1;
      FUN_rom_003a1c(local_2,0x40,2,uVar6);
      puVar3 = local_2;
      puVar2 = &DAT_ram_1578;
      do {
        puVar2[-0xe0] = *puVar2;
        puVar2[-0xc0] = *puVar3;
        puVar2[-0xa0] = puVar3[0x20];
        puVar7 = puVar2 + 1;
        puVar3 = puVar3 + 1;
        bVar8 = puVar2 != (undefined1 *)0x1597;
        puVar2 = puVar7;
      } while (bVar8);
      puVar3 = &DAT_ram_14f8;
      do {
        *puVar3 = 0;
        bVar8 = puVar3 != (undefined1 *)0x1597;
        puVar3 = puVar3 + 1;
      } while (bVar8);
      FUN_rom_003922(2);
      FUN_rom_00396e(&DAT_ram_1498,0x100);
      FUN_rom_0039d0(2);
      FUN_rom_003a1c(&DAT_ram_1498,0x100,1,0);
      FUN_rom_003d60();
      iVar5 = 7;
      do {
        puVar3 = &DAT_ram_1498 + iVar5 * 0x20;
        iVar4 = 0;
        do {
          *puVar3 = puVar3[-0x20];
          puVar3 = puVar3 + 1;
          bVar8 = iVar4 != 0x1f;
          iVar4 = iVar4 + 1;
        } while (bVar8);
        iVar5 = iVar5 + -1;
      } while (iVar5 != 0);
      FUN_rom_003922(1);
      FUN_rom_00396e(&DAT_ram_1498,0x100);
      FUN_rom_0039d0(1);
      FUN_rom_003a1c(&DAT_ram_1498,0x100,3,iVar5);
      FUN_rom_003a1c(local_2,0x40,4,iVar5);
      puVar2 = &DAT_ram_1578;
      puVar3 = local_2;
      do {
        puVar2[-0xe0] = *puVar2;
        puVar2[-0xc0] = *puVar3;
        puVar2[-0xa0] = puVar3[0x20];
        puVar2 = puVar2 + 1;
        puVar3 = puVar3 + 1;
      } while (puVar2 != puVar7);
      puVar3 = &DAT_ram_14f8;
      do {
        puVar2 = puVar3 + 1;
        *puVar3 = 0;
        puVar3 = puVar2;
      } while (puVar2 != &DAT_ram_1598);
      FUN_rom_003922(4);
      FUN_rom_00396e(&DAT_ram_1498,0x100);
      FUN_rom_0039d0(4);
      FUN_rom_003a1c(&DAT_ram_1498,0x100,3,0);
      FUN_rom_003dd2();
      iVar5 = 7;
      do {
        puVar3 = &DAT_ram_1498 + iVar5 * 0x20;
        iVar4 = 0;
        do {
          *puVar3 = puVar3[-0x20];
          puVar3 = puVar3 + 1;
          bVar8 = iVar4 != 0x1f;
          iVar4 = iVar4 + 1;
        } while (bVar8);
        iVar5 = iVar5 + -1;
      } while (iVar5 != 0);
    }
    FUN_rom_003922(3);
    FUN_rom_00396e(&DAT_ram_1498,0x100);
    FUN_rom_0039d0(3);
    FUN_rom_003eac();
  }
  return;
}





// Function: FUN_rom_004068

undefined1 FUN_rom_004068(void)

{
  DAT_ram_15d0 = DAT_ram_1288;
  DAT_ram_15d1 = DAT_ram_1289;
  DAT_ram_15d2 = DAT_ram_1286;
  DAT_ram_15d3 = DAT_ram_1287;
  DAT_ram_15d4 = DAT_ram_1284;
  DAT_ram_15d5 = DAT_ram_1285;
  DAT_ram_15d6 = DAT_ram_1282;
  DAT_ram_15d7 = DAT_ram_1283;
  DAT_ram_15d8 = DAT_ram_1926;
  DAT_ram_15d9 = DAT_ram_1927;
  DAT_ram_15da = DAT_ram_1924;
  DAT_ram_15db = DAT_ram_1925;
  DAT_ram_15dc = DAT_ram_1280;
  DAT_ram_15dd = DAT_ram_1281;
  DAT_ram_15de = DAT_ram_127e;
  DAT_ram_15df = DAT_ram_127f;
  DAT_ram_15e0 = DAT_ram_127c;
  DAT_ram_15e1 = DAT_ram_127d;
  DAT_ram_15e2 = DAT_ram_127a;
  DAT_ram_15e3 = DAT_ram_127b;
  DAT_ram_15e4 = DAT_ram_1278;
  DAT_ram_15e5 = DAT_ram_1279;
  return DAT_ram_1279;
}





// Function: FUN_rom_0040c2

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

uint FUN_rom_0040c2(void)

{
  byte bVar1;
  uint uVar2;
  undefined1 *puVar3;
  bool bVar4;
  
  bVar1 = Peripherals::GPIOF.LATF._0_1_;
  uVar2 = (uint)bVar1;
  if ((bVar1 & 2) == 0) {
    if ((_DAT_ram_1e1a & 8) == 0) {
      if ((_DAT_ram_1bf4 & 0x100) == 0) {
        if ((_DAT_ram_1e1c & 2) != 0) {
          if (_DAT_ram_1288 < _DAT_ram_1be8) {
            _DAT_ram_1288 = _DAT_ram_1be8;
            _DAT_ram_1262 = _DAT_ram_1262 | 0x1000;
          }
          if (_DAT_ram_1286 < _DAT_ram_1bd2) {
            _DAT_ram_1286 = _DAT_ram_1bd2;
            _DAT_ram_1262 = _DAT_ram_1262 | 0x1000;
          }
          if (_DAT_ram_1284 < _DAT_ram_1bc8) {
            _DAT_ram_1284 = _DAT_ram_1bc8;
            _DAT_ram_1262 = _DAT_ram_1262 | 0x1000;
          }
        }
        uVar2 = _DAT_ram_1bf2;
        if ((_DAT_ram_1bf2 & 1) != 0) {
          if (_DAT_ram_1282 < _DAT_ram_1be2) {
            _DAT_ram_1282 = _DAT_ram_1be2;
            _DAT_ram_1262 = _DAT_ram_1262 | 0x1000;
          }
          if (_DAT_ram_1926 < _DAT_ram_1bd0) {
            _DAT_ram_1926 = _DAT_ram_1bd0;
            _DAT_ram_1262 = _DAT_ram_1262 | 0x1000;
          }
          uVar2 = _DAT_ram_1924;
          if (_DAT_ram_1924 < _DAT_ram_1bce) {
            _DAT_ram_1924 = _DAT_ram_1bce;
            _DAT_ram_1262 = _DAT_ram_1262 | 0x1000;
          }
        }
        if (((_DAT_ram_1e1c & 2) != 0) || ((_DAT_ram_1bf2 & 1) != 0)) {
          if (_DAT_ram_1280 < _DAT_ram_1bdc) {
            _DAT_ram_1280 = _DAT_ram_1bdc;
            _DAT_ram_1262 = _DAT_ram_1262 | 0x1000;
          }
          if (_DAT_ram_127e < _DAT_ram_1bda) {
            _DAT_ram_127e = _DAT_ram_1bda;
            _DAT_ram_1262 = _DAT_ram_1262 | 0x1000;
          }
          if (_DAT_ram_127c < _DAT_ram_1bd8) {
            _DAT_ram_127c = _DAT_ram_1bd8;
            _DAT_ram_1262 = _DAT_ram_1262 | 0x1000;
          }
          if (_DAT_ram_127a < _DAT_ram_1bd6) {
            _DAT_ram_127a = _DAT_ram_1bd6;
            _DAT_ram_1262 = _DAT_ram_1262 | 0x1000;
          }
          uVar2 = _DAT_ram_1278;
          if (_DAT_ram_1278 <= _DAT_ram_1bd4) {
            uVar2 = 199;
            if (199 < _DAT_ram_1bd4) {
              _DAT_ram_1278 = _DAT_ram_1bd4;
              _DAT_ram_1262 = _DAT_ram_1262 | 0x1000;
            }
          }
        }
        if ((_DAT_ram_1262 & 0x1000) != 0) {
          _DAT_ram_1262 = _DAT_ram_1262 & 0xefff;
          FUN_rom_004068();
          FUN_rom_003922(0);
          FUN_rom_00396e(&DAT_ram_15d0,0x16);
          uVar2 = FUN_rom_0039d0(0);
          return uVar2;
        }
      }
    }
    else {
      _DAT_ram_1e1a = _DAT_ram_1e1a & 0xfff7;
      puVar3 = &DAT_ram_15d0;
      do {
        *puVar3 = 0;
        bVar4 = puVar3 != &DAT_ram_15e6;
        puVar3 = puVar3 + 1;
      } while (bVar4);
      FUN_rom_003ae0();
      FUN_rom_003922(0);
      FUN_rom_00396e(&DAT_ram_15d0,0x16);
      uVar2 = FUN_rom_0039d0(0);
    }
  }
  return uVar2;
}





// Function: FUN_rom_0041a2

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_0041a2(void)

{
  ushort uVar1;
  
  if ((_DAT_ram_1928 & 2) != 0) {
    FUN_rom_003a1c(0x181e,0x100,DAT_ram_1923,DAT_ram_1922);
    uVar1 = Peripherals::I2C2.I2C2CON;
    Peripherals::I2C2.I2C2CON = uVar1 | 0x1000;
    _DAT_ram_1928 = _DAT_ram_1928 & 0xfffd;
  }
  return;
}





// Function: FUN_rom_0041be

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_0041be(void)

{
  ushort uVar1;
  byte bVar2;
  
  if ((_DAT_ram_1928 & 1) != 0) {
    bVar2 = Peripherals::GPIOF.LATF._0_1_;
    if ((bVar2 & 2) == 0) {
      FUN_rom_003922(DAT_ram_1921);
      FUN_rom_00396e(0x171e,0x100);
      FUN_rom_0039d0(DAT_ram_1921);
    }
    uVar1 = Peripherals::I2C2.I2C2CON;
    Peripherals::I2C2.I2C2CON = uVar1 | 0x1000;
    _DAT_ram_1928 = _DAT_ram_1928 & 0xfffe;
  }
  return;
}





// Function: FUN_rom_0041e0

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* WARNING: Restarted to delay deadcode elimination for space: ram */

undefined2 FUN_rom_0041e0(undefined2 param_1)

{
  int iVar1;
  undefined1 extraout_var;
  undefined1 extraout_var_00;
  undefined1 uVar2;
  undefined1 extraout_var_01;
  undefined1 extraout_var_02;
  undefined2 uVar3;
  uint uVar4;
  int iVar5;
  
  if ((_DAT_ram_1e1a & 0x100) == 0) {
    return param_1;
  }
  uVar4 = 0x9fff;
  if (0x9fff < _DAT_ram_191e) {
    if ((_DAT_ram_1606 == _DAT_ram_1390) &&
       (iVar5 = _DAT_ram_1388, uVar3 = _DAT_ram_138c, iVar1 = FUN_rom_005ae0(), iVar5 == iVar1)) {
      FUN_rom_005acc(uVar3);
      FUN_rom_005ad4(_DAT_ram_1606);
      uVar3 = CONCAT11(extraout_var_01,3);
      uVar2 = extraout_var;
    }
    else {
      FUN_rom_005acc(0);
      FUN_rom_005ad4(_DAT_ram_1606);
      uVar3 = CONCAT11(extraout_var_02,2);
      uVar2 = extraout_var_00;
    }
    uVar3 = FUN_rom_005a88(CONCAT11(uVar2,0xff),uVar3);
    _DAT_ram_1e1a = _DAT_ram_1e1a & 0xfeff;
    return uVar3;
  }
  FUN_rom_003a1c(0x1294,0x100,DAT_ram_191f,0);
  uVar3 = 0x100;
  if (uVar4 < _DAT_ram_191e + 0x100) {
    uVar3 = 0xfc;
  }
  FUN_rom_005b86(0x1294,&DAT_ram_1606,uVar3,0);
  _DAT_ram_191e = _DAT_ram_191e + 0x100;
  return 0x100;
}





// Function: thunk_FUN_rom_0041e0

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* WARNING: Restarted to delay deadcode elimination for space: ram */

undefined2 thunk_FUN_rom_0041e0(undefined2 param_1)

{
  int iVar1;
  undefined1 extraout_var;
  undefined1 extraout_var_00;
  undefined1 uVar2;
  undefined1 extraout_var_01;
  undefined1 extraout_var_02;
  undefined2 uVar3;
  uint uVar4;
  int iVar5;
  
  if ((_DAT_ram_1e1a & 0x100) == 0) {
    return param_1;
  }
  uVar4 = 0x9fff;
  if (0x9fff < _DAT_ram_191e) {
    if ((_DAT_ram_1606 == _DAT_ram_1390) &&
       (iVar5 = _DAT_ram_1388, uVar3 = _DAT_ram_138c, iVar1 = FUN_rom_005ae0(), iVar5 == iVar1)) {
      FUN_rom_005acc(uVar3);
      FUN_rom_005ad4(_DAT_ram_1606);
      uVar3 = CONCAT11(extraout_var_01,3);
      uVar2 = extraout_var;
    }
    else {
      FUN_rom_005acc(0);
      FUN_rom_005ad4(_DAT_ram_1606);
      uVar3 = CONCAT11(extraout_var_02,2);
      uVar2 = extraout_var_00;
    }
    uVar3 = FUN_rom_005a88(CONCAT11(uVar2,0xff),uVar3);
    _DAT_ram_1e1a = _DAT_ram_1e1a & 0xfeff;
    return uVar3;
  }
  FUN_rom_003a1c(0x1294,0x100,DAT_ram_191f,0);
  uVar3 = 0x100;
  if (uVar4 < _DAT_ram_191e + 0x100) {
    uVar3 = 0xfc;
  }
  FUN_rom_005b86(0x1294,&DAT_ram_1606,uVar3,0);
  _DAT_ram_191e = _DAT_ram_191e + 0x100;
  return 0x100;
}





// Function: FUN_rom_004260

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_004260(void)

{
  ushort uVar1;
  byte bVar2;
  
  if ((_DAT_ram_1928 & 0x10) != 0) {
    bVar2 = Peripherals::GPIOF.LATF._0_1_;
    if ((bVar2 & 2) == 0) {
      FUN_rom_003922(6);
      FUN_rom_00396e(&DAT_ram_160e,0x100);
      FUN_rom_0039d0(6);
    }
    uVar1 = Peripherals::I2C2.I2C2CON;
    Peripherals::I2C2.I2C2CON = uVar1 | 0x1000;
    _DAT_ram_1928 = _DAT_ram_1928 & 0xffef;
  }
  return;
}





// Function: FUN_rom_00427e

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_00427e(void)

{
  ushort uVar1;
  byte bVar2;
  
  if ((_DAT_ram_1928 & 0x40) != 0) {
    bVar2 = Peripherals::GPIOF.LATF._0_1_;
    if ((bVar2 & 2) == 0) {
      FUN_rom_003922(7);
      FUN_rom_00396e(&DAT_ram_15e6,0x20);
      FUN_rom_0039d0(7);
    }
    uVar1 = Peripherals::I2C2.I2C2CON;
    Peripherals::I2C2.I2C2CON = uVar1 | 0x1000;
    _DAT_ram_1928 = _DAT_ram_1928 & 0xffbf;
  }
  return;
}





// Function: FUN_rom_00429c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_00429c(void)

{
  ushort uVar1;
  
  if ((_DAT_ram_1928 & 0x20) != 0) {
    FUN_rom_003a1c(0x15b0,0x20,_DAT_ram_160c,_DAT_ram_160a);
    uVar1 = Peripherals::I2C2.I2C2CON;
    Peripherals::I2C2.I2C2CON = uVar1 | 0x1000;
    _DAT_ram_1928 = _DAT_ram_1928 & 0xffdf;
  }
  return;
}





// Function: FUN_rom_0042b0

undefined4 FUN_rom_0042b0(int param_1)

{
  return CONCAT22(&DAT_ram_15d0,(uint)(byte)(&DAT_ram_15d0)[param_1]);
}





// Function: FUN_rom_0042b8

undefined4 FUN_rom_0042b8(int param_1)

{
  return CONCAT22(0x15b0,(uint)*(byte *)(param_1 + 0x15b0));
}





// Function: FUN_rom_0042c0

undefined4 FUN_rom_0042c0(int param_1)

{
  return CONCAT22(&DAT_ram_1598,(uint)(byte)(&DAT_ram_1598)[param_1]);
}





// Function: FUN_rom_0042c8

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_0042c8(void)

{
  byte bVar1;
  undefined1 *puVar2;
  int iVar3;
  bool bVar4;
  
  if ((_DAT_ram_1e1a & 0x40) != 0) {
    bVar1 = Peripherals::GPIOF.LATF._0_1_;
    if ((bVar1 & 2) == 0) {
      _DAT_ram_1e1a = _DAT_ram_1e1a & 0xffbf;
      puVar2 = &DAT_ram_1498;
      do {
        *puVar2 = 0;
        bVar4 = puVar2 != (undefined1 *)0x1597;
        puVar2 = puVar2 + 1;
      } while (bVar4);
      iVar3 = 1;
      do {
        FUN_rom_003922(iVar3);
        FUN_rom_00396e(&DAT_ram_1498,0x100);
        FUN_rom_0039d0(iVar3);
        bVar4 = iVar3 != 5;
        iVar3 = iVar3 + 1;
      } while (bVar4);
      DAT_ram_128a = 0;
      DAT_ram_128b = 0;
      DAT_ram_128c = 0;
      _DAT_ram_1396 = 0;
      puVar2 = &DAT_ram_1598;
      do {
        *puVar2 = 0;
        bVar4 = puVar2 != (undefined1 *)0x15af;
        puVar2 = puVar2 + 1;
      } while (bVar4);
    }
  }
  return;
}





// Function: FUN_rom_00430c

uint FUN_rom_00430c(uint param_1,uint param_2,uint *param_3,uint param_4)

{
  uint uVar1;
  
  if (param_4 == param_1) {
    *param_3 = 0;
  }
  else {
    uVar1 = *param_3;
    *param_3 = uVar1 + 1;
    if (param_2 <= uVar1 + 1) {
      *param_3 = 0;
      param_4 = param_1;
    }
  }
  return param_4 & 0xff;
}





// Function: FUN_rom_004326

undefined1 FUN_rom_004326(uint param_1,uint param_2,uint param_3,undefined1 param_4)

{
  if (param_1 < param_2) {
    if (param_1 <= param_3) {
      param_4 = 0;
    }
  }
  else {
    param_4 = 1;
  }
  return param_4;
}





// Function: FUN_rom_00433c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_00433c(void)

{
  uint uVar1;
  ushort uVar2;
  char cVar3;
  uint uVar4;
  uint uVar5;
  undefined2 uVar6;
  
  uVar2 = Peripherals::ADC.ADCBUF12;
  uVar4 = _DAT_ram_1bf2 >> 3 & 1;
  uVar6 = 0;
  cVar3 = FUN_rom_004326(uVar2,0x26c,0x136,_DAT_ram_1266 >> 1 & 1);
  if (cVar3 == '\0') {
    uVar6 = 1;
  }
  uVar4 = FUN_rom_00430c(uVar6,3,0x1db2,uVar4);
  uVar4 = uVar4 & 1;
  uVar5 = uVar4 << 3;
  uVar1 = _DAT_ram_1bf2 & 0xfff7 | uVar5;
  if (_DAT_ram_1e22 == 2) {
    if (uVar4 != 0) {
      if ((_DAT_ram_125a & 0x2000) == 0) {
        _DAT_ram_1bf2 = uVar1;
        return;
      }
      _DAT_ram_125a = _DAT_ram_125a & 0xdfff;
      _DAT_ram_1bf2 = uVar1;
      _DAT_ram_1db0 = 0;
      return;
    }
    if (_DAT_ram_1268 == 3 || _DAT_ram_1268 == 4) {
      _DAT_ram_125a = _DAT_ram_125a | 0x2000;
      uVar4 = _DAT_ram_1db0 + 1;
      _DAT_ram_1d4c = _DAT_ram_1d4c + uVar4 * -2;
      if (uVar4 < 0x4b || _DAT_ram_1db0 == 0x4a) {
        _DAT_ram_1bf2 = uVar1;
        _DAT_ram_1db0 = uVar4;
        return;
      }
      _DAT_ram_1bf2 = _DAT_ram_1bf2 & 0xfff6;
      _DAT_ram_192a = _DAT_ram_192a | 1;
      _DAT_ram_1db0 = uVar5;
      FUN_rom_004b50();
      _DAT_ram_125a = _DAT_ram_125a & 0xdfff;
      _DAT_ram_1e10 = 0x9c4;
      _DAT_ram_1e22 = uVar5;
      return;
    }
  }
  else {
    _DAT_ram_125a = _DAT_ram_125a & 0xdfff;
    _DAT_ram_1db0 = 0;
    if (uVar4 != 0) {
      _DAT_ram_1bf2 = uVar1;
      _DAT_ram_1db0 = 0;
      return;
    }
    if (_DAT_ram_1e22 == 0) goto LAB_rom_0043c6;
  }
  _DAT_ram_1bf2 = _DAT_ram_1bf2 & 0xfff6 | uVar5;
  _DAT_ram_192a = _DAT_ram_192a | 1;
  uVar1 = _DAT_ram_1bf2;
LAB_rom_0043c6:
  _DAT_ram_1bf2 = uVar1;
  FUN_rom_004b50();
  _DAT_ram_1e22 = uVar5;
  _DAT_ram_1e10 = 0x9c4;
  return;
}





// Function: FUN_rom_0043d4

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_rom_0043d4(void)

{
  int iVar1;
  int iVar2;
  ushort uVar3;
  bool bVar4;
  
  uVar3 = Peripherals::ADC.ADCBUF4;
  _DAT_ram_1d94 = uVar3;
  *(ushort *)(_DAT_ram_1dc4 * 2 + 0x1db4) = uVar3;
  iVar1 = uVar3 + _DAT_ram_1d68;
  iVar2 = _DAT_ram_1dc4 + 1;
  bVar4 = _DAT_ram_1dc4 != 6;
  _DAT_ram_1dc4 = iVar2;
  if (bVar4 && 6 < iVar2) {
    _DAT_ram_1dc4 = 0;
  }
  _DAT_ram_1d68 = iVar1 - *(int *)(_DAT_ram_1dc4 * 2 + 0x1db4);
  _DAT_ram_1d46 = iVar1 >> 3;
  return CONCAT26(iVar2,CONCAT24(_DAT_ram_1d68,CONCAT22(0x1db4,iVar1)));
}





// Function: FUN_rom_004400

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

uint FUN_rom_004400(void)

{
  byte bVar1;
  uint uVar2;
  
  if (_DAT_ram_1e22 != 2) {
    _DAT_ram_1dc6 = 0;
    return _DAT_ram_1e22;
  }
  bVar1 = Peripherals::GPIOD.PORTD._1_1_;
  _DAT_ram_1dc8 = bVar1 >> 1 & 1;
  bVar1 = Peripherals::GPIOD.PORTD._1_1_;
  _DAT_ram_1d1a = bVar1 & 1;
  uVar2 = _DAT_ram_1d1a;
  if ((_DAT_ram_1dc8 == _DAT_ram_1d1a) && (uVar2 = 0x500, 0x500 < _DAT_ram_1d9e)) {
    uVar2 = _DAT_ram_1dc6 + 1;
    if (_DAT_ram_1dc6 == 0x12 || (int)uVar2 < 0x13) {
      _DAT_ram_1dc6 = uVar2;
      return uVar2;
    }
    _DAT_ram_1dc6 = 0x14;
    _DAT_ram_1e20 = _DAT_ram_1e20 | 0x40;
    _DAT_ram_1bf0 = _DAT_ram_1bf0 | 1;
    _DAT_ram_1bf2 = _DAT_ram_1bf2 & 0xfffe;
    FUN_rom_001fd6();
    _DAT_ram_192a = _DAT_ram_192a | 1;
    FUN_rom_004b50();
    _DAT_ram_1e22 = 3;
    return 3;
  }
  _DAT_ram_1dc6 = 0;
  return uVar2;
}





// Function: FUN_rom_00444c

/* WARNING: Heritage AFTER dead removal. Example location: W1 : 0x00445a */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* WARNING: Restarted to delay deadcode elimination for space: ram */

void FUN_rom_00444c(void)

{
  uint uVar1;
  ushort uVar2;
  undefined4 in_W1W0;
  undefined4 uVar3;
  uint uVar4;
  uint uVar5;
  bool bVar6;
  
  uVar5 = _DAT_ram_1d9e >> 2;
  uVar2 = Peripherals::ADC.ADCBUF5;
  uVar3 = FUN_rom_001100(uVar2,(int)((ulong)in_W1W0 >> 0x10),_DAT_ram_1d2e);
  _DAT_ram_1d66 = _DAT_ram_1d2c + ((int)((ulong)uVar3 >> 0x10) << 3 | (uint)uVar3 >> 0xd);
  bVar6 = -1 < (int)(uVar5 - _DAT_ram_1d66);
  if ((bVar6 || !SBORROW2(uVar5,_DAT_ram_1d66)) && (!bVar6 || SBORROW2(uVar5,_DAT_ram_1d66))) {
    uVar5 = _DAT_ram_1d66;
  }
  uVar4 = 0x1df;
  if (_DAT_ram_1268 != 4) {
    uVar4 = 0x36f;
  }
  uVar1 = _DAT_ram_1e20 & 0x10;
  if ((_DAT_ram_1e20 & 0x10) == 0) {
    if ((uVar4 <= uVar5) && ((_DAT_ram_1da4 < 8000 || ((_DAT_ram_125a & 0x10) != 0)))) {
      _DAT_ram_1e20 = _DAT_ram_1e20 | 0x10;
      _DAT_ram_1d6c = uVar1;
      _DAT_ram_1d6e = uVar1;
      _DAT_ram_1d70 = uVar1;
      _DAT_ram_1dca = uVar1;
      _DAT_ram_1e0e = 200;
      return;
    }
  }
  else if (_DAT_ram_1e22 == 2) {
    _DAT_ram_1d70 = 0;
    _DAT_ram_1d6e = 0;
    _DAT_ram_1d6c = 0;
    if (_DAT_ram_1dca + 2 == 0 || _DAT_ram_1dca == -1) {
      _DAT_ram_1d6c = 0;
      _DAT_ram_1d6e = 0;
      _DAT_ram_1d70 = 0;
      _DAT_ram_1dca = _DAT_ram_1dca + 2;
      return;
    }
    _DAT_ram_1dca = 1;
    _DAT_ram_1bf0 = _DAT_ram_1bf0 | 1;
    _DAT_ram_1bf2 = _DAT_ram_1bf2 & 0xfffe;
    FUN_rom_001fd6();
    _DAT_ram_192a = _DAT_ram_192a | 1;
    FUN_rom_004b50();
    _DAT_ram_1e22 = 3;
    return;
  }
  _DAT_ram_1dca = 0;
  return;
}





// Function: FUN_rom_0044dc

void FUN_rom_0044dc(void)

{
  ushort uVar1;
  
  uVar1 = Peripherals::PWM1.IOCON1;
  Peripherals::PWM1.IOCON1 = uVar1 & 0xfdff;
  uVar1 = Peripherals::PWM1.IOCON1;
  Peripherals::PWM1.IOCON1 = uVar1 & 0xfeff;
  uVar1 = Peripherals::PWM2.IOCON2;
  Peripherals::PWM2.IOCON2 = uVar1 & 0xfdff;
  uVar1 = Peripherals::PWM2.IOCON2;
  Peripherals::PWM2.IOCON2 = uVar1 & 0xfeff;
  uVar1 = Peripherals::PWM3.IOCON3;
  Peripherals::PWM3.IOCON3 = uVar1 & 0xfdff;
  uVar1 = Peripherals::PWM3.IOCON3;
  Peripherals::PWM3.IOCON3 = uVar1 & 0xfeff;
  return;
}





// Function: FUN_rom_004508

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

long FUN_rom_004508(void)

{
  ushort uVar1;
  
  if ((_DAT_ram_126a & 0x30) == 0x30) {
    _DAT_ram_1dce = 1;
    if (0x206c < _DAT_ram_1da4) {
      _DAT_ram_1d7e = 0x206c;
      _DAT_ram_1d80 = 0;
      _DAT_ram_1d7a = 0x206c;
      _DAT_ram_1d7c = 0;
      _DAT_ram_1d76 = 0x206c;
      _DAT_ram_1d78 = 0;
      FUN_rom_004b50();
      goto LAB_rom_004548;
    }
    if (0x7fff < _DAT_ram_1da4 + 0xe05dU && SBORROW2(_DAT_ram_1da4,0x1fa3) ||
        0 < (int)(_DAT_ram_1da4 + 0xe05dU) && !SBORROW2(_DAT_ram_1da4,0x1fa3)) goto LAB_rom_004548;
  }
  else {
    if (_DAT_ram_1dce == 0) goto LAB_rom_004548;
    _DAT_ram_126a = _DAT_ram_126a & 0xffdf;
    _DAT_ram_1dce = 0;
  }
  FUN_rom_0044dc();
LAB_rom_004548:
  if ((_DAT_ram_126a & 0x400) == 0) {
    if (_DAT_ram_1dcc != 0) {
      uVar1 = Peripherals::PWM3.IOCON3;
      Peripherals::PWM3.IOCON3 = uVar1 & 0xfdff;
      _DAT_ram_1dcc = 0;
    }
    return (ulong)_DAT_ram_126a << 0x10;
  }
  _DAT_ram_1dcc = 1;
  uVar1 = Peripherals::PWM3.IOCON3;
  Peripherals::PWM3.IOCON3 = uVar1 | 0x200;
  return CONCAT22(_DAT_ram_126a,1);
}





// Function: FUN_rom_004b50

void FUN_rom_004b50(void)

{
  ushort uVar1;
  
  uVar1 = Peripherals::PWM1.IOCON1;
  Peripherals::PWM1.IOCON1 = uVar1 | 0x200;
  uVar1 = Peripherals::PWM1.IOCON1;
  Peripherals::PWM1.IOCON1 = uVar1 | 0x100;
  uVar1 = Peripherals::PWM2.IOCON2;
  Peripherals::PWM2.IOCON2 = uVar1 | 0x200;
  uVar1 = Peripherals::PWM2.IOCON2;
  Peripherals::PWM2.IOCON2 = uVar1 | 0x100;
  uVar1 = Peripherals::PWM3.IOCON3;
  Peripherals::PWM3.IOCON3 = uVar1 | 0x200;
  uVar1 = Peripherals::PWM3.IOCON3;
  Peripherals::PWM3.IOCON3 = uVar1 | 0x100;
  return;
}





// Function: FUN_rom_004b7c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int FUN_rom_004b7c(void)

{
  ushort uVar1;
  int iVar2;
  
  iVar2 = _DAT_ram_1cbe + 1;
  if (_DAT_ram_1cbe == 0) {
    uVar1 = Peripherals::PWM1.IOCON1;
    Peripherals::PWM1.IOCON1 = uVar1 & 0xfeff;
    uVar1 = Peripherals::PWM2.IOCON2;
    Peripherals::PWM2.IOCON2 = uVar1 & 0xfdff;
    _DAT_ram_1cbe = iVar2;
    return 1;
  }
  if (_DAT_ram_1cbe == 2) {
    uVar1 = Peripherals::PWM1.IOCON1;
    Peripherals::PWM1.IOCON1 = uVar1 & 0xfdff;
    uVar1 = Peripherals::PWM2.IOCON2;
    Peripherals::PWM2.IOCON2 = uVar1 & 0xfeff;
    uVar1 = Peripherals::PWM3.IOCON3;
    Peripherals::PWM3.IOCON3 = uVar1 & 0xfdff;
    uVar1 = Peripherals::PWM3.IOCON3;
    Peripherals::PWM3.IOCON3 = uVar1 & 0xfeff;
    iVar2 = 4;
    _DAT_ram_1262 = _DAT_ram_1262 | 0x2000;
  }
  _DAT_ram_1cbe = iVar2;
  return iVar2;
}





// Function: FUN_rom_004bd4

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined2 FUN_rom_004bd4(void)

{
  uint uVar1;
  undefined2 uVar2;
  bool bVar3;
  
  if ((_DAT_ram_1e18 & 0x100) == 0) {
    uVar2 = 0;
    _DAT_ram_1dee = 0;
    _DAT_ram_1df0 = 0;
  }
  else {
    uVar1 = _DAT_ram_1dee + 1;
    bVar3 = _DAT_ram_1dee != 0x3e6;
    _DAT_ram_1dee = uVar1;
    if (0x3e6 < uVar1 && bVar3) {
      _DAT_ram_1dee = 0;
      _DAT_ram_1df0 = _DAT_ram_1df0 + 1;
    }
    uVar2 = 1;
    if (_DAT_ram_1df0 < DAT_ram_1bcd) {
      uVar2 = 0;
    }
  }
  return uVar2;
}





// Function: FUN_rom_004c04

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_004c04(void)

{
  ushort uVar1;
  
  FUN_rom_004b50();
  _DAT_ram_125a = 0;
  _DAT_ram_126a = 0;
  _DAT_ram_1cbe = 0;
  _DAT_ram_1d40 = 0xa0;
  uVar1 = Peripherals::PWM2.IOCON2;
  Peripherals::PWM2.IOCON2 = uVar1 | 0x8000;
  uVar1 = Peripherals::PWM2.IOCON2;
  Peripherals::PWM2.IOCON2 = uVar1 | 0x4000;
  _DAT_ram_1268 = 0;
  _DAT_ram_1d36 = 0x28e;
  _DAT_ram_126e = 0;
  _DAT_ram_1270 = 0;
  _DAT_ram_1d4e = 0xffb0;
  _DAT_ram_1da2 = 10000;
  _DAT_ram_1da4 = 24000;
  _DAT_ram_1d7e = 24000;
  _DAT_ram_1d80 = 0;
  _DAT_ram_1d7a = 24000;
  _DAT_ram_1d7c = 0;
  _DAT_ram_1d76 = 24000;
  _DAT_ram_1d78 = 0;
  _DAT_ram_1e3a = 0x5dc;
  if ((((_DAT_ram_1e10 == 0) && ((_DAT_ram_1e1c & 0x800) != 0)) && ((_DAT_ram_1bf2 & 8) != 0)) &&
     ((_DAT_ram_1e1c & 2) != 0)) {
    if ((_DAT_ram_1e1e & 1) == 0) {
      _DAT_ram_1e1c = _DAT_ram_1e1c | 8;
      _DAT_ram_1bea = _DAT_ram_1bea & 0xfffb;
    }
    else {
      _DAT_ram_1e1e = _DAT_ram_1e1e & 0xfffe;
      _DAT_ram_1e1c = _DAT_ram_1e1c & 0xfff7;
    }
    _DAT_ram_1e0a = 0;
    _DAT_ram_1e22 = 1;
  }
  _DAT_ram_1262 = _DAT_ram_1262 & 0x9fff;
  return;
}





// Function: FUN_rom_004c72

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined2 FUN_rom_004c72(void)

{
  _DAT_ram_1e0a = _DAT_ram_1e0a + 1;
  if (_DAT_ram_1e0a != 0) {
    _DAT_ram_1e0e = 0xa0;
    _DAT_ram_1264 = _DAT_ram_1264 & 0xdfff;
    _DAT_ram_1e1c = _DAT_ram_1e1c & 0xfffb;
    _DAT_ram_1e20 = 0;
    _DAT_ram_1bee = 0;
    _DAT_ram_1bf0 = 0;
    _DAT_ram_1bec = 0;
    if (((_DAT_ram_1e18 & 0x800) != 0) || (_DAT_ram_1d4a = 0xc4d, (_DAT_ram_1e1e & 2) != 0)) {
      _DAT_ram_1d4a = 0x6b3;
    }
    _DAT_ram_1e22 = 2;
    _DAT_ram_1262 = _DAT_ram_1262 | 0x4000;
    _DAT_ram_1e18 = _DAT_ram_1e18 & 0x7fff;
  }
  Peripherals::OC2.OC2RS = 400;
  _DAT_ram_1e38 = 400;
  return 400;
}





// Function: FUN_rom_004ca6

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_004ca6(void)

{
  byte bVar1;
  uint uVar2;
  undefined1 *puVar3;
  
  if (((_DAT_ram_1264 & 0x2000) != 0) && (_DAT_ram_1e20 != 0)) {
    _DAT_ram_1264 = _DAT_ram_1264 & 0xdfff;
    if ((_DAT_ram_1e20 & 10) != 0) {
      _DAT_ram_1e1e = _DAT_ram_1e1e | 3;
    }
    _DAT_ram_125a = 0;
    _DAT_ram_126a = 0;
    _DAT_ram_1bf0 = _DAT_ram_1bf0 | 1;
    _DAT_ram_1bf2 = _DAT_ram_1bf2 & 0xfffe;
    FUN_rom_001fd6(_DAT_ram_1e20 & 10);
    _DAT_ram_192a = _DAT_ram_192a | 1;
  }
  if ((int)_DAT_ram_1e18 < 0) goto LAB_rom_004ce4;
  if ((_DAT_ram_1e18 & 0x800) == 0) {
LAB_rom_004cde:
    _DAT_ram_1e18 = _DAT_ram_1e18 | 0xc000;
  }
  else {
    if (_DAT_ram_1268 == 3) {
      bVar1 = Peripherals::GPIOD.LATD._0_1_;
      if ((bVar1 & 0x10) == 0) goto LAB_rom_004cde;
    }
    _DAT_ram_1e18 = _DAT_ram_1e18 & 0xbfff | 0x8000;
  }
LAB_rom_004ce4:
  _DAT_ram_125a = 0;
  _DAT_ram_126a = 0;
  _DAT_ram_1268 = 0;
  _DAT_ram_1262 = _DAT_ram_1262 & 0xbfff;
  FUN_rom_004b50();
  _DAT_ram_1d4e = 0xffb0;
  puVar3 = &DAT_ram_0802;
  if (((undefined1 *)(_DAT_ram_1e1c & 0x802) != &DAT_ram_0802) || ((_DAT_ram_1262 & 1) != 0)) {
    _DAT_ram_1e10 = 0x9c4;
    _DAT_ram_1e22 = 0;
    return 0x80209c4;
  }
  if ((((_DAT_ram_1262 & 0x40) == 0) || ((_DAT_ram_1e18 & 0x4000) == 0)) &&
     (((_DAT_ram_1262 & 8) == 0 || ((_DAT_ram_1e18 & 0x4000) != 0)))) {
    uVar2 = _DAT_ram_1262;
    if ((_DAT_ram_1e1e & 1) != 0) {
      puVar3 = (undefined1 *)(_DAT_ram_1e20 & 10);
      uVar2 = _DAT_ram_1e20;
      if (puVar3 == (undefined1 *)0x0) {
        uVar2 = 0x9c4;
        _DAT_ram_1e10 = 0x9c4;
        _DAT_ram_1e22 = 0;
      }
    }
    return CONCAT22(puVar3,uVar2);
  }
  _DAT_ram_1e22 = 0;
  _DAT_ram_1e10 = 0x9c4;
  _DAT_ram_1262 = _DAT_ram_1262 & 0xff97;
  return 0x80209c4;
}





// Function: FUN_rom_004d3a

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

uint FUN_rom_004d3a(void)

{
  uint uVar1;
  bool bVar2;
  
  if ((_DAT_ram_1e18 & 0x100) != 0) {
    _DAT_ram_1e18 = _DAT_ram_1e18 & 0xfeff;
    _DAT_ram_1df4 = 0;
    _DAT_ram_1df2 = 0;
    _DAT_ram_1d4e = 0xffb0;
  }
  uVar1 = _DAT_ram_1df2 + 1;
  bVar2 = _DAT_ram_1df2 != 0x3e6;
  _DAT_ram_1df2 = uVar1;
  if (0x3e6 < uVar1 && bVar2) {
    _DAT_ram_1df2 = 0;
    _DAT_ram_1df4 = _DAT_ram_1df4 + 1;
  }
  if (DAT_ram_1bcc == 0xfa) {
    uVar1 = _DAT_ram_1e1c;
    if (((_DAT_ram_1e1c & 0x20) == 0) && (((_DAT_ram_1bf2 & 8) == 0 || ((_DAT_ram_1e1c & 2) == 0))))
    {
      _DAT_ram_1e10 = 0x32;
      _DAT_ram_1e22 = _DAT_ram_1e1c & 0x20;
      return 0;
    }
  }
  else {
    uVar1 = _DAT_ram_1df4;
    if (DAT_ram_1bcc <= _DAT_ram_1df4) {
      uVar1 = 0x32;
      _DAT_ram_1e10 = 0x32;
      _DAT_ram_1e22 = 0;
    }
  }
  return uVar1;
}





// Function: FUN_rom_004d86

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

byte FUN_rom_004d86(void)

{
  byte bVar1;
  
  bVar1 = Peripherals::GPIOF.LATF._0_1_;
  if ((bVar1 & 2) == 0) {
    bVar1 = 0x32;
    _DAT_ram_1e10 = 0x32;
    _DAT_ram_1e22 = 0;
  }
  return bVar1;
}





// Function: FUN_rom_004d94

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_004d94(void)

{
  ushort uVar1;
  
  if (((_DAT_ram_1bea & 0x800) != 0) && ((_DAT_ram_1266 & 0x80) != 0)) {
    uVar1 = Peripherals::GPIOF.LATF;
    Peripherals::GPIOF.LATF = uVar1 | 2;
    return;
  }
  uVar1 = Peripherals::GPIOF.LATF;
  Peripherals::GPIOF.LATF = uVar1 & 0xfffd;
  return;
}





// Function: FUN_rom_004dbc

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_004dbc(undefined2 param_1,undefined2 param_2)

{
  uint uVar1;
  
  if (_DAT_ram_1cc0 == 0) {
    if (_DAT_ram_1e4c < 0x24) {
      uVar1 = 0x23;
    }
    else {
      uVar1 = _DAT_ram_1e4c;
      if (0xe7 < _DAT_ram_1e4c) {
        uVar1 = 0xe8;
      }
    }
    _DAT_ram_1e16 = (uint)(byte)(&DAT_ram_1cde)[-uVar1];
    return CONCAT22(0x1bf6,(uint)(byte)(&DAT_ram_1cde)[-uVar1]);
  }
  _DAT_ram_1cc0 = _DAT_ram_1cc0 + -1;
  return CONCAT22(param_2,_DAT_ram_1cc0);
}





// Function: FUN_rom_004dea

/* WARNING: Heritage AFTER dead removal. Example location: W1 : 0x004ebe */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* WARNING: Restarted to delay deadcode elimination for space: ram */

void FUN_rom_004dea(void)

{
  byte bVar1;
  uint uVar2;
  int iVar3;
  undefined4 in_W1W0;
  uint uVar4;
  ulong uVar5;
  bool bVar6;
  
  if (_DAT_ram_1cc0 == 0) {
    if (0x23 < _DAT_ram_1e4a) {
      in_W1W0 = CONCAT22(0xe8,(int)in_W1W0);
    }
    uVar4 = (uint)((ulong)in_W1W0 >> 0x10);
    uVar5 = (ulong)CONCAT12((&DAT_ram_1cde)[-uVar4],_DAT_ram_1e4a);
    _DAT_ram_1e14 = (uint)(byte)(&DAT_ram_1cde)[-uVar4];
    _DAT_ram_1cc6 = (int)in_W1W0;
    _DAT_ram_1cc4 = 0x42;
    uVar2 = 0x42;
    if ((9 < _DAT_ram_1cc6) && (uVar2 = 0x44, _DAT_ram_1cc4 = uVar2, 0x19 < _DAT_ram_1cc6)) {
      if (0x7fff < _DAT_ram_1cc6 - 0x29U && SBORROW2(_DAT_ram_1cc6,0x29) ||
          0 < (int)(_DAT_ram_1cc6 - 0x29U) && !SBORROW2(_DAT_ram_1cc6,0x29)) {
        if (0x7fff < _DAT_ram_1cc6 - 0x31U && SBORROW2(_DAT_ram_1cc6,0x31) ||
            0 < (int)(_DAT_ram_1cc6 - 0x31U) && !SBORROW2(_DAT_ram_1cc6,0x31)) {
          _DAT_ram_1cc4 = 0x55;
          uVar2 = _DAT_ram_1cc4;
        }
        else {
          uVar2 = (_DAT_ram_1cc6 + -0x2a) * 0x514;
          _DAT_ram_1cc4 = ((int)uVar2 >> 10) + 0x50;
          if ((int)uVar2 >> 10 != 5 && 0x54 < (int)_DAT_ram_1cc4) {
            _DAT_ram_1cc4 = 0x55;
          }
        }
      }
      else {
        _DAT_ram_1cc4 = _DAT_ram_1cc6 + 0x26;
        if ((_DAT_ram_1cc6 - 0x1dU < 0x8000 || !SBORROW2(_DAT_ram_1cc4,0x43)) &&
            ((int)(_DAT_ram_1cc6 - 0x1dU) < 1 || SBORROW2(_DAT_ram_1cc4,0x43))) {
          _DAT_ram_1cc4 = 0x44;
          uVar2 = _DAT_ram_1cc4;
        }
      }
    }
    if (((_DAT_ram_126c < 0x14) || (bVar1 = Peripherals::GPIOF.LATF._0_1_, (bVar1 & 1) != 0)) ||
       (_DAT_ram_1268 != 3 && _DAT_ram_1268 != 4)) {
      _DAT_ram_1dfa = 0;
      _DAT_ram_1dfc = 0;
      _DAT_ram_1cc2 = 0x69;
    }
    else {
      iVar3 = _DAT_ram_1dfa + 1;
      bVar6 = _DAT_ram_1dfa != 999;
      _DAT_ram_1dfa = iVar3;
      if (bVar6 && 999 < iVar3) {
        _DAT_ram_1dfa = 1000;
        _DAT_ram_1cc2 = _DAT_ram_1cc4;
      }
      FUN_rom_001100(_DAT_ram_126c,uVar4,_DAT_ram_1e4e,0);
      _DAT_ram_1df6 = uVar2;
      _DAT_ram_1df8 = uVar4;
      if (uVar2 == 0x2183 && uVar4 == uVar2 < 0x2183 || (int)uVar4 < (int)(uint)(uVar2 < 0x2183)) {
        _DAT_ram_1dfc = 0;
      }
      else {
        iVar3 = _DAT_ram_1dfc + 1;
        bVar6 = _DAT_ram_1dfc != 0x4af;
        _DAT_ram_1dfc = iVar3;
        if (bVar6 && 0x4af < iVar3) {
          _DAT_ram_1dfc = 0x4b0;
          _DAT_ram_1e20 = _DAT_ram_1e20 | 0x400;
        }
      }
    }
    if ((_DAT_ram_1cc2 <= (uint)(uVar5 >> 0x10)) || ((uint)uVar5 < 0x29)) {
      _DAT_ram_1e20 = _DAT_ram_1e20 | 0x400;
    }
  }
  return;
}





// Function: FUN_rom_004efc

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ulong FUN_rom_004efc(int param_1,uint param_2)

{
  byte bVar1;
  uint uVar2;
  
  if (_DAT_ram_1cc0 == 0) {
    if (_DAT_ram_1e48 < 0x24) {
      uVar2 = 0x23;
    }
    else {
      uVar2 = 0xe8;
      if (_DAT_ram_1e48 < 0xe8) {
        uVar2 = _DAT_ram_1e48;
      }
    }
    bVar1 = (&DAT_ram_1cde)[-uVar2];
    param_2 = (uint)bVar1;
    _DAT_ram_1e12 = param_2;
    if ((int)_DAT_ram_1e1a < 0) {
      param_1 = 0x55;
      if (param_2 < 0x56) {
        param_1 = 0xf5;
        if (_DAT_ram_1e48 < 0xf6) {
          _DAT_ram_1e1a = _DAT_ram_1e1a & 0x7f7f;
          return (ulong)CONCAT12(bVar1,0xf5);
        }
      }
    }
    else {
      param_1 = _DAT_ram_1268;
      if ((_DAT_ram_1e1a & 0x80) == 0) {
        if (_DAT_ram_1268 == 3) {
          param_1 = 0x68;
          if (param_2 < 0x69) {
            param_1 = 0xf9;
            if (_DAT_ram_1e48 < 0xfa) goto LAB_rom_004f90;
          }
          _DAT_ram_192a = _DAT_ram_192a | 1;
          _DAT_ram_1e1a = _DAT_ram_1e1a | 0x80;
          return (ulong)CONCAT12(bVar1,param_1);
        }
        if (_DAT_ram_1268 == 4) {
          param_1 = 0x6a;
          if (param_2 < 0x6b) {
            param_1 = 0xf9;
            if (_DAT_ram_1e48 < 0xfa) goto LAB_rom_004f90;
          }
          _DAT_ram_1e1a = _DAT_ram_1e1a | 0x8000;
        }
      }
      else {
        _DAT_ram_1e1e = _DAT_ram_1e1e | 4;
        if (param_2 < 0x56) {
          if (_DAT_ram_1e48 < 0xf6) {
            _DAT_ram_1e1a = _DAT_ram_1e1a & 0x7f7f;
          }
        }
        if (_DAT_ram_1268 == 4) {
          param_1 = 0x6a;
          if (param_2 < 0x6b) {
            param_1 = 0xf9;
            if (_DAT_ram_1e48 < 0xfa) goto LAB_rom_004f90;
          }
          _DAT_ram_1e1a = _DAT_ram_1e1a | 0x8000;
          return (ulong)CONCAT12(bVar1,param_1);
        }
      }
    }
  }
LAB_rom_004f90:
  return CONCAT22(param_2,param_1);
}





// Function: FUN_rom_004f92

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_004f92(undefined2 param_1,uint param_2)

{
  int iVar1;
  uint uVar2;
  
  if ((_DAT_ram_1264 & 0x4000) == 0) {
    if ((_DAT_ram_1e1a & 0x20) == 0) {
      iVar1 = _DAT_ram_1268;
      uVar2 = param_2;
      if (_DAT_ram_1268 == 3) {
        iVar1 = 0x66;
        uVar2 = _DAT_ram_1d16;
        if (0x66 < _DAT_ram_1d16) {
          _DAT_ram_192a = _DAT_ram_192a | 1;
          _DAT_ram_1e1a = _DAT_ram_1e1a | 0x20;
        }
      }
      goto LAB_rom_004fba;
    }
    _DAT_ram_1e1e = _DAT_ram_1e1e | 4;
  }
  iVar1 = 0x55;
  uVar2 = _DAT_ram_1d16;
  if (_DAT_ram_1d16 < 0x56) {
    _DAT_ram_1e1a = _DAT_ram_1e1a & 0xffdf;
    return CONCAT22(_DAT_ram_1d16,0x55);
  }
LAB_rom_004fba:
  return CONCAT22(uVar2,iVar1);
}





// Function: FUN_rom_004fbc

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_004fbc(void)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  byte bVar4;
  
  bVar4 = Peripherals::GPIOC.PORTC._1_1_;
  uVar1 = bVar4 >> 5 & 2;
  bVar4 = Peripherals::GPIOC.PORTC._1_1_;
  uVar2 = bVar4 >> 3 & 4;
  bVar4 = Peripherals::GPIOD.PORTD._1_1_;
  uVar3 = (bVar4 & 4) << 1;
  _DAT_ram_1e08 = _DAT_ram_1e08 & 0xfff1 | uVar1 | uVar2 | uVar3;
  uVar1 = (uVar1 | uVar2 | uVar3 | 0xb0) >> 1;
  Peripherals::I2C2.I2C2ADD = uVar1;
  return CONCAT22(uVar3,uVar1);
}





// Function: FUN_rom_004fee

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_004fee(void)

{
  byte bVar1;
  GPIOE *pGVar2;
  uint uVar3;
  ushort uVar4;
  bool bVar5;
  
  if ((_DAT_ram_1bf2 & 8) == 0) {
    uVar4 = Peripherals::GPIOD.LATD;
    Peripherals::GPIOD.LATD = uVar4 | 8;
    _DAT_ram_1e0c = 0;
    goto LAB_rom_005080;
  }
  if ((_DAT_ram_1e22 == 2) && ((_DAT_ram_1e20 & 0x100) == 0)) {
    if ((_DAT_ram_1e1a & 0xa0) == 0) {
LAB_rom_005010:
      if ((_DAT_ram_1268 == 4) && ((_DAT_ram_1e20 & 2) != 0)) goto LAB_rom_005076;
    }
    else if (_DAT_ram_1268 == 4) {
      if ((_DAT_ram_1e18 & 0x800) == 0) goto LAB_rom_005076;
      goto LAB_rom_005010;
    }
    if (((_DAT_ram_1e1c & 2) == 0) && (bVar1 = Peripherals::GPIOD.LATD._0_1_, (bVar1 & 8) == 0)) {
      bVar1 = Peripherals::GPIOD.LATD._0_1_;
      if ((bVar1 & 0x10) == 0) {
        uVar4 = Peripherals::ADC.ADCBUF5;
        uVar3 = 0x2d9;
      }
      else {
        uVar4 = Peripherals::ADC.ADCBUF5;
        uVar3 = 0x153;
      }
      _DAT_ram_1e0c = 0;
      if (uVar4 <= uVar3) {
        uVar4 = Peripherals::GPIOD.LATD;
        Peripherals::GPIOD.LATD = uVar4 | 8;
      }
    }
    else if ((_DAT_ram_1e1a & 0xa0) == 0) {
      if (_DAT_ram_1268 == 4) {
        uVar4 = Peripherals::ADC.ADCBUF5;
        if (uVar4 < 0x17a) goto LAB_rom_005080;
      }
      else {
        if (_DAT_ram_1268 != 3) goto LAB_rom_005080;
        pGVar2 = (GPIOE *)Peripherals::ADC.ADCBUF5;
        if (pGVar2 < &Peripherals::GPIOE) {
          _DAT_ram_1e0c = 0;
          goto LAB_rom_005080;
        }
      }
      uVar3 = _DAT_ram_1e0c + 1;
      bVar5 = _DAT_ram_1e0c != 0x3a;
      _DAT_ram_1e0c = uVar3;
      if (0x3a < uVar3 && bVar5) {
        _DAT_ram_1e0c = 0x3c;
        uVar4 = Peripherals::GPIOD.LATD;
        Peripherals::GPIOD.LATD = uVar4 & 0xfff7;
      }
    }
  }
  else {
LAB_rom_005076:
    uVar4 = Peripherals::GPIOD.LATD;
    Peripherals::GPIOD.LATD = uVar4 | 8;
    _DAT_ram_1e0c = 0;
  }
LAB_rom_005080:
  bVar1 = Peripherals::GPIOD.LATD._0_1_;
  if ((bVar1 & 8) == 0) {
    _DAT_ram_1bf2 = _DAT_ram_1bf2 | 1;
    return;
  }
  _DAT_ram_1bf2 = _DAT_ram_1bf2 & 0xfffe;
  return;
}





// Function: FUN_rom_00508e

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined2 FUN_rom_00508e(void)

{
  DAT_ram_1d08 = 0x48;
  DAT_ram_1d09 = 0x30;
  DAT_ram_1d0a = 0x35;
  DAT_ram_1d0b = 0x57;
  DAT_ram_1d0c = 0x44;
  _DAT_ram_1e4c = 0xc1;
  _DAT_ram_1e48 = 0xc1;
  _DAT_ram_1e16 = 0x19;
  _DAT_ram_1e12 = 0x19;
  _DAT_ram_1d18 = 0x19;
  _DAT_ram_1e10 = 10;
  _DAT_ram_1d3e = 0x15e;
  _DAT_ram_1e22 = 0;
  Peripherals::OC2.OC2RS = 400;
  _DAT_ram_1e38 = 400;
  return 400;
}





// Function: FUN_rom_0050c2

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_0050c2(void)

{
  byte bVar1;
  
  if ((_DAT_ram_1e1a & 1) != 0) {
    bVar1 = Peripherals::I2C2.I2C2STAT._0_1_;
    if ((bVar1 & 0x10) != 0) {
      _DAT_ram_1e1a = _DAT_ram_1e1a & 0xfffe;
      FUN_rom_005ae4();
    }
  }
  return;
}





// Function: FUN_rom_0050d4

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_0050d4(undefined2 param_1,undefined2 param_2)

{
  uint uVar1;
  undefined4 uVar2;
  bool bVar3;
  
  if ((_DAT_ram_1e1a & 0x200) == 0) {
    _DAT_ram_1dfe = 0;
    _DAT_ram_1e00 = 0;
  }
  else {
    uVar1 = _DAT_ram_1dfe + 1;
    _DAT_ram_1e00 = _DAT_ram_1e00 + (0xfffe < _DAT_ram_1dfe);
    param_1 = 0x773f;
    param_2 = 0x1b;
    bVar3 = _DAT_ram_1dfe != 0x773e;
    _DAT_ram_1dfe = uVar1;
    if ((0x1a < _DAT_ram_1e00 && (uVar1 < 0x773f) + 0x1b <= _DAT_ram_1e00) &&
        (bVar3 || _DAT_ram_1e00 - 0x1b != (uint)(uVar1 < 0x773f))) {
      _DAT_ram_1e1a = _DAT_ram_1e1a & 0xfdff;
      FUN_rom_005a88(0x77ff,0xff,uVar1 & 0xff00);
      uVar2 = FUN_rom_005ab8();
      _DAT_ram_1bbc = (int)uVar2;
      return uVar2;
    }
  }
  return CONCAT22(param_2,param_1);
}





// Function: FUN_rom_005108

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_005108(void)

{
  byte bVar1;
  ushort uVar2;
  uint uVar3;
  bool bVar4;
  
  if ((_DAT_ram_1262 & 0x4000) == 0) {
    uVar2 = Peripherals::GPIOE.LATE;
    Peripherals::GPIOE.LATE = uVar2 & 0xffbf;
    uVar3 = 0;
    _DAT_ram_1e02 = 0;
  }
  else {
    bVar1 = Peripherals::GPIOE.LATE._0_1_;
    uVar3 = CONCAT11((char)((_DAT_ram_1262 & 0x4000) >> 8),bVar1);
    if ((bVar1 & 0x40) == 0) {
      uVar3 = _DAT_ram_1e02 + 1;
      bVar4 = _DAT_ram_1e02 != 0x1c;
      _DAT_ram_1e02 = uVar3;
      if (0x1c < uVar3 && bVar4) {
        _DAT_ram_1e02 = 0x1e;
        uVar2 = Peripherals::GPIOE.LATE;
        Peripherals::GPIOE.LATE = uVar2 | 0x40;
        return CONCAT22(_DAT_ram_1262,0x1e);
      }
    }
  }
  return CONCAT22(_DAT_ram_1262,uVar3);
}





// Function: FUN_rom_00512c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_00512c(void)

{
  byte bVar1;
  uint uVar2;
  
  if (0x13e < _DAT_ram_125e) {
    _DAT_ram_1e1c = _DAT_ram_1e1c | 0x20;
    return CONCAT22(_DAT_ram_125e,0x13e);
  }
  uVar2 = 0xff;
  if (_DAT_ram_125e < 0x100) {
    _DAT_ram_1e1c = _DAT_ram_1e1c & 0xffdf;
    bVar1 = Peripherals::GPIOF.LATF._0_1_;
    uVar2 = (uint)bVar1;
    if ((bVar1 & 2) != 0) {
      _DAT_ram_1bea = _DAT_ram_1bea & 0xf7ff;
    }
  }
  return CONCAT22(_DAT_ram_125e,uVar2);
}





// Function: FUN_rom_00514a

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_00514a(void)

{
  uint uVar1;
  byte bVar2;
  uint uVar3;
  uint uVar4;
  bool bVar5;
  
  uVar1 = _DAT_ram_1e20 & 1;
  uVar4 = _DAT_ram_1e1c;
  if (uVar1 == 0) {
    _DAT_ram_1262 = _DAT_ram_1262 & 0xfffe;
    _DAT_ram_1e06 = 0;
    if ((_DAT_ram_1e1c & 0x80) == 0) {
      uVar3 = 0;
      _DAT_ram_1e04 = 0;
    }
    else {
      bVar2 = Peripherals::GPIOD.LATD._0_1_;
      uVar3 = (uint)bVar2;
      if ((bVar2 & 8) == 0) {
        uVar4 = _DAT_ram_1e04 + 1;
        uVar3 = 999;
        bVar5 = _DAT_ram_1e04 != 0x3e6;
        _DAT_ram_1e04 = uVar4;
        if (0x3e6 < uVar4 && bVar5) {
          _DAT_ram_1e1c = _DAT_ram_1e1c & 0xff7f;
          _DAT_ram_1e06 = uVar1;
          return CONCAT22(uVar4,999);
        }
      }
    }
  }
  else {
    uVar3 = uVar1;
    if ((_DAT_ram_1e1c & 0x80) == 0) {
      uVar4 = _DAT_ram_1e06 + 1;
      uVar3 = 7999;
      bVar5 = _DAT_ram_1e06 != 0x1f3e;
      _DAT_ram_1e06 = uVar4;
      if (0x1f3e < uVar4 && bVar5) {
        _DAT_ram_1262 = _DAT_ram_1262 | 1;
        _DAT_ram_1e1c = _DAT_ram_1e1c | 0x80;
        return CONCAT22(uVar4,7999);
      }
    }
  }
  return CONCAT22(uVar4,uVar3);
}





// Function: FUN_rom_00518c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_00518c(void)

{
  byte bVar1;
  char cVar2;
  uint uVar3;
  
  if (_DAT_ram_1e20 == 4) {
    if ((_DAT_ram_1bea & 0x20) == 0) goto LAB_rom_0051b8;
    uVar3 = 4;
  }
  else {
    if (_DAT_ram_1e20 == 0) goto LAB_rom_0051b8;
    uVar3 = _DAT_ram_1e20 & 10;
    if (uVar3 != 0) {
      _DAT_ram_1e1e = _DAT_ram_1e1e | 3;
    }
  }
  _DAT_ram_125a = 0;
  _DAT_ram_126a = 0;
  _DAT_ram_1bf0 = _DAT_ram_1bf0 | 1;
  _DAT_ram_1bf2 = _DAT_ram_1bf2 & 0xfffe;
  FUN_rom_001fd6(uVar3);
  _DAT_ram_192a = _DAT_ram_192a | 1;
  FUN_rom_004b50();
  _DAT_ram_1e22 = 3;
LAB_rom_0051b8:
  cVar2 = FUN_rom_004bd4();
  if (cVar2 == '\0') {
    _DAT_ram_1264 = _DAT_ram_1264 & 0xefff;
  }
  else {
    _DAT_ram_1264 = _DAT_ram_1264 | 0x1000;
  }
  if (((_DAT_ram_1264 & 0x1000) != 0) && ((_DAT_ram_1e1c & 2) == 0)) {
    _DAT_ram_1bf2 = _DAT_ram_1bf2 & 0xfffe;
    FUN_rom_001fd6();
    _DAT_ram_192a = _DAT_ram_192a | 1;
    FUN_rom_004b50();
    _DAT_ram_1e22 = 5;
    _DAT_ram_1264 = _DAT_ram_1264 & 0xefff;
  }
  bVar1 = Peripherals::GPIOF.LATF._0_1_;
  if ((bVar1 & 2) != 0) {
    FUN_rom_004b50();
    _DAT_ram_1e22 = 6;
  }
  if (_DAT_ram_1e0e != 0) {
    _DAT_ram_1e0e = _DAT_ram_1e0e + -1;
  }
  if (_DAT_ram_1d40 != 0) {
    _DAT_ram_1d40 = _DAT_ram_1d40 + -1;
  }
  return;
}





// Function: FUN_rom_0051fe

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_0051fe(void)

{
  ushort uVar1;
  byte bVar2;
  
  bVar2 = Peripherals::INTERRUPT.IFS1._1_1_;
  if ((bVar2 & 8) != 0) {
    if (_DAT_ram_1e22 == 2) {
      FUN_rom_00518c();
    }
    else if (_DAT_ram_1e22 < 2) {
      if (_DAT_ram_1e22 == 0) {
        FUN_rom_004c04();
      }
      else if (_DAT_ram_1e22 == 1) {
        FUN_rom_004c72();
      }
    }
    else if (_DAT_ram_1e22 == 5) {
      FUN_rom_004d3a();
    }
    else if (_DAT_ram_1e22 == 6) {
      FUN_rom_004d86();
    }
    else if (_DAT_ram_1e22 == 3) {
      FUN_rom_004ca6();
    }
    FUN_rom_004dbc();
    FUN_rom_004dea();
    FUN_rom_004efc();
    FUN_rom_004f92();
    _DAT_ram_1e20 = _DAT_ram_1e20 & 0xfffd | (_DAT_ram_1264 >> 0xe & 1 | _DAT_ram_1e1a >> 0xf) * 2;
    FUN_rom_00571e();
    FUN_rom_0029fa();
    FUN_rom_004fbc();
    FUN_rom_004fee();
    FUN_rom_005108();
    FUN_rom_00546c();
    FUN_rom_004d94();
    FUN_rom_003b5e();
    FUN_rom_0040c2();
    FUN_rom_003f0e();
    FUN_rom_0042c8();
    FUN_rom_0050d4();
    FUN_rom_00512c();
    FUN_rom_00514a();
    uVar1 = Peripherals::INTERRUPT.IFS1;
    Peripherals::INTERRUPT.IFS1 = uVar1 & 0xf7ff;
  }
  return;
}





// Function: FUN_rom_005288

void FUN_rom_005288(void)

{
  ushort uVar1;
  byte bVar2;
  
  Peripherals::TIMER3.T3CON = 0;
  Peripherals::TIMER3.TMR3 = 0;
  Peripherals::TIMER3.PR3 = 2000;
  uVar1 = Peripherals::TIMER3.T3CON;
  Peripherals::TIMER3.T3CON = uVar1 | 0x8000;
  Peripherals::OC2.OC2CON = 8;
  Peripherals::OC2.OC2R = 0;
  Peripherals::OC2.OC2RS = 400;
  Peripherals::OC2.OC2CON = 0xe;
  Peripherals::PWM_MASTER.PTCON = 0x400;
  bVar2 = Peripherals::INTERRUPT.IPC14._0_1_;
  Peripherals::INTERRUPT.IPC14._0_1_ = bVar2 & 0x8f | 0x50;
  uVar1 = Peripherals::INTERRUPT.IEC3;
  Peripherals::INTERRUPT.IEC3 = uVar1 | 0x200;
  Peripherals::PWM_MASTER.SEVTCMP = 0;
  bVar2 = Peripherals::PWM_MASTER.PTCON2._0_1_;
  Peripherals::PWM_MASTER.PTCON2._0_1_ = bVar2 & 0xf8 | 3;
  Peripherals::PWM_MASTER.PTPER = 0x11e;
  Peripherals::PWM1.IOCON1 = 0xc800;
  Peripherals::PWM1.PWMCON1 = 0x1001;
  Peripherals::PWM1.PDC1 = 0x126;
  Peripherals::PWM1.DTR1 = 0x2a;
  Peripherals::PWM1.ALTDTR1 = 0x2a;
  Peripherals::PWM1.PHASE1 = 0;
  Peripherals::PWM2.IOCON2 = 0xc800;
  Peripherals::PWM2.PWMCON2 = 0x1001;
  Peripherals::PWM2.PDC2 = 0x126;
  Peripherals::PWM2.DTR2 = 0x2a;
  Peripherals::PWM2.ALTDTR2 = 0x2a;
  Peripherals::PWM2.PHASE2 = 0;
  Peripherals::PWM3.IOCON3 = 0xc800;
  Peripherals::PWM3.PWMCON3 = 0x1001;
  Peripherals::PWM3.PDC3 = 0x126;
  Peripherals::PWM3.DTR3 = 0x2f;
  Peripherals::PWM3.ALTDTR3 = 0x2f;
  Peripherals::PWM3.PHASE3 = 0x82;
  Peripherals::PWM5.PWMCON5 = 0x280;
  Peripherals::PWM5.PHASE5 = 0x49b;
  Peripherals::PWM5.PDC5 = 0x372;
  Peripherals::PWM5.IOCON5 = 0x8c00;
  Peripherals::PWM1.FCLCON1 = 0x10;
  Peripherals::PWM2.FCLCON2 = 0x10;
  Peripherals::PWM3.FCLCON3 = 0x10;
  FUN_rom_004b50(0x10,0x126,0x1001,0xc800,0);
  uVar1 = Peripherals::PWM_MASTER.PTCON;
  Peripherals::PWM_MASTER.PTCON = uVar1 | 0x8000;
  return;
}





// Function: FUN_rom_005312

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_rom_005312(uint *param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4)

{
  byte bVar1;
  uint uVar2;
  OC4 *pOVar3;
  
  if (_DAT_ram_1e22 == 2) {
    bVar1 = Peripherals::GPIOD.LATD._0_1_;
    uVar2 = (uint)bVar1;
    if ((bVar1 & 0x10) != 0) {
      pOVar3 = (OC4 *)*param_1;
      param_4 = 0x191;
      if (pOVar3 < &Peripherals::OC4) {
LAB_rom_00532e:
        *param_1 = 0x191;
        return CONCAT26(pOVar3,CONCAT24(uVar2,CONCAT22(0x191,param_1)));
      }
      uVar2 = 0xf000;
      if ((OC4 *)0xf000 < pOVar3) goto LAB_rom_00532e;
      goto LAB_rom_005348;
    }
  }
  pOVar3 = (OC4 *)*param_1;
  if (pOVar3 < (OC4 *)0x2) {
    *param_1 = 1;
    return CONCAT26(pOVar3,CONCAT24(1,CONCAT22(param_4,param_1)));
  }
  uVar2 = 0xf000;
  if ((OC4 *)0xf000 < pOVar3) {
    uVar2 = 1;
    *param_1 = 1;
  }
LAB_rom_005348:
  return CONCAT26(pOVar3,CONCAT24(uVar2,CONCAT22(param_4,param_1)));
}





// Function: FUN_rom_00534a

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_00534a(void)

{
  uint uVar1;
  
  uVar1 = (uint)(_DAT_ram_1e4e * 0xb) / 0x22f;
  if (10 < uVar1) {
    uVar1 = 10;
  }
  return CONCAT22(uVar1,uVar1);
}





// Function: FUN_rom_005364

undefined2 FUN_rom_005364(int param_1,int param_2)

{
  undefined2 uVar1;
  bool bVar2;
  
  param_1 = param_1 - param_2;
  uVar1 = 0;
  if ((param_1 - 5U < 0x8000 || !SBORROW2(param_1,5)) &&
      ((int)(param_1 - 5U) < 1 || SBORROW2(param_1,5))) {
    uVar1 = 1;
    if ((param_1 - 1U < 0x8000 || !SBORROW2(param_1,1)) &&
        ((int)(param_1 - 1U) < 1 || SBORROW2(param_1,1))) {
      uVar1 = 4;
      bVar2 = -1 < param_1 + 5;
      if ((bVar2 || SCARRY2(param_1,5)) && (!bVar2 || !SCARRY2(param_1,5))) {
        uVar1 = 3;
        bVar2 = -1 < param_1 + 1;
        if ((bVar2 || SCARRY2(param_1,1)) && (!bVar2 || !SCARRY2(param_1,1))) {
          uVar1 = 2;
        }
      }
    }
  }
  return uVar1;
}





// Function: FUN_rom_005398

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined2 FUN_rom_005398(int *param_1,int param_2,int param_3,int param_4)

{
  int iVar1;
  int *piVar2;
  int iVar3;
  bool bVar4;
  
  iVar3 = 0;
  piVar2 = param_1;
  while( true ) {
    iVar1 = *piVar2;
    bVar4 = -1 < param_4 - iVar1;
    if ((bVar4 || !SBORROW2(param_4,iVar1)) && (!bVar4 || SBORROW2(param_4,iVar1))) break;
    iVar3 = iVar3 + 1;
    piVar2 = piVar2 + 1;
  }
  bVar4 = -1 < iVar3 - param_3;
  if ((bVar4 || SBORROW2(iVar3,param_3)) && (!bVar4 || !SBORROW2(iVar3,param_3))) {
    iVar3 = param_3 + -1;
  }
  if (_DAT_ram_1e50 == 0) {
    _DAT_ram_1e50 = 1;
  }
  bVar4 = -1 < iVar3 - _DAT_ram_1e50;
  if ((bVar4 || !SBORROW2(iVar3,_DAT_ram_1e50)) && (!bVar4 || SBORROW2(iVar3,_DAT_ram_1e50))) {
    param_4 = param_4 + 5;
    iVar1 = param_1[_DAT_ram_1e50 + -1];
    bVar4 = param_4 - iVar1 < 0;
    if (bVar4 && SBORROW2(param_4,iVar1) || !bVar4 && !SBORROW2(param_4,iVar1)) goto LAB_rom_0053ca;
  }
  _DAT_ram_1e50 = iVar3;
LAB_rom_0053ca:
  return *(undefined2 *)(_DAT_ram_1e50 * 2 + param_2);
}





// Function: FUN_rom_0053d2

/* WARNING: Heritage AFTER dead removal. Example location: W1 : 0x0053e4 */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* WARNING: Restarted to delay deadcode elimination for space: ram */

undefined4 FUN_rom_0053d2(void)

{
  undefined4 uVar1;
  
  if (_DAT_ram_1e22 == 2) {
    FUN_rom_00110c(_DAT_ram_1d44 * 100,0,0x22f,0);
  }
  if (_DAT_ram_1e12 < 0x58) {
    if (_DAT_ram_1d18 < 0x58) {
      uVar1 = FUN_rom_005398(0x1e2e,0,5,0);
      return uVar1;
    }
  }
  _DAT_ram_1e38 = 1999;
  return 0x54c4;
}





// Function: FUN_rom_00540a

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_rom_00540a(void)

{
  int iVar1;
  int iVar2;
  
  if (_DAT_ram_1bd4 + 3000 < _DAT_ram_1e3a) {
    iVar1 = 0x32;
  }
  else if (_DAT_ram_1bd4 + 100 < _DAT_ram_1e3a) {
    iVar1 = 0x19;
  }
  else if (_DAT_ram_1bd4 - 100 < _DAT_ram_1e3a) {
    iVar1 = 0xc;
  }
  else if (_DAT_ram_1e3a + 3000 < _DAT_ram_1bd4) {
    iVar1 = -0x32;
  }
  else if (_DAT_ram_1e3a + 0x4b0 < _DAT_ram_1bd4) {
    iVar1 = -0xc;
  }
  else {
    if (_DAT_ram_1bd4 <= _DAT_ram_1e3a + 300) goto LAB_rom_005454;
    iVar1 = -6;
  }
  _DAT_ram_1e38 = _DAT_ram_1e38 + iVar1;
LAB_rom_005454:
  iVar1 = _DAT_ram_1e38;
  if (1999 < _DAT_ram_1e38) {
    _DAT_ram_1e38 = 1999;
    return CONCAT26(iVar1,CONCAT24(1999,CONCAT22(3000,_DAT_ram_1e3a)));
  }
  iVar2 = 0xc6;
  if ((_DAT_ram_1e38 - 0xc6U < 0x8000 || !SBORROW2(_DAT_ram_1e38,0xc6)) &&
      ((int)(_DAT_ram_1e38 - 0xc6U) < 1 || SBORROW2(_DAT_ram_1e38,0xc6))) {
    iVar2 = 199;
    _DAT_ram_1e38 = iVar2;
  }
  return CONCAT26(iVar1,CONCAT24(iVar2,CONCAT22(3000,_DAT_ram_1e3a)));
}





// Function: FUN_rom_00546c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined2 FUN_rom_00546c(undefined2 param_1)

{
  byte bVar1;
  ushort uVar2;
  undefined1 uVar3;
  uint uVar4;
  undefined1 uVar6;
  undefined2 uVar5;
  
  if ((_DAT_ram_1e18 & 1) == 0) {
    _DAT_ram_1e18 = _DAT_ram_1e18 & 0xdfff;
  }
  bVar1 = Peripherals::GPIOD.LATD._0_1_;
  uVar4 = CONCAT11((char)((uint)param_1 >> 8),bVar1);
  if (((bVar1 & 0x10) == 0) || ((_DAT_ram_1262 & 0x20) != 0)) {
LAB_rom_00548a:
    if (((_DAT_ram_126a & 0x20) == 0) && (DAT_ram_15f6 == '\0')) {
      uVar3 = (char)(uVar4 >> 8);
      if ((_DAT_ram_1e18 & 0x2000) != 0) {
        uVar4 = 0;
        if (_DAT_ram_1d18 < 0x3f) goto LAB_rom_00549e;
        uVar6 = 0;
        uVar3 = 0;
        if (_DAT_ram_1d18 < 0x57) goto LAB_rom_0054aa;
      }
      uVar6 = uVar3;
      uVar2 = Peripherals::GPIOD.LATD;
      Peripherals::GPIOD.LATD = uVar2 | 0x20;
      goto LAB_rom_0054aa;
    }
  }
  else {
    uVar4 = 0xc4d;
    if (_DAT_ram_1d4a != 0xc4d) goto LAB_rom_00548a;
    uVar4 = _DAT_ram_1e1a & 0xa0;
    if (uVar4 != 0) goto LAB_rom_00548a;
  }
LAB_rom_00549e:
  uVar6 = (undefined1)(uVar4 >> 8);
  uVar2 = Peripherals::GPIOD.LATD;
  Peripherals::GPIOD.LATD = uVar2 & 0xffdf;
LAB_rom_0054aa:
  bVar1 = Peripherals::GPIOD.LATD._0_1_;
  uVar5 = CONCAT11(uVar6,bVar1);
  if ((bVar1 & 0x20) == 0) {
    if (_DAT_ram_1268 == 4) {
      _DAT_ram_1e38 = 400;
      return 400;
    }
    uVar5 = 100;
    _DAT_ram_1e38 = 100;
  }
  return uVar5;
}





// Function: FUN_rom_0054c2

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_0054c2(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  undefined4 in_W1W0;
  undefined2 uVar4;
  undefined2 extraout_W1;
  bool bVar5;
  
  uVar4 = (undefined2)((ulong)in_W1W0 >> 0x10);
  iVar1 = _DAT_ram_1e52 + 1;
  bVar5 = _DAT_ram_1e52 != 8;
  _DAT_ram_1e52 = iVar1;
  if (bVar5 && 8 < iVar1) {
    _DAT_ram_1e52 = 0;
    iVar2 = FUN_rom_00534a();
    iVar1 = FUN_rom_005364(_DAT_ram_1e12);
    iVar3 = FUN_rom_005364(_DAT_ram_1d18);
    if (iVar3 < iVar1) {
      iVar1 = iVar3;
    }
    _DAT_ram_1e3a = _DAT_ram_1e3a + *(int *)((iVar1 + iVar2 * 5) * 2 + -0x6cb6);
    iVar2 = FUN_rom_0053d2();
    iVar1 = _DAT_ram_1bb0;
    if (_DAT_ram_1bb0 < iVar2) {
      iVar1 = iVar2;
    }
    bVar5 = -1 < _DAT_ram_1e3a - iVar1;
    if ((bVar5 || !SBORROW2(_DAT_ram_1e3a,iVar1)) && (!bVar5 || SBORROW2(_DAT_ram_1e3a,iVar1))) {
      _DAT_ram_1e3a = iVar1;
    }
    if (0x54c4 < _DAT_ram_1e3a) {
      _DAT_ram_1e3a = 0x54c4;
    }
    if ((_DAT_ram_1e3a - 0x5dbU < 0x8000 || !SBORROW2(_DAT_ram_1e3a,0x5db)) &&
        ((int)(_DAT_ram_1e3a - 0x5dbU) < 1 || SBORROW2(_DAT_ram_1e3a,0x5db))) {
      _DAT_ram_1e3a = 0x5dc;
    }
    FUN_rom_00540a();
    uVar4 = extraout_W1;
  }
  return CONCAT22(uVar4,_DAT_ram_1e38);
}





// Function: FUN_rom_005522

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ulong FUN_rom_005522(void)

{
  uint uVar1;
  ushort uVar2;
  ulong in_W1W0;
  ulong uVar3;
  uint uVar4;
  
  if ((_DAT_ram_1e1c & 0x100) != 0) {
    _DAT_ram_1e1c = _DAT_ram_1e1c & 0xfeff;
    if (199 < _DAT_ram_1bd4) {
      uVar3 = FUN_rom_0054c2();
      _DAT_ram_1e54 = (uint)uVar3;
      uVar4 = _DAT_ram_1e3c;
      if (_DAT_ram_1e3c < _DAT_ram_1e54) {
        uVar4 = _DAT_ram_1e54;
      }
      if ((_DAT_ram_1e18 & 1) != 0) {
        uVar3 = (ulong)_DAT_ram_1bb2 * 0x7d1;
        uVar1 = (int)(uVar3 >> 0x10) << 8 | (uint)uVar3 >> 8;
        uVar3 = uVar3 >> 8;
        if (uVar4 < uVar1) {
          uVar4 = uVar1;
        }
      }
      Peripherals::OC2.OC2RS = uVar4;
      return uVar3;
    }
    uVar2 = Peripherals::OC2.OC2RS;
    Peripherals::OC2.OC2RS = uVar2 + 0x14;
    uVar2 = Peripherals::OC2.OC2RS;
    in_W1W0 = CONCAT22(uVar2,399);
    if (399 < uVar2) {
      in_W1W0 = CONCAT22(uVar2,400);
      Peripherals::OC2.OC2RS = 400;
    }
  }
  return in_W1W0;
}





// Function: FUN_rom_00556a

undefined2 FUN_rom_00556a(void)

{
  ushort uVar1;
  
  Peripherals::UART1.U1MODE = 6;
  Peripherals::UART1.U1BRG = 0x28a;
  Peripherals::UART1.U1STA = 0;
  uVar1 = Peripherals::INTERRUPT.IFS0;
  Peripherals::INTERRUPT.IFS0 = uVar1 & 0xefff;
  uVar1 = Peripherals::INTERRUPT.IEC0;
  Peripherals::INTERRUPT.IEC0 = uVar1 & 0xefff;
  uVar1 = Peripherals::INTERRUPT.IFS0;
  Peripherals::INTERRUPT.IFS0 = uVar1 & 0xf7ff;
  uVar1 = Peripherals::INTERRUPT.IEC0;
  Peripherals::INTERRUPT.IEC0 = uVar1 & 0xf7ff;
  uVar1 = Peripherals::UART1.U1MODE;
  Peripherals::UART1.U1MODE = uVar1 | 0x8000;
  uVar1 = Peripherals::UART1.U1STA;
  Peripherals::UART1.U1STA = uVar1 | 0x400;
  return 0x28a;
}





// Function: FUN_rom_005582

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined2 FUN_rom_005582(void)

{
  byte bVar1;
  ushort uVar2;
  undefined2 uVar3;
  uint uVar4;
  bool bVar5;
  
  bVar1 = Peripherals::UART1.U1STA._0_1_;
  if (((bVar1 & 1) == 0) || (uVar4 = _DAT_ram_1bf4 & 0x100, uVar4 != 0)) goto LAB_rom_0056f0;
  uVar2 = Peripherals::UART1.U1RXREG;
  _DAT_ram_1cf8 = uVar4;
  if (_DAT_ram_1ce2 == 2) {
    *(ushort *)(&DAT_ram_1cc8 + _DAT_ram_1ce0 * 2) = uVar2;
    uVar4 = _DAT_ram_1ce0 + 1;
    bVar5 = _DAT_ram_1ce0 != 4;
    _DAT_ram_1ce0 = uVar4;
    if (bVar5) goto LAB_rom_0056f0;
    if ((_DAT_ram_1cc8 + _DAT_ram_1cca + _DAT_ram_1ccc + _DAT_ram_1cce & 0xff) == uVar2) {
      _DAT_ram_1d12 = _DAT_ram_1cc8 + _DAT_ram_1cca * 0x100;
      _DAT_ram_1d10 = _DAT_ram_1ccc + _DAT_ram_1cce * 0x100;
    }
LAB_rom_00561a:
    _DAT_ram_1ce2 = 0;
    goto LAB_rom_0056f0;
  }
  if (_DAT_ram_1ce2 < 2) {
    if (_DAT_ram_1ce2 == 0) {
      if (uVar2 == 0x49) {
        uVar4 = 1;
      }
      else {
        if (uVar2 != 0x56) goto LAB_rom_0056f0;
        uVar4 = 8;
      }
      _DAT_ram_1ce0 = _DAT_ram_1ce2;
      _DAT_ram_1ce2 = uVar4;
      goto LAB_rom_0056f0;
    }
    if ((_DAT_ram_1ce2 == 1) && (uVar2 == 0x34)) {
      _DAT_ram_1ce2 = 2;
      goto LAB_rom_0056f0;
    }
  }
  else {
    if (_DAT_ram_1ce2 == 8) {
      if (uVar2 == 0x34) {
        _DAT_ram_1e1c = _DAT_ram_1e1c | 0x40;
      }
      else {
        if (uVar2 != 0x33) goto LAB_rom_00561a;
        _DAT_ram_1e1c = _DAT_ram_1e1c & 0xffbf;
      }
      _DAT_ram_1ce2 = 9;
      goto LAB_rom_0056f0;
    }
    if (_DAT_ram_1ce2 != 9) goto LAB_rom_0056ee;
    *(ushort *)(&DAT_ram_1cc8 + _DAT_ram_1ce0 * 2) = uVar2;
    uVar4 = _DAT_ram_1ce0 + 1;
    bVar5 = _DAT_ram_1ce0 != 0xc;
    _DAT_ram_1ce0 = uVar4;
    if (bVar5) goto LAB_rom_0056f0;
    DAT_ram_1d03 = (char)_DAT_ram_1cd4;
    DAT_ram_1d04 = (char)_DAT_ram_1cd6;
    DAT_ram_1d05 = (char)_DAT_ram_1cd8;
    DAT_ram_1d06 = (char)_DAT_ram_1cda;
    DAT_ram_1d07 = (char)_DAT_ram_1cdc;
    if ((byte)(DAT_ram_1cde +
              (char)_DAT_ram_1cca + (char)_DAT_ram_1cc8 + (char)_DAT_ram_1ccc + (char)_DAT_ram_1cce
              + (char)_DAT_ram_1cd0 + (char)_DAT_ram_1cd2 + DAT_ram_1d03 + DAT_ram_1d04 +
              DAT_ram_1d05 + DAT_ram_1d06 + DAT_ram_1d07) == uVar2) {
      if ((_DAT_ram_1cca & 0x80) == 0) {
        _DAT_ram_1bf2 = _DAT_ram_1bf2 & 0xfbff;
      }
      else {
        _DAT_ram_1bf2 = _DAT_ram_1bf2 | 0x400;
      }
      _DAT_ram_1cca = _DAT_ram_1cca & 0x7f;
      _DAT_ram_1d14 = _DAT_ram_1cca * 0x100 + _DAT_ram_1cc8;
      _DAT_ram_1e46 = _DAT_ram_1cce;
      _DAT_ram_1d16 = _DAT_ram_1cd0;
      if (_DAT_ram_1cd0 < _DAT_ram_1e12) {
        _DAT_ram_1d18 = _DAT_ram_1e12;
      }
      else {
        _DAT_ram_1d18 = _DAT_ram_1cd0;
      }
      if (_DAT_ram_1cd2 == 0) {
        _DAT_ram_1e1a = _DAT_ram_1e1a & 0xfffd;
      }
      else {
        _DAT_ram_1e1a = _DAT_ram_1e1a | 2;
      }
    }
    if (_DAT_ram_1cde == 0xaa) {
      _DAT_ram_1e20 = _DAT_ram_1e20 & 0xfeff;
      _DAT_ram_1bf2 = _DAT_ram_1bf2 | 4;
LAB_rom_0056c2:
      _DAT_ram_1264 = _DAT_ram_1264 & 0xbfff;
    }
    else {
      if (_DAT_ram_1cde == 0x55) {
        _DAT_ram_1e20 = _DAT_ram_1e20 & 0xfeff;
LAB_rom_0056c0:
        _DAT_ram_1bf2 = _DAT_ram_1bf2 & 0xfffb;
        goto LAB_rom_0056c2;
      }
      if (_DAT_ram_1cde != 0x5a) {
        if (_DAT_ram_1cde != 0xa5) {
          if (_DAT_ram_1cde == 0xa0) {
            _DAT_ram_1e20 = _DAT_ram_1e20 & 0xfeff;
            _DAT_ram_1bf2 = _DAT_ram_1bf2 & 0xfffb;
            _DAT_ram_1264 = _DAT_ram_1264 & 0xbfff;
            _DAT_ram_1e20 = _DAT_ram_1e20 | 0x80;
          }
          goto LAB_rom_0056ee;
        }
        _DAT_ram_1e20 = _DAT_ram_1e20 | 0x100;
        goto LAB_rom_0056c0;
      }
      _DAT_ram_1e20 = _DAT_ram_1e20 & 0xfeff;
      _DAT_ram_1bf2 = _DAT_ram_1bf2 & 0xfffb;
      _DAT_ram_1264 = _DAT_ram_1264 | 0x4000;
    }
    _DAT_ram_1e20 = _DAT_ram_1e20 & 0xff7f;
  }
LAB_rom_0056ee:
  _DAT_ram_1ce2 = 0;
LAB_rom_0056f0:
  bVar1 = Peripherals::UART1.U1STA._0_1_;
  if (((bVar1 & 1) != 0) && ((_DAT_ram_1bf4 & 0x100) != 0)) {
    uVar2 = Peripherals::UART1.U1RXREG;
    _DAT_ram_1cf8 = 0;
  }
  bVar1 = Peripherals::UART1.U1STA._0_1_;
  if (((((bVar1 & 8) != 0) || (bVar1 = Peripherals::UART1.U1STA._0_1_, (bVar1 & 4) != 0)) ||
      (bVar1 = Peripherals::UART1.U1STA._0_1_, (bVar1 & 2) != 0)) ||
     (uVar3 = 30000, 30000 < _DAT_ram_1cf8)) {
    uVar3 = FUN_rom_00556a();
    _DAT_ram_1cf8 = 0;
  }
  return uVar3;
}





// Function: FUN_rom_00571e

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_00571e(void)

{
  byte bVar1;
  int iVar2;
  ushort uVar3;
  uint uVar4;
  undefined1 uVar5;
  undefined1 uVar6;
  uint uVar7;
  int iVar8;
  byte *pbVar9;
  byte *pbVar10;
  bool bVar11;
  
  uVar4 = _DAT_ram_1cec;
  if (_DAT_ram_1cec == 0) {
    if (((_DAT_ram_1bf4 & 0x100) == 0) || ((_DAT_ram_1e18 & 2) == 0)) {
      if (_DAT_ram_1ce4 == 0) {
        pbVar10 = &DAT_ram_1cee;
        iVar8 = 0;
        do {
          if (iVar8 == 0) {
            DAT_ram_1cee = 'X';
            _DAT_ram_1ce6 = 0;
          }
          else {
            uVar5 = DAT_ram_1cf1;
            uVar6 = DAT_ram_1cf3;
            if (iVar8 == 1) {
              if (((_DAT_ram_1264 & 0x1000) == 0) && (_DAT_ram_1e22 != 5)) {
                DAT_ram_1cef = 0;
              }
              else {
                DAT_ram_1cef = 2;
              }
            }
            else if (iVar8 == 2) {
              DAT_ram_1cf0 = (undefined1)((uint)_DAT_ram_1be0 >> 8);
            }
            else {
              uVar5 = (char)_DAT_ram_1be0;
              if (iVar8 != 3) {
                if (iVar8 == 4) {
                  DAT_ram_1cf2 = (undefined1)((uint)_DAT_ram_1d0e >> 8);
                  uVar5 = DAT_ram_1cf1;
                }
                else {
                  uVar5 = DAT_ram_1cf1;
                  uVar6 = (char)_DAT_ram_1d0e;
                  if (iVar8 != 5) {
                    if (iVar8 == 6) {
                      DAT_ram_1cf4 = (undefined1)((uint)_DAT_ram_1e42 >> 8);
                      uVar6 = DAT_ram_1cf3;
                    }
                    else {
                      uVar6 = DAT_ram_1cf3;
                      if (iVar8 == 7) {
                        DAT_ram_1cf5 = (undefined1)_DAT_ram_1e42;
                      }
                    }
                  }
                }
              }
            }
            DAT_ram_1cf3 = uVar6;
            DAT_ram_1cf1 = uVar5;
            _DAT_ram_1ce6 = _DAT_ram_1ce6 + (uint)*pbVar10;
          }
          iVar2 = iVar8 + 1;
          pbVar10 = pbVar10 + 1;
          bVar11 = iVar8 == 6;
          iVar8 = iVar2;
        } while (bVar11 || iVar2 < 7);
        _DAT_ram_1ce8 = 8;
        DAT_ram_1cf6 = (undefined1)_DAT_ram_1ce6;
        _DAT_ram_1ce4 = 1;
      }
    }
    else {
      pbVar10 = (byte *)0x1cf9;
      pbVar9 = &DAT_ram_1cee;
      iVar8 = 0;
      do {
        if (iVar8 == 0) {
          DAT_ram_1cee = '`';
          _DAT_ram_1ce6 = 0;
        }
        else {
          bVar1 = *pbVar10;
          *pbVar9 = bVar1;
          _DAT_ram_1ce6 = _DAT_ram_1ce6 + (uint)bVar1;
        }
        iVar2 = iVar8 + 1;
        pbVar10 = pbVar10 + 1;
        pbVar9 = pbVar9 + 1;
        bVar11 = iVar8 == 6;
        iVar8 = iVar2;
      } while (bVar11 || iVar2 < 7);
      _DAT_ram_1ce8 = 8;
      DAT_ram_1cf6 = (undefined1)_DAT_ram_1ce6;
    }
  }
  bVar1 = Peripherals::UART1.U1STA._1_1_;
  if (((bVar1 & 1) != 0) &&
     (uVar7 = _DAT_ram_1cea + 1, bVar11 = _DAT_ram_1cea != 8, _DAT_ram_1cea = uVar7,
     8 < uVar7 && bVar11)) {
    _DAT_ram_1cea = 0;
    Peripherals::UART1.U1TXREG = (uint)(byte)(&DAT_ram_1cee)[uVar4];
    _DAT_ram_1cec = uVar4 + 1;
    if (7 < _DAT_ram_1cec && uVar4 != 7) {
      if ((DAT_ram_1cee == '`') && ((_DAT_ram_1e18 & 2) != 0)) {
        _DAT_ram_1e18 = _DAT_ram_1e18 & 0xfffd;
        uVar3 = Peripherals::I2C2.I2C2CON;
        Peripherals::I2C2.I2C2CON = uVar3 | 0x1000;
      }
      _DAT_ram_1cec = 0;
      _DAT_ram_1ce4 = 0;
    }
  }
  return;
}





// Function: thunk_FUN_rom_005582

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void thunk_FUN_rom_005582(void)

{
  _DAT_ram_1cf8 = _DAT_ram_1cf8 + 1;
  FUN_rom_005582();
  return;
}





// Function: FUN_rom_0059a2

undefined4 FUN_rom_0059a2(void)

{
  ushort uVar1;
  
  uVar1 = Peripherals::GPIOB.TRISB;
  Peripherals::GPIOB.TRISB = uVar1 | 0x80;
  Peripherals::GPIOC.TRISC = 0xf000;
  Peripherals::GPIOC.LATC = 0;
  Peripherals::GPIOD.TRISD = 0xfc3;
  Peripherals::GPIOD.LATD = 8;
  Peripherals::GPIOD.ODCD = 0x801;
  Peripherals::GPIOE.TRISE = 0;
  Peripherals::GPIOE.LATE = 8;
  Peripherals::GPIOF.TRISF = 0x3c;
  Peripherals::GPIOF.LATF = 0x40;
  Peripherals::GPIOG.TRISG = 0x1cc;
  Peripherals::GPIOG.LATG = 0x200;
  return 0x80200;
}





// Function: FUN_rom_0059ce

void FUN_rom_0059ce(void)

{
  byte bVar1;
  ushort uVar2;
  
  Peripherals::CLOCK.PLLFBD = 0x30;
  bVar1 = Peripherals::CLOCK.CLKDIV._0_1_;
  Peripherals::CLOCK.CLKDIV._0_1_ = bVar1 & 0xe0 | 2;
  bVar1 = Peripherals::CLOCK.CLKDIV._0_1_;
  Peripherals::CLOCK.CLKDIV._0_1_ = bVar1 & 0x3f;
  Peripherals::CLOCK.OSCCON._1_1_ = 0x78;
  Peripherals::CLOCK.OSCCON._1_1_ = 0x9a;
  Peripherals::CLOCK.OSCCON._1_1_ = 3;
  Peripherals::CLOCK.OSCCON._0_1_ = 0x46;
  Peripherals::CLOCK.OSCCON._0_1_ = 0x57;
  Peripherals::CLOCK.OSCCON._0_1_ = 1;
  do {
    bVar1 = Peripherals::CLOCK.OSCCON._1_1_;
  } while ((bVar1 & 0x70) != 0x30);
  do {
    bVar1 = Peripherals::CLOCK.OSCCON._0_1_;
  } while ((bVar1 & 0x20) == 0);
  Peripherals::CLOCK.ACLKCON = 0x2740;
  uVar2 = Peripherals::CLOCK.ACLKCON;
  Peripherals::CLOCK.ACLKCON = uVar2 | 0x8000;
  do {
    bVar1 = Peripherals::CLOCK.ACLKCON._1_1_;
  } while ((bVar1 & 0x40) == 0);
  return;
}





// Function: FUN_rom_005a1c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_005a1c(void)

{
  undefined1 uVar1;
  byte bVar2;
  
  bVar2 = SPLIM.SR._0_1_;
  SPLIM.SR._0_1_ = bVar2 | 0xe0;
  uVar1 = SPLIM.SR._0_1_;
  uVar1 = SPLIM.SR._0_1_;
  _DAT_ram_126a = 0;
  _DAT_ram_125a = 0;
  FUN_rom_0059ce(0xe0);
  FUN_rom_005bd8();
  FUN_rom_005bcc();
  FUN_rom_0059a2();
  FUN_rom_005bae();
  FUN_rom_005af6();
  FUN_rom_005288();
  FUN_rom_00150c();
  FUN_rom_00556a();
  FUN_rom_0038a6();
  FUN_rom_00508e();
  bVar2 = SPLIM.SR._0_1_;
  bVar2 = bVar2 & 0x1f;
  SPLIM.SR._0_1_ = bVar2;
  do {
    FUN_rom_0050c2(bVar2);
    FUN_rom_005522();
    FUN_rom_0051fe();
    FUN_rom_0041a2();
    FUN_rom_0041be();
    thunk_FUN_rom_0041e0();
    FUN_rom_004260();
    FUN_rom_00427e();
    bVar2 = FUN_rom_00429c();
  } while( true );
}





// Function: FUN_rom_005a88

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_005a88(uint param_1,uint param_2,uint param_3)

{
  uint uVar1;
  
  uVar1 = _DAT_ram_27fa & 0xf1;
  if ((char)param_1 != -1) {
    uVar1 = _DAT_ram_27fa & 0xc1 | (param_1 & 3) << 4;
  }
  if ((char)param_2 != -1) {
    uVar1 = uVar1 & 0xff3f | (param_2 & 3) << 6;
  }
  if ((char)param_3 != -1) {
    uVar1 = uVar1 & 0xfffe | param_3 & 1;
  }
  _DAT_ram_27fa = uVar1;
  return;
}





// Function: FUN_rom_005ab8

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined2 FUN_rom_005ab8(void)

{
  return _DAT_ram_27fa;
}





// Function: FUN_rom_005ac0

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined2 FUN_rom_005ac0(void)

{
  return _DAT_ram_27f8;
}





// Function: FUN_rom_005ac8

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined2 FUN_rom_005ac8(void)

{
  return _DAT_ram_27f6;
}





// Function: FUN_rom_005acc

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_005acc(undefined2 param_1)

{
  _DAT_ram_27f4 = param_1;
  return;
}





// Function: FUN_rom_005ad0

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined2 FUN_rom_005ad0(void)

{
  return _DAT_ram_27f4;
}





// Function: FUN_rom_005ad4

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_005ad4(undefined2 param_1)

{
  _DAT_ram_27f2 = param_1;
  return;
}





// Function: FUN_rom_005ad8

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined2 FUN_rom_005ad8(void)

{
  return _DAT_ram_27f2;
}





// Function: FUN_rom_005ae0

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined2 FUN_rom_005ae0(void)

{
  return _DAT_ram_27f0;
}





// Function: FUN_rom_005ae4

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_005ae4(void)

{
  code *UNRECOVERED_JUMPTABLE;
  
  _DAT_ram_27fc = _DAT_ram_27fe;
  _DAT_ram_27fe = 1;
  FUN_rom_005a88(1,0xff,0);
  PC = 0;
  UNRECOVERED_JUMPTABLE = (code *)PC;
                    /* WARNING: Could not recover jumptable at 0x005af2. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*UNRECOVERED_JUMPTABLE)();
  return;
}





// Function: FUN_rom_005af6

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_005af6(void)

{
  ushort uVar1;
  byte bVar2;
  uint uVar3;
  
  Peripherals::TIMER1.T1CON = 0;
  Peripherals::TIMER1.TMR1 = 0;
  Peripherals::TIMER1.PR1 = 10000;
  bVar2 = Peripherals::INTERRUPT.IPC0._1_1_;
  Peripherals::INTERRUPT.IPC0._1_1_ = bVar2 & 0x8f | 0x20;
  uVar1 = Peripherals::INTERRUPT.IFS0;
  Peripherals::INTERRUPT.IFS0 = uVar1 & 0xfff7;
  uVar1 = Peripherals::INTERRUPT.IEC0;
  Peripherals::INTERRUPT.IEC0 = uVar1 | 8;
  uVar1 = Peripherals::TIMER1.T1CON;
  Peripherals::TIMER1.T1CON = uVar1 | 0x8000;
  Peripherals::TIMER3._8_2_ = 0;
  Ram0106 = 0;
  Peripherals::TIMER3._4_2_ = 1000;
  bVar2 = Peripherals::INTERRUPT.IPC1._1_1_;
  Peripherals::INTERRUPT.IPC1._1_1_ = bVar2 & 0x8f | 0x40;
  uVar1 = Peripherals::INTERRUPT.IFS0;
  Peripherals::INTERRUPT.IFS0 = uVar1 & 0xff7f;
  uVar1 = Peripherals::INTERRUPT.IEC0;
  Peripherals::INTERRUPT.IEC0 = uVar1 | 0x80;
  uVar3 = Peripherals::TIMER3._8_2_;
  Peripherals::TIMER3._8_2_ = uVar3 | 0x8000;
  Peripherals::TIMER5._8_2_ = 0;
  Ram0114 = 0;
  Peripherals::TIMER5._4_2_ = 50000;
  uVar1 = Peripherals::INTERRUPT.IFS1;
  Peripherals::INTERRUPT.IFS1 = uVar1 & 0xf7ff;
  uVar1 = Peripherals::INTERRUPT.IEC1;
  Peripherals::INTERRUPT.IEC1 = uVar1 & 0xf7ff;
  uVar3 = Peripherals::TIMER5._8_2_;
  Peripherals::TIMER5._8_2_ = uVar3 | 0x8000;
  _DAT_ram_1264 = 0;
  return 50000;
}





// Function: FUN_rom_005b74

uint FUN_rom_005b74(byte param_1,uint *param_2)

{
  uint uVar1;
  
  uVar1 = *(uint *)((uint)(byte)(param_1 ^ (byte)*param_2) * 2 + -0x6eb6);
  *param_2 = *param_2 >> 8 ^ uVar1;
  return uVar1;
}





// Function: FUN_rom_005b86

void FUN_rom_005b86(int param_1,undefined2 param_2)

{
  undefined4 in_W3W2;
  uint uVar1;
  uint uVar2;
  uint uVar3;
  bool bVar4;
  
  uVar1 = 0;
  uVar2 = 0;
  while( true ) {
    uVar3 = (uint)((ulong)in_W3W2 >> 0x10);
    if (uVar3 <= uVar2 && uVar3 + (uVar1 < (uint)in_W3W2) <= uVar2) break;
    FUN_rom_005b74(*(undefined1 *)(param_1 + uVar1),param_2);
    bVar4 = 0xfffe < uVar1;
    uVar1 = uVar1 + 1;
    uVar2 = uVar2 + bVar4;
  }
  return;
}





// Function: FUN_rom_005bae

undefined4 FUN_rom_005bae(void)

{
  ushort uVar1;
  
  Peripherals::ADC.ADCON = 0x1007;
  Peripherals::ADC.ADSTAT = 0;
  Peripherals::ADC.ADBASE = 0;
  Peripherals::ADC.ADCPC0 = 0x1f1f;
  Peripherals::ADC.ADCPC1 = 0x1f;
  Peripherals::ADC.ADCPC2 = 0x1f1f;
  Peripherals::ADC.ADCPC3 = 0x1f1f;
  Peripherals::ADC.ADPCFG = 0x80;
  uVar1 = Peripherals::ADC.ADCON;
  Peripherals::ADC.ADCON = uVar1 | 0x8000;
  return 0x1f1f0080;
}





// Function: FUN_rom_005bcc

undefined2 FUN_rom_005bcc(void)

{
  ushort uVar1;
  
  Peripherals::CMP3.CMPCON3 = 0x81;
  Peripherals::CMP3.CMPDAC3 = 0x1a3;
  uVar1 = Peripherals::CMP3.CMPCON3;
  Peripherals::CMP3.CMPCON3 = uVar1 | 0x8000;
  return 0x1a3;
}





// Function: FUN_rom_005bd8

undefined2 FUN_rom_005bd8(void)

{
  ushort uVar1;
  
  Peripherals::CMP4.CMPCON4 = 0x101;
  Peripherals::CMP4.CMPDAC4 = 0;
  uVar1 = Peripherals::CMP4.CMPCON4;
  Peripherals::CMP4.CMPCON4 = uVar1 | 0x8000;
  return 0x101;
}





// Function: thunk_FUN_rom_005c70

/* WARNING: This function may have set the stack pointer */
/* WARNING: Removing unreachable block (rom,0x005c86) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void thunk_FUN_rom_005c70(void)

{
  code *UNRECOVERED_JUMPTABLE;
  
  SPLIM = 0x27e0;
  _DAT_ram_1e58 = 0x5c7a;
  FUN_rom_005c92();
  _DAT_ram_1e58 = 0x5c80;
  FUN_rom_005ca2(0xa442,0);
  _DAT_ram_1e58 = 0x5c8e;
  FUN_rom_00a61c();
  break();
  PC = 0;
  UNRECOVERED_JUMPTABLE = (code *)PC;
                    /* WARNING: Could not recover jumptable at 0x005c90. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*UNRECOVERED_JUMPTABLE)();
  return;
}





// Function: FUN_rom_005c70

/* WARNING: This function may have set the stack pointer */
/* WARNING: Removing unreachable block (rom,0x005c86) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_005c70(void)

{
  code *UNRECOVERED_JUMPTABLE;
  
  SPLIM = 0x27e0;
  _DAT_ram_1e58 = 0x5c7a;
  FUN_rom_005c92();
  _DAT_ram_1e58 = 0x5c80;
  FUN_rom_005ca2(0xa442,0);
  _DAT_ram_1e58 = 0x5c8e;
  FUN_rom_00a61c();
  break();
  PC = 0;
  UNRECOVERED_JUMPTABLE = (code *)PC;
                    /* WARNING: Could not recover jumptable at 0x005c90. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*UNRECOVERED_JUMPTABLE)();
  return;
}





// Function: FUN_rom_005c92

undefined2 FUN_rom_005c92(void)

{
  uint uVar1;
  
  uVar1 = CORCON;
  CORCON = uVar1 & 0xfffb;
  Ram0034 = 0;
  uVar1 = CORCON;
  CORCON = uVar1 | 4;
  return 0;
}





// Function: FUN_rom_005ca2

undefined2 FUN_rom_005ca2(uint param_1,undefined2 param_2)

{
  uint uVar1;
  byte bVar2;
  int iVar3;
  int iVar4;
  undefined4 uVar5;
  undefined1 *puVar6;
  int iVar7;
  bool bVar8;
  
  Ram0032 = param_2;
  iVar4 = 0;
  while( true ) {
    bVar2 = TBLPAG;
    puVar6 = *(undefined1 **)((uint3)bVar2 << 0x10 | (uint3)param_1 & 0xfffffe);
    if (puVar6 == (undefined1 *)0x0) break;
    iVar7 = Ram0032;
    Ram0032 = iVar7 + iVar4 + (uint)(0xfffd < param_1);
    bVar2 = TBLPAG;
    iVar7 = *(int *)((uint3)bVar2 << 0x10 | (uint3)(param_1 + 2) & 0xfffffe);
    uVar1 = param_1 + 4;
    iVar3 = Ram0032;
    Ram0032 = iVar3 + iVar4 + (uint)(0xfffd < param_1 + 2);
    bVar2 = TBLPAG;
    param_1 = param_1 + 6;
    iVar3 = Ram0032;
    Ram0032 = iVar3 + iVar4 + (uint)(0xfffd < uVar1);
    if ((*(uint *)((uint3)bVar2 << 0x10 | (uint3)uVar1 & 0xfffffe) & 0x7f) == 0) {
      do {
        *puVar6 = 0;
        bVar8 = iVar7 != 0;
        iVar7 = iVar7 + -1;
        puVar6 = puVar6 + 1;
      } while (bVar8 && iVar7 != 0);
    }
    else {
      uVar5 = FUN_rom_005cdc(iVar4);
      param_1 = (uint)((ulong)uVar5 >> 0x10);
      iVar4 = (int)uVar5;
    }
  }
  return 0;
}





// Function: FUN_rom_005cdc

void FUN_rom_005cdc(int param_1,uint param_2,undefined1 *param_3,int param_4,int param_5)

{
  undefined1 uVar1;
  uint uVar2;
  undefined1 uVar3;
  int iVar4;
  uint uVar5;
  undefined1 *puVar6;
  int iVar7;
  
  while( true ) {
    uVar1 = TBLPAG;
    uVar5 = param_2 + 2;
    uVar2 = *(uint *)(CONCAT12(uVar1,param_2) & 0xfffffe);
    if ((param_2 & 1) != 0) {
      uVar2 = uVar2 >> 8;
    }
    *param_3 = (char)uVar2;
    if (param_4 == 1) goto LAB_rom_005cfa;
    uVar1 = TBLPAG;
    uVar2 = *(uint *)(CONCAT12(uVar1,uVar5) & 0xfffffe);
    if ((uVar5 & 1) != 0) {
      uVar2 = uVar2 >> 8;
    }
    param_3[1] = (char)uVar2;
    iVar7 = param_4 + -2;
    if (iVar7 == 0) break;
    puVar6 = param_3 + 2;
    if (param_5 != 0) {
      uVar3 = TBLPAG;
      uVar1 = 0;
      if ((param_2 & 1) == 0) {
        uVar1 = *(undefined1 *)(CONCAT12(uVar3,param_2) | 1);
      }
      puVar6 = param_3 + 3;
      param_3[2] = uVar1;
      iVar7 = param_4 + -3;
      if (iVar7 == 0) break;
    }
    iVar4 = Ram0032;
    Ram0032 = iVar4 + param_1 + (uint)(0xfffd < param_2);
    param_2 = uVar5;
    param_3 = puVar6;
    param_4 = iVar7;
  }
  uVar5 = param_2 + 1;
LAB_rom_005cfa:
  iVar7 = Ram0032;
  Ram0032 = iVar7 + param_1 + (uint)(0xfffe < uVar5);
  return;
}





// Function: FUN_rom_005d00

undefined4 FUN_rom_005d00(uint param_1,int param_2)

{
  ulong in_W1W0;
  long lVar1;
  
  lVar1 = (in_W1W0 & 0xffff) * (ulong)param_1;
  return CONCAT22(param_2 * (int)in_W1W0 +
                  (int)(in_W1W0 >> 0x10) * param_1 + (int)((ulong)lVar1 >> 0x10),(int)lVar1);
}





// Function: FUN_rom_005d0c

undefined4 FUN_rom_005d0c(int param_1,uint param_2,int param_3,uint param_4)

{
  uint uVar1;
  undefined4 uVar2;
  bool bVar3;
  
  uVar1 = param_2 ^ param_4;
  if ((int)param_2 < 0) {
    bVar3 = param_1 != 0;
    param_1 = -param_1;
    param_2 = -(uint)bVar3 - param_2;
  }
  if ((int)param_4 < 0) {
    bVar3 = param_3 != 0;
    param_3 = -param_3;
    param_4 = -(uint)bVar3 - param_4;
  }
  uVar2 = FUN_rom_005d2a(param_1,param_2,param_3,param_4);
  if ((int)uVar1 < 0) {
    uVar2 = CONCAT22(-(uint)((int)uVar2 != 0) - (int)((ulong)uVar2 >> 0x10),-(int)uVar2);
  }
  return uVar2;
}





// Function: FUN_rom_005d2a

void FUN_rom_005d2a(uint param_1,uint param_2,uint param_3,uint param_4)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  int iVar6;
  ulong uVar7;
  int iVar8;
  
  uVar7 = 0;
  iVar8 = 0x20;
  do {
    uVar3 = param_1 << 1;
    uVar4 = param_1 & 0x8000;
    uVar1 = (int)uVar7 << 1 | (uint)((param_2 & 0x8000) != 0);
    uVar2 = (int)(uVar7 >> 0x10) << 1 | (uint)((uVar7 & 0x8000) != 0);
    uVar5 = uVar1 - param_3;
    uVar1 = (uint)(uVar1 < param_3);
    iVar6 = (uVar2 - param_4) - uVar1;
    uVar7 = CONCAT22(iVar6,uVar5);
    param_1 = uVar3 | 1;
    if (uVar2 < param_4 || uVar2 < param_4 + uVar1) {
      uVar7 = CONCAT22(iVar6 + param_4 + (uint)CARRY2(uVar5,param_3),uVar5 + param_3);
      param_1 = uVar3;
    }
    iVar8 = iVar8 + -1;
    param_2 = param_2 << 1 | (uint)(uVar4 != 0);
  } while (iVar8 != 0);
  return;
}





// Function: FUN_rom_005fc2

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_005fc2(void)

{
  undefined2 *puVar1;
  bool bVar2;
  
  _DAT_ram_19b0 = &DAT_ram_1bf4;
  _DAT_ram_19b2 = &DAT_ram_1bf2;
  _DAT_ram_19b4 = &DAT_ram_1bee;
  _DAT_ram_19b6 = &DAT_ram_1bec;
  _DAT_ram_19ba = &DAT_ram_1946;
  _DAT_ram_19bc = &DAT_ram_1944;
  _DAT_ram_19be = &DAT_ram_1942;
  _DAT_ram_19d0 = &DAT_ram_1be8;
  _DAT_ram_19d2 = &DAT_ram_1be6;
  _DAT_ram_19d4 = &DAT_ram_1be4;
  _DAT_ram_19d6 = &DAT_ram_19a4;
  _DAT_ram_19d8 = &DAT_ram_194c;
  _DAT_ram_19da = &DAT_ram_194a;
  _DAT_ram_19dc = &DAT_ram_1bd2;
  _DAT_ram_19de = &DAT_ram_1bc8;
  _DAT_ram_19f0 = &DAT_ram_1be2;
  _DAT_ram_19f2 = &DAT_ram_1bde;
  _DAT_ram_19f4 = &DAT_ram_1bc0;
  _DAT_ram_19f6 = &DAT_ram_1bd0;
  _DAT_ram_19f8 = &DAT_ram_1bce;
  _DAT_ram_19fa = &DAT_ram_197a;
  _DAT_ram_1a10 = &DAT_ram_1bea;
  _DAT_ram_1a12 = &DAT_ram_1bb2;
  _DAT_ram_1a14 = &DAT_ram_1bc6;
  _DAT_ram_1a16 = &DAT_ram_1bb6;
  _DAT_ram_1a18 = &DAT_ram_1bb4;
  _DAT_ram_1a1a = &DAT_ram_19a2;
  _DAT_ram_1a1c = &DAT_ram_19a0;
  _DAT_ram_1a1e = &DAT_ram_199e;
  _DAT_ram_1a20 = &DAT_ram_199c;
  _DAT_ram_1a22 = &DAT_ram_199a;
  _DAT_ram_1a24 = &DAT_ram_1998;
  _DAT_ram_1a26 = &DAT_ram_1996;
  _DAT_ram_1a28 = &DAT_ram_1994;
  _DAT_ram_1a2a = &DAT_ram_1bca;
  _DAT_ram_1a30 = &DAT_ram_1bd4;
  _DAT_ram_1a32 = &DAT_ram_19a6;
  _DAT_ram_1a34 = &DAT_ram_1bdc;
  _DAT_ram_1a36 = &DAT_ram_19ae;
  _DAT_ram_1a38 = &DAT_ram_1bda;
  _DAT_ram_1a3a = &DAT_ram_19ac;
  _DAT_ram_1a3c = &DAT_ram_1bd8;
  _DAT_ram_1a3e = &DAT_ram_19aa;
  _DAT_ram_1a40 = &DAT_ram_1bd6;
  _DAT_ram_1a42 = &DAT_ram_19a8;
  _DAT_ram_1a46 = &DAT_ram_1992;
  _DAT_ram_1a48 = &DAT_ram_1990;
  _DAT_ram_1a70 = &DAT_ram_1bbc;
  _DAT_ram_1a72 = &DAT_ram_1bba;
  _DAT_ram_1a74 = &DAT_ram_1bb8;
  _DAT_ram_1a76 = &DAT_ram_198e;
  _DAT_ram_1a78 = &DAT_ram_198c;
  _DAT_ram_1b90 = &DAT_ram_198a;
  _DAT_ram_1b92 = &DAT_ram_1988;
  _DAT_ram_1b94 = &DAT_ram_1986;
  _DAT_ram_1b96 = &DAT_ram_1984;
  _DAT_ram_1b98 = &DAT_ram_1982;
  _DAT_ram_1b9c = &DAT_ram_197e;
  puVar1 = (undefined2 *)&DAT_ram_19b0;
  do {
    *(undefined2 *)*puVar1 = 0;
    bVar2 = puVar1 != (undefined2 *)0x1bae;
    puVar1 = puVar1 + 1;
  } while (bVar2);
  _DAT_ram_1bf4 = 1;
  _DAT_ram_197c = 0x5dc;
  _DAT_ram_19a2 = 0x15c0;
  _DAT_ram_19a0 = 0x21c0;
  _DAT_ram_199e = 0xb00;
  _DAT_ram_199c = 0xd00;
  _DAT_ram_1998 = 0xdc0;
  _DAT_ram_1996 = 0x15c0;
  _DAT_ram_1994 = 0x15c0;
  _DAT_ram_1bca = &Peripherals::TIMER3;
  _DAT_ram_1bc6 = 0x3133;
  _DAT_ram_1bb6 = 0x640;
  _DAT_ram_1bb4 = 0x1180;
  _DAT_ram_1992 = 0x135;
  _DAT_ram_1990 = 0x13b;
  _DAT_ram_198a = 0xef;
  _DAT_ram_1988 = 0xff;
  _DAT_ram_1986 = 0x1f;
  _DAT_ram_1984 = 0x3fff;
  _DAT_ram_1982 = 0x3fff;
  _DAT_ram_197e = 0x1f;
  _DAT_ram_199a = 0x10ab;
  _DAT_ram_194e = 0;
  _DAT_ram_1948 = 0;
  return 0x1f10ab;
}





// Function: FUN_rom_00610c

undefined3 FUN_rom_00610c(void)

{
  ushort uVar1;
  byte bVar2;
  uint uVar3;
  
  FUN_rom_005fc2();
  Peripherals::I2C2.I2C2CON = 0x9040;
  Peripherals::I2C2.I2C2ADD = 0x58;
  bVar2 = Peripherals::INTERRUPT.IPC12._0_1_;
  uVar3 = bVar2 & 0xff8f | 0x30;
  Peripherals::INTERRUPT.IPC12._0_1_ = (char)uVar3;
  Peripherals::INTERRUPT.IFS3 = 0;
  uVar1 = Peripherals::INTERRUPT.IEC3;
  Peripherals::INTERRUPT.IEC3 = uVar1 | 2;
  return CONCAT12(0x8f,uVar3);
}





// Function: FUN_rom_006126

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_006126(void)

{
  uint uVar1;
  ushort uVar2;
  
  if (_DAT_ram_1e40 == 0x21) {
    uVar2 = Peripherals::I2C2.I2C2RCV;
    DAT_ram_1979 = (undefined1)uVar2;
  }
  else {
    uVar2 = Peripherals::I2C2.I2C2RCV;
    (&DAT_ram_1958)[_DAT_ram_1e40] = (char)uVar2;
    _DAT_ram_1e40 = _DAT_ram_1e40 + 1;
  }
  _DAT_ram_1e3e = 0;
  if (DAT_ram_1958 == '\0') {
    if (1 < _DAT_ram_1e40) {
      uVar2 = Peripherals::I2C2.I2C2RCV;
      if (uVar2 == 0x13) {
        _DAT_ram_1940 = _DAT_ram_1940 | 2;
      }
      uVar2 = Peripherals::I2C2.I2C2RCV;
      if (uVar2 == 0x14) {
        _DAT_ram_1940 = _DAT_ram_1940 | 4;
      }
      uVar2 = Peripherals::I2C2.I2C2RCV;
      if (uVar2 == 0x15) {
        _DAT_ram_1940 = _DAT_ram_1940 | 8;
      }
      goto LAB_rom_00616a;
    }
  }
  _DAT_ram_1940 = 0;
LAB_rom_00616a:
  if (_DAT_ram_1e40 == 4) {
    if (DAT_ram_1958 == ';') {
      uVar1 = (uint)DAT_ram_195a * 0x100 + (uint)DAT_ram_195b;
      if (uVar1 < 0xf7ff) {
        _DAT_ram_1922 = uVar1 + 0x800;
      }
      else {
        _DAT_ram_1922 = -0x801;
      }
      _DAT_ram_1928 = _DAT_ram_1928 | 2;
      return;
    }
  }
  else if (_DAT_ram_1e40 == 3) {
    if (DAT_ram_1958 == ' ') {
      if (DAT_ram_195a < 0xb) {
        if (DAT_ram_1959 == '\x02') {
          uVar1 = (uint)DAT_ram_195a;
          if (DAT_ram_195a < 8) {
            _DAT_ram_160c = 1;
          }
          else {
            _DAT_ram_160c = 2;
            uVar1 = uVar1 - 8;
          }
          _DAT_ram_160a = uVar1 << 5;
          _DAT_ram_1928 = _DAT_ram_1928 | 0x20;
          return;
        }
        if (DAT_ram_1959 == '\x03') {
          uVar1 = (uint)DAT_ram_195a;
          if (DAT_ram_195a < 8) {
            _DAT_ram_160c = _DAT_ram_1e40;
          }
          else {
            _DAT_ram_160c = 4;
            uVar1 = uVar1 - 8;
          }
          _DAT_ram_160a = uVar1 << 5;
          _DAT_ram_1928 = _DAT_ram_1928 | 0x20;
        }
      }
    }
  }
  return;
}





// Function: FUN_rom_0061ea

bool FUN_rom_0061ea(int param_1,int param_2)

{
  ushort uVar1;
  int iVar2;
  uint uVar3;
  
  uVar1 = Peripherals::I2C2.I2C2ADD;
  iVar2 = uVar1 * 2;
  for (uVar3 = 0; uVar3 < param_2 - 1U; uVar3 = uVar3 + 1) {
    iVar2 = iVar2 + (uint)*(byte *)(param_1 + uVar3);
  }
  return (char)(*(char *)(param_1 + param_2 + -1) + (char)iVar2) != '\0';
}





// Function: FUN_rom_006216

undefined2 FUN_rom_006216(uint param_1)

{
  ushort uVar1;
  
  if (param_1 == 4) {
    uVar1 = 0xfa;
  }
  else if (param_1 < 4) {
    if (param_1 == 1) {
      uVar1 = 0xfe;
    }
    else {
      if (param_1 != 2) {
        return 1;
      }
      uVar1 = 0xce;
    }
  }
  else if (param_1 == 8) {
    uVar1 = 0x55;
  }
  else {
    if (param_1 != 0x10) {
      return 1;
    }
    uVar1 = 0xaa;
  }
  Peripherals::I2C2.I2C2TRN = uVar1;
  return 0;
}





// Function: FUN_rom_006252

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ulong FUN_rom_006252(void)

{
  ushort uVar1;
  uint uVar2;
  ulong uVar3;
  
  if (_DAT_ram_1e40 < 3) {
    if (_DAT_ram_1e40 != 0) {
      _DAT_ram_1950 = _DAT_ram_1950 | 1;
    }
  }
  else {
    _DAT_ram_192c = _DAT_ram_1e40 - 2;
    if (0xe < _DAT_ram_192c && _DAT_ram_1e40 != 0x11) {
      _DAT_ram_1950 = _DAT_ram_1950 | 2;
    }
  }
  uVar3 = FUN_rom_006216(_DAT_ram_1950);
  if ((char)uVar3 != '\0') {
    if (_DAT_ram_1e3e <= _DAT_ram_192c * 2 && _DAT_ram_192c * 2 - _DAT_ram_1e3e != 0) {
      if ((_DAT_ram_1e3e & 1) == 0) {
        uVar2 = (uint)**(byte **)(&DAT_ram_19b0 +
                                 (uint)(byte)(&DAT_ram_1959)[_DAT_ram_1e3e >> 1] * 2);
      }
      else {
        if ((_DAT_ram_1e3e & 1) != 1) {
          return CONCAT22(_DAT_ram_1e3e,_DAT_ram_1e3e) & 0xffff0001;
        }
        uVar2 = **(uint **)(&DAT_ram_19b0 + (uint)(byte)(&DAT_ram_1959)[_DAT_ram_1e3e >> 1] * 2) >>
                8;
      }
      Peripherals::I2C2.I2C2TRN = uVar2;
      uVar1 = Peripherals::I2C2.I2C2TRN;
      _DAT_ram_1956 = _DAT_ram_1956 + uVar1;
      return CONCAT22(&DAT_ram_19b0,uVar1);
    }
    _DAT_ram_1950 = _DAT_ram_1950 | 2;
    _DAT_ram_1940 = 0;
    uVar3 = CONCAT22(_DAT_ram_1e3e,-_DAT_ram_1956) & 0xffff00ff;
    Peripherals::I2C2.I2C2TRN = -_DAT_ram_1956 & 0xff;
  }
  return uVar3;
}





// Function: FUN_rom_0062bc

undefined4 FUN_rom_0062bc(uint param_1)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  undefined4 uVar4;
  
  param_1 = param_1 & 0xff;
  uVar2 = (uint)(byte)(&DAT_ram_1958)[param_1];
  uVar1 = uVar2 - 5;
  uVar3 = -(uint)(uVar2 < 5);
  if (uVar3 < (uVar1 < 0x38) || uVar2 == 0x3d && -(uint)(uVar1 < 0x38) == (uint)(uVar2 < 5)) {
                    /* WARNING: Could not emulate address calculation at 0x0062ce */
                    /* WARNING: Treating indirect jump as call */
    uVar4 = (*(code *)((int3)(int)uVar1 * 2 + 0x62e2))
                      (uVar1,uVar3,
                       (uint)(byte)(&DAT_ram_1959)[param_1] +
                       (uint)(byte)(&DAT_ram_195a)[param_1] * 0x100);
    return uVar4;
  }
  return CONCAT22(uVar3,uVar1);
}





// Function: FUN_rom_00644a

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_rom_00644a(uint param_1)

{
  byte bVar1;
  byte bVar2;
  uint uVar3;
  TIMER1 *pTVar4;
  uint uVar5;
  int *piVar6;
  undefined2 uVar7;
  TIMER1 *pTVar8;
  
  param_1 = param_1 & 0xff;
  bVar1 = (&DAT_ram_195a)[param_1];
  uVar3 = (uint)(byte)(&DAT_ram_1959)[param_1] + (uint)bVar1 * 0x100;
  bVar2 = (&DAT_ram_1958)[param_1];
  pTVar8 = (TIMER1 *)CONCAT11(bVar1,bVar2);
  if (bVar2 == 0x30) {
    if ((uVar3 & 0xf010) != 0) goto LAB_rom_006548;
    goto LAB_rom_006544;
  }
  if (bVar2 < 0x30) {
    if (bVar2 != 0x13) {
      if (bVar2 < 0x13) {
        if (((bVar2 != 6) && (bVar2 != 7)) && (bVar2 != 5)) goto LAB_rom_006544;
      }
      else if (bVar2 != 0x17) {
        if (bVar2 < 0x17) {
          if (bVar2 == 0x16) goto LAB_rom_0064c8;
        }
        else if ((bVar2 == 0x23) || (bVar2 == 0x24)) goto LAB_rom_0064c8;
        goto LAB_rom_006544;
      }
    }
LAB_rom_0064c8:
    if (uVar3 == 0) goto LAB_rom_006544;
    goto LAB_rom_006548;
  }
  pTVar4 = _DAT_ram_1a1c;
  if (bVar2 == 0x35) {
LAB_rom_00651c:
    pTVar8 = pTVar4;
    if (uVar3 <= pTVar8->TMR1 - 0x100) {
LAB_rom_006544:
      uVar7 = 0;
      goto LAB_rom_00654a;
    }
  }
  else {
    if (bVar2 < 0x35) {
      if (bVar2 == 0x32) {
        if (uVar3 < 0x3134) {
          uVar5 = 0x2fff;
LAB_rom_00650c:
          if (uVar5 < uVar3) goto LAB_rom_006544;
        }
      }
      else if (bVar2 < 0x32) {
        pTVar8 = (TIMER1 *)CONCAT11(bVar1,1);
        if (DAT_ram_15f6 != '\x01') {
          if ((_DAT_ram_1bea & 0x400) == 0) {
            if (uVar3 != 0) goto LAB_rom_006544;
          }
          else if ((uVar3 < 0x100) || (uVar3 == 0x8000)) goto LAB_rom_006544;
        }
      }
      else {
        if (bVar2 == 0x33) {
          uVar5 = 0x640;
        }
        else {
          if (bVar2 != 0x34) goto LAB_rom_006544;
          uVar5 = 0x1180;
        }
        if (uVar3 <= uVar5) {
          uVar5 = 0x3f;
          goto LAB_rom_00650c;
        }
      }
      goto LAB_rom_006548;
    }
    pTVar4 = _DAT_ram_1a20;
    if (bVar2 == 0x37) goto LAB_rom_00651c;
    piVar6 = _DAT_ram_1a1a;
    if ((bVar2 < 0x37) || (piVar6 = _DAT_ram_1a1e, bVar2 == 0x38)) {
      pTVar8 = &Peripherals::TIMER1;
      if (*piVar6 + 0x100U <= uVar3) goto LAB_rom_006544;
    }
    else if ((bVar2 != 0x3d) ||
            ((pTVar8 = (TIMER1 *)(uVar3 & 0xff), pTVar8 != (TIMER1 *)0x0 &&
             ((pTVar8 < (TIMER1 *)0xf0 || (pTVar8 == (TIMER1 *)0xfa)))))) goto LAB_rom_006544;
  }
LAB_rom_006548:
  uVar7 = 1;
LAB_rom_00654a:
  return CONCAT26(pTVar8,CONCAT24(uVar7,CONCAT22(&DAT_ram_1958,uVar3)));
}





// Function: FUN_rom_006582

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* WARNING: Restarted to delay deadcode elimination for space: ram */

ulong FUN_rom_006582(void)

{
  ushort uVar1;
  uint uVar2;
  ulong uVar3;
  
  if (_DAT_ram_1e40 == 3) {
    if (DAT_ram_1959 < 0x10) {
      _DAT_ram_192e = 0;
      goto LAB_rom_0065c2;
    }
  }
  else if (_DAT_ram_1e40 == 4) {
    if ((DAT_ram_195a != 0) && ((uint)DAT_ram_1959 + (uint)DAT_ram_195a < 0x101)) {
      _DAT_ram_192e = 1;
      goto LAB_rom_0065c2;
    }
  }
  else if (_DAT_ram_1e40 < 5) {
    if (_DAT_ram_1e40 != 0) {
      _DAT_ram_1950 = _DAT_ram_1950 | 1;
    }
    goto LAB_rom_0065c2;
  }
  _DAT_ram_1950 = _DAT_ram_1950 | 2;
LAB_rom_0065c2:
  uVar3 = FUN_rom_006216(_DAT_ram_1950);
  if ((char)uVar3 == '\0') {
    return uVar3;
  }
  if (_DAT_ram_192e == 0) {
    if (0xf < _DAT_ram_1e3e) {
      _DAT_ram_1950 = _DAT_ram_1950 | 2;
      Peripherals::I2C2.I2C2TRN = -_DAT_ram_1956 & 0xff;
      return CONCAT22(_DAT_ram_1e3e,-_DAT_ram_1956) & 0xffff00ff;
    }
    uVar2 = (uint)DAT_ram_1959 << 4;
  }
  else {
    if (DAT_ram_195a <= _DAT_ram_1e3e) {
      _DAT_ram_1950 = _DAT_ram_1950 | 2;
      Peripherals::I2C2.I2C2TRN = -_DAT_ram_1956 & 0xff;
      return CONCAT22(_DAT_ram_1e3e,-_DAT_ram_1956) & 0xffff00ff;
    }
    uVar2 = (uint)DAT_ram_1959;
  }
  Peripherals::I2C2.I2C2TRN = (uint)(byte)(&DAT_ram_160e)[uVar2 + _DAT_ram_1e3e];
  uVar1 = Peripherals::I2C2.I2C2TRN;
  _DAT_ram_1956 = uVar1 + _DAT_ram_1956;
  return CONCAT22(&DAT_ram_160e,_DAT_ram_1956);
}





// Function: FUN_rom_006612

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ushort FUN_rom_006612(void)

{
  uint uVar1;
  uint uVar2;
  ushort uVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  
  if (_DAT_ram_1e40 < 4) {
    if (_DAT_ram_1e40 != 0) {
      _DAT_ram_1950 = _DAT_ram_1950 | 1;
    }
  }
  else if (0xfe < (DAT_ram_1959 + _DAT_ram_1e40) - 4 && DAT_ram_1959 + _DAT_ram_1e40 != 0x103) {
    _DAT_ram_1950 = _DAT_ram_1950 | 2;
  }
  uVar3 = FUN_rom_006216(_DAT_ram_1950);
  if ((char)uVar3 != '\0') {
    uVar1 = _DAT_ram_1e40 - 3;
    uVar2 = (uint)DAT_ram_1959;
    uVar7 = _DAT_ram_1bf4 & 0x100;
    uVar4 = (uint)((byte)(_DAT_ram_1928 >> 4) & 1);
    uVar5 = 0;
    while (uVar5 < uVar1) {
      if ((uVar7 != 0) || (uVar6 = uVar1, 199 < uVar2 + uVar5)) {
        (&DAT_ram_160e)[uVar2 + uVar5] = (&DAT_ram_195a)[uVar5];
        uVar4 = 1;
        uVar6 = uVar5;
      }
      uVar5 = uVar6 + 1;
    }
    _DAT_ram_1928 = _DAT_ram_1928 & 0xffef | uVar4 << 4;
    if (uVar4 == 0) {
      uVar3 = 0xaa;
    }
    else {
      uVar3 = 0x31;
    }
    _DAT_ram_1950 = _DAT_ram_1950 | 2;
    Peripherals::I2C2.I2C2TRN = uVar3;
  }
  return uVar3;
}





// Function: FUN_rom_006692

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ushort FUN_rom_006692(void)

{
  ushort uVar1;
  undefined1 *puVar2;
  undefined1 extraout_var;
  undefined1 *puVar3;
  uint extraout_W2;
  uint uVar4;
  
  if (_DAT_ram_1e40 != 8) {
    if (_DAT_ram_1e40 < 9) {
      if (_DAT_ram_1e40 != 0) {
        _DAT_ram_1950 = _DAT_ram_1950 | 1;
      }
    }
    else {
      _DAT_ram_1950 = _DAT_ram_1950 | 2;
    }
  }
  uVar1 = FUN_rom_006216(_DAT_ram_1950);
  if ((char)uVar1 == '\0') {
    return uVar1;
  }
  _DAT_ram_1950 = _DAT_ram_1950 | 2;
  if (((DAT_ram_1959 != '\x05') || (DAT_ram_195a != -0x5a)) || (DAT_ram_195c != '\x01')) {
    _DAT_ram_1e1a = _DAT_ram_1e1a & 0xfdff;
    FUN_rom_00a688(0x19ff,CONCAT11(extraout_var,0xff),extraout_W2 & 0xff00);
    _DAT_ram_1bbc = FUN_rom_00a6b8();
    Peripherals::I2C2.I2C2TRN = 0xce;
    return 0xce;
  }
  puVar2 = &DAT_ram_195b;
  puVar3 = (undefined1 *)CONCAT11(extraout_var,2);
  uVar4 = extraout_W2;
  if (DAT_ram_195b == '\x02') {
    puVar2 = (undefined1 *)(uint)DAT_ram_195d;
    puVar3 = (undefined1 *)(_DAT_ram_1bba >> 8);
    uVar4 = _DAT_ram_1bba;
    if ((puVar2 == puVar3) && (puVar2 = (undefined1 *)0x0, DAT_ram_195e == (char)_DAT_ram_1bba)) {
      _DAT_ram_1e1a = _DAT_ram_1e1a | 0x200;
      FUN_rom_00a688(0xff,0xff,1);
      _DAT_ram_1bbc = FUN_rom_00a6b8();
      uVar1 = 0x31;
      goto LAB_rom_00670e;
    }
  }
  _DAT_ram_1e1a = _DAT_ram_1e1a & 0xfdff;
  FUN_rom_00a688(CONCAT11((char)((uint)puVar2 >> 8),0xff),CONCAT11((char)((uint)puVar3 >> 8),0xff),
                 uVar4 & 0xff00);
  _DAT_ram_1bbc = FUN_rom_00a6b8();
  uVar1 = 0xce;
LAB_rom_00670e:
  Peripherals::I2C2.I2C2TRN = uVar1;
  return uVar1;
}





// Function: FUN_rom_00672a

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined2 FUN_rom_00672a(void)

{
  undefined2 uVar1;
  
  if (_DAT_ram_1e40 == 4) {
    if ((_DAT_ram_1e1a & 0x200) == 0) {
      _DAT_ram_1950 = _DAT_ram_1950 | 0x10;
    }
  }
  else if (_DAT_ram_1e40 < 5) {
    if (_DAT_ram_1e40 != 0) {
      _DAT_ram_1950 = _DAT_ram_1950 | 1;
    }
  }
  else {
    _DAT_ram_1950 = _DAT_ram_1950 | 2;
  }
  uVar1 = FUN_rom_006216(_DAT_ram_1950);
  if ((char)uVar1 != '\0') {
    _DAT_ram_1950 = _DAT_ram_1950 | 2;
    if ((DAT_ram_1959 == 'P') && (DAT_ram_195a == 'H')) {
      _DAT_ram_1e1a = _DAT_ram_1e1a | 1;
      Peripherals::I2C2.I2C2TRN = 0x31;
      return 0x31;
    }
    uVar1 = 0xce;
    Peripherals::I2C2.I2C2TRN = 0xce;
  }
  return uVar1;
}





// Function: FUN_rom_00676a

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ushort FUN_rom_00676a(void)

{
  byte bVar1;
  ushort uVar2;
  uint uVar3;
  int iVar4;
  uint uVar5;
  undefined1 *puVar6;
  
  if (_DAT_ram_1e40 - 7 < 0x10 || _DAT_ram_1e40 == 0x17) {
    if (DAT_ram_1959 < 0x11) {
      if ((_DAT_ram_1e1a & 0x200) == 0) {
        _DAT_ram_1950 = _DAT_ram_1950 | 0x10;
      }
    }
    else {
LAB_rom_006786:
      _DAT_ram_1950 = _DAT_ram_1950 | 2;
    }
  }
  else {
    if (0x17 < _DAT_ram_1e40) goto LAB_rom_006786;
    if (_DAT_ram_1e40 != 0) {
      _DAT_ram_1950 = _DAT_ram_1950 | 1;
    }
  }
  uVar2 = FUN_rom_006216(_DAT_ram_1950);
  bVar1 = DAT_ram_1959;
  if ((char)uVar2 == '\0') {
    return uVar2;
  }
  _DAT_ram_1950 = _DAT_ram_1950 | 2;
  uVar3 = DAT_ram_1959 + 4;
  iVar4 = 0;
  for (uVar5 = 1; uVar5 <= uVar3; uVar5 = uVar5 + 1) {
    iVar4 = iVar4 + (uint)(byte)(&DAT_ram_1958)[uVar5];
  }
  _DAT_ram_1956 = -iVar4 & 0xff;
  if (_DAT_ram_1956 != (byte)(&DAT_ram_195d)[DAT_ram_1959]) {
    Peripherals::I2C2.I2C2TRN = 0xce;
    return 0xce;
  }
  uVar5 = _DAT_ram_1956;
  if (DAT_ram_195c == '\0') {
    if ((_DAT_ram_1e40 < 8) || (DAT_ram_1959 == 0)) {
      uVar2 = 0xce;
      goto LAB_rom_00683e;
    }
    for (uVar5 = 5; uVar5 <= uVar3; uVar5 = uVar5 + 1) {
      *(undefined1 *)(uVar5 + 0x1709) = (&DAT_ram_1958)[uVar5];
    }
    _DAT_ram_1920 = (uint)DAT_ram_195a * 0x100 + (uint)DAT_ram_195b;
    if (_DAT_ram_1920 < 0xf7ff) {
      _DAT_ram_1920 = _DAT_ram_1920 + 0x800;
    }
    else {
      _DAT_ram_1920 = 0xf7ff;
    }
    uVar5 = _DAT_ram_1920 & 0xff;
    puVar6 = (undefined1 *)(uVar5 + 0x171e);
    _DAT_ram_1608 = (uint)bVar1;
    for (uVar3 = 0; uVar3 < bVar1; uVar3 = uVar3 + 1) {
      _DAT_ram_1930 = uVar3 + uVar5;
      *puVar6 = (&DAT_ram_170e)[uVar3];
      puVar6 = puVar6 + 1;
    }
    uVar5 = _DAT_ram_1930;
    if (_DAT_ram_1930 == 0xff) {
      _DAT_ram_1928 = _DAT_ram_1928 | 1;
    }
  }
  FUN_rom_00a688(0xff,uVar5 & 0xff00);
  uVar2 = 0x31;
LAB_rom_00683e:
  Peripherals::I2C2.I2C2TRN = uVar2;
  return uVar2;
}





// Function: FUN_rom_006848

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* WARNING: Restarted to delay deadcode elimination for space: ram */

ulong FUN_rom_006848(void)

{
  ushort uVar1;
  ulong uVar2;
  
  if (_DAT_ram_1e40 == 5) {
    if (DAT_ram_1959 < 0x11) goto LAB_rom_006866;
  }
  else if (_DAT_ram_1e40 < 6) {
    if (_DAT_ram_1e40 != 0) {
      _DAT_ram_1950 = _DAT_ram_1950 | 1;
    }
    goto LAB_rom_006866;
  }
  _DAT_ram_1950 = _DAT_ram_1950 | 2;
LAB_rom_006866:
  uVar2 = FUN_rom_006216(_DAT_ram_1950);
  if ((char)uVar2 != '\0') {
    if (_DAT_ram_1e3e == DAT_ram_1959) {
      _DAT_ram_1950 = _DAT_ram_1950 | 2;
      Peripherals::I2C2.I2C2TRN = -_DAT_ram_1956 & 0xff;
      return CONCAT22(_DAT_ram_1e3e,-_DAT_ram_1956) & 0xffff00ff;
    }
    Peripherals::I2C2.I2C2TRN = (uint)*(byte *)(_DAT_ram_1e3e + 0x181e);
    uVar1 = Peripherals::I2C2.I2C2TRN;
    _DAT_ram_1956 = uVar1 + _DAT_ram_1956;
    uVar2 = CONCAT22(_DAT_ram_1e3e,_DAT_ram_1956);
  }
  return uVar2;
}





// Function: FUN_rom_006894

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_006894(void)

{
  char cVar1;
  uint uVar2;
  
  if (_DAT_ram_1e40 != 2) {
    if (_DAT_ram_1e40 < 3) {
      if (_DAT_ram_1e40 != 0) {
        _DAT_ram_1950 = _DAT_ram_1950 | 8;
      }
    }
    else {
      _DAT_ram_1950 = _DAT_ram_1950 | 2;
    }
  }
  cVar1 = FUN_rom_006216(_DAT_ram_1950);
  if (cVar1 != '\0') {
    _DAT_ram_1950 = _DAT_ram_1950 | 2;
    Peripherals::I2C2.I2C2TRN = 0x31;
    uVar2 = _DAT_ram_1e1a & 0x100;
    if (uVar2 == 0) {
      _DAT_ram_1e1a = _DAT_ram_1e1a | 0x100;
      _DAT_ram_191e = 0x800;
      _DAT_ram_1606 = uVar2;
      FUN_rom_00a688(0x8ff,1);
    }
  }
  return;
}





// Function: FUN_rom_0068d4

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_0068d4(void)

{
  ushort uVar1;
  undefined4 uVar2;
  
  if (_DAT_ram_1e40 == 3) {
    if (DAT_ram_1959 == -0x20) goto LAB_rom_0068e8;
  }
  else if (_DAT_ram_1e40 == 0) goto LAB_rom_0068e8;
  _DAT_ram_1950 = _DAT_ram_1950 | 8;
LAB_rom_0068e8:
  uVar2 = FUN_rom_006216(_DAT_ram_1950);
  if ((char)uVar2 != '\0') {
    if (_DAT_ram_1e3e == 10) {
      Peripherals::I2C2.I2C2TRN = -_DAT_ram_1956 & 0xff;
      _DAT_ram_1950 = _DAT_ram_1950 | 2;
      return 0xa000a;
    }
    Peripherals::I2C2.I2C2TRN = (uint)(byte)(&DAT_ram_1d03)[_DAT_ram_1e3e];
    uVar1 = Peripherals::I2C2.I2C2TRN;
    _DAT_ram_1956 = uVar1 + _DAT_ram_1956;
    uVar2 = CONCAT22(_DAT_ram_1e3e,_DAT_ram_1956);
  }
  return uVar2;
}





// Function: FUN_rom_006912

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined2 FUN_rom_006912(void)

{
  undefined2 uVar1;
  
  if (_DAT_ram_1e40 != 5) {
    if (_DAT_ram_1e40 < 6) {
      if (_DAT_ram_1e40 != 0) {
        _DAT_ram_1950 = _DAT_ram_1950 | 1;
      }
    }
    else {
      _DAT_ram_1950 = _DAT_ram_1950 | 2;
    }
  }
  uVar1 = FUN_rom_006216(_DAT_ram_1950);
  if ((char)uVar1 != '\0') {
    _DAT_ram_1950 = _DAT_ram_1950 | 2;
    if ((DAT_ram_1959 == 'P') && (DAT_ram_195a == 'H')) {
      if (DAT_ram_195b == '\x01') {
        _DAT_ram_1bf4 = _DAT_ram_1bf4 | 0x100;
LAB_rom_006950:
        Peripherals::I2C2.I2C2TRN = 0x31;
        return 0x31;
      }
      if (DAT_ram_195b == '\0') {
        _DAT_ram_1bf4 = _DAT_ram_1bf4 & 0xfeff;
        goto LAB_rom_006950;
      }
    }
    _DAT_ram_1bf4 = _DAT_ram_1bf4 & 0xfeff;
    uVar1 = 0xce;
    Peripherals::I2C2.I2C2TRN = 0xce;
  }
  return uVar1;
}





// Function: FUN_rom_00695e

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ushort FUN_rom_00695e(void)

{
  ushort uVar1;
  int iVar2;
  bool bVar3;
  
  if ((_DAT_ram_1e40 < 2) && (_DAT_ram_1e40 != 0)) {
    _DAT_ram_1950 = _DAT_ram_1950 | 8;
  }
  uVar1 = FUN_rom_006216(_DAT_ram_1950);
  if ((char)uVar1 != '\0') {
    if ((_DAT_ram_1bf4 & 0x100) != 0) {
      if (_DAT_ram_1e40 == 0xb) {
        _DAT_ram_1950 = _DAT_ram_1950 | 4;
        iVar2 = 0;
        do {
          (&DAT_ram_1cfa)[iVar2] = (&DAT_ram_1959)[iVar2];
          bVar3 = iVar2 != 8;
          iVar2 = iVar2 + 1;
        } while (bVar3);
        _DAT_ram_1e18 = _DAT_ram_1e18 | 2;
        uVar1 = 0x31;
      }
      else {
        if (((&DAT_ram_1cf8)[_DAT_ram_1e3e] != '\n') || (*(char *)(_DAT_ram_1e3e + 0x1cf9) != '\r'))
        {
          Peripherals::I2C2.I2C2TRN = (uint)(byte)(&DAT_ram_1cfa)[_DAT_ram_1e3e];
          uVar1 = Peripherals::I2C2.I2C2TRN;
          _DAT_ram_1956 = _DAT_ram_1956 + uVar1;
          return uVar1;
        }
        _DAT_ram_1950 = _DAT_ram_1950 | 4;
        uVar1 = -_DAT_ram_1956 & 0xff;
      }
      Peripherals::I2C2.I2C2TRN = uVar1;
      return uVar1;
    }
    uVar1 = 0x55;
    Peripherals::I2C2.I2C2TRN = 0x55;
  }
  return uVar1;
}





// Function: FUN_rom_0069c8

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ushort FUN_rom_0069c8(void)

{
  byte bVar1;
  ushort uVar2;
  
  if ((_DAT_ram_1e40 < 2) && (_DAT_ram_1e40 != 0)) {
    _DAT_ram_1950 = _DAT_ram_1950 | 8;
  }
  uVar2 = FUN_rom_006216(_DAT_ram_1950);
  if ((char)uVar2 != '\0') {
    if ((_DAT_ram_1bf4 & 0x100) != 0) {
      if (_DAT_ram_1e40 == 0x16) {
        DAT_ram_15f7 = DAT_ram_1959;
        DAT_ram_15f8 = DAT_ram_195a;
        DAT_ram_15f9 = DAT_ram_195b;
        DAT_ram_15fa = DAT_ram_195c;
        DAT_ram_15e6 = DAT_ram_195d;
        DAT_ram_15e7 = DAT_ram_195e;
        DAT_ram_15e8 = DAT_ram_195f;
        DAT_ram_15e9 = DAT_ram_1960;
        DAT_ram_15ea = DAT_ram_1961;
        DAT_ram_15eb = DAT_ram_1962;
        DAT_ram_15ec = DAT_ram_1963;
        DAT_ram_15ed = DAT_ram_1964;
        DAT_ram_15ee = DAT_ram_1965;
        DAT_ram_15ef = DAT_ram_1966;
        DAT_ram_15f0 = DAT_ram_1967;
        DAT_ram_15f1 = DAT_ram_1968;
        DAT_ram_15f2 = DAT_ram_1969;
        DAT_ram_15f3 = DAT_ram_196a;
        DAT_ram_15f4 = DAT_ram_196b;
        DAT_ram_15f5 = DAT_ram_196c;
        _DAT_ram_1928 = _DAT_ram_1928 | 0x40;
        _DAT_ram_125a = _DAT_ram_125a | 0x40;
        uVar2 = 0x31;
      }
      else {
        if (_DAT_ram_1e3e != 0x14) {
          bVar1 = DAT_ram_15f7;
          if ((((_DAT_ram_1e3e != 0) && (bVar1 = DAT_ram_15f8, _DAT_ram_1e3e != 1)) &&
              (bVar1 = DAT_ram_15f9, _DAT_ram_1e3e != 2)) &&
             (bVar1 = DAT_ram_15fa, _DAT_ram_1e3e != 3)) {
            bVar1 = (&DAT_ram_15e2)[_DAT_ram_1e3e];
          }
          Peripherals::I2C2.I2C2TRN = (uint)bVar1;
          uVar2 = Peripherals::I2C2.I2C2TRN;
          _DAT_ram_1956 = _DAT_ram_1956 + uVar2;
          return uVar2;
        }
        uVar2 = -_DAT_ram_1956 & 0xff;
      }
      _DAT_ram_1950 = _DAT_ram_1950 | 4;
      Peripherals::I2C2.I2C2TRN = uVar2;
      return uVar2;
    }
    uVar2 = 0x55;
    Peripherals::I2C2.I2C2TRN = 0x55;
  }
  return uVar2;
}





// Function: FUN_rom_006b18

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined2 FUN_rom_006b18(void)

{
  undefined2 uVar1;
  
  if (_DAT_ram_1e40 == 3) {
    if (DAT_ram_1959 == '\x01') goto LAB_rom_006b36;
  }
  else if (_DAT_ram_1e40 < 4) {
    if (_DAT_ram_1e40 != 0) {
      _DAT_ram_1950 = _DAT_ram_1950 | 1;
    }
    goto LAB_rom_006b36;
  }
  _DAT_ram_1950 = _DAT_ram_1950 | 2;
LAB_rom_006b36:
  uVar1 = FUN_rom_006216(_DAT_ram_1950);
  if ((char)uVar1 != '\0') {
    _DAT_ram_1950 = _DAT_ram_1950 | 2;
    if ((_DAT_ram_1bf4 & 0x100) != 0) {
      _DAT_ram_1e1a = _DAT_ram_1e1a | 0x48;
      Peripherals::I2C2.I2C2TRN = 0x31;
      return 0x31;
    }
    uVar1 = 0xaa;
    Peripherals::I2C2.I2C2TRN = 0xaa;
  }
  return uVar1;
}





// Function: FUN_rom_006b54

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined2 FUN_rom_006b54(void)

{
  undefined2 uVar1;
  
  if (_DAT_ram_1e40 == 3) {
    if ((((_DAT_ram_1e18 & 0x100) == 0) && (_DAT_ram_1e22 != 5)) && (DAT_ram_1959 == -0x56))
    goto LAB_rom_006b7a;
  }
  else if (_DAT_ram_1e40 < 4) {
    if (_DAT_ram_1e40 != 0) {
      _DAT_ram_1950 = _DAT_ram_1950 | 1;
    }
    goto LAB_rom_006b7a;
  }
  _DAT_ram_1950 = _DAT_ram_1950 | 2;
LAB_rom_006b7a:
  uVar1 = FUN_rom_006216(_DAT_ram_1950);
  if ((char)uVar1 != '\0') {
    _DAT_ram_1950 = _DAT_ram_1950 | 2;
    _DAT_ram_1e18 = _DAT_ram_1e18 | 0x100;
    _DAT_ram_1bcc = _DAT_ram_1bca;
    uVar1 = 0x31;
    Peripherals::I2C2.I2C2TRN = 0x31;
  }
  return uVar1;
}





// Function: FUN_rom_006b90

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined2 FUN_rom_006b90(void)

{
  undefined2 uVar1;
  
  if (_DAT_ram_1e40 == 3) {
    if (DAT_ram_1959 == '\x01') goto LAB_rom_006bae;
  }
  else if (_DAT_ram_1e40 < 4) {
    if (_DAT_ram_1e40 != 0) {
      _DAT_ram_1950 = _DAT_ram_1950 | 1;
    }
    goto LAB_rom_006bae;
  }
  _DAT_ram_1950 = _DAT_ram_1950 | 2;
LAB_rom_006bae:
  uVar1 = FUN_rom_006216(_DAT_ram_1950);
  if ((char)uVar1 != '\0') {
    _DAT_ram_1950 = _DAT_ram_1950 | 2;
    if ((_DAT_ram_1bf4 & 0x100) != 0) {
      *_DAT_ram_1a10 = 0;
      *_DAT_ram_1a12 = 0;
      DAT_ram_15f6 = 0;
      _DAT_ram_1bea = _DAT_ram_1bea & 0xfff7;
      _DAT_ram_1928 = _DAT_ram_1928 | 0x40;
      Peripherals::I2C2.I2C2TRN = 0x31;
      return 0x31;
    }
    uVar1 = 0xaa;
    Peripherals::I2C2.I2C2TRN = 0xaa;
  }
  return uVar1;
}





// Function: FUN_rom_006bd6

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_006bd6(void)

{
  byte bVar1;
  
  _DAT_ram_1bf0 =
       _DAT_ram_1bf0 & 0xfff1 | _DAT_ram_1e20 >> 3 & 2 | (_DAT_ram_1e20 & 1) << 2 |
       ((_DAT_ram_1e20 >> 1 | _DAT_ram_1e20 >> 3) & 1) << 3;
  if ((_DAT_ram_1e20 & 4) != 0) {
    bVar1 = Peripherals::GPIOD.LATD._0_1_;
    if ((bVar1 & 0x20) != 0) {
      _DAT_ram_1bec = _DAT_ram_1bec | 0x100;
      if ((_DAT_ram_1bea & 0x20) != 0) {
        _DAT_ram_1bf0 = _DAT_ram_1bf0 | 0x40;
      }
      goto LAB_rom_006c10;
    }
  }
  _DAT_ram_1bec = _DAT_ram_1bec & 0xfeff;
LAB_rom_006c10:
  if ((_DAT_ram_1e1c & 2) == 0) {
    _DAT_ram_1bf2 = _DAT_ram_1bf2 & 0xfffd;
    if ((_DAT_ram_1bea & 0x800) == 0) {
      _DAT_ram_1bf0 = _DAT_ram_1bf0 | 0x10;
    }
  }
  else {
    _DAT_ram_1bf2 = _DAT_ram_1bf2 | 2;
    _DAT_ram_1bf0 = _DAT_ram_1bf0 & 0xffef;
  }
  _DAT_ram_1bbc = FUN_rom_00a6b8();
  _DAT_ram_1bba = FUN_rom_00a6c0();
  _DAT_ram_1bb8 = FUN_rom_00a6c8();
  _DAT_ram_198e = FUN_rom_00a6d0();
  _DAT_ram_198c = FUN_rom_00a6d8();
  return;
}





// Function: FUN_rom_006c42

/* WARNING: Heritage AFTER dead removal. Example location: W1 : 0x006c9a */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* WARNING: Restarted to delay deadcode elimination for space: ram */

ulonglong FUN_rom_006c42(void)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  
  iVar1 = _DAT_ram_1e22;
  if (_DAT_ram_1e22 == 2) {
    _DAT_ram_1be0 = _DAT_ram_1260 << 2;
    if (((_DAT_ram_1bf4 & 0x100) != 0) || (_DAT_ram_1be2 = _DAT_ram_1be0, _DAT_ram_1d1a == 0)) {
      _DAT_ram_1be2 = _DAT_ram_125e << 2;
    }
    _DAT_ram_1d0e = (uint)((ulong)((long)_DAT_ram_1d1e * 0x1e80) >> 10);
    _DAT_ram_1bde = _DAT_ram_1d0e;
    if (_DAT_ram_1d0e < 0x21) {
      _DAT_ram_1bde = 0x20;
    }
    uVar5 = _DAT_ram_1bde;
    uVar2 = FUN_rom_005d00(_DAT_ram_1be0,(int)((ulong)((long)_DAT_ram_1d1e * 0x1e80) >> 0x10),
                           _DAT_ram_1bde);
    iVar3 = (int)((ulong)uVar2 >> 0x10);
    _DAT_ram_1bbe = iVar3 << 2 | (uint)uVar2 >> 0xe;
    if (_DAT_ram_1bbe < 7) {
      _DAT_ram_1bbe = 6;
    }
    uVar2 = FUN_rom_005d00(_DAT_ram_1be2,iVar3,uVar5);
    _DAT_ram_1bc0 = (int)((ulong)uVar2 >> 0x10) << 2 | (uint)uVar2 >> 0xe;
    if (_DAT_ram_1bc0 < 7) {
      _DAT_ram_1bc0 = 6;
    }
  }
  else {
    _DAT_ram_1be2 = 0;
    _DAT_ram_1bde = 0;
    _DAT_ram_1bc0 = 0;
    _DAT_ram_1be0 = 0;
    _DAT_ram_1bbe = 0;
  }
  if (((_DAT_ram_1bf2 & 1) != 0) && (iVar1 == 2)) {
    uVar5 = 0xffcc;
    if (_DAT_ram_199e < 0xffcc) {
      uVar5 = _DAT_ram_199e + 0x33;
    }
    uVar4 = 0;
    if (0x33 < _DAT_ram_199c) {
      uVar4 = _DAT_ram_199c - 0x33;
    }
    if (_DAT_ram_1be2 < _DAT_ram_199e) {
      _DAT_ram_1bec = _DAT_ram_1bec | 8;
    }
    else if (((_DAT_ram_1264 & 8) == 0) && (uVar5 <= _DAT_ram_1be2)) {
      _DAT_ram_1bec = _DAT_ram_1bec & 0xfff7;
    }
    if (_DAT_ram_199c < _DAT_ram_1be2) {
      _DAT_ram_1bec = _DAT_ram_1bec | 4;
    }
    else if ((_DAT_ram_1be2 < 0xc99) && (_DAT_ram_1be2 <= uVar4)) {
      _DAT_ram_1bec = _DAT_ram_1bec & 0xfffb;
    }
  }
  uVar2 = CONCAT22(_DAT_ram_199a,(int)((ulong)_DAT_ram_199a * 0x3d >> 0x10) << 10);
  if (((_DAT_ram_1bf2 & 1) != 0) && (iVar1 == 2)) {
    uVar2 = CONCAT22(_DAT_ram_199a,_DAT_ram_1bde);
    if (_DAT_ram_199a < _DAT_ram_1bde) {
      _DAT_ram_1bec = _DAT_ram_1bec | 0x40;
      goto LAB_rom_006d46;
    }
    if ((uint)((ulong)_DAT_ram_199a * 0x3d >> 6) < _DAT_ram_1bde) goto LAB_rom_006d46;
  }
  _DAT_ram_1bec = _DAT_ram_1bec & 0xffbf;
LAB_rom_006d46:
  if ((_DAT_ram_1bf4 & 0x100) == 0) {
    _DAT_ram_197a = 0;
  }
  else {
    _DAT_ram_197a = _DAT_ram_1be0;
  }
  return (ulonglong)CONCAT24(_DAT_ram_1bf4,uVar2) & 0xffff0100ffffffff;
}





// Function: FUN_rom_006d5a

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_006d5a(void)

{
  IC1 *pIVar1;
  
  _DAT_ram_1bdc = (IC1 *)(_DAT_ram_1e16 * 0x40);
  _DAT_ram_1bda = (IC1 *)(_DAT_ram_1d16 * 0x40);
  _DAT_ram_1bd8 = (IC1 *)(_DAT_ram_1e12 * 0x40);
  _DAT_ram_1bd6 = _DAT_ram_1e46 << 6;
  if (_DAT_ram_1cc0 != 0) {
    return;
  }
  pIVar1 = (IC1 *)0x0;
  if (&Peripherals::IC1 < _DAT_ram_1998) {
    pIVar1 = _DAT_ram_1998 + -0x50;
  }
  if (((_DAT_ram_1e20 & 8) == 0) && (_DAT_ram_1bdc < _DAT_ram_1998)) {
    if (_DAT_ram_1bdc < pIVar1 || (int)_DAT_ram_1bdc - (int)pIVar1 == 0) {
      _DAT_ram_1bec = _DAT_ram_1bec & 0xffef;
    }
  }
  else {
    _DAT_ram_1bec = _DAT_ram_1bec | 0x10;
  }
  pIVar1 = (IC1 *)0x0;
  if (&Peripherals::IC1 < _DAT_ram_1996) {
    pIVar1 = _DAT_ram_1996 + -0x50;
  }
  if (_DAT_ram_1bda < _DAT_ram_1996) {
    if (_DAT_ram_1bda < pIVar1 || (int)_DAT_ram_1bda - (int)pIVar1 == 0) {
      _DAT_ram_194e = _DAT_ram_194e & 0xfeff;
    }
  }
  else {
    _DAT_ram_194e = _DAT_ram_194e | 0x100;
  }
  pIVar1 = (IC1 *)0x0;
  if (&Peripherals::IC1 < _DAT_ram_1994) {
    pIVar1 = _DAT_ram_1994 + -0x50;
  }
  if (_DAT_ram_1bd8 < _DAT_ram_1994) {
    if (_DAT_ram_1bd8 < pIVar1 || (int)_DAT_ram_1bd8 - (int)pIVar1 == 0) {
      _DAT_ram_194e = _DAT_ram_194e & 0xfdff;
    }
  }
  else {
    _DAT_ram_194e = _DAT_ram_194e | 0x200;
  }
  if (((_DAT_ram_1e20 & 2) == 0) && ((_DAT_ram_194e & 0x300) == 0)) {
    _DAT_ram_1bec = _DAT_ram_1bec & 0xffdf;
  }
  else {
    _DAT_ram_1bec = _DAT_ram_1bec | 0x20;
  }
  if ((_DAT_ram_1e20 & 8) == 0) {
    _DAT_ram_1e18 = _DAT_ram_1e18 & 0xfdff;
  }
  else {
    _DAT_ram_1e18 = _DAT_ram_1e18 | 0x200;
  }
  if ((_DAT_ram_1264 & 0x4000) == 0) {
    if (-1 < _DAT_ram_1e1a) {
      _DAT_ram_1bf0 = _DAT_ram_1bf0 & 0xfcff;
      return;
    }
    _DAT_ram_1bf0 = _DAT_ram_1bf0 & 0xfeff;
    _DAT_ram_1bf0 = _DAT_ram_1bf0 | 0x200;
    return;
  }
  _DAT_ram_1bf0 = _DAT_ram_1bf0 & 0xfdff | 0x100;
  return;
}





// Function: FUN_rom_006dfe

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_006dfe(void)

{
  int iVar1;
  
  iVar1 = _DAT_ram_1932;
  if (_DAT_ram_1bf0 != _DAT_ram_1932) {
    _DAT_ram_1932 = _DAT_ram_1bf0;
    _DAT_ram_1e1c = _DAT_ram_1e1c & 0xfff7;
  }
  if (_DAT_ram_1bf0 == 0) {
    _DAT_ram_194e = _DAT_ram_194e & 0xfffe;
  }
  else if (_DAT_ram_1bf0 != 0x10) {
    _DAT_ram_194e = _DAT_ram_194e | 1;
  }
  if (_DAT_ram_1bec == 0) {
    _DAT_ram_194e = _DAT_ram_194e & 0xfffd;
    return CONCAT22(_DAT_ram_1bf0,iVar1);
  }
  _DAT_ram_194e = _DAT_ram_194e | 2;
  return CONCAT22(_DAT_ram_1bf0,iVar1);
}





// Function: FUN_rom_006e24

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

uint FUN_rom_006e24(void)

{
  long lVar1;
  uint uVar2;
  uint uVar3;
  
  _DAT_ram_1be6 = _DAT_ram_1bf2 & 2;
  uVar2 = _DAT_ram_1be6;
  _DAT_ram_1be8 = _DAT_ram_1be6;
  if ((_DAT_ram_1bf2 & 2) == 0) goto LAB_rom_006eca;
  _DAT_ram_1be8 = _DAT_ram_1d14;
  uVar2 = _DAT_ram_1d14;
  if (_DAT_ram_1d14 < 0xc81) {
    uVar2 = 0xc80;
  }
  if (_DAT_ram_1bbe < 0x8c) {
    uVar3 = _DAT_ram_1bbe + 0xf;
  }
  else {
    uVar3 = (int)((ulong)_DAT_ram_1bbe * 0x472 >> 0x10) << 6 |
            (uint)((ulong)_DAT_ram_1bbe * 0x472 >> 10) & 0x3f;
  }
  lVar1 = CONCAT22(_DAT_ram_1bbe >> 5,_DAT_ram_1bbe << 0xb);
  isDivideOverflow(lVar1,uVar2);
  _DAT_ram_1be6 = (uint)(lVar1 / (long)(int)uVar2);
  lVar1 = CONCAT22(uVar3 >> 5,uVar3 << 0xb);
  isDivideOverflow(lVar1,uVar2);
  uVar2 = (uint)(lVar1 / (long)(int)uVar2);
  if ((_DAT_ram_1bbe < 0x8c) && ((_DAT_ram_1bf2 & 0x400) == 0)) {
    uVar2 = uVar2 + 7;
  }
  if (_DAT_ram_1d10 < 0xf001) {
    if (_DAT_ram_1d10 < _DAT_ram_1bbe) {
      if (_DAT_ram_1d10 < 0xf) {
LAB_rom_006ea2:
        _DAT_ram_1be4 = _DAT_ram_1d10;
      }
      else {
        _DAT_ram_1be4 = _DAT_ram_1bbe;
      }
    }
    else {
      _DAT_ram_1be4 = uVar3;
      if (_DAT_ram_1d10 <= uVar3) goto LAB_rom_006ea2;
    }
  }
  if ((_DAT_ram_1be6 <= _DAT_ram_1d12) && (_DAT_ram_1be6 = uVar2, _DAT_ram_1d12 <= uVar2)) {
    _DAT_ram_1be6 = _DAT_ram_1d12;
  }
  uVar2 = _DAT_ram_1be4;
  if (_DAT_ram_1be6 < 4) {
    _DAT_ram_1be6 = 4;
  }
LAB_rom_006eca:
  _DAT_ram_1be4 = uVar2;
  uVar2 = _DAT_ram_1be6;
  if (((_DAT_ram_1bf2 & 2) != 0) && (_DAT_ram_1cc0 == 0)) {
    uVar2 = 0xffff;
    if (_DAT_ram_19a2 < 0xfedf) {
      uVar2 = _DAT_ram_19a2 + 0x120;
    }
    if (_DAT_ram_1be8 < _DAT_ram_19a2) {
      _DAT_ram_1bec = _DAT_ram_1bec | 2;
    }
    else if (uVar2 < _DAT_ram_1be8) {
      _DAT_ram_1bec = _DAT_ram_1bec & 0xfffd;
    }
    uVar3 = 0;
    if (0xa0 < _DAT_ram_19a0) {
      uVar3 = _DAT_ram_19a0 - 0xa0;
    }
    if (_DAT_ram_19a0 < _DAT_ram_1be8) {
      _DAT_ram_1bec = _DAT_ram_1bec | 1;
      return _DAT_ram_19a0;
    }
    uVar2 = _DAT_ram_19a0;
    if (_DAT_ram_1be8 < uVar3) {
      _DAT_ram_1bec = _DAT_ram_1bec & 0xfffe;
    }
  }
  return uVar2;
}





// Function: FUN_rom_006f0c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_006f0c(void)

{
  ushort uVar1;
  ulong uVar2;
  uint uVar3;
  int iVar4;
  
  _DAT_ram_19ae = _DAT_ram_1e16;
  if (_DAT_ram_1e16 != 0) {
    _DAT_ram_19ae =
         ((int)((ulong)_DAT_ram_1e16 * 0x4a7 >> 0x10) << 8 |
         (uint)((ulong)_DAT_ram_1e16 * 0x4a7 >> 8) & 0xff) + 1;
  }
  _DAT_ram_19a6 = 0;
  uVar3 = 800;
  if (800 < _DAT_ram_1bd4) {
    uVar1 = Peripherals::OC2.OC2RS;
    iVar4 = (int)((ulong)uVar1 * 0x20c4 >> 0x10);
    _DAT_ram_19a6 = iVar4 + 1;
    uVar3 = 0xfe;
    if (0xfd < _DAT_ram_19a6 && iVar4 != 0xfd) {
      _DAT_ram_19a6 = 0xff;
      uVar3 = _DAT_ram_19a6;
    }
  }
  if (_DAT_ram_1d16 == 0) {
    _DAT_ram_19ac = _DAT_ram_1d16;
  }
  else {
    _DAT_ram_19ac =
         ((int)((ulong)_DAT_ram_1d16 * 0x2f1 >> 0x10) << 8 |
         (uint)((ulong)_DAT_ram_1d16 * 0x2f1 >> 8) & 0xff) + 1;
    uVar3 = _DAT_ram_19ac;
  }
  if (_DAT_ram_1e12 == 0) {
    _DAT_ram_19aa = _DAT_ram_1e12;
  }
  else {
    _DAT_ram_19aa =
         ((int)((ulong)_DAT_ram_1e12 * 0x2f1 >> 0x10) << 8 |
         (uint)((ulong)_DAT_ram_1e12 * 0x2f1 >> 8) & 0xff) + 1;
    uVar3 = _DAT_ram_19aa;
  }
  if (_DAT_ram_1e46 != 0) {
    uVar2 = (ulong)_DAT_ram_1e46 * 0x3a8 >> 8;
    _DAT_ram_19a8 = (int)uVar2 + 1;
    return CONCAT22((int)(uVar2 >> 0x10),_DAT_ram_19a8);
  }
  _DAT_ram_19a8 = _DAT_ram_1e46;
  return CONCAT22(_DAT_ram_1e46,uVar3);
}





// Function: FUN_rom_006f98

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_006f98(void)

{
  byte bVar1;
  uint uVar2;
  undefined1 *puVar3;
  uint uVar4;
  undefined1 *puVar5;
  
  if ((_DAT_ram_1bf2 & 0x402) == 2) {
    if (_DAT_ram_1934 == 1) {
      uVar2 = _DAT_ram_1be8 - 0xaa0;
      uVar4 = 800;
LAB_rom_006fc6:
      if (uVar4 < uVar2) goto LAB_rom_006fca;
    }
    else {
      if (_DAT_ram_1934 == 2) {
        uVar2 = _DAT_ram_1be8 - 0xd20;
        uVar4 = 0x400;
        goto LAB_rom_006fc6;
      }
      if (_DAT_ram_1934 == 3) {
        uVar2 = _DAT_ram_1be8 + 0xea20;
        uVar4 = 0x10e0;
        goto LAB_rom_006fc6;
      }
LAB_rom_006fca:
      _DAT_ram_1934 = 0;
    }
    if ((_DAT_ram_1934 == 0) && (0xb3f < _DAT_ram_1be8)) {
      if (_DAT_ram_1be8 < 0xd81) {
        _DAT_ram_1934 = 1;
      }
      else if (_DAT_ram_1be8 < 0x1081) {
        _DAT_ram_1934 = 2;
      }
      else if ((0x167f < _DAT_ram_1be8) && (_DAT_ram_1be8 < 0x2621)) {
        _DAT_ram_1934 = 3;
      }
    }
  }
  else {
    if ((_DAT_ram_1bf2 & 0x402) == 0x402) {
      if (_DAT_ram_1934 == 2) {
        puVar3 = (undefined1 *)(_DAT_ram_1be8 - 0xe60);
        puVar5 = &DAT_ram_19a0;
LAB_rom_00701a:
        if (puVar5 < puVar3) {
          _DAT_ram_1934 = 0;
        }
      }
      else if (_DAT_ram_1934 == 3) {
        puVar3 = (undefined1 *)(_DAT_ram_1be8 + 0xda80);
        puVar5 = (undefined1 *)0xfa0;
        goto LAB_rom_00701a;
      }
      if ((_DAT_ram_1934 == 0) && (0xeff < _DAT_ram_1be8)) {
        if (_DAT_ram_1be8 < 0x26c1) {
          _DAT_ram_1934 = 2;
        }
        else if (_DAT_ram_1be8 < 0x3481) {
          _DAT_ram_1934 = 3;
        }
      }
      bVar1 = (byte)_DAT_ram_1934 | 4;
      goto LAB_rom_00704e;
    }
    _DAT_ram_1934 = 0;
  }
  bVar1 = (byte)_DAT_ram_1934;
LAB_rom_00704e:
  _DAT_ram_1bf2 = CONCAT11(bVar1,DAT_ram_1bf2);
  return;
}





// Function: FUN_rom_007052

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_007052(void)

{
  byte bVar1;
  ushort uVar2;
  
  bVar1 = Peripherals::GPIOD.LATD._0_1_;
  if (((bVar1 & 8) == 0) && ((_DAT_ram_1e1c & 2) != 0)) {
    uVar2 = Peripherals::GPIOD.LATD;
    Peripherals::GPIOD.LATD = uVar2 | 4;
    return;
  }
  uVar2 = Peripherals::GPIOD.LATD;
  Peripherals::GPIOD.LATD = uVar2 & 0xfffb;
  return;
}





// Function: FUN_rom_007064

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_007064(undefined2 param_1,uint param_2)

{
  uint uVar1;
  
  uVar1 = _DAT_ram_1bd2;
  if ((_DAT_ram_1bf2 & 2) == 0) {
    _DAT_ram_1bd2 = 0;
    _DAT_ram_1bc8 = 0;
  }
  else {
    if (_DAT_ram_1bc8 <= _DAT_ram_1be4) {
      _DAT_ram_1bc8 = _DAT_ram_1be4;
    }
    param_2 = _DAT_ram_1be6;
    if (_DAT_ram_1bd2 <= _DAT_ram_1be6) {
      _DAT_ram_1bd2 = _DAT_ram_1be6;
      return CONCAT22(_DAT_ram_1be6,uVar1);
    }
  }
  return CONCAT22(param_2,_DAT_ram_1bd2);
}





// Function: FUN_rom_007086

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_007086(undefined2 param_1,uint param_2)

{
  uint uVar1;
  
  uVar1 = _DAT_ram_1bd0;
  if ((_DAT_ram_1bf2 & 1) == 0) {
    _DAT_ram_1bd0 = 0;
  }
  else {
    param_2 = _DAT_ram_1bde;
    if (_DAT_ram_1bd0 <= _DAT_ram_1bde) {
      _DAT_ram_1bd0 = _DAT_ram_1bde;
      return CONCAT22(_DAT_ram_1bde,uVar1);
    }
  }
  return CONCAT22(param_2,_DAT_ram_1bd0);
}





// Function: FUN_rom_00709c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_00709c(undefined2 param_1,uint param_2)

{
  uint uVar1;
  
  uVar1 = _DAT_ram_1bce;
  if ((_DAT_ram_1bf2 & 1) == 0) {
    _DAT_ram_1bce = 0;
  }
  else {
    param_2 = _DAT_ram_1bc0;
    if (_DAT_ram_1bce <= _DAT_ram_1bc0) {
      _DAT_ram_1bce = _DAT_ram_1bc0;
      return CONCAT22(_DAT_ram_1bc0,uVar1);
    }
  }
  return CONCAT22(param_2,_DAT_ram_1bce);
}





// Function: FUN_rom_0070b2

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_rom_0070b2(uint param_1,uint param_2,uint param_3,uint param_4)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  
  iVar3 = _DAT_ram_194c;
  uVar2 = _DAT_ram_194a;
  uVar4 = _DAT_ram_1936;
  uVar5 = _DAT_ram_1938;
  if ((_DAT_ram_192a & 1) == 0) {
    if ((_DAT_ram_1bf2 & 2) == 0) {
      _DAT_ram_1936 = 0;
      _DAT_ram_1938 = 0;
      _DAT_ram_194a = 0;
      _DAT_ram_194c = 0;
      param_1 = 0;
      _DAT_ram_19a4 = 0;
      uVar4 = _DAT_ram_1936;
      uVar5 = _DAT_ram_1938;
    }
    else {
      param_1 = _DAT_ram_1936 + 1;
      param_2 = _DAT_ram_1938 + (0xfffe < _DAT_ram_1936);
      param_3 = _DAT_ram_1936;
      param_4 = _DAT_ram_1938;
      uVar4 = param_1;
      uVar5 = param_2;
      if (((param_1 < 0x9c3) <= param_2 && (_DAT_ram_1936 != 0x9c2 || param_2 != param_1 < 0x9c3))
         && (uVar4 = param_1, uVar5 = param_2, _DAT_ram_1940 == 0)) {
        param_2 = _DAT_ram_1938 - (_DAT_ram_1936 < 0x9c3);
        param_1 = _DAT_ram_19a4;
        uVar4 = _DAT_ram_1936 - 0x9c3;
        uVar5 = param_2;
        if (_DAT_ram_19a4 != 0xffff) {
          _DAT_ram_19a4 = _DAT_ram_19a4 + 1;
          iVar1 = _DAT_ram_194a + _DAT_ram_1be4;
          _DAT_ram_194c = _DAT_ram_194c + (uint)CARRY2(_DAT_ram_194a,_DAT_ram_1be4);
          _DAT_ram_194a = iVar1;
          _DAT_ram_1936 = _DAT_ram_1936 - 0x9c3;
          _DAT_ram_1938 = param_2;
          return CONCAT26(_DAT_ram_194c,CONCAT24(iVar1,CONCAT22(iVar3,uVar2)));
        }
      }
    }
  }
  _DAT_ram_1938 = uVar5;
  _DAT_ram_1936 = uVar4;
  return CONCAT26(param_2,CONCAT24(param_1,CONCAT22(param_4,param_3)));
}





// Function: FUN_rom_007106

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_007106(void)

{
  ushort uVar1;
  byte bVar2;
  uint uVar3;
  ushort uVar4;
  
  bVar2 = Peripherals::I2C2.I2C2STAT._0_1_;
  if ((bVar2 & 8) == 0) {
    bVar2 = Peripherals::I2C2.I2C2STAT._0_1_;
    if ((bVar2 & 0x10) != 0) {
      bVar2 = Peripherals::I2C2.I2C2STAT._0_1_;
      if ((bVar2 & 4) != 0) {
        _DAT_ram_194e = _DAT_ram_194e & 0xff7f;
      }
    }
  }
  else {
    _DAT_ram_194e = _DAT_ram_194e | 0x80;
  }
  bVar2 = Peripherals::GPIOG.PORTG._0_1_;
  if ((bVar2 & 4) != 0) {
    bVar2 = Peripherals::GPIOG.PORTG._0_1_;
    if ((bVar2 & 8) != 0) {
      if ((_DAT_ram_194e & 0x80) == 0) {
        _DAT_ram_193a = 0;
        goto LAB_rom_007136;
      }
    }
  }
  _DAT_ram_193a = _DAT_ram_193a + 1;
LAB_rom_007136:
  uVar3 = _DAT_ram_193a;
  uVar4 = 9999;
  if (9999 < _DAT_ram_193a) {
    _DAT_ram_193a = 0;
    _DAT_ram_194e = _DAT_ram_194e & 0xff7f;
    uVar4 = Peripherals::INTERRUPT.IEC3;
    Peripherals::INTERRUPT.IEC3 = uVar4 & 0xfffd;
    Peripherals::I2C2.I2C2CON = 0;
    Peripherals::I2C2.I2C2CON = 0x9040;
    uVar4 = Peripherals::I2C2.I2C2RCV;
    uVar1 = Peripherals::GPIOF.TRISF;
    Peripherals::GPIOF.TRISF = uVar1 | 0x10;
    uVar1 = Peripherals::GPIOF.TRISF;
    Peripherals::GPIOF.TRISF = uVar1 | 0x20;
    _DAT_ram_1e40 = 0;
    _DAT_ram_1e3e = 0;
    Peripherals::INTERRUPT.IFS3 = 0;
    uVar1 = Peripherals::INTERRUPT.IEC3;
    Peripherals::INTERRUPT.IEC3 = uVar1 | 2;
  }
  return CONCAT22(uVar3,uVar4);
}





// Function: FUN_rom_00715a

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_00715a(void)

{
  char cVar1;
  
  if ((_DAT_ram_1bf0 & 0x4e) == 0) {
    _DAT_ram_1bf0 = _DAT_ram_1bf0 & 0xfffe;
  }
  else if ((((_DAT_ram_1bea & 0x20) != 0) || ((_DAT_ram_1bf0 & 0x40) == 0)) &&
          (cVar1 = Peripherals::PWM2.IOCON2._1_1_, cVar1 < '\0')) {
    _DAT_ram_1bf0 = _DAT_ram_1bf0 | 1;
  }
  if ((_DAT_ram_1e1c & 0x6000) != 0) {
    _DAT_ram_1bf0 = _DAT_ram_1bf0 | 1;
  }
  return CONCAT22(0x6000,_DAT_ram_1e1c & 0x6000);
}





// Function: FUN_rom_00717e

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int FUN_rom_00717e(void)

{
  uint uVar1;
  
  uVar1 = _DAT_ram_1262;
  _DAT_ram_1bea = _DAT_ram_1bea | 4;
  _DAT_ram_1262 = _DAT_ram_1262 & 0xfffb;
  if (_DAT_ram_1268 == 3) {
    _DAT_ram_1262 = _DAT_ram_1262 | 2;
    return _DAT_ram_1268;
  }
  if (_DAT_ram_1268 == 4) {
    _DAT_ram_1262 = uVar1 & 0xfff9;
  }
  return _DAT_ram_1268;
}





// Function: FUN_rom_007194

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_007194(void)

{
  ushort uVar1;
  uint uVar2;
  
  if (((_DAT_ram_1bf2 & 2) != 0) && (_DAT_ram_1cc0 == 0)) {
    if (((_DAT_ram_194e & 1) == 0) ||
       (((_DAT_ram_1e1c & 8) != 0 || (uVar2 = _DAT_ram_194e, (_DAT_ram_1bea & 1) == 0)))) {
      if (((_DAT_ram_194e & 2) == 0) || ((_DAT_ram_1bea & 2) == 0)) {
        if ((_DAT_ram_1bea & 4) == 0) goto LAB_rom_0071f8;
        if ((_DAT_ram_1262 & 2) == 0) {
          if ((_DAT_ram_1e18 & 0x800) == 0) goto LAB_rom_0071d6;
        }
        else if ((_DAT_ram_1e18 & 0x800) != 0) {
LAB_rom_0071d6:
          _DAT_ram_1262 = _DAT_ram_1262 | 4;
        }
        if ((_DAT_ram_1262 & 4) == 0) goto LAB_rom_0071f4;
        if ((_DAT_ram_1262 & 2) == 0) {
          if ((_DAT_ram_1e18 & 0x800) == 0) goto LAB_rom_0071f4;
        }
        else if ((_DAT_ram_1e18 & 0x800) != 0) goto LAB_rom_0071f4;
        _DAT_ram_1bea = _DAT_ram_1bea & 0xfffb;
        goto LAB_rom_0071f0;
      }
      uVar2 = _DAT_ram_1e22;
      if ((_DAT_ram_1e22 != 2) && (uVar2 = 0, (_DAT_ram_1bec & 0xc) != 0)) goto LAB_rom_0071f0;
    }
    FUN_rom_00717e(uVar2);
  }
LAB_rom_0071f0:
  if ((_DAT_ram_1bea & 4) == 0) {
LAB_rom_0071f8:
    uVar1 = Peripherals::GPIOE.LATE;
    Peripherals::GPIOE.LATE = uVar1 & 0xff7f;
    return;
  }
LAB_rom_0071f4:
  uVar1 = Peripherals::GPIOE.LATE;
  Peripherals::GPIOE.LATE = uVar1 | 0x80;
  return;
}





// Function: FUN_rom_0071fc

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_0071fc(void)

{
  byte bVar1;
  uint uVar2;
  uint uVar3;
  bool bVar4;
  
  _DAT_ram_1952 = 0x5dc;
  if (_DAT_ram_1e22 == 2) {
    if ((_DAT_ram_194e & 4) == 0) {
      _DAT_ram_194e = _DAT_ram_194e | 4;
LAB_rom_007216:
      _DAT_ram_197c = 0x5dc;
    }
  }
  else if ((_DAT_ram_194e & 4) != 0) {
    _DAT_ram_194e = _DAT_ram_194e & 0xfffb;
    goto LAB_rom_007216;
  }
  if (((_DAT_ram_1e18 & 1) != 0) || (uVar3 = _DAT_ram_1bb2, _DAT_ram_1bb2 <= _DAT_ram_1bb0)) {
    uVar3 = _DAT_ram_1bb0 + 0x96;
  }
  if (((_DAT_ram_1e1a & 0x2000) != 0) && (uVar3 < 0x2ee1)) {
    uVar3 = 12000;
  }
  if ((_DAT_ram_1bd4 < 200) || (bVar1 = Peripherals::GPIOD.LATD._0_1_, (bVar1 & 0x20) == 0))
  goto LAB_rom_00728c;
  if (_DAT_ram_1bd4 < uVar3) {
    if (uVar3 - _DAT_ram_1bd4 < 1000) {
      uVar3 = _DAT_ram_193c + 1;
      bVar4 = _DAT_ram_193c == 0x30;
      _DAT_ram_193c = uVar3;
      if (uVar3 < 0x31 || bVar4) goto LAB_rom_00728c;
    }
    else {
      uVar3 = _DAT_ram_193c + 1;
      bVar4 = _DAT_ram_193c == 8;
      _DAT_ram_193c = uVar3;
      if (uVar3 < 9 || bVar4) goto LAB_rom_00728c;
    }
    _DAT_ram_193c = 0;
    _DAT_ram_1e3c = _DAT_ram_1e3c + 1;
  }
  else {
    if (_DAT_ram_1bd4 - uVar3 < 1000) {
      uVar3 = _DAT_ram_193c + 1;
      bVar4 = _DAT_ram_193c == 0x30;
      _DAT_ram_193c = uVar3;
      if (uVar3 < 0x31 || bVar4) goto LAB_rom_00728c;
    }
    else {
      uVar3 = _DAT_ram_193c + 1;
      bVar4 = _DAT_ram_193c == 8;
      _DAT_ram_193c = uVar3;
      if (uVar3 < 9 || bVar4) goto LAB_rom_00728c;
    }
    _DAT_ram_193c = 0;
    _DAT_ram_1e3c = _DAT_ram_1e3c - 1;
  }
LAB_rom_00728c:
  FUN_rom_009f12(&DAT_ram_1e3c);
  uVar2 = _DAT_ram_1e3c;
  uVar3 = 2000;
  if (2000 < _DAT_ram_1e3c) {
    uVar3 = 0x7d1;
    _DAT_ram_1e3c = uVar3;
  }
  return CONCAT22(uVar2,uVar3);
}





// Function: FUN_rom_0072a0

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_0072a0(undefined2 param_1,undefined2 param_2)

{
  uint uVar1;
  uint uVar2;
  
  if ((_DAT_ram_1bea & 8) == 0) {
    _DAT_ram_1bf2 = _DAT_ram_1bf2 & 0xff7f;
  }
  else if ((_DAT_ram_1e18 & 1) == 0) {
    uVar1 = _DAT_ram_1e54;
    uVar2 = _DAT_ram_1e3c;
    if (_DAT_ram_1e54 <= _DAT_ram_1e3c) {
      uVar1 = _DAT_ram_1bb2;
      uVar2 = _DAT_ram_1bb0;
      if (_DAT_ram_1bb0 < _DAT_ram_1bb2) {
        _DAT_ram_1bf2 = _DAT_ram_1bf2 & 0xff7f;
        return CONCAT22(_DAT_ram_1bb0,_DAT_ram_1bb2);
      }
    }
    _DAT_ram_1bf2 = _DAT_ram_1bf2 | 0x80;
    return CONCAT22(uVar2,uVar1);
  }
  return CONCAT22(param_2,param_1);
}





// Function: FUN_rom_0072d6

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_0072d6(void)

{
  if ((((_DAT_ram_1bf0 & 1) != 0) || ((_DAT_ram_1e1a & 0xa0) != 0)) ||
     (DAT_ram_1bef = 0, (_DAT_ram_1e20 & 0x5c0) != 0)) {
    DAT_ram_1bef = 0x80;
  }
  if ((_DAT_ram_1bf0 & 2) != 0) {
    DAT_ram_1bee = 1;
    return;
  }
  if ((_DAT_ram_1e20 & 0x80) != 0) {
    DAT_ram_1bee = 2;
    return;
  }
  if ((_DAT_ram_1bf0 & 0x100) == 0) {
    if ((_DAT_ram_1bf0 & 0x200) == 0) {
      if ((_DAT_ram_1e1a & 0x20) != 0) goto LAB_rom_00730a;
      if ((_DAT_ram_1e1a & 0x80) == 0) {
        if ((_DAT_ram_1bf0 & 0x1000) != 0) {
          DAT_ram_1bee = 7;
          return;
        }
        if ((_DAT_ram_1bf0 & 0x2000) != 0) {
          DAT_ram_1bee = 8;
          return;
        }
        if ((_DAT_ram_1bf0 & 0x40) != 0) {
          DAT_ram_1bee = 0xe;
          return;
        }
        if (_DAT_ram_1e22 == 5) {
          DAT_ram_1bee = 0x14;
          return;
        }
        if ((_DAT_ram_1e20 & 0x540) != 0) {
          DAT_ram_1bee = 0xf;
          return;
        }
        if ((_DAT_ram_1bf0 & 0x10) == 0) {
          DAT_ram_1bee = 0;
          return;
        }
        DAT_ram_1bee = 0xb;
        return;
      }
    }
    DAT_ram_1bee = 5;
  }
  else {
LAB_rom_00730a:
    DAT_ram_1bee = 4;
  }
  DAT_ram_1bef = DAT_ram_1bef;
  return;
}





// Function: FUN_rom_007346

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_007346(void)

{
  if (_DAT_ram_193e != _DAT_ram_1bec) {
    if (((_DAT_ram_193e & 0x10) != (_DAT_ram_1bec & 0x10)) && ((_DAT_ram_1bec & 0x10) != 0)) {
      if (DAT_ram_1946 != -1) {
        DAT_ram_1946 = DAT_ram_1946 + '\x01';
      }
    }
    if (((_DAT_ram_193e & 0x20) != (_DAT_ram_1bec & 0x20)) && ((_DAT_ram_1bec & 0x20) != 0)) {
      if (DAT_ram_1947 != -1) {
        DAT_ram_1947 = DAT_ram_1947 + '\x01';
      }
    }
    if (((_DAT_ram_193e & 0x40) != (_DAT_ram_1bec & 0x40)) && ((_DAT_ram_1bec & 0x40) != 0)) {
      if (DAT_ram_1944 != -1) {
        DAT_ram_1944 = DAT_ram_1944 + '\x01';
      }
    }
    if (((_DAT_ram_193e & 0x80) != (_DAT_ram_1bec & 0x80)) && ((_DAT_ram_1bec & 0x80) != 0)) {
      if (DAT_ram_1945 != -1) {
        DAT_ram_1945 = DAT_ram_1945 + '\x01';
      }
    }
    if (((_DAT_ram_193e & 0x100) != (_DAT_ram_1bec & 0x100)) && ((_DAT_ram_1bec & 0x100) != 0)) {
      if (DAT_ram_1942 != -1) {
        DAT_ram_1942 = DAT_ram_1942 + '\x01';
      }
    }
    _DAT_ram_193e = _DAT_ram_1bec;
  }
  return;
}





// Function: FUN_rom_0073ce

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ushort FUN_rom_0073ce(void)

{
  undefined1 uVar1;
  byte bVar2;
  char cVar3;
  uint uVar4;
  int iVar5;
  ushort uVar6;
  uint uVar7;
  byte *pbVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  byte *pbVar11;
  bool bVar12;
  
  if (_DAT_ram_1e40 - 5 < 0x18 || _DAT_ram_1e40 == 0x1d) {
    uVar7 = 1;
    uVar4 = 2;
    do {
      if (_DAT_ram_1e40 == uVar4 + 3) {
        uVar7 = 0;
      }
      bVar12 = uVar4 != 0x1a;
      uVar4 = uVar4 + 3;
    } while (bVar12);
    _DAT_ram_1950 = _DAT_ram_1950 & 0xfffe | uVar7;
    if (uVar7 == 0) {
      pbVar8 = &DAT_ram_dfb8;
      do {
        bVar2 = (&DAT_ram_1958)[*pbVar8];
        if (bVar2 < 0x18) {
          if ((0x15 < bVar2) || ((4 < bVar2 && ((bVar2 < 8 || (bVar2 == 0x13))))))
          goto LAB_rom_00742a;
LAB_rom_00742e:
          iVar5 = 1;
        }
        else {
          if ((bVar2 < 0x23) || ((0x24 < bVar2 && (0xc < (byte)(bVar2 - 0x30) && bVar2 != 0x3d))))
          goto LAB_rom_00742e;
LAB_rom_00742a:
          iVar5 = 0;
        }
        _DAT_ram_1950 = _DAT_ram_1950 & 0xfffd | iVar5 * 2;
      } while ((*pbVar8 + 4 < _DAT_ram_1e40) && (pbVar8 = pbVar8 + 1, iVar5 == 0));
    }
  }
  else if (_DAT_ram_1e40 < 0x1e) {
    if (_DAT_ram_1e40 != 0) {
      _DAT_ram_1950 = _DAT_ram_1950 | 1;
    }
  }
  else {
    _DAT_ram_1950 = _DAT_ram_1950 | 2;
  }
  uVar6 = FUN_rom_006216(_DAT_ram_1950);
  uVar4 = _DAT_ram_1e40;
  if ((char)uVar6 != '\0') {
    uVar9 = 0xdfb80000;
    do {
      uVar1 = *(undefined1 *)((ulong)uVar9 >> 0x10);
      uVar10 = CONCAT31((int3)((ulong)uVar9 >> 8),uVar1);
      cVar3 = FUN_rom_00644a(uVar1);
      if (cVar3 != '\0') {
        _DAT_ram_1950 = _DAT_ram_1950 | 2;
      }
      uVar9 = CONCAT22((int)((ulong)uVar10 >> 0x10) + 1,(uint)uVar10);
    } while (((uint)uVar10 & 0xff) + 4 < uVar4);
    uVar6 = 0xce;
    if ((_DAT_ram_1950 & 2) == 0) {
      pbVar8 = &DAT_ram_dfb8;
      do {
        pbVar11 = pbVar8 + 2;
        uVar4 = (uint)*pbVar8;
        FUN_rom_0062bc(*pbVar8);
        pbVar8 = pbVar11;
      } while (uVar4 + 4 < _DAT_ram_1e40);
      uVar6 = 0x31;
    }
    Peripherals::I2C2.I2C2TRN = uVar6;
    _DAT_ram_1950 = _DAT_ram_1950 | 2;
  }
  return uVar6;
}





// Function: FUN_rom_00749a

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* WARNING: Restarted to delay deadcode elimination for space: ram */

ushort FUN_rom_00749a(void)

{
  uint uVar1;
  ushort uVar2;
  
  if (_DAT_ram_1e40 != 4) {
    if (_DAT_ram_1e40 < 5) {
      if (_DAT_ram_1e40 != 0) {
        _DAT_ram_1950 = _DAT_ram_1950 | 1;
      }
    }
    else {
      _DAT_ram_1950 = _DAT_ram_1950 | 2;
    }
  }
  uVar2 = FUN_rom_006216(_DAT_ram_1950);
  if ((char)uVar2 != '\0') {
    uVar2 = (ushort)DAT_ram_1959;
    if (DAT_ram_1959 == 1) {
      if (0x17 < _DAT_ram_1e3e) {
        _DAT_ram_1950 = _DAT_ram_1950 | 2;
        Peripherals::I2C2.I2C2TRN = -_DAT_ram_1956 & 0xffU;
        return -_DAT_ram_1956 & 0xffU;
      }
      if (DAT_ram_195a == 0) {
        uVar1 = FUN_rom_008ec0();
        Peripherals::I2C2.I2C2TRN = uVar1 & 0xff;
      }
      else {
        Peripherals::I2C2.I2C2TRN = 0;
      }
      uVar2 = Peripherals::I2C2.I2C2TRN;
      _DAT_ram_1956 = _DAT_ram_1956 + uVar2;
      return uVar2;
    }
    if (DAT_ram_1959 == 0) {
      if (0x15 < _DAT_ram_1e3e) {
        _DAT_ram_1950 = _DAT_ram_1950 | 2;
        Peripherals::I2C2.I2C2TRN = -_DAT_ram_1956 & 0xffU;
        return -_DAT_ram_1956 & 0xffU;
      }
      if (DAT_ram_195a == 0) {
        uVar1 = FUN_rom_008eb0();
        Peripherals::I2C2.I2C2TRN = uVar1 & 0xff;
      }
      else {
        Peripherals::I2C2.I2C2TRN = 0;
      }
      uVar2 = Peripherals::I2C2.I2C2TRN;
      _DAT_ram_1956 = _DAT_ram_1956 + uVar2;
      return uVar2;
    }
    if ((DAT_ram_1959 == 2) || (DAT_ram_1959 == 3)) {
      if (DAT_ram_1959 == 2) {
        if (_DAT_ram_1e3e < 0x1c) {
LAB_rom_006ac8:
          if (DAT_ram_195a < 0xb) {
            uVar1 = FUN_rom_008eb8(_DAT_ram_1e3e);
            Peripherals::I2C2.I2C2TRN = uVar1 & 0xff;
          }
          else {
            Peripherals::I2C2.I2C2TRN = 0;
          }
          uVar2 = Peripherals::I2C2.I2C2TRN;
          _DAT_ram_1956 = _DAT_ram_1956 + uVar2;
          return uVar2;
        }
      }
      else if ((DAT_ram_1959 == 3) && (_DAT_ram_1e3e < 0x1d)) goto LAB_rom_006ac8;
      _DAT_ram_1950 = _DAT_ram_1950 | 2;
      Peripherals::I2C2.I2C2TRN = -_DAT_ram_1956 & 0xffU;
      return -_DAT_ram_1956 & 0xffU;
    }
    Peripherals::I2C2.I2C2TRN = 0;
  }
  return uVar2;
}





// Function: FUN_rom_0074d6

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ushort FUN_rom_0074d6(void)

{
  uint uVar1;
  ushort uVar2;
  
  if (_DAT_ram_1e40 != 0) {
    _DAT_ram_1956 = 0;
    _DAT_ram_1950 = 0;
    uVar1 = FUN_rom_0061ea(&DAT_ram_1958);
    _DAT_ram_1950 = _DAT_ram_1950 & 0xfffb | (uVar1 & 1) << 2;
  }
  uVar2 = 0xfa;
  if ((_DAT_ram_1950 & 4) == 0) {
    if (DAT_ram_1958 == 0x24) {
      uVar2 = FUN_rom_006912();
      goto LAB_rom_0075aa;
    }
    if (DAT_ram_1958 < 0x24) {
      if (DAT_ram_1958 == 0x12) {
        uVar2 = FUN_rom_006b90();
        goto LAB_rom_0075aa;
      }
      if (DAT_ram_1958 < 0x12) {
        if (DAT_ram_1958 == 1) {
          uVar2 = FUN_rom_0073ce();
          goto LAB_rom_0075aa;
        }
        if (DAT_ram_1958 == 0) {
          uVar2 = FUN_rom_006252();
          goto LAB_rom_0075aa;
        }
        if (DAT_ram_1958 == 0x11) {
          uVar2 = FUN_rom_006b54();
          goto LAB_rom_0075aa;
        }
      }
      else {
        if (DAT_ram_1958 == 0x21) {
          uVar2 = FUN_rom_006b18();
          goto LAB_rom_0075aa;
        }
        if (DAT_ram_1958 < 0x21) {
          if (DAT_ram_1958 == 0x20) {
            uVar2 = FUN_rom_00749a();
            goto LAB_rom_0075aa;
          }
        }
        else {
          if (DAT_ram_1958 == 0x22) {
            uVar2 = FUN_rom_006582();
            goto LAB_rom_0075aa;
          }
          if (DAT_ram_1958 == 0x23) {
            uVar2 = FUN_rom_006612();
            goto LAB_rom_0075aa;
          }
        }
      }
    }
    else {
      if (DAT_ram_1958 == 0x3d) {
        uVar2 = FUN_rom_006894();
        goto LAB_rom_0075aa;
      }
      if (DAT_ram_1958 < 0x3d) {
        if (DAT_ram_1958 == 0x3b) {
          uVar2 = FUN_rom_006848();
          goto LAB_rom_0075aa;
        }
        if (0x3a < DAT_ram_1958) {
          uVar2 = FUN_rom_00672a();
          goto LAB_rom_0075aa;
        }
        if (DAT_ram_1958 == 0x3a) {
          uVar2 = FUN_rom_00676a();
          goto LAB_rom_0075aa;
        }
      }
      else {
        if (DAT_ram_1958 == 0xe2) {
          uVar2 = FUN_rom_00695e();
          goto LAB_rom_0075aa;
        }
        if (DAT_ram_1958 < 0xe2) {
          if (DAT_ram_1958 == 0xe0) {
            uVar2 = FUN_rom_0068d4();
            goto LAB_rom_0075aa;
          }
        }
        else {
          if (DAT_ram_1958 == 0xe3) {
            uVar2 = FUN_rom_0069c8();
            goto LAB_rom_0075aa;
          }
          if (DAT_ram_1958 == 0xff) {
            uVar2 = FUN_rom_006692();
            goto LAB_rom_0075aa;
          }
        }
      }
    }
    uVar2 = 0x55;
  }
  Peripherals::I2C2.I2C2TRN = uVar2;
LAB_rom_0075aa:
  _DAT_ram_1e40 = 0;
  _DAT_ram_1e3e = _DAT_ram_1e3e + 1;
  return uVar2;
}





// Function: FUN_rom_0075fa

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined2 FUN_rom_0075fa(void)

{
  undefined2 uVar1;
  
  FUN_rom_007052();
  FUN_rom_006f0c();
  FUN_rom_007346();
  FUN_rom_0071fc();
  FUN_rom_0072a0();
  FUN_rom_006f98();
  uVar1 = 0x5db;
  if (_DAT_ram_197c < 0x5dc) {
    uVar1 = 0x5dc;
    _DAT_ram_1bb0 = 0x5dc;
  }
  else {
    _DAT_ram_1bb0 = _DAT_ram_197c;
  }
  if ((_DAT_ram_192a & 1) == 0) {
    FUN_rom_006bd6();
    FUN_rom_006c42();
    FUN_rom_006e24();
    FUN_rom_006d5a();
    FUN_rom_006dfe();
    FUN_rom_007064();
    FUN_rom_007086();
    FUN_rom_00709c();
    FUN_rom_00715a();
    uVar1 = FUN_rom_007194();
  }
  return uVar1;
}





// Function: FUN_rom_007630

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* WARNING: Restarted to delay deadcode elimination for space: ram */

void FUN_rom_007630(void)

{
  if ((_DAT_ram_125a & 0x40) != 0) {
    _DAT_ram_125a = _DAT_ram_125a & 0xffbf;
    if ((((DAT_ram_15ee == 0) && (DAT_ram_15ef == ' ')) && (DAT_ram_15f0 == 0)) &&
       (DAT_ram_15f1 == 0)) {
      _DAT_ram_1d24 = 0;
    }
    else {
      _DAT_ram_1d24 = ((int)((uint)DAT_ram_15f1 * 0x100 + (uint)DAT_ram_15f0) / 0x7a & 0xffU) + 1;
    }
    _DAT_ram_1e44 = CONCAT11(0x15,DAT_ram_15f3) * 0x100 + (uint)DAT_ram_15f2;
    if (_DAT_ram_1e44 - 0x321 < 0x9e || _DAT_ram_1e44 == 0x3bf) {
      Peripherals::PWM5.PDC5 = _DAT_ram_1e44;
    }
    _DAT_ram_1e42 = CONCAT11(0x15,DAT_ram_15f5) * 0x100 + (uint)DAT_ram_15f4;
    _DAT_ram_1d32 = CONCAT11(0x15,DAT_ram_15e7) * 0x100 + (uint)DAT_ram_15e6;
    _DAT_ram_1d30 = ((int)((uint)DAT_ram_15e9 * 0x100 + (uint)DAT_ram_15e8) >> 4) + 1;
    _DAT_ram_1d2e = CONCAT11(0x15,DAT_ram_15eb) * 0x100 + (uint)DAT_ram_15ea;
    _DAT_ram_1d2c = ((int)((uint)DAT_ram_15ed * 0x100 + (uint)DAT_ram_15ec) >> 4) + 1;
    _DAT_ram_1d2a = CONCAT11(0x15,DAT_ram_15f8) * 0x100 + (uint)DAT_ram_15f7;
    _DAT_ram_1d28 = ((int)((uint)DAT_ram_15fa * 0x100 + (uint)DAT_ram_15f9) >> 4) + 1;
    _DAT_ram_1d26 = CONCAT11(0x15,DAT_ram_15ef) * 0x100 + (uint)DAT_ram_15ee;
    return;
  }
  return;
}





// Function: FUN_rom_0076fc

/* WARNING: Heritage AFTER dead removal. Example location: W1 : 0x007720 */
/* WARNING: Removing unreachable block (rom,0x007718) */
/* WARNING: Removing unreachable block (rom,0x007742) */
/* WARNING: Removing unreachable block (rom,0x007748) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* WARNING: Restarted to delay deadcode elimination for space: ram */

void FUN_rom_0076fc(void)

{
  ushort uVar1;
  
  uVar1 = Peripherals::ADC.ADCBUF5;
  *(ushort *)(&DAT_ram_0880 + _DAT_ram_1252 * 2) = uVar1;
  do {
  } while( true );
}





// Function: FUN_rom_007770

/* WARNING: Heritage AFTER dead removal. Example location: W1 : 0x0077ac */
/* WARNING: Removing unreachable block (rom,0x007796) */
/* WARNING: Removing unreachable block (rom,0x0077c2) */
/* WARNING: Removing unreachable block (rom,0x0077be) */
/* WARNING: Removing unreachable block (rom,0x0077c4) */
/* WARNING: Removing unreachable block (rom,0x0077e0) */
/* WARNING: Removing unreachable block (rom,0x0077dc) */
/* WARNING: Removing unreachable block (rom,0x0077e2) */
/* WARNING: Removing unreachable block (rom,0x00781c) */
/* WARNING: Removing unreachable block (rom,0x00781e) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* WARNING: Restarted to delay deadcode elimination for space: ram */

void FUN_rom_007770(void)

{
  ushort uVar1;
  
  uVar1 = Peripherals::ADC.ADCBUF4;
  *(ulong *)(&DAT_ram_110a + _DAT_ram_1252 * 4) = (ulong)uVar1;
  do {
  } while( true );
}





// Function: FUN_rom_007832

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_007832(void)

{
  ushort uVar1;
  uint uVar2;
  bool bVar3;
  
  uVar1 = Peripherals::ADC.ADCBUF11;
  _DAT_ram_1210 = _DAT_ram_1210 + uVar1;
  uVar1 = Peripherals::ADC.ADCBUF10;
  _DAT_ram_120e = _DAT_ram_120e + uVar1;
  uVar1 = Peripherals::ADC.ADCBUF1;
  _DAT_ram_120c = _DAT_ram_120c + uVar1;
  uVar2 = _DAT_ram_120a + 1;
  bVar3 = _DAT_ram_120a != 0x1e;
  _DAT_ram_120a = uVar2;
  if (0x1e < uVar2 && bVar3) {
    _DAT_ram_120a = 0;
    _DAT_ram_1e48 = _DAT_ram_1210 >> 7;
    _DAT_ram_1e4c = _DAT_ram_120e >> 7;
    _DAT_ram_1e4a = _DAT_ram_120c >> 7;
    _DAT_ram_1210 = 0;
    _DAT_ram_120e = 0;
    _DAT_ram_120c = 0;
  }
  return;
}





// Function: FUN_rom_007878

/* WARNING: Heritage AFTER dead removal. Example location: W1 : 0x007892 */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* WARNING: Restarted to delay deadcode elimination for space: ram */

void FUN_rom_007878(void)

{
  undefined4 in_W1W0;
  long lVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  
  if (_DAT_ram_1268 == 4) {
    lVar1 = (long)_DAT_ram_1d44 * 0x3b05;
  }
  else {
    _DAT_ram_1212 = 0;
    iVar4 = _DAT_ram_1d44;
    uVar2 = FUN_rom_005d00(_DAT_ram_1d44,(int)((ulong)in_W1W0 >> 0x10),0xf0a,0);
    iVar3 = (int)((ulong)uVar2 >> 0x10);
    _DAT_ram_1214 = iVar3 << 1 | (uint)uVar2 >> 0xf;
    if (_DAT_ram_1d4a != 0x6b3) goto LAB_rom_0078bc;
    lVar1 = FUN_rom_005d00(iVar4,iVar3,0x3b05,0);
  }
  _DAT_ram_1214 = (int)((ulong)lVar1 >> 0x10) << 1 | (uint)lVar1 >> 0xf;
LAB_rom_0078bc:
  if ((int)_DAT_ram_1214 < 0x4e) {
    if ((int)_DAT_ram_1214 < 0) {
      _DAT_ram_1214 = 0;
    }
  }
  else {
    _DAT_ram_1214 = 0x4d;
  }
  _DAT_ram_1d3a = _DAT_ram_1214;
  return;
}





// Function: FUN_rom_0078d6

/* WARNING: Heritage AFTER dead removal. Example location: W1 : 0x00791a */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* WARNING: Restarted to delay deadcode elimination for space: ram */

void FUN_rom_0078d6(void)

{
  uint uVar1;
  uint uVar2;
  int iVar3;
  long lVar4;
  undefined4 uVar5;
  int iVar6;
  int iVar7;
  long lVar8;
  uint uVar9;
  
  lVar4 = (long)(_DAT_ram_1da0 >> 2) * 0xc03;
  lVar8 = lVar4 + -0x60180;
  iVar7 = -((int)((ulong)lVar8 >> 0x10) * 0x40 | (uint)lVar8 >> 10);
  _DAT_ram_1d50 = iVar7 + 0x400;
  if (iVar7 == 0 || _DAT_ram_1d50 < 0x400) {
    if ((iVar7 + 0x301U < 0x8000 || !SBORROW2(_DAT_ram_1d50,0xff)) &&
        ((int)(iVar7 + 0x301U) < 1 || SBORROW2(_DAT_ram_1d50,0xff))) {
      _DAT_ram_1d50 = 0x100;
    }
  }
  else {
    _DAT_ram_1d50 = 0x400;
  }
  iVar7 = _DAT_ram_1d50;
  uVar5 = FUN_rom_005d00(_DAT_ram_1d50,(int)((ulong)lVar4 >> 0x10),12000);
  iVar3 = (int)((ulong)uVar5 >> 0x10);
  _DAT_ram_1d56 = iVar3 << 6 | (uint)uVar5 >> 10;
  _DAT_ram_1d58 = (int)_DAT_ram_1d56 >> 0xf;
  uVar9 = _DAT_ram_1d56;
  uVar5 = FUN_rom_005d00(iVar7,iVar3,0xaf0);
  iVar7 = _DAT_ram_1218;
  uVar2 = _DAT_ram_1216;
  iVar6 = (int)((ulong)uVar5 >> 0x10);
  uVar1 = iVar6 << 6 | (uint)uVar5 >> 10;
  iVar3 = _DAT_ram_1d44 - _DAT_ram_1d36;
  _DAT_ram_1d54 = uVar1;
  uVar5 = FUN_rom_005d00(iVar3,iVar6,uVar9);
  iVar6 = (int)((ulong)uVar5 >> 0x10);
  _DAT_ram_121c = (iVar6 + iVar7 + (uint)CARRY2((uint)uVar5,uVar2)) * 8 | (uint)uVar5 + uVar2 >> 0xd
  ;
  if ((int)_DAT_ram_121c < 0) {
    _DAT_ram_121a = 0;
  }
  else {
    _DAT_ram_121a = _DAT_ram_121c;
    if (4000 < (int)_DAT_ram_121c) {
      _DAT_ram_121a = 4000;
    }
  }
  lVar8 = (long)(int)(_DAT_ram_121a - _DAT_ram_121c) * (long)_DAT_ram_1d52;
  uVar9 = _DAT_ram_121a;
  uVar5 = FUN_rom_005d00(iVar3,iVar6,uVar1);
  uVar1 = (uint)uVar5 + (uint)lVar8;
  _DAT_ram_1216 = uVar1 + uVar2;
  _DAT_ram_1218 =
       (int)((ulong)uVar5 >> 0x10) + (int)((ulong)lVar8 >> 0x10) +
       (uint)CARRY2((uint)uVar5,(uint)lVar8) + iVar7 + (uint)CARRY2(uVar1,uVar2);
  if (_DAT_ram_1216 == 0 && _DAT_ram_1218 == 500 || _DAT_ram_1218 < 500) {
    if (_DAT_ram_1218 < 0) {
      _DAT_ram_1216 = 0;
      _DAT_ram_1218 = 0;
    }
  }
  else {
    _DAT_ram_1216 = 0;
    _DAT_ram_1218 = 500;
  }
  _DAT_ram_1d38 = uVar9;
  return;
}





// Function: FUN_rom_0079c6

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_0079c6(void)

{
  byte bVar1;
  uint uVar2;
  bool bVar3;
  
  bVar1 = Peripherals::GPIOD.PORTD._0_1_;
  uVar2 = CONCAT11((byte)(_DAT_ram_1e1c >> 9),(char)(_DAT_ram_1e1c >> 1)) & 0xff01;
  if ((byte)uVar2 == (bVar1 & 1)) {
    _DAT_ram_121e = 0;
  }
  else {
    uVar2 = _DAT_ram_121e + 1;
    bVar3 = _DAT_ram_121e != 1;
    _DAT_ram_121e = uVar2;
    if (1 < uVar2 && bVar3) {
      bVar1 = Peripherals::GPIOD.PORTD._0_1_;
      uVar2 = (bVar1 & 1) * 2;
      _DAT_ram_1e1c = _DAT_ram_1e1c & 0xfffd | uVar2;
      _DAT_ram_121e = 0;
      return CONCAT22(uVar2,_DAT_ram_1e1c);
    }
  }
  return CONCAT22(CONCAT11(2,bVar1 & 1),uVar2);
}





// Function: FUN_rom_0079f6

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_0079f6(void)

{
  byte bVar1;
  
  bVar1 = Peripherals::GPIOD.PORTD._1_1_;
  if ((DAT_ram_1e1d & 8) == (bVar1 & 8)) {
    _DAT_ram_1d3e = 3;
    if ((_DAT_ram_1e1c & 0x800) == 0) {
      _DAT_ram_1d3e = 0x15e;
    }
  }
  else {
    if (_DAT_ram_1d3e == 0) {
      bVar1 = Peripherals::GPIOD.PORTD._1_1_;
      _DAT_ram_1e1c = _DAT_ram_1e1c & 0xf7ff | (bVar1 >> 3 & 1) << 0xb;
      return;
    }
    _DAT_ram_1d3e = _DAT_ram_1d3e + -1;
  }
  return;
}





// Function: FUN_rom_007a2c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_007a2c(void)

{
  if ((_DAT_ram_1e22 == 0) && (_DAT_ram_1e10 != 0)) {
    _DAT_ram_1e10 = _DAT_ram_1e10 + -1;
  }
  return;
}





// Function: FUN_rom_007a3c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ulong FUN_rom_007a3c(undefined2 param_1,uint param_2)

{
  byte bVar1;
  uint uVar2;
  uint uVar3;
  
  uVar3 = _DAT_ram_1e22;
  if (_DAT_ram_1e22 == 2) {
    param_2 = _DAT_ram_1e20 & 1;
    uVar3 = _DAT_ram_1e20;
    if (param_2 == 0) {
      if (_DAT_ram_1d42 < 0x1402) {
        _DAT_ram_1220 = param_2;
        return CONCAT22(_DAT_ram_1e20,0x1401) & 0x1ffff;
      }
      uVar2 = _DAT_ram_1220 + 1;
      if (0x17 < uVar2 && _DAT_ram_1220 != 0x17) {
        _DAT_ram_1264 = _DAT_ram_1264 | 0x80;
      }
      uVar3 = 0x1781;
      if (0x1781 < _DAT_ram_1d42) {
LAB_rom_007a6a:
        _DAT_ram_1264 = _DAT_ram_1264 | 0x80;
        _DAT_ram_1220 = uVar2;
        return CONCAT22(_DAT_ram_1e20,uVar3) & 0x1ffff;
      }
      bVar1 = Peripherals::GPIOD.LATD._0_1_;
      uVar3 = CONCAT11(0x17,bVar1);
      _DAT_ram_1220 = uVar2;
      if (((bVar1 & 0x10) != 0) && ((_DAT_ram_1264 & 0x400) == 0)) goto LAB_rom_007a6a;
      goto LAB_rom_007a74;
    }
  }
  _DAT_ram_1220 = 0;
LAB_rom_007a74:
  return CONCAT22(param_2,uVar3);
}





// Function: FUN_rom_007a76

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_007a76(undefined2 param_1,uint param_2)

{
  uint uVar1;
  bool bVar2;
  
  uVar1 = _DAT_ram_1e22;
  if (_DAT_ram_1e22 == 2) {
    uVar1 = _DAT_ram_1e20;
    if ((_DAT_ram_1e20 & 1) == 0) {
      uVar1 = _DAT_ram_1268;
      if (_DAT_ram_1268 != 3) {
LAB_rom_007aa4:
        _DAT_ram_1222 = _DAT_ram_1e20 & 1;
        return CONCAT22(param_2,uVar1);
      }
      if ((_DAT_ram_1d42 <= _DAT_ram_1bb4) &&
         (uVar1 = _DAT_ram_1bb4, param_2 = _DAT_ram_1d42, _DAT_ram_1d38 < 1)) goto LAB_rom_007aa4;
      param_2 = _DAT_ram_1222 + 1;
      uVar1 = 0x225;
      bVar2 = _DAT_ram_1222 != 0x224;
      _DAT_ram_1222 = param_2;
      if (0x224 < param_2 && bVar2) {
        _DAT_ram_1264 = _DAT_ram_1264 | 0x100;
        return CONCAT22(param_2,0x225);
      }
      goto LAB_rom_007aaa;
    }
  }
  _DAT_ram_1222 = 0;
LAB_rom_007aaa:
  return CONCAT22(param_2,uVar1);
}





// Function: FUN_rom_007aac

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int FUN_rom_007aac(void)

{
  byte bVar1;
  ushort uVar2;
  int iVar3;
  bool bVar4;
  
  if (_DAT_ram_1e22 == 2) {
    if (_DAT_ram_1268 == 3) {
      uVar2 = Peripherals::ADC.ADCBUF5;
      iVar3 = 0x2eb;
      if ((0x2eb < uVar2) &&
         (iVar3 = 14999,
         0x7fff < _DAT_ram_1da4 + 0xc569U && SBORROW2(_DAT_ram_1da4,14999) ||
         0 < (int)(_DAT_ram_1da4 + 0xc569U) && !SBORROW2(_DAT_ram_1da4,14999))) {
LAB_rom_007af8:
        uVar2 = Peripherals::GPIOF.LATF;
        Peripherals::GPIOF.LATF = uVar2 | 1;
        return iVar3;
      }
    }
    else {
      iVar3 = _DAT_ram_1268;
      if (_DAT_ram_1268 != 4) goto LAB_rom_007b08;
      if (_DAT_ram_1be8 < 0x1fd1) {
        if (_DAT_ram_1be8 < 0x1f70) {
          _DAT_ram_1d5a = 0x2198;
        }
      }
      else {
        _DAT_ram_1d5a = 0x251c;
      }
      uVar2 = Peripherals::ADC.ADCBUF5;
      iVar3 = 0x16b;
      if (((0x16b < uVar2) && (iVar3 = _DAT_ram_1d5a, _DAT_ram_1d5a < _DAT_ram_1da4)) &&
         ((_DAT_ram_1264 & 0x800) != 0)) goto LAB_rom_007af8;
    }
    if (((_DAT_ram_125a & 0x10) == 0) &&
       (iVar3 = 8000,
       (_DAT_ram_1da4 + 0xe0c0U < 0x8000 || !SBORROW2(_DAT_ram_1da4,8000)) &&
       ((int)(_DAT_ram_1da4 + 0xe0c0U) < 1 || SBORROW2(_DAT_ram_1da4,8000)))) {
LAB_rom_007b08:
      uVar2 = Peripherals::GPIOF.LATF;
      Peripherals::GPIOF.LATF = uVar2 & 0xfffe;
      return iVar3;
    }
  }
  else {
    bVar1 = Peripherals::GPIOF.LATF._0_1_;
    iVar3 = CONCAT11((char)((uint)_DAT_ram_1e22 >> 8),bVar1);
    if (((bVar1 & 1) == 0) &&
       (iVar3 = _DAT_ram_1224 + 1, bVar4 = _DAT_ram_1224 != 4, _DAT_ram_1224 = iVar3,
       bVar4 && 4 < iVar3)) {
      _DAT_ram_1224 = 0;
      uVar2 = Peripherals::GPIOF.LATF;
      Peripherals::GPIOF.LATF = uVar2 | 1;
    }
  }
  return iVar3;
}





// Function: FUN_rom_007b20

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

uint FUN_rom_007b20(void)

{
  uint uVar1;
  
  uVar1 = _DAT_ram_1bb6;
  if (_DAT_ram_1bb6 <= _DAT_ram_1d42) {
    uVar1 = 0x6e;
    if (_DAT_ram_1d40 < 0x6f) {
      uVar1 = 0xc4d;
      if (_DAT_ram_1d4e != 0xc4d) {
        _DAT_ram_126a = _DAT_ram_126a | 8;
      }
    }
  }
  return uVar1;
}





// Function: FUN_rom_007b48

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_007b48(void)

{
  uint uVar1;
  byte bVar2;
  bool bVar3;
  
  bVar2 = Peripherals::GPIOD.PORTD._0_1_;
  _DAT_ram_1228 = bVar2 >> 6 & 1;
  if (_DAT_ram_1228 != _DAT_ram_122a) {
    _DAT_ram_1226 = _DAT_ram_1226 + 1;
    _DAT_ram_122a = _DAT_ram_1228;
  }
  uVar1 = _DAT_ram_122e + 1;
  bVar3 = _DAT_ram_122e != 0x1f2;
  _DAT_ram_122e = uVar1;
  if (0x1f2 < uVar1 && bVar3) {
    if (_DAT_ram_1e22 == 2) {
      bVar2 = Peripherals::GPIOD.LATD._0_1_;
      if ((bVar2 & 0x20) == 0) {
        _DAT_ram_122c = 0;
      }
      if (_DAT_ram_1226 < 5) {
        uVar1 = _DAT_ram_122c + 1;
        bVar3 = _DAT_ram_122c != 0x4e;
        _DAT_ram_122c = uVar1;
        if (0x4e < uVar1 && bVar3) {
          _DAT_ram_1e20 = _DAT_ram_1e20 | 4;
        }
      }
      else if (7 < _DAT_ram_1226) {
        _DAT_ram_122c = 0;
        _DAT_ram_1e20 = _DAT_ram_1e20 & 0xfffb;
      }
    }
    else {
      _DAT_ram_122c = 0;
    }
    _DAT_ram_1bd4 = _DAT_ram_1226 * 0x96;
    _DAT_ram_1226 = 0;
    _DAT_ram_122e = 0;
    _DAT_ram_1e1c = _DAT_ram_1e1c | 0x100;
  }
  return;
}





// Function: FUN_rom_007ba6

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

long FUN_rom_007ba6(undefined2 param_1,uint param_2)

{
  int iVar1;
  bool bVar2;
  
  if ((_DAT_ram_125a & 0x20) == 0) {
    _DAT_ram_126a = _DAT_ram_126a & 0xfbdf;
    iVar1 = 0;
    _DAT_ram_1230 = 0;
    _DAT_ram_1232 = 0;
    goto LAB_rom_007c50;
  }
  if (((_DAT_ram_1bea & 0x200) == 0) || ((_DAT_ram_1266 & 0x80) == 0)) {
LAB_rom_007bdc:
    _DAT_ram_126a = _DAT_ram_126a & 0xffef;
  }
  else if (_DAT_ram_1268 == 4) {
    if (0x3f < _DAT_ram_1bbe) goto LAB_rom_007bd0;
    param_2 = 0x402;
    if ((_DAT_ram_1bf2 & 0x402) == 2) {
      _DAT_ram_126a = _DAT_ram_126a | 0x10;
    }
  }
  else {
LAB_rom_007bd0:
    param_2 = _DAT_ram_1bbe;
    if ((0x45 < _DAT_ram_1bbe) || (_DAT_ram_1268 == 3)) goto LAB_rom_007bdc;
  }
  if ((_DAT_ram_126a & 0x10) == 0) {
    _DAT_ram_126a = _DAT_ram_126a & 0xffdf;
    _DAT_ram_1232 = 0;
  }
  else {
    param_2 = _DAT_ram_1be4;
    if (_DAT_ram_1be4 < 0xb) {
      if (_DAT_ram_1d44 < 0x1a) {
        if (_DAT_ram_1be4 < 8) {
          iVar1 = _DAT_ram_1232 + 1;
          param_2 = 1000;
          bVar2 = _DAT_ram_1232 != 999;
          _DAT_ram_1232 = iVar1;
          if (bVar2 && 999 < iVar1) {
            _DAT_ram_1232 = 1000;
            _DAT_ram_126a = _DAT_ram_126a | 0x20;
          }
        }
        goto LAB_rom_007c0e;
      }
    }
    _DAT_ram_126a = _DAT_ram_126a & 0xffdf;
    _DAT_ram_1232 = 0;
  }
LAB_rom_007c0e:
  iVar1 = _DAT_ram_1268;
  if (_DAT_ram_1268 == 3) {
    iVar1 = 0x600;
    param_2 = _DAT_ram_1264 & 0x600;
    if (param_2 == 0) {
      if (_DAT_ram_1be4 < 0xd) {
        if (_DAT_ram_1d44 < 0x1a) {
          param_2 = 0;
          iVar1 = _DAT_ram_1d44;
          if (_DAT_ram_1be4 < 10) {
            iVar1 = _DAT_ram_1230 + 1;
            param_2 = 1000;
            bVar2 = _DAT_ram_1230 != 999;
            _DAT_ram_1230 = iVar1;
            if (bVar2 && 999 < iVar1) {
              _DAT_ram_1230 = 1000;
              _DAT_ram_126a = _DAT_ram_126a | 0x400;
              return CONCAT22(1000,iVar1);
            }
          }
LAB_rom_007c50:
          return CONCAT22(param_2,iVar1);
        }
      }
      _DAT_ram_126a = _DAT_ram_126a & 0xfbff;
      _DAT_ram_1230 = param_2;
      return ((ulong)_DAT_ram_1264 & 0x600) << 0x10;
    }
  }
  _DAT_ram_126a = _DAT_ram_126a & 0xfbff;
  _DAT_ram_1230 = 0;
  return CONCAT22(param_2,iVar1);
}





// Function: FUN_rom_007c52

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_007c52(void)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  
  uVar3 = _DAT_ram_125a & 0x120;
  if ((uVar3 == 0) && (_DAT_ram_1268 == 0)) {
    uVar3 = _DAT_ram_1e22;
    if (_DAT_ram_1e22 == 2) {
      if (_DAT_ram_1234 == 0xd || _DAT_ram_1234 + 1 < 0xe) {
        uVar3 = 10000;
        _DAT_ram_1da2 = 10000;
        _DAT_ram_1234 = _DAT_ram_1234 + 1;
      }
      else {
        _DAT_ram_1234 = 0xf;
        if (0x7fff < _DAT_ram_1da0 - 0x17eU && SBORROW2(_DAT_ram_1da0,0x17e) ||
            0 < (int)(_DAT_ram_1da0 - 0x17eU) && !SBORROW2(_DAT_ram_1da0,0x17e)) {
          uVar3 = 0xd48;
          _DAT_ram_1da2 = 0xd48;
        }
        else {
          uVar3 = 10000;
        }
        _DAT_ram_125a = _DAT_ram_125a | 0x100;
        _DAT_ram_1236 = uVar3;
      }
      goto LAB_rom_007c8c;
    }
  }
  _DAT_ram_1234 = 0;
LAB_rom_007c8c:
  uVar2 = _DAT_ram_1236;
  if ((0xd48 < (int)_DAT_ram_1236) && ((_DAT_ram_125a & 0x100) != 0)) {
    uVar3 = _DAT_ram_1236 - 0x32;
    uVar1 = _DAT_ram_1236 - 0xd7a;
    _DAT_ram_1236 = uVar3;
    if ((uVar1 < 0x8000 || !SBORROW2(uVar3,0xd48)) && ((int)uVar1 < 1 || SBORROW2(uVar3,0xd48))) {
      _DAT_ram_1236 = 0xd48;
    }
    _DAT_ram_1da2 = _DAT_ram_1236;
    uVar3 = _DAT_ram_1236;
  }
  return CONCAT22(uVar2,uVar3);
}





// Function: FUN_rom_007caa

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_007caa(undefined2 param_1,undefined2 param_2)

{
  byte bVar1;
  uint uVar2;
  
  if (_DAT_ram_1e22 == 1) {
    _DAT_ram_1bec = _DAT_ram_1bec & 0xff7f;
    return CONCAT22(param_2,1);
  }
  uVar2 = _DAT_ram_1e18;
  if ((_DAT_ram_1e18 & 0x800) != 0) {
    uVar2 = _DAT_ram_1d4e;
    if (_DAT_ram_1d4e == 0xc4d) {
      bVar1 = Peripherals::GPIOD.LATD._0_1_;
      if ((bVar1 & 0x10) == 0) {
        if (_DAT_ram_1268 == 3) {
          _DAT_ram_1bec = _DAT_ram_1bec | 0x80;
          return 0xc4d0c4d;
        }
      }
    }
  }
  if ((_DAT_ram_1268 == 4) || ((_DAT_ram_1e18 & 0x800) == 0)) {
    _DAT_ram_1bec = _DAT_ram_1bec & 0xff7f;
  }
  return CONCAT22(uVar2,_DAT_ram_1268);
}





// Function: FUN_rom_007ce2

undefined1 FUN_rom_007ce2(uint param_1,uint param_2,uint param_3,undefined1 param_4)

{
  if (param_1 < param_2) {
    if (param_1 <= param_3) {
      param_4 = 0;
    }
  }
  else {
    param_4 = 1;
  }
  return param_4;
}





// Function: FUN_rom_007cf8

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int FUN_rom_007cf8(void)

{
  ushort uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  bool bVar6;
  
  uVar2 = _DAT_ram_1e18;
  uVar4 = _DAT_ram_1266 & 1;
  uVar1 = Peripherals::ADC.ADCBUF13;
  uVar5 = _DAT_ram_1e18 >> 0xb & 1;
  uVar3 = FUN_rom_007ce2(uVar1,0x26c,0x136,uVar4);
  if (uVar5 == (uVar3 & 0xff)) {
    _DAT_ram_1238 = 0;
  }
  else {
    uVar5 = _DAT_ram_1238 + 1;
    bVar6 = _DAT_ram_1238 != 0x35;
    _DAT_ram_1238 = uVar5;
    if (0x35 < uVar5 && bVar6) {
      uVar1 = Peripherals::ADC.ADCBUF13;
      uVar4 = FUN_rom_007ce2(uVar1,0x26c,0x136,uVar4);
      uVar4 = uVar4 & 1;
      _DAT_ram_1e18 = uVar2 & 0xf7ff | uVar4 << 0xb;
      _DAT_ram_1238 = 0;
      if ((_DAT_ram_1bf2 & 0x10) == 0) {
        if (uVar4 == 0) {
          _DAT_ram_1262 = _DAT_ram_1262 & 0xfff7 | 0x40;
        }
      }
      else if (uVar4 != 0) {
        _DAT_ram_1262 = _DAT_ram_1262 & 0xffbf;
        _DAT_ram_1262 = _DAT_ram_1262 | 8;
      }
    }
  }
  if (_DAT_ram_1e22 != 3) {
    _DAT_ram_1262 = _DAT_ram_1262 & 0xffb7;
  }
  if ((_DAT_ram_1e18 & 0x800) == 0) {
    _DAT_ram_1bf2 = _DAT_ram_1bf2 | 0x10;
  }
  else {
    _DAT_ram_1bf2 = _DAT_ram_1bf2 & 0xffef;
  }
  return _DAT_ram_1e22;
}





// Function: FUN_rom_007d6e

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_007d6e(void)

{
  byte bVar1;
  ushort uVar2;
  uint uVar3;
  char cVar4;
  undefined2 uVar5;
  
  uVar2 = Peripherals::ADC.ADCBUF15;
  cVar4 = FUN_rom_007ce2(uVar2,0x2c9,0x26c,_DAT_ram_1266 >> 10 & 1);
  if (cVar4 == '\0') {
    bVar1 = Peripherals::GPIOD.LATD._0_1_;
    if (((bVar1 & 8) == 0) && ((_DAT_ram_1e18 & 0x100) == 0)) {
      if (_DAT_ram_1e22 != 5) {
        _DAT_ram_123a = _DAT_ram_123a + 1;
        goto LAB_rom_007d96;
      }
    }
  }
  _DAT_ram_123a = 0;
LAB_rom_007d96:
  uVar3 = _DAT_ram_123a;
  if (0x2ed < _DAT_ram_123a) {
    _DAT_ram_123a = 0x2ee;
    return CONCAT22(uVar3,0x2ee);
  }
  uVar5 = 499;
  if (499 < _DAT_ram_123a) {
    _DAT_ram_1bcc = _DAT_ram_1bca;
    _DAT_ram_1e18 = _DAT_ram_1e18 | 0x100;
    uVar5 = _DAT_ram_1bca;
  }
  return CONCAT22(uVar3,uVar5);
}





// Function: FUN_rom_007db2

uint FUN_rom_007db2(uint param_1,uint param_2,uint *param_3,uint param_4)

{
  uint uVar1;
  
  if (param_4 == param_1) {
    *param_3 = 0;
  }
  else {
    uVar1 = *param_3;
    *param_3 = uVar1 + 1;
    if (param_2 <= uVar1 + 1) {
      *param_3 = 0;
      param_4 = param_1;
    }
  }
  return param_4 & 0xff;
}





// Function: FUN_rom_007dcc

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_007dcc(void)

{
  ushort uVar1;
  byte bVar2;
  char cVar3;
  uint uVar4;
  undefined2 uVar5;
  
  if ((((_DAT_ram_1e22 == 2) && (_DAT_ram_1e0e == 0)) && ((_DAT_ram_1e20 & 0x10) == 0)) &&
     (((_DAT_ram_125a & 4) == 0 && ((_DAT_ram_1e1c & 0x800) != 0)))) {
    bVar2 = Peripherals::PWM1.IOCON1._1_1_;
    if ((((bVar2 & 2) != 0) &&
        ((bVar2 = Peripherals::PWM1.IOCON1._1_1_, (bVar2 & 1) != 0 &&
         (bVar2 = Peripherals::PWM2.IOCON2._1_1_, (bVar2 & 2) != 0)))) &&
       (bVar2 = Peripherals::PWM2.IOCON2._1_1_, (bVar2 & 1) != 0)) {
      _DAT_ram_123c = 0;
      return;
    }
    if (_DAT_ram_1268 == 3 || _DAT_ram_1268 == 4) {
      uVar1 = Peripherals::ADC.ADCBUF5;
      uVar4 = _DAT_ram_1e1c >> 2 & 1;
      uVar5 = 0;
      cVar3 = FUN_rom_007ce2(uVar1,0x15f,0x17f,_DAT_ram_1266 >> 0xf);
      if (cVar3 == '\0') {
        uVar5 = 1;
      }
      uVar4 = FUN_rom_007db2(uVar5,5,&DAT_ram_123c,uVar4);
      _DAT_ram_1e1c = _DAT_ram_1e1c & 0xfffb | (uVar4 & 1) << 2;
      return;
    }
  }
  _DAT_ram_123c = 0;
  return;
}





// Function: FUN_rom_007e42

int FUN_rom_007e42(int param_1,int param_2)

{
  bool bVar1;
  
  if (param_2 < param_1) {
    param_2 = param_1 + -1;
  }
  else {
    bVar1 = -1 < param_1 - param_2;
    if ((bVar1 || !SBORROW2(param_1,param_2)) && (!bVar1 || SBORROW2(param_1,param_2))) {
      param_2 = param_1 + 1;
    }
  }
  return param_2;
}





// Function: FUN_rom_007e54

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_007e54(void)

{
  if ((_DAT_ram_125a & 0x20) == 0) {
LAB_rom_007ebc:
    _DAT_ram_1244 = _DAT_ram_1d4e;
  }
  else {
    _DAT_ram_1242 = _DAT_ram_1d44 * 0x17 >> 10;
    if (_DAT_ram_1242 < 0) {
      _DAT_ram_1242 = 0;
    }
    if (10 < _DAT_ram_1242) {
      _DAT_ram_1242 = 10;
    }
    if ((_DAT_ram_1bea & 0x100) == 0) {
LAB_rom_007e86:
      if ((_DAT_ram_1bea & 0x80) == 0) {
LAB_rom_007ea2:
        if ((_DAT_ram_1bea & 0x40) != 0) {
          if (_DAT_ram_1d4e == 0xc4d) {
            _DAT_ram_123e = _DAT_ram_1242 + 0xc69;
            goto LAB_rom_007eb6;
          }
        }
        goto LAB_rom_007ebc;
      }
      if (_DAT_ram_1d4e != 0xc4d) goto LAB_rom_007ea2;
      _DAT_ram_1244 = ((uint)(&DAT_ram_d000 + _DAT_ram_1bc6) >> 2) + 0xbff;
    }
    else {
      if (_DAT_ram_1d4e != 0xc4d) goto LAB_rom_007e86;
      _DAT_ram_1240 = _DAT_ram_1242 + 0xc11;
LAB_rom_007eb6:
      _DAT_ram_1244 = FUN_rom_007e42(_DAT_ram_1244);
    }
    _DAT_ram_1d3a = 0;
  }
  _DAT_ram_1246 = (int)((long)_DAT_ram_1244 * (long)_DAT_ram_1d32 >> 0xd) + _DAT_ram_1d30;
  _DAT_ram_1d4c = (_DAT_ram_1246 - _DAT_ram_1d3a) - _DAT_ram_1d38;
  return;
}





// Function: FUN_rom_007ee0

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int FUN_rom_007ee0(void)

{
  int iVar1;
  
  iVar1 = _DAT_ram_1e22;
  if (_DAT_ram_1e22 == 2) {
    iVar1 = 0x800;
    if ((_DAT_ram_1e1c & 0x800) == 0) {
      _DAT_ram_1d4e = 0xffb0;
      iVar1 = _DAT_ram_1268 + -3;
      if (iVar1 == 0 || _DAT_ram_1268 == 4) {
        _DAT_ram_125a = _DAT_ram_125a | 0x10;
        _DAT_ram_1e0e = 0;
      }
      else {
        _DAT_ram_1e22 = 3;
        iVar1 = FUN_rom_009750();
      }
    }
  }
  if (((_DAT_ram_125a & 0x10) != 0) &&
     (((_DAT_ram_1e1c & 0x800) != 0 || ((_DAT_ram_1e1a & 2) != 0)))) {
    _DAT_ram_125a = _DAT_ram_125a & 0xffef;
    FUN_rom_009750();
    iVar1 = 0x9c4;
    _DAT_ram_1e10 = 0x9c4;
    _DAT_ram_1e22 = 0;
  }
  return iVar1;
}





// Function: FUN_rom_007f22

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_007f22(void)

{
  ushort uVar1;
  char cVar2;
  uint uVar3;
  undefined2 uVar4;
  
  if (((_DAT_ram_125a & 0x10) == 0) || (_DAT_ram_1e22 != 2)) {
    _DAT_ram_1e18 = _DAT_ram_1e18 & 0xefff;
    _DAT_ram_1248 = 0;
    return;
  }
  uVar1 = Peripherals::ADC.ADCBUF5;
  uVar3 = _DAT_ram_1e18 >> 0xc & 1;
  uVar4 = 0;
  cVar2 = FUN_rom_007ce2(uVar1,0x15f,0x17f,_DAT_ram_1266 >> 0xf);
  if (cVar2 == '\0') {
    uVar4 = 1;
  }
  uVar3 = FUN_rom_007db2(uVar4,5,&DAT_ram_1248,uVar3);
  _DAT_ram_1e18 = _DAT_ram_1e18 & 0xefff | (uVar3 & 1) << 0xc;
  if ((uVar3 & 1) == 0) {
    return;
  }
  _DAT_ram_192a = _DAT_ram_192a | 1;
  FUN_rom_009750();
  if ((_DAT_ram_1e1c & 0x802) != 2) {
    _DAT_ram_1e22 = 3;
    return;
  }
  if (_DAT_ram_1e20 != 0) {
    _DAT_ram_1e22 = 3;
    return;
  }
  _DAT_ram_1264 = _DAT_ram_1264 | 0x2000;
  _DAT_ram_1e22 = 3;
  return;
}





// Function: FUN_rom_007f86

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

uint FUN_rom_007f86(void)

{
  ushort uVar1;
  uint uVar2;
  
  uVar2 = _DAT_ram_1264;
  if (((_DAT_ram_1264 & 0x180) == 0) && ((_DAT_ram_1e1c & 4) == 0)) {
    return _DAT_ram_1264 & 0x180;
  }
  if (_DAT_ram_1e22 == 2) {
    _DAT_ram_1264 = _DAT_ram_1264 & 0xfe7f;
    _DAT_ram_1e1c = _DAT_ram_1e1c & 0xfffb;
    _DAT_ram_1e20 = _DAT_ram_1e20 | 1;
    if (((uVar2 & 0x400) == 0) && (((uVar2 & 0x200) != 0 || (_DAT_ram_1268 == 4)))) {
      _DAT_ram_1bf0 = _DAT_ram_1bf0 | 0x1000;
    }
    else {
      _DAT_ram_1bf0 = _DAT_ram_1bf0 | 0x2000;
    }
    uVar1 = Peripherals::ADC.ADCBUF4;
    _DAT_ram_1926 =
         (int)((ulong)uVar1 * 0x1e80 >> 0x10) << 6 | (uint)((ulong)uVar1 * 0x1e80 >> 10) & 0x3f;
    _DAT_ram_1924 =
         (int)((ulong)_DAT_ram_1be0 * (ulong)_DAT_ram_1926 >> 0x10) << 2 |
         (uint)((ulong)_DAT_ram_1be0 * (ulong)_DAT_ram_1926 >> 0xe) & 3;
    _DAT_ram_1262 = _DAT_ram_1262 | 0x1000;
    _DAT_ram_1bf0 = _DAT_ram_1bf0 | 1;
    _DAT_ram_1bf2 = _DAT_ram_1bf2 & 0xfffe;
    _DAT_ram_1bce = _DAT_ram_1924;
    _DAT_ram_1bd0 = _DAT_ram_1926;
    _DAT_ram_1bde = _DAT_ram_1926;
    FUN_rom_006bd6();
    _DAT_ram_192a = _DAT_ram_192a | 1;
    FUN_rom_009750();
    _DAT_ram_1e22 = 3;
    return 3;
  }
  _DAT_ram_1264 = _DAT_ram_1264 & 0xfe7f;
  _DAT_ram_1e1c = _DAT_ram_1e1c & 0xfffb;
  return _DAT_ram_1e22;
}





// Function: FUN_rom_007ff4

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

byte FUN_rom_007ff4(void)

{
  ushort uVar1;
  char cVar2;
  undefined1 uVar3;
  byte bVar4;
  byte bVar5;
  
  cVar2 = Peripherals::PWM1.PWMCON1._1_1_;
  if (cVar2 < '\0') {
    bVar5 = Peripherals::CMP3.CMPCON3._0_1_;
    if ((bVar5 & 8) == 0) {
      _DAT_ram_125a = _DAT_ram_125a | 4;
      if (_DAT_ram_1e22 == 2) {
        _DAT_ram_1e20 = _DAT_ram_1e20 | 1;
        bVar5 = Peripherals::GPIOD.LATD._0_1_;
        if ((bVar5 & 0x10) == 0) {
          _DAT_ram_1bf0 = _DAT_ram_1bf0 | 0x2000;
        }
        else {
          _DAT_ram_1bf0 = _DAT_ram_1bf0 | 0x1000;
        }
        uVar1 = Peripherals::ADC.ADCBUF4;
        _DAT_ram_1926 =
             (int)((ulong)uVar1 * 0x1e80 >> 0x10) << 6 | (uint)((ulong)uVar1 * 0x1e80 >> 10) & 0x3f;
        _DAT_ram_1924 =
             (int)((ulong)_DAT_ram_1be0 * (ulong)_DAT_ram_1926 >> 0x10) << 2 |
             (uint)((ulong)_DAT_ram_1be0 * (ulong)_DAT_ram_1926 >> 0xe) & 3;
        _DAT_ram_1262 = _DAT_ram_1262 | 0x1000;
        _DAT_ram_1bf0 = _DAT_ram_1bf0 | 1;
        _DAT_ram_1bf2 = _DAT_ram_1bf2 & 0xfffe;
        _DAT_ram_1bce = _DAT_ram_1924;
        _DAT_ram_1bd0 = _DAT_ram_1926;
        _DAT_ram_1bde = _DAT_ram_1926;
        FUN_rom_006bd6();
        _DAT_ram_192a = _DAT_ram_192a | 1;
        FUN_rom_009750();
        _DAT_ram_1e22 = 3;
      }
      uVar1 = Peripherals::PWM1.PWMCON1;
      Peripherals::PWM1.PWMCON1 = uVar1 & 0xefff;
      uVar1 = Peripherals::PWM1.PWMCON1;
      Peripherals::PWM1.PWMCON1 = uVar1 | 0x1000;
      bVar5 = Peripherals::PWM1.FCLCON1._0_1_;
      Peripherals::PWM1.FCLCON1._0_1_ = bVar5 | 3;
      uVar3 = Peripherals::PWM1.FCLCON1._0_1_;
      uVar3 = Peripherals::PWM1.FCLCON1._0_1_;
      uVar1 = Peripherals::PWM2.PWMCON2;
      Peripherals::PWM2.PWMCON2 = uVar1 & 0xefff;
      uVar1 = Peripherals::PWM2.PWMCON2;
      Peripherals::PWM2.PWMCON2 = uVar1 | 0x1000;
      bVar5 = Peripherals::PWM2.FCLCON2._0_1_;
      Peripherals::PWM2.FCLCON2._0_1_ = bVar5 | 3;
      uVar3 = Peripherals::PWM2.FCLCON2._0_1_;
      uVar3 = Peripherals::PWM2.FCLCON2._0_1_;
      uVar1 = Peripherals::PWM3.PWMCON3;
      Peripherals::PWM3.PWMCON3 = uVar1 & 0xefff;
      uVar1 = Peripherals::PWM3.PWMCON3;
      Peripherals::PWM3.PWMCON3 = uVar1 | 0x1000;
      bVar5 = Peripherals::PWM3.FCLCON3._0_1_;
      Peripherals::PWM3.FCLCON3._0_1_ = bVar5 | 3;
      uVar3 = Peripherals::PWM3.FCLCON3._0_1_;
      uVar3 = Peripherals::PWM3.FCLCON3._0_1_;
      DAT_ram_124a = 1;
      return 1;
    }
  }
  if (DAT_ram_124a == 1) {
    DAT_ram_124a = 2;
    bVar5 = Peripherals::PWM1.FCLCON1._0_1_;
    Peripherals::PWM1.FCLCON1._0_1_ = bVar5 & 0xfc;
    bVar5 = Peripherals::PWM1.FCLCON1._0_1_;
    Peripherals::PWM1.FCLCON1._0_1_ = bVar5 & 0xfc;
    return bVar5 & 0xfc;
  }
  if (DAT_ram_124a == 2) {
    DAT_ram_124a = 3;
    bVar5 = Peripherals::PWM2.FCLCON2._0_1_;
    Peripherals::PWM2.FCLCON2._0_1_ = bVar5 & 0xfc;
    bVar5 = Peripherals::PWM2.FCLCON2._0_1_;
    Peripherals::PWM2.FCLCON2._0_1_ = bVar5 & 0xfc;
    return bVar5 & 0xfc;
  }
  bVar5 = DAT_ram_124a;
  if (DAT_ram_124a == 3) {
    DAT_ram_124a = 0;
    bVar5 = Peripherals::PWM3.FCLCON3._0_1_;
    Peripherals::PWM3.FCLCON3._0_1_ = bVar5 & 0xfc;
    bVar4 = Peripherals::PWM3.FCLCON3._0_1_;
    bVar5 = bVar4 & 0xfc;
    Peripherals::PWM3.FCLCON3._0_1_ = bVar4 & 0xfc;
  }
  return bVar5;
}





// Function: FUN_rom_0080ee

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_0080ee(int param_1)

{
  ushort uVar1;
  int iVar2;
  bool bVar3;
  
  if (_DAT_ram_1d5c == -0x50) {
    _DAT_ram_124e = 0;
    _DAT_ram_124c = 0;
  }
  iVar2 = _DAT_ram_124e;
  if ((_DAT_ram_124e - 300U < 0x8000 || !SBORROW2(_DAT_ram_124e,300)) &&
      ((int)(_DAT_ram_124e - 300U) < 1 || SBORROW2(_DAT_ram_124e,300))) {
    _DAT_ram_124e = _DAT_ram_124e + 1;
    _DAT_ram_1d5c = (int)((long)_DAT_ram_124e * 0x1c) + 0x50;
    iVar2 = (int)((ulong)((long)_DAT_ram_124e * 0x1c) >> 0x10);
    if (param_1 < _DAT_ram_1d5c) {
      _DAT_ram_125a = _DAT_ram_125a | 0x20;
      _DAT_ram_1d5c = param_1;
      if (param_1 == 0x6b3) {
        uVar1 = Peripherals::GPIOD.LATD;
        Peripherals::GPIOD.LATD = uVar1 | 0x10;
        iVar2 = _DAT_ram_124c + 1;
        bVar3 = _DAT_ram_124c != 0x49;
        _DAT_ram_124c = iVar2;
        if (bVar3 && 0x49 < iVar2) {
          _DAT_ram_124e = 0;
          _DAT_ram_1262 = _DAT_ram_1262 & 0xdfff;
          _DAT_ram_124c = 0;
          _DAT_ram_1268 = 3;
          _DAT_ram_126e = 0x8000;
          _DAT_ram_1270 = 0x166;
          _DAT_ram_1262 = _DAT_ram_1262 | 0x100;
        }
      }
      else if (param_1 == 0xc4d) {
        _DAT_ram_124e = 0;
        _DAT_ram_1262 = _DAT_ram_1262 & 0xdfff;
        _DAT_ram_1268 = 3;
        uVar1 = Peripherals::GPIOD.LATD;
        Peripherals::GPIOD.LATD = uVar1 & 0xffef;
        _DAT_ram_126e = 0;
        _DAT_ram_1270 = 0;
      }
    }
  }
  return CONCAT22(iVar2,_DAT_ram_1d5c);
}





// Function: FUN_rom_008158

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_008158(void)

{
  int iVar1;
  ushort uVar2;
  bool bVar3;
  
  if (0x7fff < _DAT_ram_1d4e - 0xb11U && SBORROW2(_DAT_ram_1d4e,0xb11) ||
      0 < (int)(_DAT_ram_1d4e - 0xb11U) && !SBORROW2(_DAT_ram_1d4e,0xb11)) {
    _DAT_ram_1d5e = 0x8659;
    _DAT_ram_1250 = 0;
  }
  else {
    if ((_DAT_ram_1e18 & 0x800) == 0) {
      if ((_DAT_ram_1e1e & 6) == 0) {
        _DAT_ram_1d5e = 0x8659;
        goto LAB_rom_008198;
      }
    }
    _DAT_ram_1d5e = 0x4fa;
    if ((_DAT_ram_126a & 8) != 0) {
      if (_DAT_ram_1250 + 1 < 0x226 || _DAT_ram_1250 == 0x225) {
        _DAT_ram_1d5e = 0;
        _DAT_ram_1250 = _DAT_ram_1250 + 1;
      }
      else {
        _DAT_ram_1250 = 0x226;
      }
    }
  }
LAB_rom_008198:
  _DAT_ram_1d60 = 0;
  iVar1 = _DAT_ram_1270 - (uint)(_DAT_ram_126e < _DAT_ram_1d5e);
  _DAT_ram_1d4e = _DAT_ram_1d4a - (iVar1 * 4 | _DAT_ram_126e - _DAT_ram_1d5e >> 0xe);
  bVar3 = -1 < _DAT_ram_1d4e - _DAT_ram_1d4a;
  if ((bVar3 || SBORROW2(_DAT_ram_1d4e,_DAT_ram_1d4a)) &&
      (!bVar3 || !SBORROW2(_DAT_ram_1d4e,_DAT_ram_1d4a))) {
    _DAT_ram_126e = 0;
    _DAT_ram_1270 = 0;
    _DAT_ram_1d4e = _DAT_ram_1d4a;
    _DAT_ram_1e0e = 5;
    _DAT_ram_126a = _DAT_ram_126a & 0xfff7;
    _DAT_ram_1e1e = _DAT_ram_1e1e & 0xfff9;
    uVar2 = Peripherals::GPIOD.LATD;
    Peripherals::GPIOD.LATD = uVar2 & 0xffef;
    _DAT_ram_1264 = _DAT_ram_1264 & 0xfdff;
    return CONCAT22(iVar1 >> 0xe,5);
  }
  _DAT_ram_1264 = _DAT_ram_1264 | 0x200;
  _DAT_ram_126e = _DAT_ram_126e - _DAT_ram_1d5e;
  _DAT_ram_1270 = iVar1;
  return CONCAT22(iVar1 >> 0xe,_DAT_ram_1d4e);
}





// Function: FUN_rom_0081d4

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_0081d4(void)

{
  int iVar1;
  uint uVar2;
  bool bVar3;
  
  if (_DAT_ram_1d4e < 0xb9a) {
    _DAT_ram_1d62 = 0x9547;
  }
  else {
    _DAT_ram_1d62 = 0x4c75;
  }
  _DAT_ram_1d64 = 0;
  bVar3 = CARRY2(_DAT_ram_1d62,_DAT_ram_126e);
  _DAT_ram_126e = _DAT_ram_1d62 + _DAT_ram_126e;
  _DAT_ram_1270 = _DAT_ram_1270 + (uint)bVar3;
  uVar2 = (uint)(_DAT_ram_126e < 0x7fff);
  if (_DAT_ram_126e == 0x7fff && _DAT_ram_1270 - 0x166U == uVar2 ||
      (int)((_DAT_ram_1270 - 0x166U) - uVar2) < 0 !=
      (SBORROW2(_DAT_ram_1270,0x166) || SBORROW2(_DAT_ram_1270,uVar2 + 0x166))) {
    _DAT_ram_1264 = _DAT_ram_1264 | 0x400;
  }
  else {
    _DAT_ram_126e = 0x8000;
    _DAT_ram_1270 = 0x166;
    if ((_DAT_ram_1d44 - 0xc0U < 0x8000 || !SBORROW2(_DAT_ram_1d44,0xc0)) &&
        ((int)(_DAT_ram_1d44 - 0xc0U) < 1 || SBORROW2(_DAT_ram_1d44,0xc0))) {
      uVar2 = (int)((ulong)((long)_DAT_ram_1d44 * -0x2a6) >> 0x10) << 8 |
              (uint)((ulong)((long)_DAT_ram_1d44 * -0x2a6) >> 8) & 0xff;
      iVar1 = uVar2 + 0x1418;
      _DAT_ram_1d34 = 0x13ec;
      if (((uVar2 + 0x2c < 0x8000 || !SBORROW2(iVar1,0x13ec)) &&
           ((int)(uVar2 + 0x2c) < 1 || SBORROW2(iVar1,0x13ec))) &&
         (_DAT_ram_1d34 = iVar1,
         (uVar2 + 0x1bd < 0x8000 || !SBORROW2(iVar1,0x125b)) &&
         ((int)(uVar2 + 0x1bd) < 1 || SBORROW2(iVar1,0x125b)))) {
        _DAT_ram_1d34 = 0x125c;
      }
      _DAT_ram_1e0e = 5;
      _DAT_ram_126a = _DAT_ram_126a | 2;
      _DAT_ram_125a = _DAT_ram_125a & 0xfffd;
      _DAT_ram_125a = _DAT_ram_125a | 1;
      _DAT_ram_1264 = _DAT_ram_1264 | 0x800;
    }
    _DAT_ram_1d20 = 0x32;
    _DAT_ram_1268 = 4;
    _DAT_ram_1264 = _DAT_ram_1264 & 0xfbff;
  }
  _DAT_ram_1d4e = _DAT_ram_1d4a - (_DAT_ram_1270 << 2 | _DAT_ram_126e >> 0xe);
  return;
}





// Function: FUN_rom_008266

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_008266(void)

{
  int iVar1;
  uint uVar2;
  
  if (((_DAT_ram_1e18 & 0x800) != 0) && ((_DAT_ram_1e1e & 6) != 0)) {
    _DAT_ram_1e1e = _DAT_ram_1e1e & 0xfff9;
  }
  if ((_DAT_ram_1e20 & 1) == 0) {
    if (((((_DAT_ram_1e18 & 0x800) == 0) && ((_DAT_ram_1e1a & 0xa0) == 0)) ||
        (_DAT_ram_1bb6 <= _DAT_ram_1d42)) && (_DAT_ram_1d20 == 0)) {
      if ((_DAT_ram_1264 & 0x800) != 0) {
        uVar2 = (int)((ulong)((long)_DAT_ram_1d44 * -0xd3d) >> 0x10) << 8 |
                (uint)((ulong)((long)_DAT_ram_1d44 * -0xd3d) >> 8) & 0xff;
        iVar1 = uVar2 + 0x33a6;
        _DAT_ram_1d34 = 0x34bc;
        if (((uVar2 - 0x116 < 0x8000 || !SBORROW2(iVar1,0x34bc)) &&
             ((int)(uVar2 - 0x116) < 1 || SBORROW2(iVar1,0x34bc))) &&
           (_DAT_ram_1d34 = iVar1,
           (uVar2 + 0xf53 < 0x8000 || !SBORROW2(iVar1,0x2453)) &&
           ((int)(uVar2 + 0xf53) < 1 || SBORROW2(iVar1,0x2453)))) {
          _DAT_ram_1d34 = 0x2454;
        }
        _DAT_ram_126a = _DAT_ram_126a | 2;
        _DAT_ram_125a = _DAT_ram_125a & 0xfffe;
        _DAT_ram_125a = _DAT_ram_125a | 2;
        _DAT_ram_1264 = _DAT_ram_1264 & 0xf7ff;
      }
      _DAT_ram_1268 = 3;
    }
    else if (((_DAT_ram_1e4e < 0xc1) &&
             ((_DAT_ram_1d44 - 0xc0U < 0x8000 || !SBORROW2(_DAT_ram_1d44,0xc0)) &&
              ((int)(_DAT_ram_1d44 - 0xc0U) < 1 || SBORROW2(_DAT_ram_1d44,0xc0)))) &&
            ((_DAT_ram_1264 & 0x800) == 0)) {
      uVar2 = (int)((ulong)((long)_DAT_ram_1d44 * -0x2a6) >> 0x10) << 8 |
              (uint)((ulong)((long)_DAT_ram_1d44 * -0x2a6) >> 8) & 0xff;
      iVar1 = uVar2 + 0x1418;
      _DAT_ram_1d34 = 0x13ec;
      if (((uVar2 + 0x2c < 0x8000 || !SBORROW2(iVar1,0x13ec)) &&
           ((int)(uVar2 + 0x2c) < 1 || SBORROW2(iVar1,0x13ec))) &&
         (_DAT_ram_1d34 = iVar1,
         (uVar2 + 0x1bd < 0x8000 || !SBORROW2(iVar1,0x125b)) &&
         ((int)(uVar2 + 0x1bd) < 1 || SBORROW2(iVar1,0x125b)))) {
        _DAT_ram_1d34 = 0x125c;
      }
      _DAT_ram_1e0e = 5;
      _DAT_ram_126a = _DAT_ram_126a | 2;
      _DAT_ram_125a = _DAT_ram_125a & 0xfffd;
      _DAT_ram_125a = _DAT_ram_125a | 1;
      _DAT_ram_1264 = _DAT_ram_1264 | 0x800;
    }
  }
  else {
    _DAT_ram_125a = 0;
    _DAT_ram_126a = 0;
  }
  _DAT_ram_1262 = _DAT_ram_1262 & 0xffdf;
  _DAT_ram_1d4a = 0x6b3;
  return;
}





// Function: FUN_rom_00831a

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

uint FUN_rom_00831a(void)

{
  byte bVar1;
  ushort uVar2;
  uint uVar3;
  uint uVar4;
  
  _DAT_ram_1d4a = 0xc4d;
  uVar3 = 0x800;
  uVar4 = _DAT_ram_1e18 & 0x800;
  if (uVar4 == 0) {
    _DAT_ram_1262 = _DAT_ram_1262 & 0xffdf | 0x100;
  }
  if (((_DAT_ram_1262 & 0x100) != 0) && (uVar4 != 0)) {
    uVar3 = 0xc0;
    if ((_DAT_ram_1d44 - 0xc0U < 0x8000 || !SBORROW2(_DAT_ram_1d44,0xc0)) &&
        ((int)(_DAT_ram_1d44 - 0xc0U) < 1 || SBORROW2(_DAT_ram_1d44,0xc0))) {
      _DAT_ram_1262 = _DAT_ram_1262 | 0x20;
    }
    _DAT_ram_1262 = _DAT_ram_1262 & 0xfeff;
  }
  if ((_DAT_ram_1e20 & 1) == 0) {
    if ((_DAT_ram_125a & 0x10) == 0) {
      if ((((uVar4 == 0) || ((_DAT_ram_126a & 8) != 0)) && (_DAT_ram_1d4e != 0xc4d)) &&
         ((_DAT_ram_1e1a & 0xa0) == 0)) {
        _DAT_ram_1262 = _DAT_ram_1262 & 0xffdf;
        uVar4 = FUN_rom_008158();
        _DAT_ram_1264 = _DAT_ram_1264 & 0xfbff;
        return uVar4;
      }
      if (((_DAT_ram_1262 & 0x20) != 0) || ((_DAT_ram_1e1a & 0xa0) != 0)) {
        uVar4 = FUN_rom_0081d4();
        uVar2 = Peripherals::GPIOD.LATD;
        Peripherals::GPIOD.LATD = uVar2 | 0x10;
        return uVar4;
      }
      bVar1 = Peripherals::GPIOD.LATD._0_1_;
      if ((bVar1 & 0x10) == 0) {
        _DAT_ram_1d4a = 0xc4d;
        return (uint)bVar1;
      }
      if (0x7fff < _DAT_ram_1d44 - 0xd2U && SBORROW2(_DAT_ram_1d44,0xd2) ||
          0 < (int)(_DAT_ram_1d44 - 0xd2U) && !SBORROW2(_DAT_ram_1d44,0xd2)) {
        _DAT_ram_1d4a = 0xc4d;
        return 0xd2;
      }
      uVar4 = FUN_rom_0081d4();
      return uVar4;
    }
  }
  else {
    _DAT_ram_125a = 0;
    _DAT_ram_126a = 0;
  }
  return uVar3;
}





// Function: FUN_rom_00838a

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_00838a(void)

{
  if ((_DAT_ram_1262 & 0x2000) == 0) {
    if ((_DAT_ram_1da0 < 0x300) || ((_DAT_ram_125a & 0x80) != 0)) {
      FUN_rom_00977c();
    }
    else {
      _DAT_ram_125a = _DAT_ram_125a | 0x80;
    }
  }
  _DAT_ram_1d4e = FUN_rom_0080ee(_DAT_ram_1d4a);
  return;
}





// Function: FUN_rom_0083aa

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_0083aa(void)

{
  if ((_DAT_ram_1e22 == 2) && ((_DAT_ram_1e1c & 0x800) != 0)) {
    if (_DAT_ram_1268 == 3) {
      FUN_rom_00831a();
      return;
    }
    if (_DAT_ram_1268 == 4) {
      FUN_rom_008266();
    }
    else if (_DAT_ram_1268 == 0) {
      FUN_rom_00838a();
      return;
    }
  }
  return;
}





// Function: FUN_rom_0083ca

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_0083ca(void)

{
  ushort uVar1;
  int iVar2;
  int iVar3;
  
  if ((_DAT_ram_1da4 < 8000) || (iVar2 = 7999, iVar3 = _DAT_ram_1da4, (_DAT_ram_126a & 0x20) != 0))
  {
    iVar2 = 0x13b;
    iVar3 = _DAT_ram_1d94;
    if ((_DAT_ram_1d94 - 0x13bU < 0x8000 || !SBORROW2(_DAT_ram_1d94,0x13b)) &&
        ((int)(_DAT_ram_1d94 - 0x13bU) < 1 || SBORROW2(_DAT_ram_1d94,0x13b))) {
      iVar2 = _DAT_ram_1d4a;
      iVar3 = _DAT_ram_1d4e;
      if (_DAT_ram_1d4e == _DAT_ram_1d4a) {
        if (_DAT_ram_1e4e < 0x12a) {
          uVar1 = Peripherals::GPIOF.LATF;
          Peripherals::GPIOF.LATF = uVar1 & 0xffbf;
        }
        return CONCAT22(_DAT_ram_1e4e,0x129);
      }
    }
  }
  uVar1 = Peripherals::GPIOF.LATF;
  Peripherals::GPIOF.LATF = uVar1 | 0x40;
  return CONCAT22(iVar3,iVar2);
}





// Function: FUN_rom_0084a6

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_0084a6(void)

{
  ushort uVar1;
  
  Peripherals::SPI2.SPI2STAT = 0;
  Peripherals::SPI2.SPI2CON1 = 0x122;
  Peripherals::SPI2.SPI2CON2 = 0;
  uVar1 = Peripherals::SPI2.SPI2STAT;
  Peripherals::SPI2.SPI2STAT = uVar1 | 0x8000;
  _DAT_ram_1292 = 0;
  return 0x122;
}





// Function: FUN_rom_0084b6

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_0084b6(ushort *param_1)

{
  byte bVar1;
  ushort uVar2;
  ushort uVar3;
  
  uVar2 = Peripherals::GPIOG.LATG;
  Peripherals::GPIOG.LATG = uVar2 & 0xfdff;
  uVar2 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = 0xd7;
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  _DAT_ram_1394 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = 0;
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  uVar2 = Peripherals::SPI2.SPI2BUF;
  *param_1 = uVar2;
  uVar3 = Peripherals::GPIOG.LATG;
  Peripherals::GPIOG.LATG = uVar3 | 0x200;
  return CONCAT22(param_1,uVar2);
}





// Function: FUN_rom_0084dc

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

byte FUN_rom_0084dc(void)

{
  byte bVar1;
  ushort uVar2;
  
  do {
    FUN_rom_0084b6(&DAT_ram_1292);
  } while ((_DAT_ram_1292 & 0x80) == 0);
  uVar2 = Peripherals::GPIOG.LATG;
  Peripherals::GPIOG.LATG = uVar2 & 0xfdff;
  uVar2 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = 0x3d;
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  uVar2 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = 0x2a;
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  uVar2 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = 0x80;
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  _DAT_ram_1394 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = 0xa6;
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  uVar2 = Peripherals::GPIOG.LATG;
  Peripherals::GPIOG.LATG = uVar2 | 0x200;
  return bVar1;
}





// Function: FUN_rom_008522

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_008522(uint param_1)

{
  byte bVar1;
  ushort uVar2;
  
  do {
    FUN_rom_0084b6(&DAT_ram_1292);
  } while ((_DAT_ram_1292 & 0x80) == 0);
  uVar2 = Peripherals::GPIOG.LATG;
  Peripherals::GPIOG.LATG = uVar2 & 0xfdff;
  uVar2 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = 0x81;
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  uVar2 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = param_1 >> 8 & 3;
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  uVar2 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = param_1 & 0xff;
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  _DAT_ram_1394 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = 0;
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  uVar2 = Peripherals::GPIOG.LATG;
  Peripherals::GPIOG.LATG = uVar2 | 0x200;
  return;
}





// Function: FUN_rom_00856e

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ushort FUN_rom_00856e(int param_1,uint param_2)

{
  byte bVar1;
  ushort uVar2;
  ushort uVar3;
  uint uVar4;
  
  do {
    FUN_rom_0084b6(&DAT_ram_1292);
  } while ((_DAT_ram_1292 & 0x80) == 0);
  uVar3 = Peripherals::GPIOG.LATG;
  Peripherals::GPIOG.LATG = uVar3 & 0xfdff;
  uVar3 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = 0x84;
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  uVar3 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = 0;
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  uVar3 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = 0;
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  _DAT_ram_1394 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = 0;
  uVar3 = _DAT_ram_1394;
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
    uVar3 = CONCAT11((char)(uVar3 >> 8),bVar1);
  } while ((bVar1 & 1) == 0);
  for (uVar4 = 0; uVar4 < param_2; uVar4 = uVar4 + 1) {
    _DAT_ram_1394 = Peripherals::SPI2.SPI2BUF;
    Peripherals::SPI2.SPI2BUF = (uint)*(byte *)(param_1 + uVar4);
    do {
      bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
      uVar3 = (ushort)bVar1;
    } while ((bVar1 & 1) == 0);
  }
  uVar2 = Peripherals::GPIOG.LATG;
  Peripherals::GPIOG.LATG = uVar2 | 0x200;
  return uVar3;
}





// Function: FUN_rom_0085d0

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_0085d0(uint param_1)

{
  byte bVar1;
  ushort uVar2;
  
  do {
    FUN_rom_0084b6(&DAT_ram_1292);
  } while ((_DAT_ram_1292 & 0x80) == 0);
  uVar2 = Peripherals::GPIOG.LATG;
  Peripherals::GPIOG.LATG = uVar2 & 0xfdff;
  uVar2 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = 0x88;
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  uVar2 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = param_1 >> 8 & 3;
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  uVar2 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = param_1 & 0xff;
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  _DAT_ram_1394 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = 0;
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  uVar2 = Peripherals::GPIOG.LATG;
  Peripherals::GPIOG.LATG = uVar2 | 0x200;
  return;
}





// Function: FUN_rom_00861c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_00861c(int param_1,uint param_2)

{
  byte bVar1;
  ushort uVar2;
  uint uVar3;
  ulong in_W3W2;
  
  do {
    FUN_rom_0084b6(&DAT_ram_1292);
  } while ((_DAT_ram_1292 & 0x80) == 0);
  uVar2 = Peripherals::GPIOG.LATG;
  Peripherals::GPIOG.LATG = uVar2 & 0xfdff;
  uVar2 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = 0xd2;
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  uVar2 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = (uint)in_W3W2 >> 8 & 3;
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  uVar2 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = (ushort)(in_W3W2 & 0xffff00ff);
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  uVar2 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = (ushort)((in_W3W2 & 0xffff00ff) >> 0x10);
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  uVar2 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = 0;
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  uVar2 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = 0;
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  uVar2 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = 0;
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  _DAT_ram_1394 = Peripherals::SPI2.SPI2BUF;
  Peripherals::SPI2.SPI2BUF = 0;
  do {
    bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
  } while ((bVar1 & 1) == 0);
  for (uVar3 = 0; uVar3 < param_2; uVar3 = uVar3 + 1) {
    _DAT_ram_1394 = Peripherals::SPI2.SPI2BUF;
    Peripherals::SPI2.SPI2BUF = 0;
    do {
      bVar1 = Peripherals::SPI2.SPI2STAT._0_1_;
    } while ((bVar1 & 1) == 0);
    uVar2 = Peripherals::SPI2.SPI2BUF;
    *(char *)(param_1 + uVar3) = (char)uVar2;
  }
  uVar2 = Peripherals::GPIOG.LATG;
  Peripherals::GPIOG.LATG = uVar2 | 0x200;
  return;
}





// Function: FUN_rom_0086ba

void FUN_rom_0086ba(void)

{
  undefined1 *puVar1;
  int iVar2;
  bool bVar3;
  
  puVar1 = &DAT_ram_1498;
  do {
    *puVar1 = 0;
    bVar3 = puVar1 != (undefined1 *)0x1597;
    puVar1 = puVar1 + 1;
  } while (bVar3);
  iVar2 = 0;
  do {
    FUN_rom_008522(iVar2);
    FUN_rom_00856e(&DAT_ram_1498,0x100);
    FUN_rom_0085d0(iVar2);
    bVar3 = iVar2 != 7;
    iVar2 = iVar2 + 1;
  } while (bVar3);
  return;
}





// Function: FUN_rom_0086e0

undefined1 FUN_rom_0086e0(void)

{
  DAT_ram_1288 = DAT_ram_15d0;
  DAT_ram_1289 = DAT_ram_15d1;
  DAT_ram_1286 = DAT_ram_15d2;
  DAT_ram_1287 = DAT_ram_15d3;
  DAT_ram_1284 = DAT_ram_15d4;
  DAT_ram_1285 = DAT_ram_15d5;
  DAT_ram_1282 = DAT_ram_15d6;
  DAT_ram_1283 = DAT_ram_15d7;
  DAT_ram_1926 = DAT_ram_15d8;
  DAT_ram_1927 = DAT_ram_15d9;
  DAT_ram_1924 = DAT_ram_15da;
  DAT_ram_1925 = DAT_ram_15db;
  DAT_ram_1280 = DAT_ram_15dc;
  DAT_ram_1281 = DAT_ram_15dd;
  DAT_ram_127e = DAT_ram_15de;
  DAT_ram_127f = DAT_ram_15df;
  DAT_ram_127c = DAT_ram_15e0;
  DAT_ram_127d = DAT_ram_15e1;
  DAT_ram_127a = DAT_ram_15e2;
  DAT_ram_127b = DAT_ram_15e3;
  DAT_ram_1278 = DAT_ram_15e4;
  DAT_ram_1279 = DAT_ram_15e5;
  return DAT_ram_15e5;
}





// Function: FUN_rom_00873a

bool FUN_rom_00873a(int param_1,uint param_2,uint param_3,char param_4)

{
  char cVar1;
  char *pcVar2;
  
  param_2 = param_2 & 0xff;
  cVar1 = '\0';
  pcVar2 = (char *)(param_1 + param_2);
  for (; param_2 <= (param_3 & 0xff); param_2 = param_2 + 1) {
    cVar1 = cVar1 + *pcVar2;
    pcVar2 = pcVar2 + 2;
  }
  return -cVar1 != param_4;
}





// Function: FUN_rom_00875e

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_00875e(void)

{
  uint uVar1;
  undefined1 *puVar2;
  undefined1 extraout_var;
  uint uVar3;
  uint extraout_W1;
  undefined1 extraout_var_00;
  undefined1 extraout_var_01;
  undefined1 extraout_var_02;
  undefined1 extraout_var_03;
  undefined1 extraout_var_04;
  undefined1 extraout_var_05;
  undefined1 extraout_var_06;
  undefined1 extraout_var_07;
  undefined1 extraout_var_08;
  undefined1 extraout_var_09;
  undefined1 extraout_var_10;
  undefined1 extraout_var_11;
  int iVar4;
  bool bVar5;
  
  if ((_DAT_ram_1290 & 2) == 0) {
    _DAT_ram_1290 = _DAT_ram_1290 | 2;
    FUN_rom_0084b6(&DAT_ram_1292);
    FUN_rom_00861c(&DAT_ram_160e,0x100,6,0);
    if ((DAT_ram_160e == -1) && (DAT_ram_160f == -1)) {
      _DAT_ram_1290 = _DAT_ram_1290 | 8;
    }
    else {
      _DAT_ram_1290 = _DAT_ram_1290 & 0xfff7;
    }
    if (((_DAT_ram_1292 & 1) == 0) || ((_DAT_ram_1290 & 8) != 0)) {
      FUN_rom_0084dc();
      FUN_rom_0086ba();
      DAT_ram_128a = 0;
      DAT_ram_128b = 0;
      DAT_ram_128c = 0;
      _DAT_ram_1396 = 0;
      puVar2 = &DAT_ram_160e;
      do {
        *puVar2 = 0;
        bVar5 = puVar2 != (undefined1 *)0x170d;
        puVar2 = puVar2 + 1;
      } while (bVar5);
      puVar2 = &DAT_ram_15d0;
      do {
        *puVar2 = 0;
        bVar5 = puVar2 != &DAT_ram_15e5;
        puVar2 = puVar2 + 1;
      } while (bVar5);
      puVar2 = &DAT_ram_1598;
      do {
        *puVar2 = 0;
        bVar5 = puVar2 != (undefined1 *)0x15af;
        puVar2 = puVar2 + 1;
      } while (bVar5);
    }
    else {
      FUN_rom_00861c(&DAT_ram_15d0,0x16,0,0);
      FUN_rom_00861c(&DAT_ram_1598,0x18,5,0);
      FUN_rom_00861c(&DAT_ram_15e6,0x20,7,0);
      DAT_ram_128a = DAT_ram_1598;
      DAT_ram_128b = DAT_ram_1599;
      DAT_ram_128c = DAT_ram_159a;
      _DAT_ram_1396 = (uint)DAT_ram_159b;
      if ((DAT_ram_15e6 == '\0') && (DAT_ram_15e7 == '\0')) {
        _DAT_ram_125a = _DAT_ram_125a & 0xffbf;
      }
      else {
        _DAT_ram_125a = _DAT_ram_125a | 0x40;
      }
      uVar1 = CONCAT11(extraout_var,1);
      if (DAT_ram_15f6 == '\x01') {
        _DAT_ram_1bb2 = 0x8000;
        _DAT_ram_1e18 = _DAT_ram_1e18 | 1;
        _DAT_ram_1bea = _DAT_ram_1bea | 0x408;
      }
      iVar4 = 0;
      do {
        uVar1 = FUN_rom_00873a(&DAT_ram_160e,uVar1 & 0xff00,6,CONCAT11(0x16,DAT_ram_1615));
        uVar3 = _DAT_ram_128e & 0xfffe;
        _DAT_ram_128e = uVar3 | uVar1 & 1;
        uVar1 = FUN_rom_00873a(&DAT_ram_160e,CONCAT11((char)(uVar3 >> 8),8),
                               CONCAT11(extraout_var_00,0x26),CONCAT11(0x16,DAT_ram_1635));
        uVar3 = _DAT_ram_128e & 0xfffd;
        _DAT_ram_128e = uVar3 | (uVar1 & 1) * 2;
        uVar1 = FUN_rom_00873a(&DAT_ram_160e,CONCAT11((char)(uVar3 >> 8),0x28),
                               CONCAT11(extraout_var_01,0x6e),CONCAT11(0x16,DAT_ram_167d));
        uVar3 = _DAT_ram_128e & 0xfffb;
        _DAT_ram_128e = uVar3 | (uVar1 & 1) << 2;
        uVar1 = FUN_rom_00873a(&DAT_ram_160e,CONCAT11((char)(uVar3 >> 8),0x70),
                               CONCAT11(extraout_var_02,0x73),CONCAT11(0x16,DAT_ram_1682));
        uVar3 = _DAT_ram_128e & 0xfff7;
        _DAT_ram_128e = uVar3 | (uVar1 & 1) << 3;
        uVar1 = FUN_rom_00873a(&DAT_ram_160e,CONCAT11((char)(uVar3 >> 8),0x75),
                               CONCAT11(extraout_var_03,0x8c),CONCAT11(0x16,DAT_ram_1681));
        uVar3 = _DAT_ram_128e & 0xffef;
        _DAT_ram_128e = uVar3 | (uVar1 & 1) << 4;
        uVar1 = FUN_rom_00873a(&DAT_ram_160e,CONCAT11((char)(uVar3 >> 8),0x8d),
                               CONCAT11(extraout_var_04,0x90),CONCAT11(0x16,DAT_ram_169f));
        uVar3 = _DAT_ram_128e & 0xffdf;
        _DAT_ram_128e = uVar3 | (uVar1 & 1) << 5;
        uVar1 = FUN_rom_00873a(&DAT_ram_160e,CONCAT11((char)(uVar3 >> 8),0x92),
                               CONCAT11(extraout_var_05,0x9e),CONCAT11(0x16,DAT_ram_169e));
        uVar3 = _DAT_ram_128e & 0xffbf;
        _DAT_ram_128e = uVar3 | (uVar1 & 1) << 6;
        uVar1 = FUN_rom_00873a(&DAT_ram_160e,CONCAT11((char)(uVar3 >> 8),0x9f),
                               CONCAT11(extraout_var_06,0xa2),CONCAT11(0x16,DAT_ram_16b1));
        uVar3 = _DAT_ram_128e & 0xff7f;
        _DAT_ram_128e = uVar3 | (uVar1 & 1) << 7;
        uVar1 = FUN_rom_00873a(&DAT_ram_160e,CONCAT11((char)(uVar3 >> 8),0xa4),
                               CONCAT11(extraout_var_07,0xb0),CONCAT11(0x16,DAT_ram_16b0));
        _DAT_ram_128e = _DAT_ram_128e & 0xfeff | (uVar1 & 1) << 8;
        uVar1 = FUN_rom_00873a(&DAT_ram_160e,CONCAT11((char)(_DAT_ram_128e >> 8),0xb1),
                               CONCAT11(extraout_var_08,0xb4),CONCAT11(0x16,DAT_ram_16c3));
        _DAT_ram_128e = _DAT_ram_128e & 0xfdff | (uVar1 & 1) << 9;
        uVar1 = FUN_rom_00873a(&DAT_ram_160e,CONCAT11((char)(_DAT_ram_128e >> 8),0xb6),
                               CONCAT11(extraout_var_09,199),CONCAT11(0x16,DAT_ram_16c2));
        _DAT_ram_128e = _DAT_ram_128e & 0xfbff | (uVar1 & 1) << 10;
        uVar1 = FUN_rom_00873a(&DAT_ram_160e,CONCAT11((char)(_DAT_ram_128e >> 8),200),
                               CONCAT11(extraout_var_10,0xcb),CONCAT11(0x16,DAT_ram_16da));
        _DAT_ram_128e = _DAT_ram_128e & 0xf7ff | (uVar1 & 1) << 0xb;
        uVar1 = FUN_rom_00873a(&DAT_ram_160e,CONCAT11((char)(_DAT_ram_128e >> 8),0xcd),
                               CONCAT11(extraout_var_11,0xff),CONCAT11(0x16,DAT_ram_16d9));
        _DAT_ram_128e = _DAT_ram_128e & 0xefff | (uVar1 & 1) << 0xc;
        if (_DAT_ram_128e == 0) break;
        FUN_rom_00861c(&DAT_ram_160e,0x100,6,0);
        bVar5 = iVar4 != 2;
        uVar1 = extraout_W1;
        iVar4 = iVar4 + 1;
      } while (bVar5);
    }
    FUN_rom_0086e0();
  }
  return;
}





// Function: FUN_rom_008960

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined1 FUN_rom_008960(void)

{
  DAT_ram_1498 = DAT_ram_1396;
  DAT_ram_1499 = DAT_ram_128a;
  DAT_ram_149a = DAT_ram_128b;
  DAT_ram_149b = DAT_ram_128c;
  DAT_ram_149c = DAT_ram_1bf4;
  DAT_ram_149d = DAT_ram_1bf5;
  DAT_ram_149e = DAT_ram_1bf2;
  DAT_ram_149f = DAT_ram_1bf3;
  DAT_ram_14a0 = DAT_ram_1bee;
  DAT_ram_14a1 = DAT_ram_1bef;
  DAT_ram_14a2 = DAT_ram_1bec;
  DAT_ram_14a3 = DAT_ram_1bed;
  DAT_ram_14a4 = (char)_DAT_ram_1bbc;
  DAT_ram_14a5 = (char)((uint)_DAT_ram_1bbc >> 8);
  DAT_ram_14a6 = (char)_DAT_ram_1bba;
  DAT_ram_14a7 = (char)((uint)_DAT_ram_1bba >> 8);
  DAT_ram_14a8 = (char)_DAT_ram_1bb8;
  DAT_ram_14a9 = (char)((uint)_DAT_ram_1bb8 >> 8);
  DAT_ram_14aa = DAT_ram_1bea;
  DAT_ram_14ab = DAT_ram_1beb;
  DAT_ram_14ac = (char)_DAT_ram_1bc6;
  DAT_ram_14ad = (char)((uint)_DAT_ram_1bc6 >> 8);
  DAT_ram_14ae = (char)_DAT_ram_1bb6;
  DAT_ram_14af = (char)((uint)_DAT_ram_1bb6 >> 8);
  DAT_ram_14b0 = (char)_DAT_ram_1bb4;
  DAT_ram_14b1 = (char)((uint)_DAT_ram_1bb4 >> 8);
  DAT_ram_14b2 = DAT_ram_1bca;
  DAT_ram_14b3 = DAT_ram_1bcb;
  return DAT_ram_1bcb;
}





// Function: FUN_rom_0089d2

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined1 FUN_rom_0089d2(void)

{
  DAT_ram_1498 = DAT_ram_1396;
  DAT_ram_1499 = DAT_ram_1be8;
  DAT_ram_149a = DAT_ram_1be9;
  DAT_ram_149b = DAT_ram_1be6;
  DAT_ram_149c = DAT_ram_1be7;
  DAT_ram_149d = DAT_ram_1be4;
  DAT_ram_149e = DAT_ram_1be5;
  DAT_ram_149f = DAT_ram_1be2;
  DAT_ram_14a0 = DAT_ram_1be3;
  DAT_ram_14a1 = DAT_ram_1bde;
  DAT_ram_14a2 = DAT_ram_1bdf;
  DAT_ram_14a3 = DAT_ram_1bd2;
  DAT_ram_14a4 = DAT_ram_1bd3;
  DAT_ram_14a5 = (char)_DAT_ram_1bc8;
  DAT_ram_14a6 = (char)((uint)_DAT_ram_1bc8 >> 8);
  DAT_ram_14a7 = DAT_ram_1bd0;
  DAT_ram_14a8 = DAT_ram_1bd1;
  DAT_ram_14a9 = DAT_ram_1bdc;
  DAT_ram_14aa = DAT_ram_1bdd;
  DAT_ram_14ab = DAT_ram_1bda;
  DAT_ram_14ac = DAT_ram_1bdb;
  DAT_ram_14ad = DAT_ram_1bd8;
  DAT_ram_14ae = DAT_ram_1bd9;
  DAT_ram_14af = DAT_ram_1bd6;
  DAT_ram_14b0 = DAT_ram_1bd7;
  DAT_ram_14b1 = (char)_DAT_ram_1bb2;
  DAT_ram_14b2 = (char)((uint)_DAT_ram_1bb2 >> 8);
  DAT_ram_14b3 = DAT_ram_1bd4;
  DAT_ram_14b4 = DAT_ram_1bd5;
  return DAT_ram_1bd5;
}





// Function: FUN_rom_008a48

undefined2 FUN_rom_008a48(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  bool bVar4;
  undefined1 auStack_2 [8];
  undefined1 local_a;
  undefined1 local_b;
  undefined4 local_26;
  
  DAT_ram_1598 = DAT_ram_128a;
  DAT_ram_1599 = DAT_ram_128b;
  DAT_ram_159a = DAT_ram_128c;
  DAT_ram_159b = DAT_ram_1396;
  iVar1 = 0;
  iVar2 = 1;
  do {
    local_26 = 0x8a6c;
    iVar3 = iVar2;
    FUN_rom_00861c(auStack_2,0x20,1,iVar2 << 5);
    *(undefined1 *)(iVar1 + 0x159c) = local_a;
    *(undefined1 *)(iVar1 + 0x159d) = local_b;
    iVar2 = iVar3 + 1;
    iVar1 = iVar1 + 2;
  } while (iVar3 != 7);
  iVar2 = 0;
  do {
    local_26 = 0x8a90;
    FUN_rom_00861c(auStack_2,0x20,2,iVar2 << 5);
    *(undefined1 *)(iVar2 * 2 + 0x15aa) = local_a;
    *(undefined1 *)(iVar2 * 2 + 0x15ab) = local_b;
    bVar4 = iVar2 != 2;
    iVar2 = iVar2 + 1;
  } while (bVar4);
  return 0x17;
}





// Function: FUN_rom_008aac

void FUN_rom_008aac(void)

{
  FUN_rom_008a48();
  FUN_rom_008522(5);
  FUN_rom_00856e(&DAT_ram_1598,0x18);
  FUN_rom_0085d0(5);
  return;
}





// Function: FUN_rom_008abc

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

byte FUN_rom_008abc(byte param_1)

{
  uint uVar1;
  bool bVar2;
  
  if ((_DAT_ram_1bf2 & 1) != 0) {
    if ((_DAT_ram_1bf4 & 0x100) == 0) {
      param_1 = Peripherals::GPIOF.LATF._0_1_;
      if ((param_1 & 2) == 0) {
        _DAT_ram_1276 = _DAT_ram_1276 + (0xfffe < _DAT_ram_1274);
        param_1 = 0xdf;
        bVar2 = _DAT_ram_1274 != 0x93de;
        uVar1 = (uint)(_DAT_ram_1274 + 1 < 0x93df);
        _DAT_ram_1274 = _DAT_ram_1274 + 1;
        if ((3 < _DAT_ram_1276 && uVar1 + 4 <= _DAT_ram_1276) &&
            (bVar2 || _DAT_ram_1276 - 4 != uVar1)) {
          _DAT_ram_1274 = 0;
          _DAT_ram_1276 = 0;
          if ((_DAT_ram_128c < 0xff || _DAT_ram_128c < (_DAT_ram_128a < 0xfffe) + 0xff) ||
              _DAT_ram_128a == 0xfffe && _DAT_ram_128c - 0xff == (uint)(_DAT_ram_128a < 0xfffe)) {
            bVar2 = 0xfffe < _DAT_ram_128a;
            _DAT_ram_128a = _DAT_ram_128a + 1;
            _DAT_ram_128c = _DAT_ram_128c + bVar2;
          }
          uVar1 = _DAT_ram_1272 + 1;
          param_1 = 0x9f;
          bVar2 = _DAT_ram_1272 != 0x59e;
          _DAT_ram_1272 = uVar1;
          if (0x59e < uVar1 && bVar2) {
            _DAT_ram_1272 = _DAT_ram_1bf4 & 0x100;
            param_1 = FUN_rom_008aac();
          }
        }
      }
    }
  }
  return param_1;
}





// Function: FUN_rom_008b0e

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_008b0e(void)

{
  byte bVar1;
  undefined1 *puVar2;
  undefined1 *puVar3;
  int iVar4;
  int iVar5;
  uint uVar6;
  undefined1 *puVar7;
  bool bVar8;
  undefined1 local_2 [68];
  
  if (((((_DAT_ram_192a & 1) != 0) && (uVar6 = _DAT_ram_1bf4 & 0x100, uVar6 == 0)) &&
      (bVar1 = Peripherals::GPIOF.LATF._0_1_, (bVar1 & 2) == 0)) && (DAT_ram_1bee != '\0')) {
    _DAT_ram_192a = _DAT_ram_192a & 0xfffe;
    FUN_rom_00861c(&DAT_ram_1498,0x100,1,0);
    if ((((_DAT_ram_1e1c & 2) == 0) || ((_DAT_ram_1bf2 & 8) == 0)) || (_DAT_ram_1e22 == 5)) {
      FUN_rom_00861c(&DAT_ram_1498,0x100,1,0);
      FUN_rom_008960();
      FUN_rom_008522(1);
      FUN_rom_00856e(&DAT_ram_1498,0x100);
      FUN_rom_0085d0(1);
      FUN_rom_00861c(&DAT_ram_1498,0x100,3,0);
      FUN_rom_0089d2();
    }
    else {
      _DAT_ram_1396 = _DAT_ram_1396 + 1;
      FUN_rom_00861c(local_2,0x40,2,uVar6);
      puVar3 = local_2;
      puVar2 = &DAT_ram_1578;
      do {
        puVar2[-0xe0] = *puVar2;
        puVar2[-0xc0] = *puVar3;
        puVar2[-0xa0] = puVar3[0x20];
        puVar7 = puVar2 + 1;
        puVar3 = puVar3 + 1;
        bVar8 = puVar2 != (undefined1 *)0x1597;
        puVar2 = puVar7;
      } while (bVar8);
      puVar3 = &DAT_ram_14f8;
      do {
        *puVar3 = 0;
        bVar8 = puVar3 != (undefined1 *)0x1597;
        puVar3 = puVar3 + 1;
      } while (bVar8);
      FUN_rom_008522(2);
      FUN_rom_00856e(&DAT_ram_1498,0x100);
      FUN_rom_0085d0(2);
      FUN_rom_00861c(&DAT_ram_1498,0x100,1,0);
      FUN_rom_008960();
      iVar5 = 7;
      do {
        puVar3 = &DAT_ram_1498 + iVar5 * 0x20;
        iVar4 = 0;
        do {
          *puVar3 = puVar3[-0x20];
          puVar3 = puVar3 + 1;
          bVar8 = iVar4 != 0x1f;
          iVar4 = iVar4 + 1;
        } while (bVar8);
        iVar5 = iVar5 + -1;
      } while (iVar5 != 0);
      FUN_rom_008522(1);
      FUN_rom_00856e(&DAT_ram_1498,0x100);
      FUN_rom_0085d0(1);
      FUN_rom_00861c(&DAT_ram_1498,0x100,3,iVar5);
      FUN_rom_00861c(local_2,0x40,4,iVar5);
      puVar2 = &DAT_ram_1578;
      puVar3 = local_2;
      do {
        puVar2[-0xe0] = *puVar2;
        puVar2[-0xc0] = *puVar3;
        puVar2[-0xa0] = puVar3[0x20];
        puVar2 = puVar2 + 1;
        puVar3 = puVar3 + 1;
      } while (puVar2 != puVar7);
      puVar3 = &DAT_ram_14f8;
      do {
        puVar2 = puVar3 + 1;
        *puVar3 = 0;
        puVar3 = puVar2;
      } while (puVar2 != &DAT_ram_1598);
      FUN_rom_008522(4);
      FUN_rom_00856e(&DAT_ram_1498,0x100);
      FUN_rom_0085d0(4);
      FUN_rom_00861c(&DAT_ram_1498,0x100,3,0);
      FUN_rom_0089d2();
      iVar5 = 7;
      do {
        puVar3 = &DAT_ram_1498 + iVar5 * 0x20;
        iVar4 = 0;
        do {
          *puVar3 = puVar3[-0x20];
          puVar3 = puVar3 + 1;
          bVar8 = iVar4 != 0x1f;
          iVar4 = iVar4 + 1;
        } while (bVar8);
        iVar5 = iVar5 + -1;
      } while (iVar5 != 0);
    }
    FUN_rom_008522(3);
    FUN_rom_00856e(&DAT_ram_1498,0x100);
    FUN_rom_0085d0(3);
    FUN_rom_008aac();
  }
  return;
}





// Function: FUN_rom_008c68

undefined1 FUN_rom_008c68(void)

{
  DAT_ram_15d0 = DAT_ram_1288;
  DAT_ram_15d1 = DAT_ram_1289;
  DAT_ram_15d2 = DAT_ram_1286;
  DAT_ram_15d3 = DAT_ram_1287;
  DAT_ram_15d4 = DAT_ram_1284;
  DAT_ram_15d5 = DAT_ram_1285;
  DAT_ram_15d6 = DAT_ram_1282;
  DAT_ram_15d7 = DAT_ram_1283;
  DAT_ram_15d8 = DAT_ram_1926;
  DAT_ram_15d9 = DAT_ram_1927;
  DAT_ram_15da = DAT_ram_1924;
  DAT_ram_15db = DAT_ram_1925;
  DAT_ram_15dc = DAT_ram_1280;
  DAT_ram_15dd = DAT_ram_1281;
  DAT_ram_15de = DAT_ram_127e;
  DAT_ram_15df = DAT_ram_127f;
  DAT_ram_15e0 = DAT_ram_127c;
  DAT_ram_15e1 = DAT_ram_127d;
  DAT_ram_15e2 = DAT_ram_127a;
  DAT_ram_15e3 = DAT_ram_127b;
  DAT_ram_15e4 = DAT_ram_1278;
  DAT_ram_15e5 = DAT_ram_1279;
  return DAT_ram_1279;
}





// Function: FUN_rom_008cc2

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

uint FUN_rom_008cc2(void)

{
  byte bVar1;
  uint uVar2;
  undefined1 *puVar3;
  bool bVar4;
  
  bVar1 = Peripherals::GPIOF.LATF._0_1_;
  uVar2 = (uint)bVar1;
  if ((bVar1 & 2) == 0) {
    if ((_DAT_ram_1e1a & 8) == 0) {
      if ((_DAT_ram_1bf4 & 0x100) == 0) {
        if ((_DAT_ram_1e1c & 2) != 0) {
          if (_DAT_ram_1288 < _DAT_ram_1be8) {
            _DAT_ram_1288 = _DAT_ram_1be8;
            _DAT_ram_1262 = _DAT_ram_1262 | 0x1000;
          }
          if (_DAT_ram_1286 < _DAT_ram_1bd2) {
            _DAT_ram_1286 = _DAT_ram_1bd2;
            _DAT_ram_1262 = _DAT_ram_1262 | 0x1000;
          }
          if (_DAT_ram_1284 < _DAT_ram_1bc8) {
            _DAT_ram_1284 = _DAT_ram_1bc8;
            _DAT_ram_1262 = _DAT_ram_1262 | 0x1000;
          }
        }
        uVar2 = _DAT_ram_1bf2;
        if ((_DAT_ram_1bf2 & 1) != 0) {
          if (_DAT_ram_1282 < _DAT_ram_1be2) {
            _DAT_ram_1282 = _DAT_ram_1be2;
            _DAT_ram_1262 = _DAT_ram_1262 | 0x1000;
          }
          if (_DAT_ram_1926 < _DAT_ram_1bd0) {
            _DAT_ram_1926 = _DAT_ram_1bd0;
            _DAT_ram_1262 = _DAT_ram_1262 | 0x1000;
          }
          uVar2 = _DAT_ram_1924;
          if (_DAT_ram_1924 < _DAT_ram_1bce) {
            _DAT_ram_1924 = _DAT_ram_1bce;
            _DAT_ram_1262 = _DAT_ram_1262 | 0x1000;
          }
        }
        if (((_DAT_ram_1e1c & 2) != 0) || ((_DAT_ram_1bf2 & 1) != 0)) {
          if (_DAT_ram_1280 < _DAT_ram_1bdc) {
            _DAT_ram_1280 = _DAT_ram_1bdc;
            _DAT_ram_1262 = _DAT_ram_1262 | 0x1000;
          }
          if (_DAT_ram_127e < _DAT_ram_1bda) {
            _DAT_ram_127e = _DAT_ram_1bda;
            _DAT_ram_1262 = _DAT_ram_1262 | 0x1000;
          }
          if (_DAT_ram_127c < _DAT_ram_1bd8) {
            _DAT_ram_127c = _DAT_ram_1bd8;
            _DAT_ram_1262 = _DAT_ram_1262 | 0x1000;
          }
          if (_DAT_ram_127a < _DAT_ram_1bd6) {
            _DAT_ram_127a = _DAT_ram_1bd6;
            _DAT_ram_1262 = _DAT_ram_1262 | 0x1000;
          }
          uVar2 = _DAT_ram_1278;
          if (_DAT_ram_1278 <= _DAT_ram_1bd4) {
            uVar2 = 199;
            if (199 < _DAT_ram_1bd4) {
              _DAT_ram_1278 = _DAT_ram_1bd4;
              _DAT_ram_1262 = _DAT_ram_1262 | 0x1000;
            }
          }
        }
        if ((_DAT_ram_1262 & 0x1000) != 0) {
          _DAT_ram_1262 = _DAT_ram_1262 & 0xefff;
          FUN_rom_008c68();
          FUN_rom_008522(0);
          FUN_rom_00856e(&DAT_ram_15d0,0x16);
          uVar2 = FUN_rom_0085d0(0);
          return uVar2;
        }
      }
    }
    else {
      _DAT_ram_1e1a = _DAT_ram_1e1a & 0xfff7;
      puVar3 = &DAT_ram_15d0;
      do {
        *puVar3 = 0;
        bVar4 = puVar3 != &DAT_ram_15e6;
        puVar3 = puVar3 + 1;
      } while (bVar4);
      FUN_rom_0086e0();
      FUN_rom_008522(0);
      FUN_rom_00856e(&DAT_ram_15d0,0x16);
      uVar2 = FUN_rom_0085d0(0);
    }
  }
  return uVar2;
}





// Function: FUN_rom_008da2

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_008da2(void)

{
  ushort uVar1;
  
  if ((_DAT_ram_1928 & 2) != 0) {
    FUN_rom_00861c(0x181e,0x100,DAT_ram_1923,DAT_ram_1922);
    uVar1 = Peripherals::I2C2.I2C2CON;
    Peripherals::I2C2.I2C2CON = uVar1 | 0x1000;
    _DAT_ram_1928 = _DAT_ram_1928 & 0xfffd;
  }
  return;
}





// Function: FUN_rom_008dbe

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_008dbe(void)

{
  ushort uVar1;
  byte bVar2;
  
  if ((_DAT_ram_1928 & 1) != 0) {
    bVar2 = Peripherals::GPIOF.LATF._0_1_;
    if ((bVar2 & 2) == 0) {
      FUN_rom_008522(DAT_ram_1921);
      FUN_rom_00856e(0x171e,0x100);
      FUN_rom_0085d0(DAT_ram_1921);
    }
    uVar1 = Peripherals::I2C2.I2C2CON;
    Peripherals::I2C2.I2C2CON = uVar1 | 0x1000;
    _DAT_ram_1928 = _DAT_ram_1928 & 0xfffe;
  }
  return;
}





// Function: FUN_rom_008de0

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* WARNING: Restarted to delay deadcode elimination for space: ram */

undefined2 FUN_rom_008de0(undefined2 param_1)

{
  int iVar1;
  undefined1 extraout_var;
  undefined1 extraout_var_00;
  undefined1 uVar2;
  undefined1 extraout_var_01;
  undefined1 extraout_var_02;
  undefined2 uVar3;
  uint uVar4;
  int iVar5;
  
  if ((_DAT_ram_1e1a & 0x100) == 0) {
    return param_1;
  }
  uVar4 = 0x9fff;
  if (0x9fff < _DAT_ram_191e) {
    if ((_DAT_ram_1606 == _DAT_ram_1390) &&
       (iVar5 = _DAT_ram_1388, uVar3 = _DAT_ram_138c, iVar1 = FUN_rom_00a6e0(), iVar5 == iVar1)) {
      FUN_rom_00a6cc(uVar3);
      FUN_rom_00a6d4(_DAT_ram_1606);
      uVar3 = CONCAT11(extraout_var_01,3);
      uVar2 = extraout_var;
    }
    else {
      FUN_rom_00a6cc(0);
      FUN_rom_00a6d4(_DAT_ram_1606);
      uVar3 = CONCAT11(extraout_var_02,2);
      uVar2 = extraout_var_00;
    }
    uVar3 = FUN_rom_00a688(CONCAT11(uVar2,0xff),uVar3);
    _DAT_ram_1e1a = _DAT_ram_1e1a & 0xfeff;
    return uVar3;
  }
  FUN_rom_00861c(0x1294,0x100,DAT_ram_191f,0);
  uVar3 = 0x100;
  if (uVar4 < _DAT_ram_191e + 0x100) {
    uVar3 = 0xfc;
  }
  FUN_rom_00a786(0x1294,&DAT_ram_1606,uVar3,0);
  _DAT_ram_191e = _DAT_ram_191e + 0x100;
  return 0x100;
}





// Function: thunk_FUN_rom_008de0

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* WARNING: Restarted to delay deadcode elimination for space: ram */

undefined2 thunk_FUN_rom_008de0(undefined2 param_1)

{
  int iVar1;
  undefined1 extraout_var;
  undefined1 extraout_var_00;
  undefined1 uVar2;
  undefined1 extraout_var_01;
  undefined1 extraout_var_02;
  undefined2 uVar3;
  uint uVar4;
  int iVar5;
  
  if ((_DAT_ram_1e1a & 0x100) == 0) {
    return param_1;
  }
  uVar4 = 0x9fff;
  if (0x9fff < _DAT_ram_191e) {
    if ((_DAT_ram_1606 == _DAT_ram_1390) &&
       (iVar5 = _DAT_ram_1388, uVar3 = _DAT_ram_138c, iVar1 = FUN_rom_00a6e0(), iVar5 == iVar1)) {
      FUN_rom_00a6cc(uVar3);
      FUN_rom_00a6d4(_DAT_ram_1606);
      uVar3 = CONCAT11(extraout_var_01,3);
      uVar2 = extraout_var;
    }
    else {
      FUN_rom_00a6cc(0);
      FUN_rom_00a6d4(_DAT_ram_1606);
      uVar3 = CONCAT11(extraout_var_02,2);
      uVar2 = extraout_var_00;
    }
    uVar3 = FUN_rom_00a688(CONCAT11(uVar2,0xff),uVar3);
    _DAT_ram_1e1a = _DAT_ram_1e1a & 0xfeff;
    return uVar3;
  }
  FUN_rom_00861c(0x1294,0x100,DAT_ram_191f,0);
  uVar3 = 0x100;
  if (uVar4 < _DAT_ram_191e + 0x100) {
    uVar3 = 0xfc;
  }
  FUN_rom_00a786(0x1294,&DAT_ram_1606,uVar3,0);
  _DAT_ram_191e = _DAT_ram_191e + 0x100;
  return 0x100;
}





// Function: FUN_rom_008e60

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_008e60(void)

{
  ushort uVar1;
  byte bVar2;
  
  if ((_DAT_ram_1928 & 0x10) != 0) {
    bVar2 = Peripherals::GPIOF.LATF._0_1_;
    if ((bVar2 & 2) == 0) {
      FUN_rom_008522(6);
      FUN_rom_00856e(&DAT_ram_160e,0x100);
      FUN_rom_0085d0(6);
    }
    uVar1 = Peripherals::I2C2.I2C2CON;
    Peripherals::I2C2.I2C2CON = uVar1 | 0x1000;
    _DAT_ram_1928 = _DAT_ram_1928 & 0xffef;
  }
  return;
}





// Function: FUN_rom_008e7e

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_008e7e(void)

{
  ushort uVar1;
  byte bVar2;
  
  if ((_DAT_ram_1928 & 0x40) != 0) {
    bVar2 = Peripherals::GPIOF.LATF._0_1_;
    if ((bVar2 & 2) == 0) {
      FUN_rom_008522(7);
      FUN_rom_00856e(&DAT_ram_15e6,0x20);
      FUN_rom_0085d0(7);
    }
    uVar1 = Peripherals::I2C2.I2C2CON;
    Peripherals::I2C2.I2C2CON = uVar1 | 0x1000;
    _DAT_ram_1928 = _DAT_ram_1928 & 0xffbf;
  }
  return;
}





// Function: FUN_rom_008e9c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_008e9c(void)

{
  ushort uVar1;
  
  if ((_DAT_ram_1928 & 0x20) != 0) {
    FUN_rom_00861c(0x15b0,0x20,_DAT_ram_160c,_DAT_ram_160a);
    uVar1 = Peripherals::I2C2.I2C2CON;
    Peripherals::I2C2.I2C2CON = uVar1 | 0x1000;
    _DAT_ram_1928 = _DAT_ram_1928 & 0xffdf;
  }
  return;
}





// Function: FUN_rom_008eb0

undefined4 FUN_rom_008eb0(int param_1)

{
  return CONCAT22(&DAT_ram_15d0,(uint)(byte)(&DAT_ram_15d0)[param_1]);
}





// Function: FUN_rom_008eb8

undefined4 FUN_rom_008eb8(int param_1)

{
  return CONCAT22(0x15b0,(uint)*(byte *)(param_1 + 0x15b0));
}





// Function: FUN_rom_008ec0

undefined4 FUN_rom_008ec0(int param_1)

{
  return CONCAT22(&DAT_ram_1598,(uint)(byte)(&DAT_ram_1598)[param_1]);
}





// Function: FUN_rom_008ec8

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_008ec8(void)

{
  byte bVar1;
  undefined1 *puVar2;
  int iVar3;
  bool bVar4;
  
  if ((_DAT_ram_1e1a & 0x40) != 0) {
    bVar1 = Peripherals::GPIOF.LATF._0_1_;
    if ((bVar1 & 2) == 0) {
      _DAT_ram_1e1a = _DAT_ram_1e1a & 0xffbf;
      puVar2 = &DAT_ram_1498;
      do {
        *puVar2 = 0;
        bVar4 = puVar2 != (undefined1 *)0x1597;
        puVar2 = puVar2 + 1;
      } while (bVar4);
      iVar3 = 1;
      do {
        FUN_rom_008522(iVar3);
        FUN_rom_00856e(&DAT_ram_1498,0x100);
        FUN_rom_0085d0(iVar3);
        bVar4 = iVar3 != 5;
        iVar3 = iVar3 + 1;
      } while (bVar4);
      DAT_ram_128a = 0;
      DAT_ram_128b = 0;
      DAT_ram_128c = 0;
      _DAT_ram_1396 = 0;
      puVar2 = &DAT_ram_1598;
      do {
        *puVar2 = 0;
        bVar4 = puVar2 != (undefined1 *)0x15af;
        puVar2 = puVar2 + 1;
      } while (bVar4);
    }
  }
  return;
}





// Function: FUN_rom_008f0c

uint FUN_rom_008f0c(uint param_1,uint param_2,uint *param_3,uint param_4)

{
  uint uVar1;
  
  if (param_4 == param_1) {
    *param_3 = 0;
  }
  else {
    uVar1 = *param_3;
    *param_3 = uVar1 + 1;
    if (param_2 <= uVar1 + 1) {
      *param_3 = 0;
      param_4 = param_1;
    }
  }
  return param_4 & 0xff;
}





// Function: FUN_rom_008f26

undefined1 FUN_rom_008f26(uint param_1,uint param_2,uint param_3,undefined1 param_4)

{
  if (param_1 < param_2) {
    if (param_1 <= param_3) {
      param_4 = 0;
    }
  }
  else {
    param_4 = 1;
  }
  return param_4;
}





// Function: FUN_rom_008f3c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_008f3c(void)

{
  uint uVar1;
  ushort uVar2;
  char cVar3;
  uint uVar4;
  uint uVar5;
  undefined2 uVar6;
  
  uVar2 = Peripherals::ADC.ADCBUF12;
  uVar4 = _DAT_ram_1bf2 >> 3 & 1;
  uVar6 = 0;
  cVar3 = FUN_rom_008f26(uVar2,0x26c,0x136,_DAT_ram_1266 >> 1 & 1);
  if (cVar3 == '\0') {
    uVar6 = 1;
  }
  uVar4 = FUN_rom_008f0c(uVar6,3,0x1db2,uVar4);
  uVar4 = uVar4 & 1;
  uVar5 = uVar4 << 3;
  uVar1 = _DAT_ram_1bf2 & 0xfff7 | uVar5;
  if (_DAT_ram_1e22 == 2) {
    if (uVar4 != 0) {
      if ((_DAT_ram_125a & 0x2000) == 0) {
        _DAT_ram_1bf2 = uVar1;
        return;
      }
      _DAT_ram_125a = _DAT_ram_125a & 0xdfff;
      _DAT_ram_1bf2 = uVar1;
      _DAT_ram_1db0 = 0;
      return;
    }
    if (_DAT_ram_1268 == 3 || _DAT_ram_1268 == 4) {
      _DAT_ram_125a = _DAT_ram_125a | 0x2000;
      uVar4 = _DAT_ram_1db0 + 1;
      _DAT_ram_1d4c = _DAT_ram_1d4c + uVar4 * -2;
      if (uVar4 < 0x4b || _DAT_ram_1db0 == 0x4a) {
        _DAT_ram_1bf2 = uVar1;
        _DAT_ram_1db0 = uVar4;
        return;
      }
      _DAT_ram_1bf2 = _DAT_ram_1bf2 & 0xfff6;
      _DAT_ram_192a = _DAT_ram_192a | 1;
      _DAT_ram_1db0 = uVar5;
      FUN_rom_009750();
      _DAT_ram_125a = _DAT_ram_125a & 0xdfff;
      _DAT_ram_1e10 = 0x9c4;
      _DAT_ram_1e22 = uVar5;
      return;
    }
  }
  else {
    _DAT_ram_125a = _DAT_ram_125a & 0xdfff;
    _DAT_ram_1db0 = 0;
    if (uVar4 != 0) {
      _DAT_ram_1bf2 = uVar1;
      _DAT_ram_1db0 = 0;
      return;
    }
    if (_DAT_ram_1e22 == 0) goto LAB_rom_008fc6;
  }
  _DAT_ram_1bf2 = _DAT_ram_1bf2 & 0xfff6 | uVar5;
  _DAT_ram_192a = _DAT_ram_192a | 1;
  uVar1 = _DAT_ram_1bf2;
LAB_rom_008fc6:
  _DAT_ram_1bf2 = uVar1;
  FUN_rom_009750();
  _DAT_ram_1e22 = uVar5;
  _DAT_ram_1e10 = 0x9c4;
  return;
}





// Function: FUN_rom_008fd4

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_rom_008fd4(void)

{
  int iVar1;
  int iVar2;
  ushort uVar3;
  bool bVar4;
  
  uVar3 = Peripherals::ADC.ADCBUF4;
  _DAT_ram_1d94 = uVar3;
  *(ushort *)(_DAT_ram_1dc4 * 2 + 0x1db4) = uVar3;
  iVar1 = uVar3 + _DAT_ram_1d68;
  iVar2 = _DAT_ram_1dc4 + 1;
  bVar4 = _DAT_ram_1dc4 != 6;
  _DAT_ram_1dc4 = iVar2;
  if (bVar4 && 6 < iVar2) {
    _DAT_ram_1dc4 = 0;
  }
  _DAT_ram_1d68 = iVar1 - *(int *)(_DAT_ram_1dc4 * 2 + 0x1db4);
  _DAT_ram_1d46 = iVar1 >> 3;
  return CONCAT26(iVar2,CONCAT24(_DAT_ram_1d68,CONCAT22(0x1db4,iVar1)));
}





// Function: FUN_rom_009000

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

uint FUN_rom_009000(void)

{
  byte bVar1;
  uint uVar2;
  
  if (_DAT_ram_1e22 != 2) {
    _DAT_ram_1dc6 = 0;
    return _DAT_ram_1e22;
  }
  bVar1 = Peripherals::GPIOD.PORTD._1_1_;
  _DAT_ram_1dc8 = bVar1 >> 1 & 1;
  bVar1 = Peripherals::GPIOD.PORTD._1_1_;
  _DAT_ram_1d1a = bVar1 & 1;
  uVar2 = _DAT_ram_1d1a;
  if ((_DAT_ram_1dc8 == _DAT_ram_1d1a) && (uVar2 = 0x500, 0x500 < _DAT_ram_1d9e)) {
    uVar2 = _DAT_ram_1dc6 + 1;
    if (_DAT_ram_1dc6 == 0x12 || (int)uVar2 < 0x13) {
      _DAT_ram_1dc6 = uVar2;
      return uVar2;
    }
    _DAT_ram_1dc6 = 0x14;
    _DAT_ram_1e20 = _DAT_ram_1e20 | 0x40;
    _DAT_ram_1bf0 = _DAT_ram_1bf0 | 1;
    _DAT_ram_1bf2 = _DAT_ram_1bf2 & 0xfffe;
    FUN_rom_006bd6();
    _DAT_ram_192a = _DAT_ram_192a | 1;
    FUN_rom_009750();
    _DAT_ram_1e22 = 3;
    return 3;
  }
  _DAT_ram_1dc6 = 0;
  return uVar2;
}





// Function: FUN_rom_00904c

/* WARNING: Heritage AFTER dead removal. Example location: W1 : 0x00905a */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* WARNING: Restarted to delay deadcode elimination for space: ram */

void FUN_rom_00904c(void)

{
  uint uVar1;
  ushort uVar2;
  undefined4 in_W1W0;
  undefined4 uVar3;
  uint uVar4;
  uint uVar5;
  bool bVar6;
  
  uVar5 = _DAT_ram_1d9e >> 2;
  uVar2 = Peripherals::ADC.ADCBUF5;
  uVar3 = FUN_rom_005d00(uVar2,(int)((ulong)in_W1W0 >> 0x10),_DAT_ram_1d2e);
  _DAT_ram_1d66 = _DAT_ram_1d2c + ((int)((ulong)uVar3 >> 0x10) << 3 | (uint)uVar3 >> 0xd);
  bVar6 = -1 < (int)(uVar5 - _DAT_ram_1d66);
  if ((bVar6 || !SBORROW2(uVar5,_DAT_ram_1d66)) && (!bVar6 || SBORROW2(uVar5,_DAT_ram_1d66))) {
    uVar5 = _DAT_ram_1d66;
  }
  uVar4 = 0x1df;
  if (_DAT_ram_1268 != 4) {
    uVar4 = 0x36f;
  }
  uVar1 = _DAT_ram_1e20 & 0x10;
  if ((_DAT_ram_1e20 & 0x10) == 0) {
    if ((uVar4 <= uVar5) && ((_DAT_ram_1da4 < 8000 || ((_DAT_ram_125a & 0x10) != 0)))) {
      _DAT_ram_1e20 = _DAT_ram_1e20 | 0x10;
      _DAT_ram_1d6c = uVar1;
      _DAT_ram_1d6e = uVar1;
      _DAT_ram_1d70 = uVar1;
      _DAT_ram_1dca = uVar1;
      _DAT_ram_1e0e = 200;
      return;
    }
  }
  else if (_DAT_ram_1e22 == 2) {
    _DAT_ram_1d70 = 0;
    _DAT_ram_1d6e = 0;
    _DAT_ram_1d6c = 0;
    if (_DAT_ram_1dca + 2 == 0 || _DAT_ram_1dca == -1) {
      _DAT_ram_1d6c = 0;
      _DAT_ram_1d6e = 0;
      _DAT_ram_1d70 = 0;
      _DAT_ram_1dca = _DAT_ram_1dca + 2;
      return;
    }
    _DAT_ram_1dca = 1;
    _DAT_ram_1bf0 = _DAT_ram_1bf0 | 1;
    _DAT_ram_1bf2 = _DAT_ram_1bf2 & 0xfffe;
    FUN_rom_006bd6();
    _DAT_ram_192a = _DAT_ram_192a | 1;
    FUN_rom_009750();
    _DAT_ram_1e22 = 3;
    return;
  }
  _DAT_ram_1dca = 0;
  return;
}





// Function: FUN_rom_0090dc

void FUN_rom_0090dc(void)

{
  ushort uVar1;
  
  uVar1 = Peripherals::PWM1.IOCON1;
  Peripherals::PWM1.IOCON1 = uVar1 & 0xfdff;
  uVar1 = Peripherals::PWM1.IOCON1;
  Peripherals::PWM1.IOCON1 = uVar1 & 0xfeff;
  uVar1 = Peripherals::PWM2.IOCON2;
  Peripherals::PWM2.IOCON2 = uVar1 & 0xfdff;
  uVar1 = Peripherals::PWM2.IOCON2;
  Peripherals::PWM2.IOCON2 = uVar1 & 0xfeff;
  uVar1 = Peripherals::PWM3.IOCON3;
  Peripherals::PWM3.IOCON3 = uVar1 & 0xfdff;
  uVar1 = Peripherals::PWM3.IOCON3;
  Peripherals::PWM3.IOCON3 = uVar1 & 0xfeff;
  return;
}





// Function: FUN_rom_009108

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

long FUN_rom_009108(void)

{
  ushort uVar1;
  
  if ((_DAT_ram_126a & 0x30) == 0x30) {
    _DAT_ram_1dce = 1;
    if (0x206c < _DAT_ram_1da4) {
      _DAT_ram_1d7e = 0x206c;
      _DAT_ram_1d80 = 0;
      _DAT_ram_1d7a = 0x206c;
      _DAT_ram_1d7c = 0;
      _DAT_ram_1d76 = 0x206c;
      _DAT_ram_1d78 = 0;
      FUN_rom_009750();
      goto LAB_rom_009148;
    }
    if (0x7fff < _DAT_ram_1da4 + 0xe05dU && SBORROW2(_DAT_ram_1da4,0x1fa3) ||
        0 < (int)(_DAT_ram_1da4 + 0xe05dU) && !SBORROW2(_DAT_ram_1da4,0x1fa3)) goto LAB_rom_009148;
  }
  else {
    if (_DAT_ram_1dce == 0) goto LAB_rom_009148;
    _DAT_ram_126a = _DAT_ram_126a & 0xffdf;
    _DAT_ram_1dce = 0;
  }
  FUN_rom_0090dc();
LAB_rom_009148:
  if ((_DAT_ram_126a & 0x400) == 0) {
    if (_DAT_ram_1dcc != 0) {
      uVar1 = Peripherals::PWM3.IOCON3;
      Peripherals::PWM3.IOCON3 = uVar1 & 0xfdff;
      _DAT_ram_1dcc = 0;
    }
    return (ulong)_DAT_ram_126a << 0x10;
  }
  _DAT_ram_1dcc = 1;
  uVar1 = Peripherals::PWM3.IOCON3;
  Peripherals::PWM3.IOCON3 = uVar1 | 0x200;
  return CONCAT22(_DAT_ram_126a,1);
}





// Function: FUN_rom_009750

void FUN_rom_009750(void)

{
  ushort uVar1;
  
  uVar1 = Peripherals::PWM1.IOCON1;
  Peripherals::PWM1.IOCON1 = uVar1 | 0x200;
  uVar1 = Peripherals::PWM1.IOCON1;
  Peripherals::PWM1.IOCON1 = uVar1 | 0x100;
  uVar1 = Peripherals::PWM2.IOCON2;
  Peripherals::PWM2.IOCON2 = uVar1 | 0x200;
  uVar1 = Peripherals::PWM2.IOCON2;
  Peripherals::PWM2.IOCON2 = uVar1 | 0x100;
  uVar1 = Peripherals::PWM3.IOCON3;
  Peripherals::PWM3.IOCON3 = uVar1 | 0x200;
  uVar1 = Peripherals::PWM3.IOCON3;
  Peripherals::PWM3.IOCON3 = uVar1 | 0x100;
  return;
}





// Function: FUN_rom_00977c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int FUN_rom_00977c(void)

{
  ushort uVar1;
  int iVar2;
  
  iVar2 = _DAT_ram_1cbe + 1;
  if (_DAT_ram_1cbe == 0) {
    uVar1 = Peripherals::PWM1.IOCON1;
    Peripherals::PWM1.IOCON1 = uVar1 & 0xfeff;
    uVar1 = Peripherals::PWM2.IOCON2;
    Peripherals::PWM2.IOCON2 = uVar1 & 0xfdff;
    _DAT_ram_1cbe = iVar2;
    return 1;
  }
  if (_DAT_ram_1cbe == 2) {
    uVar1 = Peripherals::PWM1.IOCON1;
    Peripherals::PWM1.IOCON1 = uVar1 & 0xfdff;
    uVar1 = Peripherals::PWM2.IOCON2;
    Peripherals::PWM2.IOCON2 = uVar1 & 0xfeff;
    uVar1 = Peripherals::PWM3.IOCON3;
    Peripherals::PWM3.IOCON3 = uVar1 & 0xfdff;
    uVar1 = Peripherals::PWM3.IOCON3;
    Peripherals::PWM3.IOCON3 = uVar1 & 0xfeff;
    iVar2 = 4;
    _DAT_ram_1262 = _DAT_ram_1262 | 0x2000;
  }
  _DAT_ram_1cbe = iVar2;
  return iVar2;
}





// Function: FUN_rom_0097d4

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined2 FUN_rom_0097d4(void)

{
  uint uVar1;
  undefined2 uVar2;
  bool bVar3;
  
  if ((_DAT_ram_1e18 & 0x100) == 0) {
    uVar2 = 0;
    _DAT_ram_1dee = 0;
    _DAT_ram_1df0 = 0;
  }
  else {
    uVar1 = _DAT_ram_1dee + 1;
    bVar3 = _DAT_ram_1dee != 0x3e6;
    _DAT_ram_1dee = uVar1;
    if (0x3e6 < uVar1 && bVar3) {
      _DAT_ram_1dee = 0;
      _DAT_ram_1df0 = _DAT_ram_1df0 + 1;
    }
    uVar2 = 1;
    if (_DAT_ram_1df0 < DAT_ram_1bcd) {
      uVar2 = 0;
    }
  }
  return uVar2;
}





// Function: FUN_rom_009804

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_009804(void)

{
  ushort uVar1;
  
  FUN_rom_009750();
  _DAT_ram_125a = 0;
  _DAT_ram_126a = 0;
  _DAT_ram_1cbe = 0;
  _DAT_ram_1d40 = 0xa0;
  uVar1 = Peripherals::PWM2.IOCON2;
  Peripherals::PWM2.IOCON2 = uVar1 | 0x8000;
  uVar1 = Peripherals::PWM2.IOCON2;
  Peripherals::PWM2.IOCON2 = uVar1 | 0x4000;
  _DAT_ram_1268 = 0;
  _DAT_ram_1d36 = 0x28e;
  _DAT_ram_126e = 0;
  _DAT_ram_1270 = 0;
  _DAT_ram_1d4e = 0xffb0;
  _DAT_ram_1da2 = 10000;
  _DAT_ram_1da4 = 24000;
  _DAT_ram_1d7e = 24000;
  _DAT_ram_1d80 = 0;
  _DAT_ram_1d7a = 24000;
  _DAT_ram_1d7c = 0;
  _DAT_ram_1d76 = 24000;
  _DAT_ram_1d78 = 0;
  _DAT_ram_1e3a = 0x5dc;
  if ((((_DAT_ram_1e10 == 0) && ((_DAT_ram_1e1c & 0x800) != 0)) && ((_DAT_ram_1bf2 & 8) != 0)) &&
     ((_DAT_ram_1e1c & 2) != 0)) {
    if ((_DAT_ram_1e1e & 1) == 0) {
      _DAT_ram_1e1c = _DAT_ram_1e1c | 8;
      _DAT_ram_1bea = _DAT_ram_1bea & 0xfffb;
    }
    else {
      _DAT_ram_1e1e = _DAT_ram_1e1e & 0xfffe;
      _DAT_ram_1e1c = _DAT_ram_1e1c & 0xfff7;
    }
    _DAT_ram_1e0a = 0;
    _DAT_ram_1e22 = 1;
  }
  _DAT_ram_1262 = _DAT_ram_1262 & 0x9fff;
  return;
}





// Function: FUN_rom_009872

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined2 FUN_rom_009872(void)

{
  _DAT_ram_1e0a = _DAT_ram_1e0a + 1;
  if (_DAT_ram_1e0a != 0) {
    _DAT_ram_1e0e = 0xa0;
    _DAT_ram_1264 = _DAT_ram_1264 & 0xdfff;
    _DAT_ram_1e1c = _DAT_ram_1e1c & 0xfffb;
    _DAT_ram_1e20 = 0;
    _DAT_ram_1bee = 0;
    _DAT_ram_1bf0 = 0;
    _DAT_ram_1bec = 0;
    if (((_DAT_ram_1e18 & 0x800) != 0) || (_DAT_ram_1d4a = 0xc4d, (_DAT_ram_1e1e & 2) != 0)) {
      _DAT_ram_1d4a = 0x6b3;
    }
    _DAT_ram_1e22 = 2;
    _DAT_ram_1262 = _DAT_ram_1262 | 0x4000;
    _DAT_ram_1e18 = _DAT_ram_1e18 & 0x7fff;
  }
  Peripherals::OC2.OC2RS = 400;
  _DAT_ram_1e38 = 400;
  return 400;
}





// Function: FUN_rom_0098a6

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_0098a6(void)

{
  byte bVar1;
  uint uVar2;
  undefined1 *puVar3;
  
  if (((_DAT_ram_1264 & 0x2000) != 0) && (_DAT_ram_1e20 != 0)) {
    _DAT_ram_1264 = _DAT_ram_1264 & 0xdfff;
    if ((_DAT_ram_1e20 & 10) != 0) {
      _DAT_ram_1e1e = _DAT_ram_1e1e | 3;
    }
    _DAT_ram_125a = 0;
    _DAT_ram_126a = 0;
    _DAT_ram_1bf0 = _DAT_ram_1bf0 | 1;
    _DAT_ram_1bf2 = _DAT_ram_1bf2 & 0xfffe;
    FUN_rom_006bd6(_DAT_ram_1e20 & 10);
    _DAT_ram_192a = _DAT_ram_192a | 1;
  }
  if ((int)_DAT_ram_1e18 < 0) goto LAB_rom_0098e4;
  if ((_DAT_ram_1e18 & 0x800) == 0) {
LAB_rom_0098de:
    _DAT_ram_1e18 = _DAT_ram_1e18 | 0xc000;
  }
  else {
    if (_DAT_ram_1268 == 3) {
      bVar1 = Peripherals::GPIOD.LATD._0_1_;
      if ((bVar1 & 0x10) == 0) goto LAB_rom_0098de;
    }
    _DAT_ram_1e18 = _DAT_ram_1e18 & 0xbfff | 0x8000;
  }
LAB_rom_0098e4:
  _DAT_ram_125a = 0;
  _DAT_ram_126a = 0;
  _DAT_ram_1268 = 0;
  _DAT_ram_1262 = _DAT_ram_1262 & 0xbfff;
  FUN_rom_009750();
  _DAT_ram_1d4e = 0xffb0;
  puVar3 = &DAT_ram_0802;
  if (((undefined1 *)(_DAT_ram_1e1c & 0x802) != &DAT_ram_0802) || ((_DAT_ram_1262 & 1) != 0)) {
    _DAT_ram_1e10 = 0x9c4;
    _DAT_ram_1e22 = 0;
    return 0x80209c4;
  }
  if ((((_DAT_ram_1262 & 0x40) == 0) || ((_DAT_ram_1e18 & 0x4000) == 0)) &&
     (((_DAT_ram_1262 & 8) == 0 || ((_DAT_ram_1e18 & 0x4000) != 0)))) {
    uVar2 = _DAT_ram_1262;
    if ((_DAT_ram_1e1e & 1) != 0) {
      puVar3 = (undefined1 *)(_DAT_ram_1e20 & 10);
      uVar2 = _DAT_ram_1e20;
      if (puVar3 == (undefined1 *)0x0) {
        uVar2 = 0x9c4;
        _DAT_ram_1e10 = 0x9c4;
        _DAT_ram_1e22 = 0;
      }
    }
    return CONCAT22(puVar3,uVar2);
  }
  _DAT_ram_1e22 = 0;
  _DAT_ram_1e10 = 0x9c4;
  _DAT_ram_1262 = _DAT_ram_1262 & 0xff97;
  return 0x80209c4;
}





// Function: FUN_rom_00993a

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

uint FUN_rom_00993a(void)

{
  uint uVar1;
  bool bVar2;
  
  if ((_DAT_ram_1e18 & 0x100) != 0) {
    _DAT_ram_1e18 = _DAT_ram_1e18 & 0xfeff;
    _DAT_ram_1df4 = 0;
    _DAT_ram_1df2 = 0;
    _DAT_ram_1d4e = 0xffb0;
  }
  uVar1 = _DAT_ram_1df2 + 1;
  bVar2 = _DAT_ram_1df2 != 0x3e6;
  _DAT_ram_1df2 = uVar1;
  if (0x3e6 < uVar1 && bVar2) {
    _DAT_ram_1df2 = 0;
    _DAT_ram_1df4 = _DAT_ram_1df4 + 1;
  }
  if (DAT_ram_1bcc == 0xfa) {
    uVar1 = _DAT_ram_1e1c;
    if (((_DAT_ram_1e1c & 0x20) == 0) && (((_DAT_ram_1bf2 & 8) == 0 || ((_DAT_ram_1e1c & 2) == 0))))
    {
      _DAT_ram_1e10 = 0x32;
      _DAT_ram_1e22 = _DAT_ram_1e1c & 0x20;
      return 0;
    }
  }
  else {
    uVar1 = _DAT_ram_1df4;
    if (DAT_ram_1bcc <= _DAT_ram_1df4) {
      uVar1 = 0x32;
      _DAT_ram_1e10 = 0x32;
      _DAT_ram_1e22 = 0;
    }
  }
  return uVar1;
}





// Function: FUN_rom_009986

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

byte FUN_rom_009986(void)

{
  byte bVar1;
  
  bVar1 = Peripherals::GPIOF.LATF._0_1_;
  if ((bVar1 & 2) == 0) {
    bVar1 = 0x32;
    _DAT_ram_1e10 = 0x32;
    _DAT_ram_1e22 = 0;
  }
  return bVar1;
}





// Function: FUN_rom_009994

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_009994(void)

{
  ushort uVar1;
  
  if (((_DAT_ram_1bea & 0x800) != 0) && ((_DAT_ram_1266 & 0x80) != 0)) {
    uVar1 = Peripherals::GPIOF.LATF;
    Peripherals::GPIOF.LATF = uVar1 | 2;
    return;
  }
  uVar1 = Peripherals::GPIOF.LATF;
  Peripherals::GPIOF.LATF = uVar1 & 0xfffd;
  return;
}





// Function: FUN_rom_0099bc

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_0099bc(undefined2 param_1,undefined2 param_2)

{
  uint uVar1;
  
  if (_DAT_ram_1cc0 == 0) {
    if (_DAT_ram_1e4c < 0x24) {
      uVar1 = 0x23;
    }
    else {
      uVar1 = _DAT_ram_1e4c;
      if (0xe7 < _DAT_ram_1e4c) {
        uVar1 = 0xe8;
      }
    }
    _DAT_ram_1e16 = (uint)(byte)(&DAT_ram_1cde)[-uVar1];
    return CONCAT22(0x1bf6,(uint)(byte)(&DAT_ram_1cde)[-uVar1]);
  }
  _DAT_ram_1cc0 = _DAT_ram_1cc0 + -1;
  return CONCAT22(param_2,_DAT_ram_1cc0);
}





// Function: FUN_rom_0099ea

/* WARNING: Heritage AFTER dead removal. Example location: W1 : 0x009abe */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* WARNING: Restarted to delay deadcode elimination for space: ram */

void FUN_rom_0099ea(void)

{
  byte bVar1;
  uint uVar2;
  int iVar3;
  undefined4 in_W1W0;
  uint uVar4;
  ulong uVar5;
  bool bVar6;
  
  if (_DAT_ram_1cc0 == 0) {
    if (0x23 < _DAT_ram_1e4a) {
      in_W1W0 = CONCAT22(0xe8,(int)in_W1W0);
    }
    uVar4 = (uint)((ulong)in_W1W0 >> 0x10);
    uVar5 = (ulong)CONCAT12((&DAT_ram_1cde)[-uVar4],_DAT_ram_1e4a);
    _DAT_ram_1e14 = (uint)(byte)(&DAT_ram_1cde)[-uVar4];
    _DAT_ram_1cc6 = (int)in_W1W0;
    _DAT_ram_1cc4 = 0x42;
    uVar2 = 0x42;
    if ((9 < _DAT_ram_1cc6) && (uVar2 = 0x44, _DAT_ram_1cc4 = uVar2, 0x19 < _DAT_ram_1cc6)) {
      if (0x7fff < _DAT_ram_1cc6 - 0x29U && SBORROW2(_DAT_ram_1cc6,0x29) ||
          0 < (int)(_DAT_ram_1cc6 - 0x29U) && !SBORROW2(_DAT_ram_1cc6,0x29)) {
        if (0x7fff < _DAT_ram_1cc6 - 0x31U && SBORROW2(_DAT_ram_1cc6,0x31) ||
            0 < (int)(_DAT_ram_1cc6 - 0x31U) && !SBORROW2(_DAT_ram_1cc6,0x31)) {
          _DAT_ram_1cc4 = 0x55;
          uVar2 = _DAT_ram_1cc4;
        }
        else {
          uVar2 = (_DAT_ram_1cc6 + -0x2a) * 0x514;
          _DAT_ram_1cc4 = ((int)uVar2 >> 10) + 0x50;
          if ((int)uVar2 >> 10 != 5 && 0x54 < (int)_DAT_ram_1cc4) {
            _DAT_ram_1cc4 = 0x55;
          }
        }
      }
      else {
        _DAT_ram_1cc4 = _DAT_ram_1cc6 + 0x26;
        if ((_DAT_ram_1cc6 - 0x1dU < 0x8000 || !SBORROW2(_DAT_ram_1cc4,0x43)) &&
            ((int)(_DAT_ram_1cc6 - 0x1dU) < 1 || SBORROW2(_DAT_ram_1cc4,0x43))) {
          _DAT_ram_1cc4 = 0x44;
          uVar2 = _DAT_ram_1cc4;
        }
      }
    }
    if (((_DAT_ram_126c < 0x14) || (bVar1 = Peripherals::GPIOF.LATF._0_1_, (bVar1 & 1) != 0)) ||
       (_DAT_ram_1268 != 3 && _DAT_ram_1268 != 4)) {
      _DAT_ram_1dfa = 0;
      _DAT_ram_1dfc = 0;
      _DAT_ram_1cc2 = 0x69;
    }
    else {
      iVar3 = _DAT_ram_1dfa + 1;
      bVar6 = _DAT_ram_1dfa != 999;
      _DAT_ram_1dfa = iVar3;
      if (bVar6 && 999 < iVar3) {
        _DAT_ram_1dfa = 1000;
        _DAT_ram_1cc2 = _DAT_ram_1cc4;
      }
      FUN_rom_005d00(_DAT_ram_126c,uVar4,_DAT_ram_1e4e,0);
      _DAT_ram_1df6 = uVar2;
      _DAT_ram_1df8 = uVar4;
      if (uVar2 == 0x2183 && uVar4 == uVar2 < 0x2183 || (int)uVar4 < (int)(uint)(uVar2 < 0x2183)) {
        _DAT_ram_1dfc = 0;
      }
      else {
        iVar3 = _DAT_ram_1dfc + 1;
        bVar6 = _DAT_ram_1dfc != 0x4af;
        _DAT_ram_1dfc = iVar3;
        if (bVar6 && 0x4af < iVar3) {
          _DAT_ram_1dfc = 0x4b0;
          _DAT_ram_1e20 = _DAT_ram_1e20 | 0x400;
        }
      }
    }
    if ((_DAT_ram_1cc2 <= (uint)(uVar5 >> 0x10)) || ((uint)uVar5 < 0x29)) {
      _DAT_ram_1e20 = _DAT_ram_1e20 | 0x400;
    }
  }
  return;
}





// Function: FUN_rom_009afc

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ulong FUN_rom_009afc(int param_1,uint param_2)

{
  byte bVar1;
  uint uVar2;
  
  if (_DAT_ram_1cc0 == 0) {
    if (_DAT_ram_1e48 < 0x24) {
      uVar2 = 0x23;
    }
    else {
      uVar2 = 0xe8;
      if (_DAT_ram_1e48 < 0xe8) {
        uVar2 = _DAT_ram_1e48;
      }
    }
    bVar1 = (&DAT_ram_1cde)[-uVar2];
    param_2 = (uint)bVar1;
    _DAT_ram_1e12 = param_2;
    if ((int)_DAT_ram_1e1a < 0) {
      param_1 = 0x55;
      if (param_2 < 0x56) {
        param_1 = 0xf5;
        if (_DAT_ram_1e48 < 0xf6) {
          _DAT_ram_1e1a = _DAT_ram_1e1a & 0x7f7f;
          return (ulong)CONCAT12(bVar1,0xf5);
        }
      }
    }
    else {
      param_1 = _DAT_ram_1268;
      if ((_DAT_ram_1e1a & 0x80) == 0) {
        if (_DAT_ram_1268 == 3) {
          param_1 = 0x68;
          if (param_2 < 0x69) {
            param_1 = 0xf9;
            if (_DAT_ram_1e48 < 0xfa) goto LAB_rom_009b90;
          }
          _DAT_ram_192a = _DAT_ram_192a | 1;
          _DAT_ram_1e1a = _DAT_ram_1e1a | 0x80;
          return (ulong)CONCAT12(bVar1,param_1);
        }
        if (_DAT_ram_1268 == 4) {
          param_1 = 0x6a;
          if (param_2 < 0x6b) {
            param_1 = 0xf9;
            if (_DAT_ram_1e48 < 0xfa) goto LAB_rom_009b90;
          }
          _DAT_ram_1e1a = _DAT_ram_1e1a | 0x8000;
        }
      }
      else {
        _DAT_ram_1e1e = _DAT_ram_1e1e | 4;
        if (param_2 < 0x56) {
          if (_DAT_ram_1e48 < 0xf6) {
            _DAT_ram_1e1a = _DAT_ram_1e1a & 0x7f7f;
          }
        }
        if (_DAT_ram_1268 == 4) {
          param_1 = 0x6a;
          if (param_2 < 0x6b) {
            param_1 = 0xf9;
            if (_DAT_ram_1e48 < 0xfa) goto LAB_rom_009b90;
          }
          _DAT_ram_1e1a = _DAT_ram_1e1a | 0x8000;
          return (ulong)CONCAT12(bVar1,param_1);
        }
      }
    }
  }
LAB_rom_009b90:
  return CONCAT22(param_2,param_1);
}





// Function: FUN_rom_009b92

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_009b92(undefined2 param_1,uint param_2)

{
  int iVar1;
  uint uVar2;
  
  if ((_DAT_ram_1264 & 0x4000) == 0) {
    if ((_DAT_ram_1e1a & 0x20) == 0) {
      iVar1 = _DAT_ram_1268;
      uVar2 = param_2;
      if (_DAT_ram_1268 == 3) {
        iVar1 = 0x66;
        uVar2 = _DAT_ram_1d16;
        if (0x66 < _DAT_ram_1d16) {
          _DAT_ram_192a = _DAT_ram_192a | 1;
          _DAT_ram_1e1a = _DAT_ram_1e1a | 0x20;
        }
      }
      goto LAB_rom_009bba;
    }
    _DAT_ram_1e1e = _DAT_ram_1e1e | 4;
  }
  iVar1 = 0x55;
  uVar2 = _DAT_ram_1d16;
  if (_DAT_ram_1d16 < 0x56) {
    _DAT_ram_1e1a = _DAT_ram_1e1a & 0xffdf;
    return CONCAT22(_DAT_ram_1d16,0x55);
  }
LAB_rom_009bba:
  return CONCAT22(uVar2,iVar1);
}





// Function: FUN_rom_009bbc

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_009bbc(void)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  byte bVar4;
  
  bVar4 = Peripherals::GPIOC.PORTC._1_1_;
  uVar1 = bVar4 >> 5 & 2;
  bVar4 = Peripherals::GPIOC.PORTC._1_1_;
  uVar2 = bVar4 >> 3 & 4;
  bVar4 = Peripherals::GPIOD.PORTD._1_1_;
  uVar3 = (bVar4 & 4) << 1;
  _DAT_ram_1e08 = _DAT_ram_1e08 & 0xfff1 | uVar1 | uVar2 | uVar3;
  uVar1 = (uVar1 | uVar2 | uVar3 | 0xb0) >> 1;
  Peripherals::I2C2.I2C2ADD = uVar1;
  return CONCAT22(uVar3,uVar1);
}





// Function: FUN_rom_009bee

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_009bee(void)

{
  byte bVar1;
  GPIOE *pGVar2;
  uint uVar3;
  ushort uVar4;
  bool bVar5;
  
  if ((_DAT_ram_1bf2 & 8) == 0) {
    uVar4 = Peripherals::GPIOD.LATD;
    Peripherals::GPIOD.LATD = uVar4 | 8;
    _DAT_ram_1e0c = 0;
    goto LAB_rom_009c80;
  }
  if ((_DAT_ram_1e22 == 2) && ((_DAT_ram_1e20 & 0x100) == 0)) {
    if ((_DAT_ram_1e1a & 0xa0) == 0) {
LAB_rom_009c10:
      if ((_DAT_ram_1268 == 4) && ((_DAT_ram_1e20 & 2) != 0)) goto LAB_rom_009c76;
    }
    else if (_DAT_ram_1268 == 4) {
      if ((_DAT_ram_1e18 & 0x800) == 0) goto LAB_rom_009c76;
      goto LAB_rom_009c10;
    }
    if (((_DAT_ram_1e1c & 2) == 0) && (bVar1 = Peripherals::GPIOD.LATD._0_1_, (bVar1 & 8) == 0)) {
      bVar1 = Peripherals::GPIOD.LATD._0_1_;
      if ((bVar1 & 0x10) == 0) {
        uVar4 = Peripherals::ADC.ADCBUF5;
        uVar3 = 0x2d9;
      }
      else {
        uVar4 = Peripherals::ADC.ADCBUF5;
        uVar3 = 0x153;
      }
      _DAT_ram_1e0c = 0;
      if (uVar4 <= uVar3) {
        uVar4 = Peripherals::GPIOD.LATD;
        Peripherals::GPIOD.LATD = uVar4 | 8;
      }
    }
    else if ((_DAT_ram_1e1a & 0xa0) == 0) {
      if (_DAT_ram_1268 == 4) {
        uVar4 = Peripherals::ADC.ADCBUF5;
        if (uVar4 < 0x17a) goto LAB_rom_009c80;
      }
      else {
        if (_DAT_ram_1268 != 3) goto LAB_rom_009c80;
        pGVar2 = (GPIOE *)Peripherals::ADC.ADCBUF5;
        if (pGVar2 < &Peripherals::GPIOE) {
          _DAT_ram_1e0c = 0;
          goto LAB_rom_009c80;
        }
      }
      uVar3 = _DAT_ram_1e0c + 1;
      bVar5 = _DAT_ram_1e0c != 0x3a;
      _DAT_ram_1e0c = uVar3;
      if (0x3a < uVar3 && bVar5) {
        _DAT_ram_1e0c = 0x3c;
        uVar4 = Peripherals::GPIOD.LATD;
        Peripherals::GPIOD.LATD = uVar4 & 0xfff7;
      }
    }
  }
  else {
LAB_rom_009c76:
    uVar4 = Peripherals::GPIOD.LATD;
    Peripherals::GPIOD.LATD = uVar4 | 8;
    _DAT_ram_1e0c = 0;
  }
LAB_rom_009c80:
  bVar1 = Peripherals::GPIOD.LATD._0_1_;
  if ((bVar1 & 8) == 0) {
    _DAT_ram_1bf2 = _DAT_ram_1bf2 | 1;
    return;
  }
  _DAT_ram_1bf2 = _DAT_ram_1bf2 & 0xfffe;
  return;
}





// Function: FUN_rom_009c8e

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined2 FUN_rom_009c8e(void)

{
  DAT_ram_1d08 = 0x48;
  DAT_ram_1d09 = 0x30;
  DAT_ram_1d0a = 0x35;
  DAT_ram_1d0b = 0x57;
  DAT_ram_1d0c = 0x44;
  _DAT_ram_1e4c = 0xc1;
  _DAT_ram_1e48 = 0xc1;
  _DAT_ram_1e16 = 0x19;
  _DAT_ram_1e12 = 0x19;
  _DAT_ram_1d18 = 0x19;
  _DAT_ram_1e10 = 10;
  _DAT_ram_1d3e = 0x15e;
  _DAT_ram_1e22 = 0;
  Peripherals::OC2.OC2RS = 400;
  _DAT_ram_1e38 = 400;
  return 400;
}





// Function: FUN_rom_009cc2

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_009cc2(void)

{
  byte bVar1;
  
  if ((_DAT_ram_1e1a & 1) != 0) {
    bVar1 = Peripherals::I2C2.I2C2STAT._0_1_;
    if ((bVar1 & 0x10) != 0) {
      _DAT_ram_1e1a = _DAT_ram_1e1a & 0xfffe;
      FUN_rom_00a6e4();
    }
  }
  return;
}





// Function: FUN_rom_009cd4

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_009cd4(undefined2 param_1,undefined2 param_2)

{
  uint uVar1;
  undefined4 uVar2;
  bool bVar3;
  
  if ((_DAT_ram_1e1a & 0x200) == 0) {
    _DAT_ram_1dfe = 0;
    _DAT_ram_1e00 = 0;
  }
  else {
    uVar1 = _DAT_ram_1dfe + 1;
    _DAT_ram_1e00 = _DAT_ram_1e00 + (0xfffe < _DAT_ram_1dfe);
    param_1 = 0x773f;
    param_2 = 0x1b;
    bVar3 = _DAT_ram_1dfe != 0x773e;
    _DAT_ram_1dfe = uVar1;
    if ((0x1a < _DAT_ram_1e00 && (uVar1 < 0x773f) + 0x1b <= _DAT_ram_1e00) &&
        (bVar3 || _DAT_ram_1e00 - 0x1b != (uint)(uVar1 < 0x773f))) {
      _DAT_ram_1e1a = _DAT_ram_1e1a & 0xfdff;
      FUN_rom_00a688(0x77ff,0xff,uVar1 & 0xff00);
      uVar2 = FUN_rom_00a6b8();
      _DAT_ram_1bbc = (int)uVar2;
      return uVar2;
    }
  }
  return CONCAT22(param_2,param_1);
}





// Function: FUN_rom_009d08

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_009d08(void)

{
  byte bVar1;
  ushort uVar2;
  uint uVar3;
  bool bVar4;
  
  if ((_DAT_ram_1262 & 0x4000) == 0) {
    uVar2 = Peripherals::GPIOE.LATE;
    Peripherals::GPIOE.LATE = uVar2 & 0xffbf;
    uVar3 = 0;
    _DAT_ram_1e02 = 0;
  }
  else {
    bVar1 = Peripherals::GPIOE.LATE._0_1_;
    uVar3 = CONCAT11((char)((_DAT_ram_1262 & 0x4000) >> 8),bVar1);
    if ((bVar1 & 0x40) == 0) {
      uVar3 = _DAT_ram_1e02 + 1;
      bVar4 = _DAT_ram_1e02 != 0x1c;
      _DAT_ram_1e02 = uVar3;
      if (0x1c < uVar3 && bVar4) {
        _DAT_ram_1e02 = 0x1e;
        uVar2 = Peripherals::GPIOE.LATE;
        Peripherals::GPIOE.LATE = uVar2 | 0x40;
        return CONCAT22(_DAT_ram_1262,0x1e);
      }
    }
  }
  return CONCAT22(_DAT_ram_1262,uVar3);
}





// Function: FUN_rom_009d2c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_009d2c(void)

{
  byte bVar1;
  uint uVar2;
  
  if (0x13e < _DAT_ram_125e) {
    _DAT_ram_1e1c = _DAT_ram_1e1c | 0x20;
    return CONCAT22(_DAT_ram_125e,0x13e);
  }
  uVar2 = 0xff;
  if (_DAT_ram_125e < 0x100) {
    _DAT_ram_1e1c = _DAT_ram_1e1c & 0xffdf;
    bVar1 = Peripherals::GPIOF.LATF._0_1_;
    uVar2 = (uint)bVar1;
    if ((bVar1 & 2) != 0) {
      _DAT_ram_1bea = _DAT_ram_1bea & 0xf7ff;
    }
  }
  return CONCAT22(_DAT_ram_125e,uVar2);
}





// Function: FUN_rom_009d4a

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_009d4a(void)

{
  uint uVar1;
  byte bVar2;
  uint uVar3;
  uint uVar4;
  bool bVar5;
  
  uVar1 = _DAT_ram_1e20 & 1;
  uVar4 = _DAT_ram_1e1c;
  if (uVar1 == 0) {
    _DAT_ram_1262 = _DAT_ram_1262 & 0xfffe;
    _DAT_ram_1e06 = 0;
    if ((_DAT_ram_1e1c & 0x80) == 0) {
      uVar3 = 0;
      _DAT_ram_1e04 = 0;
    }
    else {
      bVar2 = Peripherals::GPIOD.LATD._0_1_;
      uVar3 = (uint)bVar2;
      if ((bVar2 & 8) == 0) {
        uVar4 = _DAT_ram_1e04 + 1;
        uVar3 = 999;
        bVar5 = _DAT_ram_1e04 != 0x3e6;
        _DAT_ram_1e04 = uVar4;
        if (0x3e6 < uVar4 && bVar5) {
          _DAT_ram_1e1c = _DAT_ram_1e1c & 0xff7f;
          _DAT_ram_1e06 = uVar1;
          return CONCAT22(uVar4,999);
        }
      }
    }
  }
  else {
    uVar3 = uVar1;
    if ((_DAT_ram_1e1c & 0x80) == 0) {
      uVar4 = _DAT_ram_1e06 + 1;
      uVar3 = 7999;
      bVar5 = _DAT_ram_1e06 != 0x1f3e;
      _DAT_ram_1e06 = uVar4;
      if (0x1f3e < uVar4 && bVar5) {
        _DAT_ram_1262 = _DAT_ram_1262 | 1;
        _DAT_ram_1e1c = _DAT_ram_1e1c | 0x80;
        return CONCAT22(uVar4,7999);
      }
    }
  }
  return CONCAT22(uVar4,uVar3);
}





// Function: FUN_rom_009d8c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_009d8c(void)

{
  byte bVar1;
  char cVar2;
  uint uVar3;
  
  if (_DAT_ram_1e20 == 4) {
    if ((_DAT_ram_1bea & 0x20) == 0) goto LAB_rom_009db8;
    uVar3 = 4;
  }
  else {
    if (_DAT_ram_1e20 == 0) goto LAB_rom_009db8;
    uVar3 = _DAT_ram_1e20 & 10;
    if (uVar3 != 0) {
      _DAT_ram_1e1e = _DAT_ram_1e1e | 3;
    }
  }
  _DAT_ram_125a = 0;
  _DAT_ram_126a = 0;
  _DAT_ram_1bf0 = _DAT_ram_1bf0 | 1;
  _DAT_ram_1bf2 = _DAT_ram_1bf2 & 0xfffe;
  FUN_rom_006bd6(uVar3);
  _DAT_ram_192a = _DAT_ram_192a | 1;
  FUN_rom_009750();
  _DAT_ram_1e22 = 3;
LAB_rom_009db8:
  cVar2 = FUN_rom_0097d4();
  if (cVar2 == '\0') {
    _DAT_ram_1264 = _DAT_ram_1264 & 0xefff;
  }
  else {
    _DAT_ram_1264 = _DAT_ram_1264 | 0x1000;
  }
  if (((_DAT_ram_1264 & 0x1000) != 0) && ((_DAT_ram_1e1c & 2) == 0)) {
    _DAT_ram_1bf2 = _DAT_ram_1bf2 & 0xfffe;
    FUN_rom_006bd6();
    _DAT_ram_192a = _DAT_ram_192a | 1;
    FUN_rom_009750();
    _DAT_ram_1e22 = 5;
    _DAT_ram_1264 = _DAT_ram_1264 & 0xefff;
  }
  bVar1 = Peripherals::GPIOF.LATF._0_1_;
  if ((bVar1 & 2) != 0) {
    FUN_rom_009750();
    _DAT_ram_1e22 = 6;
  }
  if (_DAT_ram_1e0e != 0) {
    _DAT_ram_1e0e = _DAT_ram_1e0e + -1;
  }
  if (_DAT_ram_1d40 != 0) {
    _DAT_ram_1d40 = _DAT_ram_1d40 + -1;
  }
  return;
}





// Function: FUN_rom_009dfe

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_009dfe(void)

{
  ushort uVar1;
  byte bVar2;
  
  bVar2 = Peripherals::INTERRUPT.IFS1._1_1_;
  if ((bVar2 & 8) != 0) {
    if (_DAT_ram_1e22 == 2) {
      FUN_rom_009d8c();
    }
    else if (_DAT_ram_1e22 < 2) {
      if (_DAT_ram_1e22 == 0) {
        FUN_rom_009804();
      }
      else if (_DAT_ram_1e22 == 1) {
        FUN_rom_009872();
      }
    }
    else if (_DAT_ram_1e22 == 5) {
      FUN_rom_00993a();
    }
    else if (_DAT_ram_1e22 == 6) {
      FUN_rom_009986();
    }
    else if (_DAT_ram_1e22 == 3) {
      FUN_rom_0098a6();
    }
    FUN_rom_0099bc();
    FUN_rom_0099ea();
    FUN_rom_009afc();
    FUN_rom_009b92();
    _DAT_ram_1e20 = _DAT_ram_1e20 & 0xfffd | (_DAT_ram_1264 >> 0xe & 1 | _DAT_ram_1e1a >> 0xf) * 2;
    FUN_rom_00a31e();
    FUN_rom_0075fa();
    FUN_rom_009bbc();
    FUN_rom_009bee();
    FUN_rom_009d08();
    FUN_rom_00a06c();
    FUN_rom_009994();
    FUN_rom_00875e();
    FUN_rom_008cc2();
    FUN_rom_008b0e();
    FUN_rom_008ec8();
    FUN_rom_009cd4();
    FUN_rom_009d2c();
    FUN_rom_009d4a();
    uVar1 = Peripherals::INTERRUPT.IFS1;
    Peripherals::INTERRUPT.IFS1 = uVar1 & 0xf7ff;
  }
  return;
}





// Function: FUN_rom_009e88

void FUN_rom_009e88(void)

{
  ushort uVar1;
  byte bVar2;
  
  Peripherals::TIMER3.T3CON = 0;
  Peripherals::TIMER3.TMR3 = 0;
  Peripherals::TIMER3.PR3 = 2000;
  uVar1 = Peripherals::TIMER3.T3CON;
  Peripherals::TIMER3.T3CON = uVar1 | 0x8000;
  Peripherals::OC2.OC2CON = 8;
  Peripherals::OC2.OC2R = 0;
  Peripherals::OC2.OC2RS = 400;
  Peripherals::OC2.OC2CON = 0xe;
  Peripherals::PWM_MASTER.PTCON = 0x400;
  bVar2 = Peripherals::INTERRUPT.IPC14._0_1_;
  Peripherals::INTERRUPT.IPC14._0_1_ = bVar2 & 0x8f | 0x50;
  uVar1 = Peripherals::INTERRUPT.IEC3;
  Peripherals::INTERRUPT.IEC3 = uVar1 | 0x200;
  Peripherals::PWM_MASTER.SEVTCMP = 0;
  bVar2 = Peripherals::PWM_MASTER.PTCON2._0_1_;
  Peripherals::PWM_MASTER.PTCON2._0_1_ = bVar2 & 0xf8 | 3;
  Peripherals::PWM_MASTER.PTPER = 0x11e;
  Peripherals::PWM1.IOCON1 = 0xc800;
  Peripherals::PWM1.PWMCON1 = 0x1001;
  Peripherals::PWM1.PDC1 = 0x126;
  Peripherals::PWM1.DTR1 = 0x2a;
  Peripherals::PWM1.ALTDTR1 = 0x2a;
  Peripherals::PWM1.PHASE1 = 0;
  Peripherals::PWM2.IOCON2 = 0xc800;
  Peripherals::PWM2.PWMCON2 = 0x1001;
  Peripherals::PWM2.PDC2 = 0x126;
  Peripherals::PWM2.DTR2 = 0x2a;
  Peripherals::PWM2.ALTDTR2 = 0x2a;
  Peripherals::PWM2.PHASE2 = 0;
  Peripherals::PWM3.IOCON3 = 0xc800;
  Peripherals::PWM3.PWMCON3 = 0x1001;
  Peripherals::PWM3.PDC3 = 0x126;
  Peripherals::PWM3.DTR3 = 0x2f;
  Peripherals::PWM3.ALTDTR3 = 0x2f;
  Peripherals::PWM3.PHASE3 = 0x82;
  Peripherals::PWM5.PWMCON5 = 0x280;
  Peripherals::PWM5.PHASE5 = 0x49b;
  Peripherals::PWM5.PDC5 = 0x372;
  Peripherals::PWM5.IOCON5 = 0x8c00;
  Peripherals::PWM1.FCLCON1 = 0x10;
  Peripherals::PWM2.FCLCON2 = 0x10;
  Peripherals::PWM3.FCLCON3 = 0x10;
  FUN_rom_009750(0x10,0x126,0x1001,0xc800,0);
  uVar1 = Peripherals::PWM_MASTER.PTCON;
  Peripherals::PWM_MASTER.PTCON = uVar1 | 0x8000;
  return;
}





// Function: FUN_rom_009f12

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_rom_009f12(uint *param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4)

{
  byte bVar1;
  uint uVar2;
  OC4 *pOVar3;
  
  if (_DAT_ram_1e22 == 2) {
    bVar1 = Peripherals::GPIOD.LATD._0_1_;
    uVar2 = (uint)bVar1;
    if ((bVar1 & 0x10) != 0) {
      pOVar3 = (OC4 *)*param_1;
      param_4 = 0x191;
      if (pOVar3 < &Peripherals::OC4) {
LAB_rom_009f2e:
        *param_1 = 0x191;
        return CONCAT26(pOVar3,CONCAT24(uVar2,CONCAT22(0x191,param_1)));
      }
      uVar2 = 0xf000;
      if ((OC4 *)0xf000 < pOVar3) goto LAB_rom_009f2e;
      goto LAB_rom_009f48;
    }
  }
  pOVar3 = (OC4 *)*param_1;
  if (pOVar3 < (OC4 *)0x2) {
    *param_1 = 1;
    return CONCAT26(pOVar3,CONCAT24(1,CONCAT22(param_4,param_1)));
  }
  uVar2 = 0xf000;
  if ((OC4 *)0xf000 < pOVar3) {
    uVar2 = 1;
    *param_1 = 1;
  }
LAB_rom_009f48:
  return CONCAT26(pOVar3,CONCAT24(uVar2,CONCAT22(param_4,param_1)));
}





// Function: FUN_rom_009f4a

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_009f4a(void)

{
  uint uVar1;
  
  uVar1 = (uint)(_DAT_ram_1e4e * 0xb) / 0x22f;
  if (10 < uVar1) {
    uVar1 = 10;
  }
  return CONCAT22(uVar1,uVar1);
}





// Function: FUN_rom_009f64

undefined2 FUN_rom_009f64(int param_1,int param_2)

{
  undefined2 uVar1;
  bool bVar2;
  
  param_1 = param_1 - param_2;
  uVar1 = 0;
  if ((param_1 - 5U < 0x8000 || !SBORROW2(param_1,5)) &&
      ((int)(param_1 - 5U) < 1 || SBORROW2(param_1,5))) {
    uVar1 = 1;
    if ((param_1 - 1U < 0x8000 || !SBORROW2(param_1,1)) &&
        ((int)(param_1 - 1U) < 1 || SBORROW2(param_1,1))) {
      uVar1 = 4;
      bVar2 = -1 < param_1 + 5;
      if ((bVar2 || SCARRY2(param_1,5)) && (!bVar2 || !SCARRY2(param_1,5))) {
        uVar1 = 3;
        bVar2 = -1 < param_1 + 1;
        if ((bVar2 || SCARRY2(param_1,1)) && (!bVar2 || !SCARRY2(param_1,1))) {
          uVar1 = 2;
        }
      }
    }
  }
  return uVar1;
}





// Function: FUN_rom_009f98

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined2 FUN_rom_009f98(int *param_1,int param_2,int param_3,int param_4)

{
  int iVar1;
  int *piVar2;
  int iVar3;
  bool bVar4;
  
  iVar3 = 0;
  piVar2 = param_1;
  while( true ) {
    iVar1 = *piVar2;
    bVar4 = -1 < param_4 - iVar1;
    if ((bVar4 || !SBORROW2(param_4,iVar1)) && (!bVar4 || SBORROW2(param_4,iVar1))) break;
    iVar3 = iVar3 + 1;
    piVar2 = piVar2 + 1;
  }
  bVar4 = -1 < iVar3 - param_3;
  if ((bVar4 || SBORROW2(iVar3,param_3)) && (!bVar4 || !SBORROW2(iVar3,param_3))) {
    iVar3 = param_3 + -1;
  }
  if (_DAT_ram_1e50 == 0) {
    _DAT_ram_1e50 = 1;
  }
  bVar4 = -1 < iVar3 - _DAT_ram_1e50;
  if ((bVar4 || !SBORROW2(iVar3,_DAT_ram_1e50)) && (!bVar4 || SBORROW2(iVar3,_DAT_ram_1e50))) {
    param_4 = param_4 + 5;
    iVar1 = param_1[_DAT_ram_1e50 + -1];
    bVar4 = param_4 - iVar1 < 0;
    if (bVar4 && SBORROW2(param_4,iVar1) || !bVar4 && !SBORROW2(param_4,iVar1)) goto LAB_rom_009fca;
  }
  _DAT_ram_1e50 = iVar3;
LAB_rom_009fca:
  return *(undefined2 *)(_DAT_ram_1e50 * 2 + param_2);
}





// Function: FUN_rom_009fd2

/* WARNING: Heritage AFTER dead removal. Example location: W1 : 0x009fe4 */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* WARNING: Restarted to delay deadcode elimination for space: ram */

undefined4 FUN_rom_009fd2(void)

{
  undefined4 uVar1;
  
  if (_DAT_ram_1e22 == 2) {
    FUN_rom_005d0c(_DAT_ram_1d44 * 100,0,0x22f,0);
  }
  if (_DAT_ram_1e12 < 0x58) {
    if (_DAT_ram_1d18 < 0x58) {
      uVar1 = FUN_rom_009f98(0x1e2e,0,5,0);
      return uVar1;
    }
  }
  _DAT_ram_1e38 = 1999;
  return 0x54c4;
}





// Function: FUN_rom_00a00a

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_rom_00a00a(void)

{
  int iVar1;
  int iVar2;
  
  if (_DAT_ram_1bd4 + 3000 < _DAT_ram_1e3a) {
    iVar1 = 0x32;
  }
  else if (_DAT_ram_1bd4 + 100 < _DAT_ram_1e3a) {
    iVar1 = 0x19;
  }
  else if (_DAT_ram_1bd4 - 100 < _DAT_ram_1e3a) {
    iVar1 = 0xc;
  }
  else if (_DAT_ram_1e3a + 3000 < _DAT_ram_1bd4) {
    iVar1 = -0x32;
  }
  else if (_DAT_ram_1e3a + 0x4b0 < _DAT_ram_1bd4) {
    iVar1 = -0xc;
  }
  else {
    if (_DAT_ram_1bd4 <= _DAT_ram_1e3a + 300) goto LAB_rom_00a054;
    iVar1 = -6;
  }
  _DAT_ram_1e38 = _DAT_ram_1e38 + iVar1;
LAB_rom_00a054:
  iVar1 = _DAT_ram_1e38;
  if (1999 < _DAT_ram_1e38) {
    _DAT_ram_1e38 = 1999;
    return CONCAT26(iVar1,CONCAT24(1999,CONCAT22(3000,_DAT_ram_1e3a)));
  }
  iVar2 = 0xc6;
  if ((_DAT_ram_1e38 - 0xc6U < 0x8000 || !SBORROW2(_DAT_ram_1e38,0xc6)) &&
      ((int)(_DAT_ram_1e38 - 0xc6U) < 1 || SBORROW2(_DAT_ram_1e38,0xc6))) {
    iVar2 = 199;
    _DAT_ram_1e38 = iVar2;
  }
  return CONCAT26(iVar1,CONCAT24(iVar2,CONCAT22(3000,_DAT_ram_1e3a)));
}





// Function: FUN_rom_00a06c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined2 FUN_rom_00a06c(undefined2 param_1)

{
  byte bVar1;
  ushort uVar2;
  undefined1 uVar3;
  uint uVar4;
  undefined1 uVar6;
  undefined2 uVar5;
  
  if ((_DAT_ram_1e18 & 1) == 0) {
    _DAT_ram_1e18 = _DAT_ram_1e18 & 0xdfff;
  }
  bVar1 = Peripherals::GPIOD.LATD._0_1_;
  uVar4 = CONCAT11((char)((uint)param_1 >> 8),bVar1);
  if (((bVar1 & 0x10) == 0) || ((_DAT_ram_1262 & 0x20) != 0)) {
LAB_rom_00a08a:
    if (((_DAT_ram_126a & 0x20) == 0) && (DAT_ram_15f6 == '\0')) {
      uVar3 = (char)(uVar4 >> 8);
      if ((_DAT_ram_1e18 & 0x2000) != 0) {
        uVar4 = 0;
        if (_DAT_ram_1d18 < 0x3f) goto LAB_rom_00a09e;
        uVar6 = 0;
        uVar3 = 0;
        if (_DAT_ram_1d18 < 0x57) goto LAB_rom_00a0aa;
      }
      uVar6 = uVar3;
      uVar2 = Peripherals::GPIOD.LATD;
      Peripherals::GPIOD.LATD = uVar2 | 0x20;
      goto LAB_rom_00a0aa;
    }
  }
  else {
    uVar4 = 0xc4d;
    if (_DAT_ram_1d4a != 0xc4d) goto LAB_rom_00a08a;
    uVar4 = _DAT_ram_1e1a & 0xa0;
    if (uVar4 != 0) goto LAB_rom_00a08a;
  }
LAB_rom_00a09e:
  uVar6 = (undefined1)(uVar4 >> 8);
  uVar2 = Peripherals::GPIOD.LATD;
  Peripherals::GPIOD.LATD = uVar2 & 0xffdf;
LAB_rom_00a0aa:
  bVar1 = Peripherals::GPIOD.LATD._0_1_;
  uVar5 = CONCAT11(uVar6,bVar1);
  if ((bVar1 & 0x20) == 0) {
    if (_DAT_ram_1268 == 4) {
      _DAT_ram_1e38 = 400;
      return 400;
    }
    uVar5 = 100;
    _DAT_ram_1e38 = 100;
  }
  return uVar5;
}





// Function: FUN_rom_00a0c2

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_00a0c2(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  undefined4 in_W1W0;
  undefined2 uVar4;
  undefined2 extraout_W1;
  bool bVar5;
  
  uVar4 = (undefined2)((ulong)in_W1W0 >> 0x10);
  iVar1 = _DAT_ram_1e52 + 1;
  bVar5 = _DAT_ram_1e52 != 8;
  _DAT_ram_1e52 = iVar1;
  if (bVar5 && 8 < iVar1) {
    _DAT_ram_1e52 = 0;
    iVar2 = FUN_rom_009f4a();
    iVar1 = FUN_rom_009f64(_DAT_ram_1e12);
    iVar3 = FUN_rom_009f64(_DAT_ram_1d18);
    if (iVar3 < iVar1) {
      iVar1 = iVar3;
    }
    _DAT_ram_1e3a = _DAT_ram_1e3a + *(int *)((iVar1 + iVar2 * 5) * 2 + -0x20b6);
    iVar2 = FUN_rom_009fd2();
    iVar1 = _DAT_ram_1bb0;
    if (_DAT_ram_1bb0 < iVar2) {
      iVar1 = iVar2;
    }
    bVar5 = -1 < _DAT_ram_1e3a - iVar1;
    if ((bVar5 || !SBORROW2(_DAT_ram_1e3a,iVar1)) && (!bVar5 || SBORROW2(_DAT_ram_1e3a,iVar1))) {
      _DAT_ram_1e3a = iVar1;
    }
    if (0x54c4 < _DAT_ram_1e3a) {
      _DAT_ram_1e3a = 0x54c4;
    }
    if ((_DAT_ram_1e3a - 0x5dbU < 0x8000 || !SBORROW2(_DAT_ram_1e3a,0x5db)) &&
        ((int)(_DAT_ram_1e3a - 0x5dbU) < 1 || SBORROW2(_DAT_ram_1e3a,0x5db))) {
      _DAT_ram_1e3a = 0x5dc;
    }
    FUN_rom_00a00a();
    uVar4 = extraout_W1;
  }
  return CONCAT22(uVar4,_DAT_ram_1e38);
}





// Function: FUN_rom_00a122

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ulong FUN_rom_00a122(void)

{
  uint uVar1;
  ushort uVar2;
  ulong in_W1W0;
  ulong uVar3;
  uint uVar4;
  
  if ((_DAT_ram_1e1c & 0x100) != 0) {
    _DAT_ram_1e1c = _DAT_ram_1e1c & 0xfeff;
    if (199 < _DAT_ram_1bd4) {
      uVar3 = FUN_rom_00a0c2();
      _DAT_ram_1e54 = (uint)uVar3;
      uVar4 = _DAT_ram_1e3c;
      if (_DAT_ram_1e3c < _DAT_ram_1e54) {
        uVar4 = _DAT_ram_1e54;
      }
      if ((_DAT_ram_1e18 & 1) != 0) {
        uVar3 = (ulong)_DAT_ram_1bb2 * 0x7d1;
        uVar1 = (int)(uVar3 >> 0x10) << 8 | (uint)uVar3 >> 8;
        uVar3 = uVar3 >> 8;
        if (uVar4 < uVar1) {
          uVar4 = uVar1;
        }
      }
      Peripherals::OC2.OC2RS = uVar4;
      return uVar3;
    }
    uVar2 = Peripherals::OC2.OC2RS;
    Peripherals::OC2.OC2RS = uVar2 + 0x14;
    uVar2 = Peripherals::OC2.OC2RS;
    in_W1W0 = CONCAT22(uVar2,399);
    if (399 < uVar2) {
      in_W1W0 = CONCAT22(uVar2,400);
      Peripherals::OC2.OC2RS = 400;
    }
  }
  return in_W1W0;
}





// Function: FUN_rom_00a16a

undefined2 FUN_rom_00a16a(void)

{
  ushort uVar1;
  
  Peripherals::UART1.U1MODE = 6;
  Peripherals::UART1.U1BRG = 0x28a;
  Peripherals::UART1.U1STA = 0;
  uVar1 = Peripherals::INTERRUPT.IFS0;
  Peripherals::INTERRUPT.IFS0 = uVar1 & 0xefff;
  uVar1 = Peripherals::INTERRUPT.IEC0;
  Peripherals::INTERRUPT.IEC0 = uVar1 & 0xefff;
  uVar1 = Peripherals::INTERRUPT.IFS0;
  Peripherals::INTERRUPT.IFS0 = uVar1 & 0xf7ff;
  uVar1 = Peripherals::INTERRUPT.IEC0;
  Peripherals::INTERRUPT.IEC0 = uVar1 & 0xf7ff;
  uVar1 = Peripherals::UART1.U1MODE;
  Peripherals::UART1.U1MODE = uVar1 | 0x8000;
  uVar1 = Peripherals::UART1.U1STA;
  Peripherals::UART1.U1STA = uVar1 | 0x400;
  return 0x28a;
}





// Function: FUN_rom_00a182

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined2 FUN_rom_00a182(void)

{
  byte bVar1;
  ushort uVar2;
  undefined2 uVar3;
  uint uVar4;
  bool bVar5;
  
  bVar1 = Peripherals::UART1.U1STA._0_1_;
  if (((bVar1 & 1) == 0) || (uVar4 = _DAT_ram_1bf4 & 0x100, uVar4 != 0)) goto LAB_rom_00a2f0;
  uVar2 = Peripherals::UART1.U1RXREG;
  _DAT_ram_1cf8 = uVar4;
  if (_DAT_ram_1ce2 == 2) {
    *(ushort *)(&DAT_ram_1cc8 + _DAT_ram_1ce0 * 2) = uVar2;
    uVar4 = _DAT_ram_1ce0 + 1;
    bVar5 = _DAT_ram_1ce0 != 4;
    _DAT_ram_1ce0 = uVar4;
    if (bVar5) goto LAB_rom_00a2f0;
    if ((_DAT_ram_1cc8 + _DAT_ram_1cca + _DAT_ram_1ccc + _DAT_ram_1cce & 0xff) == uVar2) {
      _DAT_ram_1d12 = _DAT_ram_1cc8 + _DAT_ram_1cca * 0x100;
      _DAT_ram_1d10 = _DAT_ram_1ccc + _DAT_ram_1cce * 0x100;
    }
LAB_rom_00a21a:
    _DAT_ram_1ce2 = 0;
    goto LAB_rom_00a2f0;
  }
  if (_DAT_ram_1ce2 < 2) {
    if (_DAT_ram_1ce2 == 0) {
      if (uVar2 == 0x49) {
        uVar4 = 1;
      }
      else {
        if (uVar2 != 0x56) goto LAB_rom_00a2f0;
        uVar4 = 8;
      }
      _DAT_ram_1ce0 = _DAT_ram_1ce2;
      _DAT_ram_1ce2 = uVar4;
      goto LAB_rom_00a2f0;
    }
    if ((_DAT_ram_1ce2 == 1) && (uVar2 == 0x34)) {
      _DAT_ram_1ce2 = 2;
      goto LAB_rom_00a2f0;
    }
  }
  else {
    if (_DAT_ram_1ce2 == 8) {
      if (uVar2 == 0x34) {
        _DAT_ram_1e1c = _DAT_ram_1e1c | 0x40;
      }
      else {
        if (uVar2 != 0x33) goto LAB_rom_00a21a;
        _DAT_ram_1e1c = _DAT_ram_1e1c & 0xffbf;
      }
      _DAT_ram_1ce2 = 9;
      goto LAB_rom_00a2f0;
    }
    if (_DAT_ram_1ce2 != 9) goto LAB_rom_00a2ee;
    *(ushort *)(&DAT_ram_1cc8 + _DAT_ram_1ce0 * 2) = uVar2;
    uVar4 = _DAT_ram_1ce0 + 1;
    bVar5 = _DAT_ram_1ce0 != 0xc;
    _DAT_ram_1ce0 = uVar4;
    if (bVar5) goto LAB_rom_00a2f0;
    DAT_ram_1d03 = (char)_DAT_ram_1cd4;
    DAT_ram_1d04 = (char)_DAT_ram_1cd6;
    DAT_ram_1d05 = (char)_DAT_ram_1cd8;
    DAT_ram_1d06 = (char)_DAT_ram_1cda;
    DAT_ram_1d07 = (char)_DAT_ram_1cdc;
    if ((byte)(DAT_ram_1cde +
              (char)_DAT_ram_1cca + (char)_DAT_ram_1cc8 + (char)_DAT_ram_1ccc + (char)_DAT_ram_1cce
              + (char)_DAT_ram_1cd0 + (char)_DAT_ram_1cd2 + DAT_ram_1d03 + DAT_ram_1d04 +
              DAT_ram_1d05 + DAT_ram_1d06 + DAT_ram_1d07) == uVar2) {
      if ((_DAT_ram_1cca & 0x80) == 0) {
        _DAT_ram_1bf2 = _DAT_ram_1bf2 & 0xfbff;
      }
      else {
        _DAT_ram_1bf2 = _DAT_ram_1bf2 | 0x400;
      }
      _DAT_ram_1cca = _DAT_ram_1cca & 0x7f;
      _DAT_ram_1d14 = _DAT_ram_1cca * 0x100 + _DAT_ram_1cc8;
      _DAT_ram_1e46 = _DAT_ram_1cce;
      _DAT_ram_1d16 = _DAT_ram_1cd0;
      if (_DAT_ram_1cd0 < _DAT_ram_1e12) {
        _DAT_ram_1d18 = _DAT_ram_1e12;
      }
      else {
        _DAT_ram_1d18 = _DAT_ram_1cd0;
      }
      if (_DAT_ram_1cd2 == 0) {
        _DAT_ram_1e1a = _DAT_ram_1e1a & 0xfffd;
      }
      else {
        _DAT_ram_1e1a = _DAT_ram_1e1a | 2;
      }
    }
    if (_DAT_ram_1cde == 0xaa) {
      _DAT_ram_1e20 = _DAT_ram_1e20 & 0xfeff;
      _DAT_ram_1bf2 = _DAT_ram_1bf2 | 4;
LAB_rom_00a2c2:
      _DAT_ram_1264 = _DAT_ram_1264 & 0xbfff;
    }
    else {
      if (_DAT_ram_1cde == 0x55) {
        _DAT_ram_1e20 = _DAT_ram_1e20 & 0xfeff;
LAB_rom_00a2c0:
        _DAT_ram_1bf2 = _DAT_ram_1bf2 & 0xfffb;
        goto LAB_rom_00a2c2;
      }
      if (_DAT_ram_1cde != 0x5a) {
        if (_DAT_ram_1cde != 0xa5) {
          if (_DAT_ram_1cde == 0xa0) {
            _DAT_ram_1e20 = _DAT_ram_1e20 & 0xfeff;
            _DAT_ram_1bf2 = _DAT_ram_1bf2 & 0xfffb;
            _DAT_ram_1264 = _DAT_ram_1264 & 0xbfff;
            _DAT_ram_1e20 = _DAT_ram_1e20 | 0x80;
          }
          goto LAB_rom_00a2ee;
        }
        _DAT_ram_1e20 = _DAT_ram_1e20 | 0x100;
        goto LAB_rom_00a2c0;
      }
      _DAT_ram_1e20 = _DAT_ram_1e20 & 0xfeff;
      _DAT_ram_1bf2 = _DAT_ram_1bf2 & 0xfffb;
      _DAT_ram_1264 = _DAT_ram_1264 | 0x4000;
    }
    _DAT_ram_1e20 = _DAT_ram_1e20 & 0xff7f;
  }
LAB_rom_00a2ee:
  _DAT_ram_1ce2 = 0;
LAB_rom_00a2f0:
  bVar1 = Peripherals::UART1.U1STA._0_1_;
  if (((bVar1 & 1) != 0) && ((_DAT_ram_1bf4 & 0x100) != 0)) {
    uVar2 = Peripherals::UART1.U1RXREG;
    _DAT_ram_1cf8 = 0;
  }
  bVar1 = Peripherals::UART1.U1STA._0_1_;
  if (((((bVar1 & 8) != 0) || (bVar1 = Peripherals::UART1.U1STA._0_1_, (bVar1 & 4) != 0)) ||
      (bVar1 = Peripherals::UART1.U1STA._0_1_, (bVar1 & 2) != 0)) ||
     (uVar3 = 30000, 30000 < _DAT_ram_1cf8)) {
    uVar3 = FUN_rom_00a16a();
    _DAT_ram_1cf8 = 0;
  }
  return uVar3;
}





// Function: FUN_rom_00a31e

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_00a31e(void)

{
  byte bVar1;
  int iVar2;
  ushort uVar3;
  uint uVar4;
  undefined1 uVar5;
  undefined1 uVar6;
  uint uVar7;
  int iVar8;
  byte *pbVar9;
  byte *pbVar10;
  bool bVar11;
  
  uVar4 = _DAT_ram_1cec;
  if (_DAT_ram_1cec == 0) {
    if (((_DAT_ram_1bf4 & 0x100) == 0) || ((_DAT_ram_1e18 & 2) == 0)) {
      if (_DAT_ram_1ce4 == 0) {
        pbVar10 = &DAT_ram_1cee;
        iVar8 = 0;
        do {
          if (iVar8 == 0) {
            DAT_ram_1cee = 'X';
            _DAT_ram_1ce6 = 0;
          }
          else {
            uVar5 = DAT_ram_1cf1;
            uVar6 = DAT_ram_1cf3;
            if (iVar8 == 1) {
              if (((_DAT_ram_1264 & 0x1000) == 0) && (_DAT_ram_1e22 != 5)) {
                DAT_ram_1cef = 0;
              }
              else {
                DAT_ram_1cef = 2;
              }
            }
            else if (iVar8 == 2) {
              DAT_ram_1cf0 = (undefined1)((uint)_DAT_ram_1be0 >> 8);
            }
            else {
              uVar5 = (char)_DAT_ram_1be0;
              if (iVar8 != 3) {
                if (iVar8 == 4) {
                  DAT_ram_1cf2 = (undefined1)((uint)_DAT_ram_1d0e >> 8);
                  uVar5 = DAT_ram_1cf1;
                }
                else {
                  uVar5 = DAT_ram_1cf1;
                  uVar6 = (char)_DAT_ram_1d0e;
                  if (iVar8 != 5) {
                    if (iVar8 == 6) {
                      DAT_ram_1cf4 = (undefined1)((uint)_DAT_ram_1e42 >> 8);
                      uVar6 = DAT_ram_1cf3;
                    }
                    else {
                      uVar6 = DAT_ram_1cf3;
                      if (iVar8 == 7) {
                        DAT_ram_1cf5 = (undefined1)_DAT_ram_1e42;
                      }
                    }
                  }
                }
              }
            }
            DAT_ram_1cf3 = uVar6;
            DAT_ram_1cf1 = uVar5;
            _DAT_ram_1ce6 = _DAT_ram_1ce6 + (uint)*pbVar10;
          }
          iVar2 = iVar8 + 1;
          pbVar10 = pbVar10 + 1;
          bVar11 = iVar8 == 6;
          iVar8 = iVar2;
        } while (bVar11 || iVar2 < 7);
        _DAT_ram_1ce8 = 8;
        DAT_ram_1cf6 = (undefined1)_DAT_ram_1ce6;
        _DAT_ram_1ce4 = 1;
      }
    }
    else {
      pbVar10 = (byte *)0x1cf9;
      pbVar9 = &DAT_ram_1cee;
      iVar8 = 0;
      do {
        if (iVar8 == 0) {
          DAT_ram_1cee = '`';
          _DAT_ram_1ce6 = 0;
        }
        else {
          bVar1 = *pbVar10;
          *pbVar9 = bVar1;
          _DAT_ram_1ce6 = _DAT_ram_1ce6 + (uint)bVar1;
        }
        iVar2 = iVar8 + 1;
        pbVar10 = pbVar10 + 1;
        pbVar9 = pbVar9 + 1;
        bVar11 = iVar8 == 6;
        iVar8 = iVar2;
      } while (bVar11 || iVar2 < 7);
      _DAT_ram_1ce8 = 8;
      DAT_ram_1cf6 = (undefined1)_DAT_ram_1ce6;
    }
  }
  bVar1 = Peripherals::UART1.U1STA._1_1_;
  if (((bVar1 & 1) != 0) &&
     (uVar7 = _DAT_ram_1cea + 1, bVar11 = _DAT_ram_1cea != 8, _DAT_ram_1cea = uVar7,
     8 < uVar7 && bVar11)) {
    _DAT_ram_1cea = 0;
    Peripherals::UART1.U1TXREG = (uint)(byte)(&DAT_ram_1cee)[uVar4];
    _DAT_ram_1cec = uVar4 + 1;
    if (7 < _DAT_ram_1cec && uVar4 != 7) {
      if ((DAT_ram_1cee == '`') && ((_DAT_ram_1e18 & 2) != 0)) {
        _DAT_ram_1e18 = _DAT_ram_1e18 & 0xfffd;
        uVar3 = Peripherals::I2C2.I2C2CON;
        Peripherals::I2C2.I2C2CON = uVar3 | 0x1000;
      }
      _DAT_ram_1cec = 0;
      _DAT_ram_1ce4 = 0;
    }
  }
  return;
}





// Function: thunk_FUN_rom_00a182

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void thunk_FUN_rom_00a182(void)

{
  _DAT_ram_1cf8 = _DAT_ram_1cf8 + 1;
  FUN_rom_00a182();
  return;
}





// Function: FUN_rom_00a5a2

undefined4 FUN_rom_00a5a2(void)

{
  ushort uVar1;
  
  uVar1 = Peripherals::GPIOB.TRISB;
  Peripherals::GPIOB.TRISB = uVar1 | 0x80;
  Peripherals::GPIOC.TRISC = 0xf000;
  Peripherals::GPIOC.LATC = 0;
  Peripherals::GPIOD.TRISD = 0xfc3;
  Peripherals::GPIOD.LATD = 8;
  Peripherals::GPIOD.ODCD = 0x801;
  Peripherals::GPIOE.TRISE = 0;
  Peripherals::GPIOE.LATE = 8;
  Peripherals::GPIOF.TRISF = 0x3c;
  Peripherals::GPIOF.LATF = 0x40;
  Peripherals::GPIOG.TRISG = 0x1cc;
  Peripherals::GPIOG.LATG = 0x200;
  return 0x80200;
}





// Function: FUN_rom_00a5ce

void FUN_rom_00a5ce(void)

{
  byte bVar1;
  ushort uVar2;
  
  Peripherals::CLOCK.PLLFBD = 0x30;
  bVar1 = Peripherals::CLOCK.CLKDIV._0_1_;
  Peripherals::CLOCK.CLKDIV._0_1_ = bVar1 & 0xe0 | 2;
  bVar1 = Peripherals::CLOCK.CLKDIV._0_1_;
  Peripherals::CLOCK.CLKDIV._0_1_ = bVar1 & 0x3f;
  Peripherals::CLOCK.OSCCON._1_1_ = 0x78;
  Peripherals::CLOCK.OSCCON._1_1_ = 0x9a;
  Peripherals::CLOCK.OSCCON._1_1_ = 3;
  Peripherals::CLOCK.OSCCON._0_1_ = 0x46;
  Peripherals::CLOCK.OSCCON._0_1_ = 0x57;
  Peripherals::CLOCK.OSCCON._0_1_ = 1;
  do {
    bVar1 = Peripherals::CLOCK.OSCCON._1_1_;
  } while ((bVar1 & 0x70) != 0x30);
  do {
    bVar1 = Peripherals::CLOCK.OSCCON._0_1_;
  } while ((bVar1 & 0x20) == 0);
  Peripherals::CLOCK.ACLKCON = 0x2740;
  uVar2 = Peripherals::CLOCK.ACLKCON;
  Peripherals::CLOCK.ACLKCON = uVar2 | 0x8000;
  do {
    bVar1 = Peripherals::CLOCK.ACLKCON._1_1_;
  } while ((bVar1 & 0x40) == 0);
  return;
}





// Function: FUN_rom_00a61c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_00a61c(void)

{
  undefined1 uVar1;
  byte bVar2;
  
  bVar2 = SPLIM.SR._0_1_;
  SPLIM.SR._0_1_ = bVar2 | 0xe0;
  uVar1 = SPLIM.SR._0_1_;
  uVar1 = SPLIM.SR._0_1_;
  _DAT_ram_126a = 0;
  _DAT_ram_125a = 0;
  FUN_rom_00a5ce(0xe0);
  FUN_rom_00a7d8();
  FUN_rom_00a7cc();
  FUN_rom_00a5a2();
  FUN_rom_00a7ae();
  FUN_rom_00a6f6();
  FUN_rom_009e88();
  FUN_rom_00610c();
  FUN_rom_00a16a();
  FUN_rom_0084a6();
  FUN_rom_009c8e();
  bVar2 = SPLIM.SR._0_1_;
  bVar2 = bVar2 & 0x1f;
  SPLIM.SR._0_1_ = bVar2;
  do {
    FUN_rom_009cc2(bVar2);
    FUN_rom_00a122();
    FUN_rom_009dfe();
    FUN_rom_008da2();
    FUN_rom_008dbe();
    thunk_FUN_rom_008de0();
    FUN_rom_008e60();
    FUN_rom_008e7e();
    bVar2 = FUN_rom_008e9c();
  } while( true );
}





// Function: FUN_rom_00a688

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_00a688(uint param_1,uint param_2,uint param_3)

{
  uint uVar1;
  
  uVar1 = _DAT_ram_27fa & 0xf1;
  if ((char)param_1 != -1) {
    uVar1 = _DAT_ram_27fa & 0xc1 | (param_1 & 3) << 4;
  }
  if ((char)param_2 != -1) {
    uVar1 = uVar1 & 0xff3f | (param_2 & 3) << 6;
  }
  if ((char)param_3 != -1) {
    uVar1 = uVar1 & 0xfffe | param_3 & 1;
  }
  _DAT_ram_27fa = uVar1;
  return;
}





// Function: FUN_rom_00a6b8

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined2 FUN_rom_00a6b8(void)

{
  return _DAT_ram_27fa;
}





// Function: FUN_rom_00a6c0

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined2 FUN_rom_00a6c0(void)

{
  return _DAT_ram_27f8;
}





// Function: FUN_rom_00a6c8

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined2 FUN_rom_00a6c8(void)

{
  return _DAT_ram_27f6;
}





// Function: FUN_rom_00a6cc

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_00a6cc(undefined2 param_1)

{
  _DAT_ram_27f4 = param_1;
  return;
}





// Function: FUN_rom_00a6d0

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined2 FUN_rom_00a6d0(void)

{
  return _DAT_ram_27f4;
}





// Function: FUN_rom_00a6d4

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_00a6d4(undefined2 param_1)

{
  _DAT_ram_27f2 = param_1;
  return;
}





// Function: FUN_rom_00a6d8

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined2 FUN_rom_00a6d8(void)

{
  return _DAT_ram_27f2;
}





// Function: FUN_rom_00a6e0

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined2 FUN_rom_00a6e0(void)

{
  return _DAT_ram_27f0;
}





// Function: FUN_rom_00a6e4

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_rom_00a6e4(void)

{
  code *UNRECOVERED_JUMPTABLE;
  
  _DAT_ram_27fc = _DAT_ram_27fe;
  _DAT_ram_27fe = 1;
  FUN_rom_00a688(1,0xff,0);
  PC = 0;
  UNRECOVERED_JUMPTABLE = (code *)PC;
                    /* WARNING: Could not recover jumptable at 0x00a6f2. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*UNRECOVERED_JUMPTABLE)();
  return;
}





// Function: FUN_rom_00a6f6

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_rom_00a6f6(void)

{
  ushort uVar1;
  byte bVar2;
  uint uVar3;
  
  Peripherals::TIMER1.T1CON = 0;
  Peripherals::TIMER1.TMR1 = 0;
  Peripherals::TIMER1.PR1 = 10000;
  bVar2 = Peripherals::INTERRUPT.IPC0._1_1_;
  Peripherals::INTERRUPT.IPC0._1_1_ = bVar2 & 0x8f | 0x20;
  uVar1 = Peripherals::INTERRUPT.IFS0;
  Peripherals::INTERRUPT.IFS0 = uVar1 & 0xfff7;
  uVar1 = Peripherals::INTERRUPT.IEC0;
  Peripherals::INTERRUPT.IEC0 = uVar1 | 8;
  uVar1 = Peripherals::TIMER1.T1CON;
  Peripherals::TIMER1.T1CON = uVar1 | 0x8000;
  Peripherals::TIMER3._8_2_ = 0;
  Ram0106 = 0;
  Peripherals::TIMER3._4_2_ = 1000;
  bVar2 = Peripherals::INTERRUPT.IPC1._1_1_;
  Peripherals::INTERRUPT.IPC1._1_1_ = bVar2 & 0x8f | 0x40;
  uVar1 = Peripherals::INTERRUPT.IFS0;
  Peripherals::INTERRUPT.IFS0 = uVar1 & 0xff7f;
  uVar1 = Peripherals::INTERRUPT.IEC0;
  Peripherals::INTERRUPT.IEC0 = uVar1 | 0x80;
  uVar3 = Peripherals::TIMER3._8_2_;
  Peripherals::TIMER3._8_2_ = uVar3 | 0x8000;
  Peripherals::TIMER5._8_2_ = 0;
  Ram0114 = 0;
  Peripherals::TIMER5._4_2_ = 50000;
  uVar1 = Peripherals::INTERRUPT.IFS1;
  Peripherals::INTERRUPT.IFS1 = uVar1 & 0xf7ff;
  uVar1 = Peripherals::INTERRUPT.IEC1;
  Peripherals::INTERRUPT.IEC1 = uVar1 & 0xf7ff;
  uVar3 = Peripherals::TIMER5._8_2_;
  Peripherals::TIMER5._8_2_ = uVar3 | 0x8000;
  _DAT_ram_1264 = 0;
  return 50000;
}





// Function: FUN_rom_00a774

uint FUN_rom_00a774(byte param_1,uint *param_2)

{
  uint uVar1;
  
  uVar1 = *(uint *)((uint)(byte)(param_1 ^ (byte)*param_2) * 2 + -0x22b6);
  *param_2 = *param_2 >> 8 ^ uVar1;
  return uVar1;
}





// Function: FUN_rom_00a786

void FUN_rom_00a786(int param_1,undefined2 param_2)

{
  undefined4 in_W3W2;
  uint uVar1;
  uint uVar2;
  uint uVar3;
  bool bVar4;
  
  uVar1 = 0;
  uVar2 = 0;
  while( true ) {
    uVar3 = (uint)((ulong)in_W3W2 >> 0x10);
    if (uVar3 <= uVar2 && uVar3 + (uVar1 < (uint)in_W3W2) <= uVar2) break;
    FUN_rom_00a774(*(undefined1 *)(param_1 + uVar1),param_2);
    bVar4 = 0xfffe < uVar1;
    uVar1 = uVar1 + 1;
    uVar2 = uVar2 + bVar4;
  }
  return;
}





// Function: FUN_rom_00a7ae

undefined4 FUN_rom_00a7ae(void)

{
  ushort uVar1;
  
  Peripherals::ADC.ADCON = 0x1007;
  Peripherals::ADC.ADSTAT = 0;
  Peripherals::ADC.ADBASE = 0;
  Peripherals::ADC.ADCPC0 = 0x1f1f;
  Peripherals::ADC.ADCPC1 = 0x1f;
  Peripherals::ADC.ADCPC2 = 0x1f1f;
  Peripherals::ADC.ADCPC3 = 0x1f1f;
  Peripherals::ADC.ADPCFG = 0x80;
  uVar1 = Peripherals::ADC.ADCON;
  Peripherals::ADC.ADCON = uVar1 | 0x8000;
  return 0x1f1f0080;
}





// Function: FUN_rom_00a7cc

undefined2 FUN_rom_00a7cc(void)

{
  ushort uVar1;
  
  Peripherals::CMP3.CMPCON3 = 0x81;
  Peripherals::CMP3.CMPDAC3 = 0x1a3;
  uVar1 = Peripherals::CMP3.CMPCON3;
  Peripherals::CMP3.CMPCON3 = uVar1 | 0x8000;
  return 0x1a3;
}





// Function: FUN_rom_00a7d8

undefined2 FUN_rom_00a7d8(void)

{
  ushort uVar1;
  
  Peripherals::CMP4.CMPCON4 = 0x101;
  Peripherals::CMP4.CMPDAC4 = 0;
  uVar1 = Peripherals::CMP4.CMPCON4;
  Peripherals::CMP4.CMPCON4 = uVar1 | 0x8000;
  return 0x101;
}





