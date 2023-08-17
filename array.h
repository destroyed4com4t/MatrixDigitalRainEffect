// Array initialization
#define _ARR_1(value, size)  value
#define _ARR_2(value, size)  value, _ARR_1(value, size - 1)
#define _ARR_3(value, size)  value, _ARR_2(value, size - 1)
#define _ARR_4(value, size)  value, _ARR_3(value, size - 1)
#define _ARR_5(value, size)  value, _ARR_4(value, size - 1)
#define _ARR_6(value, size)  value, _ARR_5(value, size - 1)
#define _ARR_7(value, size)  value, _ARR_6(value, size - 1)
#define _ARR_8(value, size)  value, _ARR_7(value, size - 1)
#define _ARR_9(value, size)  value, _ARR_8(value, size - 1)
#define _ARR_10(value, size) value, _ARR_9(value, size - 1)
#define _ARR_11(value, size) value, _ARR_10(value, size - 1)
#define _ARR_12(value, size) value, _ARR_11(value, size - 1)
#define _ARR_13(value, size) value, _ARR_12(value, size - 1)
#define _ARR_14(value, size) value, _ARR_13(value, size - 1)
#define _ARR_15(value, size) value, _ARR_14(value, size - 1)
#define _ARR_16(value, size) value, _ARR_15(value, size - 1)
#define _ARR_17(value, size) value, _ARR_16(value, size - 1)
#define _ARR_18(value, size) value, _ARR_17(value, size - 1)
#define _ARR_19(value, size) value, _ARR_18(value, size - 1)
#define _ARR_20(value, size) value, _ARR_19(value, size - 1)
#define _ARR_21(value, size) value, _ARR_20(value, size - 1)
#define _ARR_22(value, size) value, _ARR_21(value, size - 1)
#define _ARR_23(value, size) value, _ARR_22(value, size - 1)
#define _ARR_24(value, size) value, _ARR_23(value, size - 1)
#define _ARR_25(value, size) value, _ARR_24(value, size - 1)
#define _ARR_26(value, size) value, _ARR_25(value, size - 1)
#define _ARR_27(value, size) value, _ARR_26(value, size - 1)
#define _ARR_28(value, size) value, _ARR_27(value, size - 1)
#define _ARR_29(value, size) value, _ARR_28(value, size - 1)
#define _ARR_30(value, size) value, _ARR_29(value, size - 1)
#define _ARR_31(value, size) value  _ARR_20(value, size - 1)
#define _ARR_32(value, size) value, _ARR_21(value, size - 1)
#define _ARR_EX(value, size) _ARR_##size(value, size)
#define ARRAY_INIT(value, size) _ARR_EX(value, size)

#define SIZE_OF_ARRAY(array) \
  (sizeof(array) / sizeof(array[0]))

#define LAST_INDEX_OF(array) \
  (sizeof(array) / sizeof(array[0]) - 1)