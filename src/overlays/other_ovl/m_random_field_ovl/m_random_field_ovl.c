#include "global.h"
#include "m_random_field.h"

#include "m_common_data.h"
#include "sys_math.h"

typedef enum RandomFieldBit {
  /* 0 */ mRF_BIT_SLOPE_LEFT,
  /* 1 */ mRF_BIT_SLOPE_RIGHT,
  /* 2 */ mRF_BIT_BRIDGE_UPPER,
  /* 3 */ mRF_BIT_BRIDGE_LOWER,
  /* 4 */ mRF_BIT_SHRINE,
  /* 5 */ mRF_BIT_POLICE,
  /* 6 */ mRF_BIT_MUSEUM,
  /* 7 */ mRF_BIT_POOL,
  /* 8 */ mRF_BIT_NEEDLEWORK,

  /* 9 */ mRF_BIT_NUM
} RandomFieldBit;

#define mRF_TO_BIT(type) (1 << (mRF_BIT_##type))

typedef enum RandomFieldBlockGroup {
  /* 0 */ mRF_BLOCK_GROUP_CLIFF,
  /* 1 */ mRF_BLOCK_GROUP_RIVER,
  /* 2 */ mRF_BLOCK_GROUP_BRIDGE,
  /* 3 */ mRF_BLOCK_GROUP_SLOPE,
  /* 4 */ mRF_BLOCK_GROUP_RIVER_CLIFF_ANY,
  /* 5 */ mRF_BLOCK_GROUP_RIVER_CLIFF_1,
  /* 6 */ mRF_BLOCK_GROUP_RIVER_CLIFF_2,
  /* 7 */ mRF_BLOCK_GROUP_RIVER_CLIFF_3,
  /* 8 */ mRF_BLOCK_GROUP_CLIFF_ANY,

  /* 9 */ mRF_BLOCK_GROUP_NUM
} RandomFieldBlockGroup;

typedef enum RandomFieldStepMode {
  /* 0 */ mRF_STEPMODE_TWO,
  /* 1 */ mRF_STEPMODE_THREE,

  /* 2 */ mRF_STEPMODE_NUM
} RandomFieldStepMode;

typedef enum RandomFieldCliffHeight {
  /* 0 */ mRF_CLIFF_HEIGHT_ABOVE,
  /* 1 */ mRF_CLIFF_HEIGHT_BELOW,
  /* 2 */ mRF_CLIFF_HEIGHT_BOTH,

  /* 3 */ mRF_CLIFF_HEIGHT_NUM
} RandomFieldCliffHeight;

typedef enum RandomFieldRiverSide {
  /* 0 */ mRF_RIVER_SIDE_LEFT,
  /* 1 */ mRF_RIVER_SIDE_RIGHT,
  /* 2 */ mRF_RIVER_SIDE_BOTH,

  /* 3 */ mRF_RIVER_SIDE_NUM
} RandomFieldRiverSide;

u8 l_base_blocks[BLOCK_TOTAL_NUM] = {
  0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08,
  0x09, 0x0C, 0x0C, 0x0B, 0x0C, 0x0C, 0x0A,
  0x02, 0x27, 0x27, 0x0E, 0x27, 0x27, 0x04,
  0x02, 0x27, 0x27, 0x27, 0x27, 0x27, 0x04,
  0x02, 0x27, 0x27, 0x27, 0x27, 0x27, 0x04,
  0x02, 0x27, 0x27, 0x27, 0x27, 0x27, 0x04,
  0x02, 0x27, 0x27, 0x27, 0x27, 0x27, 0x04,
  0x53, 0x53, 0x53, 0x53, 0x53, 0x53, 0x53
};

typedef struct cliff_data_s {
  u8* types;
  s32 count;
} mRF_trace_data_c;

u8 l_cliff_startA_factor[] = { mFM_BLOCK_TYPE_CLIFF_HORIZONTAL, mFM_BLOCK_TYPE_CLIFF_CORNER_LEFT_TOP };
mRF_trace_data_c l_cliff_startA = { l_cliff_startA_factor, ARRAY_COUNT(l_cliff_startA_factor) };

u8 l_cliff_startB_factor[] = { mFM_BLOCK_TYPE_CLIFF_HORIZONTAL, mFM_BLOCK_TYPE_CLIFF_CORNER_RIGHT_BOTTOM, mFM_BLOCK_TYPE_CLIFF_CORNER_LEFT_TOP };
mRF_trace_data_c l_cliff_startB = { l_cliff_startB_factor, ARRAY_COUNT(l_cliff_startB_factor) };

u8 l_cliff_startC_factor[] = { mFM_BLOCK_TYPE_CLIFF_HORIZONTAL, mFM_BLOCK_TYPE_CLIFF_CORNER_RIGHT_BOTTOM };
mRF_trace_data_c l_cliff_startC = { l_cliff_startC_factor, ARRAY_COUNT(l_cliff_startC_factor) };

mRF_trace_data_c* l_cliff_start_table[] = {
  &l_cliff_startA,
  &l_cliff_startB,
  &l_cliff_startC
};

u8 l_cliff_next_direct[] = {
  mRF_DIRECT_EAST,
  mRF_DIRECT_NORTH,
  mRF_DIRECT_NORTH,
  mRF_DIRECT_EAST,
  mRF_DIRECT_SOUTH,
  mRF_DIRECT_SOUTH,
  mRF_DIRECT_EAST
};

u8 l_cliff1_next[] = { mFM_BLOCK_TYPE_CLIFF_HORIZONTAL, mFM_BLOCK_TYPE_CLIFF_CORNER_RIGHT_BOTTOM, mFM_BLOCK_TYPE_CLIFF_CORNER_LEFT_TOP };
mRF_trace_data_c l_cliff1_next_class = { l_cliff1_next, ARRAY_COUNT(l_cliff1_next) };

u8 l_cliff2_next[] = { mFM_BLOCK_TYPE_CLIFF_VERTICAL_RIGHT, mFM_BLOCK_TYPE_CLIFF_CORNER_RIGHT_TOP };
mRF_trace_data_c l_cliff2_next_class = { l_cliff2_next, ARRAY_COUNT(l_cliff2_next) };

u8 l_cliff3_next[] = { mFM_BLOCK_TYPE_CLIFF_VERTICAL_RIGHT, mFM_BLOCK_TYPE_CLIFF_CORNER_RIGHT_TOP };
mRF_trace_data_c l_cliff3_next_class = { l_cliff3_next, ARRAY_COUNT(l_cliff3_next) };

u8 l_cliff4_next[] = { mFM_BLOCK_TYPE_CLIFF_HORIZONTAL, mFM_BLOCK_TYPE_CLIFF_CORNER_RIGHT_BOTTOM, mFM_BLOCK_TYPE_CLIFF_CORNER_LEFT_TOP };
mRF_trace_data_c l_cliff4_next_class = { l_cliff4_next, ARRAY_COUNT(l_cliff4_next) };

u8 l_cliff5_next[] = { mFM_BLOCK_TYPE_CLIFF_VERTICAL_LEFT, mFM_BLOCK_TYPE_CLIFF_CORNER_LEFT_BOTTOM };
mRF_trace_data_c l_cliff5_next_class = { l_cliff5_next, ARRAY_COUNT(l_cliff5_next) };

u8 l_cliff6_next[] = { mFM_BLOCK_TYPE_CLIFF_VERTICAL_LEFT, mFM_BLOCK_TYPE_CLIFF_CORNER_LEFT_BOTTOM };
mRF_trace_data_c l_cliff6_next_class = { l_cliff6_next, ARRAY_COUNT(l_cliff6_next) };

u8 l_cliff7_next[] = { mFM_BLOCK_TYPE_CLIFF_HORIZONTAL, mFM_BLOCK_TYPE_CLIFF_CORNER_RIGHT_BOTTOM, mFM_BLOCK_TYPE_CLIFF_CORNER_LEFT_TOP };
mRF_trace_data_c l_cliff7_next_class = { l_cliff7_next, ARRAY_COUNT(l_cliff7_next) };

mRF_trace_data_c* l_cliff_next_data[] = {
  &l_cliff1_next_class,
  &l_cliff2_next_class,
  &l_cliff3_next_class,
  &l_cliff4_next_class,
  &l_cliff5_next_class,
  &l_cliff6_next_class,
  &l_cliff7_next_class
};

// Found in `m_random_field.c` in AC
u8 l_river_next_direct[mRF_RIVER_NUM] = {
  mRF_DIRECT_SOUTH,
  mRF_DIRECT_EAST,
  mRF_DIRECT_WEST,
  mRF_DIRECT_EAST,
  mRF_DIRECT_SOUTH,
  mRF_DIRECT_WEST,
  mRF_DIRECT_SOUTH
};

u8 l_river1_next[] = { mFM_BLOCK_TYPE_RIVER_SOUTH, mFM_BLOCK_TYPE_RIVER_SOUTH_EAST, mFM_BLOCK_TYPE_RIVER_SOUTH_WEST };
mRF_trace_data_c l_river1_next_class = { l_river1_next, ARRAY_COUNT(l_river1_next) };

u8 l_river2_next[] = { mFM_BLOCK_TYPE_RIVER_EAST, mFM_BLOCK_TYPE_RIVER_EAST_SOUTH };
mRF_trace_data_c l_river2_next_class = { l_river2_next, ARRAY_COUNT(l_river2_next) };

u8 l_river3_next[] = { mFM_BLOCK_TYPE_RIVER_WEST, mFM_BLOCK_TYPE_RIVER_WEST_SOUTH };
mRF_trace_data_c l_river3_next_class = { l_river3_next, ARRAY_COUNT(l_river3_next) };

u8 l_river4_next[] = { mFM_BLOCK_TYPE_RIVER_EAST, mFM_BLOCK_TYPE_RIVER_EAST_SOUTH };
mRF_trace_data_c l_river4_next_class = { l_river4_next, ARRAY_COUNT(l_river4_next) };

u8 l_river5_next[] = { mFM_BLOCK_TYPE_RIVER_SOUTH, mFM_BLOCK_TYPE_RIVER_SOUTH_EAST, mFM_BLOCK_TYPE_RIVER_SOUTH_WEST };
mRF_trace_data_c l_river5_next_class = { l_river5_next, ARRAY_COUNT(l_river5_next) };

u8 l_river6_next[] = { mFM_BLOCK_TYPE_RIVER_WEST, mFM_BLOCK_TYPE_RIVER_WEST_SOUTH };
mRF_trace_data_c l_river6_next_class = { l_river6_next, ARRAY_COUNT(l_river6_next) };

u8 l_river7_next[] = { mFM_BLOCK_TYPE_RIVER_SOUTH, mFM_BLOCK_TYPE_RIVER_SOUTH_EAST, mFM_BLOCK_TYPE_RIVER_SOUTH_WEST };
mRF_trace_data_c l_river7_next_class = { l_river7_next, ARRAY_COUNT(l_river7_next) };

mRF_trace_data_c* l_river_next_data[] = {
  &l_river1_next_class,
  &l_river2_next_class,
  &l_river3_next_class,
  &l_river4_next_class,
  &l_river5_next_class,
  &l_river6_next_class,
  &l_river7_next_class
};

s32 mRF_GetRandom(s32 max) {
    return RANDOM(max);
}

s32 mRF_D2ToD1(s32 blockX, s32 blockZ) {
  return blockZ * BLOCK_X_NUM + blockX;
}

void mRF_Direct2BlockNo(s32* blockX, s32* blockZ, s32 baseBlockX, s32 baseBlockZ, u8 direct) {
  static s32 x_offset[mRF_DIRECT_NUM] = { 0, -1, 0, 1 };
  static s32 z_offset[mRF_DIRECT_NUM] = { -1, 0, 1, 0 };

  blockX[0] = baseBlockX + x_offset[direct];
  blockZ[0] = baseBlockZ + z_offset[direct];
}

s32 mRF_CheckCorrectBlockNo(s32 blockX, s32 blockZ, s32 blockXMin, s32 blockXMax, s32 blockZMin, s32 blockZMax) {
  if (blockX < blockXMin) {
    return FALSE;
  }

  if (blockX > blockXMax) {
    return FALSE;
  }

  if (blockZ < blockZMin) {
    return FALSE;
  }

  if (blockZ > blockZMax) {
    return FALSE;
  }

  return TRUE;
}

typedef struct block_group_s {
  s32 min;
  s32 max;
} mRF_BlockGroup_c;

s32 mRF_CheckBlockGroup(u8 type, s32 group) {
  static mRF_BlockGroup_c blockGroup[9] = {
    { mFM_BLOCK_TYPE_CLIFF_HORIZONTAL, mFM_BLOCK_TYPE_CLIFF_CORNER_LEFT_BOTTOM },
    { mFM_BLOCK_TYPE_RIVER_SOUTH, mFM_BLOCK_TYPE_RIVER_WEST_SOUTH },
    { mFM_BLOCK_TYPE_RIVER_SOUTH_BRIDGE, mFM_BLOCK_TYPE_RIVER_WEST_SOUTH_BRIDGE },
    { mFM_BLOCK_TYPE_SLOPE_HORIZONTAL, mFM_BLOCK_TYPE_SLOPE_CORNER_LEFT_BOTTOM },
    { mFM_BLOCK_TYPE_WATERFALL_SOUTH_CLIFF_HORIZONTAL, mFM_BLOCK_TYPE_WATERFALL_WEST_CLIFF_CORNER_LEFT_BOTTOM },
    { mFM_BLOCK_TYPE_WATERFALL_SOUTH_CLIFF_HORIZONTAL, mFM_BLOCK_TYPE_RIVER_SOUTH_CLIFF_CORNER_LEFT_BOTTOM },
    { mFM_BLOCK_TYPE_RIVER_EAST_CLIFF_HORIZONTAL, mFM_BLOCK_TYPE_RIVER_EAST_CLIFF_CORNER_LEFT_TOP },
    { mFM_BLOCK_TYPE_RIVER_WEST_CLIFF_HORIZONTAL, mFM_BLOCK_TYPE_WATERFALL_WEST_CLIFF_CORNER_LEFT_BOTTOM },
    { 0, 0 }
  };
  mRF_BlockGroup_c block_group;
  
  if (group == mRF_BLOCK_GROUP_CLIFF_ANY) {
    /* Check any acre with a cliff in it, even if it has rivers or slopes */
    if (
      (type >= blockGroup[mRF_BLOCK_GROUP_CLIFF].min && type <= blockGroup[mRF_BLOCK_GROUP_CLIFF].max) ||
      (type >= blockGroup[mRF_BLOCK_GROUP_SLOPE].min && type <= blockGroup[mRF_BLOCK_GROUP_SLOPE].max) ||
      (type >= blockGroup[mRF_BLOCK_GROUP_RIVER_CLIFF_ANY].min && type <= blockGroup[mRF_BLOCK_GROUP_RIVER_CLIFF_ANY].max)
    ) {
      return TRUE;
    }
    return FALSE;
  }
  else {
    block_group = blockGroup[group];

    if (type >= block_group.min && type <= block_group.max) {
      return TRUE;
    }  
  }

  return FALSE;
}


void mRF_CpyBlockData(u8* dst, u8* src) {
  s32 i;

  for (i = 0; i < BLOCK_TOTAL_NUM; i++) {
    *dst++ = *src++;
  }
}

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/other_ovl/m_random_field_ovl/m_random_field_ovl/mRF_TraceCliffBlock.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/other_ovl/m_random_field_ovl/m_random_field_ovl/mRF_SetEndCliffBlock.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/other_ovl/m_random_field_ovl/m_random_field_ovl/mRF_LastCheckCliff.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/other_ovl/m_random_field_ovl/m_random_field_ovl/mRF_DecideBaseCliff.s")


// static in AC, ?? in AF
u8 river1_album_data[7] = {
    mFM_BLOCK_TYPE_WATERFALL_SOUTH_CLIFF_HORIZONTAL,
    mFM_BLOCK_TYPE_WATERFALL_SOUTH_CLIFF_CORNER_RIGHT_BOTTOM,
    mFM_BLOCK_TYPE_RIVER_SOUTH_CLIFF_VERTICAL_RIGHT,
    mFM_BLOCK_TYPE_RIVER_SOUTH_CLIFF_CORNER_RIGHT_TOP,
    mFM_BLOCK_TYPE_WATERFALL_SOUTH_CLIFF_CORNER_LEFT_TOP,
    mFM_BLOCK_TYPE_RIVER_SOUTH_CLIFF_VERTICAL_LEFT,
    mFM_BLOCK_TYPE_RIVER_SOUTH_CLIFF_CORNER_LEFT_BOTTOM
};

u8 river2_album_data[7] = {
    mFM_BLOCK_TYPE_RIVER_EAST_CLIFF_HORIZONTAL,
    mFM_BLOCK_TYPE_WATERFALL_EAST_CLIFF_CORNER_RIGHT_BOTTOM,
    mFM_BLOCK_TYPE_WATERFALL_EAST_CLIFF_VERTICAL_RIGHT,
    mFM_BLOCK_TYPE_RIVER_EAST_CLIFF_CORNER_RIGHT_TOP,
    mFM_BLOCK_TYPE_RIVER_EAST_CLIFF_CORNER_LEFT_TOP,
    mFM_BLOCK_TYPE_NONE,
    mFM_BLOCK_TYPE_NONE
};

u8 river3_album_data[7] = {
    mFM_BLOCK_TYPE_RIVER_WEST_CLIFF_HORIZONTAL,
    mFM_BLOCK_TYPE_NONE,
    mFM_BLOCK_TYPE_NONE,
    mFM_BLOCK_TYPE_RIVER_WEST_CLIFF_CORNER_RIGHT_TOP,
    mFM_BLOCK_TYPE_RIVER_WEST_CLIFF_CORNER_LEFT_TOP,
    mFM_BLOCK_TYPE_WATERFALL_WEST_CLIFF_VERTICAL_LEFT,
    mFM_BLOCK_TYPE_WATERFALL_WEST_CLIFF_CORNER_LEFT_BOTTOM
};

u8 river_no_album_data[7] = {
    mFM_BLOCK_TYPE_NONE,
    mFM_BLOCK_TYPE_NONE,
    mFM_BLOCK_TYPE_NONE,
    mFM_BLOCK_TYPE_NONE,
    mFM_BLOCK_TYPE_NONE,
    mFM_BLOCK_TYPE_NONE,
    mFM_BLOCK_TYPE_NONE
};

u8* river_cliff_album_data[7] = {
    river1_album_data,    /* south river */
    river2_album_data,    /* east river */
    river3_album_data,    /* west river */
    river_no_album_data,  /* south -> east river */
    river_no_album_data,  /* east -> south river */
    river_no_album_data,  /* south -> west river */
    river_no_album_data   /* west -> south river */
};

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/other_ovl/m_random_field_ovl/m_random_field_ovl/mRF_RiverAlbuminCliff.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/other_ovl/m_random_field_ovl/m_random_field_ovl/mRF_DecideRiverAlbuminCliff.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/other_ovl/m_random_field_ovl/m_random_field_ovl/mRF_GetCenterCrossZoneRiverCount.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/other_ovl/m_random_field_ovl/m_random_field_ovl/mRF_TraceRiverPart2.s")

// static in AC, ?? in AF
s32 startX_table[4] = { 1, 2, 4, 5 };

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/other_ovl/m_random_field_ovl/m_random_field_ovl/mRF_TraceRiverPart1.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/other_ovl/m_random_field_ovl/m_random_field_ovl/mRF_InitFlag.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/other_ovl/m_random_field_ovl/m_random_field_ovl/mRF_LastCheckRiver.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/other_ovl/m_random_field_ovl/m_random_field_ovl/mRF_DecideBaseRiver.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/other_ovl/m_random_field_ovl/m_random_field_ovl/mRF_SetRandomBlockData.s")

// static in AC, ?? in AF
u8 cross_data[7] = {
    mFM_BLOCK_TYPE_WATERFALL_SOUTH_CLIFF_HORIZONTAL,
    mFM_BLOCK_TYPE_WATERFALL_SOUTH_CLIFF_CORNER_RIGHT_BOTTOM,
    mFM_BLOCK_TYPE_WATERFALL_SOUTH_CLIFF_CORNER_LEFT_TOP,
    mFM_BLOCK_TYPE_WATERFALL_EAST_CLIFF_CORNER_RIGHT_BOTTOM,
    mFM_BLOCK_TYPE_WATERFALL_EAST_CLIFF_VERTICAL_RIGHT,
    mFM_BLOCK_TYPE_WATERFALL_WEST_CLIFF_VERTICAL_LEFT,
    mFM_BLOCK_TYPE_WATERFALL_WEST_CLIFF_CORNER_LEFT_BOTTOM
};

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/other_ovl/m_random_field_ovl/m_random_field_ovl/mRF_GetRiverCrossCliffInfo.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/other_ovl/m_random_field_ovl/m_random_field_ovl/func_80923A14_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/other_ovl/m_random_field_ovl/m_random_field_ovl/func_80923BC4_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/other_ovl/m_random_field_ovl/m_random_field_ovl/mRF_GetBlockBase.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/other_ovl/m_random_field_ovl/m_random_field_ovl/mRF_InitBlockBase.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/other_ovl/m_random_field_ovl/m_random_field_ovl/mRF_SetShortData.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/other_ovl/m_random_field_ovl/m_random_field_ovl/mRF_SearchAlreadyUse.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/other_ovl/m_random_field_ovl/m_random_field_ovl/mRF_TypeCombCount.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/other_ovl/m_random_field_ovl/m_random_field_ovl/mRF_IndexInType2BlockNo.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/other_ovl/m_random_field_ovl/m_random_field_ovl/mRF_SelectBlock.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/other_ovl/m_random_field_ovl/m_random_field_ovl/mRF_CopyBlockBaseHeightData.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/other_ovl/m_random_field_ovl/m_random_field_ovl/mRF_MakeBaseLandformStep2.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/other_ovl/m_random_field_ovl/m_random_field_ovl/func_809244A0_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/other_ovl/m_random_field_ovl/m_random_field_ovl/mRF_MakeBaseHeightTable.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/other_ovl/m_random_field_ovl/m_random_field_ovl/func_80924528_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/other_ovl/m_random_field_ovl/m_random_field_ovl/func_809246D4_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/other_ovl/m_random_field_ovl/m_random_field_ovl/func_809247E0_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/other_ovl/m_random_field_ovl/m_random_field_ovl/func_80924958_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/other_ovl/m_random_field_ovl/m_random_field_ovl/func_809249D4_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/other_ovl/m_random_field_ovl/m_random_field_ovl/func_80924A84_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/other_ovl/m_random_field_ovl/m_random_field_ovl/func_80924B98_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/other_ovl/m_random_field_ovl/m_random_field_ovl/func_80924C9C_jp.s")

//static in AC, ?? in AF
FieldMakeCombination init_cmb = { 1, mRF_FIELD_STEP1 };

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/other_ovl/m_random_field_ovl/m_random_field_ovl/mRF_InitCombTable.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/other_ovl/m_random_field_ovl/m_random_field_ovl/mRF_SetSeaBlockWithBridgeRiver.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/other_ovl/m_random_field_ovl/m_random_field_ovl/func_80924E6C_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/other_ovl/m_random_field_ovl/m_random_field_ovl/mRF_MakeRandomField_ovl.s")
