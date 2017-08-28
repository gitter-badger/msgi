#include "stdafx.h"
#include "LibGV.hpp"
#include "Actor.hpp"
#include <assert.h>
#include "LibDG.hpp"

// TODO: Actually a SYSTEM alloc from lib DG
struct struct_8
{
    DWORD mId;
    DWORD* field_4;
};
static_assert(sizeof(struct_8) == 0x8, "struct_8 should be 0x8");
struct struct_lib_gv
{
    Actor mBase;
    DWORD gRenderedFramesCount_dword_6BFF00;
    DWORD dword_6BFF04_time_related;
    struct_8* struct_8_ptr_6BFF08; // Seems to point to one of mStruct8_128Array_06BFF80
    GV_FnPtr field_6BFF0C_fn_ptrs[26];
    struct_8* struct_8_ptr_6BFF74;
    struct_8* struct_8_ptr_6BFF78;
    //DWORD pad_field_6BFF7C; // TODO: This isn't in ida, how can the rest of this struct be correct????
    struct_8 mStruct8_128Array_06BFF80[128];
    DWORD mDWORD_Pad1;
    DWORD gGv_dword_6C0380;
    DWORD gGv_dword_6C0384;
    DWORD gGv_dword_6C0388[6];
    DWORD gGv_dword_6C03A0;
    DWORD gGv_dword_6C03A4;
    DWORD gGv_dword_6C03A8;
    DWORD mDWORD_Pad2;
    DWORD gGv_dword_6C03B0_array1[81];
    DWORD gGv_dword_6C04F4_array2[81];
    DWORD gGv_dword_6C0638_active_array_idx;
};
MSG_ASSERT_SIZEOF(struct_lib_gv, 0x75C);

MGS_VAR(1, 0x6BFEE0, struct_lib_gv, g_lib_gv_stru_6BFEE0, {});

// Other likely LibGvd funcs
MSG_FUNC_NOT_IMPL(0x40A72A, struct_8 *__cdecl(), LibGvd_sub_40A72A);
MSG_FUNC_NOT_IMPL(0x40A6CD, char* __cdecl(), LibGvd_sub_40A6CD);
MSG_FUNC_NOT_IMPL(0x40A603, int __cdecl(int), LibGvd_sub_40A603);

void LibGVCppForceLink() { }

MSG_FUNC_NOT_IMPL(0x40B35E, void __cdecl(), LibGV_Reset_System2_Memory_40B35E);
MSG_FUNC_NOT_IMPL(0x40A6AC, void __cdecl(), LibGV_Init_Allocs_40A6AC);
MSG_FUNC_NOT_IMPL(0x40A4B1, void __cdecl(), sub_40A4B1);

MSG_FUNC_NOT_IMPL(0x4455A0, __int64 __cdecl(), TimingRelated_4455A0);

MGS_VAR(1, 0x791A04, DWORD, dword_791A04, 0);
MGS_VAR_EXTERN(int, gActiveBuffer_dword_791A08);

void __cdecl LibGV_40B3BC()
{
    g_lib_gv_stru_6BFEE0.gGv_dword_6C03B0_array1[0] = 0;
    g_lib_gv_stru_6BFEE0.gGv_dword_6C04F4_array2[0] = 0;
    g_lib_gv_stru_6BFEE0.gGv_dword_6C0638_active_array_idx = 0;
}

void __cdecl LibGV_Update_40A54E(Actor* pActor)
{
    ++g_lib_gv_stru_6BFEE0.gRenderedFramesCount_dword_6BFF00;

    int currentTime = TimingRelated_4455A0(); // TODO: Truncation?
    int timeDiff = currentTime - g_lib_gv_stru_6BFEE0.dword_6BFF04_time_related;
    g_lib_gv_stru_6BFEE0.dword_6BFF04_time_related = currentTime;
    dword_791A04 = timeDiff;

    if (!gLibDG_2_stru_6BB930.dword_6BB950_do_not_flip_buffers)
    {
        // flip active buffer
        gActiveBuffer_dword_791A08 = !gActiveBuffer_dword_791A08;
    }

    if (!gActorPauseFlags_dword_791A0C)
    {
        assert(g_lib_gv_stru_6BFEE0.gGv_dword_6C0638_active_array_idx >=0 && g_lib_gv_stru_6BFEE0.gGv_dword_6C0638_active_array_idx <= 1);
        g_lib_gv_stru_6BFEE0.gGv_dword_6C0638_active_array_idx = 1 - g_lib_gv_stru_6BFEE0.gGv_dword_6C0638_active_array_idx;

        if (g_lib_gv_stru_6BFEE0.gGv_dword_6C0638_active_array_idx != 0)
        {
            g_lib_gv_stru_6BFEE0.gGv_dword_6C03B0_array1[0] = 0;
        }
        else
        {
            g_lib_gv_stru_6BFEE0.gGv_dword_6C04F4_array2[0] = 0;
        }
    } 
}
MSG_FUNC_IMPL(0x40A54E, LibGV_Update_40A54E);


MGS_VAR_EXTERN(int, gActiveBuffer_dword_791A08);

void LibGv_Init_sub_40A4F6()
{
    ActorList_Init();
    LibGV_Reset_System2_Memory_40B35E();
    LibGv_ClearFunctionPointers_40A69D();
    LibGV_Init_Allocs_40A6AC();
    sub_40A4B1();
    Actor_PushBack(0, &g_lib_gv_stru_6BFEE0.mBase, nullptr);
    Actor_Init(&g_lib_gv_stru_6BFEE0.mBase, LibGV_Update_40A54E, nullptr, "C:\\mgs\\source\\LibGV\\gvd.c");
    gActiveBuffer_dword_791A08 = 0;
    g_lib_gv_stru_6BFEE0.gRenderedFramesCount_dword_6BFF00 = 0;
    //nullsub_6(nullsub_4);
}
MSG_FUNC_IMPL(0x40A4F6, LibGv_Init_sub_40A4F6);


void CC LibGv_ClearFunctionPointers_40A69D()
{
    memset(g_lib_gv_stru_6BFEE0.field_6BFF0C_fn_ptrs, 0, sizeof(g_lib_gv_stru_6BFEE0.field_6BFF0C_fn_ptrs));
}
MSG_FUNC_IMPL(0x40A69D, LibGv_ClearFunctionPointers_40A69D);


void __cdecl LibGV_SetFnPtr_sub_40A68D(char id, GV_FnPtr fn)
{
    // Convert the a-z index to 0-25
    const int idx = id - 'a';

    assert(idx < _countof(g_lib_gv_stru_6BFEE0.field_6BFF0C_fn_ptrs));
    g_lib_gv_stru_6BFEE0.field_6BFF0C_fn_ptrs[idx] = fn;
}
MSG_FUNC_IMPL(0x40A68D, LibGV_SetFnPtr_sub_40A68D);
