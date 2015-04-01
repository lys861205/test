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
	static const int N = 60; //�ܹ����ٸ���

	static const int SI = 1;//ÿ���۵�ʱ����

	tw_timer* slots[N]; //ÿ�����Ǹ�ʱ������
	
	int cur_slot;  //��ǰ��
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

		int rotation = ticks / N;  //����Ȧ

		int ts = (cur_slot + ticks % N ) % N; //�ڼ�����

		tw_timer* timer = new tw_timer(rotation, ts, timeout);

		if(!slots[ts]) //��ǰ�����κ�Ԫ�أ��Ѹö�ʱ������Ϊ�ò۵�ͷ���
		{
			slots[ts] = timer;
		}
		else   //�Ѹö�ʱ�����뵽�ò۵Ķ�ʱ������ǰ��
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

		if(timer == slots[ts]) //ɾ�����ǲ۵�ͷ���
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
		//�Ӳ�������ɾ����ʱ��


		//�Ѷ�ʱ�����뵽����
		int timeout = timer->timeout;
		int ticks = timeout/SI;
		int q_slot = (cur_slot + ticks % N ) % N;
		if(timer->time_slot == q_slot) //���Ҫɾ���Ķ�ʱ���Ĳ۸���ǰ��ʱ���Ĳ���ȿ���ֱ�ӷ���
		{
			return;
		}
		
		//�ڶ�Ӧ�Ĳ���ɾ����ʱ��
		int ts = timer->time_slot;
		if(timer == slots[ts]) //ɾ�����ǲ۵�ͷ���
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

		//�ı䶨ʱ���ĵ�ǰ��
		timer->time_slot = q_slot;
		if(!slots[ts]) //��ǰ�����κ�Ԫ�أ��Ѹö�ʱ������Ϊ�ò۵�ͷ���
		{
			slots[ts] = timer;
		}
		else   //�Ѹö�ʱ�����뵽�ò۵Ķ�ʱ������ǰ��
		{
			timer->next = slots[ts];
			slots[ts]->prev = timer;
			slots[ts] = timer;
		}
	}

	//SIʱ�䵽�����øú���,ʱ������ǰ����һ���Ĳ۵ļ��
	void tick()
	{
		tw_timer* timer = slots[cur_slot];
		while(timer)
		{
			if(timer->rotation > 0) //ת��Ȧ��
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

		

		cur_slot = ++cur_slot % N;  //����ʱ���ֵĵ�ǰ�ۣ��Է�ӳʱ���ֵ�ת��
	}
};



#endif // TIME_WHEEL_TIMER
