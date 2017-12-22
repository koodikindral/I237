#ifndef MAT_HWDEFS_H
#define MAT_HWDEFS_H

#define DDR(x) (*(&x - 1))
#define PIN(x) (*(&x - 2))

#define MFRC522_SS_PORT     PORTB
#define MFRC522_SS_BIT      0

#define MFRC522_RST_PORT    PORTL
#define MFRC522_RST_BIT     0

#define SCK_DDR     DDRB
#define SCK_BIT     DDB1
#define MOSI_DDR    DDRB
#define MOSI_BIT    DDB2
#define MISO_DDR    DDRB
#define MISO_BIT    DDB3

#endif