#ifndef _HIFI_H
#define _HIFI_H

typedef unsigned char cfg_u8;
typedef union {
    struct {
        cfg_u8 offset;
        cfg_u8 value;
    }struct_value;
    struct {
        cfg_u8 command;
        cfg_u8 param;
    }struct_param;
}cfg_reg;


#define CFG_META_SWITCH (255)
#define CFG_META_DELAY  (254)
#define CFG_META_BURST  (253)

#endif

