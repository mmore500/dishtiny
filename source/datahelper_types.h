#pragma once

#include <array>

#include "H5Cpp.h"

#include "Config.h"

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
};
namespace bundles {
    struct RegulatorData {
        const size_t value;
        // SRP: I don't think regulators are doubles
        double regulator;
        const TagBytes tag;
    };
};
namespace std {
    template<>
    struct hash<TagBytes> {
        size_t operator()(const TagBytes& data) const {
            return emp::murmur_hash(std::span<std::byte>(
                (std::byte*)data.arr.begin(),
                (std::byte*)data.arr.end()
            ));
        }
    };

    template<>
    struct hash<bundles::RegulatorData> {
        size_t operator()(const bundles::RegulatorData& data) const {
            return emp::TupleHash<size_t, double, TagBytes>{}(
                std::tuple{
                    data.value,
                    data.regulator,
                    data.tag
                }
            );
        }
    };
};

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

    /* inline static */ const H5::ArrayType arr(H5::PredType::NATIVE_CHAR, 1, internal::dims);
    /* inline static */ const H5::VarLenType var_array{&arr};

    H5::CompType make_regulator_data() {
        H5::CompType regulator_data;

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
            "regulator",
            HOFFSET(bundles::RegulatorData, tag),
            arr
        );

        return regulator_data;
    }

    H5::CompType regulator_data(make_regulator_data());

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

    /* inline static */ const H5::ArrayType arr(H5::PredType::STD_U8BE, 1, internal::dims);
    /* inline static */ const H5::VarLenType var_array{&arr};


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
