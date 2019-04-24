const unsigned int HZ = 500;

int portGET_IPSR(void)
{
	int result;
	
  __asm volatile ("MRS %0, ipsr" : "=r" (result) );
  
  return (result);
}
