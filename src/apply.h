/*
 * The MIT License (MIT)
 *
 * Copyright © 2015 Franklin "Snaipe" Mathieu <http://snai.pe/>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef CSPTR_CPP_APPLY_H_
# define CSPTR_CPP_APPLY_H_

# include "vararg.h"

# define APPLY_0(Macro, ...)
# define APPLY_1(Macro, Type, Head, ...) Macro(Type, Head) APPLY_0(Macro, Type, __VA_ARGS__)
# define APPLY_2(Macro, Type, Head, ...) Macro(Type, Head) APPLY_1(Macro, Type, __VA_ARGS__)
# define APPLY_3(Macro, Type, Head, ...) Macro(Type, Head) APPLY_2(Macro, Type, __VA_ARGS__)
# define APPLY_4(Macro, Type, Head, ...) Macro(Type, Head) APPLY_3(Macro, Type, __VA_ARGS__)
# define APPLY_5(Macro, Type, Head, ...) Macro(Type, Head) APPLY_4(Macro, Type, __VA_ARGS__)
# define APPLY_6(Macro, Type, Head, ...) Macro(Type, Head) APPLY_5(Macro, Type, __VA_ARGS__)
# define APPLY_7(Macro, Type, Head, ...) Macro(Type, Head) APPLY_6(Macro, Type, __VA_ARGS__)
# define APPLY_8(Macro, Type, Head, ...) Macro(Type, Head) APPLY_7(Macro, Type, __VA_ARGS__)
# define APPLY_9(Macro, Type, Head, ...) Macro(Type, Head) APPLY_8(Macro, Type, __VA_ARGS__)
# define APPLY_10(Macro, Type, Head, ...) Macro(Type, Head) APPLY_9(Macro, Type, __VA_ARGS__)
# define APPLY_11(Macro, Type, Head, ...) Macro(Type, Head) APPLY_10(Macro, Type, __VA_ARGS__)
# define APPLY_12(Macro, Type, Head, ...) Macro(Type, Head) APPLY_11(Macro, Type, __VA_ARGS__)
# define APPLY_13(Macro, Type, Head, ...) Macro(Type, Head) APPLY_12(Macro, Type, __VA_ARGS__)
# define APPLY_14(Macro, Type, Head, ...) Macro(Type, Head) APPLY_13(Macro, Type, __VA_ARGS__)
# define APPLY_15(Macro, Type, Head, ...) Macro(Type, Head) APPLY_14(Macro, Type, __VA_ARGS__)
# define APPLY_16(Macro, Type, Head, ...) Macro(Type, Head) APPLY_15(Macro, Type, __VA_ARGS__)
# define APPLY_17(Macro, Type, Head, ...) Macro(Type, Head) APPLY_16(Macro, Type, __VA_ARGS__)
# define APPLY_18(Macro, Type, Head, ...) Macro(Type, Head) APPLY_17(Macro, Type, __VA_ARGS__)
# define APPLY_19(Macro, Type, Head, ...) Macro(Type, Head) APPLY_18(Macro, Type, __VA_ARGS__)
# define APPLY_20(Macro, Type, Head, ...) Macro(Type, Head) APPLY_19(Macro, Type, __VA_ARGS__)
# define APPLY_21(Macro, Type, Head, ...) Macro(Type, Head) APPLY_20(Macro, Type, __VA_ARGS__)
# define APPLY_22(Macro, Type, Head, ...) Macro(Type, Head) APPLY_21(Macro, Type, __VA_ARGS__)
# define APPLY_23(Macro, Type, Head, ...) Macro(Type, Head) APPLY_22(Macro, Type, __VA_ARGS__)
# define APPLY_24(Macro, Type, Head, ...) Macro(Type, Head) APPLY_23(Macro, Type, __VA_ARGS__)
# define APPLY_25(Macro, Type, Head, ...) Macro(Type, Head) APPLY_24(Macro, Type, __VA_ARGS__)
# define APPLY_26(Macro, Type, Head, ...) Macro(Type, Head) APPLY_25(Macro, Type, __VA_ARGS__)
# define APPLY_27(Macro, Type, Head, ...) Macro(Type, Head) APPLY_26(Macro, Type, __VA_ARGS__)
# define APPLY_28(Macro, Type, Head, ...) Macro(Type, Head) APPLY_27(Macro, Type, __VA_ARGS__)
# define APPLY_29(Macro, Type, Head, ...) Macro(Type, Head) APPLY_28(Macro, Type, __VA_ARGS__)
# define APPLY_30(Macro, Type, Head, ...) Macro(Type, Head) APPLY_29(Macro, Type, __VA_ARGS__)
# define APPLY_31(Macro, Type, Head, ...) Macro(Type, Head) APPLY_30(Macro, Type, __VA_ARGS__)
# define APPLY_32(Macro, Type, Head, ...) Macro(Type, Head) APPLY_31(Macro, Type, __VA_ARGS__)
# define APPLY_33(Macro, Type, Head, ...) Macro(Type, Head) APPLY_32(Macro, Type, __VA_ARGS__)
# define APPLY_34(Macro, Type, Head, ...) Macro(Type, Head) APPLY_33(Macro, Type, __VA_ARGS__)
# define APPLY_35(Macro, Type, Head, ...) Macro(Type, Head) APPLY_34(Macro, Type, __VA_ARGS__)
# define APPLY_36(Macro, Type, Head, ...) Macro(Type, Head) APPLY_35(Macro, Type, __VA_ARGS__)
# define APPLY_37(Macro, Type, Head, ...) Macro(Type, Head) APPLY_36(Macro, Type, __VA_ARGS__)
# define APPLY_38(Macro, Type, Head, ...) Macro(Type, Head) APPLY_37(Macro, Type, __VA_ARGS__)
# define APPLY_39(Macro, Type, Head, ...) Macro(Type, Head) APPLY_38(Macro, Type, __VA_ARGS__)
# define APPLY_40(Macro, Type, Head, ...) Macro(Type, Head) APPLY_39(Macro, Type, __VA_ARGS__)
# define APPLY_41(Macro, Type, Head, ...) Macro(Type, Head) APPLY_40(Macro, Type, __VA_ARGS__)
# define APPLY_42(Macro, Type, Head, ...) Macro(Type, Head) APPLY_41(Macro, Type, __VA_ARGS__)
# define APPLY_43(Macro, Type, Head, ...) Macro(Type, Head) APPLY_42(Macro, Type, __VA_ARGS__)
# define APPLY_44(Macro, Type, Head, ...) Macro(Type, Head) APPLY_43(Macro, Type, __VA_ARGS__)
# define APPLY_45(Macro, Type, Head, ...) Macro(Type, Head) APPLY_44(Macro, Type, __VA_ARGS__)
# define APPLY_46(Macro, Type, Head, ...) Macro(Type, Head) APPLY_45(Macro, Type, __VA_ARGS__)
# define APPLY_47(Macro, Type, Head, ...) Macro(Type, Head) APPLY_46(Macro, Type, __VA_ARGS__)
# define APPLY_48(Macro, Type, Head, ...) Macro(Type, Head) APPLY_47(Macro, Type, __VA_ARGS__)
# define APPLY_49(Macro, Type, Head, ...) Macro(Type, Head) APPLY_48(Macro, Type, __VA_ARGS__)
# define APPLY_50(Macro, Type, Head, ...) Macro(Type, Head) APPLY_49(Macro, Type, __VA_ARGS__)
# define APPLY_51(Macro, Type, Head, ...) Macro(Type, Head) APPLY_50(Macro, Type, __VA_ARGS__)
# define APPLY_52(Macro, Type, Head, ...) Macro(Type, Head) APPLY_51(Macro, Type, __VA_ARGS__)
# define APPLY_53(Macro, Type, Head, ...) Macro(Type, Head) APPLY_52(Macro, Type, __VA_ARGS__)
# define APPLY_54(Macro, Type, Head, ...) Macro(Type, Head) APPLY_53(Macro, Type, __VA_ARGS__)
# define APPLY_55(Macro, Type, Head, ...) Macro(Type, Head) APPLY_54(Macro, Type, __VA_ARGS__)
# define APPLY_56(Macro, Type, Head, ...) Macro(Type, Head) APPLY_55(Macro, Type, __VA_ARGS__)
# define APPLY_57(Macro, Type, Head, ...) Macro(Type, Head) APPLY_56(Macro, Type, __VA_ARGS__)
# define APPLY_58(Macro, Type, Head, ...) Macro(Type, Head) APPLY_57(Macro, Type, __VA_ARGS__)
# define APPLY_59(Macro, Type, Head, ...) Macro(Type, Head) APPLY_58(Macro, Type, __VA_ARGS__)
# define APPLY_60(Macro, Type, Head, ...) Macro(Type, Head) APPLY_59(Macro, Type, __VA_ARGS__)
# define APPLY_61(Macro, Type, Head, ...) Macro(Type, Head) APPLY_60(Macro, Type, __VA_ARGS__)
# define APPLY_62(Macro, Type, Head, ...) Macro(Type, Head) APPLY_61(Macro, Type, __VA_ARGS__)
# define APPLY_63(Macro, Type, Head, ...) Macro(Type, Head) APPLY_62(Macro, Type, __VA_ARGS__)

# define APPLY__(Macro, Type, n, ...) APPLY_##n(Macro, Type, __VA_ARGS__)
# define APPLY_(Macro, Type, n, ...) APPLY__(Macro, Type, n, __VA_ARGS__)
# define APPLY(Macro, Type, ...) APPLY_(Macro, Type, ARG_LENGTH(__VA_ARGS__), __VA_ARGS__)

#endif /* !CSPTR_CPP_APPLY_H_ */
