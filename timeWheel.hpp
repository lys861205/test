#ifndef TIME_WHEEL_TIMER
#define TIME_WHEEL_TIMER


#include <time.h>
//#include <netinet/in.h>
#include <stdio.h>


#define BUFFER_SIZE 64

class tw_timer;

// struct client_data
// {
// 	sockaddr_in address;
// 	int sockfd;
// 	char buf[BUFFER_SIZE];
// 	tw_timer* timer;
// };
void testFun(int m)
{
  printf("test: %d\n", m);
}
typedef void (*pFun)(int m);
class tw_timer
{
public:
	int rotation;
	int time_slot;
	//void(*cb_func)(client_data*);
	//client_data* user_data;
	pFun cb_func;
	int data;
	tw_timer* next;
	tw_timer* prev;

	int timeout;
public:
	tw_timer(int rot, int ts, int t):
	  next(NULL), prev(NULL), rotation(rot), time_slot(ts), timeout(t)
	  {

	  }
};


class time_wheel
{
	static const int N = 60; //总共多少个槽

	static const int SI = 1;//每个槽的时间间隔

	tw_timer* slots[N]; //每个槽是个时间链表
	
	int cur_slot;  //当前槽
public:
	time_wheel() : cur_slot(0)
	{
		for( int i = 0; i < N; ++i )
		{
			slots[i] = NULL;
		}
	}

	~time_wheel()
	{
		for( int i = 0; i < N; ++i )
		{
			tw_timer* tmp = slots[i];
			while(tmp)
			{
				slots[i] = tmp->next;
				delete tmp;
				tmp = slots[i];
			}
		}
	}

	tw_timer* add_timer( int timeout )
	{
		if( timeout < 0 )
		{
			return NULL;
		}
		int ticks = 0;
		if( timeout < SI )
		{
			ticks = 1;
		}
		else 
		{
			ticks = timeout/SI;
		}

		int rotation = ticks / N;  //多少圈

		int ts = (cur_slot + ticks % N ) % N; //第几个槽

		tw_timer* timer = new tw_timer(rotation, ts, timeout);

		if(!slots[ts]) //当前槽无任何元素，把该定时器设置为该槽的头结点
		{
			slots[ts] = timer;
		}
		else   //把该定时器插入到该槽的定时器的最前面
		{
			timer->next = slots[ts];
			slots[ts]->prev = timer;
			slots[ts] = timer;
		}

		return timer;
	}

	void del_timer(tw_timer* timer)
	{
		if( !timer )
		{
			return;
		}

		int ts = timer->time_slot;

		if(timer == slots[ts]) //删除的是槽的头结点
		{
			slots[ts] = slots[ts]->next;
			if(slots[ts])
			{
				slots[ts]->prev = NULL;
			}
			delete timer;
		}

		else 
		{
			timer->prev->next = timer->next;
			if(timer->next)
				timer->next->prev = timer->prev;
			delete timer;
		}
	}

	void adjust_timer(tw_timer* timer)
	{
		if( !timer )
		{
			return;
		}
		//从槽中链表删除定时器


		//把定时器加入到槽中
		int timeout = timer->timeout;
		int ticks = timeout/SI;
		int q_slot = (cur_slot + ticks % N ) % N;
		if(timer->time_slot == q_slot) //如果要删除的定时器的槽跟当前定时器的槽相等可以直接返回
		{
			return;
		}
		
		//在对应的槽中删除定时器
		int ts = timer->time_slot;
		if(timer == slots[ts]) //删除的是槽的头结点
		{
			slots[ts] = slots[ts]->next;
			if(slots[ts])
			{
				slots[ts]->prev = NULL;
			}
		}
		else 
		{
			timer->prev->next = timer->next;
			if(timer->next)
				timer->next->prev = timer->prev;
		}

		//改变定时器的当前槽
		timer->time_slot = q_slot;
		if(!slots[ts]) //当前槽无任何元素，把该定时器设置为该槽的头结点
		{
			slots[ts] = timer;
		}
		else   //把该定时器插入到该槽的定时器的最前面
		{
			timer->next = slots[ts];
			slots[ts]->prev = timer;
			slots[ts] = timer;
		}
	}

	//SI时间到，调用该函数,时间轮向前滚动一个的槽的间隔
	void tick()
	{
		tw_timer* timer = slots[cur_slot];
		while(timer)
		{
			if(timer->rotation > 0) //转的圈数
			{
				--timer->rotation;
				timer = timer->next;
			}
			else
			{
				timer->cb_func(timer->data);
				if( timer == slots[cur_slot])
				{
					slots[cur_slot] = timer->next;
					delete timer;
					if(slots[cur_slot])
					{
						slots[cur_slot]->prev = NULL;
					}
					timer = slots[cur_slot];
				}
				else
				{
					timer->prev->next = timer->next;
					if(timer->next)
						timer->next->prev = timer->prev;
					tw_timer* ttmp = timer->next;
					delete timer;
					timer = ttmp;
				}
			}

		}

		

		cur_slot = ++cur_slot % N;  //更新时间轮的当前槽，以反映时间轮的转动
	}
};



#endif // TIME_WHEEL_TIMER
