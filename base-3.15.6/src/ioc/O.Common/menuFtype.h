/* menuFtype.h generated from menuFtype.dbd */

#ifndef INC_menuFtype_H
#define INC_menuFtype_H

#ifndef menuFtype_NUM_CHOICES
typedef enum {
    menuFtypeSTRING                 /* STRING */,
    menuFtypeCHAR                   /* CHAR */,
    menuFtypeUCHAR                  /* UCHAR */,
    menuFtypeSHORT                  /* SHORT */,
    menuFtypeUSHORT                 /* USHORT */,
    menuFtypeLONG                   /* LONG */,
    menuFtypeULONG                  /* ULONG */,
    menuFtypeFLOAT                  /* FLOAT */,
    menuFtypeDOUBLE                 /* DOUBLE */,
    menuFtypeENUM                   /* ENUM */
} menuFtype;
#define menuFtype_NUM_CHOICES 10
#endif


#endif /* INC_menuFtype_H */
