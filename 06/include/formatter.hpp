namespace fmt {

template<Streamable... Args>
std::string format(std::string_view format, Args... args)
{
    auto index = impl::parse(format, sizeof...(args));
    std::string result(format.data(), format.size());
    impl::process(result, 0, index, args...);
    return result;
}

namespace impl {

namespace {
inline void replace_pattern(std::string& result, const std::string& str, std::size_t cur_idx, Index& index)
{
    for (auto idx : std::ranges::reverse_view(index.map[cur_idx])) { // reverse to not invalidate prev indexes
        result.replace(index.idx[idx].start, index.idx[idx].end-index.idx[idx].start + 1, str);
    }
}

inline void update_index(Index& index, std::size_t cur_idx, ssize_t diff)
{
    if (diff!=0) {
        const auto UpdateOneEntry = [&](std::size_t cur) {
          std::for_each(index.idx.begin() + cur, index.idx.end(), [diff](auto& index) {
            index.start += diff;
            index.end += diff;
          });
        };
        for (auto idx : index.map[cur_idx]) {
            UpdateOneEntry(idx);
        }
    }
}
} // namespace

template<Streamable T>
std::string to_string(const T& val)
{
    std::stringstream ss;
    ss << val;
    return ss.str();
}

Index parse(std::string_view format, std::size_t args_num)
{
    static constexpr const char open_bracket = '{';
    static constexpr const char close_bracket = '}';
    Index result;
    std::string_view::iterator start = std::find(format.begin(), format.end(), open_bracket);;
    std::string_view::iterator end = std::find(start, format.end(), close_bracket);
    std::unordered_set<int> used;
    do {
        int idx{};
        auto [ptr, ec] = std::from_chars(start+1, end, idx);
        if (ec!=std::errc()) {
            throw std::invalid_argument(std::string("Can not convert `")+std::string(start, end  + 1)+"` to integer");
        }
        if (idx<0 || static_cast<std::size_t>(idx)>args_num) {
            throw std::invalid_argument(std::string("Found invalid index: ")+std::to_string(idx));
        }
        result.idx.emplace_back(start-format.begin(), end-format.begin());
        result.map[idx].emplace_back(result.idx.size()-1);
        used.insert(idx);
        start = std::find(start + 1, format.end(), open_bracket);
        end = std::find(start, format.end(), close_bracket);
    } while (start != format.end());
    if (used.size() != args_num){
        std::stringstream sstream;
        sstream << "Used args mismatch. Used: " << used.size() << " . Total args: args_num" << args_num;
        throw std::invalid_argument(sstream.str());
    }
    return result;
}

template<Streamable T, Streamable... Args>
void process(std::string& result, std::size_t idx, Index& index, const T& arg, const Args&... args)
{
    process(result, idx, index, arg);
    ++idx;
    process(result, idx, index, args...);
}

template<Streamable T>
void process(std::string& result, std::size_t cur, Index& index, const T& arg)
{
    std::string str = to_string(arg);
    std::string pattern = "{"+std::to_string(cur)+"}";
    replace_pattern(result, str, cur, index);
    update_index(index, cur, std::ssize(str)-std::ssize(pattern));
}

} //namespace impl
} // namespace fmt