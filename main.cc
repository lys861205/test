#include "timeWheel.hpp"
#include <windows.h>

int main()
{
  time_wheel tWheel;
  tw_timer* pTimer = tWheel.add_timer(10);
  if (!pTimer)
  {
	printf("create timer failed\n");
	return 0;
  }
  pTimer->cb_func = testFun;
  pTimer->data = 10;


  tw_timer* pTimer1 = tWheel.add_timer(65);
  if (!pTimer1)
  {
	printf("create timer failed\n");
	return 0;
  }
  pTimer1->cb_func = testFun;
  pTimer1->data = 15;

  int m = 0;

  while(1)
  {
	++m;
	if (m == 20)
	{
	  tw_timer* pTimer2 = tWheel.add_timer(23);
	  pTimer2->cb_func = testFun;
	  pTimer2->data = 23;
	}
	printf(".");
	Sleep(1000);
	tWheel.tick();
  }
  return 0;
}