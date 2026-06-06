/**
 * @file Engine/Core/Containers/Strings/StringView.hpp
 * This is a Kryos Engine Library header
 */

#pragma once

#include "Core/Core.hpp"
#include <format>

namespace Kryos
{
    /// @note Forward declare
    class String;

    /**
     * @brief
     * A light-weight string for Kryos Engine
     */
    class KRYOS_API StringView
    {
    public:
        using Iterator = const char *;

        /// @note An invalid position in a string
        static constexpr SizeType NPosition = -1ULL;

        /// @brief This function is for Kryos 
        Iterator Begin() const noexcept { return mData; }
        /// @brief This function is for Kryos 
        Iterator End() const noexcept { return mData + mLength; }
        /// @brief This function is for Kryos 
        Iterator CBegin() const noexcept { return mData; }
        /// @brief This function is for Kryos 
        Iterator CEnd() const noexcept { return mData + mLength; }

        /// @brief This function is for <algorithm>
        Iterator begin() const noexcept { return Begin(); }
        /// @brief This function is for <algorithm>
        Iterator end() const noexcept { return End(); }
        /// @brief This function is for <algorithm>
        Iterator cbegin() const noexcept { return CBegin(); }
        /// @brief This function is for <algorithm>
        Iterator cend() const noexcept { return CEnd(); }

        StringView() noexcept;
        StringView(const char *cstr);
        StringView(const char *cstr, SizeType length) noexcept;
        StringView(const String &str) noexcept;

        ~StringView() noexcept = default;

        // Explicitly ensuring clean assignments across views
        StringView(const StringView&) noexcept = default;
        StringView(StringView&&) noexcept = default;
        StringView& operator=(const StringView&) noexcept = default;
        StringView& operator=(StringView&&) noexcept = default;

        const char *CStr() const noexcept { return mData; }
        SizeType GetLength() const noexcept { return mLength; }
        bool IsEmpty() const noexcept { return mLength == 0; }
        const char &operator[](SizeType index) const { return mData[index]; }
        const char &At(SizeType index) const;
        String ToString() const;

        StringView SubString(SizeType offset, SizeType count = NPosition) const;

        SizeType Find(char ch, SizeType pos = 0) const noexcept;
        SizeType Find(StringView other, SizeType pos = 0) const noexcept;

        bool Contains(char ch) const noexcept { return Find(ch) != NPosition; }
        bool Contains(StringView other) const noexcept { return Find(other) != NPosition; }

        bool StartsWith(StringView other) const noexcept;
        bool EndsWith(StringView other) const noexcept;

        bool operator==(const StringView &other) const noexcept;
        bool operator!=(const StringView &other) const noexcept;

    private:
        const char *mData = nullptr;
        SizeType mLength = 0;
    };
}

/// @note Tell the <format> library how to format String
template <>
struct std::formatter<Kryos::StringView> : std::formatter<std::string_view>
{
    auto format(const Kryos::StringView &sv, std::format_context &ctx) const
    {
        return std::formatter<std::string_view>::format(
            std::string_view(sv.CStr(), sv.GetLength()), ctx);
    }
};