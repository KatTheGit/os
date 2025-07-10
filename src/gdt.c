// ACCESS BYTE
// P, DPL (2 bits), S, E, DC, RW, A
//
// P: present bit. Set to 1.
// DPL: Descriptor privilege level bits. 0 = kernel, 3 = user.
// S: descriptor type. 0 = system segment like task state segment, 1 = data.
// E: executable. 0 = data, 1 = exec
// DC: Direction bit. 0: segment grows UP. 1: segment grows down. For code
// segment: whether code in this segment can be executed by lower privilege
// level.
// RW: Readable/Writeable bit. For code: Wether can be read. For data: whether
// can be written to.
// A: Accesed bit, Whether was accessed. CPU sets this.
#define ACCESS_BYTE_KERNEL_CODE 0b10011010
#define ACCESS_BYTE_KERNEL_DATA 0b10010010
#define ACCESS_BYTE_USER_CODE   0b11111010
#define ACCESS_BYTE_USER_DATA   0b11110010

// FLAGS
// G, DB, L, reserved
// G: granularity. size limit value is scaled by. 0 for byte, 1 for 4KiB
// DB: size flag. 0: 16 bit, 1: 32 bit.
// L: Long mode. 1: 64 bit. 0: the prior.
// reserved: 0
#define GDT_FLAG 0b00001100


/**
 * Generates a segment descriptor for the GDT.
 * https://wiki.osdev.org/Global_Descriptor_Table#Segment_Descriptor
 * base is a 32 bit value.
 * limit is a 20 bit value.
 */
uint64_t gdt_entry(uint32_t base, uint32_t limit,
                   uint8_t access_byte, uint8_t flags) {
  
  // resize variables to make bitwise operations easier
  uint64_t access_byte_64 = access_byte;
  uint64_t flags_64 = flags;


  uint64_t segment_descriptor = 0;
  
  // place flags and access byte
  segment_descriptor |= (flags_64 << 52);
  segment_descriptor |= (access_byte_64 << 40);

  // place the limit in the descriptor.
  uint64_t limit_p1 =  limit & 0b00001111111111111111;
  uint64_t limit_p2 = (limit & 0b11110000000000000000) >> 16;

  segment_descriptor |= limit_p1;
  segment_descriptor |= (limit_p2 << 48);

  // place the base in the descriptor
  uint64_t base_p1_p2 =  base & 0b00000000111111111111111111111111;
  uint64_t base_p3    = (base & 0b11111111000000000000000000000000) >> 24;

  segment_descriptor |= (base_p1_p2 << 16);
  segment_descriptor |= (base_p3 << 56);

  return segment_descriptor;
}

void set_gdt(uint16_t limit, uint64_t * base);
void gdt_init() {

  // GDT
  int gdt_size = 5;
  uint64_t gdt[gdt_size];

  // pointer to GDT
  uint64_t * gdt_pointer = &gdt[0];


  gdt[0] = 0; // null descriptor
  gdt[1] = gdt_entry(0, 0x000FFFFF, ACCESS_BYTE_KERNEL_CODE, GDT_FLAG); // 0x08
  gdt[2] = gdt_entry(0, 0x000FFFFF, ACCESS_BYTE_KERNEL_DATA, GDT_FLAG); // 0x10
  gdt[3] = gdt_entry(0, 0x000FFFFF, ACCESS_BYTE_USER_CODE, GDT_FLAG);   // 0x18
  gdt[4] = gdt_entry(0, 0x000FFFFF, ACCESS_BYTE_USER_DATA, GDT_FLAG);   // 0x20

  // tell the cpu where the gdt is, and reload segment registers
  // gdt.asm
  set_gdt(gdt_size*8-1, gdt_pointer);
}
