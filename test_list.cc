#include "list.h"
#include <stdlib.h>
#include <stdio.h>

struct Entry {
  struct list_head list;    
  int a;
  int b;
};


int main()
{
  LIST_HEAD(head); 
  Entry* entry = (Entry*)malloc(sizeof(Entry));
  entry->a = 100;
  entry->b = 200;
  list_add_tail(&entry->list, &head);
  entry = (Entry*)malloc(sizeof(Entry));
  entry->a = 300;
  entry->b = 400;
  list_add_tail(&entry->list, &head);
  Entry* entry2 = (Entry*)malloc(sizeof(Entry));
  entry2->a = 500;
  entry2->b = 600;
  list_add_tail(&entry2->list, &head);
  struct list_head* pos;
  list_for_each(pos, &head)
  {
    Entry* tmp = list_entry(pos, Entry, list);
    printf("%d %d\n", tmp->a, tmp->b);
  }
  list_del(&entry->list);
  list_for_each(pos, &head)
  {
    Entry* tmp = list_entry(pos, Entry, list);
    printf("%d %d\n", tmp->a, tmp->b);
  }

  
  return 0;
}
