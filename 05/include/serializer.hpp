template<typename T>
typename std::enable_if<!std::is_array<T>::value, Serializer&>::type
operator<<(Serializer& ser, const T& obj)
{
#ifdef DEBUG
    std::cout << "Serializing object of " << typeid(T).name();
#endif
    ser.save(obj);
    return ser;
}

template<typename T>
typename std::enable_if<std::is_array<T>::value, Serializer&>::type
operator<<(Serializer& ser, const T& arr)
{
#ifdef DEBUG
    std::cout << "Serializing array of " << typeid(T).name();
#endif
    for (const auto& val : arr) {
        ser.save(val);
    }
    return ser;
}

template<typename T>
void Serializer::print(T&& arg)
{
#ifdef DEBUG
    std::cout << __PRETTY_FUNCTION__ << std::endl;
#endif
    out_ << arg;
}

template<>
void Serializer::print<bool>(bool&& arg)
{
#ifdef DEBUG
    std::cout << __PRETTY_FUNCTION__ << std::endl;
#endif
    out_ << std::boolalpha << arg;
}
template<>
void Serializer::print<std::string_view>(std::string_view&& arg)
{
    out_ << arg.size() << SEPARATOR << arg;
}

template<>
void Serializer::print<char*>(char*&& arg)
{
    print(std::string_view(arg));
}

template<>
void Serializer::print<const char*>(const char*&& arg)
{
    print(std::string_view(arg));
}

template<>
void Serializer::print<std::string>(std::string&& arg)
{
    print(std::string_view(arg));
}

template<typename T, typename... Args>
Serializer::Error Serializer::process(T&& first, Args&& ... args)
{
#ifdef DEBUG
    std::cout << __PRETTY_FUNCTION__ << std::endl;
#endif
    Error err = process(std::forward<T>(first));
    if (err==CorruptedArchive) {
        return err;
    }
    out_ << SEPARATOR;
    if (!out_) {
        return Error::CorruptedArchive;
    }
    return process(std::forward<Args>(args)...);
}

template<typename T>
typename std::enable_if<!is_serializable<T, void(Serializer&)>::value, Serializer::Error>::type
Serializer::process(T&& arg)
{
#ifdef DEBUG
    std::cout << __PRETTY_FUNCTION__ << std::endl;
#endif
    print(std::forward<T>(arg));
    return !out_ ? CorruptedArchive : NoError;
}

template<typename T>
typename std::enable_if<is_serializable<T, void(Serializer&)>::value, Serializer::Error>::type
Serializer::process(T&& arg)
{
#ifdef DEBUG
    std::cout << __PRETTY_FUNCTION__ << std::endl;
#endif
    return save(std::forward<T>(arg));
}