#define KBD 0x60

unsigned char read_scan_code(void){
    return inb(KBD);
}