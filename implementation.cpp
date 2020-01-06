#include <cstddef>
#include <iostream>
#include <limits>
#include <new>

#include "vint.h"

namespace {
    const size_t MAX_SIZE = vint::max_size();

    size_t next_capacity(size_t current = 0ul) {
        if (current < 16) {
            return 16;
        } else {
            if (current >= MAX_SIZE - current / 2) {
                return MAX_SIZE;
            } else {
                return current + current / 2;
            }
        }
    }
}  // namespace

vint::vint(size_t size, vint::valuetype default_value) : m_size(size), m_capacity(next_capacity(m_size)) {
    try {
        m_storage = new std::byte[m_capacity * sizeof(vint::valuetype)];
        m_data = new vint::valuetype*[m_capacity];
    } catch (std::bad_alloc exc) {
        std::cerr << __FILE__ << ":" << __LINE__ << '\n' << exc.what();
        throw exc;
    }
    for (size_t i = 0; i < m_size; ++i) {
        m_data[i] = new (m_storage + i * sizeof(vint::valuetype)) vint::valuetype(default_value);
    }
    for (size_t i = m_size; i < m_capacity; ++i) {
        m_data[i] = nullptr;
    }
}
vint::vint(vint const& oth) : m_size(oth.m_size), m_capacity(oth.m_capacity) {
    try {
        m_storage = new std::byte[m_capacity * sizeof(vint::valuetype)];
        m_data = new vint::valuetype*[m_capacity];
    } catch (std::bad_alloc exc) {
        std::cerr << __FILE__ << ":" << __LINE__ << '\n' << exc.what();
        throw exc;
    }
    for (size_t i = 0; i < m_size; ++i) {
        m_data[i] = new (m_storage + i * sizeof(vint::valuetype)) vint::valuetype(*(oth.m_data[i]));
    }
    for (size_t i = m_size; i < m_capacity; ++i) {
        m_data[i] = nullptr;
    }
}

vint::~vint() {
    for (size_t i = 0; i < m_capacity; ++i) {
        if (m_data[i]) (*m_data[i]).~valuetype();
    }
    delete[] m_data;
    delete[] m_storage;
    m_storage = nullptr;
    m_size = 0;
    m_capacity = 0;
}

vint& vint::operator=(vint oth) {
    this->swap(oth);
    return *this;
}

void vint::push_back(valuetype val) {
    if (m_size + 1 >= m_capacity) {
        if (m_capacity == MAX_SIZE) {
            throw allocation_falilure("Cannot push one element back\n");
        } else {  // realloc
            // create temporary variables
            size_t tmp_cap = next_capacity(m_capacity);
            std::byte* tmp_storage = nullptr;
            vint::valuetype** tmp_data = nullptr;
            try {
                tmp_storage = new std::byte[tmp_cap * sizeof(vint::valuetype)];
            } catch (std::bad_alloc e) {
                throw e;
            }

            tmp_data = new vint::valuetype* [tmp_cap] {};

            // copydefault_value old data

            for (size_t i = 0; i < m_size; ++i) {
                tmp_data[i] = new (tmp_storage + i * sizeof(vint::valuetype)) vint::valuetype(*m_data[i]);
            }

            // delete old data

            for (size_t i = 0; i < m_capacity; ++i) {
                if (m_data[i]) (*m_data[i]).~valuetype();
            }
            delete[] m_data;
            delete[] m_storage;

            // swap
            std::swap(m_capacity, tmp_cap);
            std::swap(m_storage, tmp_storage);
            std::swap(m_data, tmp_data);

            // add one element
            ++m_size;
            m_data[m_size - 1] = new (m_storage + (m_size - 1) * sizeof(vint::valuetype)) vint::valuetype(val);
        }
    } else {  // just add the element
        ++m_size;
        m_data[m_size - 1] = new (m_storage + (m_size - 1) * sizeof(vint::valuetype)) vint::valuetype(val);
    }
}

vint::valuetype vint::operator[](size_t index) const { return *m_data[index]; }
vint::valuetype& vint::operator[](size_t index) { return *m_data[index]; }
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
size_t vint::max_size() { return m_max_size; }
void vint::swap(vint& oth) {
    std::swap(oth.m_size, m_size);
    std::swap(oth.m_capacity, m_capacity);
    std::swap(oth.m_storage, m_storage);
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
