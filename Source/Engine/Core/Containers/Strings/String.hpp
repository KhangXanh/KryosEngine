/**
 * @file Engine/Core/Containers/Strings/String.hpp
 * This is a Kryos Engine Library header
 */

#pragma once

#include "Core/Memory/Allocator.hpp"
#include <cstring>
#include <utility>
#include <format>

namespace Kryos
{
    /// @note Forward declare
    class StringView;

    /**
     * @brief
     * A string for Kryos Engine
     */
    class KRYOS_API String
    {
    public:
        using Iterator = const char *;

        /// @note An invalid position in a string
        static constexpr SizeType NPosition = -1ULL;

        /// @note 15 bytes leaves room for the null terminator in a 16-byte block
        /// @note Total class size will be 32 bytes on a 64-bit architecture
        static constexpr SizeType SSO_CAPACITY = 15;

        /// @brief This function is for Kryos 
        Iterator Begin() const noexcept { return CStr(); }
        /// @brief This function is for Kryos 
        Iterator End() const noexcept { return CStr() + mSize; }
        /// @brief This function is for Kryos 
        Iterator CBegin() const noexcept { return CStr(); }
        /// @brief This function is for Kryos 
        Iterator CEnd() const noexcept { return CStr() + mSize; }

        /// @brief This function is for <algorithm>
        Iterator begin() const noexcept { return Begin(); }
        /// @brief This function is for <algorithm>
        Iterator end() const noexcept { return End(); }
        /// @brief This function is for <algorithm>
        Iterator cbegin() const noexcept { return CBegin(); }
        /// @brief This function is for <algorithm>
        Iterator cend() const noexcept { return CEnd(); }

        String() noexcept;
        String(const char *cstr);
        String(const String &other);
        String(const StringView &view);
        String(String &&other) noexcept; // Move Constructor: Steals memory instead of copying

        ~String();

        String &operator=(const char *cstr);
        String &operator=(const String &other);
        String &operator=(String &&other) noexcept;

        const char *CStr() const noexcept { return HIsSSO() ? mSSOBuffer : mHeapData; }
        char *Data() noexcept { return HIsSSO() ? mSSOBuffer : mHeapData; }

        SizeType GetSize() const noexcept { return mSize; }
        SizeType GetCapacity() const noexcept { return mCapacity; }
        bool IsEmpty() const noexcept { return mSize == 0; }

        char &operator[](SizeType index) { return Data()[index]; }
        const char &operator[](SizeType index) const { return CStr()[index]; }

    private:
        // If our capacity fits within the SSO limit, we are using the internal buffer
        bool HIsSSO() const noexcept { return mCapacity <= SSO_CAPACITY; }
        void HInitialize(const char *cstr, SizeType len);
        void HAssign(const char *cstr, SizeType len);
        void HRelease();

    private:
        // The union shares the 8-byte pointer space with a 16-byte character array
        union
        {
            char *mHeapData;
            char mSSOBuffer[SSO_CAPACITY + 1];
        };
        SizeType mSize = 0;
        SizeType mCapacity = SSO_CAPACITY;

        // If TAllocator has no state, it takes up 0 bytes in the class
        KS_NO_UNIQUE_ADDRESS
        TAllocator<char> mAllocator;
    };
}

/// @note Tell the <format> library how to format String
template <>
struct std::formatter<Kryos::String> : std::formatter<const char *>
{
    auto format(const Kryos::String &str, std::format_context &ctx) const
    {
        return std::formatter<const char *>::format(str.CStr(), ctx);
    }
};