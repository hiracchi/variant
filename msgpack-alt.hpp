#ifndef MSGPACK_ALT_H
#define MSGPACK_ALT_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "variant.hpp"

class MsgPack {
public:
    typedef int8_t INT8;
    typedef int16_t INT16;
    typedef int32_t INT32;
    typedef int64_t INT64;
    typedef uint8_t UINT8;
    typedef uint16_t UINT16;
    typedef uint32_t UINT32;
    typedef uint64_t UINT64;

public:
    explicit MsgPack(const Variant& data = Variant());
    MsgPack(const MsgPack& rhs);
    ~MsgPack();

    MsgPack& operator=(const MsgPack& rhs);

public:
    Variant getVariant() const;

    /// MsgPack形式のファイルを読み込む
    ///
    /// @retval true  ファイルの読み込みに成功した
    /// @retval false ファイルの読み込みに失敗した
    bool load(const std::string& path);

    void save(const std::string& path) const;

    void pack(const std::string& str);
    std::string dump() const;

protected:
    Variant loadBinary(std::istream& ifs);
    int unpack_positiveFixNum(unsigned char c);
    int unpack_negativeFixNum(unsigned char c);
    UINT8 unpack_uint8(std::istream& ifs);
    UINT16 unpack_uint16(std::istream& ifs);
    UINT32 unpack_uint32(std::istream& ifs);
    UINT64 unpack_uint64(std::istream& ifs);

    Variant unpack_bin8(std::istream& ifs);
    Variant unpack_bin16(std::istream& ifs);
    Variant unpack_bin32(std::istream& ifs);

    INT8 unpack_int8(std::istream& ifs);
    INT16 unpack_int16(std::istream& ifs);
    INT32 unpack_int32(std::istream& ifs);
    INT64 unpack_int64(std::istream& ifs);

    float unpack_float(std::istream& ifs);
    double unpack_double(std::istream& ifs);

    Variant unpack_fixraw(const char in, std::istream& ifs);
    Variant unpack_str8(std::istream& ifs);
    Variant unpack_str16(std::istream& ifs);
    Variant unpack_str32(std::istream& ifs);

    Variant unpack_ext8(std::istream& ifs);
    Variant unpack_ext16(std::istream& ifs);
    Variant unpack_ext32(std::istream& ifs);
    Variant unpack_fixext1(std::istream& ifs);
    Variant unpack_fixext2(std::istream& ifs);
    Variant unpack_fixext4(std::istream& ifs);
    Variant unpack_fixext8(std::istream& ifs);
    Variant unpack_fixext16(std::istream& ifs);

    Variant unpack_fixarray(const char in, std::istream& ifs);
    Variant unpack_array16(std::istream& ifs);
    Variant unpack_array32(std::istream& ifs);
    Variant unpack_fixmap(const char in, std::istream& ifs);
    Variant unpack_map16(std::istream& ifs);
    Variant unpack_map32(std::istream& ifs);

    std::string dump(const Variant& data) const;
    std::string dump_scalar(const Variant& data) const;
    std::string dump_array(const Variant& data) const;
    std::string dump_map(const Variant& data) const;

    std::string pack(bool value) const;
    std::string pack(UINT8 value) const;
    std::string pack(UINT16 value) const;
    std::string pack_uint32(UINT32 value) const;
    std::string pack_uint64(UINT64 value) const;
    std::string pack(INT8 value) const;
    std::string pack(INT16 value) const;
    std::string pack_int32(INT32 value) const;
    std::string pack_int64(INT64 value) const;
    std::string pack(double value) const;
    std::string pack(const char* pBuf, const int size) const;

    template<typename T>
    void write(std::ostringstream& os, T value) const {
        os.write((char*)&value, sizeof(T));
    }

protected:
    Variant unpack_ext(std::istream& ifs,
                               const std::size_t size);

protected:
    template<typename T>
    T changeEndian(T value) const {
        char* p = reinterpret_cast<char*>(&value);
        std::reverse(p, p + sizeof(T));

        return value;
    }

    void changeEndian(char* p, std::size_t size) const {
        std::reverse(p, p + size);
    }

    bool isLittleEndian() const {
        int i = 1;
        return *(reinterpret_cast<char*>(&i)) == 1;
    }

    bool isBigEndian() const {
        return !(this->isLittleEndian());
    }

    template<typename T>
    T toBigEndian(T value) const {
        if (this->isLittleEndian() == true) {
            value = this->changeEndian(value);
        }
        return value;
    }

    template<typename T>
    T toLittleEndian(T value) const {
        if (this->isBigEndian() == true) {
            value = this->changeEndian(value);
        }
        return value;
    }

protected:
    Variant data_;

    /// デバッグ用変数
    /// 現在の読み込み位置(byte)を記憶する
    std::size_t debugCurrentPos_;
};


// Implementation **************************************************************
MsgPack::MsgPack(const Variant& data) : data_(data) {
}


MsgPack::MsgPack(const MsgPack& rhs) : data_(rhs.data_) {
}


MsgPack::~MsgPack() {
}


MsgPack& MsgPack::operator=(const MsgPack& rhs) {
    if (this != &rhs) {
        this->data_ = rhs.data_;
    }

    return *this;
}


Variant MsgPack::getVariant() const {
    return this->data_;
}


bool MsgPack::load(const std::string& path) {
    std::ifstream ifs;
    ifs.open(path.c_str(), std::ios::in | std::ios::binary);
    if (!ifs) {
        return false;
    }

    this->debugCurrentPos_ = 0; // initialize
    this->data_ = this->loadBinary(ifs);
    return true;
}


void MsgPack::pack(const std::string& str) {
    std::istringstream iss(str);
    this->debugCurrentPos_ = 0; // initialize
    this->data_ = this->loadBinary(iss);
}


Variant MsgPack::loadBinary(std::istream& ifs) {
    Variant ans;

    if (ifs.eof() != true) {
        unsigned char c;
        ifs.read((char*)&c, 1);
        ++(this->debugCurrentPos_);

        switch (c) {
        case (unsigned char)(0xc0):
            // do nothing
            break;

        case (unsigned char)(0xc2):
            ans.set(false);
            break;

        case (unsigned char)(0xc3):
            ans.set(true);
            break;

        case (unsigned char)(0xc4):
            ans = this->unpack_bin8(ifs);
            break;

        case (unsigned char)(0xc5):
            ans = this->unpack_bin16(ifs);
            break;

        case (unsigned char)(0xc6):
            ans = this->unpack_bin32(ifs);
            break;

        case (unsigned char)(0xc7):
            ans = this->unpack_ext8(ifs);
            break;

        case (unsigned char)(0xc8):
            ans = this->unpack_ext16(ifs);
            break;

        case (unsigned char)(0xc9):
            ans = this->unpack_ext32(ifs);
            break;

        case (unsigned char)(0xca):
            ans = this->unpack_float(ifs);
            break;

        case (unsigned char)(0xcb):
            ans = this->unpack_double(ifs);
            break;

        case (unsigned char)(0xcc):
            ans = this->unpack_uint8(ifs);
            break;

        case (unsigned char)(0xcd):
            ans = this->unpack_uint16(ifs);
            break;

        case (unsigned char)(0xce):
            ans = this->unpack_uint32(ifs);
            break;

        case (unsigned char)(0xcf):
            ans = (unsigned long)this->unpack_uint64(ifs);
            break;

        case (unsigned char)(0xd0):
            ans = this->unpack_int8(ifs);
            break;

        case (unsigned char)(0xd1):
            ans = this->unpack_int16(ifs);
            break;

        case (unsigned char)(0xd2):
            ans = this->unpack_int32(ifs);
            break;

        case (unsigned char)(0xd3):
            ans = (long)this->unpack_int64(ifs);
            break;

        case (unsigned char)(0xd4):
            ans = this->unpack_fixext1(ifs);
            break;

        case (unsigned char)(0xd5):
            ans = this->unpack_fixext2(ifs);
            break;

        case (unsigned char)(0xd6):
            ans = this->unpack_fixext4(ifs);
            break;

        case (unsigned char)(0xd7):
            ans = this->unpack_fixext8(ifs);
            break;

        case (unsigned char)(0xd8):
            ans = this->unpack_fixext16(ifs);
            break;

        case (unsigned char)(0xd9):
            ans = this->unpack_str8(ifs);
            break;

        case (unsigned char)(0xda):
            ans = this->unpack_str16(ifs);
            break;

        case (unsigned char)(0xdb):
            ans = this->unpack_str32(ifs);
            break;

        case (unsigned char)(0xdc):
            ans = this->unpack_array16(ifs);
            break;

        case (unsigned char)(0xdd):
            ans = this->unpack_array32(ifs);
            break;

        case (unsigned char)(0xde):
            ans = this->unpack_map16(ifs);
            break;

        case (unsigned char)(0xdf):
            ans = this->unpack_map32(ifs);
            break;

        default:
            if (c <= (unsigned char)(0x7f)) {
                ans = this->unpack_positiveFixNum(c);
            } else if (((unsigned char)(0xe0) <= c) && (c <= (unsigned char)(0xff))) {
                ans = this->unpack_negativeFixNum(c);
            } else if (((unsigned char)(0xa0) <= c) && (c <= (unsigned char)(0xbf))) {
                ans = this->unpack_fixraw(c, ifs);
            } else if (((unsigned char)(0x90) <= c) && (c <= (unsigned char)(0x9f))) {
                ans = this->unpack_fixarray(c, ifs);
            } else if (((unsigned char)(0x80) <= c) && (c <= (unsigned char)(0x8f))) {
                ans = this->unpack_fixmap(c, ifs);
            } else {
                std::cerr << "msgpack unknown id=";
                std::cerr << std::hex << std::showbase << c;
                std::cerr << " @ ";
                std::cerr << this->debugCurrentPos_;
                std::cerr << std::endl;
                abort();
            }
            break;
        }
    }

    return ans;
}


int MsgPack::unpack_positiveFixNum(unsigned char c) {
    return (c & 127);
}


int MsgPack::unpack_negativeFixNum(unsigned char c) {
    return -(c & 31);
}


Variant MsgPack::unpack_bin8(std::istream& ifs) {
    const std::size_t size = this->unpack_uint8(ifs);

    Variant ans;
    if (size != 0) {
        char* pBuf = new char[size];
        ifs.read(pBuf, size);
        this->debugCurrentPos_ += size;

        const std::string str(pBuf, size);
        ans.set(str);

        delete[] pBuf;
        pBuf = NULL;
    }

    return ans;
}


Variant MsgPack::unpack_bin16(std::istream& ifs) {
    const std::size_t size = this->unpack_uint16(ifs);

    Variant ans;
    if (size != 0) {
        char* pBuf = new char[size];
        ifs.read(pBuf, size);
        this->debugCurrentPos_ += size;

        const std::string str(pBuf, size);
        ans.set(str);

        delete[] pBuf;
        pBuf = NULL;
    }

    return ans;
}


Variant MsgPack::unpack_bin32(std::istream& ifs) {
    const std::size_t size = this->unpack_uint32(ifs);

    Variant ans;
    if (size != 0) {
        char* pBuf = new char[size];
        ifs.read(pBuf, size);
        this->debugCurrentPos_ += size;

        const std::string str(pBuf, size);
        ans.set(str);

        delete[] pBuf;
        pBuf = NULL;
    }

    return ans;
}


Variant MsgPack::unpack_ext(std::istream& ifs,
                            const std::size_t size) {
    const int type = this->unpack_int8(ifs);
    Variant data;
    if (size != 0) {
        char* pBuf = new char[size];
        ifs.read(pBuf, size);
        this->debugCurrentPos_ += size;

        const std::string str(pBuf, size);
        data.set(str);

        delete[] pBuf;
        pBuf = NULL;
    }

    Variant ans;
    ans.push_back(type);
    ans.push_back(data);

    return ans;
}


Variant MsgPack::unpack_ext8(std::istream& ifs) {
    const std::size_t size = this->unpack_uint8(ifs);
    return this->unpack_ext(ifs, size);
}


Variant MsgPack::unpack_ext16(std::istream& ifs) {
    const std::size_t size = this->unpack_uint16(ifs);
    return this->unpack_ext(ifs, size);
}


Variant MsgPack::unpack_ext32(std::istream& ifs) {
    const std::size_t size = this->unpack_uint32(ifs);
    return this->unpack_ext(ifs, size);
}


Variant MsgPack::unpack_fixext1(std::istream& ifs) {
    return this->unpack_ext(ifs, 1);
}


Variant MsgPack::unpack_fixext2(std::istream& ifs) {
    return this->unpack_ext(ifs, 2);
}


Variant MsgPack::unpack_fixext4(std::istream& ifs) {
    return this->unpack_ext(ifs, 4);
}


Variant MsgPack::unpack_fixext8(std::istream& ifs) {
    return this->unpack_ext(ifs, 8);
}


Variant MsgPack::unpack_fixext16(std::istream& ifs) {
    return this->unpack_ext(ifs, 16);
}


MsgPack::UINT8 MsgPack::unpack_uint8(std::istream& ifs) {
    unsigned char value;
    ifs.read((char*)&value, 1);
    ++(this->debugCurrentPos_);

    return value;
}


MsgPack::UINT16 MsgPack::unpack_uint16(std::istream& ifs) {
    union {
        char buf[2];
        UINT16 value;
    } u;
    ifs.read(u.buf, 2);
    this->debugCurrentPos_ += 2;

    if (this->isLittleEndian() == true) {
        this->changeEndian(&(u.buf[0]), 2);
    }

    return u.value;
}


MsgPack::UINT32 MsgPack::unpack_uint32(std::istream& ifs) {
    union {
        char buf[4];
        UINT32 value;
    } u;
    ifs.read(u.buf, 4);
    this->debugCurrentPos_ += 4;

    if (this->isLittleEndian() == true) {
        this->changeEndian(&(u.buf[0]), 4);
    }

    return u.value;
}


MsgPack::UINT64 MsgPack::unpack_uint64(std::istream& ifs) {
    union {
        char buf[8];
        UINT64 value;
    } u;
    ifs.read(u.buf, 8);
    this->debugCurrentPos_ += 8;

    if (this->isLittleEndian() == true) {
        this->changeEndian(&(u.buf[0]), 8);
    }

    return u.value;
}


MsgPack::INT8 MsgPack::unpack_int8(std::istream& ifs) {
    char value;
    ifs.read((char*)&value, 1);
    ++(this->debugCurrentPos_);

    return value;
}


MsgPack::INT16 MsgPack::unpack_int16(std::istream& ifs) {
    union {
        char buf[2];
        INT16 value;
    } u;
    ifs.read(u.buf, 2);
    this->debugCurrentPos_ += 2;

    if (this->isLittleEndian() == true) {
        this->changeEndian(&(u.buf[0]), 2);
    }

    return u.value;
}


MsgPack::INT32 MsgPack::unpack_int32(std::istream& ifs) {
    union {
        char buf[4];
        INT32 value;
    } u;
    ifs.read(u.buf, 4);
    this->debugCurrentPos_ += 4;

    if (this->isLittleEndian() == true) {
        this->changeEndian(&(u.buf[0]), 4);
    }

    return u.value;
}


MsgPack::INT64 MsgPack::unpack_int64(std::istream& ifs) {
    union {
        char buf[8];
        INT64 value;
    } u;
    ifs.read(u.buf, 8);
    this->debugCurrentPos_ += 8;

    if (this->isLittleEndian() == true) {
        this->changeEndian(&(u.buf[0]), 8);
    }

    return u.value;
}


float MsgPack::unpack_float(std::istream& ifs) {
    assert(sizeof(float) == 4);
    float value;
    ifs.read((char*)&value, 4);
    this->debugCurrentPos_ += 4;

    if (this->isLittleEndian() == true) {
        value = this->changeEndian<float>(value);
    }

    return value;
}

double MsgPack::unpack_double(std::istream& ifs)
{
    assert(sizeof(double) == 8);
    double value;
    ifs.read((char*)&value, 8);
    this->debugCurrentPos_ += 8;

    if (this->isLittleEndian() == true) {
        value = this->changeEndian<double>(value);
    }

    return value;
}


Variant MsgPack::unpack_fixraw(const char in, std::istream& ifs) {
    const std::size_t size = (in & 31);

    Variant ans;
    if (size != 0) {
        char* pBuf = new char[size];
        ifs.read(pBuf, size);
        this->debugCurrentPos_ += size;

        const std::string str(pBuf, size);
        ans.set(str);

        delete[] pBuf;
        pBuf = NULL;
    }

    return ans;
}


Variant MsgPack::unpack_str8(std::istream& ifs) {
    // NOT support UTF-8!
    const std::size_t size = this->unpack_uint8(ifs);

    Variant ans;
    if (size != 0) {
        char* pBuf = new char[size];
        ifs.read(pBuf, size);
        this->debugCurrentPos_ += size;

        const std::string str(pBuf, size);
        ans.set(str);

        delete[] pBuf;
        pBuf = NULL;
    }

    return ans;
}


Variant MsgPack::unpack_str16(std::istream& ifs) {
    // NOT support UTF-8!
    const std::size_t size = this->unpack_uint16(ifs);

    Variant ans;
    if (size != 0) {
        char* pBuf = new char[size];
        ifs.read(pBuf, size);
        this->debugCurrentPos_ += size;

        const std::string str(pBuf, size);
        ans.set(str);

        delete[] pBuf;
        pBuf = NULL;
    }

    return ans;
}


Variant MsgPack::unpack_str32(std::istream& ifs) {
    // NOT support UTF-8!
    const std::size_t size = this->unpack_uint32(ifs);

    Variant ans;
    if (size != 0) {
        char* pBuf = new char[size];
        ifs.read(pBuf, size);
        this->debugCurrentPos_ += size;

        const std::string str(pBuf, size);

        ans.set(str);

        delete[] pBuf;
        pBuf = NULL;
    }

    return ans;
}


Variant MsgPack::unpack_fixarray(const char in, std::istream& ifs) {
    const std::size_t size = (in & 15);

    Variant ans;
    for (std::size_t i = 0; i < size; ++i) {
        const Variant tmp = this->loadBinary(ifs);
        ans.push_back(tmp);
    }

    return ans;
}


Variant MsgPack::unpack_array16(std::istream& ifs) {
    const std::size_t size = this->unpack_uint16(ifs);

    Variant ans;
    for (std::size_t i = 0; i < size; ++i) {
        const Variant tmp = this->loadBinary(ifs);
        ans.push_back(tmp);
    }

    return ans;
}


Variant MsgPack::unpack_array32(std::istream& ifs)
{
    const std::size_t size = this->unpack_uint32(ifs);

    Variant ans;
    for (std::size_t i = 0; i < size; ++i) {
        const Variant tmp = this->loadBinary(ifs);
        ans.push_back(tmp);
    }

    return ans;
}


Variant MsgPack::unpack_fixmap(const char in, std::istream& ifs) {
    const std::size_t size = (in & 15);

    Variant ans;
    for (std::size_t i = 0; i < size; ++i) {
        const Variant key = this->loadBinary(ifs);
        const Variant value = this->loadBinary(ifs);
        ans.add(key, value);
    }

    return ans;
}


Variant MsgPack::unpack_map16(std::istream& ifs) {
    const std::size_t size = this->unpack_uint16(ifs);

    Variant ans;
    for (std::size_t i = 0; i < size; ++i) {
        const Variant key = this->loadBinary(ifs);
        const Variant value = this->loadBinary(ifs);
        ans.add(key, value);
    }

    return ans;
}


Variant MsgPack::unpack_map32(std::istream& ifs) {
    const std::size_t size = this->unpack_uint32(ifs);

    Variant ans;
    for (std::size_t i = 0; i < size; ++i) {
        const Variant key = this->loadBinary(ifs);
        const Variant value = this->loadBinary(ifs);
        ans.add(key, value);
    }

    return ans;
}


void MsgPack::save(const std::string& path) const {
    std::ofstream ofs;
    ofs.open(path.c_str(), std::ios::out | std::ios::trunc | std::ios::binary);
    ofs << this->dump();
    ofs.close();
}


std::string MsgPack::dump() const {
    return this->dump(this->data_);
}


std::string MsgPack::dump(const Variant& data) const {
    std::ostringstream os;

    switch (data.type()) {
    case Variant::ARRAY:
        os << this->dump_array(data);
        break;

    case Variant::MAP:
        os << this->dump_map(data);
        break;

    default:
        os << this->dump_scalar(data);
        break;
    }

    return os.str();
}


std::string MsgPack::dump_scalar(const Variant& data) const {
    std::ostringstream os;

    switch (data.type()) {
    case Variant::BOOLEAN:
        os << this->pack(data.get_bool());
        break;

    case Variant::STRING:
        {
            const std::string str = data.get_str();
            os << this->pack(str.c_str(), str.size());
        }
        break;

    case Variant::INT:
        {
            int value = data.get_int();
#if COMPILE_VALUE_SIZEOF_INT == 4
            os << this->pack_int32(value);
#else
            os << this->pack_int64(value);
#endif
        }
        break;

    case Variant::LONG:
        {
            long value = data.get_long();
#if COMPILE_VALUE_SIZEOF_LONG == 4
            os << this->pack_int32(value);
#else
            os << this->pack_int64(value);
#endif
        }
        break;

    case Variant::UINT:
        {
            unsigned int value = data.get_uint();
#if COMPILE_VALUE_SIZEOF_INT == 4
            os << this->pack_uint32(value);
#else
            os << this->pack_uint64(value);
#endif
        }
        break;

    case Variant::ULONG:
        {
            unsigned long value = data.get_ulong();
#if COMPILE_VALUE_SIZEOF_LONG == 4
            os << this->pack_uint32(value);
#else
            os << this->pack_uint64(value);
#endif
        }
        break;

    case Variant::DOUBLE:
        {
            double value = data.get_double();
            os << this->pack(value);
        }
        break;

    case Variant::NONE:
        {
            std::ostringstream os_tmp;
            this->write(os_tmp, char(0xc0));
            os << os_tmp.str();
        }
        break;

    default:
        // something wrong.
        abort();
        break;
    }

    return os.str();
}


std::string MsgPack::dump_array(const Variant& data) const {
    assert(data.type() == Variant::ARRAY);
    std::ostringstream os;

    assert(sizeof(int) == 4);
    const int size = data.size();
    this->write(os, char(0xdd));
    this->write(os, this->toBigEndian(size));

    for (Variant::ArrayConstIterator p = data.beginArray(); p != data.endArray(); ++p) {
        os << this->dump(*p);
    }

    return os.str();
}


std::string MsgPack::dump_map(const Variant& data) const {
    assert(data.type() == Variant::MAP);
    std::ostringstream os;

    assert(sizeof(int) == 4);
    const int size = data.size();
    this->write(os, char(0xdf));
    this->write(os, this->toBigEndian(size));

    for (Variant::MapConstIterator p = data.beginMap(); p != data.endMap(); ++p) {
        os << this->dump(p->first);
        os << this->dump(p->second);
    }

    return os.str();
}


std::string MsgPack::pack(const bool value) const {
    std::ostringstream os;
    if (value == true) {
        this->write(os, char(0xc3));
    } else {
        this->write(os, char(0xc2));
    }

    return os.str();
}


std::string MsgPack::pack(const UINT8 value) const {
    std::ostringstream os;

    this->write(os, char(0xcc));
    this->write(os, this->toBigEndian(value));

    return os.str();
}


std::string MsgPack::pack(const UINT16 value) const {
    std::ostringstream os;

    this->write(os, char(0xcd));
    this->write(os, this->toBigEndian(value));

    return os.str();
}


std::string MsgPack::pack_uint32(const UINT32 value) const {
    std::ostringstream os;

    this->write(os, char(0xce));
    this->write(os, this->toBigEndian(value));

    return os.str();
}


std::string MsgPack::pack_uint64(const UINT64 value) const {
    std::ostringstream os;

    this->write(os, char(0xcf));
    this->write(os, this->toBigEndian(value));

    return os.str();
}


std::string MsgPack::pack(const INT8 value) const {
    std::ostringstream os;

    this->write(os, char(0xd0));
    this->write(os, this->toBigEndian(value));

    return os.str();
}


std::string MsgPack::pack(const INT16 value) const {
    std::ostringstream os;

    this->write(os, char(0xd1));
    this->write(os, this->toBigEndian(value));

    return os.str();
}


std::string MsgPack::pack_int32(const INT32 value) const {
    std::ostringstream os;

    this->write(os, char(0xd2));
    this->write(os, this->toBigEndian(value));

    return os.str();
}


std::string MsgPack::pack_int64(const INT64 value) const {
    std::ostringstream os;

    this->write(os, char(0xd3));
    this->write(os, this->toBigEndian(value));

    return os.str();
}


std::string MsgPack::pack(const double value) const {
    assert(sizeof(double) == 8);
    std::ostringstream os;

    this->write(os, char(0xcb));
    this->write(os, this->toBigEndian(value));

    return os.str();
}


std::string MsgPack::pack(const char* pBuf, const int size) const {
    assert(pBuf != NULL);
    std::ostringstream os;

    assert(sizeof(int) == 4);
    this->write(os, char(0xc6)); // bin32
    this->write(os, this->toBigEndian(size));
    os.write(pBuf, size);

    return os.str();
}


#endif // MSGPACK_ALT_H
