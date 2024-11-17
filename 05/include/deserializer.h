#pragma once

#include <istream>
#include <concepts>
#include <cstdint>

#include "is_serializable.h"

class Deserializer {
    friend class Deserializer_Test;
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
    explicit Deserializer(std::istream& in)
            :in_(in) { }

    template<typename T>
    Error load(T& object)
    {
        return object.serialize(*this);
    }

    template<typename... Args>
    Error operator()(Args& ... args)
    {
        return process(args...);
    }
private:
    template<typename T>
    void read(T& arg);

    template<typename T, typename... Args>
    Error process(T& first, Args& ... args);

    template<typename T>
    typename std::enable_if<!is_serializable<T, void(Deserializer&)>::value, Error>::type
    process(T& arg);

    template<typename T>
    typename std::enable_if<is_serializable<T, void(Deserializer&)>::value, Error>::type
    process(T& arg);

private:
    std::istream& in_;
};

#include "deserializer.hpp"