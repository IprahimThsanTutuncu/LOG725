#pragma once

#include "DataType.h"

class DataTypeHolder
{
public:
    DataTypeHolder() : mask(0) {}

    template<typename T>
    void setData(DATA_TYPE target, T&& data)
    {
        auto index = static_cast<unsigned int>(target);
        delete dataArray[index];

        if constexpr (std::is_lvalue_reference_v<T>) {
            dataArray[index] = static_cast<DataType*>(&data);
        }
        else {
            dataArray[index] = new T(std::move(data));
        }

        mask |= (1 << index);
    }

    template<typename T>
    T* getData(DATA_TYPE target)
    {
        return static_cast<T*>(dataArray[static_cast<unsigned int>(target)]);
    }

    bool hasData(DATA_TYPE target) const
    {
        return (mask & (1 << static_cast<unsigned int>(target))) != 0;
    }

    ~DataTypeHolder() {
        for (auto& data : dataArray) {
            delete data;
        }
    }

private:
    DataType* dataArray[20] = { nullptr };
    unsigned int mask{ 0 };
};
