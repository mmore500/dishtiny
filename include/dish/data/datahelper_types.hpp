#pragma once
#ifndef DISH_DATAHELPER_TYPES_HPP_INCLUDE
#define DISH_DATAHELPER_TYPES_HPP_INCLUDE

#include <array>
#include <cassert>

#include "H5Cpp.h"

#include "../config/Config.hpp"

namespace dish {

// TODO: implement conversion operator inside emp::BitSet
struct TagBytes {
    std::array<char, Config::tag_t::GetNumBytes()> arr;

    TagBytes(const Config::tag_t& tag) {
        std::copy(
            (char*)tag.GetBytes().begin(),
            (char*)tag.GetBytes().end(),
            arr.begin()
        );
    }

    bool operator==(const TagBytes& rhs) const {
        return arr == rhs.arr;
    }

};
namespace bundles {
    struct RegulatorData {
        const size_t value;
        double regulator;
        const TagBytes tag;

        RegulatorData(size_t value_, double regulator_, const TagBytes& tag_)
        : value(value_)
        , regulator(regulator_)
        , tag(tag_)
        { ; }

        bool operator==(const RegulatorData& rhs) const {
            return std::tuple{value, regulator, tag}
                == std::tuple{rhs.value, rhs.regulator, rhs.tag};
        }
    };
};

} // namespace dish

namespace std {
    template<>
    struct hash<dish::TagBytes> {
        size_t operator()(const dish::TagBytes& data) const {
            return emp::murmur_hash(std::span<std::byte>(
                (std::byte*)data.arr.begin(),
                (std::byte*)data.arr.end()
            ));
        }
    };

    template<>
    struct hash<dish::bundles::RegulatorData> {
        size_t operator()(const dish::bundles::RegulatorData& data) const {
            return emp::TupleHash<size_t, double, dish::TagBytes>{}(
                std::tuple{
                    data.value,
                    data.regulator,
                    data.tag
                }
            );
        }
    };
} // namespace dish

namespace dish {

namespace memtype {
    // idea taken from https://forum.hdfgroup.org/t/templatized-instantiation-of-h5-native-xxx-types/4168/2
    H5::PredType hid_from_type_t(const char &) { return H5::PredType::NATIVE_CHAR; }
    H5::PredType hid_from_type_t(const uint32_t &) { return H5::PredType::NATIVE_UINT32; }
    H5::PredType hid_from_type_t(const uint64_t &) { return H5::PredType::NATIVE_UINT64; }
    H5::PredType hid_from_type_t(const int &) { return H5::PredType::NATIVE_INT; }
    H5::PredType hid_from_type_t(const double &) { return H5::PredType::NATIVE_DOUBLE; }

    /// Determine HID type
    template <typename T>
    H5::PredType hid_from_type() { return hid_from_type_t(T()); }

    H5::ArrayType MakeArrayType() {
        const hsize_t dims[1]{Config::tag_t::GetNumBytes()};
        return H5::ArrayType(H5::PredType::NATIVE_CHAR, 1, dims);
    }

    H5::VarLenType MakeVarLenTypeSpec(const H5::DataType& type) {
        // SRP: don't use H5::PredType unless you want to cry
        /* inline static */ const H5::VarLenType var_array{type};
        return var_array;
    }

    H5::CompType MakeRegulatorData() {
        H5::CompType regulator_data(
            sizeof(bundles::RegulatorData)
        );

        regulator_data.insertMember(
            "value",
            HOFFSET(bundles::RegulatorData, value),
            hid_from_type<size_t>()
        );

        regulator_data.insertMember(
            "regulator",
            HOFFSET(bundles::RegulatorData, regulator),
            hid_from_type<double>()
        );

        regulator_data.insertMember(
            "tag",
            HOFFSET(bundles::RegulatorData, tag),
            MakeArrayType()
        );
        return regulator_data;
    }
};

namespace filetype {
    // https://support.hdfgroup.org/HDF5/release/dttable.html
    H5::PredType hid_from_type_t(const char &) { return H5::PredType::STD_I8LE; }
    H5::PredType hid_from_type_t(const uint32_t &) { return H5::PredType::STD_I32LE; }
    H5::PredType hid_from_type_t(const uint64_t &) { return H5::PredType::STD_I64LE; }
    H5::PredType hid_from_type_t(const int &) {
        switch(sizeof(int)) {
            case 4:
                return H5::PredType::STD_I32LE;
            case 8:
                return H5::PredType::STD_I64LE;
            default:
                assert(false && "Bad platform");
        }
    }
    H5::PredType hid_from_type_t(const double &) { return H5::PredType::IEEE_F64LE; }



    /// Determine HID type
    template <typename T>
    H5::PredType hid_from_type() { return hid_from_type_t(T()); }
};


template <typename T>
T convert_type(const T& a) {
    return a;
}

std::array<char, Config::tag_t::GetNumBytes()> convert_type(const Config::tag_t& a) {
    return TagBytes(a).arr;
}

} // namespace dish

#endif // #ifndef DISH_DATAHELPER_TYPES_HPP_INCLUDE
