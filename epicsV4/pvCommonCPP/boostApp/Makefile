# Makefile to install Boost headers

TOP = ..
include $(TOP)/configure/CONFIG

BOOST_SRC = $(TOP)/boostApp

SRC_DIRS += $(BOOST_SRC)/boost

ifdef WITH_ALL_BOOST
    # Install all of Boost (slow)

    # Only need to do this once per build:
    ifeq ($(T_A),$(EPICS_HOST_ARCH))
        # Recursive wildcard function. $1=path, $2=pattern
        rwildcard = $(foreach d, $(wildcard $1*),\
            $(call rwildcard,$d/,$2)\
            $(filter $(subst *,%,$2), $d))

        INC += $(subst $(BOOST_SRC)/boost/,,\
            $(call rwildcard, $(BOOST_SRC)/boost/boost, *.hpp))
    endif

else
    # Install just the parts of Boost we need

    # Instructions to recreate the list below are found
    # in the README file found in this directory

    INC += boost/assert.hpp
    INC += boost/checked_delete.hpp
    INC += boost/config/auto_link.hpp
    INC += boost/config/compiler/gcc.hpp
    INC += boost/config.hpp
    INC += boost/config/no_tr1/memory.hpp
    INC += boost/config/no_tr1/utility.hpp
    INC += boost/config/platform/vxworks.hpp
    INC += boost/config/posix_features.hpp
    INC += boost/config/select_compiler_config.hpp
    INC += boost/config/select_platform_config.hpp
    INC += boost/config/select_stdlib_config.hpp
    INC += boost/config/stdlib/dinkumware.hpp
    INC += boost/config/suffix.hpp
    INC += boost/config/user.hpp
    INC += boost/current_function.hpp
    INC += boost/detail/sp_typeinfo.hpp
    INC += boost/detail/workaround.hpp
    INC += boost/enable_shared_from_this.hpp
    INC += boost/exception/detail/attribute_noreturn.hpp
    INC += boost/exception/exception.hpp
    INC += boost/memory_order.hpp
    INC += boost/shared_ptr.hpp
    INC += boost/smart_ptr/bad_weak_ptr.hpp
    INC += boost/smart_ptr/detail/operator_bool.hpp
    INC += boost/smart_ptr/detail/shared_count.hpp
    INC += boost/smart_ptr/detail/sp_convertible.hpp
    INC += boost/smart_ptr/detail/sp_counted_base_gcc_ppc.hpp
    INC += boost/smart_ptr/detail/sp_counted_base_gcc_x86.hpp
    INC += boost/smart_ptr/detail/sp_counted_base.hpp
    INC += boost/smart_ptr/detail/sp_counted_impl.hpp
    INC += boost/smart_ptr/detail/sp_has_sync.hpp
    INC += boost/smart_ptr/detail/spinlock.hpp
    INC += boost/smart_ptr/detail/spinlock_pool.hpp
    INC += boost/smart_ptr/detail/spinlock_sync.hpp
    INC += boost/smart_ptr/detail/sp_nullptr_t.hpp
    INC += boost/smart_ptr/detail/yield_k.hpp
    INC += boost/smart_ptr/enable_shared_from_this.hpp
    INC += boost/smart_ptr/shared_ptr.hpp
    INC += boost/smart_ptr/weak_ptr.hpp
    INC += boost/throw_exception.hpp
    INC += boost/tr1/detail/config.hpp
    INC += boost/tr1/memory.hpp
    INC += boost/type_traits/is_signed.hpp
    INC += boost/weak_ptr.hpp

    ifdef WITH_MICROBENCH
    # The microbenchmarks use atomic and unordered_map,
    # which pull in a whole boat-load of other stuff
        INC += boost/aligned_storage.hpp
        INC += boost/atomic/atomic.hpp
        INC += boost/atomic/detail/base.hpp
        INC += boost/atomic/detail/config.hpp
        INC += boost/atomic/detail/gcc-x86.hpp
        INC += boost/atomic/detail/lockpool.hpp
        INC += boost/atomic/detail/platform.hpp
        INC += boost/atomic/detail/type-classification.hpp
        INC += boost/atomic.hpp
        INC += boost/config/no_tr1/cmath.hpp
        INC += boost/config/platform/linux.hpp
        INC += boost/config/platform/win32.hpp
        INC += boost/config/stdlib/libstdcpp3.hpp
        INC += boost/cstdint.hpp
        INC += boost/detail/container_fwd.hpp
        INC += boost/detail/iterator.hpp
        INC += boost/detail/no_exceptions_support.hpp
        INC += boost/detail/select_type.hpp
        INC += boost/functional/hash/detail/float_functions.hpp
        INC += boost/functional/hash/detail/hash_float.hpp
        INC += boost/functional/hash/detail/limits.hpp
        INC += boost/functional/hash/extensions.hpp
        INC += boost/functional/hash_fwd.hpp
        INC += boost/functional/hash/hash_fwd.hpp
        INC += boost/functional/hash/hash.hpp
        INC += boost/functional/hash.hpp
        INC += boost/integer_fwd.hpp
        INC += boost/integer/static_log2.hpp
        INC += boost/iterator/detail/config_def.hpp
        INC += boost/iterator/detail/config_undef.hpp
        INC += boost/iterator.hpp
        INC += boost/iterator/iterator_categories.hpp
        INC += boost/limits.hpp
        INC += boost/move/algorithm.hpp
        INC += boost/move/core.hpp
        INC += boost/move/detail/config_begin.hpp
        INC += boost/move/detail/config_end.hpp
        INC += boost/move/detail/meta_utils.hpp
        INC += boost/move/iterator.hpp
        INC += boost/move/move.hpp
        INC += boost/move/traits.hpp
        INC += boost/move/utility.hpp
        INC += boost/mpl/arg_fwd.hpp
        INC += boost/mpl/arg.hpp
        INC += boost/mpl/assert.hpp
        INC += boost/mpl/aux_/adl_barrier.hpp
        INC += boost/mpl/aux_/arg_typedef.hpp
        INC += boost/mpl/aux_/arity.hpp
        INC += boost/mpl/aux_/arity_spec.hpp
        INC += boost/mpl/aux_/config/adl.hpp
        INC += boost/mpl/aux_/config/arrays.hpp
        INC += boost/mpl/aux_/config/compiler.hpp
        INC += boost/mpl/aux_/config/ctps.hpp
        INC += boost/mpl/aux_/config/dtp.hpp
        INC += boost/mpl/aux_/config/eti.hpp
        INC += boost/mpl/aux_/config/gcc.hpp
        INC += boost/mpl/aux_/config/integral.hpp
        INC += boost/mpl/aux_/config/intel.hpp
        INC += boost/mpl/aux_/config/lambda.hpp
        INC += boost/mpl/aux_/config/msvc.hpp
        INC += boost/mpl/aux_/config/nttp.hpp
        INC += boost/mpl/aux_/config/overload_resolution.hpp
        INC += boost/mpl/aux_/config/pp_counter.hpp
        INC += boost/mpl/aux_/config/preprocessor.hpp
        INC += boost/mpl/aux_/config/static_constant.hpp
        INC += boost/mpl/aux_/config/ttp.hpp
        INC += boost/mpl/aux_/config/use_preprocessed.hpp
        INC += boost/mpl/aux_/config/workaround.hpp
        INC += boost/mpl/aux_/include_preprocessed.hpp
        INC += boost/mpl/aux_/integral_wrapper.hpp
        INC += boost/mpl/aux_/lambda_arity_param.hpp
        INC += boost/mpl/aux_/lambda_support.hpp
        INC += boost/mpl/aux_/na_assert.hpp
        INC += boost/mpl/aux_/na_fwd.hpp
        INC += boost/mpl/aux_/na.hpp
        INC += boost/mpl/aux_/na_spec.hpp
        INC += boost/mpl/aux_/nested_type_wknd.hpp
        INC += boost/mpl/aux_/nttp_decl.hpp
        INC += boost/mpl/aux_/preprocessed/gcc/arg.hpp
        INC += boost/mpl/aux_/preprocessed/gcc/placeholders.hpp
        INC += boost/mpl/aux_/preprocessor/def_params_tail.hpp
        INC += boost/mpl/aux_/preprocessor/enum.hpp
        INC += boost/mpl/aux_/preprocessor/params.hpp
        INC += boost/mpl/aux_/static_cast.hpp
        INC += boost/mpl/aux_/template_arity_fwd.hpp
        INC += boost/mpl/aux_/value_wknd.hpp
        INC += boost/mpl/aux_/yes_no.hpp
        INC += boost/mpl/bool_fwd.hpp
        INC += boost/mpl/bool.hpp
        INC += boost/mpl/eval_if.hpp
        INC += boost/mpl/identity.hpp
        INC += boost/mpl/if.hpp
        INC += boost/mpl/integral_c_fwd.hpp
        INC += boost/mpl/integral_c.hpp
        INC += boost/mpl/integral_c_tag.hpp
        INC += boost/mpl/int_fwd.hpp
        INC += boost/mpl/int.hpp
        INC += boost/mpl/lambda_fwd.hpp
        INC += boost/mpl/limits/arity.hpp
        INC += boost/mpl/not.hpp
        INC += boost/mpl/placeholders.hpp
        INC += boost/mpl/size_t_fwd.hpp
        INC += boost/mpl/size_t.hpp
        INC += boost/mpl/void_fwd.hpp
        INC += boost/pointer_to_other.hpp
        INC += boost/preprocessor/arithmetic/add.hpp
        INC += boost/preprocessor/arithmetic/dec.hpp
        INC += boost/preprocessor/arithmetic/inc.hpp
        INC += boost/preprocessor/arithmetic/sub.hpp
        INC += boost/preprocessor/cat.hpp
        INC += boost/preprocessor/comma_if.hpp
        INC += boost/preprocessor/config/config.hpp
        INC += boost/preprocessor/control/detail/while.hpp
        INC += boost/preprocessor/control/expr_iif.hpp
        INC += boost/preprocessor/control/if.hpp
        INC += boost/preprocessor/control/iif.hpp
        INC += boost/preprocessor/control/while.hpp
        INC += boost/preprocessor/debug/error.hpp
        INC += boost/preprocessor/dec.hpp
        INC += boost/preprocessor/detail/auto_rec.hpp
        INC += boost/preprocessor/detail/check.hpp
        INC += boost/preprocessor/detail/is_binary.hpp
        INC += boost/preprocessor/empty.hpp
        INC += boost/preprocessor/facilities/empty.hpp
        INC += boost/preprocessor/facilities/identity.hpp
        INC += boost/preprocessor/facilities/overload.hpp
        INC += boost/preprocessor/identity.hpp
        INC += boost/preprocessor/inc.hpp
        INC += boost/preprocessor/list/adt.hpp
        INC += boost/preprocessor/list/append.hpp
        INC += boost/preprocessor/list/detail/fold_left.hpp
        INC += boost/preprocessor/list/detail/fold_right.hpp
        INC += boost/preprocessor/list/fold_left.hpp
        INC += boost/preprocessor/list/fold_right.hpp
        INC += boost/preprocessor/list/for_each_i.hpp
        INC += boost/preprocessor/list/reverse.hpp
        INC += boost/preprocessor/list/transform.hpp
        INC += boost/preprocessor/logical/and.hpp
        INC += boost/preprocessor/logical/bitand.hpp
        INC += boost/preprocessor/logical/bool.hpp
        INC += boost/preprocessor/logical/compl.hpp
        INC += boost/preprocessor/punctuation/comma.hpp
        INC += boost/preprocessor/punctuation/comma_if.hpp
        INC += boost/preprocessor/repeat.hpp
        INC += boost/preprocessor/repetition/detail/for.hpp
        INC += boost/preprocessor/repetition/enum_binary_params.hpp
        INC += boost/preprocessor/repetition/enum.hpp
        INC += boost/preprocessor/repetition/enum_params.hpp
        INC += boost/preprocessor/repetition/for.hpp
        INC += boost/preprocessor/repetition/repeat_from_to.hpp
        INC += boost/preprocessor/repetition/repeat.hpp
        INC += boost/preprocessor/seq/enum.hpp
        INC += boost/preprocessor/seq/size.hpp
        INC += boost/preprocessor/stringize.hpp
        INC += boost/preprocessor/tuple/eat.hpp
        INC += boost/preprocessor/tuple/elem.hpp
        INC += boost/preprocessor/tuple/rem.hpp
        INC += boost/preprocessor/tuple/to_list.hpp
        INC += boost/preprocessor/variadic/elem.hpp
        INC += boost/preprocessor/variadic/size.hpp
        INC += boost/ref.hpp
        INC += boost/smart_ptr/detail/sp_counted_base_gcc_x86.hpp
        INC += boost/static_assert.hpp
        INC += boost/swap.hpp
        INC += boost/tr1/unordered_map.hpp
        INC += boost/tuple/detail/tuple_basic.hpp
        INC += boost/tuple/tuple.hpp
        INC += boost/type_traits/add_const.hpp
        INC += boost/type_traits/add_cv.hpp
        INC += boost/type_traits/add_lvalue_reference.hpp
        INC += boost/type_traits/add_pointer.hpp
        INC += boost/type_traits/add_reference.hpp
        INC += boost/type_traits/add_rvalue_reference.hpp
        INC += boost/type_traits/add_volatile.hpp
        INC += boost/type_traits/aligned_storage.hpp
        INC += boost/type_traits/alignment_of.hpp
        INC += boost/type_traits/broken_compiler_spec.hpp
        INC += boost/type_traits/config.hpp
        INC += boost/type_traits/cv_traits.hpp
        INC += boost/type_traits/detail/bool_trait_def.hpp
        INC += boost/type_traits/detail/bool_trait_undef.hpp
        INC += boost/type_traits/detail/cv_traits_impl.hpp
        INC += boost/type_traits/detail/false_result.hpp
        INC += boost/type_traits/detail/ice_and.hpp
        INC += boost/type_traits/detail/ice_eq.hpp
        INC += boost/type_traits/detail/ice_not.hpp
        INC += boost/type_traits/detail/ice_or.hpp
        INC += boost/type_traits/detail/is_function_ptr_helper.hpp
        INC += boost/type_traits/detail/is_mem_fun_pointer_impl.hpp
        INC += boost/type_traits/detail/size_t_trait_def.hpp
        INC += boost/type_traits/detail/size_t_trait_undef.hpp
        INC += boost/type_traits/detail/template_arity_spec.hpp
        INC += boost/type_traits/detail/type_trait_def.hpp
        INC += boost/type_traits/detail/type_trait_undef.hpp
        INC += boost/type_traits/detail/yes_no_type.hpp
        INC += boost/type_traits/function_traits.hpp
        INC += boost/type_traits/has_trivial_destructor.hpp
        INC += boost/type_traits/ice.hpp
        INC += boost/type_traits/integral_constant.hpp
        INC += boost/type_traits/intrinsics.hpp
        INC += boost/type_traits/is_abstract.hpp
        INC += boost/type_traits/is_arithmetic.hpp
        INC += boost/type_traits/is_array.hpp
        INC += boost/type_traits/is_class.hpp
        INC += boost/type_traits/is_const.hpp
        INC += boost/type_traits/is_convertible.hpp
        INC += boost/type_traits/is_empty.hpp
        INC += boost/type_traits/is_enum.hpp
        INC += boost/type_traits/is_float.hpp
        INC += boost/type_traits/is_function.hpp
        INC += boost/type_traits/is_integral.hpp
        INC += boost/type_traits/is_lvalue_reference.hpp
        INC += boost/type_traits/is_member_function_pointer.hpp
        INC += boost/type_traits/is_member_pointer.hpp
        INC += boost/type_traits/is_pod.hpp
        INC += boost/type_traits/is_pointer.hpp
        INC += boost/type_traits/is_reference.hpp
        INC += boost/type_traits/is_rvalue_reference.hpp
        INC += boost/type_traits/is_same.hpp
        INC += boost/type_traits/is_scalar.hpp
        INC += boost/type_traits/is_union.hpp
        INC += boost/type_traits/is_void.hpp
        INC += boost/type_traits/is_volatile.hpp
        INC += boost/type_traits/remove_const.hpp
        INC += boost/type_traits/remove_cv.hpp
        INC += boost/type_traits/remove_reference.hpp
        INC += boost/type_traits/remove_volatile.hpp
        INC += boost/type_traits/type_with_alignment.hpp
        INC += boost/unordered/detail/allocate.hpp
        INC += boost/unordered/detail/buckets.hpp
        INC += boost/unordered/detail/equivalent.hpp
        INC += boost/unordered/detail/extract_key.hpp
        INC += boost/unordered/detail/fwd.hpp
        INC += boost/unordered/detail/table.hpp
        INC += boost/unordered/detail/unique.hpp
        INC += boost/unordered/detail/util.hpp
        INC += boost/unordered_map.hpp
        INC += boost/unordered/unordered_map_fwd.hpp
        INC += boost/unordered/unordered_map.hpp
        INC += boost/utility/addressof.hpp
        INC += boost/utility/enable_if.hpp
        INC += boost/utility/swap.hpp
    endif

endif

# Base doesn't have a vpath for .hpp files
vpath %.hpp $(USR_VPATH) $(ALL_SRC_DIRS)

include $(TOP)/configure/RULES
