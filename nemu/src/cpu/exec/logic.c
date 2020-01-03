#include "cpu/exec.h"
#include "cpu/cc.h"

make_EHelper(test) {
	rtl_and(&t1, &id_dest->val, &id_src->val);
	t0 = 0;
	rtl_set_OF(&t0);
	rtl_set_CF(&t0);
	rtl_update_ZFSF(&t1, id_dest->width);

  print_asm_template2(test);
}

make_EHelper(and) {
	rtl_and(&t1, &id_dest->val, &id_src->val);
	t0 = 0;

	rtl_set_OF(&t0);
	rtl_set_CF(&t0);
	rtl_update_ZFSF(&t1, id_dest->width);
	operand_write(id_dest, &t1);

  print_asm_template2(and);
}

make_EHelper(xor) {
	rtl_xor(&t1, &id_dest->val, &id_src->val);
	t0 = 0;

	rtl_set_OF(&t0);
	rtl_set_CF(&t0);
	rtl_update_ZFSF(&t1, id_dest->width);
	operand_write(id_dest, &t1);

  print_asm_template2(xor);
}

make_EHelper(or) {
	rtl_or(&t2, &id_dest->val, &id_src->val);
	operand_write(id_dest, &t2);
	rtl_update_ZFSF(&t2, id_dest->width);
	t0 = 0;
	rtl_set_OF(&t0);
	rtl_set_CF(&t0);

  print_asm_template2(or);
}

make_EHelper(sar) {
  TODO();
  // unnecessary to update CF and OF in NEMU

  print_asm_template2(sar);
}

make_EHelper(shl) {
  TODO();
  // unnecessary to update CF and OF in NEMU

  print_asm_template2(shl);
}

make_EHelper(shr) {
  TODO();
  // unnecessary to update CF and OF in NEMU

  print_asm_template2(shr);
}

make_EHelper(setcc) {
  uint32_t cc = decoding.opcode & 0xf;

  rtl_setcc(&t2, cc);
  operand_write(id_dest, &t2);

  print_asm("set%s %s", get_cc_name(cc), id_dest->str);
}

make_EHelper(not) {
  TODO();

  print_asm_template1(not);
}
