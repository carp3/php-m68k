--TEST--
m68k_nop basic test
--FILE--
<?php
m68k_init();
m68k_pulse_reset();
var_dump(m68k_get_reg(1));
?>
--EXPECT--
int(0)

