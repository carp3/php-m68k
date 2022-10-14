# Motorola M68000 PHP Emulator extension

A simple wrapper for Musashi's M68K Emulator library



---

## Requirements

* PHP 8.x

## Installation

    phpize
    ./configure
    make
    make test
    make install

Add the following line to your `php.ini`

    extension=m68k.so

## Docs
Please refer to [Musashi's README file](https://github.com/kstenerud/Musashi).
## Extra Functions

You need to register your memory call backs. you must call these methods BEFORE calling `m68k_pulse_reset` or `m68k_m68k_execute`


Example:
```php
    m68k_init();
    
    
    m68k_set_read_memory_8_callback('read8');
    m68k_set_read_memory_16_callback('read16');
    m68k_set_read_memory_32_callback('read32');


    m68k_set_write_memory_8_callback('write8');
    m68k_set_write_memory_16_callback('write16');
    m68k_set_write_memory_32_callback('write32');
    
    
    m68k_pulse_reset();
    m68k_m68k_execute();
    

    function read16(int $address) :int
    {
        ...
        return $value;
    }

    function read32(int $address) :int
    {
        ...
        return $value;
    }

    function read8(int $address) :int
    {
        ...
        return $value;
    }

    function write16(int $address, int $value)
    {
        ...
    }

    function write32(int $address,int $value)
    {
        ...
    }

    function write8(int $address, int $value)
    {
        ...
    }
    
```

