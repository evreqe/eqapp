#pragma once

#define EQ_FUNCTION_CHECKS_OFF() \
    __pragma(optimize("ytg", on)) __pragma(runtime_checks("", off)) __pragma(check_stack(off)) __pragma(strict_gs_check(push,off))
#define EQ_FUNCTION_CHECKS_ON() \
    __pragma(optimize("", on)) __pragma(runtime_checks("",restore)) __pragma(check_stack())    __pragma(strict_gs_check(pop))

#define EQ_FUNCTION_AT_ADDRESS(rettype, func, variable)                                            \
    EQ_FUNCTION_CHECKS_OFF()                                                                       \
    __declspec(noinline) rettype func {                                                            \
        using TargetFunction = rettype(*)();                                                       \
        return ((TargetFunction)variable)();                                                       \
    }                                                                                              \
    EQ_FUNCTION_CHECKS_ON()

#define EQ_FUNCTION_AT_VIRTUAL_ADDRESS(rettype, func, offset)                                      \
    EQ_FUNCTION_CHECKS_OFF()                                                                       \
    __declspec(noinline) rettype func {                                                            \
        using TargetFunction = rettype(*)();                                                       \
        return ((TargetFunction)(*(reinterpret_cast<uintptr_t**>(this)[0] + (offset/8))))();       \
    }                                                                                              \
    EQ_FUNCTION_CHECKS_ON()

#define EQ_FORWARD_FUNCTION_TO_VTABLE(rettype, function, Class, member)                            \
    EQ_FUNCTION_CHECKS_OFF()                                                                       \
    __declspec(noinline) rettype Class::function {                                                 \
        using TargetFunction = rettype(*)();                                                       \
        return ((TargetFunction)(Class::sm_vftable->member))();                                    \
    }                                                                                              \
    EQ_FUNCTION_CHECKS_ON()

#define EQ_FUNCTION_AT_VIRTUAL_TABLE_ADDRESS(rettype, function, address, offset)                   \
    EQ_FUNCTION_CHECKS_OFF()                                                                       \
    __declspec(noinline) rettype function {                                                        \
        using TargetFunction = rettype(*)();                                                       \
        return (*(TargetFunction*)((address + offset * sizeof(uintptr_t))))();                     \
    }                                                                                              \
    EQ_FUNCTION_CHECKS_ON()
