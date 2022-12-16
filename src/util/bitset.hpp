#pragma once

#include <limits>
#include <vector>

namespace leafsync {

class bitset {
public:
    typedef unsigned int size_type;
    typedef unsigned char block_type;

    bitset();
    bitset(const bitset& copy);
    bitset(bitset&& move);

    void resize(size_type n);
    void set(size_type i, bool value);
    bool get(size_type i) const;

    bool operator[](size_type i) const;
    bitset& operator=(bitset&& other);
    bitset& operator=(const bitset& other);

    size_type size() const;
    const std::vector<block_type>& blocks() const;
    std::vector<block_type>& blocks();

    static constexpr size_type bits_per_block = std::numeric_limits<block_type>::digits;
    static constexpr size_type min_blocks(size_type n);
    static constexpr size_type bit_mask(size_type i);
    inline block_type& bit_block(size_type i);
    inline const block_type& bit_block(size_type i) const;

private:

    size_type _size = 0;
    std::vector<block_type> _blocks;

};

}