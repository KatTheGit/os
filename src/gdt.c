// GDT
uint64_t gdt[2];
// pointer to GDT
uint64_t * gdt_pointer = &gdt[0];


void mkgdt() {
  // null descriptor
  gdt[0] = 0;
  gdt[1] = 1;
}