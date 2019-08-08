#include <lua.hpp> 
#include <string.h> 

struct data
{
  int a;
  int b;
};
int main(int argc ,char** argv)
{
  lua_State *L = luaL_newstate();

  luaL_openlibs(L);

  int ret = 0 ;
  //加载lua脚本文件
  if (luaL_loadfile(L,"func.lua"))
  {
    printf("call error: %s\n", lua_tostring(L, -1));
    return -1;
  }
  lua_resume( L, 0 );
  lua_getglobal(L,"lua_test"); // 找到switch函数的入口
  int t = lua_type(L, LUA_GLOBALSINDEX);
  printf("type: %d %s\n", t, lua_typename(L, lua_type(L,LUA_GLOBALSINDEX)));
  printf("stack size: %d\n", lua_gettop(L));
  lua_pushstring(L, "this is a");
  lua_pushstring(L, "this is b");
  printf("stack size: %d\n", lua_gettop(L));
  if (lua_pcall(L, 2, 1, NULL))
  {
    printf("call error: %s\n", lua_tostring(L, -1));
    return -1;
  }

  lua_getfield(L, -1, "a");  

  printf("%s\n", lua_tostring(L, -1));
  printf("stack size: %d\n", lua_gettop(L));

  lua_pop(L, 1);

  lua_getfield(L, -1, "b");  
  printf("%s\n", lua_tostring(L, -1));
  printf("stack size: %d\n", lua_gettop(L));

  lua_settop(L, 0);
  printf("clear lua statck\n");

  /************************* test table ********************/
  lua_getglobal(L, "test_table");
  printf("stack size: %d\n", lua_gettop(L));
  lua_newtable(L);
  printf("stack size: %d\n", lua_gettop(L));

  lua_pushstring(L, "Hello Lua");
  lua_setfield(L, -2, "lua");

  lua_pushstring(L, "Hello C");
  lua_setfield(L, -2, "C");

  lua_pushstring(L, "Hello C++");
  lua_setfield(L, -2, "C++");

  lua_pushstring(L, "Hello Python");
  lua_setfield(L, -2, "Python");

  lua_pushstring(L, "Hello Java");
  lua_setfield(L, -2, "java");

  if (lua_pcall(L, 1, 0, NULL))
  {
    printf("call error: %s\n", lua_tostring(L, -1));
    return -1;
  }
  /************************* test userdata ********************/
  lua_settop(L, 0);
  lua_getglobal(L, "test_userdata");
  int size = sizeof(struct data);
  data* pd = (data*)lua_newuserdata(L, size);
  pd->a = 2;
  pd->b = 4;
  printf("stack size: %d\n", lua_gettop(L));
  if (lua_pcall(L, 1, 0, NULL))
  {
    printf("call error: %s\n", lua_tostring(L, -1));
    return -1;
  }

  lua_getglobal(L, "test_userdata1");
  size = sizeof(struct data);
  data* pd2 = (data*)lua_newuserdata(L, size);
  pd2->a = 2;
  pd2->b = 4;
  printf("stack size: %d\n", lua_gettop(L));
  if (lua_pcall(L, 1, 0, NULL))
  {
    printf("call error: %s\n", lua_tostring(L, -1));
    return -1;
  }

  lua_getglobal(L, "user_data");
  printf("type: %d %s\n", t, lua_typename(L, lua_type(L,-1)));
  data* pd3 = (data*)lua_touserdata(L, -1);
  printf("%d %d\n",  pd3->a, pd3->b);

  printf("****** test ligthuserdata function **********\n");
  lua_getglobal(L, "test_ligthuserdata");
  struct data pd5 = {a:100,b:200,};
  //struct data pd6 = {.a = 100, .b=200,};
  lua_pushlightuserdata(L, &pd5);
  if (lua_pcall(L, 1, 1, NULL))
  {
    printf("call error: %s\n", lua_tostring(L, -1));
    return -1;
  }
  data* pd6 = (data*)lua_touserdata(L, -1);
  printf("%d %d\n", pd6->a, pd6->b);




  

  // lua_pop(L, 1);   //弹出表的value
  //
  // lua_getfield(L, -1, "code");
  //
  // printf("%d\n", lua_tointeger(L, -1));

  lua_close(L);
  return 0;

}
