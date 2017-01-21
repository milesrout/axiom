void panic()
{
    __asm__ __volatile__("\tcli\n\thlt":::"memory");
}
