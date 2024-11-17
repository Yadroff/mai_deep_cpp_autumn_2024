#pragma once

#include <ostream>
#include <cstdint>

#include "is_serializable.h"

class Serializer {
    friend class Serializer_Test;
public:
    enum Error {
      NoError,
      CorruptedArchive
    };

    struct CorruptedData {
      template<typename Serializer>
      Serializer::Error serialize(Serializer&)
      {
          return CorruptedArchive;
      }
    };

    static constexpr char SEPARATOR = ' ';
public:
    explicit Serializer(std::ostream& out)
            :out_(out) { }

    template<typename T>
    Error save(T&& object)
    {
        return object.serialize(*this);
    }

    template<typename... Args>
    Error operator()(Args... args)
    {
        if (was_calls) {
            out_ << SEPARATOR;
        }
        Error ret = process(std::forward<Args>(args)...);
        if (!was_calls) {
            was_calls = true;
        }
        return ret;
    }
private:
    template<typename T>
    void print(T&& arg);

    template<typename T, typename... Args>
    Error process(T&& first, Args&& ... args);

    template<typename T>
    typename std::enable_if<!is_serializable<T, void(Serializer&)>::value, Error>::type
    process(T&& arg);

    template<typename T>
    typename std::enable_if<is_serializable<T, void(Serializer&)>::value, Error>::type
    process(T&& arg);

private:
    std::ostream& out_;
    bool was_calls = false;
};

#include "serializer.hpp"