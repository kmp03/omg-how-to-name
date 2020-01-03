#include <iostream>
#include <limits>
#include <new>

#include "header.h"

namespace {
    size_t next_capacity(size_t current = 0ul) {
        if (current < 10) {
            return 10;
        } else {
            if (static_cast<double>(current - 2) > (static_cast<double>(std::numeric_limits<size_t>::max() / 1.5))) {
                return std::numeric_limits<size_t>::max();
            } else {
                return static_cast<size_t>(current * 1.5);
            }
        }
    }
}  // namespace

vint::vint(size_t size, vint::valuetype default_value) : m_size(size), m_capacity(next_capacity(m_size)) {
    try {
        m_data = new valuetype[m_capacity];
    } catch (std::bad_alloc exc) {
        std::cerr << __FILE__ << ":" << __LINE__ << '\n' << exc.what();
        throw exc;
    }
    for (size_t i = 0; i < m_size; ++i) m_data[i] = default_value;
}
vint::vint(vint const& oth) : m_size(oth.m_size), m_capacity(oth.m_capacity) {
    try {
        m_data = new valuetype[m_capacity];
    } catch (std::bad_alloc exc) {
        std::cerr << __FILE__ << ":" << __LINE__ << '\n' << exc.what();
        throw exc;
    }
    for (size_t i = 0; i < m_size; ++i) m_data[i] = oth.m_data[i];
}

vint::~vint() {
    delete[] m_data;
    m_data = nullptr;
    m_size = 0;
    m_capacity = 0;
}

vint& vint::operator=(vint oth) {
    this->swap(oth);
    return *this;
}

void vint::push_back(valuetype val) {
    if (m_size + 1 == m_capacity) {
        if (m_capacity == std::numeric_limits<size_t>::max()) {
            throw allocation_falilure("Cannot push one element back\n");
        } else {  // realloc
            size_t tmp_cap = next_capacity(m_capacity);
            valuetype* tmp_data = nullptr;
            try {
                tmp_data = new valuetype[tmp_cap];
            } catch (std::bad_alloc e) {
                throw e;
            }

            for (size_t i = 0; i < m_size; ++i) {
                tmp_data[i] = m_data[i];
            }
            tmp_data[m_size] = val;

            delete[] m_data;
            m_data = tmp_data;
            ++m_size;
            m_capacity = tmp_cap;
        }
    } else {  // just add the element
        ++m_size;
        m_data[m_size - 1] = val;
    }
}

vint::valuetype vint::operator[](size_t index) const { return m_data[index]; }
vint::valuetype& vint::operator[](size_t index) { return m_data[index]; }
bool vint::operator==(vint const& oth) {
    if (!(oth.m_size == m_size)) return false;
    // invariant: m_size == oth.m_size
    for (size_t i = 0; i < m_size; ++i) {
        if (oth[i] != (*this)[i]) return false;
    }
    return true;
}
bool vint::operator!=(vint const& oth) { return !(*this == oth); }

size_t vint::size() const { return m_size; }
size_t vint::capacity() const { return m_capacity; }

void vint::swap(vint& oth) {
    std::swap(oth.m_size, m_size);
    std::swap(oth.m_capacity, m_capacity);
    std::swap(oth.m_data, m_data);
}
void swap(vint& lhs, vint& rhs) { lhs.swap(rhs); }
std::ostream& operator<<(std::ostream& os, vint const& arr) {
    os << '[';
    for (size_t i = 0; i < arr.size(); ++i) {
        os << arr[i] << ' ';
    }
    os << "]\n";
    return os;
}
