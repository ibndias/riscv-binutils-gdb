/* riscv.h.  RISC-V opcode list for GDB, the GNU debugger.
   Copyright (C) 2011-2020 Free Software Foundation, Inc.
   Contributed by Andrew Waterman

   This file is part of GDB, GAS, and the GNU binutils.

   GDB, GAS, and the GNU binutils are free software; you can redistribute
   them and/or modify them under the terms of the GNU General Public
   License as published by the Free Software Foundation; either version
   3, or (at your option) any later version.

   GDB, GAS, and the GNU binutils are distributed in the hope that they
   will be useful, but WITHOUT ANY WARRANTY; without even the implied
   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
   the GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING3. If not,
   see <http://www.gnu.org/licenses/>.  */

#ifndef _RISCV_H_
#define _RISCV_H_

#include "riscv-opc.h"
#include <stdlib.h>
#include <stdint.h>

typedef uint64_t insn_t;

static inline unsigned int riscv_insn_length (insn_t insn)
{
  if ((insn & 0x3) != 0x3) /* RVC.  */
    return 2;
  if ((insn & 0x1f) != 0x1f) /* Base ISA and extensions in 32-bit space.  */
    return 4;
  if ((insn & 0x3f) == 0x1f) /* 48-bit extensions.  */
    return 6;
  if ((insn & 0x7f) == 0x3f) /* 64-bit extensions.  */
    return 8;
  /* Longer instructions not supported at the moment.  */
  return 2;
}

static const char * const riscv_rm[8] =
{
  "rne", "rtz", "rdn", "rup", "rmm", 0, 0, "dyn"
};

static const char * const riscv_pred_succ[16] =
{
  0,   "w",  "r",  "rw",  "o",  "ow",  "or",  "orw",
  "i", "iw", "ir", "irw", "io", "iow", "ior", "iorw"
};

/* List of vsetvli vsew constants.  */
static const char * const riscv_vsew[8] =
{
  "e8", "e16", "e32", "e64", "e128", "e256", "e512", "e1024"
};

/* List of vsetvli vlmul constants.  */
static const char * const riscv_vlmul[8] =
{
  "m1", "m2", "m4", "m8", 0, "mf8", "mf4", "mf2"
};

/* List of vsetvli vediv constants.  */
static const char * const riscv_vediv[4] =
{
  "d1", "d2", "d4", "d8"
};

static const char * const riscv_vta[2] =
{
  "tu", "ta"
};

static const char * const riscv_vma[2] =
{
  "mu", "ma"
};

#define RVC_JUMP_BITS 11
#define RVC_JUMP_REACH ((1ULL << RVC_JUMP_BITS) * RISCV_JUMP_ALIGN)

#define RVC_BRANCH_BITS 8
#define RVC_BRANCH_REACH ((1ULL << RVC_BRANCH_BITS) * RISCV_BRANCH_ALIGN)

#define RV_X(x, s, n)  (((x) >> (s)) & ((1 << (n)) - 1))
#define RV_IMM_SIGN(x) (-(((x) >> 31) & 1))

#define EXTRACT_ITYPE_IMM(x) \
  (RV_X(x, 20, 12) | (RV_IMM_SIGN(x) << 12))
#define EXTRACT_STYPE_IMM(x) \
  (RV_X(x, 7, 5) | (RV_X(x, 25, 7) << 5) | (RV_IMM_SIGN(x) << 12))
#define EXTRACT_SBTYPE_IMM(x) \
  ((RV_X(x, 8, 4) << 1) | (RV_X(x, 25, 6) << 5) | (RV_X(x, 7, 1) << 11) | (RV_IMM_SIGN(x) << 12))
#define EXTRACT_UTYPE_IMM(x) \
  ((RV_X(x, 12, 20) << 12) | (RV_IMM_SIGN(x) << 32))
#define EXTRACT_UJTYPE_IMM(x) \
  ((RV_X(x, 21, 10) << 1) | (RV_X(x, 20, 1) << 11) | (RV_X(x, 12, 8) << 12) | (RV_IMM_SIGN(x) << 20))
#define EXTRACT_RVC_IMM(x) \
  (RV_X(x, 2, 5) | (-RV_X(x, 12, 1) << 5))
#define EXTRACT_RVC_LUI_IMM(x) \
  (EXTRACT_RVC_IMM (x) << RISCV_IMM_BITS)
#define EXTRACT_RVC_SIMM3(x) \
  (RV_X(x, 10, 2) | (-RV_X(x, 12, 1) << 2))
#define EXTRACT_RVC_UIMM8(x) \
  (RV_X(x, 5, 8))
#define EXTRACT_RVC_ADDI4SPN_IMM(x) \
  ((RV_X(x, 6, 1) << 2) | (RV_X(x, 5, 1) << 3) | (RV_X(x, 11, 2) << 4) | (RV_X(x, 7, 4) << 6))
#define EXTRACT_RVC_ADDI16SP_IMM(x) \
  ((RV_X(x, 6, 1) << 4) | (RV_X(x, 2, 1) << 5) | (RV_X(x, 5, 1) << 6) | (RV_X(x, 3, 2) << 7) | (-RV_X(x, 12, 1) << 9))
#define EXTRACT_RVC_LW_IMM(x) \
  ((RV_X(x, 6, 1) << 2) | (RV_X(x, 10, 3) << 3) | (RV_X(x, 5, 1) << 6))
#define EXTRACT_RVC_LD_IMM(x) \
  ((RV_X(x, 10, 3) << 3) | (RV_X(x, 5, 2) << 6))
#define EXTRACT_RVC_LWSP_IMM(x) \
  ((RV_X(x, 4, 3) << 2) | (RV_X(x, 12, 1) << 5) | (RV_X(x, 2, 2) << 6))
#define EXTRACT_RVC_LDSP_IMM(x) \
  ((RV_X(x, 5, 2) << 3) | (RV_X(x, 12, 1) << 5) | (RV_X(x, 2, 3) << 6))
#define EXTRACT_RVC_SWSP_IMM(x) \
  ((RV_X(x, 9, 4) << 2) | (RV_X(x, 7, 2) << 6))
#define EXTRACT_RVC_SDSP_IMM(x) \
  ((RV_X(x, 10, 3) << 3) | (RV_X(x, 7, 3) << 6))
#define EXTRACT_RVC_B_IMM(x) \
  ((RV_X(x, 3, 2) << 1) | (RV_X(x, 10, 2) << 3) | (RV_X(x, 2, 1) << 5) | (RV_X(x, 5, 2) << 6) | (-RV_X(x, 12, 1) << 8))
#define EXTRACT_RVC_J_IMM(x) \
  ((RV_X(x, 3, 3) << 1) | (RV_X(x, 11, 1) << 4) | (RV_X(x, 2, 1) << 5) | (RV_X(x, 7, 1) << 6) | (RV_X(x, 6, 1) << 7) | (RV_X(x, 9, 2) << 8) | (RV_X(x, 8, 1) << 10) | (-RV_X(x, 12, 1) << 11))
#define EXTRACT_RVV_VI_IMM(x) \
  (RV_X(x, 15, 5) | (-RV_X(x, 19, 1) << 5))
#define EXTRACT_RVV_VI_UIMM(x) \
  (RV_X(x, 15, 5))
#define EXTRACT_RVV_OFFSET(x) \
  (RV_X(x, 29, 3))
#define EXTRACT_RVV_VC_IMM(x) \
  (RV_X(x, 20, 11))

#define ENCODE_ITYPE_IMM(x) \
  (RV_X(x, 0, 12) << 20)
#define ENCODE_STYPE_IMM(x) \
  ((RV_X(x, 0, 5) << 7) | (RV_X(x, 5, 7) << 25))
#define ENCODE_SBTYPE_IMM(x) \
  ((RV_X(x, 1, 4) << 8) | (RV_X(x, 5, 6) << 25) | (RV_X(x, 11, 1) << 7) | (RV_X(x, 12, 1) << 31))
#define ENCODE_UTYPE_IMM(x) \
  (RV_X(x, 12, 20) << 12)
#define ENCODE_UJTYPE_IMM(x) \
  ((RV_X(x, 1, 10) << 21) | (RV_X(x, 11, 1) << 20) | (RV_X(x, 12, 8) << 12) | (RV_X(x, 20, 1) << 31))
#define ENCODE_RVC_IMM(x) \
  ((RV_X(x, 0, 5) << 2) | (RV_X(x, 5, 1) << 12))
#define ENCODE_RVC_LUI_IMM(x) \
  ENCODE_RVC_IMM ((x) >> RISCV_IMM_BITS)
#define ENCODE_RVC_SIMM3(x) \
  (RV_X(x, 0, 3) << 10)
#define ENCODE_RVC_UIMM8(x) \
  (RV_X(x, 0, 8) << 5)
#define ENCODE_RVC_ADDI4SPN_IMM(x) \
  ((RV_X(x, 2, 1) << 6) | (RV_X(x, 3, 1) << 5) | (RV_X(x, 4, 2) << 11) | (RV_X(x, 6, 4) << 7))
#define ENCODE_RVC_ADDI16SP_IMM(x) \
  ((RV_X(x, 4, 1) << 6) | (RV_X(x, 5, 1) << 2) | (RV_X(x, 6, 1) << 5) | (RV_X(x, 7, 2) << 3) | (RV_X(x, 9, 1) << 12))
#define ENCODE_RVC_LW_IMM(x) \
  ((RV_X(x, 2, 1) << 6) | (RV_X(x, 3, 3) << 10) | (RV_X(x, 6, 1) << 5))
#define ENCODE_RVC_LD_IMM(x) \
  ((RV_X(x, 3, 3) << 10) | (RV_X(x, 6, 2) << 5))
#define ENCODE_RVC_LWSP_IMM(x) \
  ((RV_X(x, 2, 3) << 4) | (RV_X(x, 5, 1) << 12) | (RV_X(x, 6, 2) << 2))
#define ENCODE_RVC_LDSP_IMM(x) \
  ((RV_X(x, 3, 2) << 5) | (RV_X(x, 5, 1) << 12) | (RV_X(x, 6, 3) << 2))
#define ENCODE_RVC_SWSP_IMM(x) \
  ((RV_X(x, 2, 4) << 9) | (RV_X(x, 6, 2) << 7))
#define ENCODE_RVC_SDSP_IMM(x) \
  ((RV_X(x, 3, 3) << 10) | (RV_X(x, 6, 3) << 7))
#define ENCODE_RVC_B_IMM(x) \
  ((RV_X(x, 1, 2) << 3) | (RV_X(x, 3, 2) << 10) | (RV_X(x, 5, 1) << 2) | (RV_X(x, 6, 2) << 5) | (RV_X(x, 8, 1) << 12))
#define ENCODE_RVC_J_IMM(x) \
  ((RV_X(x, 1, 3) << 3) | (RV_X(x, 4, 1) << 11) | (RV_X(x, 5, 1) << 2) | (RV_X(x, 6, 1) << 7) | (RV_X(x, 7, 1) << 6) | (RV_X(x, 8, 2) << 9) | (RV_X(x, 10, 1) << 8) | (RV_X(x, 11, 1) << 12))
#define ENCODE_RVV_VC_IMM(x) \
  (RV_X(x, 0, 11) << 20)

#define VALID_ITYPE_IMM(x) (EXTRACT_ITYPE_IMM(ENCODE_ITYPE_IMM(x)) == (x))
#define VALID_STYPE_IMM(x) (EXTRACT_STYPE_IMM(ENCODE_STYPE_IMM(x)) == (x))
#define VALID_SBTYPE_IMM(x) (EXTRACT_SBTYPE_IMM(ENCODE_SBTYPE_IMM(x)) == (x))
#define VALID_UTYPE_IMM(x) (EXTRACT_UTYPE_IMM(ENCODE_UTYPE_IMM(x)) == (x))
#define VALID_UJTYPE_IMM(x) (EXTRACT_UJTYPE_IMM(ENCODE_UJTYPE_IMM(x)) == (x))
#define VALID_RVC_IMM(x) (EXTRACT_RVC_IMM(ENCODE_RVC_IMM(x)) == (x))
#define VALID_RVC_LUI_IMM(x) (ENCODE_RVC_LUI_IMM(x) != 0 && EXTRACT_RVC_LUI_IMM(ENCODE_RVC_LUI_IMM(x)) == (x))
#define VALID_RVC_SIMM3(x) (EXTRACT_RVC_SIMM3(ENCODE_RVC_SIMM3(x)) == (x))
#define VALID_RVC_UIMM8(x) (EXTRACT_RVC_UIMM8(ENCODE_RVC_UIMM8(x)) == (x))
#define VALID_RVC_ADDI4SPN_IMM(x) (EXTRACT_RVC_ADDI4SPN_IMM(ENCODE_RVC_ADDI4SPN_IMM(x)) == (x))
#define VALID_RVC_ADDI16SP_IMM(x) (EXTRACT_RVC_ADDI16SP_IMM(ENCODE_RVC_ADDI16SP_IMM(x)) == (x))
#define VALID_RVC_LW_IMM(x) (EXTRACT_RVC_LW_IMM(ENCODE_RVC_LW_IMM(x)) == (x))
#define VALID_RVC_LD_IMM(x) (EXTRACT_RVC_LD_IMM(ENCODE_RVC_LD_IMM(x)) == (x))
#define VALID_RVC_LWSP_IMM(x) (EXTRACT_RVC_LWSP_IMM(ENCODE_RVC_LWSP_IMM(x)) == (x))
#define VALID_RVC_LDSP_IMM(x) (EXTRACT_RVC_LDSP_IMM(ENCODE_RVC_LDSP_IMM(x)) == (x))
#define VALID_RVC_SWSP_IMM(x) (EXTRACT_RVC_SWSP_IMM(ENCODE_RVC_SWSP_IMM(x)) == (x))
#define VALID_RVC_SDSP_IMM(x) (EXTRACT_RVC_SDSP_IMM(ENCODE_RVC_SDSP_IMM(x)) == (x))
#define VALID_RVC_B_IMM(x) (EXTRACT_RVC_B_IMM(ENCODE_RVC_B_IMM(x)) == (x))
#define VALID_RVC_J_IMM(x) (EXTRACT_RVC_J_IMM(ENCODE_RVC_J_IMM(x)) == (x))
#define VALID_RVV_VC_IMM(x) (EXTRACT_RVV_VC_IMM(ENCODE_RVV_VC_IMM(x)) == (x))

#define RISCV_RTYPE(insn, rd, rs1, rs2) \
  ((MATCH_ ## insn) | ((rd) << OP_SH_RD) | ((rs1) << OP_SH_RS1) | ((rs2) << OP_SH_RS2))
#define RISCV_ITYPE(insn, rd, rs1, imm) \
  ((MATCH_ ## insn) | ((rd) << OP_SH_RD) | ((rs1) << OP_SH_RS1) | ENCODE_ITYPE_IMM(imm))
#define RISCV_STYPE(insn, rs1, rs2, imm) \
  ((MATCH_ ## insn) | ((rs1) << OP_SH_RS1) | ((rs2) << OP_SH_RS2) | ENCODE_STYPE_IMM(imm))
#define RISCV_SBTYPE(insn, rs1, rs2, target) \
  ((MATCH_ ## insn) | ((rs1) << OP_SH_RS1) | ((rs2) << OP_SH_RS2) | ENCODE_SBTYPE_IMM(target))
#define RISCV_UTYPE(insn, rd, bigimm) \
  ((MATCH_ ## insn) | ((rd) << OP_SH_RD) | ENCODE_UTYPE_IMM(bigimm))
#define RISCV_UJTYPE(insn, rd, target) \
  ((MATCH_ ## insn) | ((rd) << OP_SH_RD) | ENCODE_UJTYPE_IMM(target))

#define RISCV_NOP RISCV_ITYPE(ADDI, 0, 0, 0)
#define RVC_NOP MATCH_C_ADDI

#define RISCV_CONST_HIGH_PART(VALUE) \
  (((VALUE) + (RISCV_IMM_REACH/2)) & ~(RISCV_IMM_REACH-1))
#define RISCV_CONST_LOW_PART(VALUE) ((VALUE) - RISCV_CONST_HIGH_PART (VALUE))
#define RISCV_PCREL_HIGH_PART(VALUE, PC) RISCV_CONST_HIGH_PART((VALUE) - (PC))
#define RISCV_PCREL_LOW_PART(VALUE, PC) RISCV_CONST_LOW_PART((VALUE) - (PC))

#define RISCV_JUMP_BITS RISCV_BIGIMM_BITS
#define RISCV_JUMP_ALIGN_BITS 1
#define RISCV_JUMP_ALIGN (1 << RISCV_JUMP_ALIGN_BITS)
#define RISCV_JUMP_REACH ((1ULL << RISCV_JUMP_BITS) * RISCV_JUMP_ALIGN)

#define RISCV_IMM_BITS 12
#define RISCV_BIGIMM_BITS (32 - RISCV_IMM_BITS)
#define RISCV_IMM_REACH (1LL << RISCV_IMM_BITS)
#define RISCV_BIGIMM_REACH (1LL << RISCV_BIGIMM_BITS)
#define RISCV_RVC_IMM_REACH (1LL << 6)
#define RISCV_BRANCH_BITS RISCV_IMM_BITS
#define RISCV_BRANCH_ALIGN_BITS RISCV_JUMP_ALIGN_BITS
#define RISCV_BRANCH_ALIGN (1 << RISCV_BRANCH_ALIGN_BITS)
#define RISCV_BRANCH_REACH (RISCV_IMM_REACH * RISCV_BRANCH_ALIGN)

/* RV fields.  */

#define OP_MASK_OP		0x7f
#define OP_SH_OP		0
#define OP_MASK_RS2		0x1f
#define OP_SH_RS2		20
#define OP_MASK_RS1		0x1f
#define OP_SH_RS1		15
#define OP_MASK_RS3		0x1f
#define OP_SH_RS3		27
#define OP_MASK_RD		0x1f
#define OP_SH_RD		7
#define OP_MASK_SHAMT		0x3f
#define OP_SH_SHAMT		20
#define OP_MASK_SHAMTW		0x1f
#define OP_SH_SHAMTW		20
#define OP_MASK_RM		0x7
#define OP_SH_RM		12
#define OP_MASK_PRED		0xf
#define OP_SH_PRED		24
#define OP_MASK_SUCC		0xf
#define OP_SH_SUCC		20
#define OP_MASK_AQ		0x1
#define OP_SH_AQ		26
#define OP_MASK_RL		0x1
#define OP_SH_RL		25

#define OP_MASK_CUSTOM_IMM	0x7f
#define OP_SH_CUSTOM_IMM	25
#define OP_MASK_CSR		0xfff
#define OP_SH_CSR		20

#define OP_MASK_FUNCT3         0x7
#define OP_SH_FUNCT3           12
#define OP_MASK_FUNCT7         0x7f
#define OP_SH_FUNCT7           25
#define OP_MASK_FUNCT2         0x3
#define OP_SH_FUNCT2           25

/* RVC fields.  */

#define OP_MASK_OP2            0x3
#define OP_SH_OP2              0

#define OP_MASK_CRS2 0x1f
#define OP_SH_CRS2 2
#define OP_MASK_CRS1S 0x7
#define OP_SH_CRS1S 7
#define OP_MASK_CRS2S 0x7
#define OP_SH_CRS2S 2

#define OP_MASK_CFUNCT6                0x3f
#define OP_SH_CFUNCT6          10
#define OP_MASK_CFUNCT4                0xf
#define OP_SH_CFUNCT4          12
#define OP_MASK_CFUNCT3                0x7
#define OP_SH_CFUNCT3          13
#define OP_MASK_CFUNCT2                0x3
#define OP_SH_CFUNCT2          5

/* RVV fields.  */

#define OP_MASK_VD		0x1f
#define OP_SH_VD		7
#define OP_MASK_VS1		0x1f
#define OP_SH_VS1		15
#define OP_MASK_VS2		0x1f
#define OP_SH_VS2		20
#define OP_MASK_VIMM		0x1f
#define OP_SH_VIMM		15
#define OP_MASK_VMASK		0x1
#define OP_SH_VMASK		25
#define OP_MASK_VFUNCT6		0x3f
#define OP_SH_VFUNCT6		26

#define OP_MASK_VLMUL		0x7
#define OP_SH_VLMUL		0
#define OP_MASK_VSEW		0x7
#define OP_SH_VSEW		3
#define OP_MASK_VTA		0x1
#define OP_SH_VTA		6
#define OP_MASK_VMA		0x1
#define OP_SH_VMA		7
#define OP_MASK_VEDIV		0x3
#define OP_SH_VEDIV		8
#define OP_MASK_VTYPE_RES	0x1
#define OP_SH_VTYPE_RES 	10

#define OP_MASK_VWD		0x1
#define OP_SH_VWD		26

/* ABI names for selected x-registers.  */

#define X_RA 1
#define X_SP 2
#define X_GP 3
#define X_TP 4
#define X_T0 5
#define X_T1 6
#define X_T2 7
#define X_T3 28

#define NGPR 32
#define NFPR 32
#define NVECR 32
#define NVECM 1

/* These fake label defines are use by both the assembler, and
   libopcodes.  The assembler uses this when it needs to generate a fake
   label, and libopcodes uses it to hide the fake labels in its output.  */
#define RISCV_FAKE_LABEL_NAME ".L0 "
#define RISCV_FAKE_LABEL_CHAR ' '

/* Replace bits MASK << SHIFT of STRUCT with the equivalent bits in
   VALUE << SHIFT.  VALUE is evaluated exactly once.  */
#define INSERT_BITS(STRUCT, VALUE, MASK, SHIFT) \
  (STRUCT) = (((STRUCT) & ~((insn_t)(MASK) << (SHIFT))) \
	      | ((insn_t)((VALUE) & (MASK)) << (SHIFT)))

/* Extract bits MASK << SHIFT from STRUCT and shift them right
   SHIFT places.  */
#define EXTRACT_BITS(STRUCT, MASK, SHIFT) \
  (((STRUCT) >> (SHIFT)) & (MASK))

/* Extract the operand given by FIELD from integer INSN.  */
#define EXTRACT_OPERAND(FIELD, INSN) \
  EXTRACT_BITS ((INSN), OP_MASK_##FIELD, OP_SH_##FIELD)

/* The maximal number of subset can be required. */
#define MAX_SUBSET_NUM 4

/* All RISC-V instructions belong to at least one of these classes.  */

enum riscv_insn_class
  {
   INSN_CLASS_NONE,

   INSN_CLASS_I,
   INSN_CLASS_C,
   INSN_CLASS_A,
   INSN_CLASS_M,
   INSN_CLASS_F,
   INSN_CLASS_D,
   INSN_CLASS_D_AND_C,
   INSN_CLASS_F_AND_C,
   INSN_CLASS_Q,
   INSN_CLASS_V,
   INSN_CLASS_V_AND_F,
   INSN_CLASS_V_OR_ZVAMO,
   INSN_CLASS_V_AND_ZVEDIV,
   INSN_CLASS_V_OR_ZVLSSEG,
   INSN_CLASS_V_AND_ZVQMAC,
  };

/* This structure holds information for a particular instruction.  */

struct riscv_opcode
{
  /* The name of the instruction.  */
  const char *name;
  /* The requirement of xlen for the instruction, 0 if no requirement.  */
  unsigned xlen_requirement;
  /* Class to which this instruction belongs.  Used to decide whether or
     not this instruction is legal in the current -march context.  */
  enum riscv_insn_class insn_class;
  /* A string describing the arguments for this instruction.  */
  const char *args;
  /* The basic opcode for the instruction.  When assembling, this
     opcode is modified by the arguments to produce the actual opcode
     that is used.  If pinfo is INSN_MACRO, then this is 0.  */
  insn_t match;
  /* If pinfo is not INSN_MACRO, then this is a bit mask for the
     relevant portions of the opcode when disassembling.  If the
     actual opcode anded with the match field equals the opcode field,
     then we have found the correct instruction.  If pinfo is
     INSN_MACRO, then this field is the macro identifier.  */
  insn_t mask;
  /* A function to determine if a word corresponds to this instruction.
     Usually, this computes ((word & mask) == match).  If the constraints
     checking is disable, then most of the function should check only the
     basic encoding for the instruction.  */
  int (*match_func) (const struct riscv_opcode *op, insn_t word,
		     int constraints);
  /* For a macro, this is INSN_MACRO.  Otherwise, it is a collection
     of bits describing the instruction, notably any relevant hazard
     information.  */
  unsigned long pinfo;
};

/* Instruction is a simple alias (e.g. "mv" for "addi").  */
#define	INSN_ALIAS		0x00000001

/* These are for setting insn_info fields.

   Nonbranch is the default.  Noninsn is used only if there is no match.
   There are no condjsr or dref2 instructions.  So that leaves condbranch,
   branch, jsr, and dref that we need to handle here, encoded in 3 bits.  */
#define INSN_TYPE		0x0000000e

/* Instruction is an unconditional branch.  */
#define INSN_BRANCH		0x00000002
/* Instruction is a conditional branch.  */
#define INSN_CONDBRANCH		0x00000004
/* Instruction is a jump to subroutine.  */
#define INSN_JSR		0x00000006
/* Instruction is a data reference.  */
#define INSN_DREF		0x00000008

/* We have 5 data reference sizes, which we can encode in 3 bits.  */
#define INSN_DATA_SIZE		0x00000070
#define INSN_DATA_SIZE_SHIFT	4
#define INSN_1_BYTE		0x00000010
#define INSN_2_BYTE		0x00000020
#define INSN_4_BYTE		0x00000030
#define INSN_8_BYTE		0x00000040
#define INSN_16_BYTE		0x00000050

/* Instruction is actually a macro.  It should be ignored by the
   disassembler, and requires special treatment by the assembler.  */
#define INSN_MACRO		0xffffffff

/* This is a list of macro expanded instructions.

   _I appended means immediate
   _A appended means address
   _AB appended means address with base register
   _D appended means 64 bit floating point constant
   _S appended means 32 bit floating point constant.  */

enum
{
  M_LA,
  M_LLA,
  M_LA_TLS_GD,
  M_LA_TLS_IE,
  M_LB,
  M_LBU,
  M_LH,
  M_LHU,
  M_LW,
  M_LWU,
  M_LD,
  M_SB,
  M_SH,
  M_SW,
  M_SD,
  M_FLW,
  M_FLD,
  M_FLQ,
  M_FSW,
  M_FSD,
  M_FSQ,
  M_CALL,
  M_J,
  M_LI,
  M_VMSGE,
  M_VMSGEU,
  M_NUM_MACROS
};


extern const char * const riscv_gpr_names_numeric[NGPR];
extern const char * const riscv_gpr_names_abi[NGPR];
extern const char * const riscv_fpr_names_numeric[NFPR];
extern const char * const riscv_fpr_names_abi[NFPR];
extern const char * const riscv_vecr_names_numeric[NVECR];
extern const char * const riscv_vecm_names_numeric[NVECM];

extern const struct riscv_opcode riscv_opcodes[];
extern const struct riscv_opcode riscv_insn_types[];

#endif /* _RISCV_H_ */
