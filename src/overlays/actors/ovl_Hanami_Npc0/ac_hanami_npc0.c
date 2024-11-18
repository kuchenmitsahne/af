#include "ac_hanami_npc0.h"
#include "m_lib.h"
#include "m_actor_dlftbls.h"
#include "m_object.h"
#include "overlays/gamestates/ovl_play/m_play.h"
#include "m_demo.h"
#include "macros.h"
#include "overlays/actors/ovl_Tools/ac_tools.h"
#include "audio.h"

#define THIS ((Hanami_Npc0*)thisx)

void aHM0_actor_ct(Actor* thisx, Game_Play* game_play);
void aHM0_actor_dt(Actor* thisx, Game_Play* game_play);
void aHM0_actor_init(Actor* thisx, Game_Play* game_play);
void aHM0_actor_save(Actor* thisx, Game_Play* game_play);
void func_809DE5DC_jp(Actor* thisx, s32 index);
void func_809DE830_jp(Hanami_Npc0* this, Game_Play* game_play);
void func_809DE800_jp(Actor* thisx, Game_Play* game_play, s32 index);
void func_809DE948_jp(Hanami_Npc0* this, Game_Play* game_play, s32 index);
void func_809DE978_jp(Actor* thisx);

#if 0
ActorProfile Hanami_Npc0_Profile = {
    /* */ ACTOR_HANAMI_NPC0,
    /* */ ACTOR_PART_NPC,
    /* */ 0,
    /* */ 0x0000,
    /* */ GAMEPLAY_KEEP,
    /* */ sizeof(Hanami_Npc0),
    /* */ aHM0_actor_ct,
    /* */ aHM0_actor_dt,
    /* */ aHM0_actor_init,
    /* */ (void*)none_proc1,
    /* */ aHM0_actor_save,
};
#endif

extern struct_809AEFA4 aHM0_ct_data;

void aHM0_actor_ct(Actor* thisx, Game_Play* game_play) {
    // TODO: import data
    // static struct_809AEFA4 aHM0_ct_data = {
    //     &aHM0_actor_move,
    //     &aHM0_actor_draw,
    //     4,
    //     &aHM0_talk_request,
    //     &func_809DEA20_jp,
    //     aHM0_talk_end_chk,
    // };
    
    Hanami_Npc0* this = THIS;
    
    if (common_data.clip.unk_040->unk_BC(thisx, game_play) == 1) {
        this->unk_7C0 = &func_809DE948_jp;
        common_data.clip.unk_040->unk_C0(thisx, game_play, &aHM0_ct_data);
    }
}

void aHM0_actor_save(Actor *thisx, Game_Play *game_play) {
    common_data.clip.unk_040->unk_C8(thisx, game_play);
}

void aHM0_actor_dt(Actor *thisx, Game_Play *game_play) {
    common_data.clip.unk_040->unk_C4(thisx, game_play);
}

void aHM0_actor_init(Actor* thisx, Game_Play* game_play) {
    common_data.clip.unk_040->unk_CC(thisx, game_play);
}

extern s32 D_809DEB00_jp[];

void func_809DE434_jp(Actor* thisx, s32 index) {
    // TODO: import data
    // static s32 D_809DEB00_jp[] = { 0x00000048, 0x0000004A, 0x00000049 };
    common_data.clip.unk_040->unk_104(thisx, D_809DEB00_jp[index], 0, index);
}

void aHM0_actor_move(void) {
    common_data.clip.unk_040->unk_D0();
}

void func_809DE4A0_jp(Actor* thisx) {
    Hanami_Npc0* this = THIS;
    
    this->unk_7D4 = 4;
    this->unk_7D5 = 0x12;
    this->unk_7D6 = 2;
}

// TODO: if confirmed, rename to aHM0_make_tumbler
void func_809DE4C0_jp(Actor* thisx, Game_Play* game_play) {
    Hanami_Npc0* this = THIS;
    ToolActor* tool;

    if (((this->unk_940 & 1) == 1) && (this->unk_860 == 0)) {
        // TODO: resolve warning, possibly by having Hanami_Npc0 struct start with ToolActor
        tool = common_data.clip.toolClip->aTOL_birth_proc(TOOL_TUMBLER, 3, this, game_play, -1, 0);
        if (tool != 0) {
            this->unk_860 = tool;
        }
    }
}

void func_809DE538_jp(Actor* thisx) {
    Hanami_Npc0* this = THIS;

    if (this->unk_188 == 2) {
        if (this->unk_72B == 0) {
            this->unk_7C6 = 0xFF;
            return;
        }
        this->unk_72B--;
    }
}

void func_809DE56C_jp(Actor* thisx) {
    Hanami_Npc0* this = THIS;

    if (this->unk_188 == 2) {
        if (this->unk_72B == 0) {
            this->unk_7C6 = 0xFF;
        } else {
            this->unk_72B--;
        }
    }

    // TODO: resolve warning by either casting &this->actor to (s32) or by changing signatures of both
    // sAdo_OngenPos and Na_OngenPos
    sAdo_OngenPos(&this->actor, this->unk_944, &this->actor.world.pos);
}

void func_809DE5BC_jp(Actor* thisx) {
    Hanami_Npc0* this = THIS;
    
    if (this->unk_188 == 1) {
        this->unk_7C6 = 0xFF;
    }
}

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Hanami_Npc0/ac_hanami_npc0/func_809DE5DC_jp.s")

void func_809DE6B4_jp(Actor* thisx, UNK_TYPE arg1 UNUSED) {
    Hanami_Npc0* this = THIS;

    this->unk_7C9 = 1;
}

void func_809DE6C8_jp(Actor* thisx, UNK_TYPE arg1 UNUSED) {
    func_809DE5DC_jp(thisx, 0);
}

void func_809DE6EC_jp(Actor* thisx, UNK_TYPE arg1 UNUSED) {
    Hanami_Npc0* this = THIS;

    // TODO: Figure out if this takes arguments (could be up to two based on assembly, which would fit with signature)    
    this->unk_93C();
}

extern void* D_809DEB34_jp[];

void func_809DE714_jp(Actor* thisx, UNK_TYPE arg1, s32 processIndex) {
    // TODO: import data
    // static void* D_809DEB34_jp[] = { func_809DE6C8_jp, func_809DE6B4_jp, func_809DE6EC_jp };
    Hanami_Npc0* this = THIS;
    // TODO: confirm whether this is appropriate function type, and update the three signatures
    // of the above function is so.
    ((Hanami_Npc0ActionFunc)D_809DEB34_jp[processIndex])(this, arg1);
}

extern UNK_TYPE D_809DEB40_jp[];

void func_809DE744_jp(Hanami_Npc0* this, Game_Play* game_play UNUSED) {
    // TODO: import data
    // static UNK_TYPE D_809DEB40_jp[] = { 0x00000000, 0x00000001, 0x00000002 };

    if (this->unk_7C6 == 0xFF) {
        if (this->unk_7C5 == 0x12) {
            s32 index = ((s32) (2.0f * fqrand())) << (this->unk_940 & 1);

            func_809DE5DC_jp(&this->actor, D_809DEB40_jp[index]);
        }
        this->unk_80C = 0x137;
        this->unk_911 = 1;
        func_809DE4A0_jp(&this->actor);
    }
}

void func_809DE7D0_jp(Actor* thisx, UNK_TYPE arg1 UNUSED) {
    Hanami_Npc0* this = THIS;

    this->unk_7A8 = 0;
    this->unk_7D0 = func_809DE714_jp;
    func_809DE4A0_jp(thisx);
}

extern Hanami_Npc0ActionFunc D_809DEB4C_jp[];
// TODO: verify signatures of this and of functions in array
void func_809DE800_jp(Actor* thisx, Game_Play* game_play, s32 index) {
    //TODO: import data
    // static Hanami_Npc0ActionFunc D_809DEB4C_jp = { func_809DE7D0_jp, func_809DE744_jp }
    Hanami_Npc0* this = THIS;
    
    (*D_809DEB4C_jp[index])(this, game_play);
}

extern s16 D_809DEB54_jp[4];

void func_809DE830_jp(Hanami_Npc0* this, Game_Play* game_play) {
    // TODO: import data
    // static s16 D_809DEB54_jp[4] = { 0x2000, 0xC000, -0x2000, 0x4000 };
    s32 index = this->actor.fgName + 0xFFFF2FCE;

    this->unk_7A4 = &func_809DE800_jp;
    this->unk_7FD = 0;
    this->unk_8AC = -1;
    this->unk_92B = 1;
    this->unk_92C = 0x48;
    this->unk_940 = index;
    this->unk_80C = 0x137;
    this->unk_911 = 1;
    this->actor.colStatus.mass = MASS_HEAVY;
    {
        s16 temp_v0 = D_809DEB54_jp[index];

        this->actor.shape.rot.y = temp_v0;
        this->actor.world.rot.y = temp_v0;
        this->unk_8DC = temp_v0;
    }
    common_data.clip.unk_040->unk_110(&this->actor, game_play, 8, 0);
}

void func_809DE8D0_jp(Hanami_Npc0* this, Game_Play* game_play) {
    if (common_data.clip.unk_040->unk_110(&this->actor, game_play, -1, 1) == 0) {
        common_data.clip.unk_040->unk_110(&this->actor, game_play, -1, 2);
    }

    func_809DE4C0_jp(&this->actor, game_play);
}

extern Hanami_Npc0ActionFunc D_809DEB5C_jp[];

void func_809DE948_jp(Hanami_Npc0* this, Game_Play* game_play, s32 index) {
    // TODO: import data
    // static Hanami_Npc0ActionFunc D_809DEB5C_jp[] = { func_809DE830_jp, func_809DE8D0_jp };

    (*D_809DEB5C_jp[index])(this, game_play);
}

extern u32 D_809DEB64_jp[];

void func_809DE978_jp(Actor* thisx) {
    // TODO: import data
    // last entry may just be padding
    // static u32 D_809DEB64_jp[] = { 0x0000192D, 0x0000193C, 0x0000191E, 0x0000194B, 0x0000195A, 0x00001969, 0x00000000 };
    Hanami_Npc0* this = THIS;
    enum NpcLooks looks = mNpc_GetNpcLooks(thisx);

    mDemo_Set_msg_num(D_809DEB64_jp[looks] + ((s32) (fqrand() * 3.0f)) + (this->unk_940 * 3));
}

void aHM0_talk_request(Actor* thisx, UNK_TYPE arg1 UNUSED) {
    mDemo_Request(7, thisx, func_809DE978_jp);
}

// TODO: Possibly aHM0_talk_init, which exists in map, but type is different from function of same name
// as found in ac_kamakura_npc0.c and ac_npc_engineer.c (both are void)
s32 func_809DEA20_jp(UNK_TYPE arg0 UNUSED, UNK_TYPE arg1 UNUSED) {
    mDemo_Set_ListenAble();
    
    return 1;
}

s32 aHM0_talk_end_chk(Actor* thisx, UNK_TYPE arg1 UNUSED) {
    s32 result = FALSE;
    
    if (!mDemo_Check(7, thisx)) {
        result = TRUE;
    }
    
    return result;
}

void aHM0_actor_draw(void) {
    common_data.clip.unk_040->unk_E4();
}
