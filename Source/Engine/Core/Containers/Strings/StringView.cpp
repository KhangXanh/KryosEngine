/**
 * @file Engine/Core/Containers/Strings/StringView.cpp
 * This is a Kryos Engine Implementation file
 */

#include "StringView.hpp"
#include "String.hpp"

namespace Kryos
{
    StringView::StringView() noexcept : mData(nullptr), mLength(0) {}

    StringView::StringView(const char *cstr, SizeType length) noexcept : mData(cstr), mLength(length) {}

    StringView::StringView(const char *cstr) : mData(cstr), mLength(0)
    {
        if (cstr)
        {
            while (cstr[mLength] != '\0')
            {
                mLength++;
            }
        }
    }

    StringView::StringView(const String &str) noexcept : mData(str.CStr()), mLength(str.GetSize()) {}

    const char &StringView::At(SizeType index) const
    {
        if (index >= mLength)
        {
            static const char null_char = '\0';
            return null_char;
        }
        return mData[index];
    }

    StringView StringView::SubString(SizeType offset, SizeType count) const
    {
        if (IsEmpty() || offset > mLength)
            return StringView();

        SizeType realCount = (count == NPosition || offset + count > mLength)
                                 ? mLength - offset
                                 : count;

        return StringView(mData + offset, realCount);
    }

    String StringView::ToString() const
    {
        return String(*this);
    }

    StringView::SizeType StringView::Find(char ch, SizeType pos) const noexcept
    {
        for (SizeType i = pos; i < mLength; ++i)
            if (mData[i] == ch)
                return i;
        return NPosition;
    }

    StringView::SizeType StringView::Find(StringView other, SizeType pos) const noexcept
    {
        if (pos > mLength)
            return NPosition;
        if (other.mLength == 0)
            return pos;
        if (other.mLength > mLength - pos)
            return NPosition;

        for (SizeType i = pos; i <= mLength - other.mLength; ++i)
        {
            if (std::memcmp(mData + i, other.mData, other.mLength) == 0)
                return i;
        }
        return NPosition;
    }

    bool StringView::StartsWith(StringView other) const noexcept
    {
        return mLength >= other.mLength &&
               std::memcmp(mData, other.mData, other.mLength) == 0;
    }

    bool StringView::EndsWith(StringView other) const noexcept
    {
        return mLength >= other.mLength &&
               std::memcmp(mData + mLength - other.mLength, other.mData, other.mLength) == 0;
    }

    bool StringView::operator==(const StringView &other) const noexcept
    {
        if (mLength != other.mLength)
            return false;
        if (mData == other.mData)
            return true;

        for (SizeType i = 0; i < mLength; ++i)
        {
            if (mData[i] != other.mData[i])
                return false;
        }
        return true;
    }

    bool StringView::operator!=(const StringView &other) const noexcept
    {
        return !(*this == other);
    }
}