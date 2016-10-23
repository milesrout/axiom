static __inline__ void port_out8(uint16_t port, uint8_t val)
{
    __asm__ __volatile__("outb %0, %1" : : "a"(val), "Nd"(port) );
}
static __inline__ void port_out16(uint16_t port, uint16_t val)
{
    __asm__ __volatile__("outw %0, %1" : : "a"(val), "Nd"(port) );
}
static __inline__ void port_out32(uint16_t port, uint32_t val)
{
    __asm__ __volatile__("outl %0, %1" : : "a"(val), "Nd"(port) );
}
static __inline__ uint8_t port_in8(uint16_t port)
{
    uint8_t ret;
    __asm__ __volatile__("inb %1, %0"
                 : "=a"(ret)
                 : "Nd"(port));
    return ret;
}
static __inline__ uint16_t port_in16(uint16_t port)
{
    uint16_t ret;
    __asm__ __volatile__("inw %1, %0"
                 : "=a"(ret)
                 : "Nd"(port));
    return ret;
}
static __inline__ uint32_t port_in32(uint16_t port)
{
    uint32_t ret;
    __asm__ __volatile__("inl %1, %0"
                 : "=a"(ret)
                 : "Nd"(port));
    return ret;
}
