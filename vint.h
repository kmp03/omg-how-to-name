#ifndef HEADER_H
#define HEADER_H
#include <cstddef>
#include <fstream>
#include <istream>
#include <limits>
#include <new>
#include <type_traits>

struct vint {
    using valuetype = int;

    explicit vint(size_t size = 0, valuetype default_value = {});
    vint(vint const &);
    vint(vint &&) = delete;
    ~vint();

    vint &operator=(vint);
    vint &operator=(vint &&) = delete;

    void push_back(valuetype);
    valuetype operator[](size_t) const;
    valuetype &operator[](size_t);
    bool operator==(vint const &);
    bool operator!=(vint const &);

    size_t size() const;
    size_t capacity() const;
    static size_t max_size();

    void swap(vint &);

private:
    struct allocation_falilure : std::runtime_error {
        allocation_falilure(std::string problem) : std::runtime_error(problem) {}
    };
    size_t m_size;
    size_t m_capacity;
    static size_t m_max_size;
    std::byte *m_storage = nullptr;
    valuetype **m_data;
};

inline size_t vint::m_max_size = std::numeric_limits<size_t>::max();

void swap(vint &, vint &);
std::ostream &operator<<(std::ostream &, vint const &);

#endif  // HEADER_H
