// ��3��ͷ�ļ�����������
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include "stdio.h"

// test.hello()
// hello lua!!
static int test_hello(lua_State* L)
{
	printf("hello lua!!\r\n");
	return 0;
}

// test.fn1(3)
// 3
// test.fn1(1, 2, 3) // ����ջ��
// 3
static int test_fn1(lua_State* L) 
{
	return 1; // ���ش���Luaջ�Ĳ����ĸ���
}

// test.typeName(123)
// lua type : number
// test.typeName("asdf")
// lua type : string
// test.typeName(function() end)
// lua type : function
// test.typeName()
// lua type : no value
// test.typeName(nil)
// lua type : nil
static int test_type_name(lua_State* L)
{
	int ltype = lua_type(L, 1);
	printf("lua type: %s\r\n", lua_typename(L, ltype));
	return 0; // ���ش���Luaջ�Ĳ����ĸ���
}

// ����ʵ���ˣ���Lua�е���C����
static const struct luaL_Reg testlib[] =
{
	{"hello", test_hello},
	{"fn1", test_fn1},
	{"typeName", test_type_name},
	{NULL, NULL}
};

// ������ӵ�test��
int lua_opentest(lua_State* L)
{
	luaL_newlib(L, testlib);
	return 1;
}