/**
 * @file Engine/Core/Container/Strings/String.cpp
 * This is a Kryos Engine Implementation file
 */

#include "String.hpp"
#include "StringView.hpp"

namespace Kryos
{
    String::String() noexcept : mSize(0), mCapacity(SSO_CAPACITY)
    {
        mSSOBuffer[0] = '\0';
    }

    String::String(const char *cstr)
    {
        if (!cstr)
            cstr = "";
        SizeType len = std::strlen(cstr);
        HInitialize(cstr, len);
    }

    String::String(const String &other)
    {
        HInitialize(other.CStr(), other.mSize);
    }

    String::String(const Kryos::StringView &view)
    {
        HInitialize(view.CStr(), view.GetLength());
    }

    String::String(String &&other) noexcept
        : mSize(other.mSize), mCapacity(other.mCapacity)
    {
        if (other.HIsSSO())
        {
            std::memcpy(mSSOBuffer, other.mSSOBuffer, SSO_CAPACITY + 1);
        }
        else
        {
            mHeapData = other.mHeapData;
        }

        // Reset moved-from object safely
        other.mSize = 0;
        other.mCapacity = SSO_CAPACITY;
        other.mSSOBuffer[0] = '\0';
    }

    String::~String()
    {
        HRelease();
    }

    void String::HInitialize(const char *cstr, SizeType len)
    {
        mSize = len;
        if (len <= SSO_CAPACITY)
        {
            mCapacity = SSO_CAPACITY;
            std::memcpy(mSSOBuffer, cstr, len);
            mSSOBuffer[len] = '\0';
        }
        else
        {
            mCapacity = len;
            mHeapData = mAllocator.Allocate(mCapacity + 1);
            std::memcpy(mHeapData, cstr, len);
            mHeapData[len] = '\0';
        }
    }

    void String::HAssign(const char *cstr, SizeType len)
    {
        if (len <= mCapacity)
        {
            // Memory Reuse: We already have enough space, avoid reallocating
            char *dest = Data();
            std::memmove(dest, cstr, len); // memmove is safer here in case of overlapping substrings
            dest[len] = '\0';
            mSize = len;
        }
        else
        {
            // We need more space. Allocate first, then release, to handle exceptions/safety.
            char *newData = mAllocator.Allocate(len + 1);
            std::memcpy(newData, cstr, len);
            newData[len] = '\0';

            HRelease();

            mHeapData = newData;
            mCapacity = len;
            mSize = len;
        }
    }

    void String::HRelease()
    {
        if (!HIsSSO() && mHeapData)
        {
            // +1 to account for the null terminator we allocated
            mAllocator.Deallocate(mHeapData, mCapacity + 1);
        }

        // Reset to empty SSO state
        mSize = 0;
        mCapacity = SSO_CAPACITY;
        mSSOBuffer[0] = '\0';
    }

    String &String::operator=(const char *cstr)
    {
        if (!cstr)
            cstr = "";
        if (cstr == CStr())
            return *this; // Self-assignment check

        HAssign(cstr, std::strlen(cstr));
        return *this;
    }

    String &String::operator=(const String &other)
    {
        if (this != &other)
        {
            HAssign(other.CStr(), other.mSize);
        }
        return *this;
    }

    String &String::operator=(String &&other) noexcept
    {
        // Move Assignment Operator
        if (this != &other)
        {
            HRelease(); // Clean up current memory

            mSize = other.mSize;
            mCapacity = other.mCapacity;

            if (other.HIsSSO())
            {
                std::memcpy(mSSOBuffer, other.mSSOBuffer, SSO_CAPACITY + 1);
            }
            else
            {
                mHeapData = other.mHeapData;
            }

            other.mSize = 0;
            other.mCapacity = SSO_CAPACITY;
            other.mSSOBuffer[0] = '\0';
        }
        return *this;
    }
}