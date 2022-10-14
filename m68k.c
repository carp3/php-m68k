/*
  +----------------------------------------------------------------------+
  | M68K PHP extension                                                   |
  +----------------------------------------------------------------------+
  | Copyright (c) 2022 Pedram Azimaei                                    |
  +----------------------------------------------------------------------+
  | Permission is hereby granted, free of charge, to any person          |
  | obtaining a copy of this software and associated documentation files |
  | (the "Software"), to deal in the Software without restriction,       |
  | including without limitation the rights to use, copy, modify, merge, |
  | publish, distribute, sublicense, and/or sell copies of the Software, |
  | and to permit persons to whom the Software is furnished to do so,    |
  | subject to the following conditions:                                 |
  |                                                                      |
  | The above copyright notice and this permission notice shall be       |
  | included in all copies or substantial portions of the Software.      |
  |                                                                      |
  | THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,      |
  | EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF   |
  | MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND                |
  | NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS  |
  | BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN   |
  | ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN    |
  | CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE     |
  | SOFTWARE.                                                            |
  +----------------------------------------------------------------------+
  | Author: Pedram Azimaei <pedram.azimaie@gmail.com>                    |
  +----------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "m68k.h"
#include "musashi//m68k.h"
#include "zend_exceptions.h"
#include "ext/standard/info.h"
#include "musashi/m68k.h"


typedef struct m68k_ext_runtime_config {
    zend_fcall_info fcall_read_mem_8;
    zend_fcall_info_cache fcall_read_mem_8_cache;
    zend_fcall_info fcall_read_mem_16;
    zend_fcall_info_cache fcall_read_mem_16_cache;
    zend_fcall_info fcall_read_mem_32;
    zend_fcall_info_cache fcall_read_mem_32_cache;

    zend_fcall_info fcall_write_mem_8;
    zend_fcall_info_cache fcall_write_mem_8_cache;
    zend_fcall_info fcall_write_mem_16;
    zend_fcall_info_cache fcall_write_mem_16_cache;
    zend_fcall_info fcall_write_mem_32;
    zend_fcall_info_cache fcall_write_mem_32_cache;

} m68k_ext_runtime_config;

m68k_ext_runtime_config m68k_ext_runtime_config_holder;

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_m68k_pulse_reset, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_m68k_init, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_m68k_set_read_memory_8_callback, 0, 1, IS_VOID, 0)
                ZEND_ARG_TYPE_INFO(0, reg, IS_CALLABLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_m68k_set_read_memory_16_callback, 0, 1, IS_VOID, 0)
                ZEND_ARG_TYPE_INFO(0, reg, IS_CALLABLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_m68k_set_read_memory_32_callback, 0, 1, IS_VOID, 0)
                ZEND_ARG_TYPE_INFO(0, reg, IS_CALLABLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_m68k_set_write_memory_8_callback, 0, 1, IS_VOID, 0)
                ZEND_ARG_TYPE_INFO(0, reg, IS_CALLABLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_m68k_set_write_memory_16_callback, 0, 1, IS_VOID, 0)
                ZEND_ARG_TYPE_INFO(0, reg, IS_CALLABLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_m68k_set_write_memory_32_callback, 0, 1, IS_VOID, 0)
                ZEND_ARG_TYPE_INFO(0, reg, IS_CALLABLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_m68k_get_reg, 0, 1, IS_LONG, 0)
                ZEND_ARG_TYPE_INFO(0, reg, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_m68k_execute, 0, 1, IS_LONG, 0)
                ZEND_ARG_TYPE_INFO(0, reg, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_m68k_set_irq, 0, 1, IS_VOID, 0)
                ZEND_ARG_TYPE_INFO(0, level, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* Add all functions. (Keep PHP_FE_END as last element) */
static const zend_function_entry functions[] = {
        PHP_FE(m68k_pulse_reset, arginfo_m68k_pulse_reset)
        PHP_FE(m68k_get_reg, arginfo_m68k_get_reg)
        PHP_FE(m68k_execute, arginfo_m68k_execute)
        PHP_FE(m68k_set_irq, arginfo_m68k_set_irq)
        PHP_FE(m68k_init, arginfo_m68k_init)
        PHP_FE(m68k_set_read_memory_8_callback, arginfo_m68k_set_read_memory_8_callback)
        PHP_FE(m68k_set_read_memory_16_callback, arginfo_m68k_set_read_memory_16_callback)
        PHP_FE(m68k_set_read_memory_32_callback, arginfo_m68k_set_read_memory_32_callback)
        PHP_FE(m68k_set_write_memory_8_callback, arginfo_m68k_set_write_memory_8_callback)
        PHP_FE(m68k_set_write_memory_16_callback, arginfo_m68k_set_write_memory_16_callback)
        PHP_FE(m68k_set_write_memory_32_callback, arginfo_m68k_set_write_memory_32_callback)
        PHP_FE_END
};

zend_module_entry m68k_module_entry = {STANDARD_MODULE_HEADER, PHP_M68K_EXTNAME, functions, NULL, NULL, NULL, NULL,
                                       NULL, PHP_M68K_VERSION, STANDARD_MODULE_PROPERTIES};

#ifdef COMPILE_DL_M68K
ZEND_GET_MODULE(m68k)
#endif


PHP_FUNCTION (m68k_pulse_reset) {
    m68k_pulse_reset();
}

PHP_FUNCTION (m68k_init) {
    m68k_init();
    m68k_set_cpu_type(M68K_CPU_TYPE_68000);
}

PHP_FUNCTION (m68k_get_reg) {
    zend_long reg;
    zend_ulong retval;

    ZEND_PARSE_PARAMETERS_START(1, 1)
            Z_PARAM_LONG(reg)ZEND_PARSE_PARAMETERS_END();
    retval = m68k_get_reg(NULL, reg & 0xff);

    RETURN_LONG(retval);
}

PHP_FUNCTION (m68k_execute) {
    zend_long reg;
    zend_ulong retval;

    ZEND_PARSE_PARAMETERS_START(1, 1)
            Z_PARAM_LONG(reg)ZEND_PARSE_PARAMETERS_END();
    retval = m68k_execute(reg & 0xffff);
    RETURN_LONG(retval);
}

PHP_FUNCTION (m68k_set_irq) {
    zend_long level;

    ZEND_PARSE_PARAMETERS_START(1, 1)
            Z_PARAM_LONG(level)ZEND_PARSE_PARAMETERS_END();
    m68k_set_irq(level & 0xff);
}


PHP_FUNCTION (m68k_set_read_memory_8_callback) {
    ZEND_PARSE_PARAMETERS_START(1, 1)
            Z_PARAM_FUNC_OR_NULL(m68k_ext_runtime_config_holder.fcall_read_mem_8,
                                 m68k_ext_runtime_config_holder.fcall_read_mem_8_cache)ZEND_PARSE_PARAMETERS_END();
}

PHP_FUNCTION (m68k_set_read_memory_16_callback) {
    ZEND_PARSE_PARAMETERS_START(1, 1)
            Z_PARAM_FUNC_OR_NULL(m68k_ext_runtime_config_holder.fcall_read_mem_16,
                                 m68k_ext_runtime_config_holder.fcall_read_mem_16_cache)ZEND_PARSE_PARAMETERS_END();
}

PHP_FUNCTION (m68k_set_read_memory_32_callback) {
    ZEND_PARSE_PARAMETERS_START(1, 1)
            Z_PARAM_FUNC_OR_NULL(m68k_ext_runtime_config_holder.fcall_read_mem_32,
                                 m68k_ext_runtime_config_holder.fcall_read_mem_32_cache)ZEND_PARSE_PARAMETERS_END();
}

PHP_FUNCTION (m68k_set_write_memory_8_callback) {
    ZEND_PARSE_PARAMETERS_START(1, 1)
            Z_PARAM_FUNC_OR_NULL(m68k_ext_runtime_config_holder.fcall_write_mem_8,
                                 m68k_ext_runtime_config_holder.fcall_write_mem_8_cache)ZEND_PARSE_PARAMETERS_END();
}

PHP_FUNCTION (m68k_set_write_memory_16_callback) {
    ZEND_PARSE_PARAMETERS_START(1, 1)
            Z_PARAM_FUNC_OR_NULL(m68k_ext_runtime_config_holder.fcall_write_mem_16,
                                 m68k_ext_runtime_config_holder.fcall_write_mem_16_cache)ZEND_PARSE_PARAMETERS_END();
}

PHP_FUNCTION (m68k_set_write_memory_32_callback) {
    ZEND_PARSE_PARAMETERS_START(1, 1)
            Z_PARAM_FUNC_OR_NULL(m68k_ext_runtime_config_holder.fcall_write_mem_32,
                                 m68k_ext_runtime_config_holder.fcall_write_mem_32_cache)ZEND_PARSE_PARAMETERS_END();
}

unsigned int m68k_read_memory_8(unsigned int address) {

    zval params[1];
    zval retval;

    ZVAL_LONG(&params[0], address);
    m68k_ext_runtime_config_holder.fcall_read_mem_8.params = params;
    m68k_ext_runtime_config_holder.fcall_read_mem_8.param_count = 1;
    m68k_ext_runtime_config_holder.fcall_read_mem_8.retval = &retval;


    zend_call_function(&m68k_ext_runtime_config_holder.fcall_read_mem_8,
                       &m68k_ext_runtime_config_holder.fcall_read_mem_8_cache);

    return zval_get_long(&retval);

}

unsigned int m68k_read_memory_16(unsigned int address) {
    zval params[1];
    zval retval;

    ZVAL_LONG(&params[0], address);
    m68k_ext_runtime_config_holder.fcall_read_mem_16.params = params;
    m68k_ext_runtime_config_holder.fcall_read_mem_16.param_count = 1;
    m68k_ext_runtime_config_holder.fcall_read_mem_16.retval = &retval;


    zend_call_function(&m68k_ext_runtime_config_holder.fcall_read_mem_16,
                       &m68k_ext_runtime_config_holder.fcall_read_mem_16_cache);

    return zval_get_long(&retval);
}

unsigned int m68k_read_memory_32(unsigned int address) {
    zval params[1];
    zval retval;

    ZVAL_LONG(&params[0], address);
    m68k_ext_runtime_config_holder.fcall_read_mem_32.params = params;
    m68k_ext_runtime_config_holder.fcall_read_mem_32.param_count = 1;
    m68k_ext_runtime_config_holder.fcall_read_mem_32.retval = &retval;


    zend_call_function(&m68k_ext_runtime_config_holder.fcall_read_mem_32,
                       &m68k_ext_runtime_config_holder.fcall_read_mem_32_cache);

    return zval_get_long(&retval);
}

void m68k_write_memory_8(unsigned int address, unsigned int value) {
    zval params[2];

    ZVAL_LONG(&params[0], address);
    ZVAL_LONG(&params[1], value);
    zval retval;
    m68k_ext_runtime_config_holder.fcall_write_mem_8.retval = &retval;
    m68k_ext_runtime_config_holder.fcall_write_mem_8.params = params;
    m68k_ext_runtime_config_holder.fcall_write_mem_8.param_count = 2;


    zend_call_function(&m68k_ext_runtime_config_holder.fcall_write_mem_8,
                       &m68k_ext_runtime_config_holder.fcall_write_mem_8_cache);
}

void m68k_write_memory_16(unsigned int address, unsigned int value) {
    zval params[2];

    ZVAL_LONG(&params[0], address);
    ZVAL_LONG(&params[1], value);
    zval retval;
    m68k_ext_runtime_config_holder.fcall_write_mem_16.retval = &retval;
    m68k_ext_runtime_config_holder.fcall_write_mem_16.params = params;
    m68k_ext_runtime_config_holder.fcall_write_mem_16.param_count = 2;


    zend_call_function(&m68k_ext_runtime_config_holder.fcall_write_mem_16,
                       &m68k_ext_runtime_config_holder.fcall_write_mem_16_cache);
}

void m68k_write_memory_32(unsigned int address, unsigned int value) {
    zval params[2];

    ZVAL_LONG(&params[0], address);
    ZVAL_LONG(&params[1], value);
    zval retval;
    m68k_ext_runtime_config_holder.fcall_write_mem_32.retval = &retval;
    m68k_ext_runtime_config_holder.fcall_write_mem_32.params = params;
    m68k_ext_runtime_config_holder.fcall_write_mem_32.param_count = 2;


    zend_call_function(&m68k_ext_runtime_config_holder.fcall_write_mem_32,
                       &m68k_ext_runtime_config_holder.fcall_write_mem_32_cache);
}



